#include "CSetWebMultiCtrl.h"
#include "XMLOpera.h"
#include <boost/lexical_cast.hpp>

CSetWebMultiCtrl::CSetWebMultiCtrl(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CSetWebMultiCtrl::SetWebMultiSipList(int DecType,int DecMethod,int Rate,int LenBuf,string DecUrl,string Title,CData &oResult,char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 0
    if (Dectype == 1)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_RTSP_SENDCMD, DecID+1, DecUrl.c_str(), DecID+1, 3000, DecID+1, ((true == true)?"tcp":"udp"));
    }
    else if(Dectype == 2)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_H323_SENDCMD, DecID+1, DecUrl.c_str(), DecID+1, 3000);    
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

bool CSetWebMultiCtrl::SetWebMultiSip(int DecMode, int DecID, string DecUrl, string UserName, string UserPw, CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 0
    if (Dectype == 1)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_RTSP_SENDCMD, DecID+1, DecUrl.c_str(), DecID+1, 3000, DecID+1, ((true == true)?"tcp":"udp"));
    }
    else if(Dectype == 2)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_H323_SENDCMD, DecID+1, DecUrl.c_str(), DecID+1, 3000);    
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

bool CSetWebMultiCtrl::SetWebMultiH323(int DecID,int VideoEncType,int AudioEncType,string DecUrl,string Contact,string SendVideoSrc,string SendAudioSrc, CData &oResult, char *cResult)
{
    mLogDebug("run SetWebMultiH323(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 0
    if (Dectype == 1)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_RTSP_SENDCMD, DecID+1, DecUrl.c_str(), DecID+1, 3000, DecID+1, ((true == true)?"tcp":"udp"));
    }
    else if(Dectype == 2)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_H323_SENDCMD, DecID+1, DecUrl.c_str(), DecID+1, 3000);    
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

bool CSetWebMultiCtrl::SetWebMultiRtsp(int DecMethod,int DecID,int DecMode,int DecFormat,string DecUrl, CData &oResult, char *cResult)
{
    mLogDebug("run SetWebMultiRtsp(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 0
    if (Dectype == 1)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_RTSP_SENDCMD, DecID+1, DecUrl.c_str(), DecID+1, 3000, DecID+1, ((true == true)?"tcp":"udp"));
    }
    else if(Dectype == 2)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_H323_SENDCMD, DecID+1, DecUrl.c_str(), DecID+1, 3000);    
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

