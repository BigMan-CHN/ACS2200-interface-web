#include "CGetHostConfig.h"
#include <boost/lexical_cast.hpp>

CGetHostConfig::CGetHostConfig(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_network(cJSON_CreateObject())
    ,_channels(cJSON_CreateArray())
    ,_sysinfo(cJSON_CreateObject())
    ,_diskinfo(cJSON_CreateObject())
    ,_decnum(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

InterfaceResCode CGetHostConfig::GetHostConfig(string &sResult)
{
    //TODO
    mLogInfo("获取主机网络配置和通道配置信息...");
    CData oResult = SVSMAP();
    char cResult[1024] = {0};
#if 1
    if(getMPSConfig(oResult, cResult) == false){
        mLogError("Failed to run  getMPSConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 
    //getMPSConfig(oResult, cResult);
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("getMPSConfig\n");
    for (int i = 0; i < iRecvLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cResult[i]);
	    }
	    else 
	    {
		    printf("%c", cResult[i]);
	    }
    }
#endif 
 
#if 1
    if( _AnalyParas.ConvertSTRING2CJSONGetHostConfig(oResult, cResult, _network, _sysinfo, _diskinfo, _decnum) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetHostConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

bool CGetHostConfig::ConvertCDATA2CJSON(CData &oResult)
{
    CNetworkNode networkData;
    CChannelNode channelData;
    CData oNetworkNode = SVSMAP();
    CData oRtspNode = SVSMAP();
    CData oChannelsNode = SVSMAP();
    if(!getNetworkNode(oResult,oNetworkNode)){
        mLogError("Failed to run getNetworkNode(CData,CData).");
        return false;
    }
    if(!analyzeNetworkCDataNodeNetwork(oNetworkNode,&networkData)){
        mLogError("Failed to run analyzeNetworkCDataNodeNetwork(CData,CNetworkNode).");
        return false;
    }

    if(!AddNetworkNode(networkData)){
        mLogError("Failed to run AddNetworkNode(networkData).");
        return false;
    }

    if(!getRtspCDataNode(oResult,oRtspNode)){
        mLogError("Failed to run getRtspCDataNode(CData,CData).");
        return false;
    }
    if(!analyzeRtspCDataNodeChannel(oRtspNode,&channelData)){
        mLogError("Failed to run analyzeRtspCDataNodeChannel(CData,CChannelNode).");
        return false;
    }
    if(!getChannlesCDataVectorNode(oResult,oChannelsNode)){
        mLogError("Failed to run getChannlesCDataVectorNode(CData,CData).");
        return false;
    }
    //打印返回信息
    try{
//        CData ch=SVSMAP();
        oChannelsNode= oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["channels"];
        int size = oChannelsNode.RefVector().size();
        mLogInfo("get channles size:"<<size);
    }catch(exception& e){
        mLogError("get channles size error."<<e.what());
    }

    //TODO
    //添加 channelData信息
    //合成channelNode
    int size = oChannelsNode.RefVector().size();
    for (std::size_t i = 0; i < size; i++)
    {
        CData oChannel = oChannelsNode.RefVector().at(i).RefMap();
        //打印返回信息
        try{
            string sResult;
            if(!CData::ToCustomXMLString(oChannel.RefMap(),sResult)){
                mLogDebug("Convert xml data to xml string failed.");
            }
            mLogDebug(" sResult:\r\n"<<sResult);
        }catch(exception& e){
            mLogDebug("ToCustomXMLString(oChannel.RefMap(),...) error."<<e.what());
        }
        mLogDebug("add num i: "<<(int)i);
        if(!analyzeChannelsCDataNodeChannel(oChannel,(int)i,&channelData)){
            mLogError("Failed to run analyzeChannelsCDataNodeChannel(CData,chNum,...)=(CData,"<<i<<".");
            return false;
        }else{
            JSONChannelNode JChannelNode;
            convertCData2channelNode(&networkData,&channelData,&JChannelNode);
            if(!AddChannelsNode(JChannelNode)){
                mLogError("Failed to run AddChannelsNode(JSONChannelNode).");
                return false;
            }
        }
    }
    return true;
}

bool CGetHostConfig::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    if(_network == NULL){
        mLogWarn("cJSON network == NULL.");
        return false;
    }
    if(_channels == NULL){
        mLogWarn("cJSON channels == NULL.");
        return false;
    }

    //result中添加节点
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_SYSINFO,_sysinfo);
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_NETWORK,_network);
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_DISKINFO,_diskinfo);
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_DECNUM,_decnum);

    return true;
}

