#ifndef __QA_MODEL_HPP__
#define __QA_MODEL_HPP__

#include <string>
#include <list>

enum QA_ERRCODE {
    QA_SUCCESS,
    GET_QUESTION_LIST_FAILED,
    ADD_QUESTION_FAILED,
    GET_ANSWER_LIST_DAILED,
    ADD_ANSWER_FAILED,
};

class Question {
    public:
        Question();
        Question(unsigned int q_id, const std::string& ctime, const std::string& user_id, const std::string& head, unsigned int views = 0);
        void SetQuestionInfo(const std::string& info, unsigned int like, unsigned int unlike);
        void SetVotes(unsigned int votes);

        unsigned int GetQId() const;
        std::string GetQCTime() const;
        std::string GetUserId() const;
        std::string GetQHead() const;
        unsigned int GetQViews() const;
        unsigned int GetQVotes() const;
        void GetQInfo(std::string& info);
        unsigned int GetQLike() const;
        unsigned int GetQUnlike() const;
        ~Question();
    private:
        unsigned int _q_id;
        std::string _ctime;
        std::string _user_id;
        std::string _head;
        std::string _info;
        unsigned int _views;
        unsigned int _like;
        unsigned int _unlike;
        unsigned int _votes;
};

class Answer {
    public:
        Answer(unsigned a_id, const std::string& ctime, unsigned int q_id, const std::string& user_id, const std::string& info, unsigned int like, unsigned int unlike);
        unsigned int GetAId() const;
        std::string GetACTime() const;
        unsigned int GetQId() const;
        std::string GetUserId() const;
        void GetAInfo(std::string& info);
        unsigned int GetALike() const;
        unsigned int GetAUnlike() const;
        ~Answer();
    private:
        unsigned int _a_id;
        std::string _ctime;
        unsigned int _q_id;
        std::string _user_id;
        std::string _info;
        unsigned int _like;
        unsigned int _unlike;
};

class QAModel {
    public:
        QAModel();
        bool GetQuestionList(std::list<Question>& qList);
        bool GetAnswerList(unsigned int q_id, Question& q, std::list<Answer>& aList);
        bool AddQuestion(const std::string& user_id, const std::string& head, const std::string& info, std::list<Question>& qList);
        bool AddAnswer(unsigned int q_id, const std::string& user_id, const std::string& info, Question& q, std::list<Answer>& aList);
        ~QAModel();
        bool DelQuestion(unsigned int q_id, const std::string& user_id, std::list<Question>& qList);
        bool DelAnswer(unsigned int a_id, unsigned int q_id, const std::string& user_id, Question& q, std::list<Answer>& aList);
};

#endif
