#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GlobalDef.h"
#include "CardReaderLayer.h"
#include "TransMssion.h"
#include "CKB51XX.h"
#include "ICC_CardCmd.h"
#include "StrOp.h"
#include "TlvOp.h"
#include "ECash.h"
#include "TransTag.h"
#include "Debug.h"
#include "PrjDef.h"


//R20005 add by sulin �޸�g_ucTagArr[]��������λ�ô���bug��ת��ECash�ж���
U8 g_ucTagArr[] =
{
	'\x04','g',    //Bitel Defined DataEOT 
	'\x17','b',    //Bitel Defined TrmScript 
	'\x18','b',    //Bitel Defined TrmCAPK  
	'\x19','b',    //Bitel Defined PDOL_data 
	'\x1A','b',    //Bitel Defined CDOL1data 
	'\x1B','b',    //Bitel Defined CDOL1data 
	'\x4F','v',    //Ӧ�ñ�ʶ
	'\x50','a',    //Ӧ�ñ�ǩ
	'\x51','b',    //K-CASH only? 
	'\x57','b',    //������Ч����
	'\x5A','c',    //Ӧ�����ʺ�
	'\x5D','c',    //Ŀ¼�����ļ���DDF������
	'\x5F','\x20','a',   //�ֿ�������
	'\x5F','\x24','n',   //Ӧ��ʧЧ����
	'\x5F','\x25','n',   //Ӧ����Ч����
	'\x5F','\x28','n',   //�����й��Ҵ���
	'\x5F','\x2A','n',   //���׻��Ҵ���
	'\x5F','\x2D','a',   //��ѡ����
	'\x5F','\x30','n',   //������
	'\x5F','\x34','n',   //Ӧ�����ʺ�(PAN)ϵ�к�
	'\x5F','\x36','n',   //���׻���ָ��
	'\x5F','\x50','a',   //������URL
	'\x61','t',
	'\x6F','t',
	'\x70','t',    //'t'��ʾ�ṹ��
	'\x71','t',    //�����нű�1
	'\x72','t',    //�����нű�2
	'\x73','t',    //Ŀ¼�Զ���ģ��
	'\x77','t',    //��Ӧ����ģ���ʽ2
	'\x80','b',    //��Ӧ����ģ���ʽ1 
	'\x81','b',    //������Ȩ���
	'\x82','b',    //Ӧ�ý�������
	'\x84','v',    //ר���ļ���DF������
	'\x83','b',    //����ģ��
	'\x86','b',    //�����нű�
	'\x87','b',    //Ӧ�����ȼ�ָʾ��
	'\x88','b',    //���ļ���ʾSFI
	'\x89','a',    //��Ȩ��
	'\x8A','a',    //��ȨӦ����
	'\x8C','b',    //��Ƭ���չ������ݶ����б�1(CDOL1)
	'\x8D','b',    //��Ƭ���չ������ݶ����б�2(CDOL2)
	'\x8E','b',    //�ֿ�����֤����(CVM)�б�
	'\x8F','b',    //��֤���Ĺ�Կ����
	'\x90','b',    //�����й�Կ֤��
	'\x91','b',    //�����м�������
	'\x92','b',    //�����й�Կ����
	'\x93','b',    //ǩ����̬Ӧ������
	'\x94','b',    //Ӧ���ļ���λ��
	'\x95','b',    //�ն���֤���
	'\x97','b',    //����֤�����ݶ����б�(TDOL)
	'\x98','b',    //����֤��(TC)��ϣֵ
	'\x99','b',    //���׸�������(PIN)����
	'\x9A','n',    //��������
	'\x9B','b',    //����״̬��Ϣ
	'\x9C','n',    //��������
	'\x9D','b',    //Ŀ¼�����ļ�(DDF)��
	'\x9F','\x01','n',   //�յ��б�ʶ
	'\x9F','\x02','n',   //��Ȩ���(����)
	'\x9F','\x03','n',   //�������(����)
	'\x9F','\x04','b',   //�������(������)
	'\x9F','\x05','b',   //Ӧ���Զ�������
	'\x9F','\x06','b',   //Ӧ�ñ�ʶ
	'\x9F','\x07','b',   //Ӧ����;����
	'\x9F','\x08','b',   //Ӧ�ð汾�š�
	'\x9F','\x09','b',   //Ӧ�ð汾��
	'\x9F','\x0B','a',   //�ֿ���������չ
	'\x9F','\x0D','b',   //��������Ϊ����-ȱʡ
	'\x9F','\x0E','b',   //��������Ϊ����-�ܾ�
	'\x9F','\x0F','b',   //��������Ϊ����-����
	'\x9F','\x10','b',   //������Ӧ������
	'\x9F','\x11','n',   //�����д��������
	'\x9F','\x12','a',   //Ӧ����ѡ��
	'\x9F','\x13','b',   //��������������(ATC)�Ĵ���
	'\x9F','\x14','b',   //�����ѻ���������
	'\x9F','\x15','n',   //�̻�������
	'\x9F','\x16','a',   //�̻���ʶ
	'\x9F','\x17','b',   //��������(PIN)���Դ���
	'\x9F','\x18','b',   //�����нű���ʶ
	'\x9F','\x1A','n',   //�ն˹��Ҵ���
	'\x9F','\x1B','b',   //�ն�����޶�
	'\x9F','\x1C','a',   //�ն˱�ʶ
	'\x9F','\x1D','b',   //�ն˷��չ�������
	'\x9F','\x1E','a',   //�ӿ��豸(IFD)ϵ�к�
	'\x9F','\x1F','a',   //һ�ŵ���Ч����
	'\x9F','\x20','c',   //���ŵ��Զ�������
	'\x9F','\x21','n',   //����ʱ��
	'\x9F','\x22','b',   //��֤���Ĺ�Կ����
	'\x9F','\x23','b',   //�����ѻ���������
	'\x9F','\x24','b',   //Ӧ��ʧЧ����
	'\x9F','\x25','n',   //Ӧ����Ч����
	'\x9F','\x26','b',   //Ӧ������
	'\x9F','\x27','b',   //������Ϣ����
	'\x9F','\x2D','b',   //IC��PIN���ܹ�Կ֤��
	'\x9F','\x2E','b',   //IC��PIN���ܹ�Կָ��
	'\x9F','\x2F','b',   //IC��PIN���ܹ�Կ����
	'\x9F','\x32','b',   //�����й�Կָ��
	'\x9F','\x33','b',   //�ն�����
	'\x9F','\x34','b',   //�ֿ�����֤����(CVM)���
	'\x9F','\x35','n',   //�ն�����
	'\x9F','\x36','b',   //Ӧ�ý���ϵ�к�
	'\x9F','\x37','b',   //�����
	'\x9F','\x38','b',   //����ѡ�����ݶ����б�(PDOL)
	'\x9F','\x39','n',   //POS���뷽ʽ
	'\x9F','\x3A','b',   //�ο����ҽ��
	'\x9F','\x3B','n',   //Ӧ�òο�����
	'\x9F','\x3C','n',   //���ײο�����
	'\x9F','\x3D','n',   //���ײο�����ָ��
	'\x9F','\x40','b',   //�����ն�����
	'\x9F','\x41','n',   //������ż�����
	'\x9F','\x42','n',   //Ӧ�û��Ҵ���
	'\x9F','\x43','n',   //Ӧ�òο�����ָ��
	'\x9F','\x44','n',   //Ӧ�û���ָ��
	'\x9F','\x45','b',   //������֤��
	'\x9F','\x46','b',   //IC����Կ֤��
	'\x9F','\x47','b',   //IC����Կָ��
	'\x9F','\x48','b',   //IC����Կ����
	'\x9F','\x49','b',   //��̬���ݶ����б�(DDOL)
	'\x9F','\x4A','b',   //��̬���ݼ�����ǩ�б�
	'\x9F','\x4B','b',   //ǩ����̬Ӧ������
	'\x9F','\x4C','b',   //DDA��������У���Ƭ���ɵ���ʱ��仯��ͬ�������
	'\x9F','\x4D','b',   //��־���
	'\x9F','\x4E','b',   //�̻�����
	'\x9F','\x4F','b',   //��־��ʽ
	'\x9F','\x51','n',   //ʹ�õĻ��Ҵ���
	'\x9F','\x52','b',   //VISA only    
	'\x9F','\x53','a',   //�����ѻ�����������
	'\x9F','\x53','b',   //VISA only 
	'\x9F','\x54','n',   //�ۼ��ѻ����׽��������
	'\x9F','\x55','b',   //VISA only 
	'\x9F','\x56','b',   //��������ָ֤ʾλ
	'\x9F','\x57','n',   //�����й��Ҵ���
	'\x9F','\x58','b',   //�����ѻ���������
	'\x9F','\x59','b',   //�����ѻ���������
	'\x9F','\x5A','a',   //������URL2
	'\x9F','\x5B','b',
	'\x9F','\x5C','n',   //�ۼ��ѻ����׽������
	'\x9F','\x5D','n',   //�����ѻ����ѽ��
	'\x9F','\x61','a',   //�ֿ���֤����
	'\x9F','\x62','c',
	'\x9F','\x66','b',   //�ն˽�������
	'\x9F','\x68','b',   //��13����Ƭ���Ӵ���
	'\x9F','\x6B','n',   //��Ƭ�ֿ�����֤��������
	'\x9F','\x6C','b',   //��Ƭ��������
	'\x9F','\x6D','n',   //�����ֽ�������ֵ
	'\x9F','\x72','b',   //�����ѻ�����������
	'\x9F','\x73','n',   //����ת������
	'\x9F','\x74','a',   //VISA only 
	'\x9F','\x75','n',   //�ۼ��ѻ����׽��������
	'\x9F','\x76','n',   //��2Ӧ�û���
	'\x9F','\x77','n',   //�����ֽ��������
	'\x9F','\x78','n',   //�����ֽ𵥱ʽ����޶�
	'\x9F','\x79','n',   //�����ֽ����
	'\x9F','\x7A','n',   //�����ֽ��ն�֧��ָʾ��
	'\x9F','\x7B','n',   //�����ֽ��ն˽����޶�
	'\x9F','\x7F','b',   //VISA only 
	'\xA5','t',          //FCIר��ģ��
	'\x0BF','\x0C','t', //FCI�������Զ�������
	'\xC1','v',          //MONETA only? 
	'\xC2','v',          //MONETA only?  
	'\xDF','\x60','b',    //CAPP����ָʾλ      
	'\xDF','\x69','b'    //����֧��ָʾλ  
};

