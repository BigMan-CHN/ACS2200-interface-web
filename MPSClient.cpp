#include "MPSClient.h"

MPSClient::MPSClient(string ip, int port, int timeOut)
    :_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
{
    mLogDebug("ip:"<<this->_ip<<",port:"<<this->_port<<",timeOut:"<<this->_timeOut);
}

ResponseCode MPSClient::GetConfigNew(const char *cCmdSend,int cCmdLength, MPSOperationRes &opResCode, CData &oResult, char *cRecult)
{
    mLogInfo("GetConfig...");
    CData oParams = SVSMAP();
    ResponseCode res = eResponseCodeSuccess;

     //执行接口
 //    CData oResult = SVSMAP();
     res = socketSendNew(MODULE_CONFIG,CONFIG_METHOD_GETCONFIG,oParams,cCmdSend,cCmdLength,cRecult);

     //判断返回信息是否为执行成功
     return res;
}

ResponseCode MPSClient::GetConsoleNew(const char *cCmdSend,int cCmdLength, MPSOperationRes &opResCode, CData &oResult, char *cRecult)
{
    mLogInfo("GetConsoleNew...");
    CData oParams = SVSMAP();
    ResponseCode res = eResponseCodeSuccess;

     //执行接口
 //    CData oResult = SVSMAP();
     res = socketSendConsoleCmd(MODULE_CONFIG,CONFIG_METHOD_GETCONFIG,oParams,cCmdSend,cCmdLength,cRecult);

     //判断返回信息是否为执行成功
     return res;
}

ResponseCode MPSClient::GetSerialNew(int SerialID, const char *cCmdSend,int cCmdLength, MPSOperationRes &opResCode, CData &oResult, char *cRecult)
{
    mLogInfo("GetSerialNew...");
    CData oParams = SVSMAP();
    ResponseCode res = eResponseCodeSuccess;

     //执行接口
 //    CData oResult = SVSMAP();
     res = socketSendSerialCmd(MODULE_CONFIG,CONFIG_METHOD_GETCONFIG,oParams, SerialID, cCmdSend,cCmdLength,cRecult);

     //判断返回信息是否为执行成功
     return res;
}

ResponseCode MPSClient::GetConfig(MPSOperationRes &opResCode, CData &oResult)
{
    mLogInfo("GetConfig...");
     CData oParams = SVSMAP();

     //执行接口
 //    CData oResult = SVSMAP();
     oResult = socketSend(MODULE_CONFIG,CONFIG_METHOD_GETCONFIG,oParams);

     //判断返回信息是否为执行成功
     return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::GetSysConfig(MPSOperationRes &opResCode, CData &oResult)
{
    mLogInfo("GetSysConfig...");
    CData oParams = SVSMAP();

    //执行接口
//    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_CONFIG,CONFIG_METHOD_GETSYSCONFIG,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::GetMediaState(MPSOperationRes &opResCode, MPSMediaStatus &state)
{
    mLogInfo("GetMediaState...");
    CData oParams = SVSMAP();
    if(!getMediaGetStateParams(oParams)){
        mLogError("Failed to run getMediaGetStateParams(...)");
        return eResponseCodeErrorParam;
    }

    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_GETSTATUS,oParams);

    //判断返回信息是否为执行成功
    ResponseCode resCode = eResponseCodeErrorRecv;
    if(!isSuccessResponseCode(oResult,resCode,opResCode)){
        mLogError("Failed to run isSuccessResponseCode(...)");
        return resCode;
    }

    //判断是否执行成功
    if(resCode != eResponseCodeSuccess){
        mLogError("responseCode:"<<resCode);
        mLogError("responseError:"<<GetResponseError(resCode));
        return resCode;
    }

    //执行成功,获取返回内容的状态节点
    int mediaState;
    try{
        mediaState = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()[MEDIA_GETSTATUS_RES_STATUSMPS].RefLong16();
        mLogDebug("oResult node:"<<MEDIA_GETSTATUS_RES_STATUSMPS<<" val:"<<mediaState);
    }catch(exception & e){
        mLogError("Faild to try get oResult node:"<<SKYVIS_TRANSMIT_RESPONSE<<"-"<<MEDIA_GETSTATUS_RES_STATUSMPS);
        return eResponseCodeErrorRecv;
    }

    //转换状态值
    if(mediaState >= eMPSStatusSize){
        mLogError("mediaState:"<<mediaState<<" >= eMPSStatusSize:"<<eMPSStatusSize);
        return eResponseCodeErrorRecv;
    }

    state = (MPSMediaStatus)mediaState;
    mLogInfo("GetMediaState success");
    return eResponseCodeSuccess;
}

ResponseCode MPSClient::MediaPrepare(int chNum,string caseId,MPSOperationRes &opResCode , string &resKeyID)
{
    mLogInfo("MediaPrepare...");
    mLogInfo("chNum:"<<chNum<<",caseId:"<<caseId);
    CData oParams = SVSMAP();
    if(!getMediaPrepareParams(chNum,caseId,oParams)){
        mLogError("Failed to run getMediaPrepareParams(chNum,caseId,...)=("<<chNum<<","<<caseId<<", ...)");
        return eResponseCodeErrorParam;
    }

    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_PREPARE,oParams);

    //判断返回信息是否为执行成功
    ResponseCode resCode = eResponseCodeErrorRecv;
    if(!isSuccessResponseCode(oResult,resCode,opResCode)){
        mLogError("Failed to run isSuccessResponseCode(...)");
        return resCode;
    }

    //判断是否执行成功
    if(resCode != eResponseCodeSuccess){
        mLogError("responseCode:"<<resCode);
        mLogError("responseError:"<<GetResponseError(resCode));
        return resCode;
    }

    //执行成功,获取返回内容的状态节点
    try{
        resKeyID = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()[MEDIA_PREPARE_RES_KEYID].RefString();
        mLogDebug("oResult node:"<<MEDIA_PREPARE_RES_KEYID<<" val:"<<resKeyID);
    }catch(exception & e){
        mLogError("Faild to try get oResult node:"<<SKYVIS_TRANSMIT_RESPONSE<<"-"<<MEDIA_PREPARE_RES_KEYID);
        return eResponseCodeErrorRecv;
    }

    //
    if(caseId.length() != 0){
        if(strcmp(resKeyID.c_str(),caseId.c_str()) != 0){
            mLogError("resKeyID:"<<resKeyID<<" != caseId:"<<caseId);
            return eResponseCodeErrorParam;
        }
    }
    mLogInfo("MediaPrepare success");
    return eResponseCodeSuccess;
}

ResponseCode MPSClient::MediaStart(int chNum, string caseId, MPSOperationRes &opResCode )
{
    mLogInfo("MediaStart...");
    mLogInfo("chNum:"<<chNum<<",caseId:"<<caseId);
    CData oParams = SVSMAP();
    if(!getMediaStartParams(chNum,caseId,oParams)){
        mLogError("Failed to run getMediaStartParams(chNum,caseId,...)=("<<chNum<<","<<caseId<<", ...)");
        return eResponseCodeErrorParam;
    }

    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_START,oParams);

    //判断返回信息是否为执行成功
    ResponseCode resCode = eResponseCodeErrorRecv;
    if(!isSuccessResponseCode(oResult,resCode,opResCode)){
        mLogError("Failed to run isSuccessResponseCode(...)");
        return resCode;
    }

    //判断是否执行成功
    if(resCode != eResponseCodeSuccess){
        mLogError("responseCode:"<<resCode);
        mLogError("responseError:"<<GetResponseError(resCode));
        return resCode;
    }

    mLogInfo("MediaStart success");
    return eResponseCodeSuccess;
}

