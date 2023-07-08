#pragma once
#include "Item.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END
class CCoin :
    public CItem
{
protected:
	explicit CCoin(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCoin(const CCoin& rhs);
	virtual ~CCoin();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

	static	CCoin* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	HRESULT	Add_Component(void);



protected:
	virtual void Free() override;
};

