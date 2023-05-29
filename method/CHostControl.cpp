#include "CHostControl.h"
#include "CGetHostStatus.h"

CHostControl::CHostControl(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
{

}

InterfaceResCode CHostControl::HostInit(int chNum,string caseid)
{
    MPSMediaStatus mediaStatus;

    if(chNum < 1){
        mLogError("Failed params:chNum="<<chNum<<" < 1.");
        return eInterfaceResCodeRequErrorParam;
    }

    if(_hostStatus.GetMediaStatus(chNum-1,mediaStatus) == false){
        mLogError("Failed to run CGetHostStatus::GetMeidaStatus().");
        return eInterfaceResCodeErrorLocalDataConvert;
    }
    if(mediaStatus == eMPSStatusClose){//无此事件或已经结束
        //Init
        return Close2Init(chNum,caseid);
    }else if(mediaStatus == eMPSStatusReady){//准备就绪
        //check keyid
        //!=keyid
        //close
        //Init
        return Ready2Init(chNum,caseid);
    }else if(mediaStatus == eMPSStatusRunning){//正在工作
        //check keyid
        //!=keyid
        //stop
        //Init
        return Running2Init(chNum,caseid);
    }else if(mediaStatus == eMPSStatusPaused){//暂停
        //error
        return Pause2Init(chNum,caseid);
    }else if(mediaStatus == eMPSStatusStopped){//已经停止
        //close
        //Init
        return Stop2Init(chNum,caseid);
    }else{
        //error
        mLogError("CGetHostStatus::GetMeidaStatus() status:"<<mediaStatus<<" error!");
        return eInterfaceResCodeErrorLocalStatus;
    }
}

InterfaceResCode CHostControl::HostStart(int chNum,string caseid)
{
    MPSMediaStatus mediaStatus;

    if(chNum < 1){
        mLogError("Failed params:chNum="<<chNum<<" < 1.");
        return eInterfaceResCodeRequErrorParam;
    }

    if(_hostStatus.GetMediaStatus(chNum-1,mediaStatus) == false){
        mLogError("Failed to run CGetHostStatus::GetMeidaStatus().");
        return eInterfaceResCodeErrorLocalDataConvert;
    }
    if(mediaStatus == eMPSStatusClose){//无此事件或已经结束
        //start
        return Close2Start(chNum,caseid);
    }else if(mediaStatus == eMPSStatusReady){//准备就绪
        //check keyid
        //!=keyid
        //close
        //start
        return Ready2Start(chNum,caseid);
    }else if(mediaStatus == eMPSStatusRunning){//正在工作
        //check keyid
        //!=keyid
        //close
        //start
        return Running2Start(chNum,caseid);
    }else if(mediaStatus == eMPSStatusPaused){//暂停
        //error
        return Pause2Start(chNum,caseid);
    }else if(mediaStatus == eMPSStatusStopped){//已经停止
        //close
        //start
        return Stop2Start(chNum,caseid);
    }else{
        //error
        mLogError("CGetHostStatus::GetMeidaStatus() status:"<<mediaStatus<<" error!");
        return eInterfaceResCodeErrorLocalStatus;
    }
}

InterfaceResCode CHostControl::HostStop(int chNum)
{
    MPSMediaStatus mediaStatus;

    if(chNum < 1){
        mLogError("Failed params:chNum="<<chNum<<" < 1.");
        return eInterfaceResCodeRequErrorParam;
    }

    if(_hostStatus.GetMediaStatus(chNum-1,mediaStatus) == false){
        mLogError("Failed to run CGetHostStatus::GetMeidaStatus().");
        return eInterfaceResCodeErrorLocalDataConvert;
    }
    if(mediaStatus == eMPSStatusClose){//无此事件或已经结束
        //start
        return Close2Stop(chNum);
    }else if(mediaStatus == eMPSStatusReady){//准备就绪
        //check keyid
        //!=keyid
        //close
        //start
        return Ready2Stop(chNum);
    }else if(mediaStatus == eMPSStatusRunning){//正在工作
        //check keyid
        //!=keyid
        //close
        //start
        return Running2Stop(chNum);
    }else if(mediaStatus == eMPSStatusPaused){//暂停
        //error
        return Pause2Stop(chNum);
    }else if(mediaStatus == eMPSStatusStopped){//已经停止
        //close
        //start
        return Stop2Stop(chNum);
    }else{
        //error
        mLogError("CGetHostStatus::GetMeidaStatus() status:"<<mediaStatus<<" error!");
        return eInterfaceResCodeErrorLocalStatus;
    }
}

InterfaceResCode CHostControl::HostClose(int chNum)
{
    MPSMediaStatus mediaStatus;

    if(chNum < 1){
        mLogError("Failed params:chNum="<<chNum<<" < 1.");
        return eInterfaceResCodeRequErrorParam;
    }

    if(_hostStatus.GetMediaStatus(chNum-1,mediaStatus) == false){
        mLogError("Failed to run CGetHostStatus::GetMeidaStatus().");
        return eInterfaceResCodeErrorLocalDataConvert;
    }
    if(mediaStatus == eMPSStatusClose){//无此事件或已经结束
        return Close2Close(chNum);
    }else if(mediaStatus == eMPSStatusReady){//准备就绪
        //close
        return Ready2Close(chNum);
    }else if(mediaStatus == eMPSStatusRunning){//正在工作
        //stop
        //close
        return Running2Close(chNum);
    }else if(mediaStatus == eMPSStatusPaused){//暂停
        //error
        return Pause2Close(chNum);
    }else if(mediaStatus == eMPSStatusStopped){//已经停止
        //close
        return Stop2Close(chNum);
    }else{
        //error
        mLogError("CGetHostStatus::GetMeidaStatus() status:"<<mediaStatus<<" error!");
        return eInterfaceResCodeErrorLocalStatus;
    }
}

InterfaceResCode CHostControl::HostShutdown()
{
    mLogInfo("system('shutdown -h now')");
    std::system("shutdown -h now");
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControl::HostReboot()
{
    mLogInfo("system('/sbin/reboot')");
    std::system("/sbin/reboot");
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControl::Close2Init(int chNum,string caseid)
{
    mLogDebug("CHostControl::Close2Init(caseid)=("<<caseid<<")...");
    return MediaPrepareAll(chNum,caseid);
}

InterfaceResCode CHostControl::Ready2Init(int chNum, string caseid)
{
    mLogDebug("CHostControl::Ready2Init(caseid)=("<<caseid<<")...");
    string nowCaseid;
    if(chNum < 1){
        mLogError("Failed params:chNum="<<chNum<<" < 1.");
        return eInterfaceResCodeRequErrorParam;
    }
    if(_hostStatus.GetMediaNowKeyID(chNum-1,nowCaseid) == false){
        mLogError("Failed to run CGetHostStatus::GetMediaNowKeyID(chNum,...)=("<<chNum<<",...).");
        return eInterfaceResCodeErrorLocalDataConvert;
    }
    if(strcmp(nowCaseid.c_str(),caseid.c_str()) == 0){
        return eInterfaceResCodeSuccess;
    }else{
        //close--prepare
        mLogWarn("nowCaseid:"<<nowCaseid<<" != caseid:"<<caseid);
        //close
        InterfaceResCode resCode = MediaCloseAll(chNum);
        if(resCode != eInterfaceResCodeSuccess){
            mLogError("Failed to run CHostControl::MediaCloseAll(chNum)=("<<chNum<<").");
            return resCode;
        }
        //prepare
        return MediaPrepareAll(chNum,caseid);
    }
}

InterfaceResCode CHostControl::Running2Init(int chNum, string caseid)
{
    mLogDebug("CHostControl::Running2Init(caseid)=("<<caseid<<")...");
    //close -- stop
    InterfaceResCode resCode = MediaStopAll(chNum,true);
    if(resCode != eInterfaceResCodeSuccess){
        mLogError("Failed to run CHostControl::MediaStopAll(chNum,closeFlag)=("<<chNum<<",true).");
        return resCode;
    }
    //prepare
    return MediaPrepareAll(chNum,caseid);
}

InterfaceResCode CHostControl::Pause2Init(int chNum, string caseid)
{
    mLogDebug("CHostControl::Pause2Init(caseid)=("<<caseid<<")...");
    mLogError("Host status :Pause,can not Init.");
    return eInterfaceResCodeErrorLocalStatus;
}

InterfaceResCode CHostControl::Stop2Init(int chNum, string caseid)
{
    //close
    InterfaceResCode resCode = MediaCloseAll(chNum);
    if(resCode != eInterfaceResCodeSuccess){
        mLogError("Failed to run CHostControl::MediaCloseAll(chNum)=("<<chNum<<").");
        return resCode;
    }
    //prepare
    return MediaPrepareAll(chNum,caseid);
}

InterfaceResCode CHostControl::Close2Start(int chNum,string caseid)
{
    mLogDebug("CHostControl::Close2Start(caseid)=("<<caseid<<")...");
    //prepare
    InterfaceResCode resCode = MediaPrepareAll(chNum,caseid);
    if(resCode != eInterfaceResCodeSuccess){
        mLogError("Failed to run CHostControl::MediaPrepareAll(chNum,caseid)=("<<chNum<<","<<caseid<<").");
        return resCode;
    }
    //start
    return MediaStartAll(chNum,caseid);
}

InterfaceResCode CHostControl::Ready2Start(int chNum,string caseid)
{
    mLogDebug("CHostControl::Ready2Start(caseid)=("<<caseid<<")...");
    string nowCaseid;
    if(chNum < 1){
        mLogError("Failed params:chNum="<<chNum<<" < 1.");
        return eInterfaceResCodeRequErrorParam;
    }
    if(_hostStatus.GetMediaNowKeyID(chNum-1,nowCaseid) == false){
        mLogError("Failed to run CGetHostStatus::GetMediaNowKeyID(chNum,...)=("<<chNum<<",...).");
        return eInterfaceResCodeErrorLocalDataConvert;
    }
    if(strcmp(nowCaseid.c_str(),caseid.c_str()) == 0){
        //start
        return MediaStartAll(chNum,caseid);
    }else{
        //close--start
        mLogWarn("nowCaseid:"<<nowCaseid<<" != caseid:"<<caseid);
        //close
        InterfaceResCode resCode = MediaCloseAll(chNum);
        if(resCode != eInterfaceResCodeSuccess){
            mLogError("Failed to run CHostControl::MediaCloseAll(chNum)=("<<chNum<<").");
            return resCode;
        }
        //prepare
        resCode = MediaPrepareAll(chNum,caseid);
        if(resCode != eInterfaceResCodeSuccess){
            mLogError("Failed to run CHostControl::MediaPrepareAll(chNum,caseid)=("<<chNum<<","<<caseid<<").");
            return resCode;
        }
        //start
        return MediaStartAll(chNum,caseid);
    }
}

InterfaceResCode CHostControl::Running2Start(int chNum,string caseid)
{
    mLogDebug("CHostControl::Running2Start(caseid)=("<<caseid<<")...");
    string nowCaseid;
    if(chNum < 1){
        mLogError("Failed params:chNum="<<chNum<<" < 1.");
        return eInterfaceResCodeRequErrorParam;
    }
    if(_hostStatus.GetMediaNowKeyID(chNum-1,nowCaseid) == false){
        mLogError("Failed to run CGetHostStatus::GetMediaNowKeyID(chNum,...)=("<<chNum<<",...).");
        return eInterfaceResCodeErrorLocalDataConvert;
    }
    if(strcmp(nowCaseid.c_str(),caseid.c_str()) == 0){
        //arealy start same keyid
        return eInterfaceResCodeSuccess;
    }else{
        //stop--close--start
        mLogWarn("nowCaseid:"<<nowCaseid<<" != caseid:"<<caseid);
        //stop--close主通道
        InterfaceResCode resCode = MediaStopAll(chNum,true);
        if(resCode != eInterfaceResCodeSuccess){
            mLogError("Failed to run CHostControl::MediaStopAll(chNum,closeFlag)=("<<chNum<<",true).");
            return resCode;
        }
        //prepare
        resCode = MediaPrepareAll(chNum,caseid);
        if(resCode != eInterfaceResCodeSuccess){
            mLogError("Failed to run CHostControl::MediaPrepareAll(chNum,caseid)=("<<chNum<<","<<caseid<<").");
            return resCode;
        }
        //start
        return MediaStartAll(chNum,caseid);
    }
}

InterfaceResCode CHostControl::Pause2Start(int chNum,string caseid)
{
    mLogDebug("CHostControl::Pause2Start(caseid)=("<<caseid<<")...");
    mLogError("Host status :Pause,can not start.");
    return eInterfaceResCodeErrorLocalStatus;
}

InterfaceResCode CHostControl::Stop2Start(int chNum,string caseid)
{
    mLogDebug("CHostControl::Stop2Start(caseid)=("<<caseid<<")...");
    //close
    InterfaceResCode resCode = MediaCloseAll(chNum);
    if(resCode != eInterfaceResCodeSuccess){
        mLogError("Failed to run CHostControl::MediaCloseAll(chNum)=("<<chNum<<").");
        return resCode;
    }
    //prepare
    resCode = MediaPrepareAll(chNum,caseid);
    if(resCode != eInterfaceResCodeSuccess){
        mLogError("Failed to run CHostControl::MediaPrepareAll(chNum,caseid)=("<<chNum<<","<<caseid<<").");
        return resCode;
    }
    //start
    return MediaStartAll(chNum,caseid);
}

InterfaceResCode CHostControl::Close2Stop(int chNum)
{
    mLogDebug("CHostControl::Close2Stop(chNum,...)=("<<chNum<<",...)...");
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControl::Ready2Stop(int chNum)
{
    mLogDebug("CHostControl::Ready2Stop(chNum,...)=("<<chNum<<",...)...");

    return MediaCloseAll(chNum);
}

InterfaceResCode CHostControl::Running2Stop(int chNum)
{
    mLogDebug("CHostControl::Running2Stop(chNum,...)=("<<chNum<<",...)...");

    return MediaStopAll(chNum);
}

InterfaceResCode CHostControl::Pause2Stop(int chNum)
{
    mLogDebug("CHostControl::Pause2Stop(chNum,...)=("<<chNum<<",...)...");
    mLogError("Host status :Pause,can not stop.");
    return eInterfaceResCodeErrorLocalStatus;
}

InterfaceResCode CHostControl::Stop2Stop(int chNum)
{
    mLogDebug("CHostControl::Stop2Stop(chNum,...)=("<<chNum<<",...)...");

    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControl::Close2Close(int chNum)
{
    mLogDebug("CHostControl::Close2Close(chNum,...)=("<<chNum<<",...)...");
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControl::Ready2Close(int chNum)
{
    mLogDebug("CHostControl::Ready2Close(chNum,...)=("<<chNum<<",...)...");
    return MediaCloseAll(chNum);
}

InterfaceResCode CHostControl::Running2Close(int chNum)
{
    mLogDebug("CHostControl::Running2Close(chNum,...)=("<<chNum<<",...)...");

    InterfaceResCode resCode = MediaStopAll(chNum,true);
    if(resCode != eInterfaceResCodeSuccess){
        mLogError("Failed to run CHostControl::MediaCloseAll().");
        return resCode;
    }
}

InterfaceResCode CHostControl::Pause2Close(int chNum)
{
    mLogDebug("CHostControl::Pause2Close(chNum,...)=("<<chNum<<",...)...");
    mLogError("Host status :Pause,can not close.");
    return eInterfaceResCodeErrorLocalStatus;
}

InterfaceResCode CHostControl::Stop2Close(int chNum)
{
    mLogDebug("CHostControl::Stop2Close(chNum,...)=("<<chNum<<",...)...");
    //close
    return MediaCloseAll(chNum);
}

InterfaceResCode CHostControl::MediaPrepareAll(int chNum,string caseId)
{
    mLogInfo("打开设备通道的视频流...");
    mLogInfo("chNum:"<<chNum<<",caseID:"<<caseId);

    MPSOperationRes opResCode = eMPSResultOK;
    string resId;
    ResponseCode resCode = _client->MediaPrepare(chNum,caseId,opResCode,resId);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaPrepare(chNum,caseId,...)=("<<chNum<<","<<caseId<<",...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    return eInterfaceResCodeSuccess;
}


InterfaceResCode CHostControl::MediaStartAll(int chNum,string caseId)
{
    mLogInfo("开始设备通道的视频录制...");
    mLogInfo("chNum:"<<chNum<<",caseID:"<<caseId);
    MPSOperationRes opResCode = eMPSResultOK;
    //开始录制主通道
    ResponseCode resCode = _client->MediaStart(chNum,caseId,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaStart(chNum,caseId,...)=("<<chNum<<","<<caseId<<",...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

#ifdef START_STOP_PROOF_ALONE_CONTROL
    //开始录制证据
    mLogInfo("MPSClient::StartProofSave(...)");
    resCode = _client->StartProofSave(opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("StartProofSave(...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }
#endif
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControl::MediaStopAll(int chNum)
{
    mLogInfo("停止设备通道的视频录制...");
    mLogInfo("chNum:"<<chNum);

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode;
    resCode = _client->MediaStop(chNum,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaStop(chNum,...)=("<<chNum<<",...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControl::MediaStopAll(int chNum, bool closeFlag)
{
    mLogInfo("停止设备通道的视频录制...");
    mLogInfo("chNum:"<<chNum<<",closeFlag:"<<closeFlag);

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode;
    resCode = _client->MediaStop(chNum,closeFlag,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaStop(chNum,closeFlag,...)=("<<chNum<<","<<closeFlag<<",...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }
    return eInterfaceResCodeSuccess;
}

InterfaceResCode CHostControl::MediaCloseAll(int chNum)
{
    mLogInfo("关闭设备通道的视频流...");
    mLogInfo("chNum:"<<chNum);

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaClose(chNum,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaClose(chNum,...)=("<<chNum<<",...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }
    return eInterfaceResCodeSuccess;
}

