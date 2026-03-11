#ifdef  __cplusplus
extern "C" {
#endif

int DG_encode_g711a(char *a_psrc, char *a_pdst, int in_data_len);
int DG_decode_g711a(char *a_psrc, char *a_pdst, int in_data_len);
int DG_encode_g711u(char *a_psrc, char *a_pdst, int in_data_len);
int DG_decode_g711u(char *a_psrc, char *a_pdst, int in_data_len);

#ifdef  __cplusplus
}
#endif

