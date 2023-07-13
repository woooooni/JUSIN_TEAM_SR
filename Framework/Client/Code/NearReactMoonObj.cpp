#include "../Include/stdafx.h"
#include "NearReactMoonObj.h"
#include "Export_Function.h"

CNearReactMoonObj::CNearReactMoonObj(LPDIRECT3DDEVICE9 pGraphicDev) : CNearReactObj(pGraphicDev, OBJ_ID::NEAR_REACT_MOON)
{
}

CNearReactMoonObj::CNearReactMoonObj(const CNearReactMoonObj& rhs) : CNearReactObj(rhs), m_bIsClose(rhs.m_bIsClose)
{
}

CNearReactMoonObj::~CNearReactMoonObj()
{
}

HRESULT CNearReactMoonObj::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Flower_Move", L"Proto_Tex_FlowerMove", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Flower_Stop", L"Proto_Tex_FlowerStop", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"Flower_Move", TRUE), E_FAIL);

	return S_OK;

}

_int CNearReactMoonObj::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);


	CGameObject* player = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");

	_vec3 src, tmp;

	player->Get_TransformCom()->Get_Info(INFO_POS, &src);

	m_pTransformCom->Get_Info(INFO_POS, &tmp);

	if (D3DXVec3Length(&(src - tmp)) <= m_fFindRange && m_bIsClose == false)
	{
		m_pAnimator->Play_Animation(L"Flower_Stop", FALSE);
		m_bIsClose = true;
	}
	else if (D3DXVec3Length(&(src - tmp)) > m_fFindRange && m_bIsClose == true)
	{
		m_pAnimator->Play_Animation(L"Flower_Move", TRUE);
		m_bIsClose = false;

	}



	_int Ret = __super::Update_Object(fTimeDelta);


	return Ret;

}

void CNearReactMoonObj::LateUpdate_Object(void)
{
}

void CNearReactMoonObj::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();


}

void CNearReactMoonObj::Free()
{
	__super::Free();
}

CNearReactMoonObj* CNearReactMoonObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 p_pos)
{
	CNearReactMoonObj* ret = new CNearReactMoonObj(pGraphicDev);

	if (ret->Ready_Object() == E_FAIL)
	{
		Safe_Release(ret);
		MSG_BOX("NearCheckMoonObj_Create_Failed");
		return nullptr;
	}

	ret->Get_TransformCom()->Set_Pos(&p_pos);


	return ret;

}

HRESULT CNearReactMoonObj::Ready_Component(void)
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
