#ifndef GBUTIL_H
#define GBUTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string>

/*
bool GBUtil::memcpy_safe(void* dest , size_t dest_Len,  void* src , size_t src_len)
{
       if(src_len > dest_Len){
           return false;
       }

       memcpy(dest,  src, src_len);
          return true;
}
*/

class GBUtil
{
public:
    static bool memcpy_safe(void* dest , size_t dest_Len, const std::string& src  )
    {
           if(src.length() > dest_Len){
               return false;
           }

           memcpy(dest,  src.c_str(), src.length());
              return true;
    }
};



#endif // GBUTIL_H
