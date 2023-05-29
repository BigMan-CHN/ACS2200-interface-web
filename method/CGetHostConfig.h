#ifndef CGETHOSTCONFIG_H
#define CGETHOSTCONFIG_H
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
#include <cJSON.h>
#include <ctime>

#include "interfaceDefines.h"
#include "MPSClient.h"
#include "CAnalyzeParaProc.h"

//存储节点network信息
typedef struct{
    string ip;
    string gateway;
    string netmask;
}CNetworkNode, *PCNetworkNode;

//存储节点channel信息
typedef struct{
    string interRtmp_Ip;
    int    interRtmp_Port;
    string interRtmp_AppName;
    string interRtmp_ReleaseName;
    bool   interRtmp_Enable;

    string rtsp_Ip;
    int    rtsp_Port;
    string rtsp_AppName;
    bool   rtsp_Enable;

    int    channelNum;
    bool   channelEnable;
}CChannelNode, *PCChannelNode;

//存储节点channel信息
typedef struct{
    int channelNum;
    string channelName;
    string rtspUrl;
    string rtmpUrl;

    bool   rtspEnable;
    bool   rtmpEnable;
    bool   channelEnable;
}JSONChannelNode, *PJSONChannelNode;

class CGetHostConfig
{
public:
    CGetHostConfig(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief GetHostConfig 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode GetHostConfig(string &sResult);

private:
    //转换CData为需求的CJSON格式
    bool ConvertCDATA2CJSON(CData &oResult);
    //bool ConvertSTRING2CJSON(CData &oResult, char *cResult);
    //合成返回信息
    bool ComposeResult(cJSON * result);
    string ComposeResult();
    //添加nework节点信息
    bool AddNetworkNode(CNetworkNode & networkData);
    //添加sysinfo节点信息
    bool AddSysInfoNode(void);
    //添加diskinfo节点信息
    bool AddDiskInfoNode(void);
    //添加channel节点信息
    bool AddChannelsNode(JSONChannelNode & channelData);

private:
    /**
     * @brief getMPSConfig 获取主机配置信息
     * @param oResult
     * @return true：成功，false：失败.
     */
    bool getMPSConfig(CData &oResult, char *cResult);
    /**
     * @brief getNetworkNode 通过解析GetConfig返回信息得到network节点
     * @param oResult
     * @param oNetWork
     * @return
     */
    bool getNetworkNode(CData & oResult,CData & oNetWork);
    /**
     * @brief getChannlesCDataVectorNode 通过解析GetConfig返回信息得到channels节点
     * @param oResult
     * @param oChannles
     * @return
     */
    bool getChannlesCDataVectorNode(CData &oResult, CData &oChannels);
    /**
     * @brief getRtspCDataNode 通过解析GetConfig返回信息得到rtsp节点
     * @param oResult
     * @param oRtsp
     * @return
     */
    bool getRtspCDataNode(CData & oResult, CData & oRtsp);

    /**
     * @brief analyzeNetworkCDataNodeNetwork 分析network节点下网络配置信息
     * @param oNetwork
     * @param networkData
     * @return
     */
    bool analyzeNetworkCDataNodeNetwork(CData & oNetwork, PCNetworkNode networkData);
    /**
     * @brief analyzeChannelsCDataNodeChannel 分析channels数组下各个通道配置信息
     * @param channel
     * @param channleNum
     * @param chData
     * @return true：成功，false：失败.
     */
    bool analyzeChannelsCDataNodeChannel(CData &oChannel, int channleNum, PCChannelNode chData);

    bool analyzeRtspCDataNodeChannel(CData &oRtsp, PCChannelNode chData);

    /**
     * @brief convertCData2channelNode 转换合成数据
     * @param oNetwork
     * @param oChannelData
     * @param jChannelData
     */
    bool convertCData2channelNode(PCNetworkNode oNetwork, PCChannelNode oChannelData,PJSONChannelNode jChannelData);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    CAnalyzeParaProc _AnalyParas;

    //用于存储结果
    cJSON *_result;
    cJSON *_network;
    cJSON *_channels;
    cJSON *_sysinfo;
    cJSON *_diskinfo;
    cJSON *_decnum;
};

#endif // CGETHOSTCONFIG_H
