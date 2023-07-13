#include "JellyCombined.h"
#include "Export_Function.h"
#include	"JellyStone.h"
#include	<time.h>

CJellyCombined::CJellyCombined(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::JELLY_COMBINED), m_eColor(JELLY_COLLOR_COMBINE::JELLY_COMBINEEND)
{
	m_tInfo.m_bIsPushable = true;
	m_tInfo.m_bIsGrabbable = true;

}

CJellyCombined::CJellyCombined(const CJellyCombined& rhs) : CFieldObject(rhs), m_eColor(rhs.m_eColor)
{
}

CJellyCombined::~CJellyCombined()
{
}

HRESULT CJellyCombined::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	return S_OK;
}

_int CJellyCombined::Update_Object(const _float& fTimeDelta)
{
	if (Is_Active())
	{
		Add_RenderGroup(RENDER_ALPHA, this);
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_GRAB);

	}


	return __super::Update_Object(fTimeDelta);
}

void CJellyCombined::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CJellyCombined::Render_Object(void)
{
	__super::Render_Object();
}

void CJellyCombined::Free()
{
	__super::Free();
}

CJellyCombined* CJellyCombined::Create(LPDIRECT3DDEVICE9 p_Dev, JELLY_COLLOR_COMBINE p_Color, const _uint& p_EventNum, const _vec3 p_Pos)
{
	CJellyCombined* ret = new CJellyCombined(p_Dev);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("JellyCombineCreateFailed");
		return nullptr;
	}

	ret->m_eColor = p_Color;
	switch (ret->m_eColor)
	{	
	case JELLY_COLLOR_COMBINE::RED :
		ret->m_pAnimator->Add_Animation(L"Red", L"Proto_Tex_Jelly_Red", 1.f);
		ret->m_pAnimator->Play_Animation(L"Red", false);
		break;
	case JELLY_COLLOR_COMBINE::GREEN:
		ret->m_pAnimator->Add_Animation(L"Green", L"Proto_Tex_Jelly_Green", 1.f);
		ret->m_pAnimator->Play_Animation(L"Green", false);

		break;
	case JELLY_COLLOR_COMBINE::BLUE:
		ret->m_pAnimator->Add_Animation(L"Blue", L"Proto_Tex_Jelly_Blue", 1.f);
		ret->m_pAnimator->Play_Animation(L"Blue", false);

		break;


	default:
		break;
	}
	ret->Set_SubscribeEvent(p_EventNum);
	ret->m_pTransformCom->Set_Pos(&p_Pos);

	return ret;
}

void CJellyCombined::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CJellyCombined::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BALPAN || _eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER)
		return;


	Push_Me(pCollider);
}

void CJellyCombined::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CJellyCombined::Ready_Component()
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

void CJellyCombined::Event_Start(_uint iEventNum)
{
}

void CJellyCombined::Event_End(_uint iEventNum)
{
}

