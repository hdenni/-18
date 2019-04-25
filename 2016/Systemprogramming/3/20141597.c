#include "20141597.h"

int main() {
	char inst[80];
	char inst_tmp[80];
	char inst_str[20];  // string for input instructions
	int a, i;   		// counters
	int f_check, flag;	//flag
	int loader_flag = 0;
						// Initialize
	char dump_tmp[F6][3];
	char memory_tmp[F6+1];

	memory[F6] = '\0';

	last_add = 0;
	read_opcode();
	dump_makeM();
	shead = NULL;
	start = NULL;
	base = NULL;
	alloc_rcode = 0;
	
	for(i=0; i<4; i++)
		progaddr[i] = '0';
	progaddr[4] = '\0';

	while (1) {
		fflush(stdin);

		// Initialize string
		strcpy(inst_str, "\0");
		strcpy(inst, "\0");
		strcpy(inst_tmp, "\0");

		printf("sicsim> ");
		gets(inst_tmp);

		//////////////////////////////////////////////////////////////////////////////////////
		//								check string										//
		//////////////////////////////////////////////////////////////////////////////////////
		i = 0; flag = 0;
		while (1)
		{
			if(inst_tmp[i] == '\0' || inst_tmp[i] == 'n')
				break;	

			if(inst_tmp[i] == ' ' || inst_tmp[i] == '\t')
			{
				i++;
				continue;
			}
			else
			{
				flag=1;
				break;
			}
		}
		if(flag==0) continue;
		f_check = -1;

		// check space in front of instruction
		i=0; a=0;
		while(1)
		{
			if(inst_tmp[i] == ' ' || inst_tmp[i] == '\t')
				i++;
			else break;
		}

		while(1)
		{
			inst[a] = inst_tmp[a+i];
			a++;
			if(inst_tmp[a+i] == '\0')
			{
				inst[a] = '\0';
				break;
			}
		}

		// check space back of instruction
		i=-1; a=0;
		flag=0;

		while(1)
		{
			i++;
			if(inst[i] != ' ' && inst[i] != '\t')
				continue;
			if(inst[i] == ' ' || inst[i] == '\t')
				flag = 1;
			break;
		}

		if(flag==1)
		{
			a=i;
			while(1)
			{
				if(inst[a] == '\n' || inst[a] == '\0')
					break;
				if(inst[a] != ' ' && inst[a] != '\t')
					flag = 2;
				a++;
			}
		}


		// check instruction (ex. opcode, fill, edit...)
		sscanf(inst, "%[^ \t]", inst_str);

		if (flag == 1)
			strcpy(inst, inst_str);
		flag = 0;
		//////////////////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////////////////
		// 								select menu											//
		//////////////////////////////////////////////////////////////////////////////////////

		// 1) Instruction about shell
		if (!strcmp(inst, "q") || !strcmp(inst, "quit"))
		{
			make_history(inst);
			break;
		}
		
		else if(!strcmp(inst, "d") || !strcmp(inst, "dir"))
		{
			f_check = shell_dir();
			if(f_check!=0)
			{
				printf("Error - dir\n");
				continue;
			}
			make_history(inst);
		
		}
		else if (!strcmp(inst, "h") || !strcmp(inst, "help"))
		{
			f_check = shell_help();
			if (f_check != 0)
			{
				printf("Error - help\n");
				continue;
			}
			make_history(inst);
		}

		else if (!strcmp(inst, "hi") || !strcmp(inst, "history"))
		{
			make_history(inst);
			f_check = shell_history();
			if (f_check != 0)
			{
				printf("Error - history\n");
				continue;
			}
		}

		else if (!strcmp(inst_str, "type"))
		{
			char m[10];
			
			strcpy(m, "\0");
			sscanf(inst, "%[^ \t]%s", inst_str, m);

			f_check = shell_type(m);
			if (f_check != 0)
			{
				printf("File is not exist\n");
				continue;
			}
			make_history(inst);

		}

		//////////////////////////////////////////////////////////////////////////////////////
		// 2) Instruction about memory
		else if (!strcmp(inst_str, "du") || !strcmp(inst_str, "dump"))
		{
			int start, end, t_flag = 0;
			char trash = '\0', last_tmp[50];
			start = -1; end = -1;
			last_tmp[0] = '\0';
			sscanf(inst, "%[^ \t]%x%[^1234567890ABCDEF]%x%s", inst_str, &start, inst_tmp, &end, last_tmp);

			flag = 0; a = 0;

			while (1)
			{
				if (last_tmp[a] == '\0' || last_tmp[a] == '\n')
					break;
				if (last_tmp[a] != ' ' && last_tmp[a] != '\t')
				{
					t_flag = 1;
					break;
				}
				a++;
			}
			if (t_flag == 1)
			{
				printf("Error!\n");
				continue;
			}
			a = 0;
			while (1)
			{
				trash = inst_tmp[a];
				a++;
				if (trash == ',')
					flag = 1;
				else if (trash == ' ' || trash == '\t')
					continue;
				else if (trash == '\0')
					break;
				else
				{
					flag = 2;
					break;
				}
			}

			if (start == -1)
				start = last_add;

			if ((end != -1 && start > end) || start < 0)
			{
				printf("Error - start address > end address\n");
				continue;
			}
			if (flag == 2 && end != -1)
			{
				printf("Error(dump) : Boundary Error\n");
				continue;
			}
			else if (flag == 0 && end != -1)
			{
				printf("Error(dump) : You need <,> beside two value\n");
				continue;
			}

			dump_printM(start, end);
			make_history(inst);
		}

		else if (!strcmp(inst_str, "e") || !strcmp(inst_str, "edit"))
		{
			int addr = 0;
			char value[80], trash = '\0';
			sscanf(inst, "%[^ \t]%x%[^1234567890ABCDEF]%s", inst_str, &addr, inst_tmp, value);

			flag = 0;
			a = 0;
			while (1)
			{
				trash = inst_tmp[a];
				a++;
				if (trash == ',')
					flag = 1;
				else if (trash == ' ' || trash == '\t')
					continue;
				else if (trash == '\0')
					break;
				else
				{
					flag = 2;
					break;
				}
			}

			if (flag == 2 || flag == 0)
			{
				printf("Boundary Error!\n");
				continue;
			}
			if (strlen(value)>3)
			{
				printf("Error(edit) : value error!\n");
				continue;
			}
			f_check = dump_edit(addr, value);
			if (f_check != 0)
			{
				printf("Error(edit) : Boundary error!\n");
				continue;
			}
			make_history(inst);
		}

		else if (!strcmp(inst_str, "f") || !strcmp(inst_str, "fill"))
		{
			int start, end;
			int i_flag = 0;
			char trash[2][20], tras;
			char value[80];

			sscanf(inst, "%[^ \t]%x%[^1234567890ABCDEF]%x%[^1234567890ABCDEF]%s", inst_str, &start, trash[0], &end, trash[1], value);

			flag = 0;
			for (i = 0; i < 2; i++)
			{
				a = 0;
				while (1)
				{
					tras = trash[i][a];
					a++;
					if (tras == ',')
						flag = 1;
					else if (tras == ' ' || tras == '\t')
						continue;
					else if (tras == '\0')
						break;
					else
					{
						flag = 2;
						break;
					}
				}
				if (flag == 2 || flag == 0)
				{
					printf("Boundary Error!\n");
					i_flag = 1;
					continue;
				}
				flag = 0;
			}

			if (i_flag == 1)
				continue;

			if (strlen(value)>3)
			{
				printf("Error(fill) : value error!\n");
				continue;
			}
			f_check = dump_fill(start, end, value);
			if (f_check != 0)
			{
				printf("Error(fill) : Boundary error!\n");
				continue;
			}
			make_history(inst);
		}

		else if (!strcmp(inst, "reset"))
		{
			make_history(inst);
			dump_makeM();
		}
		//////////////////////////////////////////////////////////////////////////////////////
		// 3) Instruction about opcode
		else if (!strcmp(inst_str, "opcode"))
		{
			char m[10];
			char result[3];

			sscanf(inst, "%[^ \t]%s", inst_str, m);

			strcpy(result, opcode_mnemonic(m));
			if (result == NULL)
			{
				printf("The opcode is not exist\n");
				continue;
			}
			printf("opcode is %s\n", result);
			make_history(inst);
		}

		else if (!strcmp(inst, "opcodelist"))
		{
			f_check = opcode_opcodelist();
			if (f_check != 0)
			{
				printf("Error - opcodelist\n");
				continue;
			}
			make_history(inst);
		}

		//////////////////////////////////////////////////////////////////////////////////////
		// 4) SIC/XE assembler
		else if (!strcmp(inst_str, "assemble"))
		{
			char m[100];
			int i;
			symbol* swalk1, *swalk2;
			O* pwalk1, *pwalk2;

			if(shead != NULL)
			{
				swalk1 = shead;
				while(swalk1!=NULL)
				{
					swalk2 = swalk1;
					swalk1 = swalk1->link;
					free(swalk2);
				}
			}

			shead = NULL;
			
			error_count = 0;
			for (i = 0; i<F5; i++)
				error_line[i] = -1;

			sscanf(inst, "%[^ \t]%s", inst_str, m);
			f_check = asm_assemble(m);
			if (f_check != 0)
			{
				printf("assemble error!\n");
				continue;
			}
			make_history(inst);

			free(base);

			pwalk1 = start;
			while(pwalk1!=NULL)
			{
				pwalk2= pwalk1;
				pwalk1 = pwalk1->link;
				free(pwalk2);
			}

			start = NULL;
			base = NULL;

		}
		else if(!strcmp(inst, "symbol"))
		{
			print_symbol_table();
			make_history(inst);
		}

		// project 3 추가구현
		else if (!strcmp(inst_str, "progaddr"))
		{
			char m[30]="\0";
			char trash[30]="\0";
			int value_error=0;

			sscanf(inst, "%[^ \t]%s%s", inst_str, m, trash);
			if(strcmp(trash, "\0"))
			{
				printf("input error!\n");
				continue;
			}

			strcpy(progaddr, "\0");

			if(strlen(m) > 4 || strlen(m) == 0)
			{
				printf("address range error!\n");
				continue;
			}

			if(strlen(m) == 1)
			{
				progaddr[0] = '0';
				progaddr[1] = '0';
				progaddr[2] = '0';
				progaddr[3] = m[0];
				progaddr[4] = '\0';
			}
			else if(strlen(m) == 2)
			{
				progaddr[0] = '0';
				progaddr[1] = '0';
				progaddr[2] = m[0];
				progaddr[3] = m[1];
				progaddr[4] = '\0';
			}
			else if(strlen(m) == 3)
			{
				progaddr[0] = '0';
				progaddr[1] = m[0];
				progaddr[2] = m[1];
				progaddr[3] = m[2];
				progaddr[4] = '\0';
			}
			else if(strlen(m) == 4)
				strcpy(progaddr, m);

			for(i=0; i<4; i++)
			{
				if((progaddr[i] < '0') || (progaddr[i] > '9' && progaddr[i] < 'A') || (progaddr[i] > 'Z'))
				{
					printf("address range error!\n");
					value_error=1;
				}
			}
			
			if(value_error==1) continue;

			make_history(inst);
						
		}
		else if(!strcmp(inst_str, "loader"))
		{
			char input_tmp[30];
			char* Fname_link;
			int success_link = 0;
			int input_count;

			// dump메모리에 대한 값을 임시변수에 저장해놓는다.
			// 만약 error발생시 원래의 dump값으로 되돌림
			for(i=0; i<F6; i++)
			{
				strcpy(dump_tmp[i], dump[i]);
				dump_tmp[i][2] = '\0';
			}
			strcpy(memory_tmp, memory);
			memory_tmp[F6] = '\0';


			// ESTAB이 만들어져 있을 경우 free
			if(loader_flag == 1)
				free_ESTAB();
			
			// r code array가 alloc되었을 경우, free
			if(alloc_rcode == 1)
			{
				for(i=0; i<input_num; i++)
					free(prog_rcode[i]);
				free(prog_rcode);
			}

			strcpy(input_tmp, inst);
		
			input_num = open_linkfile(input_tmp);
			
			if(input_num == 0)
			{
				printf("input error!\n");
				continue;
			}
			
			prog_rcode = (Rcode**)malloc(input_num*sizeof(Rcode*));

			// pass 1
			input_count = 0;
			
			strcpy(input_tmp, inst);
			Fname_link = strtok(input_tmp, " \t\n\0");
			
			alloc_rcode = 0;
			while(Fname_link != NULL)
			{
				Fname_link = strtok(NULL, " \t\0\n");
				if(Fname_link == NULL) break;
				success_link = link_pass1(Fname_link, input_count);
				input_count+=1;
				if(success_link == 1) break;
			}
			if(success_link == 1)
			{
				printf("link error! (pass1)\n");
				loader_flag = 1;
				continue;
			}

			
			// pass 2
			input_count=0;
			strcpy(input_tmp, inst);
			Fname_link = strtok(input_tmp, " \t\0\n");

			while(Fname_link != NULL)
			{
				Fname_link = strtok(NULL, " \t\0\n");
				if(Fname_link == NULL) break;
				success_link = link_pass2(Fname_link, input_count);
				input_count+=1;
				if(success_link == 1) break;
			}

			if(success_link == 1)
			{
				printf("link error!(pass2)\n");
				strcpy(memory, memory_tmp);
				memory[F6] = '\0';
				for(i=0; i<F6; i++)
				{
					strcpy(dump[i], dump_tmp[i]);
					dump[i][2] = '\0';
				}
				loader_flag = 1;
				continue;
			}
			
			print_ESTAB();

			loader_flag = 1;
			make_history(inst);		
			
		}


		// error
		else
		{
			No_Inst_Error();
			continue;
		}

		//////////////////////////////////////////////////////////////////////////////////////
	}

	free_linkedlist();

	return 0;
}


