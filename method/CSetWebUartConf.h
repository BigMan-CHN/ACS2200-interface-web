#ifndef SetWebUartConf_H
#define SetWebUartConf_H
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

class CSetWebUartConf
{
public:
    CSetWebUartConf(string ip=MPS_IP, int port=MPS_PORT, int timeOut=MPS_TIMEOUT);

    /**
     * @brief GetHostDecInfo 获取主机的网络配置信息和通道配置信息
     * @param sResult
     * @return 0：成功，!0：失败.num, baudrate, parity, databits, stopbits,requestStr
     */
    InterfaceResCode SetWebUartConf(string &sResult,int UartPort,int BaudRate,int DateBit,int StopBit,int ParityBit);
	InterfaceResCode SetWebOpenUart(string &sResult,int UartPort, int BaudRate,int DateBit,int StopBit, int ParityBit);
	InterfaceResCode SetWebGPIOMCU(string &sResult,int GPIONum,int Value);
	InterfaceResCode SetWebSendANDRecvUart(string &sResult,int UartPort, int DateType, string Data);
	
	InterfaceResCode UartSetConfig(string &sResult,int num, int baudrate,int parity,int databits, int stopbits, string requestStr);
	InterfaceResCode UartSendData(string &sResult, int num, int type, int buflength, string sendstring, string requestStr);
	InterfaceResCode UartGetConfig(string &sResult,int num, string requestStr);
	InterfaceResCode UartCreateTable(string &sResult, string dev_name, int dev_type, int dev_protocol, string requestStr);
	InterfaceResCode UartDelTable(string &sResult, string dev_name, string requestStr);
	InterfaceResCode UartSearchTable(string &sResult, string requestStr);
	InterfaceResCode UartAlterTable(string &sResult, string dev_name, string new_dev_name, int dev_type,int dev_protocol, string requestStr);
	
	InterfaceResCode UartAddTableControlCmd(string &sResult, string dev_name, string cmd_recv_code, string cmd_fun_des, string requestStr);
	InterfaceResCode UartAddTableCenterControlCmd(string &sResult, string dev_name, string cmd_recv_code, int cmd_type, string data_buf, int protocol_type,  string cmd_name, string requestStr);
	InterfaceResCode UartAddTableCameraCmd(string &sResult, string dev_name, string cmd_name, int protocol_type, int cmd_protocol_type, int cmd_holder_addr,string cmd_correlation_vi, string requestStr);

	InterfaceResCode UartDelTableControlCmd(string &sResult, string dev_name, string cmd_recv_code, string requestStr);
	InterfaceResCode UartDelTableControlExecuteCmd(string &sResult, string dev_name, string cmd_recv_code, string cmd_name, string requestStr);
	InterfaceResCode UartDelTableControlCameraCmd(string &sResult, string dev_name, string cmd_name, string requestStr);
	
	InterfaceResCode UartSearchTableControlCmd(string &sResult, string dev_name, string requestStr);
	InterfaceResCode UartSearchTableControlExecuteCmd(string &sResult, string dev_name, string cmd_recv_code, string requestStr);
	InterfaceResCode UartSearchCameraTableControlCmd(string &sResult, string dev_name, string requestStr);
	
	InterfaceResCode UartAlterTableControlCmd(string &sResult, string dev_name, string cmd_recv_code, string new_cmd_recv_code, string cmd_fun_des, string requestStr);
	InterfaceResCode UartAlterTableControlExecuteCmd(string &sResult, string dev_name, string cmd_recv_code, int cmd_type, string data_buf, int protocol_type, string cmd_name, string cmd_name_new, string requestStr);
	InterfaceResCode UartAlterTableControlCameraCmd(string &sResult, string dev_name, int protocol_type, string cmd_name, string cmd_name_new, int cmd_protocol_type, int cmd_holder_addr,string cmd_correlation_vi, string requestStr);

