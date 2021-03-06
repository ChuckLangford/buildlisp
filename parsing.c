#include <stdio.h>
#include <stdlib.h>
#include "mpc.h"

#ifdef _WIN32
#include <string.h>

static char buffer[2048];

/* Notes for me:
 * cc -std=c99 -Wall parsing.c mpc.c -ledit -lm -o ./out/parsing
 */

/* Fake readline function */
char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer) + 1);
  strcpy(cpy, buffer);
  cpy[strcpy(cpy) -1] = '\0';
  return cpy;
}

/* Face add_history function */
void add_history(char* unused);

#else
#include <editline/readline.h>
#endif

/* User operator string to see which operation to perform */
long eval_op(long x, char* op, long y) {
  if(strcmp(op, "+") == 0) { return x + y; }
  if(strcmp(op, "-") == 0) { return x - y; }
  if(strcmp(op, "*") == 0) { return x * y; }
  if(strcmp(op, "/") == 0) { return x / y; }
  return 0;
}

long eval(mpc_ast_t* t) {
  /* If tagged as number, return it directly */
  if (strstr(t->tag, "number")) {
    return atoi(t->contents);
  }

  /* The operator is always second child */
  char* op = t->children[1]->contents;

  /* We store the third child inside of 'x' */
  long x = eval(t->children[2]);

  /* Iterate the remaining children and combine */
  int i = 3;
  while(strstr(t->children[i]->tag, "expr")) {
    x = eval_op(x, op, eval(t->children[i]));
    i++;
  }

  return x;
}

int main(int argc, char** argv)
{
  /* Create some parsers */
  mpc_parser_t* Number = mpc_new("number");
  mpc_parser_t* Operator = mpc_new("operator");
  mpc_parser_t* Expr = mpc_new("expr");
  mpc_parser_t* Lispy = mpc_new("lispy");

  /* Define them with the following language */
  mpca_lang(MPCA_LANG_DEFAULT,
      " \
        number    : /-?[0-9]+/ ;  \
        operator  : '+' | '-' | '*' | '/'; \
        expr      : <number> | '(' <operator> <expr>+ ')' ; \
        lispy     : /^/ <operator> <expr>+ /$/ ; \
      ",
      Number, Operator, Expr, Lispy);

  puts("Lispy Version 0.0.1");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    /* Output our prompt */
    char* input = readline("lispy> ");

    add_history(input);

    /* Attempt to Parse the user Input */
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, Lispy, &r)) {
      /* On success print the ast */
      /* mpc_ast_print(r.output); */
      long result = eval(r.output);
      printf("%li\n", result);
      mpc_ast_delete(r.output);
    } else {
      /* Otherwise print the error */
      mpc_err_print(r.error);
      mpc_err_delete(r.error);
    }

    free(input);
  }

  /* Undefine and Delete our Parsers */
  mpc_cleanup(4, Number, Operator, Expr, Lispy);

  return 0;
}