string CGetHostConfig::ComposeResult()
{
    cJSON * result = cJSON_CreateObject();
    if(ComposeResult(result) == false){
        cJSON_Delete(result);
        return "";
    }else{
        char *cRes = cJSON_Print(_result);
        string res = cRes;
        cJSON_Delete(result);
        return res;
    }
}

bool CGetHostConfig::AddNetworkNode(CNetworkNode &networkData)
{
    if(_network == NULL){
        mLogError("cJSON network == NULL.");
        return false;
    }

    cJSON_AddItemToObject(_network,GET_HOST_CONFIG_RES_NW_IP        ,cJSON_CreateString("192.168.6.99"));
    cJSON_AddItemToObject(_network,GET_HOST_CONFIG_RES_NW_GATEWAY   ,cJSON_CreateString("255.255.255.0"));
    cJSON_AddItemToObject(_network,GET_HOST_CONFIG_RES_NW_NETMASK   ,cJSON_CreateString("192.168.16.1"));

    return true;
}

bool CGetHostConfig::AddSysInfoNode(void)
{
    if(_sysinfo == NULL){
        mLogError("cJSON network == NULL.");
        return false;
    }

    cJSON_AddItemToObject(_sysinfo,GET_HOST_CONFIG_RES_SYS_DEVTYPE  ,cJSON_CreateString("SV-1"));
    cJSON_AddItemToObject(_sysinfo,GET_HOST_CONFIG_RES_SYS_HVER     ,cJSON_CreateString("V1.0.0.0"));
    cJSON_AddItemToObject(_sysinfo,GET_HOST_CONFIG_RES_SYS_SVER   ,cJSON_CreateString("V1.0.0.0"));
    cJSON_AddItemToObject(_sysinfo,GET_HOST_CONFIG_RES_SYS_SN     ,cJSON_CreateString("SV1"));
    cJSON_AddItemToObject(_sysinfo,GET_HOST_CONFIG_RES_SYS_CHANNELNUM     ,cJSON_CreateNumber(16));

    return true;
}

bool CGetHostConfig::AddDiskInfoNode(void)
{
    if(_diskinfo == NULL){
        mLogError("cJSON network == NULL.");
        return false;
    }

    cJSON_AddItemToObject(_diskinfo,GET_HOST_CONFIG_RES_DI_TOTALSIZE ,cJSON_CreateNumber(1024));
    cJSON_AddItemToObject(_diskinfo,GET_HOST_CONFIG_RES_DI_FREESIZE  ,cJSON_CreateNumber(200));
    cJSON_AddItemToObject(_diskinfo,GET_HOST_CONFIG_RES_DI_AUTODELSIZE,cJSON_CreateNumber(16));

    return true;
}



