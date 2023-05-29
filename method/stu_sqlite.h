#ifndef __STU_SQLITE_H__
#define __STU_SQLITE_H__
#include "sqlite3.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct student_info
{
	int id;
	char name[64];
	char val[256];
	char time[64];
}STU;

//#define DEBUG
#ifdef DEBUG
#define DBUG(fmt, arg...) printf("[%s:%d]"fmt, __func__, __LINE__, ##arg)
#else
#define DBUG(fmt, arg...) 
#endif
extern void delete_all_stu_db(sqlite3 *db);
extern void insert_stu_db(sqlite3 *db, STU *stu);
extern void delete_stu_db(sqlite3 *db, STU *stu);
extern void order_by_id_stu_db_get_table(sqlite3 *db);
extern void find_stu_db(sqlite3 *db, STU *stu, char *Result);
extern int find_stu_db_get_table(sqlite3 *db, STU *stu);
extern void print_stu_db_get_table(sqlite3 *db, char *db_name);
extern sqlite3 *create_db(char *db_name);
extern void create_db_table(sqlite3 *db);
extern void update_stu_db(sqlite3 *db, STU *stu);

extern void create_self_db_table(sqlite3 *db,char *Table);
extern void find_self_stu_db(sqlite3 *db, char *table, char *Result);
extern void delete_self_stu_db(sqlite3 *db, char *Table);
extern void update_self_stu_db(sqlite3 *db, char *Table);
extern void insert_self_stu_db(sqlite3 *db, char *Table);

#ifdef __cplusplus
}  /* end of the 'extern "C"' block */
#endif
#endif