void make_history(char instruction[80])
{
	history *pwalk, *temp;

	temp = (history*)malloc(sizeof(history));

	strcpy(temp->inst, instruction);
	temp->link = NULL;

	if (!head)
		head = temp;
	else
	{
		pwalk = head;
		while (pwalk->link != NULL)
			pwalk = pwalk->link;
		pwalk->link = temp;
	}
}

int shell_help()
{
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");

	return 0;
}
int shell_history()
{
	int count = 1;
	history* pwalk = head;

	if (pwalk == NULL)
		return 1;

	while (pwalk != NULL)
	{
		printf("%d\t%s\n", count++, pwalk->inst);
		pwalk = pwalk->link;
	}

	return 0;
}
int shell_dir()
{
	DIR* dp;
	struct dirent *f;
	struct stat buf;
	int count = 0;

	dp = opendir(".");
	if (!dp) return 1;

	while (1) {
		f = readdir(dp);
		lstat(f->d_name, &buf);
		if (!f) break;

		if (!strcmp(f->d_name, ".") || !strcmp(f->d_name, ".."))
			continue;

		if (S_ISDIR(buf.st_mode))
			printf("%20s/\t", f->d_name);
		else
		{
			if (buf.st_mode & S_IXUSR || buf.st_mode & S_IXGRP || buf.st_mode & S_IXOTH)
				printf("%20s*\t", f->d_name);
			else printf("%20s\t", f->d_name);
		}
		count++;

		if (count % 4 == 0) printf("\n");
	}
	printf("\n");

	closedir(dp);
	return 0;
}
int shell_type(char* inst)
{
	FILE* fp;
	char x;

	fp = fopen(inst, "r");
	if (!fp) return 1;

	while (1)
	{
		x = fgetc(fp);
		if(x == EOF) break;

		printf("%c", x);
	}

	fclose(fp);

	return 0;

}

void dump_makeM()
{
	int i;

	for (i = 0; i<F6; i++)
	{
		dump[i][0] = '0';
		dump[i][1] = '0';
		dump[i][2] = '\0';

		memory[i] = '.';
	} // initialize 

	for (i = 0; i<F5; i++)
	{
		add[i][0] = hex((i % 65536) / 4096);
		add[i][1] = hex((i % 4096) / 256);
		add[i][2] = hex((i % 256) / 16);
		add[i][3] = hex(i % 16);
		add[i][4] = '0';
		add[i][5] = '\0';
	}

}
void dump_printM(int start, int end)
{
	int line, lastline;
	int i, j;

	line = find_line(start);

	// dump start
	if (end == -1)
	{
		lastline = line + 9;
		end = lastline * 16 + 15;
	}
	// dump start, end
	else lastline = find_line(end);

	for (i = find_line(start) * 16; i<F6; i++)
	{
		// P : memory address
		if (i % 16 == 0)
			printf("%s ", add[line]);

		// P : memory
		if (i < start || i > end) printf("   ");
		else printf("%s ", dump[i]);

		// P : memory - ASCII CODE
		if (i % 16 == 15)
		{
			printf("; ");
			for (j = i - 15; j<i; j++)
				printf("%c", memory[j]);
			printf("\n");
			line++;
		}

		if (line == lastline + 1)
		{
			last_add = i + 1;
			break;
		}
	}

	// check last address
	if (end < last_add)
		last_add = end + 1;
}
int dump_edit(int addr, char value[80])
{
	int ret = dump_int(value);

	// Boundary Error
	if (addr < 0 || addr > F6)
		return 1;

	strcpy(dump[addr], value);
	if (strlen(dump[addr]) == 1)
	{
		dump[addr][1] = dump[addr][0];
		dump[addr][0] = '0';
	}

	// If memory is under 0x20 or over 0x7E
	if (ret < 32 || ret > 126)
		memory[addr] = '.';

	else memory[addr] = ret;

	return 0;
}
int dump_fill(int start, int end, char value[80])
{
	int i;
	int ret;

	if (start > end) return 1;

	for (i = start; i <= end; i++)
	{
		strcpy(dump[i], value);
		if (strlen(dump[i]) == 1)
		{
			dump[i][1] = dump[i][0];
			dump[i][0] = '0';
		}

		ret = dump_int(dump[i]);

		// If memory is under 0x20 or over 0x7E
		if (ret < 32 || ret > 126)
			memory[i] = '.';

		else memory[i] = ret;
	}
	return 0;

}

int find_line(int x)
{
	int i;

	for (i = 1; i<0xFFFFFFF; i = i * 16)
	{
		if (x < i)
			return x / 16;
	}
	return -1;
}
char hex(int n)
{
	switch (n) {
	case 0: return '0';
	case 1: return '1';
	case 2: return '2';
	case 3: return '3';
	case 4: return '4';
	case 5: return '5';
	case 6: return '6';
	case 7: return '7';
	case 8: return '8';
	case 9: return '9';
	case 10: return 'A';
	case 11: return 'B';
	case 12: return 'C';
	case 13: return 'D';
	case 14: return 'E';
	case 15: return 'F';
	default: return 'X';
	}
}
int hex_i(char x)
{
	switch (x) {
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	case 'A': return 10;
	case 'B': return 11;
	case 'C': return 12;
	case 'D': return 13;
	case 'E': return 14;
	case 'F': return 15;
	default: return -1;
	}
}
int dump_int(char* str)
{
	int len = strlen(str);
	int sum, i, line;

	for (i = 0, sum = 0; i<len; i++)
	{
		line = 0;
		if (str[i] >= 'A' && str[i] <= 'F')
			line += str[i] - 'A' + 10;
		else if (str[i] >= '0' && str[i] <= '9')
			line += str[i] - '0';
		if (i == 0) sum += line * 16;
		else sum += line;
	}

	return sum;

}


void read_opcode()
{
	FILE* fp;
	int ret, i;
	char code[3], inst[10];
	char fm[10];

	fp = fopen("opcode.txt", "r");
	if (!fp) {
		fprintf(stderr, "File open error! (opcode.txt)\n");
		exit(1);
	}

	table = (Thead**)malloc(T_size*sizeof(Thead*));

	for (i = 0; i<T_size; i++) {
		table[i] = (Thead*)malloc(sizeof(Thead));

		table[i]->count = 0;
		table[i]->link = NULL;
	}

	while (!feof(fp))
	{
		fscanf(fp, "%s %s %s", code, inst, fm);
		ret = opcodetable_hash(code);

		make_table(code, inst, ret, fm);
	}

	fclose(fp);
}
void make_table(char* code, char* inst, int ret, char* fm) {
	T *tmp, *twalk;

	tmp = (T*)malloc(sizeof(T));
	strcpy(tmp->code, code);
	strcpy(tmp->inst, inst);
	strcpy(tmp->fm, fm);
	tmp->link = NULL;

	if (table[ret]->count == 0)
		table[ret]->link = tmp;
	else {
		twalk = table[ret]->link;
		while (twalk->link != NULL)
			twalk = twalk->link;
		twalk->link = tmp;
	}

	table[ret]->count += 1;

}
int opcodetable_hash(char *s) {
	int i, sum;

	int len = strlen(s);
	for (i = 0, sum = 0; i<len; i++)
		sum += s[i];
	return sum % 20;
}

