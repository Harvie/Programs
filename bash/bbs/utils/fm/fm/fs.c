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

GNode *tree_root;

void init_tree(char *name)
{
	char *path;

	if (name != NULL)
		tree_root = g_node_new(create_new_file(name, NULL, TRUE));
	else {
		path = g_get_current_dir();
		tree_root = g_node_new(create_new_file(path, NULL, TRUE));
		free(path);
	}
}

File *create_new_file(char *name, char *path, gboolean exit_on_error)
{
	struct stat info, link_info;
	File *file = malloc(sizeof(File));
	char *file_path;

	if (path != NULL)
		file_path = g_strconcat(path, "/", name, NULL);
	else
		file_path = name;

	if (lstat(file_path, &link_info) == -1) {
		if (exit_on_error)
			PRINT_ERRNO_AND_EXIT();
		else {
			/* Sometimes a file is created and then removed, this under Linux
			 * inotify events can create confusion because we try to stat a
			 * file that no longer exists. The file is removed before we can do
			 * the stat. So we ignore the errno and return NULL to take account
			 * of it. */

			/* PRINT_ERRNO_INFO(); */
			free(file_path);
			free(file);

			return NULL;
		}
	} else if (S_ISLNK(link_info.st_mode)) {
		if (link_info.st_size == 0)
			link_info.st_size = PATH_MAX;

		file->link_path = malloc(link_info.st_size + 1);
		file->link_path[link_info.st_size] = '\0';
		file->link_path[0] = '\0';
		file->link = TRUE;

		if (readlink(file_path, file->link_path, link_info.st_size) == -1)
			PRINT_ERRNO_INFO();
	} else {
		file->link_path = NULL;
		file->link = FALSE;
	}
	if (stat(file_path, &info) == -1) {
		if (exit_on_error)
			PRINT_ERRNO_AND_EXIT();
		else {
			PRINT_ERRNO_INFO();
			file->type = file_type;
		}
	} else if (S_ISDIR(info.st_mode))
		file->type = directory_type;
	else
		file->type = file_type;

	if (file->link == TRUE)
		file->info = link_info;
	else
		file->info = info;

	file->name = strdup(name);
	file->read = FALSE;
	file->open = FALSE;
	file->show_dotfiles = FALSE;
	file->line = NULL;
	file->dotfiles = NULL;
	file->watch = -1;

	if (path != NULL)
		free(file_path);

	return file;
}

gboolean read_directory(GNode *directory)
{
	DIR *dir;
	struct dirent *entry;
	File *new_file;
	char *path = get_path(directory);
	int i;

	if ((dir = opendir(path)) == NULL) {
		PRINT_ERRNO_INFO();
		return FALSE;
	}
	for (errno = 0, i = 0; (entry = readdir(dir)) != NULL; ) {
		if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
			continue;

		if ((new_file = create_new_file(entry->d_name, path, FALSE)) == NULL)
			continue;

		if (new_file->name[0] == '.') {
			insert_sorted_in_dotfiles(directory, new_file);
			i = 1;
		} else {
			insert_sorted_in_tree(directory, new_file);
			i = 1;
		}
	}
	if (errno == EBADF)
		PRINT_ERRNO_INFO();
	if (closedir(dir) == -1)
		PRINT_ERRNO_INFO();
	free(path);
	if (i > 0) {
		FILE(directory)->read = TRUE;
#ifdef INOTIFY
		add_watch_directory(directory);
#endif
		return TRUE;
	}

	return FALSE;
}

void free_element_data(gpointer element_data, gpointer data)
{
	if (FILE((GNode *) element_data)->type == directory_type)
		destroy_directory_content((GNode *) element_data);
	free_node_data((GNode *) element_data, NULL);
}

void destroy_dotfiles(GList *dotfiles)
{
	g_list_foreach(dotfiles, free_element_data, NULL);
	g_list_free(dotfiles);
}

gboolean free_node_data(GNode *node, gpointer data)
{
	free(FILE(node)->name);
	free(FILE(node)->link_path);
	destroy_dotfiles(FILE(node)->dotfiles);
	free(node->data);

	return FALSE;
}

void destroy_directory_content(GNode *directory)
{
	destroy_directory_content_real(directory, TRUE);
}

void destroy_directory_content_real(GNode *directory, gboolean remove_watch)
{
	GNode *file_ptr, *file_ptr2;

#ifdef INOTIFY
	if (FILE(directory)->read == TRUE)
		remove_watch_directory(directory, remove_watch);
#endif
	for (file_ptr = g_node_first_child(directory); file_ptr != NULL; ) {
		file_ptr2 = file_ptr;
		file_ptr = g_node_next_sibling(file_ptr);

		g_node_unlink(file_ptr2);
		g_node_traverse(file_ptr2, G_PRE_ORDER, G_TRAVERSE_ALL, -1, free_node_data, NULL);
		g_node_destroy(file_ptr2);
	}
	destroy_dotfiles(FILE(directory)->dotfiles);
	FILE(directory)->dotfiles = NULL;
	FILE(directory)->read = FALSE;
}

