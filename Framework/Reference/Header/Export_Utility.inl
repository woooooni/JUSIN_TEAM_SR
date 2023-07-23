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
inline void Reserve_SceneChange(CScene* pScene)
{
	CManagement::GetInstance()->Reserve_SceneChange(pScene);
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

// PickingMgr

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

// LightMgr
HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	return	CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLightInfo, iIndex);
}

// KeyMgr
inline HRESULT Ready_KeyMgr(LPDIRECT3DDEVICE9 _pDevice, HWND _hWnd)
{
	return CKeyMgr::GetInstance()->Ready_KeyMgr(_pDevice, _hWnd);
}

inline void Update_KeyMgr()
{
	CKeyMgr::GetInstance()->Update_KeyMgr();
}


// CollisionMgr
inline void Add_CollisionGroup(CCollider* pCol, COLLISION_GROUP pState)
{
	CCollisionMgr::GetInstance()->Add_CollisionGroup(pCol, pState);
}

inline HRESULT Ready_CollisionMgr(LPDIRECT3DDEVICE9 _pDevice)
{
	return CCollisionMgr::GetInstance()->Ready_CollisionMgr(_pDevice);
}

inline void Update_Collision()
{
	CCollisionMgr::GetInstance()->Update_Collision();
}

inline void CheckGroupType(COLLISION_GROUP _eLeft, COLLISION_GROUP _eRight)
{
	CCollisionMgr::GetInstance()->CheckGroupType(_eLeft, _eRight);
}

inline void Reset()
{
	CCollisionMgr::GetInstance()->Reset();
}

inline HRESULT Add_Event(EVENT* pEvent)
{
	return CInteractionMgr::GetInstance()->Add_Event(pEvent);
}

inline void Set_Event()
{
	CInteractionMgr::GetInstance()->Set_Event();
}

inline HRESULT Add_Subscribe(_uint pEventKey, CGameObject* pSubscriber)
{
	return CInteractionMgr::GetInstance()->Add_Subscribe(pEventKey, pSubscriber);
}

inline HRESULT Check_Event_Start(const _uint& pCheckNum)
{
	return CInteractionMgr::GetInstance()->Check_Event_Start(pCheckNum);
}

inline void Add_Reset(const _uint& resetIndex, const _uint& eventKey)
{
	CInteractionMgr::GetInstance()->Add_Reset(resetIndex, eventKey);
}

inline void Reset(const _uint& resetIndex)
{
	CInteractionMgr::GetInstance()->Reset(resetIndex);

}

inline void Add_Reset(const _uint& resetIndex, const _uint& minIndex, const _uint& maxIndex)
{
	CInteractionMgr::GetInstance()->Add_Reset(resetIndex, minIndex, maxIndex);
}

// EventMgr

inline HRESULT Ready_EventMgr()
{
	return CEventMgr::GetInstance()->Ready_EventMgr();
}

inline void Update_EventMgr(const _float& fTimeDelta)
{
	CEventMgr::GetInstance()->Update_EventMgr(fTimeDelta);
}

inline void DeleteObjEvt(CGameObject* pObj)
{
	CEventMgr::GetInstance()->DeleteObjEvt(pObj);
}

inline vector<CGameObject*>& Get_DelteObj_Vec(OBJ_TYPE _eObjType)
{
	return CEventMgr::GetInstance()->Get_DelteObj_Vec(_eObjType);
}

inline HRESULT Ready_SoundMgr()
{
	return CSoundMgr::GetInstance()->Ready_SoundMgr();
}

inline void Play_Sound(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	CSoundMgr::GetInstance()->Play_Sound(pSoundKey, eID, fVolume);
}

inline void Play_BGM(TCHAR* pSoundKey, float fVolume)
{
	CSoundMgr::GetInstance()->Play_BGM(pSoundKey, fVolume);
}

inline void Stop_Sound(CHANNELID eID)
{
	CSoundMgr::GetInstance()->Stop_Sound(eID);
}

inline void Stop_All()
{
	CSoundMgr::GetInstance()->Stop_All();
}

inline void Set_ChannelVolume(CHANNELID eID, float fVolume)
{
	CSoundMgr::GetInstance()->Set_ChannelVolume(eID, fVolume);
}

void			Release_Utility()
{
	CKeyMgr::GetInstance()->DestroyInstance();
	CPickingMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CCameraMgr::GetInstance()->DestroyInstance();
	CProtoMgr::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
	CInteractionMgr::GetInstance()->DestroyInstance();
	CEventMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
}

