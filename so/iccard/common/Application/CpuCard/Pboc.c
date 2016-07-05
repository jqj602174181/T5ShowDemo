#include <stdlib.h>
#include <string.h>
#include "GlobalDef.h"
#include "ECash.h"
#include "TransTag.h"
#include "Debug.h"
#include "pboc.h"
#include "Des.h"
#include "stdio.h"


#define  DEVICE_KEY_LEN          16
#define  DEVICE_DATA_LEN         16


#ifndef DWORD
typedef unsigned long       DWORD;
#endif

/*<  ��ȡ��¼ʱʹ�õı�ǩ�ṹ�� */
typedef struct
{
    char name[4+1];  
    int flag;  
} TAGS; 

/*! AID ����ı�ʶ */
#define SELECT_FIRST 0x01
#define SELECT_NEXT  0x02

#define TERMINATE                 (-3)                 /*< ������ֹ  */
#define GOTO                      (-4)                 /*< ���ؽ���  */
#define MAXLEN_ISSAPPDATA         (32)                 /*<  ÿ���ű��32���ֽ� */

#define AID_LIST_LEN              1024                 /*<  IC ��Ƭ��AIDӦ���б���Ϣ���� */
static char g_IcAidList[AID_LIST_LEN];                 /*<  IC ��Ƭ��AIDӦ���б���Ϣ */
static int g_nAidListNum;                              /*< AID��ǩ�б�ĸ���  */

static unsigned char s_AidList[16][32+1];              /*< AID�ĳ����32  ��ASC�ַ�  */
static unsigned char s_TcTag[30][3];                   /*< ��ǩ�2���ֽ�  */
static unsigned char s_ArqcTag[30][3];


static unsigned char s_InfoTag[20][3];                /*< ��ȡIC����Ϣ��ǩ  */
static ST_DEVICE_PARAM g_sDeviceParam;                /*< �豸�ṹ������  */
static TERMPARAM g_sParam;                            /*< ���ڽṹ��  */
static ICCARDPARAM g_ICCardParam;                     /*< ���׹������� ȫ�ֽṹ��  */


/*<  ��̬���ݱ�ǩ */
static unsigned char *sStaticTagList[]=
{
    EMV_TAG_9F79_IC_EC_BALANCE,     EMV_TAG_9F4F_IC_PBOCLOGFORMAT,         EMV_TAG_9F13_IC_LASTATC,
    EMV_TAG_9F78_IC_EC_STL,         EMV_TAG_9F6D_IC_EC_RESETTHRESHOLD,     EMV_TAG_9F17_IC_PINTRYCNTR,
    EMV_TAG_9F77_IC_EC_BALANCELIMIT,EMV_TAG_9F36_IC_ATC
};

/*<  ������ǩ�б� */
static unsigned char *s_BaseInfoTag[]=
{
    EMV_TAG_5A_IC_PAN,                 EMV_TAG_5F20_IC_HOLDERNAME,      EMV_TAG_5F20_IC_HOLDERNAME,      
    EMV_TAG_9F62_IC_CERTTYPE,          EMV_TAG_9F61_IC_CERTNO,          EMV_TAG_57_IC_TRACK2EQUDATA,     
    EMV_TAG_5F25_IC_APPEFFECTDATE,     EMV_TAG_5F24_IC_APPEXPIREDATE,   EMV_TAG_5F34_IC_PANSN,           
    EMV_TAG_8C_IC_CDOL1,               EMV_TAG_8D_IC_CDOL2,             EMV_TAG_9F08_IC_APPVERNO
    
};

/*<  TCĬ�ϱ��� */
static unsigned char *s_TcTagDefault[]=
{
	EMV_TAG_9F33_TM_CAP,            EMV_TAG_95_TM_TVR,              EMV_TAG_9F37_TM_UNPNUM,         
	EMV_TAG_9F1E_TM_IFDSN,          EMV_TAG_9F10_IC_ISSAPPDATA,     EMV_TAG_9F26_IC_AC,          
	EMV_TAG_9F36_IC_ATC,            EMV_TAG_82_IC_AIP,              EMV_TAG_DF31_IC_IISSCRIRES,     
	EMV_TAG_9F1A_TM_CNTRYCODE,      EMV_TAG_9A_TM_TRANSDATE
};

/*<  ARQCĬ�ϱ��� */
static unsigned char *s_ArqcTagDefault[]=
{
	EMV_TAG_9F26_IC_AC,             EMV_TAG_9F27_IC_CID,           EMV_TAG_9F10_IC_ISSAPPDATA,   
	EMV_TAG_9F37_TM_UNPNUM,         EMV_TAG_9F36_IC_ATC,           EMV_TAG_95_TM_TVR,  
	EMV_TAG_9A_TM_TRANSDATE,        EMV_TAG_9C_TM_TRANSTYPE,       EMV_TAG_9F02_TM_AUTHAMNTN ,    
	EMV_TAG_5F2A_TM_CURCODE,        EMV_TAG_82_IC_AIP,             EMV_TAG_9F1A_TM_CNTRYCODE,
	EMV_TAG_9F03_TM_OTHERAMNTN,     EMV_TAG_9F33_TM_CAP,           EMV_TAG_9F34_TM_CVMRESULT, 
	EMV_TAG_9F35_TM_TERMTYPE,       EMV_TAG_9F1E_TM_IFDSN ,        EMV_TAG_84_IC_DFNAME,           
	EMV_TAG_9F09_TM_APPVERNO,       EMV_TAG_9F41_TM_TRSEQCNTR
};


/*!
 * \brief        Pboc_SetAidNum      
 * \par ˵��     ����AID�б�ĸ���      
 * \param[in]    nNum  AID�б����          
 * \retval       0 �ɹ� -1  ʧ��
 * \note                
 */
void Pboc_SetAidNum( int nNum )
{
    g_nAidListNum = nNum;
}

/*!
 * \brief        Pboc_GetAidNum       
 * \par ˵��     ��ȡAID�б�ĸ���      
 * \param[out]   nNum    ��ǰAID�б�ĸ���
 * \retval       0 �ɹ� -1  ʧ��
 * \note                
 */
void Pboc_GetAidNum( int * nNum )
{
    *nNum = g_nAidListNum;
}

/*!
 * \brief        Pboc_Meth_AidList      
 * \par ˵��     AID�б�ѡ��          
 * \retval       0 �ɹ� -1  ʧ��
 * \note         ��ʱֻѡ��һ��      
 */
