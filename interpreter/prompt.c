#include <stdio.h>
#include "mpc.h"
#include <math.h>

typedef struct {
    int type;
    long num;
    double dub;
    int err;
} lval;

enum { LVAL_NUM, LVAL_ERR, LVAL_DUB };
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

lval lval_num(long x) {
    lval v;
    v.type = LVAL_NUM;
    v.num = x;
    return v;
}

lval lval_err(int x) {
    lval v;
    v.type = LVAL_ERR;
    v.err = x;
    return v;
}

lval lval_dub(double x) {
    lval v;
    v.type = LVAL_DUB;
    v.dub = x;
    return v;
}

void lval_print(lval v) {
    switch(v.type) {
        case LVAL_NUM: printf("%li", v.num); break;
        
        case LVAL_DUB: printf("%f", v.dub); break;

        case LVAL_ERR:
            if (v.err == LERR_DIV_ZERO) {
                printf("Error: Divison By Zero");
            }
            if (v.err == LERR_BAD_OP) {
                printf("Error: Invalid Operator");
            }
            if (v.err == LERR_BAD_NUM) {
                printf("Error: Invalid Number");
            }
        break;
    }
}

void lval_println(lval v) { lval_print(v); putchar('\n'); }

static char input[2048];

lval eval_op(lval x, char* op, lval y) {
    if (x.type == LVAL_ERR) { return x; }
    if (y.type == LVAL_ERR) { return y; }

	if (strcmp(op, "+") == 0) { return lval_num(x.num + y.num); }
	if (strcmp(op, "-") == 0) { return lval_num(x.num - y.num); }
	if (strcmp(op, "*") == 0) { return lval_num(x.num * y.num); }
	if (strcmp(op, "/") == 0) { 
        return y.num == 0
            ? lval_err(LERR_DIV_ZERO)
            : lval_num(x.num / y.num);
    } 
	if (strcmp(op, "^") == 0) { return lval_num(pow (x.num, y.num)); }
	if (strcmp(op, "%") == 0) { return lval_num(x.num % y.num); }
    if (strcmp(op, "min") == 0) { return lval_num(x.num < y.num ? x.num : y.num); }
    if (strcmp(op, "max.num") == 0) { return lval_num(x.num > y.num ? x.num : y.num); }
   
	return lval_err(LERR_BAD_OP);
}


lval eval(mpc_ast_t* t) {
	/* this is our base case.  if the expression is tagged as a number, we're at a leaf and can return */
	if (strstr(t->tag, "number")) {
        errno = 0;
        long x = strtol(t->contents, NULL, 10);
		return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);	
	}

    if (strstr(t->tag, "double")) {
        double x = strtod(t->contents, NULL);
        return lval_dub(x);
    }

	/* at this point we've determined that there's more work to do. the second child is always an operator
	 * we'll map this its counterpart in C later */
	char* op = t->children[1]->contents;
	
	/* the third child (and the first operand of the expression) will be an expression, which we'll now evaluate
	 * this is equivalent to recursively evaluating the leftmost branch of the AST */
	lval x = eval(t->children[2]);

    if ((strcmp(op, "-") == 0) && !strstr(t->children[3]->tag, "expr")) {
        x.num = x.num * -1;
    }
	
	/* we prepare to iterate over the remainder of the expressions by first taking note of our current position in the 
	 * brisk, which is 4th child 3rd index. */
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
	mpc_parser_t* Double = mpc_new("double");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Expr = mpc_new("expr");
	mpc_parser_t* Brisk = mpc_new("brisk");

	/* Define with language */
	mpca_lang(MPCA_LANG_DEFAULT,
		"							                                        \
		number	: /-?[0-9]+[^.]/;	                                        \
        double  : /-?[0-9]+.[0-9]+/ ;                                       \
		operator: '+' | '-' | '*' | '/' | '%' | '^' | \"min\" | \"max\";    \
		expr	: <number> | <double>  | '(' <operator> <expr>+ ')' ;       \
		brisk	: /^/ <operator> <expr>+ /$/ ;			                    \
		",
	Number, Double, Operator, Expr, Brisk 
	);

	puts("Brisk Version 0.0.0.0.1");
	puts("Press Ctrl+c to Exit\n");

	while (1) {
		fputs("brisk> ", stdout);
		fflush(stdout);
		fgets(input, 2048, stdin);
		mpc_result_t r;
		if (mpc_parse("<stdin>", input, Brisk, &r)) {
			lval result = eval(r.output);
			lval_println(result);
			mpc_ast_delete(r.output);
		} else {
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
	}

	mpc_cleanup(4, Number, Double, Operator, Expr, Brisk);
	
	return 0;
}
