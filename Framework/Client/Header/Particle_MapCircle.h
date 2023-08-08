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


class CParticle_MapCircle : public CGameObject
{
	CLONE(CParticle_MapCircle)
private:
	explicit CParticle_MapCircle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticle_MapCircle(const CParticle_MapCircle& rhs);
	virtual ~CParticle_MapCircle();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT				Ready_Component(void);
public:
	void				Random_Particle(_vec3& _vMax, _uint _iCount, _uint iR = 255, _uint iG = 255, _uint iB = 255, _uint _iMaxAlpha = 255);

public:
	static CParticle_MapCircle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void		Update_Show(const _float& fTimeDelta);
	void		Update_Reset(const _float& fTimeDelta);
	void		Update_Move(const _float& fTimeDelta);
	void		BillBoard();

private:
	_float m_fAccTime;
	_float m_fEndTime;
	_float m_fStartTime;

	_float m_fAccMoveTime;
	_float m_fMoveTime;

	_int m_iMaxAlpha;
	_int m_iAlpha;
	_uint m_iR;
	_uint m_iG;
	_uint m_iB;

	_vec3 m_vMin;
	_vec3 m_vMax;
	_uint m_iCount;

	bool m_bShow = false;
	bool m_bStop = false;
private:
	virtual void Free() override;
};

