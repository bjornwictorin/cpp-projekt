

#include <Algorithm>
#include "newsgroup.h"

using namespace std;



bool delete_article(int id){
	auto it = article_list.begin();	
	auto end = article_list.end();
	Article a = Article("", "", "", id);
	if(find(it, end, a) != end){
		article_list.remove(article_list.get(it));
		return true;
	}else{
		return false;
	}
}


Article get_article(int id){
	auto it = article_list.begin();
	auto end = article_list.end();
	Article a = Article("", "", "", id);
	if(find(it, end, a) != end){
		return article_list.get(it);
	}else{
		return null;
	}
}

bool create_article(string author, string title, string text){
	article_list.add(Article(author, title, text, uniqueid));
	++uniqueid;
	
	return true;
}
