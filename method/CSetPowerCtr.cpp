#include "CSetPowerCtrl.h"
#include <boost/lexical_cast.hpp>

CSetPowerCtrl::CSetPowerCtrl(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CSetPowerCtrl::SetPowerCmd(int Cmd, CData &oResult, char *cResult)
{
    mLogDebug("run SetPowerCmd(...)");
    //获取状态
    //add fwb 20201111
    char cPowerCmd[6] = {0};
    int  cCmdLength = 0;
    memset(cPowerCmd, 0, sizeof(cPowerCmd)); 

    cCmdLength  = 6;

    cPowerCmd[0] = 0; 
    cPowerCmd[1] = 4; 
    cPowerCmd[2] = 0;  
    cPowerCmd[3] = 2;
    cPowerCmd[4] = 0;
    switch (Cmd)  
    {
        case 1: cPowerCmd[5] = 1; break;//系统重启
        case 2: cPowerCmd[5] = 3; break;//恢复出厂设置
        case 3: cPowerCmd[5] = 5; break;//系统远程关机
        case 4: cPowerCmd[5] = 7; break;//系统远程开机
        default: break;
    }
    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cPowerCmd,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSetPowerCtrl::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CSetPowerCtrl::ComposeResult()
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

InterfaceResCode CSetPowerCtrl::SetPowerCtrlCmd(string &sResult,int Cmd)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetPowerCtrlCmd Cmd %d\n", Cmd);
#if 1
    if(SetPowerCmd(Cmd, oResult, cResult) == false){
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
    if( _AnalyParas.ConvertSTRING2CJSONSetPowerCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}




