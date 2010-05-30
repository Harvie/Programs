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

void get_key(void)
{
	static int command_repetitions = -1;
	static chtype previous_key = 0;
	char *info;
	chtype key;

#ifdef INOTIFY
	while ((key = getch()) == ERR)
		watch_events();
#else
	key = getch();
#endif
	if (key != KEY_RESIZE)
		clear_info();

	if (isdigit(key)) {
		if (command_repetitions == -1)
			command_repetitions = key - '0';
		else {
			command_repetitions *= 10;
			command_repetitions += key - '0';
		}
		previous_key = key;

		return;
	}

	if (previous_key == 'f') {
		select_next_line_by_first_letter((char) key);
		command_repetitions = -1;
		previous_key = 0;

		return;
	} else if (previous_key == 'F') {
		select_previous_line_by_first_letter((char) key);
		command_repetitions = -1;
		previous_key = 0;

		return;
	}

	if (command_repetitions == -1)
		command_repetitions = 1;

	switch (key) {
		case KEY_RESIZE:
			endwin();
			init_curses();
			update_last_line();
			print_lines(first_line, last_line, TRUE);
			select_line_inside_window();
			PRINT_LAST_INFO();
			break;
		case 0x0c: /* Ctrl + l */
			clearok(curscr, TRUE);
			if (!G_NODE_IS_ROOT(NODE(selected_line)))
				select_file(NODE(selected_line)->parent);
			update_directory(NODE(selected_line));
			break;
		case 0x1b: /* Escape */
			command_repetitions = -1;
			previous_key = 0;

			return;
		case KEY_NPAGE:
		case 0x06: /* Ctrl + f */
			select_line(last_line);
			scroll_tree(command_repetitions * (getmaxy(tree_window) - 1));
			break;
		case KEY_PPAGE:
		case 0x02: /* Ctrl + b */
			select_line(first_line);
			scroll_tree(command_repetitions * (-(getmaxy(tree_window) - 1)));
			break;
		case 0x04: /* Ctrl + d */
			scroll_tree(command_repetitions * getmaxy(tree_window) / 2);
			select_nth_line(command_repetitions * getmaxy(tree_window) / 2);
			break;
		case 0x15: /* Ctrl + u */
			scroll_tree(command_repetitions * -getmaxy(tree_window) / 2);
			select_nth_line(command_repetitions * -getmaxy(tree_window) / 2);
			break;
		case 'g':
			if (previous_key == 'g')
				select_line(lines);
			break;
		case 'G':
			select_line(g_list_last(lines));
			break;
		case 0x07: /* Ctrl + g */
			if ((info = get_file_info(NODE(selected_line))) != NULL) {
				print_info(info, FALSE);
				free(info);
			}
			break;
		case 'z':
			if (previous_key == 'z')
				scroll_tree(g_list_position(first_line, selected_line) -
						getmaxy(tree_window) / 2);
			break;
		case 'H':
			select_line(first_line);
			break;
		case 'L':
			select_line(last_line);
			break;
		case KEY_DOWN:
		case 'j':
			select_nth_line(command_repetitions);
			break;
		case KEY_UP:
		case 'k':
			select_nth_line(-command_repetitions);
			break;
		case KEY_RIGHT:
		case 'l':
			if (FILE(NODE(selected_line))->type == directory_type &&
					FILE(NODE(selected_line))->open == FALSE)
				open_directory(NODE(selected_line));
			else if (FILE(NODE(selected_line))->type == directory_type &&
					FILE(NODE(selected_line))->open == TRUE &&
					FILE(NODE(selected_line))->show_dotfiles == FALSE)
				show_dotfiles(NODE(selected_line));
			else if (FILE(NODE(selected_line))->type == file_type)
				read_file(NODE(selected_line));
			break;
		case KEY_LEFT:
		case 'h':
close_parent:		if (FILE(NODE(selected_line))->type == directory_type &&
					FILE(NODE(selected_line))->open == TRUE &&
					FILE(NODE(selected_line))->show_dotfiles == TRUE &&
					!G_NODE_IS_LEAF(NODE(selected_line)) &&
					FILE(g_node_first_child(NODE(selected_line)))->name[0] == '.')
				hide_dotfiles(NODE(selected_line));
			else if (FILE(NODE(selected_line))->type == directory_type &&
					FILE(NODE(selected_line))->open == TRUE &&
					!G_NODE_IS_LEAF(NODE(selected_line)))
				close_directory(NODE(selected_line));
			else if (!G_NODE_IS_ROOT(NODE(selected_line))) {
				select_file(NODE(selected_line)->parent);
				goto close_parent;
			}
			break;
		case 'q':
			endwin();
			clean_up();
			exit(EXIT_SUCCESS);
			break;
		case '{':
			if (g_node_prev_sibling(NODE(selected_line)) != NULL)
				select_file(g_node_prev_sibling(NODE(selected_line)));
			else
				select_nth_line(-1);
			break;
		case '}':
			if (g_node_next_sibling(NODE(selected_line)) != NULL)
				select_file(g_node_next_sibling(NODE(selected_line)));
			else
				select_nth_line(1);
			break;
		case '[':
			if (previous_key == '[' && !G_NODE_IS_ROOT(NODE(selected_line)))
				select_file(NODE(selected_line)->parent);
			break;
		case 's':
			if (!G_NODE_IS_ROOT(NODE(selected_line)))
				run_shell(NODE(selected_line)->parent);
			else
				run_shell(NODE(selected_line));
			break;
		case KEY_ENTER:
		case 0x0d: /* Enter */
			if (FILE(NODE(selected_line))->type == file_type)
				edit_file(NODE(selected_line));
			break;
	}
	if (key != KEY_RESIZE) {
		command_repetitions = -1;
		previous_key = key;
	}
}

