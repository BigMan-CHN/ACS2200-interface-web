#include "CSetDecCtrl.h"
#include <boost/lexical_cast.hpp>

CSetDecCtrl::CSetDecCtrl(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CSetDecCtrl::SetDecStart(int Dectype,int Channal, int CacheTime, string sAddr, bool TcpMode, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
            
    sprintf(cChnCmd,INTERFACE_SETDECSTART_RTSP_SENDCMD, Channal, sAddr.c_str(), Channal, CacheTime, Channal, ((TcpMode == true)?"tcp":"udp"));

#if 0
    if (Dectype == 1)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_RTSP_SENDCMD, Channal+1, sAddr.c_str(), Channal+1, CacheTime, Channal+1, ((TcpMode == true)?"tcp":"udp"));
    }
    else if(Dectype == 2)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_H323_SENDCMD, Channal+1, sAddr.c_str(), Channal+1, CacheTime);    
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

bool CSetDecCtrl::SetDecStop(int Dectype, int Channal, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};

    sprintf(cChnCmd,INTERFACE_SETDECSTOP_RTSP_SENDCMD, Channal);
#if 0
    if (Dectype == 1)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTOP_RTSP_SENDCMD, Channal+1);
    }
    else if(Dectype == 2)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTOP_H323_SENDCMD, Channal+1);    
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

bool CSetDecCtrl::SetDecAddPic(int Channal, string imgPath, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};

    sprintf(cChnCmd,INTERFACE_SETDECPICCFG_SENDCMD, Channal, 4);

    //add fwb 20201027
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

bool CSetDecCtrl::SetDecClrPic(int Channal, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};

    sprintf(cChnCmd,INTERFACE_SETDECPICCFG_SENDCMD, Channal, 5);

    //add fwb 20201027
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

bool CSetDecCtrl::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CSetDecCtrl::ComposeResult()
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

InterfaceResCode CSetDecCtrl::SetDecCtrlStart(string &sResult,int Dectype,int Channal, int CacheTime, string sAddr, bool TcpMode)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStart Dectype %d Channal %d CacheTime %d sAddr %s TcpMode %d\n", Dectype, Channal, CacheTime, sAddr.c_str(), TcpMode);
#if 1
    if(SetDecStart(Dectype, Channal, CacheTime, sAddr, TcpMode, oResult, cResult) == false){
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
    if( _AnalyParas.ConvertSTRING2CJSONSetDecCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetDecCtrl::SetDecCtrlStop(string &sResult,int Dectype,int Channal)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 1
    if(SetDecStop(Dectype, Channal, oResult, cResult) == false){
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
    if( _AnalyParas.ConvertSTRING2CJSONSetDecCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetDecCtrl::SetDecCtrlAddPic(string &sResult,int Channal,string imgPath)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 1
    if(SetDecAddPic(Channal, imgPath, oResult, cResult) == false){
        mLogError("Failed to run  SetDecAddPic(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetDecCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetDecCtrl::SetDecCtrlClrPic(string &sResult,int Channal)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 1
    if(SetDecClrPic(Channal, oResult, cResult) == false){
        mLogError("Failed to run  SetDecAddPic(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetDecCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}
