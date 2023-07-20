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
	virtual HRESULT			Ready_Layer_Player() PURE;
	virtual HRESULT			Ready_Layer_Camera() PURE;
	virtual HRESULT			Ready_Layer_Terrrain() PURE;
	virtual HRESULT			Ready_Layer_Environment() PURE;
	virtual HRESULT			Ready_Layer_Monster() PURE;
	virtual HRESULT			Ready_Layer_InterationObj() PURE;
	virtual HRESULT			Ready_Layer_Effect() PURE;
	virtual HRESULT			Ready_Layer_UI() PURE;

protected:
	void					PlayVideo(HWND _hWnd, const wstring& _strFilePath);

protected:
	HRESULT Ready_AllLayer()
	{
		for (_uint i = 0; i < (_uint)LAYER_TYPE::LAYER_END; ++i)
		{
			Engine::CLayer* pLayer = Engine::CLayer::Create();
			NULL_CHECK_RETURN(pLayer, E_FAIL);
			m_mapLayer.insert({ (LAYER_TYPE)i, pLayer });
		}

		return S_OK;
	}

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	map<LAYER_TYPE, CLayer*>			m_mapLayer;
	SCENE_TYPE							m_eType;
	HWND								m_hVideoHandle;
public:

	const SCENE_TYPE& Get_SceneType() { return m_eType; }

	virtual void	Free();
};

END