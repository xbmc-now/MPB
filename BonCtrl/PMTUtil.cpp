#include "StdAfx.h"
#include "PMTUtil.h"

#include "../Common/EpgTimerUtil.h"


CPMTUtil::CPMTUtil(void)
{
	this->program_number = 0xFFFF;
	this->PCR_PID = 0xFFFF;
}


CPMTUtil::~CPMTUtil(void)
{
	Clear();
}

BOOL CPMTUtil::AddPacket(CTSPacketUtil* packet)
{
	if( packet == NULL ){
		return FALSE;
	}
	if( buffUtil.Add188TS(packet) == TRUE ){
		BYTE* section = NULL;
		DWORD sectionSize = 0;
		while( buffUtil.GetSectionBuff( &section, &sectionSize ) == TRUE ){
			if( DecodePMT(section, sectionSize) == FALSE ){
				return FALSE;
			}
		}
	}else{
		return FALSE;
	}
	return TRUE;
}

void CPMTUtil::Clear()
{
	for( size_t i=0 ;i<ESInfoList.size(); i++ ){
		SAFE_DELETE(ESInfoList[i]);
	}
	ESInfoList.clear();

	PIDList.clear();
}

BOOL CPMTUtil::DecodePMT(BYTE* data, DWORD dataSize)
{
	Clear();

	if( data == NULL ){
		return FALSE;
	}

	if( dataSize < 7 ){
		return FALSE;
	}

	DWORD readSize = 0;
	//////////////////////////////////////////////////////
	//��͏���
	table_id = data[0];
	section_syntax_indicator = (data[1]&0x80)>>7;
	section_length = ((WORD)data[1]&0x0F)<<8 | data[2];
	readSize+=3;

	if( section_syntax_indicator != 1 ){
		//�Œ�l����������
		_OutputDebugString(L"CPMTUtil::section_syntax_indicator Err");
		return FALSE;
	}
	if( table_id != 0x02 ){
		//table_id����������
		_OutputDebugString(L"CPMTUtil::table_id Err");
		return FALSE;
	}
	if( readSize+section_length > dataSize && section_length > 3){
		//�T�C�Y�ُ�
		_OutputDebugString(L"CPMTUtil::section_length %d Err", section_length);
		return FALSE;
	}
	//CRC�`�F�b�N
	crc32 = ((DWORD)data[3+section_length-4])<<24 |
		((DWORD)data[3+section_length-3])<<16 |
		((DWORD)data[3+section_length-2])<<8 |
		data[3+section_length-1];
	if( crc32 != _Crc32(3+section_length-4, data) ){
		_OutputDebugString(L"CPMTUtil::crc32 Err");
		return FALSE;
	}

	if( section_length > 8 ){
		program_number = ((WORD)data[readSize])<<8 | data[readSize+1];
		version_number = (data[readSize+2]&0x3E)>>1;
		current_next_indicator = data[readSize+2]&0x01;
		section_number = data[readSize+3];
		last_section_number = data[readSize+4];
		PCR_PID = ((WORD)data[readSize+5]&0x1F)<<8 | data[readSize+6];
		program_info_length = ((WORD)data[readSize+7]&0x0F)<<8 | data[readSize+8];
		readSize += 9;

		//descriptor
		WORD infoRead = 0;
		while(infoRead < program_info_length){
			BYTE descriptor_tag = data[readSize];
			BYTE descriptor_length = data[readSize+1];
			readSize+=2;

			if( descriptor_tag == 0x09 && descriptor_length >= 4){
				//CA
				WORD CA_PID = ((WORD)data[readSize+2]&0x1F)<<8 | (WORD)data[readSize+3];
				PIDList.insert(pair<WORD,WORD>(CA_PID, 0));
			}
			readSize += descriptor_length;

			infoRead+= 2+descriptor_length;
		}

		while( readSize < (DWORD)section_length+3-4 ){
			ES_INFO_DATA* item = new ES_INFO_DATA;
			item->stream_type = data[readSize];
			item->elementary_PID = ((WORD)data[readSize+1]&0x1F)<<8 | data[readSize+2];
			item->ES_info_length = ((WORD)data[readSize+3]&0x0F)<<8 | data[readSize+4];
			readSize += 5;

			PIDList.insert(pair<WORD,WORD>(item->elementary_PID, item->stream_type));

			//descriptor
			infoRead = 0;
			while(infoRead < item->ES_info_length){
				BYTE descriptor_tag = data[readSize];
				BYTE descriptor_length = data[readSize+1];
				readSize+=2;

				if( descriptor_tag == 0x09 && descriptor_length >= 4){
					//CA
					WORD CA_PID = ((WORD)data[readSize+2]&0x1F)<<8 | (WORD)data[readSize+3];
					PIDList.insert(pair<WORD,WORD>(CA_PID, 0));
				}
				readSize += descriptor_length;

				infoRead+= 2+descriptor_length;
			}

//			readSize+=item->ES_info_length;
			ESInfoList.push_back(item);
		}
	}

	return TRUE;
}
