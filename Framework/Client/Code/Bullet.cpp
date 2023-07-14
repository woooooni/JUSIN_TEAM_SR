
#include "..\Header\Bullet.h"
#include "Export_Function.h"
#include "TriCol.h"
#include "Transform.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID, CGameObject* _pOwner)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_BULLET, _eID)
{
}

CBullet::CBullet(const CBullet& rhs)
	: CGameObject(rhs),
	m_pOwner(rhs.m_pOwner)
{
}

CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_Object(void)
{
	return S_OK;
}

_int CBullet::Update_Object(const _float& fTimeDelta)
{
	return __super::Update_Object(fTimeDelta);
}

void CBullet::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBullet::Render_Object(void)
{
	__super::Render_Object();
}


void CBullet::Free()
{
	__super::Free();
}
