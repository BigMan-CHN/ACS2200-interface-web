TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    cgiAnalysis.h \
    interfaceParams.h \
    interfaceProcess.h \
    InterfaceResponse.h \
    interfaceDefines.h \
    MPSClient.h \
    MPSClientDefine.h \
    method/CAnalyzeParaProc.h \
    method/GetTaskContent.h \
    method/CGetHostStatus.h \
    method/CGetChannelContents.h \
    common/CTimeCvt.h \
    method/CGetSysConfig.h \
    config.h \
    method/CGetHostConfig.h \
    method/CGetWebHostConfig.h \
    method/CGetHostDecInfo.h \
    method/CGetHostChnInfo.h \
    method/CGetWebHostChnInfo.h \
    method/CSetTaskDelete.h \
    method/CSetDecCtrl.h \
    method/CSetOsdCtrl.h \
    method/CSetPutNoteFileCtrl.h \
    method/CSetRecCtrl.h \
    method/CSetWebEffectCtrl.h \
    method/CSetWebExtendCtrl.h \
    method/CSetWebRecCtrl.h \
    method/CSetWebMultiCtrl.h \
    method/CSetWebAudioCtrl.h \
    method/CDisciplineCtrl.h \
    method/CStreamCtrl.h \
    method/CSetPowerCtrl.h \
    method/CSendConsoleCmd.h \
    method/CSendSerialCmd.h \
    method/CHostControlDec.h \
    method/CHostControlDiscipline.h \
    method/CHostControl.h \
    method/CSetpoint.h \
	method/GetWebInfraredStudyList.h\
	method/SetWebInfraredStudy.h \
	method/SetWebInfraredDel.h \
	method/SetWebInfraredSend.h \
	method/SetWebInfraredCreate.h \
	method/GetWebAudioConf.h \
	method/SetWebAudioConf.h \
	method/GetWebAudioGroupConf.h \
	method/SetWebAudioGroupConf.h \
	method/GetWebAudioMatrixConf.h \
	method/SetWebAudioMatrixConf.h \
	method/GetWebVideoParam.h \
	method/SetWebVideoParam.h \
	method/GetWebAudioParam.h \
	method/SetWebAudioParam.h \
	method/GetWebRtspParam.h \
	method/SetWebRtspParam.h \
	method/GetWebRtmpParam.h \
	method/SetWebRtmpParam.h \
	method/GetWebLogFile.h \
	method/SetWebLogFileConf.h \
	method/SetWebDevID.h \
	method/SetWebRtmpBroadConf.h \
	method/SetWebFirmUpdate.h \
	method/SetWebZKTUpdate.h \
	method/SetWebZKTDownload.h \
	method/SetWebConfigUpdate.h \
	method/SetWebDiscipFileUpdate.h \
	method/SetWebBackupCfgFile.h \
	method/SetWebRecoverCFGFile.h \
    method/LoginInterface.h \
    method/CGetWebAddrList.h \
    method/CGetWebVideoChnList.h \
    method/CSetWebFtpConf.h \
    method/CSetWebNewMosaic.h \
    method/CSetWebNewWhine.h \
    method/CSetWebStorageConf.h \
    method/CSetWebSyncTime.h \
    method/CSetWebUartConf.h \
    method/CSetWebUserPassword.h \
    method/CSetWebVideoChnConf.h \
    method/CSetWebVideoOutPutConf.h \
    method/CSetWebWhine.h \
    method/CGetSerialCmd.h \
    method/CGetWebCaptionList.h \
    method/CGetWebMICGain.h \
    method/CGetWebMosaicList.h \
    method/CGetWebWhineList.h \
    method/CSetWebMICGain.h \
    method/XMLOpera.h \
    method/XMLDefines.h \
    method/MultiPartyOpera.h \
        method/CGetWebFtpConf.h\
        method/CGetWebUartConf.h\
        method/CGetWebUserPassword.h\
        method/CGetWebStorageConf.h\
