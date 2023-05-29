#include "interfaceProcess.h"
#include "interfaceDefines.h"
#include "interfaceParams.h"
#include "InterfaceResponse.h"

#include "method/CSetWebMultiCtrl.h"
#include "method/CGetWebHostConfig.h"
#include "method/CSetDecCtrl.h"
#include "method/CSetPutNoteFileCtrl.h"
#include "method/CSetOsdCtrl.h"
#include "method/CSetPowerCtrl.h"
#include "method/CStreamCtrl.h"
#include "method/CDisciplineCtrl.h"
#include "method/CSetRecCtrl.h"
#include "method/CSetWebRecCtrl.h"
#include "method/CSetWebAudioCtrl.h"
#include "method/CSetWebEffectCtrl.h"
#include "method/CSetWebExtendCtrl.h"
#include "method/CGetHostDecInfo.h"
#include "method/CGetHostChnInfo.h"
#include "method/CGetWebHostChnInfo.h"
#include "method/CGetHostStatus.h"
#include "method/GetTaskContent.h"
#include "method/CGetChannelContents.h"
#include "method/CGetHostConfig.h"
#include "method/CSetTaskDelete.h"
#include "method/CSendConsoleCmd.h"
#include "method/CSendSerialCmd.h"
#include "method/CHostControlDec.h"
#include "method/CHostControlDiscipline.h"
#include "method/CHostControl.h"
#include "method/CSetpoint.h"
//add new method 
//by libx in 2022/02/23
//--[BEGIN]--
#include "method/GetWebInfraredStudyList.h"
#include "method/SetWebInfraredStudy.h"
#include "method/SetWebInfraredDel.h"
#include "method/SetWebInfraredSend.h"
#include "method/SetWebInfraredCreate.h"
#include "method/GetWebAudioConf.h"
#include "method/SetWebAudioConf.h"
#include "method/GetWebAudioGroupConf.h"
#include "method/SetWebAudioGroupConf.h"
#include "method/GetWebAudioMatrixConf.h"
#include "method/SetWebAudioMatrixConf.h"
#include "method/GetWebVideoParam.h"
#include "method/SetWebVideoParam.h"
#include "method/GetWebAudioParam.h"
#include "method/SetWebAudioParam.h"
#include "method/GetWebRtspParam.h"
#include "method/SetWebRtspParam.h"
#include "method/GetWebRtmpParam.h"
#include "method/SetWebRtmpParam.h"
#include "method/GetWebLogFile.h"
#include "method/SetWebLogFileConf.h"
#include "method/SetWebDevID.h"
#include "method/SetWebRtmpBroadConf.h"
#include "method/SetWebFirmUpdate.h"
#include "method/SetWebZKTUpdate.h"
#include "method/SetWebZKTDownload.h"
#include "method/SetWebConfigUpdate.h"
#include "method/SetWebDiscipFileUpdate.h"
#include "method/SetWebBackupCfgFile.h"
#include "method/SetWebRecoverCFGFile.h"
#include "method/MultiPartyOpera.h"
//--[END]--

//add by ltz in 2022/02/24
//--[BEGIN]--
#include "method/LoginInterface.h"
#include "method/CGetWebAddrList.h"
#include "method/CGetWebVideoChnList.h"
#include "method/CSetWebFtpConf.h"
#include "method/CSetWebNewMosaic.h"
#include "method/CSetWebNewWhine.h"
#include "method/CSetWebStorageConf.h"
#include "method/CSetWebSyncTime.h"
#include "method/CSetWebUartConf.h"
#include "method/CSetWebUserPassword.h"
#include "method/CSetWebVideoChnConf.h"
#include "method/CSetWebVideoOutPutConf.h"
#include "method/CSetWebWhine.h"
#include "method/CGetSerialCmd.h"
#include "method/CGetWebCaptionList.h"
#include "method/CGetWebMICGain.h"
#include "method/CGetWebMosaicList.h"
#include "method/CGetWebWhineList.h"
#include "method/CSetWebMICGain.h"
//--[END]--
//add by ltz in 2022/03/02
#include "method/CGetWebFtpConf.h"
#include "method/CGetWebUartConf.h"
#include "method/CGetWebUserPassword.h"
#include "method/CGetWebStorageConf.h"
#include "method/CGetWebDevID.h"
#include "method/CGetWebVideoOutPutConf.h"
#include "method/GetWebInfraredFileInf.h"
#include "method/SetWebInfraredFileDel.h"
#include "method/CSetWebMICWhineButton.h"
#include "method/SetWebSystermShoutDown.h"
#include "method/CGetWebMICWhineButton.h"
#include "method/SetWebVideoMatrix.h"
#include "method/GetWebVideoMatrix.h"
#include "method/CGetAudioDefultGain.h"
#include "method/SetWebDSPMatrix.h"
#include "method/GetWebDSPMatrix.h"
#include "method/CSetWebStartAutoRec.h"
#include "method/CGetWebStartAutoRec.h"
#include "method/CSetWebStartRec.h"
#include "method/CGetWebStartRec.h"
#include "method/GetHostDecinformation.h"
#include "method/CWebAudioMute.h"

//--[END]--
InterfaceProcess::InterfaceProcess(string ip, int port, int timeOut)
    : _ip(ip), _port(port), _timeOut(timeOut), _client(new MPSClient(ip, port, timeOut)), _debugMode(false)
{
    mLogDebug("ip:" << this->_ip << ",port:" << this->_port << ",timeOut:" << this->_timeOut);
}

