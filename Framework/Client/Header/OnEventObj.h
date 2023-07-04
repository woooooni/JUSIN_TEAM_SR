#pragma once
#include "Engine_Define.h"
#include "GameObject.h"

class COnEventObj : public CGameObject
{

public:
	virtual void OnEvent()	PURE;

};

