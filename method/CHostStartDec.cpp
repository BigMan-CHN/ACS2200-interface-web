#include "CHostStartDec.h"

CHostStartDec::CHostStartDec(string ip, int port, int timeOut)
    :_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
{

}

InterfaceResCode CHostStartDec::StartDec(int chNum, string url)
{
    mLogInfo("开始解码...");
    mLogInfo("chNum:"<<chNum);

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaStartInteRtsp(chNum,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("ConsoleSendCmd(cmd,...,deviceID)=("<<chNum<<",...,"<<deviceID<<") error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    return eInterfaceResCodeSuccess;
}
