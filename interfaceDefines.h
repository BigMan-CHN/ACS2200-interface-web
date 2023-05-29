/********************************************************
* interfaceDefines.h
* 定义接口信息
*
* Copyright (c) 2019, Tyvis
* All rights reserved.
*
* @version		V1.00
* @author		es
* @date			2019-5-17
 *******************************************************/
#ifndef INTERFACEDEFINES_H
#define INTERFACEDEFINES_H

/********************************************************
 * headers
 *******************************************************/
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
#include <boost/format.hpp>

/********************************************************
 * lib headers
 *******************************************************/
#include <log.h>
#include <TransmitClient.h>
#include <xmlParser.h>
#include <LibXML.h>
#include <Server.h>

#include "MPSClientDefine.h"

using namespace std;
using namespace SVS;

//caseid max len
#define REQUEST_PARAMS_NODE_CASEID_LEN  100
/********************************************************
 * request json
 *******************************************************/
//test node
#define REQUEST_TEST_NODE_DEBUG "TEST_DEBUG" /*测试节点*/

// request node name
#define REQUEST_NODE_METHOD     "method"
#define REQUEST_NODE_PARAMS     "params"
#define REQUEST_NODE_CHANNELNUM "channelNum"
#define REQUEST_NODE_RTSPDECNUM "DECID"
#define REQUEST_NODE_H323DECNUM "H323DecNum"
#define REQUEST_NODE_CHNNUM     "ChannelNum"
#define REQUEST_NODE_RTMPNUM    "RtmpCliPushNum"


// request params node name
// method hostInit
#define REQUEST_PARAMS_NODE_CASEID     "caseid"
/********************************************************
 * response json
 *******************************************************/
// resposne node name
#define RESPONSE_NODE_ERROR      "error" /* string # 错误描述 */
#define RESPONSE_NODE_CODE       "code" /* int # 0：成功，!0：失败. */
#define RESPONSE_NODE_FLAG       "flag" /* string # 1：成功，0：失败. */
#define RESPONSE_NODE_RESULTS    "results" /* json # 返回信息 */

// resposne node flag
#define RESPONSE_NODE_FLAG_SUCCESS       "1" /* 成功 */
#define RESPONSE_NODE_FLAG_FAILED        "0" /* 失败 */


/********************************************************
 * method
 *******************************************************/
#define METHOD_HOST_INIT                    "hostInit"
#define METHOD_HOST_START                   "hostStart"
#define METHOD_HOST_PAUSE                   "hostPause"
#define METHOD_HOST_RESUME                  "hostResume"
#define METHOD_HOST_STOP                    "hostStop"
#define METHOD_HOST_CLOSE                   "hostClose"
#define METHOD_HOST_STATUS                  "hostStatus"
#define METHOD_HOST_REBOOT                  "reboot"
#define METHOD_HOST_SHUTDOWN                "shutdown"
#define METHOD_HOST_SETPOINT                "setpoint"
#define METHOD_GET_CHANNEL_CONTENTS         "getChannelContents"
#define METHOD_GET_TASK_CONTENTS            "getTaskContents"
#define METHOD_SET_TASK_DELETE              "setTaskDelete"
#define METHOD_GET_SYS_CONFIG               "getSysConfig"
#define METHOD_GET_HOST_CONFIG              "GetHostConfig"
#define METHOD_GET_HOST_DECINFO             "GetHostDecInfo"
#define METHOD_GET_HOST_STATUS              "hostDecStatus"
#define METHOD_GET_HOST_CHNINFO             "GetHostChnInfo"

#define METHOD_SET_START_OSD                "SetOSD"
#define METHOD_SET_STOP_OSD                 "DelOSD"

#define METHOD_SET_PUTNOTEFILE              "PutNoteFile"

//20210603 fwb add decoder
#define METHOD_SET_HOST_START_DEC           "hostStartDec"
#define METHOD_SET_HOST_STOP_DEC            "hostStopDec"

#define METHOD_SET_HOST_ADDPIC_DEC           "hostSetPicture"
#define METHOD_SET_HOST_CLRPIC_DEC           "hostClearPicture"

#define METHOD_SEND_HTTP_CONTENT             "sendSerialPortRetransHttpContent"

#define METHOD_SET_START_DEC                "SetStartDec"
#define METHOD_SET_STOP_DEC                 "SetStopDec"
#define METHOD_SET_START_REC                "StartRecord"
#define METHOD_SET_STOP_REC                 "StopRecord"
#define METHOD_SET_TASKDEL_REC              "SetTaskDelete"
#define METHOD_SET_START_DISCIPLINE         "SetStartDiscipline"
#define METHOD_SET_STOP_DISCIPLINE          "SetStopDiscipline"
#define METHOD_SET_START_DISCIPLINE         "SetStartDiscipline"
#define METHOD_GET_STATUS_DISCIPLINE        "GetDiscipLineStatus"

#define METHOD_GET_INFOLIST_REC             "GetRecInfo"

#define METHOD_SET_RTMPPUSH_STREAM          "SetRtmpCliPush"
#define METHOD_SET_OPEN_STREAM              "SetOpenStream"
#define METHOD_SET_CLOSE_STREAM             "SetCloseStream"

#define METHOD_SET_CONTROL_POWER            "SetSysCmd"
#define METHOD_SEND_CONSOLE_CMD             "SetConsoleCmd"
#define METHOD_SEND_SERIAL_CMD              "SetSerialCmd"

#define METHOD_HOST_DEC_STATUS              "hostDecStatus"

#define METHOD_HOST_START_DISCIPLINE        "hostStartDiscipline"
#define METHOD_HOST_STOP_DISCIPLINE         "hostStopDiscipline"
#define METHOD_HOST_GET_DISCIPLINE_STATUS   "hostDiscipLineStatus"

//add Webinterface
#define METHOD_GET_WEB_HOST_CONFIG          "GetWebHostConfig"
#define METHOD_GET_WEB_HOST_CHNINFO         "GetWebHostChnInfo"
#define METHOD_SET_WEB_REC                  "SetWebRecord"
#define METHOD_SET_WEB_INFOLIST_REC         "GetWebRecInfo"
#define METHOD_SET_WEB_SEGMODEL_REC         "SetWebRecSegModel"
#define METHOD_SET_WEB_SEGMODELSELECT_REC   "SetWebRecSegModelSelect"
#define METHOD_SET_WEB_MATRIX_REC           "SetWebRecMatrix"

#define METHOD_SET_WEB_STARTDEC_MULTI       "SetWebStartDec"
#define METHOD_SET_WEB_STOPDEC_MULTI        "SetWebStopDec"
#define METHOD_SET_WEB_RTSPCONTACT_MULTI    "SetWebRtspContact"
#define METHOD_SET_WEB_H323CONTACT_MULTI    "SetWebH323Contact"
#define METHOD_SET_WEB_SIPCONTACT_MULTI     "SetWebSipContact"
#define METHOD_SET_WEB_ADDRLIST_MULTI       "SetWebAddrList"

#define METHOD_SET_WEB_MICINPUT_AUDIO        "SetWebMICInputVol"
#define METHOD_SET_WEB_LINEINPUT_AUDIO       "SetWebLineInputVol"
#define METHOD_SET_WEB_DECINPUT_AUDIO        "SetWebDecInputVol"
#define METHOD_SET_WEB_BALANCEOUTPUT_AUDIO   "SetWebBalanceOutputVo"
#define METHOD_SET_WEB_LINEOUTPUT_AUDIO      "SetWebLineOutputVol"
#define METHOD_SET_WEB_DECOUTPUT_AUDIO       "SetWebDecOutputVol"

#define METHOD_SET_WEB_NEWCAPTION_EFFECT     "SetWebNewCaption"
#define METHOD_SET_WEB_CAPTION_EFFECT        "SetWebCaption"

#define METHOD_SET_WEB_NETCFG_EXTEND          "SetWebNetConf"
#define METHOD_SET_WEB_SYNCTIME_EXTEND        "SetWebSyncTime"
#define METHOD_SET_WEB_DEVID_EXTEND           "SetWebDevID"
#define METHOD_SET_WEB_SYSUPDATE_EXTEND       "SetWebSysUpdate"
#define METHOD_SET_WEB_CFGFILE_EXTEND         "SetWebCfgFile"
#define METHOD_SET_WEB_LOGFILE_EXTEND         "SetWebLogFile"

//add new method 
//by libx in 2022/02/23
//--[BEGIN]--
/* 扩展功能界面 - 红外管理接口 */
#define METHOD_GET_WEB_INFRARED_STUDYLIST_EXTEND        "GetWebInfraredStudyList"
#define METHOD_SET_WEB_INFRARED_STUDY_EXTEND            "SetWebInfraredStudy"
#define METHOD_SET_WEB_INFRARED_DEL_EXTEND              "SetWebInfraredDel"
#define METHOD_SET_WEB_INFRARED_SEND_EXTEND             "SetWebInfraredSend"
#define METHOD_SET_WEB_INFRARED_CREATE_EXTEND           "SetWebInfraredCreate"

/* 扩展功能界面 - 音频设置 */
#define METHOD_GET_WEB_AUDIO_CONF_EXTEND                "GetWebAudioConf"
#define METHOD_SET_WEB_AUDIO_CONF_EXTEND                "SetWebAudioConf"
#define METHOD_GET_WEB_AUDIO_GROUP_CONF_EXTEND          "GetWebAudioGroupConf"
#define METHOD_SET_WEB_AUDIO_GROUP_CONF_EXTEND          "SetWebAudioGroupConf"
#define METHOD_GET_WEB_AUDIO_MATRIX_CONF_EXTEND         "GetWebAudioMatrixConf"
#define METHOD_SET_WEB_AUDIO_MATRIX_CONF_EXTEND         "SetWebAudioMatrixConf"

/* 扩展功能界面 - 编码流媒体接口 */
#define METHOD_GET_WEB_VIDEO_PARAM_EXTEND               "GetWebVideoParam"
#define METHOD_SET_WEB_VIDEO_PARAM_EXTEND               "SetWebVideoParam"
#define METHOD_GET_WEB_AUDIO_PARAM_EXTEND               "GetWebAudioParam"
#define METHOD_SET_WEB_AUDIO_PARAM_EXTEND               "SetWebAudioParam"
#define METHOD_GET_WEB_RTSP_PARAM_EXTEND                "GetWebRtspParam"
#define METHOD_SET_WEB_RTSP_PARAM_EXTEND                "SetWebRtspParam"
#define METHOD_GET_WEB_RTMP_PARAM_EXTEND                "GetWebRtmpParam"
#define METHOD_SET_WEB_RTMP_PARAM_EXTEND                "SetWebRtmpParam"

/* 扩展功能界面 - 日志管理接口 */
#define METHOD_GET_WEB_LOG_FILE_EXTEND                  "GetWebLogFile"
#define METHOD_SET_WEB_LOG_FILE_CONF_EXTEND             "SetWebLogFileConf"

