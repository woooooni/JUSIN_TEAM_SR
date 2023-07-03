#include "Export_Utility.h"

HRESULT		Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppInstance)
{
	CManagement*		pManagement = CManagement::GetInstance();
	NULL_CHECK_RETURN(pManagement, E_FAIL);

	*ppInstance = pManagement;

	return S_OK;
}

HRESULT		Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}
_int			Update_Scene(const _float& fTimeDelta)
{
	return CManagement::GetInstance()->Update_Scene(fTimeDelta);
}
void			LateUpdate_Scene()
{
	CManagement::GetInstance()->LateUpdate_Scene();
}
void			Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CManagement::GetInstance()->Render_Scene(pGraphicDev);
}

inline CScene * GetCurrScene()
{
	return CManagement::GetInstance()->GetCurrScene();
}

inline CLayer * Get_Layer(LAYER_TYPE eLayerType)
{
	return CManagement::GetInstance()->GetCurrScene()->Get_Layer(eLayerType);
}

HRESULT			Ready_Proto(const _tchar* pProtoTag, CComponent* pComponent)
{
	return CProtoMgr::GetInstance()->Ready_Proto(pProtoTag, pComponent);
}
CComponent*		Clone_Proto(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Proto(pProtoTag);
}

inline CCamera * CreateCamera(HWND _hWnd, LPDIRECT3DDEVICE9 _pDevice, _float _fNear, _float _fFar)
{
	return CCameraMgr::GetInstance()->CreateCamera(_hWnd, _pDevice, _fNear, _fFar);
}

void		Add_RenderGroup(RENDERID eType, CGameObject* pGameObject)
{
	CRenderer::GetInstance()->Add_RenderGroup(eType, pGameObject);
}
void		Render_GameObject(LPDIRECT3DDEVICE9& pGraphicDev)
{
	CRenderer::GetInstance()->Render_GameObject(pGraphicDev);
}
void		Clear_RenderGroup()
{
	CRenderer::GetInstance()->Clear_RenderGroup();
}

inline HRESULT Ready_PickingMgr(LPDIRECT3DDEVICE9 _pDevice, HWND _hWnd)
{
	return CPickingMgr::GetInstance()->Ready_PickingMgr(_pDevice, _hWnd);
}

inline void Update_PickingMgr()
{
	CPickingMgr::GetInstance()->Update_PickingMgr();
}

inline BOOL IsPicking(CGameObject * _pObj, _vec3 * _vHit)
{
	return CPickingMgr::GetInstance()->IsPicking(_pObj, _vHit);
}

HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	return	CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

inline HRESULT Ready_KeyMgr(LPDIRECT3DDEVICE9 _pDevice, HWND _hWnd)
{
	return CKeyMgr::GetInstance()->Ready_KeyMgr(_pDevice, _hWnd);
}

inline void Update_KeyMgr()
{
	CKeyMgr::GetInstance()->Update_KeyMgr();
}

void			Release_Utility()
{
	CKeyMgr::GetInstance()->DestroyInstance();
	CPickingMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CCameraMgr::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}