S32 ICC_GetAidList( U8 *pszDDFname,  S8 nDDFnameLen,  AID_LIST **ppAidList,  S32 *pnAidNum  )
{
    S32  nRetVal = RET_SUCCESS;
    S32  nAidNum = 0;
    S32  nAdfEntryLen=0;
    S32  num = 0;
    U8   *p = NULL;
    U8   *pTmp = NULL;
    U8   ucData[  MAX_LEN ];
    S32  nDataLen = 0;
    S32  nRecordLen = 0;
    U8   SFI = 0;
    U8   P1 = 0;
    U8   Index = 0;
    AID_LIST   *pAidList = NULL;
    AID_LIST   *pAidListHead = NULL;
    RECORD_LIST  *pRecordList = NULL;
    RECORD_LIST  *pRecordListHead = NULL;

    
    /*!<���ļ���ѡDDF*/
    memset( ucData, 0x0, sizeof(ucData) );  
    DbgPrintf("ѡ���ļ�%s\n", pszDDFname );
    nRetVal = ICC_CardCmdSelectFile( 0x04, 0x00, pszDDFname, nDDFnameLen, ucData,  &nDataLen );
    DbgPrintHex("�ļ�ѡ����", ucData, nDataLen );
    if( RET_SUCCESS !=  nRetVal )
    {
         return nRetVal;    
    }
    
    /*!<���ұ�ǩ0x88�Ķ��ļ���ʶ����SFI��*/
    p = (U8 *)centerm_find_tag( (U8 *)TAG_SFI, strlen(TAG_SFI), ucData, nDataLen );    
    if( NULL == p )
    {
        DbgPrintf("δ���ҵ���ǩ0x88�Ķ��ļ���ʶ����SFI��\n" );
        return RET_NO_FOUND_SFI;
    }
    
    /*!<�ж�sfi�ĳ����Ƿ�������*/
    p++;
    if( (*p) != 0x01 )
    {
        *ppAidList=NULL;
        DbgPrintf("���ļ���ʶ����SFI�����ȴ���\n" );
        return RET_SFI_LEN_ERR;
    }

    // ��ȡ���м�¼
    p ++;
    SFI = (*p); //��¼�Ķ��ļ���
 
   /*!<����ͷ�ڵ�*/
    pRecordList = (RECORD_LIST *)malloc( sizeof(RECORD_LIST) );
    if( NULL == pRecordList )
    {
            return RET_MALLOC_ERR;    
    }
    pRecordListHead = pRecordList;/*!<�����ͷ��ַ*/
    memset( pRecordList, 0x0, sizeof( RECORD_LIST ) ); 
    
    while(1)
    {
        /*!<��ȡ��¼*/
        Index ++;/*!<��¼��*/
        nDataLen = 0;
        memset( ucData, 0x0, sizeof(ucData) );  
        nRetVal = ICC_CardCmdReadSFI(  SFI,  Index,  ucData, &nDataLen );
        if( RET_SUCCESS != nRetVal )
        {
             break;
        }
        DbgPrintf("��ȡӦ���б��ȡ SFI=%d Index=%d \n", SFI, Index );
        DbgPrintHex("���", ucData, nDataLen );
        
        /*!<�������¼�Ľڵ�*/
        pRecordList->pnext = (RECORD_LIST *)malloc( sizeof(RECORD_LIST) );
        memset( pRecordList->pnext, 0x0, sizeof(RECORD_LIST));
        
       /*!<�����¼����*/
        pRecordList->precord = (U8 *)malloc( nDataLen );
        memset( pRecordList->precord, 0x0, nDataLen);       
        memcpy( pRecordList->precord,ucData, nDataLen );
        pRecordList->nRecordLen = nDataLen;
        
        /*!<ָ����һ���ڵ�*/
        pRecordList = pRecordList->pnext;        


        if( 255 == Index )
         {
               break;
         }
         
    }   

    //  ����ÿ����¼   
    pRecordList->pnext=NULL;
    pRecordList->precord=NULL;    
    pRecordList = pRecordListHead;

    while(  pRecordList->pnext != NULL )   
    {                            
        //  ��ȡ��¼������Ŀ¼�������
        p = (U8 *)centerm_find_tag( (U8 *)TAG_APP,  strlen(TAG_APP),  pRecordList->precord,  pRecordList->nRecordLen );
        while( p )
        {
            /*!<�ж�tag����*/
            p++;                        
            if(   0  == (*p)  ) 
            {
                    p++;
                    goto nextloop;
            }
            
            nAdfEntryLen = (*p);  //  ָ���ǩ61����
            p++;

            if( (*p) == 0x4F )/*!<ADF ���ƣ�AID*/
            {
                p++;
                if( ((*p) < 5) || ((*p) >16) ) /*!< ���ȴ������*/
                {
                    p--;
                    goto nextloop;                  
                }

               if( pAidList )
               {
                      while( NULL != pAidList->next )
                      {
                          pAidList = pAidList->next;
                      }
                      
                      pAidList->next =  (AID_LIST *)malloc( sizeof(AID_LIST) );;
                      pAidList = pAidList->next;
               }
               else 
               {
                       pAidList =  (AID_LIST *)malloc( sizeof(AID_LIST) );;
               }                               
                memset( pAidList, 0x0, sizeof( AID_LIST ) );
                
                /*!<ADF ���ƣ�AID*/
                pAidList->AidLen = (*p);  /*!<aid ����*/
                p++;   //  pָ��tag=0x4F��ֵ
                memcpy( pAidList->AID, p, pAidList->AidLen );/*!<aid*/       
                DbgPrintHex("Ӧ�ñ�ʶAID", pAidList->AID, pAidList->AidLen  );
                                
                nAidNum ++;   
                if( 1 == nAidNum  )
                {
                     pAidListHead = pAidList;
                }


                /*!<Ӧ�ñ�ǩ*/
                p-=2;   //  ����ָ��tag=0x61��Ŀ¼��ڵ�ַ
                pTmp = (U8 *)centerm_find_tag( (U8*)TAG_APP_LABEL, 1, p,  nAdfEntryLen );
                if( !pTmp )
                {
                    DbgPrintf("Ӧ�ñ�ǩ(0x50)δ�ҵ�");
                    pAidList->AppTagLen=0;
                    goto nextloop;
                }
                
                pTmp += 1;
                if(  ( (*pTmp) < 1 ) ||( (*pTmp) > 16 ))/*!< ���ȴ������*/
                {
                    pAidList->AppTagLen=0;
                    goto nextloop;
                }
                
                pAidList->AppTagLen = (*pTmp);
                pTmp ++;
                memcpy(pAidList->AppTag, pTmp, pAidList->AppTagLen);
                DbgPrintHex("Ӧ�ñ�ǩ", pAidList->AppTag, pAidList->AppTagLen  );

                /*!<Ӧ������Ȩ��ʶ��*/
                pTmp = (U8 *)centerm_find_tag( (U8*)TAG_APP_PRIORITY_INDICATOR, strlen(TAG_APP_PRIORITY_INDICATOR ),  p, nAdfEntryLen );
                if( pTmp  )
                {
                    pTmp ++;
                    if( *pTmp )
                    {
                        pTmp ++;
                        pAidList->APPpriority = (*pTmp);
                        DbgPrintHex("Ӧ������Ȩ��ʶ��",  (U8*)&pAidList->APPpriority, 1  );
                    }
                }
                 
                /*!<Ӧ����������*/
                pTmp = (U8 *)centerm_find_tag( (U8*)TAG_APP_PREFERRED_NAME,  strlen(TAG_APP_PREFERRED_NAME), p,  nAdfEntryLen );
                if( pTmp )
                {
                    pTmp += 2;
                    if(  ( (*pTmp) >= 1 ) && ( (*pTmp) <= 16 ))/*!< ���ȴ������*/
                    {
                        pAidList->APNlen = (*pTmp);
                        pTmp ++;
                        memcpy(pAidList->APN, pTmp, pAidList->APNlen);
                        DbgPrintHex("Ӧ����������", pAidList->APN, pAidList->APNlen  );
                    }
                }

            }
            else if(  0x9D == (*p) )  //  DDF  Ŀ¼�����ļ����,Ŀ¼�����ļ���DDF��
            {
                p++;
                if( ((*p) < 5) || ((*p) >16) )  //  len=0,����
                {
                    p--;
                    goto nextloop;
                }
                
                while( NULL != pAidList->next )
                {
                    pAidList = pAidList->next;
                }
                
                if( RET_SUCCESS == ICC_GetAidList( p+1,  (*p),  &(pAidList->next) ,  &num  ) )
                {
                    if( num > 0 )
                    {
                        nAidNum += num;
                    }
                }
                
            }

nextloop:       
            p = (U8 *)centerm_find_tag( (U8 *)TAG_APP, strlen( TAG_APP ), p, pRecordList->nRecordLen-( pRecordList->precord - p ) );
        }

        //  ָ����һ����¼
        pRecordList = pRecordList->pnext;
        
    }

    *ppAidList = pAidListHead;//��Aid�б�ֵ��ppAidList   
    *pnAidNum = nAidNum;
    nRetVal = RET_SUCCESS;
    
    //�ͷ�������ڴ�pRecordList
    while( NULL != pRecordListHead )
    {
        if( NULL != pRecordListHead->precord )
        {
            free( pRecordListHead->precord );  
            pRecordListHead->precord = NULL;
        } 
        pRecordList = pRecordListHead;
        pRecordListHead = pRecordListHead->pnext;
        free( pRecordList );
    }
    
    return nRetVal;
} 
   