string InterfaceProcess::processRequest(string &requestStr)
{
    InterfaceResCode resCode = eInterfaceResCodeSuccess;
    InterfaceParams params;
    interfaceResponse response;
    do
    {
        //分析获取请求数据
        if (!params.analysisParams(requestStr))
        {
            mLogError("Failed to run params.analysisParams(requestStr)");
            resCode = eInterfaceResCodeRequErrorParam;
            break;
        }
        string sMethod;
        if (!params.getMethod(sMethod))
        {
            mLogError("get params error node:" << REQUEST_NODE_METHOD);
            resCode = eInterfaceResCodeRequErrorParam;
            break;
        }

        //printf("method.c_str %s\n", requestStr.c_str());
        mLogInfo("request method:" << sMethod);
        if (!strcmp(sMethod.c_str(), "_version"))
        {
            GetHostStatus status;
            string sResult;
            resCode = status.GetVersion(sResult);
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_HOST_INIT))
        {
            //hostInit 打开设备指定通道的视频流
            mLogInfo("hostInit 打开设备指定通道的视频流");
            int chNum;
            if (!params.getParamsNodeValueofInt(HOSTNIT_PARAMS_CHANNELNUM, chNum))
            {
                mLogError("get params error node:" << HOSTNIT_PARAMS_CHANNELNUM);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            
            string caseId;
            if (!params.getParamsNodeValueofStr(HOSTNIT_PARAMS_CASEID, caseId))
            {
                mLogError("get params error node:" << HOSTNIT_PARAMS_CASEID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!checkCaseIdisEnable(caseId))
            {
                mLogError("Failed to run checkCaseIdisEnable(caseId)=(" << caseId << ")");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!checkChannelNumisEnable(chNum))
            {
                mLogError("Failed to run checkChannelNumisEnable(chNum)=(" << chNum << ")");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CHostControl hostControl(this->_ip);
            resCode = hostControl.HostInit(chNum, caseId);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_HOST_START))
        {
            //hostStart 开始设备指定通道的视频录制
            mLogInfo("hostStart 开始设备指定通道的视频录制");
            int chNum;
            if (!params.getParamsNodeValueofInt(HOSTNIT_PARAMS_CHANNELNUM, chNum))
            {
                mLogError("get params error node:" << HOSTNIT_PARAMS_CHANNELNUM);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            string caseId;
            if (!params.getParamsNodeValueofStr(HOSTNIT_PARAMS_CASEID, caseId))
            {
                mLogError("get params error node:" << HOSTNIT_PARAMS_CASEID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!checkCaseIdisEnable(caseId))
            {
                mLogError("Failed to run checkCaseIdisEnable(caseId)=(" << caseId << ")");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!checkChannelNumisEnable(chNum))
            {
                mLogError("Failed to run checkChannelNumisEnable(chNum)=(" << chNum << ")");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CHostControl hostControl(this->_ip);
            resCode = hostControl.HostStart(chNum, caseId);
            //            resCode = this->HostStartMethod(caseId);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_HOST_STOP))
        {
            //hostStop 停止设备指定通道的视频录制
            mLogInfo("hostStop 停止设备指定通道的视频录制");
            int chNum;
            if (!params.getParamsNodeValueofInt(HOSTNIT_PARAMS_CHANNELNUM, chNum))
            {
                mLogError("get params error node:" << HOSTNIT_PARAMS_CHANNELNUM);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!checkChannelNumisEnable(chNum))
            {
                mLogError("Failed to run checkChannelNumisEnable(chNum)=(" << chNum << ")");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CHostControl hostControl(this->_ip);
            resCode = hostControl.HostStop(chNum);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_HOST_CLOSE))
        {
            //hostClose 关闭设备指定通道的视频流
            mLogInfo("hostClose 关闭设备指定通道的视频流");
            int chNum;
            if (!params.getParamsNodeValueofInt(HOSTNIT_PARAMS_CHANNELNUM, chNum))
            {
                mLogError("get params error node:" << HOSTNIT_PARAMS_CHANNELNUM);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!checkChannelNumisEnable(chNum))
            {
                mLogError("Failed to run checkChannelNumisEnable(chNum)=(" << chNum << ")");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CHostControl hostControl(this->_ip);
            resCode = hostControl.HostClose(chNum);
            //            resCode = this->HostCloseMethod();
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_HOST_STATUS))
        {
            //hostStatus 获取设备指定通道的直播状态
            mLogInfo("hostStatus 获取设备指定通道的直播状态");
            int chNum;
            if (!params.getParamsNodeValueofInt(HOSTNIT_PARAMS_CHANNELNUM, chNum))
            {
                mLogError("get params error node:" << HOSTNIT_PARAMS_CHANNELNUM);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!checkChannelNumisEnable(chNum))
            {
                mLogError("Failed to run checkChannelNumisEnable(chNum)=(" << chNum << ")");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            GetHostStatus hostStatus;
            string sResult;
            resCode = hostStatus.GethostStatus(chNum, sResult);
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_HOST_SHUTDOWN))
        {
            //shutdown 关闭主机
            mLogError("没有此方法:" << sMethod);
            resCode = eInterfaceResCodeReqNoMethod;
            break;
            mLogInfo("shutdown 关闭主机");
            CHostControl hostControl(this->_ip);
            resCode = hostControl.HostShutdown();
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_HOST_REBOOT))
        {
            //reboot 重启主机
            mLogInfo("reboot 重启主机");
            CHostControl hostControl(this->_ip);
            resCode = hostControl.HostReboot();
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_HOST_SETPOINT))
        {
            //setpoint 视频打点
            mLogError("没有此方法:" << sMethod);
            resCode = eInterfaceResCodeReqNoMethod;
            break;
            //*
            mLogInfo("setpoint 视频打点");
            int chNum;
            if (!params.getParamsNodeValueofInt(HOSTNIT_PARAMS_CHANNELNUM, chNum))
            {
                mLogError("get params error node:" << HOSTNIT_PARAMS_CHANNELNUM);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CSetpoint setPoint(this->_ip);
            string sResult;
            resCode = setPoint.Setpoint(sResult, chNum);
            response.setResponseResults(sResult);
            break;
            //*/
        }
        else if (!strcmp(sMethod.c_str(), METHOD_GET_CHANNEL_CONTENTS))
        {
            //getChannelContents 获取指定通道时间段内内容列表
            mLogInfo("getChannelContents 获取指定通道时间段内内容列表");

            string startTime, endTime;
            if (!params.getParamsNodeValueofStr(GET_CHANNEL_CONTENTS_PARAMS_START_DATE, startTime))
            {
                mLogError("get params error node:" << GET_CHANNEL_CONTENTS_PARAMS_START_DATE);
                startTime = "0000-01-01";
                //                resCode = eInterfaceResCodeRequErrorParam;
                //                break;
            }
            if (!params.getParamsNodeValueofStr(GET_CHANNEL_CONTENTS_PARAMS_END_DATE, endTime))
            {
                mLogError("get params error node:" << GET_CHANNEL_CONTENTS_PARAMS_END_DATE);
                //                resCode = eInterfaceResCodeRequErrorParam;
                //                break;
                endTime = "9999-12-31";
            }

            CGetChannelContents getChContents(this->_ip);
            string sResult;
            resCode = getChContents.GetChannelContents(startTime, endTime, sResult);
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_GET_TASK_CONTENTS))
        {
            //getTaskContents 获取指定任务的内容列表
            mLogInfo("getTaskContents 获取指定任务的内容列表");
            string caseId;
            if (!params.getParamsNodeValueofStr(GET_TESK_CONTENTS_PARAMS_CASEID, caseId))
            {
                mLogError("get params error node:" << GET_TESK_CONTENTS_PARAMS_CASEID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            string sResult;
            resCode = this->GetTaskContentMethod(caseId, sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_GET_HOST_CONFIG))
        {
            //getHostConfig
            mLogInfo("getHostConfig 获取主机网络配置和通道配置信息");
            CGetHostConfig getHostConfig(this->_ip);
            string sResult;
            resCode = getHostConfig.GetHostConfig(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
     /*   else if (!strcmp(sMethod.c_str(), METHOD_GET_HOST_STATUS))
        {
            //getHostDecInfo
            int DecId;
            mLogInfo("getHostDecInfo 获取解码通道配置信息");
            if (!params.getParamsNodeValueofInt(HOST_DEC_STATUS_PARAMS_CHANNEL, DecId))
            {
                mLogError("get params error node:" << HOST_DEC_STATUS_PARAMS_CHANNEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CGetHostDecInfo getHostDecInfo(this->_ip);
            string sResult;
            resCode = getHostDecInfo.GetHostDecInfo(sResult, DecId-2);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }*/
 /*       
        else if (!strcmp(sMethod.c_str(), METHOD_GET_HOST_CHNINFO))
        {
            //getHostChnInfo
            int ChnNum;
            mLogInfo("getHostChnInfo 获取解码通道配置信息");
            if (!params.getParamsNodeValueofInt(HOST_DEC_INFO_PARAMS_CHN_CHANNEL, ChnNum))
            {
                mLogError("get params error node:" << HOST_DEC_INFO_PARAMS_CHN_CHANNEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CGetHostChnInfo getHostChnInfo(this->_ip);
            string sResult;
            resCode = getHostChnInfo.GetHostChnInfo(sResult, ChnNum);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
	*/	
        else if (!strcmp(sMethod.c_str(), METHOD_SEND_HTTP_CONTENT))
        {
            //getHostChnInfo
            string url,content;
            mLogInfo("sendSerialPortRetransHttpContent 发送HttpContent");
            if (!params.getParamsNodeValueofStr(SEND_HTTP_CONTENT_DSTURL, url))
            {
                mLogError("get params error node:" << SEND_HTTP_CONTENT_DSTURL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofStr(SEND_HTTP_CONTENT_CONTENT, content))
            {
                mLogError("get params error node:" << SEND_HTTP_CONTENT_CONTENT);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CGetHostChnInfo getHostChnInfo(this->_ip);
            string sResult;
            resCode = getHostChnInfo.SendHttpContent(sResult, url, content);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_HOST_START_DEC))
        {
            //setdecstart
            int Dectype = 0, Channal, CacheTime;
            string url;
            bool TcpMode;
            mLogInfo("sethostdecstart");
            if (!params.getParamsNodeValueofInt(HOST_START_DEC_PARAMS_CHANNEL, Channal))
            {
                mLogError("set params error node:" << HOST_START_DEC_PARAMS_CHANNEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(HOST_START_DEC_PARAMS_URL, url))
            {
                mLogError("set params error node:" << HOST_START_DEC_PARAMS_URL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(HOST_START_DEC_PARAMS_CACHE_TIME, CacheTime))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_CACHETIME);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofBool(HOST_START_DEC_PARAMS_TCP_MODE, TcpMode))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_TCPMODE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CSetDecCtrl setDecCtrl(this->_ip);
            string sResult;
            resCode = setDecCtrl.SetDecCtrlStart(sResult, Dectype, Channal-2, CacheTime, url, TcpMode);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_HOST_STOP_DEC))
        {
            //setdecstop
            int Dectype = 0, Channal;
            mLogInfo("setdecstop");
            if (!params.getParamsNodeValueofInt(HOST_STOP_DEC_PARAMS_CHANNEL, Channal))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_CHANNEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSetDecCtrl setDecCtrl(this->_ip);
            string sResult;
            resCode = setDecCtrl.SetDecCtrlStop(sResult, Dectype, Channal-2);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_HOST_ADDPIC_DEC))
        {
            //setdecstop
            int Channal;
            string imgPath;
            mLogInfo("setdecstop");
            if (!params.getParamsNodeValueofInt(HOST_STOP_DEC_PARAMS_CHANNEL, Channal))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_CHANNEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            
            if (!params.getParamsNodeValueofStr(HOST_START_DEC_PARAMS_IMGPATH, imgPath))
            {
                mLogError("set params error node:" << HOST_START_DEC_PARAMS_IMGPATH);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSetDecCtrl setDecCtrl(this->_ip);
            string sResult;
            resCode = setDecCtrl.SetDecCtrlAddPic(sResult, Channal-2, imgPath);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_HOST_CLRPIC_DEC))
        {
            //setdecstop
            int Channal;
            mLogInfo("setdecstop");
            if (!params.getParamsNodeValueofInt(HOST_STOP_DEC_PARAMS_CHANNEL, Channal))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_CHANNEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSetDecCtrl setDecCtrl(this->_ip);
            string sResult;
            resCode = setDecCtrl.SetDecCtrlClrPic(sResult, Channal-2);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_START_DEC))
        {
            //setdecstart
            int Dectype, Channal, CacheTime;
            string sAddr;
            bool TcpMode;
            mLogInfo("setdecstart");
            if (!params.getParamsNodeValueofInt(SET_START_DEC_PARAMS_DECTYPE, Dectype))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_DECTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_START_DEC_PARAMS_CHANNEL, Channal))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_CHANNEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_START_DEC_PARAMS_ADDR, sAddr))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_ADDR);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_START_DEC_PARAMS_CACHETIME, CacheTime))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_CACHETIME);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofBool(SET_START_DEC_PARAMS_TCPMODE, TcpMode))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_TCPMODE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CSetDecCtrl setDecCtrl(this->_ip);
            string sResult;
            resCode = setDecCtrl.SetDecCtrlStart(sResult, Dectype, Channal, CacheTime, sAddr, TcpMode);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_STOP_DEC))
        {
            //setdecstop
            int Dectype, Channal;
            mLogInfo("setdecstop");
            if (!params.getParamsNodeValueofInt(SET_START_DEC_PARAMS_DECTYPE, Dectype))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_DECTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_START_DEC_PARAMS_CHANNEL, Channal))
            {
                mLogError("set params error node:" << SET_START_DEC_PARAMS_CHANNEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSetDecCtrl setDecCtrl(this->_ip);
            string sResult;
            resCode = setDecCtrl.SetDecCtrlStop(sResult, Dectype, Channal);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_START_REC))
        {
            //setrecstart
            int RecMode,ChannelID;
            string TaskId, Title;
            mLogInfo("setrecstart");
            if (!params.getParamsNodeValueofInt(SET_START_REC_PARAMS_RECMODE, RecMode))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_RECMODE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_START_REC_PARAMS_CHNAME, ChannelID))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_CHNAME);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_START_REC_PARAMS_TASKID, TaskId))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_TASKID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_START_REC_PARAMS_DESRIPTOR, Title))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_DESRIPTOR);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetRecCtrl setRecCtrl(this->_ip);
            string sResult;
            resCode = setRecCtrl.SetRecCtrlStart(sResult, RecMode, ChannelID, TaskId, Title);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_STOP_REC))
        {
            //setrecstop
            int RecMode;
            string TaskId;
            mLogInfo("setrecstop");
            if (!params.getParamsNodeValueofInt(SET_START_REC_PARAMS_RECMODE, RecMode))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_RECMODE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_START_REC_PARAMS_TASKID, TaskId))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_TASKID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetRecCtrl setRecCtrl(this->_ip);
            string sResult;
            resCode = setRecCtrl.SetRecCtrlStop(sResult, RecMode, TaskId);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_GET_INFOLIST_REC))
        {
            //setrecstart
            int GetMode;
            string TaskId,StartTime,EndTime;
            mLogInfo("GetRecInfo");
            if (!params.getParamsNodeValueofInt(GET_INFOLIST_REC_PARAMS_GETMODE, GetMode))
            {
                mLogError("set params error node:" << GET_INFOLIST_REC_PARAMS_GETMODE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(GET_INFOLIST_REC_PARAMS_TASKID, TaskId))
            {
                mLogError("set params error node:" << GET_INFOLIST_REC_PARAMS_TASKID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(GET_INFOLIST_REC_PARAMS_STARTTIME, StartTime))
            {
                mLogError("set params error node:" << GET_INFOLIST_REC_PARAMS_STARTTIME);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(GET_INFOLIST_REC_PARAMS_ENDTIME, EndTime))
            {
                mLogError("set params error node:" << GET_INFOLIST_REC_PARAMS_STARTTIME);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetRecCtrl setRecCtrl(this->_ip);
            string sResult;
            resCode = setRecCtrl.GetRecCtrlInfoList(sResult, GetMode, TaskId, StartTime, EndTime);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_TASKDEL_REC))
        {
            //SetTaskDelete
            string TaskId;
            mLogInfo("SetTaskDelete");
            if (!params.getParamsNodeValueofStr(SET_START_REC_PARAMS_TASKID, TaskId))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_TASKID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSetRecCtrl setRecCtrl(this->_ip);
            string sResult;
            resCode = setRecCtrl.SetRecCtrlTaskDel(sResult, TaskId);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), "SetStartDiscipline"))
        {
            bool LoopFlag;
            int  PlayList;
			mLogInfo("SetStartDiscipline");
            if (!params.getParamsNodeValueofBool(SET_START_DISCIPLINE_PARAMS_LOOPFLAG, LoopFlag))
            {
                mLogError("set params error node:" << SET_START_DISCIPLINE_PARAMS_LOOPFLAG);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_START_DISCIPLINE_PARAMS_PLAYLIST, PlayList))
            {
                mLogError("set params error node:" << SET_START_DISCIPLINE_PARAMS_PLAYLIST);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CDiscipLineCtrl DiscipLineCtrl(this->_ip);
            string sResult;
            resCode = DiscipLineCtrl.SetDiscipLineCtrlStart(sResult,LoopFlag,PlayList);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), "SetStopDiscipline"))
        {
			mLogInfo("SetStopDiscipline");
            CDiscipLineCtrl DiscipLineCtrl(this->_ip);
            string sResult;
            resCode = DiscipLineCtrl.SetDiscipLineCtrlStop(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), "GetDisciplineStatus"))
        {
			mLogInfo("GetDisciplineStatus");
            CDiscipLineCtrl DiscipLineCtrl(this->_ip);
            string sResult;
            resCode = DiscipLineCtrl.GetDiscipLineCtrlStatus(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_RTMPPUSH_STREAM))
        {
            //SetRtmpCliPush
            int RtmpID, Channel;
            string Addr;
            mLogInfo("SetRtmpCliPush");
            if (!params.getParamsNodeValueofInt(SET_START_REC_PARAMS_RTMPID, RtmpID))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_RTMPID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofStr(SET_START_REC_PARAMS_RTMPADDR, Addr))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_RTMPADDR);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofInt(SET_START_REC_PARAMS_RTMPCHANNEL, Channel))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_RTMPCHANNEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CStreamCtrl StreamCtrl(this->_ip);
            string sResult;
            resCode = StreamCtrl.SetStreamCtrlRtmpPush(sResult, RtmpID, Channel, Addr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_OPEN_STREAM))
        {
            //setrecstop
            bool AllFlag;
            int ChannelID;
            mLogInfo("setrecstop");
            if (!params.getParamsNodeValueofBool(SET_START_REC_PARAMS_OPENALLFLAG, AllFlag))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_OPENALLFLAG);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_START_REC_PARAMS_OPENCHANNELID, ChannelID))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_OPENCHANNELID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CStreamCtrl StreamCtrl(this->_ip);
            string sResult;
            resCode = StreamCtrl.SetStreamCtrlOpen(sResult, AllFlag, ChannelID);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_CLOSE_STREAM))
        {
            //setrecstop
            bool AllFlag;
            int ChannelID;
            mLogInfo("setrecstop");
            if (!params.getParamsNodeValueofBool(SET_START_REC_PARAMS_CLOSEALLFLAG, AllFlag))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_CLOSEALLFLAG);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_START_REC_PARAMS_CLOSECHANNELID, ChannelID))
            {
                mLogError("set params error node:" << SET_START_REC_PARAMS_CLOSECHANNELID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CStreamCtrl StreamCtrl(this->_ip);
            string sResult;
            resCode = StreamCtrl.SetStreamCtrlClose(sResult, AllFlag, ChannelID);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_CONTROL_POWER))
        {
            //setrecstop
            int Cmd;
            mLogInfo("setrecstop");
            if (!params.getParamsNodeValueofInt(SET_START_POWER_PARAMS_CMD, Cmd))
            {
                mLogError("set params error node:" << SET_START_POWER_PARAMS_CMD);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSetPowerCtrl PowerCtrl(this->_ip);
            string sResult;
            resCode = PowerCtrl.SetPowerCtrlCmd(sResult, Cmd);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), "SetConsoleCmd"))
        {
            //setrecstop
            string Cmd;
            mLogInfo("SetConsoleCmd");
            if (!params.getParamsNodeValueofStr(SEND_CONSOLE_PARAMS_CMD, Cmd))
            {
                mLogError("set params error node:" << SEND_CONSOLE_PARAMS_CMD);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSendConsoleCmd CSendConsoleCmd(this->_ip);
            string sResult;
            resCode = CSendConsoleCmd.SendConsoleCmd(sResult, Cmd);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SEND_SERIAL_CMD))
        {
            //setrecstop
            
            int ChnNum,CtrlType;
			double Val;
            mLogInfo("SetSerialCmd");
            if (!params.getParamsNodeValueofInt(AUDIO_CHN_CONF_PARAMS_CTRLTYPE, CtrlType))
            {
                mLogError("set params error node:" << AUDIO_CHN_CONF_PARAMS_CTRLTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(AUDIO_CHN_CONF_PARAMS_CHNNUM, ChnNum))
            {
                mLogError("set params error node:" << AUDIO_CHN_CONF_PARAMS_CHNNUM);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofDbl(AUDIO_CHN_CONF_PARAMS_VAL, Val))
            {
                mLogError("set params error node:" << AUDIO_CHN_CONF_PARAMS_VAL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSendSerialCmd CSendSerialCmd(this->_ip);
            string sResult;
            resCode = CSendSerialCmd.SendSerialCmd(sResult, Val, ChnNum, CtrlType);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_START_OSD))
        {
            //setTaskDelete 标记caseid为可删除
            mLogInfo("METHOD_SET_START_OSD 标记caseid为可删除");

            string Title;
            int OsdMode, ChannelID;
            mLogInfo("SetOsd");
            if (!params.getParamsNodeValueofInt(SET_CONFIG_ADD_OSD_OSDMODE, OsdMode))
            {
                mLogError("set params error node:" << SET_CONFIG_ADD_OSD_OSDMODE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_CONFIG_ADD_OSD_CHANNELID, ChannelID))
            {
                mLogError("set params error node:" << SET_CONFIG_ADD_OSD_OSDMODE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_CONFIG_ADD_OSD_TITLE, Title))
            {
                mLogError("get params error node:" << SET_CONFIG_ADD_OSD_TITLE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CSetOsdCtrl CSetOsdCtrl(this->_ip);
            string sResult;
            resCode = CSetOsdCtrl.SetOsdCtrlStart(sResult, OsdMode, ChannelID, Title);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_STOP_OSD))
        {
            //setTaskDelete 标记caseid为可删除
            mLogInfo("METHOD_SET_STOP_OSD 标记caseid为可删除");

            int OsdMode, ChannelID;
            mLogInfo("DelOsd");
            if (!params.getParamsNodeValueofInt(SET_CONFIG_DEL_OSD_OSDMODE, OsdMode))
            {
                mLogError("set params error node:" << SET_CONFIG_ADD_OSD_OSDMODE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_CONFIG_DEL_OSD_CHANNELID, ChannelID))
            {
                mLogError("set params error node:" << SET_CONFIG_ADD_OSD_OSDMODE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSetOsdCtrl CSetOsdCtrl(this->_ip);
            string sResult;
            resCode = CSetOsdCtrl.SetOsdCtrlStop(sResult, OsdMode, ChannelID);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_PUTNOTEFILE))
        {
            mLogInfo("METHOD_SET_PUTNOTEFILE 标记caseid为可删除");

            string TrialId, Content,FileName;
            int FileSize;
            if (!params.getParamsNodeValueofStr(SET_CONFIG_PUTNOTEFILE_TRIALID, TrialId))
            {
                mLogError("get params error node:" << SET_CONFIG_PUTNOTEFILE_TRIALID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_CONFIG_PUTNOTEFILE_CONTENT, Content))
            {
                mLogError("get params error node:" << SET_CONFIG_PUTNOTEFILE_CONTENT);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_CONFIG_PUTNOTEFILE_FILENAME, FileName))
            {
                mLogError("get params error node:" << SET_CONFIG_PUTNOTEFILE_FILENAME);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_CONFIG_PUTNOTEFILE_FILESIZE, FileSize))
            {
                mLogError("get params error node:" << SET_CONFIG_PUTNOTEFILE_FILESIZE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSetPutNoteFileCtrl CSetPutNoteFileCtrl(this->_ip);
            string sResult;
            resCode = CSetPutNoteFileCtrl.SetPutNoteFileCtrlStart(sResult,TrialId,Content,FileName,FileSize);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_TASK_DELETE))
        {
            //setTaskDelete 标记caseid为可删除
            mLogInfo("setTaskDelete 标记caseid为可删除");

            string caseId;
            if (!params.getParamsNodeValueofStr(GET_TESK_CONTENTS_PARAMS_CASEID, caseId))
            {
                mLogError("get params error node:" << GET_TESK_CONTENTS_PARAMS_CASEID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            CSetTaskDelete getHostConfig(this->_ip);
            resCode = getHostConfig.SetTaskDelete(caseId);
            break;
        }
        //webinterface add
        else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_HOST_CONFIG))
        {
            //WebGetHostConfig
            mLogInfo("GetWebHostConfig");
            CGetWebHostConfig getWebHostConfig(this->_ip);
            string sResult;
            resCode = getWebHostConfig.GetWebHostConfig(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_HOST_CHNINFO))
        {
            //getWebHostChnInfo
            int ChnNum;
            mLogInfo("getWebHostChnInfo");
            if (!params.getParamsNodeValueofInt(HOST_DEC_INFO_PARAMS_CHN_CHANNEL, ChnNum))
            {
                mLogError("get params error node:" << HOST_DEC_INFO_PARAMS_CHN_CHANNEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CGetWebHostChnInfo getWebHostChnInfo(this->_ip);
            string sResult;
            resCode = getWebHostChnInfo.GetWebHostChnInfo(sResult, ChnNum);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_REC))
        {
            //setWebrec
            int OnOff,ChnID,CurrentState;
            string TaskID;
            mLogInfo("SetWebRecord");
            if (!params.getParamsNodeValueofInt(SET_WEB_REC_PARAMS_CHANNELID, ChnID))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_CHANNELID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			
            if (!params.getParamsNodeValueofInt(SET_WEB_REC_PARAMS_SWITCHONOFF, OnOff))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_SWITCHONOFF);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			
			if (!params.getParamsNodeValueofInt("CurrentState", CurrentState))
            {
                mLogError("set params error node:" << "CurrentState");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			
            if (!params.getParamsNodeValueofStr(GET_REC_CONFIG_START_TASKID, TaskID))
            {
                mLogError("set params error node:" << GET_REC_CONFIG_START_TASKID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebRecCtrl setWebRecCtrl(this->_ip);
            string sResult;
            resCode = setWebRecCtrl.SetWebRecCtrl(sResult, ChnID, OnOff, TaskID, CurrentState);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_INFOLIST_REC))
        {
            //setrecstart
            int PageSize,PageNum,GetMode;
            string TaskId,StartTime,EndTime;
            mLogInfo("GetWebRecInfo");
            if (!params.getParamsNodeValueofStr(GET_INFOLIST_REC_PARAMS_TASKID, TaskId))
            {
                mLogError("set params error node:" << GET_INFOLIST_REC_PARAMS_TASKID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(GET_INFOLIST_REC_PARAMS_STARTTIME, StartTime))
            {
                mLogError("set params error node:" << GET_INFOLIST_REC_PARAMS_STARTTIME);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(GET_INFOLIST_REC_PARAMS_ENDTIME, EndTime))
            {
                mLogError("set params error node:" << GET_INFOLIST_REC_PARAMS_STARTTIME);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("GetMode", GetMode))
            {
                mLogError("set params error node:" << "GetMode");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("PageSize", PageSize))
            {
                mLogError("set params error node:" << "PageSize");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("PageNum", PageNum))
            {
                mLogError("set params error node:" << "PageNum");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebRecCtrl setWebRecCtrl(this->_ip);
            string sResult;
            resCode = setWebRecCtrl.GetWebRecCtrlInfoList(sResult, TaskId, StartTime, EndTime, PageSize, PageNum, GetMode);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_SEGMODEL_REC))
        {
            //setWebrec
            int ChnID;
            string SegModel;
            mLogInfo("setrecsegmodel");
            if (!params.getParamsNodeValueofInt(SET_WEB_REC_PARAMS_CHANNELID, ChnID))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_CHANNELID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_REC_PARAMS_SEGMODEL, SegModel))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_SEGMODEL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebRecCtrl setWebRecCtrl(this->_ip);
            string sResult;
            resCode = setWebRecCtrl.SetWebRecSegModel(sResult, ChnID, SegModel);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_SEGMODELSELECT_REC))
        {
            //setWebrec
            int ChnID, WinID, VideoID;
            mLogInfo("setrecsegmodel");
            if (!params.getParamsNodeValueofInt(SET_WEB_REC_PARAMS_CHANNELID, ChnID))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_CHANNELID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_REC_PARAMS_WINDOWID, WinID))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_WINDOWID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_REC_PARAMS_VIDEOSOURCEID, VideoID))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_VIDEOSOURCEID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebRecCtrl setWebRecCtrl(this->_ip);
            string sResult;
            resCode = setWebRecCtrl.SetWebRecSegModelSelect(sResult, ChnID, WinID, VideoID);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_MATRIX_REC))
        {
            //setWebrec
            int OnOff, WinID, VideoID;
            mLogInfo("setrecsegmodel");
            if (!params.getParamsNodeValueofInt(SET_WEB_REC_PARAMS_SWITCHONOFF, OnOff))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_SWITCHONOFF);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_REC_PARAMS_WINDOWID, WinID))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_WINDOWID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_REC_PARAMS_VIDEOSOURCEID, VideoID))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_VIDEOSOURCEID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebRecCtrl setWebRecCtrl(this->_ip);
            string sResult;
            resCode = setWebRecCtrl.SetWebRecMatrix(sResult, OnOff, WinID, VideoID);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_STARTDEC_MULTI))
        {
            //SetWebStartDec
            int DecType, DecID;
            string DecUrl;
            mLogInfo("SetWebStartDec");
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECTYPE, DecType))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECID, DecID))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_DECURL, DecUrl))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECURL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebMultiCtrl setWebMultiCtrl(this->_ip);
            string sResult;
            resCode = setWebMultiCtrl.SetWebMultiCtrlStart(sResult, DecType, DecID, DecUrl);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_STOPDEC_MULTI))
        {
            //SetWebStopDec
            int DecType, DecID;
            mLogInfo("SetWebStopDec");
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECTYPE, DecType))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECID, DecID))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebMultiCtrl setWebMultiCtrl(this->_ip);
            string sResult;
            resCode = setWebMultiCtrl.SetWebMultiCtrlStop(sResult, DecType, DecID);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_MICINPUT_AUDIO))
        {
            //SetWebMICInputVol
            int VolSize, VolChn;
            mLogInfo("SetWebMICInputVol");
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLSIZE, VolSize))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLSIZE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLCHN, VolChn))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLCHN);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioCtrl setWebAudioCtrl(this->_ip);
            string sResult;
            resCode = setWebAudioCtrl.SetWebAudioCtrlMicInput(sResult, VolSize, VolChn);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_LINEINPUT_AUDIO))
        {
            //SetWebMICInputVol
            int VolSize, VolChn;
            mLogInfo("SetWebMICInputVol");
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLSIZE, VolSize))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLSIZE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLCHN, VolChn))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLCHN);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioCtrl setWebAudioCtrl(this->_ip);
            string sResult;
            resCode = setWebAudioCtrl.SetWebAudioCtrlLineInput(sResult, VolSize, VolChn);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_DECINPUT_AUDIO))
        {
            //SetWebMICInputVol
            int VolSize, VolChn;
            mLogInfo("SetWebMICInputVol");
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLSIZE, VolSize))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLSIZE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLCHN, VolChn))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLCHN);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioCtrl setWebAudioCtrl(this->_ip);
            string sResult;
            resCode = setWebAudioCtrl.SetWebAudioCtrlDecInput(sResult, VolSize, VolChn);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_BALANCEOUTPUT_AUDIO))
        {
            //SetWebMICInputVol
            int VolSize, VolChn;
            mLogInfo("SetWebMICInputVol");
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLSIZE, VolSize))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLSIZE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLCHN, VolChn))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLCHN);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioCtrl setWebAudioCtrl(this->_ip);
            string sResult;
            resCode = setWebAudioCtrl.SetWebAudioCtrlBalanceOutput(sResult, VolSize, VolChn);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_LINEOUTPUT_AUDIO))
        {
            //SetWebMICInputVol
            int VolSize, VolChn;
            mLogInfo("SetWebMICInputVol");
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLSIZE, VolSize))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLSIZE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLCHN, VolChn))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLCHN);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioCtrl setWebAudioCtrl(this->_ip);
            string sResult;
            resCode = setWebAudioCtrl.SetWebAudioCtrlLineOutput(sResult, VolSize, VolChn);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_DECOUTPUT_AUDIO))
        {
            //SetWebMICInputVol
            int VolSize, VolChn;
            mLogInfo("SetWebMICInputVol");
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLSIZE, VolSize))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLSIZE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_VOLCHN, VolChn))
            {
                mLogError("set params error node:" << SET_WEB_AUDIO_PARAMS_VOLCHN);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioCtrl setWebAudioCtrl(this->_ip);
            string sResult;
            resCode = setWebAudioCtrl.SetWebAudioCtrlDecOutput(sResult, VolSize, VolChn);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_NEWCAPTION_EFFECT))
        {
            
            int Chn, Post, Num, Type, OverlayDisplay, Color, FontSize, SelectType,TimeORText;
            string Content;
            mLogInfo("SetWebNewCaption");
            if (!params.getParamsNodeValueofInt(SET_WEB_EFFECT_PARAMS_VOERLAYCHN, Chn))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYCHN);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("OverlayDisplay", OverlayDisplay))
            {
                mLogError("set params error node:" << "OverlayDisplay");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_EFFECT_PARAMS_VOERLAYPOST, Post))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYPOST);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_EFFECT_PARAMS_VOERLAYNUM, Num))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYNUM);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_EFFECT_PARAMS_VOERLAYTYPE, Type))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT, Content))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("FontSize", FontSize))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYNUM);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("Color", Color))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("TimeORText", TimeORText))
            {
                mLogError("set params error node:" << "TimeORText");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("SelectType", SelectType))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebEffectCtrl setWebEffectCtrl(this->_ip);
            string sResult;
            resCode = setWebEffectCtrl.SetWebEffectCtrlNew(sResult, Chn, Post, Num, Type, Content, OverlayDisplay, Color, FontSize, SelectType,TimeORText);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_CAPTION_EFFECT))
        {
            
            int Chn, Post, Num, Type, OverlayDisplay, FontSize, Color, SelectType, TimeORText;
            string Content;
            mLogInfo("SetWebCaption");
            if (!params.getParamsNodeValueofInt(SET_WEB_EFFECT_PARAMS_VOERLAYCHN, Chn))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYCHN);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("OverlayDisplay", OverlayDisplay))
            {
                mLogError("set params error node:" << "OverlayDisplay");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_EFFECT_PARAMS_VOERLAYPOST, Post))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYPOST);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_EFFECT_PARAMS_VOERLAYNUM, Num))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYNUM);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_EFFECT_PARAMS_VOERLAYTYPE, Type))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("Color", Color))
            {
                mLogError("set params error node:" << "Color");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("FontSize", FontSize))
            {
                mLogError("set params error node:" << "FontSize");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("TimeORText", TimeORText))
            {
                mLogError("set params error node:" << "TimeORText");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofInt("SelectType", SelectType))
            {
                mLogError("set params error node:" << "SelectType");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT, Content))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }


            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebEffectCtrl setWebEffectCtrl(this->_ip);
            string sResult;
            resCode = setWebEffectCtrl.SetWebEffectCtrlNew(sResult, Chn, Post, Num, Type, Content, OverlayDisplay, Color, FontSize, SelectType,TimeORText);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_NETCFG_EXTEND))
        {
            string Ip, GateWay, NetMask, MacAddr, MainDNS, BackupDNS;
            mLogInfo("SetWebNetConf");

            if (!params.getParamsNodeValueofStr(SET_WEB_EXTEND_PARAMS_IP, Ip))
            {
                mLogError("set params error node:" << SET_WEB_EXTEND_PARAMS_IP);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_EXTEND_PARAMS_GATEWAY, GateWay))
            {
                mLogError("set params error node:" << SET_WEB_EXTEND_PARAMS_GATEWAY);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_EXTEND_PARAMS_NETMASK, NetMask))
            {
                mLogError("set params error node:" << SET_WEB_EXTEND_PARAMS_NETMASK);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_EXTEND_PARAMS_MACADDR, MacAddr))
            {
                mLogError("set params error node:" << SET_WEB_EXTEND_PARAMS_MACADDR);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_EXTEND_PARAMS_MAINDNS, MainDNS))
            {
                mLogError("set params error node:" << SET_WEB_EXTEND_PARAMS_MAINDNS);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_EXTEND_PARAMS_BACKUPDNS, BackupDNS))
            {
                mLogError("set params error node:" << SET_WEB_EXTEND_PARAMS_BACKUPDNS);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebExtendCtrl setWebExtendCtrl(this->_ip);
            string sResult;
            resCode = setWebExtendCtrl.SetWebExtendCtrlNetCfg(sResult, Ip, GateWay, NetMask, MacAddr, MainDNS, BackupDNS);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        //else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_SYNCTIME_EXTEND))
        //{
        //    string SyncTime;
        //    mLogInfo("SetWebSyncTime");

        //    if (!params.getParamsNodeValueofStr(SET_WEB_EXTEND_PARAMS_SYNCTIME, SyncTime))
        //    {
        //        mLogError("set params error node:" << SET_WEB_EXTEND_PARAMS_SYNCTIME);
        //        resCode = eInterfaceResCodeRequErrorParam;
        //        break;
        //    }
        //    
        //    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
        //    CSetWebExtendCtrl setWebExtendCtrl(this->_ip);
        //    string sResult;
        //    resCode = setWebExtendCtrl.SetWebExtendCtrlSyncTime(sResult, SyncTime);
        //    //设置返回信息 内容列表
        //    response.setResponseResults(sResult);
        //    break;
        //}
        //add new method
        //by libx in 2022/02/23
        //--[BEGIN]--
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_DEV_ID_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    std::string devId = "";
            if (!params.getParamsNodeValueofStr(SET_WEB_DEV_ID_PARAMS_DEVID, devId))
            {
                mLogError("Method: " << sMethod.c_str() << " parse params failed: no DevID");
            }

		    CSetWebDevID setWebDevID(this->_ip);
		    std::string sResult;
		    resCode = setWebDevID.SetWebDevID(sResult, devId);
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_RTMP_BROAD_CONF_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    int isEnabled = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_RTMP_BROAD_CONF_PARAMS_ISENABLED, isEnabled))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no IsEnabled");
		    }

			std::string broadAddr = "";
			if (!params.getParamsNodeValueofStr(SET_WEB_RTMP_BROAD_CONF_PARAMS_BROADADDR, broadAddr))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no BroadAddr");
			}

		    CSetWebRtmpBroadConf setWebRtmpBroadConf(this->_ip);
		    std::string sResult;
		    resCode = setWebRtmpBroadConf.SetWebRtmpBroadConf(sResult, isEnabled, broadAddr);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), "GetWebRtmpBroadConf"))
        {
            mLogInfo("GetWebRtmpBroadConf");
		    CSetWebRtmpBroadConf GetWebRtmpBroadConf(this->_ip);
		    std::string sResult;
		    resCode = GetWebRtmpBroadConf.GetWebRtmpBroadConf(sResult);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_FIRM_UPDATE_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    std::string firmUpdate = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_FIRM_UPDATE_PARAMS_FIRMUPDATE, firmUpdate))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no FirmUpdate");
		    }

		    CSetWebFirmUpdate setWebFirmUpdate(this->_ip);
		    std::string sResult;
		    resCode = setWebFirmUpdate.SetWebFirmUpdate(sResult, firmUpdate);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_ZKT_UPDATE_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    std::string zKTUpdate = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_ZKT_UPDATE_PARAMS_ZKTUPDATE, zKTUpdate))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no ZKTUpdate");
		    }

		    CSetWebZKTUpdate setWebZKTUpdate(this->_ip);
		    std::string sResult;
		    resCode = setWebZKTUpdate.SetWebZKTUpdate(sResult, zKTUpdate);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_ZKT_DOWNLOAD_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    std::string zKTUpdate = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_ZKT_DOWNLOAD_PARAMS_ZKTUPDATE, zKTUpdate))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no ZKTUpdate");
		    }

		    CSetWebZKTDownload setWebZKTDownload(this->_ip);
		    std::string sResult;
		    resCode = setWebZKTDownload.SetWebZKTDownload(sResult, zKTUpdate);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_CONFIG_UPDATE_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    std::string cfgUpdate = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_ZKT_DOWNLOAD_PARAMS_ZKTUPDATE, cfgUpdate))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no CfgUpdate");
		    }

		    CSetWebConfigUpdate setWebConfigUpdate(this->_ip);
		    std::string sResult;
		    resCode = setWebConfigUpdate.SetWebConfigUpdate(sResult, cfgUpdate);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_DISCIPFILE_UPDATE_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    std::string discipFileUpdate = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_DISCIPFILE_UPDATE_PARAMS_DISCIPFILEUPDATE, discipFileUpdate))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no discipFileUpdate");
		    }

		    CSetWebDiscipFileUpdate setWebDiscipFileUpdate(this->_ip);
		    std::string sResult;
		    resCode = setWebDiscipFileUpdate.SetWebDiscipFileUpdate(sResult, discipFileUpdate);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_BACKUP_CFGFILE_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    std::string backUPCfg = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_DISCIPFILE_UPDATE_PARAMS_DISCIPFILEUPDATE, backUPCfg))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no BackUPCfg");
		    }

		    CSetWebBackupCfgFile setWebBackupCfgFile(this->_ip);
		    std::string sResult;
		    resCode = setWebBackupCfgFile.SetWebBackupCfgFile(sResult, backUPCfg);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_RECOVER_CFGFILE_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    std::string recoverCFG = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_RECOVER_CFGFILE_PARAMS_RECOVERCFG, recoverCFG))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no RecoverCFG");
		    }

		    CSetWebRecoverCFGFile setWebRecoverCFGFile(this->_ip);
		    std::string sResult;
		    resCode = setWebRecoverCFGFile.SetWebRecoverCFGFile(sResult, recoverCFG);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_LOG_FILE_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    CGetWebLogFile getWebLogFile(this->_ip);
		    std::string sResult;
		    resCode = getWebLogFile.GetWebLogFile(sResult);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_LOG_FILE_CONF_EXTEND)) 
        {
            mLogInfo(sMethod.c_str());
		    int logSize = 0;
		    if (!params.getParamsNodeValueofInt("LogPartitionSize", logSize))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no LogSize");
		    }

			int logSaveTime = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_LOG_FILE_CONF_PARAMS_LOGSAVETIME, logSaveTime))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no LogSaveTime");
			}

			int logLevel = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_LOG_FILE_CONF_PARAMS_LOGLEVEL, logLevel))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no LogLevel");
			}

		    CSetWebLogFileConf setWebLogFileConf(this->_ip);
		    std::string sResult;
		    resCode = setWebLogFileConf.SetWebLogFileConf(sResult, logSize, logSaveTime, logLevel);
		    response.setResponseResults(sResult);
		    break;
		}
        else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_VIDEO_PARAM_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    //2. 创建方法对象
		    CGetWebVideoParam getWebVideoParam(this->_ip);
		    string sResult;
		    //3. 执行方法
		    resCode = getWebVideoParam.GetWebVideoParam(sResult);
		    //4. 获取结果(响应)
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_VIDEO_PARAM_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    int chnNum = 0;
			
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_CHNNUM, chnNum))
            {
                mLogError("Method: " << METHOD_SET_WEB_VIDEO_PARAM_EXTEND << " parse params failed: no ChnNum");
            }
			string chnName;
            if (!params.getParamsNodeValueofStr(SET_WEB_VIDEO_PARAMS_CHNNAME, chnName))
            {
                mLogError("Method: " << METHOD_SET_WEB_VIDEO_PARAM_EXTEND << " parse params failed: no ChnName");
            }

			int resolutionSet = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_VIDEO_PARAMS_RESOLUTIONSET, resolutionSet))
			{
				mLogError("Method: " << METHOD_SET_WEB_VIDEO_PARAM_EXTEND << " parse params failed: no ResolutioSet");
			}

			int codeMode = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_VIDEO_PARAMS_CODEMODE, codeMode))
			{
				mLogError("Method: " << METHOD_SET_WEB_VIDEO_PARAM_EXTEND << " parse params failed: no CodeMode");
			}

			int rateType = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_VIDEO_PARAMS_RATETYPE, rateType))
			{
				mLogError("Method: " << METHOD_SET_WEB_VIDEO_PARAM_EXTEND << " parse params failed: no RateType");
			}

			int rateSize = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_VIDEO_PARAMS_RATESIZE, rateSize))
			{
				mLogError("Method: " << METHOD_SET_WEB_VIDEO_PARAM_EXTEND << " parse params failed: no RateSize");
			}

			int frameSet = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_VIDEO_PARAMS_FRAMESET, frameSet))
			{
				mLogError("Method: " << METHOD_SET_WEB_VIDEO_PARAM_EXTEND << " parse params failed: no FrameSet");
			}

			int gopSet = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_VIDEO_PARAMS_GOPSET, gopSet))
			{
				mLogError("Method: " << METHOD_SET_WEB_VIDEO_PARAM_EXTEND << " parse params failed: no GopSet");
			}

		    CSetWebVideoParam setWebVideoParam(this->_ip);
		    std::string sResult;
		    resCode = setWebVideoParam.SetWebVideoParam(sResult, chnNum, chnName, resolutionSet, codeMode, rateType, rateSize, frameSet, gopSet);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_AUDIO_PARAM_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    //2. 创建方法对象
		    CGetWebAudioParam getWebAudioParam(this->_ip);
		    string sResult;
		    //3. 执行方法
		    resCode = getWebAudioParam.GetWebAudioParam(sResult);
		    //4. 获取结果(响应)
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_AUDIO_PARAM_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    int soundNum = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_SOUNDNUM, soundNum))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no SoundNum");
		    }

		    std::string soundSource = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_AUDIO_PARAMS_SOUNDSOURCE, soundSource))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no SoundSource");
		    }

		    int codeMode = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_CODEMODE, codeMode))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no CodeMode");
		    }

		    int sampleRate = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_SAMPLERATE, sampleRate))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no SampleRate");
		    }

		    int rateSet = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_PARAMS_RATESET, rateSet))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no RateSet");
		    }

		    CSetWebAudioParam setWebAudioParam(this->_ip);
		    std::string sResult;
		    resCode = setWebAudioParam.SetWebAudioParam(sResult, soundNum, soundSource, codeMode, sampleRate, rateSet);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_RTSP_PARAM_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    //2. 创建方法对象
		    CGetWebRtspParam getWebRtspParam(this->_ip);
		    string sResult;
		    //3. 执行方法
		    resCode = getWebRtspParam.GetWebRtspParam(sResult);
		    //4. 获取结果(响应)
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), "SetWebRtspUserInfo"))
		{
            mLogInfo(sMethod.c_str());
			int ChannelNum,Encrypt;
			string UserName,Password;
			if (!params.getParamsNodeValueofInt("ChannelNum", ChannelNum))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no ChannelNum");
		    }

		    if (!params.getParamsNodeValueofStr("UserName", UserName))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no UserName");
		    }
			
			if (!params.getParamsNodeValueofStr("Password", Password))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no Password");
		    }

		    if (!params.getParamsNodeValueofInt("Encrypt", Encrypt))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no Encrypt");
		    }
		    //2. 创建方法对象
		    CGetWebRtspParam SetWebRtspUserInfo(this->_ip);
		    string sResult;
		    //3. 执行方法 
		    resCode = SetWebRtspUserInfo.SetWebRtspUserInfo(sResult, ChannelNum, UserName, Password, Encrypt);
		    //4. 获取结果(响应)
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_RTSP_PARAM_EXTEND))
		{
            mLogInfo(sMethod.c_str());
            int RtspNum = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_RTSP_PARAMS_RTSPNUM, RtspNum))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no SoundNum");
		    }

		    std::string rtspSource = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_RTSP_PARAMS_RTSPSOURCE, rtspSource))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no RtspSource");
		    }

		    std::string rtspUrl = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_RTSP_PARAMS_RTSPURL, rtspUrl))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no RtspUrl");
		    }

		    CSetWebRtspParam setWebRtspParam(this->_ip);
		    std::string sResult;
		    resCode = setWebRtspParam.SetWebRtspParam(sResult, RtspNum, rtspSource, rtspUrl);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_RTMP_PARAM_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    //2. 创建方法对象
		    CGetWebRtmpParam getWebRtmpParam(this->_ip);
		    string sResult;
		    //3. 执行方法
		    resCode = getWebRtmpParam.GetWebRtmpParam(sResult);
		    //4. 获取结果(响应)
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_RTMP_PARAM_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    int num = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_RTMP_PARAMS_NUM, num))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no Num");
		    }

		    std::string rtmpName = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_RTMP_PARAMS_RTMPNAME, rtmpName))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no RtmpName");
		    }

		    int isHttp = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_RTMP_PARAMS_ISHTTP, isHttp))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no IsHttp");
		    }

            int isHls = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_RTMP_PARAMS_ISHLS, isHls))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no IsHls");
		    }

            int isRtmp = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_RTMP_PARAMS_ISRTMP, isRtmp))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no IsRtmp");
		    }

            int isRtsp = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_RTMP_PARAMS_ISRTSP, isRtsp))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no IsRtsp");
			}

            int isBroadcast = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_RTMP_PARAMS_ISBROADCAST, isBroadcast))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no IsBroadcast");
		    }

		    std::string broadcastAddr = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_RTMP_PARAMS_BROADCASTADDR, broadcastAddr))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no BroadcastAddr");
		    }

            int isPushStream = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_RTMP_PARAMS_ISPUSHSTREAM, isPushStream))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no IsPushStream");
			}

			std::string pushAddr = "";
			if (!params.getParamsNodeValueofStr(SET_WEB_RTMP_PARAMS_PUSHADDR, pushAddr))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no PushAddr");
			}

		    CSetWebRtmpParam setWebRtmpParam(this->_ip);
		    std::string sResult;
		    resCode = setWebRtmpParam.SetWebRtmpParam(sResult, num, rtmpName, isHttp, isHls, isRtmp, isRtsp,
                isBroadcast, broadcastAddr, isPushStream, pushAddr);
		    response.setResponseResults(sResult);
		    break;
		}
        else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_AUDIO_CONF_EXTEND))
        {
            mLogInfo(sMethod.c_str());
            //1. 无参
			//2. 创建方法对象
			CGetWebAudioConf getWebAudioConfig(this->_ip);
			string sResult;
			//3. 执行方法
			resCode = getWebAudioConfig.GetWebAudioConf(sResult);
			//4. 获取结果(响应)
			response.setResponseResults(sResult);
			break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_AUDIO_CONF_EXTEND))
		{
            mLogInfo("SetWebAudioConf");

		    int isPower = 0;
            if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_CONF_PARAMS_ISPOWER, isPower))
            {
                mLogError("Method: " << METHOD_SET_WEB_AUDIO_CONF_EXTEND << " parse params failed: no IsPower");
            }

            int isIncentives = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_CONF_PARAMS_ISINCENTIVES, isIncentives))
			{
				mLogError("Method: " << METHOD_SET_WEB_AUDIO_CONF_EXTEND << " parse params failed: no IsIncentives");
			}

            int isFeedback = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_CONF_PARAMS_ISFEEDBACK, isFeedback))
			{
				mLogError("Method: " << METHOD_SET_WEB_AUDIO_CONF_EXTEND << " parse params failed: no IsFeedback");
			}

            int isAEC = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_CONF_PARAMS_ISAEC, isAEC))
			{
				mLogError("Method: " << METHOD_SET_WEB_AUDIO_CONF_EXTEND << " parse params failed: no IsAEC");
			}

            int isAGC = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_CONF_PARAMS_ISAGC, isAGC))
			{
				mLogError("Method: " << METHOD_SET_WEB_AUDIO_CONF_EXTEND << " parse params failed: no IsAGC");
			}

            int spl = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_CONF_PARAMS_SPL, spl))
			{
				mLogError("Method: " << METHOD_SET_WEB_AUDIO_CONF_EXTEND << " parse params failed: no SPL");
			}
            int IsANS = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_CONF_PARAMS_ISANS, IsANS))
			{
				mLogError("Method: " << METHOD_SET_WEB_AUDIO_CONF_EXTEND << " parse params failed: no SPL");
			}

		    CSetWebAudioConf setWebAudioConf(this->_ip);
		    std::string sResult;
		    resCode = setWebAudioConf.SetWebAudioConf(sResult, isPower, IsANS, isIncentives, isFeedback, isAEC, isAGC, spl);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_AUDIO_GROUP_CONF_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    //1. 无参
            //2. 创建方法对象
            CGetWebAudioGroupConf getWebAudioGroupConfig(this->_ip);
            string sResult;
            //3. 执行方法
            resCode = getWebAudioGroupConfig.GetWebAudioGroupConf(sResult);
            //4. 获取结果(响应)
            response.setResponseResults(sResult);
            break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_AUDIO_GROUP_CONF_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    int groupName = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_GROUPNAME, groupName))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_AUDIO_GROUP_CONF_EXTEND << " parse params failed: no GroupName");
		    }

		    std::string chnNum = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_CHNNUM, chnNum))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_AUDIO_GROUP_CONF_EXTEND << " parse params failed: no ChnNum");
		    }

		    CSetWebAudioGroupConf setWebAudioGroupConf(this->_ip);
		    std::string sResult;
		    resCode = setWebAudioGroupConf.SetWebAudioGroupConf(sResult, groupName, chnNum);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_AUDIO_MATRIX_CONF_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    //1. 无参
		    //2. 创建方法对象
		    CGetWebAudioMatrixConf getWebAudioMatrixConfig(this->_ip);
		    string sResult;
		    //3. 执行方法
		    resCode = getWebAudioMatrixConfig.GetWebAudioMatrixConf(sResult);
		    //4. 获取结果(响应)
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_AUDIO_MATRIX_CONF_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    std::string chnNum = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_AUDIO_MATRIX_CONFIG_PARAMS_CHNNUM, chnNum))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_AUDIO_MATRIX_CONF_EXTEND << " parse params failed: no ChnNum");
		    }

		    CSetWebAudioMatrixConf setWebAudioMatrixConf(this->_ip);
		    std::string sResult;
		    resCode = setWebAudioMatrixConf.SetWebAudioMatrixConf(sResult, chnNum);
		    response.setResponseResults(sResult);
		    break;
		}
        else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_INFRARED_STUDYLIST_EXTEND))
        {
            mLogInfo(sMethod.c_str());
		    int port = 0;
		    if (!params.getParamsNodeValueofInt(GET_WEB_INFRARED_STUDYLIST_PARAMS_PORT, port))
		    {
			    mLogError("Method: " << METHOD_GET_WEB_INFRARED_STUDYLIST_EXTEND << " parse params failed: no Port");
			    //这里是否应该直接break??
		    }

            std::string fileName = "";
			if (!params.getParamsNodeValueofStr(GET_WEB_INFRARED_STUDYLIST_PARAMS_FILENAME, fileName))
			{
				mLogError("Method: " << METHOD_GET_WEB_INFRARED_STUDYLIST_EXTEND << " parse params failed: no FileName");
				//这里是否应该直接break??
			}

            std::string sResult = "";
            CGetWebInfraredStudyList getWebInfraredStudyList(this->_ip);
            resCode = getWebInfraredStudyList.GetWebInfraredStudyList(sResult, port, fileName);
            response.setResponseResults(sResult);
            break;

        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_INFRARED_STUDY_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    int port = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_INFRARED_STUDY_PARAMS_PORT, port))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_STUDY_EXTEND << " parse params failed: no Port");
			    //这里是否应该直接break??
		    }

			int num = 0;
			if (!params.getParamsNodeValueofInt(SET_WEB_INFRARED_STUDY_PARAMS_NUM, num))
			{
				mLogError("Method: " << METHOD_SET_WEB_INFRARED_STUDY_EXTEND << " parse params failed: no Num");
				//这里是否应该直接break??
			}

		    std::string fileName = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_INFRARED_STUDY_PARAMS_FILENAME, fileName))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_STUDY_EXTEND << " parse params failed: no FileName");
			    //这里是否应该直接break??
		    }

			std::string order = "";
			if (!params.getParamsNodeValueofStr(SET_WEB_INFRARED_STUDY_PARAMS_ORDER, order))
			{
				mLogError("Method: " << METHOD_SET_WEB_INFRARED_STUDY_EXTEND << " parse params failed: no Order");
				//这里是否应该直接break??
			}

			std::string describe = "";
			if (!params.getParamsNodeValueofStr(SET_WEB_INFRARED_STUDY_PARAMS_DESCRIBE, describe))
			{
				mLogError("Method: " << METHOD_SET_WEB_INFRARED_STUDY_EXTEND << " parse params failed: no Describe");
				//这里是否应该直接break??
			}

		    std::string sResult = "";
		    CSetWebInfraredStudy setWebInfraredStudy(this->_ip);
		    resCode = setWebInfraredStudy.SetWebInfraredStudy(sResult, port, fileName, num, order, describe);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_INFRARED_DEL_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    int port = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_INFRARED_DEL_PARAMS_PORT, port))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_DEL_EXTEND << " parse params failed: no Port");
			    //这里是否应该直接break??
		    }

		    std::string fileName = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_INFRARED_DEL_PARAMS_FILENAME, fileName))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_DEL_EXTEND << " parse params failed: no FileName");
			    //这里是否应该直接break??
		    }

		    int num = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_INFRARED_DEL_PARAMS_NUM, num))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_DEL_EXTEND << " parse params failed: no Num");
			    //这里是否应该直接break??
		    }

		    std::string order = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_INFRARED_DEL_PARAMS_ORDER, order))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_DEL_EXTEND << " parse params failed: no Order");
			    //这里是否应该直接break??
		    }

		    std::string describe = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_INFRARED_DEL_PARAMS_DESCRIBE, describe))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_DEL_EXTEND << " parse params failed: no Describe");
			    //这里是否应该直接break??
		    }

		    std::string sResult = "";
		    CSetWebInfraredDel setWebInfraredDel(this->_ip);
		    resCode = setWebInfraredDel.SetWebInfraredDel(sResult, port, fileName, num, order, describe);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_INFRARED_SEND_EXTEND))
		{
            mLogInfo(sMethod.c_str());
		    int port = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_INFRARED_SEND_PARAMS_PORT, port))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_SEND_EXTEND << " parse params failed: no Port");
			    //这里是否应该直接break??
		    }

		    std::string fileName = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_INFRARED_SEND_PARAMS_FILENAME, fileName))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_SEND_EXTEND << " parse params failed: no FileName");
			    //这里是否应该直接break??
		    }

		    int num = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_INFRARED_SEND_PARAMS_NUM, num))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_SEND_EXTEND << " parse params failed: no Num");
			    //这里是否应该直接break??
		    }

		    std::string order = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_INFRARED_SEND_PARAMS_ORDER, order))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_SEND_EXTEND << " parse params failed: no Order");
			    //这里是否应该直接break??
		    }

		    std::string describe = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_INFRARED_SEND_PARAMS_DESCRIBE, describe))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_SEND_EXTEND << " parse params failed: no Describe");
			    //这里是否应该直接break??
		    }

		    std::string sResult = "";
		    CSetWebInfraredSend setWebInfraredSend(this->_ip);
		    resCode = setWebInfraredSend.SetWebInfraredSend(sResult, port, fileName, num, order, describe);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_INFRARED_CREATE_EXTEND))
		{
            mLogInfo(sMethod.c_str());
			string DevID,DevType,Manufacturer;
		    int port = 0;
		    if (!params.getParamsNodeValueofInt(SET_WEB_INFRARED_CREATE_PARAMS_PORT, port))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_CREATE_EXTEND << " parse params failed: no Port");
			    //这里是否应该直接break??
		    }

		    std::string fileName = "";
		    if (!params.getParamsNodeValueofStr(SET_WEB_INFRARED_CREATE_PARAMS_FILENAME, fileName))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_CREATE_EXTEND << " parse params failed: no FileName");
			    //这里是否应该直接break??
		    }
		    if (!params.getParamsNodeValueofStr(SET_WEB_DEV_ID_PARAMS_DEVID, DevID))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_CREATE_EXTEND << " parse params failed: no FileName");
			    //这里是否应该直接break??
		    }
		    if (!params.getParamsNodeValueofStr("DevType", DevType))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_CREATE_EXTEND << " parse params failed: no FileName");
			    //这里是否应该直接break??
		    }
		    if (!params.getParamsNodeValueofStr("Manufacturer", Manufacturer))
		    {
			    mLogError("Method: " << METHOD_SET_WEB_INFRARED_CREATE_EXTEND << " parse params failed: no FileName");
			    //这里是否应该直接break??
		    }

		    std::string sResult = "";
		    CSetWebInfraredCreate setWebInfraredCreate(this->_ip);
		    resCode = setWebInfraredCreate.SetWebInfraredCreate(sResult, port, fileName, DevID, DevType, Manufacturer);
		    response.setResponseResults(sResult);
		    break;
		}
        else if (!strcmp(sMethod.c_str(), METHOD_ADD_WEB_PARTY))
        {
		    //mLogInfo(sMethod.c_str());
			mLogInfo("AddWebParty");
            std::string name = "";
		    if (!params.getParamsNodeValueofStr(ADD_WEB_PARTY_PARAMS_NAME, name))
		    {
			    mLogError("Method: " << sMethod.c_str() << " parse params failed: no Name");
			    //这里是否应该直接break??
		    }

			std::string protocol = "";
			if (!params.getParamsNodeValueofStr(ADD_WEB_PARTY_PARAMS_PROTOCOL, protocol))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no Protocol");
				//这里是否应该直接break??
			}

			std::string address = "";
			if (!params.getParamsNodeValueofStr(ADD_WEB_PARTY_PARAMS_ADDRESS, address))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no Address");
				//这里是否应该直接break??
			}

			std::string bitRate = "";
			if (!params.getParamsNodeValueofStr(ADD_WEB_PARTY_PARAMS_BITRATE, bitRate))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no BitRate");
				//这里是否应该直接break??
			}

			std::string bufferLength = "";
			if (!params.getParamsNodeValueofStr(ADD_WEB_PARTY_PARAMS_BUFFERLENGTH, bufferLength))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no BufferLength");
				//这里是否应该直接break??
			}

			std::string transport = "";
			if (!params.getParamsNodeValueofStr(ADD_WEB_PARTY_PARAMS_TRANSPORT, transport))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no Transport");
				//这里是否应该直接break??
			}

		    std::string sResult = "";
		    CMultiPartyOpera multiPartyOpera(this->_ip);
		    resCode = multiPartyOpera.InsertOneParty(sResult, name, protocol, address, bitRate, bufferLength, transport);
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_DEL_WEB_PARTY))
		{
		    mLogInfo(sMethod.c_str());

			int id = 0;
			if (!params.getParamsNodeValueofInt(DEL_WEB_PARTY_PARAMS_ID, id))
			{
				mLogError("Method: " << sMethod.c_str() << " parse params failed: no ID");
				//这里是否应该直接break??
			}

		    std::string sResult = "";
            CMultiPartyOpera multiPartyOpera(this->_ip);
		    resCode = multiPartyOpera.DeleteOneParty(sResult, id);
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_GET_PARTY_LIST))
		{
            mLogInfo(sMethod.c_str());

		    std::string sResult = "";
            CMultiPartyOpera multiPartyOpera(this->_ip);
		    resCode = multiPartyOpera.GetPartyList(sResult);
		    response.setResponseResults(sResult);
		    break;
		}
        //--[END]--
        //add by ltz in 2022/02/24
        //--[BEGIN]--
        else if (!strcmp(sMethod.c_str(), METHOD_LOGIN_INTERFACE))
        {
            string name;
            string password;
            mLogInfo("METHOD_LOGIN_ADMIN");
            //获取params节点中"name"节点
            if (!params.getParamsNodeValueofStr(LOGIN_PARAMS_USERNAME, name))
            {
                mLogError("set params error node:" << "name");
                resCode = eInterfaceResCodeRequErrorParam;
                //resCode = eInterfaceResCodeError;
                break;
            }
            if (!params.getParamsNodeValueofStr(LOGIN_PARAMS_PASSWORD, password))
            {
                mLogError("set params error node:" << "password");
                resCode = eInterfaceResCodeRequErrorParam;
                //resCode = eInterfaceResCodeError;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            LoginInterface Login;
            string sResult;
            resCode = Login.LoginCheck(sResult, name, password);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_RTSPCONTACT_MULTI))
        {
            //SetWebRtspContact
            int DecMethod, DecID, DecFormat, DecMode, DecType;
            string DecUrl;
            mLogInfo("SetWebRtspContact");
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECMOTHOD, DecMethod))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECMOTHOD);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            /*
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECTYPE, DecType))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECMOTHOD);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            */
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECID, DecID))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECFORMAT, DecFormat))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECFORMAT);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECMODE, DecMode))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECMODE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_DECURL, DecUrl))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECURL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebMultiCtrl setWebMultiCtrl(this->_ip);
            string sResult;
            resCode = setWebMultiCtrl.SetWebMultiCtrlRtsp(sResult, DecMethod, DecID, DecMode, DecFormat, DecUrl);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_H323CONTACT_MULTI))
        {
            //SetWebRtspContact
            int DecType, DecID, VideoEncType, AudioEncType;
            string DecUrl, Contact, SendVideoSrc, SendAudioSrc;
            mLogInfo("SetWebH323Contact");
            /*
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECTYPE, DecType))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            */
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECID, DecID))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_VIDEOENCTYPE, VideoEncType))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_VIDEOENCTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_AUDIOENCTYPE, AudioEncType))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_AUDIOENCTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_DECURL, DecUrl))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECURL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_CONTACT, Contact))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_CONTACT);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_SENDVIDEOSRC, SendVideoSrc))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_SENDVIDEOSRC);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_SENDAUDIOSRC, SendAudioSrc))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_SENDAUDIOSRC);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebMultiCtrl setWebMultiCtrl(this->_ip);
            string sResult;
            resCode = setWebMultiCtrl.SetWebMultiCtrlH323(sResult, DecID, VideoEncType, AudioEncType, DecUrl, Contact, SendVideoSrc, SendAudioSrc);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_SIPCONTACT_MULTI))
        {
            //WebSetSipContact
            int DecType, DecID, DecMode;
            string DecUrl, UserName, UserPw;
            mLogInfo("SetWebSipContact");
            /*
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECTYPE, DecType))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }*/
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECMODE, DecMode))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECID, DecID))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_DECURL, DecUrl))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECURL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_USERNAME, UserName))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_USERNAME);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_USERPW, UserPw))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_USERPW);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebMultiCtrl setWebMultiCtrl(this->_ip);
            string sResult;
            resCode = setWebMultiCtrl.SetWebMultiCtrlSip(sResult, DecMode, DecID, DecUrl, UserName, UserPw);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_ADDRLIST_MULTI))
        {
            
            int DecType, DecMethod, Rate, LenBuf;
            string DecUrl, Title;
            mLogInfo("SetWebAddrList");
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECTYPE, DecType))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECTYPE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_DECMOTHOD, DecMethod))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECMOTHOD);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_RATE, Rate))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_RATE);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_MULTI_PARAMS_LENBUF, LenBuf))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_LENBUF);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_DECURL, DecUrl))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECURL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_MULTI_PARAMS_TITLE, Title))
            {
                mLogError("set params error node:" << SET_WEB_MULTI_PARAMS_DECURL);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebMultiCtrl setWebMultiCtrl(this->_ip);
            string sResult;
            resCode = setWebMultiCtrl.SetWebMultiCtrlSipList(sResult, DecType, DecMethod, Rate, LenBuf, DecUrl, Title);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        //else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_ADDR_LIST))
        //{
        //    //WebGetHostConfig
        //    mLogInfo("GetWebAddrList 获取添加后的通讯录列表信息");
        //    CGetWebAddrList getWebAddrList(this->_ip);
        //    string sResult;
        //    resCode = getWebAddrList.GetWebAddrList(sResult);
        //    //设置返回信息 内容列表
        //    response.setResponseResults(sResult);
        //    break;
        //}
//        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_MOSAIC))
//        {
//            int  MosaicDisplay, MosaicNum, MosaicType;
//			string MosaicChn,MosaicWinX,MosaicWinY,MosaicWinH,MosaicWinW;
//            mLogInfo("SetWebMosaic");
//            //获取params节点中DevID节点
//            if (!params.getParamsNodeValueofStr(SET_WEB_NEW_MOSAIC_PARAMS_MOSAICCHN, MosaicChn))
//            {
//                mLogError("set params error node:" << "MosaicChn");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
//            if (!params.getParamsNodeValueofInt("MosaicDisplay", MosaicDisplay))
//            {
//                mLogError("set params error node:" << "MosaicDisplay");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
//            if (!params.getParamsNodeValueofInt(SET_WEB_NEW_MOSAIC_PARAMS_MOSAICNUM, MosaicNum))
//            {
//                mLogError("set params error node:" << "MosaicNum");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
//            if (!params.getParamsNodeValueofInt(SET_WEB_NEW_MOSAIC_PARAMS_MOSAICTYPE, MosaicType))
//            {
//                mLogError("set params error node:" << "MosaicType");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
//			if (!params.getParamsNodeValueofStr("MosaicWinX", MosaicWinX))
//            {
//                mLogError("set params error node:" << "MosaicWinX");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
//            if (!params.getParamsNodeValueofStr("MosaicWinY", MosaicWinY))
//            {
//                mLogError("set params error node:" << "MosaicWinY");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
//            if (!params.getParamsNodeValueofStr("MosaicWinH", MosaicWinH))
//            {
//                mLogError("set params error node:" << "MosaicWinH");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
//            if (!params.getParamsNodeValueofStr("MosaicWinW", MosaicWinW))
//            {
//                mLogError("set params error node:" << "MosaicWinW");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
//            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
//            CSetWebNewMosaic setWebMosaic(this->_ip);
//            string sResult;
//            resCode = setWebMosaic.SetWebMosaic(sResult, MosaicChn, MosaicDisplay, MosaicNum, MosaicType, MosaicWinX, MosaicWinY, MosaicWinH, MosaicWinW);
//            //设置返回信息 内容列表
//            response.setResponseResults(sResult);
//            break;
//        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_NEW_WHINE))
        {
            int WhineChn, WhineType;
            mLogInfo("SetWebNewWhine");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt(SET_WEB_NEW_WHINE_PARAMS_WHINECHN, WhineChn))
            {
                mLogError("set params error node:" << "WhineChn");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_NEW_WHINE_PARAMS_WHINETYPE, WhineType))
            {
                mLogError("set params error node:" << "WhineType");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebNewWhine setWebNewWhine(this->_ip);
            string sResult;
            resCode = setWebNewWhine.SetWebNewWhine(sResult, WhineChn, WhineType);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_NEW_WHINE))
        {
            int WhineChn, WhineType;
            mLogInfo("GetWebNewWhine");
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebNewWhine getWebNewWhine(this->_ip);
            string sResult;
            resCode = getWebNewWhine.GetWebNewWhine(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_WHINE))
        {
            int WhineChn, WhineType;
            mLogInfo("METHOD_SET_WEB_WHINE");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt(SET_WEB_WHINE_PARAMS_WHINECHN, WhineChn))
            {
                mLogError("set params error node:" << "WhineChn");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_WHINE_PARAMS_WHINETYPE, WhineType))
            {
                mLogError("set params error node:" << "WhineType");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebWhine setWebWhine(this->_ip);
            string sResult;
            resCode = setWebWhine.SetWebWhine(sResult, WhineChn, WhineType);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_SYNC_TIME))
        {
            string NtpIp, SysTime;
			int Mode;
            mLogInfo("SetWebSyncTime");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofStr(SET_WEB_SYNC_TIME_PARAMS_NTPIP, NtpIp))
            {
                mLogError("set params error node:" << "NtpIp");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_WEB_SYNC_TIME_PARAMS_SYSTIME, SysTime))
            {
                mLogError("set params error node:" << "SysTime");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("Mode", Mode))
            {
                mLogError("set params error node:" << "Mode");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebSyncTime SetWebSyncTime(this->_ip);
            string sResult;
            resCode = SetWebSyncTime.SetWebSyncTime(sResult, NtpIp, SysTime, Mode);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_USER_PASSWORD))
        {
            string UserName, OldPassword, NewPassword, VerifyPassword;
            mLogInfo("SetWebUserPassword");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofStr(SET_USER_PASSWORD_PARAMS_USERNAME, UserName))
            {
                mLogError("set params error node:" << "UserName");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_USER_PASSWORD_PARAMS_OLDPASSWORD, OldPassword))
            {
                mLogError("set params error node:" << "OldPassword");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_USER_PASSWORD_PARAMS_NEWPASSWORD, NewPassword))
            {
                mLogError("set params error node:" << "NewPassword");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_USER_PASSWORD_PARAMS_VERIFYPASSWORD, VerifyPassword))
            {
                mLogError("set params error node:" << "VerifyPassword");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUserPassword setWebUserPassword(this->_ip);
            string sResult;
            resCode = setWebUserPassword.SetWebUserPassword(sResult, UserName, OldPassword, NewPassword, VerifyPassword);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_FTP_CONF))
        {
            string FtpServerIp, FtpName, FtpPassword, FtpPath;
            int FtpPort;
            mLogInfo("SetWebFtpConf");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofStr(SET_FTP_CONF_PARAMS_FTPSERVERIP, FtpServerIp))
            {
                mLogError("set params error node:" << "FtpServerIp");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_FTP_CONF_PARAMS_FTPPORT, FtpPort))
            {
                mLogError("set params error node:" << "FtpPort");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_FTP_CONF_PARAMS_FTPNAME, FtpName))
            {
                mLogError("set params error node:" << "FtpName");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_FTP_CONF_PARAMS_FTPPASSWORD, FtpPassword))
            {
                mLogError("set params error node:" << "FtpPassword");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SET_FTP_CONF_PARAMS_FTPPATH, FtpPath))
            {
                mLogError("set params error node:" << "FtpPath");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebFtpConf setWebFtpConf(this->_ip);
            string sResult;
            resCode = setWebFtpConf.SetWebFtpConf(sResult, FtpServerIp, FtpPort, FtpName, FtpPassword, FtpPath);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_UART_CONF))
        {
            int UartPort, BaudRate, DateBit, StopBit, ParityBit;
            mLogInfo("SetWebUartConf");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt(SET_WEB_UART_CONF_PARAMS_UARTPORT, UartPort))
            {
                mLogError("set params error node:" << "UartPort");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_UART_CONF_PARAMS_BAUDRATE, BaudRate))
            {
                mLogError("set params error node:" << "BaudRate");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_UART_CONF_PARAMS_DATEBIT, DateBit))
            {
                mLogError("set params error node:" << "DateBit");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_UART_CONF_PARAMS_STOPBIT, StopBit))
            {
                mLogError("set params error node:" << "StopBit");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_UART_CONF_PARAMS_PARITYBIT, ParityBit))
            {
                mLogError("set params error node:" << "ParityBit");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf setWebUartConf(this->_ip);
            string sResult;
            resCode = setWebUartConf.SetWebUartConf(sResult, UartPort, BaudRate, DateBit, StopBit, ParityBit);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_VIDEO_OUT_PUT_CONF))
        {
            int Luminance, Contrast, Hue, Satuature, OutpuResolution;
            string ChnName;
            mLogInfo("SetWebVideoOutPutConf");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofStr(SET_WEB_VIDEO_OUT_PUT_CONF_PARAMS_CHNNAME, ChnName))
            {
                mLogError("set params error node:" << "ChnName");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("Luminance", Luminance))
            {
                mLogError("set params error node:" << "Luminance");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("Contrast", Contrast))
            {
                mLogError("set params error node:" << "Contrast");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("Hue", Hue))
            {
                mLogError("set params error node:" << "Hue");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("Satuature", Satuature))
            {
                mLogError("set params error node:" << "Satuature");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofInt(SET_WEB_VIDEO_OUT_PUT_CONF_PARAMS_OUTPURESOLUTION, OutpuResolution))
            {
                mLogError("set params error node:" << "OutpuResolution");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebVideoOutPutConf setWebVideoOutPutConf(this->_ip);
            string sResult;
            resCode = setWebVideoOutPutConf.SetWebVideoOutPutConf(sResult, ChnName, Luminance, Contrast, Hue, Satuature, OutpuResolution);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_VIDEO_CHN_CONF))
        {
            int ChnPort, IsShow;
            string ChnName;
            mLogInfo("SetWebVideoChnConf");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofStr(SET_WEB_VIDEO_CHN_CONF_PARAMS_CHNNAME, ChnName))
            {
                mLogError("set params error node:" << "ChnName");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_VIDEO_CHN_CONF_PARAMS_CHNPORT, ChnPort))
            {
                mLogError("set params error node:" << "ChnPort");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_VIDEO_CHN_CONF_PARAMS_ISSHOW, IsShow))
            {
                mLogError("set params error node:" << "IsShow");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebVideoChnConf setWebVideoChnConf(this->_ip);
            string sResult;
            resCode = setWebVideoChnConf.SetWebVideoChnConf(sResult, ChnName, ChnPort, IsShow);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_VIDEO_CHN_LIST))
        {
            int ChnPort, IsShow;
            string ChnName;
            mLogInfo("GetWebVideoChnList");

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CGetWebVideoChnList getWebVideoChnList(this->_ip);
            string sResult;
            resCode = getWebVideoChnList.GetWebVideoChnList(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
        else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_STORAGE_CONF))
        {
            int AudioRecording, StorageThreshold, FileType, AutoDel, OverlayDate, SegSize;
            mLogInfo("SetWebStorageConf");
            //获取params节点中DevID节点

//		    if (!params.getParamsNodeValueofInt(GET_WEB_STORAGE_CONF_PARAMS_AUDIORECORDING, AudioRecording))
//            {
//                mLogError("set params error node:" << "AudioRecording");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
            if (!params.getParamsNodeValueofInt(GET_WEB_STORAGE_CONF_PARAMS_STORAGETHRESOLD, StorageThreshold))
            {
                mLogError("set params error node:" << "StorageThreshold");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(GET_WEB_STORAGE_CONF_PARAMS_FILETYPE, FileType))
            {
                mLogError("set params error node:" << "FileType");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(GET_WEB_STORAGE_CONF_PARAMS_AUTODEL, AutoDel))
            {
                mLogError("set params error node:" << "AutoDel");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			
//            if (!params.getParamsNodeValueofInt(GET_WEB_STORAGE_CONF_PARAMS_OVERLAYDATE, OverlayDate))
//            {
//                mLogError("set params error node:" << "OverlayDate");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
            if (!params.getParamsNodeValueofInt(GET_WEB_STORAGE_CONF_PARAMS_SEGSIZE, SegSize))
            {
                mLogError("set params error node:" << "SegSize");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebStorageConf setWebStorageConf(this->_ip);
            string sResult;
            resCode = setWebStorageConf.SetWebStorageConf(sResult, StorageThreshold, FileType, AutoDel, SegSize);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_CAPTION_LIST))
		{
		    mLogInfo("GetWebCaptionList");
			int OverlayChn,SelectType;
            if (!params.getParamsNodeValueofInt(SET_WEB_EFFECT_PARAMS_VOERLAYCHN, OverlayChn))
            {
                mLogError("set params error node:" << SET_WEB_EFFECT_PARAMS_VOERLAYCHN);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("SelectType", SelectType))
            {
                mLogError("set params error node:" << "SelectType");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebCaptionList getWebCaptionList(this->_ip);
		    string sResult;
		    resCode = getWebCaptionList.GetWebCaptionList(sResult, OverlayChn, SelectType);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_GET_SERIAL_CMD))
		{
		    int ChnNum,CtrlType;
		    mLogInfo("GetSerialCmd");
		    //获取params节点中DevID节点
		    if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
		    {
			    mLogError("set params error node:" << "ChnNum");
			    resCode = eInterfaceResCodeRequErrorParam;
			    break;
		    }
		    if (!params.getParamsNodeValueofInt("CtrlType", CtrlType))
		    {
			    mLogError("set params error node:" << "CtrlType");
			    resCode = eInterfaceResCodeRequErrorParam;
			    break;
		    }

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetSerialCmd getSerialCmd(this->_ip);
		    string sResult;
		    resCode = getSerialCmd.GetSerialCmd(sResult, ChnNum, CtrlType);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
		    }
//		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_MOSAIC_LIST))
//		    {
//		    mLogInfo("GetWebMosaicList");
//		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
//		    CGetWebMosaicList getWebMosaicList(this->_ip);
//		    string sResult;
//		    resCode = getWebMosaicList.GetWebMosaicList(sResult);
//		    //设置返回信息 内容列表
//		    response.setResponseResults(sResult);
//		    break;
//		}
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_MOSAIC_LIST))
		{
		    mLogInfo("GetWebMosaicList");
			int ChnnelID;
		    if (!params.getParamsNodeValueofInt("ChnnelID", ChnnelID))
		    {
			    mLogError("set params error node:" << "ChnnelID");
			    resCode = eInterfaceResCodeRequErrorParam;
			    break;
		    }
		    CGetWebMosaicList GetWebNewMosaicList(this->_ip);
		    string sResult;
		    resCode = GetWebNewMosaicList.GetWebNewMosaicList(sResult,ChnnelID);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_WHINE_LIST))
		{
		    int WhineChn, WhineType;
		    mLogInfo("GetWebWhineList");

		    if (!params.getParamsNodeValueofInt("WhineChn", WhineChn))
		    {
			    mLogError("set params error node:" << "SerialID");
			    resCode = eInterfaceResCodeRequErrorParam;
			    break;
		    }
		    if (!params.getParamsNodeValueofInt("WhineType", WhineType))
		    {
			    mLogError("set params error node:" << "WhineType");
			    resCode = eInterfaceResCodeRequErrorParam;
			    break;
		    }

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebWhineList getWebWhineList(this->_ip);
		    string sResult;
		    resCode = getWebWhineList.GetWebWhineList(sResult, WhineChn, WhineType);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
		}
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_MIC_GAIN))
		{
		    int MicVal,ChnNam;
		    mLogInfo("SetWebMICGain");

		    if (!params.getParamsNodeValueofInt("MicVal", MicVal))
		    {
			    mLogError("set params error node:" << "MicVal");
			    resCode = eInterfaceResCodeRequErrorParam;
			    break;
		    }
		    if (!params.getParamsNodeValueofInt("ChnNam", ChnNam))
		    {
			    mLogError("set params error node:" << "ChnNam");
			    resCode = eInterfaceResCodeRequErrorParam;
			    break;
		    }

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebMICGain setWebMICGain(this->_ip);
		    string sResult;
		    resCode = setWebMICGain.SetWebMICGain(sResult, MicVal, ChnNam);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
		}
        else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_MIC_GAIN))
        {
		    mLogInfo("GetWebMICGain");
			int ChnNam;
		    if (!params.getParamsNodeValueofInt("ChnNam", ChnNam))
		    {
			    mLogError("set params error node:" << "ChnNam");
			    resCode = eInterfaceResCodeRequErrorParam;
			    break;
		    }

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebMICGain getWebMICGain(this->_ip);
		    string sResult;
		    resCode = getWebMICGain.GetWebMICGain(sResult,ChnNam);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		//--[END]--
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_USER_PASSWORD))
        {
		    mLogInfo("GetWebUserPassword");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebUserPassword getWebUserPassword(this->_ip);
		    string sResult;
		    resCode = getWebUserPassword.GetWebUserPassword(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_FTP_CONF))
        {
		    mLogInfo("GetWebFtpConf");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebFtpConf getWebFtpConf(this->_ip);
		    string sResult;
		    resCode = getWebFtpConf.GetWebFtpConf(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_UART_CONF))
        {
		    mLogInfo("GetWebUartConf");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebUartConf getWebUartConf(this->_ip);
		    string sResult;
		    resCode = getWebUartConf.GetWebUartConf(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
	    else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_STORAGE_CONF))
        {
		    mLogInfo("GetWebStorageConf");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebStorageConf getWebStorageConf(this->_ip);
		    string sResult;
		    resCode = getWebStorageConf.GetWebStorageConf(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
	     else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_DEVID))
        {
		    mLogInfo("GetWebDevID");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebDevID getWebDevID(this->_ip);
		    string sResult;
		    resCode = getWebDevID.GetWebDevID(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_VIDEO_OUTPUT_CONF))
        {
		    mLogInfo("GetWebVideoOutPutConf");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebVideoOutPutConf getWebVideoOutPutConf(this->_ip);
		    string sResult;
		    resCode = getWebVideoOutPutConf.GetWebVideoOutPutConf(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_INFRARED_FILE_INF))
        {
		    mLogInfo("GetWebInfraredFileInf");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebInfraredFileInf getWebInfraredFileInf(this->_ip);
		    string sResult;
		    resCode = getWebInfraredFileInf.GetWebInfraredFileInf(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_INFRARED_FILE_DEL))
        {
		    mLogInfo("SetWebInfraredFileDel");
			string FileName;
            if (!params.getParamsNodeValueofStr("FileName", FileName))
            {
                mLogError("set params error node:" << "FileName");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebInfraredFileDel setWebInfraredFileDel(this->_ip);
		    string sResult;
		    resCode = setWebInfraredFileDel.SetWebInfraredFileDel(sResult,FileName);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_MIC_WHINE_BUTTON))
        {
		    mLogInfo("SetWebMICWhineButton");
			int ButtonVal,ChnName;
            if (!params.getParamsNodeValueofInt("ButtonVal", ButtonVal))
            {
                mLogError("set params error node:" << "ButtonVal");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("ChnName", ChnName))
            {
                mLogError("set params error node:" << "ChnName");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebMICWhineButton setWebMICWhineButton(this->_ip);
		    string sResult;
		    resCode = setWebMICWhineButton.SetWebMICWhineButton(sResult,ButtonVal,ChnName);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_SYSTERM_SHUTDOWN))
        {
		    mLogInfo("SetWebSystermShoutDown");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebSystermShoutDown setWebSystermShoutDown(this->_ip);
		    string sResult;
		    resCode = setWebSystermShoutDown.SetWebSystermShoutDown(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_REC_SEGMODEL))
        {
		    mLogInfo("GetWebRecSegModel");

			int ChannelID;
            if (!params.getParamsNodeValueofInt(SET_WEB_REC_PARAMS_CHANNELID, ChannelID))
            {
                mLogError("set params error node:" << SET_WEB_REC_PARAMS_CHANNELID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebRecCtrl getWebRecSegModel(this->_ip);
		    string sResult;
		    resCode = getWebRecSegModel.GetWebRecSegModel(sResult, ChannelID);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_MIC_WHINE_BUTTON))
        {
		    mLogInfo("GetWebMICWhineButton");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebMICWhineButton getWebMICWhineButton(this->_ip);
		    string sResult;
		    resCode = getWebMICWhineButton.GetWebMICWhineButton(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_VIDEO_MATRIX))
        {
		    mLogInfo("SetWebVideoMatrix");

			int ChnnelID;
			string ChnnelName;
            if (!params.getParamsNodeValueofInt("ChnnelID", ChnnelID))
            {
                mLogError("set params error node:" << "ChnnelID");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofStr("ChnnelName", ChnnelName))
            {
                mLogError("set params error node:" << "ChnnelName");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebVideoMatrix setWebVideoMatrix(this->_ip);
		    string sResult;
		    resCode = setWebVideoMatrix.SetWebVideoMatrix(sResult, ChnnelID, ChnnelName);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_VIDEO_MATRIX))
        {
		    mLogInfo("GetWebVideoMatrix");

			//printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebVideoMatrix getWebVideoMatrix(this->_ip);
		    string sResult;
		    resCode = getWebVideoMatrix.GetWebVideoMatrix(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_AUDIO_DEFAULT_GAIN))
        {
		    mLogInfo("GetAudioDefultGain");
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetAudioDefultGain getAudioDefultGain(this->_ip);
		    string sResult;
		    resCode = getAudioDefultGain.GetAudioDefultGain(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebAudioSQLCreatAndUpdate"))
        {
		    mLogInfo("SetWebAudioSQLCreatAndUpdate");
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetAudioDefultGain SetWebAudioSQLCreatAndUpdate(this->_ip);
		    string sResult;
		    resCode = SetWebAudioSQLCreatAndUpdate.SetWebAudioSQLCreatAndUpdate(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_DSP_MATRIX))
        {
		    mLogInfo("SetWebDSPMatrix");

			string InPutChnnel;
			int OutputType;
            if (!params.getParamsNodeValueofStr("InPutChnnel", InPutChnnel))
            {
                mLogError("set params error node:" << "InPutChnnel");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("OutputType", OutputType))
            {
                mLogError("set params error node:" << "OutputType");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebDSPMatrix setWebDSPMatrix(this->_ip);
		    string sResult;
		    resCode = setWebDSPMatrix.SetWebDSPMatrix(sResult, InPutChnnel, OutputType);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_DSP_MATRIX))
        {
		    mLogInfo("GetWebDSPMatrix");
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebDSPMatrix getWebDSPMatrix(this->_ip);
		    string sResult;
		    resCode = getWebDSPMatrix.GetWebDSPMatrix(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_START_REC))
        {
		    mLogInfo("SetWebStartRec");

			int VideoEncChnnel,IfRec;
            if (!params.getParamsNodeValueofInt("VideoEncChnnel", VideoEncChnnel))
            {
                mLogError("set params error node:" << "VideoEncChnnel");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("IfRec", IfRec))
            {
                mLogError("set params error node:" << "IfRec");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebStartRec setWebStartRec(this->_ip);
		    string sResult;
		    resCode = setWebStartRec.SetWebStartRec(sResult, VideoEncChnnel, IfRec);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_START_REC))
        {
		    mLogInfo("GetWebStartRec");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebStartRec getWebStartRec(this->_ip);
		    string sResult;
		    resCode = getWebStartRec.GetWebStartRec(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_START_AUTO_REC))
        {
		    mLogInfo("SetWebStartAutoRec");

			int StartRec;
            if (!params.getParamsNodeValueofInt("StartRec", StartRec))
            {
                mLogError("set params error node:" << "StartRec");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebStartAutoRec setWebStartAutoRec(this->_ip);
		    string sResult;
		    resCode = setWebStartAutoRec.SetWebStartAutoRec(sResult, StartRec);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_START_AUTO_REC))
        {
		    mLogInfo("GetWebStartAutoRec");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebStartAutoRec getWebStartAutoRec(this->_ip);
		    string sResult;
		    resCode = getWebStartAutoRec.GetWebStartAutoRec(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
	    else if (!strcmp(sMethod.c_str(), METHOD_GET_HOST_DECINFO))
        {
            //getHostDecInfo
            int DecId;
            mLogInfo("getHostDecInfo");
            if (!params.getParamsNodeValueofInt("DECID", DecId))
            { 
                mLogError("get params error node:" << "DECID");
                resCode = eInterfaceResCodeRequErrorParam;
                break; 
            }

            CGetHostDecinformation getHostDecinformation(this->_ip);
            string sResult;
            resCode = getHostDecinformation.GetHostDecinformation(sResult, DecId);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_NEW_WHINE_BUTTON))
        {
            int WhineChn, WhineButton;
            mLogInfo("SetWebNewWhineButton");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt(SET_WEB_NEW_WHINE_PARAMS_WHINECHN, WhineChn))
            {
                mLogError("set params error node:" << "WhineChn");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("WhineButton", WhineButton))
            {
                mLogError("set params error node:" << "WhineButton");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebNewWhine setWebNewWhineButton(this->_ip);
            string sResult;
            resCode = setWebNewWhineButton.SetWebNewWhineButton(sResult, WhineChn, WhineButton);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_NEW_WHINE_BUTTON))
        {
            
            mLogInfo("GetWebNewWhineButton");
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebNewWhine getWebNewWhineButton(this->_ip);
            string sResult;
            resCode = getWebNewWhineButton.GetWebNewWhineButton(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_SET_WEB_AUDIO_MUTE))
        {
            int MuteVal, CtrlType, ChnNum;
            mLogInfo("SetWebAudioMute");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("MuteVal", MuteVal))
            {
                mLogError("set params error node:" << "MuteVal");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("CtrlType", CtrlType))
            {
                mLogError("set params error node:" << "CtrlType");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
            {
                mLogError("set params error node:" << "ChnNum");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CWebAudioMute setWebAudioMute(this->_ip);
            string sResult;
            resCode = setWebAudioMute.SetWebAudioMute(sResult, MuteVal, CtrlType, ChnNum);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), METHOD_GET_WEB_AUDIO_MUTE))
        {
            int CtrlType, ChnNum;
            mLogInfo("GetWebAudioMute");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("CtrlType", CtrlType))
            {
                mLogError("set params error node:" << "CtrlType");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
            {
                mLogError("set params error node:" << "ChnNum");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CWebAudioMute getWebAudioMute(this->_ip);
            string sResult;
            resCode = getWebAudioMute.GetWebAudioMute(sResult, CtrlType, ChnNum);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }	
		else if (!strcmp(sMethod.c_str(), "GetWebRecTaskID"))
        {
            
            mLogInfo("GetWebRecTaskID");
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebRecCtrl getWebRecTaskID(this->_ip);
            string sResult;
            resCode = getWebRecTaskID.GetWebRecTaskID(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebRcrvRecord"))
        {
            
            mLogInfo("SetWebRcrvRecord");
			int RecOn;
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("RecOn", RecOn))
            {
                mLogError("set params error node:" << "RecOn");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebRecCtrl setWebRcrvRecord(this->_ip);
            string sResult;
            resCode = setWebRcrvRecord.SetWebRcrvRecord(sResult,RecOn);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "GetWebRcrvRecord"))
        {
            
            mLogInfo("GetWebRcrvRecord");
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebRecCtrl getWebRcrvRecord(this->_ip);
            string sResult;
            resCode = getWebRcrvRecord.GetWebRcrvRecord(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebOpenUart"))
        {
            int UartPort, DateBit, StopBit;
			int ParityBit, BaudRate;
            mLogInfo("SetWebOpenUart");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt(SET_WEB_UART_CONF_PARAMS_UARTPORT, UartPort))
            {
                mLogError("set params error node:" << "UartPort");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_UART_CONF_PARAMS_BAUDRATE, BaudRate))
            {
                mLogError("set params error node:" << "BaudRate");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("DataBit", DateBit))
            {
                mLogError("set params error node:" << "DataBit");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_UART_CONF_PARAMS_STOPBIT, StopBit))
            {
                mLogError("set params error node:" << "StopBit");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt(SET_WEB_UART_CONF_PARAMS_PARITYBIT, ParityBit))
            {
                mLogError("set params error node:" << "ParityBit");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf SetWebOpenUart(this->_ip);
            string sResult;
            resCode = SetWebOpenUart.SetWebOpenUart(sResult, UartPort, BaudRate, DateBit, StopBit, ParityBit);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebSendANDRecvUart"))
        {
            int UartPort, DateType;
			string  Data;
            mLogInfo("SetWebSendANDRecvUart");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt(SET_WEB_UART_CONF_PARAMS_UARTPORT, UartPort))
            {
                mLogError("set params error node:" << "UartPort");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("DateType", DateType))
            {
                mLogError("set params error node:" << "DateType");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("Data", Data))
            {
                mLogError("set params error node:" << "Data");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf SetWebSendANDRecvUart(this->_ip);
            string sResult;
            resCode = SetWebSendANDRecvUart.SetWebSendANDRecvUart(sResult, UartPort, DateType, Data);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebCloseUart"))
        {
		    mLogInfo("SetWebCloseUart");
			int UartPort, DateType;
            if (!params.getParamsNodeValueofInt("UartPort", UartPort))
            {
                mLogError("set params error node:" << "UartPort");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebUartConf SetWebCloseUart(this->_ip);
		    string sResult;
		    resCode = SetWebCloseUart.SetWebCloseUart(sResult, UartPort);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), "GetWebUartMCU"))
        {
		    mLogInfo("GetWebUartMCU");
			int UartPort, DateType;
            if (!params.getParamsNodeValueofInt("UartPort", UartPort))
            {
                mLogError("set params error node:" << "UartPort");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebUartConf GetWebUartMCU(this->_ip);
		    string sResult;
		    resCode = GetWebUartMCU.GetWebUartMCU(sResult, UartPort);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), "GetWebSyncTime"))
        {
		    mLogInfo("GetWebSyncTime");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebSyncTime GetWebSyncTime(this->_ip);
		    string sResult;
		    resCode = GetWebSyncTime.GetWebSyncTime(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), "GetWebVideoMatrixName"))
        {
		    mLogInfo("GetWebVideoMatrixName");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebVideoMatrix GetWebVideoMatrixName(this->_ip);
		    string sResult;
		    resCode = GetWebVideoMatrixName.GetWebVideoMatrixName(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebVideoMatrixName"))
        {
			string  InPutChnnelName1, InPutChnnelName2, InPutChnnelName3, InPutChnnelName4,InPutChnnelName5,InPutChnnelName6,InPutChnnelName7,InPutChnnelName8,InPutChnnelName9,InPutChnnelName10,InPutChnnelName11,InPutChnnelName12;
			string  OutPutChnnelName1,OutPutChnnelName2,OutPutChnnelName3,OutPutChnnelName4,OutPutChnnelName5,OutPutChnnelName6,OutPutChnnelName7,OutPutChnnelName8;
            mLogInfo("SetWebVideoMatrixName");
            
            if (!params.getParamsNodeValueofStr("InPutChnnelName1", InPutChnnelName1))
            {
                mLogError("set params error node:" << "InPutChnnelName1");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("InPutChnnelName2", InPutChnnelName2))
            {
                mLogError("set params error node:" << "InPutChnnelName2");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("InPutChnnelName3", InPutChnnelName3))
            {
                mLogError("set params error node:" << "InPutChnnelName3");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }   
			if (!params.getParamsNodeValueofStr("InPutChnnelName4", InPutChnnelName4))
            {
                mLogError("set params error node:" << "InPutChnnelName4");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofStr("InPutChnnelName5", InPutChnnelName5))
            {
                mLogError("set params error node:" << "InPutChnnelName5");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("InPutChnnelName6", InPutChnnelName6))
            {
                mLogError("set params error node:" << "InPutChnnelName6");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("InPutChnnelName7", InPutChnnelName7))
            {
                mLogError("set params error node:" << "InPutChnnelName7");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("InPutChnnelName8", InPutChnnelName8))
            {
                mLogError("set params error node:" << "InPutChnnelName8");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }   
			if (!params.getParamsNodeValueofStr("InPutChnnelName9", InPutChnnelName9))
            {
                mLogError("set params error node:" << "InPutChnnelName9");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofStr("OutPutChnnelName1", OutPutChnnelName1))
            {
                mLogError("set params error node:" << "OutPutChnnelName1");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }   
			if (!params.getParamsNodeValueofStr("OutPutChnnelName2", OutPutChnnelName2))
            {
                mLogError("set params error node:" << "OutPutChnnelName2");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebVideoMatrix SetWebVideoMatrixName(this->_ip);
            string sResult;
            resCode = SetWebVideoMatrixName.SetWebVideoMatrixName(sResult, InPutChnnelName1, InPutChnnelName2, InPutChnnelName3, InPutChnnelName4,InPutChnnelName5,InPutChnnelName6,InPutChnnelName7,InPutChnnelName8,InPutChnnelName9,OutPutChnnelName1,OutPutChnnelName2);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebAudioSPL"))
        {
            int ChnNum, SPL;
			
            mLogInfo("SetWebAudioSPL");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
            {
                mLogError("set params error node:" << "ChnNum");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("SPL", SPL))
            {
                mLogError("set params error node:" << "SPL");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioConf SetWebAudioSPL(this->_ip);
            string sResult;
            resCode = SetWebAudioSPL.SetWebAudioSPL(sResult, ChnNum, SPL);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "GetWebAudioSPLAndPower"))
        {
		    mLogInfo("GetWebAudioSPLAndPower");
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebAudioConf GetWebAudioSPL(this->_ip);
		    string sResult;
		    resCode = GetWebAudioSPL.GetWebAudioSPL(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebAudioPower"))
        {
            int ChnNum, IsPower;
			
            mLogInfo("SetWebAudioPower");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
            {
                mLogError("set params error node:" << "ChnNum");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("IsPower", IsPower))
            {
                mLogError("set params error node:" << "IsPower");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioConf SetWebAudioPower(this->_ip);
            string sResult;
            resCode = SetWebAudioPower.SetWebAudioPower(sResult, ChnNum, IsPower);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebAudioMotVal"))
        {
            int ChnNum, MotVal;
			
            mLogInfo("SetWebAudioMotVal");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
            {
                mLogError("set params error node:" << "ChnNum");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("MotVal", MotVal))
            {
                mLogError("set params error node:" << "IsPower");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioConf SetWebAudioMotVal(this->_ip);
            string sResult;
            resCode = SetWebAudioMotVal.SetWebAudioMotVal(sResult, ChnNum, MotVal);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebAudioMotTime"))
        {
            int ChnNum, MotTime;
			
            mLogInfo("SetWebAudioPower");
            //获取params节点中DevID节点
//            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
//            {
//                mLogError("set params error node:" << "ChnNum");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
            if (!params.getParamsNodeValueofInt("MotTime", MotTime))
            {
                mLogError("set params error node:" << "IsPower");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioConf SetWebAudioMotTime(this->_ip);
            string sResult;
            resCode = SetWebAudioMotTime.SetWebAudioMotTime(sResult, ChnNum, MotTime);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebAudioFeedback"))
        {
            int ChnNum, IsFeedback;
			
            mLogInfo("SetWebAudioFeedback");
            //获取params节点中DevID节点
//            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
//            {
//                mLogError("set params error node:" << "ChnNum");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
            if (!params.getParamsNodeValueofInt("IsFeedback", IsFeedback))
            {
                mLogError("set params error node:" << "IsFeedback");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioConf SetWebAudioFeedback(this->_ip);
            string sResult;
            resCode = SetWebAudioFeedback.SetWebAudioFeedback(sResult, ChnNum, IsFeedback);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebAudioAEC"))
        {
            int ChnNum, IsAEC;
			
            mLogInfo("SetWebAudioAEC");
            //获取params节点中DevID节点
//            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
//            {
//                mLogError("set params error node:" << "ChnNum");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
            if (!params.getParamsNodeValueofInt("IsAEC", IsAEC))
            {
                mLogError("set params error node:" << "IsAEC");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioConf SetWebAudioAEC(this->_ip);
            string sResult;
            resCode = SetWebAudioAEC.SetWebAudioAEC(sResult, ChnNum, IsAEC);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebAudioANS"))
        {
            int ChnNum, IsANS;
			
            mLogInfo("SetWebAudioANS");
            //获取params节点中DevID节点
//            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
//            {
//                mLogError("set params error node:" << "ChnNum");
//                resCode = eInterfaceResCodeRequErrorParam;
//                break;
//            }
            if (!params.getParamsNodeValueofInt("IsANS", IsANS))
            {
                mLogError("set params error node:" << "IsANS");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioConf SetWebAudioANS(this->_ip);
            string sResult;
            resCode = SetWebAudioANS.SetWebAudioANS(sResult, ChnNum, IsANS);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebAudioAGC"))
        {
            int ChnNum, IsAGC;
			
            mLogInfo("SetWebAudioAGC");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
            {
                mLogError("set params error node:" << "ChnNum");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("IsAGC", IsAGC))
            {
                mLogError("set params error node:" << "IsAGC");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioConf SetWebAudioAGC(this->_ip);
            string sResult;
            resCode = SetWebAudioAGC.SetWebAudioAGC(sResult, ChnNum, IsAGC);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebAudioIncentives"))
        {
            int ChnNum, IsIncentives;
			
            mLogInfo("SetWebAudioIncentives");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("ChnNum", ChnNum))
            {
                mLogError("set params error node:" << "ChnNum");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("IsIncentives", IsIncentives))
            {
                mLogError("set params error node:" << "IsIncentives");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebAudioConf SetWebAudioIncentives(this->_ip);
            string sResult;
            resCode = SetWebAudioIncentives.SetWebAudioIncentives(sResult, ChnNum, IsIncentives);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "GetWebAudioPower"))
        {
		    mLogInfo("GetWebAudioPower");
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebAudioConf GetWebAudioPower(this->_ip);
		    string sResult;
		    resCode = GetWebAudioPower.GetWebAudioPower(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), "GetWebAudioBoardType"))
        {
		    mLogInfo("GetWebAudioBoardType");
		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebAudioConf GetWebAudioBoardType(this->_ip);
		    string sResult;
		    resCode = GetWebAudioBoardType.GetWebAudioBoardType(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebRecFileDel"))
        {
            
            int Type;
            string TaskID,FileName;
            mLogInfo("SetWebRecFileDel");
            if (!params.getParamsNodeValueofStr(GET_INFOLIST_REC_PARAMS_TASKID, TaskID))
            {
                mLogError("set params error node:" << GET_INFOLIST_REC_PARAMS_TASKID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("Type", Type))
            {
                mLogError("set params error node:" << "Type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("FileName", FileName))
            {
                mLogError("set params error node:" << "FileName");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebRecCtrl SetWebRecFileDel(this->_ip);
            string sResult;
            resCode = SetWebRecFileDel.SetWebRecFileDel(sResult, TaskID, Type, FileName);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebSystermReboot"))
        {
		    mLogInfo("SetWebSystermReboot");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CSetWebSystermShoutDown SetWebSystermReboot(this->_ip);
		    string sResult;
		    resCode = SetWebSystermReboot.SetWebSystermReboot(sResult);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), "SendAudioSerialCmd"))
        {
            //setrecstop
            int SerialID;
            string Cmd;
            mLogInfo("SendAudioSerialCmd");
            if (!params.getParamsNodeValueofInt(SEND_SERILAL_PARAMS_SERIALID, SerialID))
            {
                mLogError("set params error node:" << SEND_SERILAL_PARAMS_SERIALID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr(SEND_SERILAL_PARAMS_CMD, Cmd))
            {
                mLogError("set params error node:" << SEND_SERILAL_PARAMS_CMD);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSendSerialCmd sendAudioSerialCmd(this->_ip);
            string sResult;
            resCode = sendAudioSerialCmd.SendAudioSerialCmd(sResult, SerialID, Cmd);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetAudioGain"))
        {
            //setrecstop
            int SerialID,AudioChn,Val;
            //string Cmd;
            mLogInfo("SetAudioGain");
            if (!params.getParamsNodeValueofInt(SEND_SERILAL_PARAMS_SERIALID, SerialID))
            {
                mLogError("set params error node:" << SEND_SERILAL_PARAMS_SERIALID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("AudioChn", AudioChn))
            {
                mLogError("set params error node:" << "AudioChn");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("Val", Val))
            {
                mLogError("set params error node:" << "Val");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            CSendSerialCmd SetAudioGain(this->_ip);
            string sResult;
            resCode = SetAudioGain.SetAudioGain(sResult, SerialID, AudioChn, Val);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebTaskIDUpload"))
        {
            string TaskId;
            mLogInfo("SetWebTaskIDUpload");
            if (!params.getParamsNodeValueofStr(GET_INFOLIST_REC_PARAMS_TASKID, TaskId))
            {
                mLogError("set params error node:" << GET_INFOLIST_REC_PARAMS_TASKID);
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebRecCtrl SetWebTaskIDUpload(this->_ip);
            string sResult;
            resCode = SetWebTaskIDUpload.SetWebTaskIDUpload(sResult, TaskId);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebMosaic"))
		{
			int ArryNums = 20;//JSON数组最大数量
			int MosaicDisplay[ArryNums],MosaicNum[ArryNums],MosaicType[ArryNums],ArryNumbers;
			string MosaicChn[ArryNums],MosaicWinX[ArryNums],MosaicWinY[ArryNums],MosaicWinH[ArryNums],MosaicWinW[ArryNums];
			cJSON* arrayItem[ArryNums];
			mLogInfo("SetWebMosaic");
			//取出JSON数组中ArryList数组的每一组数据,JSON数组数目保存在ArryNumbers中，每组数据保存在arrayItem数组中
			if (!params.getParamsNodeValueofArry("ArryList", ArryNumbers, arrayItem))
			{
				mLogError("set params error node:" << "ArryList");
				resCode = eInterfaceResCodeRequErrorParam;
				break;
			}
			//申请的数组小于实际的数组大小报错
			if(ArryNums < ArryNumbers)
			{
				mLogError("ArryNumbers is larger than requst arry,ArryNums:" << ArryNums<<"   ArryNumbers:"<<ArryNumbers);
				resCode = eInterfaceResCodeRequErrorParam;
				break;
			}
			//取出arrayItem数组中的数据
			for(int i = 0; i < ArryNumbers; i++)
			{
				if (!params.getNodeValueofStr(arrayItem[i],"MosaicChn", MosaicChn[i]))
				{
					mLogError("set params error node:" << "MosaicChn");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				if (!params.getNodeValueofInt(arrayItem[i],"MosaicDisplay", MosaicDisplay[i]))
				{
					mLogError("set params error node:" << "MosaicDisplay");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				if (!params.getNodeValueofInt(arrayItem[i],"MosaicNum", MosaicNum[i]))
				{
					mLogError("set params error node:" << "MosaicNum");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				if (!params.getNodeValueofInt(arrayItem[i],"MosaicType", MosaicType[i]))
				{
					mLogError("set params error node:" << "MosaicType");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				if (!params.getNodeValueofStr(arrayItem[i],"MosaicWinX", MosaicWinX[i]))
				{
					mLogError("set params error node:" << "MosaicWinX");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				if (!params.getNodeValueofStr(arrayItem[i],"MosaicWinY", MosaicWinY[i]))
				{
					mLogError("set params error node:" << "MosaicWinY");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				if (!params.getNodeValueofStr(arrayItem[i],"MosaicWinH", MosaicWinH[i]))
				{
					mLogError("set params error node:" << "MosaicWinH");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				if (!params.getNodeValueofStr(arrayItem[i],"MosaicWinW", MosaicWinW[i]))
				{
					mLogError("set params error node:" << "MosaicWinW");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				
			}
			
			 //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
			 CSetWebNewMosaic SetWebNewMosaic(this->_ip);
			 string sResult;
			 resCode = SetWebNewMosaic.SetWebNewMosaic(sResult, ArryNumbers, MosaicChn, MosaicDisplay, MosaicNum, MosaicType, MosaicWinX, MosaicWinY, MosaicWinH, MosaicWinW);
			 //设置返回信息 内容列表
			 response.setResponseResults(sResult);
			 //释放arrayItem申请的空间
//			 for(int i = 0; i < ArryNumbers; i++)
//			{
//				cJSON_Delete(arrayItem[i]);
//			}
			break;
		}
		else if (!strcmp(sMethod.c_str(), "GetWebRecordTime"))
        {
            mLogInfo("GetWebRecordTime");
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetRecCtrl GetWebRecordTime(this->_ip);
            string sResult;
            resCode = GetWebRecordTime.GetWebRecordTime(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "GetWebDecInfo"))
        {
            //getHostDecInfo
            int DecId;
            mLogInfo("GetWebDecInfo");
            CGetHostDecinformation GetWebDecInfo(this->_ip);
            string sResult;
            resCode = GetWebDecInfo.GetWebDecInfo(sResult);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetWebStartDecNew"))
		{
			int ArryNums = 20;//JSON数组最大数量
			int DecID[ArryNums],DecType[ArryNums],ArryNumbers;
			string DecUrl[ArryNums];
			cJSON* arrayItem[ArryNums];
			mLogInfo("SetWebStartDecNew");
			//取出JSON数组中ArryList数组的每一组数据,JSON数组数目保存在ArryNumbers中，每组数据保存在arrayItem数组中
			if (!params.getParamsNodeValueofArry("ArryList", ArryNumbers, arrayItem))
			{
				mLogError("set params error node:" << "ArryList");
				resCode = eInterfaceResCodeRequErrorParam;
				break;
			}
			//申请的数组小于实际的数组大小报错
			if(ArryNums < ArryNumbers)
			{
				mLogError("ArryNumbers is larger than requst arry,ArryNums:" << ArryNums<<"   ArryNumbers:"<<ArryNumbers);
				resCode = eInterfaceResCodeRequErrorParam;
				break;
			}
			//取出arrayItem数组中的数据
			for(int i = 0; i < ArryNumbers; i++)
			{
				if (!params.getNodeValueofStr(arrayItem[i],"DecUrl", DecUrl[i]))
				{
					mLogError("set params error node:" << "DecUrl");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				if (!params.getNodeValueofInt(arrayItem[i],"DecID", DecID[i]))
				{
					mLogError("set params error node:" << "DecID");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				if (!params.getNodeValueofInt(arrayItem[i],"DecType", DecType[i]))
				{
					mLogError("set params error node:" << "DecType");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				
			}
			
			 //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
			 CSetWebMultiCtrl SetWebStartDecNew(this->_ip);
			 string sResult;
			 resCode = SetWebStartDecNew.SetWebStartDecNew(sResult, ArryNumbers, DecType, DecID, DecUrl);
			 //设置返回信息 内容列表
			 response.setResponseResults(sResult);
			 //释放arrayItem申请的空间
//			 for(int i = 0; i < ArryNumbers; i++)
//			{
//				cJSON_Delete(arrayItem[i]);
//			}
			break;
		}
		else if (!strcmp(sMethod.c_str(), "SetWebStopDecNew"))
		{
			int ArryNums = 20;//JSON数组最大数量
			int DecID[ArryNums],DecType[ArryNums],ArryNumbers;
			cJSON* arrayItem[ArryNums];
			mLogInfo("SetWebStopDecNew");
			//取出JSON数组中ArryList数组的每一组数据,JSON数组数目保存在ArryNumbers中，每组数据保存在arrayItem数组中
			if (!params.getParamsNodeValueofArry("ArryList", ArryNumbers, arrayItem))
			{
				mLogError("set params error node:" << "ArryList");
				resCode = eInterfaceResCodeRequErrorParam;
				break;
			}
			//申请的数组小于实际的数组大小报错
			if(ArryNums < ArryNumbers)
			{
				mLogError("ArryNumbers is larger than requst arry,ArryNums:" << ArryNums<<"   ArryNumbers:"<<ArryNumbers);
				resCode = eInterfaceResCodeRequErrorParam;
				break;
			}
			//取出arrayItem数组中的数据
			for(int i = 0; i < ArryNumbers; i++)
			{
				if (!params.getNodeValueofInt(arrayItem[i],"DecID", DecID[i]))
				{
					mLogError("set params error node:" << "DecID");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				if (!params.getNodeValueofInt(arrayItem[i],"DecType", DecType[i]))
				{
					mLogError("set params error node:" << "DecType");
					resCode = eInterfaceResCodeRequErrorParam;
					break;
				}
				
			}
			
			 //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
			 CSetWebMultiCtrl SetWebStopDecNew(this->_ip);
			 string sResult;
			 resCode = SetWebStopDecNew.SetWebStopDecNew(sResult, ArryNumbers, DecType, DecID);
			 //设置返回信息 内容列表
			 response.setResponseResults(sResult);
			 //释放arrayItem申请的空间
//			 for(int i = 0; i < ArryNumbers; i++)
//			{
//				cJSON_Delete(arrayItem[i]);
//			}
			break;
		}
		else if (!strcmp(sMethod.c_str(), "UartSetConfig"))
        {
            int num, baudrate, parity;
			int databits, stopbits;
            mLogInfo("UartSetConfig");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("num", num))
            {
                mLogError("set params error node:" << "num");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("baudrate", baudrate))
            {
                mLogError("set params error node:" << "baudrate");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("parity", parity))
            {
                mLogError("set params error node:" << "parity");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("databits", databits))
            {
                mLogError("set params error node:" << "databits");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("stopbits", stopbits))
            {
                mLogError("set params error node:" << "stopbits");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartSetConfig(this->_ip);
            string sResult;
            resCode = UartSetConfig.UartSetConfig(sResult, num, baudrate, parity, databits, stopbits,requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartGetConfig"))
        {
            int num, baudrate, parity;
			int databits, stopbits;
            mLogInfo("UartGetConfig");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("num", num))
            {
                mLogError("set params error node:" << "num");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartGetConfig(this->_ip);
            string sResult;
            resCode = UartGetConfig.UartGetConfig(sResult, num, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartSendData"))
        {
            int num, type, buflength;
			string sendstring;
            mLogInfo("UartSendData");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("num", num))
            {
                mLogError("set params error node:" << "num");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("type", type))
            {
                mLogError("set params error node:" << "type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("buflength", buflength))
            {
                mLogError("set params error node:" << "buflength");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("string", sendstring))
            {
                mLogError("set params error node:" << "string");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartSendData(this->_ip);
            string sResult;
            resCode = UartSendData.UartSendData(sResult, num, type, buflength, sendstring, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartCreateTable"))
        {
            int dev_type, dev_protocol;
			string dev_name;
            mLogInfo("UartCreateTable");
            if (!params.getParamsNodeValueofInt("dev_type", dev_type))
            {
                mLogError("set params error node:" << "dev_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("dev_protocol", dev_protocol))
            {
                mLogError("set params error node:" << "dev_protocol");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartCreateTable(this->_ip);
            string sResult;
            resCode = UartCreateTable.UartCreateTable(sResult, dev_name, dev_type, dev_protocol, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartDelTable"))
        {
            int dev_type, dev_protocol;
			string dev_name;
            mLogInfo("UartDelTable");

            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartDelTable(this->_ip);
            string sResult;
            resCode = UartDelTable.UartDelTable(sResult, dev_name, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartSearchTable"))
        {
            int dev_type, dev_protocol;
			string dev_name;
            mLogInfo("UartSearchTable");

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartSearchTable(this->_ip);
            string sResult;
            resCode = UartSearchTable.UartSearchTable(sResult, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartAlterTable"))
        {
            int dev_protocol, dev_type, buflength;
			string dev_name,new_dev_name;
            mLogInfo("UartAlterTable");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("new_dev_name", new_dev_name))
            {
                mLogError("set params error node:" << "new_dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("dev_type", dev_type))
            {
                mLogError("set params error node:" << "dev_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("dev_protocol", dev_protocol))
            {
                mLogError("set params error node:" << "dev_protocol");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartAlterTable(this->_ip);
            string sResult;
            resCode = UartAlterTable.UartAlterTable(sResult, dev_name, new_dev_name, dev_type, dev_protocol, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartAddTableControlCmd"))
        {
			string dev_name, cmd_recv_code, cmd_fun_des;
            mLogInfo("UartAddTableControlCmd");
            
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_recv_code", cmd_recv_code))
            {
                mLogError("set params error node:" << "cmd_recv_code");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_fun_des", cmd_fun_des))
            {
                mLogError("set params error node:" << "cmd_fun_des");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartAddTableControlCmd(this->_ip);
            string sResult;
            resCode = UartAddTableControlCmd.UartAddTableControlCmd(sResult, dev_name, cmd_recv_code, cmd_fun_des, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartAddTableCenterControlCmd"))
        {
            int cmd_type, protocol_type;
			string dev_name, cmd_recv_code, data_buf, cmd_name;
            mLogInfo("UartAddTableCenterControlCmd");
            
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_recv_code", cmd_recv_code))
            {
                mLogError("set params error node:" << "cmd_recv_code");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("cmd_type", cmd_type))
            {
                mLogError("set params error node:" << "cmd_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("data_buf", data_buf))
            {
                mLogError("set params error node:" << "data_buf");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("protocol_type", protocol_type))
            {
                mLogError("set params error node:" << "protocol_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_name", cmd_name))
            {
                mLogError("set params error node:" << "cmd_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartAddTableCenterControlCmd(this->_ip);
            string sResult;
            resCode = UartAddTableCenterControlCmd.UartAddTableCenterControlCmd(sResult, dev_name, cmd_recv_code, cmd_type, data_buf, protocol_type, cmd_name, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartAddTableCameraCmd"))
        {
            int protocol_type, cmd_protocol_type, cmd_holder_addr;
			string dev_name, cmd_name, cmd_correlation_vi;
            mLogInfo("UartAddTableCameraCmd");
            
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

			if (!params.getParamsNodeValueofInt("protocol_type", protocol_type))
            {
                mLogError("set params error node:" << "protocol_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_name", cmd_name))
            {
                mLogError("set params error node:" << "cmd_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("cmd_protocol_type", cmd_protocol_type))
            {
                mLogError("set params error node:" << "cmd_protocol_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("cmd_holder_addr", cmd_holder_addr))
            {
                mLogError("set params error node:" << "cmd_holder_addr");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofStr("cmd_correlation_vi", cmd_correlation_vi))
            {
                mLogError("set params error node:" << "cmd_correlation_vi");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartAddTableCameraCmd(this->_ip);
            string sResult;
            resCode = UartAddTableCameraCmd.UartAddTableCameraCmd(sResult, dev_name, cmd_name, protocol_type, cmd_protocol_type, cmd_holder_addr, cmd_correlation_vi, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartDelTableControlCmd"))
        {
			string dev_name,cmd_recv_code;
            mLogInfo("UartDelTableControlCmd");
           
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_recv_code", cmd_recv_code))
            {
                mLogError("set params error node:" << "cmd_recv_code");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartDelTableControlCmd(this->_ip);
            string sResult;
            resCode = UartDelTableControlCmd.UartDelTableControlCmd(sResult, dev_name, cmd_recv_code, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartDelTableControlExecuteCmd"))
        {
			string dev_name,cmd_recv_code,cmd_name;
            mLogInfo("UartDelTableControlExecuteCmd");
           
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_recv_code", cmd_recv_code))
            {
                mLogError("set params error node:" << "cmd_recv_code");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_name", cmd_name))
            {
                mLogError("set params error node:" << "cmd_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartDelTableControlExecuteCmd(this->_ip);
            string sResult;
            resCode = UartDelTableControlExecuteCmd.UartDelTableControlExecuteCmd(sResult, dev_name, cmd_recv_code, cmd_name, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartDelTableControlCameraCmd"))
        {
			string dev_name,cmd_name;
            mLogInfo("UartDelTableControlCameraCmd");
           
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_name", cmd_name))
            {
                mLogError("set params error node:" << "cmd_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartDelTableControlCameraCmd(this->_ip);
            string sResult;
            resCode = UartDelTableControlCameraCmd.UartDelTableControlCameraCmd(sResult, dev_name, cmd_name, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartSearchTableControlCmd"))
        {
			string dev_name;
            mLogInfo("UartSearchTableControlCmd");
           
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartSearchTableControlCmd(this->_ip);
            string sResult;
            resCode = UartSearchTableControlCmd.UartSearchTableControlCmd(sResult, dev_name, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartSearchTableControlExecuteCmd"))
        {
			string dev_name,cmd_recv_code;
            mLogInfo("UartSearchTableControlCmd");
           
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_recv_code", cmd_recv_code))
            {
                mLogError("set params error node:" << "cmd_recv_code");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartSearchTableControlExecuteCmd(this->_ip);
            string sResult;
            resCode = UartSearchTableControlExecuteCmd.UartSearchTableControlExecuteCmd(sResult, dev_name, cmd_recv_code, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartSearchCameraTableControlCmd"))
        {
			string dev_name;
            mLogInfo("UartSearchCameraTableControlCmd");
           
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartSearchCameraTableControlCmd(this->_ip);
            string sResult;
            resCode = UartSearchCameraTableControlCmd.UartSearchCameraTableControlCmd(sResult, dev_name, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartAlterTableControlCmd"))
        {
			string dev_name,cmd_recv_code,new_cmd_recv_code,cmd_fun_des;
            mLogInfo("UartAlterTableControlCmd");
            
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_recv_code", cmd_recv_code))
            {
                mLogError("set params error node:" << "cmd_recv_code");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofStr("new_cmd_recv_code", new_cmd_recv_code))
            {
                mLogError("set params error node:" << "new_cmd_recv_code");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_fun_des", cmd_fun_des))
            {
                mLogError("set params error node:" << "cmd_fun_des");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartAlterTableControlCmd(this->_ip);
            string sResult;
            resCode = UartAlterTableControlCmd.UartAlterTableControlCmd(sResult, dev_name, cmd_recv_code, new_cmd_recv_code, cmd_fun_des, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartAlterTableControlExecuteCmd"))
        {
            int cmd_type, protocol_type;
			string dev_name,cmd_recv_code,data_buf,cmd_name,cmd_name_new;
            mLogInfo("UartAlterTableControlExecuteCmd");
            
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_recv_code", cmd_recv_code))
            {
                mLogError("set params error node:" << "cmd_recv_code");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("cmd_type", cmd_type))
            {
                mLogError("set params error node:" << "cmd_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("data_buf", data_buf))
            {
                mLogError("set params error node:" << "data_buf");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofInt("protocol_type", protocol_type))
            {
                mLogError("set params error node:" << "protocol_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_name", cmd_name))
            {
                mLogError("set params error node:" << "cmd_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_name_new", cmd_name_new))
            {
                mLogError("set params error node:" << "cmd_name_new");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartAlterTableControlExecuteCmd(this->_ip);
            string sResult;
            resCode = UartAlterTableControlExecuteCmd.UartAlterTableControlExecuteCmd(sResult, dev_name, cmd_recv_code, cmd_type, data_buf, protocol_type, cmd_name, cmd_name_new, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartAlterTableControlCameraCmd"))
        {
            int protocol_type, cmd_protocol_type,cmd_holder_addr;
			string dev_name,cmd_name,cmd_correlation_vi,cmd_name_new;
            mLogInfo("UartAlterTableControlCameraCmd");
            
            if (!params.getParamsNodeValueofStr("dev_name", dev_name))
            {
                mLogError("set params error node:" << "dev_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

			if (!params.getParamsNodeValueofInt("protocol_type", protocol_type))
            {
                mLogError("set params error node:" << "protocol_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_name", cmd_name))
            {
                mLogError("set params error node:" << "cmd_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("cmd_protocol_type", cmd_protocol_type))
            {
                mLogError("set params error node:" << "cmd_protocol_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("cmd_holder_addr", cmd_holder_addr))
            {
                mLogError("set params error node:" << "cmd_holder_addr");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			if (!params.getParamsNodeValueofStr("cmd_correlation_vi", cmd_correlation_vi))
            {
                mLogError("set params error node:" << "cmd_correlation_vi");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofStr("cmd_name_new", cmd_name_new))
            {
                mLogError("set params error node:" << "cmd_name_new");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartAlterTableControlCameraCmd(this->_ip);
            string sResult;
            resCode = UartAlterTableControlCameraCmd.UartAlterTableControlCameraCmd(sResult, dev_name, protocol_type, cmd_name, cmd_name_new, cmd_protocol_type, cmd_holder_addr, cmd_correlation_vi, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartPTZControl"))
        {
            int move_spped, control_type;
			string cmd_name;
            mLogInfo("UartPTZControl");
            
            if (!params.getParamsNodeValueofStr("cmd_name", cmd_name))
            {
                mLogError("set params error node:" << "cmd_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

			if (!params.getParamsNodeValueofInt("move_spped", move_spped))
            {
                mLogError("set params error node:" << "move_spped");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofInt("control_type", control_type))
            {
                mLogError("set params error node:" << "control_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartPTZControl(this->_ip);
            string sResult;
            resCode = UartPTZControl.UartPTZControl(sResult, cmd_name, move_spped, control_type, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "UartCameraPresetControl"))
        {
            int preset_num, control_type;
			string cmd_name;
            mLogInfo("UartCameraPresetControl");
            
            if (!params.getParamsNodeValueofStr("cmd_name", cmd_name))
            {
                mLogError("set params error node:" << "cmd_name");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

			if (!params.getParamsNodeValueofInt("preset_num", preset_num))
            {
                mLogError("set params error node:" << "preset_num");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            if (!params.getParamsNodeValueofInt("control_type", control_type))
            {
                mLogError("set params error node:" << "control_type");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }

            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf UartCameraPresetControl(this->_ip);
            string sResult;
            resCode = UartCameraPresetControl.UartCameraPresetControl(sResult, cmd_name, preset_num, control_type, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "SetGPIOConfig"))
        {
            int channel, Value;
			
            mLogInfo("SetGPIOConfig");
            //获取params节点中DevID节点
            if (!params.getParamsNodeValueofInt("channel", channel))
            {
                mLogError("set params error node:" << "GPIONum");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            if (!params.getParamsNodeValueofInt("Value", Value))
            {
                mLogError("set params error node:" << "Value");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
            //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
            CSetWebUartConf SetGPIOConfig(this->_ip);
            string sResult;
            resCode = SetGPIOConfig.SetGPIOConfig(sResult, channel, Value, requestStr);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
            break;
        }
		else if (!strcmp(sMethod.c_str(), "GetGPIOConfig"))
        {
		    mLogInfo("GetGPIOConfig");

		    //printf("SetRecCtrlStart RecMode %d ChnID %d TaskId %s Title %s\n", RecMode, ChnID, TaskId.c_str(), Title.c_str());
		    CGetWebUartConf GetGPIOConfig(this->_ip);
		    string sResult;
		    resCode = GetGPIOConfig.GetGPIOConfig(sResult, requestStr);
		    //设置返回信息 内容列表
		    response.setResponseResults(sResult);
		    break;
        }
		else if (!strcmp(sMethod.c_str(), "JsonArrytest"))
        {
            int ArryNums = 20;//JSON数组最大数量
            int Type[ArryNums],ArryNumbers;
            string TaskID[ArryNums],FileName[ArryNums];
			cJSON* arrayItem[ArryNums];
            mLogInfo("JsonArrytest");
			//取出JSON数组中的每一组数据
            if (!params.getParamsNodeValueofArry("ArryList", ArryNumbers, arrayItem))
            {
                mLogError("set params error node:" << "ArryList");
                resCode = eInterfaceResCodeRequErrorParam;
                break;
            }
			//申请的数组小于实际的数组大小报错
			if(ArryNums < ArryNumbers)
			{
				mLogError("ArryNumbers is larger than requst arry,ArryNums:" << ArryNums<<"   ArryNumbers:"<<ArryNumbers);
				resCode = eInterfaceResCodeRequErrorParam;
				break;
			}
			//取出数组中的数据
			for(int i = 0; i < ArryNumbers; i++)
			{
				
				if (!params.getNodeValueofInt(arrayItem[i], "Type", Type[i]))
	            {
	                mLogError("set params error node:" << "Type");
	                resCode = eInterfaceResCodeRequErrorParam;
	                break;
	            }

	            if (!params.getNodeValueofStr(arrayItem[i], "FileName", FileName[i]))
	            {
	                mLogError("set params error node:" << "FileName");
	                resCode = eInterfaceResCodeRequErrorParam;
	                break;
	            }

				if (!params.getNodeValueofStr(arrayItem[i], "TaskID", TaskID[i]))
	            {
	                mLogError("set params error node:" << "TaskID");
	                resCode = eInterfaceResCodeRequErrorParam;
	                break;
	            }
				printf("Type[%d]=%d TaskID[%d]=%s FileName[%d]=%s\n",i,Type[i],i,TaskID[i].c_str(),i,FileName[i].c_str());
			}
			
            //CSetWebRecCtrl SetWebRecFileDel(this->_ip);
            string sResult;
           // resCode = SetWebRecFileDel.SetWebRecFileDel(sResult, TaskID, Type, FileName);
            //设置返回信息 内容列表
            response.setResponseResults(sResult);
		    //cJSON_Delete(arrayItem[0]);
            break;
        }
        else
        {
            resCode = eInterfaceResCodeReqNoMethod;
            mLogError("没有此方法:" << sMethod);
            break;
        }

    } while (false);

    //生成返回信息
    string errorStr;
    string resStr;
    if (resCode != eInterfaceResCodeSuccess)
    {
        errorStr = GetInterfaceResError(resCode);
        //        response.clearAllMsg();
        //        resStr = response.simpleResponsePrintofFalied(errorStr);
        resStr = response.simpleResponsePrintofFalied(errorStr, resCode);
    }
    else
    {
        response.setResponseSuccessFlag();
        response.setResponseError(errorStr);
        resStr = response.responsePrint(_debugMode);
    }
    params.clearAll();
    response.clearAllMsg();
    mLogDebug("response str:" << resStr);
    return resStr;
}

void InterfaceProcess::debugMode()
{
    _debugMode = true;
}

InterfaceResCode InterfaceProcess::HostStatusMethod(CHStatus &chState)
{
    mLogInfo("获取设备指定通道的状态...");
    mLogDebug("run HostSTatusMethod(...)");
    //获取状态
    MPSMediaStatus state;
    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetMediaState(opResCode, state);
    if (resCode != eResponseCodeSuccess)
    {
        mLogError("GetMediaState(...) error:" << resCode);
        return CovertResponseCode(resCode);
    }
    else if (opResCode != eMPSResultOK)
    {
        mLogError("mps response flag:" << opResCode << "," << getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    //转换状态值
    chState = CovertMediaStatus(state);
    if (chState >= eCHStatusSize)
    {
        //转换状态值失败
        mLogError("CovertMediadStatus(MPSMediaStatus)=(" << state << ") error. chState:" << chState);
        return eInterfaceResCodeErrorRecv;
    }
    return eInterfaceResCodeSuccess;
}

InterfaceResCode InterfaceProcess::HostStatusMethodV2(CHStatus &chState)
{
    GetHostStatus getStatus;
    int chNum;
    if (getStatus.GetStatus(chNum, chState) != true)
    {
        mLogError("Failed to run GetHostStatus::GetStatus(...)");
        return eInterfaceResCodeErrorLocalDataConvert;
    }

    //
    //
    if (chState >= eCHStatusSize)
    {
        //转换状态值失败
        mLogError("GetHostStatus::GetStatus(chState)=(" << chState << ") error. chState >= eCHStatusSize:" << eCHStatusSize);
        return eInterfaceResCodeErrorRecv;
    }
    return eInterfaceResCodeSuccess;
}
/*
InterfaceResCode InterfaceProcess::HostInitMethod(string caseId, string &resCaseId)
{
    mLogInfo("打开设备指定通道的视频流...");
    mLogInfo("caseID:"<<caseId);

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaPrepare(caseId,opResCode,resCaseId);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaPrepare(caseId,...)=("<<caseId<<",...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    return eInterfaceResCodeSuccess;
}
//*/
/*
InterfaceResCode InterfaceProcess::HostStartMethod(string caseId)
{
    mLogInfo("开始设备指定通道的视频录制...");
    mLogInfo("caseID:"<<caseId);

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaStart(caseId,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaStart(caseId,...)=("<<caseId<<",...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

#ifdef START_STOP_PROOF_ALONE_CONTROL
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
//*/
/*
InterfaceResCode InterfaceProcess::HostStopMethod(bool closeFlag)
{
    mLogInfo("停止设备指定通道的视频录制...");
    mLogInfo("closeFlag:"<<closeFlag);

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode;
#ifdef START_STOP_PROOF_ALONE_CONTROL
    resCode = _client->StopProofSave(opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("StartProofSave(...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }
#endif

    resCode = _client->MediaStop(closeFlag,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaStop(closeFlag,...)=("<<closeFlag<<",...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }
//    boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(200));
    return eInterfaceResCodeSuccess;
}
//*/
/*
InterfaceResCode InterfaceProcess::HostCloseMethod()
{
    mLogInfo("关闭设备指定通道的视频流...");

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->MediaClose(true,opResCode);
    if(resCode != eResponseCodeSuccess){
        mLogError("MediaClose(...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }
    return eInterfaceResCodeSuccess;
}
//*/

InterfaceResCode InterfaceProcess::GetTaskContentMethod(string caseId, cJSON *result)
{
    CGetTaskContent getTaskContent(this->_ip);
    return getTaskContent.GetTaskContent(caseId, result);
}

InterfaceResCode InterfaceProcess::GetTaskContentMethod(string caseId, string &result)
{
    CGetTaskContent getTaskContent(this->_ip);
    return getTaskContent.GetTaskContent(caseId, result);
}

bool InterfaceProcess::checkCaseIdisEnable(string caseId)
{
    //TODO 检查caseid是否有效
    int len = caseId.length();
    if (len > REQUEST_PARAMS_NODE_CASEID_LEN)
    {
        mLogError("caseId.length():" << len << " > " << REQUEST_PARAMS_NODE_CASEID_LEN);
        return false;
    }
    if ((caseId[0] < 'A' || caseId[0] > 'Z') && (caseId[0] < 'a' || caseId[0] > 'z'))
    {
        mLogError("caseId[0]:" << caseId[0] << " scope[a,z][A,Z].");
        return false;
    }
    for (int i = 0; i < len; i++)
    {
        char c = caseId[i];
        if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && (c < '0' || c > '9'))
        {
            if (c == '-' || c == '_')
            {
                break;
            }
            mLogError("caseId.length():" << len << " caseId[" << i << "]:[" << caseId[i] << "], scope[a,z][A,Z][0,9]{-,_}.");
            return false;
        }
    }
    return true;
}

bool InterfaceProcess::checkChannelNumisEnable(int chNum)
{
    if (chNum < 1)
    {
        mLogError("chNum:" << chNum << " < 1 ,error!");
        return false;
    }
    else
    {
        return true;
    }
}
