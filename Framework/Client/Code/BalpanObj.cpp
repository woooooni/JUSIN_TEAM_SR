#include "../Include/stdafx.h"
#include "BalpanObj.h"
#include "Export_Function.h"

CBalpanObj::CBalpanObj(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev), m_bIsPushed(false), m_wstrTargName(L"Player"), m_bIsAutoReset(false), m_iPushedEventNum(0)
{
}

CBalpanObj::CBalpanObj(const CBalpanObj& rhs) : CFieldObject(rhs), m_bIsPushed(rhs.m_bIsPushed), m_bIsAutoReset(rhs.m_bIsAutoReset), m_iPushedEventNum(rhs.m_iPushedEventNum)
{
	m_wstrTargName = rhs.m_wstrTargName;
}

CBalpanObj::~CBalpanObj()
{
}

HRESULT CBalpanObj::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"UnActivate", L"Proto_Tex_BalpanUnactivate", 0.2f);
	m_pAnimator->Add_Animation(L"Idle", L"Proto_Tex_BalpanNotPressed", 0.2f);
	m_pAnimator->Add_Animation(L"Pushed", L"Proto_Tex_BalpanPressed", 0.2f);

	m_pAnimator->Play_Animation(L"Idle", false);


	return S_OK;
}

_int CBalpanObj::Update_Object(const _float& fTimeDelta)
{
	if(m_listActivateNum.empty() && !m_bIsPushed)
		m_pAnimator->Play_Animation(L"Idle", false);

	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BALPAN);




	

    _int       ret = __super::Update_Object(fTimeDelta);
    return ret;
}

void CBalpanObj::LateUpdate_Object(void)
{
	if (!m_listActivateNum.empty())
	{
		for (auto iter = m_listActivateNum.begin(); iter != m_listActivateNum.end();)
		{
			if (Check_Event_Start(*iter) == S_OK)
			{
				iter = m_listActivateNum.erase(iter);
			}
			else
				++iter;
		}
	}


}

void CBalpanObj::Render_Object(void)
{
	Set_Billboard();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CBalpanObj::Free()
{
    __super::Free();
}

CBalpanObj* CBalpanObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum,const _vec3 p_Pos)
{
    CBalpanObj* ret = new CBalpanObj(p_Dev);

    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("Balpan Create Failed");
        return nullptr;
    }

	ret->Get_TransformCom()->Set_Pos(&p_Pos);
	ret->Set_EventNum(p_EventNum);



    return ret;
}

void CBalpanObj::Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID)
{	
	if (m_listActivateNum.empty() && pCollisionObj->Get_Name() == m_wstrTargName)
	{
		if (m_bIsPushed && !m_bIsAutoReset)
			return;

		m_bIsPushed = true;
		m_pAnimator->Play_Animation(L"Pushed", false);
		if (m_iPushedEventNum != 0)
			Engine::Check_Event_Start(m_iPushedEventNum);
	}

		
}

void CBalpanObj::Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

void CBalpanObj::Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID)
{
	if (m_bIsAutoReset && m_bIsPushed && pCollisionObj->Get_Name() == m_wstrTargName)
	{
		Reset_Pushed();
		m_pAnimator->Play_Animation(L"Idle", false);
		if (m_iPushedEventNum != 0)
			Engine::Check_Event_Start(m_iPushedEventNum);


	}
}

 void CBalpanObj::Reset_Pushed() { m_bIsPushed = false; 	}

 void CBalpanObj::Set_AutoReset() { m_bIsAutoReset = !m_bIsAutoReset; }

void CBalpanObj::Add_ActivateState(const _uint& pNum)
 {
	 m_listActivateNum.push_back(pNum);
	 Engine::Add_Subscribe(pNum, this);
	 m_pAnimator->Play_Animation(L"UnActivate", false);
 }

 HRESULT CBalpanObj::Ready_Component()
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

 void CBalpanObj::Event_Start(_uint iEventNum)
 {
	 auto iter = find(m_listActivateNum.begin(), m_listActivateNum.end(), iEventNum);

	 if (iter != m_listActivateNum.end())
	 {

	 }
 }

 void CBalpanObj::Event_End(_uint iEventNum)
 {
 }
