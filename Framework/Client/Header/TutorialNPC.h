#pragma once
#include "NPC.h"

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
	NPCTYPE		eType;
	NPCSTATE	eState;
};

class CTutorialNPC :
    public CNPC
{
private:
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
	HRESULT		Add_Component(void);
	void		Set_Type(NPCTYPE eType);

private:
	tagNPCInfo			m_tInfo;

public:
	static  CTutorialNPC* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos, NPCTYPE eType);

private:
	virtual void		Free() override;
};

