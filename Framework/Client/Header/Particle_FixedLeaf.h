#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;

END


class CParticle_FixedLeaf : public CGameObject
{
	CLONE(CParticle_FixedLeaf)
private:
	explicit CParticle_FixedLeaf(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticle_FixedLeaf(const CParticle_FixedLeaf& rhs);
	virtual ~CParticle_FixedLeaf();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Ready_Component(void);
public:
	void			Random_Particle(_vec3& _vPos);

public:
	static CParticle_FixedLeaf* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	_float m_fAccTime;
	_float m_fEndTime;

	_uint m_iAlpha;
	_uint m_iR;
	_uint m_iG;
	_uint m_iB;
private:
	virtual void Free() override;
};

