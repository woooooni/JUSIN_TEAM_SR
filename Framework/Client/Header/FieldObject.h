#pragma once
#include "Engine_Define.h"
#include "GameObject.h"
class CFieldObject : public CGameObject
{
public:
	virtual void	OnAttacked() {};
	

protected:
	bool m_bIsPushable = false;
	bool m_bIsGrabbable = false;
	bool m_bIsAttackable = false;
};

