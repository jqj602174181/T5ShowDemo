#include <windows.h>
#include <TCHAR.h>
#include <stdio.h>
#include "Debug.h"
#include "WindowImage.h"


/*****************************************************************
��������DrawAddress
������  ������֤��ַ���뵽����ͼƬ��ÿ11��һ�У���Ҫ����������Ӣ�Ļ��ӵ������
������  szAddr-��ͥסַ��Ϣ��hMemDC-�豸���
����ֵ����
��ע��  ��
******************************************************************/
void DrawAddress( const char *szAddr, HDC hMemDC )
{
	int iIndex = 0;
	char szTmpt[100] = { 0 };
	int iLine = 0;
	int iCharNum = 0;
	int i = 0;

	while ( strlen(szAddr) - iIndex > 22 )
	{
		memset( szTmpt, 0, sizeof( szTmpt ) );
		memcpy( szTmpt, szAddr + iIndex, 22 );

		for( i = 0; i < 22; i++ )
		{
			if( szTmpt[i] > 0 )
			{
				//�ҵ����ֻ�Ӣ����ĸ
				iCharNum++;
			}
		}
		if( iCharNum % 2 == 0 )//��Ӣ���ַ�����Ϊż����ֱ�ӽ�ȡ22���ַ�
		{
			TextOut( hMemDC, 65, 105 + (iLine * 15), szTmpt, lstrlen(szTmpt) );
			iIndex += 22;
		}
		else//�������ַ�����Ϊ��������ȡ21��
		{
			szTmpt[21] = '\0';
			TextOut( hMemDC, 65, 105 + (iLine * 15), szTmpt, lstrlen(szTmpt) );
			iIndex += 21;
		}
		iLine++;
	}
	memcpy( szTmpt, szAddr + iIndex, strlen(szAddr) - iIndex + 1 );
	TextOut( hMemDC, 65, 105 + (iLine * 15), szTmpt, lstrlen(szTmpt) );
}



/*****************************************************************
��������DrawLogo
������  ����logo
������  hMemDC--�����е��ڴ�DC
����ֵ�������С
��ע��  ��
******************************************************************/
void DrawLogo( HDC hMemDC )
{
	char szTime[ MAXCHAR ] = { 0 };
	char szLogo[ ] = "�й����ٱ��չɷ����޹�˾";
	SYSTEMTIME systemTime;

	GetLocalTime( &systemTime );
	sprintf_s( szTime, sizeof(szTime),"%02d/%02d/%02d  %02d:%02d:%02d", systemTime.wYear%1000, systemTime.wMonth
		, systemTime.wDay, systemTime.wHour, systemTime.wMinute,  systemTime.wSecond );

	TextOut( hMemDC, 55, 140, szTime, lstrlen(szTime) );
	TextOut( hMemDC, 35, 155, szLogo, lstrlen(szLogo) );

}


/*****************************************************************
��������GetBitCounts
������  ��ȡλͼ�����С�Լ����ֽڣ��͸ߣ����أ�
������  bmp--λͼ�����lpWidth[out]--ָ���ȵ�ָ�룬lpHeight--ָ��߶�ָ��
����ֵ�������С
��ע��  ��
******************************************************************/
DWORD GetBitCounts( HBITMAP bmp, int *lpWidth, int *lpHeight )
{
	BITMAP bm;
	int nLineBits;
	int nBitCounts;

	//ͨ��λͼ�����ȡλͼ��Ϣ
	GetObject( bmp, sizeof(BITMAP), &bm );

	//��ȡ��Ƭ�߶�
	if( lpHeight != NULL )
	{
		*lpHeight = bm.bmHeight;
	}

	nLineBits = ( (bm.bmWidthBytes+7)/8 ) * 8;
	//��ȡ��Ƭ����
	if( lpWidth != NULL )
	{
		*lpWidth = nLineBits;
	}

	//�������
	nBitCounts = nLineBits * bm.bmHeight;

	return nBitCounts;
}


