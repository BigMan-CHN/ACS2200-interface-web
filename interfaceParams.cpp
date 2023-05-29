#include "interfaceParams.h"
#include "interfaceDefines.h"

InterfaceParams::InterfaceParams()
    :_sJsonData("")
    ,_root(NULL)
    ,_params(NULL)
    ,_method("")
    ,_channelNum(CHANNEL_NUM_ERROR)
{

}

bool InterfaceParams::getMethod(string &sMethod)
{
    if(this->_method.length() <= 0){
        return false;
    }else{
        sMethod = this->_method;
        return true;
    }
}

bool InterfaceParams::getChannelNum(int &chNum)
{
    if(this->_channelNum == CHANNEL_NUM_ERROR){
        return false;
    }else{
        chNum = this->_channelNum;
        return true;
    }
}

bool InterfaceParams::analysisParams(char *json)
{
    string sJson = (string)(json==NULL?"":json)+"\0";
    return analysisParams(sJson);
}

bool InterfaceParams::analysisParams(string sJsonData)
{
    _sJsonData = sJsonData;
    mLogDebug("sJsonData:"<<sJsonData);
       
    //转化字符串为cJSON
    _root = cJSON_Parse(_sJsonData.c_str());
    //转化字符串为cJSON 失败
    if(!_root){
        mLogError("Failed to run cJSON_Parse(sJsonParams)=("<<sJsonData<<")");
        mLogError("Error before:"<<cJSON_GetErrorPtr());
        try{
            cJSON_Delete(_root);
        }catch(exception &e){
            mLogError("Failed to try cJSON_Delete(_root)."<<e.what());
        }

        _root = NULL;
        return false;
    }
    if(!getMethodfromRoot()){
        mLogError("Failed to run getMethodfromRoot()");
        return false;
    }
    if(!getParamsfromRoot()){
        mLogWarn("Failed to run getParamsfromRoot()");
//        return false;
    }
//    if(!getChannelNumfromRoot()){
//        mLogWarn("Failed to run getChannelNumfromRoot()");
//    }

    return true;
}

bool InterfaceParams::getParamsNodeValueofInt(string sNode, int &iValue)
{
    if(this->_params == NULL){
        mLogError("this->_params is NULL!");
        return false;
    }
    try{
        //获取sNode节点
        cJSON *jNode = cJSON_GetObjectItem(this->_params,sNode.c_str());
        //获取节点失败
        if(!jNode){
            mLogError("Failed to run cJSON_GetObjectItem(this->_params,sNode)=(this->_params,"<<sNode<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jNode);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jNode)."<<e.what());
            }
            return false;
        }else{
            try{
                if(jNode->type ==cJSON_String){
                    string sValue = jNode->valuestring;
                    iValue = atoi(sValue.c_str());
                    mLogDebug("sNode:"<<sNode<<",sValue:"<<sValue<<",iValue:"<<iValue);
                }else if(jNode->type ==cJSON_Number){
                    iValue = jNode->valueint;
                    mLogDebug("sNode:"<<sNode<<",iValue:"<<iValue);
                }
                if(iValue == INT_MIN){
                    mLogError("get sNode:"<<sNode<<" value == INT_MIN:"<<INT_MIN);
                    return false;
                }
                return true;
            }catch(exception& e){
                mLogError("Failed to try:"<<e.what());
                return false;
            }
        }
    }catch(exception &e){
        mLogError("Failed to try getParamsNodeValueofInt(Node,...)=("<<sNode<<",...)."<<e.what());
        return false;
    }
}

