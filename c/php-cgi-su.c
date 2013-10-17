/*
 * SU-EXEC Wrapper
 * Execute script under it's owner's privileges
 * CopyLefted by: Harvie 2oo9
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#define INTERPRETER "/usr/bin/php-cgi"
//#define INTERPRETER "/usr/bin/perl"

void auth_fail() {
	puts("Error: Permission denied!\n");
	exit(-1);
}

int main(int argc, char **argv, char **environ) {
	if(argc != 2) { //Do not accept more than one argument
		printf(
			"SetUID wrapper for %s interpretter\n"
			"Usage: %s script\n\n",
			INTERPRETER, argv[0]
		);
		return -1;
	}
	struct stat st;
	if(!stat(argv[1], &st)) {
		//Get user info
			struct passwd *pw;
			if(!(pw = getpwuid(st.st_uid))) auth_fail();
		//Change groups
			if(initgroups(pw->pw_name, pw->pw_gid)) auth_fail();
		//Change UID a GID
			if(setgid(pw->pw_gid)) auth_fail();
			if(setegid(pw->pw_gid)) auth_fail();
			if(setuid(pw->pw_uid)) auth_fail();
			if(seteuid(pw->pw_uid)) auth_fail();
		//Fail if still have root privileges
			if(getuid() == 0 || getgid() == 0) auth_fail();
		//Launch binary
			return(execve(INTERPRETER, argv, environ));
	} else {
		printf("Error: Can't stat file: %s\n\n", argv[1]);
		return -1;
	}
}
