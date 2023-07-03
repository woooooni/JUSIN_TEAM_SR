#pragma once

#include	"Engine_Define.h"
#include	"GameObject.h"

class CBreakableObj : public CGameObject
{

protected:
	int iHP = 0;
	virtual		HRESULT			Get_Damage(const _int& p_Dmg)	PURE;
	virtual		void			On_Destroy()					PURE;
};