/* 扩展功能界面 - 高级设置接口 */
#define METHOD_SET_WEB_DEV_ID_EXTEND                    "SetWebDevID"
#define METHOD_SET_WEB_RTMP_BROAD_CONF_EXTEND           "SetWebRtmpBroadConf"
#define METHOD_SET_WEB_FIRM_UPDATE_EXTEND               "SetWebFirmUpdate"
#define METHOD_SET_WEB_ZKT_UPDATE_EXTEND                "SetWebZKTUpdate"
#define METHOD_SET_WEB_ZKT_DOWNLOAD_EXTEND              "SetWebZKTDownload"
#define METHOD_SET_WEB_CONFIG_UPDATE_EXTEND             "SetWebConfigUpdate"
#define METHOD_SET_WEB_DISCIPFILE_UPDATE_EXTEND         "SetWebDiscipFileUpdate"
#define METHOD_SET_WEB_BACKUP_CFGFILE_EXTEND            "SetWebBackupCfgFile"
#define METHOD_SET_WEB_RECOVER_CFGFILE_EXTEND           "SetWebRecoverCFGFile"

/* 多方互动界面 - 联系人接口*/
#define METHOD_ADD_WEB_PARTY                            "AddWebParty"
#define METHOD_DEL_WEB_PARTY                            "DelWebParty"
#define METHOD_GET_PARTY_LIST                           "GetWebAddrList"
//--[END]--

//add by ltz in 2022/02/24
//--[BEGIN]--
#define METHOD_LOGIN_INTERFACE                          "LoginInterface"
#define METHOD_SET_WEB_SIPADDRLIST_MULTI                "SetWebSipAddrList"
#define METHOD_SET_WEB_MOSAIC                           "SetWebMosaic"
#define METHOD_SET_WEB_NEW_WHINE                        "SetWebNewWhine"
#define METHOD_GET_WEB_NEW_WHINE                        "GetWebNewWhine"
#define METHOD_SET_WEB_WHINE                            "SetWebWhine"
#define METHOD_SET_WEB_SYNC_TIME                        "SetWebSyncTime"
#define METHOD_SET_USER_PASSWORD                        "SetWebUserPassword"
#define METHOD_SET_FTP_CONF                             "SetWebFtpConf"
#define METHOD_SET_WEB_UART_CONF                        "SetWebUartConf"
#define METHOD_SET_WEB_VIDEO_OUT_PUT_CONF               "SetWebVideoOutPutConf"
#define METHOD_SET_WEB_VIDEO_CHN_CONF                   "SetWebVideoChnConf"
#define METHOD_GET_WEB_VIDEO_CHN_LIST                   "GetWebVideoChnList"
#define METHOD_SET_WEB_STORAGE_CONF                     "SetWebStorageConf"
//#define METHOD_GET_WEB_ADDR_LIST                        "GetWebAddrList"
#define METHOD_GET_WEB_CAPTION_LIST                     "GetWebCaptionList"
#define METHOD_GET_SERIAL_CMD                           "GetSerialCmd"
#define METHOD_GET_WEB_MOSAIC_LIST                      "GetWebMosaicList"
#define METHOD_GET_WEB_WHINE_LIST                       "GetWebWhineList"
#define METHOD_SET_WEB_MIC_GAIN                         "SetWebMICGain"
#define METHOD_GET_WEB_MIC_GAIN                         "GetWebMICGain"
//--[END]--
//add by ltz in 2022/03/02
#define METHOD_GET_WEB_USER_PASSWORD                    "GetWebUserPassword"
#define METHOD_GET_WEB_FTP_CONF                         "GetWebFtpConf"
#define METHOD_GET_WEB_UART_CONF                        "GetWebUartConf"
#define METHOD_GET_WEB_STORAGE_CONF                     "GetWebStorageConf"
#define METHOD_GET_WEB_DEVID                            "GetWebDevID"
#define METHOD_GET_WEB_VIDEO_OUTPUT_CONF                "GetWebVideoOutPutConf"
#define METHOD_GET_WEB_INFRARED_FILE_INF                "GetWebInfraredFileInf"
#define METHOD_SET_WEB_INFRARED_FILE_DEL                "SetWebInfraredFileDel"
#define METHOD_SET_WEB_MIC_WHINE_BUTTON                 "SetWebMICWhineButton"
#define METHOD_GET_WEB_MIC_WHINE_BUTTON                 "GetWebMICWhineButton"
#define METHOD_SET_WEB_SYSTERM_SHUTDOWN                 "SetWebSystermShoutDown"
#define METHOD_GET_WEB_REC_SEGMODEL                     "GetWebRecSegModel"
#define METHOD_SET_WEB_VIDEO_MATRIX                     "SetWebVideoMatrix"
#define METHOD_GET_WEB_VIDEO_MATRIX                     "GetWebVideoMatrix"
#define METHOD_GET_WEB_AUDIO_DEFAULT_GAIN               "GetAudioDefultGain"
#define METHOD_SET_WEB_DSP_MATRIX                       "SetWebDSPMatrix"
#define METHOD_GET_WEB_DSP_MATRIX                       "GetWebDSPMatrix"
#define METHOD_SET_WEB_START_REC                        "SetWebStartRec"
#define METHOD_GET_WEB_START_REC                        "GetWebStartRec"
#define METHOD_SET_WEB_START_AUTO_REC                   "SetWebStartAutoRec"
#define METHOD_GET_WEB_START_AUTO_REC                   "GetWebStartAutoRec"
#define METHOD_SET_WEB_NEW_WHINE_BUTTON                 "SetWebNewWhineButton"
#define METHOD_GET_WEB_NEW_WHINE_BUTTON                 "GetWebNewWhineButton"
#define METHOD_SET_WEB_AUDIO_MUTE                       "SetWebAudioMute"
#define METHOD_GET_WEB_AUDIO_MUTE                       "GetWebAudioMute"


//--[END]--



/********************************************************
 * hostInit method & params & response
 *******************************************************/
#define HOSTNIT_PARAMS_CHANNELNUM           REQUEST_NODE_CHANNELNUM /*params*/
#define HOSTNIT_PARAMS_CASEID               "caseid" /*params*/

/********************************************************
 * hostStop method & params & response
 *******************************************************/
#define HOSTSTOP_PARAMS_CHANNELNUM           REQUEST_NODE_CHANNELNUM /*params*/
#define HOSTSTOP_PARAMS_TURBO                "turboMode" /*params*/

/********************************************************
 * setpoint method & params & response
 *******************************************************/
#define SETPOINT_PARAMS_CHANNELNUM       REQUEST_NODE_CHANNELNUM /*params*/
#define SETPOINT_RES_DURATION            "duration" /*response*/
#define SETPOINT_RES_FILEPATH            "filePath" /*response*/

/********************************************************
 * hostStatus method & params & response
 *******************************************************/
#define HOST_STATUS_PARAMS_CHANNELNUM       REQUEST_NODE_CHANNELNUM /*params*/
#define HOST_STATUS_RES_CH_STATUS           "channelStatus" /*response*/
#define HOST_STATUS_RES_CH_CASEID           "caseid" /*response*/
#define HOST_STATUS_RES_DISK_USED_PCT       "diskUsedPct" /*response*/
#define HOST_STATUS_RES_DISK_ALARM          "diskAlarm" /*response*/

/********************************************************
 * getChannelContents method & params & response
 *******************************************************/
#define GET_CHANNEL_CONTENTS_PARAMS_START_DATE          "startDate" /*params 开始时间*/
#define GET_CHANNEL_CONTENTS_PARAMS_END_DATE            "endDate" /*params 结束时间*/
#define GET_CHANNEL_CONTENTS_RES_CASETOTAL              "caseTotal" /*response 文件个数*/
#define GET_CHANNEL_CONTENTS_RES_CASELIST               "caseList" /*response 存放案件数组，包含以下节点videoFiles、caseid*/
#define GET_CHANNEL_CONTENTS_RES_CASEID                 "caseid" /*response 唯一的任务标识，案件ID*/
#define GET_CHANNEL_CONTENTS_RES_TITLE                  "title" /*response */
#define GET_CHANNEL_CONTENTS_RES_STATUS                 "status" /*response 工作状态。0代表未开始；1代表正在工作；2代表暂停；3代表结束；4代表关闭；5代表异常*/
#define GET_CHANNEL_CONTENTS_RES_ALLOWDELETE            "allowDelete" /*response 是否允许删除；0：不允许，1：允许*/
#define GET_CHANNEL_CONTENTS_RES_MONTH                  "month" /*response 录制年月*/
#define GET_CHANNEL_CONTENTS_RES_STARTTIME              "startTime" /*response 录制开始时间*/
#define GET_CHANNEL_CONTENTS_RES_ENDTIME                "endTime" /*response 录制结束时间*/
#define GET_CHANNEL_CONTENTS_RES_VIDEOFILE              "videoFiles" /*response 存放同一个案件下的文件数组，包含以下节点：caseid、filename、size、filePath、startTime、endTime、channelnum、channelDesc、ftpPath、vodurlrtsp、vodurlrtmp、filesort*/

/********************************************************
 * getTeskContents method & params & response
 *******************************************************/
#define GET_TESK_CONTENTS_PARAMS_CASEID       "caseid" /*params*/
#define GET_TESK_CONTENTS_RES_ALLOWDELETE     "allowDelete" /*response results- 是否允许删除 bool*/
#define GET_TESK_CONTENTS_RES_TOTAL           "total" /*response results- 获取的文件数量 int*/
#define GET_TESK_CONTENTS_RES_VIDEO_FILES     "videoFiles" /*response results- 获取的文件列表 table*/
#define GET_TESK_CONTENTS_RES_VF_CASEID       "caseid" /*response results-videoFiles- 文件对应的caseid string*/
#define GET_TESK_CONTENTS_RES_VF_FILENAME     "fileName" /*response results-videoFiles- 文件名称 string*/
#define GET_TESK_CONTENTS_RES_VF_SIZE         "size" /*response results-videoFiles- 文件大小 int */
#define GET_TESK_CONTENTS_RES_VF_FILEPATH     "filePath" /*response results-videoFiles- 文件在主机上的路径 string*/
#define GET_TESK_CONTENTS_RES_VF_STARTTIME    "startTime" /*response results-videoFiles- 文件开始时间(2016-07-05 15:07:01) string*/
#define GET_TESK_CONTENTS_RES_VF_ENDTIME      "endTime" /*response results-videoFiles- 文件结束时间(2016-07-05 15:09:01) string*/
#define GET_TESK_CONTENTS_RES_VF_CH_NUM       "channelNum" /*response results-videoFiles- 录制文件的通道号 int*/
#define GET_TESK_CONTENTS_RES_VF_CH_DESC      "channelDesc" /*response results-videoFiles- 录制文件的通道描述符 string*/
#define GET_TESK_CONTENTS_RES_VF_FTPPATH      "ftpPath" /*response results-videoFiles- 录制文件的ftp路径 string*/
#define GET_TESK_CONTENTS_RES_VF_VODURL_RTSP   "vodurlRtsp" /*response results-videoFiles-   string*/
#define GET_TESK_CONTENTS_RES_VF_VODURL_RTMP  "vodurlRtmp" /*response results-videoFiles-   string*/
#define GET_TESK_CONTENTS_RES_VF_FILESORT     "fileSort" /*response results-videoFiles- 文件序列号  string*/


