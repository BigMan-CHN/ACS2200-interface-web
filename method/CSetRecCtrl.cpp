#include "CSetRecCtrl.h"
#include <boost/lexical_cast.hpp>

CSetRecCtrl::CSetRecCtrl(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_VideoFiles(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CSetRecCtrl::SetRecStart(int RecMode,int ChannelID, string TaskId, string Title, CData &oResult, char *cResult)
{
    mLogDebug("run SetRecStart(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    if (RecMode == 0)
    {
        sprintf(cChnCmd,INTERFACE_SETRECORDSTART_DEFAULT_SENDCMD, 1, TaskId.c_str(), Title.c_str());
    }
    else if(RecMode == 1)
    {
        sprintf(cChnCmd,INTERFACE_SETRECORDSTART_SENDCMD, ChannelID+1, TaskId.c_str(), Title.c_str());    
    }
#endif 

    //add fwb 20201110
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 3; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConsoleNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetRecCtrl::SetRecStop(int RecMode, string TaskId, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    if (RecMode == 0)
    {
            sprintf(cChnCmd,INTERFACE_SETRECORDSTOP_DEFAULT_SENDCMD, TaskId.c_str());
    }
    else if(RecMode == 1)
    {
            sprintf(cChnCmd,INTERFACE_SETRECORDSTOP_SENDCMD, TaskId.c_str());    
    }

#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 3; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConsoleNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetRecCtrl::GetRecInfoList(int GetMode,string TaskId, string StartTime, string EndTime,CData &oResult, char *cResult)
{
    mLogDebug("run SetRecStart(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_GETRECINFOLIST_SENDCMD, GetMode, TaskId.c_str(),StartTime.c_str(),EndTime.c_str());
#endif 

    //add fwb 20201110
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 3; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 7; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConsoleNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetRecCtrl::SetRecTaskDel(string TaskId, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    sprintf(cChnCmd,INTERFACE_SETRECODELETE_SENDCMD, TaskId.c_str());    
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 3; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 3; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConsoleNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetRecCtrl::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CSetRecCtrl::ComposeResult()
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

bool CSetRecCtrl::GetComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    cJSON_AddItemToObject(_result, GET_INFOLIST_RES_VIDEOFILE, _VideoFiles);
    return true;
}

string CSetRecCtrl::GetComposeResult()
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

InterfaceResCode CSetRecCtrl::SetRecCtrlStart(string &sResult,int RecMode,int ChannelID, string TaskId, string Title)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
#if 1
    if(SetRecStart(RecMode, ChannelID, TaskId, Title, oResult, cResult) == false){
        mLogError("Failed to run  SetRecCtrlStart(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetRecCtrl(oResult, cResult, _result) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetRecCtrl::SetRecCtrlStop(string &sResult,int RecMode,string TaskId)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlStop RecMode %d ChnID %d TaskId %s\n", RecMode, ChnID, TaskId.c_str());
#if 1
    if(SetRecStop(RecMode, TaskId, oResult, cResult) == false){
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
    if( _AnalyParas.ConvertSTRING2CJSONSetRecCtrl(oResult, cResult,_result) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetRecCtrl::GetRecCtrlInfoList(string &sResult,int GetMode,string TaskId, string StartTime, string EndTime)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[INFO_LIST_MAX_BUFF] = {0};
    //memset(cResult, 0, INFO_LIST_MAX_BUFF);
    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
#if 1
    if(GetRecInfoList(GetMode, TaskId, StartTime, EndTime, oResult, cResult) == false){
        mLogError("Failed to run  SetRecCtrlStart(...)");
        return eInterfaceResCodeError;
    }
#endif 
    
#if 0
    unsigned int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("GetRecCtrlInfoList %d\n", iRecvLength);
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

#if 0
    if( _AnalyParas.ConvertSTRING2CJSONGetRecCtrl(oResult, cResult, _VideoFiles, _result) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = GetComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetRecCtrl::SetRecCtrlTaskDel(string &sResult, string TaskId)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetRecCtrlTaskDel TaskId %s\n", TaskId.c_str());
#if 1
    if(SetRecTaskDel(TaskId, oResult, cResult) == false){
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

#if 0
    if( _AnalyParas.ConvertSTRING2CJSONSetRecCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetRecCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}
bool CSetRecCtrl::GetWebRecordTimeInner(CData &oResult, char *cResult)
{
    mLogDebug("run GetWebRecordTimeInner(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
	char *ReqBody;
	int flag = 0;
#if 1
	sprintf(cChnCmd,"net_send100.app_info\n");
//	for(int i = 1; i <= 9; i++)
//	{
//		ReqBody = cChnCmd + strlen(cChnCmd);
//		sprintf(ReqBody, "net_send%d.rec_stime\nnet_send%d.rec_path\nnet_send%d.rec_current_status\n", i, i, i);
//	}
		//mLogDebug("############################PauseRecordInner Chn="<<Chn);

#endif 

    //add fwb 20201027
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

InterfaceResCode CSetRecCtrl::GetWebRecordTime(string &sResult)
{
    //TODO
    mLogInfo("GetWebRecordTime...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
	char Result[4096] = {0};
	string Method = "GetWebRecordTime";
	string IP;
    cJSON* temList[15];
	for(int i = 1; i <= 14; i++)
	{ //用于生成节点，存储解析后的参数
		temList[i] = cJSON_CreateObject();
	}
#if 1
    if(GetWebRecordTimeInner(oResult, cResult) == false){
        mLogError("Failed to run  GetWebRecordTimeInner(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONGetWebParam( oResult, cResult, _result, NULL, temList,  Method, IP, 0) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetWebParam(...)");
        return eInterfaceResCodeError;
    }
#endif 

	for(int i = 1; i <= 9; i++)
	{
		cJSON *OldRecState = cJSON_GetObjectItem(temList[i],"OldRecStatus");
		cJSON *OldTaskID = cJSON_GetObjectItem(temList[i],"OldRecTaskID");
		cJSON *RecStatus  = cJSON_GetObjectItem(temList[i],"RecStatus");
		cJSON *RecTaskID = cJSON_GetObjectItem(temList[i],"TaskID");
	
		if(OldRecState->valueint == 1)
		{
			cJSON_DeleteItemFromObject(temList[i],"RecStatus");
			cJSON_AddItemToObject(temList[i],"RecStatus",cJSON_CreateNumber(1));
			cJSON_DeleteItemFromObject(temList[i],"TaskID");
			cJSON_AddItemToObject(temList[i],"TaskID",cJSON_CreateString(OldTaskID->valuestring));
		}
		
		if((OldRecState->valueint == 0)&&(RecStatus->valueint != 2))
		{
			cJSON_DeleteItemFromObject(temList[i],"RecStatus");
			cJSON_AddItemToObject(temList[i],"RecStatus",cJSON_CreateNumber(0));
			cJSON_DeleteItemFromObject(temList[i],"TaskID");
			cJSON_AddItemToObject(temList[i],"TaskID",cJSON_CreateString(OldTaskID->valuestring));
		}

		cJSON_AddItemToArray(_VideoFiles, temList[i]);
	}

	cJSON_AddItemToObject(_result, "list", _VideoFiles);
    sResult = ComposeResult();
	cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}


