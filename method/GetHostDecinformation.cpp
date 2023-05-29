#include "GetHostDecinformation.h"
#include <boost/lexical_cast.hpp>

CGetHostDecinformation::CGetHostDecinformation(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_rtspdec(cJSON_CreateArray())
    ,_h323dec(cJSON_CreateArray())
    ,_sipdec(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CGetHostDecinformation::getDecConfig(int DecId,CData &oResult, char *cResult)
{
    mLogDebug("run getDecConfig(...)");
    //获取状态

    //add fwb 20201104
    int  i = 0;
    int  itmplen   = 0;
    char cDecTmpbuf[128] = {0};
    char cDecCmd[1024]   = {0};
	 memset(cDecCmd, 0, sizeof(1024));
	
    sprintf(cDecCmd,"net_recv%d.app_info\n", DecId);

#if 0
    for (i = 1; i < RtspNum + 1; i++)
    {
        sprintf(cDecTmpbuf,INTERFACE_GETHOSTDECRTSP_SENDCMD,i,i,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, RtspNum, cDecTmpbuf);
        memcpy(cDecCmd + itmplen, cDecTmpbuf, strlen(cDecTmpbuf));
        itmplen += strlen(cDecTmpbuf);
    }
    for (i = 1; i < H323Num + 1; i++)
    {
        sprintf(cDecTmpbuf,INTERFACE_GETHOSTDECH323_SENDCMD,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, H323Num, cDecTmpbuf);
        memcpy(cDecCmd + itmplen, cDecTmpbuf, strlen(cDecTmpbuf));
        itmplen += strlen(cDecTmpbuf);
    }
    //printf("len %d \n%s", strlen(cDecCmd), cDecCmd);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cDecCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cDecCmd) >> 8;  
    cHeadCmd[3] = strlen(cDecCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cDecCmd, strlen(cDecCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CGetHostDecinformation::getH323DecConfig(int DecId,CData &oResult, char *cResult)
{
    mLogDebug("run getDecConfig(...)");
    //获取状态

    //add fwb 20201104
    int  i = 0;
    int  itmplen   = 0;
    char cDecTmpbuf[128] = {0};
    char cDecCmd[1024]   = {0};

	 memset(cDecCmd, 0, sizeof(1024)); 
    sprintf(cDecCmd,"h323h%d.call\n", DecId);

#if 0
    for (i = 1; i < RtspNum + 1; i++)
    {
        sprintf(cDecTmpbuf,INTERFACE_GETHOSTDECRTSP_SENDCMD,i,i,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, RtspNum, cDecTmpbuf);
        memcpy(cDecCmd + itmplen, cDecTmpbuf, strlen(cDecTmpbuf));
        itmplen += strlen(cDecTmpbuf);
    }
    for (i = 1; i < H323Num + 1; i++)
    {
        sprintf(cDecTmpbuf,INTERFACE_GETHOSTDECH323_SENDCMD,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, H323Num, cDecTmpbuf);
        memcpy(cDecCmd + itmplen, cDecTmpbuf, strlen(cDecTmpbuf));
        itmplen += strlen(cDecTmpbuf);
    }
    //printf("len %d \n%s", strlen(cDecCmd), cDecCmd);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cDecCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cDecCmd) >> 8;  
    cHeadCmd[3] = strlen(cDecCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cDecCmd, strlen(cDecCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CGetHostDecinformation::getSIPDecConfig(int DecId,CData &oResult, char *cResult)
{
    mLogDebug("run getDecConfig(...)");
    //获取状态

    //add fwb 20201104
    int  i = 0;
    int  itmplen   = 0;
    char cDecTmpbuf[128] = {0};
    char cDecCmd[1024]   = {0};


    sprintf(cDecCmd,"sip%d.call\n", DecId);

#if 0
    for (i = 1; i < RtspNum + 1; i++)
    {
        sprintf(cDecTmpbuf,INTERFACE_GETHOSTDECRTSP_SENDCMD,i,i,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, RtspNum, cDecTmpbuf);
        memcpy(cDecCmd + itmplen, cDecTmpbuf, strlen(cDecTmpbuf));
        itmplen += strlen(cDecTmpbuf);
    }
    for (i = 1; i < H323Num + 1; i++)
    {
        sprintf(cDecTmpbuf,INTERFACE_GETHOSTDECH323_SENDCMD,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, H323Num, cDecTmpbuf);
        memcpy(cDecCmd + itmplen, cDecTmpbuf, strlen(cDecTmpbuf));
        itmplen += strlen(cDecTmpbuf);
    }
    //printf("len %d \n%s", strlen(cDecCmd), cDecCmd);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cDecCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cDecCmd) >> 8;  
    cHeadCmd[3] = strlen(cDecCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cDecCmd, strlen(cDecCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CGetHostDecinformation::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CGetHostDecinformation::ComposeResult()
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

InterfaceResCode CGetHostDecinformation::GetHostDecinformation(string &sResult,int DecId)
{
    //TODO
    mLogInfo("GetHostDecinformation...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
	memset(cResult, 0, 8192);
	int flag;
	string Method = "GetHostDecInfo";
    //printf("GetHostDecInfo RtspNum %d H323Num %d\n", RtspNum, H323Num);
#if 1
    if(getDecConfig(DecId, oResult, cResult) == false){
        mLogError("Failed to run  getMPSConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 

    //getDecConfig(RtspNum, H323Num, oResult, cResult);
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("GetHostDecInfo\n");
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
        if( _AnalyParas.ConvertSTRING2CJSONGetHostInfo(oResult, cResult, Method, _rtspdec, _h323dec, _sipdec, flag) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }

	//msleep(200);
	memset(cResult, 0, 8192);
	if(getH323DecConfig(DecId, oResult, cResult) == false){
        mLogError("Failed to run  getMPSConfig(...)");
        return eInterfaceResCodeError;
    }
	if( _AnalyParas.ConvertSTRING2CJSONGetHostInfo(oResult, cResult, Method, _rtspdec, _h323dec, _sipdec, flag) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }	

	if(getSIPDecConfig(DecId, oResult, cResult) == false){
        mLogError("Failed to run  getMPSConfig(...)");
        return eInterfaceResCodeError;
    }
    if( _AnalyParas.ConvertSTRING2CJSONGetHostInfo(oResult, cResult, Method, _rtspdec, _h323dec, _sipdec, flag) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostDecInfo(...)");
        return eInterfaceResCodeError;
    }	
	    //result中添加节点
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_DE_RTSPDECNUM,_rtspdec);
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_DE_H323DECNUM,_h323dec);
	cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_DE_SIPDECNUM,_sipdec);
    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}
bool CGetHostDecinformation::GetWebDecInfoInner(CData &oResult, char *cResult)
{
    mLogDebug("run GetWebDecInfoInner(...)");
    //获取状态

    int  i = 0;
    int  itmplen   = 0;
    char cDecTmpbuf[128] = {0};
    char cDecCmd[1024]   = {0};

    sprintf(cDecCmd,"yuan1.info\nyuan2.info\nyuan3.info\n");

#if 0
    for (i = 1; i < RtspNum + 1; i++)
    {
        sprintf(cDecTmpbuf,INTERFACE_GETHOSTDECRTSP_SENDCMD,i,i,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, RtspNum, cDecTmpbuf);
        memcpy(cDecCmd + itmplen, cDecTmpbuf, strlen(cDecTmpbuf));
        itmplen += strlen(cDecTmpbuf);
    }
    for (i = 1; i < H323Num + 1; i++)
    {
        sprintf(cDecTmpbuf,INTERFACE_GETHOSTDECH323_SENDCMD,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, H323Num, cDecTmpbuf);
        memcpy(cDecCmd + itmplen, cDecTmpbuf, strlen(cDecTmpbuf));
        itmplen += strlen(cDecTmpbuf);
    }
    //printf("len %d \n%s", strlen(cDecCmd), cDecCmd);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cDecCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cDecCmd) >> 8;  
    cHeadCmd[3] = strlen(cDecCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cDecCmd, strlen(cDecCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}
InterfaceResCode CGetHostDecinformation::GetWebDecInfo(string &sResult)
{
    //TODO
    mLogInfo("GetWebDecInfo...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
	int flag;
	memset(cResult, 0, 8192);
	cJSON* temList[15];
	string Method = "GetWebDecInfo";
	string IP;
    //printf("GetHostDecInfo RtspNum %d H323Num %d\n", RtspNum, H323Num);
#if 1
	for(int i = 1; i <= 4; i++)
	{ //用于生成节点，存储解析后的参数
		temList[i] = cJSON_CreateObject();
	}


    if(GetWebDecInfoInner(oResult, cResult) == false){
        mLogError("Failed to run  GetWebDecInfoInner(...)");
        return eInterfaceResCodeError;
    }
#endif 

    //getDecConfig(RtspNum, H323Num, oResult, cResult);
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("GetHostDecInfo\n");
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
    if( _AnalyParas.ConvertSTRING2CJSONGetWebParam(oResult, cResult, _result, _rtspdec, temList, Method, IP, 0)== false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetWebParam(...)");
        return eInterfaceResCodeError;
    }
	for(int i = 1; i <= 3; i++)
	{
		cJSON_AddItemToArray(_rtspdec, temList[i]);	
    }
	cJSON_AddItemToObject(_result,"CallList",_rtspdec);
	sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}


