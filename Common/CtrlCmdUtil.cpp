#include "stdafx.h"
#include "CtrlCmdUtil.h"
#include "Util.h"

DWORD GetVALUESize( char val )
{
	return sizeof(char);
}

BOOL WriteVALUE( char val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( buff == NULL || sizeof(char) > buffSize ){
		return FALSE;
	}
	memcpy(buff, &val, sizeof(char));
	if( writeSize != NULL ){
		*writeSize = sizeof(char);
	}
	return TRUE;
}

BOOL ReadVALUE( char* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( buff == NULL || sizeof(char) > buffSize ){
		return FALSE;
	}
	if( val != NULL ){
		*val = *(char*)buff;
	}
	if( readSize != NULL ){
		*readSize = sizeof(char);
	}
	return TRUE;
}

DWORD GetVALUESize( unsigned char val )
{
	return sizeof(unsigned char);
}

BOOL WriteVALUE( unsigned char val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( buff == NULL || sizeof(unsigned char) > buffSize ){
		return FALSE;
	}
	memcpy(buff, &val, sizeof(unsigned char));
	if( writeSize != NULL ){
		*writeSize = sizeof(unsigned char);
	}
	return TRUE;
}

BOOL ReadVALUE( unsigned char* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( buff == NULL || sizeof(unsigned char) > buffSize ){
		return FALSE;
	}
	if( val != NULL ){
		*val = *(unsigned char*)buff;
	}
	if( readSize != NULL ){
		*readSize = sizeof(unsigned char);
	}
	return TRUE;
}

DWORD GetVALUESize( short val )
{
	return sizeof(short);
}

BOOL WriteVALUE( short val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( buff == NULL || sizeof(short) > buffSize ){
		return FALSE;
	}
	memcpy(buff, &val, sizeof(short));
	if( writeSize != NULL ){
		*writeSize = sizeof(short);
	}
	return TRUE;
}

BOOL ReadVALUE( short* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( buff == NULL || sizeof(short) > buffSize ){
		return FALSE;
	}
	if( val != NULL ){
		*val = *(short*)buff;
	}
	if( readSize != NULL ){
		*readSize = sizeof(short);
	}
	return TRUE;
}

DWORD GetVALUESize( unsigned short val )
{
	return sizeof(unsigned short);
}

BOOL WriteVALUE( unsigned short val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( buff == NULL || sizeof(unsigned short) > buffSize ){
		return FALSE;
	}
	memcpy(buff, &val, sizeof(unsigned short));
	if( writeSize != NULL ){
		*writeSize = sizeof(unsigned short);
	}
	return TRUE;
}

BOOL ReadVALUE( unsigned short* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( buff == NULL || sizeof(unsigned short) > buffSize ){
		return FALSE;
	}
	if( val != NULL ){
		*val = *(unsigned short*)buff;
	}
	if( readSize != NULL ){
		*readSize = sizeof(unsigned short);
	}
	return TRUE;
}


DWORD GetVALUESize( int val )
{
	return sizeof(int);
}

BOOL WriteVALUE( int val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( buff == NULL || sizeof(int) > buffSize ){
		return FALSE;
	}
	memcpy(buff, &val, sizeof(int));
	if( writeSize != NULL ){
		*writeSize = sizeof(int);
	}
	return TRUE;
}

BOOL ReadVALUE( int* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( buff == NULL || sizeof(int) > buffSize ){
		return FALSE;
	}
	if( val != NULL ){
		*val = *(int*)buff;
	}
	if( readSize != NULL ){
		*readSize = sizeof(int);
	}
	return TRUE;
}

DWORD GetVALUESize( unsigned int val )
{
	return sizeof(unsigned int);
}

BOOL WriteVALUE( unsigned int val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( buff == NULL || sizeof(unsigned int) > buffSize ){
		return FALSE;
	}
	memcpy(buff, &val, sizeof(unsigned int));
	if( writeSize != NULL ){
		*writeSize = sizeof(unsigned int);
	}
	return TRUE;
}

BOOL ReadVALUE( unsigned int* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( buff == NULL || sizeof(unsigned int) > buffSize ){
		return FALSE;
	}
	if( val != NULL ){
		*val = *(unsigned int*)buff;
	}
	if( readSize != NULL ){
		*readSize = sizeof(unsigned int);
	}
	return TRUE;
}

DWORD GetVALUESize( long val )
{
	return sizeof(long);
}

BOOL WriteVALUE( long val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( buff == NULL || sizeof(long) > buffSize ){
		return FALSE;
	}
	memcpy(buff, &val, sizeof(long));
	if( writeSize != NULL ){
		*writeSize = sizeof(long);
	}
	return TRUE;
}

BOOL ReadVALUE( long* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( buff == NULL || sizeof(long) > buffSize ){
		return FALSE;
	}
	if( val != NULL ){
		*val = *(long*)buff;
	}
	if( readSize != NULL ){
		*readSize = sizeof(long);
	}
	return TRUE;
}

DWORD GetVALUESize( unsigned long val )
{
	return sizeof(unsigned long);
}

BOOL WriteVALUE( unsigned long val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( buff == NULL || sizeof(unsigned long) > buffSize ){
		return FALSE;
	}
	memcpy(buff, &val, sizeof(unsigned long));
	if( writeSize != NULL ){
		*writeSize = sizeof(unsigned long);
	}
	return TRUE;
}

BOOL ReadVALUE( unsigned long* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( buff == NULL || sizeof(unsigned long) > buffSize ){
		return FALSE;
	}
	if( val != NULL ){
		*val = *(unsigned long*)buff;
	}
	if( readSize != NULL ){
		*readSize = sizeof(unsigned long);
	}
	return TRUE;
}

DWORD GetVALUESize( __int64 val )
{
	return sizeof(__int64);
}

BOOL WriteVALUE( __int64 val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( buff == NULL || sizeof(__int64) > buffSize ){
		return FALSE;
	}
	memcpy(buff, &val, sizeof(__int64));
	if( writeSize != NULL ){
		*writeSize = sizeof(__int64);
	}
	return TRUE;
}

BOOL ReadVALUE( __int64* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( buff == NULL || sizeof(__int64) > buffSize ){
		return FALSE;
	}
	if( val != NULL ){
		*val = *(__int64*)buff;
	}
	if( readSize != NULL ){
		*readSize = sizeof(__int64);
	}
	return TRUE;
}

DWORD GetVALUESize( unsigned __int64 val )
{
	return sizeof(unsigned __int64);
}

BOOL WriteVALUE( unsigned __int64 val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( buff == NULL || sizeof(unsigned __int64) > buffSize ){
		return FALSE;
	}
	memcpy(buff, &val, sizeof(unsigned __int64));
	if( writeSize != NULL ){
		*writeSize = sizeof(unsigned __int64);
	}
	return TRUE;
}

BOOL ReadVALUE( unsigned __int64* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( buff == NULL || sizeof(unsigned __int64) > buffSize ){
		return FALSE;
	}
	if( val != NULL ){
		*val = *(unsigned __int64*)buff;
	}
	if( readSize != NULL ){
		*readSize = sizeof(unsigned __int64);
	}
	return TRUE;
}

DWORD GetVALUESize( vector<unsigned short>* val)
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	size += sizeof(unsigned short) * (DWORD)val->size();

	return size;
}

