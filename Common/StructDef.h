#ifndef __STRUCT_DEF_H__
#define __STRUCT_DEF_H__

#include "Util.h"

//�^��t�H���_���
typedef struct _REC_FILE_SET_INFO{
	wstring recFolder;			//�^��t�H���_
	wstring writePlugIn;		//�o��PlugIn
	wstring recNamePlugIn;		//�t�@�C�����ϊ�PlugIn�̎g�p
	wstring recFileName;		//�t�@�C�����ʑΉ� �^��J�n�������ɓ����Ŏg�p�B�\����Ƃ��Ă͕K�v�Ȃ�
	_REC_FILE_SET_INFO & operator= (const _REC_FILE_SET_INFO & o) {
		recFolder = o.recFolder;
		writePlugIn = o.writePlugIn;
		recNamePlugIn = o.recNamePlugIn;
		recFileName = o.recFileName;
		return *this;
	};
}REC_FILE_SET_INFO;

//�^��ݒ���
typedef struct _REC_SETTING_DATA{
	BYTE recMode;				//�^�惂�[�h
	BYTE priority;				//�D��x
	BYTE tuijyuuFlag;			//�Ǐ]���[�h
	DWORD serviceMode;			//�����Ώۃf�[�^���[�h
	BYTE pittariFlag;			//�҂�����H�^��
	wstring batFilePath;		//�^���BAT�t�@�C���p�X
	vector<REC_FILE_SET_INFO> recFolderList;		//�^��t�H���_�p�X
	BYTE suspendMode;			//�x�~���[�h
	BYTE rebootFlag;			//�^���ċN������
	BYTE useMargineFlag;		//�^��}�[�W�����ʎw��
	INT startMargine;			//�^��J�n���̃}�[�W��
	INT endMargine;				//�^��I�����̃}�[�W��
	BYTE continueRecFlag;		//�㑱����T�[�r�X���A����t�@�C���Ř^��
	BYTE partialRecFlag;		//����CH�ɕ�����M�T�[�r�X������ꍇ�A�����^�悷�邩�ǂ���
	DWORD tunerID;				//�����I�Ɏg�pTuner���Œ�
	//CMD_VER 2�ȍ~
	vector<REC_FILE_SET_INFO> partialRecFolder;	//������M�T�[�r�X�^��̃t�H���_
	//=�I�y���[�^�[�̏���
	_REC_SETTING_DATA(void){
		recMode = 1;
		priority = 1;
		tuijyuuFlag = 1;
		serviceMode = 0;
		pittariFlag = FALSE;
		batFilePath = L"";
		suspendMode = 0;
		rebootFlag = FALSE;
		useMargineFlag = FALSE;
		startMargine = 10;
		endMargine = 5;
		continueRecFlag = 0;
		partialRecFlag = 0;
		tunerID = 0;
	};
	~_REC_SETTING_DATA(void){
		recFolderList.clear();
		partialRecFolder.clear();
	}
	_REC_SETTING_DATA & operator= (const _REC_SETTING_DATA & o) {
		recMode = o.recMode;
		priority = o.priority;
		tuijyuuFlag = o.tuijyuuFlag;
		serviceMode = o.serviceMode;
		pittariFlag = o.pittariFlag;
		batFilePath = o.batFilePath;
		recFolderList = o.recFolderList;
		suspendMode = o.suspendMode;
		rebootFlag = o.rebootFlag;
		useMargineFlag = o.useMargineFlag;
		startMargine = o.startMargine;
		endMargine = o.endMargine;
		continueRecFlag = o.continueRecFlag;
		partialRecFlag = o.partialRecFlag;
		tunerID = o.tunerID;
		partialRecFolder = o.partialRecFolder;
		return *this;
	};
} REC_SETTING_DATA;

//�o�^�\����
typedef struct _RESERVE_DATA{
	wstring title;					//�ԑg��
	SYSTEMTIME startTime;			//�^��J�n����
	DWORD durationSecond;			//�^�摍����
	wstring stationName;			//�T�[�r�X��
	WORD originalNetworkID;			//ONID
	WORD transportStreamID;			//TSID
	WORD serviceID;					//SID
	WORD eventID;					//EventID
	wstring comment;				//�R�����g
	DWORD reserveID;				//�\�񎯕�ID �\��o�^����0
	BYTE recWaitFlag;				//�\��ҋ@�������H �����Ŏg�p
	BYTE overlapMode;				//���Ԃ��� 1:���Ԃ��ă`���[�i�[����Ȃ��\�񂠂� 2:�`���[�i�[����Ȃ��ė\��ł��Ȃ�
	wstring recFilePath;			//�^��t�@�C���p�X ���o�[�W�����݊��p ���g�p
	SYSTEMTIME startTimeEpg;		//�\�񎞂̊J�n����
	REC_SETTING_DATA recSetting;	//�^��ݒ�
	DWORD reserveStatus;			//�\��ǉ���� �����Ŏg�p
	//CMD_VER 5�ȍ~
	vector<wstring> recFileNameList;	//�^��\��t�@�C����
	DWORD param1;					//�����p
	//=�I�y���[�^�[�̏���
	_RESERVE_DATA(void){
		title=L"";
		ZeroMemory(&startTime, sizeof(SYSTEMTIME));
		durationSecond = 0;
		stationName = L"";
		originalNetworkID = 0;
		transportStreamID = 0;
		serviceID = 0;
		eventID = 0;
		comment = L"";
		reserveID = 0;
		recWaitFlag = FALSE;
		overlapMode = 0;
		ZeroMemory(&startTimeEpg, sizeof(SYSTEMTIME));
		recFilePath = L"";
		reserveStatus = 0;
	};
	~_RESERVE_DATA(void){
	}
	_RESERVE_DATA & operator= (const _RESERVE_DATA & o) {
		title=o.title;
		startTime = o.startTime;
		durationSecond = o.durationSecond;
		stationName = o.stationName;
		originalNetworkID = o.originalNetworkID;
		transportStreamID = o.transportStreamID;
		serviceID = o.serviceID;
		eventID = o.eventID;
		comment = o.comment;
		reserveID = o.reserveID;
		recWaitFlag = o.recWaitFlag;
		overlapMode = o.overlapMode;
		recFilePath = o.recFilePath;
		startTimeEpg = o.startTimeEpg;
		recSetting  = o.recSetting;
		reserveStatus = o.reserveStatus;
		return *this;
	};
} RESERVE_DATA;

