#include "LinuxImage.h"
#include <string.h>
#include <dlfcn.h>
#include <unistd.h> //c++
#include <sys/types.h>
#include "Debug.h"
#include "debugLog.h"
typedef unsigned char  uchar;
typedef int (*LPUNPACK)(char *src, char *dst, int bmpSave);

// Î»Í¼µÚÒ»²¿·Ö£¬ÎÄ¼þÐÅÏ¢
typedef struct {
	u_int16_t   bfType;
	u_int32_t   bfSize;
	u_int16_t   bfReserved1;
	u_int16_t   bfReserved2;
	u_int32_t   bfOffBits;
} __attribute__((packed))  BMPFILEHEADER_T;//¸æËß±àÒëÆ÷ÓÅ»¯×Ô¶¯¶ÔÆë


//ÑÕÉ«Öµ
typedef struct{
	uchar  uRed;
	uchar  uGreen;
	uchar  uBlue;
} __attribute__((packed))  RGB_T;

// Î»Í¼µÚ¶þ²¿·Ö£¬Êý¾ÝÐÅÏ¢
typedef struct{
	u_int32_t   biSize;
	u_int32_t   biWidth;
	u_int32_t   biHeight;
	u_int16_t   biPlanes;
	u_int16_t   biBitCount;
	u_int32_t   biCompression;
	u_int32_t   biSizeImage;
	u_int32_t   biXPelsPerMeter;
	u_int32_t   biYPelsPerMeter;
	u_int32_t   biClrUsed;
	u_int32_t   biClrImportant;
} __attribute__((packed))  BMPINFOHEADER_T;


//BGRÊý¾Ý×ª»»³ÉRGBÊý¾Ý
void Convert2RGBData( uchar *pdata, int nWidth, int nHeight, RGB_T *RGBData )
{
	int i = 0;
    for( i = 0; i<nWidth*nHeight; i++ )
    {
        RGBData[i].uRed = pdata[3*i+2];
        RGBData[i].uGreen = pdata[3*i+1];
        RGBData[i].uBlue = pdata[3*i];
    }
}


int SaveBmp(char* szBmpFile, uchar * pdata, int width, int height )
{     
		int  i = 0;

		//·Ö±ðÎªrgbÊý¾Ý£¬Òª±£´æµÄbmpÎÄ¼þÃû£¬Í¼Æ¬³¤¿í
		int size = width*height*3*sizeof(char) + height * 2;// Ã¿¸öÏñËØµã3¸ö×Ö½Ú

		// Î»Í¼µÚÒ»²¿·Ö£¬ÎÄ¼þÐÅÏ¢
		BMPFILEHEADER_T bfh;
		bfh.bfType = (u_int16_t)0x4d42;  //bm
		bfh.bfSize = size  // data size
			  + sizeof( BMPFILEHEADER_T ) // first section size
			  + sizeof( BMPINFOHEADER_T ); // second section size		  
		bfh.bfReserved1 = 0; // reserved
		bfh.bfReserved2 = 0; // reserved
		bfh.bfOffBits = sizeof( BMPFILEHEADER_T )+ sizeof( BMPINFOHEADER_T );//ÕæÕýµÄÊý¾ÝµÄÎ»ÖÃ

		// Î»Í¼µÚ¶þ²¿·Ö£¬Êý¾ÝÐÅÏ¢
		BMPINFOHEADER_T bih;
		bih.biSize = sizeof(BMPINFOHEADER_T);
		bih.biWidth = width;
		bih.biHeight = height;
		bih.biPlanes = 1;//Îª1£¬²»ÓÃ¸Ä
		bih.biBitCount = 24;
		bih.biCompression = 0;//²»Ñ¹Ëõ
		bih.biSizeImage = 0;
		bih.biXPelsPerMeter = 0 ;//ÏñËØÃ¿Ã×
		bih.biYPelsPerMeter = 0 ;
		bih.biClrUsed = 0;//ÒÑÓÃ¹ýµÄÑÕÉ«£¬24Î»µÄÎª0
		bih.biClrImportant = 0;//Ã¿¸öÏñËØ¶¼ÖØÒª

		//±£´æbmpÎÄ¼þ
		
		FILE * fp = fopen( szBmpFile, "wb" );
		//LOGE("SaveBmp :szBmpFile is:%s\n",szBmpFile);
		if( !fp )
			return 0;

		//ÓÉÓÚlinuxÉÏ4×Ö½Ú¶ÔÆë£¬¶øÐÅÏ¢Í·´óÐ¡Îª54×Ö½Ú£¬µÚÒ»²¿·Ö14×Ö½Ú£¬µÚ¶þ²¿·Ö40×Ö½Ú£¬
		fwrite( &bfh, sizeof( BMPFILEHEADER_T ), 1, fp );
		fwrite( &bih, sizeof(BMPINFOHEADER_T), 1, fp );
		
		//×ª»»²¢µÃµ½µÄÍ¼Æ¬Êý¾Ý
		RGB_T *pRGBData = (RGB_T *)malloc( width * height * sizeof(RGB_T) );
		Convert2RGBData( pdata, width, height , pRGBData );
		for( i=0; i<width*height; i++)
		{
			fwrite(pRGBData+i, 1, sizeof(RGB_T), fp);

			if ( (i+1)%width == 0 )//Ã¿ÐÐ½áÊø
			{
				fwrite("\x96\x08", 1, 2, fp);
			}
		}
        LOGI("SaveBmp : close the fp");
		fclose( fp );
		free(pRGBData);
		
		return 1; 
}



