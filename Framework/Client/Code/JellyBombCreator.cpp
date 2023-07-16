#include "JellyBombCreator.h"
#include	"Export_Function.h"

CJellyBombCreator::CJellyBombCreator(LPDIRECT3DDEVICE9 pDev) : CFieldObject(pDev, OBJ_ID::JELLY_BOMB_CREATOR), m_pJellyBomb(nullptr), m_bIsCreate(false)
{

}


CJellyBombCreator::CJellyBombCreator(const CJellyBombCreator& rhs) : CFieldObject(rhs), m_pJellyBomb(rhs.m_pJellyBomb), m_bIsCreate(rhs.m_bIsCreate)
{
}

CJellyBombCreator::~CJellyBombCreator()
{
}

HRESULT CJellyBombCreator::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_JellyBomb_Creator", 0.f);
	m_pAnimator->Play_Animation(L"Base", false);

	return S_OK;
}

_int CJellyBombCreator::Update_Object(const _float& fTimeDelta)
{
	if (m_bIsCreate)
	{
		m_bIsCreate = false;

		m_pJellyBomb->Reset();
		_vec3	vec;
		m_pTransformCom->Get_Info(INFO_POS, &vec);


		m_pJellyBomb->Get_TransformCom()->Set_Pos(&vec);
		auto iter = find(Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Get_GameObjectVec().begin(), Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Get_GameObjectVec().end(), m_pJellyBomb);
		if (iter == Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Get_GameObjectVec().end())
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyBomb", m_pJellyBomb);

	}

	Add_RenderGroup(RENDER_ALPHA, this);

	return __super::Update_Object(fTimeDelta);
}

void CJellyBombCreator::LateUpdate_Object(void)
{
	

	__super::LateUpdate_Object();
}

void CJellyBombCreator::Render_Object(void)
{
	__super::Render_Object();
}

void CJellyBombCreator::Free()
{
	__super::Free();
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
	Add_Subscribe(pBomb->Get_EventNum(), ret);

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
	if (!m_pJellyBomb->Is_Active())
	{
		m_bIsCreate = true;

	}
}

void CJellyBombCreator::Event_End(_uint iEventNum)
{
}

void CJellyBombCreator::Set_SubscribeEvent(_uint pEvent)
{
}