typedef struct _REC_FILE_INFO{
	DWORD id;					//ID
	wstring recFilePath;		//�^��t�@�C���p�X
	wstring title;				//�ԑg��
	SYSTEMTIME startTime;		//�J�n����
	DWORD durationSecond;		//�^�掞��
	wstring serviceName;		//�T�[�r�X��
	WORD originalNetworkID;		//ONID
	WORD transportStreamID;		//TSID
	WORD serviceID;				//SID
	WORD eventID;				//EventID
	__int64 drops;				//�h���b�v��
	__int64 scrambles;			//�X�N�����u����
	DWORD recStatus;			//�^�挋�ʂ̃X�e�[�^�X
	SYSTEMTIME startTimeEpg;	//�\�񎞂̊J�n����
	wstring comment;			//�R�����g
	wstring programInfo;		//.program.txt�t�@�C���̓��e
	wstring errInfo;			//.err�t�@�C���̓��e
	//CMD_VER 4�ȍ~
	BYTE protectFlag;
	//=�I�y���[�^�[�̏���
	_REC_FILE_INFO(void){
		id = 0;
		recFilePath = L"";
		title = L"";
		ZeroMemory(&startTime, sizeof(SYSTEMTIME));
		durationSecond = 0;
		serviceName = L"";
		originalNetworkID = 0;
		transportStreamID = 0;
		serviceID = 0;
		eventID = 0;
		drops = 0;
		scrambles = 0;
		recStatus = 0;
		ZeroMemory(&startTimeEpg, sizeof(SYSTEMTIME));
		comment = L"";
		programInfo = L"";
		errInfo = L"";
		protectFlag = 0;
	};
	~_REC_FILE_INFO(void){
	}
	_REC_FILE_INFO & operator= (const _REC_FILE_INFO & o) {
		id = o.id;
		recFilePath=o.recFilePath;
		title = o.title;
		startTime = o.startTime;
		durationSecond = o.durationSecond;
		serviceName = o.serviceName;
		originalNetworkID = o.originalNetworkID;
		transportStreamID = o.transportStreamID;
		serviceID = o.serviceID;
		eventID = o.eventID;
		drops = o.drops;
		scrambles = o.scrambles;
		recStatus = o.recStatus;
		startTimeEpg = o.startTimeEpg;
		comment = o.comment;
		programInfo = o.programInfo;
		errInfo = o.errInfo;
		protectFlag = o.protectFlag;
		return *this;
	};
	_REC_FILE_INFO & operator= (const _RESERVE_DATA & o) {
		id = 0;
		recFilePath=o.recFilePath;
		title = o.title;
		startTime = o.startTime;
		durationSecond = o.durationSecond;
		serviceName = o.stationName;
		originalNetworkID = o.originalNetworkID;
		transportStreamID = o.transportStreamID;
		serviceID = o.serviceID;
		eventID = o.eventID;
		drops = 0;
		scrambles = 0;
		recStatus = 0;
		startTimeEpg = o.startTimeEpg;
		comment = o.comment;
		programInfo = L"";
		errInfo = L"";
		protectFlag = 0;
		return *this;
	};
} REC_FILE_INFO;

typedef struct _TUNER_RESERVE_INFO{
	DWORD tunerID;
	wstring tunerName;
	vector<DWORD> reserveList;
	_TUNER_RESERVE_INFO & operator= (const _TUNER_RESERVE_INFO & o) {
		tunerID = o.tunerID;
		tunerName=o.tunerName;
		reserveList = o.reserveList;
		return *this;
	};
} TUNER_RESERVE_INFO;

//�`���[�i�[���T�[�r�X���
typedef struct _CH_DATA4{
	int space;						//�`���[�i�[���
	int ch;							//�����`�����l��
	WORD originalNetworkID;			//ONID
	WORD transportStreamID;			//TSID
	WORD serviceID;					//�T�[�r�XID
	WORD serviceType;				//�T�[�r�X�^�C�v
	BOOL partialFlag;				//������M�T�[�r�X�i�����Z�O�j���ǂ���
	BOOL useViewFlag;				//�ꗗ�\���Ɏg�p���邩�ǂ���
	wstring serviceName;			//�T�[�r�X��
	wstring chName;					//�`�����l����
	wstring networkName;			//ts_name or network_name
	BYTE remoconID;					//�����R��ID
	//=�I�y���[�^�[�̏���
	_CH_DATA4(void){
		space = 0;
		ch = 0;
		originalNetworkID = 0;
		transportStreamID = 0;
		serviceID = 0;
		serviceType = 0;
		partialFlag = FALSE;
		useViewFlag = TRUE;
		serviceName = L"";
		chName = L"";
		networkName = L"";
		remoconID = 0;
	};
	~_CH_DATA4(void){
	}
	_CH_DATA4 & operator= (const _CH_DATA4 & o) {
		space = o.space;
		ch = o.ch;
		originalNetworkID = o.originalNetworkID;
		transportStreamID = o.transportStreamID;
		serviceID = o.serviceID;
		serviceType = o.serviceType;
		partialFlag = o.partialFlag;
		useViewFlag = o.useViewFlag;
		serviceName = o.serviceName;
		chName = o.chName;
		networkName = o.networkName;
		remoconID = o.remoconID;
		return *this;
	}
} CH_DATA4;

//�S�`���[�i�[�ŔF�������T�[�r�X�ꗗ
typedef struct _CH_DATA5{
	WORD originalNetworkID;			//ONID
	WORD transportStreamID;			//TSID
	WORD serviceID;					//�T�[�r�XID
	WORD serviceType;				//�T�[�r�X�^�C�v
	BOOL partialFlag;				//������M�T�[�r�X�i�����Z�O�j���ǂ���
	wstring serviceName;			//�T�[�r�X��
	wstring networkName;			//ts_name or network_name
	BOOL epgCapFlag;				//EPG�f�[�^�擾�Ώۂ��ǂ���
	BOOL searchFlag;				//�������̃f�t�H���g�����ΏۃT�[�r�X���ǂ���
	//=�I�y���[�^�[�̏���
	_CH_DATA5(void){
		originalNetworkID = 0;
		transportStreamID = 0;
		serviceID = 0;
		serviceType = 0;
		partialFlag = FALSE;
		serviceName = L"";
		networkName = L"";
		epgCapFlag = TRUE;
		searchFlag = TRUE;
	};
	~_CH_DATA5(void){
	}
	_CH_DATA5 & operator= (const _CH_DATA5 & o) {
		originalNetworkID = o.originalNetworkID;
		transportStreamID = o.transportStreamID;
		serviceID = o.serviceID;
		serviceType = o.serviceType;
		partialFlag = o.partialFlag;
		serviceName = o.serviceName;
		networkName = o.networkName;
		epgCapFlag = o.epgCapFlag;
		searchFlag = o.searchFlag;
		return *this;
	}
} CH_DATA5;

