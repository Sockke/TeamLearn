#ifndef COMMON_H
#define COMMON_H

// msgid 业务类型
//enum {
//    LOGIN = 1, // 登陆
//};

// errno 错误类型
//enum {
//    SUCCESS, // 成功
//    FAILED, // 失败
//    REPEAT_LOGIN, // 重复登陆
//    WRONG_PWD //密码错误
//};
enum DYNAMIC_TYPE {
    DYNAMIC_SHARE = 1,
    DYNAMIC_QA,
};

enum REMARK_TYPE {
    REMARK_REMARK = 1,
    REMARK_REPLY,
};

// status
enum {
    STATUS_PLACEHOLDE,
    STATUS_EXIST,
    STATUS_DELETE
};

#endif