S32 ICC_GetAppList(  AID_LIST **ppAidList,  S32 *pnAidNum  )
{
    S8 *pszDDFname = "1PAY.SYS.DDF01";
    return ICC_GetAidList( (U8 *)pszDDFname,  strlen(pszDDFname),  ppAidList, pnAidNum  );
}

void ICC_FreeAidList( AID_LIST *pAidList )
{
    AID_LIST *pTmpAidListOne = pAidList;
    AID_LIST *pTmpAidListTwo = pAidList;   
     
    while ( pTmpAidListOne ) 
    {
         pTmpAidListTwo = pTmpAidListOne;
         pTmpAidListOne = pTmpAidListOne->next;
         free( pTmpAidListTwo );
    }  
    
}

S32 ICC_AppSelect( U8* AID, S32 nAidLen,   U8 *szFci, S32 *pnFciLen )
{
    S32 nRetVal = ICC_CardCmdSelectFile( 0x04, 0x00, AID, nAidLen, szFci, pnFciLen );
    
    DbgPrintf("Ӧ��ѡ��ʼ...\n ");
    DbgPrintHex("ѡ���Ӧ�ñ�ʶ(aid)",  AID, nAidLen );
    
    if( RET_SUCCESS == nRetVal )
    {
        DbgPrintf("Ӧ��ѡ��ɹ���\n ");
        DbgPrintHex("���ص��ļ�������Ϣ(fci)",  szFci, *pnFciLen );
    }
    else 
    {
        DbgPrintf("Ӧ��ѡ��ʧ�ܣ�\n ");
    }
    
    return nRetVal;
}

S32 ICC_GetPDOL(   U8  *szFci,  S32 nFciLen,  U8*szPdol,  S32 *pnPdolLen )
{
    S32 nRetVal = RET_DOL_NO_FOUND;
    
    DbgPrintf("��ȡPDOL�б�\n");   
      
    nRetVal = ICC_GetDOL( (U8 *)TAG_PDOL,  strlen(TAG_PDOL), szFci, nFciLen, szPdol, pnPdolLen );            

    if( RET_SUCCESS ==  nRetVal )
    {
        DbgPrintHex( "PDOL�б�", szPdol, *pnPdolLen );   
    }
    else 
    {
        DbgPrintf("��ȡPDOL�б�ʧ��\n"); 
    }
    
    return nRetVal;
}

S32 ICC_GetTagInDOL( U8* szDol,  S32 nDolLen, U8* szTagName, S32 *pnTagNameLen, S32  *pnTagLen )
{

    U8 szTag[ MAX_LEN ] = {0};
    S32 nTagLen = 0;
    S32 nPos = 0;

    if(  ( !szDol ) || ( nDolLen<=1 ) ||( 0xFF == szDol[0] ) )
    {
         return 0;
    }
    
    /*!<����tag*/
    szTag[ nTagLen++ ] = szDol[nPos];
    /*!<  ����λΪȫ1����ʾtag�Ƕ��ֽڱ�ǩ������Ϊ���ֽڱ�ǩ*/
    if( 0x1F == ( szDol[nPos++]&0x1F ) )    
    {    
        //  b8=1,���滹��һ�ֽڣ�b8=0�����һ���ֽ�
        while(   ( nPos < nDolLen ) && ( szDol[nPos]&0x80 )  ) 
        {
            if( 0xFF == szDol[ nPos ]  )
            {
                 return 0;
            } 
            szTag[ nTagLen++ ] = szDol[ nPos++ ];
        }
        
        if ( nPos  >= nDolLen )
        {
            return 0;
        }      
              
        szTag[ nTagLen++] = szDol[ nPos++ ];
    }
    
    memcpy( szTagName, szTag, nTagLen );
    *pnTagNameLen = nTagLen;
    szTagName[ nTagLen ]=0x00;
    
    /*!<tag���� 3���ֽ�*/
    if(  0x82 == szDol[ nPos ]  )
    {
        if( (nPos+3) > nDolLen )
        {
            return 0;
        }
            
        (*pnTagLen) = ( szDol[ nPos+1 ]*256 + szDol[ nPos+2 ] )&0xFFFF;
        nPos += 3;
    }
     /*!<tag����2���ֽ�*/
    else if( 0x81 == szDol[ nPos ] )
    {
        if( (nPos+2) > nDolLen  )
        {
             return 0;
        }
             
        (*pnTagLen) = szDol[ nPos+1 ];
        nPos += 2;
    }
      /*!<tag����1���ֽ�*/
    else
    {
        
        if( (nPos+1) > nDolLen  )
        {
            return 0;
        }

        //  0-127,һ�ֽڳ���  TLV ���Ⱥ����Ǳ�ǩ
        (*pnTagLen) = szDol[ nPos ];           
        nPos += 1;
        
    }
    
    DbgPrintf( "��PDOL��ȡtagֵ�ĳ���Ϊ %d\n",  *pnTagLen );
    DbgPrintHex( "tag", szTagName,   nTagLen);
    
    return nPos;

}

