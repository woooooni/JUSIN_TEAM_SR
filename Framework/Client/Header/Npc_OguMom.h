#pragma once
#include "NPC.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNpc_OguMom : public CNpc
{
private:
	explicit CNpc_OguMom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_OguMom(const CNpc_OguMom& rhs);
	virtual ~CNpc_OguMom();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CNpc_OguMom* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

