#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define MAX_TOOLS 	 100
#define MAX_LEN_PATH	 512
#define MAX_LEN_TOOLNAME 255

FILE *active_database;
char active_path[MAX_LEN_PATH] = "";

void select_db();
void initialize_db();

void change_active(FILE *new_active, const char *new_path);

typedef struct {
	int id;
	char name[MAX_LEN_TOOLNAME+1];
	unsigned int quantity;
	unsigned int cost; /* in cents. */
} tool_t;
tool_t EMPTY_RECORD = { -1, "", 0, 0 };

void cls(){
	printf("\033[2J");
}

void prompt_str(const char *prompt_string, char *output_string, int max_len);
void chomp(char *str);

int main(void){

	for(;;){
		cls();
		printf("MENU\n");
		printf("\n");
		if(active_database == NULL) {
			printf("  ********************************************\n");
			printf("  * No database is currently active.         *\n");
			printf("  * You will need to initialize or load one. *\n");
			printf("  ********************************************\n");
			printf("\n");
		}
		printf("    I - Initialize a database\n");
		printf("      - Input new records\n");
		printf("      - Search for a record\n");
		printf("      - Delete a record\n");
		printf("      - List all records\n");
		printf("      - Save database\n");
		printf("    L - Load an existing database\n");
		printf("    Q - Exit\n");
		printf("\n");

		char choice[255];
		prompt_str("Choice?", choice, 255);

		switch(*choice){

			case 'L':
			case 'l':
				select_db();
			continue;

			case 'I':
			case 'i':
				initialize_db();
			continue;

			case 'Q':
			case 'q':
			case 'E':
			case 'e':
			case 'X':
			case 'x':
				printf("Bye!\n");
			exit(0);

			default:
				/* try again. */
			continue;
		}
	}

	exit(0); /* with much success. */
}


void select_db() {
	char path[MAX_LEN_PATH];
	prompt_str("Path to DB to make active", path, MAX_LEN_PATH);

	FILE *fp = fopen(path, "r+");

	if (!fp) {
		printf("FATAL: Could not open open file \"%s\".\n", path);
		exit(200);
	}

	change_active(fp, path);
}


void initialize_db(){
	char path[MAX_LEN_PATH];
	printf("** Warning! ** This will overwrite the data in the file you specify.\n");
	prompt_str("Path to DB to initialize", path, MAX_LEN_PATH);

	FILE *fp = fopen(path, "w+");
	if (!fp) {
		printf("FATAL: Could not open open file \"%s\" for writing.\n", path);
		exit(1);
	}

	int i;
	for(i=0; i<MAX_TOOLS; i++){
		if( fwrite(&EMPTY_RECORD, sizeof(tool_t), 1, fp) != 1 ) {
			printf("FATAL: fwrite() failed on \"%s\".\n", path);
			exit(1);
		}
	}

	change_active(fp, path);
}


void change_active(FILE *new_active, const char* new_path){
	if (active_database) {
		if(fclose(active_database) != 0) {
			perror("FATAL: Error while closing previously-active database.\n");
			exit(201);
		}
	}
	active_database = new_active;
	rewind(active_database);

	strncpy(active_path, new_path, MAX_LEN_PATH-1);
	active_path[MAX_LEN_PATH-1] = '\0';
}



void prompt_str(const char *prompt_string, char* output_string, int max_len){
	char *fgets_ret;
	for(;;){
		printf("    %s: ", prompt_string);
                fflush(stdout);

		fgets_ret = fgets(output_string, max_len, stdin);
		if (fgets_ret){
			chomp(output_string);
			if(strlen(output_string) > 0){
				return;
			}
		}
	}
	chomp(output_string);
}

/* If the last character of a string is a newline, remove it from the string. */
void chomp(char *str){
	if (str[strlen(str)-1] == '\n') {
		str[strlen(str)-1] = '\0';
	}
}

