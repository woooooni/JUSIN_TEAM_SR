#include "SoundMgr.h"
#include "Engine_Define.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
	: CBase()
	// , m_pSystem(nullptr)
{

}

HRESULT CSoundMgr::Ready_SoundMgr()
{

	return S_OK;
}

void CSoundMgr::Play_Sound(TCHAR* pSoundKey, const _uint& eID, const _float& fVolume)
{
}

void CSoundMgr::Play_BGM(TCHAR* pSoundKey, const _uint& eID, const _float& fVolume)
{
}

void CSoundMgr::Stop_Sound(const _uint& eID)
{
}

void CSoundMgr::Stop_All()
{
}

void CSoundMgr::Set_ChannelVolume(const _uint& eID, const _float& fVolume)
{
}

int CSoundMgr::Volume_Up(const _uint& eID, const _float& fVolume)
{
	return 0;
}

int CSoundMgr::Volume_Down(const _uint& eID, const _float& fVolume)
{
	return 0;
}

int CSoundMgr::Pause(const _uint& eID)
{
	return 0;
}

void CSoundMgr::LoadSoundFile()
{
}

void CSoundMgr::Free()
{
	
}
