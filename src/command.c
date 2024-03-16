#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

#define DELIMITER " "
#define DOMAIN_FILE_DELIMITER " "

Command processCommand(
		const char *const buffer,
		const size_t bufferSize,
		char *response
		)
{
	char *bufferClone = malloc(sizeof(buffer[0]) * bufferSize);
	char *bufferCloneFree = bufferClone;

	strncpy(bufferClone, buffer, bufferSize);

	char *commandString = strtok(bufferClone, DELIMITER);
	char *argumentString = strtok(NULL, DELIMITER);

	bool commandFound = false;
	Command command = HELP;
#define WRAPPER(enum, text, usage, function) \
	if (strncmp(commandString, text, strlen(text)) == 0) { \
		commandFound = true; \
		command = enum; \
		function(argumentString, response); \
	}
	COMMAND_ENUM
#undef WRAPPER

	if (!commandFound) {
		strcpy(response, "Command not found. Tip: Type \"help\"\n");
	}

	free(bufferCloneFree);

	return command;
}

void commandHelp(const char *const argument, char *response)
{
	(void) argument;

	strcpy(response, "List of available commands:"
#define WRAPPER(enum, text, usage, function) \
			"\n- " usage
			COMMAND_ENUM
#undef WRAPPER
			"\n"
	      );
}

extern FILE* domainFile;

void commandDomain(const char *const argument, char *response)
{
#define DOMAIN_FOUND \
	"O nome de domínio %s tem associado o endereço IP %s\n"
#define DOMAIN_NOT_FOUND \
	"O nome de domínio %s não tem um endereço IP associado\n"

	if (domainFile == NULL) {
		sprintf(
				response,
				DOMAIN_NOT_FOUND,
				argument
		       );
		return;
	}

	if (argument == NULL || argument[0] == '\0') {
		sprintf(response, "Argument not provided\n");
		return;
	}

	fseek(domainFile, 0, SEEK_SET);

	char *line = NULL;
	size_t length = 0;
	ssize_t nread;
	bool found = false;

	while (!found && (nread = getline(&line, &length, domainFile)) != -1) {
		char *domain = strtok(line, DOMAIN_FILE_DELIMITER);
		char *ip = strtok(NULL, "\n");

		if (strcmp(argument, domain) == 0) {
			found = true;
			sprintf(response, DOMAIN_FOUND, domain, ip);
			continue;
		}
	}

	free(line);

	if (!found) {
		sprintf(
				response,
				DOMAIN_NOT_FOUND,
				argument
		       );
	}
}

// FIX: NOT WORKING
void commandIp(const char *const argument, char *response)
{
#define IP_FOUND \
	"O IP %s tem associado o dominio %s\n"
#define IP_NOT_FOUND \
	"O IP %s não tem um dominio associado\n"

	if (domainFile == NULL) {
		sprintf(
				response,
				IP_NOT_FOUND,
				argument
		       );
	}

	if (argument == NULL || argument[0] == '\0') {
		sprintf(response, "Argument not provided\n");
		return;
	}

	fseek(domainFile, 0, SEEK_SET);

	char *line = NULL;
	size_t length = 0;
	ssize_t nread;
	bool found = false;

	while (!found && (nread = getline(&line, &length, domainFile)) != -1) {
		char *domain = strtok(line, DOMAIN_FILE_DELIMITER);
		char *ip = strtok(NULL, "\n");

		if (strcmp(argument, ip) == 0) {
			found = true;
			sprintf(response, IP_FOUND, ip, domain);
			continue;
		}
	}

	free(line);

	if (!found) {
		sprintf(
				response,
				IP_NOT_FOUND,
				argument
		       );
	}
}

void commandExit(const char *const argument, char *response)
{
	(void) argument;

	strcpy(response, "Até logo!\n");
}
