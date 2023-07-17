#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CBoxCollider : public CCollider
{
	
private:
	explicit CBoxCollider();
	explicit CBoxCollider(LPDIRECT3DDEVICE9 _pDevice);
	explicit CBoxCollider(const CBoxCollider& rhs);
	virtual ~CBoxCollider();

public:
	HRESULT			Ready_BoxCollider();
	virtual	_int	Update_Component(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Component() override;
	virtual void	Render_Component()	override;

public:
	// CCollider을(를) 통해 상속됨
	virtual void	OnCollisionEnter(CCollider * _pOther, COLLISION_GROUP _eGroup) override;
	virtual void	OnCollisionStay(CCollider * _pOther, COLLISION_GROUP _eGroup) override;
	virtual void	OnCollisionExit(CCollider * _pOther, COLLISION_GROUP _eGroup) override;

public:
	static CBoxCollider*	Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void			Free();
	virtual CComponent *	Clone(void) override;

	void	Set_Scale(const _vec3& p_Scale) { m_vScale = p_Scale; }
	const	_vec3& Get_Pos() { return m_vCenterPos; }
	const	_vec3& Get_Scale() { return m_vScale; }

private:
	void InputCollider();


private:
	_vec3		m_vScale = {1, 1, 1};


};

END