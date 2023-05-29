#include "GetTaskContent.h"
#include <boost/lexical_cast.hpp>

CGetTaskContent::CGetTaskContent(string ip, int port, int timeOut)
    :_videoFiles(cJSON_CreateArray())
    ,_result(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
{

}

InterfaceResCode CGetTaskContent::GetTaskContent(string caseId, cJSON *result)
{
    mLogInfo("查询设备指定案件号文件列表...");
    mLogInfo("caseID:"<<caseId);
    //TODO:无法返回正确的result
    if(GetSelfMsg(_selfIp,_fileRootPath)== false){
        mLogError("Failed to run  GetSelfMsg(...)");
        return eInterfaceResCodeError;
    }
    MPSOperationRes opResCode = eMPSResultOK;
    CData oResult = SVSMAP();
    ResponseCode resCode = _client->GetPackageFile(caseId,opResCode,oResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetPackageFile(caseId,...)=("<<caseId<<",...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }
    cJSON *videoFiles;// = cJSON_CreateArray();
    if(ConvertPackageCDATA2CJSON(oResult,videoFiles) == false){
        mLogError("Failed to run  ConvertPackageCDATA2CJSON(...)");
        return eInterfaceResCodeError;
    }
    //    videoFiles = _videoFiles;
    //    mLogError("cJSON videoFiles:"<<cJSON_Print(videoFiles));
    if(ComposeResult(result) == false){
        mLogError("Failed to run  ComposeResult(...)");
        return eInterfaceResCodeError;
    }

//    mLogError("cJSON result:"<<cJSON_Print(result));
//    mLogError("cJSON _result:"<<cJSON_Print(_result));

    return eInterfaceResCodeSuccess;
}

InterfaceResCode CGetTaskContent::GetTaskContent(string caseId, string &sResult)
{
    cJSON * result;
    InterfaceResCode resCode;
    resCode = GetTaskContent(caseId,result);
    sResult = getSResult();
    return resCode;
}

cJSON *CGetTaskContent::getCResult()
{
    return this->_result;
}

string CGetTaskContent::getSResult()
{
    char *cbuf = cJSON_PrintUnformatted(_result);
    return cbuf;
}

bool CGetTaskContent::GetSelfMsg(string &ip, string &fileRootPath)
{
    mLogDebug("run GetSelfIp(...)");
    //获取状态
    CData oResult = SVSMAP();
    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetSysConfig(opResCode,oResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetSysConfig(...) error:"<<resCode);
        return false;
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return false;
    }

    try{
        ip = oResult.RefMap()["network"].RefMap()["ip"].RefString();
        fileRootPath = oResult.RefMap()["store"].RefMap()["localPath"].RefString();
        mLogDebug("get self ip:"<<ip);
        mLogDebug("get self file root path:"<<fileRootPath);
        return true;
    }catch(exception &e){
        try{
            ip = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["network"].RefMap()["ip"].RefString();
            fileRootPath = oResult.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["store"].RefMap()["localPath"].RefString();
            mLogDebug("get self ip:"<<ip);
            mLogDebug("get self file root path:"<<fileRootPath);
            return true;
        }catch(exception &e){
            mLogError("Failed to try get node network-ip."<<e.what());
            return false;
        }
    }
}

bool CGetTaskContent::ConvertPackageCDATA2CJSON(CData &oResult, cJSON *cResult)
{
    CDataPackageNode packageData;
    CDataVideoPrimaryNode videoData;
    CData oVideoNode = SVSMAP();
    CData oPackageNode = SVSMAP();
    if(!getCDATAPackageNode(oResult,oPackageNode)){
        mLogError("Failed to run getCDATAPackageNode(CData,CData).");
        return false;
    }
    if(!analyzePackageCDataNodePackage(oPackageNode,&packageData)){
        mLogError("Failed to run analyzePackageCDataNodePackage(CData,PCDataPackageNode).");
        return false;
    }

    if(!getPackageCDataVideoVectorNode(oResult,oVideoNode)){
        mLogError("Failed to run getPackageCDataVideoVectorNode(CData,CData).");
        return false;
    }
    int videoNodeSize = oVideoNode.RefVector().size();
    mLogDebug("get video size:"<<videoNodeSize);
    //TODO
    //待添加 packageData信息
    //待合成VideoFileNode
    for (std::size_t i = 0; i < oVideoNode.RefVector().size();i++)
    {
        mLogDebug("add num i:"<<(int)i);
        SVSVECTOR& filesMain   = oVideoNode.RefVector().at(i).RefMap()["primary"].RefVector();
//        for (SVSVECTOR::iterator it = filesMain.begin(); it != filesMain.end(); it++)
        for(std::size_t j = 0; j < filesMain.size();j++)
        {
            CData oVideoFile = filesMain.at(j);
            //打印返回信息
            try{
                string sResult;
                if(!CData::ToCustomXMLString(oVideoFile.RefMap(),sResult)){
                    mLogDebug("Convert xml data to xml string failed.");
                }
                mLogDebug(" sResult:\r\n"<<sResult);
            }catch(exception& e){
                mLogDebug("ToCustomXMLString(oResult.RefMap(),...) error."<<e.what());
            }
            mLogDebug("add num j: "<<(int)j);
            if(!analyzePackageCDataNodeVideo(oVideoFile,(int)i,(int)j,&videoData)){
                mLogError("Failed to run analyzePackageCDataNodeVideo(CData,chNum,seqNum,...)=(CData,"<<i<<","<<j<<".");
                return false;
            }else{
                VideoFileNode videoFileNode;
                convertCData2VideoFileNode(&packageData,&videoData,videoFileNode);
                if(!AddVideoFiles(videoFileNode)){
                    mLogError("Failed to run AddVideoFiles(videoData).");
                    return false;
                }
            }

//            if(it->RefMap()["fileSize"].RefLong64()==0)
//            {
//                fpath=(boost::format("/mnt/workdir/store/%s/%s/%d/%s") % _package._data.RefMap()["month"].RefString() % _package._data.RefMap()["keyID"].RefString() % i % it->RefMap()["fileName"].RefString()).str();
//                it->RefMap()["fileSize"]=(SVSLONG64)boost::filesystem::file_size(fpath);;
//            }

           // (unsigned long long)it->RefMap()["fileSize"].RefLong64();
        }
    }
    if(!AddRootNode(packageData)){
        mLogError("Failed to run AddRootNode().");
        return false;
    }
    cResult = _videoFiles;
//    mLogDebug("AddVideoFiles number:"<<videoNodeSize);
//    mLogInfo("_videoFiles :"<<cJSON_Print(cResult));
    return true;
}

bool CGetTaskContent::ComposeResult(cJSON * result)
{
    if(_result == NULL){
        mLogError("cJSON _result == NULL.");
        result = NULL;
        return false;
    }
//    int totalVideoFiles = 0;
//    if(_videoFiles == NULL){
//        mLogWarn("cJSON videoFiles == NULL.totalVideoFiles = 0.");
//        totalVideoFiles = 0;
//    }else{
//        totalVideoFiles = cJSON_GetArraySize(_videoFiles);
//    }
//    mLogDebug("get videoFiles size="<<totalVideoFiles);
//    //result中添加节点
//    cJSON_AddItemToObject(_result,GET_TESK_CONTENTS_RES_TOTAL,cJSON_CreateNumber(totalVideoFiles));
//    cJSON_AddItemToObject(_result,GET_TESK_CONTENTS_RES_VIDEO_FILES,_videoFiles);
    result = _result;
    return true;
}

string CGetTaskContent::ComposeResult()
{
    cJSON * result;
    if(ComposeResult(result) == false){
        return "";
    }else{
        char *cRes = cJSON_Print(result);
        string res = cRes;
        return res;
    }
}

bool CGetTaskContent::AddVideoFiles(VideoFileNode &videoFile)
{
    if(_videoFiles == NULL){
        mLogError("cJSON videoFiles == NULL.");
        return false;
    }
    int videoFileSize = cJSON_GetArraySize(_videoFiles);
    videoFile.fileSort = videoFileSize+1;
    cJSON * videoFileArrayObj = cJSON_CreateObject();
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_CASEID     ,cJSON_CreateString(videoFile.caseId.c_str()));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_FILENAME   ,cJSON_CreateString(videoFile.fileName.c_str()));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_SIZE       ,cJSON_CreateNumber(videoFile.size));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_FILEPATH   ,cJSON_CreateString(videoFile.filePath.c_str()));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_STARTTIME  ,cJSON_CreateString(videoFile.startTime.c_str()));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_ENDTIME    ,cJSON_CreateString(videoFile.endTime.c_str()));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_CH_NUM     ,cJSON_CreateNumber(videoFile.channelNumber));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_CH_DESC    ,cJSON_CreateString(videoFile.channelDesc.c_str()));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_FTPPATH    ,cJSON_CreateString(videoFile.ftpPath.c_str()));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_VODURL_RTSP,cJSON_CreateString(videoFile.vodurlRtsp.c_str()));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_VODURL_RTMP,cJSON_CreateString(videoFile.vodurlRtmp.c_str()));
    cJSON_AddItemToObject(videoFileArrayObj,GET_TESK_CONTENTS_RES_VF_FILESORT   ,cJSON_CreateNumber(videoFile.fileSort));
    cJSON_AddItemToArray(_videoFiles,videoFileArrayObj);
    return true;
}