ResponseCode MPSClient::StartProofSave(MPSOperationRes &opResCode)
{
    mLogInfo("StartProofSave...");
    CData oParams = SVSMAP();

    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_START_PROOF_SAVE,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::MediaStop(int chNum, MPSOperationRes &opResCode)
{
    return MediaStop(chNum,false,opResCode);
}

ResponseCode MPSClient::MediaStop(int chNum, bool closeFlag, MPSOperationRes &opResCode )
{
    mLogInfo("MediaStop...");
    mLogInfo("chNum"<<chNum<<",closeFlag:"<<closeFlag);
    CData oParams = SVSMAP();
    if(!getMediaStopParams(chNum,closeFlag,oParams)){
        mLogError("Failed to run getMediaStopParams(chNum,closeFlag,...)=("<<chNum<<","<<closeFlag<<", ...)");
        return eResponseCodeErrorParam;
    }

    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_STOP,oParams);

    //判断返回信息是否为执行成功
    ResponseCode resCode = eResponseCodeErrorRecv;
    if(!isSuccessResponseCode(oResult,resCode,opResCode)){
        mLogError("Failed to run isSuccessResponseCode(...)");
        return resCode;
    }

    //判断是否执行成功
    if(resCode != eResponseCodeSuccess){
        mLogError("responseCode:"<<resCode);
        mLogError("responseError:"<<GetResponseError(resCode));
        return resCode;
    }

    mLogInfo("MediaStop success");
    return eResponseCodeSuccess;
}

ResponseCode MPSClient::StopProofSave(MPSOperationRes &opResCode)
{
    mLogInfo("StopProofSave...");
    CData oParams = SVSMAP();

    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_STOP_PROOF_SAVE,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::MediaClose(int chNum, MPSOperationRes &opResCode )
{
    mLogInfo("MediaClose...");
    mLogInfo("chNum:"<<chNum);
    CData oParams = SVSMAP();
    if(!getMediaCloseParams(chNum,oParams)){
        mLogError("Failed to run getMediaCloseParams(...)");
        return eResponseCodeErrorParam;
    }

    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_CLOSE,oParams);

    //判断返回信息是否为执行成功
    ResponseCode resCode = eResponseCodeErrorRecv;
    if(!isSuccessResponseCode(oResult,resCode,opResCode )){
        mLogError("Failed to run isSuccessResponseCode(...)");
        return resCode;
    }

    //判断是否执行成功
    if(resCode != eResponseCodeSuccess){
        mLogError("responseCode:"<<resCode);
        mLogError("responseError:"<<GetResponseError(resCode));
        return resCode;
    }

    mLogInfo("MediaClose success");
    return eResponseCodeSuccess;
}

ResponseCode MPSClient::MediaSetPoint(int chNum, MPSOperationRes &opResCode, CData &oResult)
{
    mLogInfo("MediaSetPoint...");
    CData oParams = SVSMAP();
    if(!getMediaSetPointParams(chNum,oParams)){
        mLogError("Failed to run getMediaSetPointParams(...)");
        return eResponseCodeErrorParam;
    }

    //执行接口
//    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_SET_POINT,oParams);

    //判断返回信息是否为执行成功
    ResponseCode resCode = eResponseCodeErrorRecv;
    if(!isSuccessResponseCode(oResult,resCode,opResCode )){
        mLogError("Failed to run isSuccessResponseCode(...)");
        return resCode;
    }

    //判断是否执行成功
    if(resCode != eResponseCodeSuccess){
        mLogError("responseCode:"<<resCode);
        mLogError("responseError:"<<GetResponseError(resCode));
        return resCode;
    }

    mLogInfo("MediaSetPoint success");
    return eResponseCodeSuccess;
}

ResponseCode MPSClient::MediaSetPoint(int chNum, MPSOperationRes &opResCode, long &duration, string &fileName)
{
    CData oResult = SVSMAP();
    ResponseCode resCode = MediaSetPoint(chNum,opResCode,oResult);
    if(resCode != eResponseCodeSuccess){
        return resCode;
    }

    //执行成功,获取返回内容的状态节点
    try{
        duration = oResult.RefMap()[MEDIA_SET_POINT_RES_DURATION].RefLong32();
        mLogDebug("oResult node:"<<MEDIA_SET_POINT_RES_DURATION<<" val:"<<duration);
        fileName = oResult.RefMap()[MEDIA_SET_POINT_RES_FILEPATH].RefString();
        mLogDebug("oResult node:"<<MEDIA_SET_POINT_RES_FILEPATH<<" val:"<<fileName);
    }catch(exception & e){
        try{
            duration = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()[MEDIA_SET_POINT_RES_DURATION].RefLong32();
            mLogDebug("oResult node:"<<MEDIA_SET_POINT_RES_DURATION<<" val:"<<duration);
            fileName = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()[MEDIA_SET_POINT_RES_FILEPATH].RefString();
            mLogDebug("oResult node:"<<MEDIA_SET_POINT_RES_FILEPATH<<" val:"<<fileName);
        }catch(exception & e){
            mLogError("Faild to try get oResult node:"<<SKYVIS_TRANSMIT_RESPONSE<<"-"<<MEDIA_SET_POINT_RES_DURATION);
            mLogError("Faild to try get oResult node:"<<SKYVIS_TRANSMIT_RESPONSE<<"-"<<MEDIA_SET_POINT_RES_FILEPATH);
            return eResponseCodeErrorRecv;
        }
    }
    return eResponseCodeSuccess;
}

ResponseCode MPSClient::GetPackageFile(string caseId, MPSOperationRes &opResCode,CData &oResult)
{

    mLogInfo("GetPackageFile...");
    mLogInfo("caseId:"<<caseId);
    CData oParams = SVSMAP();
    if(!getGetPackageParams(caseId,oParams)){
        mLogError("Failed to run getGetPackageParams(caseId,...)=("<<caseId<<", ...)");
        return eResponseCodeErrorParam;
    }

    //执行接口
//    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_PACKAGE,PACKAGE_METHOD_GET_PACKAGE,oParams);

    //判断返回信息是否为执行成功
    ResponseCode resCode = eResponseCodeErrorRecv;
    if(!isSuccessResponseCode(oResult,resCode,opResCode )){
        mLogError("Failed to run isSuccessResponseCode(...)");
        return resCode;
    }

    //判断是否执行成功
    if(resCode != eResponseCodeSuccess){
        mLogError("responseCode:"<<resCode);
        mLogError("responseError:"<<GetResponseError(resCode));
        return resCode;
    }

    mLogInfo("GetPackageFile success");
    return eResponseCodeSuccess;
}

ResponseCode MPSClient::GetPackageList(MPSOperationRes &opResCode, CData &oResult)
{
    mLogInfo("GetPackageList...");
    CData oParams = SVSMAP();

    //执行接口
//    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_PACKAGE,PACKAGE_METHOD_GET_PACKAGE_LIST,oParams);

    //判断返回信息是否为执行成功
    ResponseCode resCode = eResponseCodeErrorRecv;
    if(!isSuccessResponseCode(oResult,resCode,opResCode )){
        mLogError("Failed to run isSuccessResponseCode(...)");
        return resCode;
    }

    //判断是否执行成功
    if(resCode != eResponseCodeSuccess){
        mLogError("responseCode:"<<resCode);
        mLogError("responseError:"<<GetResponseError(resCode));
        return resCode;
    }

    mLogInfo("GetPackageList success");
    return eResponseCodeSuccess;
}