typedef struct _REGIST_TCP_INFO{
	wstring ip;
	DWORD port;
	_REGIST_TCP_INFO & operator= (const _REGIST_TCP_INFO & o) {
		ip = o.ip;
		port = o.port;
		return *this;
	}
}REGIST_TCP_INFO;

//�R�}���h����M�X�g���[��
typedef struct _CMD_STREAM{
	DWORD param;	//���M���R�}���h�A��M���G���[�R�[�h
	DWORD dataSize;	//data�̃T�C�Y�iBYTE�P�ʁj
	BYTE* data;		//����M����o�C�i���f�[�^
	//=�I�y���[�^�[�̏���
	_CMD_STREAM(void){
		param = 0;
		dataSize = 0;
		data = NULL;
	}
	~_CMD_STREAM(void){
		SAFE_DELETE_ARRAY(data);
	}
} CMD_STREAM;

typedef struct _HTTP_STREAM{
	string httpHeader;	//���M���R�}���h�A��M���G���[�R�[�h
	DWORD dataSize;	//data�̃T�C�Y�iBYTE�P�ʁj
	BYTE* data;		//����M����o�C�i���f�[�^
	//=�I�y���[�^�[�̏���
	_HTTP_STREAM(void){
		httpHeader = "";
		dataSize = 0;
		data = NULL;
	}
	~_HTTP_STREAM(void){
		SAFE_DELETE_ARRAY(data);
	}
} HTTP_STREAM;

//EPG��{���
typedef struct _EPGDB_SHORT_EVENT_INFO{
	wstring event_name;			//�C�x���g��
	wstring text_char;			//���
	wstring search_event_name;	//�����g�p���̃C�x���g��
	wstring search_text_char;	//�����g�p���̏��
	_EPGDB_SHORT_EVENT_INFO & operator= (const _EPGDB_SHORT_EVENT_INFO & o) {
		event_name = o.event_name;
		text_char = o.text_char;
		search_event_name = o.search_event_name;
		search_text_char = o.search_text_char;
		return *this;
	};
} EPGDB_SHORT_EVENT_INFO;

//EPG�g�����
typedef struct _EPGDB_EXTENDED_EVENT_INFO{
	wstring text_char;			//�ڍ׏��
	wstring search_text_char;	//�����g�p���̏��
	_EPGDB_EXTENDED_EVENT_INFO & operator= (const _EPGDB_EXTENDED_EVENT_INFO & o) {
		text_char = o.text_char;
		search_text_char = o.search_text_char;
		return *this;
	};
} EPGDB_EXTENDED_EVENT_INFO;

//EPG�W�������f�[�^
typedef struct _EPGDB_CONTENT_DATA{
	BYTE content_nibble_level_1;
	BYTE content_nibble_level_2;
	BYTE user_nibble_1;
	BYTE user_nibble_2;
	_EPGDB_CONTENT_DATA & operator= (const _EPGDB_CONTENT_DATA & o) {
		content_nibble_level_1 = o.content_nibble_level_1;
		content_nibble_level_2 = o.content_nibble_level_2;
		user_nibble_1 = o.user_nibble_1;
		user_nibble_2 = o.user_nibble_2;
		return *this;
	};
}EPGDB_CONTENT_DATA;

//EPG�W���������
typedef struct _EPGDB_CONTENT_INFO{
	vector<EPGDB_CONTENT_DATA> nibbleList;
	_EPGDB_CONTENT_INFO & operator= (const _EPGDB_CONTENT_INFO & o) {
		nibbleList = o.nibbleList;
		return *this;
	};
} EPGDB_CONTEN_INFO;

//EPG�f�����
typedef struct _EPGDB_COMPONENT_INFO{
	BYTE stream_content;
	BYTE component_type;
	BYTE component_tag;
	wstring text_char;			//���
	_EPGDB_COMPONENT_INFO & operator= (const _EPGDB_COMPONENT_INFO & o) {
		stream_content = o.stream_content;
		component_type = o.component_type;
		component_tag = o.component_tag;
		text_char = o.text_char;
		return *this;
	};
} EPGDB_COMPONENT_INFO;

//EPG�������f�[�^
typedef struct _EPGDB_AUDIO_COMPONENT_INFO_DATA{
	BYTE stream_content;
	BYTE component_type;
	BYTE component_tag;
	BYTE stream_type;
	BYTE simulcast_group_tag;
	BYTE ES_multi_lingual_flag;
	BYTE main_component_flag;
	BYTE quality_indicator;
	BYTE sampling_rate;
	wstring text_char;			//�ڍ׏��
	_EPGDB_AUDIO_COMPONENT_INFO_DATA & operator= (const _EPGDB_AUDIO_COMPONENT_INFO_DATA & o) {
		stream_content = o.stream_content;
		component_type = o.component_type;
		component_tag = o.component_tag;
		stream_type = o.stream_type;
		simulcast_group_tag = o.simulcast_group_tag;
		ES_multi_lingual_flag = o.ES_multi_lingual_flag;
		main_component_flag = o.main_component_flag;
		quality_indicator = o.quality_indicator;
		sampling_rate = o.sampling_rate;
		text_char = o.text_char;
		return *this;
	};
} EPGDB_AUDIO_COMPONENT_INFO_DATA;

//EPG�������
typedef struct _EPGDB_AUDIO_COMPONENT_INFO{
	vector<EPGDB_AUDIO_COMPONENT_INFO_DATA> componentList;
	_EPGDB_AUDIO_COMPONENT_INFO & operator= (const _EPGDB_AUDIO_COMPONENT_INFO & o) {
		componentList = o.componentList;
		return *this;
	};
} EPGDB_AUDIO_COMPONENT_INFO;

//EPG�C�x���g�f�[�^
typedef struct _EPGDB_EVENT_DATA{
	WORD original_network_id;
	WORD transport_stream_id;
	WORD service_id;
	WORD event_id;
	_EPGDB_EVENT_DATA & operator= (const _EPGDB_EVENT_DATA & o) {
		original_network_id = o.original_network_id;
		transport_stream_id = o.transport_stream_id;
		service_id = o.service_id;
		event_id = o.event_id;
		return *this;
	};
}EPGDB_EVENT_DATA;

