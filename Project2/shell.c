#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "util.h"


static int isServer;
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
int sh_handle_input(char *line, int fd_toserver, int server)
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
	char* name;
	int fd_serv[2];
	int fd_child[2];
	
	/* Extract pipe descriptors and name from argv */
	name = argv[1];

	fd_serv[0] = atoi(argv[2]);
	fd_serv[1] = atoi(argv[3]);
	fd_child[0] = atoi(argv[4]);
	fd_child[1] = atoi(argv[5]);

	isServer = (argv[6][0] == '1')? 1 : 0;

	if(close(fd_serv[1]) == -1)
	{
		perror("close write pipe failed!");
		exit(-1);
	}
	

	/* Fork a child to read from the pipe continuously */
	int f;
	if((f = fork()) == -1)
	{
		perror("shell fork failed!");
		exit(-1);
	}

	/*
	 * Once inside the child
	 * look for new data from server every 1000 usecs and print it
	 */ 
	
	if(f == 0)
	{

		if(close(fd_child[1]) == -1)
		{
			perror("close read pipe failed!");
			exit(-1);
		}
		
		while(1)
		{
			usleep(1000);
			char buf[MSG_SIZE];
			int bytesRead = read(fd_serv[0], buf, MSG_SIZE);
			
			if(bytesRead != -1 && bytesRead != 0)
			{
				if(starts_with(buf, EXIT_CMD))
				{
					if(close(fd_serv[0]) == -1)
					{
						perror("close child pipe failed!");
						exit(-1);
					}
					exit(0);
				}
			
				if(!is_empty(buf)){	
					printf("%s\n", buf);
				}
				//do something
			}else if(bytesRead == 0)
			{
				perror("error reading in shell!");
				exit(-1);
			}
			
		}
	}

	if(close(fd_serv[0]) == -1)
	{
		perror("close child pipe failed!");
		exit(-1);
	}
	if(close(fd_child[0]) == -1)
	{
		perror("close child pipe failed!");
		exit(-1);
	}
	
	/* Inside the parent
	 * Send the child's pid to the server for later cleanup
	 * Start the main shell loop
	 */
	if(f >0){
	  char childPID[20];

	  sprintf(childPID, "%s %d", CMD_CHILD_PID, f);

	  if(write(fd_child[1], childPID, strlen(childPID)+1) == -1)
	    {
	      perror("write failed!");
	      exit(-1);
	    }
	
	  print_prompt(name);
	  while(1)
	    {
	      usleep(1000);
	      char* line;
	      
	      if((line = sh_read_line()) != NULL)
		{
		  print_prompt(name);
		  size_t len = strlen(line);
		  if(starts_with(line, CMD_SEG) && isServer == 0){
		    perror("Creating Seg Fault\n");
		    int* x =NULL;
		    *x = 1;
		  }
		  if(write(fd_child[1], line, len+1) == -1)
		    {
		      perror("write failed!");
		      exit(-1);
		    }
		  if(starts_with(line, EXIT_CMD))
		    break;
		}
	    }
	  
	  if(close(fd_child[1]) == -1)
	    {
	      perror("close child pipe failed!");
	      exit(-1);
	    }
	}
	return 0;

}
