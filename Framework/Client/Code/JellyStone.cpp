#include "../Include/stdafx.h"
#include "..\Header\JellyStone.h"
#include "Export_Function.h"
#include	"JellyCombined.h"
#include	"Pool.h"
#include	<time.h>


CJellyStone::CJellyStone(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::JELLY_STONE), m_eColor(JELLY_COLOR::JELLY_END), m_bCreatedCombine(false)
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

	CComponent* pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ COMPONENT_TYPE::COM_RIGIDBODY, pComponent });


    return S_OK;
}

_int CJellyStone::Update_Object(const _float& fTimeDelta)
{

	if (!m_bCreatedCombine || (m_bCreatedCombine && m_eColor >= JELLY_COLOR::BLUE))
	{
		Add_RenderGroup(RENDER_ALPHA, this);
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_GRAB);
	}




    return __super::Update_Object(fTimeDelta);
}

void CJellyStone::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CJellyStone::Render_Object(void)
{
	m_pAnimator->Render_Component();
	__super::Render_Object();

}

void CJellyStone::Free()
{
	__super::Free();
}

CJellyStone* CJellyStone::Create(LPDIRECT3DDEVICE9 p_Dev,JELLY_COLOR p_Color, const _uint& p_EventNum, const _vec3 p_Pos)
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
	ret->m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_Jelly", 0.f);
	ret->m_pAnimator->Play_Animation(L"Base", false);

	ret->Set_Color(p_Color);
	ret->m_eOriginColor = p_Color;
	ret->m_vOriginPos = p_Pos;


	ret->Set_MinHeight(ret->m_pTransformCom->Get_Scale().y * 0.5f);


	Add_Subscribe(p_EventNum, ret);

    return ret;
}

void CJellyStone::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	CJellyStone* src = nullptr;
	if ((src = dynamic_cast<CJellyStone*>(pCollider->GetOwner())) && src->m_eColor != m_eColor && m_eColor < JELLY_COLOR::BLUE && src->m_eColor < JELLY_COLOR::BLUE)
	{
		if (m_bCreatedCombine)
			return;

		if (src->m_bCreatedCombine && !m_bCreatedCombine)
		{
			m_bCreatedCombine = true;
			return;
		}

		_vec3 tmp, dst;

		m_pTransformCom->Get_Info(INFO_POS, &tmp);
		src->m_pTransformCom->Get_Info(INFO_POS, &dst);

		m_pTransformCom->Set_Pos(&((tmp + dst) * 0.5f));

		Set_Color((JELLY_COLOR)((_uint)src->Get_JellyColor() + (_uint)m_eColor + 2));

		m_bCreatedCombine = true;
		src->m_bCreatedCombine = true;

		Stop_Sound(CHANNELID::SOUND_EFFECT_ENVIRONMENT);
		Play_Sound(L"SFX_45_Jelly_Combine.wav", CHANNELID::SOUND_EFFECT_ENVIRONMENT, .5f);


	}
	else if (src)
	{
		Stop_Sound(CHANNELID::SOUND_EFFECT_ENVIRONMENT);
		Play_Sound(L"SFX_75_JellyCollision.wav", CHANNELID::SOUND_EFFECT_ENVIRONMENT, .5f);
	}
}

void CJellyStone::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BALPAN || _eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_WALL)
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
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);


	return S_OK;

}

void CJellyStone::Event_Start(_uint iEventNum)
{
}

void CJellyStone::Event_End(_uint iEventNum)
{
	
}

void CJellyStone::Reset_Event()
{
	m_pTransformCom->Set_Pos(&m_vOriginPos);
	m_eColor = m_eOriginColor;
	for (auto& iter : m_listCreatedJellys)
	{
		iter->Set_Active(false);
	}
	m_listCreatedJellys.clear();
	m_pAnimator->GetCurrAnimation()->Set_Idx((_uint)m_eColor);

}

void CJellyStone::Set_SubscribeEvent(_uint pEvent)
{
	Add_Subscribe(pEvent, this);
}

void CJellyStone::Set_Color(const JELLY_COLOR& pColor)
{
	if (((m_eColor < JELLY_COLOR::BLUE) || m_eColor == JELLY_COLOR::JELLY_END) && pColor >= JELLY_COLOR::BLUE)
	{
		m_pTransformCom->Set_Scale({ 1.2f, 1.2f, 1.2f });
		dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 1.2f, 1.2f, 1.2f });

	}
	else if (((m_eColor >= JELLY_COLOR::BLUE) && m_eColor != JELLY_COLOR::JELLY_END) && pColor < JELLY_COLOR::BLUE)
	{
		m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f });
		dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 1.f, 1.f, 1.f});

	}

	m_eColor = pColor;
	Set_MinHeight(m_pTransformCom->Get_Scale().y * 0.5f);
	m_pAnimator->GetCurrAnimation()->Set_Idx((_uint)m_eColor);
}

