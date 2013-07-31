#include "StdAfx.h"
#include "CreatePMTPacket.h"

CCreatePMTPacket::CCreatePMTPacket(void)
{
	this->needCaption = TRUE;
	this->needData = TRUE;

	this->lastPmtPID = 0xFFFF;
	this->lastPcrPID = 0xFFFF;
	this->lastPgNumber = 0xFFFF;
	this->lastVersion = 0xFF;

	this->firstDescBuff = NULL;
	this->firstDescBuffSize = 0;

	this->createPSI = NULL;
	this->createPSISize = 0;
	
	this->createPacket = NULL;
	this->createPacketSize = 0;

	this->createVer = 0;
	this->createCounter = 0;
}

CCreatePMTPacket::~CCreatePMTPacket(void)
{
	SAFE_DELETE_ARRAY(this->firstDescBuff);
	this->firstDescBuffSize = 0;
	ClearSecondBuff();

	SAFE_DELETE_ARRAY(this->createPSI);
	this->createPSISize = 0;
	SAFE_DELETE_ARRAY(this->createPacket);
	this->createPacketSize = 0;
}

//PMT�쐬���̃��[�h
//�����F
// needCaption			[IN]�����f�[�^���܂߂邩�ǂ����iTRUE:�܂߂�AFALSE�F�܂߂Ȃ��j
// needData				[IN]�f�[�^�J���[�Z�����܂߂邩�ǂ����iTRUE:�܂߂�AFALSE�F�܂߂Ȃ��j
void CCreatePMTPacket::SetCreateMode(
	BOOL needCaption,
	BOOL needData
)
{
	if( this->needCaption != needCaption || this->needData != needData ){
		this->needCaption = needCaption;
		this->needData = needData;
		CreatePMT();
	}
}

//�쐬���ƂȂ�PMT�̃p�P�b�g�����
//�߂�l�F
// �G���[�R�[�h
//�����F
// packet			//[IN] PMT�̃p�P�b�g
DWORD CCreatePMTPacket::AddData(
	CTSPacketUtil* packet
)
{
	if( packet == NULL ){
		return ERR_FALSE;
	}
	DWORD ret = ERR_NEED_NEXT_PACKET;
	ret = buffUtil.Add188TS(packet);
	if( ret == TRUE ){
		this->lastPmtPID = packet->PID;

		BYTE* section = NULL;
		DWORD sectionSize = 0;
		while( buffUtil.GetSectionBuff( &section, &sectionSize ) == TRUE ){
			ret = DecodePMT(section, sectionSize);
		}
	}
	return ret;
}

