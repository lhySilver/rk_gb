#include <strstream>
#include "BaseImage.h"
#include "ShareSDK.h"
#include <string>
#include "MySingleLock.h"


//////////////////////////////////////////////////////////////////////////
SnapImageData::SnapImageData()
    : image(NULL)
	, imgStatus(3)
{
	memset(this, 0, sizeof(SnapImageData));
}

SnapImageData::~SnapImageData()
{
    if(image != NULL){
        delete[] image;
        image = NULL;
    }
}

SnapImageData& SnapImageData::operator=(const SnapImageData& obj)
{
	if (this == &obj)
		return *this;
    imgType = obj.imgType;    // 0--jpg  1--yuv
    imgStatus = obj.imgStatus;  // 状态 0--invalid  1--valid  2--saved
    imgWidth = obj.imgWidth;
    imgHeight = obj.imgHeight;

    // 源数据的长度和指针
    imgDataLen = obj.imgDataLen;

    if(image != NULL){
        delete[] image;
        image = NULL;
    }

    if(imgDataLen != 0){
        image = new tuint8[imgDataLen];
        memcpy(image, obj.image, imgDataLen);
    }
        return *this;
}

SnapImageData::SnapImageData(const SnapImageData& obj)
	: image(NULL)
{
    imgType = obj.imgType;    // 0--jpg  1--yuv
    imgStatus = obj.imgStatus;  // 状态 0--invalid  1--valid  2--saved
    imgWidth = obj.imgWidth;
    imgHeight = obj.imgHeight;

    // 源数据的长度和指针
    imgDataLen = obj.imgDataLen;
    if(image != NULL){
        delete[] image;
        image = NULL;
    }

    if(imgDataLen != 0){
        image = new tuint8[imgDataLen];
        memcpy(image, obj.image, imgDataLen);
    }
}

tuint32 SnapImageData::baseLength()
{
	return 5*sizeof(tint32);
}

tint32 SnapImageData::length()
{
	return imgDataLen*sizeof(char) + 5*sizeof(tint32);
}

bool SnapImageData::hasData()
{
    return imgDataLen > 0 && image != NULL;
}

void SnapImageData::setImage(tuint8* imgData, tuint32 imgLen)
{
    if (imgLen <= 0)
        return;
    if (image != NULL)
        delete[] image;

    imgDataLen = imgLen;
    image = new tuint8[imgLen];
    memcpy(image, imgData, imgLen);

}

tuint32 SnapImageData::getImageSize() const
{
    return imgDataLen;
}

tuint8* SnapImageData::getImageData() const
{
    return image;
}


TargetData::TargetData()
	: image(NULL)
	, imgStatus(3)
{
	memset(this, 0, sizeof(TargetData));
}

TargetData::~TargetData()
{
	if(image != NULL){
		delete[] image;
		image = NULL;
	}
}

TargetData& TargetData::operator=(const TargetData& obj)
{
	if (this == &obj)
		return *this;
	imgType = obj.imgType;    // 0--jpg  1--yuv
	imgStatus = obj.imgStatus;  // 状态 0--invalid  1--valid  2--saved
	imgWidth = obj.imgWidth;
	imgHeight = obj.imgHeight;
	targetId = obj.targetId;
	targetType = obj.targetType;
	pos.X1 = obj.pos.X1;
	pos.Y1 = obj.pos.Y1;
	pos.X2 = obj.pos.X2;
	pos.Y2 = obj.pos.Y2;

	// 源数据的长度和指针
	imgDataLen = obj.imgDataLen;

	if(image != NULL){
		delete[] image;
		image = NULL;
	}

	if(imgDataLen != 0){
		image = new tuint8[imgDataLen];
		memcpy(image, obj.image, imgDataLen);
	}
	return *this;
}

TargetData::TargetData(const TargetData& obj)
	: image(NULL)
{
	imgType = obj.imgType;    // 0--jpg  1--yuv
	imgStatus = obj.imgStatus;  // 状态 0--invalid  1--valid  2--saved
	imgWidth = obj.imgWidth;
	imgHeight = obj.imgHeight;
	targetId = obj.targetId;
	targetType = obj.targetType;
	pos.X1 = obj.pos.X1;
	pos.Y1 = obj.pos.Y1;
	pos.X2 = obj.pos.X2;
	pos.Y2 = obj.pos.Y2;

	// 源数据的长度和指针
	imgDataLen = obj.imgDataLen;
	if(image != NULL){
		delete[] image;
		image = NULL;
	}

	if(imgDataLen != 0){
		image = new tuint8[imgDataLen];
		memcpy(image, obj.image, imgDataLen);
	}
}

tuint32 TargetData::baseLength()
{
	return 7*sizeof(tint32)+sizeof(RECT_T);
}

tint32 TargetData::length()
{
	return imgDataLen*sizeof(char) + baseLength();
}

bool TargetData::hasData()
{
	return imgDataLen > 0 && image != NULL;
}

void TargetData::setImage(tuint8* imgData, tuint32 imgLen)
{
	if (imgLen <= 0)
		return;
	if (image != NULL)
		delete[] image;

	imgDataLen = imgLen;
	image = new tuint8[imgLen];
	memcpy(image, imgData, imgLen);

}

tuint32 TargetData::getImageSize() const
{
	return imgDataLen;
}

tuint8* TargetData::getImageData() const
{
	return image;
}

