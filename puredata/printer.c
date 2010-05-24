/* Compile with:
 * gcc -shared helloworld.c -o helloworld.pd_linux
 */

#include "m_pd.h"
#include <stdio.h>

static t_class *printer_class;

typedef struct _printer {
  t_object  x_obj;
} t_printer;

void printer_bang(t_printer *x)
{
  post("Hello world !!");
	system("ls");
}

void *printer_new(t_symbol *s, int argc, t_atom *argv)
//void *printer_new(void)
{
	int i;
	printf("argc: %d\n", argc);
	t_symbol as;
	for(i = 0;i < argc;i++) {
		as = atom_getsymbol(argv[i]);
		//puts((char *)&argv[i]);
	}
  t_printer *x = (t_printer *)pd_new(printer_class);

  return (void *)x;
}

void printer_setup(void) {
  printer_class = class_new(gensym("printer"),
        (t_newmethod)printer_new,
        0, sizeof(t_printer),
        CLASS_DEFAULT, 0);
  class_addbang(printer_class, printer_bang);
}

