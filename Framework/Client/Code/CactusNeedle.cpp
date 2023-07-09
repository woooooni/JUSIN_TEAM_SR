#include "CactusNeedle.h"
#include "Export_Function.h"

CCactusNeedle::CCactusNeedle(LPDIRECT3DDEVICE9 pGraphicDev) : Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_BULLET)
{
}
CCactusNeedle::CCactusNeedle(const CCactusNeedle& rhs)
	: Engine::CGameObject(rhs)
{

}

CCactusNeedle::~CCactusNeedle()
{
}

HRESULT CCactusNeedle::Ready_Object(void)
{

	m_fMoveTime = 20.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"CactusNeedlePoison", L"Proto_Texture_CactusNeedlePoison", 0.1f);
	m_pAnimator->Add_Animation(L"CactusNeedle", L"Proto_Texture_CactusNeedle", 0.1f);
	m_pAnimator->Play_Animation(L"CactusNeedle", true);
	m_vDir = { 1,0,0 };
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 1.f, 1.f, 1.f }); 
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({0.2f, 0.2f, 0.2f });
	_vec3 AxisX = { 1,0,0 };
	m_pTransformCom->RotationAxis(AxisX, D3DXToRadian(90.f));

	return S_OK;
}

_int CCactusNeedle::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	if (m_bPoison == true)
		m_pAnimator->Play_Animation(L"CactusNeedlePoison", true);
	else
		m_pAnimator->Play_Animation(L"CactusNeedle", true);

	_vec3 AxisY = { 0,1,0 };
	m_pTransformCom->RotationAxis(AxisY, D3DXToRadian(1.f));
	m_pTransformCom->Get_Info(INFO_UP, &m_vDir);
	m_pTransformCom->Move_Pos(&m_vDir, fTimeDelta, 5.f);
	if (m_fMoveTime < 0.f)
	{
		if (Is_Active())
			Set_Active(false);
		m_fMoveTime = 0.f;
	}
	m_fMoveTime -= 10.f * fTimeDelta;
	return iExit;
}

void CCactusNeedle::LateUpdate_Object(void)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	__super::LateUpdate_Object();


}

void CCactusNeedle::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CCactusNeedle::Add_Component(void)
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
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);
	return S_OK;
}


CCactusNeedle* CCactusNeedle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCactusNeedle* pInstance = new CCactusNeedle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("CactusNeedle Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CCactusNeedle::Free()
{
	__super::Free();
}
