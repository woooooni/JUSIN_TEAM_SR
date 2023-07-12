#include "../Include/stdafx.h"
#include "HitObj.h"
#include "Export_Function.h"
#include <time.h>

CHitObj::CHitObj(LPDIRECT3DDEVICE9 p_Dev) 
	: CFieldObject(p_Dev, OBJ_ID::BUG_STATUE)
	, m_eHitType(OBJ_HITTYPE::HIT_ONCE)
	, m_fEffectTime(0.f), m_bHitted(false)
	, m_iEventNum(0)
	, m_vOrigin(0, 0, 0)
	, m_vToward(0, 0, 0)
	, m_fResetTime(0.f)
{
}

CHitObj::CHitObj(const CHitObj& rhs) : CFieldObject(rhs), m_eHitType(rhs.m_eHitType), m_fEffectTime(rhs.m_fEffectTime), m_bHitted(rhs.m_bHitted), m_iEventNum(rhs.m_iEventNum), m_vOrigin(rhs.m_vOrigin), m_vToward(rhs.m_vToward), m_fResetTime(rhs.m_fResetTime)
{
}

CHitObj::~CHitObj()
{
}

HRESULT CHitObj::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());

	FAILED_CHECK(m_pAnimator->Add_Animation(L"Normal", L"Proto_Tex_BugStatue", 0.1f));

	m_pAnimator->Play_Animation(L"Normal", false);

    return S_OK;
}

_int CHitObj::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);

	if (m_fEffectTime > 0.f)
	{
		


		m_pTransformCom->Move_Pos(&m_vToward, 1.f, fTimeDelta);

		m_fEffectTime -= fTimeDelta;
		m_fResetTime += fTimeDelta;

		if (m_fResetTime > 0.1f)
		{
			_vec3		vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			D3DXVec3Normalize(&m_vToward, &(m_vOrigin - vPos));

			if (m_vToward == _vec3(0, 0, 0))
				Make_Toward();
			m_fResetTime = 0.f;
		}
	}
	else if (m_vToward != _vec3(0, 0, 0))
	{
		m_vToward = { 0, 0, 0 };
		m_fEffectTime = 0.f;
		m_pTransformCom->Set_Pos(&m_vOrigin);
	}

    _int ret = __super::Update_Object(fTimeDelta);
    return ret;
}

void CHitObj::LateUpdate_Object(void)
{
}

void CHitObj::Render_Object(void)
{

	__super::Render_Object();
}

void CHitObj::Free()
{
    __super::Free();
}

CHitObj* CHitObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos)
{

    CHitObj* ret = new CHitObj(p_Dev);
    NULL_CHECK_RETURN(ret, nullptr);
    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("HitObjCreate Failed");
        return nullptr;
    }

	ret->m_iEventNum = p_EventNum;
	ret->m_vOrigin = p_Pos;
	ret->m_pTransformCom->Set_Pos(&p_Pos);

    return ret;
}

void CHitObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	switch (m_eHitType)
	{
	case Engine::OBJ_HITTYPE::HIT_ONCE:
	case Engine::OBJ_HITTYPE::HIT_REPEAT:

		Check_Event_Start(m_iEventNum);
		m_fEffectTime = 0.5f;
		Make_Toward();
		
		break;
	case Engine::OBJ_HITTYPE::HIT_BREAK:
		break;
	case Engine::OBJ_HITTYPE::HIT_END:
		break;
	default:
		break;
	}
}

HRESULT CHitObj::Ready_Component()
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

void CHitObj::Make_Toward()
{
	srand(unsigned(time(NULL)));
	do
	{
		m_vToward = { (float)((rand() % 90) - 45), (float)((rand() % 90) - 45) , (float)((rand() % 90) - 45) };

	} while (m_vToward == _vec3(0, 0, 0));

	D3DXVec3Normalize(&m_vToward, &m_vToward);

}

