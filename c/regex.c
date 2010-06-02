#include <stdio.h>
#include <glib.h>

//gcc `pkg-config --cflags --libs glib-2.0` regex.c -o regex

int main() {
	char string[] = "hello lol OMG NAZDAR ROFL! cus.";
	if(g_regex_match_simple("lolo", string, 0, 0)) puts("true");
		else puts("false");
}