/*****************************************************************
��������GetBitmapData
������  ͨ��λͼ�����λͼ����洢��������
������  bmp--λͼ�����byBitmap--�洢λͼ���󻺳�����dwBuffLen--��������С
����ֵ��0--�ɹ���-1--ʧ��
��ע��  ��
******************************************************************/
int GetBitmapData( HBITMAP bmp, BYTE *byBitmap, DWORD dwBuffLen )
{
	BITMAP bm;
	BITMAPINFO *pbmpinfo;
	BITMAPINFOHEADER *pInfoHead;
	HDC hdc;
	BYTE *pBits;
	int nLineBits;
	int nBitCounts;
	int nBitmapInfoSize;
	int nLx;
	int nLy;
	int nColors = 0;

	if( byBitmap == NULL || dwBuffLen == 0 || bmp == NULL )
	{
		return -1;
	}

	//��ȡλͼ��Ϣ
	GetObject( bmp, sizeof(BITMAP), &bm );
	nLineBits = 8*((bm.bmWidthBytes+7)/8);
	nBitCounts = nLineBits * bm.bmHeight ;

	//��BITMAPINFO��С
	if ( bm.bmBitsPixel <= 8 )
	{
		int nColors = 1 << bm.bmBitsPixel;
		nBitmapInfoSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*nColors;	
	}
	else
	{
		nBitmapInfoSize = sizeof(BITMAPINFOHEADER);
	}

	//����ռ�
	pbmpinfo = (BITMAPINFO *)malloc( nBitmapInfoSize );
	ZeroMemory( (void *)pbmpinfo,nBitmapInfoSize );

	//ͼƬͷ
	pInfoHead = (BITMAPINFOHEADER *)pbmpinfo;
	ZeroMemory( (void *)pInfoHead, sizeof(BITMAPINFOHEADER) );

	//����ͼƬͷ��Ϣ
	pInfoHead->biSize = sizeof(BITMAPINFOHEADER);
	pInfoHead->biBitCount =  bm.bmBitsPixel;
	pInfoHead->biPlanes = 1;
	pInfoHead->biSizeImage = nBitCounts;
	pInfoHead->biCompression = BI_RGB;
	pInfoHead->biWidth = bm.bmWidth;
	pInfoHead->biHeight = bm.bmHeight;


	//��ȡ�豸���
	hdc = CreateDC( "DISPLAY", NULL, NULL, NULL );
	nLx = GetDeviceCaps( hdc, LOGPIXELSX );
	nLy = GetDeviceCaps( hdc, LOGPIXELSY );

	pInfoHead->biXPelsPerMeter = (int)((double)nLx * DBINCHPERMETEAR );
	pInfoHead->biYPelsPerMeter = (int)((double)nLy * DBINCHPERMETEAR );

	//��ɫ��
	if ( bm.bmBitsPixel <= 8)
	{
		nColors = 1 << bm.bmBitsPixel;
		GetDIBColorTable( hdc, 0, nColors, pbmpinfo->bmiColors );
	}

	pBits = (BYTE *)malloc( nBitCounts );
	ZeroMemory( (void *)pBits, nBitCounts );

	GetDIBits( hdc, bmp, 0, bm.bmHeight, pBits, pbmpinfo, DIB_RGB_COLORS );
	memcpy_s( byBitmap, dwBuffLen, pBits, nBitCounts );

	//�ͷŷ���Ŀռ�
	free( pbmpinfo );
	free( pBits );
	return 0;
}


/*****************************************************************
��������CreateBitmapObjectFromDibFile
������  ��DIBλͼת����DDBλͼ
������  hdc-�豸�����szFileName-����λͼ�ļ�·��
����ֵ������λͼ���
��ע��  ��
******************************************************************/
HBITMAP CreateBitmapObjectFromDibFile (HDC hdc, const char *szFileName)
{
	BITMAPFILEHEADER * pbmfh ;//ͼƬ�ļ�ͷ
	BOOL               bSuccess ;
	DWORD              dwFileSize, dwHighSize, dwBytesRead ;
	HANDLE             hFile ;
	HBITMAP            hBitmap ;


	hFile = CreateFile (szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, 
		OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL) ;

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	dwFileSize = GetFileSize (hFile, &dwHighSize) ;

	if (dwHighSize)
	{
		CloseHandle (hFile) ;
		return NULL;
	}

	pbmfh = (BITMAPFILEHEADER *)malloc (dwFileSize) ;

	if (!pbmfh)
	{
		CloseHandle (hFile) ;
		return NULL;
	}

	bSuccess = ReadFile (hFile, pbmfh, dwFileSize, &dwBytesRead, NULL) ;
	CloseHandle (hFile) ;


	if (!bSuccess || (dwBytesRead != dwFileSize)         
		|| (pbmfh->bfType != * (WORD *) "BM") 
		|| (pbmfh->bfSize != dwFileSize))
	{
		free (pbmfh) ;
		return NULL ;
	}

	hBitmap = CreateDIBitmap ( hdc, (BITMAPINFOHEADER *) (pbmfh + 1), CBM_INIT,
		(BYTE *) pbmfh + pbmfh->bfOffBits,
		(BITMAPINFO *) (pbmfh + 1), DIB_RGB_COLORS) ;
	free (pbmfh) ;
	return hBitmap ;
}