void add_dotfiles(GNode *directory)
{
	GList *line_ptr;
	GNode *file_ptr;
	int position;

	for (line_ptr = g_list_next(FILE(directory)->line), position = 0;
			FILE(directory)->dotfiles != NULL; position++) {
		file_ptr = NODE(FILE(directory)->dotfiles);
		FILE(directory)->dotfiles = g_list_delete_link(FILE(directory)->dotfiles,
				FILE(directory)->dotfiles);
		g_node_insert(directory, position, file_ptr);
		lines = g_list_insert_before(lines, line_ptr, file_ptr);
		FILE(file_ptr)->line = g_list_previous(line_ptr);
		if (FILE(file_ptr)->type == directory_type && FILE(file_ptr)->open == TRUE)
			add_directory_content(file_ptr);
	}
	FILE(directory)->show_dotfiles = TRUE;
}

void remove_dotfiles(GNode *directory)
{
	GNode *file_ptr;
	GList *line_ptr, *line_ptr_aux;

	for (file_ptr = g_node_first_child(directory); file_ptr != NULL &&
			FILE(file_ptr)->name[0] == '.'; ) {
		lines = g_list_delete_link(lines, FILE(file_ptr)->line);
		FILE(directory)->dotfiles = g_list_insert(FILE(directory)->dotfiles, file_ptr, -1);
		FILE(file_ptr)->line = NULL;
		file_ptr = g_node_next_sibling(file_ptr);
		g_node_unlink(NODE(g_list_last(FILE(directory)->dotfiles)));

		for (line_ptr = g_list_previous(FILE(file_ptr)->line);
				line_ptr != FILE(directory)->line; ) {
			line_ptr_aux = line_ptr;
			line_ptr = g_list_previous(line_ptr);
			FILE(NODE(line_ptr_aux))->line = NULL;
			lines = g_list_delete_link(lines, line_ptr_aux);
		}
	}
	FILE(directory)->show_dotfiles = FALSE;
}

int file_cmp(File *file1, File *file2)
{
	if (file1->name[0] == '.' && file2->name[0] != '.')
		return -1;
	else if (file1->name[0] != '.' && file2->name[0] == '.')
		return 1;
	else if (file1->type == file2->type)
		return strcmp(file1->name, file2->name);
	else if (file1->type == directory_type)
		return -1;
	else
		return 1;
}

int file_cmp_list(gconstpointer file1, gconstpointer file2)
{
	return file_cmp(FILE((GNode *) file1), FILE((GNode *) file2));
}

void insert_sorted_in_dotfiles(GNode *directory, File *file)
{
	FILE(directory)->dotfiles = g_list_insert_sorted(FILE(directory)->dotfiles,
			g_node_new(file), file_cmp_list);
}

GNode *insert_sorted_in_tree(GNode *directory, File *file)
{
	GNode *file_ptr, *new_file;

	if (G_NODE_IS_LEAF(directory)) {
		g_node_append_data(directory, file);
		return g_node_first_child(directory);
	} else if (file_cmp(file, FILE(g_node_first_child(directory))) < 0) {
		g_node_prepend_data(directory, file);
		return g_node_first_child(directory);
	} else {
		for (file_ptr = g_node_last_child(directory); file_ptr != NULL;
				file_ptr = g_node_prev_sibling(file_ptr)) {
			if (file_cmp(file, FILE(file_ptr)) > 0) {
				new_file = g_node_new(file);
				g_node_insert_after(directory, file_ptr, new_file);
				break;
			}
		}

		return new_file;
	}
}

char *get_path(GNode *file)
{
	GNode *file_ptr;
	GString *path = g_string_new(FILE(file)->name);

	for (file_ptr = file->parent; file_ptr != NULL; file_ptr = file_ptr->parent) {
		g_string_prepend(path, "/");
		g_string_prepend(path, FILE(file_ptr)->name);
	}

	return g_string_free(path, FALSE);
}

GNode *get_next_file_real(GNode *file, gboolean go_deeper)
{
	GNode *file_ptr, *directory_ptr;

	if (go_deeper && (file_ptr = g_node_first_child(file)) != NULL && FILE(file)->open == TRUE)
		return file_ptr;
	else if ((file_ptr = g_node_next_sibling(file)) != NULL)
		return file_ptr;
	else {
		for (directory_ptr = file->parent; directory_ptr != NULL;
				directory_ptr = directory_ptr->parent) {
			if ((file_ptr = g_node_next_sibling(directory_ptr)) != NULL)
				return file_ptr;
		}

		return NULL;
	}
}

GNode *get_previous_file(GNode *file)
{
	GNode *file_ptr;

	if (G_NODE_IS_ROOT(file))
		return NULL;
	else if (file == g_node_first_sibling(file))
		return file->parent;
	else if (!G_NODE_IS_LEAF(file_ptr = g_node_prev_sibling(file)) &&
			FILE(file_ptr)->open == TRUE) {
		do
			file_ptr = g_node_last_child(file_ptr);
		while (!G_NODE_IS_LEAF(file_ptr) && FILE(file_ptr)->open == TRUE);
		return file_ptr;
	} else
		return file_ptr;
}

