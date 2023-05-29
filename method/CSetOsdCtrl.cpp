#include "CSetOsdCtrl.h"
#include <boost/lexical_cast.hpp>

CSetOsdCtrl::CSetOsdCtrl(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CSetOsdCtrl::SetOsdStart(int OsdMode,int ChannelID,string Title,CData &oResult,char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态
    char cChnCmd[4096] = {0};
#if 1
    if (OsdMode == 0)
    {
        sprintf(cChnCmd,INTERFACE_SETADDOSD_SENDCMD, 1, 1, 1, Title.c_str());   
    }
    else 
    {
        sprintf(cChnCmd,INTERFACE_SETADDOSD_SENDCMD, ChannelID+1, ChannelID+1, ChannelID+1, Title.c_str());
    }
#endif 
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（SET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetOsdCtrl::SetOsdStop(int OsdMode,int ChannelID,CData &oResult,char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态
    char cChnCmd[4096] = {0};
#if 1
    if (OsdMode == 0)
    {
        sprintf(cChnCmd,INTERFACE_SETDELOSD_SENDCMD, 1);   
    }
    else 
    {
        sprintf(cChnCmd,INTERFACE_SETDELOSD_SENDCMD, ChannelID+1);
    }
#endif 
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（SET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetOsdCtrl::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CSetOsdCtrl::ComposeResult()
{
    cJSON * result = cJSON_CreateObject();
    if(ComposeResult(result) == false){
        cJSON_Delete(result);
        return "";
    }else{
        char *cRes = cJSON_Print(_result);
        string res = cRes;
        cJSON_Delete(result);
        return res;
    }
}

InterfaceResCode CSetOsdCtrl::SetOsdCtrlStart(string &sResult,int OsdMode,int ChannelID,string Title)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStart OsdMode %d ChannelID %d Title %s\n", OsdMode, ChannelID, Title.c_str());
#if 1
    if(SetOsdStart(OsdMode, ChannelID, Title, oResult, cResult) == false){
        mLogError("Failed to run  getMPSConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
    ;
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("getMPSConfig\n");
    for (int i = 0; i < iRecvLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cResult[i]);
	    }
	    else 
	    {
		    printf("%c", cResult[i]);
	    }
    }
#endif 

#if 1
    if( _AnalyParas.ConvertSTRING2CJSONSetOsdCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetOsdCtrl::SetOsdCtrlStop(string &sResult,int OsdMode,int ChannelID)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop OsdMode %d ChannelID %d\n", OsdMode, ChannelID);
#if 1
    if(SetOsdStop(OsdMode, ChannelID, oResult, cResult) == false){
        mLogError("Failed to run  SetOsdStop(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
    ;
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("getMPSConfig\n");
    for (int i = 0; i < iRecvLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cResult[i]);
	    }
	    else 
	    {
		    printf("%c", cResult[i]);
	    }
    }
#endif 

#if 1
    if( _AnalyParas.ConvertSTRING2CJSONSetOsdCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}


