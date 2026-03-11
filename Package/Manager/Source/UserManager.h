#ifndef __INTER_MANAGER_USERMANAGER_H__
#define __INTER_MANAGER_USERMANAGER_H__

#include "Manager/User.h"
#include "Manager/UserManager.h"
#include "Infra/Singleton.h"
#include "Infra/Mutex.h"
#include "Infra/Timer.h"
#include <vector>
#include <string>
#include <map>

class CFile;

class CUserManager : public IUserManager
{
	PATTERN_SINGLETON_DECLARE(CUserManager);

public:

	/// 用户外部配置接口，必须在用户管理启动前调用
	/// \param [in] versionMajor 主版本权限
	/// \param [in] versionMinor 次版本权限
	/// \param [in] config1 用户配置文件
	/// \param [in] config2 备份的用户配置文件
	/// \param [in] customConfig 用户自定义的配置文件
	/// \param [in] adminAuthList 管理用户权限列表
	/// \param [in] adminAuthNum 管理用户权限数
	/// \param [in] userAuthList 普通用户权限数量
	/// \param [in] userAuthList 普通用户权限数量
	static void config(const Json::Value &adminAuthList, const Json::Value &userAuthList, const std::string &p1, const std::string &p2, const std::string &customPath); 

	/// 启动用户管理
	bool start();

	/// 获取整个权限列表
	const Json::Value& getAuthorityList() const;

	/// 增加组
	/// \param [in] group 要增加的组, 参考<<AuthorityList.doc>>
	bool addGroup(const Json::Value &group);

	/// 删除组
	/// \param [in] name 组名
	bool delGroup(const std::string &name);

	/// 修改组
	/// \param [in] name 要修改的组名
	/// \param [in] newGroup 新组信息, 参考<<AuthorityList.doc>>
	bool modGroup(const std::string &name, const Json::Value &newGroup);

	/// 根据组名获取组信息
	/// \param [in] name 要获取组信息的组名
	/// \param [out] group 用户存放组信息,参考<<AuthorityList.doc>>
	bool getGroup(const std::string &name, Json::Value &group);

	/// 获取所有组信息
	/// \return 所有组的信息, 参考<<AuthorityList.doc>>
	bool getGroupAll(Json::Value &groups);

	/// 添加用户
	/// \param [in] curUser 要添加的用户 
	/// \param [in] flag 是否加密密码，默认加密
	bool addUser(const Json::Value &curUser, const std::string &flag = "MD5");

	/// 根据用户名删除用户
	/// \param [in] name 要删除的用户名
	bool delUser(const std::string &name);

	/// 根据用户名修改信息
	/// \param [in] name 要修改的用户名
	/// \param [in] user 要修改的用户信息, 参见<<AuthorityList.doc>>
	/// \param [in] flag 密码加密标识
	bool modUser(const std::string &name, const Json::Value &user, const std::string & flag = "MD5");

	/// 根据用户id修改用户密码
	/// \param [in] name 要修改的用户名
	/// \param [in] passwd 新的密码
	/// \param [in] flag 密码是否加密存放
	bool modPassword(const std::string &name, const std::string &passwd, const std::string & flag = "MD5");

	/// 根据用户名获取用户信息
	/// \param [in] name 要获取用户信息的用户名
	/// \param [out] user 存放用户信息, 参见<<AuthorityList.doc>>
	bool getUser(const std::string &name, Json::Value &user);

	/// 获取所有用户, 参见<<AuthorityList.doc>>
	/// \return 所有用户的信息
	bool getUserAll(Json::Value &users);

	/// 还原默认
	bool setDefault();

	///还原预览和回放通道权限
	bool setDefaultChnsAuthority(int devChns);

	/// 保存所有信息
	bool saveFile();

	/// 判断用户是否是保留用户
	bool isReservedUser(const std::string& userName);

	/// 判断用户是否是默认用户
	bool isDefaultUser(const std::string& userName);

	/// 判断所提交的密码是否合法
	/// \param [in] name 用户名
	/// \param [in] passwd 用户密码
	/// \param [in] flag 密码保存方式，明文或者秘文
	/// \param [in] superPwdValid 暂时用作保留
	bool isPasswdValid(const std::string &name, const std::string &passwd, const std::string &flag = "MD5",  bool superPwdValid = false);

	/// 得到所有活动用户的ID列表
	std::vector<int> getActiveUserIdList();

	/// 得到活动用户
	IUser* getActiveUser(int id);

	/// 根据用户名得到得到活动用户
	IUser* getActiveUser(const std::string &userName);

	/// 释放活动用户，必须和GetActiveUser成对调用，并保证间隔尽量短
	void releaseActiveUser(IUser* user);

	/// 是否是合法的地址
	bool isAllowedAddress(const std::string &address);

	/// 设置错误编号
	void setLastError(int error);

	/// 获取错误编号
	int getLastError() const;

	/// 锁定帐户
	bool setLock(const std::string &name, int second);

	/// 是否为锁定帐户
	bool isLockUser(const std::string &name);

	/// 得到默认用户名
	std::string getDefaultUser() const;

	/// 得到默认用户密码
	std::string getDefaultPassword() const;
public:

	/// 得到锁定周期，ms为单位
	int getLockPeriod() const;

	/// 返回密码错误时的登陆次数
	int getTryLoginTimes() const;

	/// 加入活动用户
	bool addActiveUser(IUser* user);

	/// 删除活动用户
	bool removeActiveUser(IUser* user);

	/// 该用户名是否已经登陆
	bool isUserLogined(const std::string& userName);

	/// 是否为共享用户
	bool isUserSharable(const std::string& userName);

	/// 指定组是否在使用
	bool isGroupInUse(const std::string &userName);

	/// 根据用户名查找用户的位置索引
	bool hasAuthorityOf(const std::string &userName, const char* auth);

	/// 根据用户名查找用户的位置索引
	int findUser(const std::string &userName);

	/// 根据用户名查找用户的位置索引
	int findGroup(const std::string &groupName);

	/// 控制台接口
	int onConsole(int argc, char ** argv);

private:
	void onLockUser(uint arg);

	bool loadFile(const char *filePath, std::string &input);

	bool IsChildVector(const Json::Value &ChildVec, const Json::Value &Vec);
	
	void initAdminAuthoritylist();

	bool checkName(std::string strUserName, bool bChange = false);

private:
	typedef std::map<int, IUser *> UserMap;
	typedef std::map<std::string, int> LockUserMap;
	int				m_lockPeriod;
	int				m_tryLoginTime;
	CMutex			m_mutex;
	UserMap			m_activeUsers;
	LockUserMap		m_lockUsers;
	Json::Value	    m_configAll;		// Json配置总表
	int				m_errorNo;
	std::string		m_stream;			// 配置字符串流，由Json配置转换而来，最终被写到文件
	CFile			*m_fileConfig;
	CTimer		m_timerLockUser;
	CMutex		m_mutexLockMap;
	static Json::Value sm_adminAuthorityList;
	static Json::Value sm_userAurthorityList;
	static std::string sm_firstFilePath;
	static std::string sm_secondFilePath;
	static std::string sm_customFilePath;
};

#endif
