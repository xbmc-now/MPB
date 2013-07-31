#pragma once

#include "../Common/Util.h"
#include "../Common/ErrDef.h"
#include "../Common/TSBuffUtil.h"
#include "../Common/TSPacketUtil.h"

class CCATUtil
{
public:
	map<WORD,WORD> PIDList;

public:
	CCATUtil(void);
	~CCATUtil(void);

	BOOL AddPacket(CTSPacketUtil* packet);

protected:
	CTSBuffUtil buffUtil;

	BYTE table_id;
	BYTE section_syntax_indicator;
	WORD section_length;
	BYTE version_number;
	BYTE current_next_indicator;
	BYTE section_number;
	BYTE last_section_number;
	DWORD crc32;

protected:
	void Clear();
	BOOL DecodeCAT(BYTE* data, DWORD dataSize);
};

