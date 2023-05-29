#include "CGetHostStatus.h"
#include "MPSClient.h"
#include "config.h"
#include <boost/lexical_cast.hpp>

#include<stdio.h>
#include <sys/vfs.h>  //mickeybear 2016.03.07 ADD
#include <sys/statvfs.h>

/********************************************************
 * namespace
 *******************************************************/
using namespace std;
using namespace SVS;

static int callback(void *NotUsed,int argc,char **argv,char **azColName);

CData oSqliteDBVector = SVSVECTOR();

GetHostStatus::GetHostStatus(string dbPath)
    :_dbPath(dbPath)
    ,_diskUsedPct(0)
    ,_diskAlarm(false)
    ,_localPath("/mnt/workdir")
    ,_diskAlarmThreshold(10)
    ,_chStatus(eCHStatusSize)
    ,_result(cJSON_CreateObject())
{

}

InterfaceResCode GetHostStatus::GetVersion(string &sResult)
{
    if(_result == NULL){
        mLogError("cJSON result == NULL.");
        return eInterfaceResCodeError;
    }
    cJSON_AddItemToObject(_result,"version"   ,cJSON_CreateString(__VERSION_));
    cJSON_AddItemToObject(_result,"date"      ,cJSON_CreateString(__DATE_));
    string compileTime = __DATE__;
    compileTime = compileTime+" "+__TIME__;
    cJSON_AddItemToObject(_result,"Compile"   ,cJSON_CreateString(compileTime.c_str()));
    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

InterfaceResCode GetHostStatus::GethostStatus(int chNum, string &sResult)
{
    mLogInfo("获取主机状态...");

    if(chNum < 1){
        mLogError("Failed params:chNum="<<chNum<<" < 1.");
        return eInterfaceResCodeRequErrorParam;
    }
    chNum = chNum -1;
    mLogDebug("Convert chNum, convert scope[1,4] => scope[0,3].");

    CData oResult = SVSMAP();
    if(GetStatus(chNum,_chStatus)== false){
        mLogError("Failed to run  GetStatus(chNum,...)=("<<chNum<<",...).");
        return eInterfaceResCodeError;
    }

    if(GetMediaNowKeyID(chNum,_keyID)== false){
        mLogError("Failed to run  GetMediaNowKeyIDchNum,...)=("<<chNum<<",...).");
        return eInterfaceResCodeError;
    }

    if(GetMPSConfigFromFile(oResult) == false){
        mLogError("Failed to run  GetMPSConfigFromFile(...)");
        return eInterfaceResCodeError;
    }
    if(getLocalPath(oResult,_localPath) == false){
        mLogError("Failed to run  getLocalPath(...)");
        return eInterfaceResCodeError;
    }
    if(getDiskAlarm(oResult,_diskAlarmThreshold) == false){
        mLogError("Failed to run  getDiskAlarm(...)");
        return eInterfaceResCodeError;
    }

    if(calcDiskUsedPctAlarm() == false){
        mLogError("Failed to run  calcDiskUsedPctAlarm(...)");
        return eInterfaceResCodeError;
    }

    if(AddRootNode() == false){
        mLogError("Failed to run  AddRootNode(...)");
        return eInterfaceResCodeError;
    }
    sResult = ComposeResult();

    return eInterfaceResCodeSuccess;
}

bool GetHostStatus::GetStatus(int chNum, CHStatus &chState)
{
    MPSMediaStatus status;
    if(GetMediaStatus(chNum,status) != true){
        mLogError("Failed to run GetMediaStatus(...)");
        return false;
    }

    //转换状态值
    chState = CovertMediaStatus(status);
    if(chState >= eCHStatusSize){
        //转换状态值失败
        mLogError("CovertMediadStatus(MPSMediaStatus)=("<<status<<") error. chState:"<<chState);
        return false;
    }
    return true;
}

string GetHostStatus::ComposeResult()
{
    char *cRes = cJSON_Print(_result);
    string res = cRes;
    return res;
}

bool GetHostStatus::AddRootNode()
{
    if(_result == NULL){
        mLogError("cJSON result == NULL.");
        return false;
    }
    cJSON_AddItemToObject(_result,HOST_STATUS_RES_CH_STATUS        ,cJSON_CreateNumber(_chStatus));
    cJSON_AddItemToObject(_result,HOST_STATUS_RES_CH_CASEID        ,cJSON_CreateString(_keyID.c_str()));
    cJSON_AddItemToObject(_result,HOST_STATUS_RES_DISK_USED_PCT    ,cJSON_CreateNumber(_diskUsedPct));
    cJSON_AddItemToObject(_result,HOST_STATUS_RES_DISK_ALARM       ,cJSON_CreateBool(_diskAlarm));
    return true;
}

bool GetHostStatus::GetMediaStatus(int chNum, MPSMediaStatus &status)
{
    sqlite3 *db = NULL;
    char *zErrMsg = 0;
    const char* data = "Callback function called";
    int dbRes = sqlite3_open(_dbPath.c_str(),&db);
    if(dbRes != SQLITE_OK)
    {
        mLogError("Failed to run sqlite3_open(path,...)=("<<_dbPath<<",...).");
        sqlite3_close(db);
        return false;
    }

    oSqliteDBVector = SVSVECTOR();
    string sql;
    try{
        string sChNum = boost::lexical_cast<string>(chNum);
        sql = "SELECT status FROM channel WHERE id='"+sChNum+"';";
    }catch(...){
        mLogError("Failed to try boost::lexical_cast<string>(chNum[channel scope[0,3]])=("<<chNum<<").");
        return false;
    }
    dbRes = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);
    if( dbRes != SQLITE_OK ){
        mLogError("sqlite3_exec(...,sqlCmd,...,...,errorMSg)=(...,"<<sql<<",...,...,"<<zErrMsg<<").");
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return false;
    }else{
        mLogInfo("sqlite3_exec(...,sqlCmd,...)=(...,"<<sql<<",...) success![channel scope[0,3]");
    }
    return analyzeGetStatusSqlRes(status);
}

