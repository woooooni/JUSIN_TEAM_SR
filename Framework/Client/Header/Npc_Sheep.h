#pragma once
#include "Npc.h"
#include "UI_ShortCutKey.h"
#include "NPCText.h"


BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNpc_Sheep : public CNpc
{
private:
	explicit CNpc_Sheep(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_Sheep(const CNpc_Sheep& rhs);
	virtual ~CNpc_Sheep();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT Ready_Component();

public:
	_bool	Get_Collision() { return m_bCollision; }

private:
	_bool	m_bCollision = false;

public:
	static  CNpc_Sheep* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

