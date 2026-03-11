/** 智能分析服务器 所需要用到的 图片包括目标数据
*	由IPC上报给智能分析服务器
*
**/

#ifndef IMAGE_DATA__H
#define IMAGE_DATA__H

#include "base_type_define.h"
#include <list>
#pragma pack(push)
#pragma pack(4)
using std::list;
//原图信息
class SnapImageData
{
public:
    tuint32 imgType;    // 0--jpg  1--yuv
    tuint32 imgStatus;  // 状态 0--invalid  1--valid  2--saved, 其他值对应于数据的通道数
    tuint32 imgWidth;
    tuint32 imgHeight;

private:
    // 源数据的长度和指针
    tuint32 imgDataLen;
    tuint8* image;

public:
    SnapImageData();
    ~SnapImageData();
    SnapImageData& operator=(const SnapImageData& obj);
    SnapImageData(const SnapImageData& obj);

    tuint32 baseLength();

	tint32 length();

    bool   hasData();

    void   setImage(tuint8* imgData, tuint32 imgLen);
    tuint32 getImageSize() const;
    tuint8* getImageData() const;
    
};

typedef struct
{
	tint32 X1;   // 左上角x坐标
	tint32 Y1;   // 左上角y坐标
	tint32 X2;   // 右下角x坐标
	tint32 Y2;   // 右下角y坐标
}RECT_T;
//目标图信息
class TargetData
{
public:
	tuint32 imgType;    // 0--jpg  1--yuv
	tuint32 imgStatus;  // 状态 0--invalid  1--valid  2--saved, 其他值对应于数据的通道数
	tuint32 imgWidth;
	tuint32 imgHeight;
	tuint32	targetId; 	
	RECT_T  pos;
	tuint32 targetType; //1,person ; 2,car; 4,bike
private:
	// 源数据的长度和指针
	tuint32 imgDataLen;
	tuint8* image;

public:
	TargetData();
	~TargetData();
	TargetData& operator=(const TargetData& obj);
	TargetData(const TargetData& obj);

	tuint32 baseLength();

	tint32 length();

	bool   hasData();

	void   setImage(tuint8* imgData, tuint32 imgLen);
	tuint32 getImageSize() const;
	tuint8* getImageData() const;

};

#pragma pack(pop)
#endif