static int Pboc_Meth_AidList( int *nAIDNum )
{
    int nRecSum = 0;
    int nRet = 0;
    int i = 0;
    int nAidHexLen  = 0;
    int nFciLen  = 0;
    int nBufLen = 0;
    char cSelMode = SELECT_FIRST;
    char sAidHex[ 16+1 ] = { 0x00 };
    char c6D00Flag = 0;
    char sFci[ 256 ] = { 0x00 };
    char sBuf[ 1024 ] = { 0x00 };

    /**<��ȡ�ն�֧�ֵ�AID������*/
	DbgPrintf("��AID�б�ѡ��\n");
    Pboc_GetAidNum( &nRecSum );
    DbgPrintf( "��AID�б�ѡ�� ��ȡ�ն�AID��������Ϊ: %d\n",nRecSum );

    for( i = 0; i < nRecSum; i++ )
    {
        /**<���ն�֧��AIDΪDF������ѡ��*/
        memset( sAidHex, 0x00, sizeof(sAidHex) );
        nAidHexLen = strlen( s_AidList[i] )/2;
        AsciiToHex( s_AidList[i], strlen(s_AidList[i]), sAidHex );
        
        nRet = ICC_AppSelect( sAidHex, nAidHexLen, sFci, &nFciLen );
        DbgPrintf( "��AID�б�ѡ�� ѡ��AID %s ����ֵ:%2x\n", s_AidList[i], nRet );       
        
        if( nRet < 0 )
        {
            i++;
            cSelMode = SELECT_FIRST;
            continue;   
        }

        nRet = ICC_CardCmdGetErrCode();
        switch(nRet)
        {
            case 0x9000:
            {
					*nAIDNum = i+1;
                /*< ��������   6F 84  A5  50 87 9F38 5F2D 9F11 9F12 BF0C */
                memset(sBuf,0x00,sizeof(sBuf));
                nBufLen = 0;
                nRet = centerm_get_tag_value( EMV_TAG_9F38_IC_PDOL, 2, sFci, nFciLen, sBuf,&nBufLen );
                if ( ( nRet == SUCC ) && ( nBufLen > 0 ) )
                {
                    tFunc_SaveTlvData( EMV_TAG_9F38_IC_PDOL, 2, sBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
                }
                
                /*<  ���� A5 */
                memset( sBuf, 0x00, sizeof(sBuf) );
                nBufLen = 0;
                nRet = centerm_get_tag_value( EMV_TAG_A5_IC_FCIPROPTEMP, 1, sFci, nFciLen, sBuf, &nBufLen );
                if ( ( nRet == SUCC ) && (nBufLen > 0 ))
                {
                    tFunc_SaveTlvData(EMV_TAG_A5_IC_FCIPROPTEMP,1,sBuf,nBufLen,&g_ICCardParam.nIccAppDataLen,g_ICCardParam.szIccAppData);
                }

                /*<  ���� BF0C */
                memset( sBuf, 0x00, sizeof(sBuf) );
                nBufLen = 0;
                nRet = centerm_get_tag_value( EMV_TAG_BF0C_IC_FCIDISCDATA, 2, sFci, nFciLen, sBuf, &nBufLen );
                if ( ( nRet == SUCC ) && ( nBufLen > 0 ))
                {
                    tFunc_SaveTlvData( EMV_TAG_BF0C_IC_FCIDISCDATA, 2, sBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
                }
                
                return SUCC;
            }
                
            case 0x6283:
            {
                /**<DF����AID��ͬ���ն�ʹ�����б��е���һ��AID��Ƭ����SELECT����*/
            
                /**<��ͬ����DF��ѡ���¸�Ӧ��*/
                continue;
            }
            
            case 0x6A81:
            {
                if(i == 0 && cSelMode == SELECT_FIRST)
                {
                    /**<��һ��ѡ��ͷ��ؿ�Ƭ��������
                    ���ն˽�����Ƭ����*/
                    return RET_AIDLIST_LOCK;
                }
                else
                {
                    i++;
                    cSelMode = SELECT_FIRST;
                    continue;
                }
            }
            
            case 0x9303: /*< Ӧ����Զ���� ����Ҫ��֤  */
            {
                return  RET_CARD_LOCK_FOREVER;
            }

            case 0x6D00:
            {
                c6D00Flag++;
                i++;
                cSelMode = SELECT_FIRST;
                break;
            }
            
            default:
            {
                i++;
                cSelMode = SELECT_FIRST;
                continue;
            }
        }       
    }

    return SUCC;
}

/*!
 * \brief               Pboc_GetGACData
 * \par ˵��            У�鲢����GAC����ص�����
 * \param[in]           char *pszData:    GAC����ص����� 
 * \param[in]           int nLen:         ���ݳ��� 
 * \retval              0 �ɹ� -1  ʧ��
 * \note                
 */
int Pboc_GetGACData( char *pszData, int nLen )
{
    char szBuf[ 1024 ] = { 0 };
    int nBufLen = 0;
    int nRet    = FAIL;
    int nNum    = 0;
    int nIndex  = 0;
    char cCid   = 0;
    
	DbgPrintf("������GAC��\n");

    /*<  80ģ�� ����˳��������� */
    if( !memcmp( pszData, EMV_TAG_80_IC_RMTF1, strlen(EMV_TAG_80_IC_RMTF1) ) )
    {
        centerm_get_tag_value( EMV_TAG_80_IC_RMTF1, strlen(EMV_TAG_80_IC_RMTF1), pszData, nLen, (BYTE *)szBuf, &nBufLen );
        if( nBufLen < 11 || nBufLen > (11 + MAXLEN_ISSAPPDATA) )
        {
            return FAIL;
        }
        
        tFunc_SaveTlvData( EMV_TAG_9F27_IC_CID, 2, szBuf,   1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F36_IC_ATC, 2, szBuf+1, 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F26_IC_AC,  2, szBuf+3, 8, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        
        if( nBufLen > 11 )
        {
            tFunc_SaveTlvData( EMV_TAG_9F10_IC_ISSAPPDATA, 2, szBuf+11, nBufLen-11, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        }
    }
    /*<  77ģ��  */
    else if( !memcmp( pszData, EMV_TAG_77_IC_RMTF2, 1 ) )
    {
        /**< CID������� */
        nRet = centerm_get_tag_value( EMV_TAG_9F27_IC_CID, 2, pszData, nLen, szBuf, &nBufLen );
        if( nRet != SUCC || nBufLen == 0 )
        {
            return FAIL;
        }
        
        tFunc_SaveTlvData( EMV_TAG_9F27_IC_CID, 2, szBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        
        /**< ATC������� */
        nRet = centerm_get_tag_value( EMV_TAG_9F36_IC_ATC, 2, pszData, nLen, szBuf, &nBufLen );
        if( nRet != SUCC || nBufLen == 0 )
        {
            return FAIL;
        }
        tFunc_SaveTlvData( EMV_TAG_9F36_IC_ATC, 2, szBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*<  ����Ӧ���������� */
        memset( szBuf, 0, sizeof(szBuf) );
        nBufLen = 0;
        nRet = centerm_get_tag_value( EMV_TAG_9F26_IC_AC, 2, pszData, nLen, szBuf, &nBufLen );
        if( nRet != SUCC || nBufLen == 0 )
        {
            return FAIL;
        }
        tFunc_SaveTlvData( EMV_TAG_9F26_IC_AC, 2, szBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*<  ���淢�����Զ������� */
        memset( szBuf, 0, sizeof(szBuf) );
        nBufLen = 0;
        nRet = centerm_get_tag_value( EMV_TAG_9F10_IC_ISSAPPDATA, 2, pszData, nLen, szBuf, &nBufLen );
        if( nRet != SUCC || nBufLen == 0 )
        {
            return FAIL;
        }
        tFunc_SaveTlvData( EMV_TAG_9F10_IC_ISSAPPDATA, 2, szBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        
    }
    else
    {
        return FAIL;
    }
    DbgPrintf("������GAC�� �ɹ�\n");
    return SUCC;
}

 /*!
 * \brief           Pboc_FormatLog
 * \par ˵��        ���׸�ʽ
 * \param[in]       pszFmt   -- ���׸�ʽ
 * \param[in]       nLen     -- ���׸�ʽ�ĳ���
 * \param[in]       pszData  -- ��¼����
 * \param[in]       nDataLen -- ��¼���ݵĳ���
 * \param[out]      ST_PBOC_DETAIL * stPbocDetail -- ת����Ľ��׽ṹ��
 * \retval          0 -- �ɹ�   -1 ʧ��; 
 * \note                  
 */
int Pboc_FormatLog( char *pszFmt, int nLen, char *pszData, int nDataLen, ST_PBOC_DETAIL *stPbocDetail )
{
    int i = 3;
    int nOff = 0;

    DbgPrintf( "����ȡ��־��ʽ�� \n" );
    DbgPrintHex( "����ȡ��־��ʽ�� ��־��ʽ", pszFmt, nLen );
    DbgPrintHex( "����ȡ��־��ʽ�� ��־����", pszData, nDataLen );

    memset( stPbocDetail, 0x00, sizeof(ST_PBOC_DETAIL) );
    
    /*< ��������ΪȦ������  */
    if( !memcmp( pszFmt, EMV_TAG_DF4F_UPLOAD_FORMAT, 2 ) )
    {
        if( ( ( !memcmp( pszData, EMV_TAG_DF79_IC_DOUBLE_CUR_EC_BALANCE, 2 ) ) || ( !memcmp(pszData,EMV_TAG_9F79_IC_EC_BALANCE,2) ) ) && ( nDataLen >= 0x0e ) )
        {
            memcpy( stPbocDetail->sAmtType,   pszData,   2 );     /*< ��������  */
            memcpy( stPbocDetail->sBeforeAmt, pszData+2, 6 ); /*< Ȧ��ǰ��� */
            memcpy( stPbocDetail->sAfterAmt,  pszData+8, 6 );  /*< Ȧ����� */
            nOff = 14;
        }
        else
        {
            return FAIL;
        }
    }

    if( pszFmt[2] == 0x00 )
    {
        return SUCC;
    }

    while ( i < nLen ) 
    {
        switch ( pszFmt[ i++ ] ) 
        {
            case '\x9A': /*< ��������;  */
            {
                if( pszFmt[i] != 0x03 ) 
                {
                    return FAIL;
                }
                
                memcpy( stPbocDetail->szDate, pszData+nOff, pszFmt[i] );
                DbgPrintHex( "����ȡ��־��ʽ�� ��������\n", stPbocDetail->szDate, 3 );
                
                break;
            }
            
            case '\x9C': /*< ��������; */
            {
                if ( pszFmt[i] != 0x01 ) 
                {
                    return FAIL;
                }
                stPbocDetail->cTranceType[ 0 ] = *(pszData+nOff);
                DbgPrintHex( "����ȡ��־��ʽ�� ��������", stPbocDetail->cTranceType, 1 );
                
                break;
            }
            
            case '\x9F':
            {
                switch ( pszFmt[i++] )
                {
                    case '\x21': /*< ����ʱ��  */
                    {
                        if ( pszFmt[i] != 0x03 ) 
                        {
                            return FAIL;
                        }
                        
                        memcpy( stPbocDetail->szTime,pszData+nOff,pszFmt[i] );
                        DbgPrintHex( " ����ȡ��־��ʽ�� ����ʱ��", stPbocDetail->szTime, 3 );
                        
                        break;
                    }
                    
                    case '\x02': /*< ��Ȩ���  */
                    {
                        if ( pszFmt[i] != 0x06 )
                        {
                            return FAIL;
                        }
                        memcpy( stPbocDetail->szAmount, pszData+nOff, pszFmt[i] );
                        DbgPrintHex( " ����ȡ��־��ʽ�� ��Ȩ���", stPbocDetail->szAmount, 6 );
                        
                        break;
                    }
                    
                    case '\x03': /*< �������  */
                    {
                        if ( pszFmt[i] != 0x06 ) 
                        {
                            return FAIL;
                        }
                        
                        memcpy( stPbocDetail->szOtherAmount, pszData+nOff, pszFmt[i] );
                        DbgPrintHex( " ����ȡ��־��ʽ�� �������", stPbocDetail->szAmount, 6 );
                        
                        break;
                    }
                    
                    case '\x1A': /*< �ն˹��Ҵ���  */
                    {
                        if ( pszFmt[i] != 0x02 ) 
                        {
                            return FAIL;
                        }
                        
                        memcpy( stPbocDetail->szCountyCode, pszData+nOff, pszFmt[i] );
                        DbgPrintHex( "����ȡ��־��ʽ�� �ն˹��Ҵ���", stPbocDetail->szCountyCode, 2 );
                        break;
                    }
                    
                    case '\x4E': /*< �̻�����  */
                    {
                        if ( pszFmt[i] > 0x20 ) 
                        {
                            return FAIL;
                        }
                        
                        memset( stPbocDetail->szMerchen, 0x20, 20 );
                        memcpy( stPbocDetail->szMerchen, pszData+nOff, pszFmt[i] );
                        DbgPrintf( "����ȡ��־��ʽ�� �̻�����", stPbocDetail->szMerchen );
                        
                        break;
                    }
                    
                    case '\x36': /*< ���׼����� */
                    {
                        if ( pszFmt[i] != 0x02 ) 
                        {
                            return FAIL;
                        }
                        
                        memcpy( stPbocDetail->szATC, pszData+nOff, pszFmt[i] );
                        DbgPrintHex( "����ȡ��־��ʽ�� ���׼�����", stPbocDetail->szATC, 2 );
                        
                        break;
                    }
                    
                    default:
                    {
                        return FAIL;
                    }
                }
                break;
            }
            
            case '\x5F':
            {
                if ( pszFmt[i] == 0x2A ) /*< ���׻��Ҵ���  */
                {
                    i++;
                    if (pszFmt[i] != 0x02) 
                    {
                        return FAIL;
                    }
                    
                    memcpy( stPbocDetail->szCurCode, pszData+nOff, pszFmt[i] );
                    DbgPrintHex( "����ȡ��־��ʽ�� ���׻��Ҵ���", stPbocDetail->szCurCode, 2 );
                }
                break;
            }
            
            default:
            {
                return FAIL;
            }
            
        }// switch
        
        nOff += pszFmt[i];  
        i++;        
        
    }//while    
    return SUCC;
}

/*!
 * \brief       Pboc_IsAIDMatch       
 * \par ˵��    �鿴AID�Ƿ���AID�б���      
 * \param[in]   pszICCAID      ��Ҫƥ���AID    
 * \param[in]   nICCAIDLen     AID�����ݳ���       
 * \param[out]  nIndex         ƥ�������       
 * \retval      YES ƥ��  NO��ƥ��
 * \note        ������ȫƥ��ķ�ʽ       
 */
YESORNO Pboc_IsAIDMatch( char *pszICCAID, int nICCAIDLen, int *nIndex )
{
    int i = 0;
    int nNum = 0;
    int nTermAIDLen = 0;
    int nAidHexLen = 0;
    char sAidHex[16+1] = {0x00};


    DbgPrintHex( "��AIDƥ�䡿\n", pszICCAID, nICCAIDLen );

    Pboc_GetAidNum( &nNum );
    DbgPrintf( "��AIDƥ�䡿 nNum:%d\n", nNum );

    for( i = 0; i< nNum; i++ )
    {
        AsciiToHex( s_AidList[i] , strlen(s_AidList[i]), sAidHex );

        if( !memcmp(sAidHex,pszICCAID,nICCAIDLen) )
        {
            *nIndex = i;
            DbgPrintf( "��AIDƥ�䡿 ƥ��ɹ���\n" );
            
            return YES;
        }
    }
    
    DbgPrintf( "��AIDƥ�䡿 ƥ��ʧ�ܣ�\n" );   
    return NO;
}

/*!
 * \brief         Pboc_ReadDataByTags      
 * \par ˵��      �ӿ�Ƭ�ڲ���ȡ��¼��Ϣ    
 * \param[in]     nDirect  0  ��ͷ��βɨ��  1  ��β��ͷɨ��     
 * \param[in]     pTags       ��Ҫ��ȡ��TAG��ǩ    
 * \param[in]     nTagCount   TAG�ı�ǩ����
 * \param[in]     szAfl       AFL���
 * \param[in]     nAflLen     AFL�ĳ���
 * \param[out]    szAppData   ��¼��Ϣ
 * \param[out]    pnAppDataLen��¼��Ϣ�ĳ���
 * \retval        0 �ɹ� -1  ʧ��
 * \note                
 */
int Pboc_ReadDataByTags( int nDirect, TAGS *pTags, S32 nTagCount, U8* szAfl, S32 nAflLen, U8 *szAppData, S32 *pnAppDataLen  )
{

    S32 i = 0;
    S32 j = 0;
    S32 n = 0;
    S32 count = 0;
    S32 use = 0;
    S32 nDataLen = 0;
    S32 nBufLen = 0;
    S32 start = 0;
    S32 all   = 0;
    S32 nRet = 0;
    S32 nValueLen = 0;
    U8  szData[ MAX_LEN ] = { 0 };
    U8 szValue[MAX_LEN] = {0x00};
    U8 szBuf[MAX_LEN] = {0x00};

    DbgPrintf( "����ȡӦ�����ݡ�\n");
    (*pnAppDataLen ) = 0;

    //��ͷɨ��β
    if( nDirect == 0 )
    {
        start = 0;
        all   = nAflLen/4;
    }
    //��βɨ��ͷ
    else 
    {
        start = nAflLen/4-1;
        all   = 0;        
    }

    for( i = start; ( ( nDirect == 0 ) ? i < all : i >= 0 ); ( ( nDirect == 0 ) ?  i++ : i-- ) )
    {
        if( ( szAfl[4*i] < 1 ) || ( szAfl[4*i] > 100 ) )
        {
            continue;
        }
        j = nDirect ? szAfl[4*i+1] : szAfl[4*i+2];
        for( ;  (  nDirect ? j <= szAfl[4*i+2] : j >= szAfl[4*i+1] );  (nDirect ?  j++ : j-- ) )
        {

            memset( szData, 0x00, sizeof(szData) );
            nDataLen = 0;
            use = 0;

            if( RET_SUCCESS !=  ICC_CardCmdReadSFI(   szAfl[4*i]>>0x03, j,  szData,  &nDataLen ) )
            {
                DbgPrintf("����ȡӦ�����ݡ� %s\n", ICC_CardCmdShowLastErrMsg() );
                continue;
            }
            else 
            {
                DbgPrintHex("����ȡӦ�����ݡ�", (U8 *)szData,  nDataLen );
                for( n=0; n < nTagCount; n++ )
                {

                    if( 1 != pTags[n].flag )
                    {
                        DbgPrintf("n=%d\n", n );
                        if( centerm_find_tag( (U8 *)pTags[n].name, strlen(pTags[n].name), szData,  nDataLen ) )
                        {
                            DbgPrintf("�������ݣ�\n");
                            use = 1;//�ҵ���������
                            pTags[n].flag = 1;
                        }
                    }

                    /*<  �����¼�е����� */
                    if( 1 == use )
                    {
                        DbgPrintf("����ȡӦ�����ݡ� �����������ݣ�\n");

                        memset( szValue, 0x00, sizeof(szValue));
                        nValueLen = 0;
                        nRet  = centerm_get_tag_value( EMV_TAG_70_TEMP, 1, szData, nDataLen, szValue, &nValueLen);

                        if ( ( nRet == RET_SUCCESS ) && ( nValueLen>0 ) )
                        {
                            /*<  �������ݵ�ȫ�������� */
                            memset( szBuf, 0x00, sizeof(szBuf));
                            nBufLen = 0;
                            nRet = centerm_get_tag_value( pTags[n].name, strlen(pTags[n].name), szValue, nValueLen, szBuf, &nBufLen);
                            if ( ( nRet == RET_SUCCESS ) && ( nBufLen >0 ) )
                            {
                                tFunc_SaveTlvData( pTags[n].name, strlen(pTags[n].name), szBuf, nBufLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData);
                            }
                        }

                    }
                }
            }

            if ( use == 1 )
            {
                memcpy( szAppData+*pnAppDataLen, szValue, nValueLen);
                *pnAppDataLen += nValueLen;
            }
            //����Ƿ��Ѿ�������tag��ȡ����
            count = 0;
            for( n=0; n<nTagCount; n++ )  
            {
                if( 1 == pTags[n].flag )
                {
                    count++;
                }
            }       
            DbgPrintf("count %d nTagCount %d\n", count, nTagCount );              
            if( count == nTagCount )
            {
                //��Ҫ��ȡ��tag�����Ѿ���ȡ���
                return  RET_SUCCESS;   
            }

        }

    }

    DbgPrintHex("����ȡӦ�����ݡ� ��ȡ��Ӧ������Ϊ", (U8 *)szAppData, *pnAppDataLen );  

    return RET_SUCCESS;
}

/*!
 * \brief       Pboc_SaveBasicTagData       
 * \par ˵��    �����������л�ȡ�����Ľ���TAGֵ�������б���     
 * \param[in]   sInData    ����Դ       
 * \param[in]   nInLen     ����Դ����           
 * \retval      0 �ɹ� -1  ʧ��
 * \note                
 */
void Pboc_SaveBasicTagData(char * sInData,int nInLen)
{
    int i = 0;
    int nLen =0;
    int nRet = 0;
    char sData[1024] = {0x00};

    for ( i= 0; i<TABLESIZE(s_BaseInfoTag); i++)
    {
        nLen = 0;
        memset( sData, 0x00, sizeof(sData));
        nRet = centerm_get_tag_value( s_BaseInfoTag[i], strlen(s_BaseInfoTag[i]), sInData, nInLen, sData, &nLen);
        if ((nRet == RET_SUCCESS) && (nLen>0))
        {
            tFunc_SaveTlvData( s_BaseInfoTag[i], strlen(s_BaseInfoTag[i]), sData, nLen,  &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData);
            DbgPrintHex("Save Data\n", g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen);
        }
    }
}


/*!
 * \brief        Pboc_ReadRecordAll      
 * \par ˵��     ��ȡ���еļ�¼�ļ�������������     
 * \param[in]    szAfl        ��¼���      
 * \param[in]    nAflLen      AFL����      
 * \param[out]   szAppData    ��¼����
 * \param[out]   pnAppDataLen ��¼���ݵĳ���    
 * \retval       0 �ɹ� -1  ʧ��
 * \note                
 */
S32 Pboc_ReadRecordAll( U8* szAfl, S32 nAflLen, U8 *szAppData, S32 *pnAppDataLen  )
{

    S32 i = 0;
    S32 j = 0;
    S32 nRet = 0;
    S32 nDataLen = 0;
    S32 nBufLen = 0;
    U8  szData[ MAX_LEN ] = { 0 };
    U8  szBuf[MAX_LEN] = {0x00};

    DbgPrintf( "��Ӧ������......\n");
    ( *pnAppDataLen ) = 0;    

    for( i = 0; i < ( nAflLen/4 ); i++ )
    {
        if( ( szAfl[4*i] < 1 ) || ( szAfl[4*i] > 100 ) )
        {
            continue;
        }

        for( j = szAfl[4*i+1];  j <= szAfl[4*i+2];  j++ )
        {

            memset( szData, 0x00, sizeof(szData) );
            nDataLen = 0;

            if( RET_SUCCESS !=  ICC_CardCmdReadSFI(   szAfl[4*i]>>0x03, j,  szData,  &nDataLen ) )
            {
                DbgPrintf("��Ӧ�����ݴ��� %s\n", ICC_CardCmdShowLastErrMsg() );
                continue;
            }
            else 
            {
                DbgPrintHex("Ӧ������", szData,  nDataLen );
            }
            
            /*<  �������н��� 70 ģ�� */
            memset( szBuf, 0x00, sizeof(szBuf));
            nBufLen = 0;
            nRet  = centerm_get_tag_value( EMV_TAG_70_TEMP, 1,  szData, nDataLen, szBuf, &nBufLen);
            if (nRet == RET_SUCCESS)
            {
                 Pboc_SaveBasicTagData( szBuf, nBufLen);
            }
            
            memcpy( szAppData + *pnAppDataLen, szBuf, nBufLen);
            *pnAppDataLen += nBufLen;
        }
    }

    DbgPrintHex("��ȡ��Ӧ������Ϊ", szAppData, *pnAppDataLen );  

    return RET_SUCCESS;
}

/*!
 * \brief           tFunc_ReadRecordData   
 * \par ˵��        ��ȡ��Ƭ�еļ�¼�ļ�    
 * \param[in]       nType      0 ����Ƭ�еļ�¼����ȡ����   1 ֻ�Ƕ�ȡ��Ҫ��TAG   2 ֻ�Ƕ�ȡ 8C 8D
 * \param[in]       szAppTag   ��ȡ�ı�ǩ�б�   
 * \param[in]       nCount     ��ȡ�ı�ǩ�б�ĸ���
 * \param[out]      pnValueLen ��ȡ���ļ�¼���ݳ���
 * \param[out]      sTagValue  ��ȡ���ļ�¼����
 * \retval          0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_ReadRecordData(int nType,char * szAppTag[],int nCount,int * pnValueLen,char * sTagValue)
{
    int nRet = 0;
    int nAflLen = 0;
    int i = 0;
    int j = 0;
    int nNum = 0;
    char sAfl[128] = {0x00};
    TAGS *pstTag;
    char sBuf[1024] = {0x00};
    int nBufLen =0;
    char sRecordData[4096] = {0x00};
    int nRecordLen =0;

    
    ASSERT_RET( centerm_get_tag_value( EMV_TAG_94_IC_AFL, 1, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, sAfl, &nAflLen ) );

    if(nType == 0) /*< ȫ����ȡ  */
    {
        nRet =  Pboc_ReadRecordAll( sAfl, nAflLen, sRecordData, &nRecordLen );
        DbgPrintHex("App_tFunc_ReadRecordAll \n", sRecordData, nRecordLen );
    }
    else if (nType == 1) /*<  ֻ��ȡ��Ҫ��TAG����  */
    {
        
        if ( nCount>=1 )
        {
            pstTag = ( TAGS  *)malloc( nCount *sizeof(TAGS) );
        }
        for ( i=0; i < nCount ; i++)
        {
            memset( &pstTag[i], 0x00, sizeof(TAGS));
            memcpy( pstTag[i].name, szAppTag[i], strlen(szAppTag[i]));
            pstTag[i].flag = 0;
            /*<  �鿴��������ı�ǩ�Ƿ�Ϊ��̬���ݣ����Ϊ��̬���ݣ��򲻽��л�ȡ */
            for ( j = 0; j<TABLESIZE(sStaticTagList); j++ )
            {
                if ( memcmp( szAppTag[i], sStaticTagList[j], strlen( sStaticTagList[j] ) ) == 0 )
                {
                    pstTag[i].flag = 1;
                }
            }
        }
        DbgPrintf("Read Tag Count %d\n", nCount);
        nRet =  Pboc_ReadDataByTags( 0, pstTag,nCount, sAfl, nAflLen, sTagValue, pnValueLen );
        DbgPrintHex("Pboc_ReadDataByTags sTagValue\n", sTagValue, *pnValueLen);

    }
    else if(nType == 2) /*<  ֻ��ȡCDOL1  CDOL2 */
    {
        nRet = ICC_TransReadCDOL( sAfl, nAflLen,sTagValue, pnValueLen );
    }
    else
    {
        DbgPrintf("tFunc_GetRecordData  param err\n");
        return RET_PARAM_ERR;
    }

    return nRet;
}

/*!
 * \brief           Pboc_GetIcStaticData   
 * \par ˵��        ��ȡ��̬����    
 * \param[in]       sTag         ��Ҫ��ȡ��̬���ݵ�tag      
 * \param[in]       nTagLen      ��ǩ�ĳ���
 * \param[out]      pRecordData  ����TLV��ʽ������ֵ
 * \param[out]      pRecordLen   ���ݵĳ���
 * \retval          0 �ɹ� -1  ʧ��
 * \note                
 */
int Pboc_GetIcStaticData( char *sTag,  int nTagLen, char * pRecordData,int  * pRecordLen )
{

    return ICC_CardCmdGetData( sTag[0], sTag[1], pRecordData,  pRecordLen );

}

/*!
 * \brief          Pboc_PackDomain        
 * \par ˵��       ���55 ��TC����    
 * \param[in]      nType   0 ���55����    1 ���TC����
 * \param[out]     nOutLen    ������ĳ���
 * \param[out]     sOutData   �����������
 * \retval         0 �ɹ� -1  ʧ��
 * \note                
 */
int  Pboc_PackDomain(int nType,int * nOutLen ,char * sOutData)
{
    int i = 0;
    int nRet = 0;
    int nDataLen = 0;
    int nNum = 0;
    int nBufLen = 0;
    int nPointLen  = 0;
    int nTagLen = 0;
    char sBuf[1024] = {0x00};
    char sData[1024] = {0x00};
    char sTag[3+1] = {0x00};

    /*<  ��֯55�� �� TC ���� ��Щ����Ԫ����ǿ�ƣ������������� */ 
    if (nType == 0)
    {
        nNum = TABLESIZE( s_ArqcTag );
    }
    else
    {
        nNum = TABLESIZE( s_TcTag );
    }
    
    for ( i = 0; i<nNum; i++)
    {
        /*< Ϊ��  */
        memset( sTag, 0x00, sizeof(sTag));
        if ( nType == 0 )
        {
            nTagLen = strlen( s_ArqcTag[i] );
            if ( nTagLen >0 )
            {
                memcpy( sTag, s_ArqcTag[i], nTagLen);
            }
        }
        else
        {
            nTagLen = strlen( s_TcTag[i] );
            if ( nTagLen >0 )
            {
                memcpy( sTag, s_TcTag[i], nTagLen);
            }
        }
        
        if ( sTag[0] == 0x00 )
        {
            break;
        }
        DbgPrintHex("Pboc_PackDomain\n", sTag, nTagLen);
        DbgPrintf("Len %d\n", nTagLen);

        memset( sBuf, 0x00, sizeof(sBuf) );
        nBufLen = 0;
        nRet = centerm_get_tag_value( sTag, nTagLen, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, sBuf, &nBufLen );
        DbgPrintHex("data\n", sBuf, nBufLen);
        if ( nRet == RET_SUCCESS )
        {
            nDataLen = 0;
            memset( sData, 0x00, sizeof(sData) );
            nRet =  centerm_packtTag( nTagLen, sTag, nBufLen, sBuf, &nDataLen, sData );
            if (nRet == RET_SUCCESS)
            {
                memcpy( sOutData + nPointLen, sData, nDataLen );
                nPointLen += nDataLen;
            }
        }
        else
        {
            DbgPrintHex("get tag fail :\n ", sTag, nTagLen);
			if ( memcmp( sTag, EMV_TAG_9F26_IC_AC, 2) == 0 || \
					( memcmp( sTag, EMV_TAG_9F27_IC_CID, 2) == 0) || \
					( memcmp( sTag, EMV_TAG_9F10_IC_ISSAPPDATA, 2) == 0) )
			{
				*nOutLen = nPointLen;
				nRet = RET_ERROR;
			}
            
        }
    }

    *nOutLen = nPointLen;
    return SUCC;
}



/*!
 * \brief         tFunc_ConvertData     
 * \par ˵��      ת������  �������źͶ��ŵ�Ч���ݽ���ת����
 * \param[in]     szTagvalue [IN] ��Ҫת��������   [OUT] ת���������   
 * \param[in]     ValueLen   [IN] ת��ǰ���ݳ���   [OUT] ת��������ݳ���       
 * \retval       
 * \note                
 */
void  tFunc_ConvertData( unsigned char* szTagvalue, int *ValueLen )
{
    int nRetVal = RET_SUCCESS;
	unsigned char szTmp[ MAX_LEN ] = {0x00};
	unsigned char uszBuf[ MAX_LEN ] = {0x00};
    int i = 0;
    int nLen = 0;
    int nCount = 0;
    
    nLen = *ValueLen;
    memset( szTmp, 0x0, sizeof(szTmp) );
    memcpy( uszBuf, szTagvalue, nLen);

    for( i=0; i<nLen; i++ )
    {
        if(uszBuf[i] == 'F')
        {
            break;
        }
        if(uszBuf[i] == 'D')
        {
            uszBuf[i]= '=';
        }
    }         
    *ValueLen = i;
    memcpy( szTagvalue, uszBuf, i );    
}



/*!
 * \brief         Pboc_OrgLogDetail     
 * \par ˵��      ����ͨ�ø�ʽ��֯����      
 * \param[in]     stIndata   ST_PBOC_DETAIL�ṹ������   
 * \param[in]     nNum       �ӿ�Ƭ�ж�ȡ������־������    
 * \param[out]    sOutData   ��֯������ݸ�ʽ 
 * \retval        0 �ɹ� -1  ʧ��
 * \note          ��¼����2�ֽ�+ÿ����¼����3�ֽ�+ TLV(1�ֽڱ�ǩ+3�ֽڳ���+����)��ʽ��¼
 ���磺
 03106P012000000022200Q012000000000000R0040156S006141120T00260U006163101V0040156W020CHINA ZHENGZHOU BANKX0040007P012000000299700Q012000000000000R0040156S006141120T00260U006163025V0040156W020CHINA ZHENGZHOU BANKX0040005P012000000033300Q012000000000000R0040156S006141120T00260U006162958V0040156W020CHINA ZHENGZHOU BANKX0040002
 */
int Pboc_OrgLogDetail(ST_PBOC_DETAIL stIndata,int nNum, char * sOutData)
{
    int nLen = 0;
    unsigned char sBuf[40+1] = {0x00};


    /*<  ��¼��+ ÿ����¼���ȣ��̶��� */
	memset( sBuf, 0x00, sizeof(sBuf) );
    sprintf( sBuf, "%02d", nNum );
    sprintf( sBuf+2, "%03d", 106 );
	memcpy( sOutData, sBuf, 5);

    nLen = ( nNum - 1 )*106 + 5;

    /*< ��Ȩ���P  */    
    sOutData[nLen++] = 'P';
    sprintf( sOutData + nLen, "%03d", 12 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szAmount, 6 ,sBuf );
	memcpy( sOutData + nLen, sBuf, 12 );
    nLen += 12;

    //  �������Q
    sOutData[nLen++] = 'Q';
    sprintf( sOutData+nLen, "%03d", 12 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szOtherAmount, 6 , sBuf );
	memcpy( sOutData + nLen, sBuf, 12 );
    nLen += 12;

    //  ���׻��Ҵ���R
    sOutData[nLen++] = 'R';
    sprintf( sOutData + nLen, "%03d", 4 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szCurCode, 2, sBuf );
	memcpy( sOutData + nLen, sBuf, 4 );
    nLen += 4;

    // ��������S
    sOutData[nLen++] = 'S';
    sprintf( sOutData + nLen, "%03d", 6 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szDate, 3, sBuf );
	memcpy( sOutData + nLen, sBuf, 6 );
    nLen += 6;

    //  ��������T
    sOutData[nLen++] = 'T';
    sprintf( sOutData + nLen, "%03d", 2 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.cTranceType, 1, sBuf );
	memcpy( sOutData + nLen, sBuf, 2 );
    nLen += 2;

    //  ����ʱ��U
    sOutData[nLen++] = 'U';
    sprintf( sOutData + nLen, "%03d", 6 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szTime, 3, sBuf );
	memcpy( sOutData + nLen, sBuf, 6 );
    nLen += 6;

    //  �ն˹��Ҵ���V
    sOutData[nLen++] = 'V';
    sprintf( sOutData + nLen, "%03d", 4 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szCountyCode, 2, sBuf );
	memcpy( sOutData + nLen, sBuf, 4 );
    nLen += 4;

    //  �̻�����W
    sOutData[nLen++] = 'W';
    sprintf( sOutData + nLen, "%03d", 20 );
    nLen += 3;
    memcpy( sOutData + nLen, stIndata.szMerchen, 20 );
    nLen += 20;

    //  Ӧ�ý��׼�������ATC��X
    sOutData[nLen++] = 'X';
    sprintf( sOutData + nLen, "%03d", 4 );
    nLen += 3;
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szATC, 2, sBuf );
	memcpy( sOutData + nLen, sBuf, 4 );
    nLen += 4; 

    sOutData[nLen] = 0x00;

    DbgPrintf("App_LogFunc out Data %s\n", sOutData );
    return RET_SUCCESS;
}



/*!
 * \brief         Pboc_OrgCredLogDetail     
 * \par ˵��      ����ͨ�ø�ʽ��֯����  ��Ȧ�潻�׸�ʽ��    
 * \param[in]     stIndata     ST_PBOC_DETAIL��ʽ������������   
 * \param[in]     nNum         �ӿ�Ƭ�ж�ȡ��Ȧ�潻�׼�¼������
 * \param[out]    sOutData     ��֯���Ȧ������    
 * \retval        0 �ɹ� -1  ʧ��
 * \note      
 ���磺
9F79|000000033300|000000055500|141120|163101|0156|CHINA ZHENGZHOU BANK|0007|9F79|000000000000|000000033300|141120|162958|0156|CHINA ZHENGZHOU BANK|0002|
 */
int Pboc_OrgCredLogDetail(ST_PBOC_DETAIL stIndata,int nNum, char * sOutData)
{
    int nLen = 0;
    unsigned char sBuf[40+1] = {0x00};

    nLen = strlen(sOutData);

    /*< �������P  */    
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.sAmtType, 2,sBuf );
    memcpy( sOutData + nLen, sBuf, 4 );
    nLen += 4;
    sOutData[nLen++] = '|';

    /*<  �·��ű�ǰ�����ֽ���� */
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.sBeforeAmt, 6, sBuf );
    memcpy( sOutData + nLen, sBuf, 12 ); 
    nLen += 12;
    sOutData[nLen++] = '|';

    /*< �·��ű�������ֽ����  */
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.sAfterAmt, 6, sBuf );
    memcpy( sOutData + nLen, sBuf, 12 ); 
    nLen += 12;
    sOutData[nLen++] = '|';

    /*<  �������� */
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szDate, 3, sBuf );
    memcpy( sOutData + nLen, sBuf, 6);  
    nLen += 6;
    sOutData[nLen++] = '|';

    /*<  ����ʱ�� */
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii( stIndata.szTime, 3, sBuf );
    memcpy( sOutData + nLen, sBuf, 6 );  
    nLen += 6;
    sOutData[nLen++] = '|';


    /*< Ӧ�ý��׼�����  */
    memset( sBuf, 0x00, sizeof(sBuf) );
    HexToAscii(  stIndata.szATC, 2, sBuf );
    memcpy( sOutData + nLen, sBuf, 4 );  
    nLen += 4;
    sOutData[nLen++] = '|';

    sOutData[nLen] = 0x00;

    DbgPrintf("App_LogFunc out Data %s\n", sOutData );
    return RET_SUCCESS;
}



/*!
 * \brief             tFunc_SetDeviceParameter
 * \par ˵��          �����豸������
 * \param[in]         pstDeviceParam  ST_DEVICE_PARAM�豸��������
 * \retval            0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_SetDeviceParameter(ST_DEVICE_PARAM  *pstDeviceParam)
{
    int nRet = RET_SUCCESS;

	DbgPrintf("�������豸���ԡ�\n");

	memset( &g_sDeviceParam, 0x00, sizeof(ST_DEVICE_PARAM) );
	if (pstDeviceParam != NULL)
	{
		memcpy( &g_sDeviceParam, pstDeviceParam, sizeof(ST_DEVICE_PARAM) );
	}
    return RET_SUCCESS;
}

/*!
 * \brief             tMidFunc_OpenPort 
 * \par ˵��          �򿪴���
 * \param[in]         nPortComNo  �˿ں�    
 * \param[in]         nBaund      ������
 * \retval            0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_OpenPort( int  nPortComNo, int  nBaund)
{
    int nReturn = 0;
    int m_Baund = 0;
    char szBP_Cmd[3] = {0};

	DbgPrintf("���򿪴��ڡ�\n");
    if (nBaund == 9600)
    {
        m_Baund = BUAD_RATE;
    }
    m_Baund = nBaund;

    ICC_InitCardReaderParam( &g_sParam );
    if ( ( nPortComNo >= 1 ) && ( nPortComNo <= 20 ))
    {
        g_sParam.nCom = nPortComNo;
        g_sParam.nBaud = 9600; 
        g_sParam.nTransType = TYPE_COM;
        nReturn = ICC_OpenCardReader( &g_sParam );
    }
    else if( nPortComNo == 1001)
    {
        g_sParam.nTransType = TYPE_HID;
        g_sParam.nPID = HID_PID;
        g_sParam.nVID = HID_VID; 
        nReturn = ICC_OpenCardReader( &g_sParam );
        if( RET_SUCCESS != nReturn)
        {
            g_sParam.nTransType = TYPE_PCSC;
            g_sParam.nPID = HID_PID;
            g_sParam.nVID = HID_VID; 
            nReturn = ICC_OpenCardReader( &g_sParam );
        }
    }
    else
    {
        return RET_PARAM_ERR;
    }

    if( RET_SUCCESS != nReturn)
    {
        return RET_OPEN_PORT_FAIL;
    }
    
    //������ʱ������ָ��
    if(g_sParam.nTransType != TYPE_PCSC)
    {
        ICC_CardReaderCmd( (unsigned char *)ICC_PORT_9600, 4 );
        if( ( m_Baund != 9600 ) && ( g_sParam.nTransType == TYPE_COM ) )
        {
            g_sParam.nBaud = m_Baund;
            nReturn = ICC_CardReaderSynBps( &g_sParam, 0 );
            if(RET_SUCCESS != nReturn)
            {
                return RET_OPEN_PORT_FAIL;
            }
        }
    }

    return RET_SUCCESS;
}

/*!
 * \brief        tFunc_ClosePort      
 * \par ˵��     �رմ���           
 * \retval       0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_ClosePort()
{
    int nRet = RET_SUCCESS;

	DbgPrintf("���رմ��ڡ�\n");

    /*CCID�豸����ģ���л�����*/
    if( g_sParam.nTransType != TYPE_PCSC )   
    {
        if( ( g_sParam.nBaud != 9600 ) && ( g_sParam.nTransType == TYPE_COM ) )
        {
            ICC_CardReaderResetBps(0);
        }
    //zxy   ICC_CardReaderCmd((unsigned char *) ICC_DEFAULT_PORT, 3 );
    }
    nRet =  ICC_CloseCardReader( ); 
    if ( nRet != RET_SUCCESS )
    {
        nRet = RET_CLOSE_PORT_FAIL;
    }
    return nRet;
}
/*!
 * \brief            tFunc_SetExPort 
 * \par ˵��         ����ת��   
 * \param[in]        cExPort  ת�ں�
 * \retval           0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_SetExPort ( char cExPort )
{
    int nRet = RET_SUCCESS;
    char szBP_Cmd[8+1] = {0x00};

	DbgPrintf("������ת�ڡ�\n");

    //�·�BP��ָ��
    if( ( 'A' ==  cExPort ) ||  ( 'B' ==  cExPort ) || ( 'C' ==  cExPort  ) || ( 'K' ==  cExPort ) )
    {
        memset( szBP_Cmd, 0x00, sizeof(szBP_Cmd) );
        memcpy( szBP_Cmd, BP_CMD, 2 );
        szBP_Cmd[2] = cExPort;
        if( g_sParam.nTransType == TYPE_COM )
        {
            nRet =  ICC_CardReaderCmd( szBP_Cmd, 3);
            if ( nRet != RET_SUCCESS )
            {
                nRet = RET_EX_PORT_FAIL;
            }
        }
    }
    else
    {
        nRet = RET_PARAM_ERR;
    }
    return nRet;
}



/*!
 * \brief        tFunc_PowerOn      
 * \par ˵��     �Կ�Ƭ�����ϵ�     
 * \param[out]    pATRLen  IC�ϵ����ݵ����ݳ���
 * \param[out]    pATR     IC���ϵ緵������ Hex���ʽ
 * \retval        0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_PowerOn( int *pATRLen, unsigned char *pATR)
{
    int nRet = RET_PARAM_ERR;
    int nSlot = 0;
    int nAtrLen = 0;
    unsigned char uszAtr[256] = {0x00};

    *pATRLen = 0;

    /*CCID�豸�������ϵ�����*/
    if( g_sParam.nTransType == TYPE_PCSC  )
    {
        return 0;
    }

	ASSERT_RET ( tFunc_DeviceAuth() );

    tFunc_GetSlot( &nSlot );
    nRet = ICC_CardReaderPowerOn( nSlot , uszAtr, &nAtrLen, 0 );
    if( RET_SUCCESS == nRet )
    {
        if( pATRLen && pATR )
        {
            memcpy( pATR, uszAtr, nAtrLen );
            *pATRLen = nAtrLen;
            pATR[*pATRLen] = 0x00;
        }
        nRet = RET_SUCCESS;
    }
    else
    {
        nRet = ICC_CardReaderGetRecStatus( );
        if (nRet == -1002) /*< �Ӵ�ʽ�û���δ�嵽λ   */
        {
            nRet = RET_TOUCH_CARD_OUT;
        }
        else if (nRet == -1001) /*< ��֧�ֽӴ�ʽ�û���  */
        {
            nRet = RET_UNSUPT_TOUCH_CARD;
        }
        else if (nRet == -1003) /*< �Ӵ�ʽ�û����Ѿ��ϵ�  */
        {
            nRet = RET_TOUCH_CARD_ON;
        }
        else if (nRet == -3004) /*<  �ǽӴ�ʽ��δ���� */
        {
            nRet = RET_UNTOUCH_OFF;
        }
        else if (nRet == -3005 ) /*<  �ǽӴ�ʽ������ʧ�� */
        {
            nRet = RET_UNTOUCH_ON_FAIL;
        }
    }

    DbgPrintf("CT_PowerOn %d\r\n",nRet);

    return nRet;
}