bool CGetTaskContent::AddRootNode(CDataPackageNode &packageNode)
{
    int totalVideoFiles = 0;
    if(_videoFiles == NULL){
        mLogWarn("cJSON videoFiles == NULL.totalVideoFiles = 0.");
        totalVideoFiles = 0;
    }else{
        totalVideoFiles = cJSON_GetArraySize(_videoFiles);
    }
    mLogDebug("get videoFiles size="<<totalVideoFiles);
    //result中添加节点
    cJSON_AddItemToObject(_result,GET_TESK_CONTENTS_RES_ALLOWDELETE,cJSON_CreateBool(packageNode.allowDelete));
    cJSON_AddItemToObject(_result,GET_TESK_CONTENTS_RES_TOTAL,cJSON_CreateNumber(totalVideoFiles));
    cJSON_AddItemToObject(_result,GET_TESK_CONTENTS_RES_VIDEO_FILES,_videoFiles);
    return true;
}

bool CGetTaskContent::getPackageCDataVideoVectorNode(CData &result, CData &videoNode)
{
    try{
        videoNode = result.RefMap()["package"].RefMap()["video"];
        return true;
    }catch(exception &e){
        try{
            videoNode = result.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["package"].RefMap()["video"];
            return true;
        }catch(exception &e){
            mLogError("Failed to try getPackageCDataVideoVectorNode(CData,CData),e:"<<e.what());
            return false;
        }
    }
}

