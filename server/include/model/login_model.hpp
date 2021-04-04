#ifndef __LOGIN_MODEL_HPP__
#define __LOGIN_MODEL_HPP__

#include <string>

enum LOGIN_ERRCODE {
    USER_SUCCESS,
    LOGIN_FAILED,
    REG_FAILED,
    EDIT_USER_INFO_FAILED,
    MODIFY_PASSWORD_FAILED,
    MODIFY_FOCUS_OR_FANS_FAILED,
};

class LoginModel {
    public:
        LoginModel();
        bool Login(const std::string& user_id, const std::string& password);
        bool Register(const std::string& user_id, const std::string& password, const std::string& name, int gender);
        bool EditInfo(const std::string& user_id, const std::string& name, int gender, const std::string& icon, const std::string& email);
        bool ModifyPassword(const std::string& user_id, const std::string& user_password, const std::string& new_user_password);
        bool ModifyFocusAndFans(const std::string& user_id, unsigned int focus, unsigned int fans);
        ~LoginModel();
};

#endif