BOOL WriteVALUE( vector<unsigned short>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valueSize = GetVALUESize( val );
	if( buff == NULL || valueSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valueSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( (*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<unsigned short>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		unsigned short data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<unsigned long>* val)
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	size += sizeof(unsigned long) * (DWORD)val->size();

	return size;
}

BOOL WriteVALUE( vector<unsigned long>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valueSize = GetVALUESize( val );
	if( buff == NULL || valueSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valueSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( (*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<unsigned long>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		unsigned long data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<__int64>* val)
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	size += sizeof(__int64) * (DWORD)val->size();

	return size;
}

BOOL WriteVALUE( vector<__int64>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valueSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valueSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valueSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( (*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<__int64>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		__int64 data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( wstring val )
{
	return ( (DWORD)val.size() + 1 ) * sizeof(WCHAR) + sizeof(DWORD);
}

BOOL WriteVALUE( wstring val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD stringBuffSize = GetVALUESize( val );
	if( buff == NULL || stringBuffSize > buffSize ){
		return FALSE;
	}

	ZeroMemory( buff, stringBuffSize );
	//�܂��S�̂̃T�C�Y
	DWORD size = 0;
	if( WriteVALUE( stringBuffSize, buff, stringBuffSize, &size ) == FALSE ){
		return FALSE;
	}
	//���������
	if( val.size() > 0 ){
		memcpy(buff + size, val.c_str(), val.size()*sizeof(WCHAR));
	}
	if( writeSize != NULL ){
		*writeSize = stringBuffSize;
	}
	return TRUE;
}

BOOL ReadVALUE( wstring* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	
	*val = (WCHAR*)(buff + size);

	if( readSize != NULL ){
		*readSize = valSize;
	}
	return TRUE;
}

DWORD GetVALUESize( vector<wstring>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize((*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<wstring>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( (*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<wstring>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		wstring data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( SYSTEMTIME* val )
{
	return sizeof( SYSTEMTIME );
}

BOOL WriteVALUE( SYSTEMTIME* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	if( val == NULL || buff == NULL || sizeof( SYSTEMTIME ) > buffSize ){
		return FALSE;
	}
	memcpy(buff, val, sizeof( SYSTEMTIME ));
	if( writeSize != NULL ){
		*writeSize = sizeof( SYSTEMTIME );
	}
	return TRUE;
}

BOOL ReadVALUE( SYSTEMTIME* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || sizeof( SYSTEMTIME ) > buffSize ){
		return FALSE;
	}
	*val = *(SYSTEMTIME*)buff;
	if( readSize != NULL ){
		*readSize = sizeof( SYSTEMTIME );
	}
	return TRUE;
}

DWORD GetVALUESize( REC_SETTING_DATA* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->recMode);
	size += GetVALUESize(val->priority);
	size += GetVALUESize(val->tuijyuuFlag);
	size += GetVALUESize(val->serviceMode);
	size += GetVALUESize(val->pittariFlag);
	size += GetVALUESize(val->batFilePath);
	size += GetVALUESize(&val->recFolderList);
	size += GetVALUESize(val->suspendMode);
	size += GetVALUESize(val->rebootFlag);
	size += GetVALUESize(val->useMargineFlag);
	size += GetVALUESize(val->startMargine);
	size += GetVALUESize(val->endMargine);
	size += GetVALUESize(val->continueRecFlag);
	size += GetVALUESize(val->partialRecFlag);
	size += GetVALUESize(val->tunerID);

	return size;
}

BOOL WriteVALUE( REC_SETTING_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if (val != NULL ){
		if( WriteVALUE( val->recMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->priority, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->tuijyuuFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->serviceMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->pittariFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->batFilePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->recFolderList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->suspendMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->rebootFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->useMargineFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->startMargine, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->endMargine, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->continueRecFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->partialRecFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->tunerID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( REC_SETTING_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->recMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->priority, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->tuijyuuFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->serviceMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->pittariFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->batFilePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->recFolderList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->suspendMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->rebootFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->useMargineFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->startMargine, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->endMargine, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->continueRecFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->partialRecFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->tunerID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( RESERVE_DATA* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->title);
	size += GetVALUESize(&val->startTime);
	size += GetVALUESize(val->durationSecond);
	size += GetVALUESize(val->stationName);
	size += GetVALUESize(val->originalNetworkID);
	size += GetVALUESize(val->transportStreamID);
	size += GetVALUESize(val->serviceID);
	size += GetVALUESize(val->eventID);
	size += GetVALUESize(val->comment);
	size += GetVALUESize(val->reserveID);
	size += GetVALUESize(val->recWaitFlag);
	size += GetVALUESize(val->overlapMode);
	size += GetVALUESize(val->recFilePath);
	size += GetVALUESize(&val->startTimeEpg);
	size += GetVALUESize(&val->recSetting);
	size += GetVALUESize(val->reserveStatus);

	return size;
}

BOOL WriteVALUE( RESERVE_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->title, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->startTime, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->durationSecond, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->stationName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->originalNetworkID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->transportStreamID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->serviceID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->eventID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->comment, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->reserveID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->recWaitFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->overlapMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->recFilePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->startTimeEpg, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->recSetting, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->reserveStatus, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( RESERVE_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->title, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->startTime, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->durationSecond, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->stationName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->originalNetworkID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->transportStreamID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->serviceID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->eventID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->comment, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->reserveID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->recWaitFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->overlapMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->recFilePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->startTimeEpg, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->recSetting, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->reserveStatus, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<RESERVE_DATA>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<RESERVE_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<RESERVE_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		RESERVE_DATA data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<RESERVE_DATA*>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize((*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<RESERVE_DATA*>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( (*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<RESERVE_DATA*>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		RESERVE_DATA* data = new RESERVE_DATA;
		if( ReadVALUE( data, buff + pos, buffSize - pos, &size ) == FALSE ){
			SAFE_DELETE(data);
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_SERVICE_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->ONID);
	size += GetVALUESize(val->TSID);
	size += GetVALUESize(val->SID);
	size += GetVALUESize(val->service_type);
	size += GetVALUESize(val->partialReceptionFlag);
	size += GetVALUESize(val->service_provider_name);
	size += GetVALUESize(val->service_name);
	size += GetVALUESize(val->network_name);
	size += GetVALUESize(val->ts_name);
	size += GetVALUESize(val->remote_control_key_id);

	return size;
}

BOOL WriteVALUE( EPGDB_SERVICE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->ONID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->TSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->SID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->service_type, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->partialReceptionFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->service_provider_name, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->service_name, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->network_name, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->ts_name, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->remote_control_key_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_SERVICE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->ONID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->TSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->SID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->service_type, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->partialReceptionFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->service_provider_name, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->service_name, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->network_name, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->ts_name, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->remote_control_key_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<EPGDB_SERVICE_INFO>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<EPGDB_SERVICE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<EPGDB_SERVICE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		EPGDB_SERVICE_INFO data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_SHORT_EVENT_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->event_name);
	size += GetVALUESize(val->text_char);

	return size;
}

BOOL WriteVALUE( EPGDB_SHORT_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->event_name, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->text_char, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_SHORT_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->event_name, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->text_char, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_EXTENDED_EVENT_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->text_char);

	return size;
}

BOOL WriteVALUE( EPGDB_EXTENDED_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->text_char, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_EXTENDED_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->text_char, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_CONTENT_DATA* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->content_nibble_level_1);
	size += GetVALUESize(val->content_nibble_level_2);
	size += GetVALUESize(val->user_nibble_1);
	size += GetVALUESize(val->user_nibble_2);

	return size;
}

BOOL WriteVALUE( EPGDB_CONTENT_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->content_nibble_level_1, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->content_nibble_level_2, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->user_nibble_1, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->user_nibble_2, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_CONTENT_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->content_nibble_level_1, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->content_nibble_level_2, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->user_nibble_1, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->user_nibble_2, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<EPGDB_CONTENT_DATA>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<EPGDB_CONTENT_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<EPGDB_CONTENT_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		EPGDB_CONTENT_DATA data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_CONTEN_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(&val->nibbleList);

	return size;
}

BOOL WriteVALUE( EPGDB_CONTEN_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( &val->nibbleList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_CONTEN_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->nibbleList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_COMPONENT_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->stream_content);
	size += GetVALUESize(val->component_type);
	size += GetVALUESize(val->component_tag);
	size += GetVALUESize(val->text_char);

	return size;
}

BOOL WriteVALUE( EPGDB_COMPONENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->stream_content, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->component_type, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->component_tag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->text_char, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_COMPONENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->stream_content, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->component_type, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->component_tag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->text_char, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_AUDIO_COMPONENT_INFO_DATA* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->stream_content);
	size += GetVALUESize(val->component_type);
	size += GetVALUESize(val->component_tag);
	size += GetVALUESize(val->stream_type);
	size += GetVALUESize(val->simulcast_group_tag);
	size += GetVALUESize(val->ES_multi_lingual_flag);
	size += GetVALUESize(val->main_component_flag);
	size += GetVALUESize(val->quality_indicator);
	size += GetVALUESize(val->sampling_rate);
	size += GetVALUESize(val->text_char);

	return size;
}

BOOL WriteVALUE( EPGDB_AUDIO_COMPONENT_INFO_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->stream_content, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->component_type, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->component_tag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->stream_type, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->simulcast_group_tag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->ES_multi_lingual_flag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->main_component_flag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->quality_indicator, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->sampling_rate, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->text_char, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_AUDIO_COMPONENT_INFO_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->stream_content, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->component_type, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->component_tag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->stream_type, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->simulcast_group_tag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->ES_multi_lingual_flag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->main_component_flag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->quality_indicator, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->sampling_rate, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->text_char, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<EPGDB_AUDIO_COMPONENT_INFO_DATA>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<EPGDB_AUDIO_COMPONENT_INFO_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<EPGDB_AUDIO_COMPONENT_INFO_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		EPGDB_AUDIO_COMPONENT_INFO_DATA data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_AUDIO_COMPONENT_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(&val->componentList);

	return size;
}

BOOL WriteVALUE( EPGDB_AUDIO_COMPONENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( &val->componentList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_AUDIO_COMPONENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->componentList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_EVENT_DATA* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->original_network_id);
	size += GetVALUESize(val->transport_stream_id);
	size += GetVALUESize(val->service_id);
	size += GetVALUESize(val->event_id);

	return size;
}

BOOL WriteVALUE( EPGDB_EVENT_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->original_network_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->transport_stream_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->service_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->event_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_EVENT_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->original_network_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->transport_stream_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->service_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->event_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<EPGDB_EVENT_DATA>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<EPGDB_EVENT_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<EPGDB_EVENT_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		EPGDB_EVENT_DATA data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_EVENTGROUP_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->group_type);
	size += GetVALUESize(&val->eventDataList);

	return size;
}

BOOL WriteVALUE( EPGDB_EVENTGROUP_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->group_type, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->eventDataList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_EVENTGROUP_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->group_type, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->eventDataList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_EVENT_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->original_network_id);
	size += GetVALUESize(val->transport_stream_id);
	size += GetVALUESize(val->service_id);
	size += GetVALUESize(val->event_id);
	size += GetVALUESize(val->StartTimeFlag);
	size += GetVALUESize(&val->start_time);
	size += GetVALUESize(val->DurationFlag);
	size += GetVALUESize(val->durationSec);

	size += GetVALUESize(val->shortInfo);
	size += GetVALUESize(val->extInfo);
	size += GetVALUESize(val->contentInfo);
	size += GetVALUESize(val->componentInfo);
	size += GetVALUESize(val->audioInfo);
	size += GetVALUESize(val->eventGroupInfo);
	size += GetVALUESize(val->eventRelayInfo);

	size += GetVALUESize(val->freeCAFlag);

	return size;
}

BOOL WriteVALUE( EPGDB_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if( val != NULL ){
		if( WriteVALUE( val->original_network_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->transport_stream_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->service_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->event_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->StartTimeFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->start_time, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->DurationFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->durationSec, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;

		if( WriteVALUE( val->shortInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->extInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->contentInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->componentInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->audioInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->eventGroupInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->eventRelayInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->freeCAFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->original_network_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->transport_stream_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->service_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->event_id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->StartTimeFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->start_time, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->DurationFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->durationSec, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;

		{
			EPGDB_SHORT_EVENT_INFO* info = new EPGDB_SHORT_EVENT_INFO;
			if( ReadVALUE( info, buff + pos, buffSize - pos, &size ) == FALSE ){
				SAFE_DELETE(info);
				return FALSE;
			}
			if( size == sizeof(DWORD) ){
				SAFE_DELETE(info);
				val->shortInfo = NULL;
			}else{
				val->shortInfo = info;
			}
			pos += size;
		}

		{
			EPGDB_EXTENDED_EVENT_INFO* info = new EPGDB_EXTENDED_EVENT_INFO;
			if( ReadVALUE( info, buff + pos, buffSize - pos, &size ) == FALSE ){
				SAFE_DELETE(info);
				return FALSE;
			}
			if( size == sizeof(DWORD) ){
				SAFE_DELETE(info);
				val->extInfo = NULL;
			}else{
				val->extInfo = info;
			}
			pos += size;
		}

		{
			EPGDB_CONTEN_INFO* info = new EPGDB_CONTEN_INFO;
			if( ReadVALUE( info, buff + pos, buffSize - pos, &size ) == FALSE ){
				SAFE_DELETE(info);
				return FALSE;
			}
			if( size == sizeof(DWORD) ){
				SAFE_DELETE(info);
				val->contentInfo = NULL;
			}else{
				val->contentInfo = info;
			}
			pos += size;
		}

		{
			EPGDB_COMPONENT_INFO* info = new EPGDB_COMPONENT_INFO;
			if( ReadVALUE( info, buff + pos, buffSize - pos, &size ) == FALSE ){
				SAFE_DELETE(info);
				return FALSE;
			}
			if( size == sizeof(DWORD) ){
				SAFE_DELETE(info);
				val->componentInfo = NULL;
			}else{
				val->componentInfo = info;
			}
			pos += size;
		}

		{
			EPGDB_AUDIO_COMPONENT_INFO* info = new EPGDB_AUDIO_COMPONENT_INFO;
			if( ReadVALUE( info, buff + pos, buffSize - pos, &size ) == FALSE ){
				SAFE_DELETE(info);
				return FALSE;
			}
			if( size == sizeof(DWORD) ){
				SAFE_DELETE(info);
				val->audioInfo = NULL;
			}else{
				val->audioInfo = info;
			}
			pos += size;
		}

		{
			EPGDB_EVENTGROUP_INFO* info = new EPGDB_EVENTGROUP_INFO;
			if( ReadVALUE( info, buff + pos, buffSize - pos, &size ) == FALSE ){
				SAFE_DELETE(info);
				return FALSE;
			}
			if( size == sizeof(DWORD) ){
				SAFE_DELETE(info);
				val->eventGroupInfo = NULL;
			}else{
				val->eventGroupInfo = info;
			}
			pos += size;
		}

		{
			EPGDB_EVENTGROUP_INFO* info = new EPGDB_EVENTGROUP_INFO;
			if( ReadVALUE( info, buff + pos, buffSize - pos, &size ) == FALSE ){
				SAFE_DELETE(info);
				return FALSE;
			}
			if( size == sizeof(DWORD) ){
				SAFE_DELETE(info);
				val->eventRelayInfo = NULL;
			}else{
				val->eventRelayInfo = info;
			}
			pos += size;
		}

		if( ReadVALUE( &val->freeCAFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;

	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<EPGDB_EVENT_INFO*>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize((*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<EPGDB_EVENT_INFO*>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( (*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<EPGDB_EVENT_INFO*>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		EPGDB_EVENT_INFO* data = new EPGDB_EVENT_INFO;
		if( ReadVALUE( data, buff + pos, buffSize - pos, &size ) == FALSE ){
			SAFE_DELETE(data);
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_SEARCH_DATE_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->startDayOfWeek);
	size += GetVALUESize(val->startHour);
	size += GetVALUESize(val->startMin);
	size += GetVALUESize(val->endDayOfWeek);
	size += GetVALUESize(val->endHour);
	size += GetVALUESize(val->endMin);

	return size;
}

BOOL WriteVALUE( EPGDB_SEARCH_DATE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->startDayOfWeek, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->startHour, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->startMin, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->endDayOfWeek, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->endHour, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->endMin, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_SEARCH_DATE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->startDayOfWeek, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->startHour, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->startMin, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->endDayOfWeek, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->endHour, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->endMin, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<EPGDB_SEARCH_DATE_INFO>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<EPGDB_SEARCH_DATE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<EPGDB_SEARCH_DATE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		EPGDB_SEARCH_DATE_INFO data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_SEARCH_KEY_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->andKey);
	size += GetVALUESize(val->notKey);
	size += GetVALUESize(val->regExpFlag);
	size += GetVALUESize(val->titleOnlyFlag);
	size += GetVALUESize(&val->contentList);
	size += GetVALUESize(&val->dateList);
	size += GetVALUESize(&val->serviceList);
	size += GetVALUESize(&val->videoList);
	size += GetVALUESize(&val->audioList);
	size += GetVALUESize(val->aimaiFlag);
	size += GetVALUESize(val->notContetFlag);
	size += GetVALUESize(val->notDateFlag);
	size += GetVALUESize(val->freeCAFlag);

	return size;
}

BOOL WriteVALUE( EPGDB_SEARCH_KEY_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->andKey, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->notKey, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->regExpFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->titleOnlyFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->contentList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->dateList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->serviceList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->videoList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->audioList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->aimaiFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->notContetFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->notDateFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->freeCAFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_SEARCH_KEY_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->andKey, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->notKey, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->regExpFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->titleOnlyFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->contentList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->dateList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->serviceList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->videoList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->audioList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->aimaiFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->notContetFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->notDateFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->freeCAFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<EPGDB_SEARCH_KEY_INFO>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<EPGDB_SEARCH_KEY_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<EPGDB_SEARCH_KEY_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		EPGDB_SEARCH_KEY_INFO data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( SET_CH_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->useSID);
	size += GetVALUESize(val->ONID);
	size += GetVALUESize(val->TSID);
	size += GetVALUESize(val->SID);
	size += GetVALUESize(val->useBonCh);
	size += GetVALUESize(val->space);
	size += GetVALUESize(val->ch);

	return size;
}

BOOL WriteVALUE( SET_CH_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->useSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->ONID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->TSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->SID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->useBonCh, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->space, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->ch, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( SET_CH_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->useSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->ONID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->TSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->SID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->useBonCh, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->space, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->ch, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<SET_CH_INFO>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<SET_CH_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<SET_CH_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		SET_CH_INFO data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( SET_CTRL_MODE* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->ctrlID);
	size += GetVALUESize(val->SID);
	size += GetVALUESize(val->enableScramble);
	size += GetVALUESize(val->enableCaption);
	size += GetVALUESize(val->enableData);

	return size;
}

BOOL WriteVALUE( SET_CTRL_MODE* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->SID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->enableScramble, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->enableCaption, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->enableData, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;

	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( SET_CTRL_MODE* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->SID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->enableScramble, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->enableCaption, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->enableData, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;

	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( REC_FILE_SET_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->recFolder);
	size += GetVALUESize(val->writePlugIn);
	size += GetVALUESize(val->recNamePlugIn);
	size += GetVALUESize(val->recFileName);

	return size;
}

BOOL WriteVALUE( REC_FILE_SET_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->recFolder, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->writePlugIn, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->recNamePlugIn, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->recFileName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( REC_FILE_SET_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->recFolder, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->writePlugIn, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->recNamePlugIn, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->recFileName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<REC_FILE_SET_INFO>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<REC_FILE_SET_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<REC_FILE_SET_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		REC_FILE_SET_INFO data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( SET_CTRL_REC_PARAM* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->ctrlID);
	size += GetVALUESize(val->fileName);
	size += GetVALUESize(val->overWriteFlag);
	size += GetVALUESize(val->createSize);
	size += GetVALUESize(&val->saveFolder);
	size += GetVALUESize(val->pittariFlag);
	size += GetVALUESize(val->pittariONID);
	size += GetVALUESize(val->pittariTSID);
	size += GetVALUESize(val->pittariSID);
	size += GetVALUESize(val->pittariEventID);

	return size;
}

BOOL WriteVALUE( SET_CTRL_REC_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->fileName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->overWriteFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->createSize, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->saveFolder, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->pittariFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->pittariONID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->pittariTSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->pittariSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->pittariEventID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;

	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( SET_CTRL_REC_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->fileName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->overWriteFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->createSize, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->saveFolder, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->pittariFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->pittariONID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->pittariTSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->pittariSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->pittariEventID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;

	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( SET_CTRL_REC_STOP_PARAM* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->ctrlID);
	size += GetVALUESize(val->saveErrLog);

	return size;
}

BOOL WriteVALUE( SET_CTRL_REC_STOP_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->saveErrLog, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( SET_CTRL_REC_STOP_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->saveErrLog, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( SET_CTRL_REC_STOP_RES_PARAM* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->recFilePath);
	size += GetVALUESize(val->drop);
	size += GetVALUESize(val->scramble);
	size += GetVALUESize(val->subRecFlag);

	return size;
}

BOOL WriteVALUE( SET_CTRL_REC_STOP_RES_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->recFilePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->drop, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->scramble, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->subRecFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( SET_CTRL_REC_STOP_RES_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->recFilePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->drop, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->scramble, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->subRecFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( REC_FILE_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->id);
	size += GetVALUESize(val->recFilePath);
	size += GetVALUESize(val->title);
	size += GetVALUESize(&val->startTime);
	size += GetVALUESize(val->durationSecond);
	size += GetVALUESize(val->serviceName);
	size += GetVALUESize(val->originalNetworkID);
	size += GetVALUESize(val->transportStreamID);
	size += GetVALUESize(val->serviceID);
	size += GetVALUESize(val->eventID);
	size += GetVALUESize(val->drops);
	size += GetVALUESize(val->scrambles);
	size += GetVALUESize(val->recStatus);
	size += GetVALUESize(&val->startTimeEpg);
	size += GetVALUESize(val->comment);
	size += GetVALUESize(val->programInfo);
	size += GetVALUESize(val->errInfo);

	return size;
}

BOOL WriteVALUE( REC_FILE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->recFilePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->title, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->startTime, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->durationSecond, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->serviceName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->originalNetworkID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->transportStreamID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->serviceID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->eventID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->drops, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->scrambles, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->recStatus, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->startTimeEpg, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->comment, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->programInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->errInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( REC_FILE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->id, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->recFilePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->title, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->startTime, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->durationSecond, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->serviceName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->originalNetworkID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->transportStreamID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->serviceID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->eventID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->drops, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->scrambles, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->recStatus, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->startTimeEpg, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->comment, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->programInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->errInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<REC_FILE_INFO>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<REC_FILE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<REC_FILE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		REC_FILE_INFO data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPG_AUTO_ADD_DATA* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->dataID);
	size += GetVALUESize(&val->searchInfo);
	size += GetVALUESize(&val->recSetting);

	return size;
}

BOOL WriteVALUE( EPG_AUTO_ADD_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->dataID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->searchInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->recSetting, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPG_AUTO_ADD_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->dataID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->searchInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->recSetting, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<EPG_AUTO_ADD_DATA>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<EPG_AUTO_ADD_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<EPG_AUTO_ADD_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		EPG_AUTO_ADD_DATA data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( SEARCH_EPG_INFO_PARAM* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->ONID);
	size += GetVALUESize(val->TSID);
	size += GetVALUESize(val->SID);
	size += GetVALUESize(val->eventID);
	size += GetVALUESize(val->pfOnlyFlag);

	return size;
}

BOOL WriteVALUE( SEARCH_EPG_INFO_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->ONID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->TSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->SID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->eventID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->pfOnlyFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( SEARCH_EPG_INFO_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->ONID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->TSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->SID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->eventID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->pfOnlyFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( GET_EPG_PF_INFO_PARAM* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->ONID);
	size += GetVALUESize(val->TSID);
	size += GetVALUESize(val->SID);
	size += GetVALUESize(val->pfNextFlag);

	return size;
}

BOOL WriteVALUE( GET_EPG_PF_INFO_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->ONID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->TSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->SID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->pfNextFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( GET_EPG_PF_INFO_PARAM* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->ONID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->TSID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->SID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->pfNextFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( MANUAL_AUTO_ADD_DATA* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->dataID);
	size += GetVALUESize(val->dayOfWeekFlag);
	size += GetVALUESize(val->startTime);
	size += GetVALUESize(val->durationSecond);
	size += GetVALUESize(val->title);
	size += GetVALUESize(val->stationName);
	size += GetVALUESize(val->originalNetworkID);
	size += GetVALUESize(val->transportStreamID);
	size += GetVALUESize(val->serviceID);
	size += GetVALUESize(&val->recSetting);

	return size;
}

BOOL WriteVALUE( MANUAL_AUTO_ADD_DATA* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->dataID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->dayOfWeekFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->startTime, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->durationSecond, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->title, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->stationName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->originalNetworkID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->transportStreamID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->serviceID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->recSetting, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( MANUAL_AUTO_ADD_DATA* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->dataID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->dayOfWeekFlag, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->startTime, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->durationSecond, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->title, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->stationName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->originalNetworkID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->transportStreamID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->serviceID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->recSetting, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<MANUAL_AUTO_ADD_DATA>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<MANUAL_AUTO_ADD_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<MANUAL_AUTO_ADD_DATA>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		MANUAL_AUTO_ADD_DATA data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( TUNER_RESERVE_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->tunerID);
	size += GetVALUESize(val->tunerName);
	size += GetVALUESize(&val->reserveList);

	return size;
}

BOOL WriteVALUE( TUNER_RESERVE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->tunerID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->tunerName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->reserveList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;

	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( TUNER_RESERVE_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->tunerID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->tunerName, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->reserveList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;

	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<TUNER_RESERVE_INFO>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize(&(*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<TUNER_RESERVE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( &(*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<TUNER_RESERVE_INFO>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		TUNER_RESERVE_INFO data;
		if( ReadVALUE( &data, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( REGIST_TCP_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->ip);
	size += GetVALUESize(val->port);

	return size;
}

BOOL WriteVALUE( REGIST_TCP_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->ip, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->port, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( REGIST_TCP_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->ip, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->port, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( EPGDB_SERVICE_EVENT_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(&val->serviceInfo);
	size += GetVALUESize(&val->eventList);

	return size;
}

BOOL WriteVALUE( EPGDB_SERVICE_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( &val->serviceInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->eventList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( EPGDB_SERVICE_EVENT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->serviceInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->eventList, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( vector<EPGDB_SERVICE_EVENT_INFO*>* val )
{
	DWORD size = sizeof(DWORD)*2;
	if( val == NULL ){
		return size;
	}

	for( size_t i=0; i<val->size(); i++ ){
		size += GetVALUESize((*val)[i]);
	}

	return size;
}

BOOL WriteVALUE( vector<EPGDB_SERVICE_EVENT_INFO*>* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( val == NULL || buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	//�܂��S�̂̃T�C�Y
	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	DWORD count = 0;
	if( val != NULL ){
		count = (DWORD)val->size();
	}
	//���X�g�̌�
	if( WriteVALUE( count, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	
	if( val != NULL ){
		//���X�g�̒��g
		for( DWORD i=0; i < (DWORD)val->size(); i++ ){
			if( WriteVALUE( (*val)[i], buff + pos, buffSize - pos, &size ) == FALSE ){
				return FALSE;
			}
			pos += size;
		}
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	
	return TRUE;
}

BOOL ReadVALUE( vector<EPGDB_SERVICE_EVENT_INFO*>* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD)*2 ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	DWORD valCount = 0;
	//�S�̂̃T�C�Y
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}
	//���X�g�̌�
	if( ReadVALUE( &valCount, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	for( DWORD i= 0; i<valCount; i++ ){
		EPGDB_SERVICE_EVENT_INFO* data = new EPGDB_SERVICE_EVENT_INFO;
		if( ReadVALUE( data, buff + pos, buffSize - pos, &size ) == FALSE ){
			SAFE_DELETE(data);
			return FALSE;
		}
		pos += size;
		val->push_back( data );
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( TVTEST_CH_CHG_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->bonDriver);
	size += GetVALUESize(&val->chInfo);

	return size;
}

BOOL WriteVALUE( TVTEST_CH_CHG_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->bonDriver, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( &val->chInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( TVTEST_CH_CHG_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->bonDriver, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->chInfo, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( NWPLAY_PLAY_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->ctrlID);
	size += GetVALUESize(val->ip);
	size += GetVALUESize(val->udp);
	size += GetVALUESize(val->tcp);
	size += GetVALUESize(val->udpPort);
	size += GetVALUESize(val->tcpPort);

	return size;
}

BOOL WriteVALUE( NWPLAY_PLAY_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->ip, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->udp, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->tcp, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->udpPort, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->tcpPort, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( NWPLAY_PLAY_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->ip, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->udp, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->tcp, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->udpPort, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->tcpPort, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( NWPLAY_POS_CMD* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->ctrlID);
	size += GetVALUESize(val->currentPos);
	size += GetVALUESize(val->totalPos);

	return size;
}

BOOL WriteVALUE( NWPLAY_POS_CMD* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->currentPos, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->totalPos, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( NWPLAY_POS_CMD* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->currentPos, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->totalPos, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( TVTEST_STREAMING_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->enableMode);
	size += GetVALUESize(val->ctrlID);
	size += GetVALUESize(val->serverIP);
	size += GetVALUESize(val->serverPort);
	size += GetVALUESize(val->filePath);
	size += GetVALUESize(val->udpSend);
	size += GetVALUESize(val->tcpSend);
	size += GetVALUESize(val->timeShiftMode);

	return size;
}

BOOL WriteVALUE( TVTEST_STREAMING_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->enableMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->serverIP, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->serverPort, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->filePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->udpSend, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->tcpSend, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->timeShiftMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( TVTEST_STREAMING_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->enableMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->serverIP, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->serverPort, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->filePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->udpSend, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->tcpSend, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->timeShiftMode, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

DWORD GetVALUESize( NWPLAY_TIMESHIFT_INFO* val )
{
	DWORD size = sizeof(DWORD);
	if( val == NULL ){
		return size;
	}

	size += GetVALUESize(val->ctrlID);
	size += GetVALUESize(val->filePath);

	return size;
}

BOOL WriteVALUE( NWPLAY_TIMESHIFT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* writeSize )
{
	DWORD valSize = GetVALUESize( val );
	if( buff == NULL || valSize > buffSize ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	if( WriteVALUE( valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;

	if(val != NULL ){
		if( WriteVALUE( val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( WriteVALUE( val->filePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( writeSize != NULL ){
		*writeSize = pos;
	}
	return TRUE;
}

BOOL ReadVALUE( NWPLAY_TIMESHIFT_INFO* val, BYTE* buff, DWORD buffSize, DWORD* readSize )
{
	if( val == NULL || buff == NULL || buffSize < sizeof(DWORD) ){
		return FALSE;
	}

	DWORD pos = 0;
	DWORD size = 0;
	DWORD valSize = 0;
	if( ReadVALUE( &valSize, buff + pos, buffSize - pos, &size ) == FALSE ){
		return FALSE;
	}
	pos += size;
	if( buffSize < valSize ){
		return FALSE;
	}

	if( pos < valSize ){
		if( ReadVALUE( &val->ctrlID, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
		if( ReadVALUE( &val->filePath, buff + pos, buffSize - pos, &size ) == FALSE ){
			return FALSE;
		}
		pos += size;
	}

	if( readSize != NULL ){
		*readSize = pos;
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////
//���o�[�W�����R�}���h���M�p�o�C�i���쐬�֐�
BOOL CreateReserveDataStream(OLD_RESERVE_DATA* pData, CMD_STREAM* pCmd)
{
	if( pData == NULL || pCmd == NULL ){
		return FALSE;
	}
	pCmd->dataSize = sizeof(DWORD)*11 + sizeof(SYSTEMTIME) + sizeof(unsigned short)*4
		+ sizeof(unsigned char)*2 + sizeof(BOOL)*4 + sizeof(WORD)*1+ sizeof(int)*2;
	pCmd->dataSize += (DWORD)(pData->strTitle.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strStationName.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strComment.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strBatPath.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strRecFolder.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strRecFilePath.length()+1)*sizeof(WCHAR);
	pCmd->data = new BYTE[pCmd->dataSize];
	ZeroMemory(pCmd->data, pCmd->dataSize);

	DWORD dwStrSize = 0;
	DWORD dwPos = 0;

	dwStrSize = (DWORD)(pData->strTitle.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strTitle.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	memcpy(pCmd->data + dwPos, &pData->StartTime, sizeof(SYSTEMTIME));
	dwPos+=sizeof(SYSTEMTIME);

	memcpy(pCmd->data + dwPos, &pData->dwDurationSec, sizeof(DWORD));
	dwPos+=sizeof(DWORD);

	dwStrSize = (DWORD)(pData->strStationName.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strStationName.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	memcpy(pCmd->data + dwPos, &pData->usONID, sizeof(unsigned short));
	dwPos+=sizeof(unsigned short);

	memcpy(pCmd->data + dwPos, &pData->usTSID, sizeof(unsigned short));
	dwPos+=sizeof(unsigned short);

	memcpy(pCmd->data + dwPos, &pData->usServiceID, sizeof(unsigned short));
	dwPos+=sizeof(unsigned short);

	memcpy(pCmd->data + dwPos, &pData->usEventID, sizeof(unsigned short));
	dwPos+=sizeof(unsigned short);

	memcpy(pCmd->data + dwPos, &pData->ucPriority, sizeof(unsigned char));
	dwPos+=sizeof(unsigned char);

	memcpy(pCmd->data + dwPos, &pData->ucTuijyuu, sizeof(unsigned char));
	dwPos+=sizeof(unsigned char);

	dwStrSize = (DWORD)(pData->strComment.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strComment.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	memcpy(pCmd->data + dwPos, &pData->dwRecMode, sizeof(DWORD));
	dwPos+=sizeof(DWORD);

	memcpy(pCmd->data + dwPos, &pData->bPittari, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	dwStrSize = (DWORD)(pData->strBatPath.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strBatPath.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	memcpy(pCmd->data + dwPos, &pData->dwReserveID, sizeof(DWORD));
	dwPos+=sizeof(DWORD);

	memcpy(pCmd->data + dwPos, &pData->bSetWait, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	memcpy(pCmd->data + dwPos, &pData->dwPiledUpMode, sizeof(DWORD));
	dwPos+=sizeof(BOOL);

	dwStrSize = (DWORD)(pData->strRecFolder.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strRecFolder.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	memcpy(pCmd->data + dwPos, &pData->wSuspendMode, sizeof(WORD));
	dwPos+=sizeof(WORD);

	memcpy(pCmd->data + dwPos, &pData->bReboot, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	dwStrSize = (DWORD)(pData->strRecFilePath.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strRecFilePath.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	memcpy(pCmd->data + dwPos, &pData->bUseMargine, sizeof(BOOL));
	dwPos+=sizeof(BOOL);
	memcpy(pCmd->data + dwPos, &pData->iStartMargine, sizeof(int));
	dwPos+=sizeof(int);
	memcpy(pCmd->data + dwPos, &pData->iEndMargine, sizeof(int));
	dwPos+=sizeof(int);
	memcpy(pCmd->data + dwPos, &pData->dwServiceMode, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	

	return TRUE;
}

BOOL CopyReserveData(OLD_RESERVE_DATA* pstData, CMD_STREAM* pCmd)
{
	if( pstData == NULL || pCmd == NULL ){
		return FALSE;
	}
	if(pCmd->data == NULL ){
		OutputDebugString(L"��CopyReserveData�@NULL");
		return FALSE;
	}
	if(pCmd->dataSize == 0 ){
		OutputDebugString(L"��CopyReserveData�@0");
		return FALSE;
	}
	BYTE* pBuff = pCmd->data;
	DWORD dwPos = 0;
	DWORD dwStrSize=0;

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strTitle = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	pstData->StartTime = *(SYSTEMTIME*)(pBuff+dwPos);
	dwPos+=sizeof(SYSTEMTIME);

	pstData->dwDurationSec = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strStationName = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	pstData->usONID = *(unsigned short*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned short);

	pstData->usTSID = *(unsigned short*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned short);

	pstData->usServiceID = *(unsigned short*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned short);

	pstData->usEventID = *(unsigned short*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned short);

	pstData->ucPriority = *(unsigned char*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned char);

	pstData->ucTuijyuu = *(unsigned char*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned char);

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strComment = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	pstData->dwRecMode = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);

	pstData->bPittari = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strBatPath = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	pstData->dwReserveID = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);

	pstData->bSetWait = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	pstData->dwPiledUpMode = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strRecFolder = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	pstData->wSuspendMode = *(WORD*)(pBuff+dwPos);
	dwPos+=sizeof(WORD);

	pstData->bReboot = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strRecFilePath = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	if( dwPos < pCmd->dataSize ){
		pstData->bUseMargine = *(BOOL*)(pBuff+dwPos);
		dwPos+=sizeof(BOOL);
		pstData->iStartMargine = *(int*)(pBuff+dwPos);
		dwPos+=sizeof(int);
		pstData->iEndMargine = *(int*)(pBuff+dwPos);
		dwPos+=sizeof(int);
		pstData->dwServiceMode = *(DWORD*)(pBuff+dwPos);
		dwPos+=sizeof(DWORD);
	}else{
		pstData->bUseMargine = FALSE;
		pstData->iStartMargine = 10;
		pstData->iEndMargine = 2;
		pstData->dwServiceMode = 0;
	}


	return TRUE;
}

BOOL CreateSearchKeyDataStream(OLD_SEARCH_KEY* pData, CMD_STREAM* pCmd)
{
	if( pData == NULL || pCmd == NULL ){
		return FALSE;
	}
	pCmd->dataSize = sizeof(DWORD)*7 + sizeof(BOOL)*11 + sizeof(int)*12 + sizeof(WORD)*1;
	pCmd->dataSize += (DWORD)(pData->strAnd.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strNot.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->CHIDList.size())*sizeof(__int64);
	pCmd->dataSize += (DWORD)(pData->strBat.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strRecFolder.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strPattern.length()+1)*sizeof(WCHAR);
	pCmd->data = new BYTE[pCmd->dataSize];
	ZeroMemory(pCmd->data, pCmd->dataSize);

	DWORD dwStrSize = 0;
	DWORD dwPos = 0;

	dwStrSize = (DWORD)(pData->strAnd.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strAnd.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	dwStrSize = (DWORD)(pData->strNot.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strNot.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	memcpy(pCmd->data + dwPos, &pData->bTitle, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	memcpy(pCmd->data + dwPos, &pData->iJanru, sizeof(int));
	dwPos+=sizeof(int);

	memcpy(pCmd->data + dwPos, &pData->iSH, sizeof(int));
	dwPos+=sizeof(int);

	memcpy(pCmd->data + dwPos, &pData->iSM, sizeof(int));
	dwPos+=sizeof(int);

	memcpy(pCmd->data + dwPos, &pData->iEH, sizeof(int));
	dwPos+=sizeof(int);

	memcpy(pCmd->data + dwPos, &pData->iEM, sizeof(int));
	dwPos+=sizeof(int);

	memcpy(pCmd->data + dwPos, &pData->bChkMon, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	memcpy(pCmd->data + dwPos, &pData->bChkTue, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	memcpy(pCmd->data + dwPos, &pData->bChkWed, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	memcpy(pCmd->data + dwPos, &pData->bChkThu, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	memcpy(pCmd->data + dwPos, &pData->bChkFri, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	memcpy(pCmd->data + dwPos, &pData->bChkSat, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	memcpy(pCmd->data + dwPos, &pData->bChkSun, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	DWORD dwTemp = (DWORD)pData->CHIDList.size();
	memcpy(pCmd->data + dwPos, &dwTemp, sizeof(__int64));
	dwPos+=sizeof(DWORD);

	for( int i=0; i<(int)pData->CHIDList.size(); i++ ){
		memcpy(pCmd->data + dwPos, &pData->CHIDList[i], sizeof(__int64));
		dwPos+=sizeof(__int64);
	}

	memcpy(pCmd->data + dwPos, &pData->iAutoAddID, sizeof(int));
	dwPos+=sizeof(int);

	memcpy(pCmd->data + dwPos, &pData->iPriority, sizeof(int));
	dwPos+=sizeof(int);

	memcpy(pCmd->data + dwPos, &pData->iTuijyuu, sizeof(int));
	dwPos+=sizeof(int);

	memcpy(pCmd->data + dwPos, &pData->iRecMode, sizeof(int));
	dwPos+=sizeof(int);

	memcpy(pCmd->data + dwPos, &pData->iPittari, sizeof(int));
	dwPos+=sizeof(int);

	dwStrSize = (DWORD)(pData->strBat.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strBat.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	dwStrSize = (DWORD)(pData->strRecFolder.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strRecFolder.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	memcpy(pCmd->data + dwPos, &pData->wSuspendMode, sizeof(WORD));
	dwPos+=sizeof(WORD);

	memcpy(pCmd->data + dwPos, &pData->bReboot, sizeof(BOOL));
	dwPos+=sizeof(BOOL);

	memcpy(pCmd->data + dwPos, &pData->bUseMargine, sizeof(BOOL));
	dwPos+=sizeof(BOOL);
	memcpy(pCmd->data + dwPos, &pData->iStartMargine, sizeof(int));
	dwPos+=sizeof(int);
	memcpy(pCmd->data + dwPos, &pData->iEndMargine, sizeof(int));
	dwPos+=sizeof(int);
	memcpy(pCmd->data + dwPos, &pData->dwServiceMode, sizeof(DWORD));
	dwPos+=sizeof(DWORD);

	memcpy(pCmd->data + dwPos, &pData->bRegExp, sizeof(BOOL));
	dwPos+=sizeof(BOOL);
	dwStrSize = (DWORD)(pData->strPattern.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strPattern.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	return TRUE;
}

BOOL CopySearchKeyData(OLD_SEARCH_KEY* pstData, CMD_STREAM* pCmd)
{
	if( pstData == NULL || pCmd == NULL ){
		return FALSE;
	}
	if(pCmd->data == NULL ){
		OutputDebugString(L"��CopySearchKeyData�@NULL");
		return FALSE;
	}
	if(pCmd->dataSize == 0 ){
		OutputDebugString(L"��CopySearchKeyData�@0");
		return FALSE;
	}
	BYTE* pBuff = pCmd->data;
	DWORD dwPos = 0;
	DWORD dwStrSize=0;

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strAnd = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strNot = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	pstData->bTitle = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	pstData->iJanru = *(int*)(pBuff+dwPos);
	dwPos+=sizeof(int);

	pstData->iSH = *(int*)(pBuff+dwPos);
	dwPos+=sizeof(int);

	pstData->iSM = *(int*)(pBuff+dwPos);
	dwPos+=sizeof(int);

	pstData->iEH = *(int*)(pBuff+dwPos);
	dwPos+=sizeof(int);

	pstData->iEM = *(int*)(pBuff+dwPos);
	dwPos+=sizeof(int);

	pstData->bChkMon = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	pstData->bChkTue = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	pstData->bChkWed = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	pstData->bChkThu = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	pstData->bChkFri = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	pstData->bChkSat = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	pstData->bChkSun = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	DWORD dwCount = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	for( DWORD i=0; i<dwCount; i++ ){
		__int64 Item;
		Item = *(__int64*)(pBuff+dwPos);
		dwPos+=sizeof(__int64);
		pstData->CHIDList.push_back(Item);
	}

	pstData->iAutoAddID = *(int*)(pBuff+dwPos);
	dwPos+=sizeof(int);

	pstData->iPriority = *(int*)(pBuff+dwPos);
	dwPos+=sizeof(int);

	pstData->iTuijyuu = *(int*)(pBuff+dwPos);
	dwPos+=sizeof(int);

	pstData->iRecMode = *(int*)(pBuff+dwPos);
	dwPos+=sizeof(int);

	pstData->iPittari = *(int*)(pBuff+dwPos);
	dwPos+=sizeof(int);

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strBat = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strRecFolder = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	pstData->wSuspendMode = *(WORD*)(pBuff+dwPos);
	dwPos+=sizeof(WORD);

	pstData->bReboot = *(BOOL*)(pBuff+dwPos);
	dwPos+=sizeof(BOOL);

	if( dwPos < pCmd->dataSize ){
		pstData->bUseMargine = *(BOOL*)(pBuff+dwPos);
		dwPos+=sizeof(BOOL);
		pstData->iStartMargine = *(int*)(pBuff+dwPos);
		dwPos+=sizeof(int);
		pstData->iEndMargine = *(int*)(pBuff+dwPos);
		dwPos+=sizeof(int);
		pstData->dwServiceMode = *(DWORD*)(pBuff+dwPos);
		dwPos+=sizeof(DWORD);
		if( dwPos < pCmd->dataSize ){
			pstData->bRegExp = *(BOOL*)(pBuff+dwPos);
			dwPos+=sizeof(BOOL);

			dwStrSize = *(DWORD*)(pBuff+dwPos);
			dwPos+=sizeof(DWORD);
			pstData->strPattern = (WCHAR*)(pBuff+dwPos);
			dwPos+=dwStrSize;
		}else{
			pstData->bRegExp = FALSE;
			pstData->strPattern = L"";
		}
	}else{
		pstData->bUseMargine = FALSE;
		pstData->iStartMargine = 10;
		pstData->iEndMargine = 2;
		pstData->dwServiceMode = 0;
		pstData->bRegExp = FALSE;
		pstData->strPattern = L"";
	}

	return TRUE;
}

BOOL CreateEventInfoData3Stream(OLD_EVENT_INFO_DATA3* pData, CMD_STREAM* pCmd)
{
	if( pData == NULL || pCmd == NULL ){
		return FALSE;
	}
	pCmd->dataSize = sizeof(DWORD)*9 + sizeof(SYSTEMTIME) + sizeof(WORD)*4
		+ sizeof(unsigned char)*5;
	pCmd->dataSize += (DWORD)(pData->strEventName.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strEventText.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strEventExtText.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strComponentTypeText.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->strAudioComponentTypeText.length()+1)*sizeof(WCHAR);
	pCmd->dataSize += (DWORD)(pData->NibbleList.size())*sizeof(OLD_NIBBLE_DATA);
	pCmd->dataSize += (DWORD)(pData->EventRelayList.size())*sizeof(OLD_EVENT_ID_INFO);
	pCmd->dataSize += (DWORD)(pData->EventGroupList.size())*sizeof(OLD_EVENT_ID_INFO);
	pCmd->data = new BYTE[pCmd->dataSize];
	ZeroMemory(pCmd->data, pCmd->dataSize);

	DWORD dwStrSize = 0;
	DWORD dwPos = 0;

	memcpy(pCmd->data + dwPos, &pData->wOriginalNID, sizeof(WORD));
	dwPos+=sizeof(WORD);

	memcpy(pCmd->data + dwPos, &pData->wTSID, sizeof(WORD));
	dwPos+=sizeof(WORD);

	memcpy(pCmd->data + dwPos, &pData->wServiceID, sizeof(WORD));
	dwPos+=sizeof(WORD);

	memcpy(pCmd->data + dwPos, &pData->wEventID, sizeof(WORD));
	dwPos+=sizeof(WORD);

	dwStrSize = (DWORD)(pData->strEventName.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strEventName.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	dwStrSize = (DWORD)(pData->strEventText.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strEventText.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	dwStrSize = (DWORD)(pData->strEventExtText.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strEventExtText.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	memcpy(pCmd->data + dwPos, &pData->stStartTime, sizeof(SYSTEMTIME));
	dwPos+=sizeof(SYSTEMTIME);

	memcpy(pCmd->data + dwPos, &pData->dwDurationSec, sizeof(DWORD));
	dwPos+=sizeof(DWORD);

	memcpy(pCmd->data + dwPos, &pData->ucComponentType, sizeof(unsigned char));
	dwPos+=sizeof(unsigned char);

	dwStrSize = (DWORD)(pData->strComponentTypeText.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strComponentTypeText.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	memcpy(pCmd->data + dwPos, &pData->ucAudioComponentType, sizeof(unsigned char));
	dwPos+=sizeof(unsigned char);

	memcpy(pCmd->data + dwPos, &pData->ucESMultiLangFlag, sizeof(unsigned char));
	dwPos+=sizeof(unsigned char);

	memcpy(pCmd->data + dwPos, &pData->ucMainComponentFlag, sizeof(unsigned char));
	dwPos+=sizeof(unsigned char);

	memcpy(pCmd->data + dwPos, &pData->ucSamplingRate, sizeof(unsigned char));
	dwPos+=sizeof(unsigned char);

	dwStrSize = (DWORD)(pData->strAudioComponentTypeText.length()+1)*sizeof(WCHAR);
	memcpy(pCmd->data + dwPos, &dwStrSize, sizeof(DWORD));
	dwPos+=sizeof(DWORD);
	memcpy(pCmd->data + dwPos, pData->strAudioComponentTypeText.c_str(), dwStrSize);
	dwPos+=dwStrSize;

	DWORD dwTemp = (DWORD)pData->NibbleList.size();
	memcpy(pCmd->data + dwPos, &dwTemp, sizeof(DWORD));
	dwPos+=sizeof(DWORD);

	for( int i=0; i<(int)pData->NibbleList.size(); i++ ){
		memcpy(pCmd->data + dwPos, &pData->NibbleList[i], sizeof(OLD_NIBBLE_DATA));
		dwPos+=sizeof(OLD_NIBBLE_DATA);
	}

	dwTemp = (DWORD)pData->EventRelayList.size();
	memcpy(pCmd->data + dwPos, &dwTemp, sizeof(DWORD));
	dwPos+=sizeof(DWORD);

	for( int i=0; i<(int)pData->EventRelayList.size(); i++ ){
		memcpy(pCmd->data + dwPos, &pData->EventRelayList[i], sizeof(OLD_EVENT_ID_INFO));
		dwPos+=sizeof(OLD_EVENT_ID_INFO);
	}

	dwTemp = (DWORD)pData->EventGroupList.size();
	memcpy(pCmd->data + dwPos, &dwTemp, sizeof(DWORD));
	dwPos+=sizeof(DWORD);

	for( int i=0; i<(int)pData->EventGroupList.size(); i++ ){
		memcpy(pCmd->data + dwPos, &pData->EventGroupList[i], sizeof(OLD_EVENT_ID_INFO));
		dwPos+=sizeof(OLD_EVENT_ID_INFO);
	}

	return TRUE;
}

BOOL CopyEventInfoData3(OLD_EVENT_INFO_DATA3* pstData, CMD_STREAM* pCmd)
{
	if( pstData == NULL || pCmd == NULL ){
		return FALSE;
	}
	if(pCmd->data == NULL ){
		OutputDebugString(L"��CopyEventInfoData3�@NULL");
		return FALSE;
	}
	if(pCmd->dataSize == 0 ){
		OutputDebugString(L"��CopyEventInfoData3�@0");
		return FALSE;
	}
	BYTE* pBuff = pCmd->data;
	DWORD dwPos = 0;
	DWORD dwStrSize=0;

	pstData->wOriginalNID = *(WORD*)(pBuff+dwPos);
	dwPos+=sizeof(WORD);

	pstData->wTSID = *(WORD*)(pBuff+dwPos);
	dwPos+=sizeof(WORD);

	pstData->wServiceID = *(WORD*)(pBuff+dwPos);
	dwPos+=sizeof(WORD);

	pstData->wEventID = *(WORD*)(pBuff+dwPos);
	dwPos+=sizeof(WORD);

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strEventName = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strEventText = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strEventExtText = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	pstData->stStartTime = *(SYSTEMTIME*)(pBuff+dwPos);
	dwPos+=sizeof(SYSTEMTIME);

	pstData->dwDurationSec = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);

	pstData->ucComponentType = *(unsigned char*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned char);

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strComponentTypeText = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	pstData->ucAudioComponentType = *(unsigned char*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned char);

	pstData->ucESMultiLangFlag = *(unsigned char*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned char);

	pstData->ucMainComponentFlag = *(unsigned char*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned char);

	pstData->ucSamplingRate = *(unsigned char*)(pBuff+dwPos);
	dwPos+=sizeof(unsigned char);

	dwStrSize = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	pstData->strAudioComponentTypeText = (WCHAR*)(pBuff+dwPos);
	dwPos+=dwStrSize;

	DWORD dwCount = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	for( DWORD i=0; i<dwCount; i++ ){
		OLD_NIBBLE_DATA Item;
		Item = *(OLD_NIBBLE_DATA*)(pBuff+dwPos);
		dwPos+=sizeof(OLD_NIBBLE_DATA);
		pstData->NibbleList.push_back(Item);
	}

	dwCount = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	for( DWORD i=0; i<dwCount; i++ ){
		OLD_EVENT_ID_INFO Item;
		Item = *(OLD_EVENT_ID_INFO*)(pBuff+dwPos);
		dwPos+=sizeof(OLD_EVENT_ID_INFO);
		pstData->EventRelayList.push_back(Item);
	}

	dwCount = *(DWORD*)(pBuff+dwPos);
	dwPos+=sizeof(DWORD);
	for( DWORD i=0; i<dwCount; i++ ){
		OLD_EVENT_ID_INFO Item;
		Item = *(OLD_EVENT_ID_INFO*)(pBuff+dwPos);
		dwPos+=sizeof(OLD_EVENT_ID_INFO);
		pstData->EventGroupList.push_back(Item);
	}
	return TRUE;
}

void CopyOldNew(OLD_RESERVE_DATA* src, RESERVE_DATA* dest)
{
	dest->title = src->strTitle;
	dest->startTime = src->StartTime;
	dest->durationSecond = src->dwDurationSec;
	dest->stationName = src->strStationName;
	dest->originalNetworkID = src->usONID;
	dest->transportStreamID = src->usTSID;
	dest->serviceID = src->usServiceID;
	dest->eventID = src->usEventID;
	dest->comment = src->strComment;
	dest->reserveID = src->dwReserveID;
	dest->recWaitFlag = 0;
	dest->overlapMode = 0;
	dest->recFilePath = L"";
	dest->startTimeEpg = src->StartTime;
	dest->recSetting.recMode = (BYTE)src->dwRecMode;
	dest->recSetting.priority = src->ucPriority;
	dest->recSetting.tuijyuuFlag = src->ucTuijyuu;
	dest->recSetting.serviceMode = src->dwServiceMode;
	dest->recSetting.pittariFlag = src->bPittari;
	dest->recSetting.batFilePath = src->strBatPath;
	if( src->wSuspendMode == 0 ){
		dest->recSetting.suspendMode = 4;
	}else if( src->wSuspendMode == 4 ){
		dest->recSetting.suspendMode = 0;
	}else{
		dest->recSetting.suspendMode = (BYTE)src->wSuspendMode;
	}
	dest->recSetting.rebootFlag = src->bReboot;
	dest->recSetting.useMargineFlag = src->bUseMargine;
	dest->recSetting.startMargine = src->iStartMargine;
	dest->recSetting.endMargine = src->iEndMargine;
	dest->recSetting.continueRecFlag = 0;
	dest->recSetting.partialRecFlag = 0;
	dest->recSetting.tunerID = 0;
	if( src->strRecFolder.size() > 0 ){
		REC_FILE_SET_INFO folder;
		folder.recFolder = src->strRecFolder;
		folder.writePlugIn = L"Write_Default.dll";
		dest->recSetting.recFolderList.push_back(folder);
	}
}

void CopyOldNew(OLD_SEARCH_KEY* src, EPG_AUTO_ADD_DATA* dest)
{
	dest->dataID = (DWORD)src->iAutoAddID;
	if( src->bRegExp == FALSE ){
		dest->searchInfo.andKey = src->strAnd;
		dest->searchInfo.notKey = src->strNot;
	}else{
		dest->searchInfo.andKey = src->strPattern;
	}
	dest->searchInfo.regExpFlag = (BYTE)src->bRegExp;
	dest->searchInfo.titleOnlyFlag = src->bTitle;
	if( src->iJanru != -1 ){
		EPGDB_CONTENT_DATA content;
		content.user_nibble_1 = (BYTE)src->iJanru;
		content.user_nibble_2 = 0xFF;
		dest->searchInfo.contentList.push_back(content);
	}
	if( !(src->bChkSun == TRUE && src->bChkMon == TRUE && src->bChkTue == TRUE && src->bChkWed == TRUE &&
		src->bChkThu == TRUE && src->bChkFri == TRUE && src->bChkSat == TRUE && 
		src->iSH == 0 && src->iSM == 0 && src->iEH == 23 && src->iEM == 59)
		){
			if(src->bChkSun == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 0;
				date.endDayOfWeek = 0;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->searchInfo.dateList.push_back(date);
			}
			if(src->bChkMon == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 1;
				date.endDayOfWeek = 1;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->searchInfo.dateList.push_back(date);
			}
			if(src->bChkTue == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 2;
				date.endDayOfWeek = 2;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->searchInfo.dateList.push_back(date);
			}
			if(src->bChkWed == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 3;
				date.endDayOfWeek = 3;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->searchInfo.dateList.push_back(date);
			}
			if(src->bChkThu == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 4;
				date.endDayOfWeek = 4;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->searchInfo.dateList.push_back(date);
			}
			if(src->bChkFri == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 5;
				date.endDayOfWeek = 5;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->searchInfo.dateList.push_back(date);
			}
			if(src->bChkSat == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 6;
				date.endDayOfWeek = 6;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->searchInfo.dateList.push_back(date);
			}
	}
	dest->searchInfo.serviceList = src->CHIDList;

	dest->recSetting.recMode = (BYTE)src->iRecMode;
	dest->recSetting.priority = (BYTE)src->iPriority;
	dest->recSetting.tuijyuuFlag = (BYTE)src->iTuijyuu;
	dest->recSetting.serviceMode = src->dwServiceMode;
	dest->recSetting.pittariFlag = (BYTE)src->iPittari;
	dest->recSetting.batFilePath = src->strBat;
	if( src->wSuspendMode == 0 ){
		dest->recSetting.suspendMode = 4;
	}else if( src->wSuspendMode == 4 ){
		dest->recSetting.suspendMode = 0;
	}else{
		dest->recSetting.suspendMode = (BYTE)src->wSuspendMode;
	}
	dest->recSetting.rebootFlag = src->bReboot;
	dest->recSetting.useMargineFlag = src->bUseMargine;
	dest->recSetting.startMargine = src->iStartMargine;
	dest->recSetting.endMargine = src->iEndMargine;
	dest->recSetting.continueRecFlag = 0;
	dest->recSetting.partialRecFlag = 0;
	dest->recSetting.tunerID = 0;
	if( src->strRecFolder.size() > 0 ){
		REC_FILE_SET_INFO folder;
		folder.recFolder = src->strRecFolder;
		folder.writePlugIn = L"Write_Default.dll";
		dest->recSetting.recFolderList.push_back(folder);
	}
}

void CopyOldNew(OLD_SEARCH_KEY* src, EPGDB_SEARCH_KEY_INFO* dest)
{
	if( src->bRegExp == FALSE ){
		dest->andKey = src->strAnd;
		dest->notKey = src->strNot;
	}else{
		dest->andKey = src->strPattern;
	}
	dest->regExpFlag = (BYTE)src->bRegExp;
	dest->titleOnlyFlag = src->bTitle;
	if( src->iJanru != -1 ){
		EPGDB_CONTENT_DATA content;
		content.user_nibble_1 = (BYTE)src->iJanru;
		content.user_nibble_2 = 0xFF;
		dest->contentList.push_back(content);
	}
	if( !(src->bChkSun == TRUE && src->bChkMon == TRUE && src->bChkTue == TRUE && src->bChkWed == TRUE &&
		src->bChkThu == TRUE && src->bChkFri == TRUE && src->bChkSat == TRUE && 
		src->iSH == 0 && src->iSM == 0 && src->iEH == 23 && src->iEM == 59)
		){
			if(src->bChkSun == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 0;
				date.endDayOfWeek = 0;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->dateList.push_back(date);
			}
			if(src->bChkMon == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 1;
				date.endDayOfWeek = 1;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->dateList.push_back(date);
			}
			if(src->bChkTue == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 2;
				date.endDayOfWeek = 2;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->dateList.push_back(date);
			}
			if(src->bChkWed == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 3;
				date.endDayOfWeek = 3;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->dateList.push_back(date);
			}
			if(src->bChkThu == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 4;
				date.endDayOfWeek = 4;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->dateList.push_back(date);
			}
			if(src->bChkFri == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 5;
				date.endDayOfWeek = 5;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->dateList.push_back(date);
			}
			if(src->bChkSat == TRUE){
				EPGDB_SEARCH_DATE_INFO date;
				date.startDayOfWeek = 6;
				date.endDayOfWeek = 6;
				date.startHour = (WORD)src->iSH;
				date.startMin = (WORD)src->iSM;
				date.endHour = (WORD)src->iEH;
				date.endMin = (WORD)src->iEM;
				dest->dateList.push_back(date);
			}
	}
	dest->serviceList = src->CHIDList;
}
