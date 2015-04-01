#ifndef DATABASE_H
#define DATABASE_H

class Database{
	
public:
	List<Newsgroup> listNewsgroup() const;
	bool createNewsgroup(string& title);
	bool deleteNewsgroup(unsigned int id);
	List<Article> listArticlesInNewsgroup(unsigned int id) const;
	bool createArticle(unsigned int id, string title, string author, string text);
	bool deleteArticle(unsigned int groupid, unsigned int articleid);
	Article getArticle(unsigned int groupid, unsigned int articleid) const;
	
};


#endif