char* opcode_mnemonic(char* inst)
{
	int i;
	T* twalk;

	for (i = 0; i<T_size; i++) {
		twalk = table[i]->link;

		while (twalk != NULL) {
			if (!strcmp(twalk->inst, inst))
			{
				return twalk->code;
			}
			twalk = twalk->link;
		}
	}

	return "\0";
}
int opcode_opcodelist()
{
	int i;
	T* twalk;

	if (!table)
		return 1;

	for (i = 0; i<T_size; i++)
	{
		printf("%d : ", i);
		twalk = table[i]->link;
		while (twalk != NULL)
		{
			printf("[%s, %s]", twalk->inst, twalk->code);
			if (twalk->link != NULL) printf(" -> ");
			twalk = twalk->link;
		}
		printf("\n");
	}
	return 0;
}


int asm_assemble(char* inst)
{
	FILE *fp, *obj, *lst;
	char str[110];

	int len;
	
	int line_count, i;
	O* pwalk;

	int obj_len=0;
	char obj_array[100];

	int start_iadd;
	char start_add[5];


	error_count = 0;

	// Open file
	fp = fopen(inst, "r");
	if (!fp) return 1;

	error_line[0] = 0; 

	// pass 1
	line_count = 5;
	while (!feof(fp))
	{
		fgets(str, 100, fp);
				
		if (str[0] == '.' || str[0] == '\n' || str[0] == '\0') continue;
		make_symbol(str, line_count);
		line_count += 5;
	}

	fclose(fp);
	
	if (error_count!=0)
	{
		for (i = 0; i<error_count; i++)
			printf("%d\n", error_line[i]);
		return 1;
	}
	
	// pass 2
	make_object();

	if (error_count != 0)
	{
		for (i = 0; i < error_count; i++)
			printf("%d\n", error_line[i]);
		return 1;
	}
	
	// make file - object, list
	sscanf(inst, "%[^.]", str);
	len = strlen(str);

	str[len] = '.';
	str[len+1] = 'l';
	str[len+2] = 's';
	str[len+3] = 't';
	str[len+4] = '\0';

	lst = fopen(str, "w");
	if(!lst) return 1;

	// make list file
	pwalk = start;
	while(pwalk!=NULL)
	{
		fprintf(lst, "%3d\t", pwalk->line);
		if(!strcmp(pwalk->stat2, "END"))
		{
			strcpy(last_loc, pwalk->loc);
			strcpy(pwalk->loc, "\0");
		}
		fprintf(lst, "%s\t", pwalk->loc);
		fprintf(lst, "%s\t", pwalk->stat1);
		fprintf(lst, "%s\t", pwalk->stat2);
		fprintf(lst, "%s", pwalk->stat3);

		if(strcmp(pwalk->stat4, "\0"))
		{
			if(!strcmp(pwalk->stat2, "BYTE"))
				fprintf(lst, "'%s'\t%s\n", pwalk->stat4, pwalk->code);
			else fprintf(lst, ", %s\t%s\n", pwalk->stat4, pwalk->code);
		}
		else fprintf(lst, "\t%s\t\n", pwalk->code);
		pwalk=pwalk->link;
	}

	fclose(lst);

	printf("\toutput file : [%s], ", str);

	str[len+1] = 'o';
	str[len+2] = 'b';
	str[len+3] = 'j';

	obj = fopen(str, "w");
	if(!obj) return 1;

	strcpy(obj_array, "\0");
	// make object file
	pwalk = start;
	while(pwalk!=NULL)
	{
		if(!strcmp(pwalk->stat2, "START"))
		{
			int a = loc_to_int(pwalk->loc);
			int b = loc_to_int(last_loc);
			char result_loc[5];
			strcpy(result_loc, int_to_loc(b-a));

			fprintf(obj, "H%s\t", pwalk->stat1);
			fprintf(obj, "00%s", pwalk->loc);
			fprintf(obj, "00%s\n", result_loc);
		}
		else
		{
			
			if(!strcmp(pwalk->stat2, "END"))
			{
				int length[2];
				length[0] = obj_len/16;
				length[1] = obj_len%16;

				fprintf(obj, "%X%X", length[0], length[1]);
				fprintf(obj, "%s\n", obj_array);

				obj_len = 0;
				strcpy(obj_array, "\0");
			}
			else if(!strcmp(pwalk->stat2, "RESW") || !strcmp(pwalk->stat2, "RESB"))
			{
				int length[2];
				length[0] = obj_len/16;
				length[1] = obj_len%16;

				if(obj_len == 0)
				{
					pwalk=pwalk->link;
					continue;
				}

				fprintf(obj, "%X%X", length[0], length[1]);
				fprintf(obj, "%s\n", obj_array);

				obj_len = 0;
				strcpy(obj_array, "\0");
			}
			else if(obj_len == 0)
			{
				fprintf(obj, "T00%s", pwalk->loc);
				strcpy(obj_array, pwalk->code);
				obj_len += strlen(pwalk->code)/2;
			}
			else if(obj_len > 0 && obj_len + strlen(pwalk->code)/2 <= 30)
			{
				strcat(obj_array, pwalk->code);
				obj_len += strlen(pwalk->code)/2;
			}
			else if(obj_len + strlen(pwalk->code)/2 > 30)
			{
				int length[2];
				length[0] = obj_len/16;
				length[1] = obj_len%16;

				fprintf(obj, "%X%X", length[0], length[1]);
				fprintf(obj, "%s\n", obj_array);

				obj_len = 0;

				strcpy(obj_array, "\0");
				strcpy(obj_array, pwalk->code);
				fprintf(obj, "T00%s", pwalk->loc);
				obj_len += strlen(pwalk->code)/2;
			}
		}
		pwalk = pwalk->link;
	}

	pwalk = start;
	while(pwalk!=NULL)
	{
		if((pwalk->stat3[0] == '#') && (pwalk->stat3[1] >= '0' && pwalk->stat3[1] <= '9'))
		{
			pwalk=pwalk->link;
			continue;
		}
		if(pwalk->stat2[0] == '+')
		{
			char tmploc[5];
			int tmpint;

			tmpint = loc_to_int(pwalk->loc) + 1;
			strcpy(tmploc, int_to_loc(tmpint));

			fprintf(obj, "M00%s05\n", tmploc);
		}
		pwalk = pwalk->link;
	}

	start_iadd = str_to_int(start->stat3);
	strcpy(start_add, int_to_loc(start_iadd));

	fprintf(obj, "E00%s\n", start_add);

	fclose(obj);

	printf("[%s]\n", str);
	return 0;

}
void make_symbol(char* buf, int line_count)
{
	O *tmp, *pwalk;
	char tmpbuf[100];
	int len;

	strcpy(tmpbuf, buf);
	tmp = asm_strtok(buf);
	tmp->line = line_count;

	if (!start)
	{
		if (!strcmp(tmp->stat2, "START"))
		{
			len = strlen(tmp->stat3);
			if (len == 0)
				make_error_list(line_count);
			if (len == 1)
			{
				boundary_error(tmp->stat3, line_count);
				tmp->loc[0] = '0';
				tmp->loc[1] = '0';
				tmp->loc[2] = '0';
				tmp->loc[3] = tmp->stat3[0];
				tmp->loc[4] = '\0';
			}
			else if (len == 2)
			{
				boundary_error(tmp->stat3, line_count);
				tmp->loc[0] = '0';
				tmp->loc[1] = '0';
				tmp->loc[2] = tmp->stat3[0];
				tmp->loc[3] = tmp->stat3[1];
				tmp->loc[4] = '\0';
			}
			else if (len == 3)
			{
				boundary_error(tmp->stat3, line_count);
				tmp->loc[0] = '0';
				tmp->loc[1] = tmp->stat3[0];
				tmp->loc[2] = tmp->stat3[1];
				tmp->loc[3] = tmp->stat3[2];
				tmp->loc[4] = '\0';
			}
			else if (len == 4)
			{
				boundary_error(tmp->stat3, line_count);
				strcpy(tmp->loc, tmp->stat3);
			}
			else make_error_list(line_count);
			start = tmp;
		}
		else
		{
			make_error_list(line_count);
			start = tmp;
		}
	}
	else
	{
		pwalk = start;
		while (pwalk->link != NULL)
			pwalk = pwalk->link;
		pwalk->link = tmp;

		if (!strcmp(tmp->stat2, "BASE"))
			strcpy(tmp->loc, pwalk->loc); 
		else if (!strcmp(pwalk->stat2, "END"))
			strcpy(tmp->loc, "\0");
		else if (!strcmp(pwalk->stat2, "START"))
			strcpy(tmp->loc, pwalk->loc);
		else if (pwalk->stat2[0] == '+')
		{
			int ploc = loc_to_int(pwalk->loc);
			int tloc = ploc + 4;
			strcpy(tmp->loc, int_to_loc(tloc));
		}
		else if (!strcmp(pwalk->stat2, "RESW"))
		{
			int ploc = loc_to_int(pwalk->loc);
			int tloc;
			char tmploc[5];

			if (strlen(pwalk->stat3) > 4) make_error_list(line_count);
			strcpy(tmploc, pwalk->stat3);
			tmploc[4] = '\0';
			boundary_error(tmploc, line_count);

			tloc = ploc + str_to_int(tmploc) * 3;
			strcpy(tmp->loc, int_to_loc(tloc));
		}
		else if (!strcmp(pwalk->stat2, "RESB"))
		{
			int ploc = loc_to_int(pwalk->loc);
			int tloc;
			char tmploc[5];

			if (strlen(pwalk->stat3) > 4) make_error_list(line_count);
			strcpy(tmploc, pwalk->stat3);

			tloc = ploc + str_to_int(tmploc);
			strcpy(tmp->loc, int_to_loc(tloc));
		}
		else if (!strcmp(pwalk->stat2, "BASE"))
		{
			int ploc = loc_to_int(pwalk->loc);
			int tloc = ploc + 3;
			strcpy(tmp->loc, int_to_loc(tloc));
		}
		else if (!strcmp(pwalk->stat2, "BYTE"))
		{
			int ploc = loc_to_int(pwalk->loc);
			int tloc;

			if (pwalk->stat3[0] == 'X')
			{
				if (strlen(pwalk->stat4) % 2 != 0 || strlen(pwalk->stat4) == 0) make_error_list(line_count);
				if (is_hexnum(pwalk->stat4) != 0) make_error_list(line_count);

				tloc = ploc + strlen(pwalk->stat4) / 2;
			}
			else if (pwalk->stat3[0] == 'C') tloc = ploc + strlen(pwalk->stat4);
			else
			{
				make_error_list(line_count);
				return;
			}
			strcpy(tmp->loc, int_to_loc(tloc));
		}
		else
		{
			int ploc = loc_to_int(pwalk->loc);
			int tloc = ploc + find_opcode_loc(pwalk->stat2);
			strcpy(tmp->loc, int_to_loc(tloc));
		}

	}

}
void make_object()
{
	O* pwalk = start;	// temp for object code
	O* swalk;			// temp for find stat1 
	int hexcode[2], i;
	int line = 0;

	while (pwalk != NULL)
	{
		line += 5;
		for (i = 0; i<2; i++)
			hexcode[i] = 0;

		if (!strcmp(pwalk->stat2, "START"))
			strcpy(pwalk->code, "\0");
		else if (!strcmp(pwalk->stat2, "RESW") || !strcmp(pwalk->stat2, "RESB"))
			strcpy(pwalk->code, "\0");
		else if (!strcmp(pwalk->stat2, "BASE"))
		{
			char tmploc[5];
			int tmpint;

			strcpy(pwalk->code, "\0");
			base = (O*)malloc(sizeof(O));
			base->line = pwalk->line;
			strcpy(base->stat1, pwalk->stat1);
			strcpy(base->stat2, pwalk->stat2);
			strcpy(base->stat3, pwalk->stat3);
			strcpy(base->stat4, pwalk->stat4);
			
			tmpint = find_stat1(pwalk->stat3, line);
			strcpy(tmploc, int_to_loc(tmpint));
			strcpy(base->loc, tmploc);

			strcpy(base->code, "\0");

			base->link = NULL;
		}
		else if (!strcmp(pwalk->stat2, "END"))
			strcpy(pwalk->code, "\0");
		else if (!strcmp(pwalk->stat3, "\0"))
		{
			char tmpcode[3];
			
			if (pwalk->stat2[0] != '+')
			{
				strcpy(tmpcode, opcode_mnemonic(pwalk->stat2));
				hexcode[0] = hex_i(tmpcode[0]);
				hexcode[1] = hex_i(tmpcode[1]) + 3;
				pwalk->code[0] = hex(hexcode[0]);
				pwalk->code[1] = hex(hexcode[1]);
				pwalk->code[2] = '0';
				pwalk->code[3] = '0';
				pwalk->code[4] = '0';
				pwalk->code[5] = '0';
				pwalk->code[6] = '\0';
			}
			else
			{
				char tmpstat2[10];
				sscanf(pwalk->stat2, "+%s", tmpstat2);
				strcpy(tmpcode, opcode_mnemonic(tmpstat2));

				hexcode[0] = hex_i(tmpcode[0]);
				hexcode[1] = hex_i(tmpcode[1]) + 3;
				pwalk->code[0] = hex(hexcode[0]);
				pwalk->code[1] = hex(hexcode[1]);
				pwalk->code[2] = '0';
				pwalk->code[3] = '0';
				pwalk->code[4] = '0';
				pwalk->code[5] = '0';
				pwalk->code[6] = '\0';
			}
		}
		else if (!strcmp(pwalk->stat2, "BYTE"))
		{
			if (pwalk->stat3[0] == 'X')
				strcpy(pwalk->code, pwalk->stat4);
			if (pwalk->stat3[0] == 'C')
			{
				int x;
				int len = strlen(pwalk->stat4);

				for (i = 0; i < len; i++)
				{
					x = pwalk->stat4[i];

					pwalk->code[i * 2] = hex(x / 16);
					pwalk->code[i * 2 + 1] = hex(x % 16);
				}
				
			}
		}
		else if (find_opcode_loc(pwalk->stat2) == 1)
			strcpy(pwalk->code, opcode_mnemonic(pwalk->stat2));
		else if (find_opcode_loc(pwalk->stat2) == 2)
		{
			char tmpcode[3];
			strcpy(tmpcode, opcode_mnemonic(pwalk->stat2));

			pwalk->code[0] = tmpcode[0];
			pwalk->code[1] = tmpcode[1];

			if (!strcmp(pwalk->stat3, "A")) pwalk->code[2] = '0';
			else if (!strcmp(pwalk->stat3, "X")) pwalk->code[2] = '1';
			else if (!strcmp(pwalk->stat3, "L")) pwalk->code[2] = '2';
			else if (!strcmp(pwalk->stat3, "PC")) pwalk->code[2] = '8';
			else if (!strcmp(pwalk->stat3, "SW")) pwalk->code[2] = '9';
			else if (!strcmp(pwalk->stat3, "B")) pwalk->code[2] = '3';
			else if (!strcmp(pwalk->stat3, "S")) pwalk->code[2] = '4';
			else if (!strcmp(pwalk->stat3, "T")) pwalk->code[2] = '5';
			else if (!strcmp(pwalk->stat3, "F")) pwalk->code[2] = '6';
			else make_error_list(line);

			if (!strcmp(pwalk->stat4, "\0")) pwalk->code[3] = '0';
			else if (!strcmp(pwalk->stat4, "A")) pwalk->code[3] = '0';
			else if (!strcmp(pwalk->stat4, "X")) pwalk->code[3] = '1';
			else if (!strcmp(pwalk->stat4, "L")) pwalk->code[3] = '2';
			else if (!strcmp(pwalk->stat4, "PC")) pwalk->code[3] = '8';
			else if (!strcmp(pwalk->stat4, "SW")) pwalk->code[3] = '9';
			else if (!strcmp(pwalk->stat4, "B")) pwalk->code[3] = '3';
			else if (!strcmp(pwalk->stat4, "S")) pwalk->code[3] = '4';
			else if (!strcmp(pwalk->stat4, "T")) pwalk->code[3] = '5';
			else if (!strcmp(pwalk->stat4, "F")) pwalk->code[3] = '6';
			else make_error_list(line);

			pwalk->code[4] = '\0';
		}
		else
		{
			char tmpcode[3];
			char tmpstat2[10], trash;

			if (pwalk->stat2[0] >= 'A' && pwalk->stat2[0] <= 'Z')
				strcpy(tmpstat2, pwalk->stat2);
			else sscanf(pwalk->stat2, "%c%s", &trash, tmpstat2);

			strcpy(tmpcode, opcode_mnemonic(tmpstat2));

			if (!strcmp(tmpcode, "\0"))
			{
				strcpy(pwalk->code, "------\0");
				make_error_list(line);
				continue;
			}
			hexcode[0] = hex_i(tmpcode[0]);
			hexcode[1] = hex_i(tmpcode[1]);

			if (pwalk->stat3[0] == '#')
			{
				hexcode[1] += 1;
				if (pwalk->stat3[1] >= '0' && pwalk->stat3[1] <= '9') // LDA #0
				{
					if (strlen(pwalk->stat3) > 5)
					{
						strcpy(pwalk->code, "------\0");
						make_error_list(line);
						continue;
					}
					else
					{
						if (pwalk->stat2[0] != '+') // LDA #0
						{
							int op;
							char tmp[4];

							sscanf(pwalk->stat3, "#%d", &op);
							if (op > 4095)
							{
								strcpy(pwalk->code, "------\0");
								make_error_list(line);
								continue;
							}

							strcpy(tmp, int_to_obcode3(op));

							pwalk->code[0] = hex(hexcode[0]);
							pwalk->code[1] = hex(hexcode[1]);
							if (!strcmp(pwalk->stat4, "X")) pwalk->code[2] = '8';
							else pwalk->code[2] = '0';
							pwalk->code[3] = tmp[0];
							pwalk->code[4] = tmp[1];
							pwalk->code[5] = tmp[2];
							pwalk->code[6] = '\0';
						}
						else					// +LDA #0
						{
							int op;
							char tmp[6];

							sscanf(pwalk->stat3, "#%d", &op);
							if (op > F5 - 1)
							{
								strcpy(pwalk->code, "--------\0");
								make_error_list(line);
								continue;
							}

							strcpy(tmp, int_to_obcode5(op));

							pwalk->code[0] = hex(hexcode[0]);
							pwalk->code[1] = hex(hexcode[1]);
							if (!strcmp(pwalk->stat4, "X")) pwalk->code[2] = '9';
							else pwalk->code[2] = '1';
							pwalk->code[3] = tmp[0];
							pwalk->code[4] = tmp[1];
							pwalk->code[5] = tmp[2];
							pwalk->code[6] = tmp[3];
							pwalk->code[7] = tmp[4];
							pwalk->code[8] = '\0';

						}
					}
				}
				else // LDA #LENGTH
				{
					char tmpstat3[30];
					char tmp[4];
					int a, b;

					sscanf(pwalk->stat3, "#%s", tmpstat3);
					swalk = pwalk;
					swalk = swalk->link;
					if (!strcmp(swalk->stat2, "BASE")) swalk = swalk->link;
					a = loc_to_int(swalk->loc);
					b = find_stat1(tmpstat3, line);

					if (pwalk->stat2[0] != '+')
					{
						if (b - a < 0)
						{
							strcpy(tmp, int_to_obcode3(a - b));
							strcpy(tmp, change_negative(tmp));
						}
						else strcpy(tmp, int_to_obcode3(b - a));

						if (b - a >= -2048 && b - a < 2048) // pc
						{
							pwalk->code[0] = hex(hexcode[0]);
							pwalk->code[1] = hex(hexcode[1]);
							if (!strcmp(pwalk->stat4, "X")) pwalk->code[2] = 'A';
							else pwalk->code[2] = '2';
							pwalk->code[3] = tmp[0];
							pwalk->code[4] = tmp[1];
							pwalk->code[5] = tmp[2];
							pwalk->code[6] = '\0';
						}
						else if (b - a >= 2048 && b - a <= 4095) //base count
						{
							int loc1 = loc_to_int(base->loc);
							int loc2;

							char inputcode[30];
							char tmp[4];

							sscanf(pwalk->stat3, "#%s", inputcode);
							loc2 = find_opcode_loc(inputcode);

							if (loc2 - loc1 < 0)
							{
								strcpy(tmp, int_to_obcode3(loc1 - loc2));
								strcpy(tmp, change_negative(tmp));
							}
							else strcpy(tmp, int_to_obcode3(loc2 - loc1));

							pwalk->code[0] = hex(hexcode[0]);
							pwalk->code[1] = hex(hexcode[1]);
							if (!strcmp(pwalk->stat4, "X")) pwalk->code[2] = 'C';
							else pwalk->code[2] = '4';
							pwalk->code[3] = '0';
							pwalk->code[4] = tmp[0];
							pwalk->code[5] = tmp[1];
							pwalk->code[6] = tmp[2];
							pwalk->code[7] = '\0';

						}
					}
					else // +LDA #LENGTH
					{
						char tmploc[5], tmpstat3[30];
						int tmpint;
						sscanf(pwalk->stat3, "#%s", tmpstat3);
						tmpint = find_stat1(tmpstat3, line);
						strcpy(tmploc, int_to_loc(tmpint));

						pwalk->code[0] = hex(hexcode[0]);
						pwalk->code[1] = hex(hexcode[1]);
						if (!strcmp(pwalk->stat4, "X")) pwalk->code[2] = '9';
						else pwalk->code[2] = '1';
						pwalk->code[3] = tmploc[0];
						pwalk->code[4] = tmploc[1];
						pwalk->code[5] = tmploc[2];
						pwalk->code[6] = tmploc[3];
						pwalk->code[7] = '\0';
					}
				}
				
			}
			else if (pwalk->stat3[0] == '@') // J @RETADR
			{
				char tmpstat3[30];
				char tmp[4];
				int a, b;

				sscanf(pwalk->stat3, "@%s", tmpstat3);
				swalk = pwalk;
				swalk = swalk->link;
				if (!strcmp(swalk->stat2, "BASE")) swalk = swalk->link;
				a = loc_to_int(swalk->loc);
				b = find_stat1(tmpstat3, line);

				if (pwalk->stat2[0] != '+')
				{
					if (b - a < 0)
					{
						strcpy(tmp, int_to_obcode3(a - b));
						strcpy(tmp, change_negative(tmp));
					}
					else strcpy(tmp, int_to_obcode3(b - a));

					if (b - a >= -2048 && b - a < 2048) // pc
					{
						pwalk->code[0] = hex(hexcode[0]);
						pwalk->code[1] = hex(hexcode[1]);
						pwalk->code[2] = '2';
						pwalk->code[3] = tmp[0];
						pwalk->code[4] = tmp[1];
						pwalk->code[5] = tmp[2];
						pwalk->code[6] = '\0';
					}
					else if (b - a >= 2048 && b - a <= 4095) //base count
					{
						int loc1 = find_opcode_loc(base->stat3);
						int loc2;

						char inputcode[30];
						char tmp[4];

						sscanf(pwalk->stat3, "@%s", inputcode);
						loc2 = find_opcode_loc(inputcode);

						if (loc2 - loc1 < 0)
						{
							strcpy(tmp, int_to_obcode3(loc1 - loc2));
							strcpy(tmp, change_negative(tmp));
						}
						else strcpy(tmp, int_to_obcode3(loc2 - loc1));

						pwalk->code[0] = hex(hexcode[0]);
						pwalk->code[1] = hex(hexcode[1]);
						pwalk->code[2] = '4';
						pwalk->code[3] = tmp[0];
						pwalk->code[4] = tmp[1];
						pwalk->code[5] = tmp[2];
						pwalk->code[6] = '\0';

					}
				}
				else // +LDA @LENGTH
				{
					char tmploc[5], tmpstat3[30];
					int tmpint;
					sscanf(pwalk->stat3, "@%s", tmpstat3);
					tmpint = find_stat1(tmpstat3, line);
					strcpy(tmploc, int_to_loc(tmpint));

					pwalk->code[0] = hex(hexcode[0]);
					pwalk->code[1] = hex(hexcode[1]);
					if (!strcmp(pwalk->stat4, "X")) pwalk->code[2] = '9';
					else pwalk->code[2] = '1';
					pwalk->code[3] = '0';
					pwalk->code[4] = tmploc[0];
					pwalk->code[5] = tmploc[1];
					pwalk->code[6] = tmploc[2];
					pwalk->code[7] = tmploc[3];
					pwalk->code[8] = '\0';
				}
			}
			else //simple addressing
			{
				char tmp[4];
				int a, b;
			
				hexcode[1] += 3;				
				swalk = pwalk;
				swalk = swalk->link;
				if (!strcmp(swalk->stat2, "BASE")) swalk = swalk->link;
				a = loc_to_int(swalk->loc);
				b = find_stat1(pwalk->stat3, line);

				if (pwalk->stat2[0] != '+')
				{
					if (b - a < 0)
					{
						strcpy(tmp, int_to_obcode3(a - b));
						strcpy(tmp, change_negative(tmp));
					}
					else strcpy(tmp, int_to_obcode3(b - a));

					if (b - a >= -2048 && b - a < 2048) // pc
					{
						pwalk->code[0] = hex(hexcode[0]);
						pwalk->code[1] = hex(hexcode[1]);
						if (!strcmp(pwalk->stat4, "X")) pwalk->code[2] = 'A';
						else pwalk->code[2] = '2';
						pwalk->code[3] = tmp[0];
						pwalk->code[4] = tmp[1];
						pwalk->code[5] = tmp[2];
						pwalk->code[6] = '\0';
					}
					else //base count
					{
						int loc1 = loc_to_int(base->loc);
						int loc2;

						char tmp[4];

						loc2 = find_stat1(pwalk->stat3, line);

						if (loc2 - loc1 < 0)
						{
							strcpy(tmp, int_to_obcode3(loc1 - loc2));
							strcpy(tmp, change_negative(tmp));
						}
						else strcpy(tmp, int_to_obcode3(loc2 - loc1));

						pwalk->code[0] = hex(hexcode[0]);
						pwalk->code[1] = hex(hexcode[1]);
						if (!strcmp(pwalk->stat4, "X")) pwalk->code[2] = 'C';
						else pwalk->code[2] = '4';
						pwalk->code[3] = tmp[0];
						pwalk->code[4] = tmp[1];
						pwalk->code[5] = tmp[2];
						pwalk->code[6] = '\0';

					}
				}
				else  //+
				{
					char tmploc[5];
					int tmpint = find_stat1(pwalk->stat3, line);
					strcpy(tmploc, int_to_loc(tmpint));

					pwalk->code[0] = hex(hexcode[0]);
					pwalk->code[1] = hex(hexcode[1]);
					if (!strcmp(pwalk->stat4, "X")) pwalk->code[2] = '9';
					else pwalk->code[2] = '1';
					pwalk->code[3] = '0';
					pwalk->code[4] = tmploc[0];
					pwalk->code[5] = tmploc[1];
					pwalk->code[6] = tmploc[2];
					pwalk->code[7] = tmploc[3];
					pwalk->code[8] = '\0';
				}
			}
		}
		pwalk = pwalk->link;
	}
}

