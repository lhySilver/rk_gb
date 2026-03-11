#ifndef _BASE64_H
#define _BASE64_H

#ifdef __cplusplus
extern "C" {
#endif

void _base64_encode_triple(unsigned char triple[3], char result[4]);
int onvif_base64_encode(unsigned char *source, unsigned int sourcelen, char *target, unsigned int targetlen);
void str_b64decode(char* str);
int base64_decode(const char *source, unsigned char *target, unsigned int targetlen) ;

#ifdef __cplusplus
}
#endif

#endif