/************************************************************************************
*******************************����ӿں���******************************************
************************************************************************************/



/************************************************************************************
* ��������GetPhotoBmp
*  
* ���ܣ�  ��wlt�ļ�תΪbmp�ļ�
*  
*������   szImgFile[in]--wlt�ļ�
*  
*����ֵ�� TRUE---�ɹ�
*         FALSE--ʧ��
************************************************************************************/
BOOL GetPhotoBmp( char *szDllPath , char* szImgFile )
{
	//Window�»�ȡͷ����
	typedef int ( PASCAL* LPGETBMP )( char*, int ) ;

	LPGETBMP GetBmp = NULL;  //GtBmp����ָ��
	HINSTANCE hDll = NULL;   //��̬����
	BOOL bRet = FALSE;       //����ֵ
	//char szLicencePath[ MAX_PATH ] = { 0 };  //license.dat·��
	char szTempPath[ MAX_PATH ] = { 0 };     //����̬������Ŀ¼

	//��license.dat�ļ����Ƶ�c��
	//GetFullPath( _T("license.dat"), szLicencePath, _countof(szLicencePath) );
	//CopyFile( szLicencePath, _T("C:\\license.dat"), TRUE );

	//ת��ΪbmpͼƬ
	if( (hDll=LoadLibrary(szDllPath)) &&
		(GetBmp=( LPGETBMP )GetProcAddress(hDll, "GetBmp")) &&
		GetBmp(szImgFile, 1) == 1)
	{
		bRet = TRUE;
	}

	if( hDll != NULL )
	{
		FreeLibrary( hDll );
	}
	return bRet;
}


//����ͼƬ�����ͷ�ļ�˵��
BOOL SaveBitmapFile( const char *strFileName, HBITMAP bmp )
{
	HDC hdc;
	BITMAPINFO *pbmpinfo;
	BITMAPINFOHEADER *pInfoHead;
	BITMAPFILEHEADER	bmfHeader;
	BITMAP bm;

	BYTE *pBits;
	int nLineBits;
	int nBitCounts;
	int nBitmapInfoSize;
	int nLx;
	int nLy;
	int nColors = 0;
	FILE *file = NULL;

	if( bmp == NULL )
	{
		return FALSE;
	}

	//��ȡλͼ��Ϣ
	GetObject( bmp, sizeof(BITMAP), &bm );
	nLineBits = 8*((bm.bmWidthBytes+7)/8);
	nBitCounts = nLineBits * bm.bmHeight ;

	//��BITMAPINFO��С
	if ( bm.bmBitsPixel <= 8 )
	{
		int nColors = 1 << bm.bmBitsPixel;
		nBitmapInfoSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*nColors;	
	}
	else
	{
		nBitmapInfoSize = sizeof(BITMAPINFOHEADER);
	}

	//����ռ�
	pbmpinfo = (BITMAPINFO *)malloc( nBitmapInfoSize );
	ZeroMemory( (void *)pbmpinfo,nBitmapInfoSize );

	//ͼƬͷ
	pInfoHead = (BITMAPINFOHEADER *)pbmpinfo;
	ZeroMemory( (void *)pInfoHead, sizeof(BITMAPINFOHEADER) );

	//����ͼƬͷ��Ϣ
	pInfoHead->biSize = sizeof(BITMAPINFOHEADER);
	pInfoHead->biBitCount = bm.bmBitsPixel;
	pInfoHead->biPlanes = 1;
	pInfoHead->biSizeImage = nBitCounts;
	pInfoHead->biCompression = BI_RGB;
	pInfoHead->biWidth = bm.bmWidth;
	pInfoHead->biHeight = bm.bmHeight;

	//��ȡ�豸���
	hdc = CreateDC( "DISPLAY", NULL, NULL, NULL );
	nLx = GetDeviceCaps( hdc, LOGPIXELSX );
	nLy = GetDeviceCaps( hdc, LOGPIXELSY );

	pInfoHead->biXPelsPerMeter = (int)((double)nLx * DBINCHPERMETEAR );
	pInfoHead->biYPelsPerMeter = (int)((double)nLy * DBINCHPERMETEAR );

	//��ɫ��
	if ( bm.bmBitsPixel <= 8)
	{
		nColors = 1 << bm.bmBitsPixel;
		GetDIBColorTable( hdc, 0, nColors, pbmpinfo->bmiColors );
	}

	pBits = (BYTE *)malloc( nBitCounts );
	ZeroMemory( (void *)pBits, nBitCounts );

	//���ͼƬ��������
	GetDIBits( hdc, bmp, 0, bm.bmHeight, pBits, pbmpinfo, DIB_RGB_COLORS );

	//����ļ�ͷ
	ZeroMemory( (void *)&bmfHeader, sizeof(BITMAPFILEHEADER) );
	bmfHeader.bfType = * (WORD *) "BM";//'BM'    ((WORD) ('M' << 8) | 'B'); 
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + nBitmapInfoSize + nBitCounts;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + nBitmapInfoSize;


	//�洢���ļ���
	file = fopen( strFileName, "wb" );
	if( file == NULL )
	{
		free( pbmpinfo );
		free( pBits );
		return FALSE;
	}

	//д���ļ�ͷ
	fwrite( &bmfHeader, 1, sizeof(BITMAPFILEHEADER), file );
	//д��ͼƬͷ
	fwrite( pbmpinfo, 1, nBitmapInfoSize, file );
	//д���������
	fwrite( pBits, 1, nBitCounts, file );
	fclose( file );

	//�ͷŷ���Ŀռ�
	free( pbmpinfo );
	free( pBits );
	return TRUE;
}