/********************************************************
 * setOSD method & params & response
 *******************************************************/
#define SET_CONFIG_ADD_OSD_OSDMODE                "OsdMode" /*params*/
#define SET_CONFIG_ADD_OSD_CHANNELID              "ChannelID" /*params*/
#define SET_CONFIG_ADD_OSD_TITLE                  "Title" /*params*/
#define SET_CONFIG_DEL_OSD_OSDMODE                "OsdMode" /*params*/
#define SET_CONFIG_DEL_OSD_CHANNELID              "ChannelID" /*params*/

/********************************************************
 * PutNoteFile method & params & response
 *******************************************************/
#define SET_CONFIG_PUTNOTEFILE_TRIALID                "TrialId" /*params*/
#define SET_CONFIG_PUTNOTEFILE_CONTENT                "Content" /*params*/
#define SET_CONFIG_PUTNOTEFILE_FILENAME               "FileName" /*params*/
#define SET_CONFIG_PUTNOTEFILE_FILESIZE               "FileSIZE" /*params*/

/********************************************************
 * getHostConfig method & params & response
 *******************************************************/
#define GET_HOST_CONFIG_RES_SYSINFO              "SysInfo" /*response */
#define GET_HOST_CONFIG_RES_SYS_DEVTYPE          "Devtype" /*response */
#define GET_HOST_CONFIG_RES_SYS_HVER             "HVer" /*response */
#define GET_HOST_CONFIG_RES_SYS_SVER             "SVer" /*response */
#define GET_HOST_CONFIG_RES_SYS_SN               "SN" /*response */
#define GET_HOST_CONFIG_RES_SYS_FTPPORT          "FtpPort" /*response */
#define GET_HOST_CONFIG_RES_SYS_ALLRECSTATUS     "AllRecStatus" /*response */
#define GET_HOST_CONFIG_RES_SYS_CHANNELNUM       "ChannelNum" /*response */

#define GET_HOST_CONFIG_RES_NETWORK             "NetInfo" /*response */
#define GET_HOST_CONFIG_RES_NW_IP               "IP" /*response */
#define GET_HOST_CONFIG_RES_NW_GATEWAY          "Gateway" /*response */
#define GET_HOST_CONFIG_RES_NW_NETMASK          "Netmask" /*response */
#define GET_HOST_CONFIG_RES_NW_MACADDR          "MacAddr" /*response */

#define GET_HOST_CONFIG_RES_OTHERINFO             "OtherInfo" /*response */

#define GET_HOST_CONFIG_RES_DISKINFO            "DiskInfo" /*response */
#define GET_HOST_CONFIG_RES_DI_TOTALSIZE        "TOTALSIZE" /*response */
#define GET_HOST_CONFIG_RES_DI_FREESIZE         "FreeSize" /*response */
#define GET_HOST_CONFIG_RES_DI_CPURATE          "CpuRate" /*response */
#define GET_HOST_CONFIG_RES_DI_MEMRATE          "MemRate" /*response */
#define GET_HOST_CONFIG_RES_DI_DiskRATE         "DiskRate" /*response */
#define GET_HOST_CONFIG_RES_DI_NETSTATUS        "NetStatus" /*response */
#define GET_HOST_CONFIG_RES_DI_AUTODELSIZE      "AutoDelSize" /*response */

#define GET_HOST_CONFIG_RES_DECNUM             "NumInfo" /*response */
#define GET_HOST_CONFIG_RES_DE_RTSPDECNUM      "RTSPDecNum" /*response */
#define GET_HOST_CONFIG_RES_DE_DECID           "DECID"/*response */
#define GET_HOST_CONFIG_RES_DE_DECSTA          "DecStatus"/*response */
#define GET_HOST_CONFIG_RES_DE_DECURL          "DecUrl"/*response */
#define GET_HOST_CONFIG_RES_DE_DECMODE         "DecMode"/*response */
#define GET_HOST_CONFIG_RES_DE_SIPDECNUM      "SipDecNum" /*response */
#define GET_HOST_CONFIG_RES_DE_SIPID          "SipID"/*response */
#define GET_HOST_CONFIG_RES_DE_SIPSTA         "SipStatus"/*response */
#define GET_HOST_CONFIG_RES_DE_SIPCALLIP      "SipCallIp"/*response */

//add web interface
#define GET_HOST_CONFIG_RES_WEB_CHN             "Channel" /*response */
#define GET_HOST_CONFIG_RES_PARAMS              "params" /*response */
#define GET_HOST_CONFIG_RES_RETCODE             "retCode" /*response */
#define GET_HOST_CONFIG_RES_RETMES              "retMessage" /*response */
#define GET_HOST_CONFIG_RES_CONTENT              "content" /*response */

#define GET_HOST_CONFIG_RES_CHN                 "Channel" /*response */
#define GET_HOST_CONFIG_RES_CH_CHANNELNUM       "ChannelNum"/*response */
#define GET_HOST_CONFIG_RES_CH_RTMPCLIPUSHNUM   "RtmpCliPushNum"/*response */
#define GET_HOST_CONFIG_RES_CH_NCHNID           "ChannelID"/*response */
#define GET_HOST_CONFIG_RES_CH_NCHNNAME         "ChannelName"/*response */
#define GET_HOST_CONFIG_RES_CH_NCHNSTA          "ChannelStatus"/*response */
#define GET_HOST_CONFIG_RES_CH_NCHNURL          "Url"/*response */
#define GET_HOST_CONFIG_RES_CH_NCHNRECSTA       "RecStatus"/*response */
#define GET_HOST_CONFIG_RES_CH_NCHNRECTASKID    "RecTaskID"/*response */

#define GET_HOST_CONFIG_RES_DE_H323DECNUM      "H323DecNum" /*response */
#define GET_HOST_CONFIG_RES_DE_SIPNUM          "SipNum" /*response */
#define GET_HOST_CONFIG_RES_DE_H323ID          "H323ID"/*response */
#define GET_HOST_CONFIG_RES_DE_H323STA         "H323Status"/*response */
#define GET_HOST_CONFIG_RES_DE_H323CALLIP      "H323CallIp"/*response */

#define GET_HOST_CONFIG_DISCIPLINE_STATUSINFO    "StatusInfo" /*response */
#define GET_HOST_CONFIG_DS_DISCIPLINESTATUS      "DisciplineStatus" /*response */

#define GET_HOST_CONFIG_RES_CHANNELS            "channels" /*response */
#define GET_HOST_CONFIG_RES_CH_CHANNELNUM       "channelNum" /*response */
#define GET_HOST_CONFIG_RES_CH_CHANNELNAME      "channelName" /*response */
#define GET_HOST_CONFIG_RES_CH_CHANNELSTATUS    "channelStatus" /*response */
#define GET_HOST_CONFIG_RES_CH_CHANNELENABLE    "channelEnable" /*response */
#define GET_HOST_CONFIG_RES_CH_RTSPURL          "rtspUrl" /*response */
#define GET_HOST_CONFIG_RES_CH_RTSPENABLE       "rtspEnable" /*response */
#define GET_HOST_CONFIG_RES_CH_RTMPURL          "rtmpUrl" /*response */
#define GET_HOST_CONFIG_RES_CH_RTMPENABLE       "rtmpEnable" /*response */

#define GET_HOST_CONFIG_RES_CH_SERIALNUM           "SerialNum" /*response */

#define GET_REC_CONFIG_START_TASKID             "TaskID" /*response */
#define GET_REC_CONFIG_START_STATUS              "Status" /*response */

/********************************************************
 * SetSerialCmd method & params & response
 *******************************************************/
#define SEND_SERILAL_PARAMS_CMD                  "Cmd" /*params*/
#define SEND_SERILAL_PARAMS_SERIALID             "SerialID" /*params*/

/********************************************************
 * sendConsoleCmd method & params & response
 *******************************************************/
#define SEND_CONSOLE_PARAMS_CMD             "Cmd" /*params*/

/********************************************************
 * hostStartDec method & params & response
 *******************************************************/
#define HOST_START_DEC_PARAMS_CHANNEL           "channel" /*params*/
#define HOST_START_DEC_PARAMS_URL               "url" /*params*/
#define HOST_START_DEC_PARAMS_CACHE_TIME        "cacheTime" /*params*/
#define HOST_START_DEC_PARAMS_TCP_MODE          "tcpMode" /*params*/
#define HOST_START_DEC_PARAMS_IMGPATH           "imgPath" /*params*/

/********************************************************
 * SetStartDec method & params & response
 *******************************************************/
#define SET_START_DEC_PARAMS_DECTYPE           "DecType" /*params*/
#define SET_START_DEC_PARAMS_CHANNEL           "DECID" /*params*/
#define SET_START_DEC_PARAMS_ADDR              "DecUrl" /*params*/
#define SET_START_DEC_PARAMS_CACHETIME         "CacheTime" /*params*/
#define SET_START_DEC_PARAMS_TCPMODE           "TcpMode" /*params*/

/********************************************************
 * SetStartDiscipline method & params & response
 *******************************************************/
#define SET_START_DISCIPLINE_PARAMS_LOOPFLAG           "LoopFlag" /*params*/
#define SET_START_DISCIPLINE_PARAMS_PLAYLIST           "PlayList" /*params*/

/********************************************************
 * StartRecord method & params & response
 *******************************************************/
#define SET_START_REC_PARAMS_RECMODE           "RecMode" /*params*/
#define SET_START_REC_PARAMS_CHNAME            "ChannelID" /*params*/
#define SET_START_REC_PARAMS_TASKID            "TaskID" /*params*/
#define SET_START_REC_PARAMS_DESRIPTOR         "Title" /*params*/

//add web interface params
#define SET_WEB_REC_PARAMS_CHANNELID           "ChannelID" /*params*/
#define SET_WEB_REC_PARAMS_SWITCHONOFF         "SwitchOnOff" /*params*/
#define SET_WEB_REC_PARAMS_SEGMODEL            "SegModel" /*params*/

#define SET_WEB_REC_PARAMS_WINDOWID             "WindowID"
#define SET_WEB_REC_PARAMS_VIDEOSOURCEID        "VideoSourceID"

