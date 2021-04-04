#include "../../include/service/service.hpp"

#include "../../include/common.hpp"

#include <muduo/base/Logging.h>
using namespace muduo;

Service* Service::instance() {
    static Service chatService;
    return &chatService;
}

Service::Service() {
    // 业务的注册
    _serviceMap[LOGIN] = std::bind(&Service::Login, this, _1, _2, _3);
    _serviceMap[REGISTER] = std::bind(&Service::Register, this, _1, _2, _3);
    _serviceMap[EDIT_USER_INFO] = std::bind(&Service::EditUserInfo, this, _1, _2, _3);
    _serviceMap[MODIFY_PASSWORD] = std::bind(&Service::ModifyPassword, this, _1, _2, _3);
    _serviceMap[GET_SHARE_LIST] = std::bind(&Service::GetShareList, this, _1, _2, _3);
    _serviceMap[GET_SHARE] = std::bind(&Service::GetShare, this, _1, _2, _3);
    _serviceMap[ADD_SHARE] = std::bind(&Service::AddShare, this, _1, _2, _3);
    _serviceMap[DEL_SHARE] = std::bind(&Service::DelShare, this, _1, _2, _3);
    _serviceMap[GET_QUESTION_LIST] = std::bind(&Service::GetQuestionList, this, _1, _2, _3);
    _serviceMap[ADD_QUESTION] = std::bind(&Service::AddQuestion, this, _1, _2, _3);
    _serviceMap[GET_ANSWER_LIST] = std::bind(&Service::GetAnswerList, this, _1, _2, _3);
    _serviceMap[ADD_ANSWER] = std::bind(&Service::AddAnswer, this, _1, _2, _3);
    _serviceMap[DEL_QUESTION] = std::bind(&Service::DelQuestion, this, _1, _2, _3);
    _serviceMap[DEL_ANSWER] = std::bind(&Service::DelAnswer, this, _1, _2, _3);
    _serviceMap[ADD_REMARK] = std::bind(&Service::AddRemark, this, _1, _2, _3);
}

// 业务的分发
void Service::serviceHandler(int msgid, const json& req, json& rsp, Timestamp timestamp) {
    if (_serviceMap.find(msgid) == _serviceMap.end()) {
        //LOG_ERROR << "no handler with msgid:" << msgid;
        // errcode + errmsg
        return;
    }
    _serviceMap[msgid](req, rsp, timestamp);
}

/*
 * user
 *
 * */

// login
void Service::Login(const json& req, json& rsp, Timestamp timestamp) {
    // get
    std::string user_id = req["user_id"];
    std::string user_password = req["user_password"];
    // pull
    bool ret = _login_model.Login(user_id, user_password);
    if (!ret) {
        rsp["errcode"] = LOGIN_FAILED;
        rsp["errmsg"] = "Username does not exist or password is wrong";
        return;
    }
    rsp["errcode"]  = USER_SUCCESS;
    return;
}

// register
void Service::Register(const json& req, json& rsp, Timestamp timestamp) {
    // get
    std::string user_id = req["user_id"];
    std::string user_password = req["user_password"];
    std::string user_name = req["user_name"];
    int user_gender = req["user_gender"];
    // pull
    bool ret = _login_model.Register(user_id, user_password, user_name, user_gender);
    if (!ret) {
        rsp["errcode"] = REG_FAILED;
        rsp["errmsg"] = "User id already exists, registration failed";
        return;
    }
    rsp["errcode"] = USER_SUCCESS;
    return;
}

// edit user info
void Service::EditUserInfo(const json& req, json& rsp, Timestamp timestamp) {
    // get
    std::string user_id = req["user_id"];
    std::string user_name = req["user_name"];
    int user_gender = req["user_gender"];
    std::string user_icon = req["user_icon"];
    std::string user_email = req["user_email"];
    // pull
    bool ret = _login_model.EditInfo(user_id, user_name, user_gender, user_icon, user_email);
    if (!ret) {
        rsp["errcode"] = EDIT_USER_INFO_FAILED;
        rsp["errmsg"] = "The modification information is abnormal";
        return;
    }
    rsp["errcode"] = USER_SUCCESS;
    return;
}

