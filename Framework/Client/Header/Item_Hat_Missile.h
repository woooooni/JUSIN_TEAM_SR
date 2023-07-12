#pragma once
#include "Item_Hat.h"
class CItem_Hat_Missile : public CItem_Hat
{
	CLONE(CItem_Hat_Missile)

protected:
	explicit CItem_Hat_Missile(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer);
	explicit CItem_Hat_Missile(const CItem_Hat& rhs);
	virtual ~CItem_Hat_Missile();

public:
	HRESULT Ready_Object(void)							override;
	_int	Update_Object(const _float& fTimeDelta)		override;
	void	LateUpdate_Object(void)						override;
	void	Render_Object(void)							override;


	static	CItem_Hat_Missile* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer);

protected:
	HRESULT	Add_Component(void);

protected:
	virtual void Free() override;
};