#define SET_WEB_MULTI_PARAMS_DECTYPE            "DecType"
#define SET_WEB_MULTI_PARAMS_DECID              "DecID"
#define SET_WEB_MULTI_PARAMS_DECFORMAT          "DecFormat"
#define SET_WEB_MULTI_PARAMS_DECMODE            "DecMode"
#define SET_WEB_MULTI_PARAMS_DECMOTHOD          "DecMethod"
#define SET_WEB_MULTI_PARAMS_USERNAME           "UserName"
#define SET_WEB_MULTI_PARAMS_USERPW             "UserPw"
#define SET_WEB_MULTI_PARAMS_RATE               "Rate"
#define SET_WEB_MULTI_PARAMS_LENBUF             "LenBuf"
#define SET_WEB_MULTI_PARAMS_TITLE              "Title"

#define SET_WEB_MULTI_PARAMS_DECURL             "DecUrl"
#define SET_WEB_MULTI_PARAMS_CONTACT            "Contact"
#define SET_WEB_MULTI_PARAMS_VIDEOENCTYPE       "VideoEncType"
#define SET_WEB_MULTI_PARAMS_AUDIOENCTYPE       "AudioEncType"
#define SET_WEB_MULTI_PARAMS_SENDVIDEOSRC       "SendVideoSrc"
#define SET_WEB_MULTI_PARAMS_SENDAUDIOSRC       "SendAudioSrc"

#define SET_WEB_AUDIO_PARAMS_VOLSIZE            "VolSize"
#define SET_WEB_AUDIO_PARAMS_VOLCHN             "VolChn"

#define SET_WEB_EFFECT_PARAMS_VOERLAYCHN        "OverlayChn"
#define SET_WEB_EFFECT_PARAMS_VOERLAYPOST       "OverlayPost"
#define SET_WEB_EFFECT_PARAMS_VOERLAYNUM        "OverlayNum"
#define SET_WEB_EFFECT_PARAMS_VOERLAYTYPE       "OverlayType"
#define SET_WEB_EFFECT_PARAMS_VOERLAYCONTENT    "OverlayContent"

#define SET_WEB_EXTEND_PARAMS_IP                "IP"
#define SET_WEB_EXTEND_PARAMS_GATEWAY           "GateWay"
#define SET_WEB_EXTEND_PARAMS_NETMASK           "NetMask"
#define SET_WEB_EXTEND_PARAMS_MACADDR           "MacAddr"
#define SET_WEB_EXTEND_PARAMS_MAINDNS           "MainDNS"
#define SET_WEB_EXTEND_PARAMS_BACKUPDNS         "BackupDNS"

#define SET_WEB_EXTEND_PARAMS_SYNCTIME         "SyncTime"
#define SET_WEB_EXTEND_PARAMS_DEVID            "DevID"

#define SET_WEB_EXTEND_PARAMS_FIRMUPDATE          "FirmUpdate"
#define SET_WEB_EXTEND_PARAMS_ZKTUPDATE           "ZKTUpdate"
#define SET_WEB_EXTEND_PARAMS_CFGUPDATE           "CfgUpdate"
#define SET_WEB_EXTEND_PARAMS_DISCIPFILEUPDATE    "DiscipFileUpdate"

#define SET_WEB_EXTEND_PARAMS_BACKUPCFG           "BackupCfg"
#define SET_WEB_EXTEND_PARAMS_RECOVERCFG          "RecoverCFG"

#define GET_WEB_EXTEND_PARAMS_LOGLIST          "LogList"


/********************************************************
 * GetRecordList method & params & response
 *******************************************************/
#define GET_INFOLIST_REC_PARAMS_GETMODE           "GetMode" /*params*/
#define GET_INFOLIST_REC_PARAMS_TASKID            "TaskID" /*params*/
#define GET_INFOLIST_REC_PARAMS_STARTTIME         "StartTime" /*params*/
#define GET_INFOLIST_REC_PARAMS_ENDTIME           "EndTime" /*params*/

#define GET_INFOLIST_RES_VIDEOFILE              "VideoFiles" /*response */
#define GET_INFOLIST_RES_ALLOWDELETE            "AllowDelete" /*response */
#define GET_INFOLIST_RES_TOTAL                  "Total" /*response */

#define GET_INFOLIST_RES_FILESORT               "FileSort" /*response */
#define GET_INFOLIST_RES_TASKID                 "TaskID" /*response */
#define GET_INFOLIST_RES_FILENAME               "FileName" /*response */
#define GET_INFOLIST_RES_SIZE                   "Size" /*response */
#define GET_INFOLIST_RES_FILEPATH               "FilePath" /*response */
#define GET_INFOLIST_RES_STARTTIME              "StartTime" /*response */
#define GET_INFOLIST_RES_ENDTIME                "EndTime" /*response */
#define GET_INFOLIST_RES_CHANNELNUM             "ChannelNum" /*response */
#define GET_INFOLIST_RES_CHANNELDESC            "ChannelDesc" /*response */
#define GET_INFOLIST_RES_FTPPATH                "FtpPath" /*response */
#define GET_INFOLIST_RES_FTPUSR                 "Ftpusr" /*response */
#define GET_INFOLIST_RES_FTPPASSWD              "Ftppasswd" /*response */
#define GET_INFOLIST_RES_VODURLRTSP             "VodurlRtsp" /*response */
#define GET_INFOLIST_RES_VODURLRTMP             "VodurlRtmp" /*response */

/********************************************************
 * SetRtmpCliPush method & params & response
 *******************************************************/
#define SET_START_REC_PARAMS_RTMPID           "RtmpID" /*params*/
#define SET_START_REC_PARAMS_RTMPADDR         "Addr" /*params*/
#define SET_START_REC_PARAMS_RTMPCHANNEL      "Channel" /*params*/

/********************************************************
 * SetOpenStream method & params & response
 *******************************************************/
#define SET_START_REC_PARAMS_OPENALLFLAG           "AllFlag" /*params*/
#define SET_START_REC_PARAMS_OPENCHANNELID         "ChannelID" /*params*/

/********************************************************
 * SetCloseStream method & params & response
 *******************************************************/
#define SET_START_REC_PARAMS_CLOSEALLFLAG           "AllFlag" /*params*/
#define SET_START_REC_PARAMS_CLOSECHANNELID         "ChannelID" /*params*/

/********************************************************
 * SetSysCmd method & params & response
 *******************************************************/
#define SET_START_POWER_PARAMS_CMD           "Cmd" /*params*/

/********************************************************
 * hostStopDec method & params & response
 *******************************************************/
#define HOST_STOP_DEC_PARAMS_CHANNEL            "channel" /*params*/
 
/********************************************************
 * hostDecStatus method & params & response
 *******************************************************/
#define HOST_DEC_STATUS_PARAMS_CHANNEL          "channel" /*params*/
#define HOST_DEC_STATUS_RES_DEC_STATUS          "decodeStatus" /*response */

/********************************************************
 * hostDecStatus method & params & response
 *******************************************************/
#define HOST_DEC_INFO_PARAMS_RTSP_CHANNEL REQUEST_NODE_RTSPDECNUM /*params*/
#define HOST_DEC_INFO_PARAMS_H323_CHANNEL REQUEST_NODE_H323DECNUM /*params*/

/********************************************************
 * hostChnStatus method & params & response
 *******************************************************/
#define HOST_DEC_INFO_PARAMS_CHN_CHANNEL  REQUEST_NODE_CHNNUM /*params*/
#define HOST_DEC_INFO_PARAMS_RTMP_CHANNEL REQUEST_NODE_RTMPNUM /*params*/

/********************************************************
 * sendhttpcontent method & params & response
 *******************************************************/
#define SEND_HTTP_CONTENT_DSTURL            "dstUrl" /*params*/
#define SEND_HTTP_CONTENT_CONTENT           "content" /*params*/

/********************************************************
 * hostDisciplineStatus method & params & response
 *******************************************************/
#define HOST_GET_DISCIPLINE_STATUS_RES_DCPL_STATUS          "disciplineStatus" /*response*/


//add new method params 
// by libx in 2022/02/23
//--[BEGIN]--

/********************************************************
 * GetWebInfraredStudyList method & params & response
 *******************************************************/
#define GET_WEB_INFRARED_STUDYLIST_PARAMS_PORT           "Port"     /*params*/
#define GET_WEB_INFRARED_STUDYLIST_PARAMS_FILENAME       "FileName" /*params*/

#define GET_WEB_INFRARED_STUDYLIST_RES_TOTAL            GET_TESK_CONTENTS_RES_TOTAL      /*response */
#define GET_WEB_INFRARED_STUDYLIST_RES_VIDEOPARAMLIST   "VideoParamList"                 /*response */
#define GET_WEB_INFRARED_STUDYLIST_RES_NUM              SET_WEB_INFRARED_STUDY_PARAMS_NUM    /*response */
#define GET_WEB_INFRARED_STUDYLIST_RES_ORDER            SET_WEB_INFRARED_STUDY_PARAMS_ORDER  /*response */
#define GET_WEB_INFRARED_STUDYLIST_RES_DESCRIBE         SET_WEB_INFRARED_STUDY_PARAMS_DESCRIBE   /*response */

 /********************************************************
  * SetWebInfraredStudy method & params & response
  *******************************************************/
#define SET_WEB_INFRARED_STUDY_PARAMS_NUM                "Num"      /*params*/
#define SET_WEB_INFRARED_STUDY_PARAMS_PORT                GET_WEB_INFRARED_STUDYLIST_PARAMS_PORT /*params*/
#define SET_WEB_INFRARED_STUDY_PARAMS_ORDER               "Order"       /*params*/
#define SET_WEB_INFRARED_STUDY_PARAMS_DESCRIBE            "Describe"    /*params*/
#define SET_WEB_INFRARED_STUDY_PARAMS_FILENAME            "FileName"    /*params*/

/********************************************************
 * SetWebInfraredDel method & params & response
 *******************************************************/
#define SET_WEB_INFRARED_DEL_PARAMS_NUM                 SET_WEB_INFRARED_STUDY_PARAMS_NUM       /*params*/
#define SET_WEB_INFRARED_DEL_PARAMS_PORT                GET_WEB_INFRARED_STUDYLIST_PARAMS_PORT  /*params*/
#define SET_WEB_INFRARED_DEL_PARAMS_ORDER               SET_WEB_INFRARED_STUDY_PARAMS_ORDER     /*params*/
#define SET_WEB_INFRARED_DEL_PARAMS_DESCRIBE            SET_WEB_INFRARED_STUDY_PARAMS_DESCRIBE  /*params*/
#define SET_WEB_INFRARED_DEL_PARAMS_FILENAME            SET_WEB_INFRARED_STUDY_PARAMS_FILENAME  /*params*/

