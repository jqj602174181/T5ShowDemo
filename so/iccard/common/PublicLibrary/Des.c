#include "Des.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>


#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif



//BASE64 params
BYTE m_TabBase64[ 64 ];

//DES params
BYTE K[ 17 ][ 48 ];

void IniBase64Table( )
{
	BYTE table[ 64 ] = { 
		'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
		'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
		'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
		'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
		'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
		'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
		'w', 'x', 'y', 'z', '0', '1', '2', '3', 
		'4', '5', '6', '7', '8', '9', '+', '/'
	};

	memcpy( m_TabBase64, table, sizeof(m_TabBase64) );
}


int GetIndexFromBase64Table( BYTE bVal )
{
	int i = 0;
	if( bVal == '=' )
	{
		return 0;
	}

	for(  i =0; i < 64; i++ )
	{
		if( bVal == m_TabBase64[ i ] )
		{
			return i;
		}
	}

	return -1;
}



int EncryptBase64( BYTE* plainText, int nPlainTextLen, BYTE* chiperText, int nChiperTextLen )
{
	int nPlainTextCurLen = 0;
	int nChiperTextSize = 0;

	if( nChiperTextLen < ( nPlainTextLen + 2 ) / 3 * 4 )
	{
		return -1;
	}

	while( nPlainTextCurLen < nPlainTextLen )
	{
		if( nPlainTextLen - nPlainTextCurLen > 2 )
		{
			chiperText[ nChiperTextSize++ ] = m_TabBase64[ plainText[ nPlainTextCurLen ] >> 2 ];
			chiperText[ nChiperTextSize++ ] = m_TabBase64[( plainText[ nPlainTextCurLen ] << 4 | plainText[ nPlainTextCurLen + 1 ] >> 4 ) & 0x3F ];
			chiperText[ nChiperTextSize++ ] = m_TabBase64[( plainText[ nPlainTextCurLen + 1 ] << 2 | plainText[ nPlainTextCurLen + 2 ] >> 6 ) & 0x3F ];
			chiperText[ nChiperTextSize++ ] = m_TabBase64[ plainText[ nPlainTextCurLen + 2 ] & 0x3F ];
		}
		else if( nPlainTextLen - nPlainTextCurLen > 1 )
		{
			chiperText[ nChiperTextSize++ ] = m_TabBase64[ plainText[ nPlainTextCurLen ] >> 2 ];
			chiperText[ nChiperTextSize++ ] = m_TabBase64[( plainText[ nPlainTextCurLen ] << 4 | plainText[ nPlainTextCurLen + 1 ] >> 4 ) & 0x3F ];
			chiperText[ nChiperTextSize++ ] = m_TabBase64[( plainText[ nPlainTextCurLen + 1 ] << 2  ) & 0x3F ];
			chiperText[ nChiperTextSize++ ] = '=';
		}
		else
		{
			chiperText[ nChiperTextSize++ ] = m_TabBase64[ plainText[ nPlainTextCurLen ] >> 2 ];
			chiperText[ nChiperTextSize++ ] = m_TabBase64[ ( plainText[ nPlainTextCurLen ] << 4 ) & 0x3F ];
			chiperText[ nChiperTextSize++ ] = '=';
			chiperText[ nChiperTextSize++ ] = '=';
		}

		nPlainTextCurLen += 3;
	}

	return nChiperTextSize;
}


int DecryptBase64(  BYTE* chiperText, int nChiperTextLen, BYTE* plainText, int nPlainTextLen )
{
	int nPlainSize = 0;
	int i = 0, lNum = 0, rNum = 0;

	int nChiperCurLen = 0;
	int rmovs[ 3 ] = { 4, 2, 0 };
	int lmovs[ 3 ] = { 2, 4, 6 };
	
	while( nChiperTextLen - nChiperCurLen > 3 )
	{
		for(  i = 0; i < 3; i++ )
		{
			if( chiperText[ nChiperCurLen ] == '=' || chiperText[ nChiperCurLen + 1 ] == '=' )
			{
				return nPlainSize;
			}
			if( nPlainTextLen < nPlainSize )
			{
				return -1;
			}

			lNum = GetIndexFromBase64Table( chiperText[ nChiperCurLen++ ] );
			rNum = GetIndexFromBase64Table( chiperText[ nChiperCurLen ] );
			plainText[ nPlainSize++ ] =  lNum << lmovs[ i ] |  rNum >> rmovs[ i ];
		}

		nChiperCurLen++;
	}

	return nPlainSize;
}


