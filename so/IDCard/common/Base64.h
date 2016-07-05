#ifndef    _HEADER_BASE64_
#define    _HEADER_BASE64_

#ifdef __cplusplus
extern "C" {
#endif


//BASE64 functions
int GetIndexFromBase64Table( unsigned char bVal );

//BASE64 functions
int EncryptBase64( unsigned char* plainText,  int nPlainTextLen,  unsigned char* chiperText, int nChiperTextLen );
int DecryptBase64( unsigned char* chiperText, int nChiperTextLen, unsigned char* plainText,  int nPlainTextLen );


#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#endif