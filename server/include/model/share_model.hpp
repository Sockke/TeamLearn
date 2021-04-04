#ifndef __SHARE_MODEL_HPP__
#define __SHARE_MODEL_HPP__

#include <string>
#include <list>

enum SHARE_ERRCODE {
    SHARE_SUCCESS,
    GET_SHARE_LIST_FAILED,
    GET_SHARE_FAILED,
    ADD_SHARE_FAILED,
    DEL_SHARE_FAILED,
};

class ShareItem {
    public:
        ShareItem();
        ShareItem(unsigned int share_id, const std::string& user_id, const std::string& head, const std::string& ctime, unsigned int read);
        void SetItemInfo(const std::string& info, unsigned int like, unsigned int unlike);

        unsigned int GetShareId() const;
        std::string GetUserId() const;
        std::string GetShareCTime() const;
        std::string GetShareHead() const;
        unsigned int GetShareRead() const;
        void GetShareInfo(std::string& info);
        unsigned int GetShareLike() const;
        unsigned int GetShareUnlike() const;
        ~ShareItem();
    private:
        unsigned int _share_id;
        std::string _user_id;
        std::string _ctime;
        std::string _head;
        std::string _info;
        unsigned int _read;
        unsigned int _like;
        unsigned int _unlike;
};

class ShareModel {
    public:
        ShareModel();
        bool GetShareList(std::list<ShareItem>& allshares);
        bool GetShare(unsigned int share_id, ShareItem& share);
        bool AddShare(const std::string& user_id, const std::string& head, const std::string& info, std::list<ShareItem>& allshares);
        bool DelShare(unsigned int share_id, const std::string& user_id, std::list<ShareItem>& allshares);
        ~ShareModel();
};

#endif