ResponseCode MPSClient::UpdatePackageFlag(string caseId, MPSOperationRes &opResCode)
{
    mLogInfo("UpdatePackageFlag...");
    mLogInfo("caseId:"<<caseId);
    CData oParams = SVSMAP();
    if(!getUpdatePackageFlagParams(caseId,oParams)){
        mLogError("Failed to run getUpdatePackageFlagParams(caseId,...)=("<<caseId<<", ...)");
        return eResponseCodeErrorParam;
    }
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_PACKAGE,PACKAGE_METHOD_UPDATE_PACKAGE_FLAG,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::ControlSendCmd(string cmd, MPSOperationRes &opResCode, int deviceID)
{
    mLogInfo("Control SendCmd...");
    mLogInfo("cmd:"<<cmd<<",deviceID:"<<deviceID);
    CData oParams = SVSMAP();
    if(!getControlSendCmdParams(cmd,oParams,deviceID)){
        mLogError("Failed to run getControlSendCmdParams(cmd,...,deviceID)=("<<cmd<<",...,"<<deviceID<<")");
        return eResponseCodeErrorParam;
    }
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_CONTROL,CONTROL_METHOD_SENDCMD,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::ConfigSetInterRtsp(int chNum, string url, bool enable, MPSOperationRes &opResCode)
{
    mLogInfo("Config SetInterRtsp Cmd...");
    mLogInfo("chNum:"<<chNum<<",url:"<<url<<",enable"<<enable);
    CData oParams = SVSMAP();
    if(!getConfigSetInterRtspParams(chNum,url,enable,oParams)){
        mLogError("Failed to run getConfigSetInterRtspParams(chNum,url,enable,...)=("<<chNum<<","<<url<<","<<enable<<",...)");
        return eResponseCodeErrorParam;
    }
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_CONFIG,CONFIG_METHOD_SET_INTER_RTSP,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::MediaStartInteRtsp(int chNum, MPSOperationRes &opResCode)
{
    mLogInfo("Meida StartInteRtps Cmd...");
    mLogInfo("chNum:"<<chNum);
    CData oParams = SVSMAP();
    if(!getMediaStartInteRtspParams(chNum,oParams)){
        mLogError("Failed to run getMediaStartInteRtspParams(chNum,...)=("<<chNum<<",...)");
        return eResponseCodeErrorParam;
    }
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_START_INTE_RTSP,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::MediaStopInteRtsp(int chNum, MPSOperationRes &opResCode)
{
    mLogInfo("Meida StopInteRtps Cmd...");
    mLogInfo("chNum:"<<chNum);
    CData oParams = SVSMAP();
    if(!getMediaStopInteRtspParams(chNum,oParams)){
        mLogError("Failed to run getMediaStopInteRtspParams(chNum,...)=("<<chNum<<",...)");
        return eResponseCodeErrorParam;
    }
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_STOP_INTE_RTSP,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::MediaStartDecode(int chNum, string url, int cacheTime, bool tcpMode, MPSOperationRes &opResCode)
{
    mLogInfo("Meida StartInterRtsp_Api Cmd...");
    mLogInfo("chNum:"<<chNum<<",url:"<<url<<",cacheTime:"<<cacheTime<<",tcpMode:"<<tcpMode);
    CData oParams = SVSMAP();
    if(!getMediaStartDecodeParams(chNum,url,cacheTime,tcpMode,oParams)){
        mLogError("Failed to run getMediaStartDecodeParams(chNum,url,cacheTime,tcpMode...)=("<<url<<","<<cacheTime<<","<<tcpMode<<",...)");
        return eResponseCodeErrorParam;
    }
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_START_INTER_RTSP_API,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::MediaStopDecode(int chNum, MPSOperationRes &opResCode)
{
    mLogInfo("Meida StopInterRtsp_Api Cmd...");
    mLogInfo("chNum:"<<chNum);
    CData oParams = SVSMAP();
    if(!getMediaStopDecodeParams(chNum,oParams)){
        mLogError("Failed to run getMediaStopDecodeParams(chNum,...)=("<<chNum<<",...)");
        return eResponseCodeErrorParam;
    }
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_STOP_INTER_RTSP_API,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::MediaGetDecodeStatus(int chNum, MPSMediaDecodeStatus &decodeStatus, MPSOperationRes &opResCode)
{
    mLogInfo("Meida GetInteRtps_Api Cmd...");
    mLogInfo("chNum:"<<chNum);
    CData oParams = SVSMAP();
    if(!getMediaGetDecodeStatusParams(chNum,oParams)){
        mLogError("Failed to run getMediaGetDecodeStatusParams(chNum,...)=("<<chNum<<",...)");
        return eResponseCodeErrorParam;
    }
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_GET_INTER_RTSP_API,oParams);

    //判断返回信息是否为执行成功
    ResponseCode resCode = checkReturnResponseCode(opResCode,oResult);
    if(resCode != eResponseCodeSuccess){
        return resCode;
    }
    //执行成功,获取返回内容的状态节点
    int decState;
    try{
        decState = oResult.RefMap()[MEDIA_GET_INTER_RTSP_API_RES_STATUS].RefLong16();
        mLogDebug("oResult node:"<<MEDIA_GET_INTER_RTSP_API_RES_STATUS<<" val:"<<decState);
    }catch(exception & e){
        try{
            decState = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()[MEDIA_GET_INTER_RTSP_API_RES_STATUS].RefLong16();
            mLogDebug("oResult node:"<<MEDIA_GET_INTER_RTSP_API_RES_STATUS<<" val:"<<decState);
        }catch(exception & e){
            mLogError("Faild to try get oResult node:"<<SKYVIS_TRANSMIT_RESPONSE<<"-"<<MEDIA_GET_INTER_RTSP_API_RES_STATUS);
            return eResponseCodeErrorRecv;
        }
    }
    decodeStatus = (MPSMediaDecodeStatus)decState;
    //转换状态值
    if(decodeStatus >= eMPSDecodetatusSize){
        mLogError("decodeStatus:"<<decodeStatus<<" >= eMPSDecodetatusSize:"<<eMPSDecodetatusSize);
        return eResponseCodeErrorRecv;
    }
    return eResponseCodeSuccess;
}

ResponseCode MPSClient::MediaStartDiscipline(MPSOperationRes &opResCode)
{
    mLogInfo("Meida StartDiscipline Cmd...");
    CData oParams = SVSMAP();
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_START_DISCIPLINE,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::MediaStopDiscipline(MPSOperationRes &opResCode)
{
    mLogInfo("Meida StopDiscipline Cmd...");
    CData oParams = SVSMAP();
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_STOP_DISCIPLINE,oParams);

    //判断返回信息是否为执行成功
    return checkReturnResponseCode(opResCode,oResult);
}

ResponseCode MPSClient::MediaGetDisciplineStatus(MPSMediaDisciplineStatus &disciplineStatus, MPSOperationRes &opResCode)
{
    mLogInfo("Meida GetStatusDiscipline Cmd...");
    CData oParams = SVSMAP();
    //执行接口
    CData oResult = SVSMAP();
    oResult = socketSend(MODULE_MEDIA,MEDIA_METHOD_GET_STATUS_DISCIPLINE,oParams);

    //判断返回信息是否为执行成功
    ResponseCode resCode = checkReturnResponseCode(opResCode,oResult);
    if(resCode != eResponseCodeSuccess){
        return resCode;
    }
    //执行成功,获取返回内容的状态节点
    int dcpState;
    try{
        dcpState = oResult.RefMap()[MEDIA_GET_STATUS_DISCIPLINE_RES_STATUS].RefLong16();
        mLogDebug("oResult node:"<<MEDIA_GET_STATUS_DISCIPLINE_RES_STATUS<<" val:"<<dcpState);
    }catch(exception & e){
        try{
            dcpState = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()[MEDIA_GET_STATUS_DISCIPLINE_RES_STATUS].RefLong16();
            mLogDebug("oResult node:"<<MEDIA_GET_STATUS_DISCIPLINE_RES_STATUS<<" val:"<<dcpState);
        }catch(exception & e){
            mLogError("Faild to try get oResult node:"<<SKYVIS_TRANSMIT_RESPONSE<<"-"<<MEDIA_GET_STATUS_DISCIPLINE_RES_STATUS);
            return eResponseCodeErrorRecv;
        }
    }
    disciplineStatus = (MPSMediaDisciplineStatus)dcpState;
    //转换状态值
    if(disciplineStatus >= eMPSDisciplinetatusSize){
        mLogError("disciplineStatus:"<<disciplineStatus<<" >= eMPSDisciplinetatusSize:"<<eMPSDisciplinetatusSize);
        return eResponseCodeErrorRecv;
    }
    return eResponseCodeSuccess;
}
/*
ResponseCode MPSClient::socketRecvSerialCmd(string sModule, string sMethod, CData oParams, int SerialID, const char *cCmdRecv, int cCmdLength, char *cRecult)
{
    CTransmitClient client;
    ResponseCode res = eResponseCodeSuccess;
    CData oResult = SVSMAP();
    CData oReturn = SVSMAP();

    //20201123 fwb
    int  sockfd;
    //char recvline[4096], sendline[4096];
    struct sockaddr_in  servaddr;
    int servaddrlen = 0, sendnum = 0;

    //建立udp socket   
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  
    if(sockfd < 0)  
    {  
        printf("socket");  
        return eResponseCodeErrorLocalDataConvert;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddrlen = sizeof(servaddr);
    if (SerialID == 0)
    {
        servaddr.sin_port = htons(6868);
    }
    else if (SerialID == 1)
    {
        servaddr.sin_port = htons(6869);
    }
    else if (SerialID == 2)
    {
        servaddr.sin_port = htons(6870);
    }
    else if (SerialID == 3)
    {
        servaddr.sin_port = htons(6871);
    }
    //printf("len %d %s port %d\n", cCmdLength, cCmdSend, servaddr.sin_port);
    //sendnum = sendto(sockfd, cCmdSend, cCmdLength, 0, (struct sockaddr *)&servaddr, servaddrlen);  
	sendnum = recvfrom(sockfd, cCmdRecv, sizeof(cCmdRecv), 0, (struct sockaddr *)&servaddr, servaddrlen); 
    if(sendnum < 0)  
    {  
        printf("sendto error:");  
        return eResponseCodeErrorSend;
    } 
    close(sockfd);  
    return res;
}
*/
ResponseCode MPSClient::socketSendSerialCmd(string sModule, string sMethod, CData oParams, int SerialID, const char *cCmdSend, int cCmdLength, char *cRecult)
{
    CTransmitClient client;
    ResponseCode res = eResponseCodeSuccess;
    CData oResult = SVSMAP();
    CData oReturn = SVSMAP();

    //20201123 fwb
    int  sockfd;
    //char recvline[4096], sendline[4096];
    struct sockaddr_in  servaddr;
    int servaddrlen = 0, sendnum = 0, recvnum = 0;

    /* 建立udp socket */  
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  
    if(sockfd < 0)  
    {  
        mLogError(" creat socket err");  
        return eResponseCodeErrorLocalDataConvert;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddrlen = sizeof(servaddr);
    if (SerialID == 0)
    {
        servaddr.sin_port = htons(6868);
    }
    else if (SerialID == 1)
    {
        servaddr.sin_port = htons(6869);
    }
    else if (SerialID == 2)
    {
        servaddr.sin_port = htons(6870);
    }
    else if (SerialID == 3)
    {
        servaddr.sin_port = htons(6871);
    }
	else if (SerialID == 4)
    {
        servaddr.sin_port = htons(6872);
    }
    //printf("len %d %s port %d\n", cCmdLength, cCmdSend, servaddr.sin_port);
    sendnum = sendto(sockfd, cCmdSend, cCmdLength, 0, (struct sockaddr *)&servaddr, servaddrlen); 
	
	if(sendnum < 0)  
    {  
        mLogError("socketSendSerialCmd ：sendto error:");  
        return eResponseCodeErrorSend;
    } 
/*	
	fd_set fdset,fdset_tmp;
	int max_fd;	
	memset(cRecult,0,RES_BUF_MAXLEN);
	FD_ZERO(&fdset);
	FD_SET(sockfd,&fdset);
	max_fd = sockfd;
	fdset_tmp = fdset;
	//flags = fcntl(fd, F_GETFL, 0);
	rt = fcntl(sockfd, F_SETFL, O_NONBLOCK);//最好先使用flags = fcntl(fd, F_GETFL, 0);
	if (-1 == rt){
		perror("fcntl");
	}
	ret = select(max_fd+1,&fdset_tmp,NULL,NULL,NULL);
	//LogInfo(DEBUG, "##########select	set,ret:[%d]\n",ret);
	if(ret <= 0){
		LogInfo(ERROR,"select is error or timeout\n");
		continue;
	}
	
	if( !FD_ISSET(pipe_set_fd_tmp,&fdset_tmp) ){
		LogInfo(ERROR,"select ISSET is error\n");
		continue;
	}
*/
	
	struct timeval tv;
	int ret;
	tv.tv_sec = 20;
	tv.tv_usec = 0;
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0)
	{
		mLogError("socket option  SO_RCVTIMEO not support\n");
		//printf("socket option  SO_RCVTIMEO not support\n");
		return eResponseCodeErrorSend;
	}
	recvnum = recvfrom(sockfd, cRecult, RES_BUF_MAXLEN, 0, (struct sockaddr *)&servaddr, (socklen_t*)&servaddrlen); 
	//printf("recvnum = %d,cRecult=%s\n",recvnum,cRecult);	
    if(recvnum < 0)  
    {  
		//printf("recvfrom error:");
		mLogError("socketSendSerialCmd：Timeout， recvfrom error:SerialID="<<SerialID);  
		close(sockfd);
        return eResponseCodeErrorRecvTimeOut;
    }
    close(sockfd);  
    return res;
}


ResponseCode MPSClient::socketSendConsoleCmd(string sModule, string sMethod, CData oParams, const char *cCmdSend, int cCmdLength, char *cRecult)
{
    CTransmitClient client;
    ResponseCode res = eResponseCodeSuccess;
    CData oResult = SVSMAP();
    CData oReturn = SVSMAP();
    mLogDebug("Module:"<<sModule<<",Method:"<<sMethod);

#if 0
    //add fwb 20201123
    //char *cCmd = (char *)malloc(1024);
    //char cCmdRecv[1024] = {0};
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 
    //memset(cCmdRecv, 0, sizeof(1024)); 
    cCmdLength = strlen(cTmpCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cTmpCmd) >> 8;  
    cHeadCmd[3] = strlen(cTmpCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cTmpCmd, strlen(cTmpCmd));
#endif 

#if 0
    for (int i = 0; i < cCmdLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cCmdSend[i]);
	    }
	    else 
	    {
		    printf("%c", cCmdSend[i]);
	    }
    }
#endif 

    //打印参数
    try{
        string sParams;
        if(!CData::ToCustomXMLString(oParams.RefMap(),sParams)){
            mLogError("Convert xml data to xml string failed.");
        }
        mLogDebug("params:\r\n"<<sParams);
    }catch(exception& e){
        mLogDebug("ToCustomXMLString(oParams.RefMap(),...) error."<<e.what());
    }

#if 1
    do{
        //连接主机
        if( client.Connect(this->_ip,8060) == false ){
            client.Close();     // 断开连接
            mLogError("Failed to connect.ip:"<<this->_ip<<",port:"<<this->_port);
            oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = SVSLONG32(eResponseCodeNoConnected);
            break;
        }
        // 向主机发送信息并接受主机返回信息
        client.Execute(sModule,sMethod,oParams,cCmdSend,cCmdLength,oResult,cRecult,1000) ;
        res = eResponseCodeSuccess ;
#if 0
	int iRecvLength = (cRecult[2] << 8) + cRecult[3] + 4;
        printf("Execute Recv\n");
	for (int i = 0; i < iRecvLength; i++)
	{
		if (i < 4)
		{
			printf("%d", cRecult[i]);
		}
		else 
		{
			printf("%c", cRecult[i]);
		}
	}
#endif 

        if( res!= eResponseCodeSuccess ){
            client.Close();     // 断开连接
            mLogError("Failed to Execute(module,method,params,result,timeOut)=("
                      <<sModule<<","
                      <<sMethod<<",oParams,oResult,"
                      <<this->_timeOut<<",),ResponseCode:"
                      <<res);
            oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)(res);
            break;
        }
    }while(false);
    // 断开连接
    client.Close();
#else
    if (SVSFAILED(CData::FromFile(oResult, "res2.xml"))){
        mLogError("Load config from res.xml failed.");
    }


    string sResult;
    if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
        mLogError("Convert xml data to xml string failed.");
    }
    mLogDebug("oResult:\r\n"<<sResult);

    //TODO 添加节点失败
    bool findNodeResponseCodeFlag = false;
    bool findNodeResponseFlag = false;

    //查找节点 SKYVIS_TRANSMIT_RESPONSE
    try{
        SVSMAP::iterator find = oResult.RefMap().find(SKYVIS_TRANSMIT_RESPONSE);
        if (find == oResult.RefMap().end()){
            mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE);
            findNodeResponseFlag = false;
        }else{
            mLogDebug("oResult 有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE);
            findNodeResponseFlag = true;
        }
    }catch(exception &e){
        mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE<<", exception.what:"<<e.what());
        findNodeResponseFlag = false;
    }

    //查找节点 SKYVIS_TRANSMIT_TRANSMITCODE
    try{
        SVSMAP::iterator find = oResult.RefMap().find(SKYVIS_TRANSMIT_TRANSMITCODE);
        if (find == oResult.RefMap().end()){
            mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE);
            findNodeResponseCodeFlag = false;
        }else{
            mLogDebug("oResult 有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE);
            findNodeResponseCodeFlag = true;
        }
    }catch(exception &e){
        mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<", exception.what:"<<e.what());
        findNodeResponseCodeFlag = false;
    }

    if( !findNodeResponseFlag && !findNodeResponseCodeFlag){
        //两个节点都不存在
        try{
            string sResult;
            CData oRes = SVSMAP();
//            if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
//                mLogError("Convert xml data to xml string failed.");
//            }
//            mLogDebug("oResult:\r\n"<<sResult);

            oRes.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)(res);
            oRes.RefMap()[SKYVIS_TRANSMIT_RESPONSE] = (SVSMAP)(oResult.RefMap());

//            if(!CData::ToCustomXMLString(oRes.RefMap(),sResult)){
//                mLogError("Convert xml data to xml string failed.");
//            }
//            mLogDebug("oRes:\r\n"<<sResult);

            oResult.Release();
            oResult = oRes;
//            if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
//                mLogError("Convert xml data to xml string failed.");
//            }
//            mLogDebug("oResult:\r\n"<<sResult);
        }catch(exception &e){
            mLogWarn("oResult 添加节点失败 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<" node:"<<SKYVIS_TRANSMIT_RESPONSE<<", exception.what:"<<e.what());
        }
    }else{
        if(findNodeResponseFlag && !findNodeResponseCodeFlag){
            try{
                oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)(eResponseCodeSuccess);
            }catch(exception &e){
                mLogWarn("oResult 添加节点失败 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<", exception.what:"<<e.what());
            }
        }
    }

    //打印返回信息
    try{
        string sResult;
        if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
            mLogDebug("Convert xml data to xml string failed.");
        }
        mLogDebug("处理(添加节点)后 sResult:\r\n"<<sResult);
    }catch(exception& e){
        mLogDebug("ToCustomXMLString(oResult.RefMap(),...) error."<<e.what());
    }
