#include "JellyBombCreator.h"
#include	"Export_Function.h"

CJellyBombCreator::CJellyBombCreator(LPDIRECT3DDEVICE9 pDev) : CFieldObject(pDev), m_pJellyBomb(nullptr)
{

}


CJellyBombCreator::CJellyBombCreator(const CJellyBombCreator& rhs) : CFieldObject(rhs), m_pJellyBomb(rhs.m_pJellyBomb)
{
}

CJellyBombCreator::~CJellyBombCreator()
{
}

HRESULT CJellyBombCreator::Ready_Object(void)
{
	return E_NOTIMPL;
}

_int CJellyBombCreator::Update_Object(const _float& fTimeDelta)
{
	return _int();
}

void CJellyBombCreator::LateUpdate_Object(void)
{
}

void CJellyBombCreator::Render_Object(void)
{
}

void CJellyBombCreator::Free()
{
}

CJellyBombCreator* CJellyBombCreator::Create(LPDIRECT3DDEVICE9 p_Dev,	CJellyBomb*pBomb, const _uint& p_EventNum, const _vec3& p_Pos)
{
	CJellyBombCreator* ret = new CJellyBombCreator(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("JellyBombCreator Create Failed");
		return nullptr;
	}

	ret->m_pJellyBomb = pBomb;
	Add_Subscribe(p_EventNum, ret);

	ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));
	ret->m_pTransformCom->Set_Pos(&_vec3(p_Pos.x, 0.005, p_Pos.z));

	return ret;
}

void CJellyBombCreator::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CJellyBombCreator::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CJellyBombCreator::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CJellyBombCreator::Event_Start(_uint iEventNum)
{
}

void CJellyBombCreator::Event_End(_uint iEventNum)
{
}

void CJellyBombCreator::Set_SubscribeEvent(_uint pEvent)
{
}
