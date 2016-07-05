#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include "Base64.h"
#include "GlobalDef.h"


//BASE64 params
static unsigned char s_TabBase64[ 64 ] = { 
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3', 
	'4', '5', '6', '7', '8', '9', '+', '/'
};


int GetIndexFromBase64Table( unsigned char bVal )
{
	int i = 0;

	if( bVal == '=' )
	{
		return 0;
	}

	for( i =0; i < 64; i++ )
	{
		if( bVal == s_TabBase64[ i ] )
		{
			return i;
		}
	}

	return -1;
}



int EncryptBase64( unsigned char* plainText, int nPlainTextLen, unsigned char* chiperText, int nChiperTextLen )
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
			chiperText[ nChiperTextSize++ ] = s_TabBase64[ plainText[ nPlainTextCurLen ] >> 2 ];
			chiperText[ nChiperTextSize++ ] = s_TabBase64[( plainText[ nPlainTextCurLen ] << 4 | plainText[ nPlainTextCurLen + 1 ] >> 4 ) & 0x3F ];
			chiperText[ nChiperTextSize++ ] = s_TabBase64[( plainText[ nPlainTextCurLen + 1 ] << 2 | plainText[ nPlainTextCurLen + 2 ] >> 6 ) & 0x3F ];
			chiperText[ nChiperTextSize++ ] = s_TabBase64[ plainText[ nPlainTextCurLen + 2 ] & 0x3F ];
		}
		else if( nPlainTextLen - nPlainTextCurLen > 1 )
		{
			chiperText[ nChiperTextSize++ ] = s_TabBase64[ plainText[ nPlainTextCurLen ] >> 2 ];
			chiperText[ nChiperTextSize++ ] = s_TabBase64[( plainText[ nPlainTextCurLen ] << 4 | plainText[ nPlainTextCurLen + 1 ] >> 4 ) & 0x3F ];
			chiperText[ nChiperTextSize++ ] = s_TabBase64[( plainText[ nPlainTextCurLen + 1 ] << 2  ) & 0x3F ];
			chiperText[ nChiperTextSize++ ] = '=';
		}
		else
		{
			chiperText[ nChiperTextSize++ ] = s_TabBase64[ plainText[ nPlainTextCurLen ] >> 2 ];
			chiperText[ nChiperTextSize++ ] = s_TabBase64[ ( plainText[ nPlainTextCurLen ] << 4 ) & 0x3F ];
			chiperText[ nChiperTextSize++ ] = '=';
			chiperText[ nChiperTextSize++ ] = '=';
		}

		nPlainTextCurLen += 3;
	}

	return nChiperTextSize;
}


int DecryptBase64(  unsigned char* chiperText, int nChiperTextLen, unsigned char* plainText, int nPlainTextLen )
{
	int nPlainSize = 0;
	int nChiperCurLen = 0;
	int rmovs[ 3 ] = { 4, 2, 0 };
	int lmovs[ 3 ] = { 2, 4, 6 };
	int i = 0 , lNum = 0, rNum = 0;

	while( nChiperTextLen - nChiperCurLen > 3 )
	{
		for( i = 0; i < 3; i++ )
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

