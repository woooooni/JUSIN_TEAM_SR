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
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem;

public:
	virtual void Free() override;

};

END