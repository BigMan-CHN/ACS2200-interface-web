#ifndef CANALYZEPARAPROC_H
#define CANALYZEPARAPROC_H
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
#include "stu_sqlite.h"
#include "sqlite3.h"

using namespace std;
using namespace SVS;

#define MAX_ARG_NUM 16
#define CMD_STR_ERR		-1
#define ARG_STR_ERR		-2
#define CMD_DISABLE		-3
#define XML_SAVE_ERR	-4
#define PROC_SUCCESS	0

typedef enum{
 CHNSYS_VID_CAP_OP = 0,
 CHNSYS_VID_ENC_OP,
 CHNSYS_VID_DEC_OP,
 CHNSYS_VID_OUT_OP,
 CHNSYS_VID_COMP_OP,
 CHNSYS_NET_RECV_OP,  //5
 CHNSYS_NET_SEND_OP, 
 CHNSYS_PCI_VID_IN_OP,
 CHNSYS_PCI_VID_OUT_OP,
 CHNSYS_PCI_AUD_IN_OP,
 CHNSYS_PCI_AUD_OUT_OP, //10
 CHNSYS_AUD_CAP_OP,
 CHNSYS_AUD_ENC_OP,
 CHNSYS_AUD_DEC_OP,
 CHNSYS_AUD_OUT_OP,
 CHNSYS_HDMI_AUD_OUT_OP, //15
 CHNSYS_H323H_OP,
 CHNSYS_SIP_OP,
 CHNSYS_REC_OP,
 CHNSYS_DEMUX_OP,
 CHNSYS_SCREENSHOT_OP,//20
 CHNSYS_ETH_OP,   
 CHNSYS_MOSAIC_OP,
 CHNSYS_NM_OP,
 CHNSYS_MN_OP,
 CHNSYS_CORE_GET,//25
 CHNSYS_VM_OP,   

 CHNSYS_DEV_NOCHECK_STA_FLAG,

 CHNSYS_SYSLOG_OP,
 CHNSYS_SYS_OP,
 CHNSYS_TDC_OP,//30
 CHNSYS_DEL_OP,   
 CHNSYS_RZK_OP,
 CHNSYS_AM_OP, 
 CHNSYS_DVC_GET,

 CHNSYS_LINKS_GET,//35
 CHNSYS_STATUS_GET,  
 CHNSYS_ENCRYPT_GET,
 CHNSYS_WEB_GET, //web

 CHNSYS_BACK_PIC_OP,
 CHNSYS_NTP_OP,//40
 CHNSYS_AUDIO_INFO__OP, 
 CHNSYS_FTP_INFO_OP,//42
 CHNSYS_YUAN_INFO_OP,//43
 CHNSYS_RTMP_RECV_SEND_OP,
 CHNSYS_RTMP_RECV_RECV_OP,
 CHNSYS_RTMP_SEND_RECV_OP,
 CHNSYS_RTMP_SEND_SEND_OP,

 CHNSYS_DEV_NOCHECK_END_FLAG,
}OPERTE_G;

/* 保存多个指令的结构，字符串数组从0开始赋值*/  
typedef struct {  
	int number;        /*分割的字符串个数*/  
	char** string;        /*字符串数组*/  
}StringTab; 

typedef struct{
    int    RtspId;
    string RtspUrl;
    int    RtspMode;
    string   RtspProtol;

}CRtspNode, *PRtspNode;

class CAnalyzeParaProc
{
public:
    //转换String为需求的CJSON格式
    bool ConvertSTRING2CJSONGetHostConfig(CData &oResult, char *cResult, cJSON *network, cJSON *sysinfo, cJSON *diskinfo, cJSON *decnum);

    bool ConvertSTRING2CJSONGetWebHostConfig(CData &oResult, char *cResult, cJSON *network, cJSON *sysinfo, cJSON *otherinfo,string Method);

    bool ConvertSTRING2CJSONGetHostDecInfo(CData &oResult, char *cResult, cJSON *rtspdec, cJSON *h323dec);

    bool ConvertSTRING2CJSONGetHostChnInfo(CData &oResult, char *cResult, cJSON *status, cJSON *channal);

    bool ConvertSTRING2CJSONGetWebHostChnInfo(CData &oResult, char *cResult, cJSON* result, cJSON *node[], cJSON* _addressList, string Method, int ChnNum, string AudioBoard);

