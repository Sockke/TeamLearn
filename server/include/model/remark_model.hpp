#ifndef __REMARK_MODEL_HPP__
#define __REMARK_MODEL_HPP__

#include <map>
#include <string>
#include <vector>


enum REMARK_ERRCODE {
    REMARK_SCCUESS,
    REMARK_FAILED,
};

class RemarkItem {
    public:
        RemarkItem();
        RemarkItem(unsigned int remark_id, const std::string& ctime, unsigned int dynamic_id, const std::string& user_id, const std::string& info);
        bool operator<(const RemarkItem& remark_item) const;
        unsigned int GetRemarkId() const;
        std::string GetRemarkCTime() const;
        std::string GetUserId() const;
        std::string GetRemarkInfo() const;
        ~RemarkItem();
    private:
        unsigned int _remark_id;
        std::string _ctime;
        unsigned int _dynamic_id;
        std::string _user_id;
        std::string _info;
};

class ReplyItem {
    public:
        ReplyItem();
        ReplyItem(unsigned int reply_id, const std::string& ctime, const std::string& user_id, unsigned int reply_remark_id, const std::string& info);
        unsigned int GetReplyId() const;
        std::string GetReplyCTime() const;
        std::string GetUserId() const;
        unsigned int GetReplyRemarkId() const;
        std::string GetReplyInfo() const;
        ~ReplyItem();
    private:
        unsigned int _reply_id;
        std::string _ctime;
        std::string _user_id;
        unsigned int _reply_remark_id;
        std::string _info;
};

class RemarkModel {
    public:
        RemarkModel();
        bool GetRemarkList(unsigned int dynamic_id, int dynamic_type, std::map<const RemarkItem, std::vector<ReplyItem>>& remark_map);
        bool AddRemark(unsigned int dynamic_id, int dynamic_type, const std::string& user_id, const std::string& info);
        bool AddReply(unsigned int dynamic_id, int dynamic_type, const std::string& user_id, unsigned int remark_id, const std::string& info);
        bool DelRemarkList(unsigned int dynamic_id, int dynamic_type);
        bool DelRemark(unsigned int remark_id, const std::string& user_id);
        ~RemarkModel();
};

#endif
