#include "Effect_MothFlyLine.h"
#include "Export_Function.h"

CEffect_MothFlyLine::CEffect_MothFlyLine(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CEffect(pGraphicDev)
{
}
CEffect_MothFlyLine::CEffect_MothFlyLine(const CEffect_MothFlyLine& rhs)
	: CEffect(rhs)
{

}

CEffect_MothFlyLine::~CEffect_MothFlyLine()
{
}

HRESULT CEffect_MothFlyLine::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"Effect_MothFlyLine", L"Proto_Texture_Effect_Effect_MothFlyLine", 0.1f);
	m_pAnimator->Play_Animation(L"Effect_MothFlyLine", false);

	m_pTransformCom->Set_Pos(&_vec3(-5.0f, 1.1f, 0.0f));
	m_pTransformCom->Set_Scale({ 5.f,50.f, 5.f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 5.f,45.f, 5.f });
	_vec3 AxisX = { 1,0,0 };
	Set_ARGB(150, 100, 100, 255);
	m_pTransformCom->RotationAxis(AxisX, D3DXToRadian(90.f));
	Set_Active(true);
	return S_OK;
}

_int CEffect_MothFlyLine::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK; 
	if (!m_bRender)
		return S_OK;
	int iExit = __super::Update_Object(fTimeDelta);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_EFFECT);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y = 0.09f;
	m_pTransformCom->Set_Pos(&vPos);
	return iExit;
}

void CEffect_MothFlyLine::LateUpdate_Object(void)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::LateUpdate_Object();


}

void CEffect_MothFlyLine::Render_Object(void)
{
	if (Is_Active())
	{
		if (!m_bRender)
			return;
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, m_iRed, m_iGreen, m_iBlue));

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
		__super::Render_Object();
		m_pBufferCom->Render_Buffer();
		m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

HRESULT CEffect_MothFlyLine::Add_Component(void)
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


CEffect_MothFlyLine* CEffect_MothFlyLine::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_MothFlyLine* pInstance = new CEffect_MothFlyLine(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_MothFlyLine Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CEffect_MothFlyLine::Free()
{
	__super::Free();
}