/************************************************************************************
* º¯ÊýÃû£ºGetPhotoBmp
*  
* ¹¦ÄÜ£º  ½«wltÎÄ¼þ×ªÎªbmpÎÄ¼þ
*  
*²ÎÊý£º   szSoPath-----soÎÄ¼þÃû
*         szWltData----WltÍ¼Æ¬Êý¾Ý
*         nWltDataLen  ----WltÍ¼Æ¬Êý¾Ý³¤¶È
*		  szImgFile[in]--Í¼Æ¬±£´æÍêÕûÂ·¾¶
*  
*·µ»ØÖµ£º 1---³É¹¦
*         0--Ê§°Ü
************************************************************************************/
int GetPhotoBmp( char *szSoPath, char* szWltData, char* szImgFile )
{
	int nRet = 0;       //·µ»ØÖµ
	LPUNPACK unpack = NULL;
	char *dst = NULL;
	void *hInstLibrary = NULL;
	char  szSoFullPath[128] = {0};
	
	/*!<Æ´½ÓsoÍêÕûÂ·¾¶*/
	getcwd(szSoFullPath,512);
	sprintf( szSoFullPath + strlen(szSoFullPath), "/%s", szSoPath ); 
    LOGE("GetPhotoBmp: szSoFullPathis :%s",szSoFullPath);
	//¼ÓÔØso¿â
	hInstLibrary = dlopen(szSoFullPath, RTLD_NOW);
	if(hInstLibrary != NULL )
	{
		//»ñÈ¡½âÎöº¯Êý
		unpack = (LPUNPACK)dlsym(hInstLibrary, "_Z6unpackPcS_i");
		if( unpack != NULL 
		     &&  NULL != (dst=(char *)malloc( 102 * 126 * 3 )) )
		{
			//½âÎöÍ·ÏñÐÅÏ¢
			nRet = unpack( szWltData, dst, 608 );
			LOGI("unpack: nRet is %d",nRet);
			if(nRet)
			{
				//±£´æÍ¼Æ¬
				nRet = SaveBmp(szImgFile,(unsigned char *)dst, 102, 126 );
				LOGE("SaveBmp: nRet is %d",nRet);
			}
			free( dst );
		}
		LOGE("unpack != NULL");
		dlclose(hInstLibrary);
	}	
    LOGE("hInstLibrary != NULL");
    LOGE("GetPhotoBmp : the nRet is %d",nRet);
	return nRet;
}