S32 ICC_GetPdolTermData(  struct TERMDATA *pTermData, U8  *szPdol, S32 *pnPdolLen )
{
    S32  nPdolDataLen = 0;
    U8   szDate[16] = {0};
    U8   szTime[16] = {0};
    U8   ucPdolData[ MAX_LEN ] = {0};
    
    memset( ucPdolData, 0x00, sizeof(ucPdolData) );
    nPdolDataLen = 0;
    
    //  ��Ȩ���
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x02",  2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 0x06;
    memcpy( ucPdolData+nPdolDataLen, "\x00\x00\x00\x00\x00\x00", 6 );
    nPdolDataLen += 6;
 
    //  �������
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x03", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 0x06;
    memcpy( ucPdolData+nPdolDataLen, "\x00\x00\x00\x00\x00\x00",  6 );
    nPdolDataLen += 6;
    
    //  �ն˹��Ҵ���
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x1A", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 0x02;   
    memcpy( ucPdolData+nPdolDataLen,  "\x01\x56",  2 );
    nPdolDataLen += 2;
    
    //  �ն���֤���
    memcpy( ucPdolData+nPdolDataLen, "\x95", 1 );   
    nPdolDataLen += 1;
    ucPdolData[ nPdolDataLen++ ] = 0x05;
    memcpy( ucPdolData+nPdolDataLen, "\x00\x00\x00\x08\x00", 5 );
    nPdolDataLen += 5;
    
    //  ������Ȩ�ı������� 
    memcpy( ucPdolData+nPdolDataLen, "\x9A", 1 );   
    nPdolDataLen += 1;
    ucPdolData[ nPdolDataLen++] = 0x03;
    memset( szDate, 0, sizeof(szDate) );
    GetTransDate( szDate );
    AsciiToBcd( szDate, 6, ucPdolData+nPdolDataLen , 0x03 );
    nPdolDataLen += 3;
    
    //������Ȩ�ı���ʱ��
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x21", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++] = 0x03;
    memset( szTime, 0, sizeof(szTime) );
    GetTransTime( szTime );
    AsciiToBcd( szTime, 6, ucPdolData+nPdolDataLen , 0x03 );
    nPdolDataLen += 3;
    
    //  ���׻��Ҵ���
    memcpy( ucPdolData+nPdolDataLen, "\x5F\x2A", 2 );  
    nPdolDataLen+=2;
    ucPdolData[ nPdolDataLen++ ] = 2;
    memcpy( ucPdolData+nPdolDataLen, "\x01\x56",  2 );
    nPdolDataLen+=2;
    
    //  ��������
    memcpy( ucPdolData+nPdolDataLen,"\x9C", 1 );   
    nPdolDataLen += 1;
    ucPdolData[ nPdolDataLen++ ] = 1;
    memcpy( ucPdolData+nPdolDataLen, "\x01", 1 );
    nPdolDataLen += 1;

    //  �����
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x37", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 4;
    GetRandom( 4, ucPdolData+nPdolDataLen );
    nPdolDataLen += 4;
    
    //  ��1���ն˽�������
    memcpy( ucPdolData+nPdolDataLen,"\x9F\x66", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++] = 4;
    memcpy( ucPdolData+nPdolDataLen, "\xF4\x00\x00\x00",4  );
    nPdolDataLen += 4;
    
    //  �����ֽ��ն�֧��ָʾ��
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x7A", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 1;
    ucPdolData[ nPdolDataLen++ ] = pTermData->ECardIndex;
    
    //  �����ֽ��ն˽����޶�
    memcpy( ucPdolData+nPdolDataLen,"\x9F\x7B", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 6;
    memcpy( ucPdolData+nPdolDataLen,"\x00\x00\x01\x00\x00\x00", 6 );
    nPdolDataLen += 6;
    
    //  �̻�����
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x4E", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 20;
    memset( ucPdolData+nPdolDataLen, 0x0, 20 );
    memcpy( ucPdolData+nPdolDataLen, pTermData->BankName, strlen(pTermData->BankName) );
    nPdolDataLen += 20;

	//R20009 add by sulin  pboc3.0 Ҫ��֧��˫���㷨,���PBOC3.0��14����
	memcpy( ucPdolData+nPdolDataLen,"\xDF\x69",2);   
	nPdolDataLen+=2;
	ucPdolData[nPdolDataLen++] = 1;
    ucPdolData[nPdolDataLen++] = pTermData->EncryptIndex;

	//R20009 add by sulin  pboc3.0 ����ָʾλ,���PBOC3.0��14����
	memcpy( ucPdolData+nPdolDataLen,"\xDF\x60",2);   
	nPdolDataLen+=2;
	ucPdolData[nPdolDataLen++]=1;
	memcpy(ucPdolData+nPdolDataLen,"\x00",1);
	nPdolDataLen+=1;

	//R20009 add by sulin  pboc3.0 ��ʾ��Ƭ��֧�ֶַο۷ѹ���Ҳ֧���ѻ�Ԥ��Ȩ����,���PBOC3.0��14����
	memcpy( ucPdolData+nPdolDataLen,"\xDF\x61",2);   
	nPdolDataLen+=2;
	ucPdolData[nPdolDataLen++]=1;
	memcpy(ucPdolData+nPdolDataLen,"\x02",1);
	nPdolDataLen+=1;
    
	//�ն�����
	memcpy( ucPdolData+nPdolDataLen,"\x9F\x33",2);   
	nPdolDataLen+=2;
	ucPdolData[nPdolDataLen++]=3;
	memcpy(ucPdolData+nPdolDataLen,"\xE0\xE1\xC8",3);
	nPdolDataLen+=3;

    //��������
    memcpy( szPdol, ucPdolData, nPdolDataLen );
    *pnPdolLen = nPdolDataLen;
    DbgPrintHex("����PDOL���ն�����", ucPdolData, nPdolDataLen );
    
    return RET_SUCCESS;
}


S32 ICC_GetIccDataByDOL( U8 *szDol, S32 nDolLen, U8 *szDolData,  S32  nDolDataLen,  U8 *szIccData,   S32 *pnIccDataLen  )
{

    U8  szTagName[ 256 ] = {0};
    S32 nTagNameLen = 0;
    S32 nTagLen = 0;
    U8 *pTmpDol = szDol;
    S32 nTmpDolLen = nDolLen;
    S32 nRetVal = RET_SUCCESS; 
    U8 *p = NULL;
    S32 nDataLen = 0;
    S32 nTagType = 0;
      
    if(  !szDol  || !nDolLen  || !szDolData  || !nDolDataLen || !szIccData  || !pnIccDataLen )
    {
        return RET_INVALID_PARAM;
    }
    
    *pnIccDataLen = 0;
    
    while( nTmpDolLen  >  0 )
    {
        //�����ݶ���pdol�б�Ѱ�ұ�ǩ����
        nTagNameLen = 0;
        nTagLen = 0;
        memset( szTagName, 0x0, sizeof(szTagName) );
        
        nRetVal = ICC_GetTagInDOL( pTmpDol,  nTmpDolLen, szTagName, &nTagNameLen, &nTagLen );
        if( nRetVal < 0  )
        {
            *pnIccDataLen = 0;
            DbgPrintHex("DOL�쳣", szDol,  nDolLen );
            return RET_DOL_DATA_ERR;
        }

        //��һ��ѭ������
        nTmpDolLen -= nRetVal;
        pTmpDol       += nRetVal;

        //���ն�Ӧ��������Ѱ���Ա�ǩΪ��ʼ��Ӧ������
        memset( szIccData + (*pnIccDataLen),  0,  nTagLen );
        p = centerm_find_tag( szTagName, nTagNameLen, szDolData, nDolDataLen );
        if( !p )
        {
          *pnIccDataLen = 0;
           DbgPrintHex( "�ն�����(szDolData)������ָ����ǩ��ֵ���ñ�ǩΪ", szTagName, nTagNameLen );
           DbgPrintHex(  "�ն�����(szDolData)", szDolData, nDolDataLen);
            return RET_DOL_DATA_NO_FOUND;
        }

        //�����ն�Ӧ���������ǩ��ʵ�ʳ��ȣ�datalen��
        p += nTagNameLen;
        if( 0x82 == (*p)  )             
        {
            nDataLen = (*(p + 1)) * 256 + (*(p + 2));
            p += 3;
        }
        else if( 0x81 ==  (*p)  )   
        {
            nDataLen = (*(p + 1));
            p += 2;
        }
        else
        {
             nDataLen = (*p);                    
             p += 1;
        }

         //���ݱ�ǩ���ұ�ǩ������,�ڱ�ǩ��������
         nTagType = centerm_get_tag_type(  szTagName, nTagNameLen, (U8 *)g_ucTagArr, sizeof(g_ucTagArr) );
         if( !nTagType ) 
         {
                (*pnIccDataLen) += nTagLen;
                continue;
         }
            
        //���ݱ�ǩ���Ͷ�Ӧ�����ݹ�����ʵ�ʵ�Ӧ�����ݵ�Ԫ
        if( nTagType == 't' )       
        {
            //�����ǩ�ǽṹ��ǩ�Ļ���ȫ������ 
            (*pnIccDataLen) += nTagLen;
        }
        else if( nTagLen == nDataLen)       
        {
            //�����ǩָ�����Ⱥ�ʵ�ʳ�����ȣ��򿽱�����
            memcpy( szIccData + (*pnIccDataLen), p, nTagLen );
            (*pnIccDataLen) += nTagLen;
        }
        else if( nTagLen < nDataLen)       
        {
            //�����ǩָ������С��ʵ�ʳ��ȣ�����������
            if(nTagType == 'n')     
            {
                //����Ǹ�ʽ'n'�������߿�ʼ����
                p += nDataLen - nTagLen;
                memcpy( szIccData + (*pnIccDataLen), p, nTagLen );
            }
            else                 
            {
                //������ʽ������ұ߿�ʼ����
                memcpy( szIccData + (*pnIccDataLen), p, nTagLen );
                (*pnIccDataLen) += nTagLen;

            }
        }   
        else if( nTagLen > nDataLen)       
        {
            //�����ǩָ�����ȴ���ʵ�ʳ��ȣ����������
            if( nTagType == 'n' )     
            {
                //����Ǹ�ʽ'n'�������߿�ʼ���
                (*pnIccDataLen) += nTagLen - nDataLen;
                memcpy( szIccData + (*pnIccDataLen), p, nDataLen);
                (*pnIccDataLen) += nDataLen;
            }
            else
            {
                //�����cn��ʽ�Ļ�����ĩβ����FF.
                if( nTagType == 'c' )     
                {
                    memset(szIccData + (*pnIccDataLen), 0xFF, nTagLen);             
                }
                else    
                {
                    //�����������ʽ���ں��油��
                    memset( szIccData + (*pnIccDataLen),  0x0, nTagLen );
                }
                memcpy( szIccData + (*pnIccDataLen), p, nDataLen);
                (*pnIccDataLen) += nTagLen;
            }       
        }
    }
    
    DbgPrintHex("IC��Ҫ���DOL����Ϊ", szIccData, *pnIccDataLen );
    return RET_SUCCESS;
}

S32 ICC_AppInitByTermData( struct TERMDATA *pTermData , U8* szFci, S32 nFciLen, U8* szAfl, S32* pnAflLen,  U8* szAip, S32* pnAipLen )
{
   S32 nRetVal = RET_ERROR;
   U8  szPdol[ MAX_LEN ] = {0};
   S32 nPdolLen = 0;
   U8  szPdolData[ MAX_LEN ] = {0};
   S32 nPdolDataLen = 0;
   U8  szIccData[ MAX_LEN ] = {0};
   S32 nIccDataLen = 0;
   U8  szOutData[ MAX_LEN ] = {0};
   S32 nOutDataLen = 0;
   U8  szTagValue[ MAX_LEN ] = {0};
   S32 nTagValueLen = 0;
   
   DbgPrintf("��ʼӦ�ó�ʼ��.......\n");
   
    /*!<���ļ�������Ϣ�л�ȡPDOL�б�*/
    DbgPrintf("���ļ�������Ϣ�л�ȡPDOL�б�\n");
    
    if( RET_SUCCESS == ICC_GetPDOL(   szFci,  nFciLen,  szPdol,  &nPdolLen ) )
    {
         DbgPrintf("��֯IC��Ҫ����ն�����\n");
         ICC_GetPdolTermData(  pTermData, szPdolData, &nPdolDataLen );
         ICC_GetIccDataByDOL( szPdol, nPdolLen, szPdolData,  nPdolDataLen,  szIccData,  &nIccDataLen  );
    }
    else 
    {
        /*!<��Ƭû�з���PDOL�б�����*/
        DbgPrintf("��Ƭû�з���PDOL�б�����\n");
    }
    
    DbgPrintf("���ͻ�ȡ����ѡ�GPO������֪ͨ��Ƭ���׿�ʼ\n");
    if( RET_SUCCESS == ICC_CardCmdGetProcessOptions( szIccData,   nIccDataLen, szOutData,  &nOutDataLen ) )
    {
         DbgPrintHex("GPO��Ӧ", szOutData, nOutDataLen );   
          /*��ʽ1 ����ǩΪ ��80�� �Ļ������ݶ���������ΪӦ�ý���������AIP����Ӧ���ļ���λ����AFL����ֵ�����Ӷ��ɣ������ݶ���֮��û�зָ�������ǩ�ͳ��ȣ���*/
         if( RET_SUCCESS ==  centerm_get_tag_value( "\x80", 1, szOutData, nOutDataLen, szTagValue, &nTagValueLen) )
         {
              memcpy( szAip, szTagValue, 2 );
              *pnAipLen = 2;  
              memcpy( szAfl, szTagValue+2, nTagValueLen - 2 );
              *pnAflLen = nTagValueLen - 2; 
              nRetVal = RET_SUCCESS;               
         }
         /*��ʽ2��
            ��Ӧ�����е����ݶ�����һ����ǩΪ��77���Ļ������ݶ�����������԰������BER-TLV�����
            ���󣬵�����Ҫ����Ӧ�ý���������AIP����Ӧ���ļ���λ����AFL����
         */
         else if(  RET_SUCCESS ==  centerm_get_tag_value( "\x77", 1, szOutData, nOutDataLen, szTagValue, &nTagValueLen )  )
         {
             /*��ȡAIP*/
             if( RET_SUCCESS ==  centerm_get_tag_value( TAG_AIP, 1, szTagValue, nTagValueLen, szAip, pnAipLen ) )
             {
                  DbgPrintf("��ȡAIP �ɹ���\n");
                  /*��ȡAFL*/
                 if( RET_SUCCESS ==  centerm_get_tag_value( TAG_AFL, 1, szTagValue, nTagValueLen, szAfl, pnAflLen ) )
                 {
                        nRetVal = RET_SUCCESS;
                        DbgPrintf("��ȡAFLʧ�ܣ�\n");  
                 }
                 else 
                 {
                       DbgPrintf("��ȡAFLʧ�ܣ�\n");  
                 }

             }
             else 
             {
                  DbgPrintf("��ȡAIP ʧ�ܣ�\n");  
             }
         }
         else 
         {
             DbgPrintf("GPO��Ӧ���ݸ�ʽ����\n");
         }
    }
    else 
    {
       DbgPrintf("��ȡ����ѡ�GPO������ִ��ʧ�� ԭ��Ϊ%s\n", ICC_CardCmdShowLastErrMsg() );
       nRetVal = RET_APP_INIT_ERR;
    }
    
    if( RET_SUCCESS == nRetVal )
    {
         DbgPrintf("Ӧ�ó�ʼ���ɹ���\n");
         DbgPrintHex("AIP", szAip, *pnAipLen );
         DbgPrintHex("AFL",  szAfl, *pnAflLen );
    }
    else 
    {
       DbgPrintf("Ӧ�ó�ʼ��ʧ�ܣ�\n");
    }
    
    return nRetVal;
}
S32 ICC_AppInit( S8* szBankName, U8* szFci, S32 nFciLen, U8* szAfl, S32* pnAflLen,  U8* szAip, S32* pnAipLen )
{
    struct TERMDATA stTermData;
    
    memset( &stTermData, 0x0, sizeof(stTermData) );
    strncpy( stTermData.BankName, szBankName, ( strlen( szBankName ) > 20 ? 20 : strlen( szBankName ) ) );
    
    return ICC_AppInitByTermData( &stTermData ,szFci, nFciLen, szAfl, pnAflLen, szAip, pnAipLen );
}
S32 ICC_ReadAppData( U8* szAfl, S32 nAflLen, U8 *szAppData, S32 *pnAppDataLen  )
{
     
    S32 i = 0;
    S32 j = 0;
    S32 nDataLen = 0;
    U8  szData[ MAX_LEN ] = { 0 };
    
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
            
            memcpy( szAppData+(*pnAppDataLen), szData, nDataLen );
            (*pnAppDataLen) += nDataLen;
            
        }
        
    }
    
    DbgPrintHex("��ȡ��Ӧ������Ϊ", szAppData, *pnAppDataLen );  
    
    return RET_SUCCESS;
}

