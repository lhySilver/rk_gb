#ifndef _CRYPTO_H_
#define _CRYPTO_H_


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

//buf 大小是16的整数倍，不够16的整数倍，要填充0
//转换后的数据也放在同样的buf里面，转换前和
//转换后的大小是一样的大小
int _DG_AES_encrypt(char *buf,int size);
int _DG_AES_decrypt(char *buf,int size);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */


#endif
