#ifndef __EPG_TIMER_UTIL_H__
#define __EPG_TIMER_UTIL_H__

#include "Util.h"
#include "StructDef.h"
#include "EpgDataCap3Def.h"

//�`�����l����__int64�Ƃ��ăL�[�ɂ���
LONGLONG _Create64Key( WORD OriginalNetworkID, WORD TransportStreamID, WORD ServiceID );
//EventID��__int64�Ƃ��ăL�[�ɂ���
LONGLONG _Create64Key2( WORD OriginalNetworkID, WORD TransportStreamID, WORD ServiceID, WORD EventID );
//CRC32�����Ƃ߂�
unsigned long _Crc32(int n,  BYTE c[]);
//�^��\�z�T�C�Y���擾
LONGLONG _GetRecSize( DWORD OriginalNetworkID, DWORD TransportStreamID, DWORD ServiceID, BOOL ServiceOnlyFlag, DWORD DurationSecond );
//�w��v���Z�XID���N�����Ă��邩�m�F
BOOL _FindOpenExeProcess(DWORD processID);
//BCD->DWORD�ϊ�
DWORD _BCDtoDWORD(BYTE* data, BYTE size, BYTE digit);
//MJD->YYYY/MM/DD�ϊ�
BOOL _MJDtoYMD(DWORD mjd, WORD* y, WORD* m, WORD* d);
//MJD->SYSTEMTIME�ϊ�
BOOL _MJDtoSYSTEMTIME(DWORD mjd, SYSTEMTIME* time);

//ini�t�@�C������\�z�r�b�g���[�g���擾����
BOOL _GetBitrate(WORD ONID, WORD TSID, WORD SID, DWORD* bitrate);

//EPG����Text�ɕϊ�
void _ConvertEpgInfoText(EPGDB_EVENT_INFO* info, wstring& text);
void _ConvertEpgInfoText2(EPGDB_EVENT_INFO* info, wstring& text, wstring serviceName);

//�t�H���_�p�X������ۂ̃h���C�u�p�X���擾
void GetChkDrivePath(wstring directoryPath, wstring& mountPath);

void GetGenreName(BYTE nibble1, BYTE nibble2, wstring& name);
void GetGenreList(vector<GENRU_INFO>* list);

void CopyEpgInfo(EPG_EVENT_INFO* destInfo, EPGDB_EVENT_INFO* srcInfo);

#endif
