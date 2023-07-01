#ifndef PickingMgr_h__
#define PickingMgr_h__

#include "GameObject.h"

BEGIN(Engine)

class CTransform;

class  ENGINE_DLL CPickingMgr final : public CBase
{
	DECLARE_SINGLETON(CPickingMgr);

public:
	explicit CPickingMgr();
	virtual ~CPickingMgr() = default;

public:
	HRESULT Ready_PickingMgr(LPDIRECT3DDEVICE9 _pDevice, HWND _hWnd);
	void	Update_PickingMgr();

private:
	void	Compute_LocalLayInfo(_vec3* pDir, _vec3* pRayPos, CTransform* pTransform);

public:
	BOOL	IsPicking(CGameObject* _pObj, _vec3* _vHit);

public:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	_vec3		m_vRayDir;
	_vec3		m_vRayPos;

	HWND		m_hWnd;

public:
	virtual void Free() override;
};

END
#endif