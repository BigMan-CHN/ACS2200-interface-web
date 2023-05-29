#ifndef CONFIG_H
#define CONFIG_H
//===================================================================
#define __VERSION_  "1.4.5"
#define __DATE_     "2019-12-26 11:32:41"
//===================================================================
// MPS服务ip、port
//#define MPS_SOCKET_IP   "192.168.3.98"
#define MPS_SOCKET_IP   "127.0.0.1"
//#define MPS_SOCKET_PORT 5231
//add fwb 20201026
#define MPS_SOCKET_PORT 6180

// 设备配置文件
#define MPS_CONFIG_FILE_PATH            "/etc/mps/config.xml"
// 设备工作状态信息
#define MPS_MEDIA_STATUS_SQL_DB_PATH    "/etc/mps/4live.db"

// 日志配置文件
#define LOG_PROPERTIES_PATH "./log.properties"
#define LOG_CONSOLE_PROPERTIES_PATH "./console.properties"

//开始停止录制证据通道需要单独控制
//不注释 START_STOP_PROOF_ALONE_CONTROL 宏定义 表示需要单独控制
//注释   START_STOP_PROOF_ALONE_CONTROL 宏定义 表示不需要单独控制
//#define START_STOP_PROOF_ALONE_CONTROL

//===================================================================
// define unit constants
#ifndef Gsize
#define Gsize (1024.00*1024.00*1024.00)
#endif
#ifndef Msize
#define Msize (1024.00*1024.00)
#endif
#ifndef Ksize
#define Ksize (1024.00)
#endif

#endif // CONFIG_H

