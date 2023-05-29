#include "InterfaceResponse.h"
#include "interfaceDefines.h"

interfaceResponse::interfaceResponse(bool flag, string error, InterfaceResCode code, cJSON *results)
    :_root(cJSON_CreateObject())
    ,_flag(flag)
    ,_error(error)
    ,_code(code)
    ,_results(results)
{

}

string interfaceResponse::responsePrint(bool format)
{
    this->clearResponse();
    string sRes = "";
    responseAddFlag();
    //error code number
//    responseAddCode();
    responseAddError();
    responseAddResults();

    if(_root == NULL){
        mLogError("_root == NULL");
        sRes = "";
    }else{
        char * pcRes;
        if(format){
            mLogInfo("\r\n ======== \r\n ========");
            pcRes = cJSON_Print(_root);
        }else{
            pcRes = cJSON_PrintUnformatted(_root);
        }
        if(pcRes == NULL){
            mLogError("cJSON_Print(_root) == NULL");
            sRes = "";
        }else{
            sRes = (string)pcRes + "\0";
        }
        free(pcRes);
    }
    mLogDebug("create sRes:"<<sRes);
    return sRes;
}

string interfaceResponse::simpleResponsePrintofSuccess(bool format)
{
    this->clearAllMsg();
    this->setResponseSuccessFlag();
    this->setResponseCode(eInterfaceResCodeSuccess);
    this->setResponseError("");
    this->setResponseResults(NULL);
    return responsePrint(format);
}

string interfaceResponse::simpleResponsePrintofFalied(string error, InterfaceResCode code, bool format)
{
    this->clearAllMsg();
    this->setResponseFaliedFlag();
    this->setResponseCode(code);
    this->setResponseError(error);
    this->setResponseResults(NULL);
    return responsePrint(format);
}

void interfaceResponse::setHostStatusResResults(CHStatus state)
{
    mLogDebug("setHostStatusResponse(CHStatus)=("<<state<<")...");

    cJSON * jNode = cJSON_CreateNumber((double)state);
    if(jNode == NULL){
        mLogError("Failed to run cJSON_CreateNumber((double)state)=("<<((double)state)<<")");
        return ;
    }
    cJSON * node = cJSON_CreateObject();
    if(node == NULL){
        mLogError("_root == NULL");
        return ;
    }
    cJSON_AddItemToObject(node,HOST_STATUS_RES_CH_STATUS,jNode);
    setResponseResults(node);
}

void interfaceResponse::setResponseSuccessFlag()
{
    this->_flag = true;
}

void interfaceResponse::setResponseFaliedFlag()
{
    this->_flag = false;
}

void interfaceResponse::setResponseError(string error)
{
    this->_error = error;
}

void interfaceResponse::setResponseCode(InterfaceResCode code)
{
    this->_code = code;
}

bool interfaceResponse::setResponseResults(string sResults)
{
    cJSON * results;
    //转化字符串为cJSON
    results = cJSON_Parse(sResults.c_str());
    //转化字符串为cJSON 失败
    if(!results){
        mLogError("Failed to run cJSON_Parse(sResults)=("<<sResults<<")");
        mLogError("Error before:"<<cJSON_GetErrorPtr());
        cJSON_Delete(results);
        return false;
    }else{
        mLogDebug("success to set response results:"<<sResults);
        setResponseResults(results);
        return true;
    }
}

void interfaceResponse::setResponseResults(cJSON *results)
{
    this->_results = results;
}

bool interfaceResponse::addResponseResultsNode(string name, string sVal)
{
    if(name.length() <= 0){
        mLogError("addResponseResultsNode(...) name.length() <= 0");
        return false;
    }
    try{
        if(this->_results == NULL){
            this->_results = cJSON_CreateObject();
        }
        cJSON_AddItemToObject(this->_results,name.c_str(),cJSON_CreateString(sVal.c_str()));
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }

    return true;
}

bool interfaceResponse::addResponseResultsNode(string name, int iVal)
{
    if(name.length() <= 0){
        mLogError("addResponseResultsNode(...) name.length() <= 0");
        return false;
    }
    try{
        if(this->_results == NULL){
            this->_results = cJSON_CreateObject();
        }
        cJSON_AddItemToObject(this->_results,name.c_str(),cJSON_CreateNumber(iVal));
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }

    return true;
}

