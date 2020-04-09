/*	msh.c

	David Elder Waters
	03/25/2020
	CS 240
	Assignment 3
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAXCHARS 100
#define MAXWORDS 100
#define MAX_HISTORY 20

void init();
void history(char** hist, int current);
int localCommand(char** parsedCommands, char** hist, int current_hist);
int checkLocalCommand(char** parsed, char** hist, int current_hist);
int handleSpecialCharacters(char** args, char** argsSimple, char** argsSC, char** argsPiped);
void makeAlias(char** args, char*** aliasList, int counter);
int checkAliasCommand(char** args, char*** aliasList, int count);
void printAlias(char*** aliasList, int count);
void removeAlias(char** args, char*** aliasList, int* count);
void updatePath(char** args, char** argsSimple);

int makearg(char* s, char*** args);
int getSubstring(char s[], char ss[], int start);
int length(char s[]);
void doCommand(char*** args);
void doPipedCommand(char** parsedArgs, char** parsedPipedArgs);
void doAliasCommand(char*** aliasList, int counter);
void freeMemory(char** args, char** argsSimple, char** argsSC, char** argsPiped);


int main(int argc, char* argv[])
{
	char c;
	char** args;
	char* cPtr; 	// How to handle arbitrary size?
	int localCmdFlag;
	int i = 0;
	int j = 0;
	int current_alias = 0;
	int current_word = 0;
	int current_history_num = 0;
	char* hist[MAX_HISTORY];

	int execOption;
	char** argsSimple;
	char** argsSC;
	char** argsPiped;
	char*** aliasList = calloc(20, sizeof(char**));	// Max number of aliases currently is 20
	int alias_counter = 0;

	// Allocate alias pointer array pointers
	for (i = 0; i < 20; i++)
	{
		aliasList[i] = calloc(MAXWORDS, sizeof(char*));
		for (j = 0; j < MAXWORDS; j++)
			aliasList[i][j] = calloc(MAXCHARS, sizeof(char));
	}

	init();
	for (i = 0; i < MAX_HISTORY; i++)
		hist[i] = NULL;
	i = 0;

	while (1)
	{
		// Allocate memory for necessary variables
		cPtr = calloc(MAXCHARS, sizeof(char));

		argsSimple = calloc(MAXWORDS, sizeof(char*));
		argsSC = calloc(MAXWORDS, sizeof(char*));
		argsPiped = calloc(MAXWORDS, sizeof(char*));

		for (i = 0; i < MAXWORDS; i++)
		{
			argsSimple[i] = calloc(MAXCHARS, sizeof(char));
			argsSC[i] = calloc(MAXCHARS, sizeof(char));
			argsPiped[i] = calloc(MAXCHARS, sizeof(char));
		}
		i = 0;

		printf("?: ");
		while ((c = getchar()) != '\n')
		{
			// After this loop is done, cPtr will contain the whole command line string
			cPtr[i] = c;
			//printf("cPtr[%i]: %c\n", i, cPtr[i]);
			i++;
		}
		cPtr[i] = '\0';

		// If this is true, then save the typed command to history
		// Means the user didn't just hit enter in the shell
		if (!(strcmp(cPtr, "") == 0 || strcmp(cPtr, "\n") == 0))
		{
			hist[current_history_num] = strdup(cPtr);
			current_history_num = (current_history_num + 1) % MAX_HISTORY;
		}

		// Now have to parse the string
		argc = makearg(cPtr, &args);
		
		// Handles specifically $PATH passed to echo to print out the full path
		if (strcmp(args[0], "echo") == 0 && strcmp(args[1], "$PATH") == 0)
		{
			printf("%s\n", getenv("PATH"));
			freeMemory(args, argsSimple, argsSC, argsPiped);
			continue;
		}


		// Check for alias
		// 1. If new alias, add that to the alias list
		if (strcmp(args[0], "alias") == 0 && args[1] != NULL)	// They are making a new alias
		{
			if (alias_counter < 20)
			{
				makeAlias(args, aliasList, alias_counter);
				printf("Alias '%s' added\n", aliasList[alias_counter][0]);
				alias_counter++;
				continue;
			}
			else
			{
				printf("Too many aliases, cannot add new alias\n");
				freeMemory(args, argsSimple, argsSC, argsPiped);
				continue;
			}
		}

		// Printing out the aliases
		if (strcmp(args[0], "alias") == 0 && args[1] == NULL)
		{
			printAlias(aliasList, alias_counter);
			freeMemory(args, argsSimple, argsSC, argsPiped);
			continue;
		}

		// Check if tokenized first argument is an alias, if it is -- run it
		if (checkAliasCommand(args, aliasList, alias_counter) == 1)
		{
			freeMemory(args, argsSimple, argsSC, argsPiped);
			continue;
		}

		// Check if the user wants to remove an alias and handle it
		if (strcmp(args[0], "unalias") == 0 && args[1] != NULL)
		{
			removeAlias(args, aliasList, &alias_counter);
			freeMemory(args, argsSimple, argsSC, argsPiped);
			continue;
		}
		else if (strcmp(args[0], "unalias") == 0 && args[1] == NULL)
		{
			printf("Please include the alias to remove. . .\n");
			freeMemory(args, argsSimple, argsSC, argsPiped);
			continue;
		}

		// Handle user wanting to change the environment, specifically the PATH
		if (strcmp(args[0], "export") == 0 && args[1] != NULL)
		{
			// Handle exporting of PATH, if correct syntax and whatnot inside of function
			updatePath(args, argsSimple);
			freeMemory(args, argsSimple, argsSC, argsPiped);
			continue;
		}
		else if (strcmp(args[0], "export") == 0 && args[1] == NULL)
		{
			// Print out all the export variables
			printf("Printing out export variables...\n");
			freeMemory(args, argsSimple, argsSC, argsPiped);
			continue;
		}


		// Handle potential local commands
		localCmdFlag = checkLocalCommand(args, hist, current_history_num);
		if (localCmdFlag == 1)
		{
			freeMemory(args, argsSimple, argsSC, argsPiped);
			continue;
		}

		// Now check args for either a ; or | token and populate appropriate variables
		// 0 = no special characters
		// 1 = semicolon
		// 2 = pipe
		execOption = handleSpecialCharacters(args, argsSimple, argsSC, argsPiped);

		if (execOption == 1)		// Semicolon case
		{
			doCommand(&argsSimple);
			doCommand(&argsSC);
		}
		else if (execOption == 2)	// Pipe case
		{
			doPipedCommand(argsSimple, argsPiped);
		}
		else	// Should only be 0, i.e. a simple command
		{
			// Now fork() with the parsed string commands (?)
			doCommand(&args);
		}

		// Free allocated memory
		freeMemory(args, argsSimple, argsSC, argsPiped);
		
		i = 0;
	}
	return 0;
}

// Prints out the list of the current commands stored in the history (max 20 currently)
void history(char** hist, int current)
{
	int i = current;
	int history_num = 1;

	do
	{
		if (hist[i])
		{
			printf("%4d  %s\n", history_num, hist[i]);
			history_num++;
		}
		i = (i + 1) % MAX_HISTORY;
	} while (i != current);
}

// Updates the PATH variable, if able
// Checks to make sure syntax is correct first, then adds the new extension
// onto the end of the path
void updatePath(char** args, char** argsSimple)
{
	// Grab the current PATH
	char* path = getenv("PATH");
	char* checkPath = calloc(MAXCHARS, sizeof(char));
	char* newPath = calloc(MAXCHARS, sizeof(char));
	int i = 0;
	int j = 0;

	// Note, an example valid syntax for updating the path looks like:
	// RAW: export PATH=$PATH:/usr/local/foo	or, tokenized like the following
	// Tokenized: [0] export	[1] PATH=$PATH:/usr/local/foo
	// Index of =
	while (args[1][i] != '\0' && args[1][i] != '=')
		i++;

	strncpy(checkPath, args[1], i);
	checkPath[i] = '\0';
	
	// Starting syntax for updating the path is correct
	if (strcmp(checkPath, "PATH") == 0)
	{
		// Finds the index of the colon
		while (args[1][i] != '\0' && args[1][i] != ':')
			i++;
		j = i;
		// Finds the end of the string
		while (args[1][j] != '\0')
			j++;
		strncpy(newPath, args[1] + i, j - i);
		setenv("PATH", strcat(path, newPath), 1);
		printf("%s\n", getenv("PATH"));
	}
	else
	{
		printf("MSH only supports PATH updating.\nPlease use bash-compatible syntax to update the PATH\n");
	}
	free(checkPath);
	free(newPath);
}

// This runs when the shell starts and executes from file 'mshrc'
void init()
{
	int i = 0;
	char line[MAXCHARS];
	char** init_args;
	FILE* fp = fopen("mshrc", "r");
	while (fgets(line, MAXCHARS, fp))
	{
		char* pos;
		if ((pos = strchr(line, '\n')) != NULL)
			*pos = '\0';
		makearg(line, &init_args);
		doCommand(&init_args);

		while (init_args[i] != '\0')
		{
			free(init_args[i]);
			i++;
		}
		i = 0;
		free(init_args);

		for (i = 0; i < MAXCHARS; i++)
			line[i] = '\0';
		i = 0;
	}
	fclose(fp);
}

// The user wants to make a new alias. Takes the parsed arguments and
// creates a new alias which is added to the aliasList
void makeAlias(char** args, char*** aliasList, int counter)
{
	int i = 0;
	int k = 0;
	int x = 1;

	// Find the command for the new alias, for example ls in ls='ls -l'
	// This essentially gets the index for the value of the equals sign, '='
	while (args[1][i] != '=')
		i++;

	strncpy(aliasList[counter][0], args[1], i);
	aliasList[counter][0][i] = '\0';

	// Skip over the first ' in the alias command
	i += 2;

	// Grabs the rest of the arguments and puts them in their correct spot in aliasList
	while (args[x] != NULL)
	{
		if (x > 1)
			i = 0;
		k = i;
		while (args[x][i] != '\0' && args[x][i] != '\'')
			i++;
		strncpy(aliasList[counter][x], args[x] + k, i - k);
		aliasList[counter][x][i - k] = '\0';
		x++;	// Number of words
	}
	aliasList[counter][x] = '\0';	// Slight memory leak with these two null bytes, but oh well
}

// Remove a current user-created alias from the list of current aliases, if present
// Simply opens up that slot for use again for a different alias later on
// Reallocates memory at the appropriate place for the appropriate items in the aliasList
void removeAlias(char** args, char*** aliasList, int* count)
{
	int i = 0;
	int j = 0;
	
	// There are no aliases to remove
	if (*count == 0)
	{
		printf("No aliases: unable to remove alias '%s'. . .\n", args[1]);
		return;
	}
	// Try to remove the alias, if present
	else
	{
		for (i = 0; i < *count; i++)	// Go through all the current aliases
		{
			if (strcmp(args[1], aliasList[i][0]) == 0)	// Found the alias to remove
			{
				// Overwrite all the associated data of the alias back to null, aka 0
				printf("Removed alias %s\n", aliasList[i][0]);
				while (aliasList[i][j] != NULL && aliasList[i][j] != 0)
				{
					//aliasList[i][j] = 0;
					free(aliasList[i][j]);
					aliasList[i][j] = calloc(MAXWORDS, sizeof(char*));
					j++;
				}
				*count = *count - 1;
			}
			else
			{
				if (i == *count - 1)
					printf("Alias '%s' does not exist\n", args[1]);
			}
		}
	}
}

// Check and execute alias if one is present
// Returns 1 if an alias command is found and ran, 0 otherwise
int checkAliasCommand(char** args, char*** aliasList, int count)
{
	// Search for alias in aliasList
	// If present, execute everything after the alias 
	// If not present, do nothing
	int i = 0;

	// Check all the aliases
	for (i = 0; i < count; i++)
	{
		// If a match is found, run the command
		if (strcmp(args[0], aliasList[i][0]) == 0)
		{
			doAliasCommand(aliasList, i);
			return 1;
		}
	}
	return 0;
}

// Prints out the aliases
void printAlias(char*** aliasList, int count)
{
	int i = 0;
	int j = 0;
	if (aliasList[0][0] == NULL || aliasList[0][0][0] == 0)
		printf("No aliases. . .\n");
	else
	{
		// Check each alias in list
		for (i = 0; i < count; i++)
		{
			if (aliasList[i][j] == "" || aliasList[i][j] == 0)
				continue;
			else
			{
				// This allows checking all of the words associated with each alias, i.e. looping through j
				while (aliasList[i][j] != 0 && aliasList[i][j] != NULL)
				{
					printf("%s ", aliasList[i][j]);
					j++;
				}
				printf("\n");
				j = 0;
			}
		}
	}
}


// Free the dynamically allocated memory for the passed-in variables
void freeMemory(char** args, char** argsSimple, char** argsSC, char** argsPiped)
{
	int i = 0;
	while (args[i] != '\0')
	{
		free(args[i]);
		i++;
	}
	i = 0;
	while (argsSimple[i] != '\0')
	{
		free(argsSimple[i]);
		i++;
	}
	i = 0;
	while (argsSC[i] != '\0')
	{
		free(argsSC[i]);
		i++;
	}
	i = 0;
	while (argsPiped[i] != '\0')
	{
		free(argsPiped[i]);
		i++;
	}
	free(args);
	free(argsSimple);
	free(argsSC);
	free(argsPiped);
}

// Takes the tokenized (parsed) commands from the user and checks to see
// if the command(s) entered is/are local commands to the shell, such as
// exit or cd... Returns 1 if a loal command executes, 0 otherwise
int localCommand(char** parsedCommands, char** hist, int current_hist)
{
	int i, command, numLocalCommands = 3;
	char* localCommands[3];

	localCommands[0] = "exit";
	localCommands[1] = "cd";
	localCommands[2] = "history";

	for (i = 0; i < numLocalCommands; i++)
	{
		if (strcmp(parsedCommands[0], localCommands[i]) == 0)
		{
			command = i;
		}
	}

	switch (command)
	{
	case 0:		// exit
		printf("Exiting. . .\n");
		exit(0);
	case 1:		// cd
		chdir(parsedCommands[1]);
		return 1;
	case 2:		// history
		history(hist, current_hist);
		return 1;
	default:
		break;
	}
	return 0;
}

// Checks the tokenized arguments for any of the built-in local commands
// In particular, currently checks for 'exit' or 'cd'
int checkLocalCommand(char** parsed, char** hist, int current_hist)
{
	// Will have to add more check cases if want to add more local commands
	int i = 0;
	while (parsed[i] != NULL)
	{
		if (strcmp(parsed[i], "exit") == 0 || strcmp(parsed[i], "cd") == 0 || strcmp(parsed[i], "history") == 0)
			return localCommand(parsed, hist, current_hist);
		i++;
	}
}

// Splits up the parsed arguments into separate halves depending on special characters
// In particular, checks for semicolon ';' or pipes '|'
// Additionally, returns 0, 1, or 2 depending on no special characters found, a 
// semicolon found, or a pipe found respectively
// Note, currently can only handle one case or the other, not both simultaneously
// or multiples of each
int handleSpecialCharacters(char** args, char** argsSimple, char** argsSC, char** argsPiped)
{
	int i = 0;
	int j = 0;
	int SC = -1;
	int Pipe = -1;

	// After this loop index of special character known
	while (args[i] != NULL)
	{
		if (strcmp(args[i], ";") == 0)	// Semicolon found
		{
			SC = i;	// Tracks index of semicolon
			break;
		}
		if (strcmp(args[i], "|") == 0)	// Pipe found
		{
			Pipe = i;	// Tracks index of pipe
			break;
		}
		i++;
	}

	if (SC >= 0)	// There is a semicolon
	{
		i = SC + 1;
		while (args[i] != NULL)		// Populate the part corresponding to after the ';'
		{
			//argsSC[j] = args[i];
			strcpy(argsSC[j], args[i]);
			i++;
			j++;
		}
		argsSC[j] = '\0';

		i = 0;
		while (strcmp(args[i], ";") != 0)	// Populate the part before the ';'
		{
			//argsSimple[i] = args[i];
			strcpy(argsSimple[i], args[i]);
			i++;
		}
		argsSimple[i] = '\0';

		return 1;
	}
	if (Pipe >= 0)	// There is a pipe
	{
		i = Pipe + 1;
		while (args[i] != NULL)		// Populate the part corresponding to after the '|'
		{
			//argsPiped[j] = args[i];
			strcpy(argsPiped[j], args[i]);
			i++;
			j++;
		}
		argsPiped[j] = '\0';

		i = 0;
		while (strcmp(args[i], "|") != 0)	// Populate the part before the '|'
		{
			//argsSimple[i] = args[i];
			strcpy(argsSimple[j], args[i]);
			i++;
		}
		argsSimple[i] = '\0';
		return 2;
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
	char* token = calloc(length(s), sizeof(char));		// Word length
	char** strPtr = calloc(MAXWORDS, sizeof(char*));	// Maximum amount of words allowed 
	int iNext = 0;
	int string_length = length(s);
	int token_length = length(token);

	// Allocate space for each pointer in the array of pointers
	for (i = 0; i < MAXWORDS; i++)
	{
		strPtr[i] = calloc(length(s), sizeof(char));
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

// Forks and execs, but for the alias interpretation
// Works kindof -- will execute, but I am guessing there is some pathing messed up somewhere
void doAliasCommand(char*** aliasList, int counter)
{
	pid_t pid;
	int i = 0;
	int status = 0;
	//char** temp = calloc(MAXWORDS, sizeof(char*));

	if ((pid = fork()) == -1)				// Failed
		printf("Fork failed. . .\n");
	if (pid == 0)							// Child
	{
		if (execvp(aliasList[counter][1], aliasList[counter] + 1) < 0)	// Command failed
		{
			printf("aliasList[%i][%i]: '%s'\n", counter, i, aliasList[counter][i]);
			printf("Command not found: %s\n", *aliasList[counter][i]);
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

// Forks and execs the commands passed in, handling pipe circumstance
void doPipedCommand(char** parsedArgs, char** parsedPipedArgs)
{
	pid_t pid1, pid2;
	int pipefd[2];

	if (pipe(pipefd) < 0)
	{
		printf("Could not initialize pipe\n");
		return;
	}

	if ((pid1 = fork()) == -1)	// Failed
	{
		printf("Fork failed. . .\n");
		return;
	}
	if (pid1 == 0)				// Child
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);

		if (execvp(parsedArgs[0], parsedArgs) < 0)
		{
			printf("Command not found: %s\n", parsedArgs[0]);
			exit(0);
		}
	}
	else							// Parent
	{
		if ((pid2 = fork()) == -1)	// Failed
		{
			printf("Fork failed. . .\n");
			return;
		}
		if (pid2 == 0)				// Second child
		{
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
			if (execvp(parsedPipedArgs[0], parsedPipedArgs) < 0)
			{
				printf("Command not found: %s\n", parsedPipedArgs[0]);
				exit(0);
			}
		}
		else						// Wait on both children to finish
		{
			close(pipefd[1]);
			wait(NULL);
			wait(NULL);
		}
	}
}
