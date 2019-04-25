#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


#define T_size 20 		// Size of hash table
#define F5 65536
#define F6 1048576


// history struct
typedef struct his{
	char inst[80];
	struct his* link;
}history;

// opcode table struct - head
typedef struct table_head{
	int count;
	struct t* link;
}Thead;

// opcode table struct - body
typedef struct t{
	char inst[10];
	char code[3];
	struct t* link;
}T;

history* head;
Thead** table;

// memory address
char dump[F6][3];

// memory
char memory[F6];

// memory - translate to ASCII CODE
char add[F5][6];

// last address about instruction <dump>
int last_add;

// error
void No_Inst_Error();

// initialization function about opcode
void read_opcode();
void make_table(char* code, char* inst, int ret);
int opcodetable_hash(char *s);

// initialization fuction about history
void make_history(char instruction[80]);

// shell function
int shell_help();
int shell_dir();
int shell_history();

// memory function
void dump_makeM();		// reset memory
char hex(int);			// Decimal > Hexa(char)
int dump_int(char*);	// Hexa(char[2]) > Decimal
void dump_printM(int start, int end);	// print memory
int dump_edit(int addr, char value[80]);
int dump_fill(int start, int end, char value[80]);

int find_line(int x);	// Find memory address about memory

// opcode function
int opcode_mnemonic(char* inst);
int opcode_opcodelist();

void free_linkedlist();