GNode *get_next_file(GNode *file)
{
	return get_next_file_real(file, TRUE);
}

GNode *get_next_file_not_deepper(GNode *file)
{
	return get_next_file_real(file, FALSE);
}

char *get_file_info(GNode *file)
{
	struct passwd *pw;
	struct group *grp;
	struct tm *mtime;
	char mode[11], mtime_str[17], size_suffix;
	double size;

	switch (FILE(file)->info.st_mode & S_IFMT) {
		case S_IFSOCK:
			mode[0] = 's';
			break;
		case S_IFLNK:
			mode[0] = 'l';
			break;
		case S_IFREG:
			mode[0] = '-';
			break;
		case S_IFBLK:
			mode[0] = 'b';
			break;
		case S_IFDIR:
			mode[0] = 'd';
			break;
		case S_IFCHR:
			mode[0] = 'c';
			break;
		case S_IFIFO:
			mode[0] = 'p';
			break;
	}
	mode[1] = (FILE(file)->info.st_mode & S_IRUSR) ? 'r' : '-';
	mode[2] = (FILE(file)->info.st_mode & S_IWUSR) ? 'w' : '-';
	mode[3] = (FILE(file)->info.st_mode & S_IXUSR) ? 'x' : '-';

	mode[4] = (FILE(file)->info.st_mode & S_IRGRP) ? 'r' : '-';
	mode[5] = (FILE(file)->info.st_mode & S_IWGRP) ? 'w' : '-';
	mode[6] = (FILE(file)->info.st_mode & S_IXGRP) ? 'x' : '-';

	mode[7] = (FILE(file)->info.st_mode & S_IROTH) ? 'r' : '-';
	mode[8] = (FILE(file)->info.st_mode & S_IWOTH) ? 'w' : '-';
	mode[9] = (FILE(file)->info.st_mode & S_IXOTH) ? 'x' : '-';
	mode[10] = '\0';

	if (FILE(file)->info.st_mode & S_ISUID && mode[3] == 'x')
		mode[9] = 's';
	else if (FILE(file)->info.st_mode & S_ISUID && mode[3] == '-')
		mode[9] = 'S';
	if (FILE(file)->info.st_mode & S_ISGID && mode[6] == 'x')
		mode[9] = 's';
	else if (FILE(file)->info.st_mode & S_ISGID && mode[6] == '-')
		mode[9] = 'S';
	if (FILE(file)->info.st_mode & S_ISVTX && mode[9] == 'x')
		mode[9] = 't';
	else if (FILE(file)->info.st_mode & S_ISVTX && mode[9] == '-')
		mode[9] = 'T';

	if ((pw = getpwuid(FILE(file)->info.st_uid)) == NULL) {
		if (errno == 0) {
			struct passwd pw_unknown;

			pw_unknown.pw_name = "unknown";
			pw = &pw_unknown;
		} else {
			PRINT_ERRNO_INFO();
			return NULL;
		}
	} if ((grp = getgrgid(FILE(file)->info.st_gid)) == NULL) {
		if (errno == 0) {
			struct group grp_unknown;

			grp_unknown.gr_name = "unknown";
			grp = &grp_unknown;
		} else {
			PRINT_ERRNO_INFO();
			return NULL;
		}
	}

	if ((size =(double) FILE(file)->info.st_size / (1024.0 * 1024.0 * 1024.0)) > 1.0)
		size_suffix = 'G';
	else if ((size =(double) FILE(file)->info.st_size / (1024.0 * 1024.0)) > 1.0)
		size_suffix = 'M';
	else if ((size =(double) FILE(file)->info.st_size / (1024.0)) > 1.0)
		size_suffix = 'K';
	else {
		size =(double) FILE(file)->info.st_size;
		size_suffix = ' ';
	}

	if ((mtime = localtime(&(FILE(file)->info.st_mtime))) == NULL) {
		PRINT_ERRNO_INFO();
		return NULL;
	}
	if (strftime(mtime_str, sizeof(mtime_str), "%Y-%m-%d %H:%M", mtime) == 0) {
		PRINT_ERRNO_INFO();
		return NULL;
	}

	if (FILE(file)->link == TRUE) {
		if (size_suffix == ' ')
			return g_strdup_printf("%s %s %s %i %s %s -> %s", mode, pw->pw_name, grp->gr_name,
					(int) size, mtime_str, FILE(file)->name, FILE(file)->link_path);
		else
			return g_strdup_printf("%s %s %s %.2f%c %s %s -> %s", mode, pw->pw_name, grp->gr_name,
					size, size_suffix, mtime_str, FILE(file)->name, FILE(file)->link_path);
	} else {
		if (size_suffix == ' ')
			return g_strdup_printf("%s %s %s %i %s %s", mode, pw->pw_name, grp->gr_name,
					(int) size, mtime_str, FILE(file)->name);
		else
			return g_strdup_printf("%s %s %s %.2f%c %s %s", mode, pw->pw_name, grp->gr_name,
					size, size_suffix, mtime_str, FILE(file)->name);
	}
}