//EPG�C�x���g�O���[�v���
typedef struct _EPGDB_EVENTGROUP_INFO{
	BYTE group_type;
	vector<EPGDB_EVENT_DATA> eventDataList;
	_EPGDB_EVENTGROUP_INFO & operator= (const _EPGDB_EVENTGROUP_INFO & o) {
		group_type = o.group_type;
		eventDataList = o.eventDataList;
		return *this;
	};
} EPGDB_EVENTGROUP_INFO;

typedef struct _EPGDB_EVENT_INFO{
	WORD original_network_id;
	WORD transport_stream_id;
	WORD service_id;
	WORD event_id;							//�C�x���gID
	BYTE StartTimeFlag;						//start_time�̒l���L�����ǂ���
	SYSTEMTIME start_time;					//�J�n����
	BYTE DurationFlag;						//duration�̒l���L�����ǂ���
	DWORD durationSec;						//�����ԁi�P�ʁF�b�j

	EPGDB_SHORT_EVENT_INFO* shortInfo;		//��{���
	EPGDB_EXTENDED_EVENT_INFO* extInfo;		//�g�����
	EPGDB_CONTEN_INFO* contentInfo;			//�W���������
	EPGDB_COMPONENT_INFO* componentInfo;		//�f�����
	EPGDB_AUDIO_COMPONENT_INFO* audioInfo;	//�������
	EPGDB_EVENTGROUP_INFO* eventGroupInfo;	//�C�x���g�O���[�v���
	EPGDB_EVENTGROUP_INFO* eventRelayInfo;	//�C�x���g�����[���

	BYTE freeCAFlag;						//�m���X�N�����u���t���O
	_EPGDB_EVENT_INFO(void){
		shortInfo = NULL;
		extInfo = NULL;
		contentInfo = NULL;
		componentInfo = NULL;
		audioInfo = NULL;
		eventGroupInfo = NULL;
		eventRelayInfo = NULL;
	};
	~_EPGDB_EVENT_INFO(void){
		SAFE_DELETE(shortInfo);
		SAFE_DELETE(extInfo);
		SAFE_DELETE(contentInfo);
		SAFE_DELETE(componentInfo);
		SAFE_DELETE(audioInfo);
		SAFE_DELETE(eventGroupInfo);
		SAFE_DELETE(eventRelayInfo);
	};
}EPGDB_EVENT_INFO;

typedef struct _EPGDB_SERVICE_INFO{
	WORD ONID;
	WORD TSID;
	WORD SID;
	BYTE service_type;
	BYTE partialReceptionFlag;
	wstring service_provider_name;
	wstring service_name;
	wstring network_name;
	wstring ts_name;
	BYTE remote_control_key_id;
	_EPGDB_SERVICE_INFO(void){
		ONID = 0;
		TSID = 0;
		SID = 0;
		service_type = 0;
		partialReceptionFlag = 0;
		service_provider_name = L"";
		service_name = L"";
		network_name = L"";
		ts_name = L"";
		remote_control_key_id = 0;
	};
	~_EPGDB_SERVICE_INFO(void){
	};
	_EPGDB_SERVICE_INFO & operator= (const _EPGDB_SERVICE_INFO & o) {
		ONID = o.ONID;
		TSID = o.TSID;
		SID = o.SID;
		service_type = o.service_type;
		partialReceptionFlag = o.partialReceptionFlag;
		service_provider_name = o.service_provider_name;
		service_name = o.service_name;
		network_name = o.network_name;
		ts_name = o.ts_name;
		remote_control_key_id = o.remote_control_key_id;
		return *this;
	};
}EPGDB_SERVICE_INFO;

typedef struct _EPGDB_SERVICE_EVENT_INFO{
	EPGDB_SERVICE_INFO serviceInfo;
	vector<EPGDB_EVENT_INFO*> eventList;
	~_EPGDB_SERVICE_EVENT_INFO(void){
	};
	_EPGDB_SERVICE_EVENT_INFO & operator= (const _EPGDB_SERVICE_EVENT_INFO & o) {
		serviceInfo = o.serviceInfo;
		eventList = o.eventList;
		return *this;
	};
}EPGDB_SERVICE_EVENT_INFO;

typedef struct _EPGDB_SEARCH_DATE_INFO{
	BYTE startDayOfWeek;
	WORD startHour;
	WORD startMin;
	BYTE endDayOfWeek;
	WORD endHour;
	WORD endMin;
	_EPGDB_SEARCH_DATE_INFO & operator= (const _EPGDB_SEARCH_DATE_INFO & o) {
		startDayOfWeek = o.startDayOfWeek;
		startHour = o.startHour;
		startMin = o.startMin;
		endDayOfWeek = o.endDayOfWeek;
		endHour = o.endHour;
		endMin = o.endMin;
		return *this;
	};
} EPGDB_SEARCH_DATE_INFO;

//��������
typedef struct _EPGDB_SEARCH_KEY_INFO{
	wstring andKey;
	wstring notKey;
	BOOL regExpFlag;
	BOOL titleOnlyFlag;
	vector<EPGDB_CONTENT_DATA> contentList;
	vector<EPGDB_SEARCH_DATE_INFO> dateList;
	vector<__int64> serviceList;
	vector<WORD> videoList;
	vector<WORD> audioList;
	BYTE aimaiFlag;
	BYTE notContetFlag;
	BYTE notDateFlag;
	BYTE freeCAFlag;
	//CMD_VER 3�ȍ~
	//�����\��o�^�̏�����p
	BYTE chkRecEnd;					//�^��ς��̃`�F�b�N����
	WORD chkRecDay;					//�^��ς��̃`�F�b�N�Ώۊ���
	//=�I�y���[�^�[�̏���
	_EPGDB_SEARCH_KEY_INFO(void){
		andKey = L"";
		notKey = L"";
		regExpFlag = FALSE;
		titleOnlyFlag = FALSE;
		aimaiFlag = 0;
		notContetFlag = 0;
		notDateFlag = 0;
		freeCAFlag = 0;
		chkRecEnd = 0;
		chkRecDay = 6;
	};
	_EPGDB_SEARCH_KEY_INFO & operator= (const _EPGDB_SEARCH_KEY_INFO & o) {
		andKey = o.andKey;
		notKey = o.notKey;
		regExpFlag = o.regExpFlag;
		titleOnlyFlag = o.titleOnlyFlag;
		contentList = o.contentList;
		dateList = o.dateList;
		serviceList = o.serviceList;
		videoList = o.videoList;
		audioList = o.audioList;
		aimaiFlag = o.aimaiFlag;
		notContetFlag = o.notContetFlag;
		notDateFlag = o.notDateFlag;
		freeCAFlag = o.freeCAFlag;
		chkRecEnd = o.chkRecEnd;
		chkRecDay = o.chkRecDay;
		return *this;
	};
}EPGDB_SEARCH_KEY_INFO;

