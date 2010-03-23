#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define MAX_TOOLS 	 100
#define MAX_LEN_PATH	 512
#define MAX_LEN_TOOLNAME 255

FILE *active_database;
char active_path[MAX_LEN_PATH] = "";

void select_db();
void initialize_db();
void input_new_records();
void print_db();


typedef struct {
	int id;
	char name[MAX_LEN_TOOLNAME+1];
	int qty;
	int cost; /* in cents. */
} tool_t;
tool_t EMPTY_RECORD = { -1, "", 0, 0 };

void change_active(FILE *new_active, const char *new_path);
void write_tool(tool_t tool);

void cls(){
	printf("\033[2J");
}

void prompt_str(const char *prompt_string, char *output_string, int max_len);
void chomp(char *str);

int main(void){

	for(;;){
		printf("\n");
		printf("MENU\n");
		printf("\n");

		if(active_database == NULL) {
			printf("  ********************************************\n");
			printf("  * No database is currently active.         *\n");
			printf("  * You will need to initialize or load one. *\n");
			printf("  ********************************************\n");
			printf("\n");
		} else {
			printf("  +++++\n");
			printf("  + Database OPEN and ACTIVE!\n");
			printf("  +   DB Path: \"%s\"\n", active_path);
			printf("\n");
		}

		printf("    I - Initialize a database\n");
		printf("    N - Input new records\n");
		printf("      - Search for a record\n");
		printf("      - Delete a record\n");
		printf("    P - List all records\n");
		printf("    C - Close/Save database\n");
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

			case 'N':
			case 'n':
				if (active_database) {
					input_new_records();
				} else {
					printf(" !! No active database.\n");
					printf(" !! Select or initialize one first.\n");
					sleep(3);
				}
			continue;

			case 'P':
			case 'p':
				if (active_database) {
					print_db();
				} else {
					printf(" !! No active database.\n");
					printf(" !! Select or initialize one first.\n");
					sleep(3);
				}
			continue;

			case 'C':
			case 'c':
				if (active_database) {
					change_active(NULL, "");
				} else {
					printf(" !! No active database.\n");
					printf(" !! Select or initialize one first.\n");
					sleep(3);
				}
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


void print_db(){
	rewind(active_database);
	int i;
	tool_t tool;
	printf("\n");
	printf("  PRINTING TOOL DATABASE:\n");
	printf("  =======================:\n");
	printf("\n");
	printf("+------+--------+--------+---------------\n");
	printf("|  ID  |  Qty   |  Cost  | Name   ...\n");
	printf("+======+========+========+===============\n");
	for(i=0; i<MAX_TOOLS; i++){
		if(fread(&tool, sizeof(tool_t), 1, active_database) != 1){
			printf("FATAL: Error reading from database.\n");
			exit(89);
		}
		if(tool.id >= 0 && tool.id < MAX_TOOLS){
			printf("|%6d|%8d|%8d|%s\n", tool.id, tool.qty, tool.cost, tool.name);
		}
	}
	printf("+------+--------+--------+---------------\n");
	printf("\n");
}


void input_new_records() {

	char buf_tool_id[MAX_LEN_TOOLNAME];
	char buf_tool_qty[MAX_LEN_TOOLNAME];
	char buf_tool_cost[MAX_LEN_TOOLNAME];

	printf("\n");
	printf("\n");
	printf("INPUT NEW RECORDS..\n");

	for(;;){
		tool_t tool;

		printf("\nEnter tool record to add/overwrite (Or enter Q for an ID to Quit):\n");
		prompt_str("  ID           ", buf_tool_id,   MAX_LEN_TOOLNAME);
		if ( buf_tool_id[0] == 'Q' || buf_tool_id[0] == 'q' ) break;

		tool.id = atoi(buf_tool_id);
		if (tool.id < 0 || tool.id > MAX_TOOLS-1) {
			printf("  !! Tool id is out of range (must be 0-%d inclusive).\n", MAX_TOOLS-1);
			continue;
		}

		prompt_str("  Name         ", tool.name, MAX_LEN_TOOLNAME);

		prompt_str("  Quantity     ", buf_tool_qty,  MAX_LEN_TOOLNAME);
		tool.qty = atoi(buf_tool_qty);
		if (tool.qty < 0) {
			printf("  !! Tool quantity must be > 0.\n");
			continue;
		}

		prompt_str("  Cost (cents) ", buf_tool_cost, MAX_LEN_TOOLNAME);
		tool.cost = atoi(buf_tool_cost);
		if (tool.cost < 0) {
			printf("  !! Tool cost must be > 0.\n");
			continue;
		}

		printf("Adding Tool ID=%d, \"%s\", Qty=%d, at a cost of %d cents each.\n\n",
			tool.id, tool.name, tool.qty, tool.cost);

		write_tool(tool);
	}

	rewind(active_database);
}	

void write_tool(tool_t tool) {
	if(fseek(active_database, tool.id*sizeof(tool_t), SEEK_SET) != 0){
		printf("FATAL: Error writing tool to active database.\n");
		exit(99);
	}
	if(fwrite(&tool, sizeof(tool_t), 1, active_database) != 1){
		printf("FATAL: Error writing tool to active database.\n");
		exit(98);
	}
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

	if (active_database) rewind(active_database);

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

