#ifndef CGETTASKCONTENT_H
#define CGETTASKCONTENT_H
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

#define FILE_ROOT_PATH  "/store"
#define FTP_ROOT_PATH   "ftp://"

//存储节点videoFile信息
typedef struct{
    string caseId;		//唯一的任务标识，案件ID
    string fileName;	//文件名称
    double size;		//文件大小，单位G
    string filePath;	//文件在主机上的路径
    string startTime;	//文件录制开始时间
    string endTime;		//文件录制结束时间
    int channelNumber;	//通道编号
    string channelDesc;	//录制通道的通道名称
    string ftpPath;		//文件所在ftp地址
    string vodurlRtsp;	//文件点播rtsp流地址
    string vodurlRtmp;	//文件点播rtmp流地址
    int fileSort;		//文件序列号1-n
}VideoFileNode, *PVideoFileNode;

//GetPackage接口获取到的xml中视频文件节点信息package
typedef struct{
    string keyID;       // 关键字，也是目录名（二级目录）
    string title;
    int worknumber;     // 录制文件的通道号
    string startTime;   // 录制开始时间
    string endTime;     // 录制结束时间
    string month;       // 录制开始年月
    int status;         // 工作状态。 0 代表未开始； 1 代表正在工作； 2 代表暂停； 3 代表结束； 4 代表关闭； 5 代表异常
    bool allowDelete;   // 是否允许删除； 0：不允许， 1：允许
}CDataPackageNode,*PCDataPackageNode;

//GetPackage接口获取到的xml中视频文件节点信息package-video-primary
typedef struct{
    string fileName;    // 文件名
    unsigned long long fileSize;      // 文件大小，单位Byte
    int fileType;       // 文件类型
    int rate;           // 码流，单位 Kbit/s
    int resolution;     //分辨率； 0： QCIF， 1：CIF， 2： D1， 3： VGA， 4： 720p
    string startTime;   // 录制开始时间
    int duration;       // 本段文件分割时长，单位秒
    int chNum;          //文件所属通道号
    int seqNum;         //文件排序序号
}CDataVideoPrimaryNode,*PCDataVideoPrimaryNode;

class CGetTaskContent
{


public:
    CGetTaskContent(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief getTaskContent 获取指定任务内容列表
     * @param caseId 唯一的任务标识，即案件id号
     * @param result 获取的内容列表
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode GetTaskContent(string caseId,cJSON *result);
    InterfaceResCode GetTaskContent(string caseId, string &sResult);

    cJSON * getCResult();
    string getSResult();

public:
    /**
     * @brief GetSelfMsg 获取自身ip/file root path 地址，通过GetSysConfig接口获取
     * @param ip
     * @param fileRootPath
     * @return true：成功，false：失败.
     */
    bool GetSelfMsg(string &ip,string &fileRootPath);
    /**
     * @brief ConvertPackageCDATA2CJSON 通过GetPackage接口获取到的xml转换为需求的CJSON格式
     * @param oResult 通过GetPackage接口获取到的xml
     * @param cResult 转换得到需求的CJSON格式
     * @return true：成功，false：失败.
     */
    bool ConvertPackageCDATA2CJSON(CData &oResult,cJSON *cResult);
    /**
     * @brief ComposeResult 合成CJSON形式的返回数据
     * @param result 生成的CJSON格式数据
     * @return true：成功，false：失败.
     */
    bool ComposeResult(cJSON *result);
    /**
     * @brief ComposeResult 合成CJSON形式的返回数据
     * @return true：成功，false：失败.
     */
    string ComposeResult();
    /**
     * @brief AddVideoFiles 向返回数据中videoFiles数组节点添加videoFiles数据
     * @param videoFile videoFiles数据
     * @return true：成功，false：失败.
     */
    bool AddVideoFiles(VideoFileNode & videoFile);

    /**
     * @brief AddRootNode
     * @return
     */
    bool AddRootNode(CDataPackageNode &packageNode);

private:
    /**
     * @brief getPackageCDataVideoVectorNode 解析通过GetPackage接口获取到的xml中视频文件节点package-video
     * @param result 通过GetPackage接口获取到的xml
     * @param videoNode 解析后得到的video节点
     * @return true：成功，false：失败.
     */
    bool getPackageCDataVideoVectorNode(CData &result,CData &videoNode);
    /**
     * @brief getCDATAPackageNode 解析通过GetPackage接口获取到的xml中视频文件节点package
     * @param result
     * @param packageNode
     * @return
     */
    bool getCDATAPackageNode(CData &result,CData &packageNode);
    /**
     * @brief analyzePackageCDataNodePackage 解析通过GetPackage接口获取到的xml中视频文件节点信息package
     * @param video package-video-AXXX-primary-AXX
     * @param videoNode 解析后得到的数据
     * @return true：成功，false：失败.
     */
    bool analyzePackageCDataNodePackage(CData &package,PCDataPackageNode packageNode);
    /**
     * @brief analyzePackageCDataNodeVideo 解析通过GetPackage接口获取到的xml中视频文件节点信息package-video-primary
     * @param video package-video-AXXX-primary-AXX
     * @param chNum 视频文件的通道号
     * @param seqNum 视频文件的序号
     * @param videoNode 解析后得到的数据
     * @return true：成功，false：失败.
     */
    bool analyzePackageCDataNodeVideo(CData &video, int chNum, int seqNum, PCDataVideoPrimaryNode videoNode);

    /**
     * @brief convertCData2VideoFileNode 根据获取到的packageNode和videoNode生成VideoFileNode信息
     * @param packageNode
     * @param videoNode
     * @param videoFile
     */
    void convertCData2VideoFileNode(PCDataPackageNode packageNode,PCDataVideoPrimaryNode videoNode,VideoFileNode & videoFile);

    time_t string2Datetime(string str);
    string datetime2String(time_t time);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;
    string _selfIp; //用于构建rtmp流地址
    string _fileRootPath;   //文件存放根目录
    MPSClient *_client;
    //用于存储结果
    cJSON *_videoFiles;
    cJSON *_result;
};

#endif // CGETTASKCONTENT_H
