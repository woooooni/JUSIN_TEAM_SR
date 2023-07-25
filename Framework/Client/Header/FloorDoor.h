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



class CFloorDoor : public CFixedObj
{
	CLONE(CFloorDoor)

protected:
	explicit CFloorDoor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CFloorDoor(const CFloorDoor& rhs);
	virtual ~CFloorDoor();

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
	static CFloorDoor* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual void Free() override;


private:
	_vec3 m_vDir;
	_vec3 m_vStartPos;

	_float m_fOpenDelay;

	bool m_bOpenDelay = false;
	bool m_bOpen = false;
	bool m_bFinish = false;

	_float m_fAccStoneTime;
	_float m_fStoneTime;

	_float m_fAccLargeTime;
	_float m_fLargeTime;

	bool m_bPortal = false;
};

