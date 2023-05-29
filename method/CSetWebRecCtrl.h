#ifndef CSETWEBRECCTRL_H
#define CSETWEBRECCTRL_H
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
#include <sys/types.h> 
#include <sys/stat.h>
#include <dirent.h>
#include <cstddef>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "interfaceDefines.h"
#include "MPSClient.h"
#include "CAnalyzeParaProc.h"

#define INFO_LIST_MAX_BUFF 1024*1024

class CSetWebRecCtrl
{
public:
    CSetWebRecCtrl(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief SetRecCtrlStart 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebRecCtrl(string &sResult,int ChnID, int OnOff, string TaskID, int CurrentState);

    /**
     * @brief GetRecCtrlInfoList 获取主机配置信息
     * @param GetMode
     * @param TaskId
     * @param StartTime
     * @param EndTime
     * @return true：成功，false：失败.
     */
    InterfaceResCode GetWebRecCtrlInfoList(string &sResult,string TaskId, string StartTime, string EndTime, int PageSize, int PageNum, int GetMode);

    /**
     * @brief SetRecCtrlTaskDel 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebRecCtrlTaskDel(string &sResult, string TaskId);

    /**
     * @brief SetWebRecSegModel 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebRecSegModel(string &sResult,int ChnID,string SegModel);

    /**
     * @brief SetWebRecSegModelSelect 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebRecSegModelSelect(string &sResult,int ChnID,int WinID,int VideoID);

    /**
     * @brief SetWebRecMatrix 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode SetWebRecMatrix(string &sResult,int OnOff, int ChnID,int VideoID);
    /**
     * @brief GetWebRecSegModel 获取合成控制接口-分割模式选择
     * @param sResult
     * @return 0：成功，!0：失败.
     */
	InterfaceResCode GetWebRecSegModel(string &sResult,int ChannelID);
	 /**
     * @brief GetWebRecSegModel 获取合成控制接口-分割模式选择
     * @param sResult
     * @return 0：成功，!0：失败.
     */
	bool GetWebRecSegModelInner(int ChannelID, CData &oResult, char *cResult);
	  /**
     * @brief GetWebRecSegModel 获取合成控制接口-分割模式选择
     * @param sResult
     * @return 0：成功，!0：失败.
     */
	InterfaceResCode GetWebRecTaskID(string &sResult);
	  /**
     * @brief GetWebRecSegModel 获取合成控制接口-分割模式选择
     * @param sResult
     * @return 0：成功，!0：失败.
     */
	void getFiles(std::string path, std::vector<std::string> &files, std::vector<std::string> &filesPath);
	 /**
	* @brief GetWebRecSegModel 获取合成控制接口-分割模式选择
	* @param sResult
	* @return 0：成功，!0：失败.
	*/
	InterfaceResCode SetWebRcrvRecord(string &sResult, int RecOn);
	  /**
	* @brief GetWebRecSegModel 获取合成控制接口-分割模式选择
	* @param sResult
	* @return 0：成功，!0：失败.
	*/
	InterfaceResCode GetWebRcrvRecord(string &sResult);

	
	InterfaceResCode SetWebRecFileDel(string &sResult,string TaskId, int type, string filename);

	InterfaceResCode SetWebTaskIDUpload(string &sResult, string TaskId);

private:
    /**
     * @brief ComposeResult 合并结果
     * @param result
     * @return true：成功，false：失败.
     */
    bool ComposeResult(cJSON * result);
    /**
     * @brief ComposeResult 合并结果
     */
    string ComposeResult();
    /**
     * @brief ComposeResult 合并结果
     * @param result
     * @return true：成功，false：失败.
     */
    bool GetComposeResult(cJSON * result);
    /**
     * @brief ComposeResult 合并结果
     */
    string GetComposeResult();
    /**
     * @brief SetRecStart 获取主机配置信息
     * @param RtspNum
     * @param H323Num
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebRec(int ChnID,int OnOff, string TaskID, int CurrentState, CData &oResult, char *cResult);

    /**
     * @brief GetRecCtrlInfoList 获取主机配置信息
     * @param sResult
     * @param GetMode
     * @param TaskId
     * @param StartTime
     * @param EndTime
     * @return true：成功，false：失败.
     */
    bool GetWebRecInfoList(int *GetMode, string TaskId, string StartTime, string EndTime, int StartIndex, int EndIndex, CData &oResult, char *cResult);
  
    /**
     * @brief SetRecTaskDel 获取主机配置信息
     * @param TaskId
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebRecTaskDel(string TaskId, CData &oResult, char *cResult);

    /**
     * @brief SetWebSegModel 获取主机配置信息
     * @param TaskId
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebSegModel(int ChnId, string SegModel, CData &oResult, char *cResult);

    /**
     * @brief SetWebSegModelSelect 获取主机配置信息
     * @param TaskId
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebSegModelSelect(int ChnId, int WinID, int VideoID, CData &oResult, char *cResult);

    /**
     * @brief SetWebSegModelSelect 获取主机配置信息
     * @param TaskId
     * @param oResult
     * @param cResult
     * @return true：成功，false：失败.
     */
    bool SetWebMatrix(int OnOff, int WinID, int VideoID, CData &oResult, char *cResult);
	
	bool SetWebRcrvRecordInner(CData &oResult, char *cResult, int RecOn);
	bool GetWebRcrvRecordInner(CData &oResult, char *cResult);
	void RecgetFiles(std::string path, std::vector<std::string> &files, std::vector<std::string> &filesPath);
	bool SetWebRecFileDelInner(string TaskId, int type, string filename, CData &oResult, char *cResult);
    bool SetWebTaskIDUploadInner(string TaskId,     CData &oResult, char *cResult);
	bool getDecConfig(int DecId,CData &oResult, char *cResult);
private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;
	int GopCallFlag;
	string StrParam[20];

    MPSClient *_client;
    CAnalyzeParaProc _AnalyParas;

    //用于存储结果
    cJSON *_result;
	cJSON *_sipdec;
    cJSON *_VideoFiles;
};

#endif // CSETWEBRECCTRL_H
