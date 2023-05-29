#include "CSetTaskDelete.h"


CSetTaskDelete::CSetTaskDelete(string ip, int port, int timeOut)
    :_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
{

}

InterfaceResCode CSetTaskDelete::SetTaskDelete(string caseId)
{
    mLogInfo("标记caseid为可删除...");

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->UpdatePackageFlag(caseId,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("UpdatePackageFlag(caseId,...)"<<caseId<<"error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }
    return eInterfaceResCodeSuccess;
}
