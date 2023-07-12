#pragma once
#include "GameObject.h"



BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CItem :
	public CGameObject
{

protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev, ITEM_TYPE _eItemType);
	explicit CItem(const CItem& rhs);
	virtual ~CItem();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	virtual HRESULT	Add_Component(void);

protected:
	ITEM_TYPE m_eItemType;


protected:
	virtual void Free() override;

};