O* asm_strtok(char* buf)
{
	O* tmp = (O*)malloc(sizeof(O));
	char* result;
	int i = 0;

	strcpy(tmp->loc, "\0");
	strcpy(tmp->stat1, "\0");
	strcpy(tmp->stat2, "\0");
	strcpy(tmp->stat3, "\0");
	strcpy(tmp->stat4, "\0");
	strcpy(tmp->code, "------\0");
	tmp->link = NULL;

	result = strtok(buf, " \t\n");
	strcpy(tmp->stat1, result);

	while (result != NULL)
	{
		if (i == 0)
		{
			if (buf[0] == ' ' || buf[0] == '\t')
			{
				strcpy(tmp->stat1, "\0");
				i++; continue;
			}
			strcpy(tmp->stat1, result);
			result = strtok(NULL, " \t\n");
			i++;
		}
		else if (i == 1)
		{
			strcpy(tmp->stat2, result);
			result = strtok(NULL, "', \t\n");
			i++;
		}
		else if (i == 2)
		{
			strcpy(tmp->stat3, result);
			if (!strcmp(tmp->stat3, "C"))
				result = strtok(NULL, "'");
			else result = strtok(NULL, "', \t\n");
			i++;
		}
		else if (i == 3)
		{
			strcpy(tmp->stat4, result);
			result = strtok(NULL, "', \t\n");
			i++;
		}
	}
	return tmp;

}

