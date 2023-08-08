#pragma once
#include "Npc.h"

typedef enum class VillagerType
{
	MONKEY_HOOD,
	MONKEY_FUR,
	MONKEY_LEAVES1,
	MONKEY_LEAVES2,
	MONKEY_DANCER,

	VILLAGER_END

}VILLAGERTYPE;

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNpc_VillagerMonkey : public CNpc
{
private:
	explicit CNpc_VillagerMonkey(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_VillagerMonkey(const CNpc_VillagerMonkey& rhs);
	virtual ~CNpc_VillagerMonkey();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Set_Type(VILLAGERTYPE eType);

public:
	VILLAGERTYPE	m_eType;

public:
	static  CNpc_VillagerMonkey* Create(LPDIRECT3DDEVICE9 pGraphicDev, VILLAGERTYPE eType);

private:
	virtual void		Free() override;
};

