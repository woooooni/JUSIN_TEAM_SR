#include "Export_Function.h"
#include "GolemLowerJaw.h"
#include "SunGollem.h"

CGolemLowerJaw::CGolemLowerJaw(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)


{
}
CGolemLowerJaw::CGolemLowerJaw(const CGolemLowerJaw& rhs)
	: CGolemPart(rhs)

{

}

CGolemLowerJaw::~CGolemLowerJaw()
{
}

HRESULT CGolemLowerJaw::Ready_Object(void)
{
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_LowerJaw", L"Proto_Texture_SunGolem_Idle_LowerJaw", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_LowerJaw", L"Proto_Texture_SunGolem_Dirty_LowerJaw", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_LowerJaw", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.6f * 1.5f, 0.2f * 2.f, 0.2f * 1.5f });
	Set_State(SUNGOLEM_STATE::REGEN);
	return S_OK;
}

_int CGolemLowerJaw::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	int iExit = __super::Update_Object(fTimeDelta);
	
	switch (m_eState)
	{
	case SUNGOLEM_STATE::IDLE:
		Update_Idle(fTimeDelta);
		break;
	case SUNGOLEM_STATE::MOVE:
		Update_Move(fTimeDelta);
		Generate_MotionTrail(fTimeDelta);
		break;
	case SUNGOLEM_STATE::REGEN:
		Update_Regen(fTimeDelta);
		Generate_MotionTrail(fTimeDelta);
		break;
	case SUNGOLEM_STATE::ATTACK:
		Update_Attack(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIE:
		Update_Die(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIRTY:
		Update_Dirty(fTimeDelta);
		break;
	}

	return iExit;
}

void CGolemLowerJaw::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemLowerJaw::Render_Object(void)
{
	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, 1.f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 50.0f);


	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);


	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
	
}

HRESULT CGolemLowerJaw::Add_Component(void)
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

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

void CGolemLowerJaw::Update_Idle(_float fTimeDelta)
{

	if (m_bExhale)
		Move_Offset(_vec3(0.f, -0.18f, -0.055f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
	


}

void CGolemLowerJaw::Update_Dirty(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_LowerJaw", true);
	switch (m_iIndex)
	{

	case 0:
		Move_Offset(_vec3(0.0f, -0.12f, m_vOffset.z), fTimeDelta, 2.f);

		break;
	case 1:
		Move_Offset(_vec3(0.f, -0.130805f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 2:
		Move_Offset(_vec3(0.f, -0.56f, m_vOffset.z), fTimeDelta, 2.f);
		break;


	default:
		break;
	}
}

void CGolemLowerJaw::Update_Move(_float fTimeDelta)
{
	if (m_bExhale)
		Move_Offset(_vec3(0.f, -0.18f, -0.055f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}

}

void CGolemLowerJaw::Update_Attack(_float fTimeDelta)
{
	if (m_bExhale)
		Move_Offset(_vec3(0.f, -0.18f, -0.055f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}

}

void CGolemLowerJaw::Update_Die(_float fTimeDelta)
{
	_vec3 vOffset = m_vOffset;
	switch (m_iIndex)
	{

	case 0:
		Move_Offset(vOffset, fTimeDelta, 2.f);
		break;
	case 1:
		vOffset.y = -0.5f;
		Move_Offset(vOffset, fTimeDelta, 2.f);
		break;
	case 2:
		vOffset.y = -0.7;
		Move_Offset(vOffset, fTimeDelta, 2.f);
		break;
	case 3:
		vOffset.y = -0.9f;
		Move_Offset(vOffset, fTimeDelta, 2.f);
		break;
	case 4:
		vOffset.y = -1.2;
		Move_Offset(vOffset, fTimeDelta, 2.f); 
		break;
	case 5:
		vOffset.y = -1.5;
		Move_Offset(vOffset, fTimeDelta, 2.f);	
		break;


	default:
		break;
	}
}

void CGolemLowerJaw::Update_Regen(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		m_vOffset = { -0.f, - 0.6f, -0.055f };

		break;
	case 1:
		Move_Offset(_vec3(0.f, - 0.6f, m_vOffset.z), fTimeDelta, 2.f);

		break;
	case 2:
		Move_Offset(_vec3(0.f, -0.5f, m_vOffset.z), fTimeDelta, 2.f);
		m_fRotationAngle = 0.f;
		break;
	case 3:
		Move_Offset(_vec3(0.0f, -0.12f, m_vOffset.z), fTimeDelta, 2.f);

		break;
	case 4:
		Move_Offset(_vec3(0.f, -0.130805f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 5:
		Move_Offset(_vec3(0.f, -0.56f, m_vOffset.z), fTimeDelta, 2.f);
		break;


	default:
		break;
	}
}
CGolemLowerJaw* CGolemLowerJaw::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemLowerJaw* pInstance = new CGolemLowerJaw(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemLowerJaw Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemLowerJaw::Free()
{
	__super::Free();
}
