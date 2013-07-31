#pragma once
#include <winsvc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include "resource.h"
/**/
#include "../../BonCtrl/BonCtrl.h"
#include "../../Common/PathUtil.h"
#include "../../Common/TimeUtil.h"
#include "../../Common/PipeServer.h"

#include "TstOpnDef.h"
#include "TstOpnMain.h"


BOOL minTask;

CString moduleIniPath;
CString commonIniPath;
CString timerSrvIniPath;

CString iniBonDriver;
int initONID;
int initTSID;
int initSID;
int initOpenWait;
int initChgWait;
BOOL iniMin;
BOOL iniView;
BOOL iniNetwork;
BOOL iniUDP;
BOOL iniTCP;
int openLastCh;

CTstOpnMain tstmain;

map<int, wstring> bonList;
vector<CH_DATA4> serviceList;


vector<NW_SEND_INFO> udpSendList;
