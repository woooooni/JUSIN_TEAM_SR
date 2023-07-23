#pragma once
#include "Base.h"
#include "Engine_Define.h"
#define SOUND_MAX 1.0f
#define SOUND_MIN 0.0f
#define SOUND_DEFAULT 0.5f
#define SOUND_WEIGHT 0.1f

BEGIN(Engine)

class ENGINE_DLL CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)
private:
	explicit CSoundMgr();
	virtual ~CSoundMgr() = default;

public:
	HRESULT Ready_SoundMgr();
	
public:
	void Play_Sound(TCHAR* pSoundKey, const _uint& eID, const _float& fVolume);
	void Play_BGM(TCHAR* pSoundKey, const _uint& eID, const _float& fVolume);

	void Stop_Sound(const _uint& eID);
	void Stop_All();

	void Set_ChannelVolume(const _uint& eID, const _float& fVolume);

	int Volume_Up(const _uint& eID, const _float& fVolume);
	int Volume_Down(const _uint& eID, const _float& fVolume);

	int Pause(const _uint& eID);

private:
	void LoadSoundFile();

private:
	_float m_fVolume = SOUND_DEFAULT;
	_float m_fBGMVolume = SOUND_DEFAULT;
	// FMOD_BOOL m_bFmod;

private:
	enum { MAX_CHANNEL = 32 };
	/*std::map<TCHAR*, FMOD::Sound*> m_mapSound;

	FMOD::System* m_pSystem;
	_bool m_bPause = false;*/

public:
	virtual void Free() override;

};

END