/********************************************************
* SetWebInfraredSend method & params & response
*******************************************************/
#define SET_WEB_INFRARED_SEND_PARAMS_NUM                SET_WEB_INFRARED_STUDY_PARAMS_NUM       /*params*/
#define SET_WEB_INFRARED_SEND_PARAMS_PORT               GET_WEB_INFRARED_STUDYLIST_PARAMS_PORT  /*params*/
#define SET_WEB_INFRARED_SEND_PARAMS_ORDER              SET_WEB_INFRARED_STUDY_PARAMS_ORDER     /*params*/
#define SET_WEB_INFRARED_SEND_PARAMS_DESCRIBE           SET_WEB_INFRARED_STUDY_PARAMS_DESCRIBE  /*params*/
#define SET_WEB_INFRARED_SEND_PARAMS_FILENAME           SET_WEB_INFRARED_STUDY_PARAMS_FILENAME  /*params*/

/********************************************************
* SetWebInfraredCreate method & params & response
*******************************************************/
#define SET_WEB_INFRARED_CREATE_PARAMS_PORT             GET_WEB_INFRARED_STUDYLIST_PARAMS_PORT  /*params*/
#define SET_WEB_INFRARED_CREATE_PARAMS_FILENAME         SET_WEB_INFRARED_STUDY_PARAMS_FILENAME  /*params*/

/********************************************************
* SetWebAudioConf method & params & response
*******************************************************/
#define SET_WEB_AUDIO_CONF_PARAMS_ISPOWER               "IsPower"       /*params*/
#define SET_WEB_AUDIO_CONF_PARAMS_ISINCENTIVES          "IsIncentives"  /*params*/
#define SET_WEB_AUDIO_CONF_PARAMS_ISFEEDBACK            "IsFeedback"    /*params*/
#define SET_WEB_AUDIO_CONF_PARAMS_ISAEC                 "IsAEC"         /*params*/
#define SET_WEB_AUDIO_CONF_PARAMS_ISAGC                 "IsAGC"         /*params*/
#define SET_WEB_AUDIO_CONF_PARAMS_ISANS                 "IsANS"         /*params*/
#define SET_WEB_AUDIO_CONF_PARAMS_SPL                   "SPL"           /*params*/

/********************************************************
 * SetWebAudioGroupConf method & params & response
 *******************************************************/
#define SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_GROUPNAME     "GroupName" /*params*/
#define SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_CHNNUM        "ChnNum"    /*params*/

 /********************************************************
  * SetWebAudioMatrixConf method & params & response
  *******************************************************/
#define SET_WEB_AUDIO_MATRIX_CONFIG_PARAMS_CHNNUM       SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_CHNNUM    /*params*/

 /********************************************************
  * SetWebVideoParam method & params & response
  *******************************************************/
#define SET_WEB_VIDEO_PARAMS_CHNNAME                 "ChnName"      /*params*/
#define SET_WEB_VIDEO_PARAMS_RESOLUTIONSET           "ResolutionSet"    /*params*/
#define SET_WEB_VIDEO_PARAMS_CODEMODE                "CodeMode"     /*params*/
#define SET_WEB_VIDEO_PARAMS_RATETYPE                "RateType"     /*params*/
#define SET_WEB_VIDEO_PARAMS_RATESIZE                "RateSize"     /*params*/
#define SET_WEB_VIDEO_PARAMS_FRAMESET                "FrameSet"     /*params*/
#define SET_WEB_VIDEO_PARAMS_GOPSET                  "GopSet"       /*params*/

/********************************************************
* SetWebAudioParam method & params & response
*******************************************************/
#define SET_WEB_AUDIO_PARAMS_SOUNDNUM                 "SoundNum"        /*params*/
#define SET_WEB_AUDIO_PARAMS_SOUNDSOURCE              "SoundSource"     /*params*/
#define SET_WEB_AUDIO_PARAMS_CODEMODE                 SET_WEB_VIDEO_PARAMS_CODEMODE /*params*/
#define SET_WEB_AUDIO_PARAMS_GAIN                     "Gain"            /*params*/
#define SET_WEB_AUDIO_PARAMS_SAMPLERATE               "SamplingRate"    /*params*/
#define SET_WEB_AUDIO_PARAMS_SOUNDCHN                 "SoundChn"        /*params*/
#define SET_WEB_AUDIO_PARAMS_RATESET                  "RateSet"         /*params*/

/********************************************************
 * SetWebRtspParam method & params & response
 *******************************************************/
#define SET_WEB_RTSP_PARAMS_SOUNDNUM                    "SoundNum"      /*params*/
#define SET_WEB_RTSP_PARAMS_RTSPSOURCE                  "RtspSource"    /*params*/
#define SET_WEB_RTSP_PARAMS_RTSPURL                     "RtspUrl"       /*params*/

//add by ltz 2022-03-08
#define SET_WEB_RTSP_PARAMS_RTSPNUM                     "RtspNum"      /*params*/
#define AUDIO_CHN_CONF_PARAMS_CHNNUM                    "ChnNum"
#define AUDIO_CHN_CONF_PARAMS_VAL				        "Val"
#define AUDIO_CHN_CONF_PARAMS_CTRLTYPE                  "CtrlType"

//add by ltz 2022-03-08 END

/********************************************************
* SetWebRtmpParam method & params & response
*******************************************************/
#define SET_WEB_RTMP_PARAMS_NUM                         SET_WEB_INFRARED_STUDY_PARAMS_NUM   /*params*/
#define SET_WEB_RTMP_PARAMS_RTMPNAME                    "RtmpName"      /*params*/
#define SET_WEB_RTMP_PARAMS_ISHTTP                      "IsHttp"        /*params*/
#define SET_WEB_RTMP_PARAMS_ISHLS                       "IsHls"         /*params*/
#define SET_WEB_RTMP_PARAMS_ISRTMP                      "IsRtmp"        /*params*/
#define SET_WEB_RTMP_PARAMS_ISRTSP                      "IsRtsp"        /*params*/
#define SET_WEB_RTMP_PARAMS_ISBROADCAST                 "IsBroadcast"   /*params*/
#define SET_WEB_RTMP_PARAMS_BROADCASTADDR               "BroadcastAddr" /*params*/
#define SET_WEB_RTMP_PARAMS_ISPUSHSTREAM                "IsPushStream"  /*params*/
#define SET_WEB_RTMP_PARAMS_PUSHADDR                    "PushAddr"      /*params*/

/********************************************************
* SetWebLogFileConf method & params & response
*******************************************************/
#define SET_WEB_LOG_FILE_CONF_PARAMS_LOGSIZE            "LogSize"       /*params*/
#define SET_WEB_LOG_FILE_CONF_PARAMS_LOGSAVETIME        "LogSaveTime"   /*params*/
#define SET_WEB_LOG_FILE_CONF_PARAMS_LOGLEVEL           "LogLevel"      /*params*/

/********************************************************
* SetWebDevID method & params & response
*******************************************************/
#define SET_WEB_DEV_ID_PARAMS_DEVID                     "DevID" /*params*/

/********************************************************
* SetWebRtmpBroadConf method & params & response
*******************************************************/
#define SET_WEB_RTMP_BROAD_CONF_PARAMS_ISENABLED            "IsEnabled"  /*params*/
#define SET_WEB_RTMP_BROAD_CONF_PARAMS_BROADADDR           "BroadAddr"   /*params*/

/********************************************************
* SetWebFirmUpdate method & params & response
*******************************************************/
#define SET_WEB_FIRM_UPDATE_PARAMS_FIRMUPDATE               SET_WEB_EXTEND_PARAMS_FIRMUPDATE    /*params*/

/********************************************************
* SetWebZKTUpdate method & params & response
*******************************************************/
#define SET_WEB_ZKT_UPDATE_PARAMS_ZKTUPDATE             SET_WEB_EXTEND_PARAMS_ZKTUPDATE     /*params*/

/********************************************************
* SetWebZKTDownload method & params & response
*******************************************************/
#define SET_WEB_ZKT_DOWNLOAD_PARAMS_ZKTUPDATE           SET_WEB_ZKT_UPDATE_PARAMS_ZKTUPDATE /*params*/

/********************************************************
* SetWebDiscipFileUpdate method & params & response
*******************************************************/
#define SET_WEB_DISCIPFILE_UPDATE_PARAMS_DISCIPFILEUPDATE   SET_WEB_EXTEND_PARAMS_DISCIPFILEUPDATE  /*params*/

/********************************************************
* SetWebRecoverCFGFile method & params & response
*******************************************************/
#define SET_WEB_RECOVER_CFGFILE_PARAMS_RECOVERCFG       SET_WEB_EXTEND_PARAMS_RECOVERCFG    /*params*/

/********************************************************
 * GetWebAudioGroupConf method & params & response
 *******************************************************/
#define GET_WEB_AUDIO_GROUP_CONF_RES_TOTAL              GET_TESK_CONTENTS_RES_TOTAL         /*response */
#define GET_WEB_AUDIO_GROUP_CONF_RES_AUDIOGROUPLIST     "AudioGroupList"                    /*response */
#define GET_WEB_AUDIO_GROUP_CONF_RES_GROUPNAME          SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_GROUPNAME /*response */
#define GET_WEB_AUDIO_GROUP_CONF_RES_CHNNUM             SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_CHNNUM    /*response */

/********************************************************
* GetWebAudioConf method & params & response
*******************************************************/
#define GET_WEB_AUDIO_CONF_RES_TOTAL                    GET_TESK_CONTENTS_RES_TOTAL         /*response */
#define GET_WEB_AUDIO_CONF_RES_AUDIOPARAMLIST           "AudioParamList"                    /*response */
#define GET_WEB_AUDIO_CONF_RES_ISPOWER                  SET_WEB_AUDIO_CONF_PARAMS_ISPOWER   /*response */
#define GET_WEB_AUDIO_CONF_RES_ISINCENTIVES             SET_WEB_AUDIO_CONF_PARAMS_ISINCENTIVES  /*response */
#define GET_WEB_AUDIO_CONF_RES_ISFEEDBACK               SET_WEB_AUDIO_CONF_PARAMS_ISFEEDBACK   /*response */
#define GET_WEB_AUDIO_CONF_RES_ISAEC                    SET_WEB_AUDIO_CONF_PARAMS_ISAEC        /*response */
#define GET_WEB_AUDIO_CONF_RES_ISAGC                    SET_WEB_AUDIO_CONF_PARAMS_ISAGC        /*response */
#define GET_WEB_AUDIO_CONF_RES_ISANS                    SET_WEB_AUDIO_CONF_PARAMS_ISANS        /*response */
#define GET_WEB_AUDIO_CONF_RES_SPL                      SET_WEB_AUDIO_CONF_PARAMS_SPL       /*response */

/********************************************************
* GetWebAudioMatrixConf method & params & response
*******************************************************/
#define GET_WEB_AUDIO_MATRIX_CONF_RES_TOTAL             GET_TESK_CONTENTS_RES_TOTAL             /*response */
#define GET_WEB_AUDIO_MATRIX_CONF_RES_AUDIOMATRIXLIST   "AudioMatrixList"                       /*response */
//#define GET_WEB_AUDIO_MATRIX_CONF_RES_GROUPNAME         SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_GROUPNAME /*response */
#define GET_WEB_AUDIO_MATRIX_CONF_RES_CHNNUM            SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_CHNNUM    /*response */

