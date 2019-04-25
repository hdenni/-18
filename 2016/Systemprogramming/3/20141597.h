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

// ESTAB - head
typedef struct estab{
	char control_section[7];
	char addr[5];
	char length[5];
	struct estab_s* eslink;
	struct estab* elink;
}ESTAB;

typedef struct estab_s{
	char symbol_name[7];
	char addr[5];
	struct estab_s* link;
}ESTAB_S;

typedef struct rcode{
	char code[3];
	char name[7];
}Rcode;

	
O* start;
O* base;
char last_loc[5];
history* head;
Thead** table;
symbol* shead;

ESTAB* ehead;								// linked list head about external symbol table
Rcode** prog_rcode;							// array about rcode
int input_num;								// 프로그램 개수
int alloc_rcode;							// r code의 할당 여부 확인

//////////////////////////////////////////////
// 			global variable					//
//////////////////////////////////////////////

// variable about dump
char dump[F6][3]; 							// memory - translate to ASCII code
char memory[F6+1];							// memory
char add[F5][6]; 							// memory address
int last_add; 								// last address about instruction (dump)

											// variable about error
int error_line[F5];
int error_count;	

char progaddr[5];							// progaddr 명령어를 통해 받은 주소값
char csaddr[5];								// external symbol table을 만들 때 주소값을 계산하기 위해 계속 갱신하는 값

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
char* change_negative(char x[]);			// 양수인 hexa 배열을 음수 hexa 배열로 바꿈

void print_symbol_table();					
void make_symbol_table(char*, char*);

// link & loader funtcion
int open_linkfile(char tmp[80]); 			// # of file return
int link_pass1(char *filename, int count);	// pass1 수행
int link_pass2(char *filename, int count);	// pass2 수행
											// count는 몇번째 obj 프로그램인지 확인하는 index. 0부터 시작하며, rcode 배열에서 사용

char* change_negative2(char[]);				// 음수인 hexa 배열을 양수 hexa 배열로 바꿈
void print_ESTAB();							// print external symbol table
void free_ESTAB();							// free linked list about external symbol table
 

//////////////////////////////////////////////
//				extra functions				//
//////////////////////////////////////////////

// error
void make_error_list(int line);
void No_Inst_Error();
void free_linkedlist();
