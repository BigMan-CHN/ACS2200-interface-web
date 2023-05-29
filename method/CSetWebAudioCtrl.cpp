#include "CSetWebAudioCtrl.h"
#include <boost/lexical_cast.hpp>

CSetWebAudioCtrl::CSetWebAudioCtrl(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CSetWebAudioCtrl::SetWebAudioDecOutput(int VolSize,int VolChn, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETWEBAUDIOCTRL_SENDCMD, VolChn, VolSize);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
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


bool CSetWebAudioCtrl::SetWebAudioLineOutput(int VolSize,int VolChn, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETWEBAUDIOCTRL_SENDCMD, VolChn, VolSize);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
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


bool CSetWebAudioCtrl::SetWebAudioBalanceOutput(int VolSize,int VolChn, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETWEBAUDIOCTRL_SENDCMD, VolChn, VolSize);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
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

bool CSetWebAudioCtrl::SetWebAudioDecInput(int VolSize,int VolChn, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETWEBAUDIOCTRL_SENDCMD, VolChn, VolSize);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
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

bool CSetWebAudioCtrl::SetWebAudioMicInput(int VolSize,int VolChn, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETWEBAUDIOCTRL_SENDCMD, VolChn, VolSize);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
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

bool CSetWebAudioCtrl::SetWebAudioLineInput(int VolSize,int VolChn, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETWEBAUDIOCTRL_SENDCMD, VolChn, VolSize); 
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
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

bool CSetWebAudioCtrl::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CSetWebAudioCtrl::ComposeResult()
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

InterfaceResCode CSetWebAudioCtrl::SetWebAudioCtrlMicInput(string &sResult,int VolSize,int VolChn)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStart Dectype %d Channal %d CacheTime %d sAddr %s TcpMode %d\n", Dectype, Channal, CacheTime, sAddr.c_str(), TcpMode);
#if 1
    if(SetWebAudioMicInput(VolSize, VolChn, oResult, cResult) == false){
        mLogError("Failed to run  SetWebAudioCtrlMicInput(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetWebAudioCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebAudioCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebAudioCtrl::SetWebAudioCtrlLineInput(string &sResult,int VolSize,int VolChn)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 1
    if(SetWebAudioLineInput(VolSize, VolChn, oResult, cResult) == false){
        mLogError("Failed to run  SetWebAudioCtrlLineInput(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetWebAudioCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebAudioCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}


InterfaceResCode CSetWebAudioCtrl::SetWebAudioCtrlDecInput(string &sResult,int VolSize,int VolChn)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 1
    if(SetWebAudioDecInput(VolSize, VolChn, oResult, cResult) == false){
        mLogError("Failed to run  SetWebAudioCtrlLineInput(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetWebAudioCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebAudioCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebAudioCtrl::SetWebAudioCtrlBalanceOutput(string &sResult,int VolSize,int VolChn)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 1
    if(SetWebAudioBalanceOutput(VolSize, VolChn, oResult, cResult) == false){
        mLogError("Failed to run  SetWebAudioCtrlLineInput(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetWebAudioCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebAudioCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebAudioCtrl::SetWebAudioCtrlLineOutput(string &sResult,int VolSize,int VolChn)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 1
    if(SetWebAudioLineOutput(VolSize, VolChn, oResult, cResult) == false){
        mLogError("Failed to run  SetWebAudioCtrlLineInput(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetWebAudioCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebAudioCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebAudioCtrl::SetWebAudioCtrlDecOutput(string &sResult,int VolSize,int VolChn)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 1
    if(SetWebAudioDecOutput(VolSize, VolChn, oResult, cResult) == false){
        mLogError("Failed to run  SetWebAudioCtrlLineInput(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetWebAudioCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebAudioCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}
