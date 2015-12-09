#include<talk.h>

void print_str(char *print_buf, int len)
{
    int i = 0;
    printf("\nReceived from Ubuntu: ");
    for (i = 0; i<len; i++) {
	printf("%c", print_buf[i]);
    }
    printf("\n");
}
void f_talk_print(fut32 f)
{
    char print_buf[LINE_LEN];
    int  len;
    
    while (TRUE) {
	future_get_str(f, &len, print_buf);
	print_str(print_buf, len);
    }
}
