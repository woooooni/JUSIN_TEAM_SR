#include "../Include/stdafx.h"
#include "BalpanObj.h"
#include "Export_Function.h"
#include "JellyStone.h"
#include	"JellyCombined.h"

CBalpanObj::CBalpanObj(LPDIRECT3DDEVICE9 p_Dev) 
	:  CFieldObject(p_Dev, OBJ_ID::BALPAN_OBJ)
	, m_bIsPushed(false)
	, m_wstrTargName(L"Player")
	, m_bIsAutoReset(false) 
	, m_iPushedEventNum(0) 
	, m_eColor(JELLY_COLOR::JELLY_END)
	, m_bIsStoneSwitch(false)
	, m_eCorrectColor(JELLY_COLOR::JELLY_END)
{
}

CBalpanObj::CBalpanObj(const CBalpanObj& rhs) : CFieldObject(rhs), m_bIsPushed(rhs.m_bIsPushed), m_bIsAutoReset(rhs.m_bIsAutoReset), m_iPushedEventNum(rhs.m_iPushedEventNum), m_eColor(rhs.m_eColor), m_bIsStoneSwitch(rhs.m_bIsStoneSwitch), m_eCorrectColor(rhs.m_eCorrectColor)
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
	m_pAnimator->Add_Animation(L"Static", L"Proto_Tex_BalpanStatic", 0.2f);


	m_pAnimator->Play_Animation(L"Idle", false);


	return S_OK;
}

_int CBalpanObj::Update_Object(const _float& fTimeDelta)
{
	if(m_listActivateNum.empty() && !m_bIsPushed && !m_bIsStoneSwitch)
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
	__super::Render_Object();
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
	ret->Get_TransformCom()->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));
	ret->Get_TransformCom()->Set_Pos(&_vec3(p_Pos.x, 0.02f, p_Pos.z));
	ret->Set_EventNum(p_EventNum);
	Add_Subscribe(p_EventNum, ret);


    return ret;
}

void CBalpanObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{	
	if ((m_listActivateNum.empty() && ((pCollider->GetOwner()->Get_Name() == m_wstrTargName))) || ((m_wstrTargName == L"Jelly") && (dynamic_cast<CJellyStone*>(pCollider->GetOwner()))))
	{
		if (m_bIsPushed)
			return;

		if (!m_bIsStoneSwitch)
			Collide(pCollider);
		else
		{
			_vec3 tmp, src, dst;
			pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &tmp);
			tmp += pCollider->Get_Offset();

			m_pTransformCom->Get_Info(INFO_POS, &src);
			src += m_pColliderCom->Get_Offset();

			dst = src - tmp;
			dst.y = 0.f;
			if (D3DXVec3Length(&dst) < 0.1f)
			{
				Collide(pCollider);
			}
			else if (D3DXVec3Length(&dst) < 0.5f)
			{
				pCollider->GetOwner()->Get_TransformCom()->Move_Pos(D3DXVec3Normalize(&dst, &dst), 0.5f, Get_TimeDelta(L"Timer_FPS60"));
			}
		}

	}

		
}

void CBalpanObj::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if ((m_listActivateNum.empty() && ((pCollider->GetOwner()->Get_Name() == m_wstrTargName))) || ((m_wstrTargName == L"Jelly") && (dynamic_cast<CJellyStone*>(pCollider->GetOwner()))))
	{
		if (m_bIsStoneSwitch)
		{
			_vec3 tmp, src, dst;
			pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &tmp);
			tmp += pCollider->Get_Offset();

			m_pTransformCom->Get_Info(INFO_POS, &src);
			src += m_pColliderCom->Get_Offset();
			

			dst = src - tmp;
			dst.y = 0.f;
			if (D3DXVec3Length(&dst) < 0.1f && !m_bIsPushed)
			{
				Collide(pCollider);
			}
			else if (D3DXVec3Length(&dst) > 0.1f && m_bIsPushed)
				Reset_Pushed();

			if (0.02f < D3DXVec3Length(&dst) && 0.5f > D3DXVec3Length(&dst))
			{
				pCollider->GetOwner()->Get_TransformCom()->Move_Pos(D3DXVec3Normalize(&dst, &dst), 0.5f, Get_TimeDelta(L"Timer_FPS60"));

			}

		}

	}

}

void CBalpanObj::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{

	if (m_bIsAutoReset && m_bIsPushed && (pCollider->GetOwner()->Get_Name() == m_wstrTargName || ((m_wstrTargName == L"Jelly") &&  dynamic_cast<CJellyStone*>(pCollider->GetOwner()))))
	{
		Reset_Pushed();
		if(!m_bIsStoneSwitch)
			m_pAnimator->Play_Animation(L"Idle", false);
	}
}

 void CBalpanObj::Reset_Pushed() {
	 m_bIsPushed = false; 		

	if (m_iPushedEventNum != 0 && m_bIsAutoReset && ((m_eColor == m_eCorrectColor && m_eCorrectColor != JELLY_COLOR::JELLY_END) || m_eCorrectColor == JELLY_COLOR::JELLY_END))
		 Engine::Check_Event_Start(m_iPushedEventNum);
	m_eColor = JELLY_COLOR::JELLY_END;

 }

 void CBalpanObj::Set_AutoReset() { m_bIsAutoReset = !m_bIsAutoReset; }

void CBalpanObj::Add_ActivateState(const _uint& pNum)
 {
	 m_listActivateNum.push_back(pNum);
	 Engine::Add_Subscribe(pNum, this);
	 if(!m_bIsStoneSwitch)
		m_pAnimator->Play_Animation(L"UnActivate", false);
 }

void CBalpanObj::Set_Static()
{
	m_bIsAutoReset = true;
	m_bIsStoneSwitch = true;
	m_pAnimator->Play_Animation(L"Static", false);
}

void CBalpanObj::Set_StartActive(const JELLY_COLOR& pColor)
{
	m_eColor = pColor;
	m_bIsPushed = true;
	Check_Event_Start(m_iPushedEventNum);
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

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);


	return S_OK;

}

 void CBalpanObj::Event_Start(_uint iEventNum)
 {

 }

 void CBalpanObj::Event_End(_uint iEventNum)
 {
 }

 void CBalpanObj::Reset_Event()
 {
	 Reset_Pushed();
 }

 void CBalpanObj::Collide(CCollider* pCollider)
 {


	 m_bIsPushed = true;
	 if (!m_bIsStoneSwitch)
		 m_pAnimator->Play_Animation(L"Pushed", false);



	 if (m_wstrTargName == L"Jelly")
	 {
		 if (dynamic_cast<CJellyStone*>(pCollider->GetOwner()))
		 {
			 m_eColor = dynamic_cast<CJellyStone*>(pCollider->GetOwner())->Get_JellyColor();
		 }

	 }

	 if (m_iPushedEventNum != 0)
	 {
		 if (m_wstrTargName == L"Jelly" && m_eCorrectColor != JELLY_COLOR::JELLY_END && m_eColor != m_eCorrectColor)
		 {
			 return;
		 }

		 Engine::Check_Event_Start(m_iPushedEventNum);

	 }

 }
