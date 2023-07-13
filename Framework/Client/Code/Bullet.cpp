
#include "..\Header\Bullet.h"
#include "Export_Function.h"
#include "TriCol.h"
#include "Transform.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID, CGameObject* _pOwner)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_BULLET, _eID)
{
}

CBullet::CBullet(const CBullet& rhs)
	: CGameObject(rhs)
{
}

CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_Object(void)
{
	return E_NOTIMPL;
}

_int CBullet::Update_Object(const _float& fTimeDelta)
{
	return _int();
}

void CBullet::LateUpdate_Object(void)
{
}

void CBullet::Render_Object(void)
{
}

HRESULT CBullet::Add_Component(void)
{
	return E_NOTIMPL;
}

void CBullet::Free()
{
}