void expand0( const BYTE *in, BYTE *out )
{
	int i = 0;
	int j = 0;
	for(  i = 0; i < 8; i++ )
	{
		int divide = 0x80;
		for( j = 0; j < 8; j++)
		{
			*out++ = ( in[ i ] / divide ) & 1;
			divide /= 2;
		}
	}
}


void setkeystar( BYTE bits[64] )
{
	BYTE C[ 17 ][ 28 ] = { 0 };
	BYTE D[ 17 ][ 28 ] = { 0 };
	int i = 0;
	int j = 0;
	int pc_1_c[28] = { 57, 49, 41, 33, 25, 17, 9, 
		1,  58, 50, 42, 34, 26,18,
		10, 2,  59, 51, 43, 35,27,
		19, 11, 3,  60, 52, 44,36 };

	int pc_1_d[28] = { 63, 55, 47, 39, 31, 23, 15, 7, 
		62, 54, 46, 38, 30, 22, 14, 6, 
		61, 53, 45, 37, 29, 21, 13, 5, 
		28, 20, 12, 4 };

	int ls_count[16] = { 1, 1, 2, 2, 2, 2, 2, 2,
		1, 2, 2, 2, 2, 2, 2, 1 };

	for( i = 0; i < 28; i++ )
	{
		C[ 0 ][ i ] = bits[ pc_1_c[i]-1 ];
	}
	for (i=0;i<28;i++)
	{
		D[ 0 ][ i ] = bits[ pc_1_d[i]-1 ];
	}
	for (j=0;j<16;j++)
	{
		LS( C[j], C[j+1], ls_count[j] );
		LS( D[j], D[j+1], ls_count[j] );
		son( C[j+1], D[j+1], K[j+1] );
	}
}



void LS( BYTE *bits, BYTE *buffer, int count )
{
	int i = 0;
	for( i = 0; i < 28; i++ )
	{
		buffer[ i ] = bits[ (i+count) % 28 ];
	}
}


void son( BYTE *cc, BYTE *dd, BYTE *kk )
{
	int i = 0;
	char buffer[ 56 ] = { 0 };
	int pc_2[ 48 ] = {  14, 17, 11, 24, 1,  5,
		3,  28, 15, 6,  21, 10,
		23, 19, 12, 4,  26, 8,
		16, 7,  27, 20, 13, 2,
		41, 52, 31, 37, 47, 55,
		30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53,
		46, 42, 50, 36, 29, 32 };


	for( i = 0; i < 28; i++ )
	{
		buffer[ i ] = *cc++;
	}

	for( i = 28; i < 56; i++ )
	{
		buffer[ i ] = *dd++;
	}

	for( i = 0; i < 48; i++ )
	{
		*kk++ = buffer[ pc_2[ i ] - 1 ];
	}
}


void s_box(char *aa,char *bb)
{
	int i,j,k,m;
	int y,z;
	char ss[8];

	int SSS[16][4][16]={
		14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
		0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,/* err on */
		4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
		15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13,

		15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
		3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
		0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
		13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9,

		10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
		13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
		13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
		1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12,

		7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
		13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
		10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
		3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14, /* err on */

		2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
		14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6, /* err on */
		4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
		11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3,

		12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
		10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
		9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
		4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13,

		4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
		13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
		1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
		6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12,

		13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
		1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
		7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
		2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11};
		m=0;
		for (i=0;i<8;i++)
		{
			j=6*i;
			y=aa[j]*2+aa[j+5];
			z=aa[j+1]*8+aa[j+2]*4+aa[j+3]*2+aa[j+4];
			ss[i]=SSS[i][y][z];
			y=0x08;
			for (k=0;k<4;k++)
			{
				bb[m++]=(ss[i]/y)&1;
				y/=2;
			}
		}
}

