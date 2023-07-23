#pragma once
#include "GameObject.h"

class CUI_ExclamationMark;
class CUI_QuestionMark;

class CNpc
	: public CGameObject
{
	CLONE(CNpc)

protected:
	explicit CNpc(LPDIRECT3DDEVICE9 pGraphicDev, NPC_CODE _eCode);
	explicit CNpc(const CNpc& rhs);
	virtual ~CNpc();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	NPC_CODE Get_NpcCode() { return m_eCode; }

private:
	NPC_CODE m_eCode;

protected:
	CUI_ExclamationMark* m_pExclamation = nullptr;
	CUI_QuestionMark*	 m_pQuestion = nullptr;

protected:
	virtual void Free() override;
};

