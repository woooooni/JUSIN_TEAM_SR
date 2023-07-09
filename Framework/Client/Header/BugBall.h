#pragma once
#include "GameObject.h"



class CBugBall :
	public CGameObject
{

	CLONE(CBugBall)

protected:
	explicit CBugBall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBugBall(const CBugBall& rhs);
	virtual ~CBugBall();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);



public:

	void Set_Poison(_bool _bPoison) { if (m_bPoison == _bPoison) return; m_bPoison = _bPoison; }
	_bool Is_Poison() { return m_bPoison; }
	void Set_Dir(_vec3 _Dir) { m_vDir = _Dir; }
	static CBugBall* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	OBJ_DIR m_eDir;
	_vec3 m_vDir;
	_bool	m_bPoison = false;

protected:
	_float m_fMoveTime=20.f;
protected:
	virtual void Free() override;

};