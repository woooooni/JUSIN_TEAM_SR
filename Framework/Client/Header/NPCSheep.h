#pragma once
#include "NPC.h"
#include "UI_ShortCutKey.h"
#include "NPCTextBox.h"
#include "NPCText.h"
#include "NPCCow.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNPCSheep : public CNPC
{
private:
	explicit CNPCSheep(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNPCSheep(const CNPCSheep& rhs);
	virtual ~CNPCSheep();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	_bool	Get_Collision() { return m_bCollision; }

private:
	_bool	m_bCollision = false;

public:
	static  CNPCSheep* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos, NPCTYPE eType);

private:
	virtual void		Free() override;
};

