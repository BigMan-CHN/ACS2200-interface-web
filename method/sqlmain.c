#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stu_sqlite.h"
#include "sqlite3.h"
sqlite3 *reg_db = NULL;
char *help ="********************************\n"\
			"***add:    add register info    \n"\
			"***update: update register info \n"\
			"***del:    del register info    \n"\
			"***order:  order register  by id\n"\
			"***find:   find register by id\n"\
			"***print:  print all register   \n"\
			"\e[32m***login:  add login info		 \n"\
			"\e[32m***logfind:find login info		 \n"\
			"\e[32m***logprint:print all login     \n"\
			"\e[32m***cls:    clear the screen     \n"\
			"\e[32m***exit:   exit                 \n\e[37m\e[0m"\
			"********************************\n";
			
void add_fun(void)		
{
	char *help = "please enter id methon val";
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	printf("in add_fun\n");
	printf("%s\n", help);
	scanf("%d %s %s", &(stu.id), stu.name, stu.val);
	sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
	getchar();
	insert_stu_db(reg_db, &stu); //add 到数据库

}

void update_fun(void)		
{
	char *help = "please enter id methon val";
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	printf("in update_fun\n");
	printf("%s\n", help);
	scanf("%d %s %s", &(stu.id), stu.name, stu.val);
	sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
	getchar();
	update_stu_db(reg_db, &stu); //update 到数据库
}


void del_fun(void)
{
	STU stu;
	printf("in del_fun\n");
	printf("please enter id for delete methon\n");
	scanf("%s", &(stu.name));
	getchar();
	delete_stu_db(reg_db, &stu); //delete id 从数据库
}

void order_fun(void)
{
	printf("in order_fun\n");
	//order_by_id_stu_db_exec(db); order 数据库
	order_by_id_stu_db_get_table(reg_db);//get_table 数据库
}

void find_fun(void)
{
	STU stu;
	printf("in find_fun methon\n");
	scanf("%s", &stu.name);
	getchar();
	//find_stu_db_get_table(reg_db, &stu);
	find_stu_db(reg_db, &stu);
}

void print_fun(void)
{
	printf("in print_fun\n");
	print_stu_db(reg_db, "stu_info");
	//print_stu_db_get_table(reg_db, "stu_info");
}

void cls_fun(void)
{
	printf("in cls_fun\n");
	system("clear");
}

void exit_fun(void)
{
	printf("in exit_fun\n");
	sqlite3_close(reg_db);
	exit(0);
}

void help_fun(void )
{
	printf("%s\n", help);
}

void login_fun(void )
{
	char *help = "please enter  id  name";
	STU stu;
/*	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	tm_ptr = localtime(&cur_time);
	printf("in add_fun\n");
	printf("%s\n", help);
	scanf("%d %s", &(stu.id), stu.name);
	sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
	getchar();
	insert_stu_db(reg_db, &stu); //add 到数据库
	*/
	printf("in find_fun\n");
	scanf("%d", &stu.id);
	getchar();
	//find_stu_db_get_table(reg_db, &stu);
	if(find_stu_db_get_table(reg_db, &stu))
	{
		printf("Query to!\n");
	}
	else
	{
		printf("Not register!\n");
	}
	
}

typedef void (*FUN)(void);
typedef struct cmd
{
	char *cmd;
	FUN fun;
}CMD;

CMD cmd_list[] = 
{
	{"add", add_fun},
	{"update", update_fun},
	{"del", del_fun},
	{"order", order_fun},
	{"find", find_fun},
	{"print", print_fun},
	{"cls", cls_fun},
	{"exit", exit_fun},
	{"help", help_fun},
	{"login", login_fun}
};

int sqlmain(int argc, char **argv)
{
	char cmdbuf[20];
	char *register_db = "info_manage.db";
	int i = 0;
	
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库
	create_db_table(reg_db);  //创建注册表
	printf("%s\n", help);
	while(1)
	{
		printf("student manger:");	
		fflush(stdout);
	
		fgets(cmdbuf, sizeof(cmdbuf), stdin);
		cmdbuf[strlen(cmdbuf)-1] = '\0';
		for (i=0; i<sizeof(cmd_list)/sizeof(CMD); i++)
		{
			if (strcmp(cmdbuf, cmd_list[i].cmd) == 0)
			{
				cmd_list[i].fun();
				//continue;
			}
		}
		//printf("no this command\n");
	}
	return 0;
}