void F( int n, BYTE *ll, BYTE *rr, BYTE *LL, BYTE *RR )
{
	int i = 0 ;
	char buffer[ 64 ] = { 0 };
	char tmp[ 64 ] = { 0 };
	int e_r[ 48 ] ={ 32, 1,  2,  3,  4,  5,  4,  5,  
		6,  7,  8,  9,  8,  9,  10, 11, 
		12, 13, 12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21, 20 ,21, 
		22, 23, 24, 25, 24, 25, 26, 27, 
		28, 29, 28, 29, 30, 31, 32, 1 };

	int P[ 32 ] = { 16, 7,  20, 21, 29, 12, 28, 17,
		1,  15, 23, 26, 5,  18, 31, 10,
		2,  8,  24, 14, 32, 27, 3,  9,
		19, 13, 30, 6,  22, 11, 4,  25 };

	for( i = 0; i < 48; i++ )
	{
		buffer[ i ] = rr[ e_r[ i ] - 1 ];
	}

	for( i = 0; i < 48; i++ )
	{
		buffer[ i ] = ( buffer[ i ] + K[ n ][ i ] ) & 1;
	}

	s_box( buffer, tmp );

	for( i = 0; i < 32; i++ )
	{
		buffer[ i ] = tmp[ P[ i ] - 1 ];
	}

	for( i = 0; i < 32; i++ )
	{
		RR[ i ] = ( buffer[ i ] + ll[ i ] ) & 1;
	}

	for( i = 0; i < 32; i++ )
	{
		LL[ i ] = rr[ i ];
	}
}



void encrypt0( const BYTE *text, BYTE *mtext )
{
	int i = 0, j = 0;
	BYTE ll[ 64 ]  = { 0 };
	BYTE rr[ 64 ]  = { 0 };
	BYTE LL[ 64 ]  = { 0 };
	BYTE RR[ 64 ]  = { 0 };
	BYTE tmp[ 64 ] = { 0 };

	ip( text, ll, rr );

	for(  i = 1; i < 17; i++ )
	{
		F( i, ll, rr, LL, RR );

		for( j = 0; j < 32; j++ )
		{
			ll[ j ] = LL[ j ];
			rr[ j ] = RR[ j ];
		}
	}

	_ip( tmp, rr, ll );
	compress0( tmp, mtext );
}


void discrypt0( const BYTE *mtext, BYTE *text )
{
	int i = 0, j = 0;
	BYTE ll[ 64 ]  = { 0 };
	BYTE rr[ 64 ]  = { 0 };
	BYTE LL[ 64 ]  = { 0 };
	BYTE RR[ 64 ]  = { 0 };
	BYTE tmp[ 64 ] = { 0 };

	ip( mtext, ll, rr );

	for( i = 16; i > 0; i-- )
	{
		F( i, ll, rr, LL, RR );

		for( j = 0; j < 32; j++ )
		{
			ll[ j ] = LL[ j ];
			rr[ j ] = RR[ j ];
		}
	}

	_ip( tmp, rr, ll );
	compress0( tmp, text );
}


void compress0( BYTE *out, BYTE *in )
{
	int i = 0, j = 0;
	int times = 0;
	for(  i = 0; i < 8; i++ )
	{
		times = 0x80;
		in[ i ] = 0;
		for(  j = 0; j < 8; j++ )
		{
			in[ i ] += (*out++) * times;
			times /= 2;
		}
	}
}


void ip( const BYTE *text, BYTE *ll, BYTE *rr )
{
	int i = 0 ;
	BYTE buffer[ 64 ] = { 0 };
	int ip_tab[ 64 ] = { 58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9,  1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7 };

	expand0( text, buffer );

	for( i = 0; i < 32; i++ )
	{
		ll[ i ] = buffer[ ip_tab[i]-1 ];
	}

	for ( i = 0; i < 32; i++ )
	{
		rr[ i ] = buffer[ ip_tab[i+32]-1 ];
	}
}