int find_stat1(char* x, int line)
{
	O* pwalk = start;

	while (pwalk != NULL)
	{
		if (!strcmp(pwalk->stat1, x))
			return loc_to_int(pwalk->loc);
		pwalk = pwalk->link;
	}

	make_error_list(line);
	return 0;
}
char* int_to_obcode3(int x)
{
	char* ret = (char*)malloc(4*sizeof(char));
	int tmp[3];

	tmp[0] = x/256;
	ret[0] = hex(tmp[0]);

	tmp[1] = (x - tmp[0]*256)/16;
	ret[1] = hex(tmp[1]);

	tmp[2] = x%16;
	ret[2] = hex(tmp[2]);

	ret[3] = '\0';

	return ret;
}
char* int_to_obcode5(int x)
{
	char* ret = (char*)malloc(6*sizeof(char));
	int tmp[5];

	tmp[0] = x/65536;
	ret[0] = hex(tmp[0]);

	tmp[1] = (x-tmp[0]*65536)/4096;
	ret[1] = hex(tmp[1]);

	tmp[2] = (x-tmp[0]*65536-tmp[1]*4096)/256;
	ret[2] = hex(tmp[2]);

	tmp[3] = (x-tmp[0]*65536-tmp[1]*4096-tmp[2]*256)/16;
	ret[3] = hex(tmp[3]);

	tmp[4] = x%16;
	ret[4] = hex(tmp[4]);

	ret[5] = '\0';

	return ret;
}
int is_hexnum(char *x)
{
	int len = strlen(x);
	int i;

	for (i = 0; i<len; i++)
	{
		if (x[i] < '0' || (x[i] > '9' && x[i] < 'A') || x[i] > 'F')
			return 1;
	}

	return 0;
}
int str_to_int(char x[5])
{
	int ret;
	int len = strlen(x);

	if (len == 1)
	{
		x[4] = '\0';
		x[3] = x[0];
		x[2] = '0';
		x[1] = '0';
		x[0] = '0';
	}
	else if (len == 2)
	{
		x[4] = '\0';
		x[3] = x[1];
		x[2] = x[0];
		x[1] = '0';
		x[0] = '0';
	}
	else if (len == 3)
	{
		x[4] = '\0';
		x[3] = x[2];
		x[2] = x[1];
		x[1] = x[0];
		x[0] = '0';
	}

	ret = ((x[0] - '0') * 1000) + ((x[1] - '0') * 100) + ((x[2] - '0') * 10) + (x[3] - '0');

	return ret;
}
char* int_to_loc(int x)
{
	char* ret = (char*)malloc(5*sizeof(char));
	int tmp[4];

	tmp[0] = x/4096;
	ret[0] = hex(tmp[0]);

	tmp[1] = (x-tmp[0]*4096)/256;
	ret[1] = hex(tmp[1]);

	tmp[2] = (x-tmp[0]*4096-tmp[1]*256)/16;
	ret[2] = hex(tmp[2]);

	tmp[3] = x%16;
	ret[3] = hex(tmp[3]);

	ret[4] = '\0';

	return ret;
}
int loc_to_int(char x[5])
{
	int ret;

	ret = (hex_i(x[0])) * 4096 + (hex_i(x[1])) * 256 + (hex_i(x[2])) * 16 + (hex_i(x[3]));

	return ret;
}
int find_opcode_loc(char x[10])
{
	int i;
	T* twalk;

	for (i = 0; i<T_size; i++) {
		twalk = table[i]->link;

		while (twalk != NULL) {
			if (!strcmp(twalk->inst, x))
			{
				return twalk->fm[0] - '0';
			}
			twalk = twalk->link;
		}
	}

	return 0;
}
// 양수 -> 음수
char* change_negative(char x[])
{
	int len = strlen(x);
	int* calc = (int*)malloc(len*sizeof(int));
	int i;

	for (i = 0; i<len; i++)
	{
		calc[i] = hex_i(x[i]);
		calc[i] = 15 - calc[i];
	}

	calc[len - 1] += 1;

	for (i = len - 1; i>0; i--)
		calc[i - 1] += calc[i] / 16;

	for (i = 0; i<len; i++)
		calc[i] = calc[i] % 16;

	for (i = 0; i<len; i++)
		x[i] = hex(calc[i]);

	return x;
}
void boundary_error(char tmp[5], int line)
{
	if (strlen(tmp) > 4)
	{
		make_error_list(line);
		return;
	}

	if (strlen(tmp) > 3)
	{
		if (tmp[3] < 48 || (tmp[3] > 57 && tmp[3] < 65) || (tmp[3] > 70 && tmp[3] < 97))
		{
			make_error_list(line);
			return;
		}
	}
	if (strlen(tmp) > 2)
	{
		if (tmp[2] < 48 || (tmp[2] > 57 && tmp[2] < 65) || (tmp[2] > 70 && tmp[2] < 97))
		{
			make_error_list(line);
			return;
		}
	}
	if (strlen(tmp) > 1)
	{
		if (tmp[1] < 48 || (tmp[1] > 57 && tmp[1] < 65) || (tmp[1] > 70 && tmp[1] < 97))
		{
			make_error_list(line);
			return;
		}
	}
	if (tmp[0] < 48 || (tmp[0] > 57 && tmp[0] < 65) || (tmp[0] > 70 && tmp[0] < 97))
	{
		make_error_list(line);
		return;
	}

	return;

}