//�����\��o�^���
typedef struct _EPG_AUTO_ADD_DATA{
	DWORD dataID;
	EPGDB_SEARCH_KEY_INFO searchInfo;	//�����L�[
	REC_SETTING_DATA recSetting;	//�^��ݒ�
	DWORD addCount;		//�\��o�^��
	_EPG_AUTO_ADD_DATA & operator= (const _EPG_AUTO_ADD_DATA & o) {
		dataID = o.dataID;
		searchInfo = o.searchInfo;
		recSetting = o.recSetting;
		addCount = o.addCount;
		return *this;
	};
} EPG_AUTO_ADD_DATA;

typedef struct _MANUAL_AUTO_ADD_DATA{
	DWORD dataID;
	BYTE dayOfWeekFlag;				//�Ώۗj��
	DWORD startTime;				//�^��J�n���ԁi00:00��0�Ƃ��ĕb�P�ʁj
	DWORD durationSecond;			//�^�摍����
	wstring title;					//�ԑg��
	wstring stationName;			//�T�[�r�X��
	WORD originalNetworkID;			//ONID
	WORD transportStreamID;			//TSID
	WORD serviceID;					//SID
	REC_SETTING_DATA recSetting;	//�^��ݒ�
	_MANUAL_AUTO_ADD_DATA & operator= (const _MANUAL_AUTO_ADD_DATA & o) {
		dataID = o.dataID;
		dayOfWeekFlag = o.dayOfWeekFlag;
		startTime = o.startTime;
		durationSecond = o.durationSecond;
		title = o.title;
		stationName = o.stationName;
		originalNetworkID = o.originalNetworkID;
		transportStreamID = o.transportStreamID;
		serviceID = o.serviceID;
		recSetting = o.recSetting;
		return *this;
	};
} MANUAL_AUTO_ADD_DATA;

//�R�}���h���M�p
//�`�����l���ύX���
typedef struct _SET_CH_INFO{
	BOOL useSID;//wONID��wTSID��wSID�̒l���g�p�ł��邩�ǂ���
	WORD ONID;
	WORD TSID;
	WORD SID;
	BOOL useBonCh;//dwSpace��dwCh�̒l���g�p�ł��邩�ǂ���
	DWORD space;
	DWORD ch;
	//=�I�y���[�^�[�̏���
	_SET_CH_INFO & operator= (const _SET_CH_INFO & o) {
		useSID = o.useSID;
		ONID = o.ONID;
		TSID = o.TSID;
		SID = o.SID;
		useBonCh = o.useBonCh;
		space = o.space;
		ch = o.ch;
		return *this;
	};
}SET_CH_INFO;

typedef struct _SET_CTRL_MODE{
	DWORD ctrlID;
	WORD SID;
	BYTE enableScramble;
	BYTE enableCaption;
	BYTE enableData;
	//=�I�y���[�^�[�̏���
	_SET_CTRL_MODE & operator= (const _SET_CTRL_MODE & o) {
		ctrlID = o.ctrlID;
		SID = o.SID;
		enableScramble = o.enableScramble;
		enableCaption = o.enableCaption;
		enableData = o.enableData;
		return *this;
	};
} SET_CTRL_MODE;

typedef struct _SET_CTRL_REC_PARAM{
	DWORD ctrlID;
	wstring fileName;
	BYTE overWriteFlag;
	ULONGLONG createSize;
	vector<REC_FILE_SET_INFO> saveFolder;
	BYTE pittariFlag;
	WORD pittariONID;
	WORD pittariTSID;
	WORD pittariSID;
	WORD pittariEventID;
	//=�I�y���[�^�[�̏���
	_SET_CTRL_REC_PARAM & operator= (const _SET_CTRL_REC_PARAM & o) {
		ctrlID = o.ctrlID;
		fileName = o.fileName;
		overWriteFlag = o.overWriteFlag;
		createSize = o.createSize;
		saveFolder = o.saveFolder;
		pittariFlag = o.pittariFlag;
		pittariONID = o.pittariONID;
		pittariTSID = o.pittariTSID;
		pittariSID = o.pittariSID;
		pittariEventID = o.pittariEventID;
		return *this;
	};
} SET_CTRL_REC_PARAM;

typedef struct _SET_CTRL_REC_STOP_PARAM{
	DWORD ctrlID;
	BOOL saveErrLog;
	//=�I�y���[�^�[�̏���
	_SET_CTRL_REC_STOP_PARAM & operator= (const _SET_CTRL_REC_STOP_PARAM & o) {
		ctrlID = o.ctrlID;
		saveErrLog = o.saveErrLog;
		return *this;
	};
} SET_CTRL_REC_STOP_PARAM;

typedef struct _SET_CTRL_REC_STOP_RES_PARAM{
	wstring recFilePath;
	ULONGLONG drop;
	ULONGLONG scramble;
	BYTE subRecFlag;
	//=�I�y���[�^�[�̏���
	_SET_CTRL_REC_STOP_RES_PARAM & operator= (const _SET_CTRL_REC_STOP_RES_PARAM & o) {
		recFilePath = o.recFilePath;
		drop = o.drop;
		scramble = o.scramble;
		subRecFlag = o.subRecFlag;
		return *this;
	};
} SET_CTRL_REC_STOP_RES_PARAM;

typedef struct _SEARCH_EPG_INFO_PARAM{
	WORD ONID;
	WORD TSID;
	WORD SID;
	WORD eventID;
	BYTE pfOnlyFlag;
	//=�I�y���[�^�[�̏���
	_SEARCH_EPG_INFO_PARAM & operator= (const _SEARCH_EPG_INFO_PARAM & o) {
		ONID = o.ONID;
		TSID = o.TSID;
		SID = o.SID;
		eventID = o.eventID;
		pfOnlyFlag = o.pfOnlyFlag;
		return *this;
	};
} SEARCH_EPG_INFO_PARAM;

