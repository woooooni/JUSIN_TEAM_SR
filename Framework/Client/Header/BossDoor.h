#pragma once
#include "FixedObj.h"


BEGIN(Engine)


class CRcTex;
class CTransform;
class CCollider;
class CTexture;
class CCubeTex;
class CAnimator;


END



class CBossDoor : public CFixedObj
{
	CLONE(CBossDoor)

protected:
	explicit CBossDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CBossDoor(const CBossDoor& rhs);
	virtual ~CBossDoor();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;


public:
	// 충돌 호출
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;




	void		Open_Door();
	void		Set_Door(_vec3& _vPos, _vec3& _vScale, _vec3& _vDir);

	bool		Get_Finish() { return m_bFinish; }
private:
	HRESULT Ready_Component();



public:
	static CBossDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual void Free() override;


private:
	_vec3 m_vDir;
	_vec3 m_vStartPos;
	_vec3 m_vOriginPos;

	_float m_fOpenDelay;

	bool m_bOpenDelay = false;
	bool m_bOpen = false;
	bool m_bFinish = false;

	_float m_fAccStoneTime;
	_float m_fStoneTime;

	_float m_fAccLargeTime;
	_float m_fLargeTime;

	_float m_fAccSmokeTime;
	_float m_fSmokeTime;

	bool m_bPortal = false;
	_float m_fAngle;

	_vec3 m_vScale;
};