bool GetHostStatus::GetMediaNowKeyID(int chNum,string &keyID)
{
    sqlite3 *db = NULL;
    char *zErrMsg = 0;
    const char* data = "Callback function called";
    int dbRes = sqlite3_open(_dbPath.c_str(),&db);
    if(dbRes != SQLITE_OK)
    {
        mLogError("Failed to run sqlite3_open(path,...)=("<<_dbPath<<",...).");
        sqlite3_close(db);
        return false;
    }

    oSqliteDBVector = SVSVECTOR();
    string sql;
    try{
        string sChNum = boost::lexical_cast<string>(chNum);
        sql = "SELECT keyid FROM channel WHERE id='"+sChNum+"';";
    }catch(...){
        mLogError("Failed to try boost::lexical_cast<string>(chNum)=("<<chNum<<").");
        return false;
    }
    dbRes = sqlite3_exec(db, sql.c_str(), callback, (void*)data, &zErrMsg);
    if( dbRes != SQLITE_OK ){
        mLogError("sqlite3_exec(...,sqlCmd,...,...,errorMSg)=(...,"<<sql<<",...,...,"<<zErrMsg<<").");
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return false;
    }
    return analyzeGetKeyIdSqlRes(keyID);
}

bool GetHostStatus::GetMPSConfigFromFile(CData &oConfig, string filePath)
{
    mLogInfo("GetMPSConfig(...,filePath)=(...,"<<filePath<<")");
    try{
        if(CData::FromFile(oConfig,filePath) == SVSTRUE){
            string strACSID;
            try{
                strACSID = oConfig.RefMap()["ACS"].RefMap()["ACSID"].RefString();
                mLogDebug("get ACS-ACSID:"<<strACSID);
                return true;
            }catch(exception &e){
                try{
                    strACSID = oConfig.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["ACS"].RefMap()["ACSID"].RefString();
                    mLogDebug("get ACS-ACSID:"<<strACSID);
                    return true;
                }catch(exception &e){
                    mLogError("Failed to try CData::FromFile() get node ACS-ACSID.e:"<<e.what());
                    return false;
                }
            }
            return true;
        }else{
            return GetMPSConfigFromSocket(oConfig);
        }
    }catch(exception &e){
        mLogError("Failed to try CData::FromFile(...,path)=(...,"<<filePath<<")");
        return false;
    }
}

bool GetHostStatus::GetMPSConfigFromSocket(CData &oConfig)
{
    mLogDebug("run GetMPSConfigFromSocket(...)");
    //获取状态
//    CData oResult = SVSMAP();
    MPSClient *_client = new MPSClient(MPS_SOCKET_IP);
    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfig(opResCode,oConfig);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }else if(opResCode != eMPSResultOK){
        mLogError("GetMPSConfigFromSocket() mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return false;
    }

    string strACSID;
    try{
        strACSID = oConfig.RefMap()["ACS"].RefMap()["ACSID"].RefString();
        mLogDebug("get ACS-ACSID:"<<strACSID);
        return true;
    }catch(exception &e){
        try{
            strACSID = oConfig.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["ACS"].RefMap()["ACSID"].RefString();
            mLogDebug("get ACS-ACSID:"<<strACSID);
            return true;
        }catch(exception &e){
            mLogError("Failed to try GetMPSConfigFromSocket() get node ACS-ACSID.e:"<<e.what());
            return false;
        }
    }
}

