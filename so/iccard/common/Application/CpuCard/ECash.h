#ifndef  _E_CASH_H_
#define  _E_CASH_H_

#define IC_CDOL1_DATA ("9F02069F03069F1A0295055F2A029A039C019F37049F21039F4E14")

struct recordlist
{
    U8 * precord;
    S32 nRecordLen;
    struct recordlist * pnext;
};
typedef struct recordlist RECORD_LIST;

 struct aidlist_t
{
	U8 AID[16];
	U8 AidLen;
	U8 AppTag[16];
	U8 AppTagLen;   
    U8 APN[16];
	U8 APNlen;   
	U8 APPpriority;
	struct aidlist_t *next;
};
typedef struct aidlist_t  AID_LIST;


struct TERMDATA
{
    char BankName[21];//商户名称
    int  ECardIndex;  //电子现金终端支持指示器 
	char EncryptIndex;//算法指示器
    
};

#ifdef __cplusplus
extern "C" {
#endif


void ICC_FreeAidList( AID_LIST *pAidList );
S32 ICC_GetPDOL( U8* szFci,  S32 nFciLen,  U8*szPdol,  S32 *pnPdolLen );
S32 ICC_GetTagInDOL( U8* szPdol,  S32 nPdolLen, U8* szTagName, S32 *pnTagNameLen, S32  *pnTagLen );
//S32 ICC_GetPdolTermData( S8* szBankName, U8  *szPdol, S32 *pnPdolLen );
S32 ICC_GetPdolTermData(  struct TERMDATA *pTermData, U8  *szPdol, S32 *pnPdolLen );
S32 ICC_GetTagInDOL( U8* szDol,  S32 nDolLen, U8* szTagName, S32 *pnTagNameLen, S32  *pnTagLen );
S32 ICC_GetAidList( U8* pszDDFname,  S8 nDDFnameLen,  AID_LIST **ppAidList,  S32 *pnAidNum  );
S32 ICC_GetAppList( AID_LIST **ppAidList,  S32 *pnAidNum  );
S32 ICC_AppSelect( U8* AID, S32 nAidLen,   U8 *szFci, S32 *pnFciLen );
S32 ICC_AppInit( S8 *szBankName, U8 *szFci, S32 nFciLen, U8 * szAfl, S32 *pnAflLen,  U8 *szAip, S32 *pnAipLen );
S32 ICC_GetLogEntry( U8 *szFci,  S32 nFciLen, U8*pbSfi, U8 *pbNum );
S32 ICC_GetCreditLoadLogEntry( U8* szFci, S32 nFciLen, U8* pbSfi, U8* pbNum );
S32 ICC_GetLog( U8 SFI, U8 Index, U8 *szLog, S32 *pnLogLen );
S32 ICC_GetDOL( U8* szTagName, S32 nTagLen, U8* szData, S32 nDataLen, U8* szDol, S32 *pnDolLen );
S32 ICC_GetCDOL1( U8* szAppData, S32 nAppDataLen, U8* szCdol1, S32* pnCdol1Len );
S32 ICC_GetCDOL2( U8* szAppData, S32 nAppDataLen, U8* szCdol2, S32* pnCdol2Len );
S32 ICC_GetCDOLTermData( S8* szDefaultName, S8* szTransData, U8 *szCdolData, S32 *pnCdolDataLen  );
S32 ICC_ExecuteGacCmd( U8 Cmd, U8* szIccData, S32 nIccDataLen, U8* szArqc, S32* pnArqcLen );
S32 ICC_GetF55Domain( U8 *szAc, S32 nAcLen, U8 *szTermData, S32 nTermDatalen, S8 *szTransData, U8 *szF55Domain, S32 *pnF55Len );
S32 ICC_BankAuthentication( U8 *szARPC, S32 nArpcLen, U8* szAuthCode );
S32 ICC_GetCDOLIccData( U8 *szCdol, S32 nCdolLen, S8* szDefaultName, S8* szTransData, U8* szAuthCode, U8* szIccData, S32 *pnIccDataLen   );
S32 ICC_WriteScript( U8 *szArpc, S32 nArpcLen, char *szScriptResult );
S32 ICC_WriteScriptCmd( U8 *szScript, S32 nScriptLen, S32* pnIndex );
S32 ICC_TransReadAppDataByTag( U8 *szAFL, S32 nAFLlen, const U8 *szApptag, S32 nApptagLen, U8 *szAppData, S32 *pAppDataLen);
S32 ICC_TransReadCDOL( U8 *szAFL, S32 nAFLlen, U8 *szAppData, S32 *pAppDataLen);

S32 ICC_ClosedTrade( U8* szAuthCode, U8* szCDOL2, S32 nCDOL2Len, U8 *TC, S32 *pnTCLen );
S32 ICC_AppInitByTermData( struct TERMDATA *pTermData , U8* szFci, S32 nFciLen, U8* szAfl, S32* pnAflLen,  U8* szAip, S32* pnAipLen );
S32 ICC_GetIccDataByDOL( U8 *szDol, S32 nDolLen, U8 *szDolData,  S32  nDolDataLen,  U8 *szIccData,   S32 *pnIccDataLen  );
#ifdef __cplusplus
}
#endif/*!< __cplusplus*/

#endif