typedef struct _GET_EPG_PF_INFO_PARAM{
	WORD ONID;
	WORD TSID;
	WORD SID;
	BYTE pfNextFlag;
	//=�I�y���[�^�[�̏���
	_GET_EPG_PF_INFO_PARAM & operator= (const _GET_EPG_PF_INFO_PARAM & o) {
		ONID = o.ONID;
		TSID = o.TSID;
		SID = o.SID;
		pfNextFlag = o.pfNextFlag;
		return *this;
	};
} GET_EPG_PF_INFO_PARAM;

typedef struct _TVTEST_CH_CHG_INFO{
	wstring bonDriver;
	SET_CH_INFO chInfo;
	//=�I�y���[�^�[�̏���
	_TVTEST_CH_CHG_INFO & operator= (const _TVTEST_CH_CHG_INFO & o) {
		bonDriver = o.bonDriver;
		chInfo = o.chInfo;
		return *this;
	};
} TVTEST_CH_CHG_INFO;


typedef struct _TVTEST_STREAMING_INFO{
	BOOL enableMode;
	DWORD ctrlID;
	DWORD serverIP;
	DWORD serverPort;
	wstring filePath;
	BOOL udpSend;
	BOOL tcpSend;
	BOOL timeShiftMode;
	//=�I�y���[�^�[�̏���
	_TVTEST_STREAMING_INFO & operator= (const _TVTEST_STREAMING_INFO & o) {
		enableMode = o.enableMode;
		ctrlID = o.ctrlID;
		serverIP = o.serverIP;
		serverPort = o.serverPort;
		filePath = o.filePath;
		udpSend = o.udpSend;
		tcpSend = o.tcpSend;
		timeShiftMode = o.timeShiftMode;
		return *this;
	};
} TVTEST_STREAMING_INFO;

typedef struct _NWPLAY_PLAY_INFO{
	DWORD ctrlID;
	DWORD ip;
	BYTE udp;
	BYTE tcp;
	DWORD udpPort;//out�Ŏ��ۂ̊J�n�|�[�g
	DWORD tcpPort;//out�Ŏ��ۂ̊J�n�|�[�g
	//=�I�y���[�^�[�̏���
	_NWPLAY_PLAY_INFO & operator= (const _NWPLAY_PLAY_INFO & o) {
		ctrlID = o.ctrlID;
		ip = o.ip;
		udp = o.udp;
		tcp = o.tcp;
		udpPort = o.udpPort;
		tcpPort = o.tcpPort;
		return *this;
	};
} NWPLAY_PLAY_INFO;

typedef struct _NWPLAY_POS_CMD{
	DWORD ctrlID;
	__int64 currentPos;
	__int64 totalPos;//CMD2_EPG_SRV_NWPLAY_SET_POS���͖���
	//=�I�y���[�^�[�̏���
	_NWPLAY_POS_CMD & operator= (const _NWPLAY_POS_CMD & o) {
		ctrlID = o.ctrlID;
		currentPos = o.currentPos;
		totalPos = o.totalPos;
		return *this;
	};
} NWPLAY_POS_CMD;

typedef struct _NWPLAY_TIMESHIFT_INFO{
	DWORD ctrlID;
	wstring filePath;
	//=�I�y���[�^�[�̏���
	_NWPLAY_TIMESHIFT_INFO & operator= (const _NWPLAY_TIMESHIFT_INFO & o) {
		ctrlID = o.ctrlID;
		filePath = o.filePath;
		return *this;
	};
} NWPLAY_TIMESHIFT_INFO;

//���ʒm�p�p�����[�^�[
typedef struct _NOTIFY_SRV_INFO{
	DWORD notifyID;		//�ʒm���̎��
	SYSTEMTIME time;	//�ʒm��Ԃ̔�����������
	DWORD param1;		//�p�����[�^�[�P�i��ނɂ���ē��e�ύX�j
	DWORD param2;		//�p�����[�^�[�Q�i��ނɂ���ē��e�ύX�j
	DWORD param3;		//�p�����[�^�[�R�i��ނɂ���ē��e�ύX�j
	wstring param4;		//�p�����[�^�[�S�i��ނɂ���ē��e�ύX�j
	wstring param5;		//�p�����[�^�[�T�i��ނɂ���ē��e�ύX�j
	wstring param6;		//�p�����[�^�[�U�i��ނɂ���ē��e�ύX�j
	//=�I�y���[�^�[�̏���
	_NOTIFY_SRV_INFO(void){
		notifyID= 0;
		param1 = 0;
		param2 = 0;
		param3 = 0;
	};
	_NOTIFY_SRV_INFO & operator= (const _NOTIFY_SRV_INFO & o) {
		notifyID = o.notifyID;
		time = o.time;
		param1 = o.param1;
		param2 = o.param2;
		param3 = o.param3;
		param4 = o.param4;
		param5 = o.param5;
		param6 = o.param6;
		return *this;
	};
} NOTIFY_SRV_INFO;


//�A�g�T�[�o�[���
typedef struct _COOP_SERVER_INFO{
	wstring hostName;	//�A�h���X
	WORD srvPort;		//�T�[�o�[�A�v���҂��󂯃|�[�g
	BOOL wolFlag;		//WOL�ɂ��N�����s��
	BYTE mac[6];		//�T�[�o�[MAC�A�h���X
	WORD magicSendPort;	//�}�W�b�N�p�P�b�g���M�p�|�[�g�i0��LAN���u���[�h�L���X�g�A0�ȊO��WAN�ւ̑��M�j
	BYTE suspendMode;	//�o�^��̃T�X�y���h����
	//=�I�y���[�^�[�̏���
	_COOP_SERVER_INFO(void){
		hostName= L"";
		srvPort = 0;
		wolFlag = FALSE;
		memset(mac, 0, 6);
		magicSendPort = 0;
		suspendMode = 0;
	};
	_COOP_SERVER_INFO & operator= (const _COOP_SERVER_INFO & o) {
		hostName= o.hostName;
		srvPort = o.srvPort;
		wolFlag = o.wolFlag;
		memcpy(mac, o.mac, 6);
		magicSendPort = o.magicSendPort;
		suspendMode = o.suspendMode;
		return *this;
	};
} COOP_SERVER_INFO;

typedef struct _GENRU_INFO{
	BYTE nibble1;
	BYTE nibble2;
	WORD key;
	wstring name;
	//=�I�y���[�^�[�̏���
	_GENRU_INFO(void){
		nibble1= 0xFF;
		nibble2 = 0xFF;
		key = 0xFFFF;
		name = L"";
	};
} GENRU_INFO;