/*!
 * \brief           tFunc_PowerOff    
 * \par ˵��        �������õĿ��ۺŽ����µ����        
 * \retval          0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_PowerOff()
{
    int nSlot = 0;
    int nRet = 0;
    /*<  ��ȡ���ۺ� */
    memset( &g_sParam, 0x00, sizeof(g_sParam) );

    tFunc_GetSlot(&nSlot);

    if( g_sParam.nTransType == TYPE_PCSC )
    {
        return 0;
    }
    nRet =  ICC_CardReaderPowerOff( nSlot );
    if( nRet != RET_SUCCESS )
    {
        nRet = ICC_CardReaderGetRecStatus( );

        nRet = RET_POWER_OFF_FAIL;
    }
    return nRet;
}

/*!
 * \brief          tFunc_SetSlot    
 * \par ˵��       ���ÿ��ۺ�   
 * \param[in]      nSlot  ��Ҫ���õĿ��ۺ�      
 * \retval         0 �ɹ� -1  ʧ��
 * \note                
 */
void tFunc_SetSlot(int nSlot)
{
    ICC_CardReaderSetCardSlot( nSlot );
}

/*!
 * \brief          tFunc_GetSlot    
 * \par ˵��       ��ȡ��ǰ���ۺ�   
 * \param[out]     nSlot  ��ǰ���ۺ�
 * \retval         0 �ɹ� -1  ʧ��
 * \note                
 */
