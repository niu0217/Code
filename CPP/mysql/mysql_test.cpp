/* ************************************************************************
> File Name:     mysql_test.cpp
> Author:        niu0217
> Created Time:  Thu 27 Jun 2024 09:29:15 PM CST
> Description:   
 ************************************************************************/

#include <mysql/mysql.h>
#include <stdio.h>
#include <iostream>
using namespace std;

MYSQL* getMYSQLHandle()
{
  MYSQL *conn;
  conn = mysql_init(NULL);
  if(conn == nullptr)
  {
    fprintf(stderr, "mysql_init() failed\n");
    exit(1);
  }
  return conn;
}

void connectToMysqlServer(MYSQL *conn)
{
  if(mysql_real_connect(conn, "localhost", "root", "001125", NULL, 3306, NULL, 0) == NULL)
  {
    fprintf(stderr, "mysql_real_connect() failed\n");
    mysql_close(conn);
    exit(1);
  }
}

void connectToMysqlServer(MYSQL *conn, string dbname)
{
  if(mysql_real_connect(conn, "localhost", "root", "001125", dbname.c_str(), 3306, NULL, 0) == NULL)
  {
    fprintf(stderr, "mysql_real_connect() failed\n");
    mysql_close(conn);
    exit(1);
  }
}

// 创建数据库：TestDev
void testCreateDB()
{
  MYSQL *conn;
  
  // 初始化MySQL连接句柄
  conn = mysql_init(NULL);
  if(conn == nullptr)
  {
    fprintf(stderr, "mysql_init() failed\n");
    exit(1);
  }

  // 连接MySQL服务器
  if(mysql_real_connect(conn, "localhost", "root", "001125", NULL, 3306, NULL, 0) == NULL)
  {
    fprintf(stderr, "mysql_real_connect() failed\n");
    mysql_close(conn);
    exit(1);
  }

  // 创建名为 “TestDev” 的数据库 存在则不创建
  if(mysql_select_db(conn, "TestDev") == 0)
  {
    printf("Databse 'TestDev' already exites.\n");
    return;
  }
  else
  {
    if(mysql_query(conn, "CREATE DATABASE TestDev"))
    {
      fprintf(stderr, "Failed to create database: %s\n", mysql_error(conn));
      mysql_close(conn);
      exit(1);
    }
  }

  printf("Databse 'TestDev' created successfully.\n");

  mysql_close(conn);
}

// 在数据库 TestDev 中创建 order 表
void testCreateTable()
{
  MYSQL *conn;
  conn = getMYSQLHandle();
  connectToMysqlServer(conn, "TestDev");

  const char *query = "CREATE TABLE IF NOT EXISTS `order` ("
                      "name VARCHAR(50), "
                      "age INT, "
                      "address VARCHAR(100)"
                      ")";
  if(mysql_query(conn, query))
  {
    fprintf(stderr, "Failed to create table: %s\n", mysql_error(conn));
    mysql_close(conn);
    exit(1);
  }

  printf("Table 'order' created successfully.\n");
  mysql_close(conn);
}

// 向 TestDev 的 order 表中插入记录
void testInsertItems()
{
  MYSQL *conn;
  conn = getMYSQLHandle();
  connectToMysqlServer(conn, "TestDev");

  const char *query = "INSERT INTO TestDev.`order` "
                      "(name,age,address) "
                      "VALUES ('lisi', 30, 'chongqing')";
  if(mysql_query(conn, query))
  {
    fprintf(stderr, "Failed to insert item: %s\n", mysql_error(conn));
    mysql_close(conn);
    exit(1);
  }

  printf("Insert info to order successfully.\n");
  mysql_close(conn);
}

// 显示 order 表中的内容
void testShowTablesContext()
{
  MYSQL *conn;
  conn = getMYSQLHandle();
  connectToMysqlServer(conn, "TestDev");

  const char *query = "SELECT * FROM TestDev.`order`";
  if(mysql_query(conn, query))
  {
    fprintf(stderr, "Failed to get order info: %s\n", mysql_error(conn));
    mysql_close(conn);
    exit(1);
  }

  MYSQL_RES *result;
  MYSQL_ROW row;
  result = mysql_store_result(conn);
  while((row = mysql_fetch_row(result)))
  {
    for(unsigned int i = 0; i < mysql_num_fields(result); i++)
    {
      printf("%s ", row[i] ? row[i] : "NULL");
    }
    printf("\n");
  }

  mysql_free_result(result);
  mysql_close(conn);
}

// 从 TestDev 的 order 表中删除记录
void testDeleteItems()
{
  MYSQL *conn;
  conn = getMYSQLHandle();
  connectToMysqlServer(conn, "TestDev");

  const char *query = "DELETE FROM TestDev.`order` WHERE name='lisi'";
  if(mysql_query(conn, query))
  {
    fprintf(stderr, "Failed to delete order info: %s\n", mysql_error(conn));
    mysql_close(conn);
    exit(1);
  }

  printf("Delete info from order successfully.\n");
  mysql_close(conn);
}


int main()
{
  testCreateDB();

  testCreateTable();

  testInsertItems();

  testShowTablesContext();

  testDeleteItems();
  
  testInsertItems();
  testShowTablesContext();
}