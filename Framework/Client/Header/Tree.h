#pragma once
#include "FixedObj.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CTree : public CFixedObj
{
	CLONE(CTree)

protected:
	explicit CTree(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTree(const CTree& rhs);
	virtual ~CTree();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

private:
	HRESULT Ready_Component();

public:
	static CTree* Create(LPDIRECT3DDEVICE9 pGraphicDev);



protected:
	virtual void Free() override;
};

