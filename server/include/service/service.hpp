#ifndef SERVICE_H
#define SERVICE_H

#include <iostream>
#include <functional>
#include <unordered_map>
#include <mutex>
#include <muduo/net/TcpConnection.h>
#include "../json.hpp"
#include "../model/login_model.hpp"
#include "../model/share_model.hpp"
#include "../model/qa_model.hpp"
#include "../model/remark_model.hpp"

using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;


/*
业务类
*/
enum SERVICE_ID {
    LOGIN,
    REGISTER,
    EDIT_USER_INFO,
    MODIFY_PASSWORD,
    GET_SHARE_LIST,
    GET_SHARE,
    ADD_SHARE,
    DEL_SHARE,
    GET_QUESTION_LIST,
    ADD_QUESTION,
    GET_ANSWER_LIST,
    ADD_ANSWER,
    DEL_QUESTION,
    DEL_ANSWER,
    ADD_REMARK,
};

using SERVICE_HANDLER = std::function<void(const json&, json&, Timestamp)>;

// 业务模块设置成单例模式
class Service {
public:
    static Service* instance();
    // 业务的分发
    void serviceHandler(int msgid, const json&, json&, Timestamp);
    // 客户端主动退出
    //void clientQuit(const TcpConnectionPtr&);
    // 在线用户清除
    //void reset();
private:
    Service();
    Service(const Service&) = delete;
    Service& operator=(const Service&) = delete;

    // 业务声明
    // user
    void Login(const json& req, json& rsp, Timestamp timestamp);
    void Register(const json& req, json& rsp, Timestamp timestamp);
    void EditUserInfo(const json& req, json& rsp, Timestamp timestamp);
    void ModifyPassword(const json& req, json& rsp, Timestamp timestamp);

    // share
    void GetShareList(const json& req, json& rsp, Timestamp timestamp); 
    void GetShare(const json& req, json& rsp, Timestamp timestamp);
    void AddShare(const json& req, json& rsp, Timestamp timestamp);
    void DelShare(const json& req, json& rsp, Timestamp timestamp);

    // qa
    void GetQuestionList(const json& req, json& rsp, Timestamp timestamp);
    void AddQuestion(const json& req, json& rsp, Timestamp timestamp);
    void GetAnswerList(const json& req, json& rsp, Timestamp timestamp);
    void AddAnswer(const json& req, json& rsp, Timestamp timestamp);
    void DelQuestion(const json& req, json& rsp, Timestamp timestamp);
    void DelAnswer(const json& req, json& rsp, Timestamp timestamp);

    // remark
    void AddRemark(const json& req, json& rsp, Timestamp timestamp);

    // msgid -> handler
    std::unordered_map<int, SERVICE_HANDLER> _serviceMap;
    
    // model对象, 间接操作数据库
    LoginModel _login_model;
    ShareModel _share_model;
    QAModel _qa_model;
    RemarkModel _remark_model;
};

#endif