DWORD CCreatePMTPacket::DecodePMT(BYTE* data, DWORD dataSize)
{
	if( data == NULL ){
		return ERR_FALSE;
	}

	if( dataSize < 7 ){
		return ERR_FALSE;
	}

	BYTE table_id;
	BYTE section_syntax_indicator;
	WORD section_length;
	WORD program_number;
	BYTE version_number;
	BYTE current_next_indicator;
	BYTE section_number;
	BYTE last_section_number;
	WORD PCR_PID;
	WORD program_info_length;
	DWORD crc32;


	DWORD readSize = 0;
	//////////////////////////////////////////////////////
	//��͏���
	table_id = data[0];
	section_syntax_indicator = (data[1]&0x80)>>7;
	section_length = ((WORD)data[1]&0x0F)<<8 | data[2];
	readSize+=3;

	if( section_syntax_indicator != 1 ){
		//�Œ�l����������
		_OutputDebugString(L"CCreatePMTPacket::section_syntax_indicator Err");
		return ERR_FALSE;
	}
	if( table_id != 0x02 ){
		//table_id����������
		_OutputDebugString(L"CCreatePMTPacket::table_id Err");
		return ERR_FALSE;
	}
	if( readSize+section_length > dataSize && section_length > 3){
		//�T�C�Y�ُ�
		_OutputDebugString(L"CCreatePMTPacket::section_length Err");
		return ERR_FALSE;
	}
	//CRC�`�F�b�N
	crc32 = ((DWORD)data[3+section_length-4])<<24 |
		((DWORD)data[3+section_length-3])<<16 |
		((DWORD)data[3+section_length-2])<<8 |
		data[3+section_length-1];
	if( crc32 != _Crc32(3+section_length-4, data) ){
		_OutputDebugString(L"CCreatePMTPacket::CRC Err");
		return ERR_FALSE;
	}

	if( section_length < 8 ){
		_OutputDebugString(L"CCreatePMTPacket::section_length %d Err2", section_length);
		return ERR_FALSE;
	}

	program_number = ((WORD)data[readSize])<<8 | data[readSize+1];
	version_number = (data[readSize+2]&0x3E)>>1;
	current_next_indicator = data[readSize+2]&0x01;
	section_number = data[readSize+3];
	last_section_number = data[readSize+4];
	PCR_PID = ((WORD)data[readSize+5]&0x1F)<<8 | data[readSize+6];
	program_info_length = ((WORD)data[readSize+7]&0x0F)<<8 | data[readSize+8];
	readSize += 9;

	if( this->lastPcrPID == PCR_PID && this->lastPgNumber == program_number && this->lastVersion == version_number ){
		//�o�[�W���������Ȃ̂ł���ȏ�K�v�Ȃ�
		return ERR_NO_CHAGE;
	}

	this->lastPcrPID = PCR_PID;
	this->lastPgNumber = program_number;
	this->lastVersion = version_number;

	//�ĉ��
	this->emmPIDMap.clear();
	SAFE_DELETE_ARRAY(this->firstDescBuff);
	this->firstDescBuffSize = 0;
	ClearSecondBuff();

	//descriptor1
	//�o�C�i�������R�s�[
	this->firstDescBuffSize = (WORD)readSize + program_info_length;
	if(this->firstDescBuffSize > 0 ){
		this->firstDescBuff = new BYTE[this->firstDescBuffSize];
		memcpy(this->firstDescBuff, data, this->firstDescBuffSize);
	}

	//EMM���邩�����`�F�b�N
	WORD infoRead = 0;
	while(infoRead < program_info_length){
		BYTE descriptor_tag = data[readSize];
		BYTE descriptor_length = data[readSize+1];
		readSize+=2;

		if( descriptor_tag == 0x09 && descriptor_length >= 4){
			//CA
			WORD CA_PID = ((WORD)data[readSize+2]&0x1F)<<8 | (WORD)data[readSize+3];
			this->emmPIDMap.insert(pair<WORD,WORD>(CA_PID, 0));
		}
		readSize += descriptor_length;

		infoRead+= 2+descriptor_length;
	}

	//descriptor2
	while( readSize < (DWORD)section_length+3-4 ){
		SECOND_DESC_BUFF* item = new SECOND_DESC_BUFF;
		item->stream_type = data[readSize];
		item->elementary_PID = ((WORD)data[readSize+1]&0x1F)<<8 | data[readSize+2];
		item->ES_info_length = ((WORD)data[readSize+3]&0x0F)<<8 | data[readSize+4];
		item->descBuffSize = item->ES_info_length + 5;
		if( item->descBuffSize > 0 ){
			item->descBuff = new BYTE[item->descBuffSize];
			memcpy(item->descBuff, data+readSize, 5);
		}
		readSize += 5;

		//descriptor
		infoRead = 0;
		while(infoRead < item->ES_info_length){
			BYTE descriptor_tag = data[readSize];
			BYTE descriptor_length = data[readSize+1];

			if( descriptor_tag == 0x09 && descriptor_length >= 4){
				//CA
				WORD CA_PID = ((WORD)data[2+readSize+2]&0x1F)<<8 | (WORD)data[2+readSize+3];
				this->emmPIDMap.insert(pair<WORD,WORD>(CA_PID, 0));

				memcpy(item->descBuff+5+infoRead, data+readSize, 2+descriptor_length);
			}else if( descriptor_tag == 0xC0 ){
				//�K�w�`���L�q�q
				item->quality = data[2+readSize]&0x01;
				item->qualityPID = ((WORD)data[2+readSize+1]&0x1F)<<8 | data[2+readSize+2];

				memcpy(item->descBuff+5+infoRead, data+readSize, 2+descriptor_length);
			}else{
				memcpy(item->descBuff+5+infoRead, data+readSize, 2+descriptor_length);
			}
			readSize += 2+descriptor_length;
			infoRead += 2+descriptor_length;
		}

		secondDescBuff.push_back(item);
	}

	CreatePMT();

	return TRUE;
}

