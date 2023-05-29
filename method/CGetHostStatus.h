#ifndef GETHOSTSTATUS_H
#define GETHOSTSTATUS_H
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
 * SQLite3 headers
 *******************************************************/
#include <sqlite3.h>

/********************************************************
 * lib headers
 *******************************************************/
#include <log.h>
#include <TransmitClient.h>
#include <xmlParser.h>
#include <LibXML.h>
#include <Server.h>
#include <cJSON.h>

#include "config.h"
#include "interfaceDefines.h"

//存放设备状态（MediaStatus）数据库的位置
#define HOST_STATUS_SQLITE_DB_PATH  MPS_MEDIA_STATUS_SQL_DB_PATH

/********************************************************
 * namespace
 *******************************************************/
using namespace std;
using namespace SVS;

class CHostControl;
class CSetpoint;

class GetHostStatus
{
    friend class CHostControl;
    friend class CSetpoint;

public:
    GetHostStatus(string dbPath=HOST_STATUS_SQLITE_DB_PATH);

public:
    InterfaceResCode GetVersion(string &sResult);
    /**
     * @brief GethostStatus 获取设备的工作状态
     * @param chNum [1,4]
     * @param sResult
     * @return 0：成功，!0：失败.
     */
    InterfaceResCode GethostStatus(int chNum,string &sResult);
    /**
     * @brief GetStatus 获取设备状态
     * @param chNum [0,3]
     * @param chState
     * @return true：成功，false：失败.
     */
    bool GetStatus(int chNum,CHStatus &chState);

    string ComposeResult();
private:
    bool AddRootNode();

private:
    /**
     * @brief GetMediaStatus 获取Media status
     * @param chNum [0,3]
     * @param status
     * @return true：成功，false：失败.
     */
    bool GetMediaStatus(int chNum,MPSMediaStatus &status);

    /**
     * @brief GetMediaNowKeyID 获取Media 当前keyid
     * @param chNum [0,3]
     * @param keyID
     * @return
     */
    bool GetMediaNowKeyID(int chNum,string &keyID);

    /**
     * @brief GetMPSConfig 获取config配置信息
     * @param filePath
     * @param oConfig
     * @return true：成功，false：失败.
     */
    bool GetMPSConfigFromFile(CData &oConfig,string filePath = MPS_CONFIG_FILE_PATH);
    bool GetMPSConfigFromSocket(CData &oConfig);

    /**
     * @brief getLocalPath 获取存储根目录
     * @param oConfig
     * @param localPath
     * @return true：成功，false：失败.
     */
    bool getLocalPath(CData &oConfig,string &localPath);
    /**
     * @brief getDiskAlarm 获取报警阈值
     * @param oConfig
     * @param diskAlarm
     * @return true：成功，false：失败.
     */
    bool getDiskAlarm(CData &oConfig, SVSLONG32 &diskAlarm);

    /**
     * @brief calcDiskUsedPctAlarm 计算硬盘使用百分比和报警状态
     * @return
     */
    bool calcDiskUsedPctAlarm();
    /**
     * @brief analyzeSqlRes 分析sql执行结果
     * @param status
     * @return true：成功，false：失败.
     */
    bool analyzeGetStatusSqlRes(MPSMediaStatus &status);

    bool analyzeGetKeyIdSqlRes(string &keyId);
private:
    string _dbPath;
    string _localPath; //store-localPath
    SVSLONG32 _diskAlarmThreshold; //system-diskAlarm

    string _keyID;
    CHStatus _chStatus;
    double _diskUsedPct;
    bool _diskAlarm;

    cJSON *_result;
};

#endif // GETHOSTSTATUS_H