void run_program(GNode *current_directory, char *program_name_variable,
		char *default_program_name, char *arg, gboolean wait_proccess)
{
	char *path, *pwd, *program_name;
	pid_t child;
	int status;

	endwin();
	if ((child = fork()) != 0 && wait_proccess) {
		unset_sighandlers();
		waitpid(child, &status, 0);
		set_sighandlers();
	} else if (child == 0) {
		path = get_path(current_directory);
		if (chdir(path) == -1)
			exit(EXIT_FAILURE);
		pwd = g_strconcat("PWD=", path, NULL);
		putenv(pwd);
		free(path);

		if (!wait_proccess) {
			close(0);
			close(1);
			close(2);
		}

		if (program_name_variable != NULL &&
				(program_name = getenv(program_name_variable)) != NULL) {
			if (execlp(program_name, program_name, arg, NULL) == -1)
				exit(EXIT_FAILURE);
		} else {
			if (execlp(default_program_name, default_program_name, arg, NULL) == -1)
				exit(EXIT_FAILURE);
		}
	}
	init_curses();
	update_last_line();
	print_lines(first_line, last_line, TRUE);
	select_line_inside_window();

	if (wait_proccess && WIFEXITED(status) && WEXITSTATUS(status) != EXIT_SUCCESS)
		PRINT_ERROR_INFO("Something failed when running the external program");
}

void run_shell(GNode *directory)
{
	run_program(directory, "SHELL", DEFAULT_SHELL, NULL, TRUE);
}

void read_file(GNode *file)
{
	if (g_str_has_suffix(FILE(file)->name, ".pdf") ||
			g_str_has_suffix(FILE(file)->name, ".pdf.gz") ||
			g_str_has_suffix(FILE(file)->name, ".pdf.Z"))
		run_program(file->parent, NULL, DEFAULT_PDF_VIEWER, FILE(file)->name, FALSE);
	else if (g_str_has_suffix(FILE(file)->name, ".ps") ||
			g_str_has_suffix(FILE(file)->name, ".ps.gz") ||
			g_str_has_suffix(FILE(file)->name, ".ps.Z"))
		run_program(file->parent, NULL, DEFAULT_PS_VIEWER, FILE(file)->name, FALSE);
	else if (g_str_has_suffix(FILE(file)->name, ".jpg") ||
			g_str_has_suffix(FILE(file)->name, ".jpeg") ||
			g_str_has_suffix(FILE(file)->name, ".png") ||
			g_str_has_suffix(FILE(file)->name, ".gif"))
		run_program(file->parent, NULL, DEFAULT_IMAGE_VIEWER, FILE(file)->name, FALSE);
	else
		run_program(file->parent, "PAGER", DEFAULT_PAGER, FILE(file)->name, TRUE);
}

void edit_file(GNode *file)
{
	run_program(file->parent, "EDITOR", DEFAULT_EDITOR, FILE(file)->name, TRUE);
}
