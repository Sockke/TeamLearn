#include "../../include/model/remark_model.hpp"
#include "../../include/common.hpp"
#include "../../include/db/db.hpp"

#include <iostream>
#include <cstring>

/*
 * remark item
 *
 * */
RemarkItem::RemarkItem() {}
RemarkItem::RemarkItem(unsigned int remark_id, const std::string& ctime, unsigned int dynamic_id, const std::string& user_id, const std::string& info) 
    : _remark_id(remark_id)
    , _ctime(ctime)
    , _dynamic_id(dynamic_id)
    , _user_id(user_id)
    , _info(info)
{}
bool RemarkItem::operator<(const RemarkItem& remark_item) const {
    return _remark_id < remark_item._remark_id;
}
unsigned int RemarkItem::GetRemarkId() const {
    return _remark_id;
}
std::string RemarkItem::GetRemarkCTime() const {
    return _ctime;
}
std::string RemarkItem::GetUserId() const {
    return _user_id;
}
std::string RemarkItem::GetRemarkInfo() const {
    return _info;
}
RemarkItem::~RemarkItem() {}

/*
 * reply item
 *
 * */
ReplyItem::ReplyItem() {}
ReplyItem::ReplyItem(unsigned int reply_id, const std::string& ctime, const std::string& user_id, unsigned int reply_remark_id, const std::string& info) 
    : _reply_id(reply_id)
    , _ctime(ctime)
    , _user_id(user_id)
    , _reply_remark_id(reply_remark_id)
    , _info(info)
{}
unsigned int ReplyItem::GetReplyId() const {
    return _reply_id;
}
std::string ReplyItem::GetReplyCTime() const {
    return _ctime;
}
std::string ReplyItem::GetUserId() const {
    return _user_id;
}
unsigned int ReplyItem::GetReplyRemarkId() const {
    return _reply_remark_id;
}
std::string ReplyItem::GetReplyInfo() const {
    return _info;
}
ReplyItem::~ReplyItem() {}


/*
 * remark model
 *
 * */
RemarkModel::RemarkModel() {}

bool RemarkModel::GetRemarkList(unsigned int dynamic_id, int dynamic_type, std::map<const RemarkItem, std::vector<ReplyItem>>& remark_map) {
    std::map<unsigned int, RemarkItem> remark_list;
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }
    // get remark
    char sql[1024] = {0};
    sprintf(sql, "select remark_id, remark_createtime, user_id, remark_info from remark_tb where remark_type = %d and dynamic_id = %u and dynamic_type = %d and remark_status=%d;", REMARK_REMARK, dynamic_id, dynamic_type, STATUS_EXIST);
    // 查询数据库
    MYSQL_RES* result = sql_conn.query(sql);
    if (result != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != nullptr) {
            unsigned int remark_id = atoi(row[0]);
            std::string ctime = row[1];
            std::string user_id = row[2];
            std::string info = row[3];
            RemarkItem remark_item(remark_id, ctime, dynamic_id, user_id, info);
            remark_list.insert({remark_id, remark_item});
            remark_map.insert({remark_item, std::vector<ReplyItem>()});
        }
        mysql_free_result(result);
    } else {
        return false;
    }
    memset(sql, 0x00, 1024);
    sprintf(sql, "select remark_id, remark_createtime, user_id, reply_remark_id, remark_info from remark_tb where remark_type = %d and dynamic_id = %u and dynamic_type = %d and remark_status=%d;", REMARK_REPLY, dynamic_id, dynamic_type, STATUS_EXIST);
    // 查询数据库
    result = sql_conn.query(sql);
    if (result != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != nullptr) {
            unsigned int remark_id = atoi(row[0]);
            std::string ctime = row[1];
            std::string user_id = row[2];
            unsigned int reply_remark_id = atoi(row[3]);
            std::string info = row[4];
            ReplyItem reply_item(remark_id, ctime, user_id, reply_remark_id, info);
            remark_map[remark_list[reply_remark_id]].push_back(reply_item);
        }
        mysql_free_result(result);
    } else {
        return false;
    }
    return true;
}
bool RemarkModel::AddRemark(unsigned int dynamic_id, int dynamic_type, const std::string& user_id, const std::string& info) {
   // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }
    char sql[1024] = {0};
    sprintf(sql, "insert into remark_tb(remark_type, dynamic_id, dynamic_type, user_id, remark_info) values(%d, %u, %d, '%s', '%s');", REMARK_REMARK, dynamic_id, dynamic_type, user_id.c_str(), info.c_str());
    if (!sql_conn.update(sql)) {
        return false;
    }
    return true;
}
bool RemarkModel::AddReply(unsigned int dynamic_id, int dynamic_type, const std::string& user_id, unsigned int remark_id, const std::string& info) {
   // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }
    char sql[1024] = {0};
    sprintf(sql, "insert into remark_tb(remark_type, dynamic_id, dynamic_type, user_id, reply_remark_id, remark_info) values(%d, %u, %d, '%s', %u, '%s');", REMARK_REPLY, dynamic_id, dynamic_type, user_id.c_str(), remark_id, info.c_str());
    if (!sql_conn.update(sql)) {
        return false;
    }
    return true;

}
bool RemarkModel::DelRemarkList(unsigned int dynamic_id, int dynamic_type) {
   // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }
    char sql[1024] = {0};
    sprintf(sql, "update remark_tb set remark_status = %d where dynamic_id = %u and dynamic_type = %d;", STATUS_DELETE, dynamic_id, dynamic_type);
    if (!sql_conn.update(sql)) {
        return false;
    }
    return true;
}
bool RemarkModel::DelRemark(unsigned int remark_id, const std::string& user_id) {
   // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }
    char sql[1024] = {0};
    sprintf(sql, "update remark_tb set remark_status = %d where remark_id = %u and user_id = '%s';", STATUS_DELETE, remark_id, user_id.c_str());
    if (!sql_conn.update(sql)) {
        return false;
    }
    return true;
}
RemarkModel::~RemarkModel() {}
