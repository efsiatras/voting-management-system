#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./election.h"
#include "./utils/file.h"

void commands(); // Print commands
void help(); // Print help

void clearScreen(); // Clear screen with newlines

void fInsRecords(electionPtr el, FILE *finput); // Insert records from input file

int main(int argc, char *argv[]) {
	electionPtr el = NULL;

	char *input = NULL;
	char *output = NULL;
	int numofupdates = -1;

	FILE *finput = NULL;
	FILE *foutput = NULL;

	if (argc < 2) { // No arguments

		el = electionInit4();
	}

	else if (argc > 7) { // Too many arguments
		printf("Too many arguments.\nUsage: './runelection -i inputfile -o outfile -n numofupdates'\n");

		return 1;
	}

	else {
		for (int i = 1; i < argc; i += 2) { // Check all flags and parameters
			if (argv[i][0] == '-') { // If argument is flag
				switch (argv[i][1]) { // Check which option it is
					case 'i':
						if (input != NULL) {
							printf("Duplicate flag: '%s'\nUsage: './runelection -i inputfile -o outfile -n numofupdates'\n", argv[i]);
							
							return 1;
						}

						else {
							input = argv[i+1];
							finput = fopen(input, "r");
							if (finput == NULL) {
								printf("Invalid input file: File does not exist.\n");

								return 2;
							}
						}

						break;

					case 'o':
						if (output != NULL) {
							printf("Duplicate flag: '%s'\nUsage: './runelection -i inputfile -o outfile -n numofupdates'\n", argv[i]);

							return 1;
						}

						else {
							output = argv[i+1];
							foutput = fopen(output, "w");
							if (foutput == NULL) {
								printf("Invalid output file: File cannot be opened.\n");

								return 2;
							}
						}

						break;

					case 'n':
						if (numofupdates != -1) {
							printf("Duplicate flag: '%s'\nUsage: './runelection -i inputfile -o outfile -n numofupdates'\n", argv[i]);

							return 1;
						}

						else {
							int num = atoi(argv[i+1]);

							if (num <= 0) {
								printf("Invalid parameter: numofupdates must be greater than or equal to 1.\n");

								return 1;
							}

							numofupdates = num;
						}

						break;

					default:
						printf("Invalid flag: '%s'\nUsage: './runelection -i inputfile -o outfile -n numofupdates'\n", argv[i]);

						return 1;

				}
			}

			else {
				printf("Invalid argument: '%s'\nUsage: './runelection -i inputfile -o outfile -n numofupdates'\n", argv[i]);

				return 1;
			}
		}

		if ((finput != NULL) && (numofupdates != -1)) {
			int predictedNumOfRecords = fCountLines(&finput);
			fseek(finput, 0, SEEK_SET); // Move file pointer to the start

			if (predictedNumOfRecords != 0) { // If input file is not empty

				el = electionInit(predictedNumOfRecords, numofupdates);
			}

			else { // If input file is empty
				if (fclose(finput) == EOF) { // Check fclose failure
					printf("Error closing input file.\n");

					return 3;
				}
				finput = NULL;
				input = NULL;

				el = electionInit3(numofupdates);
			}

		}

		else if ((finput != NULL) && (numofupdates == -1)) {
			int predictedNumOfRecords = fCountLines(&finput);
			fseek(finput, 0, SEEK_SET); // Move file pointer to the start

			if (predictedNumOfRecords != 0) { // If input file is not empty

				el = electionInit2(predictedNumOfRecords);
			}

			else { // If input file is empty
				if (fclose(finput) == EOF) { // Check fclose failure
					printf("Error closing input file.\n");

					return 3;
				}
				finput = NULL;
				input = NULL;

				el = electionInit4();
			}
		}

		else if ((finput == NULL) && (numofupdates != -1)) { // 

			el = electionInit3(numofupdates);
		}

		else { // ((input == NULL) && (numofupdates == -1))

			el = electionInit4();
		}
	}

	if (finput != NULL) {

		fInsRecords(el, finput); // Insert records from input file
	}

	commands();

	char *command = NULL;
	char *parameter = NULL;
	char buf[128];
	fgets(buf, 128, stdin);
	command = strtok(buf, " \n");
	parameter = strtok(NULL, "\n");

	do {
		if (!strcmp(command, "lbf")) {
			if (parameter != NULL) { // If parameter is not empty

				electionLbf(el, parameter);
			}

			else {

				printf("Invalid parameter. Try 'help' for available commands.\n");
			}
		}

		else if (!strcmp(command, "lrb")) {
			if (parameter != NULL) { // If parameter is not empty

				electionLrb(el, parameter);
			}

			else {

				printf("Invalid parameter. Try 'help' for available commands.\n");
			}
		}

		else if (!strcmp(command, "ins")) {
			if (parameter != NULL) { // If parameter is not empty
				char *id, *name, *surname;

				int age;
				char *agechar;

				char *gender;

				int postcode;
				char *postcodechar;

				id = strtok(parameter, " ");
				surname = strtok(NULL, " ");
				name = strtok(NULL, " ");
				agechar = strtok(NULL, " ");
				gender = strtok(NULL, " ");
				postcodechar = strtok(NULL, "\n");

				if ((id == NULL) || (surname == NULL) || (name == NULL) || (agechar == NULL) || (gender == NULL) || (postcodechar == NULL)) { // Check for empty tokens

					printf("Invalid parameters. Try 'help' for available commands.\n");
				}

				else {
					age = atoi(agechar);
					postcode = atoi(postcodechar);
					if (electionIns(el, id, name, surname, age, gender[0], postcode, 1)) { // Check if returns error
						
						printf("	- REC-WITH %s EXISTS\n", id);
					}
				}
			}

			else {

				printf("Invalid parameters. Try 'help' for available commands.\n");
			}

		}

		else if (!strcmp(command, "find")) {
			if (parameter != NULL) { // If parameter is not empty

				electionFind(el, parameter);
			}

			else {

				printf("Invalid parameter. Try 'help' for available commands.\n");
			}
		}

		else if (!strcmp(command, "delete")) {
			if (parameter != NULL) { // If parameter is not empty

				if (electionDelete(el, parameter)) { // Check if returns error

					printf("	- KEY %s NOT-in-structs\n", parameter);
				}
			}

			else {

				printf("Invalid parameter. Try 'help' for available commands.\n");
			}
		}

		else if (!strcmp(command, "vote")) {
			if (parameter != NULL) { // If parameter is not empty

				if (electionVote(el, parameter)) { // Check if returns error

					printf("	- REC-WITH %s NOT-in-structs\n", parameter);
				}
			}

			else {

				printf("Invalid parameter. Try 'help' for available commands.\n");
			}
		}

		else if (!strcmp(command, "load")) {
			if (parameter != NULL) { // If parameter is not empty
				FILE *fvote = fopen(parameter, "r");
				if (fvote == NULL) {
					printf("Invalid vote file: File does not exist.\n");

					return 2;
				}

				char *id;
				char buf[512];

				while (fgets(buf, 512, fvote) != NULL) {
					id = strtok(buf, "\n");

					if (electionVote(el, id)) { // Check if returns error

						printf("	- REC-WITH %s NOT-in-structs\n", id);
					}
				}

				if (fclose(fvote) == EOF) { // Check fclose failure
					printf("Error closing input file.\n");

					return 3;
				}
				fvote = NULL;
			}

			else {

				printf("Invalid parameter. Try 'help' for available commands.\n");
			}
		}

		else if (!strcmp(command, "voted")) {
			if (parameter != NULL) { // If parameter is not empty

				electionVotedpc(el, atoi(parameter));
			}

			else {

				electionVoted(el);
			}
		}

		else if (!strcmp(command, "votedperpc")) {

			electionVotedperpc(el);
		}

		else if ((!strcmp(command, "exit")) || (!strcmp(command, "e"))) {
			if (finput != NULL) {
				if (fclose(finput) == EOF) { // Check fclose failure
					printf("Error closing input file.\n");

					return 3;
				}
				finput = NULL;
				input = NULL;
			}

			if (foutput != NULL) {
				electionFPrint(el, foutput); // Print output file
				if (fclose(foutput) == EOF) { // Check fclose failure
					printf("Error closing input file.\n");

					return 3;
				}
				foutput = NULL;
				output = NULL;
			}

			electionFree(el);
			printf("	- exit program\n");

			break;
		}

		else if (!strcmp(command, "help")) {

			help();
		}

		else {

			printf("Invalid command. Try 'help' for available commands.\n");
		}

		fgets(buf, 128, stdin);
		command = strtok(buf, " \n");
		parameter = strtok(NULL, "\n");
	}
	while (1);

	return 0;
}