/********************************************************
* GetWebVideoParam method & params & response
*******************************************************/
#define GET_WEB_VIDEO_PARAM_RES_TOTAL                   GET_TESK_CONTENTS_RES_TOTAL                 /*response */
#define GET_WEB_VIDEO_PARAM_RES_VIDEOPARAMLIST          GET_WEB_INFRARED_STUDYLIST_RES_VIDEOPARAMLIST   /*response */
#define GET_WEB_VIDEO_PARAM_RES_CHNNUM                  SET_WEB_AUDIO_GROUP_CONFIG_PARAMS_CHNNUM    /*response */
#define GET_WEB_VIDEO_PARAM_RES_CHNNAME                 SET_WEB_VIDEO_PARAMS_CHNNAME            /*response */
#define GET_WEB_VIDEO_PARAM_RES_RESOLUTIONSET           SET_WEB_VIDEO_PARAMS_RESOLUTIONSET          /*response */
#define GET_WEB_VIDEO_PARAM_RES_CODEMODE                SET_WEB_VIDEO_PARAMS_CODEMODE           /*response */
#define GET_WEB_VIDEO_PARAM_RES_RATETYPE                SET_WEB_VIDEO_PARAMS_RATETYPE           /*response */
#define GET_WEB_VIDEO_PARAM_RES_RATESIZE                SET_WEB_VIDEO_PARAMS_RATESIZE       /*response */
#define GET_WEB_VIDEO_PARAM_RES_FRAMESET                SET_WEB_VIDEO_PARAMS_FRAMESET       /*response */
#define GET_WEB_VIDEO_PARAM_RES_GOPSET                  SET_WEB_VIDEO_PARAMS_GOPSET         /*response */

/********************************************************
* GetWebAudioParam method & params & response
*******************************************************/
#define GET_WEB_AUDIO_PARAM_RES_TOTAL                   GET_TESK_CONTENTS_RES_TOTAL             /*response */
#define GET_WEB_AUDIO_PARAM_RES_AUDIOPARAMLIST          GET_WEB_AUDIO_CONF_RES_AUDIOPARAMLIST   /*response */
#define GET_WEB_AUDIO_PARAM_RES_SOUNDNUM                SET_WEB_AUDIO_PARAMS_SOUNDNUM       /*response */
#define GET_WEB_AUDIO_PARAM_RES_SOUNDSOURCE             SET_WEB_AUDIO_PARAMS_SOUNDSOURCE        /*response */
#define GET_WEB_AUDIO_PARAM_RES_CODEMODE                SET_WEB_VIDEO_PARAMS_CODEMODE       /*response */
#define GET_WEB_AUDIO_PARAM_RES_GAIN                    SET_WEB_AUDIO_PARAMS_GAIN           /*response */
#define GET_WEB_AUDIO_PARAM_RES_SAMPLERATE              SET_WEB_AUDIO_PARAMS_SAMPLERATE     /*response */
#define GET_WEB_AUDIO_PARAM_RES_SOUNDCHN                SET_WEB_AUDIO_PARAMS_SOUNDCHN       /*response */
#define GET_WEB_AUDIO_PARAM_RES_RATESET                 SET_WEB_AUDIO_PARAMS_RATESET        /*response */

/********************************************************
* GetWebRtspParam method & params & response
*******************************************************/
#define GET_WEB_RTSP_PARAM_RES_TOTAL                    GET_TESK_CONTENTS_RES_TOTAL             /*response */
#define GET_WEB_RTSP_PARAM_RES_AUDIOPARAMLIST           GET_WEB_AUDIO_CONF_RES_AUDIOPARAMLIST   /*response */
#define GET_WEB_RTSP_PARAM_RES_SOUNDNUM                 SET_WEB_AUDIO_PARAMS_SOUNDNUM           /*response */
#define GET_WEB_RTSP_PARAM_RES_RTSPSOURCE               SET_WEB_RTSP_PARAMS_RTSPSOURCE          /*response */
#define GET_WEB_RTSP_PARAM_RES_RTSPURL                  SET_WEB_RTSP_PARAMS_RTSPURL             /*response */

#define GET_WEB_RTSP_PARAM_RES_RTSP_PARAMLIST             "RtspParamList"           /*response */

/********************************************************
* GetWebRtmpParam method & params & response
*******************************************************/
#define GET_WEB_RTMP_PARAM_RES_TOTAL                     GET_TESK_CONTENTS_RES_TOTAL            /*response */
#define GET_WEB_RTMP_PARAM_RES_RTMPPARAMLIST              "RtmpParamList"                       /*response */
#define GET_WEB_RTMP_PARAM_RES_NUM                        SET_WEB_INFRARED_STUDY_PARAMS_NUM     /*response */
#define GET_WEB_RTMP_PARAM_RES_RTMPNAME                   SET_WEB_RTMP_PARAMS_RTMPNAME          /*response */
#define GET_WEB_RTMP_PARAM_RES_ISHTTP                     SET_WEB_RTMP_PARAMS_ISHTTP            /*response */
#define GET_WEB_RTMP_PARAM_RES_ISHLS                      SET_WEB_RTMP_PARAMS_ISHLS             /*response */
#define GET_WEB_RTMP_PARAM_RES_ISRTMP                     SET_WEB_RTMP_PARAMS_ISRTMP            /*response */
#define GET_WEB_RTMP_PARAM_RES_ISRTSP                     SET_WEB_RTMP_PARAMS_ISRTSP            /*response */
#define GET_WEB_RTMP_PARAM_RES_ISBROADCAST                SET_WEB_RTMP_PARAMS_ISBROADCAST       /*response */
#define GET_WEB_RTMP_PARAM_RES_BROADCASTADDR              SET_WEB_RTMP_PARAMS_BROADCASTADDR     /*response */
#define GET_WEB_RTMP_PARAM_RES_ISPUSHSTREAM               SET_WEB_RTMP_PARAMS_ISPUSHSTREAM      /*response */
#define GET_WEB_RTMP_PARAM_RES_PUSHADDR                   SET_WEB_RTMP_PARAMS_PUSHADDR          /*response */

/********************************************************
* GetWebLogFile method & params & response
*******************************************************/
#define GET_WEB_LOG_FILE_RES_TOTAL                        GET_TESK_CONTENTS_RES_TOTAL   /*response */
#define GET_WEB_LOG_FILE_RES_LOGLIST                      GET_WEB_EXTEND_PARAMS_LOGLIST /*response */
#define GET_WEB_LOG_FILE_RES_LOGNUM                         "LogNum"                    /*response */
#define GET_WEB_LOG_FILE_RES_LOGNAME                        "LogName"                   /*response */
#define GET_WEB_LOG_FILE_RES_LOGSIZE                        "LogSize"                   /*response */
#define GET_WEB_LOG_FILE_RES_LOGTIME                         "LogTime"                  /*response */
#define GET_WEB_LOG_FILE_RES_LOGADDR                        "LogAddr"                   /*response */

/********************************************************
 * AddWebParty method & params & response
 *******************************************************/
#define ADD_WEB_PARTY_PARAMS_NAME                           "Name"
#define ADD_WEB_PARTY_PARAMS_PROTOCOL                       "Protocol"
#define ADD_WEB_PARTY_PARAMS_ADDRESS                        "Address"
#define ADD_WEB_PARTY_PARAMS_BITRATE                        "BitRate"
#define ADD_WEB_PARTY_PARAMS_BUFFERLENGTH                   "BufferLength"
#define ADD_WEB_PARTY_PARAMS_TRANSPORT                      "Transport"

 /********************************************************
  * DelWebParty method & params & response
  *******************************************************/
#define DEL_WEB_PARTY_PARAMS_ID                             "ID"

  /********************************************************
   * GetWebAddrList method & params & response
   *******************************************************/
#define GET_WEB_ADDR_LIST_RES_TOTAL                         GET_TESK_CONTENTS_RES_TOTAL
#define GET_WEB_ADDR_LIST_RES_PARTYLIST                     "AddressList"
#define GET_WEB_ADDR_LIST_RES_ID                            DEL_WEB_PARTY_PARAMS_ID
#define GET_WEB_ADDR_LIST_RES_NAME                          ADD_WEB_PARTY_PARAMS_NAME
#define GET_WEB_ADDR_LIST_RES_PROTOCOL                      ADD_WEB_PARTY_PARAMS_PROTOCOL
#define GET_WEB_ADDR_LIST_RES_ADDRESS                       ADD_WEB_PARTY_PARAMS_ADDRESS
#define GET_WEB_ADDR_LIST_RES_BITRATE                       ADD_WEB_PARTY_PARAMS_BITRATE
#define GET_WEB_ADDR_LIST_RES_BUFFERLENGTH                  ADD_WEB_PARTY_PARAMS_BUFFERLENGTH
#define GET_WEB_ADDR_LIST_RES_TRANSPORT                     ADD_WEB_PARTY_PARAMS_TRANSPORT

//////////////////////////////////////////////////////////////////////////////////////////

#define LOGIN_PARAMS_USERNAME                           "name"
#define LOGIN_PARAMS_PASSWORD                           "password"

#define SET_WEB_NEW_MOSAIC_PARAMS_MOSAICCHN              "MosaicChn"
#define SET_WEB_NEW_MOSAIC_PARAMS_MOSAICPOST             "MosaicPost"
#define SET_WEB_NEW_MOSAIC_PARAMS_MOSAICNUM              "MosaicNum"
#define SET_WEB_NEW_MOSAIC_PARAMS_MOSAICTYPE             "MosaicType"

#define SET_WEB_NEW_WHINE_PARAMS_WHINECHN                "WhineChn"
#define SET_WEB_NEW_WHINE_PARAMS_WHINETYPE               "WhineType"

#define SET_WEB_WHINE_PARAMS_WHINECHN                     SET_WEB_NEW_WHINE_PARAMS_WHINECHN
#define SET_WEB_WHINE_PARAMS_WHINETYPE                    SET_WEB_NEW_WHINE_PARAMS_WHINETYPE

#define SET_WEB_SYNC_TIME_PARAMS_NTPIP                    "NtpIp"
#define SET_WEB_SYNC_TIME_PARAMS_SYSTIME                  "SysTime"

#define SET_USER_PASSWORD_PARAMS_USERNAME                 "UserName"
#define SET_USER_PASSWORD_PARAMS_OLDPASSWORD              "OldPassword"
#define SET_USER_PASSWORD_PARAMS_NEWPASSWORD              "NewPassword"
#define SET_USER_PASSWORD_PARAMS_VERIFYPASSWORD           "VerifyPassword"