void CCreatePMTPacket::CreatePMT()
{
	if( firstDescBuffSize == 0 || this->firstDescBuff == NULL ){
		return;
	}
	SAFE_DELETE_ARRAY(this->createPSI);
	this->createPSISize = 0;
	this->needPIDList.clear();

	BOOL findVHighQ = FALSE;
	BOOL findAHighQ = FALSE;
	BOOL findMPEG2V = FALSE;
	BOOL findAAC = FALSE;

	this->createVer++;
	if( this->createVer > 31 ){
		this->createVer = 0;
	}

	//�f�[�^�ꗗ�`�F�b�N
	for( size_t i=0; i<secondDescBuff.size(); i++ ){
		if( secondDescBuff[i]->quality == 1 ){
			//���K�w����
			if( secondDescBuff[i]->stream_type == 0x02 ){
				findVHighQ = TRUE;
			}else if( secondDescBuff[i]->stream_type == 0x0F ){
				findAHighQ = TRUE;
			}
		}
		if( secondDescBuff[i]->stream_type == 0x02 ){
			findMPEG2V = TRUE;
		}
		else if( secondDescBuff[i]->stream_type == 0x0F ){
			findAAC = TRUE;
		}
	}

	//pointer_field + FirstBuff + CRC
	this->createPSISize = 1+this->firstDescBuffSize+4;
	//+ SecondBuff 
	DWORD secondSize = 0;
	for( size_t i=0; i<secondDescBuff.size(); i++ ){
		switch(secondDescBuff[i]->stream_type){
			case 0x02:
				//MPEG2 VIDEO
				if( findVHighQ == TRUE ){
					if( secondDescBuff[i]->quality == 1 ){
						secondSize+=secondDescBuff[i]->descBuffSize;
					}
				}else{
					secondSize+=secondDescBuff[i]->descBuffSize;
				}
				break;
			case 0x0F:
				//MPEG2 AAC
				if( findAHighQ == TRUE ){
					if( secondDescBuff[i]->quality == 1 ){
						secondSize+=secondDescBuff[i]->descBuffSize;
					}
				}else{
					secondSize+=secondDescBuff[i]->descBuffSize;
				}
				break;
			case 0x1B:
				//MPEG4 VIDEO
				if( findMPEG2V == FALSE ){
					secondSize+=secondDescBuff[i]->descBuffSize;
				}
				break;
			case 0x04:
				//MPEG2 AUDIO
				if( findAAC == FALSE ){
					secondSize+=secondDescBuff[i]->descBuffSize;
				}
				break;
			case 0x06:
				//����
				if( this->needCaption == TRUE ){
					secondSize+=secondDescBuff[i]->descBuffSize;
				}
				break;
			case 0x0D:
				//�f�[�^�J���[�Z��
				if( this->needData == TRUE ){
					secondSize+=secondDescBuff[i]->descBuffSize;
				}
				break;
			default:
				break;
		}
	}
	this->createPSISize += secondSize;

	this->createPSI = new BYTE[this->createPSISize];
	ZeroMemory(this->createPSI, this->createPSISize);
	//�ŏ���Descriptor���[�v�܂ŃR�s�[
	memcpy( this->createPSI + 1, this->firstDescBuff, this->firstDescBuffSize );

	//SectionLength
	this->createPSI[2] = (BYTE)(((this->createPSISize-4)&0x00000F00)>>8);
	this->createPSI[2] |= 0xB0; 
	this->createPSI[3] = (BYTE)((this->createPSISize-4)&0x000000FF);
	//�o�[�W����
	this->createPSI[6] = this->createVer<<1;
	this->createPSI[6] |= 0xC1;

	DWORD writeSize = this->firstDescBuffSize+1;
	for( size_t i=0; i<secondDescBuff.size(); i++ ){
		switch(secondDescBuff[i]->stream_type){
			case 0x02:
				//MPEG2 VIDEO
				if( findVHighQ == TRUE ){
					if( secondDescBuff[i]->quality == 1 ){
						memcpy( this->createPSI + writeSize, secondDescBuff[i]->descBuff, secondDescBuff[i]->descBuffSize );
						writeSize += secondDescBuff[i]->descBuffSize;
						this->needPIDList.insert(pair<WORD,WORD>(secondDescBuff[i]->elementary_PID, secondDescBuff[i]->stream_type));
					}
				}else{
					memcpy( this->createPSI + writeSize, secondDescBuff[i]->descBuff, secondDescBuff[i]->descBuffSize );
					writeSize += secondDescBuff[i]->descBuffSize;
					this->needPIDList.insert(pair<WORD,WORD>(secondDescBuff[i]->elementary_PID, secondDescBuff[i]->stream_type));
				}
				break;
			case 0x0F:
				//MPEG2 AAC
				if( findAHighQ == TRUE ){
					if( secondDescBuff[i]->quality == 1 ){
						memcpy( this->createPSI + writeSize, secondDescBuff[i]->descBuff, secondDescBuff[i]->descBuffSize );
						writeSize += secondDescBuff[i]->descBuffSize;
						this->needPIDList.insert(pair<WORD,WORD>(secondDescBuff[i]->elementary_PID, secondDescBuff[i]->stream_type));
					}
				}else{
					memcpy( this->createPSI + writeSize, secondDescBuff[i]->descBuff, secondDescBuff[i]->descBuffSize );
					writeSize += secondDescBuff[i]->descBuffSize;
					this->needPIDList.insert(pair<WORD,WORD>(secondDescBuff[i]->elementary_PID, secondDescBuff[i]->stream_type));
				}
				break;
			case 0x1B:
				//MPEG4 VIDEO
				if( findMPEG2V == FALSE ){
					memcpy( this->createPSI + writeSize, secondDescBuff[i]->descBuff, secondDescBuff[i]->descBuffSize );
					writeSize += secondDescBuff[i]->descBuffSize;
					this->needPIDList.insert(pair<WORD,WORD>(secondDescBuff[i]->elementary_PID, secondDescBuff[i]->stream_type));
				}
				break;
			case 0x04:
				//MPEG2 AUDIO
				if( findAAC == FALSE ){
					memcpy( this->createPSI + writeSize, secondDescBuff[i]->descBuff, secondDescBuff[i]->descBuffSize );
					writeSize += secondDescBuff[i]->descBuffSize;
					this->needPIDList.insert(pair<WORD,WORD>(secondDescBuff[i]->elementary_PID, secondDescBuff[i]->stream_type));
				}
				break;
			case 0x06:
				//����
				if( this->needCaption == TRUE ){
					memcpy( this->createPSI + writeSize, secondDescBuff[i]->descBuff, secondDescBuff[i]->descBuffSize );
					writeSize += secondDescBuff[i]->descBuffSize;
					this->needPIDList.insert(pair<WORD,WORD>(secondDescBuff[i]->elementary_PID, secondDescBuff[i]->stream_type));
				}
				break;
			case 0x0D:
				//�f�[�^�J���[�Z��
				if( this->needData == TRUE ){
					memcpy( this->createPSI + writeSize, secondDescBuff[i]->descBuff, secondDescBuff[i]->descBuffSize );
					writeSize += secondDescBuff[i]->descBuffSize;
					this->needPIDList.insert(pair<WORD,WORD>(secondDescBuff[i]->elementary_PID, secondDescBuff[i]->stream_type));
				}
				break;
			default:
				break;
		}
	}

	unsigned long ulCrc = _Crc32(this->createPSISize-5, this->createPSI+1);
	this->createPSI[this->createPSISize-4] = (BYTE)((ulCrc&0xFF000000)>>24);
	this->createPSI[this->createPSISize-3] = (BYTE)((ulCrc&0x00FF0000)>>16);
	this->createPSI[this->createPSISize-2] = (BYTE)((ulCrc&0x0000FF00)>>8);
	this->createPSI[this->createPSISize-1] = (BYTE)(ulCrc&0x000000FF);

	CreatePacket();
}

