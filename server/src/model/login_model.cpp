#include "../../include/model/login_model.hpp"
#include "../../include/db/db.hpp"
#include "../../include/common.hpp"
#include <cstring>


LoginModel::LoginModel() {}

bool LoginModel::Login(const std::string& user_id, const std::string& password) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "select user_password from user_tb where user_id = '%s' and user_status = %d;", user_id.c_str(), STATUS_EXIST); 

    // 查询数据库
    MYSQL_RES* result = sql_conn.query(sql);
    if (result != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != nullptr) {
            if (row[0] != password) {
                return false;
            }
            break;
        }
        mysql_free_result(result);
    } else {
        return false;
    }
    return true;
}

bool LoginModel::Register(const std::string& user_id, const std::string& password, const std::string& name, int gender) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024];
    memset(sql, 0x00, 1024);
    sprintf(sql, "insert into user_tb(user_id, user_name, user_password, user_gender) values ('%s', '%s', '%s', '%d')", user_id.c_str(), name.c_str(), password.c_str(), gender); 
    return sql_conn.update(sql);
}

bool LoginModel::EditInfo(const std::string& user_id, const std::string& name, int gender, const std::string& icon, const std::string& email) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "update user_tb set user_name = '%s', user_gender = '%d', user_icon = '%s', user_email = '%s' where user_id = '%s' and user_status = %d;", name.c_str(), gender, icon.c_str(), email.c_str(), user_id.c_str(), STATUS_EXIST); 
    return sql_conn.update(sql);
}

bool LoginModel::ModifyPassword(const std::string& user_id, const std::string& user_password, const std::string& new_user_password) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "update user_tb set user_password = '%s' where user_id = '%s' and user_password = '%s' and user_status = %d;", new_user_password.c_str(), user_id.c_str(), user_password.c_str(), STATUS_EXIST); 
    return sql_conn.update(sql);
}

bool LoginModel::ModifyFocusAndFans(const std::string& user_id, unsigned int focus, unsigned int fans) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "update user_tb set user_focus = %u, user_fans = %u where user_id = %s and user_status = %d;", focus, fans, user_id.c_str(), STATUS_EXIST); 
    return sql_conn.update(sql);
}

LoginModel::~LoginModel() {}
