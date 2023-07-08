#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

// Stage별 ENUM값으로 렌더 다르게 -> 충돌(?)시 대화창 다르게 나오게
class CNPC
	: public CGameObject
{
protected:
	explicit CNPC(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNPC(const CNPC& rhs);
	virtual ~CNPC();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	static CNPC* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos);

protected:
	virtual void Free() override;
};

