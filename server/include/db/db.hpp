#ifndef DB_H
#define DB_H

#include <iostream>
#include <string>
#include "mysql/mysql.h"

/*
数据库类
*/

static std::string server = "127.0.0.1";
static std::string user = "root";
static std::string passwd = "034917";
static std::string dbname = "team_study_system_db";

class DB {
public:
    DB();
    ~DB();
    // 连接数据库
    bool connect();
    // 更新操作
    bool update(const std::string& sql);
    // 查询操作
    MYSQL_RES* query(const std::string& sql);

    // 获取该连接
    MYSQL* getConnection() const;
private:
 
    //DB(const DB&) = delete;
    //DB& operator=(const DB&) = delete;

    MYSQL* _conn;
};

#endif
