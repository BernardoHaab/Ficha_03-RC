#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

#define DELIMITER " "

static const char *const command[] = {
	[HELP] = "help",
	[DOMAIN] = "domain",
};

void processCommand(const char *const buffer, const size_t bufferSize, char *response)
{
	char *bufferClone = malloc(sizeof(buffer[0]) * bufferSize);
	char *bufferCloneFree = bufferClone;

	strncpy(bufferClone, buffer, bufferSize);

	char *commandString = strtok(bufferClone, DELIMITER);
	char *argumentString = strtok(NULL, "");

#define WRAPPER(enum, text, function) \
	if (strncmp(commandString, text, strlen(text)) == 0) { \
		function(argumentString, response); \
	}
	COMMAND_ENUM
#undef WRAPPER

	free(bufferCloneFree);
}

void commandHelp(const char *const argument, char *response)
{
	(void) argument;

	strcpy(response, "List of available commands:"
#define WRAPPER(enum, text, function) \
			"\n- " text
			COMMAND_ENUM
#undef WRAPPER
			"\n"
	      );
}

void commandDomain(const char *const argument, char *response)
{

}

void commandIp(const char *const argument, char *response)
{

}
