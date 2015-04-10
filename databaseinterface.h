#ifndef DATABASE_INTERFACE_H
#define DATABASE_INTERFACE_H
#include "article.h"
#include "newsgroup.h"

/*
*An interface for the two diffrent databases
*/


class DatabaseInterface{
	
public:
	DatabaseInterface(){};
	virtual std::list<Newsgroup> listNewsgroup() const = 0;
	virtual bool createNewsgroup(std::string& title) = 0;
	virtual bool deleteNewsgroup(unsigned int id)= 0;
	virtual std::list<Article> listArticlesInNewsgroup(unsigned int id) const = 0;
	virtual bool createArticle(unsigned int id, std::string title, std::string author, std::string text)= 0;
	virtual bool deleteArticle(unsigned int groupid, unsigned int articleid)= 0;
	virtual Article getArticle(unsigned int groupid, unsigned int articleid) = 0;
	virtual bool containsNewsgroup(unsigned int id) =0;
	virtual bool containsArticle(unsigned int groupid, unsigned int articleid)=0;
};


#endif