#pragma once
#include "../Header/GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;

END

class CParticle_SilkWorm : public CGameObject
{
    CLONE(CParticle_SilkWorm)

private:
	explicit CParticle_SilkWorm(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticle_SilkWorm(const CParticle_SilkWorm& rhs);
	virtual ~CParticle_SilkWorm();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void			Random_Particle(_vec3& _vPos);



	static void			Get_Effect(_vec3& _vPos, _vec3& _vDir, _uint _iCount, _float _fLookScale, _float _fSideScale);
private:
	HRESULT			Ready_Component(void);

private:
	_float m_fAccTime;
	_float m_fEffectTime;

	_int m_iAlpha;
	_uint m_iMaxAlpha;
	_uint m_iR;
	_uint m_iG;
	_uint m_iB;

	_vec3 m_vDir;

public:
	static CParticle_SilkWorm* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

