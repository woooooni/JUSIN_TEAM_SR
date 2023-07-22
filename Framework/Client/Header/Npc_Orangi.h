#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNpc_Orangi : public CNpc
{
private:
	explicit CNpc_Orangi(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_Orangi(const CNpc_Orangi& rhs);
	virtual ~CNpc_Orangi();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CNpc_Orangi* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

