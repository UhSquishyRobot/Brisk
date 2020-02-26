#include <stdio.h>
#include "mpc.h"

static char input[2048];

int main(int argc, char** argv) {
	/* Create parsers */
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Expr = mpc_new("expr");
	mpc_parser_t* Brisk = mpc_new("brisk");

	/* Define with language */
	mpca_lang(MPCA_LANG_DEFAULT,
		"							\
		number	: /-?[0-9]+(.[0-9]+|)/;					\
		operator: '+' | '-' | '*' | '/' | '%';			\
		expr	: <number> | '(' <operator> <expr>+ ')';	\
		brisk	: /^/ <operator> <expr>+ /$/;			\
		",
	Number, Operator, Expr, Brisk 
	);

	puts("Brisk Version 0.0.0.0.1");
	puts("Press Ctrl+c to Exit\n");

	while (1) {
		fputs("brisk> ", stdout);
		fflush(stdout);
		fgets(input, 2048, stdin);
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Brisk, &r)) {
			mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		} else {
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
	}

	mpc_cleanup(4, Number, Operator, Expr, Brisk);
	
	return 0;
}
