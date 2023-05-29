/********************************************************
* MPSClient.h
* 主机接口功能
*
* Copyright (c) 2019, Tyvis
* All rights reserved.
*
* @version		V1.00
* @author		es
* @date			2019-5-17
 *******************************************************/
#ifndef MPSCLIENT_H
#define MPSCLIENT_H
/********************************************************
 * public c/c++ standard headers
 *******************************************************/
#include <stdio.h>
#include <iostream>
#include <string.h>

/********************************************************
 * boost usart headers
 *******************************************************/
#include <boost/asio.hpp>
#include <boost/bind.hpp>

/********************************************************
 * lib headers
 *******************************************************/
#include <log.h>
#include <TransmitClient.h>
#include <xmlParser.h>
#include <LibXML.h>
#include <Server.h>

#include "MPSClientDefine.h"

#define MPS_IP      "127.0.0.1"
//#define MPS_PORT    5231
//add fwb 20201026
#define MPS_PORT    6180
#define MPS_TIMEOUT 15000

using namespace std;
using namespace SVS;

class MPSClient
{
public:
    MPSClient(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

public:
    /**
     * @brief GetConfig 获取配置
     * @param opResCode
     * @param oResult
     * @return 0：成功，!0：失败.
     */
    ResponseCode GetConfig(MPSOperationRes &opResCode,CData &oResult);
    ResponseCode GetConfigNew(const char *cCmdSend, int cCmdLength, MPSOperationRes &opResCode,CData &oResult,char *cResult);
    ResponseCode GetConsoleNew(const char *cCmdSend, int cCmdLength, MPSOperationRes &opResCode,CData &oResult,char *cResult);
    ResponseCode GetSerialNew(int SerialID, const char *cCmdSend, int cCmdLength, MPSOperationRes &opResCode,CData &oResult,char *cResult);

    /**
     * @brief GetSysConfig 获取系统配置
     * @param opResCode
     * @param oResult
     * @return 0：成功，!0：失败.
     */
    ResponseCode GetSysConfig(MPSOperationRes &opResCode,CData &oResult);
    /**
     * @brief GetMediaState 获取mps媒体状态
     * @param opResCode MPS操作返回值
     * @param state 获取到的状态
     * @return 0：成功，!0：失败.
     */
    ResponseCode GetMediaState(MPSOperationRes &opResCode, MPSMediaStatus &state);

    /**
     * @brief MediaPrepare 打开设备指定通道的视频流
     * @param caseId 任务唯一值
     * @param opResCode MPS操作返回值
     * @param resKeyID 返回的keyID
     * @return 0：成功，!0：失败.
     */
    ResponseCode MediaPrepare(int chNum,string caseId, MPSOperationRes &opResCode, string &resKeyID);

    /**
     * @brief MediaStart 开始设备指定通道的视频录制(不包含第二通道证据通道)
     * @param caseId 任务唯一值
     * @param opResCode MPS操作返回值
     * @return 0：成功，!0：失败.
     */
    ResponseCode MediaStart(int chNum,string caseId,MPSOperationRes &opResCode );

    /**
     * @brief StartProofSave 开始设备第二通道证据通道的视频录制
     * @param opResCode MPS操作返回值
     * @return 0：成功，!0：失败.
     */
    ResponseCode StartProofSave(MPSOperationRes &opResCode );
    /**
     * @brief MediaStop 停止设备指定通道的视频录制
     * @param closeFlag true:停止录制同时关闭直播流,false:仅停止录制
     * @param opResCode MPS操作返回值
     * @return 0：成功，!0：失败.
     */
    ResponseCode MediaStop(int chNum, MPSOperationRes &opResCode);
    ResponseCode MediaStop(int chNum,bool closeFlag, MPSOperationRes &opResCode);

    /**
     * @brief StopProofSave 停止设备第二通道证据通道的视频录制
     * @param opResCode MPS操作返回值
     * @return 0：成功，!0：失败.
     */
    ResponseCode StopProofSave(MPSOperationRes &opResCode );
    /**
     * @brief MediaClose 停止设备指定通道的视频流
     * @param opResCode MPS操作返回值
     * @return 0：成功，!0：失败.
     */
    ResponseCode MediaClose(int chNum, MPSOperationRes &opResCode );
    /**
     * @brief MediaSetPoint 庭审打点
     * @param opResCode MPS操作返回值
     * @return 0：成功，!0：失败.
     */
    ResponseCode MediaSetPoint(int chNum, MPSOperationRes &opResCode , CData &oResult);
    ResponseCode MediaSetPoint(int chNum, MPSOperationRes &opResCode , long &duration, string &fileName);
    /**
     * @brief GetPackageFile 获取指定的caseId的内容
     * @param caseId 案件号
     * @param opResCode MPS操作返回值
     * @param oResult 返回PackageFile
     * @return  0：成功，!0：失败.
     */
    ResponseCode GetPackageFile(string caseId,MPSOperationRes &opResCode,CData &oResult);

    /**
     * @brief GetPackageList 获取录制列表
     * @param opResCode MPS操作返回值
     * @param oResult 返回PackageList
     * @return  0：成功，!0：失败.
     */
    ResponseCode GetPackageList(MPSOperationRes &opResCode,CData &oResult);

    /**
     * @brief UpdatePackageFlag 标记Package为可删除
     * @param opResCode
     * @param oResult
     * @return  0：成功，!0：失败.
     */
    ResponseCode UpdatePackageFlag(string caseId, MPSOperationRes &opResCode);
    
    /**
     * @brief ControlSendCmd 发送中控指令
     * @param cmd 网络控制命令
     * @param opResCode
     * @param deviceID 设备ID，0：组合指令；1：中控；2：矩阵；3：混音器 <无效参数,默认为1>
     * @return 0：成功，!0：失败.
     */
    ResponseCode ControlSendCmd(string cmd,MPSOperationRes &opResCode, int deviceID=1);
    
    /**
     * @brief ConfigSetInterRtsp 设置解码地址 <对内部网页使用非对外>
     * @param chNum
     * @param url
     * @param enable
     * @param opResCode
     * @return 0：成功，!0：失败.
     */
    ResponseCode ConfigSetInterRtsp(int chNum,string url,bool enable,MPSOperationRes &opResCode);
    /**
     * @brief MeidaStartInteRtsp 开始解码 <对内部网页使用非对外>
     * @param chNum
     * @param opResCode
     * @return 0：成功，!0：失败.
     */
    ResponseCode MediaStartInteRtsp(int chNum,MPSOperationRes &opResCode);
    /**
     * @brief MeidaStopInteRtsp 停止解码 <对内部网页使用非对外>
     * @param chNum
     * @param opResCode
     * @return 0：成功，!0：失败.
     */
    ResponseCode MediaStopInteRtsp(int chNum,MPSOperationRes &opResCode);

    /**
     * @brief MediaStartDecode 控制设备进行解码
     * @param chNum 解码通道号，1-4
     * @param url 解码地址
     * @param cacheTime 缓冲时间，单位毫秒
     * @param tcpMode 是否启用tcp模式
     * @param opResCode
     * @return
     */
    ResponseCode MediaStartDecode(int chNum,string url,int cacheTime,bool tcpMode,MPSOperationRes &opResCode);
    /**
     * @brief MEdiaStopDecode 控制设备停止解码
     * @param chNum 解码通道号，1-4
     * @param opResCode
     * @return
     */
    ResponseCode MediaStopDecode(int chNum,MPSOperationRes &opResCode);
    /**
     * @brief MeidaGetDecodeStatus 获取解码状态
     * @param chNum 解码通道号，1-4
     * @param decodeStatus
     * @param opResCode
     * @return
     */
    ResponseCode MediaGetDecodeStatus(int chNum,MPSMediaDecodeStatus &decodeStatus,MPSOperationRes &opResCode);
    /**
     * @brief MediaStartDiscipline  开始播放法庭纪律
     * @param opResCode
     * @return
     */
    ResponseCode MediaStartDiscipline(MPSOperationRes &opResCode);
    /**
     * @brief MediaStopDiscipline  停止播放法庭纪律
     * @param opResCode
     * @return
     */
    ResponseCode MediaStopDiscipline(MPSOperationRes &opResCode);
    /**
     * @brief MediaGetDisciplineStatus 获取播放法庭纪律状态
     * @param disciplineStatus
     * @param opResCode
     * @return
     */
    ResponseCode MediaGetDisciplineStatus(MPSMediaDisciplineStatus &disciplineStatus,MPSOperationRes &opResCode);
private:
    /**
     * @brief socketSend 发送 socket 命令 到 MPS
     * @param sModule 模块
     * @param sMethod 方法
     * @param oParams 所需要的参数
     * @return 返回 MPS 返回的信息
     */
    CData socketSend(string sModule, string sMethod, CData oParams= SVSMAP());
    ResponseCode socketSendNew(string sModule, string sMethod, CData oParams,const char *cCmdSend,int cCmdLength, char *cRecult);
    ResponseCode socketSendConsoleCmd(string sModule, string sMethod, CData oParams,const char *cCmdSend,int cCmdLength, char *cRecult);
	//changed by ltz 2022-03-17
	ResponseCode socketSendSerialCmd(string sModule, string sMethod, CData oParams, int SerialID, const char *cCmdSend,int cCmdLength, char *cRecult);
	//ResponseCode socketRecvSerialCmd(string sModule, string sMethod, CData oParams, int SerialID, const char *cCmdRecv, int cCmdLength, char *cRecult);

    /**
     * @brief isSuccessResponseCode 判断socketSend返回的是否为正确的返回信息
     * @param oResult socketSend的返回信息
     * @param code socketSend返回信息中ResponseCode节点值
     * @param opCode socketSend返回信息中Response flag节点值
     * @return true：成功，false：失败.
     */
    bool isSuccessResponseCode(CData & oResult, ResponseCode &code, MPSOperationRes &opCode);

    /**
     * @brief checkReturnResponseCode 检查请求返回的CData，返回ResponseCode
     * @param opResCode
     * @param oResult
     * @return 0：成功，!0：失败.
     */
    ResponseCode checkReturnResponseCode(MPSOperationRes &opResCode, CData &oResult);
private:
    /**
     * @brief getMediaGetStateParams 获取media getstatus 接口需要发送的params
     * @param oParams 获取到的params
     * @return true：成功，false：失败.
     */
    bool getMediaGetStateParams(CData &oParams);

    /**
     * @brief getMediaPrepareParams 获取media prepare 接口需要发送的params
     * @param keyId 任务唯一值
     * @param oParams 获取到的params
     * @return true：成功，false：失败.
     */
    bool getMediaPrepareParams(int chNum,string keyId, CData &oParams);

    /**
     * @brief getMediaStartParams 获取media start 接口需要发送的params
     * @param keyId 任务唯一值
     * @param oParams 获取到的params
     * @return true：成功，false：失败.
     */
    bool getMediaStartParams(int chNum, string keyId, CData &oParams);

    /**
     * @brief getMediaStopParams 获取media stop 接口需要发送的params
     * @param closeFlag true:停止录制同时关闭直播流,false:仅停止录制
     * @param oParams 获取到的params
     * @return true：成功，false：失败.
     */
    bool getMediaStopParams(int chNum,bool closeFlag, CData &oParams);

    /**
     * @brief getMediaCloseParams 获取media close 接口需要发送的params
     * @param oParams 获取到的params
     * @return true：成功，false：失败.
     */
    bool getMediaCloseParams(int chNum, CData &oParams);
    /**
     * @brief getMediaSetPointParams 获取media SetPoint 接口需要发送的params
     * @param oParams 获取到的params
     * @return true：成功，false：失败.
     */
    bool getMediaSetPointParams(int chNum, CData &oParams);

    /**
     * @brief getGetPackageParams 获取GetPackage 接口需要发送的params
     * @param keyId 任务唯一值
     * @param oParams
     * @return true：成功，false：失败.
     */
    bool getGetPackageParams(string keyId, CData &oParams);

    /**
     * @brief getUpdatePackageFlagParams 获取UpdatePackageFlag 接口需要发送的params
     * @param keyId 任务唯一值
     * @param oParams
     * @return true：成功，false：失败.
     */
    bool getUpdatePackageFlagParams(string keyId, CData &oParams);
    
    /**
     * @brief getControlSendCmdParams 获取Console SendCmd 接口需要发送的params
     * @param cmd
     * @param oParams
     * @param deviceID
     * @return  true：成功，false：失败.
     */
    bool getControlSendCmdParams(string cmd, CData &oParams, int deviceID=1);

    /**
     * @brief getConfigSetInterRtspParams 获取Console SendCmd 接口需要发送的params
     * @param chNum
     * @param url
     * @param enble
     * @param oParams
     * @return true：成功，false：失败.
     */
    bool getConfigSetInterRtspParams(int chNum,string url,bool enble, CData &oParams);

    /**
     * @brief getConfigSetInterRtspParams 获取Media StartInteRtsp 接口需要发送的params
     * @param chNum
     * @param oParams
     * @return true：成功，false：失败.
     */
    bool getMediaStartInteRtspParams(int chNum,CData &oParams);
    /**
    * @brief getConfigSetInterRtspParams 获取Media StopInteRtsp 接口需要发送的params
    * @param chNum
    * @param oParams
    * @return true：成功，false：失败.
    */
    bool getMediaStopInteRtspParams(int chNum,CData &oParams);

    /**
     * @brief getMediaStartDecodeParams 获取Media StartInterRtsp_Api 接口需要发送的params
     * @param chNum
     * @param url
     * @param cacthTime
     * @param tcpMode
     * @param oParams
     * @return
     */
    bool getMediaStartDecodeParams(int chNum,string url,int cacthTime,bool tcpMode,CData &oParams);
    /**
     * @brief getMediaStopDecodeParams 获取Media StopInterRtsp_Api 接口需要发送的params
     * @param chNum
     * @param Params
     * @return
     */
    bool getMediaStopDecodeParams(int chNum, CData &oParams);
    /**
     * @brief getMediaGetDecodeStatusParams 获取Media GetInterRtsp_Api 接口需要发送的params
     * @param chNum
     * @param Params
     * @return
     */
    bool getMediaGetDecodeStatusParams(int chNum,CData &oParams);
private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

};

#endif // MPSClient_H
