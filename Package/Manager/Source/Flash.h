#ifndef __MANAGER_FLASH_H__
#define __MANAGER_FLASH_H__


extern "C" {
	#include "LUA/lua.h"
}

/*!
  \fn     int luaopen_mtd(lua_State *L)
  \bref   注册读写Flash的接口
  \param  L              LUA Script Engine
  \return 
*/
int luaopen_mtd(lua_State *L);

extern  "C" {
/* 下面这些接口根据系统不同有不同的实现 */
int	 FlashInit(void);
int	 FlashErase(unsigned long SectorAddr);
int	 FlashWrite(unsigned char *buf, unsigned long addr, unsigned long cnt);
int	 FlashRead(unsigned char *buf, unsigned long addr, unsigned long cnt);
}
#endif
