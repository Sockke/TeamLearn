#include "../../include/model/qa_model.hpp"
#include "../../include/db/db.hpp"
#include "../../include/common.hpp"
#include <cstring>


/*
 * question
 *
 * */
Question::Question() {}

Question::Question(unsigned int q_id, const std::string& ctime, const std::string& user_id, const std::string& head, unsigned int views)
    : _q_id(q_id)
    , _ctime(ctime)
    , _user_id(user_id)
    , _head(head)
    , _views(views)
{}

void Question::SetQuestionInfo(const std::string& info, unsigned int like, unsigned int unlike) {
    _info = info;
    _like = like;
    _unlike = unlike;
}

void Question::SetVotes(unsigned int votes) {
    _votes = votes;
}

unsigned int Question::GetQId() const {
    return _q_id;
}
std::string Question::GetQCTime() const {
    return _ctime;
}
std::string Question::GetUserId() const {
    return _user_id;
}
std::string Question::GetQHead() const {
    return _head;
}
unsigned int Question::GetQViews() const {
    return _views;
}
unsigned int Question::GetQVotes() const {
    return _votes;
}
void Question::GetQInfo(std::string& info) {
    _info = info;
}
unsigned int Question::GetQLike() const {
    return _like;
}
unsigned int Question::GetQUnlike() const {
    return _unlike;
}

Question::~Question() {}

/*
 * answer
 *
 * */
Answer::Answer(unsigned a_id, const std::string& ctime, unsigned int q_id, const std::string& user_id, const std::string& info, unsigned int like, unsigned int unlike) 
    : _a_id(a_id)
    , _ctime(ctime)
    , _q_id(q_id)
    , _user_id(user_id)
    , _info(info)
    , _like(like)
    , _unlike(unlike)
{}
unsigned int Answer::GetAId() const {
    return _a_id;
}
std::string Answer::GetACTime() const {
    return _ctime;
}
unsigned int Answer::GetQId() const {
    return _q_id;
}
std::string Answer::GetUserId() const {
    return _user_id;
}
void Answer::GetAInfo(std::string& info) {
    _info = info;
}
unsigned int Answer::GetALike() const {
    return _like;
}
unsigned int Answer::GetAUnlike() const {
    return _unlike;
}
Answer::~Answer() {}


/*
 * qa model
 *
 * */
QAModel::QAModel() {}

bool QAModel::GetQuestionList(std::list<Question>& qList) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "select question_id, question_createtime, user_id, question_head, question_views, question_like, question_unlike from question_tb where question_status = %d;", STATUS_EXIST); 
    // 查询数据库
    MYSQL_RES* result = sql_conn.query(sql);
    if (result != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != nullptr) {
            unsigned int q_id = atoi(row[0]);
            std::string ctime = row[1];
            std::string user_id = row[2];
            std::string head = row[3];
            unsigned int views = atoi(row[4]);
            unsigned int votes = atoi(row[5]) + atoi(row[6]);
            Question q(q_id, ctime, user_id, head, views);
            q.SetVotes(votes);
            qList.push_back(q);
        }
        mysql_free_result(result);
    } else {
        return false;
    }
    return true;
}

bool QAModel::GetAnswerList(unsigned int q_id, Question& q, std::list<Answer>& aList) {
    // 组装sql
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "select question_id, question_createtime, user_id, question_head, question_info, question_like, question_unlike from question_tb where question_id = %u and question_status = %d;", q_id, STATUS_EXIST); 

    // 查询数据库
    MYSQL_RES* result = sql_conn.query(sql);
    if (result != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != nullptr) {
            unsigned int q_id = atoi(row[0]);
            std::string ctime = row[1];
            std::string user_id = row[2];
            std::string head = row[3];
            std::string info = row[4];
            unsigned int like = atoi(row[5]);
            unsigned int unlike = atoi(row[6]);
            q = Question(q_id, ctime, user_id, head);
            q.SetQuestionInfo(info, like, unlike);
            break;
        }
        mysql_free_result(result);
    } else {
        return false;
    }
    memset(sql, 0x00, 1024);
    sprintf(sql, "select answer_id, answer_createtime, question_id, user_id, answer_info, answer_like, answer_unlike from answer_tb where question_id = %u and answer_status = %d;", q_id, STATUS_EXIST);
    // 查询数据库
    result = sql_conn.query(sql);
    if (result != nullptr) {
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(result)) != nullptr) {
            unsigned int a_id = atoi(row[0]);
            std::string ctime = row[1];
            unsigned int q_id = atoi(row[2]);
            std::string user_id = row[3];
            std::string info = row[4];
            unsigned int like = atoi(row[5]);
            unsigned int unlike = atoi(row[6]);
            Answer a(a_id, ctime, q_id, user_id, info, like, unlike);
            aList.push_back(a);
        }
        mysql_free_result(result);
    } else {
        return false;
    }
    return true;
}
bool QAModel::AddQuestion(const std::string& user_id, const std::string& head, const std::string& info, std::list<Question>& qList) {
    // add
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024];
    memset(sql, 0x00, 1024);
    sprintf(sql, "insert into question_tb(user_id, question_head, question_info) values ('%s', '%s', \"%s\")", user_id.c_str(), head.c_str(), info.c_str()); 
    if (!sql_conn.update(sql)) {
        return false;
    }
    return GetQuestionList(qList);
}

bool QAModel::AddAnswer(unsigned int q_id, const std::string& user_id, const std::string& info, Question& q, std::list<Answer>& aList) {
    // judge
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024] = {0};
    sprintf(sql, "select question_id from question_tb where question_id = %u and question_status = %d;", q_id, STATUS_EXIST); 
    // 查询数据库
    MYSQL_RES* result = sql_conn.query(sql);
    if (result != nullptr) {
        MYSQL_ROW row = mysql_fetch_row(result);
        if (row == nullptr) {
            return false;
        }
        mysql_free_result(result);
    } else {
        return false;
    }
    // add
    memset(sql, 0x00, 1024);
    sprintf(sql, "insert into answer_tb(question_id, user_id, answer_info) values(%u, '%s', '%s');", q_id, user_id.c_str(), info.c_str());
    if (!sql_conn.update(sql)) {
        return false;
    }
    return GetAnswerList(q_id, q, aList);
}

bool QAModel::DelQuestion(unsigned int q_id, const std::string& user_id, std::list<Question>& qList) {
    // update
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024];
    memset(sql, 0x00, 1024);
    sprintf(sql, "update question_tb set question_status = %d where question_id = %u and user_id = '%s';", STATUS_DELETE, q_id, user_id.c_str());
    if (!sql_conn.update(sql)){
        return false;
    }
    memset(sql, 0x00, 1024);
    sprintf(sql, "update answer_tb set answer_status = %d where question_id = %u;", STATUS_DELETE, q_id );
    if (!sql_conn.update(sql)) {
        return false;
    } 
    return GetQuestionList(qList);
}

bool QAModel::DelAnswer(unsigned int a_id, unsigned int q_id, const std::string& user_id, Question& q, std::list<Answer>& aList) {
    // update
    DB sql_conn;
    if (!sql_conn.connect()) {
        return false;
    }    
    char sql[1024];
    memset(sql, 0x00, 1024);
    sprintf(sql, "update answer_tb set answer_status = %d where answer_id = %u and user_id = '%s';", STATUS_DELETE, a_id, user_id.c_str());
    if (!sql_conn.update(sql)) {
        return false;
    }
    return GetAnswerList(q_id, q, aList);
}

QAModel::~QAModel() {}
