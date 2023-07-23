#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CTimer :	public CBase
{
private:
	CTimer();
	virtual ~CTimer();

public:
	_float			Get_TimeDelta() { return m_fTimeDelta; }
public:
	HRESULT			Ready_Timer();
	void			Update_Timer();

public:
	// 0 ~ 1까지 타임스케일을 설정.
	void Set_TimeScale(_float fTimeScale) { m_fTimeScale = fTimeScale; }
	
private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;

	_float					m_fTimeDelta;
	_float					m_fTimeScale;

public:
	static CTimer*		Create(void);
private:
	virtual void Free();
};

END