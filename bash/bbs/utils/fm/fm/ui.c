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

#include "fm.h"

WINDOW *tree_window, *info_window;
GList *lines, *selected_line, *first_line, *last_line;

void init_lines(void)
{
	lines = g_list_append(lines, tree_root);
	FILE(tree_root)->line = lines;
	selected_line = lines;
	first_line = lines;
	update_last_line();
}

void init_curses(void)
{
	int screen_size[2];

	initscr();
#ifdef INOTIFY
	halfdelay(GETCH_DELAY);
#else
	cbreak();
#endif
	nonl();
	noecho();
	keypad(stdscr, TRUE);
	curs_set(0);
	refresh();

	getmaxyx(stdscr, screen_size[0], screen_size[1]);
	tree_window = newwin(screen_size[0] - 1, screen_size[1], 0, 0);
	info_window = newwin(1, screen_size[1], screen_size[0] - 1, 0);
	scrollok(tree_window, TRUE);
	idlok(tree_window, TRUE);
}

void init_ui(char *name)
{
	init_curses();
	init_tree(name);
	init_lines();
	print_line(FILE(tree_root)->line);
#ifdef INOTIFY
	init_inotify(tree_root);
#endif
	open_directory(tree_root);
	refresh_screen();
}

void refresh_screen(void)
{
	wnoutrefresh(tree_window);
	wnoutrefresh(info_window);
	doupdate();
}

void handle_sigwinch(int signal_number)
{
	ungetch(KEY_RESIZE);
}

void print_lines(GList *start_line, GList *end_line, gboolean clear_bottom_lines)
{
	GList *line_ptr;
	int line_number;

	if (end_line == NULL)
		end_line = last_line;
	else if (start_line != NULL) {
		for (line_ptr = start_line;
				g_list_position(first_line, line_ptr) < getmaxy(tree_window);
				line_ptr = g_list_next(line_ptr)) {
			print_line(line_ptr);
			if (line_ptr == end_line)
				break;
		}
	}
	line_number = g_list_position(first_line, last_line) + 1;
	if (clear_bottom_lines && line_number < getmaxy(tree_window)) {
		wmove(tree_window, line_number, 0);
		wclrtobot(tree_window);
	}
}

void print_parents_lines(GList *line)
{
	GNode *file_ptr;
	int depth;

	for (file_ptr = NODE(line)->parent, depth = g_node_depth(file_ptr) - 1;
			!G_NODE_IS_ROOT(file_ptr); file_ptr = file_ptr->parent, depth--) {
		if (file_ptr != g_node_last_sibling(file_ptr))
			mvwaddch(tree_window, g_list_position(first_line, line),
					2 * depth - 2, ACS_VLINE);
		else
			mvwaddch(tree_window, g_list_position(first_line, line),
					2 * depth - 2, ' ');
		mvwaddch(tree_window, g_list_position(first_line, line), 2 * depth - 1, ' ');
	}
}

void print_line(GList *line)
{
	int line_number = g_list_position(first_line, line);
	GNode *file = NODE(line);
	int depth = g_node_depth(file) - 1;
	char *link_str;

	wmove(tree_window, line_number, 0);
	wclrtoeol(tree_window);

	if (line == selected_line)
		wattron(tree_window, A_REVERSE);

	if (G_NODE_IS_ROOT(file)) {
		wattron(tree_window, A_BOLD);
		waddnstr(tree_window, FILE(file)->name, getmaxx(tree_window) - 1);
		if (!g_str_has_suffix(FILE(file)->name, "/"))
			waddch(tree_window, '/');
		wattroff(tree_window, A_BOLD);
	} else {
		if (file != g_node_last_sibling(file))
			mvwaddch(tree_window, line_number, 2 * depth - 2, ACS_LTEE);
		else
			mvwaddch(tree_window, line_number, 2 * depth - 2, ACS_LLCORNER);

		waddch(tree_window, ACS_HLINE);
		if (FILE(file)->link == TRUE) {
			link_str = g_strdup_printf("%s -> %s", FILE(file)->name,
					FILE(file)->link_path);
			waddnstr(tree_window, link_str, getmaxx(tree_window) - 2 * depth - 1);
			free(link_str);
		} else
			waddnstr(tree_window, FILE(file)->name,
					getmaxx(tree_window) - 2 * depth - 1);

		if (FILE(file)->type == directory_type && ((FILE(file)->link == TRUE &&
						!g_str_has_suffix(FILE(file)->link_path, "/")) ||
					(FILE(file)->link == FALSE &&
					 !g_str_has_suffix(FILE(file)->name, "/"))))
			waddch(tree_window, '/');
		print_parents_lines(line);
	}
	wattroff(tree_window, A_REVERSE);
}

void clear_info(void)
{
	werase(info_window);
}

void print_info(char *message, gboolean bold)
{
	static char *last_message = NULL;
	static gboolean last_bold = FALSE;

	if (message != NULL) {
		if (last_message != NULL)
			free(last_message);
		last_message = strdup(message);
	}
	if (bold == TRUE || bold == FALSE)
		last_bold = bold;
	if (last_bold == TRUE)
		wattron(info_window, A_BOLD);
	werase(info_window);
	if (last_message != NULL)
		waddnstr(info_window, last_message, getmaxx(info_window));
	wattroff(info_window, A_BOLD);
}

void print_error_info(int last_errno, char *file, int line, char *message)
{
	char *message_format = "Error (%s:%i): %s";
	char *message_str;

	if (message != NULL)
		message_str = g_strdup_printf(message_format, file, line, message);
	else
		message_str = g_strdup_printf(message_format, file, line, strerror(last_errno));
	print_info(message_str, TRUE);
	free(message_str);
}

