#pragma once
#include "Item_Hat.h"
class CItem_Hat_Mask : public CItem_Hat
{
	CLONE(CItem_Hat_Mask)

protected:
	explicit CItem_Hat_Mask(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer);
	explicit CItem_Hat_Mask(const CItem_Hat& rhs);
	virtual ~CItem_Hat_Mask();

public:
	HRESULT Ready_Object(void)							override;
	_int	Update_Object(const _float& fTimeDelta)		override;
	void	LateUpdate_Object(void)						override;
	void	Render_Object(void)							override;


	static	CItem_Hat_Mask* Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer);

protected:
	HRESULT	Add_Component(void);

protected:
	virtual void Free() override;
};

