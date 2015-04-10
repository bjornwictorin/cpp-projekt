#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <list>
#include "article.h"
/*
*Class that describes a newsgroup containing diffrent articles
*/


static unsigned int uniqueid = 0;
class Newsgroup{
public:
	Newsgroup(string name, unsigned int id);
	bool deleteArticle(int id);
	Article getArticle(int id);
	std::list<Article> getAllArticles() const;
	bool createArticle(std::string author, std::string title, std::string text);
	void printAll();
	std::string getName();
	unsigned int getNumber() const;
private:
	std::list<Article> articleList;
	int count;
	std::string name;
	unsigned int id;
};

#endif
