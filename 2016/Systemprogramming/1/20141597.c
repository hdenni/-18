#include "20141597.h"

int main(){
	char inst[80];
	char inst_tmp[80];
	char inst_str[20];  // string for input instructions
	int a, i;   		// counters
	int f_check, flag;	//flag


	last_add = 0;		
	read_opcode();
	dump_makeM();		// Initialize

	while(1){
		fflush(stdin);

		strcpy(inst_str, "\0");
		strcpy(inst, "\0");
		strcpy(inst_tmp, "\0");	// Initialize string

		printf("sicsim> ");
		gets(inst_tmp);

		i=0; flag=0;
		while(1)
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
		while(1)
		{
			i++;
			if(inst[i] != ' ' && inst[i] != '\t') 
				continue;
			if(inst[i] == ' ' || inst[i] == '\t')
				flag = 1;
			break;
		}

		if(flag == 1)
		{
			a=i;
			while(1)
			{
				if(inst[a] == '\n' || inst[a] == '\0')
					break;
				if(inst[a] != ' ' && inst[a] != '\t')
					flag=2;
				a++;
			}
		}

		// check instruction (ex. opcode, fill, edit...)
		sscanf(inst, "%[^ \t]", inst_str);

		if(flag == 1)
			strcpy(inst, inst_str);
		flag = 0;


		// 1) Instruction about shell
		if(!strcmp(inst, "q") || !strcmp(inst, "quit"))
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

		else if(!strcmp(inst, "h") || !strcmp(inst, "help"))
		{
			f_check = shell_help();
			if(f_check!=0)
			{
				printf("Error - help\n");
				continue;
			}
			make_history(inst);
		}

		else if(!strcmp(inst, "hi") || !strcmp(inst, "history"))
		{
			make_history(inst);
			f_check = shell_history();
			if(f_check!=0)
			{
				printf("Error - history\n");
				continue;
			}
		}

		// 2) Instruction about memory
		else if(!strcmp(inst, "du") || !strcmp(inst_str, "dump"))
		{
			int start, end;
			char trash='\0';
			start = -1; end = -1;
			sscanf(inst, "%[^ \t]%x%[^1234567890ABCDEF]%x", inst_str, &start, inst_tmp, &end);

			flag = 0; a = 0;
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

			if(start == -1)
				start = last_add;

			if((end!=-1 && start > end) || start < 0)
			{
				printf("Error - start address > end address\n");
				continue;
			}
			if (flag == 2 && end != -1)
			{
				printf("Error(dump) : Boundary Error\n");
				continue;
			}
			else if(flag == 0 && end != -1)
			{
				printf("Error(dump) : You need <,> beside two value\n");
				continue;
			}

			dump_printM(start, end);
			make_history(inst);
		}

		else if(!strcmp(inst_str, "e") || !strcmp(inst_str, "edit"))
		{
			int addr=0;
			char value[80], trash='\0';
			sscanf(inst, "%[^ \t]%x%[^1234567890ABCDEF]%s", inst_str, &addr, inst_tmp, value);

			flag = 0;
			a = 0;
			while(1)
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



			if(strlen(value)>3)
			{
				printf("Error(edit) : value error!\n");
				continue;
			}
			f_check = dump_edit(addr, value);
			if(f_check!=0)
			{
				printf("Error(edit) : Boundary error!\n");
				continue;
			}
			make_history(inst);
		}	

		else if(!strcmp(inst_str, "f") || !strcmp(inst_str, "fill"))
		{
			int start, end;
			char trash[2][20], tras;
			char value[80];

			sscanf(inst, "%[^ \t]%x%[^1234567890ABCDEF]%x%[^1234567890ABCDEF]%s", inst_str, &start, trash[0], &end, trash[1], value);

			flag = 0;
			for (i = 0 ; i < 2 ; i++)
			{
				a = 0;
				while(1)
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
			}
			if (flag == 2 || flag == 0)
			{
				printf("Boundary Error!\n");
				continue;
			}






			if(strlen(value)>3)
			{
				printf("Error(fill) : value error!\n");
				continue;
			}
			f_check = dump_fill(start, end, value);
			if(f_check!=0)
			{
				printf("Error(fill) : Boundary error!\n");
				continue;
			}
			make_history(inst);
		}


		else if(!strcmp(inst, "reset"))
		{
			make_history(inst);
			dump_makeM();
		}

		// 3) Instruction about opcode
		else if(!strcmp(inst_str, "opcode"))
		{
			char m[10];

			sscanf(inst, "%[^ \t]%s", inst_str, m);

			f_check = opcode_mnemonic(m);
			if(f_check!=0)
			{
				printf("The opcode is not exist\n");
				continue;
			}
			make_history(inst);
		}

		else if(!strcmp(inst, "opcodelist"))
		{
			f_check = opcode_opcodelist();
			if(f_check!=0)
			{
				printf("Error - opcodelist\n");
				continue;
			}
			make_history(inst);
		}

		// error
		else
		{
			No_Inst_Error();
			continue;
		}
	}

	free_linkedlist();

	return 0;
}
void free_linkedlist()
{
	history* pwalk = head;
	T* twalk;
	int i;

	while(head!=NULL)
	{
		pwalk=head;
		head=head->link;
		free(pwalk);
	}

	for(i=0; i<T_size; i++)
	{
		while(table[i]->link != NULL){
			twalk = table[i]->link;
			table[i]->link = table[i]->link->link;
			free(twalk);
		}
	}
	free(table);

}
void No_Inst_Error()
{
	printf("The instruction is not exist\n");
}
char hex(int n)
{
	switch(n){
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
		default : return 'X';
	}
}
void dump_makeM()
{
	int i;

	for(i=0; i<F6; i++)
	{
		dump[i][0] = '0';
		dump[i][1] = '0';
		dump[i][2] = '\0';

		memory[i] = '.';
	} // initialize 

	for(i=0; i<F5; i++)
	{
		add[i][0] = hex((i%65536)/4096);
		add[i][1] = hex((i%4096)/256);
		add[i][2] = hex((i%256)/16);
		add[i][3] = hex(i%16);
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
	if(end == -1)
	{
		lastline = line+9;
		end = lastline*16+15;
	} 
	// dump start, end
	else lastline = find_line(end);

	for(i=find_line(start)*16; i<F6; i++)
	{
		// P : memory address
		if(i%16==0)
			printf("%s ", add[line]);

		// P : memory
		if(i < start || i > end) printf("   ");
		else printf("%s ", dump[i]);

		// P : memory - ASCII CODE
		if(i%16 == 15)
		{
			printf("; ");
			for(j=i-15; j<i; j++)
				printf("%c", memory[j]);
			printf("\n");
			line++;
		}

		if(line == lastline+1)
		{
			last_add = i+1;
			break;
		}
	}

	// check last address
	if(end < last_add)
		last_add = end+1;
}
int dump_edit(int addr, char value[80])
{
	int ret = dump_int(value);

	// Boundary Error
	if(addr < 0 || addr > F6)
		return 1;

	strcpy(dump[addr], value);
	if(strlen(dump[addr])==1)
	{
		dump[addr][1] = dump[addr][0];
		dump[addr][0] = '0';
	}

	// If memory is under 0x20 or over 0x7E
	if(ret < 32 || ret > 126)
		memory[addr] = '.';

	else memory[addr] = ret;

	return 0;
}

int dump_fill(int start, int end, char value[80])
{
	int i;
	int ret;

	if(start > end) return 1;

	for(i=start; i<=end; i++)
	{
		strcpy(dump[i], value);
		ret = dump_int(value);

		// If memory is under 0x20 or over 0x7E
		if(ret < 32 || ret > 126)
			memory[i] = '.';

		else memory[i] = ret;
	}
	return 0;

}

int find_line(int x)
{
	int i;

	for(i=1; i<0xFFFFFFF; i = i*16)
	{
		if(x < i)
			return x/16;
	}
	return -1;
}
int dump_int(char* str)
{
	int len = strlen(str);
	int sum, i, line;

	for(i=0, sum=0; i<len; i++)
	{
		line=0;
		if(str[i] >= 'A' && str[i] <= 'F')
			line += str[i] - 'A' + 10;
		else if(str[i] >= '0' && str[i] <= '9')
			line += str[i] - '0';
		if(i==0) sum+=line*16;
		else sum+=line;		
	}

	return sum;

}
void make_history(char instruction[80])
{
	history *pwalk, *temp;

	temp = (history*)malloc(sizeof(history));

	strcpy(temp->inst, instruction);
	temp->link = NULL;

	if(!head)
		head = temp;
	else
	{
		pwalk = head;
		while(pwalk->link!=NULL)
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

	return 0;
}
int shell_history()
{
	int count=1;
	history* pwalk = head;

	if(pwalk == NULL)
		return 1;

	while(pwalk!=NULL)
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
	int count=0;

	dp = opendir(".");
	if(!dp) return 1;

	while(1){
		f = readdir(dp);
		lstat(f->d_name, &buf);
		if(!f) break;

		if(!strcmp(f->d_name, ".") || !strcmp(f->d_name, ".."))
			continue;

		if(S_ISDIR(buf.st_mode))
			printf("%20s/\t", f->d_name);
		else
		{
			if(buf.st_mode & S_IXUSR || buf.st_mode & S_IXGRP || buf.st_mode & S_IXOTH)
				printf("%20s*\t", f->d_name);
			else printf("%20s\t", f->d_name);
		}
		count++;

		if(count%4==0) printf("\n");
	}
	printf("\n");

	closedir(dp);
	return 0;
}
void read_opcode()
{
	FILE* fp;
	int ret, i;
	char code[3], inst[10], trash[5];

	fp = fopen("opcode.txt", "r");
	if(!fp){
		fprintf(stderr, "File open error! (opcode.txt)\n");
		exit(1);
	}

	table = (Thead**)malloc(T_size*sizeof(Thead*));

	for(i=0; i<T_size; i++){
		table[i] = (Thead*)malloc(sizeof(Thead));

		table[i]->count = 0;
		table[i]->link	= NULL;
	}

	while(fscanf(fp, "%s %s %s", code, inst, trash) != EOF){
		ret = opcodetable_hash(code);

		make_table(code, inst, ret);
	}

	fclose(fp);
}
void make_table(char* code, char* inst, int ret){
	T *tmp, *twalk;

	tmp = (T*)malloc(sizeof(T));
	strcpy(tmp->code, code);
	strcpy(tmp->inst, inst);
	tmp->link = NULL;

	if(table[ret]->count == 0)
		table[ret]->link = tmp;
	else{
		twalk = table[ret]->link;
		while(twalk->link != NULL)
			twalk = twalk->link;
		twalk->link = tmp;
	}

	table[ret]->count+=1;

}
int opcodetable_hash(char *s){
	int i, sum;

	int len = strlen(s);
	for(i=0, sum=0; i<len; i++)
		sum+=s[i];
	return sum%20;
}
int opcode_opcodelist()
{
	int i;
	T* twalk;

	if(!table)
		return 1;

	for(i=0; i<T_size; i++)
	{
		printf("%d : ", i);
		twalk = table[i]->link;
		while(twalk!=NULL)
		{
			printf("[%s, %s]", twalk->inst, twalk->code);
			if(twalk->link != NULL) printf(" -> ");
			twalk = twalk->link;
		}
		printf("\n");
	}

	return 0;
}
int opcode_mnemonic(char* inst)
{
	int i;
	T* twalk;

	for(i=0; i<T_size; i++){
		twalk = table[i]->link;

		while(twalk!=NULL){	
			if(!strcmp(twalk->inst, inst))
			{
				printf("opcode is %s\n", twalk->code);
				return 0;
			}
			twalk = twalk->link;
		}
	}

	return 1;
}

