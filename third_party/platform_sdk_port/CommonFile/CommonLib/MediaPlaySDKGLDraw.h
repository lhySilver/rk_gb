#ifndef MEDIAPLAY_SDK_GL_DRAW_H_ 
#define MEDIAPLAY_SDK_GL_DRAW_H_

//以下为画图接口,注意只能在画图回调函数里用
MEDIAPLAY_API void STDCALL MEDIA_PLAY_SetColor(BYTE red, BYTE green, BYTE blue);	//设置当前颜色
MEDIAPLAY_API void STDCALL MEDIA_PLAY_SetLineWidth(int nWidth);						//设置线条宽度
MEDIAPLAY_API void STDCALL MEDIA_PLAY_DrawRect(short x1, short y1, short x2, short y2);	//画矩形


#endif //MEDIAPLAY_SDK_GL_DRAW_H_
