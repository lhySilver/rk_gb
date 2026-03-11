#ifndef MEDIAPLAY_SDK_HERDER_H_ 
#define MEDIAPLAY_SDK_HERDER_H_


#ifdef WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif //WIN32
#ifdef __ENVIRONMENT_MAC__
#include "base_type_define.h"
#endif
#include "math.h"

/**************错误码定义**************/
/**************************************/

/****************宏定义****************/

//流模式
#define STREAM_REALTIME	1			//实时流模式
#define STREAM_FILE		2			//文件流模式
#define STREAM_AUDIO	3			//音频流

//解码缓冲区大小(字节)
#define MAX_SOURCE_BUF		1024*100000	//解码缓冲区最大值
#define MIN_SOURCE_BUF		1024*50		//解码缓冲区最小值

//播放缓冲帧数
#define MAX_DIS_FRAME		50			//播放缓冲区最大缓冲帧数
#define MIN_DIS_FRAME		3			//播放缓冲区最小缓冲帧数

#define MAX_DISPLAY_WND		4			//显示区域最大数量

#define MP_MAX_OSD_LEN      256         // OSD最大长度
#define MP_MAX_OSD_NUM      16          // OSD最大个数

//渲染类型
typedef enum tagRENDER_TYPE
{
    MP_RENDER_NOTSET = 0,				// 未设置
    MP_RENDER_DDRAW,					// ddraw渲染
    MP_RENDER_D3D,						// d3d渲染
    MP_RENDER_OPENGL,		            // 非windows平台opengl渲染
    MP_RENDER_SDL2,                    // SDL_AUTO
    MP_RENDER_SDL2_SOFTWARE,           // SDL_RENDERER_SOFTWARE
    MP_RENDER_SDL2_OCX = MP_RENDER_SDL2_SOFTWARE,                    // OCX use
    MP_RENDER_SDL2_FORLIBVA
}RENDER_TYPE;

/**************************************/

/**************结构体定义**************/
typedef struct
{
	tint32 lWidth;			//画面宽,单位像素.如果是音频数据则为0.
	tint32 lHeight;			//画面高,如果是音频数据则为0
	FILETIME ftFrameTime;//帧时间,单位为100纳秒
	tint32 lType;			//视频帧类型
}MSDK_FRAME_INFO;

typedef struct
{
	BYTE *pBuf;			//帧数据
	tint32 lSize;				//帧数据长度
	tint32 lIndex;			//帧序号
	bool bIsAudio;			//是否音频帧
}MSDK_FRAME_TYPE;

typedef struct tagTRIPWIRE_AREA_INFO
{
    struct POINT {
        POINT() :x(0), y(0) {}
        tint32 x;
        tint32 y;
    };
	bool  drawAB;
	POINT lineStartPos;
	POINT lineEndPos;
	tint32 direction;
	POINT characterA;
	POINT characterB;
	POINT LAPos;
	POINT LBPos;
	POINT LCPos;
	POINT LDPos;
	POINT LEPos;
	POINT LFPos;
    tagTRIPWIRE_AREA_INFO() : drawAB(false), direction(0) {}
}TRIPWIRE_AREA_INFO;

typedef struct
{
    tint32 iOffsetX;
    tint32 iOffsetY;
    tint32 iLineSpacing;
    tint32 iPosFontSize;
}POS_INFO;

namespace TripwireInfoTransform{
    static void Method_1(float a, float b, float x, float y, float len, float &aa, float &bb)
    {
        aa = ((2 * a*(b - y)) - (2 * x)) / ((a*a) + 1);
        bb = ((x*x) + ((b - y)*(b - y)) - (len*len)) / ((a*a) + 1);
    }

    static bool Method_2(float a, float b, float &x, bool bSmall)
    {
        if ((a*a / 4 - b) < 0.0)
        {
            return false;
        }

        if (bSmall)
        {
            x = -(sqrt(a*a / 4 - b)) - (a / 2);
        }
        else
        {
            x = (sqrt(a*a / 4 - b)) - (a / 2);
        }
        return true;
    }

