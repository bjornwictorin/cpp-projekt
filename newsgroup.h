#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <list>
#include "article.h"

static unsigned int uniqueid = 0;
class Newsgroup{
public:
	Newsgroup(string name);
	bool deleteArticle(int id);
	Article editArticle(Article a);
	Article getArticle(int id);
	List<Article> getAllArticles();
	bool createArticle(Article a);
	void printAll();
	std::string getName();
private:
	List<Article> articleList;
	int count;
	std::string name;
};

#endif