bool CGetTaskContent::getCDATAPackageNode(CData &result, CData &packageNode)
{
    try{
        packageNode = result.RefMap()["package"].RefMap();
        return true;
    }catch(exception &e){
        try{
            packageNode = result.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["package"].RefMap();
            return true;
        }catch(exception &e){
            mLogError("Failed to try getCDATAPackageNode(CData,CData),e:"<<e.what());
            return false;
        }
    }
}

bool CGetTaskContent::analyzePackageCDataNodePackage(CData &package, PCDataPackageNode packageNode)
{
    //打印返回信息
//    try{
//        string sResult;
//        if(!CData::ToCustomXMLString(package.RefMap(),sResult)){
//            mLogDebug("Convert xml data to xml string failed.");
//        }
//        mLogInfo("处理(添加节点)后 sResult:\r\n"<<sResult);
//    }catch(exception& e){
//        mLogInfo("ToCustomXMLString(oResult.RefMap(),...) error."<<e.what());
//    }
    try{
        packageNode->keyID      = package.RefMap()["keyID"      ].RefString();
        packageNode->title      = package.RefMap()["title"      ].RefString();
//        packageNode->worknumber = package.RefMap()["worknumber" ].RefLong16();
        packageNode->startTime  = package.RefMap()["startTime"  ].RefString();
        packageNode->endTime    = package.RefMap()["endTime"    ].RefString();
        packageNode->month      = package.RefMap()["month"      ].RefString();
        packageNode->status     = package.RefMap()["status"     ].RefLong16();
        packageNode->allowDelete= package.RefMap()["allowDelete"].RefBool();
        mLogDebug("keyID:"         <<packageNode->keyID      );
        mLogDebug("title:"         <<packageNode->title      );
//        mLogDebug("worknumber:"    <<packageNode->worknumber );
        mLogDebug("startTime:"     <<packageNode->startTime  );
        mLogDebug("endTime:"       <<packageNode->endTime    );
        mLogDebug("month:"         <<packageNode->month      );
        mLogDebug("status:"        <<packageNode->status     );
        mLogDebug("allowDelete:"   <<packageNode->allowDelete);
        return true;
    }catch(exception &e){
        mLogError("Failed to try analyzePackageCDataNodePackage(CData,PCDataPackageNode),e:"<<e.what());
        return false;
    }
}

