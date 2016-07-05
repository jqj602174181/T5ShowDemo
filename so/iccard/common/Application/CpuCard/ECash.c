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


//R20005 add by sulin 修改g_ucTagArr[]变量定义位置错误bug，转到ECash中定义
U8 g_ucTagArr[] =
{
	'\x04','g',    //Bitel Defined DataEOT 
	'\x17','b',    //Bitel Defined TrmScript 
	'\x18','b',    //Bitel Defined TrmCAPK  
	'\x19','b',    //Bitel Defined PDOL_data 
	'\x1A','b',    //Bitel Defined CDOL1data 
	'\x1B','b',    //Bitel Defined CDOL1data 
	'\x4F','v',    //应用标识
	'\x50','a',    //应用标签
	'\x51','b',    //K-CASH only? 
	'\x57','b',    //二道等效数据
	'\x5A','c',    //应用主帐号
	'\x5D','c',    //目录定义文件（DDF）名称
	'\x5F','\x20','a',   //持卡人姓名
	'\x5F','\x24','n',   //应用失效日期
	'\x5F','\x25','n',   //应用生效日期
	'\x5F','\x28','n',   //发卡行国家代码
	'\x5F','\x2A','n',   //交易货币代码
	'\x5F','\x2D','a',   //首选语言
	'\x5F','\x30','n',   //服务码
	'\x5F','\x34','n',   //应用主帐号(PAN)系列号
	'\x5F','\x36','n',   //交易货币指数
	'\x5F','\x50','a',   //发卡行URL
	'\x61','t',
	'\x6F','t',
	'\x70','t',    //'t'表示结构体
	'\x71','t',    //发卡行脚本1
	'\x72','t',    //发卡行脚本2
	'\x73','t',    //目录自定义模板
	'\x77','t',    //响应报文模板格式2
	'\x80','b',    //响应报文模板格式1 
	'\x81','b',    //交易授权金额
	'\x82','b',    //应用交互特征
	'\x84','v',    //专用文件（DF）名称
	'\x83','b',    //命令模板
	'\x86','b',    //发卡行脚本
	'\x87','b',    //应用优先级指示器
	'\x88','b',    //短文件表示SFI
	'\x89','a',    //授权码
	'\x8A','a',    //授权应答码
	'\x8C','b',    //卡片风险管理数据对象列表1(CDOL1)
	'\x8D','b',    //卡片风险管理数据对象列表2(CDOL2)
	'\x8E','b',    //持卡人验证方法(CVM)列表
	'\x8F','b',    //认证中心公钥索引
	'\x90','b',    //发卡行公钥证书
	'\x91','b',    //发卡行鉴定数据
	'\x92','b',    //发卡行公钥余项
	'\x93','b',    //签名静态应用数据
	'\x94','b',    //应用文件定位器
	'\x95','b',    //终端验证结果
	'\x97','b',    //交易证书数据对象列表(TDOL)
	'\x98','b',    //交易证书(TC)哈希值
	'\x99','b',    //交易个人密码(PIN)数据
	'\x9A','n',    //交易日期
	'\x9B','b',    //交易状态信息
	'\x9C','n',    //交易类型
	'\x9D','b',    //目录数据文件(DDF)名
	'\x9F','\x01','n',   //收单行标识
	'\x9F','\x02','n',   //授权金额(数字)
	'\x9F','\x03','n',   //其它金额(数字)
	'\x9F','\x04','b',   //其它金额(二进制)
	'\x9F','\x05','b',   //应用自定义数据
	'\x9F','\x06','b',   //应用标识
	'\x9F','\x07','b',   //应用用途控制
	'\x9F','\x08','b',   //应用版本号。
	'\x9F','\x09','b',   //应用版本号
	'\x9F','\x0B','a',   //持卡人姓名扩展
	'\x9F','\x0D','b',   //发卡行行为代码-缺省
	'\x9F','\x0E','b',   //发卡行行为代码-拒绝
	'\x9F','\x0F','b',   //发卡行行为代码-联机
	'\x9F','\x10','b',   //发卡行应用数据
	'\x9F','\x11','n',   //发卡行代码表索引
	'\x9F','\x12','a',   //应用首选名
	'\x9F','\x13','b',   //最近联机交易序号(ATC)寄存器
	'\x9F','\x14','b',   //连续脱机交易下限
	'\x9F','\x15','n',   //商户类别代码
	'\x9F','\x16','a',   //商户标识
	'\x9F','\x17','b',   //个人密码(PIN)重试次数
	'\x9F','\x18','b',   //发卡行脚本标识
	'\x9F','\x1A','n',   //终端国家代码
	'\x9F','\x1B','b',   //终端最低限额
	'\x9F','\x1C','a',   //终端标识
	'\x9F','\x1D','b',   //终端风险管理数据
	'\x9F','\x1E','a',   //接口设备(IFD)系列号
	'\x9F','\x1F','a',   //一磁道等效数据
	'\x9F','\x20','c',   //二磁道自定义数据
	'\x9F','\x21','n',   //交易时间
	'\x9F','\x22','b',   //认证中心公钥索引
	'\x9F','\x23','b',   //连续脱机交易上限
	'\x9F','\x24','b',   //应用失效日期
	'\x9F','\x25','n',   //应用生效日期
	'\x9F','\x26','b',   //应用密文
	'\x9F','\x27','b',   //密文信息数据
	'\x9F','\x2D','b',   //IC卡PIN加密公钥证书
	'\x9F','\x2E','b',   //IC卡PIN加密公钥指数
	'\x9F','\x2F','b',   //IC卡PIN加密公钥余项
	'\x9F','\x32','b',   //发卡行公钥指数
	'\x9F','\x33','b',   //终端性能
	'\x9F','\x34','b',   //持卡人验证方法(CVM)结果
	'\x9F','\x35','n',   //终端类型
	'\x9F','\x36','b',   //应用交易系列号
	'\x9F','\x37','b',   //随机数
	'\x9F','\x38','b',   //处理选项数据对象列表(PDOL)
	'\x9F','\x39','n',   //POS输入方式
	'\x9F','\x3A','b',   //参考货币金额
	'\x9F','\x3B','n',   //应用参考货币
	'\x9F','\x3C','n',   //交易参考货币
	'\x9F','\x3D','n',   //交易参考货币指数
	'\x9F','\x40','b',   //附加终端性能
	'\x9F','\x41','n',   //交易序号计数器
	'\x9F','\x42','n',   //应用货币代码
	'\x9F','\x43','n',   //应用参考货币指数
	'\x9F','\x44','n',   //应用货币指数
	'\x9F','\x45','b',   //数据认证码
	'\x9F','\x46','b',   //IC卡公钥证书
	'\x9F','\x47','b',   //IC卡公钥指数
	'\x9F','\x48','b',   //IC卡公钥余项
	'\x9F','\x49','b',   //动态数据对象列表(DDOL)
	'\x9F','\x4A','b',   //静态数据鉴定标签列表
	'\x9F','\x4B','b',   //签名动态应用数据
	'\x9F','\x4C','b',   //DDA处理过程中，卡片生成的随时间变化不同的随机数
	'\x9F','\x4D','b',   //日志入口
	'\x9F','\x4E','b',   //商户名称
	'\x9F','\x4F','b',   //日志格式
	'\x9F','\x51','n',   //使用的货币代码
	'\x9F','\x52','b',   //VISA only    
	'\x9F','\x53','a',   //连续脱机交易限制数
	'\x9F','\x53','b',   //VISA only 
	'\x9F','\x54','n',   //累计脱机交易金额限制数
	'\x9F','\x55','b',   //VISA only 
	'\x9F','\x56','b',   //发卡行认证指示位
	'\x9F','\x57','n',   //发卡行国家代码
	'\x9F','\x58','b',   //连续脱机交易下限
	'\x9F','\x59','b',   //连续脱机交易上限
	'\x9F','\x5A','a',   //发卡行URL2
	'\x9F','\x5B','b',
	'\x9F','\x5C','n',   //累计脱机交易金额上限
	'\x9F','\x5D','n',   //可用脱机消费金额
	'\x9F','\x61','a',   //持卡人证件号
	'\x9F','\x62','c',
	'\x9F','\x66','b',   //终端交易属性
	'\x9F','\x68','b',   //表13　卡片附加处理
	'\x9F','\x6B','n',   //卡片持卡人验证方法限制
	'\x9F','\x6C','b',   //卡片交易属性
	'\x9F','\x6D','n',   //电子现金重置阈值
	'\x9F','\x72','b',   //连续脱机交易限制数
	'\x9F','\x73','n',   //货币转换因子
	'\x9F','\x74','a',   //VISA only 
	'\x9F','\x75','n',   //累计脱机交易金额限制数
	'\x9F','\x76','n',   //第2应用货币
	'\x9F','\x77','n',   //电子现金余额上限
	'\x9F','\x78','n',   //电子现金单笔交易限额
	'\x9F','\x79','n',   //电子现金余额
	'\x9F','\x7A','n',   //电子现金终端支持指示器
	'\x9F','\x7B','n',   //电子现金终端交易限额
	'\x9F','\x7F','b',   //VISA only 
	'\xA5','t',          //FCI专有模板
	'\x0BF','\x0C','t', //FCI发卡行自定义数据
	'\xC1','v',          //MONETA only? 
	'\xC2','v',          //MONETA only?  
	'\xDF','\x60','b',    //CAPP交易指示位      
	'\xDF','\x69','b'    //国密支持指示位  
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

    
    /*!<按文件名选DDF*/
    memset( ucData, 0x0, sizeof(ucData) );  
    DbgPrintf("选择文件%s\n", pszDDFname );
    nRetVal = ICC_CardCmdSelectFile( 0x04, 0x00, pszDDFname, nDDFnameLen, ucData,  &nDataLen );
    DbgPrintHex("文件选择结果", ucData, nDataLen );
    if( RET_SUCCESS !=  nRetVal )
    {
         return nRetVal;    
    }
    
    /*!<查找标签0x88的短文件标识符（SFI）*/
    p = (U8 *)centerm_find_tag( (U8 *)TAG_SFI, strlen(TAG_SFI), ucData, nDataLen );    
    if( NULL == p )
    {
        DbgPrintf("未查找到标签0x88的短文件标识符（SFI）\n" );
        return RET_NO_FOUND_SFI;
    }
    
    /*!<判断sfi的长度是否有问题*/
    p++;
    if( (*p) != 0x01 )
    {
        *ppAidList=NULL;
        DbgPrintf("短文件标识符（SFI）长度错误\n" );
        return RET_SFI_LEN_ERR;
    }

    // 读取所有记录
    p ++;
    SFI = (*p); //记录的短文件名
 
   /*!<链表头节点*/
    pRecordList = (RECORD_LIST *)malloc( sizeof(RECORD_LIST) );
    if( NULL == pRecordList )
    {
            return RET_MALLOC_ERR;    
    }
    pRecordListHead = pRecordList;/*!<保存表头地址*/
    memset( pRecordList, 0x0, sizeof( RECORD_LIST ) ); 
    
    while(1)
    {
        /*!<读取记录*/
        Index ++;/*!<记录号*/
        nDataLen = 0;
        memset( ucData, 0x0, sizeof(ucData) );  
        nRetVal = ICC_CardCmdReadSFI(  SFI,  Index,  ucData, &nDataLen );
        if( RET_SUCCESS != nRetVal )
        {
             break;
        }
        DbgPrintf("获取应用列表读取 SFI=%d Index=%d \n", SFI, Index );
        DbgPrintHex("结果", ucData, nDataLen );
        
        /*!<链表保存记录的节点*/
        pRecordList->pnext = (RECORD_LIST *)malloc( sizeof(RECORD_LIST) );
        memset( pRecordList->pnext, 0x0, sizeof(RECORD_LIST));
        
       /*!<保存记录数据*/
        pRecordList->precord = (U8 *)malloc( nDataLen );
        memset( pRecordList->precord, 0x0, nDataLen);       
        memcpy( pRecordList->precord,ucData, nDataLen );
        pRecordList->nRecordLen = nDataLen;
        
        /*!<指向下一个节点*/
        pRecordList = pRecordList->pnext;        


        if( 255 == Index )
         {
               break;
         }
         
    }   

    //  分析每个记录   
    pRecordList->pnext=NULL;
    pRecordList->precord=NULL;    
    pRecordList = pRecordListHead;

    while(  pRecordList->pnext != NULL )   
    {                            
        //  读取记录的所有目录入口数据
        p = (U8 *)centerm_find_tag( (U8 *)TAG_APP,  strlen(TAG_APP),  pRecordList->precord,  pRecordList->nRecordLen );
        while( p )
        {
            /*!<判断tag长度*/
            p++;                        
            if(   0  == (*p)  ) 
            {
                    p++;
                    goto nextloop;
            }
            
            nAdfEntryLen = (*p);  //  指向标签61长度
            p++;

            if( (*p) == 0x4F )/*!<ADF 名称（AID*/
            {
                p++;
                if( ((*p) < 5) || ((*p) >16) ) /*!< 长度错误放弃*/
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
                
                /*!<ADF 名称（AID*/
                pAidList->AidLen = (*p);  /*!<aid 长度*/
                p++;   //  p指向tag=0x4F的值
                memcpy( pAidList->AID, p, pAidList->AidLen );/*!<aid*/       
                DbgPrintHex("应用标识AID", pAidList->AID, pAidList->AidLen  );
                                
                nAidNum ++;   
                if( 1 == nAidNum  )
                {
                     pAidListHead = pAidList;
                }


                /*!<应用标签*/
                p-=2;   //  返回指向tag=0x61的目录入口地址
                pTmp = (U8 *)centerm_find_tag( (U8*)TAG_APP_LABEL, 1, p,  nAdfEntryLen );
                if( !pTmp )
                {
                    DbgPrintf("应用标签(0x50)未找到");
                    pAidList->AppTagLen=0;
                    goto nextloop;
                }
                
                pTmp += 1;
                if(  ( (*pTmp) < 1 ) ||( (*pTmp) > 16 ))/*!< 长度错误放弃*/
                {
                    pAidList->AppTagLen=0;
                    goto nextloop;
                }
                
                pAidList->AppTagLen = (*pTmp);
                pTmp ++;
                memcpy(pAidList->AppTag, pTmp, pAidList->AppTagLen);
                DbgPrintHex("应用标签", pAidList->AppTag, pAidList->AppTagLen  );

                /*!<应用优先权标识符*/
                pTmp = (U8 *)centerm_find_tag( (U8*)TAG_APP_PRIORITY_INDICATOR, strlen(TAG_APP_PRIORITY_INDICATOR ),  p, nAdfEntryLen );
                if( pTmp  )
                {
                    pTmp ++;
                    if( *pTmp )
                    {
                        pTmp ++;
                        pAidList->APPpriority = (*pTmp);
                        DbgPrintHex("应用优先权标识符",  (U8*)&pAidList->APPpriority, 1  );
                    }
                }
                 
                /*!<应用优先名称*/
                pTmp = (U8 *)centerm_find_tag( (U8*)TAG_APP_PREFERRED_NAME,  strlen(TAG_APP_PREFERRED_NAME), p,  nAdfEntryLen );
                if( pTmp )
                {
                    pTmp += 2;
                    if(  ( (*pTmp) >= 1 ) && ( (*pTmp) <= 16 ))/*!< 长度错误放弃*/
                    {
                        pAidList->APNlen = (*pTmp);
                        pTmp ++;
                        memcpy(pAidList->APN, pTmp, pAidList->APNlen);
                        DbgPrintHex("应用优先名称", pAidList->APN, pAidList->APNlen  );
                    }
                }

            }
            else if(  0x9D == (*p) )  //  DDF  目录定义文件入口,目录数据文件（DDF）
            {
                p++;
                if( ((*p) < 5) || ((*p) >16) )  //  len=0,放弃
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

        //  指向下一条记录
        pRecordList = pRecordList->pnext;
        
    }

    *ppAidList = pAidListHead;//把Aid列表赋值给ppAidList   
    *pnAidNum = nAidNum;
    nRetVal = RET_SUCCESS;
    
    //释放链表的内存pRecordList
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
    
    DbgPrintf("应用选择开始...\n ");
    DbgPrintHex("选择的应用标识(aid)",  AID, nAidLen );
    
    if( RET_SUCCESS == nRetVal )
    {
        DbgPrintf("应用选择成功！\n ");
        DbgPrintHex("返回的文件控制信息(fci)",  szFci, *pnFciLen );
    }
    else 
    {
        DbgPrintf("应用选择失败！\n ");
    }
    
    return nRetVal;
}

S32 ICC_GetPDOL(   U8  *szFci,  S32 nFciLen,  U8*szPdol,  S32 *pnPdolLen )
{
    S32 nRetVal = RET_DOL_NO_FOUND;
    
    DbgPrintf("获取PDOL列表\n");   
      
    nRetVal = ICC_GetDOL( (U8 *)TAG_PDOL,  strlen(TAG_PDOL), szFci, nFciLen, szPdol, pnPdolLen );            

    if( RET_SUCCESS ==  nRetVal )
    {
        DbgPrintHex( "PDOL列表", szPdol, *pnPdolLen );   
    }
    else 
    {
        DbgPrintf("获取PDOL列表失败\n"); 
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
    
    /*!<计算tag*/
    szTag[ nTagLen++ ] = szDol[nPos];
    /*!<  低五位为全1，表示tag是多字节标签，否则为单字节标签*/
    if( 0x1F == ( szDol[nPos++]&0x1F ) )    
    {    
        //  b8=1,后面还有一字节，b8=0，最后一个字节
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
    
    /*!<tag长度 3个字节*/
    if(  0x82 == szDol[ nPos ]  )
    {
        if( (nPos+3) > nDolLen )
        {
            return 0;
        }
            
        (*pnTagLen) = ( szDol[ nPos+1 ]*256 + szDol[ nPos+2 ] )&0xFFFF;
        nPos += 3;
    }
     /*!<tag长度2个字节*/
    else if( 0x81 == szDol[ nPos ] )
    {
        if( (nPos+2) > nDolLen  )
        {
             return 0;
        }
             
        (*pnTagLen) = szDol[ nPos+1 ];
        nPos += 2;
    }
      /*!<tag长度1个字节*/
    else
    {
        
        if( (nPos+1) > nDolLen  )
        {
            return 0;
        }

        //  0-127,一字节长度  TLV 长度后面是标签
        (*pnTagLen) = szDol[ nPos ];           
        nPos += 1;
        
    }
    
    DbgPrintf( "从PDOL获取tag值的长度为 %d\n",  *pnTagLen );
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
    
    //  授权金额
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x02",  2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 0x06;
    memcpy( ucPdolData+nPdolDataLen, "\x00\x00\x00\x00\x00\x00", 6 );
    nPdolDataLen += 6;
 
    //  其它金额
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x03", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 0x06;
    memcpy( ucPdolData+nPdolDataLen, "\x00\x00\x00\x00\x00\x00",  6 );
    nPdolDataLen += 6;
    
    //  终端国家代码
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x1A", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 0x02;   
    memcpy( ucPdolData+nPdolDataLen,  "\x01\x56",  2 );
    nPdolDataLen += 2;
    
    //  终端验证结果
    memcpy( ucPdolData+nPdolDataLen, "\x95", 1 );   
    nPdolDataLen += 1;
    ucPdolData[ nPdolDataLen++ ] = 0x05;
    memcpy( ucPdolData+nPdolDataLen, "\x00\x00\x00\x08\x00", 5 );
    nPdolDataLen += 5;
    
    //  交易授权的本地日期 
    memcpy( ucPdolData+nPdolDataLen, "\x9A", 1 );   
    nPdolDataLen += 1;
    ucPdolData[ nPdolDataLen++] = 0x03;
    memset( szDate, 0, sizeof(szDate) );
    GetTransDate( szDate );
    AsciiToBcd( szDate, 6, ucPdolData+nPdolDataLen , 0x03 );
    nPdolDataLen += 3;
    
    //交易授权的本地时间
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x21", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++] = 0x03;
    memset( szTime, 0, sizeof(szTime) );
    GetTransTime( szTime );
    AsciiToBcd( szTime, 6, ucPdolData+nPdolDataLen , 0x03 );
    nPdolDataLen += 3;
    
    //  交易货币代码
    memcpy( ucPdolData+nPdolDataLen, "\x5F\x2A", 2 );  
    nPdolDataLen+=2;
    ucPdolData[ nPdolDataLen++ ] = 2;
    memcpy( ucPdolData+nPdolDataLen, "\x01\x56",  2 );
    nPdolDataLen+=2;
    
    //  交易类型
    memcpy( ucPdolData+nPdolDataLen,"\x9C", 1 );   
    nPdolDataLen += 1;
    ucPdolData[ nPdolDataLen++ ] = 1;
    memcpy( ucPdolData+nPdolDataLen, "\x01", 1 );
    nPdolDataLen += 1;

    //  随机数
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x37", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 4;
    GetRandom( 4, ucPdolData+nPdolDataLen );
    nPdolDataLen += 4;
    
    //  表1　终端交易属性
    memcpy( ucPdolData+nPdolDataLen,"\x9F\x66", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++] = 4;
    memcpy( ucPdolData+nPdolDataLen, "\xF4\x00\x00\x00",4  );
    nPdolDataLen += 4;
    
    //  电子现金终端支持指示器
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x7A", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 1;
    ucPdolData[ nPdolDataLen++ ] = pTermData->ECardIndex;
    
    //  电子现金终端交易限额
    memcpy( ucPdolData+nPdolDataLen,"\x9F\x7B", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 6;
    memcpy( ucPdolData+nPdolDataLen,"\x00\x00\x01\x00\x00\x00", 6 );
    nPdolDataLen += 6;
    
    //  商户名称
    memcpy( ucPdolData+nPdolDataLen, "\x9F\x4E", 2 );   
    nPdolDataLen += 2;
    ucPdolData[ nPdolDataLen++ ] = 20;
    memset( ucPdolData+nPdolDataLen, 0x0, 20 );
    memcpy( ucPdolData+nPdolDataLen, pTermData->BankName, strlen(pTermData->BankName) );
    nPdolDataLen += 20;

	//R20009 add by sulin  pboc3.0 要求支持双向算法,详见PBOC3.0第14部分
	memcpy( ucPdolData+nPdolDataLen,"\xDF\x69",2);   
	nPdolDataLen+=2;
	ucPdolData[nPdolDataLen++] = 1;
    ucPdolData[nPdolDataLen++] = pTermData->EncryptIndex;

	//R20009 add by sulin  pboc3.0 交易指示位,详见PBOC3.0第14部分
	memcpy( ucPdolData+nPdolDataLen,"\xDF\x60",2);   
	nPdolDataLen+=2;
	ucPdolData[nPdolDataLen++]=1;
	memcpy(ucPdolData+nPdolDataLen,"\x00",1);
	nPdolDataLen+=1;

	//R20009 add by sulin  pboc3.0 表示卡片既支持分段扣费功能也支持脱机预授权功能,详见PBOC3.0第14部分
	memcpy( ucPdolData+nPdolDataLen,"\xDF\x61",2);   
	nPdolDataLen+=2;
	ucPdolData[nPdolDataLen++]=1;
	memcpy(ucPdolData+nPdolDataLen,"\x02",1);
	nPdolDataLen+=1;
    
	//终端性能
	memcpy( ucPdolData+nPdolDataLen,"\x9F\x33",2);   
	nPdolDataLen+=2;
	ucPdolData[nPdolDataLen++]=3;
	memcpy(ucPdolData+nPdolDataLen,"\xE0\xE1\xC8",3);
	nPdolDataLen+=3;

    //返回数据
    memcpy( szPdol, ucPdolData, nPdolDataLen );
    *pnPdolLen = nPdolDataLen;
    DbgPrintHex("用于PDOL的终端数据", ucPdolData, nPdolDataLen );
    
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
        //从数据对象pdol列表寻找标签名称
        nTagNameLen = 0;
        nTagLen = 0;
        memset( szTagName, 0x0, sizeof(szTagName) );
        
        nRetVal = ICC_GetTagInDOL( pTmpDol,  nTmpDolLen, szTagName, &nTagNameLen, &nTagLen );
        if( nRetVal < 0  )
        {
            *pnIccDataLen = 0;
            DbgPrintHex("DOL异常", szDol,  nDolLen );
            return RET_DOL_DATA_ERR;
        }

        //下一个循环设置
        nTmpDolLen -= nRetVal;
        pTmpDol       += nRetVal;

        //从终端应用数据中寻找以标签为起始的应用数据
        memset( szIccData + (*pnIccDataLen),  0,  nTagLen );
        p = centerm_find_tag( szTagName, nTagNameLen, szDolData, nDolDataLen );
        if( !p )
        {
          *pnIccDataLen = 0;
           DbgPrintHex( "终端数据(szDolData)不包含指定标签的值，该标签为", szTagName, nTagNameLen );
           DbgPrintHex(  "终端数据(szDolData)", szDolData, nDolDataLen);
            return RET_DOL_DATA_NO_FOUND;
        }

        //计算终端应用数据里标签的实际长度（datalen）
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

         //根据标签名找标签的类型,在标签数组中找
         nTagType = centerm_get_tag_type(  szTagName, nTagNameLen, (U8 *)g_ucTagArr, sizeof(g_ucTagArr) );
         if( !nTagType ) 
         {
                (*pnIccDataLen) += nTagLen;
                continue;
         }
            
        //根据标签类型对应的数据规则复制实际的应用数据单元
        if( nTagType == 't' )       
        {
            //如果标签是结构标签的话，全部填零 
            (*pnIccDataLen) += nTagLen;
        }
        else if( nTagLen == nDataLen)       
        {
            //如果标签指定长度和实际长度相等，则拷贝数据
            memcpy( szIccData + (*pnIccDataLen), p, nTagLen );
            (*pnIccDataLen) += nTagLen;
        }
        else if( nTagLen < nDataLen)       
        {
            //如果标签指定长度小于实际长度，则削减数据
            if(nTagType == 'n')     
            {
                //如果是格式'n'，则从左边开始削减
                p += nDataLen - nTagLen;
                memcpy( szIccData + (*pnIccDataLen), p, nTagLen );
            }
            else                 
            {
                //其它格式，则从右边开始削减
                memcpy( szIccData + (*pnIccDataLen), p, nTagLen );
                (*pnIccDataLen) += nTagLen;

            }
        }   
        else if( nTagLen > nDataLen)       
        {
            //如果标签指定长度大于实际长度，则填充数据
            if( nTagType == 'n' )     
            {
                //如果是格式'n'，则从左边开始填充
                (*pnIccDataLen) += nTagLen - nDataLen;
                memcpy( szIccData + (*pnIccDataLen), p, nDataLen);
                (*pnIccDataLen) += nDataLen;
            }
            else
            {
                //如果是cn格式的话，在末尾补上FF.
                if( nTagType == 'c' )     
                {
                    memset(szIccData + (*pnIccDataLen), 0xFF, nTagLen);             
                }
                else    
                {
                    //如果是其它格式，在后面补零
                    memset( szIccData + (*pnIccDataLen),  0x0, nTagLen );
                }
                memcpy( szIccData + (*pnIccDataLen), p, nDataLen);
                (*pnIccDataLen) += nTagLen;
            }       
        }
    }
    
    DbgPrintHex("IC卡要求的DOL数据为", szIccData, *pnIccDataLen );
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
   
   DbgPrintf("开始应用初始化.......\n");
   
    /*!<从文件控制信息中获取PDOL列表*/
    DbgPrintf("从文件控制信息中获取PDOL列表\n");
    
    if( RET_SUCCESS == ICC_GetPDOL(   szFci,  nFciLen,  szPdol,  &nPdolLen ) )
    {
         DbgPrintf("组织IC卡要求的终端数据\n");
         ICC_GetPdolTermData(  pTermData, szPdolData, &nPdolDataLen );
         ICC_GetIccDataByDOL( szPdol, nPdolLen, szPdolData,  nPdolDataLen,  szIccData,  &nIccDataLen  );
    }
    else 
    {
        /*!<卡片没有返回PDOL列表的情况*/
        DbgPrintf("卡片没有返回PDOL列表的情况\n");
    }
    
    DbgPrintf("发送获取处理选项（GPO）命令通知卡片交易开始\n");
    if( RET_SUCCESS == ICC_CardCmdGetProcessOptions( szIccData,   nIccDataLen, szOutData,  &nOutDataLen ) )
    {
         DbgPrintHex("GPO响应", szOutData, nOutDataLen );   
          /*格式1 ：标签为 ‘80’ 的基本数据对象。数据域为应用交互特征（AIP）和应用文件定位器（AFL）的值域连接而成，各数据对象之间没有分隔符（标签和长度）。*/
         if( RET_SUCCESS ==  centerm_get_tag_value( "\x80", 1, szOutData, nOutDataLen, szTagValue, &nTagValueLen) )
         {
              memcpy( szAip, szTagValue, 2 );
              *pnAipLen = 2;  
              memcpy( szAfl, szTagValue+2, nTagValueLen - 2 );
              *pnAflLen = nTagValueLen - 2; 
              nRetVal = RET_SUCCESS;               
         }
         /*格式2：
            响应报文中的数据对象是一个标签为‘77’的基本数据对象。数据域可以包含多个BER-TLV编码的
            对象，但至少要包含应用交互特征（AIP）和应用文件定位器（AFL）。
         */
         else if(  RET_SUCCESS ==  centerm_get_tag_value( "\x77", 1, szOutData, nOutDataLen, szTagValue, &nTagValueLen )  )
         {
             /*获取AIP*/
             if( RET_SUCCESS ==  centerm_get_tag_value( TAG_AIP, 1, szTagValue, nTagValueLen, szAip, pnAipLen ) )
             {
                  DbgPrintf("获取AIP 成功！\n");
                  /*获取AFL*/
                 if( RET_SUCCESS ==  centerm_get_tag_value( TAG_AFL, 1, szTagValue, nTagValueLen, szAfl, pnAflLen ) )
                 {
                        nRetVal = RET_SUCCESS;
                        DbgPrintf("获取AFL失败！\n");  
                 }
                 else 
                 {
                       DbgPrintf("获取AFL失败！\n");  
                 }

             }
             else 
             {
                  DbgPrintf("获取AIP 失败！\n");  
             }
         }
         else 
         {
             DbgPrintf("GPO响应数据格式错误！\n");
         }
    }
    else 
    {
       DbgPrintf("获取处理选项（GPO）命令执行失败 原因为%s\n", ICC_CardCmdShowLastErrMsg() );
       nRetVal = RET_APP_INIT_ERR;
    }
    
    if( RET_SUCCESS == nRetVal )
    {
         DbgPrintf("应用初始化成功！\n");
         DbgPrintHex("AIP", szAip, *pnAipLen );
         DbgPrintHex("AFL",  szAfl, *pnAflLen );
    }
    else 
    {
       DbgPrintf("应用初始化失败！\n");
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
    
    DbgPrintf( "读应用数据......\n");
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
                DbgPrintf("读应用数据错误 %s\n", ICC_CardCmdShowLastErrMsg() );
                continue;
            }
            else 
            {
                 DbgPrintHex("应用数据", szData,  nDataLen );
            }
            
            memcpy( szAppData+(*pnAppDataLen), szData, nDataLen );
            (*pnAppDataLen) += nDataLen;
            
        }
        
    }
    
    DbgPrintHex("读取的应用数据为", szAppData, *pnAppDataLen );  
    
    return RET_SUCCESS;
}

S32 ICC_GetLogEntry( U8* szFci, S32 nFciLen, U8* pbSfi, U8* pbNum )
{
     S32 nRetVal = RET_LOG_NO_ENTRY;        
     U8 szEntry[ 32 ] = {0};
     S32 nEntryLen = 0;
    
     DbgPrintf("获取日志入口\n"); 
     if( RET_SUCCESS ==  centerm_get_tag_value(  (U8 *)"\x9F\x4D",  2,  szFci,  nFciLen, szEntry, &nEntryLen ) )
     {
        /*日志入口的值应为2个字节*/
        if( 2 == nEntryLen )
        {
            *pbSfi = szEntry[0];
            *pbNum = szEntry[1];
            DbgPrintf("日志的SFI=0x%02X 记录数为0x%02X\n", *pbSfi, *pbNum );
            nRetVal = RET_SUCCESS;
        }
     }
     else 
     {
         DbgPrintf("获取日志入口失败\n");
         DbgPrintHex( "FCI数据为", szFci, nFciLen ); 
     }
    
     return nRetVal;
}

/*!
 * \brief         ICC_GetTransferLogEntry     
 * \par 说明      获取圈存日志的入口 	
 * \param[in]         	
 * \param[in]          
 * \param[in]	
 * \param[in]
 * \param[out]
 * \param[out]		
 * \retval              0 成功 -1  失败
 * \note                
 */
S32 ICC_GetCreditLoadLogEntry( U8* szFci, S32 nFciLen, U8* pbSfi, U8* pbNum )
{
     S32 nRetVal = RET_LOG_NO_ENTRY;        
     U8 szEntry[ 32 ] = {0};
     S32 nEntryLen = 0;
    
     DbgPrintf("获取日志入口\n"); 
     if( RET_SUCCESS ==  centerm_get_tag_value(  (U8 *)"\xDF\x4D",  2,  szFci,  nFciLen, szEntry, &nEntryLen ) )
     {
        /*日志入口的值应为2个字节*/
        if( 2 == nEntryLen )
        {
            *pbSfi = szEntry[0];
            *pbNum = szEntry[1];
            DbgPrintf("日志的SFI=0x%02X 记录数为0x%02X\n", *pbSfi, *pbNum );
            nRetVal = RET_SUCCESS;
        }
     }
     else 
     {
         DbgPrintf("获取日志入口失败\n");
         DbgPrintHex( "FCI数据为", szFci, nFciLen ); 
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
          DbgPrintHex( "读取日志数据", szLog, *pnLogLen );
          nRetVal = RET_SUCCESS;
      }
      else 
      {
          
          DbgPrintf( "读取 SFI = 0x%02X, Index =0x%02X 日志错误，原因 %s\n", SFI, Index, ICC_CardCmdShowLastErrMsg()  );
          
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
    
    DbgPrintf("获取CDOL1列表\n");   
      
    nRetVal = ICC_GetDOL( (U8 *)TAG_CDOL1,  strlen(TAG_CDOL1), szAppData, nAppDataLen, szCdol1, pnCdol1Len );            

    if( RET_SUCCESS ==  nRetVal )
    {
        DbgPrintHex( "CDOL1列表", szCdol1, *pnCdol1Len );   
    }
	else 
	{
		DbgPrintf("获取CDOL1列表失败,需要赋值");
		AsciiToHex(IC_CDOL1_DATA, strlen(IC_CDOL1_DATA), szCdol1);
		*pnCdol1Len = strlen(IC_CDOL1_DATA) / 2;
		nRetVal = RET_SUCCESS;
	}
    
    return nRetVal;
}

S32 ICC_GetCDOL2( U8* szAppData, S32 nAppDataLen, U8* szCdol2, S32* pnCdol2Len )
{
    S32 nRetVal = RET_DOL_NO_FOUND;
    
    DbgPrintf("获取CDOL2列表\n");   
      
    nRetVal = ICC_GetDOL( (U8 *)TAG_CDOL2,  strlen(TAG_CDOL2), szAppData, nAppDataLen, szCdol2, pnCdol2Len );            

    if( RET_SUCCESS ==  nRetVal )
    {
        DbgPrintHex( "CDOL2列表", szCdol2, *pnCdol2Len );   
    }
    else 
    {
        DbgPrintf("获取CDOL2列表失败\n"); 
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
    
    /*!<终端国家代码*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_COUNTRY_CODE , 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 0x02;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app( (U8*)"\x4A", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "国家代码%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 2 );
    }
    else
    {    
        memcpy( szTermAppData+nTermAppDataLen, "\x01\x56", 2);
    }
    nTermAppDataLen += 2;

    /*!<随机数*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_UNPREDICTABLE_NO, 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 4;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app( (U8*)"\x4B", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "随机数%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 4 );
    }
    else
    {    
        GetRandom( 4, szTermAppData+nTermAppDataLen );
    }
    nTermAppDataLen += 4;    


    /*!<终端验证结果*/
	memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_TVR, 1 );   
	nTermAppDataLen += 1;
	szTermAppData[ nTermAppDataLen++ ] = 0x05;
	memset( szBuff, 0x00, sizeof( szBuff ) );
	nRetVal = centerm_get_tag_value_app((U8*)"\x4C", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
	DbgPrintf( "终端验证结果%s\n", szBuff );
	if( RET_SUCCESS == nRetVal )
	{    
		AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 5 );
	}
	else
	{    
		memcpy( szTermAppData+nTermAppDataLen, "\x00\x00\x00\x08\x00", 5 );
	}
	nTermAppDataLen += 5;

    /*!<终端交易属性*/
    memcpy( szTermAppData+nTermAppDataLen, "\x9F\x66", 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 4;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app( (U8*)"\x4D", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
 	DbgPrintf( "终端交易属性%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 4 );
    }
    else
    {    
        memcpy( szTermAppData+nTermAppDataLen, "\xF4\x00\x00\x00", 4 );
    }
    nTermAppDataLen += 4;   
    
    /*!<电子现金终端支持指示器*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_SUPPORT_INDICATOR, 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 0x01;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app( (U8*)"\x4E", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "电子现金终端支持指示器%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 1 );
    }
    else
    {    
        szTermAppData[ nTermAppDataLen ] = 0x00;
    }
    nTermAppDataLen += 1;   
    
    /*!<电子现金终端交易限额*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_VLP_TRM_TRXN_LIMIT, 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 6;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app( (U8*)"\x4F", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "电子现金终端交易限额%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 6 );
    }
    else
    {    
        memcpy( szTermAppData+nTermAppDataLen, "\x00\x00\x01\x00\x00\x00", 6 );
    }
    nTermAppDataLen += 6;        

    /*!<授权金额*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_AMOUNT_AUTHORISED, 2 );
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 6;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8*)"\x50", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "授权金额 %s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 6 );
    }
    else
    {    
        memcpy( szTermAppData+nTermAppDataLen, "\x00\x00\x00\x00\x00\x00", 6 );
    }
    nTermAppDataLen += 6;
    
    /*!<其它金额*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_AMOUNT_OTHER1, 2 );
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 6;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8*)"\x51", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "其它金额%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 6 );
    }
    else
    {    
        memcpy( szTermAppData+nTermAppDataLen, "\x00\x00\x00\x00\x00\x00", 6 );
    }
    nTermAppDataLen += 6;
   
    /*!<交易货币代码*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_TRXN_CURRENCY_CODE, 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 2;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8*)"\x52", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen  );
    DbgPrintf( "交易货币代码%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {    
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 2 );
    }
    else
    {
        memcpy( szTermAppData+nTermAppDataLen, "\x01\x56", 2 );
    }
    nTermAppDataLen += 2;  

    /*!<交易日期*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_TRXN_DATE, 1 );  
    nTermAppDataLen += 1;
    szTermAppData[ nTermAppDataLen++ ] = 3;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8 *)"\x53", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen   );
    DbgPrintf( "交易日期%s\n", szBuff );
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

    /*!<交易类型*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_TRXN_TYPE, 1 );   
    nTermAppDataLen += 1;
    szTermAppData[ nTermAppDataLen++ ] = 1;  
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8 *)"\x54", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen  ); 
    DbgPrintf( "交易类型%s\n", szBuff );
    if( RET_SUCCESS == nRetVal )
    {   
        AsciiToBcd( szBuff, nBuffLen, szTermAppData+nTermAppDataLen, 1 );
    }
    else
    {   
        memcpy( szTermAppData+nTermAppDataLen, "\x00", 1);
    }
    nTermAppDataLen += 1;  

    /*!<交易时间*/
    memcpy( szTermAppData+nTermAppDataLen, TAG_TRM_TRXN_TIME, 2);   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 3;
    memset( szBuff, 0x00, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8 *)"\x55", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf( "交易时间%s\n", szBuff );
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

    /*!< 商户名称*/
    memcpy( szTermAppData+nTermAppDataLen, "\x9F\x4E", 2 );   
    nTermAppDataLen += 2;
    szTermAppData[ nTermAppDataLen++ ] = 20;
    memset( szBuff, 0x0, sizeof( szBuff ) );
    nRetVal = centerm_get_tag_value_app((U8 *)"V", 1, (U8*)szTransData, 0, (U8*)szBuff, &nBuffLen );
    DbgPrintf("商户名称： %s \n ", szBuff  );
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


	/*!< Check 持卡人验证结果*/
	memcpy( szTermAppData+nTermAppDataLen, "\x9F\x34", 2 );   
	nTermAppDataLen += 2;
	szTermAppData[ nTermAppDataLen++ ] = 0x03;
	memcpy( szTermAppData+nTermAppDataLen, "\x3f\x00\x00", 3 );
	nTermAppDataLen += 5;

	/*!< Check 交易状态信息*/
	memcpy( szTermAppData+nTermAppDataLen, "\x9B", 1 );   
	nTermAppDataLen += 1;
	szTermAppData[ nTermAppDataLen++ ] = 0x02;
	memcpy( szTermAppData+nTermAppDataLen, "\x00\x00", 2 );
	nTermAppDataLen += 2;

    memcpy( szCdolData, szTermAppData, nTermAppDataLen );
    *pnCdolDataLen = nTermAppDataLen; 
    
    DbgPrintHex( "CDOL终端数据", szTermAppData, nTermAppDataLen );
      
    return RET_SUCCESS;
}

S32 ICC_GetCDOLIccData( U8 *szCdol, S32 nCdolLen, S8* szDefaultName, S8* szTransData, U8* szAuthCode, U8* szIccData, S32 *pnIccDataLen   )
{
    U8 szCdolData[ MAX_LEN ] = {0};
    S32 nCdolDataLen = 0;
    
    ICC_GetCDOLTermData( szDefaultName, szTransData, szCdolData, &nCdolDataLen  );
    
    /*!<如果存在授权响应码则把授权响应码加到终端数据中*/
    if( szAuthCode )
    {
        memcpy( szCdolData + nCdolDataLen, TAG_ARC, 1 );   
        nCdolDataLen += 1;
        szCdolData[ nCdolDataLen++ ] = 0x02;
        memcpy( szCdolData + nCdolDataLen, szAuthCode, 2 );
        nCdolDataLen += 2;
    }
    
    DbgPrintHex( "组合的CDOL数据", szCdolData, nCdolDataLen );
    
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
            DbgPrintf("执行GAC命令码错误\n");
            return RET_CMD_ERR;    
        }                     
    }
    
    nRetVal = ICC_CardCmdGenerateAC( P1, szIccData, nIccDataLen, szOutData, &nOutDataLen );
    DbgPrintHex("GAC命令响应数据", szOutData, nOutDataLen );
    if( RET_SUCCESS == nRetVal )
    {
        nRetVal = centerm_get_tag_value( (U8 *)TAG_RESPONSE_MSG_FORMAT1, 1, szOutData, nOutDataLen, szAc, pnAcLen );
        if( RET_SUCCESS == nRetVal )
        {
            //密文信息数据(8)+应用交易计数器（ATC）(1)+应用密文（AC）(2)+发卡行应用数据(可选)
            if( *pnAcLen >= 11 )
            {
                nRetVal = RET_SUCCESS;
            }
            else
            {
                DbgPrintf("GAC命令响应数据长度错误，未达到11字节，正确格式：密文信息数据(1)+应用交易计数器（ATC）(2)+应用密文（AC）(8)+发卡行应用数据(可选)\n");
                nRetVal = RET_GAC_RESP_ERR;    
            }
        }
        else
        {
            DbgPrintf("GAC命令请求响应中缺少0x80标签\n");
            nRetVal = RET_GAC_RESP_ERR;    
        }
        /*!<以上还需要处理格式二，目前只支持格式一*/
            
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


    /*!<应用密文*/
    memcpy( szF55+nF55Len, "\x9F\x26", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 8;
    memcpy( szF55+nF55Len, szAc+3, 8 );
    nF55Len += 8;
    
    /*!<密文信息数据*/
    memcpy( szF55+nF55Len, "\x9F\x27", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 1;
    memcpy( szF55+nF55Len, szAc, 1 );
    nF55Len += 1;
    
    /*!<发卡行应用数据*/
    if( nAcLen > 11 )
    {
        memcpy( szF55+nF55Len, "\x9F\x10", 2 );  
        nF55Len += 2;
        szF55[ nF55Len++ ] = nAcLen - 11;
        memcpy( szF55+nF55Len, szAc+11, nAcLen-11);
        nF55Len += (nAcLen-11);
    }
    
    /*!<随机数*/
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
    
    /*!<应用交易计数器*/
    memcpy( szF55+nF55Len, "\x9F\x36", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 2;
    memcpy( szF55+nF55Len, szAc+1, 2 );
    nF55Len += 2;
    
    /*!<终端验证结果*/
    memcpy( szF55+nF55Len, "\x95", 1 );  
    nF55Len += 1;
    szF55[ nF55Len++ ] = 5;
    memcpy( szF55+nF55Len, "\x00\x00\x00\x08\x00", 5 );
    nF55Len += 5;

#ifdef ARQC_MORE_TAG
    /*!<上次联机应用交易计数器（ATC）寄存器*/
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
            
    /*!<交易日期*/
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
    
    /*!<交易类型*/
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

    /*!<授权金额*/
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
    
    /*!<交易货币代码*/
    memcpy( szF55+nF55Len, "\x5F\x2A", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 2;
    memcpy( szF55+nF55Len, "\x01\x56", 2 );
    nF55Len += 2;

#ifdef ARQC_MORE_TAG
    /*!<应用主账号（PAN）*/ 
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

    /*!< 应用主帐号(PAN)系列号*/
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

    /*!<应用交互特征*/
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
 
    /*!<终端国家代码*/
    memcpy( szF55+nF55Len, "\x9F\x1A", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 2;
    memcpy( szF55+nF55Len, "\x01\x56", 2 );
    nF55Len += 2;
    
    /*!<其它金额*/
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
    
    /*!<终端性能*/
    memcpy( szF55+nF55Len, "\x9F\x33", 2 );  
    nF55Len += 2;
    szF55[ nF55Len++ ] = 3;
    memcpy( szF55+nF55Len, "\x20\xA1\x00", 3 );
    nF55Len += 3;
    
    *pnF55Len = nF55Len*2;
    HexToAscii( szF55, nF55Len, szF55Domain );
    DbgPrintf( "55域为 %s\n", szF55Domain );
        
    return RET_SUCCESS;
}

/*!
 * \brief            ICC_BankAuthentication  
 * \par 说明         执行发卡行认证 获取授权相应码
 * \param[in]        szARPC      后台下发的认证数据 	
 * \param[in]        nArpcLen    认证数据的长度 
 * \param[out]       szAuthCode  授权响应码
 * \param[out]		
 * \retval              0 成功 -1  失败
 * \note             标签为‘91’的值域: 发卡行认证数据 ARPC（8 字节）+ 授权响应码（2 字节）           
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
        DbgPrintf( "发卡行认证数据错误，长度小于10\n"); 
        DbgPrintHex("错误数据", szBuf, nBufLen ); 
        return RET_AUTH_DATA_ERROR;
    }
    else if( RET_SUCCESS != nRetVal )
    {
       DbgPrintf( "不存在标签为0x91的发卡行认证数据\n");
       return RET_AUTH_DATA_ERROR;       
    }
    
    /*!<外部认证*/  
    DbgPrintHex( "外部认证数据", szBuf, 10 );
    nRetVal =  ICC_CardCmdExternalAuthenticate( szBuf, 10 );
    if( RET_SUCCESS == nRetVal )
    {
        memcpy( szAuthCode, szBuf+8, 2 );
        DbgPrintf( "发卡行认证成功！\n" ); 
        DbgPrintHex( "从认证数据中获取授权响应码", szAuthCode, 2 );      
    }
    else
    {
        DbgPrintf( "发卡行认证失败，原因：%s\n", ICC_CardCmdShowLastErrMsg() );
    }
    
    return nRetVal; 
}

S32 ICC_ClosedTrade( U8* szAuthCode, U8* szCDOL2, S32 nCDOL2Len, U8 *TC, S32 *pnTCLen )
{
    S32 nRetVal = RET_ERROR;
    U8  Cmd = CMD_GAC_AAC;/*!<默认拒绝*/
    
    if( (!memcmp(szAuthCode, "00", 2)) 
        || (!memcmp(szAuthCode, "10", 2)) 
        || (!memcmp(szAuthCode, "11", 2)) )
    {
        /*!<同意交易, 生成TC*/
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
    
    DbgPrintf("开始写脚本\n");
    /*  取发卡行脚本数据：响应报文中应当只包含标签为‘72’的脚本，
        表明发卡行脚本命令在第2个GENERATE  AC命令之后进行。
    */
    memset(szScript, 0x00, sizeof(szScript));
    nRetVal = centerm_get_tag_value( (U8*)TAG_ISSUER_SCRIPT2, 1,  szArpc, nArpcLen, szScript, &nScriptLen );
    
    if( RET_SUCCESS != nRetVal )
    {
        /*!<取发卡行脚本 71*/
        nRetVal = centerm_get_tag_value( (U8*)TAG_ISSUER_SCRIPT1, 1, szArpc, nArpcLen, szScript, &nScriptLen );
        if( RET_SUCCESS != nRetVal )
        {
            strcpy( szScriptResult, "DF31050000000000" );
            DbgPrintf("ARPC里没有脚本\n");
            return RET_NO_SCRIPT;
        } 
    }
    
    DbgPrintHex("获取的脚本数据", szScript, nScriptLen );
    
    /*!<脚本执行结果*/
    memcpy( szScriptResult, "DF3105", 6 );

    /*!<发卡行脚本标识*/
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

    /*!<执行脚本命令*/
    nRetVal =  ICC_WriteScriptCmd( szScript, nScriptLen, &nScriptNum );
    if( RET_SUCCESS == nRetVal )
    {
        /*!<脚本执行成功*/
        szScriptResult[6] = '2'; 
    }
    else
    {
        /*!<脚本执行失败*/
        szScriptResult[6] = '1';   
        nRetVal = RET_WRITE_SCRIPT_ERR;
    }

    /*!<脚本号*/
    nScriptNum = nScriptNum > 0x0F ? 0x0F : nScriptNum;
    szScriptResult[ 7 ] =  nScriptNum < 10 ? nScriptNum+0x30 : nScriptNum+0x37; 
    DbgPrintf( "脚本执行结果：%s\n", szScriptResult );
    
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
    
   DbgPrintf( "开始执行脚本命令\n");

   (*pnIndex) = 0;
    
    p = centerm_find_tag( (U8 *)TAG_ISSUER_SCRIPT_COMMAND, 1, szScript,  nScriptLen );  
    
    while( p )
    {
        
        memset( C_Apdu, 0x0, sizeof(C_Apdu) );
        ucCapduLen = 0;
        //(char *)&nCapduLen，这种方式在有可能会因为平台大小端不同，
        //导致返回数据长度异常，如0x0f,读出来变成0x0f000000
        centerm_get_tag_data( 1, p, C_Apdu, &ucCapduLen );
        (*pnIndex) ++;
        DbgPrintHex( "脚本命令", C_Apdu, ucCapduLen );

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
                    DbgPrintf( "脚本命令执行错误 序号为 %d\n", (*pnIndex) );
                    DbgPrintf("错误原因：%s\n", ICC_CardCmdShowLastErrMsg() );
                    return RET_SCRIPT_CMD_ERR;   
                }


            }
            else
            {
                DbgPrintf("写脚本时读卡器通讯失败！\n");
                return RET_READ_ERROR;   
            }
        }

    }
    
    //脚本执行成功，序号为0
    (*pnIndex) = 0;
    
    return RET_SUCCESS;
}

//R20010 add by sulin 20130502
//提高读卡速度
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

	DbgPrintf("开始读IC卡数据！\n");
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
//提高读卡速度
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

	DbgPrintf("开始读IC卡数据！\n");
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
			//			DbgPrintHex("ICC_CardCmdReadSFI数据为:", data, iDataLen ); 
		}
	}
	DbgPrintf("IC卡数据长度 =%d\n", *pAppDataLen );
	DbgPrintHex("IC卡数据:", szAppData, *pAppDataLen );  
	return RET_SUCCESS;
}
