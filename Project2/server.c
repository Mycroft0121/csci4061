#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include "util.h"

/*
 * Identify the command used at the shell 
 */
int parse_command(char *buf)
{
	int cmd;

	if (starts_with(buf, CMD_CHILD_PID))
		cmd = CHILD_PID;
	else if (starts_with(buf, CMD_P2P))
		cmd = P2P;
	else if (starts_with(buf, CMD_LIST_USERS))
		cmd = LIST_USERS;
	else if (starts_with(buf, CMD_ADD_USER))
		cmd = ADD_USER;
	else if (starts_with(buf, CMD_EXIT))
		cmd = EXIT;
	else if (starts_with(buf, CMD_KICK))
		cmd = KICK;
	else
		cmd = BROADCAST;

	return cmd;
}

/*
 * List the existing users on the server shell
 */
int list_users(user_chat_box_t *users, int fd)
{
	/* 
	 * Construct a list of user names
	 * Don't forget to send the list to the requester!
	 */
	 
	 /***** Insert YOUR code *******/
}

/*
 * Add a new user
 */
int add_user(user_chat_box_t *users, char *buf, int server_fd)
{
	/***** Insert YOUR code *******/
	
	/* 
	 * Check if user limit reached.
	 *
	 * If limit is okay, add user, set up non-blocking pipes and
	 * notify on server shell
	 *
	 * NOTE: You may want to remove any newline characters from the name string 
	 * before adding it. This will help in future name-based search.
	 */ 
}

/*
 * Broadcast message to all users. Completed for you as a guide to help with other commands :-).
 */
int broadcast_msg(user_chat_box_t *users, char *buf, int fd, char *sender)
{
	int i;
	const char *msg = "Broadcasting...", *s;
	char text[MSG_SIZE];

	/* Notify on server shell */
	if (write(fd, msg, strlen(msg) + 1) < 0)
		perror("writing to server shell");
	
	/* Send the message to all user shells */
	s = strtok(buf, "\n");
	sprintf(text, "%s : %s", sender, s);
	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].status == SLOT_EMPTY)
			continue;
		if (write(users[i].ptoc[1], text, strlen(text) + 1) < 0)
			perror("write to child shell failed");
	}
}

/*
 * Close all pipes for this user
 */
void close_pipes(int idx, user_chat_box_t *users)
{
	/***** Insert YOUR code *******/
}

/*
 * Cleanup single user: close all pipes, kill user's child process, kill user 
 * xterm process, free-up slot.
 * Remember to wait for the appropriate processes here!
 */
void cleanup_user(int idx, user_chat_box_t *users)
{
	/***** Insert YOUR code *******/
}

/*
 * Cleanup all users: given to you
 */
void cleanup_users(user_chat_box_t *users)
{
	int i;

	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].status == SLOT_EMPTY)
			continue;
		cleanup_user(i, users);
	}
}

/*
 * Cleanup server process: close all pipes, kill the parent process and its 
 * children.
 * Remember to wait for the appropriate processes here!
 */
void cleanup_server(server_ctrl_t server_ctrl)
{
	/***** Insert YOUR code *******/
}

/*
 * Utility function.
 * Find user index for given user name.
 */
int find_user_index(user_chat_box_t *users, char *name)
{
	int i, user_idx = -1;

	if (name == NULL) {
		fprintf(stderr, "NULL name passed.\n");
		return user_idx;
	}
	for (i = 0; i < MAX_USERS; i++) {
		if (users[i].status == SLOT_EMPTY)
			continue;
		if (strncmp(users[i].name, name, strlen(name)) == 0) {
			user_idx = i;
			break;
		}
	}

	return user_idx;
}

/*
 * Utility function.
 * Given a command's input buffer, extract name.
 */
char *extract_name(int cmd, char *buf)
{
	char *s = NULL;

	s = strtok(buf, " ");
	s = strtok(NULL, " ");
	if (cmd == P2P)
		return s;	/* s points to the name as no newline after name in P2P */
	s = strtok(s, "\n");	/* other commands have newline after name, so remove it*/
	return s;
}

/*
 * Send personal message. Print error on the user shell if user not found.
 */
void send_p2p_msg(int idx, user_chat_box_t *users, char *buf)
{
	/* get the target user by name (hint: call (extract_name() and send message */
	
	/***** Insert YOUR code *******/
}

