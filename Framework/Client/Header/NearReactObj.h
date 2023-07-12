#pragma once

#include "Engine_Define.h"
#include "GameObject.h"

BEGIN(Engine)
class CRcTex;
class CBoxCollider;
END

class CNearReactObj : public CGameObject
{
	CLONE(CNearReactObj)

protected:
	CNearReactObj(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID);
	CNearReactObj(const CNearReactObj& rhs);

	virtual ~CNearReactObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static	CNearReactObj* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 p_pos = { 0, 0, 0 });

protected:
	virtual HRESULT			Ready_Component(void);

	float			m_fFindRange = 3.f;
	float			m_fCurFrame = 0.f;

};