void CCreatePMTPacket::CreatePacket()
{
	this->createPacketSize = 0;
	SAFE_DELETE_ARRAY(this->createPacket);

	//TS�p�P�b�g���쐬
	int packetNum = (this->createPSISize/184) + 1;
	this->createPacketSize = 188*packetNum;

	this->createPacket = new BYTE[this->createPacketSize];
	memset(this->createPacket, 0xFF, this->createPacketSize);

	for( int i = 0 ; i<packetNum; i++ ){
		this->createPacket[188*i] = 0x47;
		this->createPacket[188*i + 1] = (BYTE)((this->lastPmtPID & 0x1F00 ) >>8);
		this->createPacket[188*i + 2] = (BYTE)(this->lastPmtPID & 0x00FF);
		this->createPacket[188*i + 3] = 0x10;
		if( i==0 ){
			//payload_unit_start_indicator
			this->createPacket[188*i + 1] |= 0x40;
		}

		if( 184*(i+1) <= this->createPSISize ){
			memcpy(this->createPacket + (188*i) + 4, this->createPSI + (184*i), 184);
		}else{
			memcpy(this->createPacket + (188*i) + 4, this->createPSI + (184*i), this->createPSISize-(184*i));
		}
	}
}

//�K�v��PID�����m�F
//�߂�l�F
// TRUE�i�K�v�j�AFALSE�i�s�K�v�j
//�����F
// PID				//[IN]�m�F����PID
BOOL CCreatePMTPacket::IsNeedPID(
	WORD PID
)
{
	if( this->lastPmtPID == PID || this->lastPcrPID == PID){
		return TRUE;
	}else{
		map<WORD,WORD>::iterator itr;
		itr = this->needPIDList.find(PID);
		if( itr != this->needPIDList.end() ){
			return TRUE;
		}
		itr = this->emmPIDMap.find(PID);
		if( itr != this->emmPIDMap.end() ){
			return TRUE;
		}
	}
	return FALSE;
}