bool CSetWebMultiCtrl::SetWebMultiStart(int Dectype,int DecID, string DecUrl, CData &oResult, char *cResult)
{
    mLogDebug("run SetWebMultiStart(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    if (Dectype == 0)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_RTSP_SENDCMD, DecID, DecUrl.c_str(), DecID, 0, DecID, ((true == true)?"tcp":"udp"));
		//sprintf(cChnCmd,"net_recv%d.url=%s\nnet_recv%d.cache=%d\nnet_recv%d.trans_protol=%s\ncomp1.win1.channel=VN1-1\nvid_enc13.channel=VN2-1\nvid_enc13.channel=VN1-1\n", DecID, DecUrl.c_str(), DecID, 0, DecID, ((true == true)?"tcp":"udp"));
    }
    else if(Dectype == 1)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_H323_SENDCMD, DecID, DecUrl.c_str(), DecID, 0);  
		//sprintf(cChnCmd,"h323h%d.call=%s,1\nh323h%d.buf_size=%d\nvid_enc11.channel=VN2-1\nvid_enc11.channel=VN3-1\n", DecID, DecUrl.c_str(), DecID, 0);  
    }
    else if(Dectype == 2)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTART_SIP_SENDCMD, DecID, DecUrl.c_str());    
		//sprintf(cChnCmd,"sip%d.call=sip:1000@%s\ncomp1.win1.channel=VN4-1\nvid_enc12.channel=VN2-1\nvid_enc12.channel=VN4-1\n", DecID, DecUrl.c_str()); 
    }
	else if(Dectype == 3)
    {
		switch(DecID)
		{
			case 1:
				sprintf(cChnCmd,"sip%d.call=sip:1000@%s\n", DecID, DecUrl.c_str());
				//sprintf(cChnCmd,INTERFACE_SETDECSTART_SIP_SENDCMD, DecID, DecUrl.c_str());
				//sprintf(cChnCmd,INTERFACE_SETDECSTART_CALL_SIP_SENDCMD, DecID, DecUrl.c_str(),12,"VN4-1"); 
				//sprintf(cChnCmd,"sip%d.call=sip:1000@%s\nvid_enc%d.channel=%s\ncomp1.win1.channel=VN4-1\n", DecID, DecUrl.c_str(),12,"VN4-1");
				break;
			case 2:
				sprintf(cChnCmd,"sip%d.call=sip:1001@%s\n", DecID, DecUrl.c_str());
				//sprintf(cChnCmd,INTERFACE_SETDECSTART_SIP_SENDCMD, DecID, DecUrl.c_str());
				///sprintf(cChnCmd,INTERFACE_SETDECSTART_CALL_SIP_SENDCMD, DecID, DecUrl.c_str(),11,"VN5-1");
				//sprintf(cChnCmd,"sip%d.call=sip:1000@%s\nvid_enc%d.channel=%s\ncomp1.win2.channel=VN5-1\n", DecID, DecUrl.c_str(),11,"VN5-1");
				break;
			case 3:
				sprintf(cChnCmd,"sip%d.call=sip:1002@%s\n", DecID, DecUrl.c_str());
				//sprintf(cChnCmd,INTERFACE_SETDECSTART_SIP_SENDCMD, DecID, DecUrl.c_str());
				//sprintf(cChnCmd,INTERFACE_SETDECSTART_CALL_SIP_SENDCMD, DecID, DecUrl.c_str(),13,"VN6-1"); 
				//sprintf(cChnCmd,"sip%d.call=sip:1000@%s\nvid_enc%d.channel=%s\ncomp1.win3.channel=VN6-1\n", DecID, DecUrl.c_str(),13,"VN6-1");
				break;
			default:
				mLogError("The sip call DecID not support!!!");
			    return false;
		} 
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
    cHeadCmd[1] = 3; //参数指令（SET）
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

bool CSetWebMultiCtrl::SetWebMultiStop(int Dectype, int DecID, CData &oResult, char *cResult)
{
    mLogDebug("run SetWebMultiStop(...)");
    //获取状态

    //add fwb 20201110
    char cChnCmd[4096] = {0};
#if 1
    if (Dectype == 0)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTOP_RTSP_SENDCMD, DecID);
		//sprintf(cChnCmd,"net_recv%d.url=\ncomp1.win1.channel=VL1-1\nvid_enc13.channel=VN2-1\n", DecID);
    }
    else if(Dectype == 1)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTOP_H323_SENDCMD, DecID);
		//sprintf(cChnCmd,"h323h%d.hangup\nvid_enc11.channel=VN2-1\n", DecID);
    }
    else if(Dectype == 2)
    {
        sprintf(cChnCmd,INTERFACE_SETDECSTOP_SIP_SENDCMD, DecID);   
		//sprintf(cChnCmd,"sip%d.hangup\ncomp1.win1.channel=VL1-1\nvid_enc12.channel=VN2-1\n", DecID);
    }
	else if(Dectype == 3)
    {
		switch(DecID)
		{
			case 1:
				sprintf(cChnCmd,INTERFACE_SETDECSTOP_SIP_SENDCMD, DecID);
				//sprintf(cChnCmd,INTERFACE_SETDECSTOP_SIP_CALL_SENDCMD, DecID,12,"VN2-1"); 
				break;
			case 2:
				sprintf(cChnCmd,INTERFACE_SETDECSTOP_SIP_SENDCMD, DecID);
				//sprintf(cChnCmd,INTERFACE_SETDECSTOP_SIP_CALL_SENDCMD, DecID,11,"VN2-1"); 
				break;
			case 3:
				sprintf(cChnCmd,INTERFACE_SETDECSTOP_SIP_SENDCMD, DecID);
				//sprintf(cChnCmd,INTERFACE_SETDECSTOP_SIP_CALL_SENDCMD, DecID,13,"VN2-1"); 
				break;
			default:
				mLogError("The stop sip call DecID not support!!!");
			    return false;
		} 
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
    cHeadCmd[1] = 3; //参数指令（SET）
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

bool CSetWebMultiCtrl::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CSetWebMultiCtrl::ComposeResult()
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

InterfaceResCode CSetWebMultiCtrl::SetWebMultiCtrlStart(string &sResult,int Dectype,int DecID, string DecUrl)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStart Dectype %d Channal %d CacheTime %d sAddr %s TcpMode %d\n", Dectype, Channal, CacheTime, sAddr.c_str(), TcpMode);
#if 1
    if(SetWebMultiStart(Dectype, DecID, DecUrl, oResult, cResult) == false){
        mLogError("Failed to run  SetWebMultiStart(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetWebMultiCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebMultiCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebMultiCtrl::SetWebMultiCtrlStop(string &sResult,int Dectype,int DecID)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 1
    if(SetWebMultiStop(Dectype, DecID, oResult, cResult) == false){
        mLogError("Failed to run  SetWebMultiStop(...)");
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
    if( _AnalyParas.ConvertSTRING2CJSONSetWebMultiCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebMultiCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebMultiCtrl::SetWebMultiCtrlRtsp(string &sResult,int DecMethod,int DecID,int DecMode,int DecFormat,string DecUrl)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
   // CData oResult = SVSMAP();
   // char cResult[4096] = {0};

    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 0
    if(SetWebMultiRtsp(DecMethod, DecID, DecMode, DecFormat, DecUrl, oResult, cResult) == false){
        mLogError("Failed to run  SetWebMultiRtsp(...)");
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

#if 0
    if( _AnalyParas.ConvertSTRING2CJSONSetWebMultiCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebMultiCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}


InterfaceResCode CSetWebMultiCtrl::SetWebMultiCtrlH323(string &sResult,int DecID,int VideoEncType,int AudioEncType,string DecUrl,string Contact,string SendVideoSrc,string SendAudioSrc)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 0
    if(SetWebMultiH323(DecID, VideoEncType, AudioEncType, DecUrl, Contact,SendVideoSrc, SendAudioSrc,oResult, cResult) == false){
        mLogError("Failed to run  SetWebMultiH323(...)");
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

#if 0
    if( _AnalyParas.ConvertSTRING2CJSONSetWebMultiCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebMultiCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebMultiCtrl::SetWebMultiCtrlSip(string &sResult,int DecMode,int DecID, string DecUrl, string UserName, string UserPw)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 0
    if(SetWebMultiSip(DecMode, DecID, DecUrl, UserName,UserPw, oResult, cResult) == false){
        mLogError("Failed to run  SetWebMultiH323(...)");
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

#if 0
    if( _AnalyParas.ConvertSTRING2CJSONSetWebMultiCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebMultiCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CSetWebMultiCtrl::SetWebMultiCtrlSipList(string &sResult,int DecType,int DecMethod,int Rate,int LenBuf,string DecUrl,string Title)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[4096] = {0};
    //printf("SetDecCtrlStop Dectype %d Channal %d\n", Dectype, Channal);
#if 0
    if(SetWebMultiSipList(DecType, DecMethod, Rate, LenBuf, DecUrl, Title, oResult, cResult) == false){
        mLogError("Failed to run  SetWebMultiH323(...)");
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

#if 0
    if( _AnalyParas.ConvertSTRING2CJSONSetWebMultiCtrl(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebMultiCtrl(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}
bool CSetWebMultiCtrl::SetWebStartDecNewInner(CData& oResult, char* cResult, int ArryNumbers, int DecType, int DecID, string DecUrl, char *cmdstr)
{
	mLogDebug("run SetWebStartDecNewInner(...)");
	//获取状态

	//add fwb 20201110
	char cChnCmd[4096] = {0};
#if 1
	if (DecType == 0)
	{
		sprintf(cChnCmd,"net_recv%d.url=%s\nnet_recv%d.cache=%d\nnet_recv%d.trans_protol=%s\n", DecID, DecUrl.c_str(), DecID, 0, DecID, ((true == true)?"tcp":"udp"));
		//sprintf(cChnCmd,"net_recv%d.url=%s\nnet_recv%d.cache=%d\nnet_recv%d.trans_protol=%s\ncomp1.win1.channel=VN1-1\nvid_enc13.channel=VN2-1\nvid_enc13.channel=VN1-1\n", DecID, DecUrl.c_str(), DecID, 0, DecID, ((true == true)?"tcp":"udp"));
	}
	else if(DecType == 1)
	{
		sprintf(cChnCmd,"h323h%d.call=%s,1\nh323h%d.buf_size=%d\n", DecID, DecUrl.c_str(), DecID, 0);  
		//sprintf(cChnCmd,"h323h%d.call=%s,1\nh323h%d.buf_size=%d\nvid_enc11.channel=VN2-1\nvid_enc11.channel=VN3-1\n", DecID, DecUrl.c_str(), DecID, 0);  
	}
	else if(DecType == 2)
	{
//		sleep(3);
		sprintf(cChnCmd,"sip%d.call=sip:1000@%s\n", DecID, DecUrl.c_str()); 
		//sprintf(cChnCmd,"sip%d.call=sip:1000@%s\ncomp1.win1.channel=VN4-1\nvid_enc12.channel=VN2-1\nvid_enc12.channel=VN4-1\n", DecID, DecUrl.c_str()); 
	}
	else if(DecType == 3)//原群呼类型，现在未使用
	{
		switch(DecID)
		{
			case 1:
				sprintf(cChnCmd,"sip%d.call=sip:1000@%s\n", DecID, DecUrl.c_str());
				//sprintf(cChnCmd,"sip%d.call=sip:1000@%s\nvid_enc%d.channel=%s\ncomp1.win1.channel=VN4-1\n", DecID, DecUrl.c_str(),12,"VN4-1");
				break;
			case 2:
				sprintf(cChnCmd,"sip%d.call=sip:1000@%s\n", DecID, DecUrl.c_str());
				//sprintf(cChnCmd,"sip%d.call=sip:1000@%s\nvid_enc%d.channel=%s\ncomp1.win2.channel=VN5-1\n", DecID, DecUrl.c_str(),11,"VN5-1");
				break;
			case 3:
				sprintf(cChnCmd,"sip%d.call=sip:1000@%s\n", DecID, DecUrl.c_str()); 
				//sprintf(cChnCmd,"sip%d.call=sip:1000@%s\nvid_enc%d.channel=%s\ncomp1.win3.channel=VN6-1\n", DecID, DecUrl.c_str(),13,"VN6-1");
				break;
			default:
				sprintf(cChnCmd,"sip%d.call=sip:1000@%s\n", DecID, DecUrl.c_str());
		} 
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
	cHeadCmd[1] = 3; //参数指令（SET）
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
bool CSetWebMultiCtrl::SetWebStartChangeInner(CData& oResult, char* cResult, char *cmdstr)
{
	mLogDebug("run SetWebStartDecNewInner(...)");
	//获取状态

	//add fwb 20201110
	char cChnCmd[4096] = {0};
#if 1

	sprintf(cChnCmd,"%s", cmdstr);


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
	cHeadCmd[1] = 3; //参数指令（SET）
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

InterfaceResCode CSetWebMultiCtrl::SetWebStartDecNew(string &sResult, int ArryNumbers, int DecType[], int DecID[], string DecUrl[])
{
	//TODO
	mLogInfo("SetWebStartDecNew...");
	
	char cResult[1024] = { 0 };
	char Result[1024] = { 0 };
	CData oResult = SVSMAP();
	char cmdstr[1024] = {0};
	char *ReqBody = NULL;
	//解码通道1-6对应VN1-1~VN6-1，解码1-6对应的源rtsp1,rtsp2,h323,sip1,sip2,sip3
	string arry[6] = {"VN1-1","VN2-1","VN3-1","VN4-1","VN5-1","VN6-1"};
	
   //1. 调用类内部方法进一步封装业务请求vid_enc10.channel=VN1-1
   for(int i= 0; i < ArryNumbers; i++)
   	{
//	   memset(cmdstr,0,128);
	   switch(DecType[i])//切换编码通道
	   {
			case 0:
				ReqBody = cmdstr + strlen(cmdstr);
				sprintf(ReqBody,"vid_enc%d.channel=%s\n", i+10, arry[DecID[i]-1].c_str());
//				sprintf(ReqBody,"vid_enc%d.channel=VN7-1\nvid_enc%d.channel=%s\n", i+10,i+10,arry[DecID[i]-1].c_str());
				break;
			case 1:
				ReqBody = cmdstr + strlen(cmdstr);
				sprintf(ReqBody,"vid_enc%d.channel=%s\n", i+10, arry[DecID[i]+1].c_str());	
//				sprintf(ReqBody,"vid_enc%d.channel=VN7-1\nvid_enc%d.channel=%s\n", i+10,i+10,arry[DecID[i]+1].c_str());	
				break;
			case 2:
				ReqBody = cmdstr + strlen(cmdstr);
				sprintf(ReqBody,"vid_enc%d.channel=%s\n",i+10, arry[DecID[i]+2].c_str());
//				sprintf(ReqBody,"vid_enc%d.channel=VN7-1\nvid_enc%d.channel=%s\n", i+10,i+10,arry[DecID[i]+2].c_str());
				break;
	   }
	   mLogInfo("SetWebStartDecNew  cmdstr:"<<cmdstr);
	   if (!SetWebStartDecNewInner(oResult, cResult, ArryNumbers, DecType[i], DecID[i], DecUrl[i], cmdstr))
	   {
			 mLogError("Failed to run  SetWebStartDecNewInner(...)");
			 return eInterfaceResCodeError;
	   }
	   
	  if ( _AnalyParas.ConvertSTRING2CJSONSetWebMultiCtrl(oResult, cResult) == false)
	  {
	        mLogError("Failed to run  ConvertSTRING2CJSONSetWebMultiCtrl(...)");
	        return eInterfaceResCodeError;
      }
	  
   	}
   sleep(5);
  
	if (!SetWebStartChangeInner(oResult, Result, cmdstr))
   {
		 mLogError("Failed to run  SetWebStartChangeInner(...)");
		 return eInterfaceResCodeError;
   }
   
   if ( _AnalyParas.ConvertSTRING2CJSONSetWebMultiCtrl(oResult, Result) == false)
   {
        mLogError("Failed to run  ConvertSTRING2CJSONSetWebMultiCtrl(...)");
        return eInterfaceResCodeError;
   }
	//将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;

}
bool CSetWebMultiCtrl::SetWebStopDecNewInner(CData& oResult, char* cResult, int ArryNumbers, int DecType, int DecID)
{
	mLogDebug("run SetWebStopDecNewInner(...)");
	//获取状态

	char cChnCmd[4096] = {0};
#if 1
	if (DecType == 0)
	{
		sprintf(cChnCmd,"net_recv%d.url=\n", DecID);
		//sprintf(cChnCmd,"net_recv%d.url=\ncomp1.win1.channel=VL1-1\nvid_enc13.channel=VN2-1\n", DecID);
	}
	else if(DecType == 1)
	{
		sprintf(cChnCmd,"h323h%d.hangup\n", DecID);
		//sprintf(cChnCmd,"h323h%d.hangup\nvid_enc11.channel=VN2-1\n", DecID);
	}
	else if(DecType == 2)
	{
		sprintf(cChnCmd,"sip%d.hangup\n", DecID);	
		//sprintf(cChnCmd,"sip%d.hangup\ncomp1.win1.channel=VL1-1\nvid_enc12.channel=VN2-1\n", DecID);
	}
	else if(DecType == 3)
	{

		sprintf(cChnCmd,"net_recv1.url=\nnet_recv2.url=\nsip1.hangup\nsip2.hangup\nsip3.hangup\nh323h1.hangup=\n");
//		sprintf(cChnCmd,INTERFACE_SETDECSTOP_SIP_CALL_SENDCMD, DecID,11,"VN2-1"); 

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
	cHeadCmd[1] = 3; //参数指令（SET）
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
InterfaceResCode CSetWebMultiCtrl::SetWebStopDecNew(string &sResult, int ArryNumbers, int DecType[], int DecID[])
{
	//TODO
	mLogInfo("SetWebStopDecNew...");

	char cResult[1024] = { 0 };
	CData oResult = SVSMAP();
	
   //1. 调用类内部方法进一步封装业务请求
//   for(int i= 0; i < ArryNumbers; i++)
//   {
//		if(DecType[i] == 0)
//		{
//			if (!SetWebStopDecNewInner(oResult, cResult, ArryNumbers, DecType[i], DecID[i]))
//			{
//				  mLogError("Failed to run	SetWebStopDecNewInner(...)");
//				  return eInterfaceResCodeError;
//			}
//			if ( _AnalyParas.ConvertSTRING2CJSONSetWebMultiCtrl(oResult, cResult) == false)
//			{
//				 mLogError("Failed to run  ConvertSTRING2CJSONSetWebMultiCtrl(...)");
//				 return eInterfaceResCodeError;
//			}
//		}
//
//   	}
   

	if (!SetWebStopDecNewInner(oResult, cResult, 0, 3, 0))
	{
		  mLogError("Failed to run	SetWebStopDecNewInner(...)");
		  return eInterfaceResCodeError;
	}
	if ( _AnalyParas.ConvertSTRING2CJSONSetWebMultiCtrl(oResult, cResult) == false)
	{
		 mLogError("Failed to run  ConvertSTRING2CJSONSetWebMultiCtrl(...)");
		 return eInterfaceResCodeError;
	}
   	
	//将step2中获取的响应格式化为Json字符串
	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;

}