    static void RefreshTripwireInfo(TRIPWIRE_AREA_INFO& info, tint32 width, tint32 height)
    {
        TRIPWIRE_AREA_INFO::POINT start;
        TRIPWIRE_AREA_INFO::POINT end;
        start.x = info.lineStartPos.x;
        start.y = info.lineStartPos.y * height / width;
        end.x = info.lineEndPos.x;
        end.y = info.lineEndPos.y * height / width;

        TRIPWIRE_AREA_INFO& tripInfo = info;

        TRIPWIRE_AREA_INFO::POINT pA;
        TRIPWIRE_AREA_INFO::POINT pB;
        //计算中点坐标
        float MidX = ((float)start.x + (float)end.x) / 2;
        float MidY = ((float)start.y + (float)end.y) / 2;

        //以起点为坐标原点平移
        float x1 = 0.0;
        float y1 = 0.0;
        float x2 = (float)(end.x - start.x);
        float y2 = (float)(start.y - end.y);
        float xM = (float)(MidX - (float)start.x);
        float yM = (float)((float)start.y - MidY);

        float length = 1000.0;//目标点A到中点的距离
        float length2 = 400.0;

        //计算斜率
        float k1 = y2 / x2;
        //该直线的垂直线的斜率
        //float k2 = (-1) * x2 / y2;
        float k2 = (-1) * x2 / y2;

        //y=ax+b 直线方程 => b=y-ax
        float b2 = yM - (xM*k2);

        //30度角的正切值
        float Tan30 = /*0.577350*/1.000000;//改成45度的正切值,貌似效果更好

        float xA, yA, xB, yB;
        float xC = 0, yC = 0, xD = 0, yD = 0;
        float xE = 0, yE = 0, xF = 0, yF = 0;

        if (((int)x2 > 0) && ((int)y2 < 0))//第四象限
        {
            //如果斜率的绝对值>1,则yA=yM+length,以此来求xA, yB=yM-length,以此来求xB
            //如果斜率的绝对值<=1,则xA=xM+length,以此来求yA, xB=xM-length,以此来求yB
            if ((-1.0 <= k2) && (k2 <= 1.0))
            {
                xA = xM + length;
                yA = (xA*(-1)*x2 / y2) + b2;

                xB = xM - length;
                yB = (xB*(-1)*x2 / y2) + b2;
            }
            else
            {
                yA = yM + length;
                xA = (yA - b2)*y2*(-1) / x2;

                yB = yM - length;
                xB = (yB - b2)*y2*(-1) / x2;
            }

            /********************************求顶点C,D,E,F的合理坐标 Start********************************************/
            float diffVab = 1 - (Tan30 * (-1) * x2 / y2);
            if ((-0.01 <= diffVab) && (diffVab <= 0.01)) //误差
            {
                //表明已经无限与Y坐标平行
                xC = xB;
                yC = yB + length2;

                //计算斜率
                float k3 = (k2 - Tan30) / (1 + (k2*Tan30));
                float b3 = yB - k3*xB;
                float aa = 0;
                float bb = 0;
                Method_1(k3, b3, xB, yB, length2, aa, bb);
                xD = 0;
                bool bRet = Method_2(aa, bb, xD, false);
                yD = k3*xD + b3;

                b3 = yA - k3*xA;
                Method_1(k3, b3, xA, yA, length2, aa, bb);
                xE = 0;
                bRet = Method_2(aa, bb, xE, true);
                yE = k3*xE + b3;

                xF = xA;
                yF = yA - length2;
            }
            else
            {
                //计算斜率
                float k3 = (k2 - Tan30) / (1 + (k2*Tan30));
                float b3 = yB - k3*xB;
                float aa = 0;
                float bb = 0;
                Method_1(k3, b3, xB, yB, length2, aa, bb);
                bool bRet = Method_2(aa, bb, xD, false);
                yD = k3*xD + b3;

                b3 = yA - k3*xA;
                Method_1(k3, b3, xA, yA, length2, aa, bb);
                bRet = Method_2(aa, bb, xE, true);
                yE = k3*xE + b3;

                //计算斜率
                float k4 = (k2 + Tan30) / (1 - (k2*Tan30));
                float b4 = yB - k4*xB;
                aa = 0;
                bb = 0;
                Method_1(k4, b4, xB, yB, length2, aa, bb);
                bool bSmall = true;
                if (k4 >= 0.0)
                {
                    bSmall = false;
                }
                else
                {
                    bSmall = true;
                }

                bRet = Method_2(aa, bb, xC, bSmall);
                yC = k4*xC + b4;

                b4 = yA - k4*xA;
                aa = 0;
                bb = 0;
                Method_1(k4, b4, xA, yA, length2, aa, bb);
                bSmall = true;
                if (k4 >= 0.0)
                {
                    bSmall = true;
                }
                else
                {
                    bSmall = false;
                }

                bRet = Method_2(aa, bb, xF, bSmall);
                yF = k4*xF + b4;
            }
            /********************************求顶点C,D,E,F的合理坐标 End********************************************/

            pA.x = int(xA) + start.x;
            pA.y = start.y - (int(yA) + 25);
            pB.x = int(xB) - 20 + start.x;
            pB.y = start.y - (int(yB) - 1);
        }
        else if (((int)x2 == 0) && ((int)y2 < 0))//垂直线与X坐标重合,且Y坐标为负值
        {
            //float xA,yA,xB,yB;
            xA = xM + length;
            yA = yM;
            xB = xM - length;
            yB = yM;

            pA.x = int(xA) + 2 + start.x;
            pA.y = start.y - (int(yA) + 10);
            pB.x = int(xB) - 22 + start.x;
            pB.y = start.y - (int(yB) + 10);

            float xx1 = xB + length2;
            float yy1 = yB;
            xC = xx1;
            yC = yy1 + length2;
            xD = xx1;
            yD = yy1 - length2;

            float xx2 = xA - length2;
            float yy2 = yA;
            xE = xx2;
            yE = yy2 + length2;
            xF = xx2;
            yF = yy2 - length2;
        }
        else if (((int)x2 < 0) && ((int)y2 < 0))//第三象限
        {
            //如果斜率的绝对值>1,则yA=yM-length,以此来求xA, yB=yM+length,以此来求xB
            //如果斜率的绝对值<=1,则xA=xM+length,以此来求yA, xB=xM-length,以此来求yB
            if ((-1.0 <= k2) && (k2 <= 1.0))
            {
                xA = xM + length;
                yA = (xA*(-1)*x2 / y2) + b2;

                xB = xM - length;
                yB = (xB*(-1)*x2 / y2) + b2;
            }
            else
            {
                yA = yM - length;
                xA = (yA - b2)*y2*(-1) / x2;

                yB = yM + length;
                xB = (yB - b2)*y2*(-1) / x2;
            }

            /********************************求顶点C,D,E,F的合理坐标 Start********************************************/
            float diffVab = 1 + (Tan30 * (-1) * x2 / y2);
            if ((-0.01 <= diffVab) && (diffVab <= 0.01)) //误差
            {
                //表明已经无限与Y坐标平行
                xD = xB;
                yD = yB - length2;

                //计算斜率
                float k3 = (k2 + Tan30) / (1 - (k2*Tan30));
                float b3 = yB - k3*xB;
                float aa = 0;
                float bb = 0;
                Method_1(k3, b3, xB, yB, length2, aa, bb);
                bool bRet = Method_2(aa, bb, xC, false);
                yC = k3*xC + b3;

                b3 = yA - k3*xA;
                Method_1(k3, b3, xA, yA, length2, aa, bb);
                bRet = Method_2(aa, bb, xF, true);
                yF = k3*xF + b3;

                xE = xA;
                yE = yA + length2;
            }
            else
            {
                //计算斜率
                float k3 = (k2 + Tan30) / (1 - (k2*Tan30));
                float b3 = yB - k3*xB;
                float aa = 0;
                float bb = 0;
                Method_1(k3, b3, xB, yB, length2, aa, bb);
                bool bRet = Method_2(aa, bb, xC, false);
                yC = k3*xC + b3;

                b3 = yA - k3*xA;
                Method_1(k3, b3, xA, yA, length2, aa, bb);
                bRet = Method_2(aa, bb, xF, true);
                yF = k3*xF + b3;

                //计算斜率
                float k4 = (k2 - Tan30) / (1 + (k2*Tan30));
                float b4 = yB - k4*xB;
                aa = 0;
                bb = 0;
                Method_1(k4, b4, xB, yB, length2, aa, bb);
                bool bSmall = true;
                if (k4 >= 0.0)
                {
                    bSmall = true;
                }
                else
                {
                    bSmall = false;
                }

                bRet = Method_2(aa, bb, xD, bSmall);
                yD = k4*xD + b4;

                b4 = yA - k4*xA;
                aa = 0;
                bb = 0;
                Method_1(k4, b4, xA, yA, length2, aa, bb);
                bSmall = true;
                if (k4 >= 0.0)
                {
                    bSmall = false;
                }
                else
                {
                    bSmall = true;
                }

                bRet = Method_2(aa, bb, xE, bSmall);
                yE = k4*xE + b4;
            }
            /********************************求顶点C,D,E,F的合理坐标 End********************************************/

            pA.x = int(xA) + start.x;
            pA.y = start.y - (int(yA) - 1);
            pB.x = int(xB) - 20 + start.x;
            pB.y = start.y - (int(yB) + 25);
        }
        else if (((int)x2 < 0) && ((int)y2 == 0))//垂直线与Y坐标重合,且X坐标为负值
        {
            xA = xM;
            yA = yM - length;
            xB = xM;
            yB = yM + length;

            pA.x = int(xA) - 9 + start.x;
            pA.y = start.y - (int(yA) - 1);
            pB.x = int(xB) - 9 + start.x;
            pB.y = start.y - (int(yB) + 25);

            float xx1 = xB;
            float yy1 = yB - length2;
            xC = xx1 + length2;
            yC = yy1;
            xD = xx1 - length2;
            yD = yy1;

            float xx2 = xA + length2;
            float yy2 = yA;
            xE = xx2 + length2;
            yE = yy2;
            xF = xx2 - length2;
            yF = yy2;
        }
        else if (((int)x2 < 0) && ((int)y2 > 0))//第二象限
        {
            //如果斜率的绝对值>1,则yA=yM-length,以此来求xA, yB=yM+length,以此来求xB
            //如果斜率的绝对值<=1,则xA=xM-length,以此来求yA, xB=xM+length,以此来求yB
            if ((-1.0 <= k2) && (k2 <= 1.0))
            {
                xA = xM - length;
                yA = (xA*(-1)*x2 / y2) + b2;

                xB = xM + length;
                yB = (xB*(-1)*x2 / y2) + b2;
            }
            else
            {
                yA = yM - length;
                xA = (yA - b2)*y2*(-1) / x2;

                yB = yM + length;
                xB = (yB - b2)*y2*(-1) / x2;
            }

            /********************************求顶点C,D,E,F的合理坐标 Start********************************************/
            float diffVab = 1 - (Tan30 * (-1) * x2 / y2);
            if ((-0.01 <= diffVab) && (diffVab <= 0.01)) //误差
            {
                //表明已经无限与Y坐标平行
                xC = xB;
                yC = yB - length2;

                //计算斜率
                float k3 = (k2 - Tan30) / (1 + (k2*Tan30));
                float b3 = yB - k3*xB;
                float aa = 0;
                float bb = 0;
                Method_1(k3, b3, xB, yB, length2, aa, bb);
                xD = 0;
                bool bRet = Method_2(aa, bb, xD, true);
                yD = k3*xD + b3;

                b3 = yA - k3*xA;
                Method_1(k3, b3, xA, yA, length2, aa, bb);
                xE = 0;
                bRet = Method_2(aa, bb, xE, false);
                yE = k3*xE + b3;

                xF = xA;
                yF = yA + length2;
            }
            else
            {
                //计算斜率
                float k3 = (k2 - Tan30) / (1 + (k2*Tan30));
                float b3 = yB - k3*xB;
                float aa = 0;
                float bb = 0;
                Method_1(k3, b3, xB, yB, length2, aa, bb);
                bool bRet = Method_2(aa, bb, xD, true);
                yD = k3*xD + b3;

                b3 = yA - k3*xA;
                Method_1(k3, b3, xA, yA, length2, aa, bb);
                bRet = Method_2(aa, bb, xE, false);
                yE = k3*xE + b3;

                //计算斜率
                float k4 = (k2 + Tan30) / (1 - (k2*Tan30));
                float b4 = yB - k4*xB;
                aa = 0;
                bb = 0;
                Method_1(k4, b4, xB, yB, length2, aa, bb);
                bool bSmall = true;
                if (k4 >= 0.0)
                {
                    bSmall = true;
                }
                else
                {
                    bSmall = false;
                }

                bRet = Method_2(aa, bb, xC, bSmall);
                yC = k4*xC + b4;

                b4 = yA - k4*xA;
                aa = 0;
                bb = 0;
                Method_1(k4, b4, xA, yA, length2, aa, bb);
                bSmall = true;
                if (k4 >= 0.0)
                {
                    bSmall = false;
                }
                else
                {
                    bSmall = true;
                }

                bRet = Method_2(aa, bb, xF, bSmall);
                yF = k4*xF + b4;
            }
            /********************************求顶点C,D,E,F的合理坐标 End********************************************/
            pA.x = int(xA) - 20 + start.x;
            pA.y = start.y - (int(yA) - 1);
            pB.x = int(xB) + start.x;
            pB.y = start.y - (int(yB) + 25);
        }
        else if (((int)x2 == 0) && ((int)y2 > 0))//垂直线与X坐标重合,且Y坐标为正值
        {
            xA = xM - length;
            yA = yM;
            xB = xM + length;
            yB = yM;

            pA.x = int(xA) - 25 + start.x;
            pA.y = start.y - (int(yA) + 12);
            pB.x = int(xB) + 5 + start.x;
            pB.y = start.y - (int(yB) + 12);

            float xx1 = xB - length2;
            float yy1 = yB;
            xC = xx1;
            yC = yy1 - length2;
            xD = xx1;
            yD = yy1 + length2;

            float xx2 = xA + length2;
            float yy2 = yA;
            xE = xx2;
            yE = yy2 - length2;
            xF = xx2;
            yF = yy2 + length2;
        }
        else if (((int)x2 > 0) && ((int)y2 > 0))//第一象限
        {
            //如果斜率的绝对值>1,则yA=yM+length,以此来求xA, yB=yM-length,以此来求xB
            //如果斜率的绝对值<=1,则xA=xM-length,以此来求yA, xB=xM+length,以此来求yB
            if ((-1.0 <= k2) && (k2 <= 1.0))
            {
                xA = xM - length;
                yA = (xA*(-1)*x2 / y2) + b2;

                xB = xM + length;
                yB = (xB*(-1)*x2 / y2) + b2;
            }
            else
            {
                yA = yM + length;
                xA = (yA - b2)*y2*(-1) / x2;

                yB = yM - length;
                xB = (yB - b2)*y2*(-1) / x2;
            }

            /********************************求顶点C,D,E,F的合理坐标 Start********************************************/
            float diffVab = 1 + (Tan30 * (-1) * x2 / y2);
            if ((-0.01 <= diffVab) && (diffVab <= 0.01)) //误差
            {
                //表明已经无限与Y坐标平行
                xD = xB;
                yD = yB + length2;

                //计算斜率
                float k3 = (k2 + Tan30) / (1 - (k2*Tan30));
                float b3 = yB - k3*xB;
                float aa = 0;
                float bb = 0;
                Method_1(k3, b3, xB, yB, length2, aa, bb);
                bool bRet = Method_2(aa, bb, xC, true);
                yC = k3*xC + b3;

                b3 = yA - k3*xA;
                Method_1(k3, b3, xA, yA, length2, aa, bb);
                bRet = Method_2(aa, bb, xF, false);
                yF = k3*xF + b3;

                xE = xA;
                yE = yA - length2;
            }
            else
            {
                //计算斜率
                float k3 = (k2 + Tan30) / (1 - (k2*Tan30));
                float b3 = yB - k3*xB;
                float aa = 0;
                float bb = 0;
                Method_1(k3, b3, xB, yB, length2, aa, bb);
                bool bRet = Method_2(aa, bb, xC, true);
                yC = k3*xC + b3;

                b3 = yA - k3*xA;
                Method_1(k3, b3, xA, yA, length2, aa, bb);
                bRet = Method_2(aa, bb, xF, false);
                yF = k3*xF + b3;

                //计算斜率
                float k4 = (k2 - Tan30) / (1 + (k2*Tan30));
                float b4 = yB - k4*xB;
                aa = 0;
                bb = 0;
                Method_1(k4, b4, xB, yB, length2, aa, bb);
                bool bSmall = true;
                if (k4 >= 0.0)
                {
                    bSmall = false;
                }
                else
                {
                    bSmall = true;
                }

                bRet = Method_2(aa, bb, xD, bSmall);
                yD = k4*xD + b4;

                b4 = yA - k4*xA;
                aa = 0;
                bb = 0;
                Method_1(k4, b4, xA, yA, length2, aa, bb);
                bSmall = true;
                if (k4 >= 0.0)
                {
                    bSmall = true;
                }
                else
                {
                    bSmall = false;
                }

                bRet = Method_2(aa, bb, xE, bSmall);
                yE = k4*xE + b4;
            }
            /********************************求顶点C,D,E,F的合理坐标 End********************************************/

            pA.x = int(xA) - 20 + start.x;
            pA.y = start.y - (int(yA) + 25);
            pB.x = int(xB) + start.x;
            pB.y = start.y - (int(yB) - 1);
        }
        else if (((int)x2 > 0) && ((int)y2 == 0))//垂直线与Y坐标重合,且X坐标为正值
        {
            xA = xM;
            yA = yM + length;
            xB = xM;
            yB = yM - length;

            pA.x = int(xA) - 8 + start.x;
            pA.y = start.y - (int(yA) + 25);
            pB.x = int(xB) - 8 + start.x;
            pB.y = start.y - (int(yB) - 1);

            float xx1 = xB;
            float yy1 = yB + length2;
            xC = xx1 - length2;
            yC = yy1;
            xD = xx1 + length2;
            yD = yy1;

            float xx2 = xA;
            float yy2 = yA - length2;
            xE = xx2 - length2;
            yE = yy2;
            xF = xx2 + length2;
            yF = yy2;
        }
        else //((int)x2 == 0) && ((int)y2 == 0)
        {
            return;
        }

        //还原到真实坐标
        float Ax = xA + (float)start.x;
        float Ay = (float)start.y - yA;
        //POINT startA(Ax, Ay);

        float Bx = xB + (float)start.x;
        float By = (float)start.y - yB;
        //POINT startB(Bx, By);

        float Cx = xC + (float)start.x;
        float Cy = (float)start.y - yC;
        //POINT startC(Cx, Cy);

        float Dx = xD + (float)start.x;
        float Dy = (float)start.y - yD;
        //POINT startD(Dx, Dy);

        float Ex = xE + (float)start.x;
        float Ey = (float)start.y - yE;
        //POINT startE(Ex, Ey);

        float Fx = xF + (float)start.x;
        float Fy = (float)start.y - yF;
        //POINT startF(Fx, Fy);

        tripInfo.characterA.x = width * pA.x / 10000;
        tripInfo.characterB.x = width * pB.x / 10000;
        tripInfo.LAPos.x = int(width* Ax/10000.0);
        tripInfo.LBPos.x = int(width* Bx/10000.0);
        tripInfo.LCPos.x = int(width* Cx/10000.0);
        tripInfo.LDPos.x = int(width * Dx/10000.0);
        tripInfo.LEPos.x = int(width* Ex/10000.0);
        tripInfo.LFPos.x = int(width* Fx/10000.0);

        tripInfo.characterA.y = height * pA.y / 10000* width / height;
        tripInfo.characterB.y = height * pB.y / 10000* width / height;
        tripInfo.LAPos.y = int(height * Ay / 10000.0* width / height);
        tripInfo.LBPos.y = int(height * By / 10000.0* width / height);
        tripInfo.LCPos.y = int(height * Cy / 10000.0* width / height);
        tripInfo.LDPos.y = int(height * Dy / 10000.0* width / height);
        tripInfo.LEPos.y = int(height * Ey / 10000.0* width / height);
        tripInfo.LFPos.y = int(height * Fy / 10000.0 * width / height);

        tripInfo.lineStartPos.x = width * tripInfo.lineStartPos.x / 10000;
        tripInfo.lineStartPos.y = height * tripInfo.lineStartPos.y / 10000;

        tripInfo.lineEndPos.x = width * tripInfo.lineEndPos.x / 10000;
        tripInfo.lineEndPos.y = height * tripInfo.lineEndPos.y / 10000;
    }
}

