/* Copyright (C) 2008 Ricardo Catalinas Jiménez <jimenezrick@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

#ifdef INOTIFY

#include "fm.h"

gboolean inotify_enabled = FALSE, debug_inotify = FALSE;
GHashTable *watches_table;
int inotify_descriptor;
char events_buffer[BUFFER_LENGTH];

void init_inotify(GNode *tree_root)
{
	if ((inotify_descriptor = inotify_init()) == -1) {
		PRINT_ERRNO_INFO();
		return;
	}
	watches_table = g_hash_table_new(g_int_hash, g_int_equal);
	inotify_enabled = TRUE;
}

void stop_inotify(void)
{
	CHECK_INOTIFY_ENABLED();
	g_hash_table_destroy(watches_table);
	close(inotify_descriptor);
	inotify_enabled = FALSE;
}

void add_watch_directory(GNode *directory)
{
	char *path;
	int watch;

	CHECK_INOTIFY_ENABLED();
	path = get_path(directory);
	if ((watch = inotify_add_watch(inotify_descriptor, path, IN_CREATE | IN_MOVED_FROM |
					IN_MOVED_TO | IN_DELETE)) == -1)
		PRINT_ERRNO_INFO();
	else {
		FILE(directory)->watch = watch;
		g_hash_table_insert(watches_table, &(FILE(directory)->watch), directory);
	}
	free(path);
}

void remove_watch_directory(GNode *directory, gboolean remove_watch)
{
	CHECK_INOTIFY_ENABLED();
	if (FILE(directory)->watch == -1)
		PRINT_ERROR_INFO("Directory doesn't have a watch associated");
	else if (remove_watch && inotify_rm_watch(inotify_descriptor, FILE(directory)->watch) == -1)
		PRINT_ERRNO_INFO();
	else if (g_hash_table_remove(watches_table, &(FILE(directory)->watch)) == FALSE)
		PRINT_ERROR_INFO("Element removal failed in watches_table");
}

void watch_events(void)
{
	GNode *modified_directory, *removed_file;
	File *new_file;
	gboolean refresh_request = FALSE;
	struct timeval time;
	struct inotify_event *event;
	int n, i;
	fd_set fds;

	CHECK_INOTIFY_ENABLED();
again:	time.tv_sec = 0;
	time.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(inotify_descriptor, &fds);
	while ((n = select(inotify_descriptor + 1, &fds, NULL, NULL, &time)) == 1) {
again2:		if ((n = read(inotify_descriptor, events_buffer, BUFFER_LENGTH)) > 0) {
			for (i = 0; i < n; i += sizeof(struct inotify_event) + event->len) {
				event = (struct inotify_event *) (events_buffer + i);
				if (debug_inotify == TRUE)
					dump_event(event);
				modified_directory = (GNode *) g_hash_table_lookup(
						watches_table, &(event->wd));

				if (event->mask & IN_CREATE || event->mask & IN_MOVED_TO) {
					if ((new_file = create_new_file(event->name,
									get_path(modified_directory), FALSE)) != NULL)
						refresh_request |= insert_in_tree(modified_directory, new_file);
				} else if (event->mask & IN_DELETE || event->mask & IN_MOVED_FROM) {
					if ((removed_file = search_node_by_name(modified_directory,
									event->name)) != NULL)
						refresh_request |= remove_from_tree(removed_file, FALSE);
				} else if (event->mask & IN_UNMOUNT) {
					refresh_request |= remove_from_tree(modified_directory, TRUE);
				}
			}
		} else if (n == -1) {
			if (errno == EINTR)
				goto again2;
			else
				PRINT_ERRNO_AND_EXIT();
		}
	}
	if (n == -1) {
		if (errno == EINTR)
			goto again;
		else
			PRINT_ERRNO_AND_EXIT();
	}
	if (refresh_request == TRUE)
		refresh_screen();
}

gboolean insert_in_tree(GNode *directory, File *file)
{
	GNode *new_file, *file_ptr;
	int position;

	if (file->name[0] == '.' && FILE(directory)->show_dotfiles == FALSE) {
		insert_sorted_in_dotfiles(directory, file);

		return FALSE;
	}
	new_file = insert_sorted_in_tree(directory, file);
	file_ptr = get_previous_file(new_file);
	position = g_list_position(lines, FILE(file_ptr)->line);
	lines = g_list_insert(lines, new_file, position + 1);
	file->line = g_list_nth(lines, position + 1);

	if (g_list_length(first_line) - g_list_length(last_line) + 1) {
		if (g_list_previous(first_line) == file->line)
			first_line = file->line;
		else if (g_list_next(last_line) == file->line)
			last_line = file->line;
	}
	if (g_list_position(lines, first_line) <= position + 1 &&
			position + 1 <= g_list_position(lines, last_line)) {
		if (position + 1 < g_list_position(lines, selected_line)) {
			if (g_list_length(first_line) <= getmaxy(tree_window))
				print_lines(first_line, last_line, FALSE);
			else {
				first_line = g_list_next(first_line);
				print_lines(first_line, file->line, FALSE);
			}
		} else {
			if (g_list_length(first_line) > getmaxy(tree_window))
				last_line = g_list_previous(last_line);

			if (g_node_last_sibling(new_file) == new_file)
				print_lines(g_list_previous(file->line), last_line, FALSE);
			else
				print_lines(file->line, last_line, FALSE);
		}

		return TRUE;
	}

	return FALSE;
}

gboolean remove_from_tree(GNode *file, gboolean unmount)
{
	int position = g_list_position(lines, FILE(file)->line);
	gboolean refresh_needed = FALSE;
	GList *line_ptr, *line_ptr2;

	if (G_NODE_IS_ROOT(file)) {
		endwin();
		clean_up();
		printf("The tree root was removed\n");
		exit(EXIT_SUCCESS);
	}
	if (g_node_is_ancestor(file, NODE(selected_line)))
		select_file(file);
	if (FILE(file)->type == directory_type) {
		close_directory(file);
		destroy_directory_content_real(file, FALSE);
		if (unmount)
			return TRUE;
	}
	g_node_unlink(file);

	if (g_list_position(lines, first_line) <= position &&
			position <= g_list_position(lines, last_line)) {
		if (first_line == FILE(file)->line && selected_line == FILE(file)->line) {
			selected_line = first_line = g_list_previous(first_line);
			lines = g_list_delete_link(lines, FILE(file)->line);
			print_lines(first_line, first_line, FALSE);
		} else if (position < g_list_position(lines, selected_line)) {
			if (first_line == FILE(file)->line)
				first_line = g_list_next(first_line);
			line_ptr = g_list_previous(FILE(file)->line);
			lines = g_list_delete_link(lines, FILE(file)->line);
			if ((line_ptr2 = g_list_previous(first_line)) != NULL) {
				first_line = line_ptr2;
				print_lines(first_line, line_ptr, FALSE);
			} else if ((line_ptr2 = g_list_next(last_line)) != NULL) {
				last_line = line_ptr2;
				print_lines(line_ptr, last_line, FALSE);
			} else
				print_lines(line_ptr, last_line, TRUE);
		} else {
			if (FILE(file)->line == selected_line)
				selected_line = g_list_previous(selected_line);
			if (last_line == FILE(file)->line)
				last_line = g_list_previous(last_line);
			line_ptr = g_list_previous(FILE(file)->line);
			lines = g_list_delete_link(lines, FILE(file)->line);
			if ((line_ptr2 = g_list_next(last_line)) != NULL) {
				last_line = line_ptr2;
				print_lines(line_ptr, last_line, FALSE);
			} else
				print_lines(line_ptr, last_line, TRUE);
		}
		refresh_needed = TRUE;
	} else {
		if (last_line == g_list_previous(FILE(file)->line)) {
			lines = g_list_delete_link(lines, FILE(file)->line);
			print_lines(last_line, last_line, FALSE);
			refresh_needed = TRUE;
		} else
			lines = g_list_delete_link(lines, FILE(file)->line);
	}
	free_node_data(file, NULL);
	g_node_destroy(file);

	return refresh_needed;
}

GNode *search_node_by_name(GNode *directory, char *name)
{
	GNode *file_ptr;

	for (file_ptr = g_node_first_child(directory); file_ptr != NULL;
			file_ptr = g_node_next_sibling(file_ptr)) {
		if (!strcmp(FILE(file_ptr)->name, name))
			return file_ptr;
	}

	return NULL;
}

void dump_event(struct inotify_event *event)
{
	static int first = 0;

	if (first != 0)
		fprintf(stderr, "================================\n");
	if (event->len > 0) {
		fprintf(stderr, "wd = %i\ncookie = %i\nlen = %i\nname = %s\nmask = ",
				event->wd, event->cookie, event->len, event->name);
	} else {
		fprintf(stderr, "wd = %i\ncookie = %i\nlen = %i\nname =\nmask = ",
				event->wd, event->cookie, event->len);
	}
	dump_mask(event->mask);
	first = 1;
}

void dump_mask(int mask)
{
	if (mask & IN_ACCESS)
		fprintf(stderr, " IN_ACCESS ");
	if (mask & IN_ATTRIB)
		fprintf(stderr, " IN_ATTRIB ");
	if (mask & IN_CLOSE_WRITE)
		fprintf(stderr, " IN_CLOSE_WRITE ");
	if (mask & IN_CLOSE_NOWRITE)
		fprintf(stderr, " IN_CLOSE_NOWRITE ");
	if (mask & IN_CREATE)
		fprintf(stderr, " IN_CREATE ");
	if (mask & IN_DELETE)
		fprintf(stderr, " IN_DELETE ");
	if (mask & IN_DELETE_SELF)
		fprintf(stderr, " IN_DELETE_SELF ");
	if (mask & IN_MODIFY)
		fprintf(stderr, " IN_MODIFY ");
	if (mask & IN_MOVE_SELF)
		fprintf(stderr, " IN_MOVE_SELF ");
	if (mask & IN_MOVED_FROM)
		fprintf(stderr, " IN_MOVED_FROM ");
	if (mask & IN_MOVED_TO)
		fprintf(stderr, " IN_MOVED_TO ");
	if (mask & IN_OPEN)
		fprintf(stderr, " IN_OPEN ");
	if (mask & IN_IGNORED)
		fprintf(stderr, " IN_IGNORED ");
	if (mask & IN_ISDIR)
		fprintf(stderr, " IN_ISDIR ");
	if (mask & IN_Q_OVERFLOW)
		fprintf(stderr, " IN_Q_OVERFLOW ");
	if (mask & IN_UNMOUNT)
		fprintf(stderr, " IN_UNMOUNT ");
	fprintf(stderr, "\n");
}

#endif
