#include "CGetWebHostConfig.h"
#include <boost/lexical_cast.hpp>

CGetWebHostConfig::CGetWebHostConfig(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_network(cJSON_CreateObject())
    ,_channels(cJSON_CreateArray())
    ,_sysinfo(cJSON_CreateObject())
    ,_diskinfo(cJSON_CreateObject())
    ,_decnum(cJSON_CreateObject())
    ,_otherinfo(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

InterfaceResCode CGetWebHostConfig::GetWebHostConfig(string &sResult)
{
    //TODO
    mLogInfo("GetWebHostConfig...");
    CData oResult = SVSMAP();
    char cResult[1024] = {0};
	string Method = "GetWebHostConfig";
#if 1
    if(getMPSConfig(oResult, cResult) == false){
        mLogError("Failed to run  getMPSConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 
    //getMPSConfig(oResult, cResult);
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
    if( _AnalyParas.ConvertSTRING2CJSONGetWebHostConfig(oResult, cResult, _network, _sysinfo, _otherinfo, Method) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetWebHostConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

bool CGetWebHostConfig::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    if(_network == NULL){
        mLogWarn("cJSON network == NULL.");
        return false;
    }
    if(_channels == NULL){
        mLogWarn("cJSON channels == NULL.");
        return false;
    }

    //result中添加节点
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_SYSINFO,_sysinfo);
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_NETWORK,_network);
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_OTHERINFO,_otherinfo);

    return true;
}

string CGetWebHostConfig::ComposeResult()
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

bool CGetWebHostConfig::getMPSConfig(CData &oResult, char *cResult)
{
    mLogDebug("run GetMPSConfig(...)");
    //获取状态
    //CData oResult = SVSMAP();

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETWEBHOSTCONFIG_SENDCMD;
	char *cTmpCmd = "sys.all\neth1.all\nstatus.all\n";
//	char *cTmpCmd = "sys.name\nsys._hwver\nsys._swver\nsys.sn\nsys.dev_id\neth1.conf\neth1.macaddr\nstatus.totalsize\nstatus.freedisk\nstatus.cpu\nstatus.mem\nstatus.temp\n";
    int  cCmdLength = 0;
	//sprintf(cTmpCmd,"sys.name\nsys._hwver\nsys._swver\nsys.sn\nsys.dev_id\neth1.conf\neth1.macaddr\nstatus.totalsize\nstatus.freedisk\nstatus.cpu\nstatus.mem\nstatus.temp\n");
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cTmpCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cTmpCmd) >> 8;  
    cHeadCmd[3] = strlen(cTmpCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cTmpCmd, strlen(cTmpCmd));
    MPSOperationRes opResCode = eMPSResultOK;

    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}
