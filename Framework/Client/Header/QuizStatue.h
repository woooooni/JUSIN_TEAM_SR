#pragma once
#include "FieldObject.h"
class CQuizStatue :
    public CFieldObject
{
protected:
	CQuizStatue(LPDIRECT3DDEVICE9 p_Dev);
	CQuizStatue(const CQuizStatue& rhs);
	virtual ~CQuizStatue();

public:
	enum class MONKEY_STATUE
	{
		EAR, MOUTH, EYE, STATUE_END
	};

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CQuizStatue* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const MONKEY_STATUE& Answer ,const _vec3 p_Pos = { 0, 0, 0 }, MONKEY_STATUE p_Monkey = MONKEY_STATUE::EAR);

	CLONE(CQuizStatue);

public:
	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;


	virtual void Event_Start(_uint iEventNum) override;

protected:

	virtual HRESULT	Ready_Component()override;
	_uint	m_iEventNum;
	MONKEY_STATUE		m_eMonkey;
	MONKEY_STATUE		m_eAnswer;

	_bool				m_bIsCorrect;
	_bool				m_bIsLocked;


};

