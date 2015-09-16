/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - A friendly command
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {

  if (nargs < 2) 
    {
      fprintf (stderr, "%s: too few arguments\n", args[0]);
      return 1;
    }
  else if (nargs > 2)
    {
      fprintf (stderr, "%s: too many arguments\n", args[0]);
      return 1;
    }
  
  if (strncmp(args[1], "--help", 7) == 0) {
    printf("Use: %s  name\n\n", args[0]);
    printf("Description:\n");
    printf("\tDisplay a friendly message\n");
    printf("Options:\n");
    printf("\t--help\t display this help and exit\n");
    printf("\tname\t the message addressed person's name\n");
    return 0;
  }
  
  printf ("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
  return 0;
}