// modify password
void Service::ModifyPassword(const json& req, json& rsp, Timestamp timestamp) {
    // get
    std::string user_id = req["user_id"];
    std::string user_password = req["user_password"];
    std::string new_user_password = req["new_user_password"];
    // pull
    bool ret = _login_model.ModifyPassword(user_id, user_password, new_user_password);
    if (!ret) {
        rsp["errcode"] = MODIFY_PASSWORD_FAILED;
        rsp["errmsg"] = "Password modification failed, the original password is wrong";
        return;
    }
    rsp["errcode"] = USER_SUCCESS;
    return;
}

/*
 * share
 *
 * */
// get share list
void Service::GetShareList(const json& req, json& rsp, Timestamp timestamp) { 
    // get
    // pull
    // share list
    std::list<ShareItem> share_list;
    bool ret = _share_model.GetShareList(share_list);
    if (!ret) {
        rsp["errcode"] = GET_SHARE_LIST_FAILED;
        rsp["errmsg"] = "Sharing list gets abnormal";
        return;
    }
    std::vector<std::string> share_list_str;
    for (const ShareItem& e : share_list) {
        json share_item;
        share_item["share_id"] = e.GetShareId();
        share_item["share_createtime"] = e.GetShareCTime();
        share_item["user_id"] = e.GetUserId();
        share_item["share_head"] = e.GetShareHead();
        share_item["share_read"] = e.GetShareRead();
        share_list_str.push_back(share_item.dump());
    }
    rsp["share_list"] = share_list_str;
    rsp["errcode"] = SHARE_SUCCESS;
    return;
}