bool CGetTaskContent::analyzePackageCDataNodeVideo(CData &video,int chNum,int seqNum, PCDataVideoPrimaryNode videoNode)
{
    try{
        videoNode->fileName     = video.RefMap()["fileName"     ].RefString();
        videoNode->fileSize     = video.RefMap()["fileSize"     ].RefLong64();
        videoNode->fileType     = video.RefMap()["fileType"     ].RefLong16();
        videoNode->rate         = video.RefMap()["rate"         ].RefLong32();
        videoNode->resolution   = video.RefMap()["resolution"   ].RefLong16();
        videoNode->startTime    = video.RefMap()["startTime"    ].RefString();
        videoNode->duration     = video.RefMap()["duration"     ].RefLong32();
        videoNode->chNum        = chNum;
        videoNode->seqNum       = seqNum;
        mLogDebug("fileName:"   <<videoNode->fileName);
        mLogDebug("fileSize:"   <<videoNode->fileSize);
        mLogDebug("fileType:"   <<videoNode->fileType);
        mLogDebug("rate:"       <<videoNode->rate);
        mLogDebug("resolution:" <<videoNode->resolution);
        mLogDebug("startTime:"  <<videoNode->startTime);
        mLogDebug("duration:"   <<videoNode->duration);
        mLogDebug("chNum:"      <<videoNode->chNum);
        mLogDebug("seqNum:"     <<videoNode->seqNum);
        return true;
    }catch(exception &e){
        mLogError("Failed to try analyzePackageCDataNodeVideo(CData,PCDataVideoPrimaryNode), e:"<<e.what());
        return false;
    }
}

void CGetTaskContent::convertCData2VideoFileNode(PCDataPackageNode packageNode, PCDataVideoPrimaryNode videoNode, VideoFileNode &videoFile)
{
    try{
        videoFile.caseId = packageNode->keyID;
        videoFile.fileName = videoNode->fileName;
        videoFile.size = videoNode->fileSize/1024.00/1024.00/1024.00;
        string fileRootPath = FILE_ROOT_PATH;
        videoFile.filePath = _fileRootPath+fileRootPath+"/" +
                             packageNode->month+"/" +
                             packageNode->keyID+"/" +
                             boost::lexical_cast<string>(videoNode->chNum)+"/" +
                             videoNode->fileName;
        videoFile.startTime = videoNode->startTime;
        //end time
        time_t startTime = string2Datetime(videoNode->startTime);
        time_t endTime = startTime + videoNode->duration;
        string strEndTime = datetime2String(endTime);
        videoFile.endTime = strEndTime;
        videoFile.channelNumber = videoNode->chNum;
        videoFile.channelDesc = "";
        string ftp = FTP_ROOT_PATH;
        videoFile.ftpPath = ftp+_selfIp+fileRootPath+"/" +packageNode->month+"/" +packageNode->keyID+"/" +boost::lexical_cast<string>(videoNode->chNum)+"/" +videoNode->fileName;;
        videoFile.vodurlRtsp = "";

        string rtmpRootPath = "rtmp://";
        videoFile.vodurlRtmp = rtmpRootPath +_selfIp +"/vod//" +packageNode->month+"/" +packageNode->keyID+"/" +boost::lexical_cast<string>(videoNode->chNum)+"/" +videoNode->fileName;
        videoFile.fileSort = videoNode->seqNum;
    }catch(exception &e){
        mLogError("Failed to try convertCData2VideoFileNode(PCDataPackageNode,PCDataVideoPrimaryNode,VideoFileNode), e:"<<e.what());
    }
}