#endif

    return res;
}


ResponseCode MPSClient::socketSendNew(string sModule, string sMethod, CData oParams, const char *cCmdSend, int cCmdLength, char *cRecult)
{
    CTransmitClient client;
    ResponseCode res = eResponseCodeSuccess;
    CData oResult = SVSMAP();
    CData oReturn = SVSMAP();
    mLogDebug("Module:"<<sModule<<",Method:"<<sMethod);

#if 0
    //add fwb 20201027
    //char *cCmd = (char *)malloc(1024);
    //char cCmdRecv[1024] = {0};
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 
    //memset(cCmdRecv, 0, sizeof(1024)); 
    cCmdLength = strlen(cTmpCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cTmpCmd) >> 8;  
    cHeadCmd[3] = strlen(cTmpCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cTmpCmd, strlen(cTmpCmd));
#endif 

#if 0
    for (int i = 0; i < cCmdLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cCmdSend[i]);
	    }
	    else 
	    {
		    printf("%c", cCmdSend[i]);
	    }
    }
#endif 

    //打印参数
    try{
        string sParams;
        if(!CData::ToCustomXMLString(oParams.RefMap(),sParams)){
            mLogError("Convert xml data to xml string failed.");
        }
        mLogDebug("params:\r\n"<<sParams);
    }catch(exception& e){
        mLogDebug("ToCustomXMLString(oParams.RefMap(),...) error."<<e.what());
    }

