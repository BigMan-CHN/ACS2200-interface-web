#include "CDisciplineCtrl.h"
#include <boost/lexical_cast.hpp>

CDiscipLineCtrl::CDiscipLineCtrl(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_statusinfo(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CDiscipLineCtrl::SetDiscipLineStart(bool LoopFlag, int PlayList, CData &oResult, char *cResult)
{
    mLogDebug("run SetDiscipLineStart(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    //sprintf(cChnCmd,INTERFACE_SETSTARTDISCIPLINE_SENDCMD, TaskId.c_str(), Title.c_str());
    memcpy(cChnCmd,INTERFACE_SETSTARTDISCIPLINE_SENDCMD, strlen(INTERFACE_SETSTARTDISCIPLINE_SENDCMD));
#endif 

    //add fwb 20201111
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

bool CDiscipLineCtrl::SetDiscipLineStop(CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    memcpy(cChnCmd,INTERFACE_SETSTOPDISCIPLINE_SENDCMD, strlen(INTERFACE_SETSTOPDISCIPLINE_SENDCMD));
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

bool CDiscipLineCtrl::GetDiscipLineStatus(CData &oResult, char *cResult)
{
    mLogDebug("run GetDiscipLineStatus(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    //sprintf(cChnCmd,INTERFACE_SETRECODELETE_SENDCMD, TaskId.c_str());    
    memcpy(cChnCmd,INTERFACE_GETSTOPDISCIPLINESTATUS_SENDCMD, strlen(INTERFACE_GETSTOPDISCIPLINESTATUS_SENDCMD));
#endif 

    //add fwb 20201111
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
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

bool CDiscipLineCtrl::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    if(_statusinfo == NULL){
        mLogWarn("cJSON network == NULL.");
        return false;
    }

    //result中添加节点
    //cJSON_AddItemToObject(_result,GET_HOST_CONFIG_DISCIPLINE_STATUSINFO, _statusinfo);
    return true;
}

string CDiscipLineCtrl::ComposeResult()
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

bool CDiscipLineCtrl::GetComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    if(_statusinfo == NULL){
        mLogWarn("cJSON network == NULL.");
        return false;
    }

    //result中添加节点
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_DISCIPLINE_STATUSINFO, _statusinfo);
    return true;
}

string CDiscipLineCtrl::GetComposeResult()
{
    cJSON * result = cJSON_CreateObject();
    if(GetComposeResult(result) == false){
        cJSON_Delete(result);
        return "";
    }else{
        char *cRes = cJSON_Print(_result);
        string res = cRes;
        cJSON_Delete(result);
        return res;
    }
}

InterfaceResCode CDiscipLineCtrl::SetDiscipLineCtrlStart(string &sResult,bool LoopFlag, int PlayList)
{
    //TODO
    mLogInfo("SetDiscipLineCtrlStart...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlStart LoopFlag %d PlayList %d\n", LoopFlag, PlayList);
#if 1
    if(SetDiscipLineStart(LoopFlag, PlayList, oResult, cResult) == false){
        mLogError("Failed to run  SetDiscipLineStart(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
    ;
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("SetRecCtrlStart\n");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetDiscipLineCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetDiscipLineCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CDiscipLineCtrl::SetDiscipLineCtrlStop(string &sResult)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
#if 1
    if(SetDiscipLineStop(oResult, cResult) == false){
        mLogError("Failed to run  SetDiscipLineStop(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
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
    if( _AnalyParas.ConvertSTRING2CJSONSetDiscipLineCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetDiscipLineCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CDiscipLineCtrl::GetDiscipLineCtrlStatus(string &sResult)
{
    //TODO
    mLogInfo("GetDiscipLineCtrlStatus...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
#if 1
    if(GetDiscipLineStatus(oResult, cResult) == false){
        mLogError("Failed to run  SetRecStop(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
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
    if( _AnalyParas.ConvertSTRING2CJSONGetDiscipLineCtrl(oResult, cResult, _statusinfo) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetDiscipLineCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = GetComposeResult();
    return eInterfaceResCodeSuccess;
}