void  _ip( BYTE *text, BYTE *ll, BYTE *rr )
{
	int i = 0;
	char tmp[ 64 ] = { 0 };
	int _ip_tab[ 64 ] = { 40, 8, 48, 16, 56, 24, 64, 32,
		39, 7, 47, 15, 55, 23, 63, 31,
		38, 6, 46, 14, 54, 22, 62, 30,
		37, 5, 45, 13, 53, 21, 61, 29,
		36, 4, 44, 12, 52, 20, 60, 28,
		35, 3, 43, 11, 51, 19, 59, 27,
		34, 2, 42, 10, 50, 18, 58, 26,
		33, 1, 41, 9,  49, 17, 57, 25 };

	for( i = 0; i < 32; i++ )
	{
		tmp[ i ] = ll[ i ];
	}

	for( i = 32; i < 64; i++ )
	{
		tmp[ i ] = rr[ i - 32 ];
	}

	for( i = 0; i < 64; i++ )
	{
		text[ i ] = tmp[ _ip_tab[i]-1 ];
	}
}


void EnDES( const BYTE* key, const BYTE* plainText, BYTE* chiperText )
{
	BYTE tmp[ 64 ] = { 0 };
	expand0( key,tmp );
	setkeystar( tmp );
	encrypt0( plainText, chiperText );
}


void DeDES( const BYTE* key, const BYTE* chiperText, BYTE* plainText )
{
	BYTE tmp[ 64 ] = { 0 };
	expand0( key, tmp );
	setkeystar( tmp );
	discrypt0( chiperText, plainText );
}


void EncryptDES_BYTE( const BYTE* key, const int nKeyLen, const BYTE* plainText,  BYTE* byteChiperText )
{
	if ( nKeyLen == 8 )
	{
		EnDES( key, plainText, byteChiperText );
	}
	else if ( nKeyLen == 16 )//16
	{
		BYTE key1[8];
		BYTE key2[8];
		BYTE tempText1[8];
		BYTE tempText2[8];
		memset( key1, 0, sizeof(key1) );
		memset( key2, 0, sizeof(key2) );

		memcpy( key1, key, min(8,nKeyLen) );
		memcpy( key2, &key[8], min(8,nKeyLen-8) );

		memset( tempText1, 0, sizeof(tempText1) );
		memset( tempText2, 0, sizeof(tempText2) );

		EnDES( key1, plainText, tempText1 );
		DeDES( key2, tempText1, tempText2 );
		EnDES( key1, tempText2, byteChiperText );
	}
	else if ( nKeyLen == 24 )
	{
		BYTE key1[8];
		BYTE key2[8];
		BYTE key3[8];

		BYTE tempText1[8];
		BYTE tempText2[8];
		memset( key1, 0, sizeof(key1) );
		memset( key2, 0, sizeof(key2) );
		memset( key3, 0, sizeof(key3) );

		memcpy( key1, key,      min(8,nKeyLen) );
		memcpy( key2, &key[8],  min(8,nKeyLen-8) );
		memcpy( key3, &key[16], min(8,nKeyLen-16) );

		memset( tempText1, 0, sizeof(tempText1) );
		memset( tempText2, 0, sizeof(tempText2) );

		EnDES( key1, plainText, tempText1 );
		DeDES( key2, tempText1, tempText2 );
		EnDES( key3, tempText2, byteChiperText );
	}
}