#if 1
    do{
        //连接主机
    
        if( client.Connect(this->_ip,this->_port) == false ){
            client.Close();     // 断开连接
            mLogError("Failed to connect.ip:"<<this->_ip<<",port:"<<this->_port);
            oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = SVSLONG32(eResponseCodeNoConnected);
            break;
        }
		mLogDebug("success to connect.ip:"<<this->_ip<<",port:"<<this->_port);
        // 向主机发送信息并接受主机返回信息
        res = client.Execute(sModule,sMethod,oParams,cCmdSend,cCmdLength,oResult,cRecult,30000) ;
		mLogDebug("success to recv from ip:"<<this->_ip<<",port:"<<this->_port);
#if 0
	int iRecvLength = (cRecult[2] << 8) + cRecult[3] + 4;
        printf("Execute Recv\n");
	for (int i = 0; i < iRecvLength; i++)
	{
		if (i < 4)
		{
			printf("%d", cRecult[i]);
		}
		else 
		{
			printf("%c", cRecult[i]);
		}
	}
#endif 

        if( res!= eResponseCodeSuccess ){
            client.Close();     // 断开连接
            mLogError("Failed to Execute(module,method,params,result,timeOut)=("
                      <<sModule<<","
                      <<sMethod<<",oParams,oResult,"
                      <<this->_timeOut<<",),ResponseCode:"
                      <<res);
            oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)(res);
            break;
        }
    }while(false);
    // 断开连接
    client.Close();
