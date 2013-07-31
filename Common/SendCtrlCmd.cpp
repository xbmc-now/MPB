#include "StdAfx.h"
#include "SendCtrlCmd.h"
/*
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "Ws2_32.lib")
*/
#include "StringUtil.h"
#include "CtrlCmdUtil.h"
#include "CtrlCmdUtil2.h"
#include "CtrlCmdDef.h"
#include "ErrDef.h"

#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")

CSendCtrlCmd::CSendCtrlCmd(void)
{
	CoInitialize(NULL);

	WSAData wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);

	this->lockEvent = _CreateEvent(FALSE, TRUE, NULL);

	this->tcpFlag = FALSE;
	this->connectTimeOut = CONNECT_TIMEOUT;

	this->pipeName = CMD2_EPG_SRV_PIPE;
	this->eventName = CMD2_EPG_SRV_EVENT_WAIT_CONNECT;

	this->ip = L"127.0.0.1";
	this->port = 5678;

}


CSendCtrlCmd::~CSendCtrlCmd(void)
{
	if( this->lockEvent != NULL ){
		UnLock();
		CloseHandle(this->lockEvent);
		this->lockEvent = NULL;
	}
	WSACleanup();

	CoUninitialize();
}

BOOL CSendCtrlCmd::Lock(LPCWSTR log, DWORD timeOut)
{
	if( this->lockEvent == NULL ){
		return FALSE;
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
	DWORD dwRet = WaitForSingleObject(this->lockEvent, timeOut);
	if( dwRet == WAIT_ABANDONED || 
		dwRet == WAIT_FAILED){
		return FALSE;
	}
	return TRUE;
}

void CSendCtrlCmd::UnLock(LPCWSTR log)
{
	if( this->lockEvent != NULL ){
		SetEvent(this->lockEvent);
	}
	if( log != NULL ){
		OutputDebugString(log);
	}
}

//�R�}���h���M���@�̐ݒ�
//�����F
// tcpFlag		[IN] TRUE�FTCP/IP���[�h�AFALSE�F���O�t���p�C�v���[�h
void CSendCtrlCmd::SetSendMode(
	BOOL tcpFlag
	)
{
	if( Lock() == FALSE ) return ;
	this->tcpFlag = tcpFlag;
	UnLock();
}

//���O�t���p�C�v���[�h���̐ڑ����ݒ�
//EpgTimerSrv.exe�ɑ΂���R�}���h�͐ݒ肵�Ȃ��Ă��i�f�t�H���g�l�ɂȂ��Ă���j
//�����F
// eventName	[IN]�r������pEvent�̖��O
// pipeName		[IN]�ڑ��p�C�v�̖��O
void CSendCtrlCmd::SetPipeSetting(
	wstring eventName,
	wstring pipeName
	)
{
	if( Lock() == FALSE ) return ;
	this->eventName = eventName;
	this->pipeName = pipeName;
	UnLock();
}

//TCP/IP���[�h���̐ڑ����ݒ�
//�����F
// ip			[IN]�ڑ���IP
// port			[IN]�ڑ���|�[�g
void CSendCtrlCmd::SetNWSetting(
	wstring ip,
	DWORD port
	)
{
	if( Lock() == FALSE ) return ;
	this->ip = ip;
	this->port = port;
	UnLock();
}

//�ڑ��������̃^�C���A�E�g�ݒ�
// timeOut		[IN]�^�C���A�E�g�l�i�P�ʁFms�j
void CSendCtrlCmd::SetConnectTimeOut(
	DWORD timeOut
	)
{
	if( Lock() == FALSE ) return ;
	this->connectTimeOut = timeOut;
	UnLock();
}

DWORD CSendCtrlCmd::SendPipe(LPCWSTR pipeName, LPCWSTR eventName, DWORD timeOut, CMD_STREAM* send, CMD_STREAM* res)
{
	if( pipeName == NULL || eventName == NULL || send == NULL || res == NULL ){
		return CMD_ERR_INVALID_ARG;
	}

	//�ڑ��҂�
	HANDLE waitEvent = _CreateEvent(FALSE, FALSE, eventName);
	if( waitEvent == NULL ){
		return CMD_ERR;
	}
	if(WaitForSingleObject(waitEvent, timeOut) != WAIT_OBJECT_0){
		CloseHandle(waitEvent);
		return CMD_ERR_TIMEOUT;
	}
	CloseHandle(waitEvent);

	//�ڑ�
	HANDLE pipe = _CreateFile( pipeName, GENERIC_READ|GENERIC_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if( pipe == INVALID_HANDLE_VALUE ){
		_OutputDebugString(L"*+* ConnectPipe Err:%d\r\n", GetLastError());
		return CMD_ERR_CONNECT;
	}

	DWORD write = 0;
	DWORD read = 0;

	//���M
	DWORD head[2];
	head[0] = send->param;
	head[1] = send->dataSize;
	if( WriteFile(pipe, head, sizeof(DWORD)*2, &write, NULL ) == FALSE ){
		CloseHandle(pipe);
		return CMD_ERR;
	}
	if( send->dataSize > 0 ){
		if( send->data == NULL ){
			CloseHandle(pipe);
			return CMD_ERR_INVALID_ARG;
		}
		DWORD sendNum = 0;
		while(sendNum < send->dataSize ){
			DWORD sendSize = 0;
			if( send->dataSize - sendNum < CMD2_SEND_BUFF_SIZE ){
				sendSize = send->dataSize - sendNum;
			}else{
				sendSize = CMD2_SEND_BUFF_SIZE;
			}
			if( WriteFile(pipe, send->data + sendNum, sendSize, &write, NULL ) == FALSE ){
				CloseHandle(pipe);
				return CMD_ERR;
			}
			sendNum += write;
		}
	}

	//��M
	if( ReadFile(pipe, head, sizeof(DWORD)*2, &read, NULL ) == FALSE ){
		CloseHandle(pipe);
		return CMD_ERR;
	}
	res->param = head[0];
	res->dataSize = head[1];
	if( res->dataSize > 0 ){
		res->data = new BYTE[res->dataSize];
		DWORD readNum = 0;
		while(readNum < res->dataSize ){
			DWORD readSize = 0;
			if( res->dataSize - readNum < CMD2_RES_BUFF_SIZE ){
				readSize = res->dataSize - readNum;
			}else{
				readSize = CMD2_RES_BUFF_SIZE;
			}
			if( ReadFile(pipe, res->data + readNum, readSize, &read, NULL ) == FALSE ){
				CloseHandle(pipe);
				return CMD_ERR;
			}
			readNum += read;
		}
	}
	CloseHandle(pipe);

	return res->param;
}

DWORD CSendCtrlCmd::SendTCP(wstring ip, DWORD port, DWORD timeOut, CMD_STREAM* sendCmd, CMD_STREAM* resCmd)
{
	if( sendCmd == NULL || resCmd == NULL ){
		return CMD_ERR_INVALID_ARG;
	}

	struct sockaddr_in server;
	SOCKET sock;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server.sin_port = htons((WORD)port);
	string strA = "";
	WtoA(ip, strA);
	server.sin_addr.S_un.S_addr = inet_addr(strA.c_str());
	DWORD socketBuffSize = 1024*1024;
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const char*)&socketBuffSize, sizeof(socketBuffSize));
	setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (const char*)&socketBuffSize, sizeof(socketBuffSize));

	int ret = connect(sock, (struct sockaddr *)&server, sizeof(server));
	if( ret == SOCKET_ERROR ){
		int a= GetLastError();
		wstring aa;
		Format(aa,L"%d",a);
		OutputDebugString(aa.c_str());
		closesocket(sock);
		return CMD_ERR_CONNECT;
	}

	DWORD read = 0;
	//���M
	DWORD head[2];
	head[0] = sendCmd->param;
	head[1] = sendCmd->dataSize;
	send(sock, (char*)head, sizeof(DWORD)*2, 0 );
	if( ret == SOCKET_ERROR ){
		closesocket(sock);
		return CMD_ERR;
	}
	if( sendCmd->dataSize > 0 ){
		if( sendCmd->data == NULL ){
			closesocket(sock);
			return CMD_ERR_INVALID_ARG;
		}
		ret = send(sock, (char*)sendCmd->data, sendCmd->dataSize, 0 );
		if( ret == SOCKET_ERROR ){
			closesocket(sock);
			return CMD_ERR;
		}
	}
	//��M
	ret = recv(sock, (char*)head, sizeof(DWORD)*2, 0 );
	if( ret == SOCKET_ERROR ){
		closesocket(sock);
		return CMD_ERR;
	}
	resCmd->param = head[0];
	resCmd->dataSize = head[1];
	if( resCmd->dataSize > 0 ){
		resCmd->data = new BYTE[resCmd->dataSize];
		read = 0;
		while(ret>0){
			ret = recv(sock, (char*)(resCmd->data + read), resCmd->dataSize - read, 0);
			if( ret == SOCKET_ERROR ){
				closesocket(sock);
				return CMD_ERR;
			}else if( ret == 0 ){
				break;
			}
			read += ret;
			if( read >= resCmd->dataSize ){
				break;
			}
		}
	}
	closesocket(sock);

	return resCmd->param;
}

