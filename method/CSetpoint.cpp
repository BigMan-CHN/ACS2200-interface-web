#include "CSetpoint.h"

CSetpoint::CSetpoint(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_duration(0)
    ,_filePath("")
{

}

InterfaceResCode CSetpoint::Setpoint(string &sResult,int chNum)
{
    mLogInfo("视频打点...");
    mLogInfo("chNum:"<<chNum);

    MPSMediaStatus mediaStatus;

    if(chNum < 1){
        mLogError("Failed params:chNum="<<chNum<<" < 1.");
        return eInterfaceResCodeRequErrorParam;
    }

    if(_hostStatus.GetMediaStatus(chNum-1,mediaStatus) == false){
        mLogError("Failed to run CGetHostStatus::GetMeidaStatus().");
        return eInterfaceResCodeErrorLocalDataConvert;
    }

    if(mediaStatus != eMPSStatusRunning){
        mLogError("MedisStatus:"<<mediaStatus<<"("<<getMPSStatusStr(mediaStatus)<<") != eMPSStatusRunning:"<<eMPSStatusRunning);
        InterfaceResCode resCode = (InterfaceResCode)((int)eInterfaceResCodeErrorMPS+(int)eMPSErrorStatusInvalid);
        return resCode;
    }

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaSetPoint(chNum,opResCode,_duration,_filePath);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaSetPoint(chNum,...)=("<<chNum<<",...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }
    mLogDebug("MediaSetPoint(chNum,...,duration,filePath)=("<<chNum<<",...,"<<_duration<<","<<_filePath<<")");

    if(AddRootNode() == false){
        mLogError("Failed to run  AddRootNode(...)");
        return eInterfaceResCodeError;
    }
    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

string CSetpoint::ComposeResult()
{
    char *cRes = cJSON_Print(_result);
    string res = cRes;
    return res;
}

bool CSetpoint::AddRootNode()
{
    if(_result == NULL){
        mLogError("cJSON result == NULL.");
        return false;
    }
    if(_filePath.length() <= 0){
        _duration = 0;
    }
    cJSON_AddItemToObject(_result,SETPOINT_RES_DURATION        ,cJSON_CreateNumber(_duration));
    cJSON_AddItemToObject(_result,SETPOINT_RES_FILEPATH        ,cJSON_CreateString(_filePath.c_str()));
    return true;
}