//�쐬PMT�̃o�b�t�@�|�C���^���擾
//�߂�l�F
// �쐬PMT�̃o�b�t�@�|�C���^
//�����F
// buff					[OUT]�쐬����PMT�p�P�b�g�ւ̃|�C���^�i����Ăяo�����܂ŗL���j
// size					[OUT]buff�̃T�C�Y
// incrementFlag		[IN]TS�p�P�b�g��Counter���C���N�������g���邩�ǂ����iTRUE:����AFALSE�F���Ȃ��j
BOOL CCreatePMTPacket::GetPacket(
	BYTE** buff,
	DWORD* size,
	BOOL incrementFlag
)
{
	if( incrementFlag == TRUE ){
		IncrementCounter();
	}
	if( this->createPacket != NULL ){
		*buff = this->createPacket;
		*size = (DWORD)this->createPacketSize;
	}else{
		return FALSE;
	}
	return TRUE;
}

//���������N���A
void CCreatePMTPacket::Clear()
{
	this->lastPmtPID = 0xFFFF;
	this->lastPcrPID = 0xFFFF;
	this->lastPgNumber = 0xFFFF;
	this->lastVersion = 0xFF;

	this->needPIDList.clear();
	this->emmPIDMap.clear();
	SAFE_DELETE_ARRAY(this->firstDescBuff);
	this->firstDescBuffSize = 0;
	ClearSecondBuff();

	SAFE_DELETE_ARRAY(this->createPSI);
	this->createPSISize = 0;
	SAFE_DELETE_ARRAY(this->createPacket);
	this->createPacketSize = 0;
}

void CCreatePMTPacket::ClearSecondBuff()
{
	for( int i=0; i<(int)secondDescBuff.size(); i++ ){
		SAFE_DELETE(secondDescBuff[i]);
	}
	secondDescBuff.clear();
}

void CCreatePMTPacket::IncrementCounter()
{
	if( this->createPacket == NULL ){
		return ;
	}

	for( int i = 0 ; i<this->createPacketSize; i+=188 ){
		this->createPacket[i+3] = (BYTE)(this->createCounter | 0x10);
		this->createCounter++;
		if( this->createCounter >= 16 ){
			this->createCounter = 0;
		}
	}
}

BYTE CCreatePMTPacket::GetVersion()
{
	return this->lastVersion;
}
