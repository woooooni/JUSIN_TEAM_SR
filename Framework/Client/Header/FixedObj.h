#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CFixedObj : public Engine::CGameObject
{

protected:
	explicit CFixedObj(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID);
	explicit CFixedObj(const CFixedObj& rhs);
	virtual ~CFixedObj();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	virtual void Free() override;
};