void tFunc_GetSlot(int * nSlot)
{
    *nSlot =  ICC_CardReaderGetCardSlot();
}
/*!
 * \brief        tFunc_CardPresent      
 * \par ˵��     nSlot      
 * \param[in]    ������Ŀ��ۺŽ��п���λ�ж�       
 * \retval       0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_CardPresent( int nSlot)
{
    int nReturn = RET_PARAM_ERR;
    
	DbgPrintf("������λ��⡿ %d \n", nSlot);
    /*<  �Ӵ���Ƭ */
    if( ( nSlot >= 0x00 ) && ( nSlot <=0x0F ) )
    {
        nReturn = ICC_CardCmdCheckCardIsInSlot( nSlot );
        if( RET_SUCCESS == nReturn )
        {
            ICC_CardReaderSetCardSlot( nSlot );
            nReturn = ICC_CardReaderGetRecStatus( );
            if( nReturn == -1003 )
            {
                nReturn = TOUCH_CARD_IN_UP;//�Ӵ���λ�Ѿ��ϵ�
            }
            else if( nReturn == -1004 )
            {
                nReturn = TOUCH_CARD_IN;//�Ӵ���λδ�ϵ�
            }
            else
            {
                nReturn = RET_OTHER_ERR;// δ֪����
            }
        }
        else
        {
            nReturn = ICC_CardReaderGetRecStatus( );
            if( nReturn != -3 )//��ʱ
            {
                nReturn = RET_OTHER_ERR;
            }
        }
    }
    /*< �ǽӿ�Ƭ  */
    else if( nSlot == 0xFF )
    {
        nReturn = ICC_CardCmdCheckCardIsInSlot( PICC_CARD_SLOT );
        if( RET_SUCCESS == nReturn )
        {
            ICC_CardReaderSetCardSlot( PICC_CARD_SLOT );
            if ( ICC_CardReaderGetRecStatus( ) == 0 )
            {
                nReturn = NOTOUCH_CARD_IN_UP;
            }
            else
            {
                nReturn = RET_OTHER_ERR;
            }
        }
        else
        {
            nReturn = ICC_CardReaderGetRecStatus( );
            if( nReturn != -3 )//��ʱ
            {
                nReturn = RET_OTHER_ERR;
            }
        }
    }
    /*< PSAM��  */
    else if ( ( nSlot >= 0x10 ) && ( nSlot <= 0x1F ) )
    {
        nReturn = ICC_CardCmdCheckCardIsInSlot( nSlot );
        if( SMART_CARD == nReturn )
        {
            ICC_CardReaderSetCardSlot( SMART_CARD );
            nReturn = ICC_CardReaderGetRecStatus( );
            if( nReturn == -1003 )
            {
                nReturn = TOUCH_CARD_IN_UP;//�Ӵ���λ�Ѿ��ϵ�
            }
            else if( nReturn == -1004 )
            {
                nReturn = TOUCH_CARD_IN;//�Ӵ���λδ�ϵ�
            }
            else
            {
                nReturn = RET_OTHER_ERR;// δ֪����
            }
        }
        else
        {
            nReturn = ICC_CardReaderGetRecStatus( );
            if( nReturn != -3 )//��ʱ
            {
                nReturn = RET_OTHER_ERR;
            }
        }
    }
    return nReturn;
}
/*!
 * \brief         tFunc_SendCommand    
 * \par ˵��      ��������  
 * \param[in]     nCmdLen       ��Ҫ���͵������
 * \param[in]     pszCmd        ��Ҫ���͵���������
 * \param[out]    pnRespLen     ����Ӧ���������ݳ���
 * \param[out]    pszResp       ����Ӧ����������
 * \retval        0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_SendCommand(int nCmdLen, unsigned char *pszCmd,int *pnRespLen, unsigned char *pszResp)
{
    int nRet = RET_PARAM_ERR;
    int nReadTime = 0;
    int nRecvLen = RET_SUCCESS;
    unsigned char  ucRecvData[ 512 ] = {0};
    //DWORD dwTick;
    
    //dwTick = GetTickCount();
    *pnRespLen = 0;

	DbgPrintf("���������\n");
    nRet = ICC_CardCmdVisitCOS( pszCmd, nCmdLen, ucRecvData ,&nRecvLen );
    if(nRet == RET_SUCCESS)
    {
        memcpy( pszResp, ucRecvData, nRecvLen );
        *pnRespLen = nRecvLen;
        pszResp[*pnRespLen] = 0x00;
        nRet = RET_SUCCESS;
    }
    else
    {
        if ( nRet != RET_SUCCESS )
        {
            nRet = ICC_CardReaderGetRecStatus( );
            if (nRet<0)
            {
                nRet = RET_SEND_APDU_FAIL;
            }
        }
        *pnRespLen = 0;
        pszResp[0] = 0x00;
    }
    //nReadTime = GetTickCount() - dwTick;
    DbgPrintf("CT_SendCommand Time:%d\r\n",nReadTime);

    return nRet;

}


/*!
 * \brief            tFunc_GetCmdErr  
 * \par ˵��         �����ǰ�Ĵ�����   
 * \param[out]       pnErrCode  ��ǰ������
 * \retval           0 �ɹ� -1  ʧ��
 * \note                
 */