typedef struct MULTI_POINT
{
    POINT szPoint[10];      // 点集合的数组
    tint32 nCount;          // 点的个数
    MULTI_POINT()
    {
        memset(szPoint, 0, sizeof(szPoint));
        nCount = 0;
    }
}S_MULTI_POINT;
/**************************************/

// 状态图标,每种状态对应一张图片
typedef enum
{
	STATE_PICTURE_AUDIO = 0x100,		//声音

	STATE_PICTURE_ZOOM_3_2 = 0x200,		//放大
	STATE_PICTURE_ZOOM_2,
	STATE_PICTURE_ZOOM_3,
	STATE_PICTURE_ZOOM_4,
	STATE_PICTURE_ZOOM_6,
	STATE_PICTURE_ZOOM_8,
	STATE_PICTURE_ZOOM_9,
	STATE_PICTURE_ZOOM_12,
	STATE_PICTURE_ZOOM_16,

	STATE_PICTURE_SPEED_1_32 = 0x300,	//播放速度
	STATE_PICTURE_SPEED_1_16,
	STATE_PICTURE_SPEED_1_8,
	STATE_PICTURE_SPEED_1_4,
	STATE_PICTURE_SPEED_1_2,
	STATE_PICTURE_SPEED_1,
	STATE_PICTURE_SPEED_2,
	STATE_PICTURE_SPEED_4,
	STATE_PICTURE_SPEED_8,
	STATE_PICTURE_SPEED_16,
	STATE_PICTURE_SPEED_32,

	STATE_PICTURE_SPEED_1_32_BACK,
	STATE_PICTURE_SPEED_1_16_BACK,
	STATE_PICTURE_SPEED_1_8_BACK,
	STATE_PICTURE_SPEED_1_4_BACK,
	STATE_PICTURE_SPEED_1_2_BACK,
	STATE_PICTURE_SPEED_1_BACK,
	STATE_PICTURE_SPEED_2_BACK,
	STATE_PICTURE_SPEED_4_BACK,
	STATE_PICTURE_SPEED_8_BACK,
	STATE_PICTURE_SPEED_16_BACK,
	STATE_PICTURE_SPEED_32_BACK,

	STATE_PICTURE_MICROPHONE = 0x400,		//麦克风
	STATE_PICTURE_PTZ ,                     //现场PTZ图标

	STATE_PICTURE_MOTION = 0x500,			//移动报警

	STATE_PICTURE_RECORD = 0x600,			//录像
	STATE_PICTURE_RECORD_MANUAL,			
	STATE_PICTURE_RECORD_SCHEDULE,
	STATE_PICTURE_RECORD_MOTION,
	STATE_PICTURE_RECORD_SENSOR,
	STATE_PICTURE_RECORD_INTELLIGENT,
	STATE_PICTURE_RECORD_POS,

	STATE_PICTURE_PLAY, ////及时回放的状态图标

    STATE_PICTURE_SOLUTION_DWELL_PLAY = 0x700,   ////方案轮询的状态
    STATE_PICTURE_SOLUTION_DWELL_PAUSE,    

    STATE_TEXT_RANGE_BEGIN = 0x800,
    STATE_TEXT_NO_RECORD_DATA,		//无录像数据
    STATE_TEXT_NO_NET_VIDEO,				//无网络视频
    STATE_TEXT_NO_PERMISSION,				//无权限
    STATE_TEXT_PLAYBACK_END,				//回放结束
    STATE_TEXT_SYSTEM_BUSY,                 //设备忙
    STATE_TEXT_TEST_TITLE,                 //test
    STATE_TEXT_RANGE_END,

}STATE_PICTURE;

