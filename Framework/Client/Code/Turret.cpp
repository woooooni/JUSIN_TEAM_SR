#include "Turret.h"
#include	"Export_Function.h"
CTurret::CTurret(LPDIRECT3DDEVICE9 pGraphicDev) : CFieldObject(pGraphicDev)
{}


CTurret::CTurret(const CTurret& rhs) : CFieldObject(rhs)
{
}

CTurret::~CTurret()
{
}

HRESULT CTurret::Ready_Object(void) 
{
	return E_NOTIMPL;
}

_int CTurret::Update_Object(const _float& fTimeDelta)
{
	return _int();
}

void CTurret::LateUpdate_Object(void)
{
}

void CTurret::Render_Object(void)
{
}

void CTurret::Add_Subscribe(_uint iNum)
{
}

void CTurret::Free()
{
}

CTurret* CTurret::Create(LPDIRECT3DDEVICE9 p_Dev,  const _uint& p_EventNum, const _vec3 p_Pos)
{
	return nullptr;
}

void CTurret::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CTurret::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CTurret::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CTurret::Event_Start(_uint iEventNum)
{
}

void CTurret::Event_End(_uint iEventNum)
{
}