S32 ICC_GetLogEntry( U8* szFci, S32 nFciLen, U8* pbSfi, U8* pbNum )
{
     S32 nRetVal = RET_LOG_NO_ENTRY;        
     U8 szEntry[ 32 ] = {0};
     S32 nEntryLen = 0;
    
     DbgPrintf("��ȡ��־���\n"); 
     if( RET_SUCCESS ==  centerm_get_tag_value(  (U8 *)"\x9F\x4D",  2,  szFci,  nFciLen, szEntry, &nEntryLen ) )
     {
        /*��־��ڵ�ֵӦΪ2���ֽ�*/
        if( 2 == nEntryLen )
        {
            *pbSfi = szEntry[0];
            *pbNum = szEntry[1];
            DbgPrintf("��־��SFI=0x%02X ��¼��Ϊ0x%02X\n", *pbSfi, *pbNum );
            nRetVal = RET_SUCCESS;
        }
     }
     else 
     {
         DbgPrintf("��ȡ��־���ʧ��\n");
         DbgPrintHex( "FCI����Ϊ", szFci, nFciLen ); 
     }
    
     return nRetVal;
}

/*!
 * \brief         ICC_GetTransferLogEntry     
 * \par ˵��      ��ȡȦ����־����� 	
 * \param[in]         	
 * \param[in]          
 * \param[in]	
 * \param[in]
 * \param[out]
 * \param[out]		
 * \retval              0 �ɹ� -1  ʧ��
 * \note                
 */
S32 ICC_GetCreditLoadLogEntry( U8* szFci, S32 nFciLen, U8* pbSfi, U8* pbNum )
{
     S32 nRetVal = RET_LOG_NO_ENTRY;        
     U8 szEntry[ 32 ] = {0};
     S32 nEntryLen = 0;
    
     DbgPrintf("��ȡ��־���\n"); 
     if( RET_SUCCESS ==  centerm_get_tag_value(  (U8 *)"\xDF\x4D",  2,  szFci,  nFciLen, szEntry, &nEntryLen ) )
     {
        /*��־��ڵ�ֵӦΪ2���ֽ�*/
        if( 2 == nEntryLen )
        {
            *pbSfi = szEntry[0];
            *pbNum = szEntry[1];
            DbgPrintf("��־��SFI=0x%02X ��¼��Ϊ0x%02X\n", *pbSfi, *pbNum );
            nRetVal = RET_SUCCESS;
        }
     }
     else 
     {
         DbgPrintf("��ȡ��־���ʧ��\n");
         DbgPrintHex( "FCI����Ϊ", szFci, nFciLen ); 
     }
    
     return nRetVal;
}


S32 ICC_GetLog( U8 SFI,  U8 Index,  U8 *szLog, S32 *pnLogLen )
{
   S32 nRetVal = RET_INVALID_PARAM;

   if(  ( Index >= 1 ) && ( Index <= 10 )  )
   {
      if( RET_SUCCESS == ICC_CardCmdReadSFI( SFI, Index, szLog, pnLogLen )  )
      {
          DbgPrintHex( "��ȡ��־����", szLog, *pnLogLen );
          nRetVal = RET_SUCCESS;
      }
      else 
      {
          
          DbgPrintf( "��ȡ SFI = 0x%02X, Index =0x%02X ��־����ԭ�� %s\n", SFI, Index, ICC_CardCmdShowLastErrMsg()  );
          
          nRetVal = RET_ERROR;
          if( 0x6A83 == ICC_CardCmdGetErrCode() )
          {
              nRetVal = RET_RECORD_NO_FOUND;  
          }
      }
  }
  
  return nRetVal;
}

S32 ICC_GetDOL( U8* szTagName, S32 nTagLen, U8* szData, S32 nDataLen, U8* szDol, S32* pnDolLen )
{
    S32 nRetVal = RET_DOL_NO_FOUND;        
    
    if( RET_SUCCESS == centerm_get_tag_value( szTagName, nTagLen,szData, nDataLen, szDol, pnDolLen ) )
    {
        nRetVal = RET_SUCCESS;
    }
    else 
    {
        *pnDolLen = 0;
    }
    
    return nRetVal;
}

S32 ICC_GetCDOL1( U8* szAppData, S32 nAppDataLen, U8* szCdol1, S32* pnCdol1Len )
{
    S32 nRetVal = RET_DOL_NO_FOUND;
    
    DbgPrintf("��ȡCDOL1�б�\n");   
      
    nRetVal = ICC_GetDOL( (U8 *)TAG_CDOL1,  strlen(TAG_CDOL1), szAppData, nAppDataLen, szCdol1, pnCdol1Len );            

    if( RET_SUCCESS ==  nRetVal )
    {
        DbgPrintHex( "CDOL1�б�", szCdol1, *pnCdol1Len );   
    }
	else 
	{
		DbgPrintf("��ȡCDOL1�б�ʧ��,��Ҫ��ֵ");
		AsciiToHex(IC_CDOL1_DATA, strlen(IC_CDOL1_DATA), szCdol1);
		*pnCdol1Len = strlen(IC_CDOL1_DATA) / 2;
		nRetVal = RET_SUCCESS;
	}
    
    return nRetVal;
}

S32 ICC_GetCDOL2( U8* szAppData, S32 nAppDataLen, U8* szCdol2, S32* pnCdol2Len )
{
    S32 nRetVal = RET_DOL_NO_FOUND;
    
    DbgPrintf("��ȡCDOL2�б�\n");   
      
    nRetVal = ICC_GetDOL( (U8 *)TAG_CDOL2,  strlen(TAG_CDOL2), szAppData, nAppDataLen, szCdol2, pnCdol2Len );            

    if( RET_SUCCESS ==  nRetVal )
    {
        DbgPrintHex( "CDOL2�б�", szCdol2, *pnCdol2Len );   
    }
    else 
    {
        DbgPrintf("��ȡCDOL2�б�ʧ��\n"); 
    }
    
    return nRetVal;
}



