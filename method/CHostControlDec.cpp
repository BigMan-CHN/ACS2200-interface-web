#include "CHostControlDec.h"

CHostControlDec::CHostControlDec(string ip, int port, int timeOut)
    :_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_result(cJSON_CreateObject())
{

}

InterfaceResCode CHostControlDec::StartDec(int chNum, string url,int cacheTime,bool tcpMode)
{
    mLogInfo("开始解码...");
    mLogInfo("chNum:"<<chNum<<",url:"<<url);

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaStartDecode(chNum,url,cacheTime,tcpMode,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaStartDecode(chNum,url,cacheTime,tcpMode...)=("<<url<<","<<cacheTime<<","<<tcpMode<<",...)"<<" error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControlDec::StopDec(int chNum)
{
    mLogInfo(" 停止解码...");
    mLogInfo("chNum:"<<chNum);

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaStopDecode(chNum,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaStopInteRtsp(chNum,...)=("<<chNum<<",...)"<<" error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControlDec::GetDecStatus(int chNum, string &sResult)
{
    mLogInfo(" 获取解码状态...");
    mLogInfo("chNum:"<<chNum);

    HttpMediaDecodeStatus httpStatus;
    MPSMediaDecodeStatus mpsStatus;
    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaGetDecodeStatus(chNum,mpsStatus,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaGetDecodeStatus(chNum,...)=("<<chNum<<",...)"<<" error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    httpStatus = CovertMediaDecodeStatus(mpsStatus);
    if(httpStatus >= eHttpDecodeStatusSize){
        mLogError("httpStatus:"<<httpStatus<<" >= eHttpDecodeStatusSize:"<<eHttpDecodeStatusSize);
        return eInterfaceResCodeErrorLocalDataConvert;
    }

    if(_result==NULL){
        _result = cJSON_CreateObject();
    }
    if(httpStatus == eHttpDecodeStatusStart){
        cJSON_AddItemToObject(_result,HOST_DEC_STATUS_RES_DEC_STATUS,cJSON_CreateTrue());
    }else{
        cJSON_AddItemToObject(_result,HOST_DEC_STATUS_RES_DEC_STATUS,cJSON_CreateFalse());
    }

    char *c = cJSON_Print(_result);
    sResult = c;
    mLogDebug("GetDecStatus(chNum,sRes)=("<<chNum<<","<<sResult<<")");
    return eInterfaceResCodeSuccess;
}

