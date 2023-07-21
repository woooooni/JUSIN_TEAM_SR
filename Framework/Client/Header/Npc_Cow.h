#pragma once
#include "Npc.h"
#include "UI_ShortCutKey.h"
#include "NPCText.h"
#include "UI_QuestionMark.h"
#include "UI_ExclamationMark.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNpc_Cow : public CNpc
{
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

private:
	_bool	m_bCollision = false;
	_bool	m_bQuestAccept = true;  // 퀘스트를 받을 수 있는가?
	CUI_ExclamationMark* m_pExclamation = nullptr;
	CUI_QuestionMark* m_pQuestion = nullptr;

public:
	static  CNpc_Cow* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

