#pragma once
#include "Npc.h"

class CUI_ExclamationMark;
class CUI_QuestionMark;

class CNpc_Cow : public CNpc
{
	CLONE(CNpc_Cow)

private:
	explicit CNpc_Cow(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_Cow(const CNpc_Cow& rhs);
	virtual ~CNpc_Cow();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	_bool	Get_Collision() { return m_bCollision; }

public:
	static  CNpc_Cow* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