void fInsRecords(electionPtr el, FILE *finput) { // Insert records from input file
	char *id, *name, *surname;
	int age;
	char *gender;
	int postcode;
	char buf[512];

	while (fgets(buf, 512, finput) != NULL) {
		id = strtok(buf, " ");
		surname = strtok(NULL, " ");
		name = strtok(NULL, " ");
		age = atoi(strtok(NULL, " "));
		gender = strtok(NULL, " ");
		postcode = atoi(strtok(NULL, "\n"));

		if (electionIns(el, id, name, surname, age, gender[0], postcode, 0)) { // Check if returns error
			
			printf("	- REC-WITH %s EXISTS\n", id);
		}
	}
}

void commands() { // Print commands
	printf("\n");
	printf("COMMANDS\n");
	printf("       List of available commands.\n");
	printf("       • lbf key (lookup bloom-filter)\n");
	printf("       • lrb key (lookup red-black tree)\n");
	printf("       • ins record\n");
	printf("       • find key\n");
	printf("       • delete key\n");
	printf("       • vote key\n");
	printf("       • load fileofkeys (to vote)\n");
	printf("       • voted\n");
	printf("       • voted postcode\n");
	printf("       • votedperpc\n");
	printf("       • help\n");
	printf("       • exit\n\n");
}


void clearScreen() { // Clear screen with newlines
	for (int cs = 0; cs < 100; cs++) { // 100 newlines
		printf("\n");
	}
}

void help() { // Print help
	clearScreen();
	commands();
	printf("AUTHOR\n");
	printf("       Written by Efstathios Siatras - sdi1600152.\n\n");
	printf("SEE ALSO\n");
	printf("       Readme available.\n\n");
}