#pragma once


#include "GameObject.h"

BEGIN(Engine)

class CTriCol;
class CTransform;
class CCollider;

END

class CBullet :
	public Engine::CGameObject
{
private:
	explicit CBullet(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;


public:
	void			SetDir(const _vec3& _vDir)	{ m_vDir = _vDir; }
	const _vec3&	GetDir()					{ return m_vDir; }


private:
	HRESULT		Add_Component(void);

private:
	CTriCol*			m_pBufferCom = nullptr;
	CTransform*			m_pTransformCom = nullptr;
	CCollider*			m_pColliderCom = nullptr;
	_float				m_fSpeed = 5.f;

	_vec3				m_vDir;
public:
	static CBullet*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

