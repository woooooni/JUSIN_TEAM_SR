#include "../Include/stdafx.h"
#include "..\Header\JellyStone.h"
#include "Export_Function.h"
#include	"JellyCombined.h"


CJellyStone::CJellyStone(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::JELLY_STONE), m_eColor(JELLY_COLLOR_NORMAL::JELLY_NORMALEND), m_bCreatedCombine(false)
{
	m_tInfo.m_bIsPushable = true;
	m_tInfo.m_bIsGrabbable = true;
}

CJellyStone::CJellyStone(const CJellyStone& rhs) : CFieldObject(rhs), m_eColor(rhs.m_eColor), m_bCreatedCombine(rhs.m_bCreatedCombine)
{
}

CJellyStone::~CJellyStone()
{
}

HRESULT CJellyStone::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

    return S_OK;
}

_int CJellyStone::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return 0;

	if (m_bCreatedCombine)
		m_bCreatedCombine = false;

	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_GRAB);


    return __super::Update_Object(fTimeDelta);
}

void CJellyStone::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CJellyStone::Render_Object(void)
{
	__super::Render_Object();


}

void CJellyStone::Free()
{
	__super::Free();
}

CJellyStone* CJellyStone::Create(LPDIRECT3DDEVICE9 p_Dev,JELLY_COLLOR_NORMAL p_Color, const _uint& p_EventNum, const _vec3 p_Pos)
{

	CJellyStone* ret = new CJellyStone(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Jellystone Create Failed");
		return nullptr;
	}

	ret->m_pTransformCom->Set_Pos(&p_Pos);
	ret->m_eColor = p_Color;

	switch (ret->m_eColor)
	{
	case JELLY_COLLOR_NORMAL::CYAN :
		ret->m_pAnimator->Add_Animation(L"Cyan", L"Proto_Tex_Jelly_Cyan", 1.f);
		ret->m_pAnimator->Play_Animation(L"Cyan", false);
		break;
	case JELLY_COLLOR_NORMAL::MAGENTA:
		ret->m_pAnimator->Add_Animation(L"Magenta", L"Proto_Tex_Jelly_Magenta", 1.f);
		ret->m_pAnimator->Play_Animation(L"Magenta", false);

		break;
	case JELLY_COLLOR_NORMAL::YELLOW:
		ret->m_pAnimator->Add_Animation(L"Yellow", L"Proto_Tex_Jelly_Yellow", 1.f);
		ret->m_pAnimator->Play_Animation(L"Yellow", false);

		break;

	default:
		break;
	}



	Add_Subscribe(p_EventNum, ret);

    return ret;
}

void CJellyStone::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	CJellyStone* src;
	if ((src = dynamic_cast<CJellyStone*>(pCollider->GetOwner())) && src->m_eColor != m_eColor)
	{
		if (m_bCreatedCombine || src->m_bCreatedCombine)
		{
			m_bCreatedCombine = true;
			Set_Active(false);
			return;

		}

		_vec3 tmp, dst;

		m_pTransformCom->Get_Info(INFO_POS, &tmp);
		src->m_pTransformCom->Get_Info(INFO_POS, &dst);
		tmp.y = 1;
		dst.y = 1;

		vector<CGameObject*>& iter = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Get_GameObjectVec();
		CJellyCombined* jCom;

		auto finder = find_if(iter.begin(), iter.end(), [&](CGameObject* A)
			{				
				return (jCom = dynamic_cast<CJellyCombined*>(A)) && !jCom->Is_Active() && ((_uint)jCom->Get_JellyColor() == (_uint)m_eColor + (_uint)src->m_eColor);
			});

		if (finder == iter.end())
		{
			CJellyCombined* jelly = CJellyCombined::Create(m_pGraphicDev, static_cast<JELLY_COLLOR_COMBINE>((_uint)m_eColor + (_uint)src->m_eColor), 0, 0.5f * (dst + tmp));

			NULL_CHECK(jelly);
			if (FAILED(Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"Jelly_Combined", jelly)))
			{
				Safe_Release(jelly);
				return;
			}

		}
		else
		{
			(*finder)->Get_TransformCom()->Set_Pos(&(0.5f * (dst + tmp)));
			(*finder)->Set_Active(true);
		}
		m_bCreatedCombine = true;
		Set_Active(false);
	}
}

void CJellyStone::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BALPAN || _eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER)
		return;


	Push_Me(pCollider);
}

void CJellyStone::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CJellyStone::Ready_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);


	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);




	return S_OK;

}

void CJellyStone::Event_Start(_uint iEventNum)
{
}

void CJellyStone::Event_End(_uint iEventNum)
{
}

void CJellyStone::Set_SubscribeEvent(_uint pEvent)
{
	Add_Subscribe(pEvent, this);
}