int main(int argc, char **argv)
{
	
	/***** Insert YOUR code *******/
	
	/* open non-blocking bi-directional pipes for communication with server shell */
	int fd_serv[2];
	int fd_child[2];

	if(pipe(fd_serv) == -1)
	{
		perror("pipe failed!");
		exit(-1);
	}
	if(pipe(fd_child) == -1)
	{	
		perror("pipe failed!");
		exit(-1);
	}

	int flags = fcntl(fd_child[0], F_GETFL, 0);
	fcntl(fd_child[0], F_SETFL, flags | O_NONBLOCK);

	int flags2 =  fcntl(fd_serv[0], F_GETFL, 0);
	fcntl(fd_serv[0], F_SETFL, flags2 | O_NONBLOCK);	
	
	char fd_servread[20];
	char fd_servwrite[20];
	char fd_childread[20];
	char fd_childwrite[20];

	sprintf(fd_servread, "%d", fd_serv[0]);
	sprintf(fd_servwrite, "%d", fd_serv[1]);
	sprintf(fd_childread, "%d", fd_child[0]);
	sprintf(fd_childwrite, "%d", fd_child[1]);

	
	
	/* Fork the server shell */

		/* 
	 	 * Inside the child.
		 * Start server's shell.
	 	 * exec the SHELL program with the required program arguments.
	 	*/

	int f;
	if((f = fork()) == -1)
	{
		perror("fork failed!");
		exit(-1);
	}
	if(f == 0)
	{		
		if(execlp("./shell", "shell", "Server", fd_servread, fd_servwrite, fd_childread, fd_childwrite, (char*)0) == -1)
		{
			perror("failed to exec!");
			exit(-1);
		}
	}
	
	if(close(fd_serv[1]) == -1)
	{
		perror("closing server pipe failed!");
		exit(-1);
	}

	if(close(fd_child[0]) == -1)
	{
		perror("closing child pipe failed!");
		exit(-1);
	}
	/* Inside the parent. This will be the most important part of this program. */

		/* Start a loop which runs every 1000 usecs.
	 	 * The loop should read messages from the server shell, parse them using the 
	 	 * parse_command() function and take the appropriate actions. */
	while (1)
	{
		/* Let the CPU breathe */
		
		usleep(1000);
		
		int numBytes;
		char buf[MSG_SIZE];

		numBytes = read(fd_serv[0], buf, MSG_SIZE);
		if(numBytes != -1 && numBytes != 0)
		{
			
			if(write(fd_child[1], buf, numBytes) == -1)
			{
				perror("write failed");
				exit(-1);
			}
			//do something
		}else if(numBytes == 0)
		{

			perror("read from pipe failed!");
			exit(-1);
		}
		
			/* 
		 	 * 1. Read the message from server's shell, if any
		 	 * 2. Parse the command
		 	 * 3. Begin switch statement to identify command and take appropriate action
		 	 *
		 	 * 		List of commands to handle here:
		 	 * 			CHILD_PID
		 	 * 			LIST_USERS
		 	 * 			ADD_USER
		 	 * 			KICK
		 	 * 			EXIT
		 	 * 			BROADCAST 
		 	 */
				
			/* Fork a process if a user was added (ADD_USER) */
				/* Inside the child */
				/*
			 	 * Start an xterm with shell program running inside it.
			 	 * execl(XTERM_PATH, XTERM, "+hold", "-e", <path for the SHELL program>, ..<rest of the arguments for the shell program>..);
			 	 */

			/* Back to our main while loop for the "parent" */
			/* 
		 	 * Now read messages from the user shells (ie. LOOP) if any, then:
		 	 * 		1. Parse the command
		 	 * 		2. Begin switch statement to identify command and take appropriate action
		 	 *
		 	 * 		List of commands to handle here:
		 	 * 			CHILD_PID
		 	 * 			LIST_USERS
		 	 * 			P2P
		 	 * 			EXIT
		 	 * 			BROADCAST
		 	 *
		 	 * 		3. You may use the failure of pipe read command to check if the 
		 	 * 		user chat windows has been closed. (Remember waitpid with WNOHANG 
		 	 * 		from recitations?)
		 	 * 		Cleanup user if the window is indeed closed.
		 	 */

	}	/* while loop ends when server shell sees the \exit command */

	return 0;
}
