#include "../../include/model/share_model.hpp"
#include "../../include/db/db.hpp"
#include "../../include/common.hpp"

/*
 * share item
 *
 * */
ShareItem::ShareItem() {}

ShareItem::ShareItem(unsigned int share_id, const std::string& user_id, const std::string& head, const std::string& ctime, unsigned int read) 
    : _share_id(share_id)
    , _user_id(user_id)
    , _head(head)
    , _ctime(ctime)
    , _read(read)
{}


void ShareItem::SetItemInfo(const std::string& info, unsigned int like, unsigned int unlike) {
    _info = info;
    _like = like;
    _unlike = unlike;
}

unsigned int ShareItem::GetShareId() const {
    return _share_id;
}
std::string ShareItem::GetUserId() const {
    return _user_id;
}
std::string ShareItem::GetShareCTime() const {
    return _ctime;
}
std::string ShareItem::GetShareHead() const {
    return _head;
}
unsigned int ShareItem::GetShareRead() const {
    return _read;
}

void ShareItem::GetShareInfo(std::string& info) {
    _info = info;
}
unsigned int ShareItem::GetShareLike() const {
    return _like;
}
unsigned int ShareItem::GetShareUnlike() const {
    return _unlike;
}

ShareItem::~ShareItem() {}


/*
 * share model
 *
 * */
ShareModel::ShareModel() {}

bool ShareModel::GetShareList(std::list<ShareItem>& allshares) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "select share_id, share_createtime, user_id, share_head, share_read from share_tb where share_status = %d order by share_createtime desc;", STATUS_EXIST);
    // 查询数据库
    MYSQL_RES* result = sql_conn.query(sql);
    if (result != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != nullptr) {
            unsigned int share_id = atoi(row[0]);
            const std::string ctime = row[1];
            const std::string user_id = row[2];
            const std::string head = row[3];
            unsigned int read = atoi(row[4]);
            ShareItem item(share_id, user_id, head, ctime, read);
            allshares.push_back(item);
        }
        mysql_free_result(result);
    } else {
        return false;
    }
    return true;
}

bool ShareModel::GetShare(unsigned share_id, ShareItem& share) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "select share_id, share_createtime, user_id, share_head, share_info, share_read, share_like, share_unlike from share_tb where share_id = %u and share_status=%d;", share_id, STATUS_EXIST);
    // 查询数据库
    MYSQL_RES* result = sql_conn.query(sql);
    if (result != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != nullptr) {
            unsigned int share_id = atoi(row[0]);
            const std::string ctime = row[1];
            const std::string user_id = row[2];
            const std::string head = row[3];
            const std::string info = row[4];
            unsigned int read = atoi(row[5]);
            unsigned int like = atoi(row[6]);
            unsigned int unlike = atoi(row[7]);
            share = ShareItem(share_id, user_id, head, ctime, read);
            share.SetItemInfo(info, like, unlike);
            break;
        }
        mysql_free_result(result);
    } else {
        return false;
    }
    // get remark
    // TODO
    return true;
}
bool ShareModel::AddShare(const std::string& user_id, const std::string& head, const std::string& info, std::list<ShareItem>& allshares) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "insert into share_tb(user_id, share_head, share_info) values('%s', '%s', '%s');", user_id.c_str(), head.c_str(), info.c_str());
    if (!sql_conn.update(sql)) {
        return false;
    }
    return GetShareList(allshares);
}
bool ShareModel::DelShare(unsigned int share_id, const std::string& user_id, std::list<ShareItem>& allshares) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "update share_tb set share_status = %d where share_id = %u and user_id = '%s';", STATUS_DELETE, share_id, user_id.c_str());
    if (!sql_conn.update(sql)) {
        return false;
    }
    return GetShareList(allshares);
}

ShareModel::~ShareModel() {}