bool InterfaceParams::getParamsNodeValueofStr(string sNode, string &sValue)
{
    if(this->_params == NULL){
        mLogError("this->_params is NULL!");
        return false;
    }
    try{
        //获取sNode节点
        cJSON *jNode = cJSON_GetObjectItem(this->_params,sNode.c_str());
        //获取节点失败
        if(!jNode){
            mLogError("Failed to run cJSON_GetObjectItem(this->_params,sNode)=(this->_params,"<<sNode<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jNode);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jNode)."<<e.what());
            }
            return false;
        }else{
            try{
                if(jNode->type == cJSON_String){
                    sValue = jNode->valuestring;
                    mLogDebug("sNode:"<<sNode<<",sValue:"<<sValue);
                    return true;
                }else{
                    mLogError("sNode->type != cJSON_String,sNode:"<<sNode);
                    sValue = "";
                    return false;
                }
            }catch(exception& e){
                sValue = "";
                mLogError("Failed to try:"<<e.what());
                return false;
            }
        }
    }catch(exception &e){
        mLogError("Failed to try getParamsNodeValueofStr(Node,...)=("<<sNode<<",...)."<<e.what());
        return false;
    }
}
bool InterfaceParams::getParamsNodeValueofDbl(string sNode, double &dValue)
{
    if(this->_params == NULL){
        mLogError("this->_params is NULL!");
        return false;
    }
    try{
        //获取sNode节点
        cJSON *jNode = cJSON_GetObjectItem(this->_params,sNode.c_str());
        //获取节点失败
        if(!jNode){
            mLogError("Failed to run cJSON_GetObjectItem(this->_params,sNode)=(this->_params,"<<sNode<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jNode);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jNode)."<<e.what());
            }
            return false;
        }else{
            try{
                if(jNode->type ==cJSON_String){
                    string sValue = jNode->valuestring;
                    dValue = atof(sValue.c_str());
                    mLogDebug("sNode:"<<sNode<<",sValue:"<<sValue<<",dValue:"<<dValue);
                }else if(jNode->type ==cJSON_Number){
                    dValue = jNode->valuedouble;
                    mLogDebug("sNode:"<<sNode<<",dValue:"<<dValue);
                }
                return true;
            }catch(exception& e){
                mLogError("Failed to try:"<<e.what());
                return false;
            }
        }
    }catch(exception &e){
        mLogError("Failed to try getParamsNodeValueofDbl(Node,...)=("<<sNode<<",...)."<<e.what());
        return false;
    }
}

bool InterfaceParams::getParamsNodeValueofBool(string sNode, bool &bValue)
{
    if(this->_params == NULL){
        mLogError("this->_params is NULL!");
        return false;
    }
    try{
        //获取sNode节点
        cJSON *jNode = cJSON_GetObjectItem(this->_params,sNode.c_str());
        //获取节点失败
        if(!jNode){
            mLogError("Failed to run cJSON_GetObjectItem(this->_params,sNode)=(this->_params,"<<sNode<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jNode);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jNode)."<<e.what());
            }
            return false;
        }else{
            try{
                if(jNode->type ==cJSON_False){
                    bValue = false;
                    mLogDebug("sNode:"<<",bValue:"<<bValue);
                }else if(jNode->type ==cJSON_True){
                    bValue = true;
                    mLogDebug("sNode:"<<sNode<<",bValue:"<<bValue);
                }
                return true;
            }catch(exception& e){
                mLogError("Failed to try:"<<e.what());
                return false;
            }
        }
    }catch(exception &e){
        mLogError("Failed to try getParamsNodeValueofBool(Node,...)=("<<sNode<<",...)."<<e.what());
        return false;
    }
}

bool InterfaceParams::getParamsNodeValueofArry(string sNode, int &ArryNumbers, cJSON *ArryNode[])
{
    if(this->_params == NULL){
        mLogError("this->_params is NULL!");
        return false;
    }
    try{
        //获取sNode节点
        cJSON *jNode = cJSON_GetObjectItem(this->_params,sNode.c_str());

        //获取节点失败
        if(!jNode){
            mLogError("Failed to run cJSON_GetObjectItem(this->_params,sNode)=(this->_params,"<<sNode<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jNode);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jNode)."<<e.what());
            }
            return false;
        }else{
            try{
                if(jNode->type == cJSON_Array){
				    ArryNumbers = cJSON_GetArraySize(jNode);
					for(int i = 0; i < ArryNumbers; i++)
					{
						ArryNode[i] = cJSON_CreateObject();
			        	ArryNode[i] = cJSON_GetArrayItem(jNode, i);
					}
                    mLogDebug("sNode:"<<sNode<<",ArryNumbers:"<<ArryNumbers);
                    return true;
                }else{
                    mLogError("sNode->type != cJSON_String,sNode:"<<sNode);
                    ArryNode = NULL;
                    return false;
                }
            }catch(exception& e){
                ArryNode = NULL;
                mLogError("Failed to try:"<<e.what());
                return false;
            }
        }
    }catch(exception &e){
        mLogError("Failed to try getParamsNodeValueofStr(Node,...)=("<<sNode<<",...)."<<e.what());
        return false;
    }
}
bool InterfaceParams::getNodeValueofInt(cJSON *JsonNode, string sNode, int &iValue)
{
    if(JsonNode == NULL){
        mLogError("JsonNode is NULL!");
        return false;
    }
    try{
        //获取sNode节点
        cJSON *jNode = cJSON_GetObjectItem(JsonNode, sNode.c_str());
        //获取节点失败
        if(!jNode){
            mLogError("Failed to run cJSON_GetObjectItem(JsonNode,sNode)=(JsonNode,"<<sNode<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jNode);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jNode)."<<e.what());
            }
            return false;
        }else{
            try{
                if(jNode->type ==cJSON_String){
                    string sValue = jNode->valuestring;
                    iValue = atoi(sValue.c_str());
                    mLogDebug("sNode:"<<sNode<<",sValue:"<<sValue<<",iValue:"<<iValue);
                }else if(jNode->type ==cJSON_Number){
                    iValue = jNode->valueint;
                    mLogDebug("sNode:"<<sNode<<",iValue:"<<iValue);
                }
                if(iValue == INT_MIN){
                    mLogError("get sNode:"<<sNode<<" value == INT_MIN:"<<INT_MIN);
                    return false;
                }
                return true;
            }catch(exception& e){
                mLogError("Failed to try:"<<e.what());
                return false;
            }
        }
    }catch(exception &e){
        mLogError("Failed to try getParamsNodeValueofInt(Node,...)=("<<sNode<<",...)."<<e.what());
        return false;
    }
}

