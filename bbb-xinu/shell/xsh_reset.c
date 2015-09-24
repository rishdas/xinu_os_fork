/* xsh_reset.c - xsh_reset */

#include <xinu.h>

/*------------------------------------------------------------------------
 * xsh_reset - Reset the CPU
 *------------------------------------------------------------------------
 */
shellcmd xsh_reset(int nargs, char *args[]) {

  volatile uint32 *prm_rstctrl = (volatile uint32 *)0x44E00F00;
  
  if ((nargs > 1) && (strncmp(args[1], "--help", 7)) == 0) {
    printf("Use: %s\n\n", args[0]);
    printf("Description:\n");
    printf("\tResets the CPU\n");
    return 0;
  }

  *prm_rstctrl = (uint32) 0x01;

  return 0;
}