void DecryptDES_BYTE( const BYTE* key, const int nKeyLen, const BYTE* byteChiperText, BYTE* plainText )
{
	if ( nKeyLen == 8 )
	{
		DeDES( key, byteChiperText, plainText );
	}
	else if ( nKeyLen == 16 )//16
	{
		BYTE key1[8];
		BYTE key2[8];
		BYTE tempText1[8];
		BYTE tempText2[8];
		memset( key1, 0, sizeof(key1) );
		memset( key2, 0, sizeof(key2) );

		memcpy( key1, key, min(8,nKeyLen) );
		memcpy( key2, &key[8], min(8,nKeyLen-8) );

		memset( tempText1, 0, sizeof(tempText1) );
		memset( tempText2, 0, sizeof(tempText2) );

		DeDES( key1, byteChiperText, tempText1 );
		EnDES( key2, tempText1, tempText2 );
		DeDES( key1, tempText2, plainText );
	}
	else if ( nKeyLen == 24 )
	{
		BYTE key1[8];
		BYTE key2[8];
		BYTE key3[8];

		BYTE tempText1[8];
		BYTE tempText2[8];
		memset( key1, 0, sizeof(key1) );
		memset( key2, 0, sizeof(key2) );
		memset( key3, 0, sizeof(key3) );

		memcpy( key1, key, min(8,nKeyLen) );
		memcpy( key2, &key[8], min(8,nKeyLen-8) );
		memcpy( key3, &key[16], min(8,nKeyLen-16) );

		memset( tempText1, 0, sizeof(tempText1) );
		memset( tempText2, 0, sizeof(tempText2) );

		DeDES( key3, byteChiperText, tempText1 );
		EnDES( key2, tempText1, tempText2 );
		DeDES( key1, tempText2, plainText );
	}
}


void EncryptDES_HEX( const BYTE* key, const int nKeyLen, const BYTE* plainText, char* hexChiperText )
{
	BYTE byteChiperText[ 64 ];

	memset( byteChiperText, 0, 64 );

	EncryptDES_BYTE( key, nKeyLen, plainText, byteChiperText );

	ByteToHex( byteChiperText, strlen((char*)byteChiperText), hexChiperText );
}


void DecryptDES_HEX( const BYTE* key, const int nKeyLen, const char* hexChiperText, BYTE* plainText )
{
	BYTE byteChiperText[1024] = {0x00};

	memset( byteChiperText, 0, strlen(hexChiperText)/2 );

	HexToByte( hexChiperText, strlen(hexChiperText), byteChiperText );

	DecryptDES_BYTE( key, nKeyLen , byteChiperText, plainText );

}


int ByteToHex( const BYTE *src, int nSrcLen, char *dst )
{
	const BYTE nibble[ ] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	int  nLen = 0;
	int i = 0;

	for( i = 0; i < nSrcLen; i++ )
	{
		dst[ nLen++ ] = nibble [ src[i] >> 4 ];
		dst[ nLen++ ] = nibble [ src[i] & 0xF ];
	}

	return nLen;
}

void ExDecryptDES_BYTE ( const BYTE* key, const int nKeyLen, const BYTE* hexChiperText, const int nChiperTextLen, BYTE* plainText )
{
	int nTempLen = nChiperTextLen;
	BYTE szTemp[32] = {0};
	int i = 0;
	while(nTempLen > 0)
	{
		memset(szTemp, 0, sizeof(szTemp));
		DecryptDES_BYTE( key, nKeyLen , &hexChiperText[i], szTemp );
		memcpy(&plainText[i], szTemp, 8);
		i += 8;
		nTempLen -= 8;
	}
}

void ExEncryptDES_BYTE ( const BYTE* key, const int nKeyLen, const BYTE* plainText, const int nPlainTextLen, BYTE* hexChiperText )
{
	int nTempLen = nPlainTextLen;
	BYTE szTemp[32] = {0};
	int i = 0;
	while(nTempLen > 0)
	{
		memset(szTemp, 0, sizeof(szTemp));
		EncryptDES_BYTE( key, nKeyLen, &plainText[i], szTemp );
		memcpy(&hexChiperText[i], szTemp, 8);
		i += 8;
		nTempLen -= 8;
	}
}



int HexToByte( const char *src, int nSrcLen, BYTE *dst )
{
	int nLen = 0;
	int i = 0;
	for( i = 0; i < nSrcLen - 1; i += 2 )
	{
		if ( isdigit( src[i] ) )
		{
			dst[ nLen ] = src[ i ] - 0x30;
		}
		else
		{
			dst[ nLen ] = src[ i ] - 0x37;
		}

		dst[ nLen ] <<= 4;

		if ( isdigit( src[i+1] ) )
		{
			dst[ nLen ] += src[ i+1 ] - 0x30;
		}
		else
		{
			dst[ nLen ] += src[ i+1 ] - 0x37;
		}

		nLen++;
	}

	return nLen;
}