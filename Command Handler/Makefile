all: command_handler.out command.out

command_handler.out: command_handler.c
	gcc -o commandHandler.out command_handler.c 

command.out: command.c
	gcc -o command.out command.c

#-lpthread
	
clean:
	rm -f *.out