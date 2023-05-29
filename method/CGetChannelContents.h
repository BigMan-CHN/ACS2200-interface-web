#ifndef CGETCHANNELCONTENTS_H
#define CGETCHANNELCONTENTS_H
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

//getChannelContents接口参数time字符串长度
#define GET_CHANNLE_CONTENTS_TIME_STR_LEN   10

//存储节点videoFile信息
typedef struct{
    string caseId;		// 唯一的任务标识，案件ID
    string title;       //
    int status;         // 工作状态。0代表未开始；1代表正在工作；2代表暂停；3代表结束；4代表关闭；5代表异常
    bool allowDelete;   // 是否允许删除；0：不允许，1：允许
    string month;       // 录制年月
    string startTime;   // 录制开始时间
    string endTime;     // 录制结束时间
}CaseNode, *PCaseNode;

class CGetChannelContents
{
public:
    CGetChannelContents(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);
    /**
     * @brief GetChannelContents 获取指定任务内容列表
     * @param result 获取的内容列表
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode GetChannelContents(string startTime, string endTime);
    InterfaceResCode GetChannelContents(string startTime, string endTime, string &sResult);

    cJSON * getCResult();
    string getSResult();

private:
    /**
     * @brief checkTimeStr 检查time字符串是否正确
     * @param timeStr
     * @return true：正确，false：错误.
     */
    bool checkTimeStr(string timeStr);
    /**
     * @brief ConvertPackageListCDATA2CJSON 通过GetPackageList接口获取到的xml转换为需求的CJSON格式
     * @param oResult 通过GetPackageList接口获取到的xml
     * @return true：成功，false：失败.
     */
    bool ConvertPackageListCDATA2CJSON(CData &oResult);
    /**
     * @brief getCDataPackageListNode 解析通过GetPackageList接口获取到的xml中视频文件节点packageList
     * @param result 通过GetPackageList接口获取到的xml
     * @param caseNode 解析后得到的case节点
     * @return true：成功，false：失败.
     */
    bool getCDataPackageListNode(CData &result,CData &caseNode);
    /**
     * @brief analyzePackageListCDataNodeCase 解析通过GetPackageList接口获取到的xml中视频文件节点信息packageList
     * @param packageList packageList
     * @param caseNode 解析后得到的数据
     * @return true：成功，false：失败.
     */
    bool analyzePackageListCDataNodeCase(CData &packageList,PCaseNode caseNode);
    /**
     * @brief checkCaseNodeTime 检查caseNode时间是否符合
     * @param caseNode case数据
     * @return true：符合，false：不符合.
     */
    bool checkCaseNodeTime(CaseNode &caseNode);
    /**
     * @brief AddCaseJNode 向返回数据中case数组节点添加caseJNode数据
     * @param caseNode case数据
     * @return true：成功，false：失败.
     */
    bool AddCaseJNode(CaseNode &caseNode);
    /**
     * @brief ComposeResult 合成CJSON形式的返回数据
     * @return true：成功，false：失败.
     */
    string ComposeResult();
private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;
    string _selfIp; //用于构建rtmp流地址
    MPSClient *_client;
    //
    string _startTime;
    string _endTime;
    //用于存储结果
    cJSON *_caseJNode;
    cJSON *_result;
    int _caseSize;
};

#endif // CGETCHANNELCONTENTS_H
