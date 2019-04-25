#include "20141597.h"

int main() {
	char inst[80];
	char inst_tmp[80];
	char inst_str[20];  // string for input instructions
	int a, i;   		// counters
	int f_check, flag;	//flag

						// Initialize
	last_add = 0;
	read_opcode();
	dump_makeM();
	shead = NULL;
	start = NULL;
	base = NULL;

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
			if (inst_tmp[i] == '\0' || inst_tmp[i] == 'n')
				break;

			if (inst_tmp[i] == ' ' || inst_tmp[i] == '\t')
			{
				i++;
				continue;
			}
			else
			{
				flag = 1;
				break;
			}
		}

		if (flag == 0) continue;
		f_check = -1;

		// check space in front of instruction
		i = 0; a = 0;
		while (1)
		{
			if (inst_tmp[i] == ' ' || inst_tmp[i] == '\t')
				i++;
			else break;
		}

		while (1)
		{
			inst[a] = inst_tmp[a + i];
			a++;

			if (inst_tmp[a + i] == '\0')
			{
				inst[a] = '\0';
				break;
			}
		}

		// check space back of instruction
		i = -1; a = 0;
		flag = 0;
		while (1)
		{
			i++;
			if (inst[i] != ' ' && inst[i] != '\t')
				continue;
			if (inst[i] == ' ' || inst[i] == '\t')
				flag = 1;
			break;
		}

		if (flag == 1)
		{
			a = i;
			while (1)
			{
				if (inst[a] == '\n' || inst[a] == '\0')
					break;
				if (inst[a] != ' ' && inst[a] != '\t')
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
		printf("Error(help)\n");
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
		}

		// error
		else
		{
			No_Inst_Error();
			continue;
		}

		//////////////////////////////////////////////////////////////////////////////////////
//
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
//
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
	int tmp;

	ret[3] = '\0';

	ret[2] = hex(x % 16);

	tmp = x % 256 - x % 16;
	ret[1] = hex(tmp / 16);

	tmp = x % 4096 - x % 256 - x % 16;
	ret[0] = hex(tmp / 256);

	return ret;
}
char* int_to_obcode5(int x)
{
	char* ret = (char*)malloc(6*sizeof(char));
	int tmp;

	ret[5] = '\0';

	ret[4] = hex(x % 16);

	tmp = x % 256 - x % 16;
	ret[3] = hex(tmp / 16);

	tmp = x % 4096 - x % 256 - x % 16;
	ret[2] = hex(tmp / 256);

	tmp = x%F5 - x % 4096 - x % 256 - x % 16;
	ret[1] = hex(tmp / 4096);

	ret[0] = hex(x / F5);

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
	int tmp;

	ret[4] = '\0';

	ret[3] = hex(x % 16);

	tmp = x % 256 - x % 16;
	ret[2] = hex(tmp / 16);

	tmp = x % 4096 - x % 256 - x % 16;
	ret[1] = hex(tmp / 256);

	ret[0] = hex(x / 4096);

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
char* change_negative(char *x)
{
	int len = strlen(x);
	int calc[5];
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
