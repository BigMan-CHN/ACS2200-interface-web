#include "CGetChannelContents.h"
#include "common/CTimeCvt.h"

CGetChannelContents::CGetChannelContents(string ip, int port, int timeOut)
    :_caseJNode(cJSON_CreateArray())
    ,_result(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
{

}

InterfaceResCode CGetChannelContents::GetChannelContents(string startTime, string endTime)
{
    mLogInfo("获取指定通道时间段内内容列表...");

    if(!checkTimeStr(startTime)){
        mLogWarn("startTime("<<startTime<<") error. e.g. \"2018-08-08\".");
        startTime = "0000-01-01";
    }
    if(!checkTimeStr(endTime)){
        mLogWarn("endTime("<<endTime<<") error. e.g. \"2018-08-08\".");
        endTime = "9999-12-31";
    }
    _startTime = startTime;
    _endTime = endTime;
    MPSOperationRes opResCode = eMPSResultOK;
    CData oResult = SVSMAP();
    ResponseCode resCode = _client->GetPackageList(opResCode,oResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetPackageList(...)=(...) error:"<<resCode);
        return CovertResponseCode(resCode);
    }else if(opResCode != eMPSResultOK){
        mLogError("mps response flag:"<<opResCode<<","<<getMPSOperationResStr(opResCode));
        return CovertMPSOperationCode(opResCode);
    }

    if(ConvertPackageListCDATA2CJSON(oResult) == false){
        mLogError("Failed to run  ConvertPackageCDATA2CJSON(...)");
        return eInterfaceResCodeError;
    }
    string strResult = ComposeResult();
    if(strResult.length() <= 0){
        mLogError("Failed to run  ComposeResult(.)");
        return eInterfaceResCodeError;
    }

//    mLogError("cJSON result:"<<cJSON_Print(result));
//    mLogError("cJSON _result:"<<cJSON_Print(_result));

    return eInterfaceResCodeSuccess;
}

InterfaceResCode CGetChannelContents::GetChannelContents(string startTime, string endTime, string &sResult)
{
    InterfaceResCode resCode;
    resCode = GetChannelContents(startTime,endTime);
    sResult = getSResult();
    return resCode;
}

cJSON *CGetChannelContents::getCResult()
{
    return this->_result;
}

string CGetChannelContents::getSResult()
{
    char *cBuf = cJSON_PrintUnformatted(this->_result);
    return cBuf;
}

bool CGetChannelContents::checkTimeStr(string timeStr)
{
    if(timeStr.length() < GET_CHANNLE_CONTENTS_TIME_STR_LEN){
        mLogError("timeStr.length() < "<<GET_CHANNLE_CONTENTS_TIME_STR_LEN<<",timeStr e.g. \"2018-08-08\".");
        return false;
    }
    CTimeCvt timeCvt;
    time_t t;
    if(!timeCvt.string2Datetime(timeStr,t)){
        mLogError("timeStr("<<timeStr<<") error. e.g. \"2018-08-08\".");
        return false;
    }
    return true;
}

bool CGetChannelContents::ConvertPackageListCDATA2CJSON(CData &oResult)
{
    CData oPackageListNode = SVSMAP();
    if(!getCDataPackageListNode(oResult,oPackageListNode)){
        mLogError("Failed to run getCDataPackageListNode(CData,CData).");
        return false;
    }

    int i = 0;
    for (SVSMAP::iterator it = oPackageListNode.RefMap().begin(); it != oPackageListNode.RefMap().end(); it++)
    {
        mLogDebug("add num i:"<<i<<", it->first:"<<it->first);
        CData opPackage = it->second;

        CaseNode caseData;
        if(!analyzePackageListCDataNodeCase(opPackage,&caseData)){
            mLogError("Failed to run analyzePackageListCDataNodeCase(CData,...)=(CData,...).");
            return false;
        }else{
//            VideoFileNode videoFileNode;
//            convertCData2VideoFileNode(&packageData,&videoData,videoFileNode);
            if(!checkCaseNodeTime(caseData)){
                mLogWarn("case:"<<caseData.caseId<<" startTime:"<<caseData.startTime<<" not in scope["<<_startTime<<","<<_endTime<<"].");
                continue;
            }
            if(!AddCaseJNode(caseData)){
                mLogError("Failed to run AddCaseJNode(CaseNode).");
                return false;
            }
        }
        i++;
    }
    return true;
}

bool CGetChannelContents::getCDataPackageListNode(CData &result, CData &caseNode)
{try{
        caseNode = result.RefMap()["packageList"].RefMap();
        return true;
    }catch(exception &e){
        try{
            caseNode = result.RefMap()[SKYVIS_TRANSMIT_RESPONSE].RefMap()["packageList"].RefMap();
            return true;
        }catch(exception &e){
            mLogError("Failed to try getCDataPackageListNode(CData,CData),e:"<<e.what());
            return false;
        }
    }
}

bool CGetChannelContents::analyzePackageListCDataNodeCase(CData &packageList, PCaseNode caseNode)
{
    try{
        caseNode->caseId     = packageList.RefMap()["keyID"      ].RefString();
        caseNode->title      = packageList.RefMap()["title"      ].RefString();
        caseNode->status     = packageList.RefMap()["status"     ].RefLong16();
        caseNode->allowDelete= packageList.RefMap()["allowDelete"].RefBool();
        caseNode->month      = packageList.RefMap()["month"      ].RefString();
        caseNode->startTime  = packageList.RefMap()["startTime"  ].RefString();
        caseNode->endTime    = packageList.RefMap()["endTime"    ].RefString();
        mLogDebug("keyID:"         <<caseNode->caseId     );
        mLogDebug("title:"         <<caseNode->title      );
        mLogDebug("status:"        <<caseNode->status     );
        mLogDebug("allowDelete:"   <<caseNode->allowDelete);
        mLogDebug("month:"         <<caseNode->month      );
        mLogDebug("startTime:"     <<caseNode->startTime  );
        mLogDebug("endTime:"       <<caseNode->endTime    );
        return true;
    }catch(exception &e){
        mLogError("Failed to try analyzePackageListCDataNodeCase(CData,PCaseNode),e:"<<e.what());
        return false;
    }
}

bool CGetChannelContents::checkCaseNodeTime(CaseNode &caseNode)
{
    if(caseNode.startTime.length() <= 0){
        //正在录制
        return true;
    }
    if(strncmp(_startTime.c_str(),caseNode.startTime.c_str(),GET_CHANNLE_CONTENTS_TIME_STR_LEN) > 0){
        return false;
    }
    if(strncmp(_endTime.c_str(),caseNode.startTime.c_str(),GET_CHANNLE_CONTENTS_TIME_STR_LEN) < 0){
        return false;
    }
    return true;
}

bool CGetChannelContents::AddCaseJNode(CaseNode &caseNode)
{
    if(_caseJNode == NULL){
        mLogError("cJSON caseJNode == NULL.");
        return false;
    }

    cJSON * caseArrayObj = cJSON_CreateObject();
    cJSON_AddItemToObject(caseArrayObj,GET_CHANNEL_CONTENTS_RES_CASEID      ,cJSON_CreateString(caseNode.caseId.c_str()));
    cJSON_AddItemToObject(caseArrayObj,GET_CHANNEL_CONTENTS_RES_TITLE       ,cJSON_CreateString(caseNode.title.c_str()));
    cJSON_AddItemToObject(caseArrayObj,GET_CHANNEL_CONTENTS_RES_STATUS      ,cJSON_CreateNumber(caseNode.status));
//    cJSON_AddItemToObject(caseArrayObj,GET_CHANNEL_CONTENTS_RES_ALLOWDELETE ,cJSON_CreateNumber(caseNode.allowDelete));
    cJSON_AddItemToObject(caseArrayObj,GET_CHANNEL_CONTENTS_RES_ALLOWDELETE ,cJSON_CreateBool(caseNode.allowDelete));
    cJSON_AddItemToObject(caseArrayObj,GET_CHANNEL_CONTENTS_RES_MONTH       ,cJSON_CreateString(caseNode.month.c_str()));
    cJSON_AddItemToObject(caseArrayObj,GET_CHANNEL_CONTENTS_RES_STARTTIME   ,cJSON_CreateString(caseNode.startTime.c_str()));
    cJSON_AddItemToObject(caseArrayObj,GET_CHANNEL_CONTENTS_RES_ENDTIME     ,cJSON_CreateString(caseNode.endTime.c_str()));
    cJSON_AddItemToArray(_caseJNode,caseArrayObj);
    return true;
}

string CGetChannelContents::ComposeResult()
{
    if(_result == NULL){
        mLogError("cJSON _result == NULL.");
        _result = NULL;
        return "";
    }
    int totalCase = 0;
    if(_caseJNode == NULL){
        mLogWarn("cJSON caseJNode == NULL.totalCase = 0.");
        totalCase = 0;
    }else{
        totalCase = cJSON_GetArraySize(_caseJNode);
    }
    mLogDebug("get case size="<<totalCase);
    //result中添加节点
    cJSON_AddItemToObject(_result,GET_CHANNEL_CONTENTS_RES_CASETOTAL,cJSON_CreateNumber(totalCase));
    cJSON_AddItemToObject(_result,GET_CHANNEL_CONTENTS_RES_CASELIST ,_caseJNode);
    char *cBuf = cJSON_PrintUnformatted(_result);
    string str = cBuf;
    return str;
}