CGameObject* CJellyStone::Get_GrabObj()
{
	if (m_eColor < JELLY_COLOR::BLUE)
		return this;

	auto& vec = Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Get_GameObjectVec();

	CJellyStone* tmp;

	auto iter = find_if(vec.begin(), vec.end(), [&](CGameObject* pObj)->bool
		{
			return ((tmp = dynamic_cast<CJellyStone*>(pObj)) && (Check_Child(tmp->Get_JellyColor())) && tmp->Get_JellyColor() != m_eOriginColor && tmp->Get_Created());
		});

	_vec3 myPos;
	m_pTransformCom->Get_Info(INFO_POS, &myPos);

	if (iter == vec.end())
	{
		srand(unsigned(time(NULL)));

		switch (m_eOriginColor)
		{
		case Engine::JELLY_COLOR::CYAN:
		case Engine::JELLY_COLOR::MAGENTA:
		case Engine::JELLY_COLOR::YELLOW:

			tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR((_uint)m_eColor - (_uint)m_eOriginColor - 2), 0, myPos);
			tmp->m_pTransformCom->Set_Pos(&myPos);
			Set_Color(m_eOriginColor);
			m_pAnimator->GetCurrAnimation()->Set_Idx((_uint)m_eColor);
			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"Jelly", tmp);
			m_bCreatedCombine = false;
			return this;
		case Engine::JELLY_COLOR::RED:

			if (rand() % 2)
			{
				tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 0, myPos);
				tmp->m_pTransformCom->Set_Pos(&myPos);
				Set_Color(JELLY_COLOR::MAGENTA) ;
				m_pAnimator->GetCurrAnimation()->Set_Idx((_uint)m_eColor);

			}
			else
			{
				tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::MAGENTA, 0, myPos);
				tmp->m_pTransformCom->Set_Pos(&myPos);
				Set_Color(JELLY_COLOR::YELLOW);
				m_pAnimator->GetCurrAnimation()->Set_Idx((_uint)m_eColor);

			}

			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"Jelly", tmp);
			m_bCreatedCombine = false;
			return this;
		case Engine::JELLY_COLOR::BLUE:
			if (rand() % 2)
			{
				tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::CYAN, 0, myPos);
				tmp->m_pTransformCom->Set_Pos(&myPos);
				Set_Color(JELLY_COLOR::MAGENTA);
				m_pAnimator->GetCurrAnimation()->Set_Idx((_uint)m_eColor);

			}
			else
			{
				tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::MAGENTA, 0, myPos);
				tmp->m_pTransformCom->Set_Pos(&myPos);
				Set_Color(JELLY_COLOR::CYAN);
				m_pAnimator->GetCurrAnimation()->Set_Idx((_uint)m_eColor);

			}

			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"Jelly", tmp);
			m_bCreatedCombine = false;
			return this;
		case Engine::JELLY_COLOR::GREEN:
			if (rand() % 2)
			{
				tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::YELLOW, 0, myPos);
				tmp->m_pTransformCom->Set_Pos(&myPos);
				Set_Color(JELLY_COLOR::CYAN);
				m_pAnimator->GetCurrAnimation()->Set_Idx((_uint)m_eColor);

			}
			else
			{
				tmp = CJellyStone::Create(m_pGraphicDev, JELLY_COLOR::CYAN, 0, myPos);
				tmp->m_pTransformCom->Set_Pos(&myPos);
				Set_Color(JELLY_COLOR::YELLOW);
				m_pAnimator->GetCurrAnimation()->Set_Idx((_uint)m_eColor);

			}

			Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"Jelly", tmp);
			m_bCreatedCombine = false;
			return this;
		case Engine::JELLY_COLOR::JELLY_END:
			break;
		default:
			break;
		}
	}
	else
	{
		tmp->m_bCreatedCombine = false;
		m_bCreatedCombine = false;
		tmp->m_pTransformCom->Set_Pos(&myPos);
		Set_Color((JELLY_COLOR)((_uint)m_eColor - (_uint)tmp->Get_JellyColor() - 2 ));
		return this;
	}

}

bool CJellyStone::Check_Child(const JELLY_COLOR& pColor)
{
	switch (m_eColor)
	{
	case Engine::JELLY_COLOR::CYAN:
	return false;
	case Engine::JELLY_COLOR::MAGENTA:
		return false;
	case Engine::JELLY_COLOR::YELLOW:
		return false;
	case Engine::JELLY_COLOR::RED:

		if (pColor == JELLY_COLOR::MAGENTA || pColor == JELLY_COLOR::YELLOW)
			return true;
		else
			return false;
		break;
	case Engine::JELLY_COLOR::BLUE:

		if (pColor == JELLY_COLOR::MAGENTA || pColor == JELLY_COLOR::CYAN)
			return true;
		else
			return false;

		break;
	case Engine::JELLY_COLOR::GREEN:

		if (pColor == JELLY_COLOR::CYAN || pColor == JELLY_COLOR::YELLOW)
			return true;
		else
			return false;

		break;
	case Engine::JELLY_COLOR::JELLY_END:
		break;
	default:
		return false;
	}
}