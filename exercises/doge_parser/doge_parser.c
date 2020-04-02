#include <stdio.h>
#include "mpc.h"

static char input[2048];

int main(int argc, char** argv) {
	mpc_parser_t* Adjective = mpc_new("adjective");
	mpc_parser_t* Noun      = mpc_new("noun");
	mpc_parser_t* Phrase    = mpc_new("phrase");
	mpc_parser_t* Doge      = mpc_new("doge");

	mpca_lang(MPCA_LANG_DEFAULT,
		  "                                           \
		    adjective : \"wow\" | \"many\"            \
			      |  \"so\" | \"such\";           \
		    noun      : \"lisp\" | \"language\"       \
			      | \"book\" | \"build\" | \"c\"; \
		    phrase    : <adjective> <noun>;           \
		    doge      : <phrase>*;                    \
		  ",
 	Adjective, Noun, Phrase, Doge);

	puts("DogeParser Version 0.0.0.0.1");
	puts("Press Ctrl+c to Exit\n");

	while (1) {
		fputs("brisk> ", stdout);
		fflush(stdout);
		fgets(input, 2048, stdin);
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Doge, &r)) {
			mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		} else {
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
	}

	mpc_cleanup(4, Adjective, Noun, Phrase, Doge);
	
	return 0;
}
