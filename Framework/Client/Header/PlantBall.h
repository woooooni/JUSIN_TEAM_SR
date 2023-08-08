#pragma once
#include "Bullet.h"


class CPlantBall :
	public CBullet
{

	CLONE(CPlantBall)

protected:
	explicit CPlantBall(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CPlantBall(const CPlantBall& rhs);
	virtual ~CPlantBall();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

private:
	HRESULT	Add_Component(void);



public:

	void Set_Poison(_bool _bPoison) { if (m_bPoison == _bPoison) return; m_bPoison = _bPoison; }
	_bool Is_Poison() { return m_bPoison; }
	void Set_Dir(_vec3 _Dir) { m_vDir = _Dir; }
	void Set_ARGB(int _iAlpha, int _iRed, int _iGreen, int _iBlue) { m_iRed = _iRed; m_iGreen = _iGreen; m_iBlue = _iBlue; m_iAlpha = _iAlpha; }
	static CPlantBall* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	OBJ_DIR m_eDir;
	_vec3 m_vDir;
	_bool	m_bPoison = false;
	int m_iRed = 255.f;
	int m_iGreen = 255.f;
	int m_iBlue = 255.f;
	int m_iAlpha = 255.f;
	_float m_fMoveTime=20.f;
private:
	virtual void Free() override;

	void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID);

};