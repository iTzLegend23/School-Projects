/*	msh.c

	David Elder Waters
	02/16/2020
	CS 240
	Assignment 2
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int makearg(char* s, char*** args);
int getSubstring(char s[], char ss[], int start);
int length(char s[]);
void doCommand(char*** args);


int main(int argc, char* argv[])
{
	char c;
	char** args;
	char* cPtr; 	// How to handle arbitrary size?
	int i = 0;

	while (1)
	{
		printf("?: ");
		cPtr = calloc(100, sizeof(char));
		while ((c = getchar()) != '\n')
		{
			// After this loop is done, cPtr will contain the whole command line string
			cPtr[i] = c;
			//printf("cPtr[i]: %c\n", cPtr[i]);
			i++;
		}
		cPtr[i] = '\0';

		// Now have to parse the string
		argc = makearg(cPtr, &args);

		// Exit the program
		if (args[0][0] == 'e' && args[0][1] == 'x' && args[0][2] == 'i' && args[0][3] == 't')
		{
			printf("Exiting. . .\n");
			break;
		}

		i = 0;
		while (args[i] != '\0')
		{
			printf("Argument %i: %s\n", i + 1, args[i]);
			i++;
		}

		// Now fork() with the parsed string commands (?)
		doCommand(&args);

		while (args[i] != '\0')
		{
			free(args[i]);
			i++;
		}
		free(args);
		free(cPtr);
		i = 0;
	}
	return 0;
}


// Returns the integer value for the index of the start
// of the next token, if any. Additionally, populates args
// at the correct location with the value of the current token
int makearg(char* s, char*** args)
{
	int i = 0;
	int j = 0;
	int counter = 0;
	char* token = malloc(length(s) * sizeof(char));	// Word length
	char** strPtr = malloc(20 * sizeof(char*));		// Maximum amount of words allowed 
	int iNext = 0;
	int string_length = length(s);
	int token_length = length(token);

	// Allocate space for each pointer in the array of pointers
	for (i = 0; i < 20; i++)
	{
		strPtr[i] = malloc(length(s) * sizeof(char));
	}
	i = 0;

	// Number of tokens
	while (s[i] != '\0')
	{
		if (s[i] == ' ')
			counter++;
		i++;
	}

	i = 0;
	while (iNext < string_length)
	{
		iNext = getSubstring(s, token, iNext);

		for (j = 0; j < length(token); j++)
		{
			strPtr[i][j] = token[j];
		}
		strPtr[i][j] = '\0';
		i++;

	}
	strPtr[i] = '\0';
	args[0] = strPtr;

	return counter + 1;
}

// Returns the index value of the end of the token, i.e. the space
// Also populates ss with the contents of the token, the substring
int getSubstring(char s[], char ss[], int start)
{
	int iStart = start;
	int iEnd;
	int j = 0;

	while (s[iStart] != ' ' && s[iStart] != '\0')
	{
		ss[j] = s[iStart];
		iStart++;
		j++;
	}

	ss[j] = '\0';
	iEnd = iStart - 1;			// Index of the last character of the token

	if (s[iStart] == '\0')		// End of string
		return length(s);

	while (s[iStart] == ' ')	// Gets index for starting point of next token
	{
		iEnd = iStart + 1;
		iStart++;
	}
	return iEnd;
}

// Returns total length of string
int length(char s[])
{
	int i = 0;
	while (s[i] != '\0')
		i++;
	return i;
}


// Forks and execs the command passed in
void doCommand(char*** args)
{
	pid_t pid;
	int i = 0;
	int status = 0;

	if ((pid = fork()) == -1)				// Failed
		printf("Fork failed. . .\n");
	if (pid == 0)							// Child
	{
		if (execvp(*args[0], *args) < 0)	// Command failed
		{
			printf("Command not found: %s\n", *args[i]);
			exit(0);
		}
		else
		{
			// Waiting on child to complete
			wait(NULL);
			return;
		}
	}
	else
	{
		while (wait(&status) != pid)
			;
	}
}