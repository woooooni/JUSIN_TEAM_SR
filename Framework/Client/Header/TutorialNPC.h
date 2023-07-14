#pragma once
#include "NPC.h"
#include "UI_ShortCutKey.h"
#include "NPCTextBox.h"
#include "Text.h"

// NPC 대화창 : PNG파일 / NPC 대화 : TEXT (UI)
// 대화 끝나면 NPC STATE -> REACT로

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

typedef enum class NPCType
{
	//TUT_OGUMOM,
	TUT_COW, TUT_SHEEP, TUT_PIG, TUT_DOOGEE,

	NPCTYPE_END

}NPCTYPE;

typedef enum class NPCState
{
	NPC_IDLE, NPC_REACT,
	NPCSTATE_END

}NPCSTATE;

struct tagNPCInfo
{
	NPCTYPE		eType = NPCTYPE::NPCTYPE_END;
	NPCSTATE	eState = NPCSTATE::NPC_IDLE;
	bool		bCollision = false;
};

class CTutorialNPC :
    public CNPC
{
	CLONE(CTutorialNPC)

protected:
	explicit CTutorialNPC(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTutorialNPC(const CTutorialNPC& rhs);
	virtual ~CTutorialNPC();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	void			Set_Type(NPCTYPE eType);
	tagNPCInfo		Get_Type() { return m_tInfo; }
	void			Set_State(NPCSTATE eState);

private:
	CUI_ShortCutKey*	m_pKeyInfo = nullptr;
	tagNPCInfo			m_tInfo;
	vector<tagNPCInfo>	m_vecNPC;

public:
	static  CTutorialNPC* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos, NPCTYPE eType);

private:
	virtual void		Free() override;
};

