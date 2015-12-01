/* xsh_talk.c - xsh_talk */

#include <xinu.h>
#include <stdio.h>

#define LINE_LEN 255

/*------------------------------------------------------------------------
 * xsh_talk - Chat with someone
 *------------------------------------------------------------------------
 */
shellcmd xsh_talk(int nargs, char *args[]) {

    uint32 dst_ipaddr;
    uint16 dst_port;
    uint16 src_port;
    uid32 slot;
    char line[] = "Test UDP msg";
    uint32 recv_len;
    int retval;
    
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
    printf("Use: %s  IP\n\n", args[0]);
    printf("Description:\n");
    printf("\tStart talking with someone else\n");
    printf("Options:\n");
    printf("\t--help\t display this help and exit\n");
    printf("\tIP\t the destination IP address\n");
    return 0;
  }

  dst_port = 7;
  src_port = 50000;
  
  if ((dot2ip (args[1], &dst_ipaddr)) == SYSERR ){
      printf ("%s: invalid IP address\n",args[0]);
      return 1;
  }
  
  slot = udp_register (dst_ipaddr,dst_port,src_port);
  if (slot == SYSERR) {
      printf ("%s: Could not register slot\n",args[0]);
      return 1;
  }
  while (TRUE) {
/*      
      if ((recv_len = udp_recv (slot, line, LINE_LEN, 1000)) == SYSERR) {
	  printf ("%s: udp_recv() error\n",args[0]);
	  return 1;
      }
      if (recv_len != TIMEOUT) {
	  printf ("RECV_MSG: %s\n",line);
      } else {
	  printf ("udp_recv() TIMEOUT!\n");
      }
*/    
/*
      if ((recv_len = read (CONSOLE,line,LINE_LEN)) == SYSERR) {
	  printf ("%s: read() error\n",args[0]);
	  return 1;
      }
      if (*line == 'F') {
	  break;
      }
*/
      recv_len = strnlen (line,LINE_LEN);
      retval = udp_send (slot,line,recv_len);
      
      printf ("SEND_MSG: %s | len = %d\n",line,recv_len);
      if ( retval == SYSERR) {
	  printf ("%s: udp_send() error\n",args[0]);
	  return 1;
      }
      break;
      
  }
  
  udp_release (slot);
  return 0;
}