CGameObject* CJellyCombined::Get_GrabObj()
{
	
	vector<CGameObject*>& src = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Get_GameObjectVec();

	_vec3	dst;

	m_pTransformCom->Get_Info(INFO_POS, &dst);


	auto iter = find_if(src.begin(), src.end(), [&](auto A)->bool
		{
			CJellyStone* src;

			return ((src = dynamic_cast<CJellyStone*>(A)) && Check_Child(src->Get_JellyColor()) && !src->Is_Active());
		});

	if (iter == src.end())
	{
		CJellyStone* tmp;
		CJellyStone* src;

		srand(unsigned(time(NULL)));
		switch (m_eColor)
		{
		case Engine::JELLY_COLLOR_COMBINE::BLUE:

			tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::CYAN, 0, dst);
			NULL_CHECK_RETURN_MSG(tmp, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", tmp);

			src = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::MAGENTA, 0, dst);
			NULL_CHECK_RETURN_MSG(src, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", src);
			Set_Active(false);
			if (rand() % 2)
			{
				return tmp;
			}
			else
				return src;


			break;
		case Engine::JELLY_COLLOR_COMBINE::GREEN:
			tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::YELLOW, 0, dst);
			NULL_CHECK_RETURN_MSG(tmp, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", tmp);

			src = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::CYAN, 0, dst);
			NULL_CHECK_RETURN_MSG(src, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", src);
			Set_Active(false);
			if (rand() % 2)
			{
				return tmp;
			}
			else
				return src;
			break;
		case Engine::JELLY_COLLOR_COMBINE::RED:
			tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::MAGENTA, 0, dst);
			NULL_CHECK_RETURN_MSG(tmp, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", tmp);

			src = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::YELLOW, 0, dst);
			NULL_CHECK_RETURN_MSG(src, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", src);
			Set_Active(false);
			if (rand() % 2)
			{
				return tmp;
			}
			else
				return src;
			break;
		case Engine::JELLY_COLLOR_COMBINE::JELLY_COMBINEEND:
			return nullptr;
		default:
			return nullptr;
		}
	}

	auto tmp = find_if(src.begin(), src.end(), [&](auto A)->bool
		{
			CJellyStone* src;
			return ((src = dynamic_cast<CJellyStone*>(A)) && ((_uint)src->Get_JellyColor() == ((_uint)m_eColor - (_uint)dynamic_cast<CJellyStone*>(*iter)->Get_JellyColor())) && !src->Is_Active());
		});


	if (tmp == src.end())
	{
		CJellyStone* jel1;
		CJellyStone* jel2;

		srand(unsigned(time(NULL)));
		switch (m_eColor)
		{
		case Engine::JELLY_COLLOR_COMBINE::BLUE:

			jel1 = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::CYAN, 0, dst);
			NULL_CHECK_RETURN_MSG(jel1, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", jel1);

			jel2 = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::MAGENTA, 0, dst);
			NULL_CHECK_RETURN_MSG(jel2, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", jel2);
			Set_Active(false);
			if (rand() % 2)
			{
				return jel1;
			}
			else
				return jel2;			
			break;

		case JELLY_COLLOR_COMBINE::GREEN:
			jel1 = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::CYAN, 0, dst);
			NULL_CHECK_RETURN_MSG(jel1, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", jel1);

			jel2 = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::YELLOW, 0, dst);
			NULL_CHECK_RETURN_MSG(jel2, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", jel2);
			Set_Active(false);
			if (rand() % 2)
			{
				return jel1;
			}
			else
				return jel2;

			break;
		case Engine::JELLY_COLLOR_COMBINE::RED:
			jel1 = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::YELLOW, 0, dst);
			NULL_CHECK_RETURN_MSG(jel1, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", jel1);

			jel2 = CJellyStone::Create(m_pGraphicDev, JELLY_COLLOR_NORMAL::MAGENTA, 0, dst);
			NULL_CHECK_RETURN_MSG(jel2, nullptr, L"Jellystone Create Failed");
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"JellyStone", jel2);
			Set_Active(false);
			if (rand() % 2)
			{
				return jel1;
			}
			else
				return jel2;

		break;		
		case Engine::JELLY_COLLOR_COMBINE::JELLY_COMBINEEND:
			return nullptr;
		default:
			return nullptr;
		}

	}

	(*iter)->Get_TransformCom()->Set_Pos(&_vec3(dst.x, 0.5f, dst.y));
	(*tmp)->Get_TransformCom()->Set_Pos(&_vec3(dst.x, 0.5f, dst.y));
	(*iter)->Set_Active(true);
	(*tmp)->Set_Active(true);
	dynamic_cast<CJellyStone*>(*iter)->Reset_Created();
	dynamic_cast<CJellyStone*>(*tmp)->Reset_Created();


	Set_Active(false);

	return *iter;

}

void CJellyCombined::Set_SubscribeEvent(_uint pEvent)
{
	Add_Subscribe(pEvent, this);
}

bool CJellyCombined::Check_Child(const JELLY_COLLOR_NORMAL& pColor)
{
	if ((_int)pColor < 0 || pColor >= JELLY_COLLOR_NORMAL::JELLY_NORMALEND)
		return false;

	switch (m_eColor)
	{
	case Engine::JELLY_COLLOR_COMBINE::BLUE:
		if (pColor == JELLY_COLLOR_NORMAL::YELLOW)
			return false;
		else
			return true;
		break;
	case Engine::JELLY_COLLOR_COMBINE::GREEN:
		if (pColor == JELLY_COLLOR_NORMAL::MAGENTA)
			return false;
		else
			return true;
		break;
	case Engine::JELLY_COLLOR_COMBINE::RED:
		if (pColor == JELLY_COLLOR_NORMAL::CYAN)
			return false;
		else
			return true;
		break;

	default:
		return false;
		break;
	}
}
