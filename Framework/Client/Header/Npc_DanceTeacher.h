#pragma once
#include "Npc.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNpc_DanceTeacher : public CNpc
{
private:
	explicit CNpc_DanceTeacher(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_DanceTeacher(const CNpc_DanceTeacher& rhs);
	virtual ~CNpc_DanceTeacher();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static  CNpc_DanceTeacher* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

