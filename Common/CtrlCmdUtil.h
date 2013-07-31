#ifndef __CTRL_CMD_UTIL_H__
#define __CTRL_CMD_UTIL_H__

#include "StructDef.h"

////////////////////////////////////////////////////////////////////////////////////////////
//�R�}���h���M�p�o�C�i���쐬�֐�

DWORD GetVALUESize( char val );
BOOL WriteVALUE( char val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( char* val, BYTE* buff, DWORD buffSize, DWORD* readSize );
DWORD GetVALUESize( unsigned char val );
BOOL WriteVALUE( unsigned char val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( unsigned char* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( short val );
BOOL WriteVALUE( short val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( short* val, BYTE* buff, DWORD buffSize, DWORD* readSize );
DWORD GetVALUESize( unsigned short val );
BOOL WriteVALUE( unsigned short val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( unsigned short* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( int val );
BOOL WriteVALUE( int val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( int* val, BYTE* buff, DWORD buffSize, DWORD* readSize );
DWORD GetVALUESize( unsigned int val );
BOOL WriteVALUE( unsigned int val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( unsigned int* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( long val );
BOOL WriteVALUE( long val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( long* val, BYTE* buff, DWORD buffSize, DWORD* readSize );
DWORD GetVALUESize( unsigned long val );
BOOL WriteVALUE( unsigned long val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( unsigned long* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( __int64 val );
BOOL WriteVALUE( __int64 val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( __int64* val, BYTE* buff, DWORD buffSize, DWORD* readSize );
DWORD GetVALUESize( unsigned __int64 val );
BOOL WriteVALUE( unsigned __int64 val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( unsigned __int64* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<unsigned short>* val);
BOOL WriteVALUE( vector<unsigned short>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<unsigned short>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<unsigned long>* val);
BOOL WriteVALUE( vector<unsigned long>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<unsigned long>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<__int64>* val);
BOOL WriteVALUE( vector<__int64>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<__int64>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( wstring val );
BOOL WriteVALUE( wstring val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( wstring* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<wstring>* val );
BOOL WriteVALUE( vector<wstring>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<wstring>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( SYSTEMTIME* val );
BOOL WriteVALUE( SYSTEMTIME* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( SYSTEMTIME* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( REC_SETTING_DATA* val );
BOOL WriteVALUE( REC_SETTING_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( REC_SETTING_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( RESERVE_DATA* val );
BOOL WriteVALUE( RESERVE_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( RESERVE_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<RESERVE_DATA>* val );
BOOL WriteVALUE( vector<RESERVE_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<RESERVE_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );
DWORD GetVALUESize( vector<RESERVE_DATA*>* val );
BOOL WriteVALUE( vector<RESERVE_DATA*>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<RESERVE_DATA*>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_SERVICE_INFO* val );
BOOL WriteVALUE( EPGDB_SERVICE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_SERVICE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<EPGDB_SERVICE_INFO>* val );
BOOL WriteVALUE( vector<EPGDB_SERVICE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<EPGDB_SERVICE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_SHORT_EVENT_INFO* val );
BOOL WriteVALUE( EPGDB_SHORT_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_SHORT_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_EXTENDED_EVENT_INFO* val );
BOOL WriteVALUE( EPGDB_EXTENDED_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_EXTENDED_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_CONTENT_DATA* val );
BOOL WriteVALUE( EPGDB_CONTENT_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_CONTENT_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<EPGDB_CONTENT_DATA>* val );
BOOL WriteVALUE( vector<EPGDB_CONTENT_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<EPGDB_CONTENT_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_CONTEN_INFO* val );
BOOL WriteVALUE( EPGDB_CONTEN_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_CONTEN_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_COMPONENT_INFO* val );
BOOL WriteVALUE( EPGDB_COMPONENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_COMPONENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_AUDIO_COMPONENT_INFO_DATA* val );
BOOL WriteVALUE( EPGDB_AUDIO_COMPONENT_INFO_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_AUDIO_COMPONENT_INFO_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<EPGDB_AUDIO_COMPONENT_INFO_DATA>* val );
BOOL WriteVALUE( vector<EPGDB_AUDIO_COMPONENT_INFO_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<EPGDB_AUDIO_COMPONENT_INFO_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_AUDIO_COMPONENT_INFO* val );
BOOL WriteVALUE( EPGDB_AUDIO_COMPONENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_AUDIO_COMPONENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_EVENT_DATA* val );
BOOL WriteVALUE( EPGDB_EVENT_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_EVENT_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<EPGDB_EVENT_DATA>* val );
BOOL WriteVALUE( vector<EPGDB_EVENT_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<EPGDB_EVENT_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_EVENTGROUP_INFO* val );
BOOL WriteVALUE( EPGDB_EVENTGROUP_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_EVENTGROUP_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_EVENT_INFO* val );
BOOL WriteVALUE( EPGDB_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<EPGDB_EVENT_INFO*>* val );
BOOL WriteVALUE( vector<EPGDB_EVENT_INFO*>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<EPGDB_EVENT_INFO*>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_SEARCH_DATE_INFO* val );
BOOL WriteVALUE( EPGDB_SEARCH_DATE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_SEARCH_DATE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<EPGDB_SEARCH_DATE_INFO>* val );
BOOL WriteVALUE( vector<EPGDB_SEARCH_DATE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<EPGDB_SEARCH_DATE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_SEARCH_KEY_INFO* val );
BOOL WriteVALUE( EPGDB_SEARCH_KEY_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_SEARCH_KEY_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<EPGDB_SEARCH_KEY_INFO>* val );
BOOL WriteVALUE( vector<EPGDB_SEARCH_KEY_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<EPGDB_SEARCH_KEY_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( SET_CH_INFO* val );
BOOL WriteVALUE( SET_CH_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( SET_CH_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<SET_CH_INFO>* val );
BOOL WriteVALUE( vector<SET_CH_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<SET_CH_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( SET_CTRL_MODE* val );
BOOL WriteVALUE( SET_CTRL_MODE* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( SET_CTRL_MODE* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( REC_FILE_SET_INFO* val );
BOOL WriteVALUE( REC_FILE_SET_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( REC_FILE_SET_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<REC_FILE_SET_INFO>* val );
BOOL WriteVALUE( vector<REC_FILE_SET_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<REC_FILE_SET_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( SET_CTRL_REC_PARAM* val );
BOOL WriteVALUE( SET_CTRL_REC_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( SET_CTRL_REC_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( SET_CTRL_REC_STOP_PARAM* val );
BOOL WriteVALUE( SET_CTRL_REC_STOP_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( SET_CTRL_REC_STOP_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( SET_CTRL_REC_STOP_RES_PARAM* val );
BOOL WriteVALUE( SET_CTRL_REC_STOP_RES_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( SET_CTRL_REC_STOP_RES_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( REC_FILE_INFO* val );
BOOL WriteVALUE( REC_FILE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( REC_FILE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<REC_FILE_INFO>* val );
BOOL WriteVALUE( vector<REC_FILE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<REC_FILE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPG_AUTO_ADD_DATA* val );
BOOL WriteVALUE( EPG_AUTO_ADD_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPG_AUTO_ADD_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<EPG_AUTO_ADD_DATA>* val );
BOOL WriteVALUE( vector<EPG_AUTO_ADD_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<EPG_AUTO_ADD_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( SEARCH_EPG_INFO_PARAM* val );
BOOL WriteVALUE( SEARCH_EPG_INFO_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( SEARCH_EPG_INFO_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( GET_EPG_PF_INFO_PARAM* val );
BOOL WriteVALUE( GET_EPG_PF_INFO_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( GET_EPG_PF_INFO_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( MANUAL_AUTO_ADD_DATA* val );
BOOL WriteVALUE( MANUAL_AUTO_ADD_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( MANUAL_AUTO_ADD_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<MANUAL_AUTO_ADD_DATA>* val );
BOOL WriteVALUE( vector<MANUAL_AUTO_ADD_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<MANUAL_AUTO_ADD_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( TUNER_RESERVE_INFO* val );
BOOL WriteVALUE( TUNER_RESERVE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( TUNER_RESERVE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<TUNER_RESERVE_INFO>* val );
BOOL WriteVALUE( vector<TUNER_RESERVE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<TUNER_RESERVE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( REGIST_TCP_INFO* val );
BOOL WriteVALUE( REGIST_TCP_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( REGIST_TCP_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( EPGDB_SERVICE_EVENT_INFO* val );
BOOL WriteVALUE( EPGDB_SERVICE_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( EPGDB_SERVICE_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( vector<EPGDB_SERVICE_EVENT_INFO*>* val );
BOOL WriteVALUE( vector<EPGDB_SERVICE_EVENT_INFO*>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( vector<EPGDB_SERVICE_EVENT_INFO*>* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( TVTEST_CH_CHG_INFO* val );
BOOL WriteVALUE( TVTEST_CH_CHG_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( TVTEST_CH_CHG_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( NWPLAY_PLAY_INFO* val );
BOOL WriteVALUE( NWPLAY_PLAY_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( NWPLAY_PLAY_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( NWPLAY_POS_CMD* val );
BOOL WriteVALUE( NWPLAY_POS_CMD* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( NWPLAY_POS_CMD* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( TVTEST_STREAMING_INFO* val );
BOOL WriteVALUE( TVTEST_STREAMING_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( TVTEST_STREAMING_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

DWORD GetVALUESize( NWPLAY_TIMESHIFT_INFO* val );
BOOL WriteVALUE( NWPLAY_TIMESHIFT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize );
BOOL ReadVALUE( NWPLAY_TIMESHIFT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize );

////////////////////////////////////////////////////////////////////////////////////////////
//���o�[�W�����R�}���h���M�p�o�C�i���쐬�֐�
BOOL CreateReserveDataStream(OLD_RESERVE_DATA* pData, CMD_STREAM* pCmd);
BOOL CopyReserveData(OLD_RESERVE_DATA* pstData, CMD_STREAM* pCmd);

BOOL CreateSearchKeyDataStream(OLD_SEARCH_KEY* pData, CMD_STREAM* pCmd);
BOOL CopySearchKeyData(OLD_SEARCH_KEY* pstData, CMD_STREAM* pCmd);

BOOL CreateEventInfoData3Stream(OLD_EVENT_INFO_DATA3* pData, CMD_STREAM* pCmd);
BOOL CopyEventInfoData3(OLD_EVENT_INFO_DATA3* pstData, CMD_STREAM* pCmd);

void CopyOldNew(OLD_RESERVE_DATA* src, RESERVE_DATA* dest);
void CopyOldNew(OLD_SEARCH_KEY* src, EPG_AUTO_ADD_DATA* dest);
void CopyOldNew(OLD_SEARCH_KEY* src, EPGDB_SEARCH_KEY_INFO* dest);

#endif