#define SET_FTP_CONF_PARAMS_FTPSERVERIP                   "FtpServerIp"
#define SET_FTP_CONF_PARAMS_FTPPORT                       "FtpPort"
#define SET_FTP_CONF_PARAMS_FTPNAME                       "FtpName"
#define SET_FTP_CONF_PARAMS_FTPPASSWORD                   "FtpPassword"
#define SET_FTP_CONF_PARAMS_FTPPATH                       "FtpPath"

#define SET_WEB_UART_CONF_PARAMS_UARTPORT                 "UartPort"
#define SET_WEB_UART_CONF_PARAMS_BAUDRATE                 "BaudRate"
#define SET_WEB_UART_CONF_PARAMS_DATEBIT                  "DateBit"
#define SET_WEB_UART_CONF_PARAMS_STOPBIT                  "StopBit"
#define SET_WEB_UART_CONF_PARAMS_PARITYBIT                "ParityBit"

#define SET_WEB_VIDEO_OUT_PUT_CONF_PARAMS_CHNNAME           "ChnName"
#define SET_WEB_VIDEO_OUT_PUT_CONF_PARAMS_OUTPUTSOURCE      "OutputSource"
#define SET_WEB_VIDEO_OUT_PUT_CONF_PARAMS_OUTPURESOLUTION   "OutpuResolution"

#define SET_WEB_VIDEO_CHN_CONF_PARAMS_CHNNAME                "ChnName"
#define SET_WEB_VIDEO_CHN_CONF_PARAMS_CHNPORT                "ChnPort"
#define SET_WEB_VIDEO_CHN_CONF_PARAMS_ISSHOW                 "IsShow"


#define GET_WEB_STORAGE_CONF_PARAMS_AUDIORECORDING          "AudioRecording"
#define GET_WEB_STORAGE_CONF_PARAMS_STORAGETHRESOLD         "StorageThreshold"
#define GET_WEB_STORAGE_CONF_PARAMS_FILETYPE                "FileType"
#define GET_WEB_STORAGE_CONF_PARAMS_AUTODEL                 "AutoDel"
#define GET_WEB_STORAGE_CONF_PARAMS_OVERLAYDATE             "OverlayDate"
#define GET_WEB_STORAGE_CONF_PARAMS_SEGSIZE                 "SegSize"

//--[END]--


/********************************************************
 * typedef
 *******************************************************/
// channel status
typedef enum CHStatus{
    eCHStatusClose	= 0, //空闲 无直播流 无录制
    eCHStatusReady = 1, //有直播流 无录制
    eCHStatusRunning = 2, //录制中 有直播流
//    eCHStatusPaused = 3, //暂停录制 有直播流
    eCHStatusStopped = 3, //已经停止录制 有直播流
    eCHStatusSize
}CHStatus;

static string getCHStatusStr(CHStatus & s){
    switch (s){
        case eCHStatusClose:return "空闲,无直播流,无录制";
        case eCHStatusReady:return "有直播流,无录制";
        case eCHStatusRunning:return "录制中,有直播流";
//        case eCHStatusPaused:return "暂停录制,有直播流";
        case eCHStatusStopped:return "已经停止录制,有直播流";
        default:return "无此状态";
    }
}

static CHStatus CovertMediaStatus(MPSMediaStatus &s){
    switch (s){
        case eMPSStatusClose:return eCHStatusClose;//"无此事件或已经结束";
        case eMPSStatusReady:return eCHStatusReady;//"准备就绪";
        case eMPSStatusRunning:return eCHStatusRunning;//"正在工作";
//        case eMPSStatusPaused:return eCHStatusPaused;//"暂停";
                case eMPSStatusPaused:return eCHStatusSize;//"暂停";
        case eMPSStatusStopped:return eCHStatusStopped;//"已经停止";
        default:return eCHStatusSize;//"无此状态";
    }
}

//---- channel status

// 自定义服务反馈给客户端的消息code

typedef enum InterfaceResCode
{
    eInterfaceResCodeSuccess = 0,                   // 成功
    eInterfaceResCodeError = 111,                   // 出错
    eInterfaceResCodeError112 = 112,                   // 出错
    eInterfaceResCodeError113 = 113,                   // 出错
    eInterfaceResCodeError114 = 114,                   // 出错
    eInterfaceResCodeError115 = 115,                   // 出错
    eInterfaceResCodeError116 = 116,                   // 出错
    eInterfaceResCodeError117 = 117,                   // 出错
    eInterfaceResCodeError118 = 118,                   // 出错
    eInterfaceResCodeError119 = 119,                   // 出错
    eInterfaceResCodeError120 = 120,                   // 出错
    eInterfaceResCodeError121 = 121,                   // 出错
    eInterfaceResCodeError122 = 122,                   // 出错
    eInterfaceResCodeError123 = 123,                   // 出错
    eInterfaceResCodeError124 = 124,                   // 出错
    eInterfaceResCodeError125 = 125,                   // 出错
    eInterfaceResCodeError126 = 126,                   // 出错
    eInterfaceResCodeError127 = 127,                   // 出错
    eInterfaceResCodeError128 = 128,                   // 出错
    eInterfaceResCodeError129 = 129,                   // 设置编码参数失败，请输入正确的码率！
    eInterfaceResCodeError130 = 130,                   // 文件正在删除中，请稍后查看...
    eInterfaceResCodeError131 = 131,                   // 删除文件失败，请确保文件已上传

    eInterfaceResCodeRequestError = 400,            // 错误请求
    eInterfaceResCodeReqNoMethod = 401,             // 不存在此方法
    eInterfaceResCodeRequErrorParam = 402,          // 缺少参数或参数类型不正确
    eInterfaceResCodePassWordError = 403,			//用户密码输入错误
    eInterfaceResCodeUserNameError = 404,			//用户名输入错误

    eInterfaceResCodeErrorLocal = 500,              // 服务器错误
    eInterfaceResCodeErrorLocalDataConvert = 501,   // 本地数据转换出错
    eInterfaceResCodeErrorLocalStatus = 502,        // 服务器错误工作状态
    eInterfaceResCodeErrorTransmit = 504,           // 请求上游服务器出错
    eInterfaceResCodeErrorNoConnected = 505,        // 尚没有连接到上游服务器
    eInterfaceResCodeErrorTransmitTimeOut = 506,	// 请求上游服务器超时
    eInterfaceResCodeErrorSend = 507,               // 发送数据到上游服务器出错
    eInterfaceResCodeErrorRecv = 508,				// 接收上游服务器数据出错
    eInterfaceResCodeErrorTransmitNoModule= 511,    // 请求上游服务器,不存在此模块
    eInterfaceResCodeErrorTransmitNoMethod = 512,   // 请求上游服务器,不存在此方法

    eInterfaceResCodeErrorMPS = 600,                // 上游服务器(MPS)错误
    eInterfaceResCodeErrorOther = 650,              // 其他错误

    eInterfaceResCodeErrorMAX = 700,                //

} InterfaceResCode;

