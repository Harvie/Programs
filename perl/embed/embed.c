#include <EXTERN.h>               /* from the Perl distribution     */
#include <perl.h>                 /* from the Perl distribution     */
static PerlInterpreter *my_perl = NULL;  /***    The Perl interpreter    ***/

//dynaloader
static void xs_init (pTHX);
EXTERN_C void boot_DynaLoader (pTHX_ CV* cv);
EXTERN_C void boot_Socket (pTHX_ CV* cv);
EXTERN_C void
xs_init(pTHX) {
	char *file = __FILE__;
	newXS("DynaLoader::boot_DynaLoader", boot_DynaLoader, file); //DynaLoader is a special case
}
//dynaloader - end


int main(int argc, char **argv, char **env) {
	my_perl = perl_alloc();
	perl_construct(my_perl);

	if(0) { //interactive?
		perl_parse(my_perl, xs_init, argc, argv, (char **)NULL);
	} else {
		char *embedding[] = { "", "-e", "0" };
		//perl_parse(my_perl, NULL, 3, embedding, NULL); //no dynamic loader
		perl_parse(my_perl, xs_init, 3, embedding, NULL);
	}

  eval_pv(
		"use LWP::Simple; getprint('http://harvie.cz/');"
		"print('lol\n');"
	, TRUE);
  //eval_pv("$a = 3; $a **= 2", TRUE);  printf("a = %d\n", SvIV(get_sv("a", FALSE)));
	//eval_pv("$a = 3.14; $a **= 2", TRUE); printf("a = %f\n", SvNV(get_sv("a", FALSE)));
  //eval_pv("$a = 'rekcaH lreP rehtonA tsuJ'; $a = reverse($a);", TRUE); printf("a = %s\n", SvPV(get_sv("a", FALSE), n_a));

	perl_run(my_perl);
	perl_destruct(my_perl);
	perl_free(my_perl);
}