time_t CGetTaskContent::string2Datetime(string str)
{
    char *cha = (char*)str.data();             // 将string转换成char*。
    tm tm_;                                    // 定义tm结构体。
    int year, month, day, hour, minute, second;// 定义时间的各个int临时变量。
    sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// 将string存储的日期时间，转换为int临时变量。
    tm_.tm_year = year - 1900;                 // 年，由于tm结构体存储的是从1900年开始的时间，所以tm_year为int临时变量减去1900。
    tm_.tm_mon = month - 1;                    // 月，由于tm结构体的月份存储范围为0-11，所以tm_mon为int临时变量减去1。
    tm_.tm_mday = day;                         // 日。
    tm_.tm_hour = hour;                        // 时。
    tm_.tm_min = minute;                       // 分。
    tm_.tm_sec = second;                       // 秒。
    tm_.tm_isdst = 0;                          // 非夏令时。
    time_t t_ = mktime(&tm_);                  // 将tm结构体转换成time_t格式。
    return t_;                                 // 返回值。
}

string CGetTaskContent::datetime2String(time_t time)
{
    tm *tm_ = localtime(&time);                // 将time_t格式转换为tm结构体
    int year, month, day, hour, minute, second;// 定义时间的各个int临时变量。
    year = tm_->tm_year + 1900;                // 临时变量，年，由于tm结构体存储的是从1900年开始的时间，所以临时变量int为tm_year加上1900。
    month = tm_->tm_mon + 1;                   // 临时变量，月，由于tm结构体的月份存储范围为0-11，所以临时变量int为tm_mon加上1。
    day = tm_->tm_mday;                        // 临时变量，日。
    hour = tm_->tm_hour;                       // 临时变量，时。
    minute = tm_->tm_min;                      // 临时变量，分。
    second = tm_->tm_sec;                      // 临时变量，秒。
    char yearStr[5], monthStr[3], dayStr[3], hourStr[3], minuteStr[3], secondStr[3];// 定义时间的各个char*变量。
    sprintf(yearStr, "%04d", year);              // 年。
    sprintf(monthStr, "%02d", month);            // 月。
    sprintf(dayStr, "%02d", day);                // 日。
    sprintf(hourStr, "%02d", hour);              // 时。
    sprintf(minuteStr, "%02d", minute);          // 分。
    if (minuteStr[1] == '\0')                  // 如果分为一位，如5，则需要转换字符串为两位，如05。
    {
        minuteStr[2] = '\0';
        minuteStr[1] = minuteStr[0];
        minuteStr[0] = '0';
    }
    sprintf(secondStr, "%02d", second);          // 秒。
    if (secondStr[1] == '\0')                  // 如果秒为一位，如5，则需要转换字符串为两位，如05。
    {
        secondStr[2] = '\0';
        secondStr[1] = secondStr[0];
        secondStr[0] = '0';
    }
    char s[20];                                // 定义总日期时间char*变量。
    sprintf(s, "%s-%s-%s %s:%s:%s", yearStr, monthStr, dayStr, hourStr, minuteStr, secondStr);// 将年月日时分秒合并。
    string str(s);                             // 定义string变量，并将总日期时间char*变量作为构造函数的参数传入。
    return str;                                // 返回转换日期时间后的string变量。
}

