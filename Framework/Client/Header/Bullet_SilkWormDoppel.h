#pragma once
#include "GameObject.h"
#include "Bullet.h"


class CBullet_SilkWormDoppel :
	public CBullet
{

	CLONE(CBullet_SilkWormDoppel)

protected:
	explicit CBullet_SilkWormDoppel(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBullet_SilkWormDoppel(const CBullet_SilkWormDoppel& rhs);
	virtual ~CBullet_SilkWormDoppel();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);
	void Create_Line();
public:
	static CBullet_SilkWormDoppel* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos,_vec3 vRandPos ,_float _fTime, COLOR_STATE _eColor, _float _iAtk);
	static CBullet_SilkWormDoppel* Create(LPDIRECT3DDEVICE9 pGraphicDev);
protected:
	_vec3 m_vDir;
	_vec3 m_vRandPos;
	_float m_fActivateTime;
	_float m_fMoveTime =10.f;
	_float m_fSpeed ;
	COLOR_STATE m_eColor;
	_bool m_bShoot =false;
	_bool m_bReady =false;
	CGameObject* m_pLine = nullptr;
protected:

	virtual void Free() override;

};