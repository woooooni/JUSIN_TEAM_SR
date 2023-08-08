#pragma once
#include "GameObject.h"

class CUI_ExclamationMark;
class CUI_ContinueMark;
class CUI_QuestionMark;


class CNpc
	: public CGameObject
{
	CLONE(CNpc)

protected:
	explicit CNpc(LPDIRECT3DDEVICE9 pGraphicDev, NPC_CODE _eCode, wstring _strNpcName);
	explicit CNpc(const CNpc& rhs);
	virtual ~CNpc();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	NPC_CODE Get_NpcCode() { return m_eCode; }
	_bool Is_TalkEnable() { return m_bTalkEnble; }
	void Set_TalkEnable(_bool _bEnable) { m_bTalkEnble = _bEnable; }

private:
	void Talk();

private:
	NPC_CODE m_eCode;
	_bool m_bTalkEnble;

protected:
	CUI_ExclamationMark* m_pExclamation = nullptr;
	CUI_ContinueMark* m_pContinue = nullptr;
	CUI_QuestionMark*	 m_pQuestion = nullptr;
	wstring m_strNpcName;

protected:
	virtual void Free() override;
};

