#include <windows.h>
#include <TCHAR.h>
#include <stdio.h>
#include "Debug.h"
#include "WindowImage.h"


/*****************************************************************
函数名：DrawAddress
描述：  将二代证地址输入到正面图片（每11字一行，主要处理里面中英文混杂的情况）
参数：  szAddr-家庭住址信息，hMemDC-设备句柄
返回值：无
备注：  无
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
				//找到数字或英文字母
				iCharNum++;
			}
		}
		if( iCharNum % 2 == 0 )//当英文字符个数为偶数，直接截取22个字符
		{
			TextOut( hMemDC, 65, 105 + (iLine * 15), szTmpt, lstrlen(szTmpt) );
			iIndex += 22;
		}
		else//当引文字符个数为奇数，截取21个
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
函数名：DrawLogo
描述：  设置logo
参数：  hMemDC--操作中的内存DC
返回值：点阵大小
备注：  无
******************************************************************/
void DrawLogo( HDC hMemDC )
{
	char szTime[ MAXCHAR ] = { 0 };
	char szLogo[ ] = "中国人寿保险股份有限公司";
	SYSTEMTIME systemTime;

	GetLocalTime( &systemTime );
	sprintf_s( szTime, sizeof(szTime),"%02d/%02d/%02d  %02d:%02d:%02d", systemTime.wYear%1000, systemTime.wMonth
		, systemTime.wDay, systemTime.wHour, systemTime.wMinute,  systemTime.wSecond );

	TextOut( hMemDC, 55, 140, szTime, lstrlen(szTime) );
	TextOut( hMemDC, 35, 155, szLogo, lstrlen(szLogo) );

}


/*****************************************************************
函数名：GetBitCounts
描述：  获取位图点阵大小以及宽（字节）和高（像素）
参数：  bmp--位图句柄，lpWidth[out]--指向宽度的指针，lpHeight--指向高度指针
返回值：点阵大小
备注：  无
******************************************************************/
DWORD GetBitCounts( HBITMAP bmp, int *lpWidth, int *lpHeight )
{
	BITMAP bm;
	int nLineBits;
	int nBitCounts;

	//通过位图句柄获取位图信息
	GetObject( bmp, sizeof(BITMAP), &bm );

	//获取照片高度
	if( lpHeight != NULL )
	{
		*lpHeight = bm.bmHeight;
	}

	nLineBits = ( (bm.bmWidthBytes+7)/8 ) * 8;
	//获取照片行数
	if( lpWidth != NULL )
	{
		*lpWidth = nLineBits;
	}

	//点阵个数
	nBitCounts = nLineBits * bm.bmHeight;

	return nBitCounts;
}


