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

#ifndef FM_H
#define FM_H

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <dirent.h>
#include <signal.h>
#include <ctype.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <locale.h>
#include <glib.h>
#include <ncurses.h>

#undef FILE
#define FILE(node) ((File *) (node)->data)
#define NODE(element) ((GNode *) (element)->data)
#define PRINT_ERRNO_AND_EXIT() print_errno_and_exit(errno, __FILE__, __LINE__)
#define PRINT_ERRNO_INFO() print_error_info(errno, __FILE__, __LINE__, NULL)
#define PRINT_ERROR_INFO(message) print_error_info(0, __FILE__, __LINE__, message)
#define PRINT_LAST_INFO() print_info(NULL, -1)

#define DEFAULT_EDITOR "vim"
#define DEFAULT_PAGER "less"
#define DEFAULT_SHELL "bash"
#define DEFAULT_PS_VIEWER "gv"
#define DEFAULT_PDF_VIEWER "xpdf"
#define DEFAULT_IMAGE_VIEWER "display"
#define GETCH_DELAY 5

typedef struct File File;
struct File {
	char *name, *link_path;
	enum Type {file_type, directory_type} type;
	struct stat info;
	gboolean read, open, show_dotfiles, link;
	GList *line, *dotfiles;
	int watch;
};

extern char *program_name;
extern GNode *tree_root;
extern WINDOW *tree_window, *info_window;
extern GList *lines, *selected_line, *first_line, *last_line;

/*  cmd.c  */
void get_key(void);
void run_program(GNode *current_directory, char *program_name_variable,
		char *default_program_name, char *arg, gboolean wait_proccess);
void run_shell(GNode *directory);
void read_file(GNode *file);
void edit_file(GNode *file);

/*  fm.c  */
void clean_up(void);
void print_errno_and_exit(int last_errno, char *file, int line);
void handle_sigsegv(int signal_number);
void handle_sigint(int signal_number);
void set_sighandlers(void);
void unset_sighandlers(void);
void print_usage(void);

/*  fs.c  */
void init_tree(char *name);
File *create_new_file(char *name, char *path, gboolean exit_on_error);
gboolean read_directory(GNode *directory);
void free_element_data(gpointer element_data, gpointer data);
void destroy_dotfiles(GList *dotfiles);
gboolean free_node_data(GNode *node, gpointer data);
void destroy_directory_content(GNode *directory);
void destroy_directory_content_real(GNode *directory, gboolean remove_watch);
void add_dotfiles(GNode *directory);
void remove_dotfiles(GNode *directory);
int file_cmp(File *file1, File *file2);
int file_cmp_list(gconstpointer file1, gconstpointer file2);
void insert_sorted_in_dotfiles(GNode *directory, File *file);
GNode *insert_sorted_in_tree(GNode *directory, File *file);
char *get_path(GNode *file);
GNode *get_next_file_real(GNode *file, gboolean go_deeper);
GNode *get_previous_file(GNode *file);
GNode *get_next_file(GNode *file);
GNode *get_next_file_not_deepper(GNode *file);
char *get_file_info(GNode *file);

/*  ui.c  */
void init_lines(void);
void init_curses(void);
void init_ui(char *name);
void refresh_screen(void);
void handle_sigwinch(int signal_number);
void print_lines(GList *start_line, GList *end_line, gboolean clear_bottom_lines);
void print_parents_lines(GList *line);
void print_line(GList *line);
void clear_info(void);
void print_info(char *message, gboolean bold);
void print_error_info(int last_errno, char *file, int line, char *message);
void update_last_line(void);
void add_directory_content(GNode *directory);
void open_directory(GNode *directory);
void close_directory(GNode *directory);
void update_directory(GNode *directory);
void show_dotfiles(GNode *directory);
void hide_dotfiles(GNode *directory);
void select_line(GList *line);
void select_file(GNode *file);
void select_nth_line(int n_lines);
void scroll_tree(int n_lines);
void select_line_inside_window(void);
void select_next_line_by_first_letter(char letter);
void select_previous_line_by_first_letter(char letter);

#ifdef INOTIFY
#include <sys/inotify.h>

#define CHECK_INOTIFY_ENABLED() if (inotify_enabled == FALSE) return
#define BUFFER_LENGTH 10 * (sizeof(struct inotify_event) + 16)

extern gboolean inotify_enabled, debug_inotify;
extern GHashTable *watches_table;
extern int inotify_descriptor;
extern char events_buffer[BUFFER_LENGTH];

/* ino.c */
void init_inotify(GNode *tree_root);
void stop_inotify(void);
void free_key(gpointer key, gpointer value, gpointer data);
void add_watch_directory(GNode *directory);
void remove_watch_directory(GNode *directory, gboolean remove_watch);
void watch_events(void);
gboolean insert_in_tree(GNode *directory, File *file);
gboolean remove_from_tree(GNode *file, gboolean unmount);
GNode *search_node_by_name(GNode *directory, char *name);
void dump_event(struct inotify_event *event);
void dump_mask(int mask);
#endif

#endif