// add share
void Service::AddShare(const json& req, json& rsp, Timestamp timestamp)  {
    // get
    std::string user_id = req["user_id"];
    std::string share_head = req["share_head"];
    std::string share_info = req["share_info"];
    // pull
    std::list<ShareItem> share_list;
    bool ret = _share_model.AddShare(user_id, share_head, share_info, share_list);
    if (!ret) {
        rsp["errcode"] = ADD_SHARE_FAILED;
        rsp["errmsg"] = "Sharing failed";
        return;
    }
    std::vector<std::string> share_list_str;
    for (const ShareItem& e : share_list) {
        json share_item_str;
        share_item_str["share_id"] = e.GetShareId();
        share_item_str["share_createtime"] = e.GetShareCTime();
        share_item_str["user_id"] = e.GetUserId();
        share_item_str["share_head"] = e.GetShareHead();
        share_item_str["share_read"] = e.GetShareRead();
        share_list_str.push_back(share_item_str.dump());
    }
    rsp["share_list"] = share_list_str;
    rsp["errcode"] = SHARE_SUCCESS;
    return; 
}
// get share
void Service::GetShare(const json& req, json& rsp, Timestamp timestamp) {
    // get 
    unsigned int share_id = req["share_id"];
    // pull
    ShareItem share_item;
    bool ret = _share_model.GetShare(share_id, share_item);
    if (!ret) {
        rsp["errcode"] = GET_SHARE_FAILED;
        rsp["errmsg"] = "Failed to open share";
        return;
    }
    json share_item_str;
    share_item_str["share_id"] = share_item.GetShareId();
    share_item_str["share_createtime"] = share_item.GetShareCTime();
    share_item_str["user_id"] = share_item.GetUserId();
    share_item_str["share_head"] = share_item.GetShareHead();
    share_item_str["share_read"] = share_item.GetShareRead();
    std::string share_info;
    share_item.GetShareInfo(share_info);
    share_item_str["share_info"] = share_info;
    share_item_str["share_like"] = share_item.GetShareLike();
    share_item_str["share_unlike"] = share_item.GetShareUnlike();
    rsp["share_item"] = share_item_str.dump();

    // remark
    std::map<const RemarkItem, std::vector<ReplyItem>> remark_list;
    ret = _remark_model.GetRemarkList(share_id, DYNAMIC_SHARE, remark_list);
    if (!ret) {
        rsp["errcode"] = GET_SHARE_FAILED;
        rsp["errmsg"] = "Failed to get remarks";
        return;
    }
    std::cout << remark_list.size() << std::endl;
    std::vector<std::string> remark_list_str;
    for (std::pair<const RemarkItem, std::vector<ReplyItem>>& e : remark_list) {
        json remark_item; 
        remark_item["remark_id"] = e.first.GetRemarkId();
        remark_item["remark_createtime"] = e.first.GetRemarkCTime();
        remark_item["user_id"] = e.first.GetUserId();
        remark_item["remark_info"] = e.first.GetRemarkInfo();
        std::vector<std::string> reply_list_str;
        for (ReplyItem& r : e.second) {
            json reply_item;
            reply_item["reply_id"] = r.GetReplyId();
            reply_item["reply_createtime"] = r.GetReplyCTime();
            reply_item["user_id"] = r.GetUserId();
            reply_item["reply_remark_id"] = r.GetReplyRemarkId();
            reply_item["reply_info"] = r.GetReplyInfo();
            reply_list_str.push_back(reply_item.dump());
        }
        remark_item["reply_list"] = reply_list_str;
        remark_list_str.push_back(remark_item.dump());
    }
    rsp["remark_list"] = remark_list_str;
    rsp["errcode"] = SHARE_SUCCESS;
    return;
}
// del share
void Service::DelShare(const json& req, json& rsp, Timestamp timestamp) {
    // get
    unsigned int share_id = req["share_id"];
    std::string user_id = req["user_id"];
    // pull
    std::list<ShareItem> share_list;
    bool ret = _share_model.DelShare(share_id, user_id, share_list);
    if (!ret) {
        rsp["errcode"] = ADD_SHARE_FAILED;
        rsp["errmsg"] = "Delete share failed";
        return;
    }
    rsp["errcode"] = SHARE_SUCCESS;
    std::vector<std::string> share_list_str;
    for (const ShareItem& e : share_list) {
        json share_item_str;
        share_item_str["share_id"] = e.GetShareId();
        share_item_str["share_createtime"] = e.GetShareCTime();
        share_item_str["user_id"] = e.GetUserId();
        share_item_str["share_head"] = e.GetShareHead();
        share_item_str["share_read"] = e.GetShareRead();
        share_list_str.push_back(share_item_str.dump());
    }
    rsp["share_list"] = share_list_str;
    return; 
}

/*
 * qa
 *
 * */