//�������֤����ͼƬ
BOOL SaveFrontImage( HMODULE hModule, unsigned int nResID, PERSONINFO personInfo, 
						const char* szPhotoPath, const char *szImagePath )
{
	HWND h_wnd = GetDesktopWindow();
	HDC hdc = GetDC(h_wnd);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP h_bitmap = LoadBitmap( hModule, MAKEINTRESOURCE(nResID) );
	HBITMAP holdbitmap = (HBITMAP)SelectObject( hMemDC, h_bitmap );
	HFONT h_font = 
		CreateFont( 12,                                          //   nHeight  
		0,                                                   //   nWidth  
		0,                                                   //   nEscapement
		0,                                                   //   nOrientation  
		400,                                   //   nWeight  400
		FALSE,                                           //   bItalic  
		FALSE,                                           //   bUnderline  
		0,                                                   //   cStrikeOut  
		ANSI_CHARSET,                             //   nCharSet  
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision  
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision  
		DEFAULT_QUALITY,                       //   nQuality  
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		"����");
	//::CREATE
	HFONT holdfont= (HFONT)SelectObject( hMemDC, h_font );

	//������Ϣ�ַ���
	char szName[ 32 ] = { 0 };
	char bY[10] = { 0 };
	char bM[5] = { 0 };
	char bD[5] = { 0 };

	//����
	HFONT h_font1 = NULL;
	HBITMAP hHeadBitemap = NULL;
	HDC hMemDC2 = NULL;
	HDC hMemDC3 = NULL;
	BITMAP bitmap;
	HBITMAP holdbitmap2 = NULL;
	COLORREF col;
	HBITMAP hNew = NULL;
	BOOL bRet = 0;
	int i = 0, nTmp = 0;

	//ȥ�����ֱ���ɫ
	SetBkMode( hMemDC, TRANSPARENT );

	//����
	if( lstrlen( personInfo.name ) == 4 )
	{
		strncpy( szName, personInfo.name, 2 );
		strcat( szName, "  " );
		strcat( szName, personInfo.name + 2 );
		TextOut( hMemDC, 65, 30, szName, lstrlen( szName ));
	}
	else
	{
		TextOut( hMemDC, 65, 30, personInfo.name, lstrlen( personInfo.name ));
	}
	
	//�Ա�
	TextOut( hMemDC, 65, 55, personInfo.sex, lstrlen(personInfo.sex) );
	//����
	TextOut( hMemDC, 130, 55, personInfo.nation, lstrlen(personInfo.nation) );

	//�������ڡ���
	memcpy( bY, personInfo.birthday, 4 );
	TextOut( hMemDC, 65, 80, bY, lstrlen(bY) );

	//�������ڡ���
	memcpy( bM, personInfo.birthday + 4, 2 );
	if( bM[0] == '0' )
	{
		bM[ 0 ] = ' ';
		TextOut( hMemDC, 110, 80, bM, lstrlen(bM) );
	}
	else
	{
		TextOut( hMemDC, 112, 80, bM, lstrlen(bM) );
	}

	//�������ڡ���
	memcpy( bD, personInfo.birthday + 6, 2 );
	if( bD[0] == '0' )
	{
		bD[ 0 ] = ' ';
		TextOut( hMemDC, 140, 80, bD, lstrlen(bD) );
	}
	else
	{
		TextOut( hMemDC, 142, 80, bD, lstrlen(bD) );
	}

	//д��ַ
	DrawAddress( personInfo.address, hMemDC );

	//���֤��
	h_font1 = CreateFont( 15,                          //   nHeight  
		0,                                                   //   nWidth  
		0,                                                   //   nEscapement
		0,                                                   //   nOrientation  
		500,                                   //   nWeight  400
		FALSE,                                           //   bItalic  
		FALSE,                                           //   bUnderline  
		0,                                                   //   cStrikeOut  
		ANSI_CHARSET,                             //   nCharSet  
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision  
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision  
		DEFAULT_QUALITY,                       //   nQuality  
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		"����");
	//::CREATE
	DeleteObject(( HFONT)SelectObject( hMemDC, h_font1 ));
	for( i = 0; i < lstrlen(personInfo.cardId); i++ )
	{
		nTmp = 117.5 + i * 9.5;
		TextOut( hMemDC, nTmp, 170, personInfo.cardId + i , 1 );
	}

	hHeadBitemap = CreateBitmapObjectFromDibFile( hdc, szPhotoPath );
	hMemDC2 = CreateCompatibleDC(hdc);
	hMemDC3 = CreateCompatibleDC(hdc);

	GetObject( hHeadBitemap, sizeof(BITMAP), &bitmap );
	holdbitmap2 = (HBITMAP)SelectObject( hMemDC2, hHeadBitemap );

	//��ȡ����ɫ��ɫֵ
	col = GetPixel( hMemDC2, 3, 3 );

	//��ͷ��ͼƬ����ɫȥ��
	TransparentBlt( hMemDC, 209, 26, bitmap.bmWidth, bitmap.bmHeight, hMemDC2, 0, 0, bitmap.bmWidth, bitmap.bmHeight, col );
	hNew = (HBITMAP)SelectObject( hMemDC, holdbitmap );
	bRet = SaveBitmapFile( szImagePath, hNew );
	SelectObject( hMemDC, holdfont );

	DeleteObject( h_bitmap );
	DeleteObject( holdbitmap2 );
	DeleteObject( hNew );
	DeleteDC( hMemDC );
	DeleteDC( hMemDC2 );

	return bRet;
}


