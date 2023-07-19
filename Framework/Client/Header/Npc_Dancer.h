#pragma once
#include "NPC.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNpc_Dancer : public CNpc
{
private:
	explicit CNpc_Dancer(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_Dancer(const CNpc_Dancer& rhs);
	virtual ~CNpc_Dancer();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CNpc_Dancer* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

