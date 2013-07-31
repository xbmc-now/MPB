#ifndef __EPG_DATA_CAP3_DEF_H__
#define __EPG_DATA_CAP3_DEF_H__

#include "Util.h"

//EPG�f�[�^�擾�X�e�[�^�X
typedef enum{
	EpgNoData			= 0x0000,	//�f�[�^���Ȃ�
	EpgNeedData			= 0x0001,	//���܂��Ă��Ȃ�
	EpgBasicAll			= 0x0010,	//��{���͂��܂���
	EpgExtendAll		= 0x0020,	//�g�����͂��܂���
	EpgHEITAll			= 0x0040,	//��{�A�g�����ɂ��܂���
	EpgLEITAll			= 0x0080,	//�����Z�O�����܂���
} EPG_SECTION_STATUS;

//EPG��{���
typedef struct _EPG_SHORT_EVENT_INFO{
	WORD event_nameLength;		//event_name�̕�����
	WCHAR* event_name;			//�C�x���g��
	WORD text_charLength;		//text_char�̕�����
	WCHAR* text_char;			//���
	_EPG_SHORT_EVENT_INFO(void){
		event_name = NULL;
		text_char = NULL;
	};
	~_EPG_SHORT_EVENT_INFO(void){
		SAFE_DELETE_ARRAY(event_name);
		SAFE_DELETE_ARRAY(text_char);
	};
} EPG_SHORT_EVENT_INFO;

//EPG�g�����
typedef struct _EPG_EXTENDED_EVENT_INFO{
	WORD text_charLength;		//text_char�̕�����
	WCHAR* text_char;			//�ڍ׏��
	_EPG_EXTENDED_EVENT_INFO(void){
		text_char = NULL;
	};
	~_EPG_EXTENDED_EVENT_INFO(void){
		SAFE_DELETE_ARRAY(text_char);
	};
} EPG_EXTENDED_EVENT_INFO;

//EPG�W�������f�[�^
typedef struct _EPG_CONTENT_DATA{
	BYTE content_nibble_level_1;
	BYTE content_nibble_level_2;
	BYTE user_nibble_1;
	BYTE user_nibble_2;
}EPG_CONTENT;

//EPG�W���������
typedef struct _EPG_CONTENT_INFO{
	WORD listSize;
	EPG_CONTENT* nibbleList;
	_EPG_CONTENT_INFO(void){
		listSize = 0;
		nibbleList = NULL;
	};
	~_EPG_CONTENT_INFO(void){
		SAFE_DELETE_ARRAY(nibbleList);
	};
} EPG_CONTEN_INFO;

//EPG�f�����
typedef struct _EPG_COMPONENT_INFO{
	BYTE stream_content;
	BYTE component_type;
	BYTE component_tag;
	WORD text_charLength;		//text_char�̕�����
	WCHAR* text_char;			//�ڍ׏��
	_EPG_COMPONENT_INFO(void){
		text_char = NULL;
	};
	~_EPG_COMPONENT_INFO(void){
		SAFE_DELETE_ARRAY(text_char);
	};
} EPG_COMPONENT_INFO;

//EPG�������
typedef struct _EPG_AUDIO_COMPONENT_INFO_DATA{
	BYTE stream_content;
	BYTE component_type;
	BYTE component_tag;
	BYTE stream_type;
	BYTE simulcast_group_tag;
	BYTE ES_multi_lingual_flag;
	BYTE main_component_flag;
	BYTE quality_indicator;
	BYTE sampling_rate;
	WORD text_charLength;		//text_char�̕�����
	WCHAR* text_char;			//�ڍ׏��
	_EPG_AUDIO_COMPONENT_INFO_DATA(void){
		text_char = NULL;
	};
	~_EPG_AUDIO_COMPONENT_INFO_DATA(void){
		SAFE_DELETE_ARRAY(text_char);
	};
} EPG_AUDIO_COMPONENT_INFO_DATA;

//EPG�������
typedef struct _EPG_AUDIO_COMPONENT_INFO{
	WORD listSize;
	EPG_AUDIO_COMPONENT_INFO_DATA* audioList;
	_EPG_AUDIO_COMPONENT_INFO(void){
		listSize = 0;
		audioList = NULL;
	};
	~_EPG_AUDIO_COMPONENT_INFO(void){
		SAFE_DELETE_ARRAY(audioList);
	};
} EPG_AUDIO_COMPONENT_INFO;

