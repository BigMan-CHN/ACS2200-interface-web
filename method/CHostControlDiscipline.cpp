#include "CHostControlDiscipline.h"

CHostControlDiscipline::CHostControlDiscipline(string ip, int port, int timeOut)
    :_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_result(cJSON_CreateObject())
{

}

InterfaceResCode CHostControlDiscipline::StartDisciplie()
{
    mLogInfo("开始播放法庭纪律...");

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaStartDiscipline(opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaStartDiscipline(...)"<<" error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControlDiscipline::StopDisciplie()
{
    mLogInfo("停止播放法庭纪律...");

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaStopDiscipline(opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaStopDiscipline(...)"<<" error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControlDiscipline::GetDisciplieStatus(string &sResult)
{
    mLogInfo(" 获取播放法庭纪律状态...");

    HttpMediaDisciplineStatus httpStatus;
    MPSMediaDisciplineStatus mpsStatus;
    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaGetDisciplineStatus(mpsStatus,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaGetDisciplineStatus(...)"<<" error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    httpStatus = CovertMediaDisciplineStatus(mpsStatus);
    if(httpStatus >= eHttpDisciplinetatusSize){
        mLogError("httpStatus:"<<httpStatus<<" >= eHttpDisciplinetatusSize:"<<eHttpDisciplinetatusSize);
        return eInterfaceResCodeErrorLocalDataConvert;
    }

    if(_result==NULL){
        _result = cJSON_CreateObject();
    }
    if(httpStatus == eHttpDisciplineStatusStart){
        cJSON_AddItemToObject(_result,HOST_GET_DISCIPLINE_STATUS_RES_DCPL_STATUS,cJSON_CreateTrue());
    }else{
        cJSON_AddItemToObject(_result,HOST_GET_DISCIPLINE_STATUS_RES_DCPL_STATUS,cJSON_CreateFalse());
    }

    char *c = cJSON_Print(_result);
    sResult = c;
    mLogDebug("GetDisciplieStatus(sRes)=("<<sResult<<")");
    return eInterfaceResCodeSuccess;
}

