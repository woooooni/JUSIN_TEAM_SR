#pragma once
#include "Npc.h"

class CUI_ExclamationMark;
class CUI_QuestionMark;

class CNpc_Sheep : public CNpc
{
	CLONE(CNpc_Sheep)

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

public:
	static  CNpc_Sheep* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