// get question list
void Service::GetQuestionList(const json& req, json& rsp, Timestamp timestamp) {
    // get
    // pull
    std::list<Question> qList;
    bool ret = _qa_model.GetQuestionList(qList);
    if (!ret) {
        rsp["errcode"] = GET_QUESTION_LIST_FAILED;
        rsp["errmsg"] = "Get question list exception";
        return;
    }
    rsp["errcode"]  = QA_SUCCESS;
    std::vector<std::string> qList_str;
    for (const Question& e : qList) {
        json q_str;
        q_str["question_id"] = e.GetQId();
        q_str["question_createtime"] = e.GetQCTime();
        q_str["user_id"] = e.GetUserId();
        q_str["question_head"] = e.GetQHead();
        q_str["views"] = e.GetQViews();
        q_str["votes"] = e.GetQVotes();
        qList_str.push_back(q_str.dump());
    }
    rsp["question_list"] = qList_str;
    return;
}
// add question
void Service::AddQuestion(const json& req, json& rsp, Timestamp timestamp)  {
    // get
    std::string user_id = req["user_id"];
    std::string question_head = req["question_head"];
    std::string question_info = req["question_info"];
    // pull
    std::list<Question> qList;
    bool ret = _qa_model.AddQuestion(user_id, question_head, question_info, qList);
    if (!ret) {
        rsp["errcode"] = ADD_QUESTION_FAILED;
        rsp["errmsg"]= "Failed to ask the question";
        return;
    }
    rsp["errcode"]  = QA_SUCCESS;
    std::vector<std::string> qList_str;
    for (const Question& e : qList) {
        json q_str;
        q_str["question_id"] = e.GetQId();
        q_str["question_createtime"] = e.GetQCTime();
        q_str["user_id"] = e.GetUserId();
        q_str["question_head"] = e.GetQHead();
        q_str["views"] = e.GetQViews();
        q_str["votes"] = e.GetQVotes();
        qList_str.push_back(q_str.dump());
    }
    rsp["question_list"] = qList_str;
    return;
}
// get answer list
void Service::GetAnswerList(const json& req, json& rsp, Timestamp timestamp)  {
    // get
    unsigned int question_id = req["question_id"];
    // pull
    Question q;
    std::list<Answer> aList;
    bool ret = _qa_model.GetAnswerList(question_id, q, aList);
    if (!ret) {
        rsp["errcode"] = GET_ANSWER_LIST_DAILED;
        rsp["errmsg"] = "Get answer exception";
        return;
    }
    rsp["errcode"]  = QA_SUCCESS;

    json q_str;
    q_str["question_id"] = q.GetQId();
    q_str["question_createtime"] = q.GetQCTime();
    q_str["user_id"] = q.GetUserId();
    q_str["question_head"] = q.GetQHead();
    std::string question_info;
    q.GetQInfo(question_info);
    q_str["question_info"] = question_info;
    q_str["question_like"] = q.GetQLike();
    q_str["question_unlike"] = q.GetQUnlike();
    rsp["question"] = q_str.dump();

    std::vector<std::string> aList_str;
    for (Answer& e : aList) {
        json a_str;
        a_str["answer_id"] = e.GetAId();
        a_str["answer_createtime"] = e.GetACTime();
        a_str["question_id"] = e.GetQId();
        a_str["user_id"] = e.GetUserId();
        std::string answer_info;
        e.GetAInfo(answer_info);
        a_str["answer_info"] = answer_info;
        a_str["answer_like"] = e.GetALike();
        a_str["answer_unlike"] = e.GetAUnlike();
        aList_str.push_back(a_str.dump());
    }
    rsp["answer"] = aList_str;
    return;
}
// add answer
void Service::AddAnswer(const json& req, json& rsp, Timestamp timestamp) {
    // get 
    unsigned int question_id = req["question_id"];
    std::string user_id = req["user_id"];
    std::string answer_info = req["answer_info"];
    // pull
    Question q;
    std::list<Answer> aList;
    bool ret = _qa_model.AddAnswer(question_id, user_id, answer_info, q, aList);
    if (!ret) {
        rsp["errcode"] = ADD_ANSWER_FAILED;
        rsp["errmsg"] = "Add answer exception";
        return;
    }
    rsp["errcode"]  = QA_SUCCESS;

    json q_str;
    q_str["question_id"] = q.GetQId();
    q_str["question_createtime"] = q.GetQCTime();
    q_str["user_id"] = q.GetUserId();
    q_str["question_head"] = q.GetQHead();
    std::string question_info;
    q.GetQInfo(question_info);
    q_str["question_info"] = question_info;
    q_str["question_like"] = q.GetQLike();
    q_str["question_unlike"] = q.GetQUnlike();
    rsp["question"] = q_str.dump();

    std::vector<std::string> aList_str;
    for (Answer& e : aList) {
        json a_str;
        a_str["answer_id"] = e.GetAId();
        a_str["answer_createtime"] = e.GetACTime();
        a_str["question_id"] = e.GetQId();
        a_str["user_id"] = e.GetUserId();
        std::string answer_info;
        e.GetAInfo(answer_info);
        a_str["answer_info"] = answer_info;
        a_str["answer_like"] = e.GetALike();
        a_str["answer_unlike"] = e.GetAUnlike();
        aList_str.push_back(a_str.dump());
    }
    rsp["answer"] = aList_str;
    return;
}
// del question
void Service::DelQuestion(const json& req, json& rsp, Timestamp timestamp) {
    // get 
    unsigned int question_id = req["question_id"];
    std::string user_id = req["user_id"];
    // pull
    std::list<Question> qList;
    bool ret = _qa_model.DelQuestion(question_id, user_id, qList);
    if (!ret) {
        rsp["errcode"] = GET_QUESTION_LIST_FAILED;
        rsp["errmsg"] = "Delete question exception";
        return;
    }
    rsp["errcode"]  = QA_SUCCESS;
    std::vector<std::string> qList_str;
    for (const Question& e : qList) {
        json q_str;
        q_str["question_id"] = e.GetQId();
        q_str["question_createtime"] = e.GetQCTime();
        q_str["user_id"] = e.GetUserId();
        q_str["question_head"] = e.GetQHead();
        q_str["views"] = e.GetQViews();
        q_str["votes"] = e.GetQVotes();
        qList_str.push_back(q_str.dump());
    }
    rsp["question_list"] = qList_str;
    return;
}

