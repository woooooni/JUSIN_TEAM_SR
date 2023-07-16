#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

// Stage별 ENUM값으로 렌더 다르게 -> 충돌(?)시 대화창 다르게 나오게
class CNpc
	: public CGameObject
{
	CLONE(CNpc)

protected:
	explicit CNpc(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc(const CNpc& rhs);
	virtual ~CNpc();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static CNpc* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos);

protected:
	virtual void Free() override;
};

