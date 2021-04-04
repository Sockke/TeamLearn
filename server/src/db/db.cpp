#include "../../include/db/db.hpp"

#include <muduo/base/Logging.h>
using namespace muduo;

DB::DB() {
    _conn = mysql_init(nullptr);
}

DB::~DB() {
    if (_conn != nullptr) {
        mysql_close(_conn);
    }
}
// 连接数据库
bool DB::connect() {
    MYSQL* p = mysql_real_connect(_conn, server.c_str(), user.c_str(), passwd.c_str(),
        dbname.c_str(), 3306, nullptr, 0);
    if (p == nullptr) {
        LOG_ERROR << __FILE__ << "-" << __LINE__ << ": connect error";
        return false;
    } else {
        mysql_query(_conn, "set names gbk");
        return true;
    }
}
// 更新操作
bool DB::update(const std::string &sql) {
    if (mysql_query(_conn, sql.c_str())) {
        LOG_ERROR << __FILE__ << "-" << __LINE__ << ":" << sql << " update error";
        return false;
    } else {
        return true;
    }
}
// 查询操作
MYSQL_RES* DB::query(const std::string &sql) {
    if (mysql_query(_conn, sql.c_str()) != 0) {
        LOG_ERROR << __FILE__ << "-" << __LINE__ << ":" << sql << " query error";
        return nullptr;
    } else {
        return mysql_use_result(_conn);
    }
}

// 获取该连接
MYSQL* DB::getConnection() const {return _conn;}