/*****************************************************************
函数名：GetBitmapData
描述：  通过位图句柄将位图点阵存储到数组中
参数：  bmp--位图句柄，byBitmap--存储位图点阵缓冲区，dwBuffLen--缓冲区大小
返回值：0--成功，-1--失败
备注：  无
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

	//获取位图信息
	GetObject( bmp, sizeof(BITMAP), &bm );
	nLineBits = 8*((bm.bmWidthBytes+7)/8);
	nBitCounts = nLineBits * bm.bmHeight ;

	//求BITMAPINFO大小
	if ( bm.bmBitsPixel <= 8 )
	{
		int nColors = 1 << bm.bmBitsPixel;
		nBitmapInfoSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*nColors;	
	}
	else
	{
		nBitmapInfoSize = sizeof(BITMAPINFOHEADER);
	}

	//分配空间
	pbmpinfo = (BITMAPINFO *)malloc( nBitmapInfoSize );
	ZeroMemory( (void *)pbmpinfo,nBitmapInfoSize );

	//图片头
	pInfoHead = (BITMAPINFOHEADER *)pbmpinfo;
	ZeroMemory( (void *)pInfoHead, sizeof(BITMAPINFOHEADER) );

	//设置图片头信息
	pInfoHead->biSize = sizeof(BITMAPINFOHEADER);
	pInfoHead->biBitCount =  bm.bmBitsPixel;
	pInfoHead->biPlanes = 1;
	pInfoHead->biSizeImage = nBitCounts;
	pInfoHead->biCompression = BI_RGB;
	pInfoHead->biWidth = bm.bmWidth;
	pInfoHead->biHeight = bm.bmHeight;


	//获取设备句柄
	hdc = CreateDC( "DISPLAY", NULL, NULL, NULL );
	nLx = GetDeviceCaps( hdc, LOGPIXELSX );
	nLy = GetDeviceCaps( hdc, LOGPIXELSY );

	pInfoHead->biXPelsPerMeter = (int)((double)nLx * DBINCHPERMETEAR );
	pInfoHead->biYPelsPerMeter = (int)((double)nLy * DBINCHPERMETEAR );

	//调色板
	if ( bm.bmBitsPixel <= 8)
	{
		nColors = 1 << bm.bmBitsPixel;
		GetDIBColorTable( hdc, 0, nColors, pbmpinfo->bmiColors );
	}

	pBits = (BYTE *)malloc( nBitCounts );
	ZeroMemory( (void *)pBits, nBitCounts );

	GetDIBits( hdc, bmp, 0, bm.bmHeight, pBits, pbmpinfo, DIB_RGB_COLORS );
	memcpy_s( byBitmap, dwBuffLen, pBits, nBitCounts );

	//释放分配的空间
	free( pbmpinfo );
	free( pBits );
	return 0;
}


/*****************************************************************
函数名：CreateBitmapObjectFromDibFile
描述：  将DIB位图转换成DDB位图
参数：  hdc-设备句柄，szFileName-本地位图文件路径
返回值：本地位图句柄
备注：  无
******************************************************************/
HBITMAP CreateBitmapObjectFromDibFile (HDC hdc, const char *szFileName)
{
	BITMAPFILEHEADER * pbmfh ;//图片文件头
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
*******************************对外接口函数******************************************
************************************************************************************/



/************************************************************************************
* 函数名：GetPhotoBmp
*  
* 功能：  将wlt文件转为bmp文件
*  
*参数：   szImgFile[in]--wlt文件
*  
*返回值： TRUE---成功
*         FALSE--失败
************************************************************************************/
BOOL GetPhotoBmp( char *szDllPath , char* szImgFile )
{
	//Window下获取头像函数
	typedef int ( PASCAL* LPGETBMP )( char*, int ) ;

	LPGETBMP GetBmp = NULL;  //GtBmp函数指针
	HINSTANCE hDll = NULL;   //动态库句柄
	BOOL bRet = FALSE;       //返回值
	//char szLicencePath[ MAX_PATH ] = { 0 };  //license.dat路径
	char szTempPath[ MAX_PATH ] = { 0 };     //本动态库所在目录

	//将license.dat文件复制到c：
	//GetFullPath( _T("license.dat"), szLicencePath, _countof(szLicencePath) );
	//CopyFile( szLicencePath, _T("C:\\license.dat"), TRUE );

	//转换为bmp图片
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


//保存图片，详见头文件说明
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

	//获取位图信息
	GetObject( bmp, sizeof(BITMAP), &bm );
	nLineBits = 8*((bm.bmWidthBytes+7)/8);
	nBitCounts = nLineBits * bm.bmHeight ;

	//求BITMAPINFO大小
	if ( bm.bmBitsPixel <= 8 )
	{
		int nColors = 1 << bm.bmBitsPixel;
		nBitmapInfoSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*nColors;	
	}
	else
	{
		nBitmapInfoSize = sizeof(BITMAPINFOHEADER);
	}

	//分配空间
	pbmpinfo = (BITMAPINFO *)malloc( nBitmapInfoSize );
	ZeroMemory( (void *)pbmpinfo,nBitmapInfoSize );

	//图片头
	pInfoHead = (BITMAPINFOHEADER *)pbmpinfo;
	ZeroMemory( (void *)pInfoHead, sizeof(BITMAPINFOHEADER) );

	//设置图片头信息
	pInfoHead->biSize = sizeof(BITMAPINFOHEADER);
	pInfoHead->biBitCount = bm.bmBitsPixel;
	pInfoHead->biPlanes = 1;
	pInfoHead->biSizeImage = nBitCounts;
	pInfoHead->biCompression = BI_RGB;
	pInfoHead->biWidth = bm.bmWidth;
	pInfoHead->biHeight = bm.bmHeight;

	//获取设备句柄
	hdc = CreateDC( "DISPLAY", NULL, NULL, NULL );
	nLx = GetDeviceCaps( hdc, LOGPIXELSX );
	nLy = GetDeviceCaps( hdc, LOGPIXELSY );

	pInfoHead->biXPelsPerMeter = (int)((double)nLx * DBINCHPERMETEAR );
	pInfoHead->biYPelsPerMeter = (int)((double)nLy * DBINCHPERMETEAR );

	//调色板
	if ( bm.bmBitsPixel <= 8)
	{
		nColors = 1 << bm.bmBitsPixel;
		GetDIBColorTable( hdc, 0, nColors, pbmpinfo->bmiColors );
	}

	pBits = (BYTE *)malloc( nBitCounts );
	ZeroMemory( (void *)pBits, nBitCounts );

	//获得图片点阵数据
	GetDIBits( hdc, bmp, 0, bm.bmHeight, pBits, pbmpinfo, DIB_RGB_COLORS );

	//获得文件头
	ZeroMemory( (void *)&bmfHeader, sizeof(BITMAPFILEHEADER) );
	bmfHeader.bfType = * (WORD *) "BM";//'BM'    ((WORD) ('M' << 8) | 'B'); 
	bmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + nBitmapInfoSize + nBitCounts;
	bmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + nBitmapInfoSize;


	//存储到文件中
	file = fopen( strFileName, "wb" );
	if( file == NULL )
	{
		free( pbmpinfo );
		free( pBits );
		return FALSE;
	}

	//写入文件头
	fwrite( &bmfHeader, 1, sizeof(BITMAPFILEHEADER), file );
	//写入图片头
	fwrite( pbmpinfo, 1, nBitmapInfoSize, file );
	//写入点阵数据
	fwrite( pBits, 1, nBitCounts, file );
	fclose( file );

	//释放分配的空间
	free( pbmpinfo );
	free( pBits );
	return TRUE;
}



//保存二代证正面图片
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
		"宋体");
	//::CREATE
	HFONT holdfont= (HFONT)SelectObject( hMemDC, h_font );

	//个人信息字符串
	char szName[ 32 ] = { 0 };
	char bY[10] = { 0 };
	char bM[5] = { 0 };
	char bD[5] = { 0 };

	//其他
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

	//去除文字背景色
	SetBkMode( hMemDC, TRANSPARENT );

	//姓名
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
	
	//性别
	TextOut( hMemDC, 65, 55, personInfo.sex, lstrlen(personInfo.sex) );
	//民族
	TextOut( hMemDC, 130, 55, personInfo.nation, lstrlen(personInfo.nation) );

	//出生日期—年
	memcpy( bY, personInfo.birthday, 4 );
	TextOut( hMemDC, 65, 80, bY, lstrlen(bY) );

	//出生日期—月
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

	//出生日期—日
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

	//写地址
	DrawAddress( personInfo.address, hMemDC );

	//身份证号
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
		"宋体");
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

	//获取背景色颜色值
	col = GetPixel( hMemDC2, 3, 3 );

	//将头像图片背景色去除
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