bool CGetHostConfig::AddChannelsNode(JSONChannelNode &channelData)
{
    if(_channels == NULL){
        mLogError("cJSON channels == NULL.");
        return false;
    }

    cJSON * chArrayObj = cJSON_CreateObject();
    cJSON_AddItemToObject(chArrayObj,GET_HOST_CONFIG_RES_CH_CHANNELNUM    ,cJSON_CreateNumber(channelData.channelNum));
    cJSON_AddItemToObject(chArrayObj,GET_HOST_CONFIG_RES_CH_CHANNELNAME   ,cJSON_CreateString(channelData.channelName.c_str()));
    cJSON_AddItemToObject(chArrayObj,GET_HOST_CONFIG_RES_CH_CHANNELSTATUS ,cJSON_CreateNull());
    cJSON_AddItemToObject(chArrayObj,GET_HOST_CONFIG_RES_CH_CHANNELENABLE ,cJSON_CreateBool(channelData.channelEnable));

    cJSON_AddItemToObject(chArrayObj,GET_HOST_CONFIG_RES_CH_RTSPURL       ,cJSON_CreateString(channelData.rtspUrl.c_str()));
    cJSON_AddItemToObject(chArrayObj,GET_HOST_CONFIG_RES_CH_RTSPENABLE    ,cJSON_CreateBool(channelData.rtspEnable));

    cJSON_AddItemToObject(chArrayObj,GET_HOST_CONFIG_RES_CH_RTMPURL       ,cJSON_CreateString(channelData.rtmpUrl.c_str()));
    cJSON_AddItemToObject(chArrayObj,GET_HOST_CONFIG_RES_CH_RTMPENABLE    ,cJSON_CreateBool(channelData.rtmpEnable));

    cJSON_AddItemToArray(_channels,chArrayObj);
    return true;
}

bool CGetHostConfig::getMPSConfig(CData &oResult, char *cResult)
{
    mLogDebug("run GetMPSConfig(...)");
    //获取状态
    //CData oResult = SVSMAP();

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cTmpCmd) + 4;
    cHeadCmd[0] = 3; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cTmpCmd) >> 8;  
    cHeadCmd[3] = strlen(cTmpCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cTmpCmd, strlen(cTmpCmd));
    MPSOperationRes opResCode = eMPSResultOK;

    // ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    ResponseCode resCode = _client->GetConsoleNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CGetHostConfig::getNetworkNode(CData &oResult, CData &oNetWork)
{
    try{
        oNetWork = oResult.RefMap()["network"].RefMap();
        return true;
    }catch(exception &e){
        try{
            oNetWork = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["network"].RefMap();
            return true;
        }catch(exception &e){
            mLogError("Failed to try getNetworkNode(CData,CData),e:"<<e.what());
            return false;
        }
    }
}

bool CGetHostConfig::getChannlesCDataVectorNode(CData &oResult, CData &oChannels)
{
    try{
        oChannels = oResult.RefMap()["channels"];
        return true;
    }catch(exception &e){
        try{
            oChannels = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["channels"];
            return true;
        }catch(exception &e){
            mLogError("Failed to try getChannlesCDataVectorNode(CData,CData),e:"<<e.what());
            return false;
        }
    }
}

bool CGetHostConfig::getRtspCDataNode(CData &oResult, CData &oRtsp)
{
    try{
        oRtsp = oResult.RefMap()["rtsp"].RefMap();
        return true;
    }catch(exception &e){
        try{
            oRtsp = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["rtsp"].RefMap();
            return true;
        }catch(exception &e){
            mLogError("Failed to try getRtspCDataNode(CData,CData),e:"<<e.what());
            return false;
        }
    }
}

bool CGetHostConfig::analyzeNetworkCDataNodeNetwork(CData &oNetwork, PCNetworkNode networkData)
{
    try{
        networkData->ip      = oNetwork.RefMap()["ip"      ].RefString();
        networkData->gateway  = oNetwork.RefMap()["gateway" ].RefString();
        networkData->netmask = oNetwork.RefMap()["netmask" ].RefString();
        return true;
    }catch(exception &e){
        mLogError("Failed to try analyzeNetworkCDataNodeNetwork(CData,PCNetworkNode),e:"<<e.what());
        return false;
    }
}

