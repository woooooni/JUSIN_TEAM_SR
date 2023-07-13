#pragma once
#include "GameObject.h"

class CBullet : public Engine::CGameObject
{
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID, CGameObject* _pOwner);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	virtual HRESULT	Add_Component(void);

protected:


protected:
	virtual void Free() override;

	CGameObject* m_pOwner = nullptr;
};

