#ifndef __PFSocket_H__
#define __PFSocket_H__

class CPFSocket
{
public:
	static int Startup(void);
	static int Cleanup(void);

private:
	CPFSocket(void);
	~CPFSocket(void);
};

#endif //__PFSocket_H__