//EPG�C�x���g�f�[�^
typedef struct _EPG_EVENT_DATA{
	WORD original_network_id;
	WORD transport_stream_id;
	WORD service_id;
	WORD event_id;
}EPG_EVENT_DATA;

//EPG�C�x���g�O���[�v���
typedef struct _EPG_EVENTGROUP_INFO{
	BYTE group_type;
	BYTE event_count;
	EPG_EVENT_DATA* eventDataList;
	_EPG_EVENTGROUP_INFO(void){
		eventDataList = NULL;
	};
	~_EPG_EVENTGROUP_INFO(void){
		SAFE_DELETE_ARRAY(eventDataList);
	};
} EPG_EVENTGROUP_INFO;

typedef struct _EPG_EVENT_INFO{
	WORD event_id;							//�C�x���gID
	BYTE StartTimeFlag;						//start_time�̒l���L�����ǂ���
	SYSTEMTIME start_time;					//�J�n����
	BYTE DurationFlag;						//duration�̒l���L�����ǂ���
	DWORD durationSec;						//�����ԁi�P�ʁF�b�j

	EPG_SHORT_EVENT_INFO* shortInfo;		//��{���
	EPG_EXTENDED_EVENT_INFO* extInfo;		//�g�����
	EPG_CONTEN_INFO* contentInfo;			//�W���������
	EPG_COMPONENT_INFO* componentInfo;		//�f�����
	EPG_AUDIO_COMPONENT_INFO* audioInfo;	//�������
	EPG_EVENTGROUP_INFO* eventGroupInfo;	//�C�x���g�O���[�v���
	EPG_EVENTGROUP_INFO* eventRelayInfo;	//�C�x���g�����[���

	BYTE freeCAFlag;						//�m���X�N�����u���t���O
	_EPG_EVENT_INFO(void){
		shortInfo = NULL;
		extInfo = NULL;
		contentInfo = NULL;
		componentInfo = NULL;
		audioInfo = NULL;
		eventGroupInfo = NULL;
		eventRelayInfo = NULL;
		freeCAFlag = 0;
	};
	~_EPG_EVENT_INFO(void){
		SAFE_DELETE(shortInfo);
		SAFE_DELETE(extInfo);
		SAFE_DELETE(contentInfo);
		SAFE_DELETE(componentInfo);
		SAFE_DELETE(audioInfo);
		SAFE_DELETE(eventGroupInfo);
		SAFE_DELETE(eventRelayInfo);
	};
}EPG_EVENT_INFO;

//�T�[�r�X�̏ڍ׏��
typedef struct _SERVICE_EXT_INFO{
	BYTE service_type;
	BYTE partialReceptionFlag;
	WCHAR* service_provider_name;
	WCHAR* service_name;
	WCHAR* network_name;
	WCHAR* ts_name;
	BYTE remote_control_key_id;
	_SERVICE_EXT_INFO(void){
		service_type = 0;
		partialReceptionFlag = FALSE;
		service_provider_name = NULL;
		service_name = NULL;
		network_name = NULL;
		ts_name = NULL;
		remote_control_key_id = 0;
	};
	~_SERVICE_EXT_INFO(void){
		SAFE_DELETE_ARRAY(service_provider_name);
		SAFE_DELETE_ARRAY(service_name);
		SAFE_DELETE_ARRAY(network_name);
		SAFE_DELETE_ARRAY(ts_name);
	};
}SERVICE_EXT_INFO;

//�T�[�r�X���
typedef struct _SERVICE_INFO{
	WORD original_network_id;	//original_network_id
	WORD transport_stream_id;	//transport_stream_id
	WORD service_id;			//service_id
	SERVICE_EXT_INFO* extInfo;	//�ڍ׏��
	_SERVICE_INFO(void){
		extInfo = NULL;
	};
	~_SERVICE_INFO(void){
		SAFE_DELETE(extInfo);
	};
}SERVICE_INFO;

#endif
