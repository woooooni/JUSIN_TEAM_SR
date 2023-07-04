#pragma once
#include "Engine_Define.h"
#include "GameObject.h"


class CGrabbableObj : public CGameObject
{
	CGrabbableObj(LPDIRECT3DDEVICE9 pGraphicDev);
	CGrabbableObj(const CGrabbableObj& rhs);
	virtual ~CGrabbableObj();


public:
	virtual			void		Grap();
	virtual			void		UnGrap();
};