    bool ConvertSTRING2CJSONSetWebAudioCtrl(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONSetWebExtendCtrl(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONSetDecCtrl(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONSetWebMultiCtrl(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONSetOsdCtrl(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONSetPutNoteFileCtrl(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONSetRecCtrl(CData &oResult, char *cResult, cJSON *cRecStatus);

    bool ConvertSTRING2CJSONSetWebRecCtrl(CData &oResult, char *cResult, cJSON *cRecStatus);

    bool ConvertSTRING2CJSONSetDiscipLineCtrl(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONGetDiscipLineCtrl(CData &oResult, char *cResult, cJSON *statusinfo);

    bool ConvertSTRING2CJSONSetWebEffectCtrl(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONSetStreamCtrl(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONSetPowerCtrl(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONSendConsoleCmd(CData &oResult, char *cResult);

    bool ConvertSTRING2CJSONGetRecCtrl(CData &oResult, char *cResult, int GetMode, cJSON *videofiles, cJSON *recresult, int StartIndex, int EndIndex);


    //add by libx in 2022/02/23
    //--[BEGIN]--
    bool ConvertSTRING2CJSONGetWebInfraredStudyList(CData& oResult, char* cResult, cJSON* result, cJSON* videoParamList);
    bool ConvertSTRING2CJSONSetWebInfraredStudy(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebInfraredDel(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebInfraredSend(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebInfraredCreate(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONGetWebAudioConf(CData& oResult, char* cResult, cJSON* result, cJSON* audioParamList);
    bool ConvertSTRING2CJSONSetWebAudioConf(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONGetWebAudioGroupConf(CData& oResult, char* cResult, cJSON* result, cJSON* audioGroupList);
    bool ConvertSTRING2CJSONSetWebAudioGroupConf(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONGetWebAudioMatrixConf(CData& oResult, char* cResult, cJSON* result, cJSON* audioMatrixList);
    bool ConvertSTRING2CJSONSetWebAudioMatrixConf(CData& oResult, char* cResult);
	//ltz changed 03-07
    bool ConvertSTRING2CJSONGetWebVideoParam(CData& oResult, char* cResult, cJSON* result, cJSON* videoParamList,cJSON* node[], string Method, string &ip, int ParamNum);
	bool ConvertSTRING2CJSONSetWebVideoParam(CData& oResult, char* cResult);
	//ltz changed 03-08
    bool ConvertSTRING2CJSONGetWebAudioParam(CData& oResult, char* cResult, cJSON* result, cJSON* audioParamList,cJSON* node[], string Method, string &ip, int ParamNum);
    bool ConvertSTRING2CJSONSetWebAudioParam(CData& oResult, char* cResult);
	//ltz changed 03-08
    bool ConvertSTRING2CJSONGetWebRtspParam(CData& oResult, char* cResult, cJSON* result, cJSON* ParamList,cJSON* node[], string Method, string &ip, int ParamNum);
    bool ConvertSTRING2CJSONSetWebRtspParam(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONGetWebRtmpParam(CData& oResult, char* cResult, cJSON* result, cJSON* ParamList,cJSON* node[], string Method, string &ip, int ParamNum);
    bool ConvertSTRING2CJSONSetWebRtmpParam(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebLogFileConf(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONGetWebLogFile(CData& oResult, char* cResult, cJSON* result, cJSON* logList);
    bool ConvertSTRING2CJSONSetWebDevID(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebRtmpBroadConf(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebFirmUpdate(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebZKTUpdate(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebZKTDownload(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebConfigUpdateInner(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebDiscipFileUpdate(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebBackupCfgFile(CData& oResult, char* cResult);
    bool ConvertSTRING2CJSONSetWebRecoverCFGFile(CData& oResult, char* cResult);
    //--[END]--
    //add by ltz 2022-03-09
    bool ConvertSTRING2CJSONGetWebUserPasswordParam(CData& oResult, char* cResult, string &Oldpassword);
    bool ConvertSTRING2CJSONGetWebVideoChnListParam(CData& oResult, char* cResult, string Method,cJSON* result, cJSON* _addressList, cJSON* node[], int chn_num);
    bool ConvertSTRING2CJSONGetWebDevIDParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList);
    bool ConvertSTRING2CJSONGetWebUartConfListParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList, cJSON* node, int chn_num);
    bool ConvertSTRING2CJSONGetWebVideoOutPutConfListParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList, cJSON* node, int chn_num);
    bool ConvertSTRING2CJSONGetWebFtpConfListParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList,string Method,string &LocallIP);
    bool ConvertSTRING2CJSONGetWebStorageConfListParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList);
    bool ConvertSTRING2CJSONParseSerialCmdParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList,cJSON* node);
    bool ConvertSTRING2CJSONLoginCheckParam(CData& oResult, char* cResult, string &Oldpassword, string &username);
    bool ConvertSTRING2CJSONGetWebRecSegModel(CData& oResult, char* cResult, cJSON* result);
    bool ConvertSTRING2CJSONGetWebCaptionListParam(CData& oResult, char* cResult,string Method, cJSON* result, cJSON* _addressList, cJSON* node[], int osd_num);
    bool ConvertSTRING2CJSONGetWebMosaicListParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList, cJSON* node, int masic_num);
    bool ConvertSTRING2CJSONGetWebDSPMatrixParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList,cJSON* node, int OutputType);
    bool ConvertSTRING2CJSONGetWebStartAutoRecParam(CData& oResult, char* cResult, cJSON* result, cJSON* _addressList);
    bool ConvertSTRING2CJSONGetWebStartRecParam(CData& oResult, char* cResult, string Method, cJSON* result, cJSON* _addressList,cJSON* node[], int VideoEncChnnel, string &ip);
	bool ConvertSTRING2CJSONGetHostInfo(CData &oResult, char *cResult, string Method, cJSON *rtspdec, cJSON *h323dec,cJSON *sipdec, int &GopCallFlag);
	bool ConvertSTRING2CJSONGetWebNewWhine(CData& oResult, char* cResult, string Method, cJSON* result, cJSON* _addressList,cJSON* node[], int ParamNumber);
	bool ConvertSTRING2CJSONGetWebParam(CData& oResult, char* cResult, cJSON* result, cJSON* ParamList,cJSON* node[], string Method, string &ip, int ParamNum);
	bool ConvertSTRING2CJSONGetParam(CData& oResult, char* cResult, cJSON* result, cJSON* ParamList,cJSON* node[], string Method, string *StrParam, int ParamNum);

private:

    /**
     * @brief ConvertParaProcessing 参数处理函数
     * @param cParaBuf
     * @param iLen
     */
    bool ConvertParaProcessing(char *cParaBuf, int iLen);

    /**
     * @brief StringSplit 分割字符串到数组中
     * @param string
     * @param split
     */
    StringTab StringSplit(char* string,const char* split);

    /**
     * @brief AnalyParaMessage 处理单条指令
     * @param string
     */
    int AnalyParaMessage(char *string);

    /**
     * @brief AnalyParaCmdSplitMessage 分割指令内容
     * @param string
     * @param top
     * @param low
     */
    void AnalyParaCmdSplitMessage(char *string, char **top, char **low);

    /**
     * @brief AnalyParaCmdSplitMessage 指令解析
     * @param top
     * @param low
     * @param string
     */
    int AnalyParaCmdExtracMessage(char *top, char *low, char *string);

    /**
     * @brief AnalyParaCmdCommandPartSplit 按'.'分割指令内容部分 
     * @param top
     * @param str
     */
    void AnalyParaCmdCommandPartSplit(char *top, char **str);

    /**
     * @brief AnalyParaCmdArgumentsPartSplit 根据需求。按‘，’分割参数内容部分
     * @param low
     * @param arg
     */
    void AnalyParaCmdArgumentsPartSplit(char *low, char **arg);

    /**
     * @brief AnalyParaCmdParseCommandHandle 网管协议指令处理
     * @param top
     * @param low
     * @param string
     */
    int AnalyParaCmdParseCommandHandle(char **str, char **arg, char *string);

    /**
     * @brief IncBuffer 字符串比较查找函数
     * @param pp
     */
    void IncBuffer(char **pp);

    /**
     * @brief SubBuffer 字符串比较查找函数
     * @param pp
     */
    void SubBuffer(unsigned char **pp);

    /**
     * @brief StringCmp 字符串比较查找函数
     * @param str1
     * @param str2
     */
    char* StringCmp( char *str1, const char *str2);

    /**
     * @brief IpAvalbCheck IP相关地址校验核对
     * @param ascii_buf
     */
    int IpAvalbCheck(char *ascii_buf);

    /**
     * @brief AnalyNetPara 解析网络接收参数
     * @param NetParaStr
     * @param NetParaArg
     */
    int AnalyNetRecvPara(int Id, char *NetParaStr, char *NetParaArg);

    /**
     * @brief AnalyNetH323Para 解析H323接收参数
     * @param NetParaStr
     * @param NetParaArg
     */
    int AnalyNetH323Para(int Id, char *NetParaStr, char **NetParaArg);

    /**
     * @brief AnalyNetChnSendPara 解析ChnSend接收参数
     * @param NetParaStr
     * @param NetParaArg
     */
    int AnalyNetChnSendPara(int Id, char *NetParaStr, char *NetParaArg);

    /**
     * @brief AnalyGetRecPara 解析ChnSend接收参数
     * @param cParaBuf
     * @param iLen
     */
    int AnalyGetRecPara(char *cParaBuf, int iLen);

    /**
     * @brief AnalyGetRecPara 解析ChnSend接收参数
     * @param cParaBuf
     * @param iLen
     */
    int AnalyGetRecParaSearch(char *cParaBuf, char *cDealBuf, char *cStartXml, char *cEndXml, int *iOffsetLen);

    //add by libx in 2022/02/23
    //--[BEGIN]--
    /**
     * @brief AnalyGetWebAudioConf 解析接收参数
     * @param pBuf
     * @param nLen
     */
    bool AnalyGetWebAudioConf(char* pBuf, int nLen);
    /**
     * @brief AnalyGetWebAudioGroupConf 解析接收参数
     * @param pBuf
     * @param nLen
     */
    bool AnalyGetWebAudioGroupConf(char* pBuf, int nLen);
    /**
     * @brief AnalyGetWebAudioMatrixConf 解析接收参数
     * @param pBuf
     * @param nLen
     */
    bool AnalyGetWebAudioMatrixConf(char* pBuf, int nLen);
    /**
     * @brief AnalyGetWebInfraredStudyList 解析接收参数
     * @param pBuf
     * @param nLen
     */
    bool AnalyGetWebInfraredStudyList(char* pBuf, int nLen);
    /**
     * @brief AnalyGetWebVideoParam 解析接收参数
     * @param pBuf
     * @param nLen
     */
    bool AnalyGetWebVideoParam(char* pBuf, int nLen);
    /**
     * @brief AnalyGetWebAudioParam 解析接收参数
     * @param pBuf
     * @param nLen
     */
    bool AnalyGetWebAudioParam(char* pBuf, int nLen);
    /**
     * @brief AnalyGetWebRtspParam 解析接收参数
     * @param pBuf
     * @param nLen
     */
    bool AnalyGetWebRtspParam(char* pBuf, int nLen);
    /**
     * @brief AnalyGetWebRtmpParam 解析接收参数
     * @param pBuf
     * @param nLen
     */
    bool AnalyGetWebRtmpParam(char* pBuf, int nLen);
    /**
     * @brief AnalyGetWebLogFile 解析接收参数
     * @param pBuf
     * @param nLen
     */
    bool AnalyGetWebLogFile(char* pBuf, int nLen);

    //--[END]--
private:
    cJSON *_network;
    cJSON *_sysinfo;
    cJSON *_diskinfo;
    cJSON *_decnum;

    cJSON *_rtspdec;
    cJSON *_h323dec;
	cJSON *_sipdec;

    cJSON *_status;
    cJSON *_channal;
    cJSON *_statusinfo;

    cJSON *cArray;
	cJSON *h323cArray;
	cJSON *sipcArray;

    cJSON *_cRecStatus;

    cJSON *_videofiles;
    cJSON *_recresult;
    cJSON *_otherinfo;

    //add by libx in 2022/02/23
    //--[BEGIN]--
    cJSON* _result;
    cJSON* _taskList;
	cJSON* _infraStudyListRes;
	cJSON* _videoParamList;
    cJSON* _audioConfListRes;
    cJSON* _audioGroupConfListRes;
    cJSON* _audioGroupList;
    cJSON* _audioMatrixListRes;
    cJSON* _audioMatrixList;
    cJSON* _videoParamResult;
    cJSON* _audioParamList;
    cJSON* _audioParamResult;
    cJSON* _rtmpParamList;
    cJSON* _rtspParamResult;
    cJSON* _rtmpParamResult;
    cJSON* _logList;
    cJSON* _logResult;
    //--[END]--
	cJSON* _rtspParamList;
	cJSON* _ParamList;
	cJSON* _ParamKnod;
	cJSON **_ParamArry;
    string oldpassword; 
	string UserName;
	string CMethod;
    string _ip;//ip地址
    string tmpParam;
	string Params[100];
	string ArryParam[100][100];
    string *StrParam;
	
    int _port;//端口号
    int _Param;
	int FileStartIndex;
	int FileEndIndex;



};

#endif // CANALYZEPARAPROC_H