	InterfaceResCode UartPTZControl(string &sResult,  string cmd_name,     int move_spped, int control_type, string requestStr);
	InterfaceResCode UartCameraPresetControl(string &sResult, string cmd_name, int preset_num, int control_type, string requestStr);
	InterfaceResCode SetGPIOConfig(string &sResult,int channel,int Value, string requestStr);

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
    bool SetWebUartConfInner(int          UartPort, int BaudRate, int DateBit, int StopBit, int ParityBit, CData& oResult, char* cResult);
	bool SetWebOpenUartInner(int UartPort,int BaudRate,int DateBit,int StopBit,int ParityBit, CData& oResult, char* cResult);
	bool SetWebGPIOMCUInner(int GPIONum,int Value, CData& oResult, char* cResult);
	bool SetWebSendANDRecvUartInner(int UartPort, int DateType, string Data, CData& oResult, char* cResult);

	bool UartSetConfigInner(int num, int baudrate,int parity,int databits, int stopbits, string requestStr, CData& oResult, char* cResult);
	bool UartSendDataInner(int num, int type, int buflength, string sendstring, string requestStr, CData& oResult, char* cResult);
	bool UartGetConfigInner(int num, string requestStr, CData& oResult, char* cResult);
	bool UartCreateTableInner(string dev_name, int dev_type, int dev_protocol, string requestStr, CData& oResult, char* cResult);
	bool UartDelTableInner( string dev_name, string requestStr, CData& oResult, char* cResult);
	bool UartSearchTableInner(string requestStr, CData& oResult, char* cResult);
	bool UartAlterTableInner(string dev_name, string new_dev_name, int dev_type,int dev_protocol, string requestStr, CData& oResult, char* cResult);

	bool UartAddTableControlCmdInner( string dev_name, string cmd_recv_code, string cmd_fun_des, string requestStr, CData& oResult, char* cResult);
	bool UartAddTableCenterControlCmdInner(string dev_name, string cmd_recv_code, int cmd_type, string data_buf, int protocol_type,  string cmd_name, string requestStr, CData& oResult, char* cResult);
	bool UartAddTableCameraCmdInner(string dev_name, string cmd_name, int protocol_type, int cmd_protocol_type, int cmd_holder_addr,string cmd_correlation_vi,string requestStr, CData& oResult, char* cResult);

	bool UartDelTableControlCmdInner(string dev_name, string cmd_recv_code, string requestStr, CData& oResult, char* cResult);
	bool UartDelTableControlExecuteCmdInner(string dev_name, string cmd_recv_code, string cmd_name, string requestStr, CData& oResult, char* cResult);
	bool UartDelTableControlCameraCmdInner(string dev_name, string cmd_name, string requestStr, CData& oResult, char* cResult);

	bool UartSearchTableControlCmdInner(string dev_name, string requestStr, CData& oResult, char* cResult);
	bool UartSearchTableControlExecuteCmdInner( string dev_name, string cmd_recv_code, string requestStr, CData& oResult, char* cResult);
	bool UartSearchCameraTableControlCmdInner( string dev_name, string requestStr, CData& oResult, char* cResult);

	bool UartAlterTableControlCmdInner(string dev_name, string cmd_recv_code, string new_cmd_recv_code, string cmd_fun_des, string requestStr, CData& oResult, char* cResult);
	bool UartAlterTableControlExecuteCmdInner(string dev_name, string cmd_recv_code, int cmd_type, string data_buf, int protocol_type, string cmd_name, string cmd_name_new, string requestStr, CData& oResult, char* cResult);
	bool UartAlterTableControlCameraCmdInner(string dev_name, int protocol_type, string cmd_name, string cmd_name_new, int cmd_protocol_type, int cmd_holder_addr,string cmd_correlation_vi, string requestStr, CData& oResult, char* cResult);

	bool UartPTZControlInner(string cmd_name,	   int move_spped, int control_type, string requestStr, CData& oResult, char* cResult);
	bool UartCameraPresetControlInner( string cmd_name, int preset_num, int control_type, string requestStr, CData& oResult, char* cResult);
	bool GetUartHttpUrlInner(CData &oResult, char *cResult);
	bool SetGPIOConfigInner(int channel, int Value, string requestStr, CData &oResult, char *cResult);

private:
    //待控制设备ip,端口号,超时时间
    string _ip;
    int _port;
    int _timeOut;

    MPSClient *_client;
    CAnalyzeParaProc _AnalyParas;

    //用于存储结果
    cJSON *_result;
    cJSON *_total;
    cJSON *_addressList;
};

#endif // CGETHOSTCHNINFO_H