bool InterfaceParams::getNodeValueofStr(cJSON *JsonNode, string sNode, string &sValue)
{
    if(JsonNode == NULL){
        mLogError("JsonNode is NULL!");
        return false;
    }
    try{
        //获取sNode节点
        cJSON *jNode = cJSON_GetObjectItem(JsonNode, sNode.c_str());
        //获取节点失败
        if(!jNode){
            mLogError("Failed to run cJSON_GetObjectItem(this->_params,sNode)=(this->_params,"<<sNode<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jNode);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jNode)."<<e.what());
            }
            return false;
        }else{
            try{
                if(jNode->type == cJSON_String){
                    sValue = jNode->valuestring;
                    mLogDebug("sNode:"<<sNode<<",sValue:"<<sValue);
                    return true;
                }else{
                    mLogError("sNode->type != cJSON_String,sNode:"<<sNode);
                    sValue = "";
                    return false;
                }
            }catch(exception& e){
                sValue = "";
                mLogError("Failed to try:"<<e.what());
                return false;
            }
        }
    }catch(exception &e){
        mLogError("Failed to try getParamsNodeValueofStr(Node,...)=("<<sNode<<",...)."<<e.what());
        return false;
    }
}

bool InterfaceParams::getNodeValueofDbl(cJSON *JsonNode, string sNode, double &dValue)
{
    if(JsonNode == NULL){
        mLogError("JsonNode is NULL!");
        return false;
    }
    try{
        //获取sNode节点
        cJSON *jNode = cJSON_GetObjectItem(JsonNode, sNode.c_str());
        //获取节点失败
        if(!jNode){
            mLogError("Failed to run cJSON_GetObjectItem(this->_params,sNode)=(this->_params,"<<sNode<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jNode);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jNode)."<<e.what());
            }
            return false;
        }else{
            try{
                if(jNode->type ==cJSON_String){
                    string sValue = jNode->valuestring;
                    dValue = atof(sValue.c_str());
                    mLogDebug("sNode:"<<sNode<<",sValue:"<<sValue<<",dValue:"<<dValue);
                }else if(jNode->type ==cJSON_Number){
                    dValue = jNode->valuedouble;
                    mLogDebug("sNode:"<<sNode<<",dValue:"<<dValue);
                }
                return true;
            }catch(exception& e){
                mLogError("Failed to try:"<<e.what());
                return false;
            }
        }
    }catch(exception &e){
        mLogError("Failed to try getParamsNodeValueofDbl(Node,...)=("<<sNode<<",...)."<<e.what());
        return false;
    }
}

bool InterfaceParams::getNodeValueofBool(cJSON *JsonNode, string sNode, bool &bValue)
{
    if(JsonNode == NULL){
        mLogError("JsonNode is NULL!");
        return false;
    }
    try{
        //获取sNode节点
        cJSON *jNode = cJSON_GetObjectItem(JsonNode, sNode.c_str());
        //获取节点失败
        if(!jNode){
            mLogError("Failed to run cJSON_GetObjectItem(this->_params,sNode)=(this->_params,"<<sNode<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jNode);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jNode)."<<e.what());
            }
            return false;
        }else{
            try{
                if(jNode->type ==cJSON_False){
                    bValue = false;
                    mLogDebug("sNode:"<<",bValue:"<<bValue);
                }else if(jNode->type ==cJSON_True){
                    bValue = true;
                    mLogDebug("sNode:"<<sNode<<",bValue:"<<bValue);
                }
                return true;
            }catch(exception& e){
                mLogError("Failed to try:"<<e.what());
                return false;
            }
        }
    }catch(exception &e){
        mLogError("Failed to try getParamsNodeValueofBool(Node,...)=("<<sNode<<",...)."<<e.what());
        return false;
    }
}

