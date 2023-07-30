#include "Export_Function.h"
#include "GolemLeftLeg.h"
#include "SunGollem.h"


CGolemLeftLeg::CGolemLeftLeg(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)

{
}
CGolemLeftLeg::CGolemLeftLeg(const CGolemLeftLeg& rhs)
	: CGolemPart(rhs)
{

}

CGolemLeftLeg::~CGolemLeftLeg()
{
}

HRESULT CGolemLeftLeg::Ready_Object(void)
{
	//227 / 315
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_LeftLeg", L"Proto_Texture_SunGolem_Idle_LeftLeg", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_LeftLeg", L"Proto_Texture_SunGolem_Dirty_LeftLeg", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_LeftLeg", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.227f*3.f * 1.5f, 0.315f*3.f * 2.f,1.f * 1.5f });

	Set_State(SUNGOLEM_STATE::REGEN);
	return S_OK;
}

_int CGolemLeftLeg::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	switch (m_eState)
	{
	case SUNGOLEM_STATE::IDLE:
		Update_Idle(fTimeDelta);
		break;
	case SUNGOLEM_STATE::MOVE:
		Update_Move(fTimeDelta);
		break;
	case SUNGOLEM_STATE::REGEN:
		Update_Regen(fTimeDelta);
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

void CGolemLeftLeg::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemLeftLeg::Render_Object(void)
{
	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;


	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	MATERIAL.material.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	MATERIAL.material.Diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	MATERIAL.material.Power = 0.0f;

	pEffect->SetValue("g_Material", &MATERIAL.material, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	
}

HRESULT CGolemLeftLeg::Add_Component(void)
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

void CGolemLeftLeg::Update_Idle(_float fTimeDelta)
{
	

	if (m_bBreath)
		Move_Offset(_vec3(-0.9f,-1.8f, m_vOffset.z), fTimeDelta, 2.f);


	if (m_iIndex == 2)
	{
		if (m_bBreath)
			m_bBreath = false;
		else
			m_bBreath = true;
	}


}

void CGolemLeftLeg::Update_Dirty(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_LeftLeg", true);
	_vec3 vDir;
	if (m_bBreath)
		vDir = { 0.f,1.f ,0.f };
	else
		vDir = { 0.f,-1.f ,0.f };

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 0.05f);
	if (m_fMoveTime > 10.f)
	{
		if (m_bBreath)
			m_bBreath = false;
		else
			m_bBreath = true;
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CGolemLeftLeg::Update_Move(_float fTimeDelta)
{
}

void CGolemLeftLeg::Update_Attack(_float fTimeDelta)
{
}

void CGolemLeftLeg::Update_Die(_float fTimeDelta)
{
}

void CGolemLeftLeg::Update_Regen(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		m_vOffset.y = -1.5f;
		break;
	case 1:
		Move_Offset(_vec3(-0.9f, -2.f, m_vOffset.z),fTimeDelta, 2.f);
		break;
	case 2:
		Move_Offset(_vec3(-0.9f, -1.7f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 3:
		Move_Offset(_vec3(-1.f, -2.2f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 4:
		Move_Offset(_vec3(-0.983166f, -2.f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 5:
		Move_Offset(_vec3(-0.9f, -1.8f, m_vOffset.z), fTimeDelta, 2.f);
		break;


	default:
		break;
	}

	

}
CGolemLeftLeg* CGolemLeftLeg::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemLeftLeg* pInstance = new CGolemLeftLeg(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemLeftLeg Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemLeftLeg::Free()
{
	__super::Free();
}
