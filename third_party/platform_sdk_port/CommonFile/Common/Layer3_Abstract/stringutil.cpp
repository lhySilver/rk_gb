#ifndef _STRINGUTIL_CPP
#define _STRINGUTIL_CPP

#include <string.h>
#include <ctype.h>
#include <cwctype>

//mod by lyq for trim use c++ function 
#include <algorithm>
#include <functional>
//mod end
#include "stringutil.h"
namespace stringutil
{
	void trimleft(string &str,char c/*=' '*/)
	{
		//trim head
	
		int len = str.length();

		int i = 0;
		while(str[i] == c && str[i] != '\0'){
			i++;
		}
		if(i != 0){
			str = string(str,i,len-i);
		}
	}
	
	void trimright(string &str,char c/*=' '*/)
	{
		//trim tail
		int i = 0;
		int len = str.length();
			
		
		for(i = len - 1; i >= 0; --i ){
			if(str[i] != c){
				break;
			}
		}

		str = string(str,0,i+1);
	}

	void trim(string &str)
	{
		//trim head
		
		int len = str.length();
		if (len<1)
		{
			return;
		}
		str.erase(str.begin(), std::find_if(str.begin(), str.end(),std::not1(std::ptr_fun(::iswspace)))); //modified by yushulin 2015.11.03  
		str.erase(std::find_if(str.rbegin(), str.rend(),std::not1(std::ptr_fun(::iswspace))).base(),str.end()); //"isspace" replaced "iswspace" by yushulin
		return ;

		//mod by lyq for trim use c++ function
        str.erase(str.begin(), std::find_if(str.begin(), str.end(),std::not1(std::ptr_fun(::isspace)))); //modified by yushulin 2015.11.03  
        str.erase(std::find_if(str.rbegin(), str.rend(),std::not1(std::ptr_fun(::isspace))).base(),str.end()); //"isspace" replaced "iswspace" by yushulin
		return ;
		//mod end

		int i = 0;
		while(isspace(str[i]) && str[i] != '\0'){
			i++;
		}
		if(i != 0){
			str = string(str,i,len-i);
		}

		//trim tail
		len = str.length();

		for(i = len - 1; i >= 0; --i ){
			if(!isspace(str[i])){
				break;
			}
		}
		str = string(str,0,i+1);
	}
}//end of namespace stringutil
#endif
