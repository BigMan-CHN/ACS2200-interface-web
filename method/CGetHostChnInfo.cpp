#include "CGetHostChnInfo.h"
#include <boost/lexical_cast.hpp>

CGetHostChnInfo::CGetHostChnInfo(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_params(cJSON_CreateObject())
    ,_channal(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CGetHostChnInfo::getChnConfig(int ChnNum,CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201104
    //int  i = 0;
    //int  itmplen   = 0;
    //char cChnTmpbuf[512] = {0};
    char cChnCmd[4096]   = {0};

    sprintf(cChnCmd,INTERFACE_GETHOSTCHNINFO_SENDCMD,ChnNum+1);
#if 0
    for (i = 1; i < RtspNum + 1; i++)
    {
        sprintf(cChnTmpbuf,INTERFACE_GETHOSTCHNINFO_SENDCMD,i,i,i,i,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, RtspNum, cChnTmpbuf);
        memcpy(cChnCmd + itmplen, cChnTmpbuf, strlen(cChnTmpbuf));
        itmplen += strlen(cChnTmpbuf);
    }
    //printf("len %d \n%s", strlen(cDecCmd), cDecCmd);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 3; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
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

bool CGetHostChnInfo::HttpContent(string url, string content, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201104
    //int  i = 0;
    //int  itmplen   = 0;
    //char cChnTmpbuf[512] = {0};
    char cChnCmd[4096]   = {0};

    sprintf(cChnCmd,INTERFACE_HTTPCONTENT_SENDCMD,url.c_str(), content.c_str());
#if 0
    for (i = 1; i < RtspNum + 1; i++)
    {
        sprintf(cChnTmpbuf,INTERFACE_GETHOSTCHNINFO_SENDCMD,i,i,i,i,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, RtspNum, cChnTmpbuf);
        memcpy(cChnCmd + itmplen, cChnTmpbuf, strlen(cChnTmpbuf));
        itmplen += strlen(cChnTmpbuf);
    }
    //printf("len %d \n%s", strlen(cDecCmd), cDecCmd);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 3; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
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

bool CGetHostChnInfo::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    if(_channal == NULL){
        mLogWarn("cJSON network == NULL.");
        return false;
    }

    //result中添加节点

    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_PARAMS,_params);
    return true;
}

string CGetHostChnInfo::ComposeResult()
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

InterfaceResCode CGetHostChnInfo::GetHostChnInfo(string &sResult,int ChnNum)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
    //printf("GetHostChnInfo ChnNum %d RtmpNum %d\n", ChnNum, RtmpNum);
#if 1
    if(getChnConfig(ChnNum, oResult, cResult) == false){
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

    if( _AnalyParas.ConvertSTRING2CJSONGetHostChnInfo(oResult, cResult, _result, _channal) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }


    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}


InterfaceResCode CGetHostChnInfo::SendHttpContent(string &sResult,string url, string content)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
    //printf("GetHostChnInfo ChnNum %d RtmpNum %d\n", ChnNum, RtmpNum);
#if 1
    if(HttpContent(url, content, oResult, cResult) == false){
        mLogError("Failed to run  HttpContent(...)");
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

    if( _AnalyParas.ConvertSTRING2CJSONGetHostChnInfo(oResult, cResult, _result, _params) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