void update_last_line(void)
{
	if (g_list_length(first_line) > getmaxy(tree_window))
		last_line = g_list_nth(first_line, getmaxy(tree_window) - 1);
	else
		last_line = g_list_last(lines);
}

void add_directory_content(GNode *directory)
{
	GNode *file_ptr, *directory_sibling;
	int position;

	directory_sibling = get_next_file_not_deepper(directory);

	for (position = g_list_position(lines, FILE(directory)->line) + 1,
			file_ptr = g_node_first_child(directory);
			file_ptr != NULL && file_ptr != directory_sibling;
			position++, file_ptr = get_next_file(file_ptr)) {
		lines = g_list_insert(lines, file_ptr, position);
		FILE(file_ptr)->line = g_list_nth(lines, position);
	}
}

void open_directory(GNode *directory)
{
	if (FILE(directory)->read == FALSE && read_directory(directory) == FALSE)
		return;

	FILE(directory)->open = TRUE;
	add_directory_content(directory);
	update_last_line();
	print_lines(g_list_next(FILE(directory)->line), last_line, TRUE);
}

void close_directory(GNode *directory)
{
	GNode *directory_sibling;
	GList *line_ptr;

	directory_sibling = get_next_file_not_deepper(directory);

	for (line_ptr = g_list_next(FILE(directory)->line); line_ptr != NULL &&
			NODE(line_ptr) != directory_sibling; line_ptr = g_list_next(FILE(directory)->line))
		lines = g_list_delete_link(lines, line_ptr);

	FILE(directory)->open = FALSE;
	update_last_line();
	print_lines(g_list_next(FILE(directory)->line), last_line, TRUE);
}

void update_directory(GNode *directory)
{
	close_directory(directory);
	destroy_directory_content(directory);
	open_directory(directory);

	if (FILE(directory)->show_dotfiles == TRUE)
		show_dotfiles(directory);
}

void show_dotfiles(GNode *directory)
{
	add_dotfiles(directory);
	update_last_line();
	print_lines(g_list_next(FILE(directory)->line), last_line, TRUE);
}

void hide_dotfiles(GNode *directory)
{
	remove_dotfiles(directory);
	update_last_line();
	print_lines(g_list_next(FILE(directory)->line), last_line, TRUE);
}

void select_line(GList *line)
{
	select_nth_line(g_list_position(lines, line) - g_list_position(lines, selected_line));
}

void select_file(GNode *file)
{
	select_line(FILE(file)->line);
}

void select_nth_line(int n_lines)
{
	GList *old_selected_line;
	int min_n_lines, max_n_lines;

	if ((max_n_lines = g_list_length(selected_line) - 1) < n_lines)
		n_lines = max_n_lines;
	else if((min_n_lines = (int) g_list_length(selected_line) -
				(int) g_list_length(lines)) > n_lines)
		n_lines = min_n_lines;

	old_selected_line = selected_line;
	if (n_lines < 0)
		selected_line = g_list_nth(lines, g_list_position(lines, selected_line) + n_lines);
	else if (n_lines > 0)
		selected_line = g_list_nth(selected_line, n_lines);

	if (g_list_position(lines, selected_line) < g_list_position(lines, first_line))
		scroll_tree(g_list_position(lines, selected_line) -
				g_list_position(lines, first_line));
	else if (g_list_position(lines, selected_line) > g_list_position(lines, last_line))
		scroll_tree(g_list_position(lines, selected_line) -
				g_list_position(lines, last_line));

	if (n_lines != 0) {
		print_line(selected_line);
		if (g_list_position(lines, first_line) <= g_list_position(lines, old_selected_line) &&
				g_list_position(lines, old_selected_line) <=
				g_list_position(lines, last_line))
			print_line(old_selected_line);
	}
}

void scroll_tree(int n_lines)
{
	int min_n_lines, max_n_lines;

	if ((min_n_lines = -g_list_position(lines, first_line)) > n_lines)
		n_lines = min_n_lines;
	else if ((max_n_lines = g_list_length(last_line) - 1) < n_lines)
		n_lines = max_n_lines;

	wscrl(tree_window, n_lines);
	if (n_lines < 0) {
		first_line = g_list_nth(lines, g_list_position(lines, first_line) + n_lines);
		update_last_line();
		print_lines(first_line, g_list_nth(first_line, -n_lines), FALSE);
	} else if (n_lines > 0) {
		first_line = g_list_nth(first_line, n_lines);
		update_last_line();
		print_lines(g_list_nth(lines, g_list_position(lines, last_line) - n_lines),
				last_line, TRUE);
	}
}

void select_line_inside_window(void)
{
	if (g_list_position(lines, selected_line) < g_list_position(lines, first_line))
		select_line(first_line);
	else if (g_list_position(lines, selected_line) > g_list_position(lines, last_line))
		select_line(last_line);
}

void select_next_line_by_first_letter(char letter)
{
	GList *line_ptr;

	for (line_ptr = g_list_next(selected_line); line_ptr != NULL;
			line_ptr = g_list_next(line_ptr)) {
		if (FILE(NODE(line_ptr))->name[0] == letter) {
			select_line(line_ptr);
			return;
		}
	}
}

void select_previous_line_by_first_letter(char letter)
{
	GList *line_ptr;

	for (line_ptr = g_list_previous(selected_line); line_ptr != NULL;
			line_ptr = g_list_previous(line_ptr)) {
		if (FILE(NODE(line_ptr))->name[0] == letter) {
			select_line(line_ptr);
			return;
		}
	}
}