typedef enum 
{
	NAVIGATE_OPEN = 50, // 导航启动
	NAVIGATE_OUT,        // 放大
	NAVIGATE_IN,         // 缩小
	NAVIGATE_CLOSE,      // 关闭导航
}NAVIGATE_COMMAND;

// typedef enum 
// {
// 	ERRORCODE_UNKNOWN = 1,    // 未知错误
// 	ERRORCODE_FULL,           // There is not enough space on the disk.
// 	ERRORCODE_NOT_FOUND,      // The system cannot find the file specified
// 	ERRORCODE_NOT_READY,      // The device is not ready.
// 	ERRORCODE_DEV_NOT_EXIST,  // The specified network resource or device is no longer available.
// 	ERRORCODE_TYPE_E_IOERROR, // I/O Error.
// 	ERRORCODE_FILE_EXISTS,	  //The file exists.
// 	ERRORCODE_TOO_MANY_OPEN_FILES,//The system cannot open the file.
// 	ERRORCODE_ACCESS_DENIED,  //Access is denied.
// 	ERRORCODE_NEGATIVE_SEEK,  // An attempt was made to move the file pointer before the beginning of the file.
// 	ERRORCODE_SEEK_ON_DEVICE, //The file pointer cannot be set on the specified device or file.
// 	ERRORCODE_DELETE_PENDING, //The file cannot be opened because it is in the process of being deleted.
// }WRITE_FILE_ERROR_CODE;