S32 ICC_GetCDOLTermData( S8* szDefaultName, S8* szTransData, U8 *szCdolData, S32 *pnCdolDataLen  )
{ 
    S8  szTermAppData[ MAX_LEN ] = {0};
    S32 nTermAppDataLen = 0;
    S8  szBuff[ MAX_LEN ] = {0};
    S32 nBuffLen = 0;
    S8  szResult[ MAX_LEN ] = {0};
    S32 nTmpLen = 0;
    S32 nRetVal = RET_SUCCESS;
   
    memset( szTermAppData, 0x00, sizeof(szTermAppData) );
    
    /*!<�ն˹��Ҵ���*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_COUNTRY_CODE , 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 0x02;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app( (U8*)"\x4A", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "���Ҵ���%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 2 );
    }
    else
    {    
        memcpy( szTermAppData+nTermAppDataLen, "\x01\x56", 2);
    }
    nTermAppDataLen += 2;

    /*!<�����*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_UNPREDICTABLE_NO, 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 4;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app( (U8*)"\x4B", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "�����%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 4 );
    }
    else
    {    
        GetRandom( 4, szTermAppData+nTermAppDataLen );
    }
    nTermAppDataLen += 4;    


    /*!<�ն���֤���*/
	memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_TVR, 1 );   
	nTermAppDataLen += 1;
	szTermAppData[ nTermAppDataLen++ ] = 0x05;
	memset( szBuff, 0x00, sizeof( szBuff ) );
	nRetVal = centerm_get_tag_value_app((U8*)"\x4C", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
	DbgPrintf( "�ն���֤���%s\n", szBuff );
	if( RET_SUCCESS == nRetVal )
	{    
		AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 5 );
	}
	else
	{    
		memcpy( szTermAppData+nTermAppDataLen, "\x00\x00\x00\x08\x00", 5 );
	}
	nTermAppDataLen += 5;

    /*!<�ն˽�������*/
    memcpy( szTermAppData+nTermAppDataLen, "\x9F\x66", 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 4;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app( (U8*)"\x4D", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
 	DbgPrintf( "�ն˽�������%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 4 );
    }
    else
    {    
        memcpy( szTermAppData+nTermAppDataLen, "\xF4\x00\x00\x00", 4 );
    }
    nTermAppDataLen += 4;   
    
    /*!<�����ֽ��ն�֧��ָʾ��*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_SUPPORT_INDICATOR, 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 0x01;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app( (U8*)"\x4E", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "�����ֽ��ն�֧��ָʾ��%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 1 );
    }
    else
    {    
        szTermAppData[ nTermAppDataLen ] = 0x00;
    }
    nTermAppDataLen += 1;   
    
    /*!<�����ֽ��ն˽����޶�*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_VLP_TRM_TRXN_LIMIT, 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 6;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app( (U8*)"\x4F", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "�����ֽ��ն˽����޶�%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 6 );
    }
    else
    {    
        memcpy( szTermAppData+nTermAppDataLen, "\x00\x00\x01\x00\x00\x00", 6 );
    }
    nTermAppDataLen += 6;        

    /*!<��Ȩ���*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_AMOUNT_AUTHORISED, 2 );
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 6;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8*)"\x50", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "��Ȩ��� %s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 6 );
    }
    else
    {    
        memcpy( szTermAppData+nTermAppDataLen, "\x00\x00\x00\x00\x00\x00", 6 );
    }
    nTermAppDataLen += 6;
    
    /*!<�������*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_AMOUNT_OTHER1, 2 );
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 6;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8*)"\x51", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "�������%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 6 );
    }
    else
    {    
        memcpy( szTermAppData+nTermAppDataLen, "\x00\x00\x00\x00\x00\x00", 6 );
    }
    nTermAppDataLen += 6;
   
    /*!<���׻��Ҵ���*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_TRXN_CURRENCY_CODE, 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 2;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8*)"\x52", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen  );
    DbgPrintf( "���׻��Ҵ���%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 2 );
    }
    else
    {
        memcpy( szTermAppData+nTermAppDataLen, "\x01\x56", 2 );
    }
    nTermAppDataLen += 2;  

    /*!<��������*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_TRXN_DATE, 1 );  
    nTermAppDataLen += 1;
    szTermAppData[ nTermAppDataLen++ ] = 3;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8 *)"\x53", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen   );
    DbgPrintf( "��������%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {
		if (nBuffLen == 8)	//YYYYMMDD
		{
			AsciiToBcd( szBuff+2, nBuffLen-2, szTermAppData+nTermAppDataLen, 3 );
		}
		else	//YYMMDD
		{
			AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 3 );
		}
    }
    else
    {
        memset( szBuff, 0, sizeof(szBuff) );
        GetTransDate( szBuff );
        AsciiToBcd( szBuff, 6, szTermAppData+nTermAppDataLen, 0x03 );
    }
    nTermAppDataLen += 3;    

    /*!<��������*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_TRXN_TYPE, 1 );   
    nTermAppDataLen += 1;
    szTermAppData[ nTermAppDataLen++ ] = 1;  
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8 *)"\x54", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen  ); 
    DbgPrintf( "��������%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {   
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 1 );
    }
    else
    {   
        memcpy( szTermAppData+nTermAppDataLen, "\x00", 1);
    }
    nTermAppDataLen += 1;  

    /*!<����ʱ��*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_TRXN_TIME, 2);   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 3;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8 *)"\x55", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "����ʱ��%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 3 );
    }
    else
    {
        memset( szBuff, 0, sizeof(szBuff) );
        GetTransTime( szBuff );
        AsciiToBcd( szBuff, 6, szTermAppData+nTermAppDataLen, 0x03 ); 
    }
    nTermAppDataLen += 3;

    /*!< �̻�����*/
    memcpy( szTermAppData+nTermAppDataLen, "\x9F\x4E", 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 20;
    memset( szBuff, 0x0, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8 *)"V", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf("�̻����ƣ� %s \n ", szBuff  );
    if( (RET_SUCCESS == nRetVal) && strlen(szBuff) )
    {
        memset( szTermAppData+nTermAppDataLen, 0x20, 20 );
        nTmpLen = 20 > strlen(szBuff) ? strlen(szBuff) : 20 ;
        memcpy( szTermAppData+nTermAppDataLen+(20-nTmpLen), szBuff, nTmpLen);
        
    }
    else
    {
        sprintf( szBuff, "%20s", szDefaultName );
        memcpy( szTermAppData+nTermAppDataLen, szBuff, 20 );
    }
    nTermAppDataLen += 20;   


	/*!< Check �ֿ�����֤���*/
	memcpy( szTermAppData+nTermAppDataLen, "\x9F\x34", 2 );   
	nTermAppDataLen += 2;
	szTermAppData[ nTermAppDataLen++ ] = 0x03;
	memcpy( szTermAppData+nTermAppDataLen, "\x3f\x00\x00", 3 );
	nTermAppDataLen += 5;

	/*!< Check ����״̬��Ϣ*/
	memcpy( szTermAppData+nTermAppDataLen, "\x9B", 1 );   
	nTermAppDataLen += 1;
	szTermAppData[ nTermAppDataLen++ ] = 0x02;
	memcpy( szTermAppData+nTermAppDataLen, "\x00\x00", 2 );
	nTermAppDataLen += 2;

    memcpy( szCdolData, szTermAppData, nTermAppDataLen );
    *pnCdolDataLen = nTermAppDataLen; 
    
    DbgPrintHex( "CDOL�ն�����", szTermAppData, nTermAppDataLen );
      
    return RET_SUCCESS;
}

S32 ICC_GetCDOLIccData( U8 *szCdol, S32 nCdolLen, S8* szDefaultName, S8* szTransData, U8* szAuthCode, U8* szIccData, S32 *pnIccDataLen   )
{
    U8 szCdolData[ MAX_LEN ] = {0};
    S32 nCdolDataLen = 0;
    
    ICC_GetCDOLTermData( szDefaultName, szTransData, szCdolData, &nCdolDataLen  );
    
    /*!<���������Ȩ��Ӧ�������Ȩ��Ӧ��ӵ��ն�������*/
    if( szAuthCode )
    {
        memcpy( szCdolData + nCdolDataLen, TAG_ARC, 1 );   
        nCdolDataLen += 1;
        szCdolData[ nCdolDataLen++ ] = 0x02;
        memcpy( szCdolData + nCdolDataLen, szAuthCode, 2 );
        nCdolDataLen += 2;
    }
    
    DbgPrintHex( "��ϵ�CDOL����", szCdolData, nCdolDataLen );
    
    return ICC_GetIccDataByDOL( szCdol, nCdolLen, szCdolData, nCdolDataLen, szIccData, pnIccDataLen );
}

S32 ICC_ExecuteGacCmd( U8 Cmd, U8* szIccData, S32 nIccDataLen, U8* szAc, S32* pnAcLen )
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
    if( RET_SUCCESS == nRetVal )
    {
        nRetVal = centerm_get_tag_value( (U8 *)TAG_RESPONSE_MSG_FORMAT1, 1, szOutData, nOutDataLen, szAc, pnAcLen );
        if( RET_SUCCESS == nRetVal )
        {
            //������Ϣ����(8)+Ӧ�ý��׼�������ATC��(1)+Ӧ�����ģ�AC��(2)+������Ӧ������(��ѡ)
            if( *pnAcLen >= 11 )
            {
                nRetVal = RET_SUCCESS;
            }
            else
            {
                DbgPrintf("GAC������Ӧ���ݳ��ȴ���δ�ﵽ11�ֽڣ���ȷ��ʽ��������Ϣ����(1)+Ӧ�ý��׼�������ATC��(2)+Ӧ�����ģ�AC��(8)+������Ӧ������(��ѡ)\n");
                nRetVal = RET_GAC_RESP_ERR;    
            }
        }
        else
        {
            DbgPrintf("GAC����������Ӧ��ȱ��0x80��ǩ\n");
            nRetVal = RET_GAC_RESP_ERR;    
        }
        /*!<���ϻ���Ҫ�����ʽ����Ŀǰֻ֧�ָ�ʽһ*/
            
    }
    
    return nRetVal;
}

S32 ICC_GetF55Domain( U8 *szAc, S32 nAcLen, U8 *szTermData, S32 nTermDatalen, S8 *szTransData, U8 *szF55Domain, S32 *pnF55Len )
{
    
    U8 szF55[ MAX_LEN ] = {0};
    U8 szBuf[ MAX_LEN ] = {0};
    S32 nBufLen = 0;
    S32 nF55Len = 0;
    S32 nRetVal = RET_SUCCESS;
    S32 nTagValueLen = 0;  
    U8 szTagValue[ MAX_LEN ] = {0};
    
    memset( szF55, 0, sizeof(szF55) );

	DbgPrintHex(" pack 55 Domain \n", szTermData, nTermDatalen );


    /*!<Ӧ������*/
    memcpy( szF55+nF55Len, "\x9F\x26", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 8;
    memcpy( szF55+nF55Len, szAc+3, 8 );
    nF55Len += 8;
    
    /*!<������Ϣ����*/
    memcpy( szF55+nF55Len, "\x9F\x27", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 1;
    memcpy( szF55+nF55Len, szAc, 1 );
    nF55Len += 1;
    
    /*!<������Ӧ������*/
    if( nAcLen > 11 )
    {
        memcpy( szF55+nF55Len, "\x9F\x10", 2 );  
        nF55Len += 2;
        szF55[ nF55Len++ ] = nAcLen - 11;
        memcpy( szF55+nF55Len, szAc+11, nAcLen-11);
        nF55Len += (nAcLen-11);
    }
    
    /*!<�����*/
    memcpy( szF55+nF55Len, "\x9F\x37", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 4;
    nBufLen = 0;
    memset( szBuf, 0, sizeof(szBuf) );
    nRetVal = centerm_get_tag_value( (U8 *)"\x9F\x37", 2, szTermData, nTermDatalen, (U8 *)szBuf, &nBufLen );
    if( RET_SUCCESS == nRetVal)
    {
        memcpy( szF55+nF55Len, szBuf, 4);
    }
    else
    {
        GetRandom( 4, szF55+nF55Len );
    }
    nF55Len += 4;
    
    /*!<Ӧ�ý��׼�����*/
    memcpy( szF55+nF55Len, "\x9F\x36", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 2;
    memcpy( szF55+nF55Len, szAc+1, 2 );
    nF55Len += 2;
    
    /*!<�ն���֤���*/
    memcpy( szF55+nF55Len, "\x95", 1 );  
    nF55Len += 1;
    szF55[ nF55Len++ ] = 5;
    memcpy( szF55+nF55Len, "\x00\x00\x00\x08\x00", 5 );
    nF55Len += 5;

#ifdef ARQC_MORE_TAG
    /*!<�ϴ�����Ӧ�ý��׼�������ATC���Ĵ���*/
    nBufLen = 0;
    memset( szBuf, 0, sizeof(szBuf) );
    nRetVal = ICC_CardCmdGetData( 0x9F, 0x13, szBuf,  &nBufLen );
    if( RET_SUCCESS ==  nRetVal )
    {
        nTagValueLen = 0;  
        memset( szTagValue, 0, sizeof(szTagValue) );
        nRetVal = centerm_get_tag_value( (U8 *)TAG_LAST_ONLINE_ATCR, 2, szBuf, nBufLen, szTagValue, &nTagValueLen );
        if( RET_SUCCESS ==  nRetVal )
        {
            memcpy( szF55+nF55Len, szBuf, nBufLen );
            nF55Len += nBufLen;
        }
    }
#endif
            
    /*!<��������*/
    nBufLen = 0;
    memset( szBuf, 0, sizeof(szBuf) );
    nRetVal = centerm_get_tag_value_app( (U8 *)"\x53", 1, (U8 *)szTransData, 0, (U8 *)szBuf, &nBufLen );
    memcpy(szF55+nF55Len, "\x9A", 1); 
    nF55Len += 1;
    szF55[nF55Len++] = 3;
    if( RET_SUCCESS ==  nRetVal )
    {    
		if( nBufLen == 8)
		{

			AsciiToBcd( szBuf+2, nBufLen-2, szF55+nF55Len, 3 );
		}
		else
		{
			AsciiToBcd( szBuf, nBufLen, szF55+nF55Len, 3 );

		}
        
    }
    else
    {    
        memcpy( szF55+nF55Len, "\x00\x00\x00", 3 );
    }
    nF55Len += 3;
    
    /*!<��������*/
    nBufLen = 0;
    memset( szBuf, 0, sizeof(szBuf) );
    nRetVal = centerm_get_tag_value_app( (U8 *)"\x54", 1, (U8 *)szTransData, 0, (U8 *)szBuf, &nBufLen );
    memcpy( szF55+nF55Len, "\x9C", 1 );  
    nF55Len+=1;
    szF55[nF55Len++]=1;
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuf, nBufLen, szF55+nF55Len, 1 );
    }
    else
    {    
        memcpy(szF55+nF55Len,"\x00",1);
    }
    nF55Len += 1;

    /*!<��Ȩ���*/
    nBufLen = 0;
    memset( szBuf, 0, sizeof(szBuf) );
    nRetVal = centerm_get_tag_value_app( (U8 *)"\x50", 1, (U8 *)szTransData, 0, (U8 *)szBuf, &nBufLen );
    memcpy( szF55+nF55Len, "\x9F\x02", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 6;
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuf, nBufLen, szF55+nF55Len, 6 );
    }
    else
    {    
        memcpy( szF55+nF55Len, "\x00\x00\x00\x00\x00\x00", 6 );      
    }
    nF55Len += 6;
    
    /*!<���׻��Ҵ���*/
    memcpy( szF55+nF55Len, "\x5F\x2A", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 2;
    memcpy( szF55+nF55Len, "\x01\x56", 2 );
    nF55Len += 2;

#ifdef ARQC_MORE_TAG
    /*!<Ӧ�����˺ţ�PAN��*/ 
    nBufLen = 0;
    memset( szBuf, 0, sizeof(szBuf) );
    nRetVal = centerm_get_tag_value( (U8 *)"\x5A", 1, (U8 *)szTermData, nTermDatalen, (U8 *)szBuf, &nBufLen );    
    memcpy( szF55+nF55Len, TAG_PAN, 1 );   
    nF55Len += 1;
    if( RET_SUCCESS == nRetVal )
    {    
        szF55[nF55Len++] = nBufLen;
		memcpy( szF55+nF55Len, szBuf, nBufLen );
        nF55Len += nBufLen;
    }
    else
    {   
        szF55[ nF55Len++ ] = 10;
        memcpy( szF55+nF55Len, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\0x00", 10 );
        nF55Len += 10;
    }

    /*!< Ӧ�����ʺ�(PAN)ϵ�к�*/
    nBufLen = 0;
    memset( szBuf, 0, sizeof(szBuf) );
    memcpy( szF55+nF55Len, TAG_PAN_SEQNO, 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 1;
    nRetVal = centerm_get_tag_value( (U8 *)"\x5F\x34", 2, (U8 *)szTermData, nTermDatalen, (U8 *)szBuf, &nBufLen );    
    if( RET_SUCCESS == nRetVal )
    {    
		memcpy( szF55+nF55Len, szBuf , nBufLen ); 
    }
    else
    {    
        memcpy( szF55+nF55Len, "\x00", 1 );
    }
    nF55Len += 1;
#endif

    /*!<Ӧ�ý�������*/
    memcpy( szF55+nF55Len, "\x82", 1 );  
    nF55Len += 1;
    szF55[ nF55Len++ ] = 2;
    nBufLen = 0;
    memset( szBuf, 0, sizeof(szBuf) );
    nRetVal = centerm_get_tag_value( (U8 *)"\x82", 1, szTermData, nTermDatalen, (U8 *)szBuf, &nBufLen );
    if( RET_SUCCESS == nRetVal)
    {
        memcpy( szF55+nF55Len, szBuf, 2 );
    }
    else
    {
        memcpy( szF55+nF55Len, "\x7C\x00", 2 );
    }
    nF55Len += 2;
 
    /*!<�ն˹��Ҵ���*/
    memcpy( szF55+nF55Len, "\x9F\x1A", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 2;
    memcpy( szF55+nF55Len, "\x01\x56", 2 );
    nF55Len += 2;
    
    /*!<�������*/
    nBufLen = 0;
    memset( szBuf, 0, sizeof(szBuf) );
    nRetVal = centerm_get_tag_value_app( (U8 *)"\x51", 1, (U8 *)szTransData, 0, (U8 *)szBuf, &nBufLen );
    memcpy(szF55+nF55Len,"\x9F\x03",2);
    nF55Len+=2;
    szF55[nF55Len++]=6;
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuf, nBufLen, szF55+nF55Len, 6 );
    }
    else
    {    
        memcpy(szF55+nF55Len,"\x00\x00\x00\x00\x00\x00",6);
    }
    nF55Len+=6;
    
    /*!<�ն�����*/
    memcpy( szF55+nF55Len, "\x9F\x33", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 3;
    memcpy( szF55+nF55Len, "\x20\xA1\x00", 3 );
    nF55Len += 3;
    
    *pnF55Len = nF55Len*2;
    HexToAscii( szF55, nF55Len, szF55Domain );
    DbgPrintf( "55��Ϊ %s\n", szF55Domain );
        
    return RET_SUCCESS;
}

/*!
 * \brief            ICC_BankAuthentication  
 * \par ˵��         ִ�з�������֤ ��ȡ��Ȩ��Ӧ��
 * \param[in]        szARPC      ��̨�·�����֤���� 	
 * \param[in]        nArpcLen    ��֤���ݵĳ��� 
 * \param[out]       szAuthCode  ��Ȩ��Ӧ��
 * \param[out]		
 * \retval              0 �ɹ� -1  ʧ��
 * \note             ��ǩΪ��91����ֵ��: ��������֤���� ARPC��8 �ֽڣ�+ ��Ȩ��Ӧ�루2 �ֽڣ�           
 */
S32 ICC_BankAuthentication( U8 *szARPC, S32 nArpcLen, U8* szAuthCode )
{
    S32 nRetVal = RET_ERROR;  
    S32 nBufLen = 0; 
    U8 szBuf[ MAX_LEN ] = {0};
 
    memset(szBuf, 0x00, sizeof(szBuf));
    nRetVal = centerm_get_tag_value( (U8 *)TAG_ISSUER_AUTH_DATA, 1, szARPC, nArpcLen, (U8 *)szBuf, &nBufLen );
    if( (RET_SUCCESS == nRetVal) && (nBufLen < 10) )
    {
        DbgPrintf( "��������֤���ݴ��󣬳���С��10\n"); 
        DbgPrintHex("��������", szBuf, nBufLen ); 
        return RET_AUTH_DATA_ERROR;
    }
    else if( RET_SUCCESS != nRetVal )
    {
       DbgPrintf( "�����ڱ�ǩΪ0x91�ķ�������֤����\n");
       return RET_AUTH_DATA_ERROR;       
    }
    
    /*!<�ⲿ��֤*/  
    DbgPrintHex( "�ⲿ��֤����", szBuf, 10 );
    nRetVal =  ICC_CardCmdExternalAuthenticate( szBuf, 10 );
    if( RET_SUCCESS == nRetVal )
    {
        memcpy( szAuthCode, szBuf+8, 2 );
        DbgPrintf( "��������֤�ɹ���\n" ); 
        DbgPrintHex( "����֤�����л�ȡ��Ȩ��Ӧ��", szAuthCode, 2 );      
    }
    else
    {
        DbgPrintf( "��������֤ʧ�ܣ�ԭ��%s\n", ICC_CardCmdShowLastErrMsg() );
    }
    
    return nRetVal; 
}

S32 ICC_ClosedTrade( U8* szAuthCode, U8* szCDOL2, S32 nCDOL2Len, U8 *TC, S32 *pnTCLen )
{
    S32 nRetVal = RET_ERROR;
    U8  Cmd = CMD_GAC_AAC;/*!<Ĭ�Ͼܾ�*/
    
    if( (!memcmp(szAuthCode, "00", 2)) 
        || (!memcmp(szAuthCode, "10", 2)) 
        || (!memcmp(szAuthCode, "11", 2)) )
    {
        /*!<ͬ�⽻��, ����TC*/
       Cmd = CMD_GAC_TC;
    }

    nRetVal = ICC_ExecuteGacCmd( Cmd, szCDOL2, nCDOL2Len, TC, pnTCLen );
    
    if( RET_SUCCESS == nRetVal )
    {
       nRetVal = RET_TRADE_ACC;
       if( TC[0]&0x40 )
       {
           nRetVal = RET_SUCCESS;
       }
    }
   
    return nRetVal;
}



S32 ICC_WriteScript( U8 *szArpc, S32 nArpcLen, char *szScriptResult )
{
   
    S32 nRetVal = RET_ERROR;
    S8  szScriptId[ MAX_LEN ] = {0};
    S32 nScriptIdLen = 0;
    U8  szScript[ MAX_LEN ] = {0};
    S32 nScriptLen = 0;
    S32 nScriptNum = 0;
    
    DbgPrintf("��ʼд�ű�\n");
    /*  ȡ�����нű����ݣ���Ӧ������Ӧ��ֻ������ǩΪ��72���Ľű���
        ���������нű������ڵ�2��GENERATE  AC����֮����С�
    */
    memset(szScript, 0x00, sizeof(szScript));
    nRetVal = centerm_get_tag_value( (U8*)TAG_ISSUER_SCRIPT2, 1,  szArpc, nArpcLen, szScript, &nScriptLen );
    
    if( RET_SUCCESS != nRetVal )
    {
        /*!<ȡ�����нű� 71*/
        nRetVal = centerm_get_tag_value( (U8*)TAG_ISSUER_SCRIPT1, 1, szArpc, nArpcLen, szScript, &nScriptLen );
        if( RET_SUCCESS != nRetVal )
        {
            strcpy( szScriptResult, "DF31050000000000" );
            DbgPrintf("ARPC��û�нű�\n");
            return RET_NO_SCRIPT;
        } 
    }
    
    DbgPrintHex("��ȡ�Ľű�����", szScript, nScriptLen );
    
    /*!<�ű�ִ�н��*/
    memcpy( szScriptResult, "DF3105", 6 );

    /*!<�����нű���ʶ*/
    memset( szScriptId, 0x00, sizeof(szScriptId) );
    nRetVal = centerm_get_tag_value( (U8*)TAG_ISSUER_SCRIPT_ID, 2, szScript, nScriptLen, szScriptId, &nScriptIdLen );
    if( RET_SUCCESS == nRetVal )
    {
         HexToAscii( szScriptId, 4, szScriptResult+8 );
    }
    else
    {
         memcpy( szScriptResult+8, "00000000", 8);
    }

    /*!<ִ�нű�����*/
    nRetVal =  ICC_WriteScriptCmd( szScript, nScriptLen, &nScriptNum );
    if( RET_SUCCESS == nRetVal )
    {
        /*!<�ű�ִ�гɹ�*/
        szScriptResult[6] = '2'; 
    }
    else
    {
        /*!<�ű�ִ��ʧ��*/
        szScriptResult[6] = '1';   
        nRetVal = RET_WRITE_SCRIPT_ERR;
    }

    /*!<�ű���*/
    nScriptNum = nScriptNum > 0x0F ? 0x0F : nScriptNum;
    szScriptResult[ 7 ] =  nScriptNum < 10 ? nScriptNum+0x30 : nScriptNum+0x37; 
    DbgPrintf( "�ű�ִ�н����%s\n", szScriptResult );
    
    return nRetVal;
}

S32 ICC_WriteScriptCmd( U8 *szScript, S32 nScriptLen, S32* pnIndex )
{
    S32 nRetVal = RET_ERROR;
    U8 * p = NULL;
    U8  C_Apdu[ MAX_LEN ] = {0};
    U8  ucCapduLen = 0;
    U8  R_Apdu[ MAX_LEN ] = {0};
    S32 nRapduLen = 0;
    
   DbgPrintf( "��ʼִ�нű�����\n");

   (*pnIndex) = 0;
    
    p = centerm_find_tag( (U8 *)TAG_ISSUER_SCRIPT_COMMAND, 1, szScript,  nScriptLen );  
    
    while( p )
    {
        
        memset( C_Apdu, 0x0, sizeof(C_Apdu) );
        ucCapduLen = 0;
        //(char *)&nCapduLen�����ַ�ʽ���п��ܻ���Ϊƽ̨��С�˲�ͬ��
        //���·������ݳ����쳣����0x0f,���������0x0f000000
        centerm_get_tag_data( 1, p, C_Apdu, &ucCapduLen );
        (*pnIndex) ++;
        DbgPrintHex( "�ű�����", C_Apdu, ucCapduLen );

        if( ucCapduLen > 2 )
        {
            nRetVal = ICC_CardCmdVisitCOS( C_Apdu, ucCapduLen, R_Apdu, &nRapduLen );
            if( RET_SUCCESS == nRetVal )
            {
                if( (  0x90 == R_Apdu[ nRapduLen - 2 ] )||(  0x62 == R_Apdu[ nRapduLen - 2 ] )||(  0x63 == R_Apdu[ nRapduLen - 2 ] )  )
                {
                    p += ( ucCapduLen + 2 );
                    p = centerm_find_tag( (U8 *)TAG_ISSUER_SCRIPT_COMMAND, 1, p, nScriptLen - ( p - szScript ) );  
                }
                else
                {
                    DbgPrintf( "�ű�����ִ�д��� ���Ϊ %d\n", (*pnIndex) );
                    DbgPrintf("����ԭ��%s\n", ICC_CardCmdShowLastErrMsg() );
                    return RET_SCRIPT_CMD_ERR;   
                }


            }
            else
            {
                DbgPrintf("д�ű�ʱ������ͨѶʧ�ܣ�\n");
                return RET_READ_ERROR;   
            }
        }

    }
    
    //�ű�ִ�гɹ������Ϊ0
    (*pnIndex) = 0;
    
    return RET_SUCCESS;
}

//R20010 add by sulin 20130502
//��߶����ٶ�
S32 ICC_TransReadAppDataByTag( U8 *szAFL, S32 nAFLlen, const U8 *szApptag, S32 nApptagLen, U8 *szAppData, S32 *pAppDataLen)
{

	S32  i=0, j=0, iDataLen =0;
	U8 data[ MAX_LEN ];
	U8 uszTag[128] = {0};
	S32 nTagLen = 0;
	S32 nTemplen = 0;
	memcpy(uszTag, szApptag, nApptagLen);
	nTagLen = nApptagLen;
	nTemplen = nTagLen;

	DbgPrintf("��ʼ��IC�����ݣ�\n");
	( *pAppDataLen ) = 0;    
	for( i = 0; i < ( nAFLlen/4); i++ )
	{
		if( (szAFL[4*i] < 1) || (szAFL[4*i] > 100) )
		{
			continue;
		}

		for( j = szAFL[4*i+1];  j <= szAFL[4*i+2];  j++ )
		{
			memset( data, 0x00, sizeof(data) );
			if( RET_SUCCESS !=ICC_CardCmdReadSFI(   szAFL[4*i]>>0x03, j,  data,  &iDataLen ) )
			{
				continue;
			}
			else
			{
				DbgPrintHex("ICC_CardCmdReadSFI:", data, iDataLen );
			}
			
			nTagLen = centerm_findapptag( data, iDataLen, uszTag, nTagLen );

			if(nTagLen != nTemplen)
			{
				nTemplen = nTagLen;
				memcpy(szAppData+(*pAppDataLen), data, iDataLen );
				(*pAppDataLen) += iDataLen;

				if( 0 == nTagLen )
				{
					DbgPrintf("CT_TransReadAppDataByTag over\r\n" ); 
					return RET_SUCCESS;
				}
			}
		}
	}
	
	return RET_SUCCESS;
}

//R20010 add by sulin 20130502
//��߶����ٶ�
S32 ICC_TransReadCDOL( U8 *szAFL, S32 nAFLlen, U8 *szAppData, S32 *pAppDataLen)
{

	S32  i=0, j=0, iDataLen =0;
	U8 data[ MAX_LEN ];
	U8 uszTag[128] = {0};
	S32 nTagLen = 0;
	S32 nTemplen = 0;

	memcpy(uszTag, "\x8C\x8D", 2);
	nTagLen = 2;
	nTemplen = nTagLen;

	DbgPrintf("��ʼ��IC�����ݣ�\n");
	( *pAppDataLen ) = 0;    
//	for( i = 0; i < ( nAFLlen/4); i++ )
	for( i = ( nAFLlen/4) -1 ; i >= 0; i-- )
	{
		if( (szAFL[4*i] < 1) || (szAFL[4*i] > 100) )
		{
			continue;
		}

		for( j = szAFL[4*i+2];  j >= szAFL[4*i+1];  j-- )
		{
			memset( data, 0x00, sizeof(data) );
			if( RET_SUCCESS !=ICC_CardCmdReadSFI(   szAFL[4*i]>>0x03, j,  data,  &iDataLen ) )
			{
				continue;
			}
			else
			{
				DbgPrintHex("ICC_CardCmdReadSFI:", data, iDataLen );
			}
			nTagLen = centerm_findapptag(data, iDataLen, uszTag, nTagLen);
			if(nTagLen != nTemplen)
			{
				nTemplen = nTagLen;
				memcpy(szAppData+(*pAppDataLen), data, iDataLen );
				(*pAppDataLen) += iDataLen;
				if(nTagLen == 0)
				{
					DbgPrintf("CT_TransReadAppDataByTag over\r\n" ); 
					return RET_SUCCESS;
				}
			}
			//			DbgPrintHex("ICC_CardCmdReadSFI����Ϊ:", data, iDataLen ); 
		}
	}
	DbgPrintf("IC�����ݳ��� =%d\n", *pAppDataLen );
	DbgPrintHex("IC������:", szAppData, *pAppDataLen );  
	return RET_SUCCESS;
}