#else
    if (SVSFAILED(CData::FromFile(oResult, "res2.xml"))){
        mLogError("Load config from res.xml failed.");
    }


    string sResult;
    if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
        mLogError("Convert xml data to xml string failed.");
    }
    mLogDebug("oResult:\r\n"<<sResult);

    //TODO 添加节点失败
    bool findNodeResponseCodeFlag = false;
    bool findNodeResponseFlag = false;

    //查找节点 SKYVIS_TRANSMIT_RESPONSE
    try{
        SVSMAP::iterator find = oResult.RefMap().find(SKYVIS_TRANSMIT_RESPONSE);
        if (find == oResult.RefMap().end()){
            mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE);
            findNodeResponseFlag = false;
        }else{
            mLogDebug("oResult 有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE);
            findNodeResponseFlag = true;
        }
    }catch(exception &e){
        mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE<<", exception.what:"<<e.what());
        findNodeResponseFlag = false;
    }

    //查找节点 SKYVIS_TRANSMIT_TRANSMITCODE
    try{
        SVSMAP::iterator find = oResult.RefMap().find(SKYVIS_TRANSMIT_TRANSMITCODE);
        if (find == oResult.RefMap().end()){
            mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE);
            findNodeResponseCodeFlag = false;
        }else{
            mLogDebug("oResult 有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE);
            findNodeResponseCodeFlag = true;
        }
    }catch(exception &e){
        mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<", exception.what:"<<e.what());
        findNodeResponseCodeFlag = false;
    }

    if( !findNodeResponseFlag && !findNodeResponseCodeFlag){
        //两个节点都不存在
        try{
            string sResult;
            CData oRes = SVSMAP();
//            if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
//                mLogError("Convert xml data to xml string failed.");
//            }
//            mLogDebug("oResult:\r\n"<<sResult);

            oRes.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)(res);
            oRes.RefMap()[SKYVIS_TRANSMIT_RESPONSE] = (SVSMAP)(oResult.RefMap());

//            if(!CData::ToCustomXMLString(oRes.RefMap(),sResult)){
//                mLogError("Convert xml data to xml string failed.");
//            }
//            mLogDebug("oRes:\r\n"<<sResult);

            oResult.Release();
            oResult = oRes;
//            if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
//                mLogError("Convert xml data to xml string failed.");
//            }
//            mLogDebug("oResult:\r\n"<<sResult);
        }catch(exception &e){
            mLogWarn("oResult 添加节点失败 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<" node:"<<SKYVIS_TRANSMIT_RESPONSE<<", exception.what:"<<e.what());
        }
    }else{
        if(findNodeResponseFlag && !findNodeResponseCodeFlag){
            try{
                oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)(eResponseCodeSuccess);
            }catch(exception &e){
                mLogWarn("oResult 添加节点失败 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<", exception.what:"<<e.what());
            }
        }
    }

    //打印返回信息
    try{
        string sResult;
        if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
            mLogDebug("Convert xml data to xml string failed.");
        }
        mLogDebug("处理(添加节点)后 sResult:\r\n"<<sResult);
    }catch(exception& e){
        mLogDebug("ToCustomXMLString(oResult.RefMap(),...) error."<<e.what());
    }
#endif

    return res;
}

CData MPSClient::socketSend(string sModule, string sMethod, CData oParams)
{
    CTransmitClient client;
    ResponseCode res = eResponseCodeSuccess;
    CData oResult = SVSMAP();
    CData oReturn = SVSMAP();
    mLogDebug("Module:"<<sModule<<",Method:"<<sMethod);

    //add fwb 20201027
    //char *cCmd = (char *)malloc(1024);
    char cCmdRecv[1024] = {0};
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 
    memset(cCmdRecv, 0, sizeof(1024)); 
    cCmdLength = strlen(cTmpCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cTmpCmd) >> 8;  
    cHeadCmd[3] = strlen(cTmpCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cTmpCmd, strlen(cTmpCmd));
#if 0
    for (int i = 0; i < cCmdLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cCmdSend[i]);
	    }
	    else 
	    {
		    printf("%c", cCmdSend[i]);
	    }
    }
#endif 

    //打印参数
    try{
        string sParams;
        if(!CData::ToCustomXMLString(oParams.RefMap(),sParams)){
            mLogError("Convert xml data to xml string failed.");
        }
        mLogDebug("params:\r\n"<<sParams);
    }catch(exception& e){
        mLogDebug("ToCustomXMLString(oParams.RefMap(),...) error."<<e.what());
    }

#if 1
    do{
        //连接主机
        if( client.Connect(this->_ip,this->_port) == false ){
            client.Close();     // 断开连接
            mLogError("Failed to connect.ip:"<<this->_ip<<",port:"<<this->_port);
            oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = SVSLONG32(eResponseCodeNoConnected);
            break;
        }
#if 0
        // 向主机发送信息并接受主机返回信息
        //res = client.Execute(sModule,sMethod,oParams,cCmdSend,cCmdLength,oResult,cRecult,this->_timeOut) ;
	int iRecvLength = (cRecult[2] << 8) + cRecult[3] + 4;
        printf("Execute Recv\n");
	for (int i = 0; i < iRecvLength; i++)
	{
		if (i < 4)
		{
			printf("%d", cRecult[i]);
		}
		else 
		{
			printf("%c", cRecult[i]);
		}
	}
#else
        //res = client.Execute(sModule,sMethod,oParams,cCmdSend,cCmdLength,oResult,cCmdRecv,this->_timeOut) ;
	int iRecvLength = (cCmdRecv[2] << 8) + cCmdRecv[3] + 4;
        printf("Execute Recv\n");
	for (int i = 0; i < iRecvLength; i++)
	{
		if (i < 4)
		{
			printf("%d", cCmdRecv[i]);
		}
		else 
		{
			printf("%c", cCmdRecv[i]);
		}
	}
#endif 

        if( res!= eResponseCodeSuccess ){
            client.Close();     // 断开连接
            mLogError("Failed to Execute(module,method,params,result,timeOut)=("
                      <<sModule<<","
                      <<sMethod<<",oParams,oResult,"
                      <<this->_timeOut<<",),ResponseCode:"
                      <<res);
            oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)(res);
            break;
        }
    }while(false);
    // 断开连接
    client.Close();
#else
    if (SVSFAILED(CData::FromFile(oResult, "res2.xml"))){
        mLogError("Load config from res.xml failed.");
    }
#endif

    string sResult;
    if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
        mLogError("Convert xml data to xml string failed.");
    }
    mLogDebug("oResult:\r\n"<<sResult);

    //TODO 添加节点失败
    bool findNodeResponseCodeFlag = false;
    bool findNodeResponseFlag = false;

    //查找节点 SKYVIS_TRANSMIT_RESPONSE
    try{
        SVSMAP::iterator find = oResult.RefMap().find(SKYVIS_TRANSMIT_RESPONSE);
        if (find == oResult.RefMap().end()){
            mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE);
            findNodeResponseFlag = false;
        }else{
            mLogDebug("oResult 有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE);
            findNodeResponseFlag = true;
        }
    }catch(exception &e){
        mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE<<", exception.what:"<<e.what());
        findNodeResponseFlag = false;
    }

    //查找节点 SKYVIS_TRANSMIT_TRANSMITCODE
    try{
        SVSMAP::iterator find = oResult.RefMap().find(SKYVIS_TRANSMIT_TRANSMITCODE);
        if (find == oResult.RefMap().end()){
            mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE);
            findNodeResponseCodeFlag = false;
        }else{
            mLogDebug("oResult 有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE);
            findNodeResponseCodeFlag = true;
        }
    }catch(exception &e){
        mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<", exception.what:"<<e.what());
        findNodeResponseCodeFlag = false;
    }

    if( !findNodeResponseFlag && !findNodeResponseCodeFlag){
        //两个节点都不存在
        try{
            string sResult;
            CData oRes = SVSMAP();
//            if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
//                mLogError("Convert xml data to xml string failed.");
//            }
//            mLogDebug("oResult:\r\n"<<sResult);

            oRes.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)(res);
            oRes.RefMap()[SKYVIS_TRANSMIT_RESPONSE] = (SVSMAP)(oResult.RefMap());

//            if(!CData::ToCustomXMLString(oRes.RefMap(),sResult)){
//                mLogError("Convert xml data to xml string failed.");
//            }
//            mLogDebug("oRes:\r\n"<<sResult);

            oResult.Release();
            oResult = oRes;
//            if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
//                mLogError("Convert xml data to xml string failed.");
//            }
//            mLogDebug("oResult:\r\n"<<sResult);
        }catch(exception &e){
            mLogWarn("oResult 添加节点失败 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<" node:"<<SKYVIS_TRANSMIT_RESPONSE<<", exception.what:"<<e.what());
        }
    }else{
        if(findNodeResponseFlag && !findNodeResponseCodeFlag){
            try{
                oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE] = (SVSLONG32)(eResponseCodeSuccess);
            }catch(exception &e){
                mLogWarn("oResult 添加节点失败 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<", exception.what:"<<e.what());
            }
        }
    }

    //打印返回信息
    try{
        string sResult;
        if(!CData::ToCustomXMLString(oResult.RefMap(),sResult)){
            mLogDebug("Convert xml data to xml string failed.");
        }
        mLogDebug("处理(添加节点)后 sResult:\r\n"<<sResult);
    }catch(exception& e){
        mLogDebug("ToCustomXMLString(oResult.RefMap(),...) error."<<e.what());
    }

    return oResult;
}