typedef enum
{
	FISHEYE_ORIGNAL = 0,		//原始模式,即顶/壁/底装模式下的鱼眼图,相当于退出鱼眼校正模式

	FISHEYE_ROOF = 0x0100,		//顶装(吸顶)
	FISHEYE_ROOF_360,			//顶装之360°矩形展开全景 + 独立子画面;子画面和矩形展开全景中的子框均支持变倍、移动操作，矩形展开全景还支持左右起始点移动操作
	FISHEYE_ROOF_2x180,			//顶装之2个关联的180°矩形展开画面，任何时刻两子窗口均组成360°全景，又叫“双全景”，两个矩形展开画面均支持左右移动起始点操作，并相互联动;
	FISHEYE_ROOF_FISH_3PTZ,		//顶装之原始图像 + 3个独立子画面，子画面和原始图像中的子框均支持变倍、移动操作，原始图像还支持旋转改变起始点操作;
	FISHEYE_ROOF_FISH_4PTZ,		//项装之原始图像 + 4个独立子画面，子画面和原始图像中的子框均支持变倍、移动操作，原始图像还支持旋转改变起始点操作
	FISHEYE_ROOF_360_6PTZ,		//项装之360°矩形展开全景 + 6个独立子画面，子画面和矩形展开全景中的子框均支持变倍、移动操作，矩形展开全景还支持左右起始点移动操作;				
	FISHEYE_ROOF_FISH_8PTZ,		//项装之原始图像 + 8个独立子画面，子画面和原始图像中的子框均支持变倍、移动操作，原始图像还支持旋转改变起始点操作;

	FISHEYE_WALL = 0x0200,		//壁装
	FISHEYE_WALL_180,			//壁装之180全景图，从左到右180°矩形展开全景，180°矩形展开全景支持上下移动操作，改变垂直视角;
	FISHEYE_WALL_180_3PTZ,		//壁装之180°矩形展开全景 + 3个独立子画面，子画面和矩形展开全景中的子框均支持变倍、移动操作，矩形展开全景支持上下移动操作，改变垂直视角 
	FISHEYE_WALL_180_4PTZ,		//壁装之180°矩形展开全景 + 4个独立子画面，子画面和矩形展开全景中的子框均支持变倍、移动操作，矩形展开全景支持上下移动操作，改变垂直视角 
	FISHEYE_WALL_180_8PTZ,		//壁装之180°矩形展开全景 + 8个独立子画面，子画面和矩形展开全景中的子框均支持变倍、移动操作，矩形展开全景支持上下移动操作，改变垂直视角 

	FISHEYE_DESKTOP = 0x0300,	//底装(桌面)
	FISHEYE_DESKTOP_360,		//底装之360°矩形展开全景 + 独立子画面;子画面和矩形展开全景中的子框均支持变倍、移动操作，矩形展开全景还支持左右起始点移动操作
	FISHEYE_DESKTOP_2x180,		//底装之2个关联的180°矩形展开画面，任何时刻两子窗口均组成360°全景，又叫“双全景”，两个矩形展开画面均支持左右移动起始点操作，并相互联动;
	FISHEYE_DESKTOP_FISH_3PTZ,	//底装之原始图像 + 3个独立子画面，子画面和原始图像中的子框均支持变倍、移动操作，原始图像还支持旋转改变起始点操作;
	FISHEYE_DESKTOP_FISH_4PTZ,	//底装之原始图像 + 4个独立子画面，子画面和原始图像中的子框均支持变倍、移动操作，原始图像还支持旋转改变起始点操作
	FISHEYE_DESKTOP_360_6PTZ,	//底装之360°矩形展开全景 + 6个独立子画面，子画面和矩形展开全景中的子框均支持变倍、移动操作，矩形展开全景还支持左右起始点移动操作;				
	FISHEYE_DESKTOP_FISH_8PTZ,	//底装之原始图像 + 8个独立子画面，子画面和原始图像中的子框均支持变倍、移动操作，原始图像还支持旋转改变起始点操作;

}FISHEYE_MODE;

