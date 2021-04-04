#include <functional>
#include <string>

#include "../include/json.hpp"
using json = nlohmann::json;

#include "../include/server.hpp"
#include "../include/service/service.hpp"

Server::Server(EventLoop *loop,
                       const InetAddress &addr,
                       const string &name)
    : _server(loop, addr, name), _loop(loop)
{
    _server.setConnectionCallback(std::bind(&Server::onConnection, this, _1));
    _server.setMessageCallback(std::bind(&Server::onMessage, this, _1, _2, _3));
    _server.setThreadNum(4);
}

void Server::start() {
    _server.start();
}

void Server::onConnection(const TcpConnectionPtr& conn) {
    if (conn->connected()) {
    } else {
        //ChatService::instance()->clientQuit(conn);
        conn->shutdown();
    }
}

void Server::onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp timestamp) {
    std::string msg = buf->retrieveAllAsString();
    if (!msg.empty()) {
        int msgid = GET_SHARE;
        json req;
        req["share_id"] = 1;
        json rsp;
        Service::instance()->serviceHandler(msgid, req, rsp, timestamp);
        if (msg == "get_share\r\n") {
            std::cout << rsp << std::endl;
        } else {
            std::cout << "failed" << std::endl;
        }
        //req["remark_type"] = 1;  // remark
        //req["dynamic_id"] = 1;  
        //req["dynamic_type"] = 1; // share
        //req["user_id"] = "17060210115";
        //req["remark_info"] = "I like your sharing very much";
        //req["question_head"] = "How do i reach the pinnacle of life?";
        //req["question_info"] = "I'm very confused now, I don't know how to reach the peak of life, can anyone tell me the way?";
        //req["share_id"] = 1;
        //req["answer_id"] = 2;
        //req["question_id"] = 1;
        //size_t pos = msg.find(' ');
        //std::string msg_len_str = msg.substr(0, pos);
        //int msg_len = atoi(msg_len_str.c_str());
        //std::string cur_msg = msg.substr(pos+1, msg_len);

        //// 对网络数据进行反序列化
        //json js = json::parse(cur_msg);
        //// 直接通过msgid来调用业务处理回调即可, 实现了网络模块与业务模块的解耦
        //int msgid = js["msgid"].get<int>();
        //// msgid -> handler(conn, msg, timestamp)
        //ChatService::instance()->serviceHandler(msgid, conn, js, timestamp);

        //msg = msg.substr(msg_len_str.size() + 1 + msg_len);
    }
}
