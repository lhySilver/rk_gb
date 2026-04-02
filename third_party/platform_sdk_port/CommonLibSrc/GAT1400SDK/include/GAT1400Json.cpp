#include "GAT1400Json.h"
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "ShareSDK.h"
#include "json.h"

//////////////////////////////////////////////////////////////////////////
//内部函数
//////////////////////////////////////////////////////////////////////////
static void _split(const std::string & src, char delim, std::vector<std::string> & elems)
{
	std::stringstream ss(src);
	std::string item;
	while (std::getline(ss, item, delim)) {
		if(!item.empty()) {
			elems.push_back(item);
		}
	}
}

static bool _HasContent(const char* value)
{
	return value != NULL && value[0] != '\0';
}

static Json::Value _PackFace(const GAT_1400_Face& Face)
{
	Json::Value FaceObject;
	FaceObject["FaceID"] = Json::Value(Face.FaceID);
	FaceObject["InfoKind"] = Json::Value(Face.InfoKind);
	FaceObject["SourceID"] = Json::Value(Face.SourceID);
	FaceObject["DeviceID"] = Json::Value(Face.DeviceID);
	if (_HasContent(Face.LocationMarkTime)) {
		FaceObject["ShotTime"] = Json::Value(Face.LocationMarkTime);
		FaceObject["LocationMarkTime"] = Json::Value(Face.LocationMarkTime);
	} else if (!Face.SubImageList.empty() && _HasContent(Face.SubImageList.front().ShotTime)) {
		FaceObject["ShotTime"] = Json::Value(Face.SubImageList.front().ShotTime);
	}
	FaceObject["LeftTopX"] = Json::Value(Face.LeftTopX);
	FaceObject["LeftTopY"] = Json::Value(Face.LeftTopY);
	FaceObject["RightBtmX"] = Json::Value(Face.RightBtmX);
	FaceObject["RightBtmY"] = Json::Value(Face.RightBtmY);
	if (_HasContent(Face.FaceAppearTime)) {
		FaceObject["FaceAppearTime"] = Json::Value(Face.FaceAppearTime);
	}
	if (_HasContent(Face.FaceDisAppearTime)) {
		FaceObject["FaceDisAppearTime"] = Json::Value(Face.FaceDisAppearTime);
	}
	char szGender[12] = {0};
	sprintf(szGender, "%d", Face.GenderCode);
	FaceObject["GenderCode"] = Json::Value(szGender);
	FaceObject["IsDriver"] = Json::Value(Face.IsDriver);
	FaceObject["IsForeigner"] = Json::Value(Face.IsForeigner);
	FaceObject["IsSuspectedTerrorist"] = Json::Value(Face.IsSuspectedTerrorist);
	FaceObject["IsCriminalInvolved"] = Json::Value(Face.IsCriminalInvolved);
	FaceObject["IsDetainees"] = Json::Value(Face.IsDetainees);
	FaceObject["IsVictim"] = Json::Value(Face.IsVictim);
	FaceObject["IsSuspiciousPerson"] = Json::Value(Face.IsSuspiciousPerson);

	char szID[12] = {0};
	sprintf(szID, "%d", Face.IDType);
	FaceObject["IDType"] = Json::Value(szID);
	FaceObject["IDNumber"] = Json::Value(Face.IDNumber);
	FaceObject["Name"] = Json::Value(Face.Name);
	FaceObject["Similaritydegree"] = Json::Value(Face.Similaritydegree);

	Json::Value ImageArray;
	for (std::list<GAT_1400_SubImage>::const_iterator suIter = Face.SubImageList.begin(); suIter != Face.SubImageList.end(); ++suIter) {
		Json::Value ImageObject;
		ImageObject["ImageID"] = Json::Value(suIter->ImageID);
		ImageObject["EventSort"] = Json::Value(suIter->EventSort);
		ImageObject["DeviceID"] = Json::Value(suIter->DeviceID);
		ImageObject["StoragePath"] = Json::Value(suIter->StoragePath);
		char szType[12] = {0};
		sprintf(szType, "%02d", suIter->Type);
		ImageObject["Type"] = Json::Value(szType);
		ImageObject["FileFormat"] = Json::Value(suIter->FileFormat);
		ImageObject["ShotTime"] = Json::Value(suIter->ShotTime);
		ImageObject["Width"] = Json::Value(suIter->Width);
		ImageObject["Height"] = Json::Value(suIter->Height);
		ImageObject["Data"] = Json::Value(suIter->Data);
		ImageArray["SubImageInfoObject"].append(ImageObject);
	}
	FaceObject["SubImageList"] = ImageArray;
	return FaceObject;
}

static Json::Value _PackPerson(const GAT_1400_Person& Person)
{
	Json::Value PersonsObject;
	PersonsObject["PersonID"] = Json::Value(Person.PersonID);
	PersonsObject["InfoKind"] = Json::Value(Person.InfoKind);
	PersonsObject["SourceID"] = Json::Value(Person.SourceID);
	PersonsObject["DeviceID"] = Json::Value(Person.DeviceID);
	PersonsObject["LeftTopX"] = Json::Value(Person.LeftTopX);
	PersonsObject["LeftTopY"] = Json::Value(Person.LeftTopY);
	PersonsObject["RightBtmX"] = Json::Value(Person.RightBtmX);
	PersonsObject["RightBtmY"] = Json::Value(Person.RightBtmY);
	if(Person.InfoKind == INFO_TYPE_MANUAL) { 
		PersonsObject["LocationMarkTime"] = Json::Value(Person.LocationMarkTime);
		PersonsObject["PersonAppearTime"] = Json::Value(Person.PersonAppearTime);
		PersonsObject["PersonDisAppearTime"] = Json::Value(Person.PersonDisAppearTime);
	}
	char szGender[12] = {0};
	sprintf(szGender, "%d", Person.GenderCode);
	PersonsObject["GenderCode"] = Json::Value(szGender);
	PersonsObject["IsDriver"] = Json::Value(Person.IsDriver);
	PersonsObject["IsForeigner"] = Json::Value(Person.IsForeigner);
	PersonsObject["IsSuspectedTerrorist"] = Json::Value(Person.IsSuspectedTerrorist);
	PersonsObject["IsCriminalInvolved"] = Json::Value(Person.IsCriminalInvolved);
	PersonsObject["IsDetainees"] = Json::Value(Person.IsDetainees);
	PersonsObject["IsVictim"] = Json::Value(Person.IsVictim);
	PersonsObject["IsSuspiciousPerson"] = Json::Value(Person.IsSuspiciousPerson);

	Json::Value ImageArray;
	for (std::list<GAT_1400_SubImage>::const_iterator subIter = Person.SubImageList.begin(); subIter != Person.SubImageList.end(); ++subIter) {
		Json::Value ImageObject;
		ImageObject["ImageID"] = Json::Value(subIter->ImageID);
		ImageObject["EventSort"] = Json::Value(subIter->EventSort);
		ImageObject["DeviceID"] = Json::Value(subIter->DeviceID);
		ImageObject["StoragePath"] = Json::Value(subIter->StoragePath);
		char szType[12] = {0};
		sprintf(szType, "%02d", subIter->Type);
		ImageObject["Type"] = Json::Value(szType);
		ImageObject["FileFormat"] = Json::Value(subIter->FileFormat);
		ImageObject["ShotTime"] = Json::Value(subIter->ShotTime);
		ImageObject["Width"] = Json::Value(subIter->Width);
		ImageObject["Height"] = Json::Value(subIter->Height);
		ImageObject["Data"] = Json::Value(subIter->Data);
		ImageArray["SubImageInfoObject"].append(ImageObject);
	}
	PersonsObject["SubImageList"] = ImageArray;
	return PersonsObject;
}

static Json::Value _PackMotorVehicle(const GAT_1400_Motor& MotorVehicle)
{
	Json::Value MotorVehicleseObject;
	MotorVehicleseObject["MotorVehicleID"] = Json::Value(MotorVehicle.MotorVehicleID);
	MotorVehicleseObject["InfoKind"] = Json::Value(MotorVehicle.InfoKind);
	MotorVehicleseObject["SourceID"] = Json::Value(MotorVehicle.SourceID);
	MotorVehicleseObject["DeviceID"] = Json::Value(MotorVehicle.DeviceID);
	MotorVehicleseObject["StorageUrl1"] = Json::Value(MotorVehicle.StorageUrl1);
	MotorVehicleseObject["LeftTopX"] = Json::Value(MotorVehicle.LeftTopX);
	MotorVehicleseObject["LeftTopY"] = Json::Value(MotorVehicle.LeftTopY);
	MotorVehicleseObject["RightBtmX"] = Json::Value(MotorVehicle.RightBtmX);
	MotorVehicleseObject["RightBtmY"] = Json::Value(MotorVehicle.RightBtmY);
	if (_HasContent(MotorVehicle.MarkTime)) {
		MotorVehicleseObject["MarkTime"] = Json::Value(MotorVehicle.MarkTime);
	}
	if (_HasContent(MotorVehicle.AppearTime)) {
		MotorVehicleseObject["AppearTime"] = Json::Value(MotorVehicle.AppearTime);
	}
	if (_HasContent(MotorVehicle.DisappearTime)) {
		MotorVehicleseObject["DisappearTime"] = Json::Value(MotorVehicle.DisappearTime);
	}

	MotorVehicleseObject["LaneNo"] = Json::Value(MotorVehicle.LaneNo);
	MotorVehicleseObject["HasPlate"] = Json::Value(MotorVehicle.HasPlate);
	MotorVehicleseObject["Speed"] = Json::Value(MotorVehicle.Speed);

	char szDirection[12] = {0};
	sprintf(szDirection, "%d", MotorVehicle.Direction);
	MotorVehicleseObject["Direction"] = Json::Value(szDirection);
	MotorVehicleseObject["VehicleLength"] = Json::Value(MotorVehicle.VehicleLength);
	if (_HasContent(MotorVehicle.PassTime)) {
		MotorVehicleseObject["PassTime"] = Json::Value(MotorVehicle.PassTime);
	}

	char szPlateClass[12] = {0};
	sprintf(szPlateClass, "%d", MotorVehicle.PlateClass);
	MotorVehicleseObject["PlateClass"] = Json::Value(szPlateClass);

	char szPlateColor[12] = {0};
	sprintf(szPlateColor, "%d", MotorVehicle.PlateColor);
	MotorVehicleseObject["PlateColor"] = Json::Value(szPlateColor);

	MotorVehicleseObject["PlateNo"] = Json::Value(MotorVehicle.PlateNo);

	char szVehicleColor[12] = {0};
	sprintf(szVehicleColor, "%d", MotorVehicle.VehicleColor);
	MotorVehicleseObject["VehicleColor"] = Json::Value(szVehicleColor);

	Json::Value ImageArray;
	for (std::list<GAT_1400_SubImage>::const_iterator subIter = MotorVehicle.SubImageList.begin(); subIter != MotorVehicle.SubImageList.end(); ++subIter) {
		Json::Value ImageObject;
		ImageObject["ImageID"] = Json::Value(subIter->ImageID);
		ImageObject["EventSort"] = Json::Value(subIter->EventSort);
		ImageObject["DeviceID"] = Json::Value(subIter->DeviceID);
		ImageObject["StoragePath"] = Json::Value(subIter->StoragePath);
		char szType[12] = {0};
		sprintf(szType, "%02d", subIter->Type);
		ImageObject["Type"] = Json::Value(szType);
		ImageObject["FileFormat"] = Json::Value(subIter->FileFormat);
		ImageObject["ShotTime"] = Json::Value(subIter->ShotTime);
		ImageObject["Width"] = Json::Value(subIter->Width);
		ImageObject["Height"] = Json::Value(subIter->Height);
		ImageObject["Data"] = Json::Value(subIter->Data);
		ImageArray["SubImageInfoObject"].append(ImageObject);
	}
	MotorVehicleseObject["SubImageList"] = ImageArray;
	return MotorVehicleseObject;
}

static Json::Value _PackNonmotorVehicle(const GAT_1400_NonMotor& NonmotorVehicle)
{
	Json::Value NonmotorVehiclesObject;
	NonmotorVehiclesObject["NonMotorVehicleID"] = Json::Value(NonmotorVehicle.NonMotorVehicleID);
	NonmotorVehiclesObject["InfoKind"] = Json::Value(NonmotorVehicle.InfoKind);
	NonmotorVehiclesObject["SourceID"] = Json::Value(NonmotorVehicle.SourceID);
	NonmotorVehiclesObject["DeviceID"] = Json::Value(NonmotorVehicle.DeviceID);
	NonmotorVehiclesObject["LeftTopX"] = Json::Value(NonmotorVehicle.LeftTopX);
	NonmotorVehiclesObject["LeftTopY"] = Json::Value(NonmotorVehicle.LeftTopY);
	NonmotorVehiclesObject["RightBtmX"] = Json::Value(NonmotorVehicle.RightBtmX);
	NonmotorVehiclesObject["RightBtmY"] = Json::Value(NonmotorVehicle.RightBtmY);
	if(NonmotorVehicle.InfoKind == INFO_TYPE_MANUAL) {
		NonmotorVehiclesObject["MarkTime"] = Json::Value(NonmotorVehicle.MarkTime);
		NonmotorVehiclesObject["AppearTime"] = Json::Value(NonmotorVehicle.AppearTime);
		NonmotorVehiclesObject["DisappearTime"] = Json::Value(NonmotorVehicle.DisappearTime);
	}
	NonmotorVehiclesObject["HasPlate"] = Json::Value(NonmotorVehicle.HasPlate);

	char szPlateClass[12] = {0};
	sprintf(szPlateClass, "%d", NonmotorVehicle.PlateClass);
	NonmotorVehiclesObject["PlateClass"] = Json::Value(szPlateClass);

	char szPlateColor[12] = {0};
	sprintf(szPlateColor, "%d", NonmotorVehicle.PlateColor);
	NonmotorVehiclesObject["PlateColor"] = Json::Value(szPlateColor);
	NonmotorVehiclesObject["PlateNo"] = Json::Value(NonmotorVehicle.PlateNo);

	char szVehicleColor[12] = {0};
	sprintf(szVehicleColor, "%d", NonmotorVehicle.VehicleColor);
	NonmotorVehiclesObject["VehicleColor"] = Json::Value(szVehicleColor);

	Json::Value ImageArray;
	for (std::list<GAT_1400_SubImage>::const_iterator subIter = NonmotorVehicle.SubImageList.begin(); subIter != NonmotorVehicle.SubImageList.end(); ++subIter) {
		Json::Value ImageObject;
		ImageObject["ImageID"] = Json::Value(subIter->ImageID);
		ImageObject["EventSort"] = Json::Value(subIter->EventSort);
		ImageObject["DeviceID"] = Json::Value(subIter->DeviceID);
		ImageObject["StoragePath"] = Json::Value(subIter->StoragePath);
		char szType[12] = {0};
		sprintf(szType, "%02d", subIter->Type);
		ImageObject["Type"] = Json::Value(szType);
		ImageObject["FileFormat"] = Json::Value(subIter->FileFormat);
		ImageObject["ShotTime"] = Json::Value(subIter->ShotTime);
		ImageObject["Width"] = Json::Value(subIter->Width);
		ImageObject["Height"] = Json::Value(subIter->Height);
		ImageObject["Data"] = Json::Value(subIter->Data);
		ImageArray["SubImageInfoObject"].append(ImageObject);
	}
	NonmotorVehiclesObject["SubImageList"] = ImageArray;
	return NonmotorVehiclesObject;
}

static Json::Value _PackThing(const GAT_1400_Thing& Thing)
{
	Json::Value ThingObject;
	ThingObject["ThingID"] = Json::Value(Thing.ThingID);
	ThingObject["InfoKind"] = Json::Value(Thing.InfoKind);
	ThingObject["SourceID"] = Json::Value(Thing.SourceID);
	ThingObject["DeviceID"] = Json::Value(Thing.DeviceID);
	ThingObject["LeftTopX"] = Json::Value(Thing.LeftTopX);
	ThingObject["LeftTopY"] = Json::Value(Thing.LeftTopY);
	ThingObject["RightBtmX"] = Json::Value(Thing.RightBtmX);
	ThingObject["RightBtmY"] = Json::Value(Thing.RightBtmY);
	if(Thing.InfoKind == INFO_TYPE_MANUAL) {
		ThingObject["LocationMarkTime"] = Json::Value(Thing.LocationMarkTime);
		ThingObject["AppearTime"] = Json::Value(Thing.AppearTime);
		ThingObject["DisAppearTime"] = Json::Value(Thing.DisAppearTime);
	}
	ThingObject["Name"] = Json::Value(Thing.Name);
	ThingObject["Shape"] = Json::Value(Thing.Shape);
	ThingObject["Color"] = Json::Value(Thing.Color);

	Json::Value ImageArray;
	for (std::list<GAT_1400_SubImage>::const_iterator subIter = Thing.SubImageList.begin(); subIter != Thing.SubImageList.end(); ++subIter) {
		Json::Value ImageObject;
		ImageObject["ImageID"] = Json::Value(subIter->ImageID);
		ImageObject["EventSort"] = Json::Value(subIter->EventSort);
		ImageObject["DeviceID"] = Json::Value(subIter->DeviceID);
		ImageObject["StoragePath"] = Json::Value(subIter->StoragePath);
		char szType[12] = {0};
		sprintf(szType, "%02d", subIter->Type);
		ImageObject["Type"] = Json::Value(szType);
		ImageObject["FileFormat"] = Json::Value(subIter->FileFormat);
		ImageObject["ShotTime"] = Json::Value(subIter->ShotTime);
		ImageObject["Width"] = Json::Value(subIter->Width);
		ImageObject["Height"] = Json::Value(subIter->Height);
		ImageObject["Data"] = Json::Value(subIter->Data);
		ImageArray["SubImageInfoObject"].append(ImageObject);
	}
	ThingObject["SubImageList"] = ImageArray;
	return ThingObject;
}

static Json::Value _PackScene(const GAT_1400_Scene& Scene)
{
	Json::Value SceneObject;
	SceneObject["SceneID"] = Json::Value(Scene.SceneID);
	SceneObject["InfoKind"] = Json::Value(Scene.InfoKind);
	SceneObject["SourceID"] = Json::Value(Scene.SourceID);
	SceneObject["DeviceID"] = Json::Value(Scene.DeviceID);

	Json::Value ImageArray;
	for (std::list<GAT_1400_SubImage>::const_iterator subIter = Scene.SubImageList.begin(); subIter != Scene.SubImageList.end(); ++subIter) {
		Json::Value ImageObject;
		ImageObject["ImageID"] = Json::Value(subIter->ImageID);
		ImageObject["EventSort"] = Json::Value(subIter->EventSort);
		ImageObject["DeviceID"] = Json::Value(subIter->DeviceID);
		ImageObject["StoragePath"] = Json::Value(subIter->StoragePath);
		char szType[12] = {0};
		sprintf(szType, "%02d", subIter->Type);
		ImageObject["Type"] = Json::Value(szType);
		ImageObject["FileFormat"] = Json::Value(subIter->FileFormat);
		ImageObject["ShotTime"] = Json::Value(subIter->ShotTime);
		ImageObject["Width"] = Json::Value(subIter->Width);
		ImageObject["Height"] = Json::Value(subIter->Height);
		ImageObject["Data"] = Json::Value(subIter->Data);
		ImageArray["SubImageInfoObject"].append(ImageObject);
	}
	SceneObject["SubImageList"] = ImageArray;
	return SceneObject;
}

static Json::Value _PackDisposition(const GAT_1400_Disposition& Disposition)
{
	Json::Value DispositionObject;
	DispositionObject["DispositionID"] = Json::Value(Disposition.DispositionID);
	DispositionObject["Title"] = Json::Value(Disposition.DispositionID);
	DispositionObject["DispositionCategory"] = Json::Value(Disposition.DispositionCategory);
	DispositionObject["TargetFeature"] = Json::Value(Disposition.TargetFeature);
	DispositionObject["ApplicantName"] = Json::Value(Disposition.ApplicantName);
	DispositionObject["ApplicantOrg"] = Json::Value(Disposition.ApplicantOrg);
	DispositionObject["BeginTime"] = Json::Value(Disposition.BeginTime);
	DispositionObject["EndTime"] = Json::Value(Disposition.EndTime);
	DispositionObject["OperateType"] = Json::Value(Disposition.OperateType);
	DispositionObject["DispositionStatus"] = Json::Value(Disposition.DispositionStatus);
	DispositionObject["DispositionRange"] = Json::Value(Disposition.DispositionRange);

	Json::Value ImageArray;
	for (std::list<GAT_1400_SubImage>::const_iterator subIter = Disposition.SubImageList.begin(); subIter != Disposition.SubImageList.end(); ++subIter) {
		Json::Value ImageObject;
		ImageObject["ImageID"] = Json::Value(subIter->ImageID);
		ImageObject["EventSort"] = Json::Value(subIter->EventSort);
		ImageObject["DeviceID"] = Json::Value(subIter->DeviceID);
		ImageObject["StoragePath"] = Json::Value(subIter->StoragePath);
		char szType[12] = {0};
		sprintf(szType, "%02d", subIter->Type);
		ImageObject["Type"] = Json::Value(szType);
		ImageObject["FileFormat"] = Json::Value(subIter->FileFormat);
		ImageObject["ShotTime"] = Json::Value(subIter->ShotTime);
		ImageObject["Width"] = Json::Value(subIter->Width);
		ImageObject["Height"] = Json::Value(subIter->Height);
		ImageObject["Data"] = Json::Value(subIter->Data);
		ImageArray["SubImageInfoObject"].append(ImageObject);
	}
	DispositionObject["SubImageList"] = ImageArray;
	return DispositionObject;
}

//视频片段对象
static Json::Value _PackVideoSliceInfo(const GAT_1400_VideoSliceInfo& VideoSliceInfo)
{
	Json::Value sliceInfoObject;
	sliceInfoObject["VideoID"] = Json::Value(VideoSliceInfo.VideoID);
	sliceInfoObject["InfoKind"] = Json::Value(VideoSliceInfo.InfoKind);
	sliceInfoObject["VideoSource"] = Json::Value(VideoSliceInfo.VideoSource);
	sliceInfoObject["StoragePath"] = Json::Value(VideoSliceInfo.StoragePath);
	sliceInfoObject["FileHash"] = Json::Value(VideoSliceInfo.FileHash);
	sliceInfoObject["FileFormat"] = Json::Value(VideoSliceInfo.FileFormat);
	sliceInfoObject["CodedFormat"] = Json::Value(VideoSliceInfo.CodedFormat);
	sliceInfoObject["AudioFlag"] = Json::Value(VideoSliceInfo.AudioFlag);
	sliceInfoObject["Title"] = Json::Value(VideoSliceInfo.Title);
	sliceInfoObject["ContentDescription"] = Json::Value(VideoSliceInfo.ContentDescription);
	sliceInfoObject["ShotPlaceCode"] = Json::Value(VideoSliceInfo.ShotPlaceCode);
	char szSecurityLevel[12] = {0};
	sprintf(szSecurityLevel, "%d", VideoSliceInfo.SecurityLevel);
	sliceInfoObject["SecurityLevel"] = Json::Value(szSecurityLevel);
	sliceInfoObject["VideoLen"] = Json::Value(VideoSliceInfo.VideoLen);
	sliceInfoObject["BeginTime"] = Json::Value(VideoSliceInfo.BeginTime);
	sliceInfoObject["EndTime"] = Json::Value(VideoSliceInfo.EndTime);
	sliceInfoObject["TimeErr"] = Json::Value(VideoSliceInfo.TimeErr);
	sliceInfoObject["Width"] = Json::Value(VideoSliceInfo.Width);
	sliceInfoObject["Height"] = Json::Value(VideoSliceInfo.Height);
	sliceInfoObject["EntryClerk"] = Json::Value(VideoSliceInfo.EntryClerk);
	sliceInfoObject["EntryClerkOrg"] = Json::Value(VideoSliceInfo.EntryClerkOrg);
	sliceInfoObject["FileSize"] = Json::Value(VideoSliceInfo.FileSize);
	return sliceInfoObject;
}

