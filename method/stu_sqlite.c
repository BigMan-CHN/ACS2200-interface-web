#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stu_sqlite.h"
#include "sqlite3.h"

/**
 * @fuction    打开数据库，没有的话会进行创建
 * @Author     wlb
 * @DateTime   2022-05-24T15:28:20+0800
 * @param      [description]
 * @return    [description]
 */
sqlite3 *create_db(char *db_name)
{
	sqlite3 *db = NULL;
	int result;
	
	result = sqlite3_open(db_name, &db); //open stu_info.db
	if (result){//failure
		fprintf(stderr, "cannot open sqlite3 base: %s", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	else{

		//printf("open success\n");
	}

	return db;
}

/**
 * @fuction    关闭数据库
 * @Author     wlb
 * @DateTime   2022-05-24T15:46:35+0800
 * @param      [description]
 */
void close_db(char *db_name)
{
	sqlite3_close(db_name);
	return;
}

/**
 * @fuction    创建默认数据库表，没有会创建，已存在的话不会再进行创建
 * @Author     wlb
 * @DateTime   2022-05-24T15:28:36+0800
 * @param      [description]
 */
void create_db_table(sqlite3 *db)
{
	DBUG();
	char *errmsg;
	char *reg_sql = "create table register_info (id INTEGER, method TEXT, val TEXT, time TEXT)";
			 
	sqlite3_exec(db, reg_sql, NULL, NULL, &errmsg);	
	DBUG("errmsg %s\n", errmsg);
}

/**
 * @fuction    创建自定义数据库表，没有会创建，已存在的话不会再进行创建
 * @Author     wlb
 * @DateTime   2022-05-24T15:28:36+0800
 * @param      [description]
 */
void create_self_db_table(sqlite3 *db,char *Table)
{
	DBUG();
	char *errmsg;
	char reg_sql[1024];
	sprintf(reg_sql,"%s",Table);		 
	sqlite3_exec(db, reg_sql, NULL, NULL, &errmsg);	
	DBUG("errmsg %s\n", errmsg);
}

/**
 * @fuction    向数据库表中插入数据
 * @Author     wlb
 * @DateTime   2022-05-24T15:29:43+0800
 * @param      [description]
 * @param      [description]
 */
void insert_stu_db(sqlite3 *db, STU *stu)
{
	char *errmsg;
	char sql[1024];
	int result;
	
	//sprintf(sql,"INSERT INTO stopedCar VALUES(\"%s\", %d,%d)",car_num,tim,our_car_flag);
	sprintf(sql, "insert into register_info values(%d, \"%s\", \"%s\", \"%s\")", stu->id, stu->name, stu->val, stu->time);
	result = sqlite3_exec(db, sql, NULL, NULL, &errmsg);	
	//DBUG("errmsg %s\n", errmsg);
	//printf("result %d\n", result);
	//sqlite3_close(db);
}
void insert_self_stu_db(sqlite3 *db, char *Table)
{
	char *errmsg;
	char sql[1024];
	int result;
	
	//sprintf(sql,"INSERT INTO stopedCar VALUES(\"%s\", %d,%d)",car_num,tim,our_car_flag);
	///sprintf(sql, "insert into register_info values(%d, \"%s\", \"%s\", \"%s\")", stu->id, stu->name, stu->val, stu->time);
	sprintf(sql, "%s", Table);
	result = sqlite3_exec(db, sql, NULL, NULL, &errmsg);	
	//DBUG("errmsg %s\n", errmsg);
	//printf("result %d\n", result);
	//sqlite3_close(db);
}

/**
 * @fuction    向数据库表中更新数据
 * @Author     fwb
 * @DateTime   2022-05-24T15:29:43+0800
 * @param      [description]
 * @param      [description]
 */
void update_stu_db(sqlite3 *db, STU *stu)
{
	char *errmsg;
	char sql[1024];
	int result;
	
	sprintf(sql, "update register_info set method='%s',val='%s',time='%s' where method='%s'", stu->name, stu->val, stu->time, stu->name);
	result = sqlite3_exec(db, sql, NULL, NULL, &errmsg);	
	DBUG("errmsg %s\n", errmsg);
	//printf("result %d\n", result);
	//sqlite3_close(db);
}
/**
 * @fuction    向数据库表中更新数据
 * @Author     fwb
 * @DateTime   2022-05-24T15:29:43+0800
 * @param      [description]
 * @param      [description]
 */
void update_self_stu_db(sqlite3 *db, char *Table)
{
	char *errmsg;
	char sql[1024];
	int result;
	
	sprintf(sql, "%s", Table);
	result = sqlite3_exec(db, sql, NULL, NULL, &errmsg);	
	DBUG("errmsg %s\n", errmsg);
	//printf("result %d\n", result);
	//sqlite3_close(db);
}


/**
 * @fuction    从数据库中删除数据
 * @Author     wlb
 * @DateTime   2022-05-24T15:30:01+0800
 * @param      [description]
 * @param      [description]
 */
void delete_stu_db(sqlite3 *db, STU *stu)
{
	char *errmsg;
	char sql[1024];
	int result;
	
	sprintf(sql, "delete from register_info where method='%s'", stu->name);
	DBUG("%s\n", sql);
	result = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	DBUG("result %d\n", result);
}
/**
 * @fuction    从数据库中删除数据
 * @Author     wlb
 * @DateTime   2022-05-24T15:30:01+0800
 * @param      [description]
 * @param      [description]
 */
void delete_self_stu_db(sqlite3 *db, char *Table)
{
	char *errmsg;
	char sql[1024];
	int result;
	
	sprintf(sql, "%s", Table);
	DBUG("%s\n", sql);
	result = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	DBUG("result %d\n", result);
}

/**
 * @fuction    从数据库中删除所有数据
 * @Author     wlb
 * @DateTime   2022-05-24T15:30:01+0800
 * @param      [description]
 * @param      [description]
 */
void delete_all_stu_db(sqlite3 *db)
{
	char *errmsg;
	char sql[100];
	int result;
	
	sprintf(sql, "delete from register_info");
	//DBUG("%s\n", sql);
	result = sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	//DBUG("result %d\n", result);
}

/**
 * @fuction    sqlite_callback 回调函数
 * @Author     wlb
 * @DateTime   2022-05-24T15:31:09+0800
 * @param      [description]
 * @param      [description]
 * @param      [description]
 * @param      [description]
 * @return    [description]
 */
int  show_stu_info(void *para, int n_column, char **column_value, char **column_name)
{
	int i;
	for(i = 0; i<n_column; i++)
	{
		//printf("%s\t", column_value[2]);
			
	}
	memcpy(para, column_value[2], strlen(column_value[2]));
	//printf("strlen(column_value[i]=%d\n",strlen(column_value[2]));
	return 0;
}

/**
 * @fuction    查找数据，通过回调函数输出
 * @Author     wlb
 * @DateTime   2022-05-24T15:32:31+0800
 * @param      [description]
 * @param      [description]
 */
void find_stu_db(sqlite3 *db, STU *stu, char *Result)
{
	char *errmsg;
	int result;
	char sql[1024];
	
	sprintf(sql, "select * from register_info where method='%s'", stu->name);
	//DBUG("sql=%s\n", sql);
	result = sqlite3_exec(db, sql, show_stu_info, (void *)Result, &errmsg);
	//DBUG("result=%d\n", result);

}
/**
 * @fuction    查找指定的数据表中的数据，通过回调函数输出
 * @Author     wlb
 * @DateTime   2022-05-24T15:32:31+0800
 * @param      [description]
 * @param      [description]
 */
void find_self_stu_db(sqlite3 *db, char *table, char *Result)
{
	char *errmsg;
	int result;
	char sql[1024];
	
	sprintf(sql, "%s", table);
	//DBUG("sql=%s\n", sql);
	result = sqlite3_exec(db, sql, show_stu_info, (void *)Result, &errmsg);
	//DBUG("result=%d\n", result);

}

/**
 * @fuction    查找数据，不经过回调函数把数据返回，直接从dbresult中读取
 * @Author     wlb
 * @DateTime   2022-05-24T15:33:16+0800
 * @param      [description]
 * @param      [description]
 * @return    [description]
 */
int find_stu_db_get_table(sqlite3 *db, STU *stu)
{
	char *errmsg, **dbresult;
	int result, nrow, ncolumn;
	int i, j, index=0;
	char sql[100];
	
	sprintf(sql, "select * from register_info where [id] ='%d'", stu->id);
	DBUG("sql=%s\n", sql);
	
	result = sqlite3_get_table(db, sql, &dbresult, &nrow, &ncolumn, &errmsg);
	DBUG("result=%d\n", result);
	if (NULL != errmsg)
	{
		printf("nrow = %d, ncolumn = %d\n", nrow, ncolumn);
		if(nrow == 0 || ncolumn == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	/*
		for (i=0; i<nrow+1; i++)
		{
			for (j=0; j<ncolumn; j++)
			{
				printf("%s\t", dbresult[index]);
				index++;
			}
			printf("\n");
		}
	*/
	}
	sqlite3_free_table(dbresult);
}

/**
 * @fuction    排序，通过回调函数输出
 * @Author     wlb
 * @DateTime   2022-05-24T15:33:50+0800
 * @param      [description]
 */
void order_by_id_stu_db_exec(sqlite3 *db)
{
	char *errmsg;
	int result;
	char *sql = "select * from register_info order by name";
	
	result = sqlite3_exec(db, sql, show_stu_info, NULL, &errmsg);
	DBUG("result=%d\n", result);

}

/**
 * @fuction    查找数据，不经过回调函数把数据返回，直接从dbresult中读取
 * @Author     wlb
 * @DateTime   2022-05-24T15:34:18+0800
 * @param      [description]
 */
void order_by_id_stu_db_get_table(sqlite3 *db)
{
	char *errmsg, **dbresult;
	int result, nrow, ncolumn;
	int i, j, index=0;
	char *sql = "select * from register_info order by id";
	
	result = sqlite3_get_table(db, sql, &dbresult, &nrow, &ncolumn, &errmsg);
	DBUG("result=%d\n", result);
	if (NULL != errmsg)
	{
		for (i=0; i<nrow+1; i++)
		{
			for (j=0; j<ncolumn; j++)
			{
				printf("%s\t", dbresult[index]);
				index++;
			}
			printf("\n");
		}
	}
	sqlite3_free_table(dbresult);
}

/**
 * @fuction    输出数据库所有数据，通过回调函数输出
 * @Author     wlb
 * @DateTime   2022-05-24T15:34:37+0800
 * @param      [description]
 * @param      [description]
 */
void print_stu_db(sqlite3 *db, char *db_name)
{
	char *errmsg;
	char sql[100];
	int result;
	
	sprintf(sql, "select * from register_info %s", db_name);
	DBUG("sql %s\n", sql);
	result = sqlite3_exec(db, sql, show_stu_info, NULL, &errmsg);
	DBUG("result=%d\n", result);
}

/**
 * @fuction    输出数据库所有数据，不经过回调函数把数据返回，直接从dbresult中读取
 * @Author     wlb
 * @DateTime   2022-05-24T15:35:10+0800
 * @param      [description]
 * @param      [description]
 */
void print_stu_db_get_table(sqlite3 *db, char *db_name)
{
	char *errmsg, **dbresult;
	int result, nrow, ncolumn;
	int i, j, index=0;
	char sql[100];
	
	sprintf(sql, "select * from register_info %s", db_name);
	result = sqlite3_get_table(db, sql, &dbresult, &nrow, &ncolumn, &errmsg);
	DBUG("result=%d\n", result);
	
	if (NULL != errmsg)
	{
		for (i=0; i<nrow+1; i++)
		{
			for (j=0; j<ncolumn; j++)
			{
				printf("%s\t", dbresult[index]);
				index++;
			}
			printf("\n");
		}
	}
	sqlite3_free_table(dbresult);
}
