#ifndef _LIB_H_
#define _LIB_H_
#include<string>
void stoch(std::string s,char *ch){
	for(int i=0;i!=s.length();i++){
		ch[i]=s[i];
	}
	ch[s.length()]='\0';
}
#endif