//视频片段集合对象
static Json::Value _PackVideoSlice(const GAT_1400_VideoSliceSet& VideoSlice)
{
	Json::Value sliceObject;
	sliceObject["VideoSliceInfo"] = _PackVideoSliceInfo(VideoSlice.VideoSliceInfo);

	if (!VideoSlice.PersonList.empty()) {
		Json::Value PersonsObjectArray;
		for (std::list<tagGat1400Person>::const_iterator iterPerson = VideoSlice.PersonList.begin(); iterPerson != VideoSlice.PersonList.end(); ++iterPerson) {
			Json::Value PersonsObject = _PackPerson(*iterPerson);
			PersonsObjectArray["PersonObject"].append(PersonsObject);
		}
		sliceObject["PersonList"] = PersonsObjectArray;
	}
	if (!VideoSlice.FaceList.empty()) {
		Json::Value FaceObjectArray;
		for (std::list<GAT_1400_Face>::const_iterator iterFace = VideoSlice.FaceList.begin(); iterFace != VideoSlice.FaceList.end(); ++iterFace) {
			Json::Value FaceObject = _PackFace(*iterFace);
			FaceObjectArray["FaceObject"].append(FaceObject);
		}
		sliceObject["FaceList"] = FaceObjectArray;
	}
	if (!VideoSlice.MotorVehicleList.empty()) {
		Json::Value MotorVehiclesObjectArray;
		for (std::list<tagGat1400Motor>::const_iterator iterMotor = VideoSlice.MotorVehicleList.begin(); iterMotor != VideoSlice.MotorVehicleList.end(); ++iterMotor) {
			Json::Value MotorVehicleseObject = _PackMotorVehicle(*iterMotor);
			MotorVehiclesObjectArray["MotorVehicleObject"].append(MotorVehicleseObject);
		}
		sliceObject["MotorVehicleList"] = MotorVehiclesObjectArray;
	}
	if (!VideoSlice.NonMotorVehicleList.empty())	{
		Json::Value NonmotorVehiclesObjectArray;
		for (std::list<tagGat1400NonMotor>::const_iterator iterNonMotor = VideoSlice.NonMotorVehicleList.begin(); iterNonMotor != VideoSlice.NonMotorVehicleList.end(); ++iterNonMotor) {
			Json::Value NonmotorVehiclesObject = _PackNonmotorVehicle(*iterNonMotor);
			NonmotorVehiclesObjectArray["NonMotorVehicleObject"].append(NonmotorVehiclesObject);
		}
		sliceObject["NonMotorVehicleList"] = NonmotorVehiclesObjectArray;
	}
	if (!VideoSlice.ThingList.empty()) {
		Json::Value ThingObjectArray;
		for (std::list<tagGat1400Thing>::const_iterator iterThing = VideoSlice.ThingList.begin(); iterThing != VideoSlice.ThingList.end(); ++iterThing) {
			Json::Value ThingObject = _PackThing(*iterThing);
			ThingObjectArray["ThingObject"].append(ThingObject);
		}
		sliceObject["ThingList"] = ThingObjectArray;
	}
	if (!VideoSlice.SceneList.empty()) {
		Json::Value SceneObjectArray;
		for (std::list<tagGat1400Scene>::const_iterator iterScene = VideoSlice.SceneList.begin(); iterScene != VideoSlice.SceneList.end(); ++iterScene) {
			Json::Value SceneObject = _PackScene(*iterScene);
			SceneObjectArray["SceneObject"].append(SceneObject);
		}
		sliceObject["SceneList"] = SceneObjectArray;
	}
	if (!VideoSlice.Data.empty()) {
		sliceObject["Data"] = VideoSlice.Data;
	}

	return sliceObject;
}

static Json::Value _PackImageInfo(const GAT_1400_Image& ImageInfo)
{
	Json::Value imageInfoObject;
	imageInfoObject["ImageID"] = Json::Value(ImageInfo.ImageID);
	imageInfoObject["InfoKind"] = Json::Value(ImageInfo.InfoKind);
	char szImageSource[12] = {0};
	sprintf(szImageSource, "%d", ImageInfo.ImageSource);
	imageInfoObject["ImageSource"] = Json::Value(szImageSource);
	imageInfoObject["SourceVideoID"] = Json::Value(ImageInfo.SourceVideoID);
	imageInfoObject["OriginImageID"] = Json::Value(ImageInfo.OriginImageID);
	imageInfoObject["EventSort"] = Json::Value(ImageInfo.EventSort);
	imageInfoObject["DeviceID"] = Json::Value(ImageInfo.DeviceID);
	imageInfoObject["FileFormat"] = Json::Value(ImageInfo.FileFormat);
	imageInfoObject["ShotTime"] = Json::Value(ImageInfo.ShotTime);
	imageInfoObject["Title"] = Json::Value(ImageInfo.Title);
	imageInfoObject["ContentDescription"] = Json::Value(ImageInfo.ContentDescription);
	imageInfoObject["ShotPlaceFullAdress"] = Json::Value(ImageInfo.ShotPlaceFullAdress);
	char szSecurityLevel[12] = {0};
	sprintf(szSecurityLevel, "%d", ImageInfo.SecurityLevel);
	imageInfoObject["SecurityLevel"] = Json::Value(szSecurityLevel);
	imageInfoObject["Width"] = Json::Value(ImageInfo.Width);
	imageInfoObject["Height"] = Json::Value(ImageInfo.Height);
	imageInfoObject["FileSize"] = Json::Value(ImageInfo.FileSize);
	return imageInfoObject;
}

static Json::Value _PackImage(const GAT_1400_ImageSet& Image)
{
	Json::Value imageObject;
	imageObject["ImageInfo"] = _PackImageInfo(Image.ImageInfo);

	if (!Image.PersonList.empty()) {
		Json::Value PersonsObjectArray;
		for (std::list<tagGat1400Person>::const_iterator iterPerson = Image.PersonList.begin(); iterPerson != Image.PersonList.end(); ++iterPerson) {
			Json::Value PersonsObject = _PackPerson(*iterPerson);
			PersonsObjectArray["PersonObject"].append(PersonsObject);
		}
		imageObject["PersonList"] = PersonsObjectArray;
	}
	if (!Image.FaceList.empty()) {
		Json::Value FaceObjectArray;
		for (std::list<GAT_1400_Face>::const_iterator iterFace = Image.FaceList.begin(); iterFace != Image.FaceList.end(); ++iterFace) {
			Json::Value FaceObject = _PackFace(*iterFace);
			FaceObjectArray["FaceObject"].append(FaceObject);
		}
		imageObject["FaceList"] = FaceObjectArray;
	}
	if (!Image.MotorVehicleList.empty()) {
		Json::Value MotorVehiclesObjectArray;
		for (std::list<tagGat1400Motor>::const_iterator iterMotor = Image.MotorVehicleList.begin(); iterMotor != Image.MotorVehicleList.end(); ++iterMotor) {
			Json::Value MotorVehicleseObject = _PackMotorVehicle(*iterMotor);
			MotorVehiclesObjectArray["MotorVehicleObject"].append(MotorVehicleseObject);
		}
		imageObject["MotorVehicleList"] = MotorVehiclesObjectArray;
	}
	if (!Image.NonMotorVehicleList.empty())	{
		Json::Value NonmotorVehiclesObjectArray;
		for (std::list<tagGat1400NonMotor>::const_iterator iterNonMotor = Image.NonMotorVehicleList.begin(); iterNonMotor != Image.NonMotorVehicleList.end(); ++iterNonMotor) {
			Json::Value NonmotorVehiclesObject = _PackNonmotorVehicle(*iterNonMotor);
			NonmotorVehiclesObjectArray["NonMotorVehicleObject"].append(NonmotorVehiclesObject);
		}
		imageObject["NonMotorVehicleList"] = NonmotorVehiclesObjectArray;
	}
	if (!Image.ThingList.empty()) {
		Json::Value ThingObjectArray;
		for (std::list<tagGat1400Thing>::const_iterator iterThing = Image.ThingList.begin(); iterThing != Image.ThingList.end(); ++iterThing) {
			Json::Value ThingObject = _PackThing(*iterThing);
			ThingObjectArray["ThingObject"].append(ThingObject);
		}
		imageObject["ThingList"] = ThingObjectArray;
	}
	if (!Image.SceneList.empty()) {
		Json::Value SceneObjectArray;
		for (std::list<tagGat1400Scene>::const_iterator iterScene = Image.SceneList.begin(); iterScene != Image.SceneList.end(); ++iterScene) {
			Json::Value SceneObject = _PackScene(*iterScene);
			SceneObjectArray["SceneObject"].append(SceneObject);
		}
		imageObject["SceneList"] = SceneObjectArray;
	}
	if (!Image.Data.empty()) {
		imageObject["Data"] = Image.Data;
	}
	return imageObject;
}

static Json::Value _PackFileInfo(const GAT_1400_File& FileInfo)
{
	Json::Value fileInfoObject;
	fileInfoObject["FileID"] = Json::Value(FileInfo.FileID);
	fileInfoObject["InfoKind"] = Json::Value(FileInfo.InfoKind);
	fileInfoObject["Source"] = Json::Value(FileInfo.Source);
	fileInfoObject["FileName"] = Json::Value(FileInfo.FileName);
	fileInfoObject["FileHash"] = Json::Value(FileInfo.FileHash);
	fileInfoObject["FileFormat"] = Json::Value(FileInfo.FileFormat);
	fileInfoObject["Title"] = Json::Value(FileInfo.Title);
	char szSecurityLevel[12] = {0};
	sprintf(szSecurityLevel, "%d", FileInfo.SecurityLevel);
	fileInfoObject["SecurityLevel"] = Json::Value(szSecurityLevel);
	fileInfoObject["SubmiterName"] = Json::Value(FileInfo.SubmiterName);
	fileInfoObject["SubmiterOrg"] = Json::Value(FileInfo.SubmiterOrg);
	fileInfoObject["EntryTime"] = Json::Value(FileInfo.EntryTime);
	fileInfoObject["FileSize"] = Json::Value(FileInfo.FileSize);
	return fileInfoObject;
}

static Json::Value _PackFile(const GAT_1400_FileSet& file)
{
	Json::Value fileObject;
	fileObject["FileInfo"] = _PackFileInfo(file.FileInfo);

	if (!file.PersonList.empty()) {
		Json::Value PersonsObjectArray;
		for (std::list<tagGat1400Person>::const_iterator iterPerson = file.PersonList.begin(); iterPerson != file.PersonList.end(); ++iterPerson) {
			Json::Value PersonsObject = _PackPerson(*iterPerson);
			PersonsObjectArray["PersonObject"].append(PersonsObject);
		}
		fileObject["PersonList"] = PersonsObjectArray;
	}
	if (!file.FaceList.empty()) {
		Json::Value FaceObjectArray;
		for (std::list<GAT_1400_Face>::const_iterator iterFace = file.FaceList.begin(); iterFace != file.FaceList.end(); ++iterFace) {
			Json::Value FaceObject = _PackFace(*iterFace);
			FaceObjectArray["FaceObject"].append(FaceObject);
		}
		fileObject["FaceList"] = FaceObjectArray;
	}
	if (!file.MotorVehicleList.empty()) {
		Json::Value MotorVehiclesObjectArray;
		for (std::list<tagGat1400Motor>::const_iterator iterMotor = file.MotorVehicleList.begin(); iterMotor != file.MotorVehicleList.end(); ++iterMotor) {
			Json::Value MotorVehicleseObject = _PackMotorVehicle(*iterMotor);
			MotorVehiclesObjectArray["MotorVehicleObject"].append(MotorVehicleseObject);
		}
		fileObject["MotorVehicleList"] = MotorVehiclesObjectArray;
	}
	if (!file.NonMotorVehicleList.empty())	{
		Json::Value NonmotorVehiclesObjectArray;
		for (std::list<tagGat1400NonMotor>::const_iterator iterNonMotor = file.NonMotorVehicleList.begin(); iterNonMotor != file.NonMotorVehicleList.end(); ++iterNonMotor) {
			Json::Value NonmotorVehiclesObject = _PackNonmotorVehicle(*iterNonMotor);
			NonmotorVehiclesObjectArray["NonMotorVehicleObject"].append(NonmotorVehiclesObject);
		}
		fileObject["NonMotorVehicleList"] = NonmotorVehiclesObjectArray;
	}
	if (!file.ThingList.empty()) {
		Json::Value ThingObjectArray;
		for (std::list<tagGat1400Thing>::const_iterator iterThing = file.ThingList.begin(); iterThing != file.ThingList.end(); ++iterThing) {
			Json::Value ThingObject = _PackThing(*iterThing);
			ThingObjectArray["ThingObject"].append(ThingObject);
		}
		fileObject["ThingList"] = ThingObjectArray;
	}
	if (!file.SceneList.empty()) {
		Json::Value SceneObjectArray;
		for (std::list<tagGat1400Scene>::const_iterator iterScene = file.SceneList.begin(); iterScene != file.SceneList.end(); ++iterScene) {
			Json::Value SceneObject = _PackScene(*iterScene);
			SceneObjectArray["SceneObject"].append(SceneObject);
		}
		fileObject["SceneList"] = SceneObjectArray;
	}
	if (!file.Data.empty()) {
		fileObject["Data"] = file.Data;
	}

	return fileObject;
}

static Json::Value _PackCaseInfo(const GAT_1400_Case& CaseInfo)
{
	Json::Value CaseInfoObject;
	CaseInfoObject["CaseID"] = Json::Value(CaseInfo.CaseID);
	CaseInfoObject["CaseLinkMark"] = Json::Value(CaseInfo.CaseLinkMark);
	CaseInfoObject["CaseName"] = Json::Value(CaseInfo.CaseName);
	CaseInfoObject["CaseAbstract"] = Json::Value(CaseInfo.CaseAbstract);
	CaseInfoObject["ClueID"] = Json::Value(CaseInfo.ClueID);
	CaseInfoObject["TimeUpLimit"] = Json::Value(CaseInfo.TimeUpLimit);
	CaseInfoObject["TimeLowLimit"] = Json::Value(CaseInfo.TimeLowLimit);
	CaseInfoObject["CreateTime"] = Json::Value(CaseInfo.CreateTime);
	CaseInfoObject["PlaceCode"] = Json::Value(CaseInfo.PlaceCode);
	CaseInfoObject["PlaceFullAddress"] = Json::Value(CaseInfo.PlaceFullAddress);
	CaseInfoObject["State"] = Json::Value(CaseInfo.State);
	return CaseInfoObject;
}

static Json::Value _PackCase(const GAT_1400_CaseSet& Case)
{
	Json::Value CaseObject;
	CaseObject["CaseInfo"] = _PackCaseInfo(Case.CaseInfo);

	if (!Case.VideoSliceList.empty()) {
		Json::Value VideoSliceObjectArray;
		for (std::list<GAT_1400_VideoSliceSet>::const_iterator iter = Case.VideoSliceList.begin(); iter != Case.VideoSliceList.end(); ++iter) {
			Json::Value VideoSliceObject = _PackVideoSlice(*iter);
			VideoSliceObjectArray["VideoSlice"].append(VideoSliceObject);
		}
		CaseObject["VideoSliceList"] = VideoSliceObjectArray;
	}

	if (!Case.ImageList.empty()) {
		Json::Value imageObjectList;
		for (std::list<GAT_1400_ImageSet>::const_iterator iter = Case.ImageList.begin(); iter != Case.ImageList.end(); ++iter) {
			Json::Value imageObject = _PackImage(*iter);
			imageObjectList["Image"].append(imageObject);
		}
		CaseObject["ImageList"] = imageObjectList;
	}

	if (!Case.FileList.empty()) {
		Json::Value fileObjectList;
		for (std::list<GAT_1400_FileSet>::const_iterator iter = Case.FileList.begin(); iter != Case.FileList.end(); ++iter) {
			Json::Value fileObject = _PackFile(*iter);
			fileObjectList["File"].append(fileObject);
		}
		CaseObject["FileList"] = fileObjectList;
	}

	if (!Case.PersonList.empty()) {
		Json::Value PersonsObjectArray;
		for (std::list<tagGat1400Person>::const_iterator iterPerson = Case.PersonList.begin(); iterPerson != Case.PersonList.end(); ++iterPerson) {
			Json::Value PersonsObject = _PackPerson(*iterPerson);
			PersonsObjectArray["PersonObject"].append(PersonsObject);
		}
		CaseObject["PersonList"] = PersonsObjectArray;
	}
	if (!Case.FaceList.empty()) {
		Json::Value FaceObjectArray;
		for (std::list<GAT_1400_Face>::const_iterator iterFace = Case.FaceList.begin(); iterFace != Case.FaceList.end(); ++iterFace) {
			Json::Value FaceObject = _PackFace(*iterFace);
			FaceObjectArray["FaceObject"].append(FaceObject);
		}
		CaseObject["FaceList"] = FaceObjectArray;
	}
	if (!Case.MotorVehicleList.empty()) {
		Json::Value MotorVehiclesObjectArray;
		for (std::list<tagGat1400Motor>::const_iterator iterMotor = Case.MotorVehicleList.begin(); iterMotor != Case.MotorVehicleList.end(); ++iterMotor) {
			Json::Value MotorVehicleseObject = _PackMotorVehicle(*iterMotor);
			MotorVehiclesObjectArray["MotorVehicleObject"].append(MotorVehicleseObject);
		}
		CaseObject["MotorVehicleList"] = MotorVehiclesObjectArray;
	}
	if (!Case.NonMotorVehicleList.empty())	{
		Json::Value NonmotorVehiclesObjectArray;
		for (std::list<tagGat1400NonMotor>::const_iterator iterNonMotor = Case.NonMotorVehicleList.begin(); iterNonMotor != Case.NonMotorVehicleList.end(); ++iterNonMotor) {
			Json::Value NonmotorVehiclesObject = _PackNonmotorVehicle(*iterNonMotor);
			NonmotorVehiclesObjectArray["NonMotorVehicleObject"].append(NonmotorVehiclesObject);
		}
		CaseObject["NonMotorVehicleList"] = NonmotorVehiclesObjectArray;
	}
	if (!Case.ThingList.empty()) {
		Json::Value ThingObjectArray;
		for (std::list<tagGat1400Thing>::const_iterator iterThing = Case.ThingList.begin(); iterThing != Case.ThingList.end(); ++iterThing) {
			Json::Value ThingObject = _PackThing(*iterThing);
			ThingObjectArray["ThingObject"].append(ThingObject);
		}
		CaseObject["ThingList"] = ThingObjectArray;
	}
	if (!Case.SceneList.empty()) {
		Json::Value SceneObjectArray;
		for (std::list<tagGat1400Scene>::const_iterator iterScene = Case.SceneList.begin(); iterScene != Case.SceneList.end(); ++iterScene) {
			Json::Value SceneObject = _PackScene(*iterScene);
			SceneObjectArray["SceneObject"].append(SceneObject);
		}
		CaseObject["SceneList"] = SceneObjectArray;
	}

	return CaseObject;
}

static Json::Value _PackApe(const GAT_1400_Ape& Ape)
{
	Json::Value apeObject;
	apeObject["ApeID"] = Json::Value(Ape.ApeID);
	apeObject["Name"] = Json::Value(Ape.Name);
	apeObject["Model"] = Json::Value(Ape.Model);
	apeObject["IPAddr"] = Json::Value(Ape.IPAddr);
	apeObject["Port"] = Json::Value(Ape.Port);
	apeObject["Longitude"] = Json::Value(Ape.Longitude);
	apeObject["Latitude"] = Json::Value(Ape.Latitude);
	apeObject["PlaceCode"] = Json::Value(Ape.PlaceCode);
	apeObject["IsOnline"] = Json::Value(Ape.IsOnline);
	apeObject["FunctionType"] = Json::Value(Ape.FunctionType);
	return apeObject;
}

static Json::Value _PackTollgate(const GAT_1400_Tollgate& Tollgate)
{
	Json::Value tollgateObject;
	tollgateObject["TollgateID"] = Json::Value(Tollgate.TollgateID);
	tollgateObject["Name"] = Json::Value(Tollgate.Name);
	tollgateObject["Longitude"] = Json::Value(Tollgate.Longitude);
	tollgateObject["Latitude"] = Json::Value(Tollgate.Latitude);
	tollgateObject["PlaceCode"] = Json::Value(Tollgate.PlaceCode);
	tollgateObject["Place"] = Json::Value(Tollgate.Place);
	tollgateObject["Status"] = Json::Value(Tollgate.Status);
	tollgateObject["TollgateCat"] = Json::Value(Tollgate.TollgateCat);
	tollgateObject["TollgateUsage"] = Json::Value(Tollgate.TollgateUsage);
	tollgateObject["LaneNum"] = Json::Value(Tollgate.LaneNum);
	tollgateObject["OrgCode"] = Json::Value(Tollgate.OrgCode);
	tollgateObject["ActiveTime"] = Json::Value(Tollgate.ActiveTime);
	return tollgateObject;
}

static Json::Value _PackLane(const GAT_1400_Tollgate_Lane& Lane)
{
	Json::Value laneObject;
	laneObject["TollgateID"] = Json::Value(Lane.TollgateID);
	laneObject["LaneId"] = Json::Value(Lane.LaneID);
	laneObject["LaneNo"] = Json::Value(Lane.LaneNo);
	laneObject["Name"] = Json::Value(Lane.Name);
	laneObject["ApeID"] = Json::Value(Lane.ApeID);
	return laneObject;
}

static Json::Value _PackAnalysisRule(const GAT_1400_AnalysisRule& Rule)
{
	Json::Value analysisRuleObject;
	analysisRuleObject["AnalysisRuleID"] = Json::Value(Rule.AnalysisRuleID);
	analysisRuleObject["VideoImageID"] = Json::Value(Rule.VideoImageID);
	analysisRuleObject["VideoImageUrl"] = Json::Value(Rule.VideoImageUrl);
	analysisRuleObject["CameraID"] = Json::Value(Rule.CameraID);
	analysisRuleObject["Width"] = Json::Value(Rule.Width);
	analysisRuleObject["Height"] = Json::Value(Rule.Height);
	analysisRuleObject["MinDuration"] = Json::Value(Rule.MinDuration);
	return analysisRuleObject;
}

static Json::Value _PackVideoLabel(const GAT_1400_VideoLabel& VideoLabel)
{
	Json::Value videoLabelObject;
	videoLabelObject["VideoLabelID"] = Json::Value(VideoLabel.VideoLabelID);
	videoLabelObject["EventSort"] = Json::Value(VideoLabel.EventSort);
	videoLabelObject["EventRuleID"] = Json::Value(VideoLabel.EventRuleID);
	videoLabelObject["VideoImageID"] = Json::Value(VideoLabel.VideoImageID);
	videoLabelObject["VideoImageUrl"] = Json::Value(VideoLabel.VideoImageUrl);
	videoLabelObject["CameraID"] = Json::Value(VideoLabel.CameraID);
	videoLabelObject["CreateTimeAbs"] = Json::Value(VideoLabel.CreateTimeAbs);
	videoLabelObject["CreateTimeRlt"] = Json::Value(VideoLabel.CreateTimeRlt);
	videoLabelObject["CreateFrameNoRlt"] = Json::Value((unsigned int)VideoLabel.CreateFrameNoRlt);
	return videoLabelObject;
}