method/CGetWebDevID.h\
method/CGetWebVideoOutPutConf.h\
method/SetWebInfraredFileDel.h\
method/GetWebInfraredFileInf.h\
method/CSetWebMICWhineButton.h\
method/SetWebSystermShoutDown.h\
method/CGetWebMICWhineButton.h\
method/SetWebVideoMatrix.h\
method/GetWebVideoMatrix.h\
method/CGetAudioDefultGain.h\
method/SetWebDSPMatrix.h\
method/GetWebDSPMatrix.h\
method/CSetWebStartAutoRec.h\
method/CGetWebStartAutoRec.h\
method/CSetWebStartRec.h\
method/CGetWebStartRec.h\
method/GetHostDecinformation.h\
method/CWebAudioMute.h\
method/stu_sqlite.h\
method/sqlite3.h
SOURCES += main.cpp \
    cgiAnalysis.cpp \
    interfaceParams.cpp \
    interfaceProcess.cpp \
    InterfaceResponse.cpp \
    MPSClient.cpp \
    method/GetTaskContent.cpp \
    method/CGetHostStatus.cpp \
    method/CGetChannelContents.cpp \
    common/CTimeCvt.cpp \
    method/CAnalyzeParaProc.cpp \
    method/CGetSysConfig.cpp \
    method/CGetHostConfig.cpp \
    method/CGetWebHostConfig.cpp \
    method/CGetHostDecInfo.cpp \
    method/CGetHostChnInfo.cpp \
    method/CGetWebHostChnInfo.cpp \
    method/CSetTaskDelete.cpp \
    method/CSetDecCtrl.cpp \
    method/CSetOsdCtrl.cpp \
    method/CSetPutNoteFileCtrl.cpp \
    method/CSetRecCtrl.cpp \
    method/CSetWebEffectCtrl.cpp \
    method/CSetWebExtendCtrl.cpp \
    method/CSetWebRecCtrl.cpp \
    method/CSetWebMultiCtrl.cpp \
    method/CSetWebAudioCtrl.cpp \
    method/CDisciplineCtrl.cpp \
    method/CStreamCtrl.cpp \
    method/CSetPowerCtrl.cpp \
    method/CSendConsoleCmd.cpp \
    method/CSendSerialCmd.cpp \
    method/CHostControlDec.cpp \
    method/CHostControlDiscipline.cpp \
    method/CHostControl.cpp \
    method/CSetpoint.cpp \
    method/GetWebInfraredStudyList.cpp \
    method/SetWebInfraredStudy.cpp \
    method/SetWebInfraredDel.cpp \
    method/SetWebInfraredSend.cpp \
    method/SetWebInfraredCreate.cpp \
    method/GetWebAudioConf.cpp \
    method/SetWebAudioConf.cpp \
    method/GetWebAudioGroupConf.cpp \
    method/SetWebAudioGroupConf.cpp \
    method/GetWebAudioMatrixConf.cpp \
    method/SetWebAudioMatrixConf.cpp \
    method/GetWebVideoParam.cpp \
    method/SetWebVideoParam.cpp \
    method/GetWebAudioParam.cpp \
    method/SetWebAudioParam.cpp \
    method/GetWebRtspParam.cpp \
    method/SetWebRtspParam.cpp \
    method/GetWebRtmpParam.cpp \
    method/SetWebRtmpParam.cpp \
    method/GetWebLogFile.cpp \
    method/SetWebLogFileConf.cpp \
    method/SetWebDevID.cpp \
    method/SetWebRtmpBroadConf.cpp \
    method/SetWebFirmUpdate.cpp \
    method/SetWebZKTUpdate.cpp \
    method/SetWebZKTDownload.cpp \
    method/SetWebConfigUpdate.cpp \
    method/SetWebDiscipFileUpdate.cpp \
    method/SetWebBackupCfgFile.cpp \
    method/SetWebRecoverCFGFile.cpp \
    method/LoginInterface.cpp \
    method/CGetWebAddrList.cpp \
    method/CGetWebVideoChnList.cpp \
    method/CSetWebFtpConf.cpp \
    method/CSetWebNewMosaic.cpp \
    method/CSetWebNewWhine.cpp \
    method/CSetWebStorageConf.cpp \
    method/CSetWebSyncTime.cpp \
    method/CSetWebUartConf.cpp \
    method/CSetWebUserPassword.cpp \
    method/CSetWebVideoChnConf.cpp \
    method/CSetWebVideoOutPutConf.cpp \
    method/CSetWebWhine.cpp \
    method/CGetSerialCmd.cpp \
    method/CGetWebCaptionList.cpp \
    method/CGetWebMICGain.cpp \
    method/CGetWebMosaicList.cpp \
    method/CGetWebWhineList.cpp \
    method/CSetWebMICGain.cpp \
    method/XMLOpera.cpp \
    method/MultiPartyOpera.cpp \
        method/CGetWebFtpConf.cpp\
        method/CGetWebUartConf.cpp\
        method/CGetWebUserPassword.cpp\
        method/CGetWebStorageConf.cpp\
