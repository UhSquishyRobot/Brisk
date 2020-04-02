#include <stdio.h>
#include "mpc.h"

static char input[2048];

long eval_op(long x, char* op, long y) {
	if (strcmp(op, "+") == 0) { return x + y; }
	if (strcmp(op, "-") == 0) { return x - y; }
	if (strcmp(op, "*") == 0) { return x * y; }
	if (strcmp(op, "/") == 0) { return x / y; }
	return 0;
}

long eval(mpc_ast_t* t) {
	/* this is our base case.  if the expression is tagged as a number, we're at a leaf and can return */
	if (strstr(t->tag, "number")) {
		return atoi(t->contents);	
	}

	/* at this point we've determined that there's more work to do. the second child is always an operator
	 * we'll map this its counterpart in C later */
	char* op = t->children[1]->contents;
	
	/* the third child (and the first operand of the expression) will be an expression, which we'll now evaluate
	 * this is equivalent to recursively evaluating the leftmost branch of the AST */
	long x = eval(t->children[2]);
	
	/* we prepare to iterate over the remainder of the expressions by first taking note of our current position in the 
	 * brisk, which is 3. */
	int i = 3;
	
	/* now, we do a reduce-like operation, where the evaluated left branch of the AST is the aggregator, the operator is
	 * the 'callback(?)' which is applied to the aggregator and the result of each remaining branch of the AST (after they've been
	 * recursively evaluted) */
	while (strstr(t->children[i]->tag, "expr")) {
		x = eval_op(x, op, eval(t->children[i]));
		i++;
	}

	return x;
}

int main(int argc, char** argv) {
	/* Create parsers */
	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Expr = mpc_new("expr");
	mpc_parser_t* Brisk = mpc_new("brisk");

	/* Define with language */
	mpca_lang(MPCA_LANG_DEFAULT,
		"							\
		number	: /-?[0-9]+/ ;		        \
		operator: '+' | '-' | '*' | '/' | '%' ;			\
		expr	: <number> | '(' <operator> <expr>+ ')' ;	\
		brisk	: /^/ <operator> <expr>+ /$/ ;			\
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
			long result = eval(r.output);
			printf("%li\n", result);
			mpc_ast_delete(r.output);
		} else {
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
	}

	mpc_cleanup(4, Number, Operator, Expr, Brisk);
	
	return 0;
}