void print_symbol_table()
{
	O* pwalk = start;
	symbol *swalk;

	shead = (symbol*)malloc(sizeof(symbol));
	strcpy(shead->label, "\0");
	strcpy(shead->loc, "\0");
	shead->link = NULL;

	while(pwalk!=NULL)
	{
		if(strcmp(pwalk->stat1, "\0"))
			make_symbol_table(pwalk->stat1, pwalk->loc);
		pwalk = pwalk->link;
	}

	swalk = shead;
	while(swalk->link!=NULL)
	{
		printf("\t%s\t%s\n", swalk->label, swalk->loc);
		swalk = swalk->link;
	}
}

void make_symbol_table(char* label, char* loc)
{
	symbol* tmp;
	symbol* front, *back;
	
	tmp = (symbol*)malloc(sizeof(symbol));
	strcpy(tmp->label, label);
	strcpy(tmp->loc, loc);
	tmp->link = NULL;

	if(!shead)
		shead = tmp;
	else
	{
		if(strcmp(shead->label, tmp->label)<0)
		{
			tmp->link = shead;
			shead = tmp;
		}
		else
		{
			front = shead;
			back = shead->link;
			while(back!=NULL){
				if(strcmp(front->label, tmp->label)>0 && strcmp(back->label, tmp->label)<0)
					break;
				front = front->link;
				back = back->link;
			}

			front->link = tmp;
			tmp->link = back;
		}
	}

}

// instruction을 인자로 받아 strtok을 이용하여 공백을 기준으로 자른다.
// 해당 자른 프로그램의 이름이 object file인지 확인하고, 아닐경우 0을 리턴한다.
// 총 입력받은 프로그램 파일의 개수를 리턴한다. 즉, 0은 입력받은 파일이 없음을 의미하여 오류를 출력하면 된다.
int open_linkfile(char tmp[80])
{
	char* result;
	int rlen=0;
	int NofFile=0;

	result = strtok(tmp, " \t");
	
	while(result!=NULL)
	{
		result = strtok(NULL, " \t");
		if(result==NULL) break;

		rlen = strlen(result);
		if(rlen < 5) return 0;

		if(result[rlen-4] != '.' || result[rlen-3] != 'o' || result[rlen-2] != 'b' || result[rlen-1] != 'j')
			return 0;

		NofFile+=1;
		
	}

	return NofFile;
}	
// pass1을 수행하는 함수
// 오류 발생시 1 리턴, 정상적으로 수행했을 시 0을 리턴한다.
int link_pass1(char* filename, int count) 
{
	FILE* fp;
	char input_str[100];
	int i, j;
	ESTAB* etmp;

	// 존재하지 않는 파일일 경우, 1을 리턴한다. 
	fp = fopen(filename, "r");
	if(!fp) return 1;
	
	// "\n"가 입력될때까지 파일을 입력받는다.
	fgets(input_str, 25, fp);
	
	// H code 입력
	if(input_str[0] != 'H') return 1;
	etmp = (ESTAB*)malloc(sizeof(ESTAB));

	// control section 이름 읽기
	for(i=1; i<7; i++)
	{
		if(input_str[i] == ' ' || input_str[i] == '\t' || input_str[i] == '\0')
		{
			etmp->control_section[i-1] = '\0';
			break;
		}
		etmp->control_section[i-1] = input_str[i];
	}

	// 프로그램 시작주소 읽기
	etmp->addr[0] = input_str[9];
	etmp->addr[1] = input_str[10];
	etmp->addr[2] = input_str[11];
	etmp->addr[3] = input_str[12];
	etmp->addr[4] = '\0';

	// 프로그램 길이 읽기
	etmp->length[0] = input_str[15];
	etmp->length[1] = input_str[16];
	etmp->length[2] = input_str[17];
	etmp->length[3] = input_str[18];
	etmp->length[4] = '\0';

	if(count==0)
	{
		strcpy(csaddr, int_to_loc(loc_to_int(etmp->addr) + loc_to_int(progaddr)));
		strcpy(etmp->addr, csaddr);	
	}
	else
	{
		strcpy(csaddr, int_to_loc(loc_to_int(etmp->addr) + loc_to_int(csaddr)));
		strcpy(etmp->addr, csaddr);
	}
	etmp->eslink = NULL;
	etmp->elink = NULL;

	if(ehead == NULL)
		ehead = etmp;
	else
	{
		ESTAB* ewalk = ehead;
		while(ewalk->elink != NULL)
			ewalk = ewalk->elink;
		ewalk->elink = etmp;
	}

	// Hcode 입력완료
	
	// Rcode, Dcode
	while(1)		// 한 줄씩 읽음
	{
		fgets(input_str, 100, fp);
		
		// D code 읽기
		if(input_str[0] == 'D')
		{
			// 입력받을 Dcode 갯수 확인
			// D code의 symbol 개수를 통해 input string의 배열 인덱스를 사용하여 정보를 읽어온다.
			int Dcode_num = strlen(input_str);
			Dcode_num -= 1;
			Dcode_num /= 12;
			
			for(i=0; i<Dcode_num; i++)
			{
				ESTAB_S* estmp = (ESTAB_S*)malloc(sizeof(ESTAB_S));
				// Read symbol name
				for(j=0; j<7; j++)
				{
					if(j==6) estmp->symbol_name[j] = '\0';
					else if(input_str[i*12+1+j] == ' ' || input_str[i*12+1+j] == '\t'){
						estmp->symbol_name[j] = '\0';
						break;
					}
					else
						estmp->symbol_name[j] = input_str[i*12 + 1 +  j];
				}
				// Read symbol address
				for(j=0; j<5; j++)
				{
					if(j==4) estmp->addr[j] = '\0';
					else if(input_str[9 + 12*i + j] == ' ' || input_str[9 + 12* i + j] == '\t' || input_str[9 + 12*i + j] == '\n')
					{
						estmp->addr[j] = '\0'; break;
					}
					else
						estmp->addr[j] = input_str[9 + 12*i + j];
				}
				
				// csaddr 설정 및 link 초기화
				strcpy(estmp->addr, int_to_loc(loc_to_int(estmp->addr) + loc_to_int(csaddr)));
				estmp->link = NULL;
				
				// 링크드리스트의 해당 위치에 값 저장
				if(etmp->eslink == NULL)
					etmp->eslink = estmp;
				else
				{
					ESTAB_S* eswalk = etmp->eslink;
					while(eswalk->link != NULL)
						eswalk = eswalk ->link;
					eswalk ->link = estmp;
				}
			} 
		}
		// R code도 D code와 동일한 방법을 사용하여 읽어온다.
		// 링크드리스트에 저장하는 D code의 symbol들과 달리, external symbol의 번호와 이름은 따로 배열에 저장한다.
		else if(input_str[0] == 'R')
		{
			// R code의 경우, 마지막 symbol은 이름이 6글자가 아닐 경우도 있기 때문에
			// 별도의 계산을 행한다.
			int Rcode_num = strlen(input_str);
			Rcode_num -= 1;
			if(Rcode_num%8!=0) Rcode_num = Rcode_num/8 + 1;
			else Rcode_num /= 8;

			prog_rcode[count] = (Rcode*)malloc(Rcode_num*sizeof(Rcode));
			alloc_rcode=1;
			
			for(i=0; i<Rcode_num; i++)
			{
				prog_rcode[count][i].code[0] = input_str[1 + 8*i];
				prog_rcode[count][i].code[1] = input_str[1 + 8*i + 1];
				prog_rcode[count][i].code[2] = '\0';
				
				for(j=0; j<7; j++)
				{
					if(j==6) prog_rcode[count][i].name[j] = '\0';
					else if(input_str[3 + 8*i + j] == ' ' || input_str[3+8*i+j]=='\t' || input_str[3+8*i+j] == '\n')
					{
						prog_rcode[count][i].name[j] = '\0'; break;
					}
					else prog_rcode[count][i].name[j] = input_str[3 + 8*i + j];
				}
			}
		}
		else if(input_str[0] == 'E')
		{
			strcpy(csaddr, int_to_loc(loc_to_int(csaddr) + loc_to_int(etmp->length)));
			break;
		}
	}
	

	fclose(fp);
	return 0;

}