//////////////////////////////////////////////////////////////////////////
//外部接口
//////////////////////////////////////////////////////////////////////////
std::string GAT1400Json::PackRegisterJson(const char* localID)
{
	Json::Value root;  // 表示整个 json 对象
	Json::Value id;
	id["DeviceID"] = Json::Value(localID);
	root["RegisterObject"] = id;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

int GAT1400Json::UnPackResponseStatus(const std::string& strJson, GAT1400_RESPONSESTATUS_ST& result)
{
	Json::Reader reader;
	Json::Value json_object;
	if (!reader.parse(strJson, json_object)) {
		TVT_LOG_ERROR("UnPackResponseStatus fail: cannot parse json");
		return -1;
	}
	if (!json_object.isObject()) {
		TVT_LOG_ERROR("UnPackResponseStatus fail: json_object is null");
		return -1;
	}

	Json::Value status_object = json_object["ResponseStatusObject"];
	if (status_object.empty()) {
		TVT_LOG_ERROR("UnPackResponseStatus fail: ResponseStatusObject is null");
		return -1;
	}

	if (status_object.isMember("RequestURL") && status_object["RequestURL"].isString()) {
		strncpy(result.ReqeustURL, status_object["RequestURL"].asString().c_str(), sizeof(result.ReqeustURL)-1);
	}

	if (status_object.isMember("StatusCode") && status_object["StatusCode"].isString()) {
		result.StatusCode = atoi(status_object["StatusCode"].asString().c_str());
	}
	if (status_object.isMember("StatusString") && status_object["StatusString"].isString()) {
		strncpy(result.StatusString, status_object["StatusString"].asString().c_str(), sizeof(result.StatusString)-1);
	}
	if (status_object.isMember("Id") && status_object["Id"].isString()) {
		strncpy(result.ID, status_object["Id"].asString().c_str(), sizeof(result.ID)-1);
	}
	if (status_object.isMember("LocalTime") && status_object["LocalTime"].isString()) {
		strncpy(result.LocalTime, status_object["LocalTime"].asString().c_str(), sizeof(result.LocalTime)-1);
	}
	
	return 0;
}

int GAT1400Json::UnPackResponseStatusList(const std::string& strJson, std::list<GAT1400_RESPONSESTATUS_ST>& resultList)
{
	Json::Reader reader;
	Json::Value json_object;
	if (!reader.parse(strJson, json_object)) {
		TVT_LOG_ERROR("UnPackResponseStatusList fail: cannot parse json");
		return -1;
	}
	if (!json_object.isObject()) {
		TVT_LOG_ERROR("UnPackResponseStatusList fail: json_object is null");
		return -1;
	}

	Json::Value status_object_list = json_object["ResponseStatusListObject"];
	if (status_object_list.empty()) {
		TVT_LOG_ERROR("UnPackResponseStatusList fail: ResponseStatusListObject is null");
		return -1;
	}

	if (status_object_list.isMember("ResponseStatusObject") && status_object_list["ResponseStatusObject"].isArray()) {
		Json::Value arrayObj = status_object_list["ResponseStatusObject"];
		for (unsigned int i = 0; i < arrayObj.size(); ++i)	{
			GAT1400_RESPONSESTATUS_ST oneResult;
			if (arrayObj[i].isMember("RequestURL") && arrayObj[i]["RequestURL"].isString()) {
				strncpy(oneResult.ReqeustURL, arrayObj[i]["RequestURL"].asString().c_str(), sizeof(oneResult.ReqeustURL)-1);
			}

			if (arrayObj[i].isMember("StatusCode") && arrayObj[i]["StatusCode"].isInt()) {
				oneResult.StatusCode = arrayObj[i]["StatusCode"].asInt();
			}

			if (arrayObj[i].isMember("StatusString") && arrayObj[i]["StatusString"].isString()) {
				strncpy(oneResult.StatusString, arrayObj[i]["StatusString"].asString().c_str(), sizeof(oneResult.StatusString)-1);
			}

			if (arrayObj[i].isMember("Id") && arrayObj[i]["Id"].isString()) {
				strncpy(oneResult.ID, arrayObj[i]["Id"].asString().c_str(), sizeof(oneResult.ID)-1);
			}

			if (arrayObj[i].isMember("LocalTime") && arrayObj[i]["LocalTime"].isString()) {
				strncpy(oneResult.LocalTime, arrayObj[i]["LocalTime"].asString().c_str(), sizeof(oneResult.LocalTime)-1);
			}
			resultList.push_back(oneResult);
		}
	}

	return 0;
}

std::string GAT1400Json::PackResponseStatusList(const std::list<GAT1400_RESPONSESTATUS_ST>& resultList)
{
	Json::Value root;
	Json::Value ResponseObjectArray;

	for (std::list<GAT1400_RESPONSESTATUS_ST>::const_iterator iter = resultList.begin(); iter != resultList.end(); ++iter) {
		Json::Value ResponseStatusObject;
		ResponseStatusObject["RequestURL"] = Json::Value(iter->ReqeustURL);
		ResponseStatusObject["StatusCode"] = Json::Value(iter->StatusCode);
		ResponseStatusObject["StatusString"] = Json::Value(iter->StatusString);
		ResponseStatusObject["Id"] = Json::Value(iter->ID);
		ResponseStatusObject["LocalTime"] = Json::Value(iter->LocalTime);
		ResponseObjectArray["ResponseStatusObject"].append(ResponseStatusObject);
	}
	root["ResponseStatusListObject"] = ResponseObjectArray;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackUnRegisterJson(const char* localID)
{
	Json::Value root;
	Json::Value id;
	id["DeviceID"] = Json::Value(localID);
	root["UnRegisterObject"] = id;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackKeepAliveJson(const char* localID)
{
	Json::Value root;
	Json::Value id;
	id["DeviceID"] = Json::Value(localID);
	root["KeepaliveObject"] = id;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

int GAT1400Json::UnPackSystemTime(const std::string& strJson, GAT1400_SYSTEMTIME_ST& result)
{
	Json::Reader reader;
	Json::Value json_object;
	if (!reader.parse(strJson, json_object)) {
		TVT_LOG_ERROR("UnPackSystemTime fail: cannot parse json");
		return -1;
	}
	if (!json_object.isObject()) {
		TVT_LOG_ERROR("UnPackSystemTime fail: json_object is null");
		return -1;
	}

	Json::Value status_object = json_object["SystemTimeObject"];
	if (status_object.empty()) {
		TVT_LOG_ERROR("UnPackSystemTime fail: SystemTimeObject is null");
		return -1;
	}

	if (status_object.isMember("LocalTime") && status_object["LocalTime"].isString()) {
		strncpy(result.LocalTime, status_object["LocalTime"].asString().c_str(), sizeof(result.LocalTime)-1);
	}
	if (status_object.isMember("TimeZone") && status_object["TimeZone"].isString()) {
		strncpy(result.TimeZone, status_object["TimeZone"].asString().c_str(), sizeof(result.TimeZone)-1);
	}
	if (status_object.isMember("TimeMode") && status_object["TimeMode"].isString()) {
		strncpy(result.TimeMode, status_object["TimeMode"].asString().c_str(), sizeof(result.TimeMode)-1);
	}
	if (status_object.isMember("VIIDServerID") && status_object["VIIDServerID"].isString()) {
		strncpy(result.VIIDServerID, status_object["VIIDServerID"].asString().c_str(), sizeof(result.VIIDServerID)-1);
	}

	return 0;
}

std::string GAT1400Json::PackFaceJson(const GAT_1400_Face& Face)
{
	Json::Value root;
	Json::Value FaceObject = _PackFace(Face);
	root["FaceObject"] = FaceObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackFaceListJson(const std::list<GAT_1400_Face>& FaceList)
{
	Json::Value root;
	Json::Value FaceObjectArray;

	for (std::list<GAT_1400_Face>::const_iterator iter = FaceList.begin(); iter != FaceList.end(); ++iter) {
		Json::Value FaceObject = _PackFace(*iter);
		FaceObjectArray["FaceObject"].append(FaceObject);
	}
	root["FaceListObject"] = FaceObjectArray;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackPersonJson(const GAT_1400_Person& Person)
{
	Json::Value root;
	Json::Value PersonObject = _PackPerson(Person);
	root["PersonObject"] = PersonObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackPersonListJson(const std::list<GAT_1400_Person>& PersonList)
{
	Json::Value root;
	Json::Value PersonsObjectArray;

	for (std::list<GAT_1400_Person>::const_iterator iter = PersonList.begin(); iter != PersonList.end(); ++iter) {
		Json::Value PersonsObject = _PackPerson(*iter);
		PersonsObjectArray["PersonObject"].append(PersonsObject);
	}
	root["PersonListObject"] = PersonsObjectArray;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackMotorVehicleJson(const GAT_1400_Motor& MotorVehicle)
{
	Json::Value root;
	Json::Value MotorVehicleseObject = _PackMotorVehicle(MotorVehicle);
	root["MotorVehicleObject"] = MotorVehicleseObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackMotorVehicleListJson(const std::list<GAT_1400_Motor>& MotorVehicleList)
{
	Json::Value root;
	Json::Value MotorVehiclesObjectArray;

	for (std::list<GAT_1400_Motor>::const_iterator iter = MotorVehicleList.begin(); iter != MotorVehicleList.end(); ++iter) {
		Json::Value MotorVehicleseObject = _PackMotorVehicle(*iter);
		MotorVehiclesObjectArray["MotorVehicleObject"].append(MotorVehicleseObject);
	}
	root["MotorVehicleListObject"] = MotorVehiclesObjectArray;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackNonMotorVehicleJson(const GAT_1400_NonMotor& NonMotorVehicle)
{
	Json::Value root;
	Json::Value NonMotorVehicleseObject = _PackNonmotorVehicle(NonMotorVehicle);
	root["NonMotorVehicleObject"] = NonMotorVehicleseObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackNonmotorVehicleListJson(const std::list<GAT_1400_NonMotor>& NonmotorVehicleList)
{
	Json::Value root;
	Json::Value NonmotorVehiclesObjectArray;

	for (std::list<GAT_1400_NonMotor>::const_iterator iter = NonmotorVehicleList.begin(); iter != NonmotorVehicleList.end(); ++iter) {
		Json::Value NonmotorVehiclesObject = _PackNonmotorVehicle(*iter);
		NonmotorVehiclesObjectArray["NonMotorVehicleObject"].append(NonmotorVehiclesObject);
	}
	root["NonMotorVehicleListObject"] = NonmotorVehiclesObjectArray;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackThingJson(const GAT_1400_Thing& Thing)
{
	Json::Value root;
	Json::Value ThingObject = _PackThing(Thing);
	root["ThingObject"] = ThingObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackThingListJson(const std::list<GAT_1400_Thing>& ThingList)
{
	Json::Value root;
	Json::Value ThingObjectArray;

	for (std::list<GAT_1400_Thing>::const_iterator iter = ThingList.begin(); iter != ThingList.end(); ++iter) {
		Json::Value ThingObject = _PackThing(*iter);
		ThingObjectArray["ThingObject"].append(ThingObject);
	}
	root["ThingListObject"] = ThingObjectArray;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackSceneJson(const GAT_1400_Scene& Scene)
{
	Json::Value root;
	Json::Value SceneObject = _PackScene(Scene);
	root["SceneObject"] = SceneObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackSceneListJson(const std::list<GAT_1400_Scene>& SceneList)
{
	Json::Value root;
	Json::Value SceneObjectArray;

	for (std::list<GAT_1400_Scene>::const_iterator iter = SceneList.begin(); iter != SceneList.end(); ++iter) {
		Json::Value SceneObject = _PackScene(*iter);
		SceneObjectArray["SceneObject"].append(SceneObject);
	}
	root["SceneListObject"] = SceneObjectArray;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackDispositionJson(const GAT_1400_Disposition& Disposition)
{
	Json::Value root;
	Json::Value DispositionObject = _PackDisposition(Disposition);
	root["DispositionObject"] = DispositionObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackDispositionListJson(const std::list<GAT_1400_Disposition>& DispositionList)
{
	Json::Value root;
	Json::Value DispositionObjectArray;

	for (std::list<GAT_1400_Disposition>::const_iterator iter = DispositionList.begin(); iter != DispositionList.end(); ++iter) {
		Json::Value DispositionObject = _PackDisposition(*iter);
		DispositionObjectArray["DispositionObject"].append(DispositionObject);
	}
	root["DispositionListObject"] = DispositionObjectArray;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string PackApeJson(const GAT_1400_Ape& ape)
{
	Json::Value root;
	Json::Value ApeObject = _PackApe(ape);
	root["APEObject"] = ApeObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackApeListJson(const std::list<GAT_1400_Ape>& apeList)
{
	Json::Value root;
	Json::Value ApeObjectList;
	for (std::list<GAT_1400_Ape>::const_iterator iter = apeList.begin(); iter != apeList.end(); ++iter) {
		Json::Value apeObject = _PackApe(*iter);
		ApeObjectList["APEObject"].append(apeObject);
	}
	root["APEListObject"] = ApeObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackTollgateJson(const GAT_1400_Tollgate& tollgate)
{
	Json::Value root;
	Json::Value tollgateObject = _PackTollgate(tollgate);
	root["TollgateObject"] = tollgateObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackTollgateListJson(const std::list<GAT_1400_Tollgate>& tollgateList)
{
	Json::Value root;
	Json::Value tollgateObjectList;
	for (std::list<GAT_1400_Tollgate>::const_iterator iter = tollgateList.begin(); iter != tollgateList.end(); ++iter) {
		Json::Value tollgateObject = _PackTollgate(*iter);
		tollgateObjectList["TollgateObject"].append(tollgateObject);
	}
	root["TollgateListObject"] = tollgateObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackLaneJson(const GAT_1400_Tollgate_Lane& lane)
{
	Json::Value root;
	Json::Value laneObject = _PackLane(lane);
	root["LaneObject"] = laneObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackLaneListJson(const std::list<GAT_1400_Tollgate_Lane>& laneList)
{
	Json::Value root;
	Json::Value laneObjectList;
	for (std::list<GAT_1400_Tollgate_Lane>::const_iterator iter = laneList.begin(); iter != laneList.end(); ++iter) {
		Json::Value laneObject = _PackLane(*iter);
		laneObjectList["LaneObject"].append(laneObject);
	}
	root["LaneListObject"] = laneObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackVideoSliceJson(const GAT_1400_VideoSliceSet& VideoSlice)
{
	Json::Value root;
	Json::Value VideoSliceObject = _PackVideoSlice(VideoSlice);
	root["VideoSlice"] = VideoSliceObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackVideoSliceListJson(const std::list<GAT_1400_VideoSliceSet>& videoSliceList)
{
	Json::Value root;
	Json::Value VideoSliceObjectArray;

	for (std::list<GAT_1400_VideoSliceSet>::const_iterator iter = videoSliceList.begin(); iter != videoSliceList.end(); ++iter) {
		Json::Value VideoSliceObject = _PackVideoSlice(*iter);
		VideoSliceObjectArray["VideoSlice"].append(VideoSliceObject);
	}
	root["VideoSliceList"] = VideoSliceObjectArray;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackVideoSliceInfoJson(const GAT_1400_VideoSliceInfo& VideoSliceInfo)
{
	Json::Value root;
	Json::Value VideoSliceInfoObject = _PackVideoSliceInfo(VideoSliceInfo);
	root["VideoSliceInfo"] = VideoSliceInfoObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackImageListJson(const std::list<GAT_1400_ImageSet>& ImageList)
{
	Json::Value root;
	Json::Value imageObjectList;
	for (std::list<GAT_1400_ImageSet>::const_iterator iter = ImageList.begin(); iter != ImageList.end(); ++iter) {
		Json::Value imageObject = _PackImage(*iter);
		imageObjectList["Image"].append(imageObject);
	}
	root["ImageListObject"] = imageObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackImageJson(const GAT_1400_ImageSet& Image)
{
	Json::Value root;
	Json::Value ImageObject = _PackImage(Image);
	root["Image"] = ImageObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackImageInfoJson(const GAT_1400_Image& ImageInfo)
{
	Json::Value root;
	Json::Value ImageInfoObject = _PackImageInfo(ImageInfo);
	root["ImageInfo"] = ImageInfoObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackFileListJson(const std::list<GAT_1400_FileSet>& FileList)
{
	Json::Value root;
	Json::Value fileObjectList;
	for (std::list<GAT_1400_FileSet>::const_iterator iter = FileList.begin(); iter != FileList.end(); ++iter) {
		Json::Value fileObject = _PackFile(*iter);
		fileObjectList["File"].append(fileObject);
	}
	root["FileListObject"] = fileObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackFileJson(const GAT_1400_FileSet& File)
{
	Json::Value root;
	Json::Value FileObject = _PackFile(File);
	root["File"] = FileObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackFileInfoJson(const GAT_1400_File& FileInfo)
{
	Json::Value root;
	Json::Value FileInfoObject = _PackFileInfo(FileInfo);
	root["FileInfo"] = FileInfoObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackAnalysisRuleJson(const GAT_1400_AnalysisRule& rule)
{
	Json::Value root;
	Json::Value analysisRuleObject = _PackAnalysisRule(rule);
	root["AnalysisRuleObject"] = analysisRuleObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackAnalysisRuleListJson(const std::list<GAT_1400_AnalysisRule>& ruleList)
{
	Json::Value root;
	Json::Value AnalysisRuleObjectList;
	for (std::list<GAT_1400_AnalysisRule>::const_iterator iter = ruleList.begin(); iter != ruleList.end(); ++iter) {
		Json::Value analysisRuleObject = _PackAnalysisRule(*iter);
		AnalysisRuleObjectList["AnalysisRuleObject"].append(analysisRuleObject);
	}
	root["AnalysisRuleListObject"] = AnalysisRuleObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackVideoLabelJson(const GAT_1400_VideoLabel& label)
{
	Json::Value root;
	Json::Value videoLabelObject = _PackVideoLabel(label);
	root["VideoLabelObject"] = videoLabelObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackVideoLabelListJson(const std::list<GAT_1400_VideoLabel>& labelList)
{
	Json::Value root;
	Json::Value VideoLabelObjectList;
	for (std::list<GAT_1400_VideoLabel>::const_iterator iter = labelList.begin(); iter != labelList.end(); ++iter) {
		Json::Value videoLabelObject = _PackVideoLabel(*iter);
		VideoLabelObjectList["VideoLabelObject"].append(videoLabelObject);
	}
	root["VideoLabelListObject"] = VideoLabelObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackDispositionNotificationListJson(const std::list<GAT_1400_Disposition_Notification>& DispositionNotificationList)
{
	Json::Value root;
	Json::Value DispositionNotificationObjectList;
	for (std::list<GAT_1400_Disposition_Notification>::const_iterator iter = DispositionNotificationList.begin(); iter != DispositionNotificationList.end(); ++iter) {
		Json::Value DispositionNotificationObject;

		DispositionNotificationObject["NotificationID"] = Json::Value(iter->NotificationID);
		DispositionNotificationObject["DispositionID"] = Json::Value(iter->DispositionID);
		DispositionNotificationObject["Title"] = Json::Value(iter->Title);
		DispositionNotificationObject["TriggerTime"] = Json::Value(iter->TriggerTime);
		DispositionNotificationObject["CntObjectID"] = Json::Value(iter->CntObjectID);
		
		DispositionNotificationObject["PersonObject"] = _PackPerson(iter->PersonObject);
		DispositionNotificationObject["MotorVehicleObject"] = _PackMotorVehicle(iter->MotorVehicleObject);

		DispositionNotificationObjectList["DispositionNotificationObject"].append(DispositionNotificationObject);
	}
	root["DispositionNotificationListObject"] = DispositionNotificationObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackSubscribeListJson(const std::list<GAT_1400_Subscribe>& SubscribeList)
{
	Json::Value root;
	Json::Value SubscribeObjectList;
	for (std::list<GAT_1400_Subscribe>::const_iterator iter = SubscribeList.begin(); iter != SubscribeList.end(); ++iter) {
		Json::Value SubscribeObject;

		SubscribeObject["SubscribeID"] = Json::Value(iter->SubscribeID);
		SubscribeObject["Title"] = Json::Value(iter->Title);
		SubscribeObject["SubscribeDetail"] = Json::Value(iter->SubscribeDetail);
		SubscribeObject["ResourceURI"] = Json::Value(iter->ResourceURI);
		SubscribeObject["ApplicantName"] = Json::Value(iter->ApplicantName);
		SubscribeObject["ApplicantOrg"] = Json::Value(iter->ApplicantOrg);
		SubscribeObject["BeginTime"] = Json::Value(iter->BeginTime);
		SubscribeObject["EndTime"] = Json::Value(iter->EndTime);
		SubscribeObject["ReceiveAddr"] = Json::Value(iter->ReceiveAddr);
		SubscribeObject["ReportInterval"] = Json::Value(iter->ReportInterval);
		SubscribeObject["OperateType"] = Json::Value(iter->OperateType);
		SubscribeObject["SubscribeStatus"] = Json::Value(iter->SubscribeStatus);
		SubscribeObject["Reason"] = Json::Value(iter->Reason);
		SubscribeObject["SubscribeCancelOrg"] = Json::Value(iter->SubscribeCancelOrg);
		SubscribeObject["SubscribeCancelPerson"] = Json::Value(iter->SubscribeCancelPerson);
		SubscribeObject["CancelTime"] = Json::Value(iter->CancelTime);
		SubscribeObject["CancelReason"] = Json::Value(iter->CancelReason);

		SubscribeObjectList["SubscribeObject"].append(SubscribeObject);
	}
	root["SubscribeListObject"] = SubscribeObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackSubscribeNotificationListJson(const std::list<GAT_1400_Subscribe_Notification>& SubscribeNotificationList)
{
	Json::Value root;
	Json::Value SubscribeNotificationObjectList;
	for (std::list<GAT_1400_Subscribe_Notification>::const_iterator iter = SubscribeNotificationList.begin(); iter != SubscribeNotificationList.end(); ++iter) {
		Json::Value SubscribeNotificationObject;

		SubscribeNotificationObject["NotificationID"] = Json::Value(iter->NotificationID);
		SubscribeNotificationObject["SubscribeID"] = Json::Value(iter->SubscribeID);
		SubscribeNotificationObject["Title"] = Json::Value(iter->Title);
		SubscribeNotificationObject["TriggerTime"] = Json::Value(iter->TriggerTime);

		std::string InfoIDs;

		if (!iter->TollgateObjectList.empty()) {
			Json::Value TollgateObjectArray;
			for (std::list<GAT_1400_Tollgate>::const_iterator iterTollgate = iter->TollgateObjectList.begin(); iterTollgate != iter->TollgateObjectList.end(); ++iterTollgate) {
				Json::Value TollgateObject = _PackTollgate(*iterTollgate);
				TollgateObjectArray["TollgateObject"].append(TollgateObject);

				if (!InfoIDs.empty()) {
					InfoIDs += ",";
				}
				InfoIDs += iterTollgate->TollgateID;
			}
			SubscribeNotificationObject["TollgateObjectList"] = TollgateObjectArray;
		}

		if (!iter->LaneObjectList.empty()) {
			Json::Value LaneObjectArray;
			for (std::list<GAT_1400_Tollgate_Lane>::const_iterator iterLane = iter->LaneObjectList.begin(); iterLane != iter->LaneObjectList.end(); ++iterLane) {
				Json::Value LaneObject = _PackLane(*iterLane);
				LaneObjectArray["LaneObject"].append(LaneObject);

				if (!InfoIDs.empty()) {
					InfoIDs += ",";
				}
				InfoIDs += iterLane->TollgateID;
			}
			SubscribeNotificationObject["LaneObjectList"] = LaneObjectArray;
		}

		if (!iter->DeviceList.empty()) {
			Json::Value DeviceObjectArray;
			for (std::list<GAT_1400_Ape>::const_iterator iterApe = iter->DeviceList.begin(); iterApe != iter->DeviceList.end(); ++iterApe) {
				Json::Value DeviceObject = _PackApe(*iterApe);
				DeviceObjectArray["APEObject"].append(DeviceObject);

				if (!InfoIDs.empty()) {
					InfoIDs += ",";
				}
				InfoIDs += iterApe->ApeID;
			}
			SubscribeNotificationObject["DeviceList"] = DeviceObjectArray;
		}

		if (!iter->FaceObjectList.empty()) {
			Json::Value FaceObjectArray;
			for (std::list<GAT_1400_Face>::const_iterator iterFace = iter->FaceObjectList.begin(); iterFace != iter->FaceObjectList.end(); ++iterFace) {
				Json::Value FaceObject = _PackFace(*iterFace);
				FaceObjectArray["FaceObject"].append(FaceObject);

				if (!InfoIDs.empty()) {
					InfoIDs += ",";
				}
				InfoIDs += iterFace->FaceID;
			}
			SubscribeNotificationObject["FaceObjectList"] = FaceObjectArray;
		}

		if (!iter->PersonObjectList.empty()) {
			Json::Value PersonsObjectArray;
			for (std::list<GAT_1400_Person>::const_iterator iterPerson = iter->PersonObjectList.begin(); iterPerson != iter->PersonObjectList.end(); ++iterPerson) {
				Json::Value PersonsObject = _PackPerson(*iterPerson);
				PersonsObjectArray["PersonObject"].append(PersonsObject);

				if (!InfoIDs.empty()) {
					InfoIDs += ",";
				}
				InfoIDs += iterPerson->PersonID;
			}
			SubscribeNotificationObject["PersonObjectList"] = PersonsObjectArray;
		}

		if (!iter->MotorVehicleObjectList.empty()) {
			Json::Value MotorVehiclesObjectArray;
			for (std::list<GAT_1400_Motor>::const_iterator iterMotor = iter->MotorVehicleObjectList.begin(); iterMotor != iter->MotorVehicleObjectList.end(); ++iterMotor) {
				Json::Value MotorVehicleseObject = _PackMotorVehicle(*iterMotor);
				MotorVehiclesObjectArray["MotorVehicleObject"].append(MotorVehicleseObject);

				if (!InfoIDs.empty()) {
					InfoIDs += ",";
				}
				InfoIDs += iterMotor->MotorVehicleID;
			}
			SubscribeNotificationObject["MotorVehicleObjectList"] = MotorVehiclesObjectArray;
		}

		if (!iter->NonMotorVehicleObjectList.empty()) {
			Json::Value NonMotorVehiclesObjectArray;
			for (std::list<GAT_1400_NonMotor>::const_iterator iterNonMotor = iter->NonMotorVehicleObjectList.begin(); iterNonMotor != iter->NonMotorVehicleObjectList.end(); ++iterNonMotor) {
				Json::Value NonMotorVehicleseObject = _PackNonmotorVehicle(*iterNonMotor);
				NonMotorVehiclesObjectArray["NonMotorVehicleObject"].append(NonMotorVehicleseObject);

				if (!InfoIDs.empty()) {
					InfoIDs += ",";
				}
				InfoIDs += iterNonMotor->NonMotorVehicleID;
			}
			SubscribeNotificationObject["NonMotorVehicleObjectList"] = NonMotorVehiclesObjectArray;
		}
		
		if (!iter->ThingObjectList.empty()) {
			Json::Value ThingObjectArray;
			for (std::list<GAT_1400_Thing>::const_iterator iterThing = iter->ThingObjectList.begin(); iterThing != iter->ThingObjectList.end(); ++iterThing) {
				Json::Value ThingObject = _PackThing(*iterThing);
				ThingObjectArray["ThingObject"].append(ThingObject);

				if (!InfoIDs.empty()) {
					InfoIDs += ",";
				}
				InfoIDs += iterThing->ThingID;
			}
			root["ThingObjectList"] = ThingObjectArray;
		}

		if (!iter->SceneObjectList.empty())	{
			Json::Value SceneObjectArray;
			for (std::list<GAT_1400_Scene>::const_iterator iterScene = iter->SceneObjectList.begin(); iterScene != iter->SceneObjectList.end(); ++iterScene) {
				Json::Value SceneObject = _PackScene(*iterScene);
				SceneObjectArray["SceneObject"].append(SceneObject);

				if (!InfoIDs.empty()) {
					InfoIDs += ",";
				}
				InfoIDs += iterScene->SceneID;
			}
			root["SceneObjectList"] = SceneObjectArray;
		}

		SubscribeNotificationObject["InfoIDs"] = Json::Value(InfoIDs);

		SubscribeNotificationObjectList["SubscribeNotificationObject"].append(SubscribeNotificationObject);
	}
	root["SubscribeNotificationListObject"] = SubscribeNotificationObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

int GAT1400Json::UnPackDispositionsList(const std::string& strJson, std::list<GAT_1400_Disposition>& dispositionList)
{
	Json::Reader reader;
	Json::Value json_object;
	if (!reader.parse(strJson, json_object)) {
		TVT_LOG_ERROR("UnPackDispositionsList fail: cannot parse json");
		return -1;
	}
	if (!json_object.isObject()) {
		TVT_LOG_ERROR("UnPackDispositionsList fail: json_object is null");
		return -1;
	}

	Json::Value DispositionListObject = json_object["DispositionListObject"];
	if (DispositionListObject.empty()) {
		TVT_LOG_ERROR("UnPackDispositionsList fail: DispositionListObject is null");
		return -1;
	}

	if (DispositionListObject.isMember("DispositionObject") && DispositionListObject["DispositionObject"].isArray()) {
		Json::Value arrayObj = DispositionListObject["DispositionObject"];
		for (unsigned int i = 0; i < arrayObj.size(); ++i)	{
			GAT_1400_Disposition oneResult;
			if (arrayObj[i].isMember("DispositionID") && arrayObj[i]["DispositionID"].isString()) {
				strncpy(oneResult.DispositionID, arrayObj[i]["DispositionID"].asString().c_str(), sizeof(oneResult.DispositionID)-1);
			}
			if (arrayObj[i].isMember("Title") && arrayObj[i]["Title"].isString()) {
				strncpy(oneResult.Title, arrayObj[i]["Title"].asString().c_str(), sizeof(oneResult.Title)-1);
			}
			if (arrayObj[i].isMember("DispositionCategory") && arrayObj[i]["DispositionCategory"].isString()) {
				oneResult.DispositionCategory = DispositionCategoryType(atoi(arrayObj[i]["DispositionCategory"].asString().c_str()));
			}
			if (arrayObj[i].isMember("TargetImageURI") && arrayObj[i]["TargetImageURI"].isString()) {
				strncpy(oneResult.TargetImageURI, arrayObj[i]["TargetImageURI"].asString().c_str(), sizeof(oneResult.TargetImageURI)-1);
			}
			if (arrayObj[i].isMember("PriorityLevel") && arrayObj[i]["PriorityLevel"].isInt()) {
				oneResult.PriorityLevel = arrayObj[i]["PriorityLevel"].asInt();
			}
			if (arrayObj[i].isMember("ApplicantName") && arrayObj[i]["ApplicantName"].isString()) {
				strncpy(oneResult.ApplicantName, arrayObj[i]["ApplicantName"].asString().c_str(), sizeof(oneResult.ApplicantName)-1);
			}
			if (arrayObj[i].isMember("ApplicantInfo") && arrayObj[i]["ApplicantInfo"].isString()) {
				strncpy(oneResult.ApplicantInfo, arrayObj[i]["ApplicantInfo"].asString().c_str(), sizeof(oneResult.ApplicantInfo)-1);
			}
			if (arrayObj[i].isMember("ApplicantOrg") && arrayObj[i]["ApplicantOrg"].isString()) {
				strncpy(oneResult.ApplicantOrg, arrayObj[i]["ApplicantOrg"].asString().c_str(), sizeof(oneResult.ApplicantOrg)-1);
			}
			if (arrayObj[i].isMember("BeginTime") && arrayObj[i]["BeginTime"].isString()) {
				strncpy(oneResult.BeginTime, arrayObj[i]["BeginTime"].asString().c_str(), sizeof(oneResult.BeginTime)-1);
			}
			if (arrayObj[i].isMember("EndTime") && arrayObj[i]["EndTime"].isString()) {
				strncpy(oneResult.EndTime, arrayObj[i]["EndTime"].asString().c_str(), sizeof(oneResult.EndTime)-1);
			}
			if (arrayObj[i].isMember("CreateTime") && arrayObj[i]["CreateTime"].isString()) {
				strncpy(oneResult.CreateTime, arrayObj[i]["CreateTime"].asString().c_str(), sizeof(oneResult.CreateTime)-1);
			}
			if (arrayObj[i].isMember("OperateType") && arrayObj[i]["OperateType"].isInt()) {
				oneResult.OperateType = arrayObj[i]["OperateType"].asInt();
			}
			if (arrayObj[i].isMember("DispositionStatus") && arrayObj[i]["DispositionStatus"].isInt()) {
				oneResult.DispositionStatus = arrayObj[i]["DispositionStatus"].asInt();
			}
			if (arrayObj[i].isMember("DispositionRange") && arrayObj[i]["DispositionRange"].isInt()) {
				oneResult.DispositionRange = arrayObj[i]["DispositionRange"].asInt();
			}
			
			dispositionList.push_back(oneResult);
		}
	}

	return 0;
}

int GAT1400Json::UnPackSubscribeList(const std::string& strJson, std::list<GAT_1400_Subscribe>& subscribeList)
{
	Json::Reader reader;
	Json::Value json_object;
	if (!reader.parse(strJson, json_object)) {
		TVT_LOG_ERROR("UnPackSubscribeList fail: cannot parse json");
		return -1;
	}
	if (!json_object.isObject()) {
		TVT_LOG_ERROR("UnPackSubscribeList fail: json_object is null");
		return -1;
	}

	Json::Value SubscribeListObject = json_object["SubscribeListObject"];
	if (SubscribeListObject.empty()) {
		TVT_LOG_ERROR("UnPackSubscribeList fail: SubscribeListObject is null");
		return -1;
	}

	if (SubscribeListObject.isMember("SubscribeObject") && SubscribeListObject["SubscribeObject"].isArray()) {
		Json::Value arrayObj = SubscribeListObject["SubscribeObject"];
		for (unsigned int i = 0; i < arrayObj.size(); ++i)	{
			GAT_1400_Subscribe oneResult;
			if (arrayObj[i].isMember("SubscribeID") && arrayObj[i]["SubscribeID"].isString()) {
				strncpy(oneResult.SubscribeID, arrayObj[i]["SubscribeID"].asString().c_str(), sizeof(oneResult.SubscribeID)-1);
			}
			if (arrayObj[i].isMember("Title") && arrayObj[i]["Title"].isString()) {
				strncpy(oneResult.Title, arrayObj[i]["Title"].asString().c_str(), sizeof(oneResult.Title)-1);
			}
			if (arrayObj[i].isMember("SubscribeDetail") && arrayObj[i]["SubscribeDetail"].isString()) {
				oneResult.SubscribeDetail = arrayObj[i]["SubscribeDetail"].asString();
			}
			if (arrayObj[i].isMember("ResourceURI") && arrayObj[i]["ResourceURI"].isString()) {
				strncpy(oneResult.ResourceURI, arrayObj[i]["ResourceURI"].asString().c_str(), sizeof(oneResult.ResourceURI)-1);
			}
			if (arrayObj[i].isMember("ApplicantName") && arrayObj[i]["ApplicantName"].isString()) {
				strncpy(oneResult.ApplicantName, arrayObj[i]["ApplicantName"].asString().c_str(), sizeof(oneResult.ApplicantName)-1);
			}
			if (arrayObj[i].isMember("ApplicantOrg") && arrayObj[i]["ApplicantOrg"].isString()) {
				strncpy(oneResult.ApplicantOrg, arrayObj[i]["ApplicantOrg"].asString().c_str(), sizeof(oneResult.ApplicantOrg)-1);
			}
			if (arrayObj[i].isMember("BeginTime") && arrayObj[i]["BeginTime"].isString()) {
				strncpy(oneResult.BeginTime, arrayObj[i]["BeginTime"].asString().c_str(), sizeof(oneResult.BeginTime)-1);
			}
			if (arrayObj[i].isMember("EndTime") && arrayObj[i]["EndTime"].isString()) {
				strncpy(oneResult.EndTime, arrayObj[i]["EndTime"].asString().c_str(), sizeof(oneResult.EndTime)-1);
			}
			if (arrayObj[i].isMember("ReceiveAddr") && arrayObj[i]["ReceiveAddr"].isString()) {
				strncpy(oneResult.ReceiveAddr, arrayObj[i]["ReceiveAddr"].asString().c_str(), sizeof(oneResult.ReceiveAddr)-1);
			}
			if (arrayObj[i].isMember("ReportInterval") && arrayObj[i]["ReportInterval"].isInt()) {
				oneResult.ReportInterval = arrayObj[i]["ReportInterval"].asInt();
			}
			if (arrayObj[i].isMember("OperateType") && arrayObj[i]["OperateType"].isInt()) {
				oneResult.OperateType = arrayObj[i]["OperateType"].asInt();
			}
			if (arrayObj[i].isMember("SubscribeStatus") && arrayObj[i]["SubscribeStatus"].isInt()) {
				oneResult.SubscribeStatus = arrayObj[i]["SubscribeStatus"].asInt();
			}
			if (arrayObj[i].isMember("Reason") && arrayObj[i]["Reason"].isString()) {
				strncpy(oneResult.Reason, arrayObj[i]["Reason"].asString().c_str(), sizeof(oneResult.Reason)-1);
			}
			if (arrayObj[i].isMember("SubscribeCancelOrg") && arrayObj[i]["SubscribeCancelOrg"].isString()) {
				strncpy(oneResult.SubscribeCancelOrg, arrayObj[i]["SubscribeCancelOrg"].asString().c_str(), sizeof(oneResult.SubscribeCancelOrg)-1);
			}
			if (arrayObj[i].isMember("SubscribeCancelPerson") && arrayObj[i]["SubscribeCancelPerson"].isString()) {
				strncpy(oneResult.SubscribeCancelPerson, arrayObj[i]["SubscribeCancelPerson"].asString().c_str(), sizeof(oneResult.SubscribeCancelPerson)-1);
			}
			if (arrayObj[i].isMember("CancelTime") && arrayObj[i]["CancelTime"].isString()) {
				strncpy(oneResult.CancelTime, arrayObj[i]["CancelTime"].asString().c_str(), sizeof(oneResult.CancelTime)-1);
			}
			if (arrayObj[i].isMember("CancelReason") && arrayObj[i]["CancelReason"].isString()) {
				strncpy(oneResult.CancelReason, arrayObj[i]["CancelReason"].asString().c_str(), sizeof(oneResult.CancelReason)-1);
			}

			subscribeList.push_back(oneResult);
		}
	}

	return 0;
}

int GAT1400Json::UnPackSubscribe(const std::string& strJson, GAT_1400_Subscribe& subscribe)
{
	Json::Reader reader;
	Json::Value json_object;
	if (!reader.parse(strJson, json_object)) {
		TVT_LOG_ERROR("UnPackSubscribe fail: cannot parse json");
		return -1;
	}
	if (!json_object.isObject()) {
		TVT_LOG_ERROR("UnPackSubscribe fail: json_object is null");
		return -1;
	}

	Json::Value SubscribeObject = json_object["SubscribeObject"];
	if (SubscribeObject.empty()) {
		TVT_LOG_ERROR("UnPackSubscribe fail: SubscribeObject is null");
		return -1;
	}
	
	if (SubscribeObject.isMember("SubscribeID") && SubscribeObject["SubscribeID"].isString()) {
		strncpy(subscribe.SubscribeID, SubscribeObject["SubscribeID"].asString().c_str(), sizeof(subscribe.SubscribeID)-1);
	}
	if (SubscribeObject.isMember("Title") && SubscribeObject["Title"].isString()) {
		strncpy(subscribe.Title, SubscribeObject["Title"].asString().c_str(), sizeof(subscribe.Title)-1);
	}
	if (SubscribeObject.isMember("SubscribeDetail") && SubscribeObject["SubscribeDetail"].isString()) {
		subscribe.SubscribeDetail = SubscribeObject["SubscribeDetail"].asString();
	}
	if (SubscribeObject.isMember("ResourceURI") && SubscribeObject["ResourceURI"].isString()) {
		strncpy(subscribe.ResourceURI, SubscribeObject["ResourceURI"].asString().c_str(), sizeof(subscribe.ResourceURI)-1);
	}
	if (SubscribeObject.isMember("ApplicantName") && SubscribeObject["ApplicantName"].isString()) {
		strncpy(subscribe.ApplicantName, SubscribeObject["ApplicantName"].asString().c_str(), sizeof(subscribe.ApplicantName)-1);
	}
	if (SubscribeObject.isMember("ApplicantOrg") && SubscribeObject["ApplicantOrg"].isString()) {
		strncpy(subscribe.ApplicantOrg, SubscribeObject["ApplicantOrg"].asString().c_str(), sizeof(subscribe.ApplicantOrg)-1);
	}
	if (SubscribeObject.isMember("BeginTime") && SubscribeObject["BeginTime"].isString()) {
		strncpy(subscribe.BeginTime, SubscribeObject["BeginTime"].asString().c_str(), sizeof(subscribe.BeginTime)-1);
	}
	if (SubscribeObject.isMember("EndTime") && SubscribeObject["EndTime"].isString()) {
		strncpy(subscribe.EndTime, SubscribeObject["EndTime"].asString().c_str(), sizeof(subscribe.EndTime)-1);
	}
	if (SubscribeObject.isMember("ReceiveAddr") && SubscribeObject["ReceiveAddr"].isString()) {
		strncpy(subscribe.ReceiveAddr, SubscribeObject["ReceiveAddr"].asString().c_str(), sizeof(subscribe.ReceiveAddr)-1);
	}
	if (SubscribeObject.isMember("ReportInterval") && SubscribeObject["ReportInterval"].isInt()) {
		subscribe.ReportInterval = SubscribeObject["ReportInterval"].asInt();
	}
	if (SubscribeObject.isMember("OperateType") && SubscribeObject["OperateType"].isInt()) {
		subscribe.OperateType = SubscribeObject["OperateType"].asInt();
	}
	if (SubscribeObject.isMember("SubscribeStatus") && SubscribeObject["SubscribeStatus"].isInt()) {
		subscribe.SubscribeStatus = SubscribeObject["SubscribeStatus"].asInt();
	}
	if (SubscribeObject.isMember("Reason") && SubscribeObject["Reason"].isString()) {
		strncpy(subscribe.Reason, SubscribeObject["Reason"].asString().c_str(), sizeof(subscribe.Reason)-1);
	}
	if (SubscribeObject.isMember("SubscribeCancelOrg") && SubscribeObject["SubscribeCancelOrg"].isString()) {
		strncpy(subscribe.SubscribeCancelOrg, SubscribeObject["SubscribeCancelOrg"].asString().c_str(), sizeof(subscribe.SubscribeCancelOrg)-1);
	}
	if (SubscribeObject.isMember("SubscribeCancelPerson") && SubscribeObject["SubscribeCancelPerson"].isString()) {
		strncpy(subscribe.SubscribeCancelPerson, SubscribeObject["SubscribeCancelPerson"].asString().c_str(), sizeof(subscribe.SubscribeCancelPerson)-1);
	}
	if (SubscribeObject.isMember("CancelTime") && SubscribeObject["CancelTime"].isString()) {
		strncpy(subscribe.CancelTime, SubscribeObject["CancelTime"].asString().c_str(), sizeof(subscribe.CancelTime)-1);
	}
	if (SubscribeObject.isMember("CancelReason") && SubscribeObject["CancelReason"].isString()) {
		strncpy(subscribe.CancelReason, SubscribeObject["CancelReason"].asString().c_str(), sizeof(subscribe.CancelReason)-1);
	}
	return 0;
}

std::string GAT1400Json::PackCaseListJson(const std::list<GAT_1400_CaseSet>& CaseList)
{
	Json::Value root;
	Json::Value CaseObjectList;
	for (std::list<GAT_1400_CaseSet>::const_iterator iter = CaseList.begin(); iter != CaseList.end(); ++iter) {
		Json::Value fileObject = _PackCase(*iter);
		CaseObjectList["Case"].append(fileObject);
	}
	root["CaseListObject"] = CaseObjectList;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackCaseJson(const GAT_1400_CaseSet& Case)
{
	Json::Value root;
	Json::Value CaseObject = _PackCase(Case);
	root["Case"] = CaseObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}

std::string GAT1400Json::PackCaseInfoJson(const GAT_1400_Case& CaseInfo)
{
	Json::Value root;
	Json::Value CaseInfoObject = _PackCaseInfo(CaseInfo);
	root["CaseInfoObject"] = CaseInfoObject;
	Json::FastWriter fast_writer;
	return fast_writer.write(root);
}


///////////////////////
//1400IN接口
///////////////////////
int GAT1400Json::UnPackRegisterJson(const std::string& strJson, std::string& ID)
{
    Json::Reader reader;
    Json::Value json_object;
    if (!reader.parse(strJson, json_object)) {
		TVT_LOG_ERROR("UnPackRegisterJson fail: cannot parse json");
        return -1;
    }
	if (!json_object.isObject()) {
		TVT_LOG_ERROR("UnPackRegisterJson fail: json_object is null");
		return -1;
	}

    Json::Value register_object = json_object["RegisterObject"];
    if (register_object.empty()) {
		TVT_LOG_ERROR("UnPackRegisterJson fail: RegisterObject is null");
        return -1;
    }

    if (register_object.isMember("DeviceID") && register_object["DeviceID"].isString()) {
        ID = register_object["DeviceID"].asString();
    }
    return 0;
}

int GAT1400Json::UnPackKeepLiveJson(const std::string& strJson, std::string& ID)
{
    Json::Reader reader;
    Json::Value json_object;
    if (!reader.parse(strJson, json_object)) {
		TVT_LOG_ERROR("UnPackKeepLiveJson fail: cannot parse json");
        return -1;
    }
	if (!json_object.isObject()) {
		TVT_LOG_ERROR("UnPackKeepLiveJson fail: json_object is null");
		return -1;
	}

    Json::Value keepalive_object = json_object["KeepaliveObject"];
    if (keepalive_object.empty()) {
		TVT_LOG_ERROR("UnPackKeepLiveJson fail: KeepaliveObject is null");
        return -1;
    }

    if (keepalive_object.isMember("DeviceID") && keepalive_object["DeviceID"].isString()) {
        ID = keepalive_object["DeviceID"].asString();
    }
    return 0;
}

int GAT1400Json::UnPackUnRegisterJson(const std::string& strJson, std::string& ID)
{
    Json::Reader reader;
    Json::Value json_object;
    if (!reader.parse(strJson, json_object)) {
		TVT_LOG_ERROR("UnPackUnRegisterJson fail: cannot parse json");
        return -1;
    }
	if (!json_object.isObject()) {
		TVT_LOG_ERROR("UnPackUnRegisterJson fail: json_object is null");
		return -1;
	}

    Json::Value register_object = json_object["UnRegisterObject"];
    if (register_object.empty()) {
		TVT_LOG_ERROR("UnPackUnRegisterJson fail: UnRegisterObject is null");
        return -1;
    }

    if (register_object.isMember("DeviceID") && register_object["DeviceID"].isString()) {
        ID = register_object["DeviceID"].asString();
    }
    return 0;
}
int GAT1400Json::UnPackSubImageListJson(const std::string& strJson, std::list<GAT_1400_SubImage>& subImageList)
{

    Json::Reader reader;
    Json::Value json_object;
    if (!reader.parse(strJson, json_object)) {
		TVT_LOG_ERROR("UnPackSubImageListJson fail: cannot parse json");
        return -1;
    }
	if (!json_object.isObject()) {
		TVT_LOG_ERROR("UnPackSubImageListJson fail: json_object is null");
		return -1;
	}

    Json::Value sub_ImageList_object = json_object["SubImageList"];
    if (sub_ImageList_object.empty()) {
		TVT_LOG_ERROR("UnPackSubImageListJson fail: SubImageList is null");
        return -1;
    }

    if (sub_ImageList_object.isMember("SubImageInfoObject") && sub_ImageList_object["SubImageInfoObject"].isArray()) {
        Json::Value ImageValue = sub_ImageList_object["SubImageInfoObject"];
        for (unsigned int i = 0; i < ImageValue.size(); ++i) {
            GAT_1400_SubImage image;
            //图像标识 必需字段
            Json::Value imageID = ImageValue[i]["ImageID"];
            if(!imageID.isNull() && imageID.isString()) {
                strncpy(image.ImageID, imageID.asString().c_str(), sizeof(image.ImageID));
            } else {
				TVT_LOG_ERROR("UnPackSubImageListJson fail: ImageID is null");
                return -1;
            }

            //事件分类 设备采集必选
            Json::Value eventSort = ImageValue[i]["EventSort"];
            if(!eventSort.isNull() && eventSort.isInt()) {
                image.EventSort = eventSort.asInt();
            } else {
				TVT_LOG_ERROR("UnPackSubImageListJson fail: EventSort is null");
                return -1;
            }

            //设备编码 
            Json::Value deviceID = ImageValue[i]["DeviceID"];
            if(!deviceID.isNull() && deviceID.isString()) {
				strncpy(image.DeviceID, deviceID.asString().c_str(), sizeof(image.DeviceID));
            }

            //存储路径
            Json::Value storagePath = ImageValue[i]["StoragePath"];
            if(!storagePath.isNull() && storagePath.isString()) {
				strncpy(image.StoragePath, storagePath.asString().c_str(), sizeof(image.StoragePath));
            }

            //图像类型
            Json::Value type = ImageValue[i]["Type"];
            if(!type.isNull() && type.isString()) {
                image.Type = (ImageType)std::stoi(type.asString());
            } else {
				TVT_LOG_ERROR("UnPackSubImageListJson fail: Type is null");
                return -1;
            }

            //图像文件格式 必需字段
            Json::Value fileFormat = ImageValue[i]["FileFormat"];
            if(!fileFormat.isNull() && fileFormat.isString()) {
                strncpy(image.FileFormat, fileFormat.asString().c_str(), sizeof(image.FileFormat));
            } else {
				TVT_LOG_ERROR("UnPackSubImageListJson fail: FileFormat is null");
                return -1;
            }

            //拍摄时间 必需字段
            Json::Value shotTime = ImageValue[i]["ShotTime"];
            if(!shotTime.isNull() && shotTime.isString()) {
                strncpy(image.ShotTime, shotTime.asString().c_str(), sizeof(image.ShotTime));
            } else {
				TVT_LOG_ERROR("UnPackSubImageListJson fail: ShotTime is null");
                return -1;
            }

            //水平像素值 必需字段
            Json::Value width = ImageValue[i]["Width"];
            if(!width.isNull() && width.isInt()) {
                image.Width = width.asInt();
            } else {
				TVT_LOG_ERROR("UnPackSubImageListJson fail: Width is null");
                return -1;
            }

            //垂直像素值 必需字段
            Json::Value height = ImageValue[i]["Height"];
            if(!height.isNull() && height.isInt()) {
                image.Height = height.asInt();
            } else {
				TVT_LOG_ERROR("UnPackSubImageListJson fail: Height is null");
                return -1;
            }

            //图像数据
            Json::Value data = ImageValue[i]["Data"];
            if(!data.isNull() && data.isString()) {
                image.Data = data.asString();
            }
   
            subImageList.push_back(image);
        }
    }
    return 0;
}
int GAT1400Json::UnPackFaceListJson(const std::string& strJson, std::list<GAT_1400_Face>& FaceList)
{
    Json::Reader reader;
    Json::Value dataValue;
    if (!reader.parse(strJson,dataValue)) {
		TVT_LOG_ERROR("UnPackFaceListJson fail: cannot parse json");
        return -1;
    }
	if (!dataValue.isObject()) {
		TVT_LOG_ERROR("UnPackFaceListJson fail: json_object is null");
		return -1;
	}

    Json::Value FaceValue;
    int isize = 0;
    int bhaveList = -1;
    if (dataValue["FaceListObject"].isNull()) {
		TVT_LOG_ERROR("UnPackFaceListJson fail: FaceListObject is null");
        return -1;
    } else {
        bhaveList = 0;
        Json::Value datavalue = dataValue["FaceListObject"];
        FaceValue = datavalue["FaceObject"];
        isize = FaceValue.size();
    }

    for(int i = 0; i < isize; i++)
    {
        GAT_1400_Face iFace;
        //人脸id 必需字段
        Json::Value FaceId = FaceValue[i]["FaceID"];
        if( !FaceId.isNull() && FaceId.isString()) {
			strncpy(iFace.FaceID, FaceId.asString().c_str(), sizeof(iFace.FaceID));
        } else {
			TVT_LOG_ERROR("UnPackFaceListJson fail: FaceID is null");
			return -1;
		}
    
        // 信息分类（手动、自动） 必需字段
        Json::Value InfoKind = FaceValue[i]["InfoKind"];
        if(!InfoKind.isNull() && InfoKind.isInt()) {
            iFace.InfoKind = InfoKind.asInt();
        } else {
			TVT_LOG_ERROR("UnPackFaceListJson fail: InfoKind is null");
			return -1;
		}

        //来源标识 必需字段
        Json::Value SourcId = FaceValue[i]["SourceID"];
        if(!SourcId.isNull() && SourcId.isString()) {
			strncpy(iFace.SourceID, SourcId.asString().c_str(), sizeof(iFace.SourceID));
        } else {
			TVT_LOG_ERROR("UnPackFaceListJson fail: SourceID is null");
			return -1;
		}
        
        //设备编码 自动采集必需字段
        Json::Value DeviceId = FaceValue[i]["DeviceID"];
		if (iFace.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (DeviceId.isNull() || !DeviceId.isString()) {
				TVT_LOG_ERROR("UnPackFaceListJson fail: DeviceID is null");
				return -1;
			}
			strncpy(iFace.DeviceID, DeviceId.asString().c_str(), sizeof(iFace.DeviceID));
		} else {
			if (!DeviceId.isNull() && DeviceId.isString()) {
				strncpy(iFace.DeviceID, DeviceId.asString().c_str(), sizeof(iFace.DeviceID));
			}
		}

        //人脸区域左上X坐标 自动采集必需字段
        Json::Value leftTopx = FaceValue[i]["LeftTopX"];
		if (iFace.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (leftTopx.isNull() || !leftTopx.isInt())	{
				TVT_LOG_ERROR("UnPackFaceListJson fail: LeftTopX is null");
				return -1;
			}
			iFace.LeftTopX = leftTopx.asInt();
		} else {
			if (!leftTopx.isNull() && leftTopx.isInt())	{
				iFace.LeftTopX = leftTopx.asInt();
			}
		}

        //人脸区域左上Y坐标 自动采集必需字段
        Json::Value leftTopy = FaceValue[i]["LeftTopY"];
		if (iFace.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (leftTopy.isNull() || !leftTopy.isInt())	{
				TVT_LOG_ERROR("UnPackFaceListJson fail: LeftTopY is null");
				return -1;
			}
			iFace.LeftTopY = leftTopy.asInt();
		} else {
			if (!leftTopy.isNull() && leftTopy.isInt())	{
				iFace.LeftTopY = leftTopy.asInt();
			}
		}

        
        //人脸区域右下角X坐标 自动采集必需字段
        Json::Value rightBtmx = FaceValue[i]["RightBtmX"];
		if (iFace.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (rightBtmx.isNull() || !rightBtmx.isInt()) {
				TVT_LOG_ERROR("UnPackFaceListJson fail: RightBtmX is null");
				return -1;
			}
			iFace.RightBtmX = rightBtmx.asInt();
		} else {
			if (!rightBtmx.isNull() && rightBtmx.isInt()) {
				iFace.RightBtmX = rightBtmx.asInt();
			}
		}

        //人脸区域右下角Y坐标 自动采集必需字段
        Json::Value rightBtmY = FaceValue[i]["RightBtmY"];
		if (iFace.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (rightBtmY.isNull() || !rightBtmY.isInt()) {
				TVT_LOG_ERROR("UnPackFaceListJson fail: RightBtmY is null");
				return -1;
			}
			iFace.RightBtmY = rightBtmY.asInt();
		} else {
			if (!rightBtmY.isNull() && rightBtmY.isInt()) {
				iFace.RightBtmY = rightBtmY.asInt();
			}
		}

        //位置标记时间 人脸采集有效
        if(iFace.InfoKind != INFO_TYPE_AUTOMATIC) {
            Json::Value locationMarkTime = FaceValue[i]["LocationMarkTime"];
            if(!locationMarkTime.isNull() && locationMarkTime.isString()) {
				strncpy(iFace.LocationMarkTime, locationMarkTime.asString().c_str(), sizeof(iFace.LocationMarkTime));
            }
        }

        //人脸出现时间 人脸采集有效
        if(iFace.InfoKind != INFO_TYPE_AUTOMATIC) {
            Json::Value faceAppearTime = FaceValue[i]["FaceAppearTime"];
            if(!faceAppearTime.isNull() && faceAppearTime.isString()) {
				strncpy(iFace.FaceAppearTime, faceAppearTime.asString().c_str(), sizeof(iFace.FaceAppearTime));
            }
        }

        //人脸消失时间 人脸采集有效
        if(iFace.InfoKind !=INFO_TYPE_AUTOMATIC) {
            Json::Value faceDisAppearTime = FaceValue[i]["FaceDisAppearTime"];
            if(!faceDisAppearTime.isNull() && faceDisAppearTime.isString()) {
				strncpy(iFace.FaceDisAppearTime, faceDisAppearTime.asString().c_str(), sizeof(iFace.FaceDisAppearTime));
            }
        }

        //证件类型
        Json::Value idType = FaceValue[i]["IDType"];
        if(!idType.isNull() && idType.isString()) {
			sscanf(idType.asString().c_str(), "%d", &iFace.IDType);
        }

        //有效证件号码
        Json::Value idNumber = FaceValue[i]["IDNumber"];
        if(!idNumber.isNull() && idNumber.isString()) {
			strncpy(iFace.IDNumber, idNumber.asString().c_str(), sizeof(iFace.IDNumber));
        }

        //姓名
        Json::Value name = FaceValue[i]["Name"];
        if(!name.isNull() && name.isString()) {
			strncpy(iFace.Name, name.asString().c_str(), sizeof(iFace.Name));
        }

        //曾用名
        Json::Value usedName = FaceValue[i]["UsedName"];
        if(!usedName.isNull() && usedName.isString()) {
			strncpy(iFace.UsedName, usedName.asString().c_str(), sizeof(iFace.UsedName));
        }

        //绰号
        Json::Value alias = FaceValue[i]["Alias"];
        if(!alias.isNull() && alias.isString()) {
			strncpy(iFace.Alias, alias.asString().c_str(), sizeof(iFace.Alias));
        }

        //性别
        Json::Value genderCode = FaceValue[i]["GenderCode"];
		if (!genderCode.isNull() && genderCode.isString()) {
			sscanf(genderCode.asString().c_str(), "%d", &iFace.GenderCode);
		}
        //最大可能年龄
        Json::Value ageUpLimit = FaceValue[i]["AgeUpLimit"];
        if(!ageUpLimit.isNull() && ageUpLimit.isInt()) {
			iFace.AgeUpLimit = ageUpLimit.asInt();
        }

        //最小可能年龄
        Json::Value ageLowerLimit = FaceValue[i]["AgeLowerLimit"];
        if(!ageLowerLimit.isNull() && ageLowerLimit.isInt()) {
			iFace.AgeLowerLimit = ageLowerLimit.asInt();
        }

        //中国各民族的罗马字母拼写法和代码
        Json::Value fthicCode = FaceValue[i]["EthicCode"];
        if(!fthicCode.isNull() && fthicCode.isString()) {
			strncpy(iFace.EthicCode, fthicCode.asString().c_str(), sizeof(iFace.EthicCode));
        }

        //国籍代码、世界各国和地区名称代码
        Json::Value nationalityCode = FaceValue[i]["NationalityCode"];
        if(!nationalityCode.isNull() && nationalityCode.isString()) {
			strncpy(iFace.NationalityCode, nationalityCode.asString().c_str(), sizeof(iFace.NationalityCode));
        }

        //籍贯省市县代码
        Json::Value nativeCityCode = FaceValue[i]["NativeCityCode"];
        if(!nativeCityCode.isNull() && nativeCityCode.isString()) {
			strncpy(iFace.NativeCityCode, nativeCityCode.asString().c_str(), sizeof(iFace.NativeCityCode));
        }

        //居住地行政区划
        Json::Value residenceAdminDivision = FaceValue[i]["ResidenceAdminDivision"];
        if(!residenceAdminDivision.isNull() && residenceAdminDivision.isString()) {
			strncpy(iFace.ResidenceAdminDivision, residenceAdminDivision.asString().c_str(), sizeof(iFace.ResidenceAdminDivision));
        }

        //汉语口音
        Json::Value chineseAccentCode = FaceValue[i]["ChineseAccentCode"];
        if(!chineseAccentCode.isNull() && chineseAccentCode.isString()) {
			strncpy(iFace.ChineseAccentCode, chineseAccentCode.asString().c_str(), sizeof(iFace.ChineseAccentCode));
        }

        //职业
        Json::Value jobCategory = FaceValue[i]["JobCategory"];
        if(!jobCategory.isNull() && jobCategory.isString()) {
			strncpy(iFace.JobCategory, jobCategory.asString().c_str(), sizeof(iFace.JobCategory));
        }

        //同行人脸数
        Json::Value accompanyNumber = FaceValue[i]["AccompanyNumber"];
        if(!accompanyNumber.isNull() && accompanyNumber.isInt()) {
			iFace.AccompanyNumber = accompanyNumber.asInt();
        }

        //肤色
        Json::Value skinColor = FaceValue[i]["SkinColor"];
        if(!skinColor.isNull() && skinColor.isString()) {
			iFace.SkinColor = skinColor.asString();
        }

        //发型
        Json::Value hairStyle = FaceValue[i]["HairStyle"];
        if(!hairStyle.isNull() && hairStyle.isString()) {
			sscanf(hairStyle.asString().c_str(), "%d", &iFace.HairStyle);
        }

        //发色
        Json::Value hairColor = FaceValue[i]["HairColor"];
        if(!hairColor.isNull() && hairColor.isString()) {
			sscanf(hairColor.asString().c_str(), "%d", &iFace.HairColor);
        }

        //脸型
        Json::Value faceStyle = FaceValue[i]["FaceStyle"];
        if(!faceStyle.isNull() && faceStyle.isString()) {
			strncpy(iFace.FaceStyle, faceStyle.asString().c_str(), sizeof(iFace.FaceStyle));
        }

        //脸部特征
        Json::Value facialFeature = FaceValue[i]["FacialFeature"];
        if(!facialFeature.isNull() && facialFeature.isString()) {
			strncpy(iFace.FacialFeature, facialFeature.asString().c_str(), sizeof(iFace.FacialFeature));
        }

        //体貌特征
        Json::Value physicalFeature = FaceValue[i]["PhysicalFeature"];
        if(!physicalFeature.isNull() && physicalFeature.isString()) {
			strncpy(iFace.PhysicalFeature, physicalFeature.asString().c_str(), sizeof(iFace.PhysicalFeature));
        }

        //口罩颜色
        Json::Value respiratorColor = FaceValue[i]["RespiratorColor"];
        if(!respiratorColor.isNull() && respiratorColor.isString()) {
			sscanf(respiratorColor.asString().c_str(), "%d", &iFace.RespiratorColor);
        }

        //帽子款式
        Json::Value capStyle = FaceValue[i]["CapStyle"];
        if(!capStyle.isNull() && capStyle.isString()) {
			sscanf(capStyle.asString().c_str(), "%d", &iFace.CapStyle);
        }

        //帽子颜色
        Json::Value capColor = FaceValue[i]["CapColor"];
        if(!capColor.isNull() && capColor.isString()) {
			sscanf(capColor.asString().c_str(), "%d", &iFace.CapColor);
        }

        //眼睛款式
        Json::Value glassStyle = FaceValue[i]["GlassStyle"];
        if(!glassStyle.isNull() && glassStyle.isString()) {
			sscanf(glassStyle.asString().c_str(), "%d", &iFace.GlassStyle);
        }

        //眼镜颜色
        Json::Value glassColor = FaceValue[i]["GlassColor"];
        if(!glassColor.isNull() && glassColor.isString()) {
			sscanf(glassColor.asString().c_str(), "%d", &iFace.GlassColor);
        }

        //是否驾驶员
        Json::Value isDriver = FaceValue[i]["IsDriver"];
        if(!isDriver.isNull() && isDriver.isInt()) {
			iFace.IsDriver = isDriver.asInt();
        }

        //是否涉外人员
        Json::Value isForeigner = FaceValue[i]["IsForeigner"];
        if(!isForeigner.isNull() && isForeigner.isInt()) {
			iFace.IsForeigner = isForeigner.asInt();
        }

        //护照证件类型
        Json::Value passportType = FaceValue[i]["PassportType"];
        if(!passportType.isNull() && passportType.isString()) {
			sscanf(passportType.asString().c_str(), "%d", &iFace.PassportType);
        }

        //出入境人员类型代码
        Json::Value immigrantTypeCode = FaceValue[i]["ImmigrantTypeCode"];
        if(!immigrantTypeCode.isNull() && immigrantTypeCode.isString()) {
			strncpy(iFace.ImmigrantTypeCode, immigrantTypeCode.asString().c_str(), sizeof(iFace.ImmigrantTypeCode));
        }

        //是否涉恐人员
        Json::Value isSuspectedTerrorist = FaceValue[i]["IsSuspectedTerrorist"];
        if(!isSuspectedTerrorist.isNull() && isSuspectedTerrorist.isInt()) {
            iFace.IsSuspectedTerrorist = isSuspectedTerrorist.asInt();
        }

        //涉恐人员编号
        Json::Value suspectedTreeoristNumber = FaceValue[i]["SuspectedTreeoristNumber"];
        if(!suspectedTreeoristNumber.isNull() && suspectedTreeoristNumber.isString()) {
			strncpy(iFace.SuspectedTreeoristNumber, suspectedTreeoristNumber.asString().c_str(), sizeof(iFace.SuspectedTreeoristNumber));
        }

        //是否涉案人员
        Json::Value isCriminalInvolved = FaceValue[i]["IsCriminalInvolved"];
        if(!isCriminalInvolved.isNull() && isCriminalInvolved.isInt()) {
            iFace.IsCriminalInvolved = isCriminalInvolved.asInt();
        }

        //涉案人员专长代码
        Json::Value criminalInvolvedSpecilisationCode = FaceValue[i]["CriminalInvolvedSpecilisationCode"];
        if(!criminalInvolvedSpecilisationCode.isNull() && criminalInvolvedSpecilisationCode.isString()) {
			strncpy(iFace.CriminalInvolvedSpecilisationCode, criminalInvolvedSpecilisationCode.asString().c_str(), sizeof(iFace.CriminalInvolvedSpecilisationCode));
        }

        //体表特殊标记
        Json::Value bodySpeciallMark = FaceValue[i]["BodySpeciallMark"];
        if(!bodySpeciallMark.isNull() && bodySpeciallMark.isString()) {
			strncpy(iFace.BodySpeciallMark, bodySpeciallMark.asString().c_str(), sizeof(iFace.BodySpeciallMark));
        }

        //作案手段
        Json::Value crimeMethod = FaceValue[i]["CrimeMethod"];
        if(!crimeMethod.isNull() && crimeMethod.isString()) {
			strncpy(iFace.CrimeMethod, crimeMethod.asString().c_str(), sizeof(iFace.CrimeMethod));
        }

        //作案特点代码
        Json::Value crimeCharacterCode = FaceValue[i]["CrimeCharacterCode"];
        if(!crimeCharacterCode.isNull() && crimeCharacterCode.isString()) {
			strncpy(iFace.CrimeCharacterCode, crimeCharacterCode.asString().c_str(), sizeof(iFace.CrimeCharacterCode));
        }

        //在逃人员信息编号规则
        Json::Value escapedCriminalNumber = FaceValue[i]["EscapedCriminalNumber"];
        if(!escapedCriminalNumber.isNull() && escapedCriminalNumber.isString()) {
			strncpy(iFace.EscapedCriminalNumber, escapedCriminalNumber.asString().c_str(), sizeof(iFace.EscapedCriminalNumber));
        }

        //是否在押人员
        Json::Value isDetainees = FaceValue[i]["IsDetainees"];
        if(!isDetainees.isNull() && isDetainees.isInt()) {
            iFace.IsDetainees = isDetainees.asInt();
        }

        //看守所编码
        Json::Value detentionHouseCode = FaceValue[i]["DetentionHouseCode"];
        if(!detentionHouseCode.isNull() && detentionHouseCode.isString()) {
			strncpy(iFace.DetentionHouseCode, detentionHouseCode.asString().c_str(), sizeof(iFace.DetentionHouseCode));
        }

        //在押人员身份
        Json::Value detainessIdentity = FaceValue[i]["DetainessIdentity"];
        if(!detainessIdentity.isNull() && detainessIdentity.isString()) {
			sscanf(detainessIdentity.asString().c_str(), "%d", &iFace.DetainessIdentity);
        }

        //在押人员特殊身份编码
        Json::Value detaineesSpecialIdentity = FaceValue[i]["DetaineesSpecialIdentity"];
        if(!detaineesSpecialIdentity.isNull() && detaineesSpecialIdentity.isString()) {
			strncpy(iFace.DetaineesSpecialIdentity, detaineesSpecialIdentity.asString().c_str(), sizeof(iFace.DetaineesSpecialIdentity));
        }

        //在押人员类型
        Json::Value memberTypeCode = FaceValue[i]["MemberTypeCode"];
        if(!memberTypeCode.isNull() && memberTypeCode.isString()) {
			strncpy(iFace.MemberTypeCode, memberTypeCode.asString().c_str(), sizeof(iFace.MemberTypeCode));
        }

        //是否被害人员
        Json::Value isVictim = FaceValue[i]["IsVictim"];
        if(!isVictim.isNull() && isVictim.isInt()) {
            iFace.IsVictim = isVictim.asInt();
        }

        //被害人类型
        Json::Value victimType = FaceValue[i]["VictimType"];
        if(!victimType.isNull() && victimType.isString()) {
			strncpy(iFace.VictimType, victimType.asString().c_str(), sizeof(iFace.VictimType));
        }

        //受伤害程度
        Json::Value injuredDegree = FaceValue[i]["InjuredDegree"];
        if(!injuredDegree.isNull() && injuredDegree.isString()) {
			iFace.InjuredDegree = injuredDegree.asString();
        }

        //尸体状态
        Json::Value corpseConditionCode = FaceValue[i]["CorpseConditionCode"];
        if(!corpseConditionCode.isNull() && corpseConditionCode.isString()) {
			strncpy(iFace.CorpseConditionCode, corpseConditionCode.asString().c_str(), sizeof(iFace.CorpseConditionCode));
        }

        //是否可疑人员
        Json::Value isSuspiciousPerson = FaceValue[i]["IsSuspiciousPerson"];
        if(!isSuspiciousPerson.isNull() && isSuspiciousPerson.isInt()) {
            iFace.IsSuspiciousPerson = isSuspiciousPerson.asInt();
        }

        //姿态分布
        Json::Value attitude = FaceValue[i]["Attitude"];
        if(!attitude.isNull() && attitude.isInt()) {
			iFace.Attitude = attitude.asInt();
        }

        //人脸相似度
        Json::Value similaritydegree = FaceValue[i]["Similaritydegree"];
        if(!memberTypeCode.isNull() && memberTypeCode.isDouble()) {
			iFace.Similaritydegree = memberTypeCode.asDouble();
        }

        //眉型
        Json::Value eyebrowStyle = FaceValue[i]["EyebrowStyle"];
        if(!eyebrowStyle.isNull() && eyebrowStyle.isString()) {
			iFace.EyebrowStyle = eyebrowStyle.asString();
        }

        //鼻型
        Json::Value noseStyle = FaceValue[i]["NoseStyle"];
        if(!noseStyle.isNull() && noseStyle.isString()) {
			iFace.NoseStyle = noseStyle.asString();
        }

        //胡型
        Json::Value mustacheStyle = FaceValue[i]["MustacheStyle"];
        if(!mustacheStyle.isNull() && mustacheStyle.isString()) {
			iFace.MustacheStyle = mustacheStyle.asString();
        }

        //嘴唇
        Json::Value lipStyle = FaceValue[i]["LipStyle"];
        if(!lipStyle.isNull() && lipStyle.isString()) {
			iFace.LipStyle = lipStyle.asString();
        }

        //皱纹眼袋
        Json::Value wriklePouch = FaceValue[i]["WriklePouch"];
        if(!wriklePouch.isNull() && wriklePouch.isString()) {
			iFace.WriklePouch = wriklePouch.asString();
        }

        //痤疮色斑
        Json::Value acneStain = FaceValue[i]["AcneStain"];
        if(!acneStain.isNull() && acneStain.isString()) {
			iFace.AcneStain = acneStain.asString();
        }

        //黑痣胎记
        Json::Value freckleBirthmark = FaceValue[i]["FreckleBirthmark"];
        if(!freckleBirthmark.isNull() && freckleBirthmark.isString()) {
			iFace.FreckleBirthmark = freckleBirthmark.asString();
        }

        //疤痕酒窝
        Json::Value scarDimple = FaceValue[i]["ScarDimple"];
        if(!scarDimple.isNull() && scarDimple.isString()) {
			iFace.ScarDimple = scarDimple.asString();
        }

        //其他特征
        Json::Value otherFeature = FaceValue[i]["OtherFeature"];
        if(!otherFeature.isNull() && otherFeature.isString()) {
			iFace.OtherFeature = otherFeature.asString();
        }

        ////图像数据
        Json::Value SubImageList = FaceValue[i]["SubImageList"];
        if(!SubImageList.isNull()) {
            std::string strjson = FaceValue[i].toStyledString();
            if(UnPackSubImageListJson(strjson, iFace.SubImageList) == -1) {
                return -1;
            }
        }
        FaceList.push_back(iFace);
    }
    return 0;
}

std::string GAT1400Json::PackResponseStatus(const GAT1400_RESPONSESTATUS_ST& result)
{
    Json::Value root;
    Json::Value ResponseStatusObject;
    ResponseStatusObject["RequestURL"] = Json::Value(result.ReqeustURL);
    ResponseStatusObject["StatusCode"] = Json::Value(result.StatusCode);
    ResponseStatusObject["StatusString"] = Json::Value(result.StatusString);
    ResponseStatusObject["Id"] = Json::Value(result.ID);
    ResponseStatusObject["LocalTime"] = Json::Value(result.LocalTime);
    root["ResponseStatusObject"] = ResponseStatusObject;

    Json::FastWriter fast_writer;
    return fast_writer.write(root);
}

std::string GAT1400Json::PackSystemTime(const GAT1400_SYSTEMTIME_ST& result)
{
    Json::Value root;
    Json::Value ResponseStatusObject;
    ResponseStatusObject["LocalTime"] = Json::Value(result.LocalTime);
    ResponseStatusObject["TimeZone"] = Json::Value(result.TimeZone);
    ResponseStatusObject["TimeMode"] = Json::Value(result.TimeMode);
    ResponseStatusObject["VIIDServerID"] = Json::Value(result.VIIDServerID);
    root["SystemTimeObject"] = ResponseStatusObject;

    Json::FastWriter fast_writer;
    return fast_writer.write(root);
}

int GAT1400Json::UnPackMotorListJson(const std::string& strJson, std::list<GAT_1400_Motor>& MotorList)
{
    Json::Reader reader;
    Json::Value dataValue;
    if (!reader.parse(strJson,dataValue)) {
		TVT_LOG_ERROR("UnPackMotorListJson fail: cannot parse json");
        return -1;
    }

	if (!dataValue.isObject()) {
		TVT_LOG_ERROR("UnPackMotorListJson fail: json_object is null");
		return -1;
	}

    Json::Value MotorValue;
    int isize = 0;
    int bhaveList = -1;
    if (dataValue["MotorVehicleListObject"].isNull()) {
		TVT_LOG_ERROR("UnPackMotorListJson fail: MotorVehicleListObject is null");
        return -1;
    } else {
        bhaveList = 0;
        Json::Value datavalue = dataValue["MotorVehicleListObject"];
        MotorValue = datavalue["MotorVehicleObject"];
        isize = MotorValue.size();
    }
    for(int i = 0; i < isize; i++)
    {
        GAT_1400_Motor iMotor;
        //人脸id 必需字段
        Json::Value motorVehicleID = MotorValue[i]["MotorVehicleID"];
        if(!motorVehicleID.isNull() && motorVehicleID.isString()) {
			strncpy(iMotor.MotorVehicleID, motorVehicleID.asString().c_str(), sizeof(iMotor.MotorVehicleID));
        } else {
			TVT_LOG_ERROR("UnPackMotorListJson fail: MotorVehicleID is null");
            return -1;
        }

        //信息分类 必需字段
        Json::Value infoKind = MotorValue[i]["InfoKind"];
        if(!infoKind.isNull() && infoKind.isInt()) {
            iMotor.InfoKind = infoKind.asInt();
        } else {
			TVT_LOG_ERROR("UnPackMotorListJson fail: InfoKind is null");
            return -1;
        }

        //来源标识
        Json::Value sourceID = MotorValue[i]["SourceID"];
        if(!sourceID.isNull() && sourceID.isString()) {
			strncpy(iMotor.SourceID, sourceID.asString().c_str(), sizeof(iMotor.SourceID));
        } else {
			TVT_LOG_ERROR("UnPackMotorListJson fail: SourceID is null");
            return -1;
        }

        //关联卡口编码
        Json::Value tollgateID = MotorValue[i]["TollgateID"];
        if(!tollgateID.isNull() && tollgateID.isString()) {
			strncpy(iMotor.TollgateID, tollgateID.asString().c_str(), sizeof(iMotor.TollgateID));
        }

        //设备编码 |采集设备ID 自动采集必选
        Json::Value deviceID = MotorValue[i]["DeviceID"];
		if(iMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (deviceID.isNull() || !deviceID.isString()) {
				TVT_LOG_ERROR("UnPackMotorListJson fail: DeviceID is null");
				return -1;
			}
			strncpy(iMotor.DeviceID, deviceID.asString().c_str(), sizeof(iMotor.DeviceID));
		} else {
			if (!deviceID.isNull() && deviceID.isString()) {
				strncpy(iMotor.DeviceID, deviceID.asString().c_str(), sizeof(iMotor.DeviceID));
			}
		}

        //近景照片 自动采集必选
        Json::Value storageUrl1 = MotorValue[i]["StorageUrl1"];
		if(iMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (storageUrl1.isNull() || !storageUrl1.isString()) {
				TVT_LOG_ERROR("UnPackMotorListJson fail: StorageUrl1 is null");
				return -1;
			}
			strncpy(iMotor.StorageUrl1, storageUrl1.asString().c_str(), sizeof(iMotor.StorageUrl1));
		} else {
			if(!storageUrl1.isNull() && storageUrl1.isString()) {
				strncpy(iMotor.StorageUrl1, storageUrl1.asString().c_str(), sizeof(iMotor.StorageUrl1));
			}
		}

        //车牌照片
        Json::Value storageUrl2 = MotorValue[i]["StorageUrl2"];
        if(!storageUrl2.isNull() && storageUrl2.isString()) {
			strncpy(iMotor.StorageUrl2, storageUrl2.asString().c_str(), sizeof(iMotor.StorageUrl2));
        }

        //远景照片
        Json::Value storageUrl3 = MotorValue[i]["StorageUrl3"];
        if(!storageUrl3.isNull() && storageUrl3.isString()) {
			strncpy(iMotor.StorageUrl3, storageUrl3.asString().c_str(), sizeof(iMotor.StorageUrl3));
        }

        //合成图
        Json::Value storageUrl4 = MotorValue[i]["StorageUrl4"];
        if(!storageUrl4.isNull() && storageUrl4.isString()) {
            strncpy(iMotor.StorageUrl4, storageUrl4.asString().c_str(), sizeof(iMotor.StorageUrl4));
        }

        //缩略图
        Json::Value storageUrl5 = MotorValue[i]["StorageUrl5"];
        if(!storageUrl5.isNull() && storageUrl5.isString()) {
            strncpy(iMotor.StorageUrl5, storageUrl5.asString().c_str(), sizeof(iMotor.StorageUrl5));
        }

        //左上角X坐标 | 自动采集必选
        Json::Value leftTopX = MotorValue[i]["LeftTopX"];
		if(iMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (leftTopX.isNull() || !leftTopX.isInt())	{
				TVT_LOG_ERROR("UnPackMotorListJson fail: LeftTopX is null");
				return -1;
			}
			iMotor.LeftTopX = leftTopX.asInt();
		} else {
			if (!leftTopX.isNull() && leftTopX.isInt())	{
				iMotor.LeftTopX = leftTopX.asInt();
			}
		}

        //左上角Y坐标 | 自动采集必选
        Json::Value leftTopY = MotorValue[i]["LeftTopY"];
		if(iMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (leftTopY.isNull() || !leftTopY.isInt())	{
				TVT_LOG_ERROR("UnPackMotorListJson fail: LeftTopY is null");
				return -1;
			}
			iMotor.LeftTopY = leftTopY.asInt();
		} else {
			if (!leftTopY.isNull() && leftTopY.isInt())	{
				iMotor.LeftTopY = leftTopY.asInt();
			}
		}

        //右下角X坐标 | 自动采集必选
        Json::Value rightBtmX = MotorValue[i]["RightBtmX"];
		if(iMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (rightBtmX.isNull() || !rightBtmX.isInt()) {
				TVT_LOG_ERROR("UnPackMotorListJson fail: RightBtmX is null");
				return -1;
			}
			iMotor.RightBtmX = rightBtmX.asInt();
		} else {
			if (!rightBtmX.isNull() && rightBtmX.isInt()) {
				iMotor.RightBtmX = rightBtmX.asInt();
			}
		}

        //右下角Y坐标 | 自动采集必选
        Json::Value rightBtmY = MotorValue[i]["RightBtmY"];
		if(iMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (rightBtmY.isNull() || !rightBtmY.isInt()) {
				TVT_LOG_ERROR("UnPackMotorListJson fail: RightBtmY is null");
				return -1;
			}
			iMotor.RightBtmY = rightBtmY.asInt();
		} else {
			if (!rightBtmY.isNull() && rightBtmY.isInt()) {
				iMotor.RightBtmY = rightBtmY.asInt();
			}
		}

        //位置标记时间
        Json::Value markTime = MotorValue[i]["MarkTime"];
        if(!markTime.isNull() && markTime.isString()) {
			strncpy(iMotor.MarkTime, markTime.asString().c_str(), sizeof(iMotor.MarkTime));
        }

        //车辆出现时间
        Json::Value appearTime = MotorValue[i]["AppearTime"];
        if(!appearTime.isNull() && appearTime.isString()) {
			strncpy(iMotor.AppearTime, appearTime.asString().c_str(), sizeof(iMotor.AppearTime));
        }

        //车辆消失时间
        Json::Value disAppearTime = MotorValue[i]["DisAppearTime"];
        if(!disAppearTime.isNull() && disAppearTime.isString()) {
			strncpy(iMotor.DisappearTime, disAppearTime.asString().c_str(), sizeof(iMotor.DisappearTime));
        }

        //车道号
        Json::Value laneNo = MotorValue[i]["LaneNo"];
        if(!laneNo.isNull() && laneNo.isInt()) {
			iMotor.LaneNo = laneNo.asInt();
        }

        //有无车牌
        Json::Value hasPlate = MotorValue[i]["HasPlate"];
        if(!hasPlate.isNull() && hasPlate.isBool()) {
			iMotor.HasPlate = hasPlate.asBool();
        }

        //车牌种类 
        Json::Value plateClass = MotorValue[i]["PlateClass"];
        if(!plateClass.isNull() && plateClass.isString()) {
			sscanf(plateClass.asString().c_str(), "%d", &iMotor.PlateClass);
        }

        //车牌颜色
        Json::Value plateColor = MotorValue[i]["PlateColor"];
        if(!plateColor.isNull() && plateColor.isString()) {
			sscanf(plateColor.asString().c_str(), "%d", &iMotor.PlateColor);
        }

        //车牌号
        Json::Value plateNo = MotorValue[i]["PlateNo"];
        if(!plateNo.isNull() && plateNo.isString()) {
			strncpy(iMotor.PlateNo, plateNo.asString().c_str(), sizeof(iMotor.PlateNo));
        }

        //挂车牌号 
        Json::Value plateNoAttach = MotorValue[i]["PlateNoAttach"];
        if(!plateNoAttach.isNull() && plateNoAttach.isString()) {
			strncpy(iMotor.PlateNoAttach, plateNoAttach.asString().c_str(), sizeof(iMotor.PlateNoAttach));
        }

        //车牌描述
        Json::Value plateDescribe = MotorValue[i]["PlateDescribe"];
        if(!plateDescribe.isNull() && plateDescribe.isString()) {
			strncpy(iMotor.PlateDescribe, plateDescribe.asString().c_str(), sizeof(iMotor.PlateDescribe));
        }

        //是否套牌
        Json::Value isDecked = MotorValue[i]["IsDecked"];
        if(!isDecked.isNull() && isDecked.isBool()) {
			iMotor.IsDecked = isDecked.asBool();
        }

        //是否涂改
        Json::Value isAltered = MotorValue[i]["IsAltered"];
        if(!isAltered.isNull() && isAltered.isBool()) {
			iMotor.IsAltered = isAltered.asBool();
        }

        //是否遮挡
        Json::Value isCovered = MotorValue[i]["IsCovered"];
        if(!isCovered.isNull() && isCovered.isBool()) {
			iMotor.IsCovered = isCovered.asBool();
        }

        //行驶速度
        Json::Value speed = MotorValue[i]["Speed"];
        if(!speed.isNull() && speed.isDouble()) {
			iMotor.Speed = speed.asDouble();
        }

        //行驶方向
        Json::Value direction = MotorValue[i]["Direction"];
        if(!direction.isNull() && direction.isString()) {
			sscanf(direction.asString().c_str(), "%d", &iMotor.Direction);
        }

        //行驶状态代码
        Json::Value drivingStatusCode = MotorValue[i]["DrivingStatusCode"];
        if(!drivingStatusCode.isNull() && drivingStatusCode.isString()) {
			sscanf(drivingStatusCode.asString().c_str(), "%d", &iMotor.DrivingStatusCode);
        }

        //车辆使用性质代码
        Json::Value usingPropertiesCode = MotorValue[i]["UsingPropertiesCode"];
        if(!usingPropertiesCode.isNull() && usingPropertiesCode.isInt()) {
			iMotor.UsingPropertiesCode = usingPropertiesCode.asInt();
        }

        //车辆类型
        Json::Value vehicleClass = MotorValue[i]["VehicleClass"];
        if(!vehicleClass.isNull() && vehicleClass.isString()) {
			sscanf(vehicleClass.asString().c_str(), "%d", &iMotor.VehicleClass);
        }

        //车辆品牌
        Json::Value vehicleBrand = MotorValue[i]["VehicleBrand"];
        if(!vehicleBrand.isNull() && vehicleBrand.isString()) {
			sscanf(vehicleBrand.asString().c_str(), "%d", &iMotor.VehicleBrand);
        }

        //车辆型号
        Json::Value vehicleModel = MotorValue[i]["VehicleModel"];
        if(!vehicleModel.isNull() && vehicleModel.isString()) {
			iMotor.VehicleModel = vehicleModel.asString();
        }

        //车辆年款
        Json::Value vehicleStyles = MotorValue[i]["VehicleStyles"];
        if(!vehicleStyles.isNull() && vehicleStyles.isString()) {
			strncpy(iMotor.VehicleStyles, vehicleStyles.asString().c_str(), sizeof(iMotor.VehicleStyles));
        }

        //车辆长度
        Json::Value vehicleLength = MotorValue[i]["VehicleLength"];
        if(!vehicleLength.isNull() && vehicleLength.isInt()) {
			iMotor.VehicleLength = vehicleLength.asInt();
        }

        //车辆宽度
        Json::Value vehicleWidth = MotorValue[i]["VehicleWidth"];
        if(!vehicleWidth.isNull() && vehicleWidth.isInt()) {
			iMotor.VehicleWidth = vehicleWidth.asInt();
        }

        //车辆高度
        Json::Value vehicleHeight = MotorValue[i]["VehicleHeight"];
        if(!vehicleHeight.isNull() && vehicleHeight.isInt()) {
			iMotor.VehicleHeight = vehicleHeight.asInt();
        }

        //车辆颜色
        Json::Value vehicleColor = MotorValue[i]["VehicleColor"];
        if(!vehicleColor.isNull() && vehicleColor.isString()) {
			sscanf(vehicleColor.asString().c_str(), "%d", &iMotor.VehicleColor);
        }

        //颜色深浅
        Json::Value vehicleColorDepth = MotorValue[i]["VehicleColorDepth"];
        if(!vehicleColorDepth.isNull() && vehicleColorDepth.isString()) {
			sscanf(vehicleColorDepth.asString().c_str(), "%d", &iMotor.VehicleColorDepth);
        }

        //车前盖
        Json::Value vehicleHood = MotorValue[i]["VehicleHood"];
        if(!vehicleHood.isNull() && vehicleHood.isString()) {
			strncpy(iMotor.VehicleHood, vehicleHood.asString().c_str(), sizeof(iMotor.VehicleHood));
        }

        //车后盖
        Json::Value vehicleTrunk = MotorValue[i]["VehicleTrunk"];
        if(!vehicleTrunk.isNull() && vehicleTrunk.isString()) {
			strncpy(iMotor.VehicleTrunk, vehicleTrunk.asString().c_str(), sizeof(iMotor.VehicleTrunk));
        }

        //车轮
        Json::Value vehicleWheel = MotorValue[i]["VehicleWheel"];
        if(!vehicleWheel.isNull() && vehicleWheel.isString()) {
			strncpy(iMotor.VehicleWheel, vehicleWheel.asString().c_str(), sizeof(iMotor.VehicleWheel));
        }

        //车轮印花纹
        Json::Value wheelPrintedPattern = MotorValue[i]["WheelPrintedPattern"];
        if(!wheelPrintedPattern.isNull() && wheelPrintedPattern.isString()) {
			strncpy(iMotor.WheelPrintedPattern, wheelPrintedPattern.asString().c_str(), sizeof(iMotor.WheelPrintedPattern));
        }

        //车窗
        Json::Value vehicleWindow = MotorValue[i]["VehicleWindow"];
        if(!vehicleWindow.isNull() && vehicleWindow.isString()) {
			strncpy(iMotor.VehicleWindow, vehicleWindow.asString().c_str(), sizeof(iMotor.VehicleWindow));
        }

        //车顶
        Json::Value vehicleRoof = MotorValue[i]["VehicleRoof"];
        if(!vehicleRoof.isNull() && vehicleRoof.isString()) {
			strncpy(iMotor.VehicleRoof, vehicleRoof.asString().c_str(), sizeof(iMotor.VehicleRoof));
        }

        //车门
        Json::Value vehicleDoor = MotorValue[i]["VehicleDoor"];
        if(!vehicleDoor.isNull() && vehicleDoor.isString()) {
			strncpy(iMotor.VehicleDoor, vehicleDoor.asString().c_str(), sizeof(iMotor.VehicleDoor));
        }

        //车侧
        Json::Value sideOfVehicle = MotorValue[i]["SideOfVehicle"];
        if(!sideOfVehicle.isNull() && sideOfVehicle.isString()) {
			strncpy(iMotor.SideOfVehicle, sideOfVehicle.asString().c_str(), sizeof(iMotor.SideOfVehicle));
        }

        //车厢
        Json::Value carOfVehicle = MotorValue[i]["CarOfVehicle"];
        if(!carOfVehicle.isNull() && carOfVehicle.isString()) {
			strncpy(iMotor.CarOfVehicle, carOfVehicle.asString().c_str(), sizeof(iMotor.CarOfVehicle));
        }

        //后视镜
        Json::Value rearviewMirror = MotorValue[i]["RearviewMirror"];
        if(!rearviewMirror.isNull() && rearviewMirror.isString()) {
			strncpy(iMotor.RearviewMirror, rearviewMirror.asString().c_str(), sizeof(iMotor.RearviewMirror));
        }

        //地盘
        Json::Value vehicleChassis = MotorValue[i]["VehicleChassis"];
        if(!vehicleChassis.isNull() && motorVehicleID.isString()) {
			strncpy(iMotor.VehicleChassis, motorVehicleID.asString().c_str(), sizeof(iMotor.VehicleChassis));
        }

        //遮挡
        Json::Value vehicleShielding = MotorValue[i]["VehicleShielding"];
        if(!vehicleShielding.isNull() && vehicleShielding.isString()) {
			strncpy(iMotor.VehicleShielding, vehicleShielding.asString().c_str(), sizeof(iMotor.VehicleShielding));
        }

        //车身贴膜颜色
        Json::Value filmColor = MotorValue[i]["FilmColor"];
        if(!filmColor.isNull() && filmColor.isString()) {
			iMotor.FilmColor = filmColor.asString();
        }

        //改装标志
        Json::Value isModified = MotorValue[i]["IsModified"];
        if(!isModified.isNull() && isModified.isBool()) {
			iMotor.IsModified = isModified.asBool();
        }

        //撞痕信息 
        Json::Value hitMarkInfo = MotorValue[i]["HitMarkInfo"];
        if(!hitMarkInfo.isNull() && hitMarkInfo.isString()) {
			iMotor.HitMarkInfo = hitMarkInfo.asString();
        }

        //车身描述
        Json::Value vehicleBodyDesc = MotorValue[i]["VehicleBodyDesc"];
        if(!vehicleBodyDesc.isNull() && vehicleBodyDesc.isString()) {
			strncpy(iMotor.VehicleBodyDesc, vehicleBodyDesc.asString().c_str(), sizeof(iMotor.VehicleBodyDesc));
        }

        //车前部物品
        Json::Value vehicleFrontItem = MotorValue[i]["VehicleFrontItem"];
        if(!vehicleFrontItem.isNull() && vehicleFrontItem.isString()) {
			char str[1024] = {0};
			strncpy(str, vehicleFrontItem.asString().c_str(), sizeof(str));
			char* p = strtok(str, ";");
			while (p)
			{
                iMotor.VehicleFrontItem.push_back(atoi(p));
				p = strtok(NULL, ";");
			}
        }

        //车前部描述
        Json::Value descOfFrontItem = MotorValue[i]["DescOfFrontItem"];
        if(!descOfFrontItem.isNull() && descOfFrontItem.isString()) {
			strncpy(iMotor.DescOfFrontItem, descOfFrontItem.asString().c_str(), sizeof(iMotor.DescOfFrontItem));
        }

        //车后部物品
        Json::Value vehicleRearItem = MotorValue[i]["VehicleRearItem"];
        if(!vehicleRearItem.isNull() && vehicleRearItem.isString()) {
			char str[1024] = {0};
			strncpy(str, vehicleRearItem.asString().c_str(), sizeof(str));
			char* p = strtok(str, ";");
			while (p)
			{
                iMotor.VehicleRearItem.push_back(atoi(p));
				p = strtok(NULL, ";");
			}
        }

        //车后部描述
        Json::Value descOfRearItem = MotorValue[i]["DescOfRearItem"];
        if(!descOfRearItem.isNull() && descOfRearItem.isString()) {
			strncpy(iMotor.DescOfRearItem, descOfRearItem.asString().c_str(), sizeof(iMotor.DescOfRearItem));
        }

        //车内人数
        Json::Value numOfPassenger = MotorValue[i]["NumOfPassenger"];
        if(!numOfPassenger.isNull() && numOfPassenger.isInt()) {
			iMotor.NumOfPassenger = numOfPassenger.asInt();
        }

        //经过时刻
        Json::Value passTime = MotorValue[i]["PassTime"];
        if(!passTime.isNull() && passTime.isString()) {
			strncpy(iMotor.PassTime, passTime.asString().c_str(), sizeof(iMotor.PassTime));
        }

        //经过道路名称
        Json::Value nameOfPassedRoad = MotorValue[i]["NameOfPassedRoad"];
        if(!nameOfPassedRoad.isNull() && nameOfPassedRoad.isString()) {
			strncpy(iMotor.NameOfPassedRoad, nameOfPassedRoad.asString().c_str(), sizeof(iMotor.NameOfPassedRoad));
        }

        //是否可疑车
        Json::Value isSuspicious = MotorValue[i]["IsSuspicious"];
        if(!isSuspicious.isNull() && isSuspicious.isBool()) {
			iMotor.IsSuspicious = isSuspicious.asBool();
        }

        //遮阳板状态(0收起， 1放下)
        Json::Value sunvisor = MotorValue[i]["Sunvisor"];
        if(!sunvisor.isNull() && sunvisor.isInt()) {
			iMotor.Sunvisor = sunvisor.asInt();
        }

        //安全带状态(0未系， 1有系)
        Json::Value safetyBelt = MotorValue[i]["SafetyBelt"];
        if(!safetyBelt.isNull() && safetyBelt.isInt()) {
			iMotor.SafetyBelt = safetyBelt.asInt();
        }

        //打电话状态(0未打电话， 1打电话中)
        Json::Value calling = MotorValue[i]["Calling"];
        if(!calling.isNull() && calling.isInt()) {
			iMotor.Calling = calling.asInt();
        }

        //号码识别可信度
        Json::Value platgeReliablity = MotorValue[i]["PlatgeReliablity"];
        if(!platgeReliablity.isNull() && platgeReliablity.isString()) {
			strncpy(iMotor.PlatgeReliablity, platgeReliablity.asString().c_str(), sizeof(iMotor.PlatgeReliablity));
        }

        //每位车牌号码可信度
        Json::Value plateCharReliability = MotorValue[i]["PlateCharReliability"];
        if(!plateCharReliability.isNull() && plateCharReliability.isString()) {
			strncpy(iMotor.PlateCharReliability, plateCharReliability.asString().c_str(), sizeof(iMotor.PlateCharReliability));
        }

        //品牌标识可信度
        Json::Value brandReliability = MotorValue[i]["BrandReliability"];
        if(!brandReliability.isNull() && brandReliability.isString()) {
			strncpy(iMotor.BrandReliability, brandReliability.asString().c_str(), sizeof(iMotor.BrandReliability));
        }

        ////图像数据
        Json::Value SubImageList = MotorValue[i]["SubImageList"];
        if(!SubImageList.isNull()) {
            std::string strjson = MotorValue[i].toStyledString();
            if(UnPackSubImageListJson(strjson, iMotor.SubImageList) == -1) {
                return -1;
            }
        }
        MotorList.push_back(iMotor);
    }
    return 0;
}

int GAT1400Json::UnPackPersonListJson(const std::string& strJson, std::list<GAT_1400_Person>& PersonList)
{
	Json::Reader reader;
	Json::Value dataValue;
	if (!reader.parse(strJson,dataValue)) {
		TVT_LOG_ERROR("UnPackPersonListJson fail: cannot parse json");
		return -1;
	}

	if (!dataValue.isObject()) {
		TVT_LOG_ERROR("UnPackPersonListJson fail: json_object is null");
		return -1;
	}

	Json::Value PersonValue;
	int isize = 0;
	int bhaveList = -1;
	if (dataValue["PersonListObject"].isNull()) {
		TVT_LOG_ERROR("UnPackPersonListJson fail: PersonListObject is null");
		return -1;
	} else {
		bhaveList = 0;
		Json::Value datavalue = dataValue["PersonListObject"];
		PersonValue = datavalue["PersonObject"];
		isize = PersonValue.size();
	}
	for(int i = 0; i < isize; i++)
	{
		GAT_1400_Person iPerson;

		//人员id 必需字段
		Json::Value PersonID = PersonValue[i]["PersonID"];
		if(!PersonID.isNull() && PersonID.isString()) {
			strncpy(iPerson.PersonID, PersonID.asString().c_str(), sizeof(iPerson.PersonID));
		} else {
			TVT_LOG_ERROR("UnPackPersonListJson fail: PersonID is null");
			return -1;
		}

		//信息分类 必需字段
		Json::Value infoKind = PersonValue[i]["InfoKind"];
		if(!infoKind.isNull() && infoKind.isInt()) {
			iPerson.InfoKind = infoKind.asInt();
		} else {
			TVT_LOG_ERROR("UnPackPersonListJson fail: InfoKind is null");
			return -1;
		}

		//来源标识
		Json::Value sourceID = PersonValue[i]["SourceID"];
		if(!sourceID.isNull() && sourceID.isString()) {
			strncpy(iPerson.SourceID, sourceID.asString().c_str(), sizeof(iPerson.SourceID));
		} else {
			TVT_LOG_ERROR("UnPackPersonListJson fail: SourceID is null");
			return -1;
		}

		//设备编码 |采集设备ID 自动采集必选
		Json::Value deviceID = PersonValue[i]["DeviceID"];
		if(iPerson.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (deviceID.isNull() || !deviceID.isString()) {
				TVT_LOG_ERROR("UnPackPersonListJson fail: DeviceID is null");
				return -1;
			}
			strncpy(iPerson.DeviceID, deviceID.asString().c_str(), sizeof(iPerson.DeviceID));
		} else {
			if (!deviceID.isNull() && deviceID.isString()) {
				strncpy(iPerson.DeviceID, deviceID.asString().c_str(), sizeof(iPerson.DeviceID));
			}
		}

		//左上角X坐标 | 自动采集必选
		Json::Value leftTopX = PersonValue[i]["LeftTopX"];
		if(iPerson.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (leftTopX.isNull() || !leftTopX.isInt())	{
				TVT_LOG_ERROR("UnPackPersonListJson fail: LeftTopX is null");
				return -1;
			}
			iPerson.LeftTopX = leftTopX.asInt();
		} else {
			if (!leftTopX.isNull() && leftTopX.isInt())	{
				iPerson.LeftTopX = leftTopX.asInt();
			}
		}

		//左上角Y坐标 | 自动采集必选
		Json::Value leftTopY = PersonValue[i]["LeftTopY"];
		if(iPerson.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (leftTopY.isNull() || !leftTopY.isInt())	{
				TVT_LOG_ERROR("UnPackPersonListJson fail: LeftTopY is null");
				return -1;
			}
			iPerson.LeftTopY = leftTopY.asInt();
		} else {
			if (!leftTopY.isNull() && leftTopY.isInt())	{
				iPerson.LeftTopY = leftTopY.asInt();
			}
		}

		//右下角X坐标 | 自动采集必选
		Json::Value rightBtmX = PersonValue[i]["RightBtmX"];
		if(iPerson.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (rightBtmX.isNull() || !rightBtmX.isInt()) {
				TVT_LOG_ERROR("UnPackPersonListJson fail: RightBtmX is null");
				return -1;
			}
			iPerson.RightBtmX = rightBtmX.asInt();
		} else {
			if (!rightBtmX.isNull() && rightBtmX.isInt()) {
				iPerson.RightBtmX = rightBtmX.asInt();
			}
		}

		//右下角Y坐标 | 自动采集必选
		Json::Value rightBtmY = PersonValue[i]["RightBtmY"];
		if(iPerson.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (rightBtmY.isNull() || !rightBtmY.isInt()) {
				TVT_LOG_ERROR("UnPackPersonListJson fail: RightBtmY is null");
				return -1;
			}
			iPerson.RightBtmY = rightBtmY.asInt();
		} else {
			if (!rightBtmY.isNull() && rightBtmY.isInt()) {
				iPerson.RightBtmY = rightBtmY.asInt();
			}
		}

		//位置标记时间
		Json::Value LocationMarkTime = PersonValue[i]["LocationMarkTime"];
		if(!LocationMarkTime.isNull() && LocationMarkTime.isString()) {
			strncpy(iPerson.LocationMarkTime, LocationMarkTime.asString().c_str(), sizeof(iPerson.LocationMarkTime));
		}

		//人员出现时间
		Json::Value appearTime = PersonValue[i]["PersonAppearTime"];
		if(!appearTime.isNull() && appearTime.isString()) {
			strncpy(iPerson.PersonAppearTime, appearTime.asString().c_str(), sizeof(iPerson.PersonAppearTime));
		}

		//人员消失时间
		Json::Value disappearTime = PersonValue[i]["PersonDisAppearTime"];
		if(!disappearTime.isNull() && disappearTime.isString()) {
			strncpy(iPerson.PersonDisAppearTime, disappearTime.asString().c_str(), sizeof(iPerson.PersonDisAppearTime));
		}

		//证件类型
		Json::Value idType = PersonValue[i]["IDType"];
		if(!idType.isNull() && idType.isString()) {
			sscanf(idType.asString().c_str(), "%d", &iPerson.IDType);
		}

		//证件号码
		Json::Value idNumber = PersonValue[i]["IDNumber"];
		if(!idNumber.isNull() && idNumber.isString()) {
			strncpy(iPerson.IDNumber, idNumber.asString().c_str(), sizeof(iPerson.IDNumber));
		}

		//姓名
		Json::Value name = PersonValue[i]["Name"];
		if(!name.isNull() && name.isString()) {
			strncpy(iPerson.Name, name.asString().c_str(), sizeof(iPerson.Name));
		}

		//曾用名
		Json::Value usedName = PersonValue[i]["UsedName"];
		if(!usedName.isNull() && usedName.isString()) {
			strncpy(iPerson.UsedName, usedName.asString().c_str(), sizeof(iPerson.UsedName));
		}

		//绰号
		Json::Value alias = PersonValue[i]["Alias"];
		if(!alias.isNull() && alias.isString()) {
			strncpy(iPerson.Alias, alias.asString().c_str(), sizeof(iPerson.Alias));
		}

		//性别
		Json::Value genderCode = PersonValue[i]["GenderCode"];
		if (!genderCode.isNull() && genderCode.isString()) {
			sscanf(genderCode.asString().c_str(), "%d", &iPerson.GenderCode);
		}
		//最大可能年龄
		Json::Value ageUpLimit = PersonValue[i]["AgeUpLimit"];
		if(!ageUpLimit.isNull() && ageUpLimit.isInt()) {
			iPerson.AgeUpLimit = ageUpLimit.asInt();
		}

		//最小可能年龄
		Json::Value ageLowerLimit = PersonValue[i]["AgeLowerLimit"];
		if(!ageLowerLimit.isNull() && ageLowerLimit.isInt()) {
			iPerson.AgeLowerLimit = ageLowerLimit.asInt();
		}

		//中国各民族的罗马字母拼写法和代码
		Json::Value fthicCode = PersonValue[i]["EthicCode"];
		if(!fthicCode.isNull() && fthicCode.isString()) {
			strncpy(iPerson.EthicCode, fthicCode.asString().c_str(), sizeof(iPerson.EthicCode));
		}

		//国籍代码、世界各国和地区名称代码
		Json::Value nationalityCode = PersonValue[i]["NationalityCode"];
		if(!nationalityCode.isNull() && nationalityCode.isString()) {
			strncpy(iPerson.NationalityCode, nationalityCode.asString().c_str(), sizeof(iPerson.NationalityCode));
		}

		//籍贯省市县代码
		Json::Value nativeCityCode = PersonValue[i]["NativeCityCode"];
		if(!nativeCityCode.isNull() && nativeCityCode.isString()) {
			strncpy(iPerson.NativeCityCode, nativeCityCode.asString().c_str(), sizeof(iPerson.NativeCityCode));
		}

		//居住地行政区划
		Json::Value residenceAdminDivision = PersonValue[i]["ResidenceAdminDivision"];
		if(!residenceAdminDivision.isNull() && residenceAdminDivision.isString()) {
			strncpy(iPerson.ResidenceAdminDivision, residenceAdminDivision.asString().c_str(), sizeof(iPerson.ResidenceAdminDivision));
		}

		//汉语口音
		Json::Value chineseAccentCode = PersonValue[i]["ChineseAccentCode"];
		if(!chineseAccentCode.isNull() && chineseAccentCode.isString()) {
			strncpy(iPerson.ChineseAccentCode, chineseAccentCode.asString().c_str(), sizeof(iPerson.ChineseAccentCode));
		}

		//单位名称
		Json::Value personOrg = PersonValue[i]["PersonOrg"];
		if(!personOrg.isNull() && personOrg.isString()) {
			strncpy(iPerson.PersonOrg, personOrg.asString().c_str(), sizeof(iPerson.PersonOrg));
		}

		//职业
		Json::Value jobCategory = PersonValue[i]["JobCategory"];
		if(!jobCategory.isNull() && jobCategory.isString()) {
			strncpy(iPerson.JobCategory, jobCategory.asString().c_str(), sizeof(iPerson.JobCategory));
		}

		//同行人数
		Json::Value AccompanyNumber = PersonValue[i]["AccompanyNumber"];
		if(!AccompanyNumber.isNull() && AccompanyNumber.isInt()) {
			iPerson.AccompanyNumber = AccompanyNumber.asInt();
		}

		//身高上限
		Json::Value HeightUpLimit = PersonValue[i]["HeightUpLimit"];
		if(!HeightUpLimit.isNull() && HeightUpLimit.isInt()) {
			iPerson.HeightUpLimit = HeightUpLimit.asInt();
		}

		//身高下限
		Json::Value HeightLowerLimit = PersonValue[i]["HeightLowerLimit"];
		if(!HeightLowerLimit.isNull() && HeightLowerLimit.isInt()) {
			iPerson.HeightLowerLimit = HeightLowerLimit.asInt();
		}

		//体型
		Json::Value BodyType = PersonValue[i]["BodyType"];
		if(!BodyType.isNull() && BodyType.isString()) {
			iPerson.BodyType = BodyType.asString();
		}

		//肤色
		Json::Value skinColor = PersonValue[i]["SkinColor"];
		if(!skinColor.isNull() && skinColor.isString()) {
			iPerson.SkinColor = skinColor.asString();
		}

		//发型
		Json::Value hairStyle = PersonValue[i]["HairStyle"];
		if(!hairStyle.isNull() && hairStyle.isString()) {
			sscanf(hairStyle.asString().c_str(), "%d", &iPerson.HairStyle);
		}

		//发色
		Json::Value hairColor = PersonValue[i]["HairColor"];
		if(!hairColor.isNull() && hairColor.isString()) {
			sscanf(hairColor.asString().c_str(), "%d", &iPerson.HairColor);
		}

		//姿态
		Json::Value Gesture = PersonValue[i]["Gesture"];
		if(!Gesture.isNull() && Gesture.isString()) {
			sscanf(Gesture.asString().c_str(), "%d", &iPerson.Gesture);
		}

		//状态
		Json::Value Status = PersonValue[i]["Status"];
		if(!Status.isNull() && Status.isString()) {
			sscanf(Status.asString().c_str(), "%d", &iPerson.Status);
		}

		//脸型
		Json::Value faceStyle = PersonValue[i]["FaceStyle"];
		if(!faceStyle.isNull() && faceStyle.isString()) {
			strncpy(iPerson.FaceStyle, faceStyle.asString().c_str(), sizeof(iPerson.FaceStyle));
		}

		//脸部特征
		Json::Value facialFeature = PersonValue[i]["FacialFeature"];
		if(!facialFeature.isNull() && facialFeature.isString()) {
			strncpy(iPerson.FacialFeature, facialFeature.asString().c_str(), sizeof(iPerson.FacialFeature));
		}

		//体貌特征
		Json::Value physicalFeature = PersonValue[i]["PhysicalFeature"];
		if(!physicalFeature.isNull() && physicalFeature.isString()) {
			strncpy(iPerson.PhysicalFeature, physicalFeature.asString().c_str(), sizeof(iPerson.PhysicalFeature));
		}

		//体表特征
		Json::Value BodyFeature = PersonValue[i]["BodyFeature"];
		if(!BodyFeature.isNull() && BodyFeature.isString()) {
			strncpy(iPerson.BodyFeature, BodyFeature.asString().c_str(), sizeof(iPerson.BodyFeature));
		}

		//习惯动作
		Json::Value HabitualMovement = PersonValue[i]["HabitualMovement"];
		if(!HabitualMovement.isNull() && HabitualMovement.isString()) {
			sscanf(HabitualMovement.asString().c_str(), "%d", &iPerson.HabitualMovement);
		}

		//行为
		Json::Value Behavior = PersonValue[i]["Behavior"];
		if(!Behavior.isNull() && Behavior.isString()) {
			sscanf(Behavior.asString().c_str(), "%d", &iPerson.Behavior);
		}

		//行为描述
		Json::Value BehaviorDescription = PersonValue[i]["BehaviorDescription"];
		if(!BehaviorDescription.isNull() && BehaviorDescription.isString()) {
			iPerson.BehaviorDescription = BehaviorDescription.asString();
		}

		//附属物
		Json::Value Appendant = PersonValue[i]["Appendant"];
		if(!Appendant.isNull() && Appendant.isString()) {
			std::vector<std::string> vecAppendant;
			_split(Appendant.asString(), ';', vecAppendant);
			for (std::vector<std::string>::iterator iter = vecAppendant.begin(); iter != vecAppendant.end(); ++iter) {
				int nAppendant = 0;
				sscanf(iter->c_str(), "%d", &nAppendant);
				iPerson.Appendant.push_back(nAppendant);
			}
		}

		//附属物描述
		Json::Value AppendantDescription = PersonValue[i]["AppendantDescription"];
		if(!AppendantDescription.isNull() && AppendantDescription.isString()) {
			iPerson.AppendantDescription = AppendantDescription.asString();
		}

		//伞颜色
		Json::Value UmbrellaColor = PersonValue[i]["UmbrellaColor"];
		if(!UmbrellaColor.isNull() && UmbrellaColor.isString()) {
			sscanf(UmbrellaColor.asString().c_str(), "%d", &iPerson.UmbrellaColor);
		}

		//口罩颜色
		Json::Value RespiratorColor = PersonValue[i]["RespiratorColor"];
		if(!RespiratorColor.isNull() && RespiratorColor.isString()) {
			sscanf(RespiratorColor.asString().c_str(), "%d", &iPerson.RespiratorColor);
		}

		//帽子款式
		Json::Value CapStyle = PersonValue[i]["CapStyle"];
		if(!CapStyle.isNull() && CapStyle.isString()) {
			sscanf(CapStyle.asString().c_str(), "%d", &iPerson.CapStyle);
		}

		//帽子颜色
		Json::Value CapColor = PersonValue[i]["CapColor"];
		if(!CapColor.isNull() && CapColor.isString()) {
			sscanf(CapColor.asString().c_str(), "%d", &iPerson.CapColor);
		}

		//眼镜款式
		Json::Value GlassStyle = PersonValue[i]["GlassStyle"];
		if(!GlassStyle.isNull() && GlassStyle.isString()) {
			sscanf(GlassStyle.asString().c_str(), "%d", &iPerson.GlassStyle);
		}

		//眼镜颜色
		Json::Value GlassColor = PersonValue[i]["GlassColor"];
		if(!GlassColor.isNull() && GlassColor.isString()) {
			sscanf(GlassColor.asString().c_str(), "%d", &iPerson.GlassColor);
		}

		//围巾颜色
		Json::Value ScarfColor = PersonValue[i]["ScarfColor"];
		if(!ScarfColor.isNull() && ScarfColor.isString()) {
			sscanf(ScarfColor.asString().c_str(), "%d", &iPerson.ScarfColor);
		}

		//包款式
		Json::Value BagStyle = PersonValue[i]["BagStyle"];
		if(!BagStyle.isNull() && BagStyle.isString()) {
			sscanf(BagStyle.asString().c_str(), "%d", &iPerson.BagStyle);
		}

		//包颜色
		Json::Value BagColor = PersonValue[i]["BagColor"];
		if(!BagColor.isNull() && BagColor.isString()) {
			sscanf(BagColor.asString().c_str(), "%d", &iPerson.BagColor);
		}

		//上衣款式
		Json::Value CoatStyle = PersonValue[i]["CoatStyle"];
		if(!CoatStyle.isNull() && CoatStyle.isString()) {
			sscanf(CoatStyle.asString().c_str(), "%d", &iPerson.CoatStyle);
		}

		//上衣长度
		Json::Value CoatLength = PersonValue[i]["CoatLength"];
		if(!CoatLength.isNull() && CoatLength.isString()) {
			iPerson.CoatLength = CoatLength.asString();
		}

		//上衣颜色
		Json::Value CoatColor = PersonValue[i]["CoatColor"];
		if(!CoatColor.isNull() && CoatColor.isString()) {
			sscanf(CoatColor.asString().c_str(), "%d", &iPerson.CoatColor);
		}

		//裤子款式
		Json::Value TrousersStyle = PersonValue[i]["TrousersStyle"];
		if(!TrousersStyle.isNull() && TrousersStyle.isString()) {
			sscanf(TrousersStyle.asString().c_str(), "%d", &iPerson.TrousersStyle);
		}

		//裤子颜色
		Json::Value TrousersColor = PersonValue[i]["TrousersColor"];
		if(!TrousersColor.isNull() && TrousersColor.isString()) {
			sscanf(TrousersColor.asString().c_str(), "%d", &iPerson.TrousersColor);
		}

		//裤子长度
		Json::Value TrousersLen = PersonValue[i]["TrousersLen"];
		if(!TrousersLen.isNull() && TrousersLen.isString()) {
			iPerson.TrousersLen = TrousersLen.asString();
		}

		//鞋子款式
		Json::Value ShoesStyle = PersonValue[i]["ShoesStyle"];
		if(!ShoesStyle.isNull() && ShoesStyle.isString()) {
			sscanf(ShoesStyle.asString().c_str(), "%d", &iPerson.ShoesStyle);
		}

		//鞋子颜色
		Json::Value ShoesColor = PersonValue[i]["ShoesColor"];
		if(!ShoesColor.isNull() && ShoesColor.isString()) {
			sscanf(ShoesColor.asString().c_str(), "%d", &iPerson.ShoesColor);
		}

		//是否驾驶员
		Json::Value IsDriver = PersonValue[i]["IsDriver"];
		if(!IsDriver.isNull() && IsDriver.isInt()) {
			iPerson.IsDriver = IsDriver.asInt();
		}

		//是否涉外人员
		Json::Value isForeigner = PersonValue[i]["IsForeigner"];
		if(!isForeigner.isNull() && isForeigner.isInt()) {
			iPerson.IsForeigner = isForeigner.asInt();
		}

		//护照证件类型
		Json::Value passportType = PersonValue[i]["PassportType"];
		if(!passportType.isNull() && passportType.isString()) {
			sscanf(passportType.asString().c_str(), "%d", &iPerson.PassportType);
		}

		//出入境人员类型代码
		Json::Value immigrantTypeCode = PersonValue[i]["ImmigrantTypeCode"];
		if(!immigrantTypeCode.isNull() && immigrantTypeCode.isString()) {
			strncpy(iPerson.ImmigrantTypeCode, immigrantTypeCode.asString().c_str(), sizeof(iPerson.ImmigrantTypeCode));
		}

		//是否涉恐人员
		Json::Value isSuspectedTerrorist = PersonValue[i]["IsSuspectedTerrorist"];
		if(!isSuspectedTerrorist.isNull() && isSuspectedTerrorist.isInt()) {
			iPerson.IsSuspectedTerrorist = isSuspectedTerrorist.asInt();
		}

		//涉恐人员编号
		Json::Value suspectedTreeoristNumber = PersonValue[i]["SuspectedTreeoristNumber"];
		if(!suspectedTreeoristNumber.isNull() && suspectedTreeoristNumber.isString()) {
			strncpy(iPerson.SuspectedTreeoristNumber, suspectedTreeoristNumber.asString().c_str(), sizeof(iPerson.SuspectedTreeoristNumber));
		}

		//是否涉案人员
		Json::Value isCriminalInvolved = PersonValue[i]["IsCriminalInvolved"];
		if(!isCriminalInvolved.isNull() && isCriminalInvolved.isInt()) {
			iPerson.IsCriminalInvolved = isCriminalInvolved.asInt();
		}

		//涉案人员专长代码
		Json::Value criminalInvolvedSpecilisationCode = PersonValue[i]["CriminalInvolvedSpecilisationCode"];
		if(!criminalInvolvedSpecilisationCode.isNull() && criminalInvolvedSpecilisationCode.isString()) {
			strncpy(iPerson.CriminalInvolvedSpecilisationCode, criminalInvolvedSpecilisationCode.asString().c_str(), sizeof(iPerson.CriminalInvolvedSpecilisationCode));
		}

		//体表特殊标记
		Json::Value bodySpeciallMark = PersonValue[i]["BodySpeciallMark"];
		if(!bodySpeciallMark.isNull() && bodySpeciallMark.isString()) {
			strncpy(iPerson.BodySpeciallMark, bodySpeciallMark.asString().c_str(), sizeof(iPerson.BodySpeciallMark));
		}

		//作案手段
		Json::Value crimeMethod = PersonValue[i]["CrimeMethod"];
		if(!crimeMethod.isNull() && crimeMethod.isString()) {
			strncpy(iPerson.CrimeMethod, crimeMethod.asString().c_str(), sizeof(iPerson.CrimeMethod));
		}

		//作案特点代码
		Json::Value crimeCharacterCode = PersonValue[i]["CrimeCharacterCode"];
		if(!crimeCharacterCode.isNull() && crimeCharacterCode.isString()) {
			strncpy(iPerson.CrimeCharacterCode, crimeCharacterCode.asString().c_str(), sizeof(iPerson.CrimeCharacterCode));
		}

		//在逃人员信息编号规则
		Json::Value escapedCriminalNumber = PersonValue[i]["EscapedCriminalNumber"];
		if(!escapedCriminalNumber.isNull() && escapedCriminalNumber.isString()) {
			strncpy(iPerson.EscapedCriminalNumber, escapedCriminalNumber.asString().c_str(), sizeof(iPerson.EscapedCriminalNumber));
		}

		//是否在押人员
		Json::Value isDetainees = PersonValue[i]["IsDetainees"];
		if(!isDetainees.isNull() && isDetainees.isInt()) {
			iPerson.IsDetainees = isDetainees.asInt();
		}

		//看守所编码
		Json::Value detentionHouseCode = PersonValue[i]["DetentionHouseCode"];
		if(!detentionHouseCode.isNull() && detentionHouseCode.isString()) {
			strncpy(iPerson.DetentionHouseCode, detentionHouseCode.asString().c_str(), sizeof(iPerson.DetentionHouseCode));
		}

		//在押人员身份
		Json::Value detainessIdentity = PersonValue[i]["DetainessIdentity"];
		if(!detainessIdentity.isNull() && detainessIdentity.isInt()) {
			iPerson.DetainessIdentity = detainessIdentity.asInt();
		}

		//在押人员特殊身份编码
		Json::Value detaineesSpecialIdentity = PersonValue[i]["DetaineesSpecialIdentity"];
		if(!detaineesSpecialIdentity.isNull() && detaineesSpecialIdentity.isString()) {
			strncpy(iPerson.DetaineesSpecialIdentity, detaineesSpecialIdentity.asString().c_str(), sizeof(iPerson.DetaineesSpecialIdentity));
		}

		//在押人员类型
		Json::Value memberTypeCode = PersonValue[i]["MemberTypeCode"];
		if(!memberTypeCode.isNull() && memberTypeCode.isString()) {
			strncpy(iPerson.MemberTypeCode, memberTypeCode.asString().c_str(), sizeof(iPerson.MemberTypeCode));
		}

		//是否被害人员
		Json::Value isVictim = PersonValue[i]["IsVictim"];
		if(!isVictim.isNull() && isVictim.isInt()) {
			iPerson.IsVictim = isVictim.asInt();
		}

		//被害人类型
		Json::Value victimType = PersonValue[i]["VictimType"];
		if(!victimType.isNull() && victimType.isString()) {
			strncpy(iPerson.VictimType, victimType.asString().c_str(), sizeof(iPerson.VictimType));
		}

		//受伤害程度
		Json::Value injuredDegree = PersonValue[i]["InjuredDegree"];
		if(!injuredDegree.isNull() && injuredDegree.isString()) {
			iPerson.InjuredDegree = injuredDegree.asString();
		}

		//尸体状态
		Json::Value corpseConditionCode = PersonValue[i]["CorpseConditionCode"];
		if(!corpseConditionCode.isNull() && corpseConditionCode.isString()) {
			strncpy(iPerson.CorpseConditionCode, corpseConditionCode.asString().c_str(), sizeof(iPerson.CorpseConditionCode));
		}

		//是否可疑人员
		Json::Value isSuspiciousPerson = PersonValue[i]["IsSuspiciousPerson"];
		if(!isSuspiciousPerson.isNull() && isSuspiciousPerson.isInt()) {
			iPerson.IsSuspiciousPerson = isSuspiciousPerson.asInt();
		}

		//图像列表
		Json::Value SubImageList = PersonValue[i]["SubImageList"];
		if(!SubImageList.isNull()) {
			std::string strjson = PersonValue[i].toStyledString();
			if(UnPackSubImageListJson(strjson, iPerson.SubImageList) == -1) {
				return -1;
			}
		}

		PersonList.push_back(iPerson);
	}
	return 0;
}

int GAT1400Json::UnPackNonMotorListJson(const std::string& strJson, std::list<GAT_1400_NonMotor>& NonMotorList)
{
	Json::Reader reader;
	Json::Value dataValue;
	if (!reader.parse(strJson,dataValue)) {
		TVT_LOG_ERROR("UnPackNonMotorListJson fail: cannot parse json");
		return -1;
	}

	if (!dataValue.isObject()) {
		TVT_LOG_ERROR("UnPackNonMotorListJson fail: json_object is null");
		return -1;
	}

	Json::Value NonMotorValue;
	int isize = 0;
	int bhaveList = -1;
	if (dataValue["NonMotorVehicleListObject"].isNull()) {
		TVT_LOG_ERROR("UnPackNonMotorListJson fail: NonMotorVehicleListObject is null");
		return -1;
	} else {
		bhaveList = 0;
		Json::Value datavalue = dataValue["NonMotorVehicleListObject"];
		NonMotorValue = datavalue["NonMotorVehicleObject"];
		isize = NonMotorValue.size();
	}
	for(int i = 0; i < isize; i++)
	{
		GAT_1400_NonMotor iNonMotor;

		//车辆标识 必需字段
		Json::Value NonMotorVehicleID = NonMotorValue[i]["NonMotorVehicleID"];
		if(!NonMotorVehicleID.isNull() && NonMotorVehicleID.isString()) {
			strncpy(iNonMotor.NonMotorVehicleID, NonMotorVehicleID.asString().c_str(), sizeof(iNonMotor.NonMotorVehicleID));
		} else {
			TVT_LOG_ERROR("UnPackNonMotorListJson fail: NonMotorVehicleID is null");
			return -1;
		}

		//信息分类 必需字段
		Json::Value infoKind = NonMotorValue[i]["InfoKind"];
		if(!infoKind.isNull() && infoKind.isInt()) {
			iNonMotor.InfoKind = infoKind.asInt();
		} else {
			TVT_LOG_ERROR("UnPackNonMotorListJson fail: InfoKind is null");
			return -1;
		}

		//来源标识
		Json::Value sourceID = NonMotorValue[i]["SourceID"];
		if(!sourceID.isNull() && sourceID.isString()) {
			strncpy(iNonMotor.SourceID, sourceID.asString().c_str(), sizeof(iNonMotor.SourceID));
		} else {
			TVT_LOG_ERROR("UnPackNonMotorListJson fail: SourceID is null");
			return -1;
		}

		//设备编码 |采集设备ID 自动采集必选
		Json::Value deviceID = NonMotorValue[i]["DeviceID"];
		if(iNonMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (deviceID.isNull() || !deviceID.isString()) {
				TVT_LOG_ERROR("UnPackNonMotorListJson fail: DeviceID is null");
				return -1;
			}
			strncpy(iNonMotor.DeviceID, deviceID.asString().c_str(), sizeof(iNonMotor.DeviceID));
		} else {
			if (!deviceID.isNull() && deviceID.isString()) {
				strncpy(iNonMotor.DeviceID, deviceID.asString().c_str(), sizeof(iNonMotor.DeviceID));
			}
		}

		//左上角X坐标 | 自动采集必选
		Json::Value leftTopX = NonMotorValue[i]["LeftTopX"];
		if(iNonMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (leftTopX.isNull() || !leftTopX.isInt())	{
				TVT_LOG_ERROR("UnPackNonMotorListJson fail: LeftTopX is null");
				return -1;
			}
			iNonMotor.LeftTopX = leftTopX.asInt();
		} else {
			if (!leftTopX.isNull() && leftTopX.isInt())	{
				iNonMotor.LeftTopX = leftTopX.asInt();
			}
		}

		//左上角Y坐标 | 自动采集必选
		Json::Value leftTopY = NonMotorValue[i]["LeftTopY"];
		if(iNonMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (leftTopY.isNull() || !leftTopY.isInt())	{
				TVT_LOG_ERROR("UnPackNonMotorListJson fail: LeftTopY is null");
				return -1;
			}
			iNonMotor.LeftTopY = leftTopY.asInt();
		} else {
			if (!leftTopY.isNull() && leftTopY.isInt())	{
				iNonMotor.LeftTopY = leftTopY.asInt();
			}
		}

		//右下角X坐标 | 自动采集必选
		Json::Value rightBtmX = NonMotorValue[i]["RightBtmX"];
		if(iNonMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (rightBtmX.isNull() || !rightBtmX.isInt()) {
				TVT_LOG_ERROR("UnPackNonMotorListJson fail: RightBtmX is null");
				return -1;
			}
			iNonMotor.RightBtmX = rightBtmX.asInt();
		} else {
			if (!rightBtmX.isNull() && rightBtmX.isInt()) {
				iNonMotor.RightBtmX = rightBtmX.asInt();
			}
		}

		//右下角Y坐标 | 自动采集必选
		Json::Value rightBtmY = NonMotorValue[i]["RightBtmY"];
		if(iNonMotor.InfoKind == INFO_TYPE_AUTOMATIC) {
			if (rightBtmY.isNull() || !rightBtmY.isInt()) {
				TVT_LOG_ERROR("UnPackNonMotorListJson fail: RightBtmY is null");
				return -1;
			}
			iNonMotor.RightBtmY = rightBtmY.asInt();
		} else {
			if (!rightBtmY.isNull() && rightBtmY.isInt()) {
				iNonMotor.RightBtmY = rightBtmY.asInt();
			}
		}

		//位置标记时间
		Json::Value MarkTime = NonMotorValue[i]["MarkTime"];
		if(!MarkTime.isNull() && MarkTime.isString()) {
			strncpy(iNonMotor.MarkTime, MarkTime.asString().c_str(), sizeof(iNonMotor.MarkTime));
		}

		//车辆出现时间
		Json::Value AppearTime = NonMotorValue[i]["AppearTime"];
		if(!AppearTime.isNull() && AppearTime.isString()) {
			strncpy(iNonMotor.AppearTime, AppearTime.asString().c_str(), sizeof(iNonMotor.AppearTime));
		}

		//车辆消失时间
		Json::Value DisappearTime = NonMotorValue[i]["DisappearTime"];
		if(!DisappearTime.isNull() && DisappearTime.isString()) {
			strncpy(iNonMotor.DisappearTime, DisappearTime.asString().c_str(), sizeof(iNonMotor.DisappearTime));
		}

		//有无车牌
		Json::Value HasPlate = NonMotorValue[i]["HasPlate"];
		if(!HasPlate.isNull() && HasPlate.isBool()) {
			iNonMotor.HasPlate = HasPlate.asBool();
		}

		//号牌种类
		Json::Value PlateClass = NonMotorValue[i]["PlateClass"];
		if(!PlateClass.isNull() && PlateClass.isString()) {
			sscanf(PlateClass.asString().c_str(), "%d", &iNonMotor.PlateClass);
		}

		//车牌颜色
		Json::Value PlateColor = NonMotorValue[i]["PlateColor"];
		if(!PlateColor.isNull() && PlateColor.isString()) {
			sscanf(PlateColor.asString().c_str(), "%d", &iNonMotor.PlateColor);
		}

		//车牌号
		Json::Value PlateNo = NonMotorValue[i]["PlateNo"];
		if(!PlateNo.isNull() && PlateNo.isString()) {
			strncpy(iNonMotor.PlateNo, PlateNo.asString().c_str(), sizeof(iNonMotor.PlateNo));
		}

		//挂车牌号
		Json::Value PlateNoAttach = NonMotorValue[i]["PlateNoAttach"];
		if(!PlateNoAttach.isNull() && PlateNoAttach.isString()) {
			strncpy(iNonMotor.PlateNoAttach, PlateNoAttach.asString().c_str(), sizeof(iNonMotor.PlateNoAttach));
		}

		//车牌描述
		Json::Value PlateDescribe = NonMotorValue[i]["PlateDescribe"];
		if(!PlateDescribe.isNull() && PlateDescribe.isString()) {
			strncpy(iNonMotor.PlateDescribe, PlateDescribe.asString().c_str(), sizeof(iNonMotor.PlateDescribe));
		}

		//是否套牌
		Json::Value IsDecked = NonMotorValue[i]["IsDecked"];
		if(!IsDecked.isNull() && IsDecked.isBool()) {
			iNonMotor.IsDecked = IsDecked.asBool();
		}

		//是否涂改
		Json::Value IsAltered = NonMotorValue[i]["IsAltered"];
		if(!IsAltered.isNull() && IsAltered.isBool()) {
			iNonMotor.IsAltered = IsAltered.asBool();
		}

		//是否遮挡
		Json::Value IsCovered = NonMotorValue[i]["IsCovered"];
		if(!IsCovered.isNull() && IsCovered.isBool()) {
			iNonMotor.IsCovered = IsCovered.asBool();
		}

		//行驶速度
		Json::Value Speed = NonMotorValue[i]["Speed"];
		if(!Speed.isNull() && Speed.isDouble()) {
			iNonMotor.Speed = Speed.asDouble();
		}

		//行驶状态代码
		Json::Value DrivingStatusCode = NonMotorValue[i]["DrivingStatusCode"];
		if(!DrivingStatusCode.isNull() && DrivingStatusCode.isString()) {
			sscanf(DrivingStatusCode.asString().c_str(), "%d", &iNonMotor.DrivingStatusCode);
		}

		//车辆使用性质 代码
		Json::Value UsingPropertiesCode = NonMotorValue[i]["UsingPropertiesCode"];
		if(!UsingPropertiesCode.isNull() && UsingPropertiesCode.isString()) {
			sscanf(UsingPropertiesCode.asString().c_str(), "%d", &iNonMotor.UsingPropertiesCode);
		}

		//车辆品牌
		Json::Value VehicleBrand = NonMotorValue[i]["VehicleBrand"];
		if(!VehicleBrand.isNull() && VehicleBrand.isString()) {
			strncpy(iNonMotor.VehicleBrand, VehicleBrand.asString().c_str(), sizeof(iNonMotor.VehicleBrand));
		}

		//车辆款型
		Json::Value VehicleType = NonMotorValue[i]["VehicleType"];
		if(!VehicleType.isNull() && VehicleType.isString()) {
			strncpy(iNonMotor.VehicleType, VehicleType.asString().c_str(), sizeof(iNonMotor.VehicleType));
		}

		//车辆长度
		Json::Value VehicleLength = NonMotorValue[i]["VehicleLength"];
		if(!VehicleLength.isNull() && VehicleLength.isInt()) {
			iNonMotor.VehicleLength = VehicleLength.asInt();
		}

		//车辆宽度
		Json::Value VehicleWidth = NonMotorValue[i]["VehicleWidth"];
		if(!VehicleWidth.isNull() && VehicleWidth.isInt()) {
			iNonMotor.VehicleWidth = VehicleWidth.asInt();
		}

		//车辆高度
		Json::Value VehicleHeight = NonMotorValue[i]["VehicleHeight"];
		if(!VehicleHeight.isNull() && VehicleHeight.isInt()) {
			iNonMotor.VehicleHeight = VehicleHeight.asInt();
		}

		//车辆颜色
		Json::Value VehicleColor = NonMotorValue[i]["VehicleColor"];
		if(!VehicleColor.isNull() && VehicleColor.isString()) {
			sscanf(VehicleColor.asString().c_str(), "%d", &iNonMotor.VehicleColor);
		}

		//车前盖
		Json::Value VehicleHood = NonMotorValue[i]["VehicleHood"];
		if(!VehicleHood.isNull() && VehicleHood.isString()) {
			strncpy(iNonMotor.VehicleHood, VehicleHood.asString().c_str(), sizeof(iNonMotor.VehicleHood));
		}

		//车后盖
		Json::Value VehicleTrunk = NonMotorValue[i]["VehicleTrunk"];
		if(!VehicleTrunk.isNull() && VehicleTrunk.isString()) {
			strncpy(iNonMotor.VehicleTrunk, VehicleTrunk.asString().c_str(), sizeof(iNonMotor.VehicleTrunk));
		}

		//车轮
		Json::Value VehicleWheel = NonMotorValue[i]["VehicleWheel"];
		if(!VehicleWheel.isNull() && VehicleWheel.isString()) {
			strncpy(iNonMotor.VehicleWheel, VehicleWheel.asString().c_str(), sizeof(iNonMotor.VehicleWheel));
		}

		//车轮印花纹
		Json::Value WheelPrintedPattern = NonMotorValue[i]["WheelPrintedPattern"];
		if(!WheelPrintedPattern.isNull() && WheelPrintedPattern.isString()) {
			strncpy(iNonMotor.WheelPrintedPattern, WheelPrintedPattern.asString().c_str(), sizeof(iNonMotor.WheelPrintedPattern));
		}

		//车窗
		Json::Value VehicleWindow = NonMotorValue[i]["VehicleWindow"];
		if(!VehicleWindow.isNull() && VehicleWindow.isString()) {
			strncpy(iNonMotor.VehicleWindow, VehicleWindow.asString().c_str(), sizeof(iNonMotor.VehicleWindow));
		}

		//车顶
		Json::Value VehicleRoof = NonMotorValue[i]["VehicleRoof"];
		if(!VehicleRoof.isNull() && VehicleRoof.isString()) {
			strncpy(iNonMotor.VehicleRoof, VehicleRoof.asString().c_str(), sizeof(iNonMotor.VehicleRoof));
		}

		//车门
		Json::Value VehicleDoor = NonMotorValue[i]["VehicleDoor"];
		if(!VehicleDoor.isNull() && VehicleDoor.isString()) {
			strncpy(iNonMotor.VehicleDoor, VehicleDoor.asString().c_str(), sizeof(iNonMotor.VehicleDoor));
		}

		//车侧
		Json::Value SideOfVehicle = NonMotorValue[i]["SideOfVehicle"];
		if(!SideOfVehicle.isNull() && SideOfVehicle.isString()) {
			strncpy(iNonMotor.SideOfVehicle, SideOfVehicle.asString().c_str(), sizeof(iNonMotor.SideOfVehicle));
		}

		//车厢
		Json::Value CarOfVehicle = NonMotorValue[i]["CarOfVehicle"];
		if(!CarOfVehicle.isNull() && CarOfVehicle.isString()) {
			strncpy(iNonMotor.CarOfVehicle, CarOfVehicle.asString().c_str(), sizeof(iNonMotor.CarOfVehicle));
		}

		//后视镜
		Json::Value RearviewMirror = NonMotorValue[i]["RearviewMirror"];
		if(!RearviewMirror.isNull() && RearviewMirror.isString()) {
			strncpy(iNonMotor.RearviewMirror, RearviewMirror.asString().c_str(), sizeof(iNonMotor.RearviewMirror));
		}

		//底盘
		Json::Value VehicleChassis = NonMotorValue[i]["VehicleChassis"];
		if(!VehicleChassis.isNull() && VehicleChassis.isString()) {
			strncpy(iNonMotor.VehicleChassis, VehicleChassis.asString().c_str(), sizeof(iNonMotor.VehicleChassis));
		}

		//遮挡
		Json::Value VehicleShielding = NonMotorValue[i]["VehicleShielding"];
		if(!VehicleShielding.isNull() && VehicleShielding.isString()) {
			strncpy(iNonMotor.VehicleShielding, VehicleShielding.asString().c_str(), sizeof(iNonMotor.VehicleShielding));
		}

		//贴膜颜色
		Json::Value FilmColor = NonMotorValue[i]["FilmColor"];
		if(!FilmColor.isNull() && FilmColor.isInt()) {
			iNonMotor.FilmColor = VehicleShielding.asInt();
		}

		//改装标志
		Json::Value IsModified = NonMotorValue[i]["IsModified"];
		if(!IsModified.isNull() && IsModified.isInt()) {
			iNonMotor.IsModified = IsModified.asInt();
		}

		//图像列表
		Json::Value SubImageList = NonMotorValue[i]["SubImageList"];
		if(!SubImageList.isNull()) {
			std::string strjson = NonMotorValue[i].toStyledString();
			if(UnPackSubImageListJson(strjson, iNonMotor.SubImageList) == -1) {
				return -1;
			}
		}

		NonMotorList.push_back(iNonMotor);
	}
	return 0;
}