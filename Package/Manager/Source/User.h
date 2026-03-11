#ifndef __INTER_MANAGER_USER_H__
#define __INTER_MANAGER_USER_H__

#include "Infra/Singleton.h"
#include "Types/Types.h"
#include <string>
#include "Manager/User.h"

class IUserManager;

class CUser : public IUser
{
public:
	/// 本地用户
	static CUser *getLocalUser();

	/// 构造函数
	CUser();

	/// 析构函数
	~CUser();

	/// 当前用户是否登陆成功
	bool valid();

	/// 用户登陆
	/// \param [in] name 要登陆的用户名
	/// \param [in] passwd 用户的登陆密码
	/// \param [in] loginType 客户端类型
	/// \param [in] flag 密码保存方式，密文方式或者明文方式
	/// \param [in] address 客户端的地址，比如192.168.0.2 而本地没有地址
	/// \return 登陆成功或者失败
	bool login(const std::string &name, const std::string &passwd, const std::string &loginType, const std::string &flag = "MD5",  const std::string &address = "");

	/// 用户注销
	bool logout();

	/// 获取登陆用户名
	const std::string &getName() const;

	/// 获取客户端类型
	const std::string &getType() const;

	/// 获取客户端地址
	const std::string &GetAddress() const;

	/// 获取有效的用户ID
	int getId() const;

	/// 检查权限是否有效
	bool isValidAuthority(const char *auth) const;

	/// 获取错误号
	int getErrorNo();

	/// 默认用户登陆
	bool loginDefault();

private:

	// 拷贝构造函数,禁止拷贝
	CUser(CUser &);

	// 清楚状态
	void clear();

private:
	IUserManager	*m_userManager;
	bool		m_locked;		// 用户已经锁定
	ulong		m_lockTime;		// 锁定时间，ms为单位
	int			m_tryLockNum;	// 登陆次数
	int			m_userId;		// 用户ID，是用户存在的唯一标示
	bool		m_valid;
	int			m_errorNo;		// 错误号
	std::string m_userName;	
	std::string m_userPasswd;	
	std::string m_iloginType;
	std::string m_passwordFlag;
	std::string m_loginAddress;
	static int sm_maxUserId;
};

#endif