/*************回调函数定义*************/
typedef void (STDCALL *DecodeCBFun)(tint32 lPort, BYTE *pBuf, tint32 lSize, tint32 lWidth, tint32 lHeight, tint32 lIndex, tint32 lType, void *pParam);
typedef void (STDCALL *DisplayCBFun)(tint32 lPort, const MSDK_FRAME_TYPE &FrameType, const MSDK_FRAME_INFO &FrameInfo, void *pParam);
typedef void (STDCALL *DrawCBFun)(tint32 lPort, HDC hDC, void *pParam);
typedef void (STDCALL *FileEndCBFun)(tint32 lPort, void *pParam);
typedef void (STDCALL *ResolutionCBFun)(tint32 lPort, void *pParam);
typedef void (STDCALL *DecodeModeCBFun)(tint32 lPort, tuint32 nDecodeMode, void *pParam);
typedef void (STDCALL *WriteFileCBFun)(tint32 lPort, const MSDK_FRAME_TYPE &FrameType, const FILETIME &FrameTime, void *pParam, tuint32 dwErrCode);
typedef void (STDCALL *AudioCaptureDataCBFun)(tint32 lPort, const void *pBuf, int nLen, void *pParam);
typedef void (STDCALL *DecodeLostFrameCBFun)(void* pParam);
typedef void (STDCALL *TLVDataCBFun)(tint32 lPort, BYTE *pBuf, tint32 lSize, void *pParam);
/**************************************/


#endif //MEDIAPLAY_SDK_HERDER_H_