void Service::DelAnswer(const json& req, json& rsp, Timestamp timestamp) {
    // get
    unsigned int answer_id = req["answer_id"];
    unsigned int question_id = req["question_id"];
    std::string user_id = req["user_id"];
    // pull
    Question q;
    std::list<Answer> aList;
    bool ret = _qa_model.DelAnswer(answer_id, question_id, user_id, q, aList);
    if (!ret) {
        rsp["errcode"] = GET_ANSWER_LIST_DAILED;
        rsp["errmsg"] = "Delete answer exception";
        return;
    }
    rsp["errcode"]  = QA_SUCCESS;

    json q_str;
    q_str["question_id"] = q.GetQId();
    q_str["question_createtime"] = q.GetQCTime();
    q_str["user_id"] = q.GetUserId();
    q_str["question_head"] = q.GetQHead();
    std::string question_info;
    q.GetQInfo(question_info);
    q_str["question_info"] = question_info;
    q_str["question_like"] = q.GetQLike();
    q_str["question_unlike"] = q.GetQUnlike();
    rsp["question"] = q_str.dump();

    std::vector<std::string> aList_str;
    for (Answer& e : aList) {
        json a_str;
        a_str["answer_id"] = e.GetAId();
        a_str["answer_createtime"] = e.GetACTime();
        a_str["question_id"] = e.GetQId();
        a_str["user_id"] = e.GetUserId();
        std::string answer_info;
        e.GetAInfo(answer_info);
        a_str["answer_info"] = answer_info;
        a_str["answer_like"] = e.GetALike();
        a_str["answer_unlike"] = e.GetAUnlike();
        aList_str.push_back(a_str.dump());
    }
    rsp["answer"] = aList_str;
    return;
}

/*
 * remark
 *
 * */
void Service::AddRemark(const json& req, json& rsp, Timestamp timestamp) {
    // get
    int remark_type = req["remark_type"];
    unsigned int dynamic_id = req["dynamic_id"];
    int dynamic_type = req["dynamic_type"];
    std::string user_id = req["user_id"];
    std::string remark_info = req["remark_info"];
    bool ret = false;
    if (remark_type == REMARK_REMARK) {
        ret = _remark_model.AddRemark(dynamic_id, dynamic_type, user_id, remark_info);
        if (!ret) {
            rsp["errcode"] = REMARK_FAILED;
            rsp["errmsg"] = "Failed to remark";
            return;
        }
    } else if (remark_type == REMARK_REPLY) {
        unsigned int reply_remark_id = req["reply_remark_id"];
        ret = _remark_model.AddReply(dynamic_id, dynamic_type, user_id, reply_remark_id, remark_info);
        if (!ret) {
            rsp["errcode"] = REMARK_FAILED;
            rsp["errmsg"] = "Failed to reply";
            return;
        }
    } else {
        rsp["errcode"] = REMARK_FAILED;
        rsp["errmsg"] = "This is an invalid remark";
        return;
    }
    rsp["errcode"] = REMARK_SCCUESS;
    return;
}
