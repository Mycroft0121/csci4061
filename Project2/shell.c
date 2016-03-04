#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "util.h"

/*
 * Read a line from stdin.
 */
char *sh_read_line(void)
{
	char *line = NULL;
	ssize_t bufsize = 0;

	getline(&line, &bufsize, stdin);
	return line;
}

/*
 * Do stuff with the shell input here.
 */
int sh_handle_input(char *line, int fd_toserver)
{
	
	/***** Insert YOUR code *******/
	
 	/* Check for \seg command and create segfault */
	
	/* Write message to server for processing */
	return 0;
}

/*
 * Check if the line is empty (no data; just spaces or return)
 */
int is_empty(char *line)
{
	while (*line != '\0') {
		if (!isspace(*line))
			return 0;
		line++;
	}
	return 1;
}

/*
 * Start the main shell loop:
 * Print prompt, read user input, handle it.
 */
void sh_start(char *name, int fd_toserver)
{
	/***** Insert YOUR code *******/
}

int main(int argc, char **argv)
{
	
	/***** Insert YOUR code *******/
	
	
	/* Extract pipe descriptors and name from argv */
	if(argc == 6)
	{
		char* name = argv[1];

		int fd_serv_write[2] = {atoi(argv[2]), atoi(argv[3])};
		int fd_serv_read[2] = {atoi(argv[4]), atoi(argv[5])};

		if(close(fd_serv_read[1]) == -1)
		{
			perror("close failed!");
			exit(-1);
		}
		if(close(fd_serv_write[0]) == -1)
		{
			perror("close failed!");
			exit(-1);
		}
	}

	/* Fork a child to read from the pipe continuously */

	/*
	 * Once inside the child
	 * look for new data from server every 1000 usecs and print it
	 */ 

	/* Inside the parent
	 * Send the child's pid to the server for later cleanup
	 * Start the main shell loop
	 */
}