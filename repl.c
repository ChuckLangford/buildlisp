#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
/* #include <editline/history.h> */

int main(int argc, char** argv)
{
  puts("Lispy Version 0.0.1");
  puts("Press Ctrl+c to Exit\n");

  while (1) {
    /* Output our prompt */
    char* input = readline("lispy> ");

    add_history(input);

    /* Echo input back to the user */
    printf("No, you're a %s\n", input);

    free(input);
  }

  return 0;
}
