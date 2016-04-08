/* xsh_talk.c - xsh_talk */

#include <talk.h>



/*------------------------------------------------------------------------
 * xsh_talk - Chat with someone
 *------------------------------------------------------------------------
 */


shellcmd xsh_talk(int nargs, char *args[]) {

    uint32 dst_ipaddr;
    uint16 dst_port;
    uint16 src_port;
    uint32 slot;
    char in_line[LINE_LEN - 20];
    char snd_line[LINE_LEN];
    uint32 recv_len;
    int snd_len;
    int retval;
    fut32 f_talk;
    pid32 print_proc;

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

  f_talk = future_alloc(FUTURE_EXCLUSIVE);
  
  print_proc = create(f_talk_print, 1024, 20,
		      "f_talk_print", 1, f_talk);

  resume(print_proc);
  dst_port = src_port = 100;
  
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
      
      bzero (in_line,sizeof (in_line));
      bzero (snd_line,LINE_LEN);
	  if ((recv_len = udp_recv (slot, snd_line, 
				    LINE_LEN, 
				    1000)) == SYSERR) {
	  printf ("%s: udp_recv() error\n",args[0]);
	  return 1;
      }
      if (recv_len != TIMEOUT) {
          snd_len = strlen(snd_line);
          future_set_str(f_talk, 
			 &snd_len, snd_line);
      }
      printf("Enter the message to send:\n");
      if ((recv_len = read (CONSOLE,in_line,
			    sizeof (in_line))) == SYSERR) {
	  printf ("%s: read() error\n",args[0]);
	  return 1;
      }
      if (*in_line == '-') {
	  break;
      }
      strcpy (snd_line,in_line);
      /* more than 20 chars hack */
      if (recv_len < 20) {
	  strncat (snd_line,"                    ",20);
      }	  
      recv_len = strnlen (snd_line,1200);
      retval = udp_send (slot, snd_line, recv_len);
      
      if ( retval == SYSERR) {
	  printf ("%s: udp_send() error\n",args[0]);
	  return 1;
      }
  }
  kill (print_proc);
  udp_release (slot);
  return 0;
}