//���汳��ͼƬ
BOOL SaveBackImage(HMODULE hModule, unsigned int nResID, PERSONINFO pInfo, const char *szImagePath )
{
	HWND h_wnd = GetDesktopWindow();
	HDC hdc = GetDC(h_wnd);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP h_bitmap = LoadBitmap( hModule, MAKEINTRESOURCE(nResID) );
	HBITMAP holdbitmap = (HBITMAP)SelectObject( hMemDC, h_bitmap );
	HFONT h_font = CreateFont(12,                                          //   nHeight  
		0,                                                   //   nWidth  
		0,                                                   //   nEscapement
		0,                                                   //   nOrientation  
		400,                                   //   nWeight  400
		FALSE,                                           //   bItalic  
		FALSE,                                           //   bUnderline  
		0,                                                   //   cStrikeOut  
		ANSI_CHARSET,                             //   nCharSet  
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision  
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision  
		DEFAULT_QUALITY,                       //   nQuality  
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		"����");
	//::CREATE
	HFONT holdfont= (HFONT)SelectObject( hMemDC, h_font );
	
	//����
	char szStar[20] = { 0 };
	char szEnd[20] = { 0 };
	char valid[50] = { 0 };
	HBITMAP hBitmap = NULL;
	BOOL bRet = 0;

	SetBkMode( hMemDC, TRANSPARENT );

	//��֤����
	TextOut( hMemDC, 135, 150, pInfo.police, strlen(pInfo.police) );

	//��Ч���ޡ���ʼʱ��
	memcpy( szStar, pInfo.validStart, 4 );
	lstrcat( szStar, "." );
	memcpy( szStar+5, pInfo.validStart+4, 2 );
	lstrcat( szStar, "." );
	memcpy( szStar+8, pInfo.validStart+6, 2 );

	//��Ч���ޡ���ֹʱ��
	memcpy( szEnd, pInfo.validEnd, 4 );
	lstrcat( szEnd, "." );
	memcpy( szEnd+5, pInfo.validEnd+4, 2 );
	lstrcat( szEnd, "." );
	memcpy( szEnd+8, pInfo.validEnd+6, 2 );

	//��Ч��
	memset( valid, 0, sizeof(valid) );
	sprintf_s( valid, sizeof(valid), "%s-%s", szStar, szEnd );
	TextOut( hMemDC, 135, 177, valid, strlen(valid) );

	hBitmap = (HBITMAP)SelectObject( hMemDC, holdbitmap );
	bRet = SaveBitmapFile( szImagePath, hBitmap );

	SelectObject( hMemDC, holdfont );
	DeleteObject(h_bitmap);
	DeleteDC(hMemDC);

	return bRet;
}


