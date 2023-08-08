#pragma once
#include "Npc.h"
//#include <Npc_VillagerMonkey.h>

typedef enum class MoonVillagerType
{
	MOON_RED, MOON_YELLOW,
	VILLAGER_END

}MOONVILLAGER;

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;

END

class CNpc_VillagerMoon : public CNpc
{
private:
	explicit CNpc_VillagerMoon(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc_VillagerMoon(const CNpc_VillagerMoon& rhs);
	virtual ~CNpc_VillagerMoon();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	void	Set_Type(MOONVILLAGER eType);

public:
	MOONVILLAGER	m_eType;

public:
	static  CNpc_VillagerMoon* Create(LPDIRECT3DDEVICE9 pGraphicDev, MOONVILLAGER eType);

private:
	virtual void		Free() override;
};