#define INTERFACERES_UNKNOWN_ERROR_MSG  "未知错误"
static string GetInterfaceResError(InterfaceResCode e)
{
    boost::format fmter("%1%,code:%2%");
	//boost::format fmter("%1%");
    if( e == eInterfaceResCodeSuccess ){
        fmter%"成功无错误"%e;
    }else if( eInterfaceResCodeSuccess < e && e < eInterfaceResCodeError){
        //(100,111) 未归类基本错误
        switch(e){
        case eInterfaceResCodeSuccess:
            fmter%"成功无错误"%eInterfaceResCodeSuccess;//return "成功无错误";
            break;
        default:
            fmter%INTERFACERES_UNKNOWN_ERROR_MSG%e;//return "未知错误,code:(100,111)";
            break;
        }
    }else if( eInterfaceResCodeError <= e && e < eInterfaceResCodeRequestError){
        //[111,400) 未归类错误
        switch(e){
        case eInterfaceResCodeSuccess:
            fmter%"成功无错误"%e;//return "成功无错误";
            break;
        case eInterfaceResCodeError:
            fmter%"错误"%e;//return "错误,code:111";
            break;
		case eInterfaceResCodeError112:
            fmter%"接收超时！获取音频增益失败"%e;//return "错误,code:112";
            break;
		case eInterfaceResCodeError113:
            fmter%"接收超时！获取静音状态失败"%e;//return "错误,code:113";
            break;
		case eInterfaceResCodeError114:
            fmter%"音频输入数据错误"%e;//return "错误,code:114";
            break;
		case eInterfaceResCodeError115:
            fmter%"音频输出数据错误"%e;//return "错误,code:115";
            break;
		case eInterfaceResCodeError116:
            fmter%"音频输入静音数据错误"%e;//return "错误,code:116";
            break;
		case eInterfaceResCodeError117:
            fmter%"音频输出静音数据错误"%e;//return "错误,code:111";
            break;
		case eInterfaceResCodeError118:
            fmter%"接收超时！获取MIC变声失败"%e;//return "错误,code:112";
            break;
		case eInterfaceResCodeError119:
            fmter%"接收超时！获取MIC变声开关失败"%e;//return "错误,code:113";
            break;
		case eInterfaceResCodeError120:
            fmter%"错误"%e;//return "错误,code:113";
            break;
		case eInterfaceResCodeError121:
            fmter%"错误"%e;//return "错误,code:111";
            break;
		case eInterfaceResCodeError122:
            fmter%"错误"%e;//return "错误,code:112";
            break;
		case eInterfaceResCodeError123:
            fmter%"错误"%e;//return "错误,code:113";
            break;
		case eInterfaceResCodeError124:
            fmter%"错误"%e;//return "错误,code:114";
            break;
		case eInterfaceResCodeError125:
            fmter%"错误"%e;//return "错误,code:115";
            break;
		case eInterfaceResCodeError126:
            fmter%"错误"%e;//return "错误,code:116";
            break;
		case eInterfaceResCodeError127:
            fmter%"错误"%e;//return "错误,code:111";
            break;
		case eInterfaceResCodeError128:
            fmter%"错误"%e;//return "错误,code:112";
            break;
		case eInterfaceResCodeError129:
            fmter%"设置失败，请输入正确的参数！"%e;//return "错误,code:113";
            break;
		case eInterfaceResCodeError130:
            fmter%"文件正在删除中，请稍后查看..."%e;//return "错误,code:113";
            break;
		case eInterfaceResCodeError131:
            fmter%"删除文件失败，请确保文件已上传"%e;//return "错误,code:113";
            break;
        default:
            fmter%INTERFACERES_UNKNOWN_ERROR_MSG%e;//return "未知错误,code:[111,400)";
            break;
        }
    }else if( eInterfaceResCodeRequestError <= e && e < eInterfaceResCodeErrorLocal){
        //[400,500) 请求错误
        switch(e){
        case eInterfaceResCodeRequestError:
            fmter%"错误请求"%e;//return "错误请求";
            break;
        case eInterfaceResCodeReqNoMethod:
            fmter%"不存在此方法"%e;//return "不存在此方法";
            break;
		case eInterfaceResCodePassWordError:
            fmter%"密码错误"%e;//密码错误
            break;
        case eInterfaceResCodeUserNameError:
            fmter%"用户名错误"%e;//用户名错误
            break;
        case eInterfaceResCodeRequErrorParam:
            fmter%"缺少参数或参数类型不正确"%e;//return "缺少参数或参数类型不正确";
            break;
        default:
            fmter%INTERFACERES_UNKNOWN_ERROR_MSG%e;//return "未知错误,code:[400,500)";
            break;
        }
    }else if( eInterfaceResCodeErrorLocal <= e && e < eInterfaceResCodeErrorMPS){
        //[500,600) 本地服务器错误
        switch(e){
        case eInterfaceResCodeErrorLocal:
            fmter%"服务器错误"%e;//return "服务器错误";
            break;
        case eInterfaceResCodeErrorLocalDataConvert:
            fmter%"服务器数据转换出错"%e;//return "服务器数据转换出错";
            break;
        case eInterfaceResCodeErrorLocalStatus:
            fmter%"服务器错误工作状态"%e;//return "服务器错误工作状态";
            break;
        case eInterfaceResCodeErrorTransmit:
            fmter%"请求上游服务器出错"%e;//return "请求上游服务器出错";
            break;
        case eInterfaceResCodeErrorNoConnected:
            fmter%"尚没有连接到上游服务器"%e;//return "尚没有连接到上游服务器";
            break;
        case eInterfaceResCodeErrorTransmitTimeOut:
            fmter%"请求上游服务器超时"%e;//return "请求上游服务器超时";
            break;
        case eInterfaceResCodeErrorSend:
            fmter%"发送数据到上游服务器出错"%e;//return "发送数据到上游服务器出错";
            break;
        case eInterfaceResCodeErrorRecv:
            fmter%"接收上游服务器数据出错"%e;//return "接收上游服务器数据出错";
            break;
        case eInterfaceResCodeErrorTransmitNoModule:
            fmter%"请求上游服务器,不存在此模块"%e;//return "请求上游服务器,不存在此模块";
            break;
        case eInterfaceResCodeErrorTransmitNoMethod:
            fmter%"请求上游服务器,不存在此方法"%e;//return "请求上游服务器,不存在此方法";
            break;
        default:
            fmter%INTERFACERES_UNKNOWN_ERROR_MSG%e;//return "未知错误,code:[500,600)";
            break;
        }
    }else if( eInterfaceResCodeErrorMPS <= e && e < eInterfaceResCodeErrorMAX){
        //[600,700) 上游服务器（MPS）错误
        int resCode = (int)e - (int)eInterfaceResCodeErrorMPS;
        string resH = "上游服务器(MPS)-";
        string resMsg;
        //获取mps错误信息
        if(e != eInterfaceResCodeErrorMPS){
            MPSOperationRes s= (MPSOperationRes)resCode;
            resMsg = getMPSOperationResStr(s);
        }else{
            resMsg = "错误";
        }
        fmter%(resH+resMsg)%e;
        /*
        switch(resCode){
        case 0: fmter%"上游服务器(MPS)错误"%e;//return "上游服务器(MPS)错误";
        case (int)(eMPSResultErrorGeneric): fmter%"上游服务器(MPS):普通错误，常见无法连接或传输出错"%e;//return "普通错误，常见无法连接或传输出错";
        case (int)(eMPSErrorStatusInvalid): fmter%"上游服务器(MPS):状态无效"%e;//return "状态无效";
        case (int)(eMPSErrorReadfile): fmter%"上游服务器(MPS):读取文件出错"%e;//return "读取文件出错";
        case (int)(eMPSErrorDeletefile): fmter%"上游服务器(MPS):删除文件出错"%e;//return "删除文件出错";
        case (int)(eMPSErrorException): fmter%"上游服务器(MPS):抛出异常，常见解析数据出错"%e;//return "抛出异常，常见解析数据出错";
        case (int)(eMPSErrorReceiverCreate): fmter%"上游服务器(MPS):无法创建接收类，供调试用，正常情况不出现"%e;//return "无法创建接收类，供调试用，正常情况不出现";
        case (int)(eMPSErrorWorkPackageCreate): fmter%"上游服务器(MPS):无法创建工作Package，供调试用，正常情况不出现"%e;//return "无法创建工作Package，供调试用，正常情况不出现";
        case (int)(eMPSErrorWorkfileSave): fmter%"上游服务器(MPS):无法保存工作文件，供调试用，正常情况不出现"%e;//return "无法保存工作文件，供调试用，正常情况不出现";
        case (int)(eMPSErrorPackagelistUpdate): fmter%"上游服务器(MPS):无法更新记录列表，供调试用，正常情况不出现"%e;//return "无法更新记录列表，供调试用，正常情况不出现";
        case (int)(eMPSErrorDspNotWork): fmter%"上游服务器(MPS):DSP没有正常工作"%e;//return "DSP没有正常工作";
        case (int)(eMPSErrorDspStatusInvalid): fmter%"上游服务器(MPS):DSP状态无效"%e;//return "DSP状态无效";
        case (int)(eMPSErrorDsp1Generic): fmter%"上游服务器(MPS):DSP1一般错误"%e;//return "DSP1一般错误";
        case (int)(eMPSErrorDsp2Generic): fmter%"上游服务器(MPS):DSP2一般错误"%e;//return "DSP2一般错误";
        case (int)(eMPSErrorNetworkMount): fmter%"上游服务器(MPS):无法挂载网络存储，常见存储服务异常"%e;//return "无法挂载网络存储，常见存储服务异常";
        case (int)(eMPSErrorNetworkUnmount): fmter%"上游服务器(MPS):卸载网络存储出错"%e;//return "卸载网络存储出错";
        case (int)(eMPSErrorNetworkCreateDirs): fmter%"上游服务器(MPS):无法在网络存储上创建文件夹结构，常见没有写权限或者存储已满"%e;//return "无法在网络存储上创建文件夹结构，常见没有写权限或者存储已满";
        case (int)(eMPSErrorLiverCreate): fmter%"上游服务器(MPS):无法创建直播类，常见网络地址设置错误"%e;//return "无法创建直播类，常见网络地址设置错误";
        case (int)(eMPSErrorLocalfileCreate): fmter%"上游服务器(MPS):无法创建本地文件"%e;//return "无法创建本地文件";
        case (int)(eMPSErrorNetworkfileCreate): fmter%"上游服务器(MPS):无法创建网络存储文件"%e;//return "无法创建网络存储文件";
        case (int)(eMPSErrorNoBackupfile): fmter%"上游服务器(MPS):不存在备份配置文件，常见没有备份就恢复的情况"%e;//return "不存在备份配置文件，常见没有备份就恢复的情况";
        case (int)(eMPSErrorInitMP3Encoder): fmter%"上游服务器(MPS):mp3编码器初始化失败"%e;//return "mp3编码器初始化失败";
        case (int)(eMPSErrorNetworkInvalid): fmter%"上游服务器(MPS):网络存储在上次开庭过程中异常，必须重启"%e;//return "网络存储在上次开庭过程中异常，必须重启";
        default: fmter%"上游服务器(MPS):未知错误"%e;//return "未知错误,code:[600,700)";
        }
        //*/
    }else{
        fmter%INTERFACERES_UNKNOWN_ERROR_MSG%e;
    }
    return fmter.str();
}

static InterfaceResCode CovertResponseCode(ResponseCode &e){
    switch (e){
    case eResponseCodeSuccess:return eInterfaceResCodeSuccess;
    case eResponseCodeNoModule:return eInterfaceResCodeErrorTransmitNoModule;
    case eResponseCodeNoMethod:return eInterfaceResCodeErrorTransmitNoMethod;
    case eResponseCodeNoConnected:return eInterfaceResCodeErrorNoConnected;
    case eResponseCodeErrorParamType:return eInterfaceResCodeErrorTransmit;
    case eResponseCodeErrorParam:return eInterfaceResCodeErrorTransmit;
    case eResponseCodeErrorServiceDataConvert:return eInterfaceResCodeErrorTransmit;
    case eResponseCodeErrorLocalDataConvert:return eInterfaceResCodeErrorLocalDataConvert;
    case eResponseCodeErrorSend:return eInterfaceResCodeErrorSend;
    case eResponseCodeErrorRecv:return eInterfaceResCodeErrorRecv;
    case eResponseCodeErrorTransmit:return eInterfaceResCodeErrorTransmit;
    default:return eInterfaceResCodeError;
    }
}

static InterfaceResCode CovertMPSOperationCode(MPSOperationRes &e){
    switch (e){
        case eMPSResultOK:
        return eInterfaceResCodeSuccess; //"成功无错误";
        case eMPSResultErrorGeneric: //return "普通错误，常见无法连接或传输出错";
        case eMPSErrorStatusInvalid: //return "状态无效";
        case eMPSErrorReadfile: //return "读取文件出错";
        case eMPSErrorDeletefile: //return "删除文件出错";
        case eMPSErrorException: //return "抛出异常，常见解析数据出错";
        case eMPSErrorReceiverCreate: //return "无法创建接收类，供调试用，正常情况不出现";
        case eMPSErrorWorkPackageCreate: //return "无法创建工作Package，供调试用，正常情况不出现";
        case eMPSErrorWorkfileSave: //return "无法保存工作文件，供调试用，正常情况不出现";
        case eMPSErrorPackagelistUpdate: //return "无法更新记录列表，供调试用，正常情况不出现";
        case eMPSErrorDspNotWork: //return "DSP没有正常工作";
        case eMPSErrorDspStatusInvalid: //return "DSP状态无效";
        case eMPSErrorDsp1Generic: //return "DSP1一般错误";
        case eMPSErrorDsp2Generic: //return "DSP2一般错误";
        case eMPSErrorNetworkMount: //return "无法挂载网络存储，常见存储服务异常";
        case eMPSErrorNetworkUnmount: //return "卸载网络存储出错";
        case eMPSErrorNetworkCreateDirs: //return "无法在网络存储上创建文件夹结构，常见没有写权限或者存储已满";
        case eMPSErrorLiverCreate: //return "无法创建直播类，常见网络地址设置错误";
        case eMPSErrorLocalfileCreate: //return "无法创建本地文件";
        case eMPSErrorNetworkfileCreate: //return "无法创建网络存储文件";
        case eMPSErrorNoBackupfile: //return "不存在备份配置文件，常见没有备份就恢复的情况";
        case eMPSErrorInitMP3Encoder: //return "mp3编码器初始化失败";
        case eMPSErrorNetworkInvalid: //return "网络存储在上次开庭过程中异常，必须重启";
        break;
        default:
        return eInterfaceResCodeErrorMPS; //"无此返回值";
    }
    int resCode = (int)eInterfaceResCodeErrorMPS +(int)e;
    return (InterfaceResCode)resCode;
}

//---- 自定义服务反馈给客户端的消息code

#endif // INTERFACEDEFINES_H

