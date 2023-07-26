#include "../Include/stdafx.h"
#include "NearReactObj.h"
#include "Export_Function.h"
#include "GameMgr.h"

CNearReactObj::CNearReactObj(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID) : CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT, _eID)
, m_fMinSize(0.7f)

{
}

CNearReactObj::CNearReactObj(const CNearReactObj& rhs) : CGameObject(rhs), m_fFindRange(rhs.m_fFindRange), m_fMinSize(rhs.m_fMinSize)

{
}

CNearReactObj::~CNearReactObj()
{
}

HRESULT CNearReactObj::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Nunk_Move", L"Proto_Texture_NearPlant", 0.2f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"Nunk_Move", FALSE), E_FAIL);

	return S_OK;
}

_int CNearReactObj::Update_Object(const _float& fTimeDelta)
{

	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);


	CGameObject* player = CGameMgr::GetInstance()->Get_Player();;

	_vec3 src, tmp;

	player->Get_TransformCom()->Get_Info(INFO_POS, &src);

	m_pTransformCom->Get_Info(INFO_POS, &tmp);

	if (D3DXVec3Length(&(src - tmp)) < m_fFindRange && m_fCurFrame <= 7.5f)
	{
		m_fCurFrame += fTimeDelta * 10.f;

	}
	else if (D3DXVec3Length(&(src - tmp)) < m_fFindRange && m_fCurFrame > 7.5f)
		m_fCurFrame = 15.f - m_fCurFrame;
	else if (D3DXVec3Length(&(src - tmp)) > m_fFindRange && m_fCurFrame <= 7.5f && m_fCurFrame != 0.f)
		m_fCurFrame = 15.f - m_fCurFrame;
	else if (D3DXVec3Length(&(src - tmp)) > m_fFindRange && m_fCurFrame > 7.5f)
	{
		m_fCurFrame += fTimeDelta * 10.f;
		if (m_fCurFrame >= (float)m_pAnimator->GetCurrAnimation()->Get_Size())
			m_fCurFrame = 0.f;
	}
	_int	dst = m_pAnimator->GetCurrAnimation()->Get_Idx();

	_float curSize = (((float)(abs(7 - dst)) / 7.f) * (1.f - m_fMinSize)) + m_fMinSize;

	Set_MinHeight(0.5f * curSize);

	_int Ret = __super::Update_Object(fTimeDelta);


	return Ret;
}

void CNearReactObj::LateUpdate_Object(void)
{
	m_pAnimator->GetCurrAnimation()->Set_Idx((_uint)m_fCurFrame);

	__super::LateUpdate_Object();
}

void CNearReactObj::Render_Object(void)
{
	_int	tmp = m_pAnimator->GetCurrAnimation()->Get_Idx();

	_float curSize = (((float)(abs(7 - tmp)) / 7.f) * (1.f - m_fMinSize)) + m_fMinSize;

	m_pTransformCom->Set_Scale({ curSize, curSize, 1.f });

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

	__super::Render_Object();

	
}

void CNearReactObj::Free()
{
	__super::Free();
}

CNearReactObj* CNearReactObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 p_pos)
{
	CNearReactObj* ret = new CNearReactObj(pGraphicDev, OBJ_ID::NEAR_REACT);

	if (ret->Ready_Object() == E_FAIL)
	{
		Safe_Release(ret);
		MSG_BOX("NearCheckObj_Create_Failed");
		return nullptr;
	}

	ret->Get_TransformCom()->Set_Pos(&p_pos);


	return ret;
}

HRESULT CNearReactObj::Ready_Component(void)
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

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);


	return S_OK;

}
