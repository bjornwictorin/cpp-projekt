#ifndef NEWSGROUP_H
#define NEWSGROUP_H

#include <string>
#include <list>
#include "article.h"

static unsigned int uniqueid = 0;
class NewsGroup{
public:
	bool delete_article(int id);
	Article edit_article(Article a);
	Article get_article(int id);	
	bool create_article(Article a);
private:
	List<Article> article_list;
	int count;
	std::string group_name;
	int group_id;
};

#endif
