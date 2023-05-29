/********************************************************
 * 名  称：main.cpp
 * 版  本： V1.00
 * 作  者： es716
 * 时  间：
 *
 * 功  能：返回信息直接为json字符串即可，无需头部信息
 *******************************************************/
/********************************************************
 * public c/c++ standard headers
 *******************************************************/
#include <stdio.h>
#include <iostream>
#include <string.h>

/********************************************************
 * CGI headers
 *******************************************************/
//#include <cgi.h>

/********************************************************
 * SQLite3 headers
 *******************************************************/
#include <sqlite3.h>

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

/********************************************************
 * headers
 *******************************************************/
#include "config.h"
#include "cgiAnalysis.h"
#include "interfaceDefines.h"
#include "interfaceParams.h"
#include "interfaceProcess.h"
#include "InterfaceResponse.h"
#include <Logger.h>

using namespace std;
using namespace log4cplus;
using namespace SVS;
using namespace SVS::log;
SVS::log::CLogger SVS::log::theLogger;

mLog m_log;


/********************************************************
* lib addr
* target addr & name
 *******************************************************/
int main(){

    char *inputstring;
    CgiAnalysis cgi;
	
	//获取主机系统时间,添加到日志文件
	time_t timep;
	struct tm *LocalTime;
	char SysTime[80];
	time(&timep);
	LocalTime = std::localtime(&timep);
//	LocalTime->tm_hour = (LocalTime->tm_hour + 8)%24;//UTC时间转换为中国时间

	strftime(SysTime, 80, "%Y-%m-%d %H:%M:%S", LocalTime);
	
    //加载log配置文件
    PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(LOG_PROPERTIES_PATH));
    m_log = mLog(LOG_PROPERTIES_PATH);
    NDC& ndc =  log4cplus::getNDC();

    //获取ip 输入到日志中
    string sNDCip = cgi.getClientIp();
    sprintf(SysTime+strlen(SysTime)," IP: %s",sNDCip.c_str());
	//ndc.push(sNDCip.c_str());
	ndc.push(SysTime);

    //获取请求数据
    inputstring = cgi.GetCgiData();
    string sIn = inputstring;

 
    //初步判断请求数据中是否为debug模式
    InterfaceParams params;
    //分析获取请求数据
    if( !params.analysisParams(sIn) ){
        mLogError("Failed to run params.analysisParams(requestStr)");
    }
    string sDebug;
    if( !params.getParamseNodeofTestDebug(sDebug) ){
        mLogError("get params error node:"<<REQUEST_NODE_METHOD);
        mLogInfo("\r\n          ===== release mode ====="
                 "\r\n(add node:"<<REQUEST_TEST_NODE_DEBUG<<" using debug mode)\r\n\r\n");
    }else{
        PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(LOG_CONSOLE_PROPERTIES_PATH));
        m_log = mLog(LOG_CONSOLE_PROPERTIES_PATH);
        cout<<"Content-type: application/json;charset=UTF-8\n\n"<<endl;
        mLogInfo("\r\n             ==r=== Debug mode Ver"<<__VERSION_<<"====="
                 "\r\n(delete node:"<<REQUEST_TEST_NODE_DEBUG<<" using release mode)\r\n\r\n");
    }
    params.clearAll();


    InterfaceProcess *process = new InterfaceProcess(MPS_SOCKET_IP,MPS_SOCKET_PORT);
    if(sDebug.length() > 0) process->debugMode();
    cout<<"Content-type: application/json;charset=UTF-8\n\n"<<endl;
    //处理请求数据
    cout <<process->processRequest(sIn)<<endl<<endl;


    //移除日志中ndc
    ndc.pop();
    ndc.remove();
    //释放cgi
    cgi.clearCgiData();
    mLogInfo("\r\n--------- complete ---------\r\n");
    return 0;
}