////////////////////////////////////////////////////////////////////////////////////////////
//���o�[�W�����R�}���h���M�p
typedef struct _OLD_RESERVE_DATA{
	wstring strTitle;
	SYSTEMTIME StartTime;
	DWORD dwDurationSec;
	wstring strStationName;
	unsigned short usONID;
	unsigned short usTSID;
	unsigned short usServiceID;
	unsigned short usEventID;
	unsigned char ucPriority;
	unsigned char ucTuijyuu;
	wstring strComment;
	DWORD dwRecMode;
	BOOL bPittari;
	wstring strBatPath;
	DWORD dwReserveID; //����ԑg���ʗpID
	BOOL bSetWait; //�\��ҋ@�������H
	DWORD dwPiledUpMode; //���Ԃ��� 1:���Ԃ��ă`���[�i�[����Ȃ��\�񂠂� 2:�`���[�i�[����Ȃ��ė\��ł��Ȃ�
	wstring strRecFolder;
	WORD wSuspendMode;
	BOOL bReboot;
	wstring strRecFilePath;
	BOOL bUseMargine;
	int iStartMargine;
	int iEndMargine;
	DWORD dwServiceMode;
	//=�I�y���[�^�[�̏���
	_OLD_RESERVE_DATA & operator= (const _OLD_RESERVE_DATA & o) {
		strTitle=o.strTitle;
		StartTime = o.StartTime;
		dwDurationSec = o.dwDurationSec;
		strStationName = o.strStationName;
		usONID = o.usONID;
		usTSID = o.usTSID;
		usServiceID = o.usServiceID;
		usEventID = o.usEventID;
		ucPriority = o.ucPriority;
		ucTuijyuu = o.ucTuijyuu;
		strComment = o.strComment;
		dwRecMode = o.dwRecMode;
		bPittari = o.bPittari;
		strBatPath = o.strBatPath;
		dwReserveID = o.dwReserveID;
		bSetWait = o.bSetWait;
		dwPiledUpMode = o.dwPiledUpMode;
		strRecFolder = o.strRecFolder;
		wSuspendMode = o.wSuspendMode;
		bReboot = o.bReboot;
		strRecFilePath = o.strRecFilePath;
		bUseMargine = o.bUseMargine;
		iStartMargine = o.iStartMargine;
		iEndMargine = o.iEndMargine;
		dwServiceMode = o.dwServiceMode;
		return *this;
	};
	_OLD_RESERVE_DATA & operator= (const _RESERVE_DATA & o) {
		strTitle=o.title;
		StartTime = o.startTime;
		dwDurationSec = o.durationSecond;
		strStationName = o.stationName;
		usONID = o.originalNetworkID;
		usTSID = o.transportStreamID;
		usServiceID = o.serviceID;
		usEventID = o.eventID;
		ucPriority = o.recSetting.priority;
		ucTuijyuu = o.recSetting.tuijyuuFlag;
		strComment = o.comment;
		dwRecMode = o.recSetting.recMode;
		bPittari = o.recSetting.pittariFlag;
		strBatPath = o.recSetting.batFilePath;
		dwReserveID = o.reserveID;
		bSetWait = 0;
		dwPiledUpMode = o.overlapMode;
		if( o.recSetting.recFolderList.size() >0 ){
			strRecFolder = o.recSetting.recFolderList[0].recFolder;
		}else{
			strRecFolder = L"";
		}
		wSuspendMode = o.recSetting.suspendMode;
		bReboot = o.recSetting.rebootFlag;
		strRecFilePath = o.recFilePath;
		bUseMargine = o.recSetting.useMargineFlag;
		iStartMargine = o.recSetting.startMargine;
		iEndMargine = o.recSetting.endMargine;
		dwServiceMode = o.recSetting.serviceMode;
		return *this;
	};
} OLD_RESERVE_DATA;


typedef struct _OLD_SEARCH_KEY{
	wstring strAnd;
	wstring strNot;
	BOOL bTitle;
	int iJanru;
	int iSH;
	int iSM;
	int iEH;
	int iEM;
	BOOL bChkMon;
	BOOL bChkTue;
	BOOL bChkWed;
	BOOL bChkThu;
	BOOL bChkFri;
	BOOL bChkSat;
	BOOL bChkSun;
	vector<__int64> CHIDList; //ONID<<24 | TSID<<16 | SID
	//�ȉ������\��o�^���֌W�̂ݎg�p
	int iAutoAddID; //�����\��o�^�ꗗ�̎��ʗp�L�[
	int iPriority;
	int iTuijyuu;
	int iRecMode;
	int iPittari;
	wstring strBat;
	wstring strRecFolder;
	WORD wSuspendMode;
	BOOL bReboot;
	BOOL bUseMargine;
	int iStartMargine;
	int iEndMargine;
	DWORD dwServiceMode;

	BOOL bRegExp;
	wstring strPattern;
	//=�I�y���[�^�[�̏���
	_OLD_SEARCH_KEY & operator= (const _OLD_SEARCH_KEY & o) {
		strAnd = o.strAnd;
		strNot = o.strNot;
		bTitle = o.bTitle;
		iJanru = o.iJanru;
		iSH = o.iSH;
		iSM = o.iSM;
		iEH = o.iEH;
		iEM = o.iEM;
		bChkMon = o.bChkMon;
		bChkTue = o.bChkTue;
		bChkWed = o.bChkWed;
		bChkThu = o.bChkThu;
		bChkFri = o.bChkFri;
		bChkSat = o.bChkSat;
		bChkSun = o.bChkSun;
		CHIDList = o.CHIDList;
		iAutoAddID = o.iAutoAddID;
		iPriority = o.iPriority;
		iTuijyuu = o.iTuijyuu;
		iRecMode = o.iRecMode;
		iPittari = o.iPittari;
		strBat = o.strBat;
		strRecFolder = o.strRecFolder;
		wSuspendMode = o.wSuspendMode;
		bReboot = o.bReboot;
		bUseMargine = o.bUseMargine;
		iStartMargine = o.iStartMargine;
		iEndMargine = o.iEndMargine;
		dwServiceMode = o.dwServiceMode;

		bRegExp = o.bRegExp;
		strPattern = o.strPattern;
		return *this;
	};
} OLD_SEARCH_KEY;

typedef struct _OLD_EVENT_ID_INFO{
	DWORD dwOriginalNID;
	DWORD dwTSID;
	DWORD dwServiceID;
	DWORD dwEventID;
}OLD_EVENT_ID_INFO;

