CC=gcc
OBJ=vm_udp_talk.o
%.o: %.c
	$(CC) -c -o $@ $< -g
build: vm_udp_talk.o
	$(CC) -o talk.bin $(OBJ) -g
clean:
	rm -f *.o *~ *.bin
