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

char *program_name;

void print_errno_and_exit(int last_errno, char *file, int line)
{
	char *message_format = "%s (%s:%i)";
	char *message;

	endwin();
	message = g_strdup_printf(message_format, program_name, file, line);
	errno = last_errno;
	perror(message);
	free(message);
	exit(EXIT_FAILURE);
}

void handle_sigsegv(int signal_number)
{
	endwin();
	fprintf(stderr, "%s\n", g_strsignal(SIGSEGV));
	exit(EXIT_FAILURE);
}

void handle_sigint(int signal_number)
{
	endwin();
	exit(EXIT_SUCCESS);
}

void set_sighandlers(void)
{
	struct sigaction act;

	act.sa_handler = handle_sigsegv;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGSEGV, &act, NULL);

	act.sa_handler = handle_sigint;
	sigaction(SIGINT, &act, NULL);

	act.sa_handler = handle_sigwinch;
	sigaction(SIGWINCH, &act, NULL);
}

void unset_sighandlers(void)
{
	struct sigaction act;

	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGSEGV, &act, NULL);
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGWINCH, &act, NULL);
}

int main(int argc, char *argv[])
{
	if (argc > 1 && !strcmp(argv[1], "-h")) {
		printf("Usage: fm [<root_directory>]\n");
		return 0;
	}

	if (setlocale(LC_ALL, "") == NULL)
		fprintf(stderr, "Error: Can't set the specified locale\n");

	set_sighandlers();
	program_name = argv[0];
	if (argc == 1)
		init_ui(NULL);
	else
		init_ui(argv[1]);
	for (;;) {
		get_key();
		refresh_screen();
	}
	return 0;
}