bool MPSClient::isSuccessResponseCode(CData &oResult, ResponseCode &code, MPSOperationRes &opCode)
{
    //*
    //获取返回信息中节点SKYVIS_TRANSMIT_TRANSMITCODE值
    int resCode = eResponseCodeErrorRecv;
    try{
        SVSMAP::iterator find = oResult.RefMap().find(SKYVIS_TRANSMIT_TRANSMITCODE);
        if (find == oResult.RefMap().end()){
            mLogError("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE);
            return false;
        }else{
            mLogDebug("oResult 有此节点 node:"<<SKYVIS_TRANSMIT_TRANSMITCODE);
            resCode = oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE].RefLong32();
        }
        mLogDebug("oResult node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<",val:"<<resCode);
    }catch(exception & e){
        //返回信息中没有节点SKYVIS_TRANSMIT_TRANSMITCODE
        mLogError("Faild to try get oResult node:"<<SKYVIS_TRANSMIT_TRANSMITCODE);
        return false;
    }

    //判断返回信息中节点SKYVIS_TRANSMIT_TRANSMITCODE值
    code = (ResponseCode)resCode;
    if(code != eResponseCodeSuccess){
        mLogError("get oResult node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<",val:"<<resCode<<"!=eResponseCodeSuccess:"<<eResponseCodeSuccess);
        return false;
    }
    //*/
//    return true;
    //*
    //判断flag节点
    try{
        SVSMAP::iterator find = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap().find(RES_SUCCESS_FLAG);
        if (find == oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap().end()){
            //有些返回没有此节点
            mLogWarn("oResult 没有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE<<" - "<<RES_SUCCESS_FLAG);
            return true;
        }else{
            mLogDebug("oResult 有此节点 node:"<<SKYVIS_TRANSMIT_RESPONSE<<" - "<<RES_SUCCESS_FLAG);
            //不确定节点flag为什么类型，全部进行分析
            try{
                resCode = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()[RES_SUCCESS_FLAG].RefLong64();
                opCode = (MPSOperationRes)resCode;
                if(IS_SUCCESS_RES_LONG_FLAG(resCode)){
                    return true;
                }else{
                    mLogError("IS_SUCCESS_RES_LONG_FLAG(resCode):"<<IS_SUCCESS_RES_LONG_FLAG(resCode));
                    return false;
                }
            }catch(...){
                try{
                    resCode = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()[RES_SUCCESS_FLAG].RefLong32();
                    opCode = (MPSOperationRes)resCode;
                    if(IS_SUCCESS_RES_LONG_FLAG(resCode)){
                        return true;
                    }else{
                        mLogError("IS_SUCCESS_RES_LONG_FLAG(resCode):"<<IS_SUCCESS_RES_LONG_FLAG(resCode));
                        return false;
                    }
                }catch(...){
                    try{
                        resCode = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()[RES_SUCCESS_FLAG].RefLong16();
                        opCode = (MPSOperationRes)resCode;
                        if(IS_SUCCESS_RES_LONG_FLAG(resCode)){
                            return true;
                        }else{
                            mLogError("IS_SUCCESS_RES_LONG_FLAG(resCode):"<<IS_SUCCESS_RES_LONG_FLAG(resCode));
                            return false;
                        }
                    }catch(...){
                        try{
                            resCode = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()[RES_SUCCESS_FLAG].RefBool();

                            if(IS_SUCCESS_RES_BOOL_FLAG(resCode)){
                                opCode = eMPSResultOK;
                                return true;
                            }else{
                                mLogError("IS_SUCCESS_RES_BOOL_FLAG(resCode):"<<IS_SUCCESS_RES_BOOL_FLAG(resCode));
                                opCode = eMPSResultErrorGeneric;
                                return false;
                            }
                        }catch(...){
                            mLogError("Faild to try get oResult node:"<<SKYVIS_TRANSMIT_RESPONSE<<" - "<<RES_SUCCESS_FLAG);
                            return false;
                        }
                    }
                }
            }

        }
        mLogDebug("oResult node:"<<SKYVIS_TRANSMIT_RESPONSE<<" - "<<RES_SUCCESS_FLAG<<" val:"<<opCode);
    }catch(exception & e){
        mLogError("Faild to try get oResult node:"<<SKYVIS_TRANSMIT_RESPONSE<<" - "<<RES_SUCCESS_FLAG);
        opCode = eMPSErrorStatusInvalid;
        return false;
    }
//    if(IS_SUCCESS_RES_FLAG(resCode)){
//        return true;
//    }else{
//        mLogError("IS_SUCCESS_RES_FLAG(resCode):"<<IS_SUCCESS_RES_FLAG(resCode));
//        return false;
//    }

    //*/
    /*
    try{
        resCode = oResult.RefMap()[SKYVIS_TRANSMIT_TRANSMITCODE].RefLong32();
        mLogDebug("oResult node:"<<SKYVIS_TRANSMIT_TRANSMITCODE<<" val:"<<resCode);
    }catch(exception & e){
        mLogError("Faild to try get oResult node:"<<SKYVIS_TRANSMIT_TRANSMITCODE);
        return false;
    }

    //判断是否执行成功
    code = (ResponseCode)resCode;
    if(resCode != eResponseCodeSuccess){
        return false;
    }else{
        return true;
    }
    //*/
}

ResponseCode MPSClient::checkReturnResponseCode(MPSOperationRes &opResCode, CData &oResult)
{
    //判断返回信息是否为执行成功
    ResponseCode resCode = eResponseCodeErrorRecv;
    if(!isSuccessResponseCode(oResult,resCode,opResCode)){
        mLogError("Failed to run isSuccessResponseCode(...)");
        return resCode;
    }

    //判断是否执行成功
    if(resCode != eResponseCodeSuccess){
        mLogError("responseCode:"<<resCode);
        mLogError("responseError:"<<GetResponseError(resCode));
        return resCode;
    }
    return eResponseCodeSuccess;
}

bool MPSClient::getMediaGetStateParams(CData &oParams)
{
    try{
        oParams = SVSMAP();
//        oParams.RefMap()[MEDIA_GETSTATUS_PARAMS_WORKNUM] = (SVSLONG16)(workNum);
//        mLogDebug("params node:"<<MEDIA_GETSTATUS_PARAMS_WORKNUM<<" val:"<<workNum);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getMediaPrepareParams(int chNum, string keyId, CData &oParams)
{
    try{
        oParams = SVSMAP();
        string sParams = "<Root type=\"MAP\">"
                "   <worknumber type=\"LONG16\">1</worknumber>"
                "   <keyID type=\"STRING\"><![CDATA[]]></keyID>"
                "   <duration type=\"LONG16\"></duration>"
                "   <texts type=\"VECTOR\">"
                "       <A0 type=\"STRING\"><![CDATA[]]></A0>"
                "       <A1 type=\"STRING\"><![CDATA[]]></A1>"
                "       <A2 type=\"STRING\"><![CDATA[]]></A2>"
                "       <A3 type=\"STRING\"><![CDATA[]]></A3>"
                "       <A4 type=\"STRING\"><![CDATA[]]></A4>"
                "   </texts>"
                "   <clients type=\"VECTOR\">"
                "       <A0 type=\"MAP\">"
                "       <ip type=\"STRING\"><![CDATA[]]></ip>"
                "       <port type=\"LONG16\"></port>"
                "   </A0>"
                "   </clients>"
                "   <disableNetworkStore type=\"BOOL\">1</disableNetworkStore>"
                "</Root>";
        if(SVSFAILED(CData::FromCustomString(oParams,sParams))){
            mLogError("CData::FromCustomString(oParams,sParams) == false.");
            return false;
        }
        oParams.RefMap()[MEDIA_PREPARE_PARAMS_WORKNUM] = (SVSLONG16)(chNum);
        oParams.RefMap()[MEDIA_PREPARE_PARAMS_KEYID] = (SVSSTRING)(keyId);
        mLogDebug("Media Prepare params node:"<<MEDIA_PREPARE_PARAMS_WORKNUM<<" val:"<<oParams.RefMap()[MEDIA_PREPARE_PARAMS_WORKNUM].RefLong16());
        mLogDebug("Media Prepare params node:"<<MEDIA_PREPARE_PARAMS_KEYID<<" val:"<<oParams.RefMap()[MEDIA_PREPARE_PARAMS_KEYID].RefString());
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getMediaStartParams(int chNum, string keyId, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[MEDIA_START_PARAMS_WORKNUM] = (SVSLONG16)(chNum);
        oParams.RefMap()[MEDIA_START_PARAMS_KEYID] = (SVSSTRING)(keyId);
        mLogDebug("Media Start params node:"<<MEDIA_START_PARAMS_WORKNUM<<" val:"<<oParams.RefMap()[MEDIA_START_PARAMS_WORKNUM].RefLong16());
        mLogDebug("Media Start params node:"<<MEDIA_START_PARAMS_KEYID<<" val:"<<oParams.RefMap()[MEDIA_START_PARAMS_KEYID].RefString());
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getMediaStopParams(int chNum, bool closeFlag, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[MEDIA_STOP_PARAMS_WORKNUM] = (SVSLONG16)(chNum);
        mLogDebug("Media Stop params node:"<<MEDIA_STOP_PARAMS_WORKNUM<<" val:"<<chNum);
        if(closeFlag){
            oParams.RefMap()[MEDIA_STOP_PARAMS_CLOSE] = (SVSLONG16)(1);
            mLogDebug("Media Stop params node:"<<MEDIA_STOP_PARAMS_CLOSE<<" val:1");
        }else{
            oParams.RefMap()[MEDIA_STOP_PARAMS_CLOSE] = (SVSLONG16)(0);
            mLogDebug("Media Stop params node:"<<MEDIA_STOP_PARAMS_CLOSE<<" val:0");
        }
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getMediaCloseParams(int chNum, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[MEDIA_CLOSE_PARAMS_WORKNUM] = (SVSLONG16)(chNum);
        mLogDebug("Media Close params node:"<<MEDIA_CLOSE_PARAMS_WORKNUM<<" val:"<<chNum);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getMediaSetPointParams(int chNum, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[MEDIA_SET_POINT_PARAMS_CHANNEL] = (SVSLONG16)(chNum);
        mLogDebug("Media Close params node:"<<MEDIA_SET_POINT_PARAMS_CHANNEL<<" val:"<<chNum);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getGetPackageParams(string keyId, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[PACKAGE__GET_PACKAGE_PARAMS_KEYID] = (SVSSTRING)(keyId);
        mLogDebug("Get Package params node:"<<PACKAGE__GET_PACKAGE_PARAMS_KEYID<<" val:"<<keyId);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getUpdatePackageFlagParams(string keyId, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[PACKAGE_UPDATE_PACAKGE_FLAG_PARAMS_CASEID] = (SVSSTRING)(keyId);
        mLogDebug("Update Package Flag params node:"<<PACKAGE_UPDATE_PACAKGE_FLAG_PARAMS_CASEID<<" val:"<<keyId);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getControlSendCmdParams(string cmd, CData &oParams, int deviceID)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[CONTROL_SENDCMD_PARAMS_CMD] = (SVSSTRING)(cmd);
        oParams.RefMap()[CONTROL_SENDCMD_PARAMS_DEVICEID] = (SVSLONG16)(deviceID);
        mLogDebug("Console SendCmd params node:"<<CONTROL_SENDCMD_PARAMS_CMD<<" val:"<<cmd);
        mLogDebug("Console SendCmd params node:"<<CONTROL_SENDCMD_PARAMS_DEVICEID<<" val:"<<deviceID);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getConfigSetInterRtspParams(int chNum, string url, bool enble, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[CONFIG_SET_INTER_RTSP_PARAMS_ID] = (SVSLONG16)(chNum);
        oParams.RefMap()[CONFIG_SET_INTER_RTSP_PARAMS_RTSPURL] = (SVSSTRING)(url);
        oParams.RefMap()[CONFIG_SET_INTER_RTSP_PARAMS_RTSPENABLE] = (SVSBOOL)(enble);
        mLogDebug("Console SendCmd params node:"<<CONFIG_SET_INTER_RTSP_PARAMS_ID<<" val:"<<chNum);
        mLogDebug("Console SendCmd params node:"<<CONFIG_SET_INTER_RTSP_PARAMS_RTSPURL<<" val:"<<url);
        mLogDebug("Console SendCmd params node:"<<CONFIG_SET_INTER_RTSP_PARAMS_RTSPENABLE<<" val:"<<enble);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getMediaStartInteRtspParams(int chNum, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[MEDIA_START_INTE_RTSP_PARAMS_ID] = (SVSLONG16)(chNum);
        mLogDebug("Console Media startInteRtsp params node:"<<MEDIA_START_INTE_RTSP_PARAMS_ID<<" val:"<<chNum);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getMediaStopInteRtspParams(int chNum, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[MEDIA_STOP_INTE_RTSP_PARAMS_ID] = (SVSLONG16)(chNum);
        mLogDebug("Console Media stopInteRtsp params node:"<<MEDIA_STOP_INTE_RTSP_PARAMS_ID<<" val:"<<chNum);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getMediaStartDecodeParams(int chNum, string url, int cacthTime, bool tcpMode, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[MEDIA_START_INTER_RTSP_API_PARAMS_DEC_NUM] = (SVSLONG16)(chNum);
        oParams.RefMap()[MEDIA_START_INTER_RTSP_API_PARAMS_URL] = (SVSSTRING)(url);
        oParams.RefMap()[MEDIA_START_INTER_RTSP_API_PARAMS_CACHE_TIME] = (SVSLONG32)(cacthTime);
        oParams.RefMap()[MEDIA_START_INTER_RTSP_API_PARAMS_USE_TCP] = (SVSBOOL)(tcpMode);
        mLogDebug("Media StartInterRtsp_Api params node:"<<MEDIA_START_INTER_RTSP_API_PARAMS_DEC_NUM<<" val:"<<chNum);
        mLogDebug("Media StartInterRtsp_Api params node:"<<MEDIA_START_INTER_RTSP_API_PARAMS_URL<<" val:"<<url);
        mLogDebug("Media StartInterRtsp_Api params node:"<<MEDIA_START_INTER_RTSP_API_PARAMS_CACHE_TIME<<" val:"<<cacthTime);
        mLogDebug("Media StartInterRtsp_Api params node:"<<MEDIA_START_INTER_RTSP_API_PARAMS_USE_TCP<<" val:"<<tcpMode);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getMediaStopDecodeParams(int chNum, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[MEDIA_STOP_INTER_RTSP_API_PARAMS_DEC_NUM] = (SVSLONG16)(chNum);
        mLogDebug("Media StopInterRtsp_Api params node:"<<MEDIA_STOP_INTER_RTSP_API_PARAMS_DEC_NUM<<" val:"<<chNum);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

bool MPSClient::getMediaGetDecodeStatusParams(int chNum, CData &oParams)
{
    try{
        oParams = SVSMAP();
        oParams.RefMap()[MEDIA_GET_INTER_RTSP_API_PARAMS_DEC_NUM] = (SVSLONG16)(chNum);
        mLogDebug("Media GetInterRtsp_Api params node:"<<MEDIA_GET_INTER_RTSP_API_PARAMS_DEC_NUM<<" val:"<<chNum);
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }
    return true;
}

