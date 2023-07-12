#pragma once
#include "GameObject.h"

BEGIN(Engine)

	class CRcTex;
class CTransform;
class CCollider;

END

class CEffect :
	public CGameObject
{

protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;

};