//保存背面图片
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
		"宋体");
	//::CREATE
	HFONT holdfont= (HFONT)SelectObject( hMemDC, h_font );
	
	//其他
	char szStar[20] = { 0 };
	char szEnd[20] = { 0 };
	char valid[50] = { 0 };
	HBITMAP hBitmap = NULL;
	BOOL bRet = 0;

	SetBkMode( hMemDC, TRANSPARENT );

	//发证机关
	TextOut( hMemDC, 135, 150, pInfo.police, strlen(pInfo.police) );

	//有效期限—起始时间
	memcpy( szStar, pInfo.validStart, 4 );
	lstrcat( szStar, "." );
	memcpy( szStar+5, pInfo.validStart+4, 2 );
	lstrcat( szStar, "." );
	memcpy( szStar+8, pInfo.validStart+6, 2 );

	//有效期限—终止时间
	memcpy( szEnd, pInfo.validEnd, 4 );
	lstrcat( szEnd, "." );
	memcpy( szEnd+5, pInfo.validEnd+4, 2 );
	lstrcat( szEnd, "." );
	memcpy( szEnd+8, pInfo.validEnd+6, 2 );

	//有效期
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
		"宋体");
	//::CREATE
	HFONT holdfont= (HFONT)SelectObject( hMemDC, h_font );

	//其他
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
	COLORREF col; //获取背景色颜色值
	HBITMAP hNew = NULL;
	BOOL bRet = FALSE;
	int i = 0, nTmp = 0;

	//去除文字背景色
	SetBkMode( hMemDC, TRANSPARENT );

	//姓名
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

	//性别
	TextOut( hMemDC, 65, 55, personInfo.sex, lstrlen(personInfo.sex) );
	//民族
	TextOut( hMemDC, 130, 55, personInfo.nation, lstrlen(personInfo.nation) );

	//出生日期—年
	memcpy( bY, personInfo.birthday, 4 );
	TextOut( hMemDC, 65, 80, bY, lstrlen(bY) );

	//出生日期—月
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

	//出生日期—日
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

	//写地址
	DrawAddress( personInfo.address, hMemDC );

	//画水印
	if (bMakeLogo)
	{
		DrawLogo( hMemDC );
	}

	//发证机关
	TextOut( hMemDC, 135, 363, personInfo.police, strlen(personInfo.police) );

	//有效期限—起始时间
	memcpy( szStar, personInfo.validStart, 4 );
	lstrcat( szStar, "." );
	memcpy( szStar+5, personInfo.validStart+4, 2 );
	lstrcat( szStar, "." );
	memcpy( szStar+8, personInfo.validStart+6, 2 );

	//有效期限—终止时间
	memcpy( szEnd, personInfo.validEnd, 4 );
	lstrcat( szEnd, "." );
	memcpy( szEnd+5, personInfo.validEnd+4, 2 );
	lstrcat( szEnd, "." );
	memcpy( szEnd+8, personInfo.validEnd+6, 2 );

	//有效期
	memset( valid, 0, sizeof(valid) );
	sprintf_s( valid, sizeof(valid), "%s-%s", szStar, szEnd );
	TextOut( hMemDC, 135, 390, valid, strlen(valid) );

	//身份证号
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
		"宋体");
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

	//获取背景色颜色值
	col = GetPixel( hMemDC2, 3, 3 );

	//将头像图片背景色去除
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




