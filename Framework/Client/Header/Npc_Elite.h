#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNpc_Elite : public CNpc
{
private:
	explicit CNpc_Elite(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_Elite(const CNpc_Elite& rhs);
	virtual ~CNpc_Elite();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CNpc_Elite* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