bool GetHostStatus::getLocalPath(CData &oConfig, string &localPath)
{
    try{
        localPath = oConfig.RefMap()["store"].RefMap()["localPath"].RefString();
        mLogDebug("get store-localPath:"<<localPath);
        return true;
    }catch(exception &e){
        try{
            localPath = oConfig.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["store"].RefMap()["localPath"].RefString();
            mLogDebug("get store-localPath:"<<localPath);
            return true;
        }catch(exception &e){
            mLogError("Failed to try getLocalPath() get node store-localPath.e:"<<e.what());
            return false;
        }
    }
}

bool GetHostStatus::getDiskAlarm(CData &oConfig, SVSLONG32 &diskAlarm)
{
    try{
        diskAlarm = oConfig.RefMap()["system"].RefMap()["diskAlarm"].RefLong32();
        mLogDebug("get system-diskAlarm:"<<diskAlarm);
        return true;
    }catch(exception &e){
        try{
            diskAlarm = oConfig.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["system"].RefMap()["diskAlarm"].RefLong32();
            mLogDebug("get system-diskAlarm:"<<diskAlarm);
            return true;
        }catch(exception &e){
            mLogError("Failed to try getDiskAlarm() get node system-diskAlarm.e:"<<e.what());
            return false;
        }
    }
}

bool GetHostStatus::calcDiskUsedPctAlarm()
{
    struct statfs vfs;
    string _folder = _localPath;
    int ret = statfs(_folder.c_str(), &vfs);
    if (ret != 0){
        mLogError("Failed to run statfs(path,...)=("<<_folder<<",...)");
        return false;
    }
    unsigned long freespace = vfs.f_bfree/1024*vfs.f_bsize/Msize;
    if (freespace < _diskAlarmThreshold){
        mLogWarn("freespace:"<<freespace<<" < threshold:"<<_diskAlarmThreshold<<",disk alarm = true");
        _diskAlarm = true;
    }else{
        _diskAlarm = false;
    }
    if (vfs.f_blocks == 0){
        mLogError("Failed ,statfs::f_blocks == 0");
        return false;
    }

    _diskUsedPct = 100.0 - (double)vfs.f_bfree/(double)vfs.f_blocks*100.0;
    mLogDebug("disk used pct:"<<_diskUsedPct);
    return true;
}

bool GetHostStatus::analyzeGetStatusSqlRes(MPSMediaStatus &status)
{
    try{
        int oSize = oSqliteDBVector.RefVector().size();
        if(oSize <= 0){
            mLogError("sqlRes.size() <= 0");
            return false;
        }
        string sStatus = oSqliteDBVector.RefVector().at(0).RefMap()["status"].RefString();
        int iStatus = atoi(sStatus.c_str());
        mLogDebug("sqlRes str status="<<sStatus<<", status="<<iStatus);
        if(iStatus >= eMPSStatusSize){
            mLogError("statsus:"<<iStatus<<" >= eMPSStatusSize:"<<eMPSStatusSize);
            return false;
        }
        status = (MPSMediaStatus)iStatus;
        return true;
    }catch(exception &e){
        mLogError("analyzeGetStatusSqlRes() Failed to try."<<e.what());
        return false;
    }
}

bool GetHostStatus::analyzeGetKeyIdSqlRes(string &keyId)
{
    try{
        int oSize = oSqliteDBVector.RefVector().size();
        if(oSize <= 0){
            mLogError("sqlRes.size() <= 0");
            return false;
        }
        keyId = oSqliteDBVector.RefVector().at(0).RefMap()["keyid"].RefString();
        mLogDebug("sqlRes str keyid="<<keyId);

        return true;
    }catch(exception &e){
        mLogWarn("analyzeGetKeyIdSqlRes() Failed to try."<<e.what());
        keyId = "";
        return true;
    }
}

/********************************************************
 * void
 *******************************************************/
static int callback(void *NotUsed,int argc,char **argv,char **azColName)
{
    int i;
    CData oParams = SVSMAP();
    for(i=0; i<argc; i++){
        mLogDebug("db ["<<i<<"] "<<azColName[i]<<" = "<<(argv[i] ? argv[i] : "NULL"));
        oParams.RefMap()[azColName[i]]=(SVSSTRING)(argv[i] ? argv[i] : "NULL");
    }
    oSqliteDBVector.RefVector().push_back(oParams);
    return 0;
}