void tFunc_GetCmdErr(int * pnErrCode)
{
    *pnErrCode = ICC_CardCmdGetErrCode( );
}

/*!
 * \brief       tFunc_SaveTlvData       
 * \par ˵��    �洢TLV��ʽ����     
 * \param[in]   sTag         ��Ҫ�洢��TAG      
 * \param[in]   nTagLen      TAG�ĳ���     
 * \param[in]   sData        �洢������
 * \param[in]   nDataLen     �洢�����ݳ���
 * \param[out]  nOutDataLen  �洢�����ݳ���
 * \param[out]  sOutData     �洢������    
 * \retval      0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_SaveTlvData(char *sTag,int nTagLen,char *sData,int nDataLen,int *nOutDataLen,char *sOutData)
{
    int nLen =0;
    if ( (nDataLen == 0)  || (nTagLen == 0 ) )
    {
        return RET_ERROR;
    }

    nLen = *nOutDataLen;
    memcpy( sOutData + nLen, sTag, nTagLen );
    nLen += nTagLen;
    sOutData[nLen++] = (char)nDataLen;
    memcpy( sOutData + nLen, sData, nDataLen );
    nLen += nDataLen;

    *nOutDataLen = nLen;
    return RET_SUCCESS;
    
}

/*!
 * \brief           tFunc_SetTermParam   
 * \par ˵��        ���ý����еĽ�������    
 * \param[in]       stTermParam  �������ܽṹ��     
 * \retval          0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_SetTermParam(ST_TERMPARA * stInTermParam)
{
    char szBuf[100] = {0};

	DbgPrintf("�����ý������ܡ�\n");

    /*<  ���ȫ�ֽṹ�� */
    memset( &g_ICCardParam, 0x00, sizeof(g_ICCardParam) );

    /*< ʹ��Ĭ�ϵ��ն�����  */
    if ( stInTermParam == NULL )
    {
        tFunc_SaveTlvData( EMV_TAG_9F01_TM_ACQID, 2, "123456", 6, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F15_TM_MCHCATCODE, 2, "01", 2, &g_ICCardParam.nIccAppDataLen,  g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F16_TM_MCHID, 2, "123456789012345", 15, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F39_TM_POSENTMODE, 2, "\x22", 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_5F2A_TM_CURCODE, 2, "\x01\x56", 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F1A_TM_CNTRYCODE, 2, "\x01\x56", 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F3C_TM_REFCURCODE, 2, "\x01\x56", 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F33_TM_CAP, 2, "\xE0\xF0\xC8", 3, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F40_TM_CAP_AD, 2,"\xFF\x00\xF0\xF0\x01", 5, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F1C_TM_TERMID, 2, "1234", 4, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData  );
        tFunc_SaveTlvData( EMV_TAG_9F35_TM_TERMTYPE, 2, "\x22", 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F1E_TM_IFDSN, 2, "CENT_CKB", 8, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F7A_TM_EC_TSI, 2, "\x01", 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F7B_TM_EC_TTL, 2, "\x00\x00\x01\x00\x00\x00", 6, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        tFunc_SaveTlvData( EMV_TAG_9F66_TM_RF_TRANSPROP, 2, "\x58\x00\x00\x00", 4, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F69_IC_CERTIFCATION_DATA, 2, "\x01", 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_DF60_TM_CAPP, 2, "\x01", 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

    }
    else
    {
		/*<  9F4E   9F42 95 */
		tFunc_SaveTlvData( EMV_TAG_9F01_TM_ACQID, 2, stInTermParam->szAcqID_n_9F01 , 6, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F15_TM_MCHCATCODE, 2, stInTermParam->szMerCateCode_n_9F15, 2, &g_ICCardParam.nIccAppDataLen,  g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F16_TM_MCHID, 2, stInTermParam->szMerID_ans_9F16, 15, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F39_TM_POSENTMODE, 2, (char *)stInTermParam->cEntryMode_n_9F39, 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_5F2A_TM_CURCODE, 2, stInTermParam->szCurCode_n_5F2A, 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F1A_TM_CNTRYCODE, 2, stInTermParam->szTermCountryCode_b_9F1A, 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F3C_TM_REFCURCODE, 2, stInTermParam->szRefCurrCode_n_9F3C, 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F33_TM_CAP, 2, stInTermParam->szTermCap_b_9F33, 3, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F40_TM_CAP_AD, 2, stInTermParam->szAddTermCap_b_9F40, 5, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F1C_TM_TERMID, 2, stInTermParam->szTermId_an_9F1C, 4, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData  );
		tFunc_SaveTlvData( EMV_TAG_9F35_TM_TERMTYPE, 2, (char *)stInTermParam->cTypeTerm_n_9F35, 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F1E_TM_IFDSN, 2, stInTermParam->szIFD_an_9F1E, 8, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F7A_TM_EC_TSI, 2, (char *)stInTermParam->cEcIndicator_9F7A, 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F7B_TM_EC_TTL, 2, stInTermParam->cEcBalanceLimit_9F7B, 6, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F66_TM_RF_TRANSPROP, 2, stInTermParam->szTransProp_9F66, 4, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_9F69_IC_CERTIFCATION_DATA, 2, (char *)stInTermParam->cSm, 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		tFunc_SaveTlvData( EMV_TAG_DF60_TM_CAPP, 2, (char *)stInTermParam->cAppExDF60, 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

    }

    return RET_SUCCESS;
}

/*!
 * \brief        tFunc_SetAidList       
 * \par ˵��     ����AIDӦ���б�        
 * \param[in]    pszAidList  AID�б�AIDӦ���ԡ�,������    
 * \param[in]    nAidLen     AID�б�ĳ���     
 * \retval       0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_SetAidList ( char *pszAidList, int nAidLen )
{
    unsigned char sAid[ 16+1 ] = { 0x00 };
    char sTmpTag  = 0;
    int nAidPoint = 0;
    int nAidNum   = 0;
    
	DbgPrintf("������Ӧ���б�\n");
    memset( s_AidList, 0x00, sizeof(s_AidList) );

    /*<  ʹ��Ĭ�ϵ�AID */
    if ( pszAidList == NULL )
    {
        memset( &s_AidList[0], 0x00, 32 );
        memcpy( &s_AidList[0], "A000000333010101", 16 );

        memset( &s_AidList[1], 0x00,32);
        memcpy( &s_AidList[1], "A000000333010102", 16 );
        
        Pboc_SetAidNum(2);
    }
    
    while (pszAidList != 0x00)
    {
        sTmpTag = *(pszAidList++);
        nAidLen--;

        if (nAidLen <= 0)
        {
            memcpy( &s_AidList[nAidNum], sAid, nAidPoint );
            memset( sAid, 0x00, sizeof(sAid) );
            nAidPoint = 0;
            nAidNum++;
            Pboc_SetAidNum( nAidNum );
            return RET_SUCCESS;
        }
        if (sTmpTag != ',')
        {
            sAid[nAidPoint++] = sTmpTag;
        }
        else
        {
            memcpy( &s_AidList[nAidNum], sAid, nAidPoint );
            memset( sAid, 0x00, sizeof(sAid) );
            nAidPoint = 0;
            nAidNum++;
            Pboc_SetAidNum( nAidNum );
        }
    }
    return RET_SUCCESS;
}

/*!
 * \brief         tFunc_SetInfoTag     
 * \par ˵��      ���ö�ȡ��Ƭ��Ϣ�е�TAG   
 * \param[in]     pszTagList  ��Ҫ��ǩ�б� Hex��    
 * \param[in]     nTagListLen  ��ǩ�����ݳ���       
 * \retval        ��
 * \note                
 */
void tFunc_SetInfoTag ( char *pszTagList[],int nNum )
{
    int i = 0;

	DbgPrintf("��������Ϣ��ǩ��\n");
    for ( i = 0; i < nNum; i ++ )
    {
		memset( s_InfoTag[i], 0x00, 3 );
        memcpy( s_InfoTag[i], pszTagList[i], strlen(pszTagList[i]) );
    }
}

/*!
 * \brief          tFunc_SetMesgTag    
 * \par ˵��       ���ñ�����Ϣ     
 * \param[in]      pszArqc   ����55��ı��ĸ�ʽ     
 * \param[in]      pszTc     TC���ĵ����ݸ�ʽ       
 * \retval         ��
 * \note                
 */
void tFunc_SetMesgTag( char *pszArqc[], int nNumArqc, char *pszTc[], int nNumTc )
{
    int i = 0;
	
	DbgPrintf("�����ñ�����Ϣ��\n");

    memset( s_ArqcTag, 0x00, sizeof(s_ArqcTag) );
    memset( s_TcTag, 0x00, sizeof(s_TcTag) );

    /*<  ����ARQC���� */
    if ( ( pszArqc != NULL )  && ( nNumArqc != 0 ) )
    {
        
        for( i = 0; i<nNumArqc; i++ )
        {
            memset( s_ArqcTag[i], 0x00, 3 );
            memcpy( s_ArqcTag[i], pszArqc[i], strlen(pszArqc[i]) );
        }
    }
    else
    {
        for( i = 0; i < TABLESIZE(s_ArqcTagDefault); i++)
        {
            memset( s_ArqcTag[i], 0x00, 3 );
            memcpy( s_ArqcTag[i], s_ArqcTagDefault[i], strlen(s_ArqcTagDefault[i]) );
        }
    }

    /*<  ����TC���� */
    if ( ( pszTc != NULL ) && ( nNumTc != 0 ) )
    {
        
        for( i = 0; i < nNumTc; i++ )
        {
            memset( s_TcTag[i], 0x00, 3 );
            memcpy( s_TcTag[i], pszTc[i], strlen(pszTc[i]) );
        }
    }
    else
    {
		DbgPrintf("�����ñ�����Ϣ�� ����Ĭ�ϱ���\n");
        for( i = 0; i < TABLESIZE(s_TcTagDefault); i++ )
        {
            memset( s_TcTag[i], 0x00, 3 );
            memcpy( s_TcTag[i], s_TcTagDefault[i], strlen(s_TcTagDefault[i]) );
        }
    }
    return;

}


int Pboc_TransAppSelectByAid( unsigned char* szAid ,int nAidLen, unsigned char *pFci, int *pFciLen )
{
	//  Ӧ�ñ�ѡ��ȷ�Ͻ��ף�����ѡ�����SELECT������PDOL��TL��Ϣ��ȷ��Ӧ���Ƿ�����
	if( RET_SUCCESS !=  ICC_AppSelect(szAid, nAidLen, pFci, pFciLen ) )
	{
		if( 0x6283 == ICC_CardCmdGetErrCode() )
		{
			return RET_CARD_LOCK ;
		}
		else if ( 0x9303 == ICC_CardCmdGetErrCode())
		{
			return RET_CARD_LOCK_FOREVER ;
		}
		else
		{
			return RET_ERROR;
		}
	}
	return RET_SUCCESS;
}

/*!
 * \brief        tFunc_SetIcAidList        
 * \par ˵��     ��Ŀ¼ѡ��֮������IC��AIDӦ���б�   	
 * \param[in]    nNum         AID�б��� ��0��ʼ     	
 * \param[in]    szAid        ��Ƭ��AID   
 * \param[in]	 nAidLen      AID����
 * \param[in]    szAppTag     AIDӦ������
 * \param[in]    nAppTagLen   AIDӦ�����Ƴ���
 * \param[in]    szApn        AID��������
 * \param[in]    nApnLen      AID�������Ƴ���
 * \param[in]    cAPPpriority AID���ȼ�	
 * \retval      
 * \note                
 */
void tFunc_SetIcAidList(int nNum,unsigned char *szAid, int nAidLen, unsigned char *szAppTag, int nAppTagLen, unsigned char *szApn, int nApnLen,  char cAPPpriority)
{
	if ( nNum == 0)
	{
		memset( g_IcAidList, 0x00, AID_LIST_LEN );
	}
	if ( nAidLen > 0 )
	{
		memcpy( g_IcAidList + nNum* 53, szAid ,nAidLen );
		g_IcAidList[ nNum* 53 + 16 ] = (char)nAidLen;
	}
	if ( nAppTagLen >0 )
	{
		memcpy( g_IcAidList + nNum* 53 +17, szAppTag ,nAppTagLen );
		g_IcAidList[ nNum* 53 + 16 + 17] = (char)nAidLen;
	}
	if ( nApnLen >0 )
	{
		memcpy( g_IcAidList + nNum* 53 + 34, szApn ,nApnLen );
		g_IcAidList[ nNum* 53 + 16 + 34 ] = (char)nApnLen;
	}

	g_IcAidList[ nNum*53 + 51 ] = cAPPpriority;

	g_IcAidList[ nNum*53 + 52 ] = '|';
}
/*!
 * \brief       tFunc_GetIcAidList       
 * \par ˵��    ��һ����ʽ����Ŀ¼ѡ��֮��AID�б�   	
 * \param[out]	szOutAidList	��Ƭ��AIDӦ���б�
 * \retval      0 �ɹ� -1  ʧ��
 * \note                
 */
int  tFunc_GetIcAidList(char * szOutAidList)
{
	if ( g_nAidListNum == 0)
	{
		return FAIL;
	}
	
	memcpy(szOutAidList,g_IcAidList,AID_LIST_LEN);
	return RET_SUCCESS;
}

/*!
 * \brief        tFunc_App_SelectInit      
 * \par ˵��     Ӧ��ѡ�񡢳�ʼ��           
 * \retval       0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_AppSelectInit()
{
    int nRet = 0;
    int nAFLlen = 0; // AFL ����
    int nAIPlen = 0; // AIP ����
	int nAtrLen = 0;
	int nAidNum = 0;
	int nAidLen = 0;
	int nFciLen = 0;
	int nErrCod = 0;
	int nIndex = 0;
	int nAIDNum = 0;
	int i = 0, nFlag = 1;
    unsigned char uszAFL[512] = {0x00};    // AFL 
    unsigned char uszAIP[4+1] = {0x00};    // AIP 
	unsigned char uszAtr[128] = {0x00};
	unsigned char uszAID[32] = {0x00};
	unsigned char uszFci[1024] = {0x00};
	AID_LIST *pAidList = NULL;

	DbgPrintf("��Ӧ��ѡ�񡢳�ʼ����\n");

	memset( uszAtr, 0x0, sizeof(uszAtr) );  
	nRet = ICC_CardCmdSelectFile( 0x04, 0x00, "1PAY.SYS.DDF01", strlen("1PAY.SYS.DDF01"), uszAtr,  &nAtrLen );
	DbgPrintHex("��Ӧ��ѡ�񡢳�ʼ���� �ļ�ѡ����", uszAtr, nAtrLen );
	if( RET_SUCCESS !=  nRet )
	{
		
		if( 0x6A82 ==  ICC_CardCmdGetErrCode()  )
		{
			DbgPrintf("��Ӧ��ѡ�񡢳�ʼ���� ��֧��Ŀ¼ѡ��\n");
			goto AidSel;
		}
		return nRet;
	}

	DbgPrintf("��Ӧ��ѡ�񡢳�ʼ���� Ӧ���б�\n");
	nRet =  ICC_GetAppList(  &pAidList,  &nAidNum  );
 	if( RET_SUCCESS != nRet)
 	{
 		ICC_FreeAidList( pAidList );
 		return RET_GET_APP_FAIL;
 	}

AidSel:
 	if( pAidList != NULL ) /*<  ֧��Ŀ¼ѡ��  */
 	{
		while( nFlag )
		{
			nAidLen = pAidList[i].AidLen;
			memcpy( uszAID, pAidList[i].AID, nAidLen );
			
			/*< ����AIDӦ���б�  */
			tFunc_SetIcAidList( i, pAidList[i].AID, pAidList[i].AidLen,  pAidList[i].AppTag,\
				pAidList[i].AppTagLen, pAidList[i].APN, pAidList[i].APNlen, pAidList[i].APPpriority );

			DbgPrintf("��Ӧ��ѡ�񡢳�ʼ���� AIDƥ��\n");
			if ( YES ==  Pboc_IsAIDMatch( uszAID, nAidLen, &nIndex ) )
			{
				DbgPrintf("��Ӧ��ѡ�񡢳�ʼ���� AID Ӧ��ѡ��\n");
				nRet  =  Pboc_TransAppSelectByAid( uszAID, nAidLen, uszFci, &nFciLen );
				if (nRet == RET_SUCCESS)
				{
					tFunc_SaveTlvData( EMV_TAG_6F_IC_FCITEMP, 1, uszFci, nFciLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
				}
			}
			if (pAidList[i].next != NULL)
			{
				i++;
			}
			else
			{
				nFlag = 0;
				if (nRet != RET_SUCCESS)
				{
					return nRet;
				}
			}	
		}
 	}
 	else /*<  ��֧��Ŀ¼ѡ���ʱ��  ����AID�б�ѡ��  */
 	{
		DbgPrintf("��Ӧ��ѡ�񡢳�ʼ���� Ӧ���б�ѡ��\n");
		nRet = Pboc_Meth_AidList( &nAIDNum );
		if(nRet != SUCC)
		{
			return nRet;
		}
 	}
	/*<  �ͷ�AID�б�ָ�� */
	DbgPrintf("��Ӧ��ѡ�񡢳�ʼ���� �ͷ�AID�б�ָ��\n");
 	ICC_FreeAidList( pAidList );

	/*<  Ӧ�ó�ʼ�� */
	DbgPrintf("��Ӧ��ѡ�񡢳�ʼ���� Ӧ�ó�ʼ��\n");
	nRet =  ICC_AppInit( BANGK, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, uszAFL, &nAFLlen, uszAIP, &nAIPlen );
	if( nRet != 0 )
	{
		return RET_INIT_FAIL;
	}

	/*<  ��ȡ  */

	/*< AIP  */
	tFunc_SaveTlvData( EMV_TAG_82_IC_AIP, 1, uszAIP, nAIPlen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
	/*<  �洢AFL  */
	tFunc_SaveTlvData( EMV_TAG_94_IC_AFL, 1, uszAFL, nAFLlen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
	DbgPrintf("��Ӧ��ѡ�񡢳�ʼ���� ����ֵ %d\n",nRet);
	return nRet;

}



/*!
 * \brief        tFunc_GetDataByTag      
 * \par ˵��     �ӽ��׹�����ͨ��TAG ��ȡ��ǩֵ�����Ϊ��̬���ݣ���ͨ��80 CA��ȡ��̬����    
 * \param[in]    sTag      ��Ҫ��ȡ��TAG        
 * \param[in]    nTagLen   TAG��ǩ�ĳ���       
 * \param[out]   nValueLen ��ǩֵ�ĳ���
 * \param[out]   sValue    ��ǩֵ(Hex���ʽ)
 * \retval       0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_GetDataByTag(char * sTag,int nTagLen,int * nValueLen,char * sValue)
{
    int nRet = 0;
    int nBufLen = 0;
    int i = 0;
    int nFlag = 0;
    int nDataLen =0;
    char sBuf[1024] = {0x00};
    char sData[128] = {0x00};

	DbgPrintf("��TAG��ȡ��\n");
    *nValueLen = 0;

    /*< �鿴TAG �Ƿ�Ϊ��̬����  */
    for( i =0; i<TABLESIZE(sStaticTagList); i++ )
    {
        if (memcmp ( sTag, sStaticTagList[i], nTagLen )   == 0 )
        {
            nFlag = 1;
            memset( sData, 0x00, sizeof(sData) );
            nDataLen = 0;
            nRet =  Pboc_GetIcStaticData(sTag, nTagLen, sData,&nDataLen);
            if (nRet == RET_SUCCESS)
            {
                centerm_get_tag_value( sTag, nTagLen, sData, nDataLen, sValue, nValueLen );
            }
        }
    }
    if (nFlag  == 0)
    {
        nRet = centerm_get_tag_value( sTag, nTagLen, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, sBuf, &nBufLen );
        if (nRet == RET_SUCCESS)
        {
            memcpy( sValue, sBuf, nBufLen );
            *nValueLen = nBufLen;
        }
    }

    return nRet;
}
/*!
 * \brief          tFunc_GetIcCardInfo    
 * \par ˵��       ��ȡ��Ƭ��Ϣ ���մ����Info Tag �б� �������ݸ�ʽ����TLV���
 * \param[out]     pnUsrInfoLen  ���ݳ���
 * \param[out]     pszUserInfo   ����
 * \retval         0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_GetIcCardInfo(int * pnUsrInfoLen,char *pszUserInfo)
{
    int i = 0;
    int nRet = 0;
    int nBufLen = 0;
    int nLen = 0;
    int nDataLen  =0;
    char sBuf[1024] = {0x00};
    char sData[1024] = {0x00};
    
	DbgPrintf("����ȡ��Ƭ��Ϣ��\n");
    /*<  �ӽ��׹��������ж�ȡ��ǩ */
    for ( i =0; i<TABLESIZE(s_InfoTag); i++ )
    {
        nBufLen = 0;
        memset( sBuf, 0x00, sizeof(sBuf) );
        nRet = tFunc_GetDataByTag( s_InfoTag[i], strlen(s_InfoTag[i]), &nBufLen, sBuf );
        DbgPrintHex("Get data s_InfoTag", s_InfoTag[i], strlen(s_InfoTag[i]) );

        if (nBufLen>0)
        {
            memset(sData,0x00,sizeof(sData));
            nRet = centerm_packtTag( strlen((char *)s_InfoTag), s_InfoTag[i], nBufLen, sBuf, &nDataLen, sData );
            
            memcmp( pszUserInfo + nLen, sData, nDataLen );
            nLen += nDataLen;
        }
    }
    *pnUsrInfoLen = nLen;

    return RET_SUCCESS;
}

/*!
 * \brief         tFunc_SetAppTransactionTag     
 * \par ˵��      ����Ӧ�ý��׹����е���Ϣ  
 * \param[in]     pszTranceInfo  ASC���ʽ���ӿ���Ҫ���õĽ�����Ϣ      
 * \retval        0 �ɹ� -1  ʧ��
 * \note          ��ʽΪ9F0206000010000000        
 */
int tFunc_SetAppTransactionTag ( char *pszTranceInfo)
{
    int nRet = 0;
    int nBufLen =0;
    char sBuf[1024] = {0x00};
    char sData[8+1] = {0x00};
    char sTagValue[256] = {0x00};
    int nTagValueLen  = 0;

	DbgPrintf("�����ý�����Ϣ��\n");
    /*<  Ӧ��û���·��������� ��ʹ��Ĭ�ϵĽ�������   ��Ĭ������û�д�TAGԪ�������� �����򸲸�*/
    /*<  ���ڵ�ǰ��TLV��ʽ���� ��ʱ�������һ��ȫ�ֱ����� */
    if (strlen(pszTranceInfo) != 0)
    {
        /*< ת�����׸�ʽ  */
        memset( &g_sParam.szTermData, 0x00, sizeof(g_sParam.szTermData) );
        AsciiToHex( pszTranceInfo, strlen(pszTranceInfo), sBuf );
        nBufLen = strlen(pszTranceInfo)/2;
        
        DbgPrintHex("�����ý�����Ϣ�� g_ICCardParam.szIccAppData before \n", g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen );

        /*!<��Ȩ���*/
        nRet = centerm_get_tag_value( EMV_TAG_9F02_TM_AUTHAMNTN, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 6;
            memcpy( sTagValue, "\x00\x00\x00\x00\x00\x00", nTagValueLen );
        }
		DbgPrintHex("�����ý�����Ϣ�� ��Ȩ���\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F02_TM_AUTHAMNTN, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<�������*/
        nRet = centerm_get_tag_value( EMV_TAG_9F03_TM_OTHERAMNTN, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if ( nRet != RET_SUCCESS )
        {  
            nTagValueLen = 6;
            memcpy( sTagValue, "\x00\x00\x00\x00\x00\x00", nTagValueLen );
        }
		DbgPrintHex("�����ý�����Ϣ�� �������\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F03_TM_OTHERAMNTN, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*!<�ն˹��Ҵ���*/ 
        nRet = centerm_get_tag_value( EMV_TAG_9F1A_TM_CNTRYCODE, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 2;
            memcpy( sTagValue, "\x01\x56", nTagValueLen );
        }
		DbgPrintHex("�����ý�����Ϣ�� �ն˹��Ҵ���\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F1A_TM_CNTRYCODE, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*!< Check �ֿ�����֤���*/
        tFunc_SaveTlvData( EMV_TAG_9F34_TM_CVMRESULT, 2, "\x3f\x00\x00", 3, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!< Check ����״̬��Ϣ*/
        tFunc_SaveTlvData( EMV_TAG_9B_TM_TSI, 1, "\x00\x00", 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*!<�ն���֤���*/
        nRet = centerm_get_tag_value( EMV_TAG_95_TM_TVR, 1, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 5;
            memcpy(  sTagValue, "\x00\x00\x00\x08\x00", nTagValueLen );
        }
		DbgPrintHex("�����ý�����Ϣ�� �ն���֤���\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_95_TM_TVR, 1, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<���׻��Ҵ���*/
        nRet = centerm_get_tag_value( EMV_TAG_5F2A_TM_CURCODE,  2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if ( nRet != RET_SUCCESS )
        {  
            nTagValueLen = 2;
            memcpy( sTagValue, "\x01\x56", nTagValueLen );
        }
		DbgPrintHex("�����ý�����Ϣ�� ���׻��Ҵ���\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_5F2A_TM_CURCODE, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<��������*/
        nRet = centerm_get_tag_value( EMV_TAG_9C_TM_TRANSTYPE, 1, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 1;
            memcpy( sTagValue, "\x00", nTagValueLen );
        }
		DbgPrintHex("�����ý�����Ϣ�� ��������\n", sTagValue, nTagValueLen);
		tFunc_SaveTlvData( EMV_TAG_9C_TM_TRANSTYPE, 1, sTagValue, nTagValueLen,  &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
		


        /*!<��������*/
        nRet = centerm_get_tag_value( EMV_TAG_9A_TM_TRANSDATE, 1, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if ( nRet != RET_SUCCESS )
        {  
			/*< ��ȡϵͳ��������  */
            nTagValueLen = 3;
            memset( sData, 0, sizeof(sData) );
            GetTransDate( sData );
            AsciiToBcd( sData, 6,sTagValue, 3 );
        }
		DbgPrintHex("�����ý�����Ϣ�� ��������\n", sTagValue, nTagValueLen);
		/*<  ���������ڵĸ�ʽ */
        if (nTagValueLen == 4)
        {
            tFunc_SaveTlvData( EMV_TAG_9A_TM_TRANSDATE, 1, sTagValue + 1, nTagValueLen - 1, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        }
        else
        {
            tFunc_SaveTlvData( EMV_TAG_9A_TM_TRANSDATE, 1, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
        }
        


        /*!<����ʱ��*/
        nRet = centerm_get_tag_value( EMV_TAG_9F21_TM_TRANSTIME, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
			/*< ��ȡϵͳ����ʱ��  */
            nTagValueLen = 3;
            memset( sData, 0, sizeof(sData) );
            GetTransTime( sData );
            AsciiToBcd( sData, 6,sTagValue, 3 );
        }
		DbgPrintHex("�����ý�����Ϣ�� ����ʱ��\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F21_TM_TRANSTIME, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<�����*/
        nRet = centerm_get_tag_value( EMV_TAG_9F37_TM_UNPNUM, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 4;
            GetRandom( 4, sTagValue );
        }
        tFunc_SaveTlvData( EMV_TAG_9F37_TM_UNPNUM, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );



        /*!<�ն˽�������*/
        nRet = centerm_get_tag_value( EMV_TAG_9F66_TM_RF_TRANSPROP, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 4;
            memcpy( sTagValue, "\xF4\x00\x00\x00", 4 );
        }
		DbgPrintHex("�����ý�����Ϣ�� �ն˽�������\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F66_TM_RF_TRANSPROP, 2, sTagValue,nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<�����ֽ��ն�֧��ָʾ��*/
		nRet = centerm_get_tag_value( EMV_TAG_9F7A_TM_EC_TSI, 2,  sBuf, nBufLen, sTagValue, &nTagValueLen );
		if ( nRet != RET_SUCCESS )
		{
			nTagValueLen = 1;
			memcpy( sTagValue, "\x01", 1 );	
		}
		DbgPrintHex("�����ý�����Ϣ�� �����ֽ�ָʾ��\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F7A_TM_EC_TSI, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );


        /*!<�����ֽ��ն˽����޶�*/
        tFunc_SaveTlvData( EMV_TAG_9F7B_TM_EC_TTL,2,"\x00\x00\x01\x00\x00\x00", 6, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*!< �̻�����*/
        nRet = centerm_get_tag_value( EMV_TAG_9F4E_TM_NAMELOC, 2, sBuf, nBufLen, sTagValue, &nTagValueLen );
        if (nRet != RET_SUCCESS)
        {  
            nTagValueLen = 20;
            memset( sTagValue, 0x20, 21 );
            memcpy( sTagValue, "CKB 5150", 8 );
        }
		DbgPrintHex("�����ý�����Ϣ�� �̻�����\n", sTagValue, nTagValueLen);
        tFunc_SaveTlvData( EMV_TAG_9F4E_TM_NAMELOC, 2, sTagValue, nTagValueLen, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        /*< �ն�����  */
		DbgPrintf("�����ý�����Ϣ�� �ն�����\n");
        tFunc_SaveTlvData( EMV_TAG_9F33_TM_CAP, 2, "\xE0\xF0\xC8", 3, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );

        DbgPrintHex("tFunc_SetAppTransactionTag g_ICCardParam.szIccAppData fater \n",g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen );
	}

    return nRet;
}


/*!
 * \brief       Pboc_ExecuteGacCmd       
 * \par ˵��    ִ��GAC����     
 * \param[in]   Cmd  ��ʾGAC����������          
 * \param[in]   szIccData     CDOL����       
 * \param[in]   nIccDataLen   CDOL���ݳ���
 * \param[out]  szAc          GAC ��Ӧ����
 * \param[out]  pnAcLen       GAC ��Ӧ���ݳ���  
 * \retval      0 �ɹ� -1  ʧ��
 * \note                
 */
int Pboc_ExecuteGacCmd( U8 Cmd, U8* szIccData, S32 nIccDataLen, U8* szAc, S32* pnAcLen )
{
    U8  P1 = 0x0;
    S32 nRetVal = RET_SUCCESS;
    U8  szOutData[ MAX_LEN ] = {0};
    S32 nOutDataLen = 0;

    switch( Cmd )
    {
    case CMD_GAC_ARQC:
        {
            P1 = 0x80;
            break;    
        }  
    case CMD_GAC_TC:
        {
            P1 = 0x40;
            break;    
        } 
    case CMD_GAC_AAC:
        {
            P1 = 0x00;
            break;    
        }
    default:
        {
            DbgPrintf("ִ��GAC���������\n");
            return RET_CMD_ERR;    
        }                     
    }

    nRetVal = ICC_CardCmdGenerateAC( P1, szIccData, nIccDataLen, szOutData, &nOutDataLen );
    DbgPrintHex("GAC������Ӧ����", szOutData, nOutDataLen );
    memcpy( szAc, szOutData, nOutDataLen );
    *pnAcLen = nOutDataLen;
    return nRetVal;
}
/*!
 * \brief            tFunc_GenARQC  
 * \par ˵��         ����ARQC   
 * \param[out]       pnArqcLen  ����ARQC�����������
 * \param[out]       pszArqc    ARQC����(ASC ���ݸ�ʽ)
 * \retval           0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_GenARQC (int * pnArqcLen, char *pszArqc)
{
    int nRet = RET_PARAM_ERR;
    int i = 0, nF55Len =0;
    int nCDOL1DataLen = 0;
    int nARQClen =0;
    int nBufLen =0;
    int nDOLLen =0;
    int nArqcHexLen  = 0;
    unsigned char uzCDOL1Data[256] = {0x00};
    unsigned char uszARQC[1024] = {0x00};
    unsigned char sBuf[1024] = {0x00};
    unsigned char  DOL[MAX_LEN];
    char szF55[512] = {0x00};
    char sArqcHex[512] = {0x00};
	char sData[1024] = {0x00};
	int nDataLen = 0;

    *pnArqcLen = 0;
    DbgPrintf("��ARQC��\n");
	
	/*<  �����ȡ CDOL1 CDOL2  */
	nRet =  tFunc_ReadRecordData( 2,NULL, 2, &nDataLen , sData);
	if ( nRet == RET_SUCCESS )
	{
		/*< ���CDOL1 ����  */
		nRet =  ICC_GetCDOL1( sData, nDataLen, DOL, &nDOLLen );
		DbgPrintHex("��ARQC�� ��ȡCDOL1 ", DOL, nDOLLen );
		if ( nRet != RET_SUCCESS )
		{
			return RET_GET_CDOL1_FAIL;
		}
		nRet =  ICC_GetIccDataByDOL( DOL, nDOLLen, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, uzCDOL1Data, &nCDOL1DataLen );
		if(nRet != RET_SUCCESS)
		{
			DbgPrintf("��ARQC�� ��ȡCDOL1 ʧ��\n");
			return RET_GET_CDOL1_FAIL;
		}
		/*<  ��ֵ���� */
		if ( nDOLLen >0 )
		{
			memcpy( g_ICCardParam.szIccAppData + g_ICCardParam.nIccAppDataLen, sData, nDataLen);
			g_ICCardParam.nIccAppDataLen += nDataLen;
		}

	}


  
    /*< GAC ����ARQC  */
	DbgPrintf("��ARQC�� GAC1 ����ARQC\n");
    memset( uszARQC, 0, sizeof(uszARQC) );
    nRet = Pboc_ExecuteGacCmd( CMD_GAC_ARQC, uzCDOL1Data, nCDOL1DataLen, uszARQC, &nARQClen );
    if( nRet != 0 )
    {
        if( ICC_CardReaderGetRecStatus( ) == -3 )//��ʱ
        {
            return RET_TIMEOUT_ERR;
        }
        return RET_GEN_ARQC_FAIL;
    }

    DbgPrintHex("��ARQC�� ��Ƭ����GAC����", uszARQC, nARQClen );
    nRet =  Pboc_GetGACData( uszARQC, nARQClen);
    if ( nRet != RET_SUCCESS )
    {
		DbgPrintf("��ARQC�� ����GAC����ʧ��\n");
        return RET_PARASE_GAC_ERR;
    }
    
	/*<  ������� */
	DbgPrintf("��ARQC�� ���ARQC���� \n");
    nRet = Pboc_PackDomain( 0, &nArqcHexLen , sArqcHex );
    if (nRet != RET_SUCCESS)
    {
		DbgPrintf("��ARQC�� ���ARQC���� ʧ��\n");
        nRet = RET_GEN_ARQC_FAIL;
    }
    
    /*<  ת������ ��HEX��ת���ɣ��ӣãɣ���  */
    HexToAscii( sArqcHex, nArqcHexLen, pszArqc );
    *pnArqcLen = 2*nArqcHexLen;
	DbgPrintf("��ARQC��ARQC���� %s \n",pszArqc);
    return nRet;
}

/*!
 * \brief        tFunc_IssueAuth      
 * \par ˵��     ��������֤     
 * \param[in]    pszARPC  ASCII���ʽ       
 * \param[in]    nARPCLen   ARPC�����ݳ���    
 * \param[out]   szAuthCode  ��������֤��Ӧ��
 * \param[out]      
 * \retval              0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_IssueAuth (char *pszARPC, int nARPCLen,char * szAuthCode)
{
    char sArpcHex[256] = {0x00};  //�Ϊ12
    int  nArpcHexLen = 0;
    int  nRet = RET_SUCCESS;

    DbgPrintf("����������֤�� pszARPC %d  %s\r\n", nARPCLen, pszARPC ); 

    /*<  ת�����ݸ�ʽ */
    memset( sArpcHex, 0x00, sizeof(sArpcHex) );
    AsciiToHex( pszARPC, nARPCLen, (char *)sArpcHex );
    nArpcHexLen = nARPCLen/2;
    if( nArpcHexLen < 2 )
    {
        DbgPrintf("ARPC����С��1\n"); 
        return RET_ERROR;
    }

    /*<  ��������֤ */  
    memset(szAuthCode, 0x00, sizeof(szAuthCode));
    nRet = ICC_BankAuthentication( (unsigned char *)sArpcHex, nArpcHexLen, szAuthCode );
    if(nRet < 0)
    {
        DbgPrintf("����������֤�� ��֤ʧ��  ERR %s \n", ICC_CardCmdShowLastErrMsg() );
        return RET_FAIL_EXT_AUTH;
    }

    DbgPrintf("����������֤�� ��Ȩ��Ӧ��  %s\n", szAuthCode);

    return RET_SUCCESS;
}
/*!
 * \brief         tFunc_CompleteTransaction     
 * \par ˵��      ������ɴ���  
 * \param[in]     in_pszAuthCode    
 * \retval        0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_CompleteTransaction(unsigned char * in_pszAuthCode)
{
    int nRet = RET_SUCCESS, nRespLen = 0, nCDOL2DataLen =0, nDOLLen =0; 
	unsigned char szRespData[256] = {0x00}; 
    unsigned char sCDOL2Data[256] = {0x00};
    unsigned char DOL[256] = {0x00};

	DbgPrintf("������GAC2�� \n");
    /*<  ����ʽ��֯CDOL2 ����  �����ն˺ͷ����е���Ȩ��Ӧ�� */
    tFunc_SaveTlvData( EMV_TAG_8A_TM_ARC, 1, in_pszAuthCode, 2, &g_ICCardParam.nIccAppDataLen, g_ICCardParam.szIccAppData );
    nRet =  ICC_GetCDOL2( g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, DOL, &nDOLLen );
    if (nRet != RET_SUCCESS)
    {
        return RET_ERROR;
    }
	DbgPrintf("������GAC2�� ��ȡCDOL2 ���ݳɹ� \n");

    /*<  ���CDOL2 ���� */
    nRet =  ICC_GetIccDataByDOL( DOL, nDOLLen, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, sCDOL2Data, &nCDOL2DataLen );
    DbgPrintHex("ICC_GetCDOL2 DOL",sCDOL2Data, nCDOL2DataLen );
    if(nRet != RET_SUCCESS)
    {
        DbgPrintf("������GAC2�� ���CDOL2 ����ʧ�� \n");
        return RET_ERROR;
    }
	DbgPrintf("������GAC2�� ���CDOL2 ���ݳɹ�\n");

    /*<  �ڶ���GAC����  */
    DbgPrintf("������GAC2�� ����GAC2 ���� \n");
    memset( szRespData, 0, sizeof(szRespData) );
    nRet = ICC_ClosedTrade( in_pszAuthCode, sCDOL2Data, nCDOL2DataLen, szRespData, &nRespLen );
    if( RET_SUCCESS != nRet )
    {
		DbgPrintf("������GAC2�� GAC ʧ�� %d \n",ICC_CardCmdShowLastErrMsg());
        nRet = RET_CMD_ICGenAAC_FAIL;
    }
	DbgPrintf("������GAC2�� ����ֵ %d \n",nRet);

    return nRet;
}


/*!
 * \brief         tFunc_WriteScript     
 * \par ˵��      ����ű�  
 * \param[in]     pszScript     �·��Ľű�����  
 * \param[in]     nScriptLen    �ű����ݵĳ���     
 * \param[out]    sResult       �ű����    
 * \retval        0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_WriteScript(char *pszScript, int nScriptLen,char *sResult)
{
    int nRet = RET_SUCCESS;

    DbgPrintf("��д�ű���\n");
    nRet = ICC_WriteScript( pszScript, nScriptLen, sResult);
    DbgPrintf("��д�ű��� ����ֵ %d �ű���� %s \n",nRet,sResult);
    return nRet;
}

/*!
 * \brief          tFunc_GetTC    
 * \par ˵��       ��֯TC�������ݸ�ʽ   
 * \param[out]     *pnTCLen   TC���ĵ����ݳ���
 * \param[out]     pszTC      TC�������� ��ASC�����ݸ�ʽ��
 * \retval         0 �ɹ� -1  ʧ��
 * \note                
 */
int tFunc_GetTC (int *pnTCLen ,char *pszTC)
{
    int nRet = 0;
    int nTcHexLen  = 0;
    char sTcHex[1024] = {0x00};
    
	DbgPrintf("������TC��\n");
    nRet = Pboc_PackDomain( 1, &nTcHexLen, sTcHex);
    if (nRet != RET_SUCCESS)
    {
        nRet = RET_GEN_TC_FAIL;
    }

    /*<  ת������ ��HEX��ת���ɣ��ӣãɣ���  */
    HexToAscii( sTcHex, nTcHexLen, pszTC);
    *pnTCLen = 2*nTcHexLen;

	DbgPrintf("������TC������:%s \n",pszTC);
    return RET_SUCCESS;
}


/*!
 * \brief        tFunc_GetLog      
 * \par ˵��     ��ȡ������־��Ȧ����־     
 * \param[in]    nType          0x01 ������־  0x02 Ȧ����־        
 * \param[out]   nNum           ��־����     
 * \param[out]   pnTxDetailLen  ��־��Ϣ�ܳ���
 * \param[out]   TxDetail       ��־��Ϣ����
 * \param[int]   TranceDetail   ��ȡ��־�ص�����
 * \retval       0 �ɹ� -1  ʧ��
 * \note         Ӧ��ѡ��tFunc_AppSelectInit�� + ��ȡ��־��Ϣ��tFunc_GetLog��          
 */
int tFunc_GetLog ( int nType,int* nNum,int *pnTxDetailLen, char *TxDetail,TranceDetail pDetail)
{
    int nRet = 0;
    int nBufLen = 0;
    int nLogSum =0;
    int i =0;
    unsigned char uszBuf[512] = {0};
    unsigned char ucRecordID = 0;
    unsigned char szLogFormat[32]={0x00};
    unsigned char bSfi = '0';
    unsigned char bNum = '0';

	DbgPrintf("����ȡ��־���ݡ�\n");

    /*<  ������־ */
    if (nType == 0x01 )
    {
        nRet = ICC_GetLogEntry( g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, &bSfi, &bNum );
		DbgPrintf("����ȡ��־���ݡ� ������־��ʽ ����ֵ %d \n",nRet);
    }
    /*<  Ȧ����־ */
    else if (nType == 0x02)
    {
        nRet = ICC_GetCreditLoadLogEntry( g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, &bSfi, &bNum );
		DbgPrintf("����ȡ��־���ݡ� Ȧ����־��ʽ ����ֵ %d \n",nRet);
	}
    else
    {
        return RET_PARAM_ERR;
    }

    if(nType == 0x01) //���׸�ʽ
    {
        /*<  ��ȡ9F4D ���׸�ʽ */
        memcpy( szLogFormat, "\x9F\x4F\x19\x9A\x03\x9F\x21\x03\x9F\x02\x06\x9F\x03\x06\x9F\x1A\x02\x5F\x2A\x02\x9F\x4E\x14\x9C\x01\x9F\x36\x02", 28);
        nLogSum = 28;
    }
    else  //Ȧ���ʽ
    {
        /*< ��ȡDF4D���׸�ʽ  */
        memcpy( szLogFormat, "\xDF\x4F\x0E\x9A\x03\x9F\x21\x03\x9F\x1A\x02\x9F\x4E\x14\x9F\x36\x02", 17);
        nLogSum = 17;
    }

    if ( nRet != RET_SUCCESS )
    {
        return nRet;
    }
    
    /*< ��ȡ��־����  */
    for( ucRecordID = 1; ucRecordID < bNum; ucRecordID++ )
    {
        memset( uszBuf, 0x00, sizeof(uszBuf) );
        nRet = ICC_GetLog( bSfi,  ucRecordID, uszBuf, &nBufLen );
		DbgPrintf("����ȡ��־���ݡ� ��ȡ��־��Ϣ ����ֵ %d \n",nRet);
        if ( nRet != RET_SUCCESS )
        {
            break;
        }
        Pboc_FormatLog( szLogFormat, nLogSum, uszBuf, nBufLen, &stPbocList[i] );

        /*<  ��֯��־��ʽ  ����ص��������� ���ջص�������ʽ����������֯  */
        if ( pDetail != NULL)
        {
            pDetail( &stPbocList[i], i+1, TxDetail );
        }
        else
        {
            if ( nType == 0x01)
            {
                Pboc_OrgLogDetail( stPbocList[i], i+1, TxDetail );
            }
            else
            {
                Pboc_OrgCredLogDetail( stPbocList[i], i+1, TxDetail );
            }
        }
        i++;
    }

    DbgPrintf("����ȡ��־���ݡ� ���� :%s \n", TxDetail);
    return RET_SUCCESS;
}

/*!
 * \brief          tFunc_DeviceAuth    
 * \par ˵��       ���ð�ȫ��֤ 	    
 * \retval         0 ��ȫ��֤�ɹ� -1  ��ȫ��֤ʧ��
 * \note                
 */
int tFunc_DeviceAuth()
{
	int nRet = RET_SUCCESS, i = 0, nRandomAuthFlag = 0, nBufLen = 0;
	char sRandom[32+1] = {0x00};
	char sRandomHex[16+1] = {0x00};
	char szEncRandom[32] = {0x00};    /*<  ���ܺ�������  */
	char szRecvEncData[32] = {0x00};  /*<  ���յ����������� */
	char szTemp[64] = {0x00};
	char szCmd[1024] = {0x00};
	char szBuf[256] = {0x00};
	char szDivEncData[64] = {0x00};
	char szNewRandom[32] = {0x00};
	char szCheckStr[16]= "CENTERM==ZNZDCPB" ;
	char szNormalKey[16] = {0x00};
	char szNormalKey1[16] = "\x7E\x1A\xD6\x40\x59\xAE\xC7\x1C\xF1\xAB\xCA\x33\x49\x94\x8C\x75";
	char szNormalKey2[16] = "\xE0\xD7\x3D\xB9\x6D\x2A\x5F\x95\xB2\xAF\x22\x3D\x57\xBD\x9A\x7A";
	char szRandomKey1[16] = "\x41\x15\x89\x07\x62\x8E\x21\xC0\xCB\xF2\x75\xE0\x40\x39\x26\xEB";
	char szRandomKey2[16] = "\x30\x4C\xC2\x19\x68\x35\xD6\xE8\x59\x6A\xFF\x15\x8A\xEE\x6F\x6F";
	char szXorData[32] = {0x00}; 
	char szRandomKey[16] = {0x00};


	/*< �����а�ȫ��֤  */
#ifndef DEVICE_AUTH
		return RET_SUCCESS;
#endif
	DbgPrintf("���豸��ȫ��֤��\n");

	/*<  ��ȡ����� R1*/
	GetRandom( 16, sRandomHex );
	DbgPrintHex("���豸��ȫ��֤�� ��ȡ�����\n",sRandomHex , 16 );

	/*< ������Կ���������R1 ���� VerifyData1 */
	DbgPrintf("���豸��ȫ��֤�� ������Կ���������\n");
	for ( i = 0; i < DEVICE_KEY_LEN; i++ )
	{
		szNormalKey[i] = (BYTE)(szNormalKey1[i]^szNormalKey2[i]);
	}
	for ( i = 0; i < DEVICE_KEY_LEN; i++ )
	{
		szRandomKey[i] = (BYTE)(szRandomKey1[i]^szRandomKey2[i]);
	}
	ExEncryptDES_BYTE( szNormalKey, DEVICE_KEY_LEN, sRandomHex, 16, szEncRandom);

	/*< ������������  */
	memset( szBuf, 0x00, sizeof(szBuf) );
	SplitData( (char *)szEncRandom, DEVICE_KEY_LEN, szBuf );
	if (((sRandomHex[0]^sRandomHex[9]) == 0xFF)&&((sRandomHex[3]^sRandomHex[11]) == 0xFF))
	{
		nRandomAuthFlag = 1;
	}

	/*<  ������ð汾��Ϣָ�� */
	sprintf( szCmd, "\x1B[0D\x02%s\x03", szBuf);

	/*<  �·�ָ����ս�� */
	nBufLen = DEVICE_KEY_LEN*2 + 6;
	if ( nBufLen != ICC_WriteDataToTransPort( szCmd, nBufLen ) )
	{
		DbgPrintf("����ȫ��֤�� ��������ʧ�� ");
		nRet = RET_DEVICE_AUTH_FAIL;
	}
	else
	{
		/*<  ��ȡ�������� */
		memset( szBuf, 0x00, sizeof( szBuf) );
		nBufLen = sizeof(szBuf);
		nRet =  ICC_ReadDataFromTransPort( szBuf, nBufLen) ;
		if (nRet <= 0)
		{
			return RET_DEVICE_AUTH_FAIL;
		}
		/*< �ӵڶ����ֽڿ�ʼ����  */
		memcpy(szDivEncData, &szBuf[2], strlen(szBuf)-3);
		MergeData( szDivEncData, strlen(szDivEncData), szRecvEncData );

		/*<  ��ӡ�豸�������� */
		memset(szTemp, 0, sizeof(szTemp));
		HexToAscii((BYTE*)szRecvEncData, strlen(szDivEncData)/2, szTemp, sizeof(szTemp) );
		DbgPrintf( "����ȫ��֤���豸�������ģ�%s", szTemp);

		/*<  �������� */
		if (nRandomAuthFlag == 1)
		{	
			ExDecryptDES_BYTE( szRandomKey, DEVICE_KEY_LEN, (BYTE *)szRecvEncData,strlen(szDivEncData)/2 , szXorData);
		}
		else
		{
			ExDecryptDES_BYTE( szNormalKey, DEVICE_KEY_LEN, (BYTE*)szRecvEncData, strlen(szDivEncData)/2, szXorData);
		}

		/*<  ��ӡ���������� */
		memset( szTemp, 0, sizeof(szTemp));
		HexToAscii( szXorData, DEVICE_KEY_LEN, szTemp, sizeof(szTemp) );
		DbgPrintf( "����ȫ��֤������������Ϊ��%s\n", szTemp);

		/*<  ��ȡ����� */
		for ( i = 0; i < 16; i++)
		{
			szNewRandom[i] = (BYTE)(szXorData[i]^szCheckStr[i]);
		}

		/*<  ��ӡ���������� */ 
		memset(szTemp, 0, sizeof(szTemp));
		HexToAscii( szNewRandom, DEVICE_KEY_LEN, szTemp);
		DbgPrintf( "����ȫ��֤�������Ϊ��%s \n", szTemp);

		/*< �ж���֤�Ƿ�ɹ�  */
		if (memcmp( sRandomHex, szNewRandom, DEVICE_KEY_LEN) == 0)
		{
			nRet = RET_SUCCESS;
			DbgPrintf("����ȫ��֤����ȫ��֤�ɹ���\n");
		}
		else
		{
			nRet = RET_DEVICE_AUTH_FAIL;
			DbgPrintf("����ȫ��֤����ȫ��֤ʧ�ܣ�\n");
		}
	}
	return nRet;
}

/*!
 * \brief        tFunc_GetPbocVersion        
 * \par ˵��     ��ȡPBOC�İ汾��  	
 * \param[out]   nPbocType   0 -- 2.0 ��Ƭ  1 -- 3.0��Ƭ
 * \retval       
 * \note         ͨ���жϿ�Ƭ��9F08��ǩ        
 */
void tFunc_GetPbocVersion(int  * pnPbocType)
{
	unsigned char sValue[1024] = {0x00};
	int nValueLen = 0, nRet = RET_SUCCESS;

	DbgPrintf("����ȡ��Ƭ�汾��");
	nRet = centerm_get_tag_value( EMV_TAG_9F08_IC_APPVERNO, 2, g_ICCardParam.szIccAppData, g_ICCardParam.nIccAppDataLen, &nValueLen, sValue );
	if( ( nRet == RET_SUCCESS ) && ( nValueLen > 0 ) )
	{
		if( memcmp( sValue, "\x00\x20", 2) == 0)	
		{
			*pnPbocType = 0;
		}
		else
		{
			*pnPbocType = 1;
		}
	}
	else
	{
		*pnPbocType = 0;
	}
}