DWORD CSendCtrlCmd::SendAddloadReserve()
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ADDLOAD_RESERVE;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendReloadEpg()
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_RELOAD_EPG;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendReloadSetting()
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_RELOAD_SETTING;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendClose()
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_CLOSE;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendRegistGUI(DWORD processID)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_REGIST_GUI;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(processID);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(processID, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendUnRegistGUI(DWORD processID)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_UNREGIST_GUI;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(processID);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(processID, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendRegistTCP(DWORD port)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_REGIST_GUI_TCP;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(port);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(port, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendUnRegistTCP(DWORD port)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_UNREGIST_GUI_TCP;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(port);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(port, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendEnumReserve(vector<RESERVE_DATA>* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ENUM_RESERVE;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}
	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendGetReserve(DWORD reserveID, RESERVE_DATA* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_GET_RESERVE;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(reserveID);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(reserveID, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}
	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendAddReserve(vector<RESERVE_DATA>* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ADD_RESERVE;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendDelReserve(vector<DWORD>* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_DEL_RESERVE;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendChgReserve(vector<RESERVE_DATA>* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_CHG_RESERVE;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�`���[�i�[���Ƃ̗\��ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]�\��ꗗ
DWORD CSendCtrlCmd::SendEnumTunerReserve(vector<TUNER_RESERVE_INFO>* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ENUM_TUNER_RESERVE;
	send.dataSize = 0;


	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�^��ςݏ��ꗗ�擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[OUT]�^��ςݏ��ꗗ
DWORD CSendCtrlCmd::SendEnumRecInfo(
	vector<REC_FILE_INFO>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ENUM_RECINFO;
	send.dataSize = 0;


	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�^��ςݏ����폜����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]�폜����ID�ꗗ
DWORD CSendCtrlCmd::SendDelRecInfo(vector<DWORD>* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_DEL_RECINFO;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�T�[�r�X�ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[OUT]�T�[�r�X�ꗗ
DWORD CSendCtrlCmd::SendEnumService(
	vector<EPGDB_SERVICE_INFO>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ENUM_SERVICE;
	send.dataSize = 0;


	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�T�[�r�X�w��Ŕԑg�����ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// service			[IN]ONID<<32 | TSID<<16 | SID�Ƃ����T�[�r�XID
// val				[OUT]�ԑg���ꗗ
DWORD CSendCtrlCmd::SendEnumPgInfo(
	ULONGLONG service,
	vector<EPGDB_EVENT_INFO*>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ENUM_PG_INFO;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(service);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(service, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�w��C�x���g�̔ԑg�����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// pgID				[IN]ONID<<48 | TSID<<32 | SID<<16 | EventID�Ƃ���ID
// val				[OUT]�ԑg���
DWORD CSendCtrlCmd::SendGetPgInfo(
	ULONGLONG pgID,
	EPGDB_EVENT_INFO* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_GET_PG_INFO;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(pgID);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(pgID, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�w��L�[���[�h�Ŕԑg������������
//�߂�l�F
// �G���[�R�[�h
//�����F
// key				[IN]�����L�[�i�����w�莞�͂܂Ƃ߂Č������ʂ��Ԃ�j
// val				[OUT]�ԑg���ꗗ
DWORD CSendCtrlCmd::SendSearchPg(
	vector<EPGDB_SEARCH_KEY_INFO>* key,
	vector<EPGDB_EVENT_INFO*>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_SEARCH_PG;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(key);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(key, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�ԑg���ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[OUT]�ԑg���ꗗ
DWORD CSendCtrlCmd::SendEnumPgAll(
	vector<EPGDB_SERVICE_EVENT_INFO*>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ENUM_PG_ALL;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�����\��o�^�����ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[OUT]�����ꗗ
DWORD CSendCtrlCmd::SendEnumEpgAutoAdd(
	vector<EPG_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ENUM_AUTO_ADD;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}
	UnLock();
	return ret;
}

//�����\��o�^������ǉ�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�����ꗗ
DWORD CSendCtrlCmd::SendAddEpgAutoAdd(
	vector<EPG_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ADD_AUTO_ADD;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�����\��o�^�������폜����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�����ꗗ
DWORD CSendCtrlCmd::SendDelEpgAutoAdd(
	vector<DWORD>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_DEL_AUTO_ADD;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�����\��o�^������ύX����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�����ꗗ
DWORD CSendCtrlCmd::SendChgEpgAutoAdd(
	vector<EPG_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_CHG_AUTO_ADD;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�����\��o�^�����ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[OUT]�����ꗗ	
DWORD CSendCtrlCmd::SendEnumManualAdd(
	vector<MANUAL_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ENUM_MANU_ADD;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}
	UnLock();
	return ret;
}

//�����\��o�^������ǉ�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�����ꗗ
DWORD CSendCtrlCmd::SendAddManualAdd(
	vector<MANUAL_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ADD_MANU_ADD;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�v���O�����\�񎩓��o�^�̏����폜
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�����ꗗ
DWORD CSendCtrlCmd::SendDelManualAdd(
	vector<DWORD>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_DEL_MANU_ADD;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�v���O�����\�񎩓��o�^�̏����ύX
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�����ꗗ
DWORD CSendCtrlCmd::SendChgManualAdd(
	vector<MANUAL_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_CHG_MANU_ADD;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}


DWORD CSendCtrlCmd::SendChkSuspend()
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_CHK_SUSPEND;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendSuspend(
	WORD val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_SUSPEND;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendReboot()
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_REBOOT;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendEpgCapNow()
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_EPG_CAP_NOW;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendFileCopy(
	wstring val,
	BYTE** resVal,
	DWORD* resValSize
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_FILE_COPY;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( res.dataSize == 0 ){
			UnLock();
			return CMD_ERR;
		}
		*resValSize = res.dataSize;
		*resVal = new BYTE[res.dataSize];
		memcpy(*resVal, res.data, res.dataSize);
	}

	UnLock();
	return ret;
}

//PlugIn�t�@�C���̈ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]1:ReName�A2:Write
// resVal		[OUT]�t�@�C�����ꗗ
DWORD CSendCtrlCmd::SendEnumPlugIn(
	WORD val,
	vector<wstring>* resVal
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_ENUM_PLUGIN;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(resVal, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//TVTest�̃`�����l���؂�ւ��p�̏����擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]ONID<<32 | TSID<<16 | SID�Ƃ����T�[�r�XID
// resVal		[OUT]�`�����l�����
DWORD CSendCtrlCmd::SendGetChgChTVTest(
	ULONGLONG val,
	TVTEST_CH_CHG_INFO* resVal
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_GET_CHG_CH_TVTEST;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(resVal, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�l�b�g���[�N���[�h��EpgDataCap_Bon�̃`�����l����؂�ւ�
//�߂�l�F
// �G���[�R�[�h
//�����F
// chInfo				[OUT]�`�����l�����
DWORD CSendCtrlCmd::SendNwTVSetCh(
	SET_CH_INFO* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWTV_SET_CH;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�l�b�g���[�N���[�h�ŋN������EpgDataCap_Bon���I��
//�߂�l�F
// �G���[�R�[�h
//�����F
// chInfo				[OUT]�`�����l�����
DWORD CSendCtrlCmd::SendNwTVClose(
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWTV_CLOSE;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�l�b�g���[�N���[�h�ŋN������Ƃ��̃��[�h
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[OUT]���[�h�i1:UDP 2:TCP 3:UDP+TCP�j
DWORD CSendCtrlCmd::SendNwTVMode(
	DWORD val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWTV_MODE;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�X�g���[���z�M�p�t�@�C�����J��
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]�J���t�@�C���̃T�[�o�[���t�@�C���p�X
// resVal			[OUT]����pCtrlID
DWORD CSendCtrlCmd::SendNwPlayOpen(
	wstring val,
	DWORD* resVal
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWPLAY_OPEN;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(resVal, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�X�g���[���z�M�p�t�@�C�������
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]����pCtrlID
DWORD CSendCtrlCmd::SendNwPlayClose(
	DWORD val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWPLAY_CLOSE;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�X�g���[���z�M�J�n
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]����pCtrlID
DWORD CSendCtrlCmd::SendNwPlayStart(
	DWORD val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWPLAY_PLAY;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�X�g���[���z�M��~
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]����pCtrlID
DWORD CSendCtrlCmd::SendNwPlayStop(
	DWORD val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWPLAY_STOP;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�X�g���[���z�M�Ō��݂̑��M�ʒu�Ƒ��t�@�C���T�C�Y���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN/OUT]�T�C�Y���
DWORD CSendCtrlCmd::SendNwPlayGetPos(
	NWPLAY_POS_CMD* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWPLAY_GET_POS;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�X�g���[���z�M�ő��M�ʒu���V�[�N����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]�T�C�Y���
DWORD CSendCtrlCmd::SendNwPlaySetPos(
	NWPLAY_POS_CMD* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWPLAY_SET_POS;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�X�g���[���z�M�ő��M���ݒ肷��
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]�T�C�Y���
DWORD CSendCtrlCmd::SendNwPlaySetIP(
	NWPLAY_PLAY_INFO* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWPLAY_SET_IP;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}
	
	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(val, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�X�g���[���z�M�p�t�@�C�����^�C���V�t�g���[�h�ŊJ��
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]�\��ID
// resVal			[OUT]�t�@�C���p�X��CtrlID
DWORD CSendCtrlCmd::SendNwTimeShiftOpen(
	DWORD val,
	NWPLAY_TIMESHIFT_INFO* resVal
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_EPG_SRV_NWPLAY_TF_OPEN;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(resVal, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendEnumReserve2(vector<RESERVE_DATA>* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;

	send.param = CMD2_EPG_SRV_ENUM_RESERVE2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	if( ret == CMD_SUCCESS ){
		DWORD readSize = 0;
		if( ReadVALUE2(ver, &ver, res.data, res.dataSize, &readSize) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
		if( ReadVALUE2(ver, val, res.data+readSize, res.dataSize-readSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}
	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendGetReserve2(DWORD reserveID, RESERVE_DATA* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_GET_RESERVE2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, reserveID)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, reserveID, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		DWORD readSize = 0;
		if( ReadVALUE2(ver, &ver, res.data, res.dataSize, &readSize) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
		if( ReadVALUE2(ver, val, res.data+readSize, res.dataSize-readSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}
	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendAddReserve2(vector<RESERVE_DATA>* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_ADD_RESERVE2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendChgReserve2(vector<RESERVE_DATA>* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_CHG_RESERVE2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�\��ǉ����\���m�F����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]�\����
// resVal			[OUT]�ǉ��\���̃X�e�[�^�X
DWORD CSendCtrlCmd::SendAddChkReserve2(RESERVE_DATA* val, WORD* resVal)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_ADDCHK_RESERVE2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		DWORD readSize = 0;
		if( ReadVALUE2(ver, &ver, res.data, res.dataSize, &readSize) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
		if( ReadVALUE2(ver, resVal, res.data+readSize, res.dataSize-readSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}


//EPG�f�[�^�t�@�C���̃^�C���X�^���v�擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// val				[IN]�擾�t�@�C����
// resVal			[OUT]�^�C���X�^���v
DWORD CSendCtrlCmd::SendGetEpgFileTime2(wstring val, LONGLONG* resVal)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_GET_EPG_FILETIME2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		DWORD readSize = 0;
		if( ReadVALUE2(ver, &ver, res.data, res.dataSize, &readSize) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
		if( ReadVALUE2(ver, resVal, res.data+readSize, res.dataSize-readSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//EPG�f�[�^�t�@�C���擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�t�@�C����
// resVal		[OUT]�t�@�C���̃o�C�i���f�[�^
// resValSize	[OUT]resVal�̃T�C�Y
DWORD CSendCtrlCmd::SendGetEpgFile2(
	wstring val,
	BYTE** resVal,
	DWORD* resValSize
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_GET_EPG_FILE2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		DWORD readSize = 0;
		if( ReadVALUE2(ver, &ver, res.data, res.dataSize, &readSize) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
		if( res.dataSize - readSize <= 0 ){
			UnLock();
			return CMD_ERR;
		}
		*resValSize = res.dataSize - readSize;
		*resVal = new BYTE[*resValSize];
		memcpy(*resVal, res.data + readSize, *resValSize);
	}

	UnLock();
	return ret;
}

//�����\��o�^�����ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[OUT]�����ꗗ
DWORD CSendCtrlCmd::SendEnumEpgAutoAdd2(
	vector<EPG_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;

	send.param = CMD2_EPG_SRV_ENUM_AUTO_ADD2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	if( ret == CMD_SUCCESS ){
		DWORD readSize = 0;
		if( ReadVALUE2(ver, &ver, res.data, res.dataSize, &readSize) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
		if( ReadVALUE2(ver, val, res.data+readSize, res.dataSize-readSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}
	UnLock();
	return ret;
}

//�����\��o�^������ǉ�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�����ꗗ
DWORD CSendCtrlCmd::SendAddEpgAutoAdd2(
	vector<EPG_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_ADD_AUTO_ADD2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�����\��o�^������ύX����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�����ꗗ
DWORD CSendCtrlCmd::SendChgEpgAutoAdd2(
	vector<EPG_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_CHG_AUTO_ADD2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�����\��o�^�����ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[OUT]�����ꗗ	
DWORD CSendCtrlCmd::SendEnumManualAdd2(
	vector<MANUAL_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;

	send.param = CMD2_EPG_SRV_ENUM_MANU_ADD2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	if( ret == CMD_SUCCESS ){
		DWORD readSize = 0;
		if( ReadVALUE2(ver, &ver, res.data, res.dataSize, &readSize) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
		if( ReadVALUE2(ver, val, res.data+readSize, res.dataSize-readSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}
	UnLock();
	return ret;
}

//�����\��o�^������ǉ�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�����ꗗ
DWORD CSendCtrlCmd::SendAddManualAdd2(
	vector<MANUAL_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_ADD_MANU_ADD2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�v���O�����\�񎩓��o�^�̏����ύX
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[IN]�����ꗗ
DWORD CSendCtrlCmd::SendChgManualAdd2(
	vector<MANUAL_AUTO_ADD_DATA>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_CHG_MANU_ADD2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�����\��o�^�����ꗗ���擾����
//�߂�l�F
// �G���[�R�[�h
//�����F
// val			[OUT]�����ꗗ	
DWORD CSendCtrlCmd::SendEnumRecInfo2(
	vector<REC_FILE_INFO>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;

	send.param = CMD2_EPG_SRV_ENUM_RECINFO2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	if( ret == CMD_SUCCESS ){
		DWORD readSize = 0;
		if( ReadVALUE2(ver, &ver, res.data, res.dataSize, &readSize) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
		if( ReadVALUE2(ver, val, res.data+readSize, res.dataSize-readSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}
	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendChgProtectRecInfo2(vector<REC_FILE_INFO>* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_EPG_SRV_CHG_PROTECT_RECINFO2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�_�C�A���O��O�ʂɕ\��
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendGUIShowDlg(
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_TIMER_GUI_SHOW_DLG;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�\��ꗗ�̏�񂪍X�V���ꂽ
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendGUIUpdateReserve(
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_TIMER_GUI_UPDATE_RESERVE;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//EPG�f�[�^�̍ēǂݍ��݂���������
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendGUIUpdateEpgData(
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_TIMER_GUI_UPDATE_EPGDATA;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

DWORD CSendCtrlCmd::SendGUINotifyInfo2(NOTIFY_SRV_INFO* val)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;

	CMD_STREAM send;
	CMD_STREAM res;

	WORD ver = (WORD)CMD_VER;
	DWORD writeSize = 0;

	send.param = CMD2_TIMER_GUI_SRV_STATUS_NOTIFY2;
	send.dataSize = 0;

	send.dataSize = GetVALUESize2(ver, val)+GetVALUESize2(ver, ver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE2(ver, ver, send.data, send.dataSize, &writeSize) == FALSE ){
		UnLock();
		return CMD_ERR;
	}
	if( WriteVALUE2(ver, val, send.data+writeSize, send.dataSize-writeSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//View�A�v���iEpgDataCap_Bon.exe�j���N��
//�߂�l�F
// �G���[�R�[�h
//�����F
// exeCmd			[IN]�R�}���h���C��
// PID				[OUT]�N������exe��PID
DWORD CSendCtrlCmd::SendGUIExecute(
	wstring exeCmd,
	DWORD* PID
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_TIMER_GUI_VIEW_EXECUTE;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(exeCmd);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(exeCmd, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(PID, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�X�^���o�C�A�x�~�A�V���b�g�_�E���ɓ����Ă������̊m�F�����[�U�[�ɍs��
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendGUIQuerySuspend(
	BYTE rebootFlag,
	BYTE suspendMode
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	WORD param = ((WORD)rebootFlag)<<8 | suspendMode;

	send.param = CMD2_TIMER_GUI_QUERY_SUSPEND;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(param);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(param, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//PC�ċN���ɓ����Ă������̊m�F�����[�U�[�ɍs��
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendGUIQueryReboot(
	BYTE rebootFlag
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	WORD param = ((WORD)rebootFlag)<<8;

	send.param = CMD2_TIMER_GUI_QUERY_REBOOT;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(param);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(param, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�T�[�o�[�̃X�e�[�^�X�ύX�ʒm
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendGUIStatusChg(
	WORD status
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_TIMER_GUI_SRV_STATUS_CHG;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(status);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(status, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//BonDriver�̐؂�ւ�
//�߂�l�F
// �G���[�R�[�h
//�����F
// bonDriver			[IN]BonDriver�t�@�C����
DWORD CSendCtrlCmd::SendViewSetBonDrivere(
	wstring bonDriver
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_SET_BONDRIVER;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(bonDriver);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(bonDriver, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�g�p����BonDriver�̃t�@�C�������擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// bonDriver			[OUT]BonDriver�t�@�C����
DWORD CSendCtrlCmd::SendViewGetBonDrivere(
	wstring* bonDriver
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_GET_BONDRIVER;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(bonDriver, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�`�����l���؂�ւ�
//�߂�l�F
// �G���[�R�[�h
//�����F
// chInfo				[OUT]�`�����l�����
DWORD CSendCtrlCmd::SendViewSetCh(
	SET_CH_INFO* chInfo
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_SET_CH;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(chInfo);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(chInfo, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//�����g�̎��Ԃ�PC���Ԃ̌덷�擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// delaySec				[OUT]�덷�i�b�j
DWORD CSendCtrlCmd::SendViewGetDelay(
	int* delaySec
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_GET_DELAY;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(delaySec, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//���݂̏�Ԃ��擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// status				[OUT]���
DWORD CSendCtrlCmd::SendViewGetStatus(
	DWORD* status
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_GET_STATUS;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(status, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//���݂̏�Ԃ��擾
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendViewAppClose(
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_CLOSE;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//���ʗpID�̐ݒ�
//�߂�l�F
// �G���[�R�[�h
//�����F
// id				[IN]ID
DWORD CSendCtrlCmd::SendViewSetID(
	int id
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_SET_ID;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(id);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(id, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}


	UnLock();
	return ret;
}

//���ʗpID�̎擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// id				[OUT]ID
DWORD CSendCtrlCmd::SendViewGetID(
	int* id
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_GET_ID;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(id, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�\��^��p��GUI�L�[�v
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendViewSetStandbyRec(
	DWORD keepFlag
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_SET_STANDBY_REC;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(keepFlag);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(keepFlag, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�X�g���[������p�R���g���[���쐬
//�߂�l�F
// �G���[�R�[�h
//�����F
// ctrlID				[OUT]����ID
DWORD CSendCtrlCmd::SendViewCreateCtrl(
	DWORD* ctrlID
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_CREATE_CTRL;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(ctrlID, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�X�g���[������p�R���g���[���쐬
//�߂�l�F
// �G���[�R�[�h
//�����F
// ctrlID				[IN]����ID
DWORD CSendCtrlCmd::SendViewDeleteCtrl(
	DWORD ctrlID
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_DELETE_CTRL;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(ctrlID);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(ctrlID, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//����R���g���[���̐ݒ�
//�߂�l�F
// �G���[�R�[�h
//�����F
// val					[IN]�ݒ�l
DWORD CSendCtrlCmd::SendViewSetCtrlMode(
	SET_CTRL_MODE val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_SET_CTRLMODE;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(&val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(&val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�^�揈���J�n
//�߂�l�F
// �G���[�R�[�h
//�����F
// val					[IN]�ݒ�l
DWORD CSendCtrlCmd::SendViewStartRec(
	SET_CTRL_REC_PARAM val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_REC_START_CTRL;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(&val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(&val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�^�揈���J�n
//�߂�l�F
// �G���[�R�[�h
//�����F
// val					[IN]�ݒ�l
DWORD CSendCtrlCmd::SendViewStopRec(
	SET_CTRL_REC_STOP_PARAM val,
	SET_CTRL_REC_STOP_RES_PARAM* resVal
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_REC_STOP_CTRL;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(&val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(&val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(resVal, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�^�撆�̃t�@�C���p�X���擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// val					[OUT]�t�@�C���p�X
DWORD CSendCtrlCmd::SendViewGetRecFilePath(
	DWORD ctrlID,
	wstring* resVal
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_REC_FILE_PATH;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(ctrlID);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(ctrlID, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(resVal, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//�^�揈���J�n
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendViewStopRecAll(
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_REC_STOP_ALL;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�t�@�C���o�͂����T�C�Y���擾
//�߂�l�F
// �G���[�R�[�h
//�����F
// resVal					[OUT]�t�@�C���o�͂����T�C�Y
DWORD CSendCtrlCmd::SendViewGetWriteSize(
	DWORD ctrlID,
	__int64* resVal
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_REC_WRITE_SIZE;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(ctrlID);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(ctrlID, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(resVal, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//EPG�擾�J�n
//�߂�l�F
// �G���[�R�[�h
//�����F
// val					[IN]�擾�`�����l�����X�g
DWORD CSendCtrlCmd::SendViewEpgCapStart(
	vector<SET_CH_INFO>* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_EPGCAP_START;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//EPG�擾�L�����Z��
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendViewEpgCapStop(
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_EPGCAP_STOP;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//EPG�f�[�^�̌���
//�߂�l�F
// �G���[�R�[�h
// val					[IN]�擾�ԑg
// resVal				[OUT]�ԑg���
DWORD CSendCtrlCmd::SendViewSearchEvent(
	SEARCH_EPG_INFO_PARAM* val,
	EPGDB_EVENT_INFO* resVal
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_SEARCH_EVENT;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(resVal, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//����or���̔ԑg�����擾����
//�߂�l�F
// �G���[�R�[�h
// val					[IN]�擾�ԑg
// resVal				[OUT]�ԑg���
DWORD CSendCtrlCmd::SendViewGetEventPF(
	GET_EPG_PF_INFO_PARAM* val,
	EPGDB_EVENT_INFO* resVal
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_GET_EVENT_PF;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	if( ret == CMD_SUCCESS ){
		if( ReadVALUE(resVal, res.data, res.dataSize, NULL) == FALSE ){
			UnLock();
			return CMD_ERR;
		}
	}

	UnLock();
	return ret;
}

//View�{�^���o�^�A�v���N��
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendViewExecViewApp(
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_EXEC_VIEW_APP;
	send.dataSize = 0;

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

//�X�g���[�~���O�z�M����ID�̐ݒ�
//�߂�l�F
// �G���[�R�[�h
DWORD CSendCtrlCmd::SendViewSetStreamingInfo(
	TVTEST_STREAMING_INFO* val
	)
{
	if( Lock() == FALSE ) return CMD_ERR_TIMEOUT;
	DWORD ret = CMD_ERR;
	CMD_STREAM send;
	CMD_STREAM res;

	send.param = CMD2_VIEW_APP_TT_SET_CTRL;
	send.dataSize = 0;

	send.dataSize = GetVALUESize(val);
	send.data = new BYTE[send.dataSize];
	if( WriteVALUE(val, send.data, send.dataSize, NULL) == FALSE ){
		UnLock();
		return CMD_ERR;
	}

	if( this->tcpFlag == FALSE ){
		ret = SendPipe(this->pipeName.c_str(), this->eventName.c_str(), this->connectTimeOut, &send, &res);
	}else{
		ret = SendTCP(this->ip.c_str(), this->port, this->connectTimeOut, &send, &res);
	}

	UnLock();
	return ret;
}

