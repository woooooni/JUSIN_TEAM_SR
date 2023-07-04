#pragma once

#include	"Engine_Define.h"
#include	"GameObject.h"

class CAttackableObj : public CGameObject
{

protected:
	virtual void OnAttacked()		PURE;
};

