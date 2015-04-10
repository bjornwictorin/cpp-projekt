#ifndef DATABASE_H
#define DATABASE_H
#include<string>
#include<map>
#include "newsgroup.h"
#include "article.h"
#include "databaseinterface.h"

/*
*The main memory version of the database containg all the newsgroups
*/


using namespace std;

static unsigned int groupid = 0;
class Database : public DatabaseInterface {
	
public:
	Database() : DatabaseInterface(){}
	virtual list<Newsgroup> listNewsgroup() const override;
	virtual bool createNewsgroup(std::string& title) override;
	virtual bool deleteNewsgroup(unsigned int id) override;
	virtual list<Article> listArticlesInNewsgroup(unsigned int id) const override;
	virtual bool createArticle(unsigned int id, std::string title, std::string author, std::string text) override;
	virtual bool deleteArticle(unsigned int groupid, unsigned int articleid) override;
	virtual Article getArticle(unsigned int groupid, unsigned int articleid) override;
	virtual bool containsNewsgroup(unsigned int id) override;
	virtual bool containsArticle(unsigned int groupid, unsigned int articleid) override;

private:
	map<unsigned int, Newsgroup> newsgroups;
};




#endif