void InterfaceParams::clearAll()
{
    mLogDebug("clearAllMsg()");
    this->_sJsonData="";
    this->_method = "";
    this->_channelNum = CHANNEL_NUM_ERROR;

    try{
        cJSON_Delete(_root);
    }catch(exception &e){
        mLogWarn("Failed to try cJSON_Delete(this->_root)."<<e.what());
        this->_root = NULL;
    }

//    try{
//        cJSON_Delete(_params);
//    }catch(exception &e){
//        mLogWarn("Failed to try cJSON_Delete(this->_params)."<<e.what());
//        this->_params = NULL;
//    }
}

bool InterfaceParams::getMethodfromRoot()
{
    try{
        //获取method节点
        cJSON *jMethod = cJSON_GetObjectItem(_root,REQUEST_NODE_METHOD);
        //获取节点失败
        if(!jMethod){
            mLogError("Failed to run cJSON_GetObjectItem(_root,NODE_METHOD)=(_root,"<<REQUEST_NODE_METHOD<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jMethod);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jMethod)."<<e.what());
            }
            jMethod = NULL;
            this->_method = "";
            return false;
        }else{
    //        mLogDebug("jMethod:"<<cJSON_Print(jMethod));
            this->_method = (string)jMethod->valuestring;
            mLogDebug("_method:"<<this->_method);
            return true;
        }
    }catch(exception &e){
        mLogError("Failed to try getMethodfromRoot()."<<e.what());
        this->_method = "";
        return false;
    }
}

bool InterfaceParams::getParamsfromRoot()
{
    try{
        //获取params节点
        this->_params = cJSON_GetObjectItem(_root,REQUEST_NODE_PARAMS);
        //获取节点失败
        if(!this->_params){
            mLogError("Failed to run cJSON_GetObjectItem(_root,NODE_PARAMS)=(_root,"<<REQUEST_NODE_PARAMS<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(this->_params);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(this->_params)."<<e.what());
            }
            this->_params = NULL;
            return false;
        }else{
            mLogDebug("_params:"<<cJSON_Print(this->_params));
            return true;
        }
    }catch(exception &e){
        mLogError("Failed to try getParamsfromRoot()."<<e.what());
        this->_params = NULL;
        return false;
    }
}

bool InterfaceParams::getChannelNumfromRoot()
{
    try{
        //获取channelNum节点
        cJSON *jChannelNum = cJSON_GetObjectItem(_root,REQUEST_NODE_CHANNELNUM);
        //获取节点失败
        if(!jChannelNum){
            mLogError("Failed to run cJSON_GetObjectItem(_root,NODE_CHANNELNUM)=(_root,"<<REQUEST_NODE_CHANNELNUM<<")");
            mLogError("Error before:"<<cJSON_GetErrorPtr());
            try{
                cJSON_Delete(jChannelNum);
            }catch(exception &e){
                mLogError("Failed to try cJSON_Delete(jChannelNum)."<<e.what());
            }
            this->_channelNum = CHANNEL_NUM_ERROR;
            return false;
        }else{
    //        mLogDebug("jChannelNum:"<<cJSON_Print(jChannelNum));
            if(jChannelNum->type ==cJSON_String){
                string sChannelNum = jChannelNum->valuestring;
                this->_channelNum = atoi(sChannelNum.c_str());
            }else if(jChannelNum->type ==cJSON_Number){
                this->_channelNum = jChannelNum->valueint;
            }
            mLogDebug("_channelNum:"<<this->_channelNum);
            return true;
        }
    }catch(exception &e){
        mLogError("Failed to try getChannelNumfromRoot()."<<e.what());
        this->_channelNum = CHANNEL_NUM_ERROR;
        return false;
    }
}

bool InterfaceParams::getParamseNodeofTestDebug(string &sVal)
{
    return this->getParamsNodeValueofStr(REQUEST_TEST_NODE_DEBUG,sVal);
}

