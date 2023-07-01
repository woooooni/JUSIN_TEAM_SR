#pragma once

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE _eType);
	virtual ~CScene();

public:
	CLayer*				Get_Layer(LAYER_TYPE _eLayerType);

public:
	virtual HRESULT		Ready_Scene();
	virtual _int		Update_Scene(const _float& fTimeDelta);
	virtual void		LateUpdate_Scene();
	virtual void		Render_Scene() PURE;

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<LAYER_TYPE, CLayer*>			m_mapLayer;
	SCENE_TYPE							m_eType;

public:
	virtual void	Free();
};

END