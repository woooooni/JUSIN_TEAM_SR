#pragma once
#include "Item_Hat.h"
class CItem_Hat_Light : public CItem_Hat
{
	CLONE(CItem_Hat_Light)

protected:
	explicit CItem_Hat_Light(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer);
	explicit CItem_Hat_Light(const CItem_Hat& rhs);
	virtual ~CItem_Hat_Light();

public:
	HRESULT Ready_Object(void)							override;
	_int	Update_Object(const _float& fTimeDelta)		override;
	void	LateUpdate_Object(void)						override;
	void	Render_Object(void)							override;

public:
	virtual void UnHat() override;

public:

	static	CItem_Hat_Light* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer);

protected:
	HRESULT	Add_Component(void);

protected:
	virtual void Free() override;
};

