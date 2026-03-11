#ifndef __ALIVE_CHECKER_H__
#define __ALIVE_CHECKER_H__

typedef unsigned short ushort;

class CPing
{
public:
	CPing();
	~CPing();

	bool ping(ulong destIP, int time_out = 100);	// time_out: ms

protected:
	bool init();
	ushort getCheckSum(ushort* addr, int len);

private:
	int m_sockfd;
};

#endif