int link_pass2(char* filename, int count)
{
	FILE* fp;
	char input_str[100];
	char input_addr[5];
	int input_length;
	int input_addnum;
	int i;
	ESTAB* pwalk = ehead;

	for(i=0; i<count; i++)
		pwalk = pwalk->elink;
	
	fp = fopen(filename, "r");
	if(!fp) return 1;

	while(1)
	{
		fgets(input_str, 100, fp);
		
		if(input_str[0] == 'T') // Read T code
		{
			input_addr[0] = input_str[3];
			input_addr[1] = input_str[4];
			input_addr[2] = input_str[5];
			input_addr[3] = input_str[6];
			input_addr[4] = '\0';

			input_addnum = loc_to_int(pwalk->addr) + loc_to_int(input_addr); 
			input_length = hex_i(input_str[7])*16 + hex_i(input_str[8]);

			for(i=0; i<input_length; i++)
			{
				int ret;

				dump[input_addnum+i][0] = input_str[9 + i*2];
				dump[input_addnum+i][1] = input_str[10 + i*2];
				
				ret = dump_int(dump[input_addnum+i]);

				if(ret<32 || ret>126) memory[input_addnum+i] = '.';
				else memory[input_addnum+i] = ret;
				
			}
		}	
		else if(input_str[0] == 'M') // Read M code
		{
			input_addr[0] = input_str[3];
			input_addr[1] = input_str[4];
			input_addr[2] = input_str[5];
			input_addr[3] = input_str[6];
			input_addr[4] = '\0';

			input_addnum = loc_to_int(pwalk->addr) + loc_to_int(input_addr);

			if(input_str[8] == '5') // 5-half byte를 계산해야 하는 경우
			{
				char dump_add[6], ext_add[6];
				// dump_add : 더하거나 빼어질 dump의 내용물
				// ext_dadd : 더하거나 뺄 external reference
				char neg_dumpadd[6] = {'\0', };
				int ext_num, neg=0;
				char ext_name[7];

				dump_add[0] = dump[input_addnum][1];
				dump_add[1] = dump[input_addnum+1][0];
				dump_add[2] = dump[input_addnum+1][1];
				dump_add[3] = dump[input_addnum+2][0];
				dump_add[4] = dump[input_addnum+2][1];
				dump_add[5] = '\0';

				if(dump_add[0] >= '8')
				{
					strcpy(neg_dumpadd, change_negative2(dump_add));
					neg=1;
				}
	
				ext_num = input_str[11] - '0';

				if(ext_num == 1) // 자기 자신의 주소를 계산해야 하는 경우
				{
					int a, b, ret;
					
					if(neg == 1)
						a = hex_i(neg_dumpadd[0]) * 65536 + hex_i(neg_dumpadd[1]) * 4096 + hex_i(neg_dumpadd[2]) * 256 + hex_i(neg_dumpadd[3]) * 16 + hex_i(neg_dumpadd[4]);
					else
						a = hex_i(dump_add[0]) * 65536 + hex_i(dump_add[1]) * 4096 + hex_i(dump_add[2]) * 256 + hex_i(dump_add[3]) * 16 + hex_i(dump_add[4]);
					
					b = hex_i(pwalk->addr[0]) * 65536 + hex_i(pwalk->addr[1]) * 4096 + hex_i(pwalk->addr[2]) * 256 + hex_i(pwalk->addr[3]) * 16 + hex_i(pwalk->addr[4]);

					if(neg==1)
					{
						if(input_str[9] == '+') ret = -1 * a + b;
						else if(input_str[9] == '-') ret = -1 * a - b;
						else return 1;
					}
					else
					{
						if(input_str[9] == '+') ret = a+b;
						else if(input_str[9] == '-') ret = a-b;
						else return 1;
					}

					if(ret >= 0)
					{
						dump_add[0] = hex(ret/F5);
						dump_add[1] = hex((ret - hex_i(dump_add[0]) * F5)/4096);
						dump_add[2] = hex((ret - hex_i(dump_add[0]) * F5 - hex_i(dump_add[1]) * 4096)/256);
						dump_add[3] = hex((ret - hex_i(dump_add[0]) * F5 - hex_i(dump_add[1]) * 4096 - hex_i(dump_add[2]) * 256)/16);
						dump_add[4] = hex(ret%16);
					}
					else
					{
						int pos = ret * -1;
						dump_add[0] = hex(pos/F5);
						dump_add[1] = hex((pos - hex_i(dump_add[0]) * F5)/4096);
						dump_add[2] = hex((pos - hex_i(dump_add[0]) * F5 - hex_i(dump_add[1]) * 4096)/256);
						dump_add[3] = hex((pos - hex_i(dump_add[0]) * F5 - hex_i(dump_add[1]) * 4096 - hex_i(dump_add[2]) * 256)/16);
						dump_add[4] = hex(pos%16);

						strcpy(dump_add, change_negative(dump_add));
					}
	
					dump[input_addnum][1] = dump_add[0];
					dump[input_addnum+1][0] = dump_add[1];
					dump[input_addnum+1][1] = dump_add[2];
					dump[input_addnum+2][0] = dump_add[3];
					dump[input_addnum+2][1] = dump_add[4];
				
					for(i=0; i<3; i++)
					{
						int ret2 = dump_int(dump[input_addnum+i]);
						if(ret2 < 32 || ret2 > 126) memory[input_addnum+i] = '.';
						else memory[input_addnum+i] = ret2;
					}
				}
				else // external symbol의 address를 계산해야 하는 경우
				{
					ESTAB* tmpwalk = ehead;
					ESTAB_S* tmpwalk2;
					int find_name=0;
					int a, b, ret;
					
					strcpy(ext_name, prog_rcode[count][ext_num-2].name);
				
					while(tmpwalk!=NULL)
					{
						tmpwalk2 = tmpwalk->eslink;
						while(tmpwalk2!=NULL)
						{
						
							if(!strcmp(tmpwalk2->symbol_name, ext_name))
							{
								ext_add[0] = '0';
								ext_add[1] = tmpwalk2->addr[0];
								ext_add[2] = tmpwalk2->addr[1];
								ext_add[3] = tmpwalk2->addr[2];
								ext_add[4] = tmpwalk2->addr[3];
								ext_add[5] = '\0';
								find_name=1;
								break;
							}
							tmpwalk2 = tmpwalk2->link;
						}
						if(find_name==1) break;
						tmpwalk = tmpwalk->elink;
					}

					if(find_name==0){
						return 1;
					}

					if(neg == 1)
						a = hex_i(neg_dumpadd[0]) * 65536 + hex_i(neg_dumpadd[1]) * 4096 + hex_i(neg_dumpadd[2]) * 256 + hex_i(neg_dumpadd[3]) * 16 + hex_i(neg_dumpadd[4]);
					else
						a = hex_i(dump_add[0]) * 65536 + hex_i(dump_add[1]) * 4096 + hex_i(dump_add[2]) * 256 + hex_i(dump_add[3]) * 16 + hex_i(dump_add[4]);

					b = hex_i(ext_add[0]) * 65536 + hex_i(ext_add[1]) * 4096 + hex_i(ext_add[2]) * 256 + hex_i(ext_add[3]) * 16 + hex_i(ext_add[4]);

					if(neg == 1)
					{
						if(input_str[9] == '+') ret = -1 * a + b;
						else if(input_str[9] == '-') ret = -1 * a - b;
						else return 1;
					}
					else
					{
						if(input_str[9] == '+') ret = a+b;
						else if(input_str[9] == '-') ret = a-b;
						else return 1;
					}

					if(ret >= 0)
					{
						dump_add[0] = hex(ret/F5);
						dump_add[1] = hex((ret - hex_i(dump_add[0]) * F5)/4096);
						dump_add[2] = hex((ret - hex_i(dump_add[0]) * F5 - hex_i(dump_add[1]) * 4096)/256);
						dump_add[3] = hex((ret - hex_i(dump_add[0]) * F5 - hex_i(dump_add[1]) * 4096 - hex_i(dump_add[2]) * 256)/16);
						dump_add[4] = hex(ret%16);
					}
					else
					{
						int pos = ret * -1;
						dump_add[0] = hex(pos/F5);
						dump_add[1] = hex((pos - hex_i(dump_add[0]) * F5)/4096);
						dump_add[2] = hex((pos - hex_i(dump_add[0]) * F5 - hex_i(dump_add[1]) * 4096)/256);
						dump_add[3] = hex((pos - hex_i(dump_add[0]) * F5 - hex_i(dump_add[1]) * 4096 - hex_i(dump_add[2]) * 256)/16);
						dump_add[4] = hex(pos%16);

						strcpy(dump_add, change_negative(dump_add));
					}

					dump[input_addnum][1] = dump_add[0];
					dump[input_addnum+1][0] = dump_add[1];
					dump[input_addnum+1][1] = dump_add[2];
					dump[input_addnum+2][0] = dump_add[3];
					dump[input_addnum+2][1] = dump_add[4];

					for(i=0; i<3; i++)
					{
						int ret2 = dump_int(dump[input_addnum+i]);
						if(ret2 <32 || ret2 >126) memory[input_addnum+i] = '.';
						else memory[input_addnum+i] = ret2;
					}

				}
			}
			else if(input_str[8] == '6') // 3byte 계산
			{
				char dump_add[7], ext_add[7];
				int ext_num;
				char ext_name[7];
				char neg_dumpadd[7] = {'\0',};
				int neg=0;

				
				for(i=0; i<3; i++)
				{
					dump_add[i*2] = dump[input_addnum+i][0];
					dump_add[i*2+1] = dump[input_addnum+i][1];
				}
				dump_add[6] = '\0';

				if(dump_add[0] >= '8')
				{
					strcpy(neg_dumpadd, change_negative2(dump_add));
					neg = 1;
				}

				ext_num = input_str[11] - '0';

				if(ext_num == 1)  // 자기 자신의 주소를 빼는 경우
				{
					int a, b, ret;
					
					if(neg == 1)
						a = hex_i(neg_dumpadd[0]) * F6 + hex_i(neg_dumpadd[1]) * 65536 + hex_i(neg_dumpadd[2]) * 4096 + hex_i(neg_dumpadd[3]) * 256 + hex_i(neg_dumpadd[4]) * 16 + hex_i(neg_dumpadd[5]);
					else
						a = hex_i(dump_add[0]) * F6 + hex_i(dump_add[1]) * 65536 + hex_i(dump_add[2]) * 4096 + hex_i(dump_add[3]) * 256 + hex_i(dump_add[4]) * 16 + hex_i(dump_add[5]);
					
					b = hex_i(pwalk->addr[0]) * 4096 + hex_i(pwalk->addr[1]) * 256 + hex_i(pwalk->addr[2]) * 16 + hex_i(pwalk->addr[3]);
										
					if(neg == 1)
					{
						if(input_str[9] == '+') ret = -1 * a + b;
						else if(input_str[9] == '-') ret = -1 * a - b;
						else return 1;
					}
					else
					{
						if(input_str[9] == '+') ret = a+b;
						else if(input_str[9] == '-') ret = a-b;
						else return 1;
					}

					if(ret >= 0)
					{
						dump_add[0] = hex(ret/F6);
						dump_add[1] = hex((ret - hex_i(dump_add[0]) * F6)/F5);
						dump_add[2] = hex((ret - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5)/4096);
						dump_add[3] = hex((ret - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5 - hex_i(dump_add[2]) * 4096)/256);
						dump_add[4] = hex((ret - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5 - hex_i(dump_add[2]) * 4096 - hex_i(dump_add[3]) * 256)/16);
						dump_add[5] = hex(ret%16);
						dump_add[6] = '\0';
					}
					else
					{
						int pos = ret * -1;
						dump_add[0] = hex(pos/F6);
						dump_add[1] = hex((pos - hex_i(dump_add[0]) * F6)/F5);
						dump_add[2] = hex((pos - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5)/4096);
						dump_add[3] = hex((pos - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5 - hex_i(dump_add[2]) * 4096)/256);
						dump_add[4] = hex((pos - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5 - hex_i(dump_add[2]) * 4096 - hex_i(dump_add[3]) * 256)/16);
						dump_add[5] = hex(pos%16);
						dump_add[6] = '\0';

						strcpy(dump_add, change_negative(dump_add));
 
					}

					for(i=0; i<3; i++)
					{
						dump[input_addnum+i][0] = dump_add[i*2];
						dump[input_addnum+i][1] = dump_add[i*2 + 1];
					}
					
					for(i=0; i<3; i++)
					{
						int ret2 = dump_int(dump[input_addnum+i]);
						if(ret2 < 32 || ret2 > 126) memory[input_addnum+i] = '.';
						else memory[input_addnum+i] = ret2;
					}
				}
				else // external symbol의 address를 계산해야 하는 경우
				{
					ESTAB* tmpwalk = ehead;
					ESTAB_S* tmpwalk2;
					int find_name=0;
					int a, b, ret;
					
					strcpy(ext_name, prog_rcode[count][ext_num-2].name);
										
					while(tmpwalk!=NULL)
					{
						tmpwalk2 = tmpwalk->eslink;
						while(tmpwalk2!=NULL)
						{
							if(!strcmp(tmpwalk2->symbol_name, ext_name))
							{
								ext_add[0] = '0';
								ext_add[1] = '0';
								ext_add[2] = tmpwalk2->addr[0];
								ext_add[3] = tmpwalk2->addr[1];
								ext_add[4] = tmpwalk2->addr[2];
								ext_add[5] = tmpwalk2->addr[3];
								ext_add[6] = '\0';
								find_name=1;
								break;
							}
							tmpwalk2 = tmpwalk2->link;
						}
						if(find_name==1) break;
						tmpwalk = tmpwalk->elink;
					}

					if(find_name==0) return 1;
		
					if(neg == 1)
						a = hex_i(neg_dumpadd[0]) * F6 + hex_i(neg_dumpadd[1]) * F5 + hex_i(neg_dumpadd[2]) * 4096 + hex_i(neg_dumpadd[3]) * 256 + hex_i(neg_dumpadd[4]) * 16 + hex_i(neg_dumpadd[5]);
					else
						a = hex_i(dump_add[0]) * F6 + hex_i(dump_add[1]) * F5 + hex_i(dump_add[2]) * 4096 + hex_i(dump_add[3]) * 256 + hex_i(dump_add[4]) * 16 + hex_i(dump_add[5]);

					b = hex_i(ext_add[0]) * F6 + hex_i(ext_add[1]) * F5 + hex_i(ext_add[2]) * 4096 + hex_i(ext_add[3]) * 256 + hex_i(ext_add[4]) * 16 + hex_i(ext_add[5]);
				
					
					if(neg == 1)
					{
						if(input_str[9] == '+') ret = -1 * a + b;
						else if(input_str[9] == '-') ret = -1 * a - b;
						else return 1;
					}
					else
					{
						if(input_str[9] == '+') ret = a+b;
						else if(input_str[9] == '-') ret = a-b;
						else return 1;
					}
									
					if(ret >= 0)
					{
						dump_add[0] = hex(ret/F6);
						dump_add[1] = hex((ret - hex_i(dump_add[0]) * F6)/F5);
						dump_add[2] = hex((ret - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5)/4096);
						dump_add[3] = hex((ret - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5 - hex_i(dump_add[2]) * 4096)/256);
						dump_add[4] = hex((ret - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5 - hex_i(dump_add[2]) * 4096 - hex_i(dump_add[3]) * 256)/16);
						dump_add[5] = hex(ret%16);
						dump_add[6] = '\0';
					}
					else
					{
						int pos = ret * -1;

						dump_add[0] = hex(pos/F6);
						dump_add[1] = hex((pos - hex_i(dump_add[0]) * F6)/F5);
						dump_add[2] = hex((pos - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5)/4096);
						dump_add[3] = hex((pos - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5 - hex_i(dump_add[2]) * 4096)/256);
						dump_add[4] = hex((pos - hex_i(dump_add[0]) * F6 - hex_i(dump_add[1]) * F5 - hex_i(dump_add[2]) * 4096 - hex_i(dump_add[3]) * 256)/16);
						dump_add[5] = hex(pos%16);
						dump_add[6] = '\0';

						strcpy(dump_add, change_negative(dump_add));
					}

					for(i=0; i<3; i++)
					{
						dump[input_addnum+i][0] = dump_add[i*2];
						dump[input_addnum+i][1] = dump_add[i*2 + 1];
					}
					
					for(i=0; i<3; i++)
					{
						int ret2 = dump_int(dump[input_addnum+i]);
						if(ret2 < 32 || ret2 > 126) memory[input_addnum+i] = '.';
						else memory[input_addnum+i] = ret2;
					}
				}
				
			}
			else return 1;

		}
		else if(input_str[0] == 'E') break;

	}

	fclose(fp);

	return 0;
}
// 음수 -> 양수
char* change_negative2(char x[])
{
	int len = strlen(x);
	int* calc = (int*)malloc(len * sizeof(int));
	int i;

	for(i=0; i<len; i++)
		calc[i] = hex_i(x[i]);

	calc[len-1] -= 1;

	for(i=len-1 ; i>0; i--)
	{
		if(calc[i] == -1)
		{
			calc[i] += 16;
			calc[i-1] -= 1;
		}
	}
	
	for(i=0; i<len; i++)
		calc[i] = 15 - calc[i];

	for(i=0; i<len; i++)
		x[i] = hex(calc[i]);

	return x;

}
void print_ESTAB()
{
	ESTAB* pwalk1 = ehead;
	ESTAB_S* pwalk2;
	int total_length=0;

	printf("\t\tcontrol\tsymbol\taddress\tlength\n");
	printf("\t\tsection\tname\n");
	printf("\t\t-----------------------------------\n");
	while(pwalk1!=NULL)
	{
		printf("\t\t%s\t\t%s\t%s\n", pwalk1->control_section, pwalk1->addr, pwalk1->length);
		pwalk2 = pwalk1->eslink;
		total_length += loc_to_int(pwalk1->length);
		while(pwalk2!=NULL)
		{
			printf("\t\t\t%s\t%s\n", pwalk2->symbol_name, pwalk2->addr);
			pwalk2=pwalk2->link;
		}
		pwalk1=pwalk1->elink;
	}
	printf("\t\t----------------------------------\n");
	printf("\t\t\ttotal length\t%s\n", int_to_loc(total_length));
}
void free_ESTAB()
{
	ESTAB* etmp;
	ESTAB_S* estmp, *estmp2;

	etmp = ehead;
	while(ehead != NULL)
	{
		estmp = ehead->eslink;
		while(estmp!=NULL)
		{
			estmp2 = estmp;
			estmp = estmp->link;
			free(estmp2);
		}
		etmp = ehead;
		ehead = ehead->elink;
		free(etmp);
	}
	ehead = NULL;

}

void make_error_list(int line)
{
	if (error_line[error_count - 1] != line)
	{
		error_line[error_count] = line;
		error_count += 1;
	}
}

void No_Inst_Error()
{
	printf("Error!\n");
}
void free_linkedlist()
{
	history* pwalk = head;
	T* twalk;
	int i;

	while (head != NULL)
	{
		pwalk = head;
		head = head->link;
		free(pwalk);
	}

	for (i = 0; i<T_size; i++)
	{
		while (table[i]->link != NULL) {
			twalk = table[i]->link;
			table[i]->link = table[i]->link->link;
			free(twalk);
		}
	}
	free(table);

}
