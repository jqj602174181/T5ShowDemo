#ifndef    _DES_H_
#define    _DES_H_

typedef unsigned char       BYTE;

	//common functions
	int ByteToHex( const BYTE *src, int nSrcLen, char *dst );
	int HexToByte( const char *src, int nSrcLen, BYTE *dst );

	//BASE64 functions
	int  GetIndexFromBase64Table( BYTE bVal );

	//DES functions
	void setkeystar( BYTE bits[64] );
	void expand0   ( const BYTE *in,    BYTE *out );
	void discrypt0 ( const BYTE *mtext, BYTE *text );
	void encrypt0  ( const BYTE *text,  BYTE *mtext );
	void compress0 ( BYTE *out,   BYTE *in );
	void s_box     ( char *aa,    char *bb );
	void _ip       ( BYTE *text,  BYTE *ll,     BYTE *rr );
	void ip        ( const BYTE *text,  BYTE *ll,     BYTE *rr );
	void LS        ( BYTE *bits,  BYTE *buffer, int count);
	void son       ( BYTE *cc,    BYTE *dd,     BYTE *kk );
	void F         ( int n,       BYTE *ll,     BYTE *rr, BYTE *LL, BYTE *RR);	
	void EnDES( const BYTE* key, const BYTE* plainText,  BYTE* chiperText );
	void DeDES( const BYTE* key, const BYTE* chiperText, BYTE* plainText );


	//BASE64 functions
	void IniBase64Table( );
	int EncryptBase64( BYTE* plainText,  int nPlainTextLen,  BYTE* chiperText, int nChiperTextLen );
	int DecryptBase64( BYTE* chiperText, int nChiperTextLen, BYTE* plainText,  int nPlainTextLen );

	//DES functions

	//算法，密钥、明文和密文均为64位,nKeyLen=8,16,24
	void EncryptDES_BYTE( const BYTE* key, const int nKeyLen, const BYTE* plainText,      BYTE* byteChiperText );
	void DecryptDES_BYTE( const BYTE* key, const int nKeyLen, const BYTE* byteChiperText, BYTE* plainText );
	void EncryptDES_HEX ( const BYTE* key, const int nKeyLen, const BYTE* plainText,      char* hexChiperText );
	void DecryptDES_HEX ( const BYTE* key, const int nKeyLen, const char* hexChiperText,  BYTE* plainText );
	void ExDecryptDES_BYTE ( const BYTE* key, const int nKeyLen, const BYTE* hexChiperText, const int nChiperTextLen, BYTE* plainText );
	void ExEncryptDES_BYTE ( const BYTE* key, const int nKeyLen, const BYTE* plainText, const int nPlainTextLen, BYTE* hexChiperText );


#endif