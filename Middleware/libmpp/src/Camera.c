#include "PAL/Misc.h"

// 获取当前亮度和统计值
int VideoGetChnLuma(int Chn, unsigned int *pU32Lum)
{
	int n = SystemGetAE();
	*pU32Lum = n;
	return 0;
}

// 获取光敏电阻采集数据
int ExSystemGetADCResult(unsigned int *pU32Result)
{
	int n = SystemReadAdc();
	*pU32Result = n;
	return 0;
}

