#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>


#define T_size 20 // Size of hash table
#define F5 65536
#define F6 1048576


//////////////////////////////////////////////
// 					struct					//
//////////////////////////////////////////////

// history struct
typedef struct his {
	char inst[80];
	struct his* link;
}history;

// opcode table struct - head
typedef struct table_head {
	int count;
	struct t* link;
}Thead;

// opcode table struct - body
typedef struct t {
	char inst[10];
	char code[3];
	char fm[10];
	struct t* link;
}T;

// object struct
typedef struct o {
	int line;
	char loc[5];
	char stat1[50];
	char stat2[10];
	char stat3[50];
	char stat4[50];
	char code[30];
	struct o* link;
}O;

typedef struct sym{
	char label[30];
	char loc[5];
	struct sym* link;
}symbol;

O* start;
O* base;
char last_loc[5];
history* head;
Thead** table;
symbol* shead;

//////////////////////////////////////////////
// 			global variable					//
//////////////////////////////////////////////

// variable about dump
char dump[F6][3]; 							// memory address
char memory[F6];							// memory
char add[F5][6]; 							// memory - translate to ASCII code
int last_add; 								// last address about instruction (dump)

											// variable about error
int error_line[F5];
int error_count;

//////////////////////////////////////////////
// 				main functions				//
//////////////////////////////////////////////

// initialization fuction about history
void make_history(char instruction[80]);

// shell function
int shell_help();
int shell_dir();
int shell_history();
int shell_type(char *inst);

// memory function
void dump_makeM();							// reset memory
void dump_printM(int start, int end);		// print memory
int dump_edit(int addr, char value[80]);
int dump_fill(int start, int end, char value[80]);

int find_line(int x);						// Find memory address about memory
char hex(int);								// Decimal > Hexa(char)
int dump_int(char*);						// Hexa(char[2]) > Decimal

											// initialization function about opcode
void read_opcode();
void make_table(char* code, char* inst, int ret, char* fm);
int opcodetable_hash(char *s);

// opcode function
char* opcode_mnemonic(char* inst);
int opcode_opcodelist();

// SIC/XE function
int asm_assemble(char* inst);
void make_symbol(char* buf, int line_count);
void make_object();

O* asm_strtok(char* buf);

int find_stat1(char* x, int line);			// return loc by int
char* int_to_obcode3(int x);
char* int_to_obcode5(int x);				// simillar function : int_to_loc function
int is_hexnum(char *x);						// check if x is hexa number
int str_to_int(char* x);
int loc_to_int(char loc[5]);
char* int_to_loc(int x);
int hex_i(char);							// Hexa(char) > Decimal
void boundary_error(char tmp[5], int line);
int find_opcode_loc(char x[10]);
char* change_negative(char* x);

void print_symbol_table();
void make_symbol_table(char*, char*);
 

//////////////////////////////////////////////
//				extra functions				//
//////////////////////////////////////////////

// error
void make_error_list(int line);
void No_Inst_Error();
void free_linkedlist();
