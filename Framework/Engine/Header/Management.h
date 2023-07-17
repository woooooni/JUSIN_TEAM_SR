#pragma once

#include "Engine_Define.h"
#include "Base.h"
#include "Scene.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	HRESULT			Set_Scene(CScene* pScene);
	void			Reserve_SceneChange(CScene* pScene) { if (m_pReserveScene) return; m_pReserveScene = pScene; }

	_int			Update_Scene(const _float& fTimeDelta);
	void			LateUpdate_Scene();
	void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	CScene*			GetCurrScene() { return m_pCurrScene; }

private:
	CScene*			m_pCurrScene;
	CScene*			m_pReserveScene;


public:
	virtual void		Free();
};

END