typedef struct _OLD_NIBBLE_DATA{
	unsigned char ucContentNibbleLv1; //content_nibble_level_1
	unsigned char ucContentNibbleLv2; //content_nibble_level_2
	unsigned char ucUserNibbleLv1; //user_nibble
	unsigned char ucUserNibbleLv2; //user_nibble
	_OLD_NIBBLE_DATA & operator= (const _OLD_NIBBLE_DATA & o) {
		ucContentNibbleLv1 = o.ucContentNibbleLv1;
		ucContentNibbleLv2 = o.ucContentNibbleLv2;
		ucUserNibbleLv1 = o.ucUserNibbleLv1;
		ucUserNibbleLv2 = o.ucUserNibbleLv2;
		return *this;
	};
}OLD_NIBBLE_DATA;

typedef struct _OLD_EVENT_INFO_DATA3{
	WORD wOriginalNID;
	WORD wTSID;
	WORD wServiceID;
	WORD wEventID;
	wstring strEventName;
	wstring strEventText;
	wstring strEventExtText;
	SYSTEMTIME stStartTime;
	DWORD dwDurationSec;
	unsigned char ucComponentType;
	wstring strComponentTypeText;
	unsigned char ucAudioComponentType;
	unsigned char ucESMultiLangFlag;
	unsigned char ucMainComponentFlag;
	unsigned char ucSamplingRate;
	wstring strAudioComponentTypeText;
	vector<OLD_NIBBLE_DATA> NibbleList;
	vector<OLD_EVENT_ID_INFO> EventRelayList;

	wstring strSearchTitle;
	wstring strSearchInfo;
	vector<OLD_EVENT_ID_INFO> EventGroupList;

	_OLD_EVENT_INFO_DATA3 & operator= (const _OLD_EVENT_INFO_DATA3 & o) {
		wOriginalNID = o.wOriginalNID;
		wTSID = o.wTSID;
		wServiceID = o.wServiceID;
		wEventID = o.wEventID;
		strEventName = o.strEventName;
		strEventText = o.strEventText;
		strEventExtText = o.strEventExtText;
		stStartTime = o.stStartTime;
		dwDurationSec = o.dwDurationSec;
		ucComponentType = o.ucComponentType;
		strComponentTypeText = o.strComponentTypeText;
		ucAudioComponentType = o.ucAudioComponentType;
		ucESMultiLangFlag = o.ucESMultiLangFlag;
		ucMainComponentFlag = o.ucMainComponentFlag;
		ucSamplingRate = o.ucSamplingRate;
		strAudioComponentTypeText = o.strAudioComponentTypeText;
		NibbleList = o.NibbleList;
		EventRelayList = o.EventRelayList;

		strSearchTitle = o.strSearchTitle;
		strSearchInfo = o.strSearchInfo;
		EventGroupList = o.EventGroupList;
		return *this;
	};
	_OLD_EVENT_INFO_DATA3 & operator= (const _EPGDB_EVENT_INFO & o) {
		wOriginalNID = o.original_network_id;
		wTSID = o.transport_stream_id;
		wServiceID = o.service_id;
		wEventID = o.event_id;
		if( o.shortInfo != NULL ){
			strEventName = o.shortInfo->event_name;
		}else{
			strEventName = L"";
		}
		if( o.shortInfo != NULL ){
			strEventText = o.shortInfo->text_char;
		}else{
			strEventText = L"";
		}
		if( o.extInfo != NULL ){
			strEventText = o.extInfo->text_char;
		}else{
			strEventText = L"";
		}
		stStartTime = o.start_time;
		dwDurationSec = o.durationSec;
		if( o.componentInfo != NULL ){
			ucComponentType = o.componentInfo->component_type;
			strComponentTypeText = o.componentInfo->text_char;
		}else{
			ucComponentType = 0;
			strComponentTypeText = L"";
		}
		if( o.audioInfo != NULL ){
			if( o.audioInfo->componentList.size() > 0 ){
				ucAudioComponentType = o.audioInfo->componentList[0].component_type;
				ucESMultiLangFlag = o.audioInfo->componentList[0].ES_multi_lingual_flag;
				ucMainComponentFlag = o.audioInfo->componentList[0].main_component_flag;
				ucSamplingRate = o.audioInfo->componentList[0].sampling_rate;
				strAudioComponentTypeText = o.audioInfo->componentList[0].text_char;
			}
		}
		if( o.contentInfo != NULL ){
			NibbleList.clear();
			for( size_t i=0; i<o.contentInfo->nibbleList.size(); i++ ){
				OLD_NIBBLE_DATA item;
				item.ucContentNibbleLv1 = o.contentInfo->nibbleList[i].content_nibble_level_1;
				item.ucContentNibbleLv2 = o.contentInfo->nibbleList[i].content_nibble_level_2;
				item.ucUserNibbleLv1 = o.contentInfo->nibbleList[i].user_nibble_1;
				item.ucUserNibbleLv2 = o.contentInfo->nibbleList[i].user_nibble_2;
				NibbleList.push_back(item);
			}
		}
		if( o.eventGroupInfo != NULL ){
			EventGroupList.clear();
			for( size_t i=0; i<o.eventGroupInfo->eventDataList.size(); i++ ){
				OLD_EVENT_ID_INFO item;
				item.dwOriginalNID = o.eventGroupInfo->eventDataList[i].original_network_id;
				item.dwTSID = o.eventGroupInfo->eventDataList[i].transport_stream_id;
				item.dwServiceID = o.eventGroupInfo->eventDataList[i].service_id;
				item.dwEventID = o.eventGroupInfo->eventDataList[i].event_id;
				EventGroupList.push_back(item);
			}
		}
		if( o.eventRelayInfo != NULL ){
			EventRelayList.clear();
			for( size_t i=0; i<o.eventRelayInfo->eventDataList.size(); i++ ){
				OLD_EVENT_ID_INFO item;
				item.dwOriginalNID = o.eventRelayInfo->eventDataList[i].original_network_id;
				item.dwTSID = o.eventRelayInfo->eventDataList[i].transport_stream_id;
				item.dwServiceID = o.eventRelayInfo->eventDataList[i].service_id;
				item.dwEventID = o.eventRelayInfo->eventDataList[i].event_id;
				EventRelayList.push_back(item);
			}
		}

		return *this;
	};
}OLD_EVENT_INFO_DATA3;

#endif