bool CGetHostConfig::analyzeChannelsCDataNodeChannel(CData &oChannel, int channleNum, PCChannelNode chData)
{
    try{
        chData->channelNum            = channleNum+1;
        chData->channelEnable         = oChannel.RefMap()["channelenable"].RefBool();

        chData->interRtmp_Ip          = oChannel.RefMap()["intertmp"].RefMap()["ip"].RefString();
        chData->interRtmp_Port        = oChannel.RefMap()["intertmp"].RefMap()["port"].RefLong16();
        chData->interRtmp_AppName     = oChannel.RefMap()["intertmp"].RefMap()["appname"].RefString();
        chData->interRtmp_ReleaseName = oChannel.RefMap()["intertmp"].RefMap()["releasename"].RefString();
        chData->interRtmp_Enable      = oChannel.RefMap()["intertmp"].RefMap()["enable"].RefBool();

//        chData->rtsp_Ip          = oRtsp.RefMap()["ip"].RefString();
//        chData->rtsp_Port        = oRtsp.RefMap()["port"].RefLong16();
//        chData->rtsp_AppName     = oRtsp.RefMap()["appname"].RefString();
//        chData->rtsp_Enable      = oRtsp.RefMap()["enable"].RefBool();
        return true;
    }catch(exception &e){
        mLogError("Failed to try analyzeChannelsCDataNodeChannel(CData,PCChannelNode),e:"<<e.what());
        return false;
    }
}

bool CGetHostConfig::analyzeRtspCDataNodeChannel(CData &oRtsp, PCChannelNode chData)
{
    try{
        chData->rtsp_Ip          = oRtsp.RefMap()["ip"].RefString();
        chData->rtsp_Port        = oRtsp.RefMap()["port"].RefLong16();
        chData->rtsp_AppName     = oRtsp.RefMap()["appname"].RefString();
        chData->rtsp_Enable      = oRtsp.RefMap()["enable"].RefBool();
        return true;
    }catch(exception &e){
        mLogError("Failed to try analyzeRtspCDataNodeChannel(CData,PCChannelNode),e:"<<e.what());
        return false;
    }
}

bool CGetHostConfig::convertCData2channelNode(PCNetworkNode oNetwork, PCChannelNode oChannelData, PJSONChannelNode jChannelData)
{
    try{
        //channel
        jChannelData->channelNum = oChannelData->channelNum;
        jChannelData->channelName = "";
        jChannelData->channelEnable = oChannelData->channelEnable;

        if(oChannelData->channelNum < 0){
            mLogError("channel-channelNum:"<<oChannelData->channelNum<<" <0");
            return false;
        }

        //rtmp
        string rtmpIp;
        if(strcmp(oChannelData->interRtmp_Ip.c_str(),"127.0.0.1") == 0){
            rtmpIp = oNetwork->ip;
        }else{
            rtmpIp = oChannelData->interRtmp_Ip;
        }
        // rtmp://192.168.6.99:1930/live/ch1
        string strRtmp = "rtmp://";
        jChannelData->rtmpUrl = strRtmp+
                                rtmpIp+ ":"+
                                boost::lexical_cast<string>(oChannelData->interRtmp_Port)+"/" +
                                oChannelData->interRtmp_AppName+"/"+
                                oChannelData->interRtmp_ReleaseName;
        mLogWarn("num:"<<oChannelData->channelNum<<"; releaseName:"<<oChannelData->interRtmp_ReleaseName);
        jChannelData->rtmpEnable = oChannelData->interRtmp_Enable;

        //rtsp
        string rtspIp;
        if(strcmp(oChannelData->rtsp_Ip.c_str(),"127.0.0.1") == 0){
            rtspIp = oNetwork->ip;
        }else{
            rtspIp = oChannelData->rtsp_Ip;
        }
        // rtsp://192.168.6.99:554/live00
        string strRtsp = "rtsp://";
        jChannelData->rtspUrl = strRtsp+
                                rtspIp+ ":"+
                                boost::lexical_cast<string>(oChannelData->rtsp_Port)+"/" +
                                oChannelData->rtsp_AppName+
                                boost::lexical_cast<string>(oChannelData->channelNum)+"0";
        jChannelData->rtspEnable = oChannelData->rtsp_Enable;

    }catch(exception &e){
        mLogError("Failed to try convertCData2channelNode(PCNetworkNode,PCChannelNode,PJSONChannelNode), e:"<<e.what());
        return false;
    }
}
