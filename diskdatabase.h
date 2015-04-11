#ifndef DATABASE_H
#define DATABASE_H
#include<string>
#include<map>
#include <sys/stat.h>
#include <dirent.h>
#include "newsgroup.h"
#include "article.h"
#include "databaseinterface.h"



using namespace std;

static unsigned int groupid = 0;
class DiskDatabase : public DatabaseInterface {
	
public:
	DiskDatabase();
	virtual list<Newsgroup> listNewsgroup() const override;
	virtual bool createNewsgroup(std::string& title) override;
	virtual bool deleteNewsgroup(unsigned int id) override;
	virtual list<Article> listArticlesInNewsgroup(unsigned int id) const override;
	virtual bool createArticle(unsigned int id, std::string title, std::string author, std::string text) override;
	virtual bool deleteArticle(unsigned int groupidparam, unsigned int articleid) override;
	virtual Article getArticle(unsigned int groupid, unsigned int articleid) override;
	virtual bool containsNewsgroup(unsigned int id) override;
	virtual bool containsArticle(unsigned int groupid, unsigned int articleid) override;

private:
	string rootfolder = "ngs";
	map<unsigned int, Newsgroup> newsgroups;
	void createNewsgroupDisk(dirent*& dirp);
	unsigned int artid = 0;
};

#endif