bool interfaceResponse::addResponseResultsNode(string name, double dVal)
{
    if(name.length() <= 0){
        mLogError("addResponseResultsNode(...) name.length() <= 0");
        return false;
    }
    try{
        if(this->_results == NULL){
            this->_results = cJSON_CreateObject();
        }
        cJSON_AddItemToObject(this->_results,name.c_str(),cJSON_CreateNumber(dVal));
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }

    return true;
}

bool interfaceResponse::addResponseResultsNode(string name, bool bVal)
{
    if(name.length() <= 0){
        mLogError("addResponseResultsNode(...) name.length() <= 0");
        return false;
    }
    try{
        if(this->_results == NULL){
            this->_results = cJSON_CreateObject();
        }
        if(bVal){
            cJSON_AddItemToObject(this->_results,name.c_str(),cJSON_CreateTrue());
        }else{
            cJSON_AddItemToObject(this->_results,name.c_str(),cJSON_CreateFalse());
        }
    }catch(exception &e){
        mLogError("Failed to try."<<e.what());
        return false;
    }

    return true;
}

void interfaceResponse::clearResponse()
{
    mLogDebug("cJSON_Delete(_root)");
    try{
        if(_root){
            cJSON_Delete(_root);
        }
        _root = cJSON_CreateObject();
    }catch(exception &e){
        mLogError("Failed to try cHSON_Delete(_Root)."<<e.what());
    }
}

void interfaceResponse::clearAllMsg()
{
    mLogDebug("clearAllMsg()");
    this->_flag = true;
    this->_error = "";
    _results = NULL;
    clearResponse();
}

void interfaceResponse::responseAddFlag()
{
    mLogDebug("add flag node...");
    string sFlag;
    bool bFlag;
    if(this->_flag){
        //成功
        sFlag = RESPONSE_NODE_FLAG_SUCCESS;
        bFlag = true;
    }else{
        //失败
        sFlag = RESPONSE_NODE_FLAG_FAILED;
        bFlag = false;
    }

//    cJSON * jNode = cJSON_CreateString(sFlag.c_str());
    cJSON * jNode = cJSON_CreateBool(bFlag);
    if(jNode == NULL){
        mLogError("Failed to run cJSON_CreateString(sFlag.c_str())=("<<sFlag.c_str()<<")");
        return ;
    }
    if(_root == NULL){
        mLogError("_root == NULL");
        return ;
    }
    cJSON_AddItemToObject(_root,RESPONSE_NODE_FLAG,jNode);
}

void interfaceResponse::responseAddError()
{
    mLogDebug("add error node...");
    if(this->_flag){
        if(this->_error.length() > 0){
            mLogWarn("flag == true,but error.length > 0!");
        }
    }
    cJSON * jNode = cJSON_CreateString(this->_error.c_str());
    if(jNode == NULL){
        mLogError("Failed to run cJSON_CreateString(this->_error.c_str())=("<<this->_error.c_str()<<")");
        return ;
    }
    if(_root == NULL){
        mLogError("_root == NULL");
        return ;
    }
    cJSON_AddItemToObject(_root,RESPONSE_NODE_ERROR,jNode);

}

void interfaceResponse::responseAddCode()
{
    mLogDebug("add code node...");
    if(this->_flag){
        if(this->_code > eInterfaceResCodeErrorMAX){
            mLogWarn("code"<<this->_code<<" > eInterfaceResCodeErrorMAX"<<eInterfaceResCodeErrorMAX<<"! set code=eInterfaceResCodeErrorMAX.");
            this->_code = eInterfaceResCodeErrorMAX;
        }
    }
    cJSON * jNode = cJSON_CreateNumber(this->_code);
    if(jNode == NULL){
        mLogError("Failed to run cJSON_CreateNumber(this->_code)=("<<this->_code<<")");
        return ;
    }
    if(_root == NULL){
        mLogError("_root == NULL");
        return ;
    }
    cJSON_AddItemToObject(_root,RESPONSE_NODE_CODE,jNode);
}

void interfaceResponse::responseAddResults()
{
    mLogDebug("add results node...");
    if(this->_results == NULL){
        // 无 _results 信息
        mLogDebug("none results.");
    }else{
        // 有 _results 信息

        if(_root == NULL){
            //根节点为空
            mLogError("_root == NULL");
            return ;
        }
        cJSON_AddItemToObject(_root,RESPONSE_NODE_RESULTS,this->_results);
    }
}

