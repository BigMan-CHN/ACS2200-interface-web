#include "CGetSysConfig.h"

CGetSysConfig::CGetSysConfig(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
{

}

bool CGetSysConfig::GetMPSSysConfig(CData &oResult)
{
    mLogDebug("run GetMPSSysConfig(...)");
    //获取状态
//    CData oResult = SVSMAP();
    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfig(opResCode,oResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return false;
    }

    string strACSID;
    try{
        strACSID = oResult.RefMap()["ACS"].RefMap()["ACSID"].RefString();
        mLogDebug("get ACS-ACSID:"<<strACSID);
        return true;
    }catch(exception &e){
        try{
            strACSID = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["ACS"].RefMap()["ACSID"].RefString();
            mLogDebug("get ACS-ACSID:"<<strACSID);
            return true;
        }catch(exception &e){
            mLogError("Failed to try get node ACS-ACSID.e:"<<e.what());
            return false;
        }
    }
}

