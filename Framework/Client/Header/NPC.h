#pragma once
#include "GameObject.h"

class CUI_ExclamationMark;
class CUI_QuestionMark;

class CNpc
	: public CGameObject
{
	CLONE(CNpc)

protected:
	explicit CNpc(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc(const CNpc& rhs);
	virtual ~CNpc();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

protected:
	_bool				 m_bCollision = false;
	_bool				 m_bQuestAccept = false;
	CUI_ExclamationMark* m_pExclamation = nullptr;
	CUI_QuestionMark*	 m_pQuestion = nullptr;

protected:
	virtual void Free() override;
};