method/CGetWebVideoOutPutConf.cpp\
method/CGetWebDevID.cpp\
method/SetWebInfraredFileDel.cpp\
method/GetWebInfraredFileInf.cpp\
method/CSetWebMICWhineButton.cpp\
method/SetWebSystermShoutDown.cpp\
method/CGetWebMICWhineButton.cpp\
method/GetWebVideoMatrix.cpp\
method/SetWebVideoMatrix.cpp\
method/CGetAudioDefultGain.cpp\
method/GetWebDSPMatrix.cpp\
method/SetWebDSPMatrix.cpp\
method/CSetWebStartAutoRec.cpp\
method/CGetWebStartAutoRec.cpp\
method/CSetWebStartRec.cpp\
method/CGetWebStartRec.cpp\
method/GetHostDecinformation.cpp\
method/CWebAudioMute.cpp\
method/stu_sqlite.c\
method/sqlite3.c
DISTFILES += \
    readMe.txt

#creat .cgi file
TARGET = interface
#TARGET = /var/www/cgi-bin/liveCaster-4

#cgi include
#INCLUDEPATH += /home/MPS/libcgi-1.0/src
#INCLUDEPATH +=/home/develop/MPS/sqlite-autoconf-3130000/install/include
#INCLUDEPATH +=/opt/sqlite/include
INCLUDEPATH +=/home/xuyf/develop-hi400/sqlite-autoconf-3380500/installdir/include
#cgi lib
#LIBS += /home/MPS/libcgi-1.0/src/libcgi.a

#log lib
INCLUDEPATH += \
    /home/xuyf/develop-hi400/libcommon_1.69.0/src      \
    /home/xuyf/develop-hi400/libcommon_1.69.0/src/LibXML  \
    /home/xuyf/develop-hi400/libcommon_1.69.0/src/LibTransmit \
    /home/xuyf/develop-hi400/libMLog \
    /home/xuyf/develop-hi400/log4cplus-1.2.1/include \
    /home/xuyf/develop-hi400/cJSONFiles/cJSON
#log lib
LIBS += \
    /home/xuyf/develop-hi400/libcommon_1.69.0/src/LibTransmit/qt600/tarnsmitv600/libtransmit.a  \
    /home/xuyf/develop-hi400/libMLog/libMLog.a \
    /home/xuyf/develop-hi400/log4cplus-1.2.1/installdir/lib/liblog4cplus.a \
    /home/xuyf/develop-hi400/cJSONFiles/cJSON/CJSON/libCJSON.a \

LIBS += /home/xuyf/develop-hi400/libcommon_1.69.0/src/LibXML/qtv600/xmlv600/libxml.a

#SQLite3 lib
LIBS +=/home/xuyf/develop-hi400/sqlite-autoconf-3380500/installdir/lib/libsqlite3.a

#boost include
INCLUDEPATH +=/home/xuyf/develop-hi400/boost_1_69_0

#add by libx in 2022/02/26
#pugixml include
INCLUDEPATH += /home/xuyf/develop-hi400/tinyxml2

#pugixml lib
LIBS += /home/xuyf/develop-hi400/tinyxml2/libtinyxml2.a
#add by ltz in 2022/03/21
INCLUDEPATH += /home/xuyf/develop-hi400/libxml2-v2.9.13/install/include/libxml2/
#INCLUDEPATH += /home/xuyf/develop/libiconv-1.16/install/include
LIBS += /home/xuyf/develop-hi400/libxml2-v2.9.13/installdir/lib/libxml2.a
#LIBS += /home/xuyf/develop-hi400/libiconv-1.16/install/lib/libcharset.a
#boost lib
LIBS += /home/xuyf/develop-hi400/boost_1_69_0/installdir/lib/libboost_system.a
LIBS += /home/xuyf/develop-hi400/boost_1_69_0/installdir/lib/libboost_thread.a
LIBS += /home/xuyf/develop-hi400/boost_1_69_0/installdir/lib/libboost_filesystem.a
LIBS +=  \
    -lrt \
    -lpthread \
    -ldl
DEFINES -= UNICODE
