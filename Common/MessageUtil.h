#ifndef __MESSAGE_UTIL_H__
#define __MESSAGE_UTIL_H__

//�L�[������String���擾����
const wchar_t* GetMessageString(const wchar_t* resourceKey);

//�^��I���X�e�[�^�X�̃��b�Z�[�W���擾
const wchar_t* GetRecStatusString(DWORD recStatus, SYSTEMTIME startRecEpg);

#endif