BOOL SaveIDCardImage(HMODULE hModule, unsigned int nResID, BOOL bMakeLogo,PERSONINFO personInfo,
	const char* szPhotoPath, const char* szImgPath )
{
	HWND h_wnd = GetDesktopWindow();
	HDC hdc = GetDC(h_wnd);
	HDC hMemDC = CreateCompatibleDC(hdc);
	HBITMAP h_bitmap = LoadBitmap( hModule, MAKEINTRESOURCE(nResID) );
	HBITMAP holdbitmap = (HBITMAP)SelectObject( hMemDC, h_bitmap );
	HFONT h_font = CreateFont( 12,                                          //   nHeight  
		0,                                                   //   nWidth  
		0,                                                   //   nEscapement
		0,                                                   //   nOrientation  
		400,                                   //   nWeight  400
		FALSE,                                           //   bItalic  
		FALSE,                                           //   bUnderline  
		0,                                                   //   cStrikeOut  
		ANSI_CHARSET,                             //   nCharSet  
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision  
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision  
		DEFAULT_QUALITY,                       //   nQuality  
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		"����");
	//::CREATE
	HFONT holdfont= (HFONT)SelectObject( hMemDC, h_font );

	//����
	char szName[ 32 ] = { 0 };
	char bY[10] = { 0 };
	char bM[5] = { 0 };
	char bD[5] = { 0 };
	char szStar[20] = { 0 };
	char szEnd[20] = { 0 };
	char valid[50];
	HFONT h_font1 = NULL;
	HBITMAP hHeadBitemap = NULL;
	HDC hMemDC2 = NULL;
	HDC hMemDC3 = NULL;
	BITMAP bitmap;
	HBITMAP holdbitmap2 = NULL;
	COLORREF col; //��ȡ����ɫ��ɫֵ
	HBITMAP hNew = NULL;
	BOOL bRet = FALSE;
	int i = 0, nTmp = 0;

	//ȥ�����ֱ���ɫ
	SetBkMode( hMemDC, TRANSPARENT );

	//����
	if( lstrlen( personInfo.name ) == 4 )
	{
		strncpy( szName, personInfo.name, 2 );
		strcat( szName, "  " );
		strcat( szName, personInfo.name + 2 );
		TextOut( hMemDC, 65, 30, szName, lstrlen( szName ));
	}
	else
	{
		TextOut( hMemDC, 65, 30, personInfo.name, lstrlen( personInfo.name ));
	}

	//�Ա�
	TextOut( hMemDC, 65, 55, personInfo.sex, lstrlen(personInfo.sex) );
	//����
	TextOut( hMemDC, 130, 55, personInfo.nation, lstrlen(personInfo.nation) );

	//�������ڡ���
	memcpy( bY, personInfo.birthday, 4 );
	TextOut( hMemDC, 65, 80, bY, lstrlen(bY) );

	//�������ڡ���
	memcpy( bM, personInfo.birthday + 4, 2 );
	if( bM[0] == '0' )
	{
		bM[ 0 ] = ' ';
		TextOut( hMemDC, 110, 80, bM, lstrlen(bM) );
	}
	else
	{
		TextOut( hMemDC, 112, 80, bM, lstrlen(bM) );
	}

	//�������ڡ���
	memcpy( bD, personInfo.birthday + 6, 2 );
	if( bD[0] == '0' )
	{
		bD[ 0 ] = ' ';
		TextOut( hMemDC, 140, 80, bD, lstrlen(bD) );
	}
	else
	{
		TextOut( hMemDC, 142, 80, bD, lstrlen(bD) );
	}

	//д��ַ
	DrawAddress( personInfo.address, hMemDC );

	//��ˮӡ
	if (bMakeLogo)
	{
		DrawLogo( hMemDC );
	}

	//��֤����
	TextOut( hMemDC, 135, 363, personInfo.police, strlen(personInfo.police) );

	//��Ч���ޡ���ʼʱ��
	memcpy( szStar, personInfo.validStart, 4 );
	lstrcat( szStar, "." );
	memcpy( szStar+5, personInfo.validStart+4, 2 );
	lstrcat( szStar, "." );
	memcpy( szStar+8, personInfo.validStart+6, 2 );

	//��Ч���ޡ���ֹʱ��
	memcpy( szEnd, personInfo.validEnd, 4 );
	lstrcat( szEnd, "." );
	memcpy( szEnd+5, personInfo.validEnd+4, 2 );
	lstrcat( szEnd, "." );
	memcpy( szEnd+8, personInfo.validEnd+6, 2 );

	//��Ч��
	memset( valid, 0, sizeof(valid) );
	sprintf_s( valid, sizeof(valid), "%s-%s", szStar, szEnd );
	TextOut( hMemDC, 135, 390, valid, strlen(valid) );

	//���֤��
	h_font1 = CreateFont( 15,                          //   nHeight  
		0,                                                   //   nWidth  
		0,                                                   //   nEscapement
		0,                                                   //   nOrientation  
		500,                                   //   nWeight  400
		FALSE,                                           //   bItalic  
		FALSE,                                           //   bUnderline  
		0,                                                   //   cStrikeOut  
		ANSI_CHARSET,                             //   nCharSet  
		OUT_DEFAULT_PRECIS,                 //   nOutPrecision  
		CLIP_DEFAULT_PRECIS,               //   nClipPrecision  
		DEFAULT_QUALITY,                       //   nQuality  
		DEFAULT_PITCH | FF_SWISS,     //   nPitchAndFamily     
		"����");
	//::CREATE
	DeleteObject(( HFONT)SelectObject( hMemDC, h_font1 ));
	for( i = 0; i < lstrlen(personInfo.cardId); i++ )
	{
		nTmp = 117.5 + i * 9.5;
		TextOut( hMemDC, nTmp, 170, personInfo.cardId + i , 1 );
	}

	hHeadBitemap = CreateBitmapObjectFromDibFile( hdc, szPhotoPath );
	hMemDC2 = CreateCompatibleDC(hdc);
	hMemDC3 = CreateCompatibleDC(hdc);


	GetObject( hHeadBitemap, sizeof(BITMAP), &bitmap );
	holdbitmap2 = (HBITMAP)SelectObject( hMemDC2, hHeadBitemap );

	//��ȡ����ɫ��ɫֵ
	col = GetPixel( hMemDC2, 3, 3 );

	//��ͷ��ͼƬ����ɫȥ��
	TransparentBlt( hMemDC, 209, 26, bitmap.bmWidth, bitmap.bmHeight, hMemDC2, 0, 0, bitmap.bmWidth, bitmap.bmHeight, col );
	hNew = (HBITMAP)SelectObject( hMemDC, holdbitmap );

	bRet = SaveBitmapFile( szImgPath, hNew );

	SelectObject( hMemDC, holdfont );
	DeleteObject( h_bitmap );
	DeleteObject( holdbitmap2 );
	DeleteObject( hNew );
	DeleteDC( hMemDC );
	DeleteDC( hMemDC2 );

	return bRet;
}




