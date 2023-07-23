#pragma once
#include "Base.h"
#include "Engine_Define.h"
#include "fmod.h"
#include "fmod.hpp"
BEGIN(Engine)

class ENGINE_DLL CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)	

private:
	explicit CSoundMgr();
	virtual  ~CSoundMgr();

public:
	HRESULT Ready_SoundMgr();

public:
	void Play_Sound(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void Play_BGM(TCHAR* pSoundKey, float fVolume);
	void Stop_Sound(CHANNELID eID);
	void Stop_All();
	void Set_ChannelVolume(CHANNELID eID, float fVolume);

private:
	void Load_SoundFile();

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem;

public:
	virtual void Free() override;

};

END