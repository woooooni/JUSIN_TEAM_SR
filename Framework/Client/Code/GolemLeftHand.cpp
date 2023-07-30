#include "Export_Function.h"
#include "GolemLeftHand.h"
#include "SunGollem.h"

CGolemLeftHand::CGolemLeftHand(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)


{
}
CGolemLeftHand::CGolemLeftHand(const CGolemLeftHand& rhs)
	: CGolemPart(rhs)
{

}

CGolemLeftHand::~CGolemLeftHand()
{
}

HRESULT CGolemLeftHand::Ready_Object(void)
{
	//227 / 315
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL); 
	m_pAnimator->Add_Animation(L"SunGolem_Idle_LeftHand", L"Proto_Texture_SunGolem_Idle_LeftHand", 0.2f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_LeftHand", L"Proto_Texture_SunGolem_Dirty_LeftHand", 0.2f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_BummerLeftHand", L"Proto_Texture_SunGolem_Dirty_BummerLeftHand", 0.2f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_LeftHand", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 1.f, 1.f ,1.f });

	Set_State(SUNGOLEM_STATE::REGEN);

	return S_OK;
}

_int CGolemLeftHand::Update_Object(const _float& fTimeDelta)
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

void CGolemLeftHand::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemLeftHand::Render_Object(void)
{
	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, m_fAlpha / 255.0f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 50.0f);


	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	MATERIAL.material.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	MATERIAL.material.Diffuse = { 0.1f, 0.1f, 0.1f, 1.0f };
	MATERIAL.material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	MATERIAL.material.Power = 0.0f;

	pEffect->SetValue("g_Material", &MATERIAL.material, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
}

HRESULT CGolemLeftHand::Add_Component(void)
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

void CGolemLeftHand::Update_Idle(_float fTimeDelta)
{
	if (m_bExhale)
	{
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-2.1f, -1.7f, -0.012f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 361)
				m_fRotationAngle += 43.1 * fTimeDelta*2.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.8f, -0.2f, -0.011f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 400.f)
				m_fRotationAngle += 27.f * fTimeDelta * 2.f;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.49f, 1.f, -0.01f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 400.f)
				m_fRotationAngle += 27.f * fTimeDelta * 2.f;
		}
	}
	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemLeftHand::Update_Dirty(_float fTimeDelta)
{
	if (m_iArmNum != 1)
		m_pAnimator->Play_Animation(L"SunGolem_Dirty_LeftHand", true);
	else {
		m_pTransformCom->Set_Scale({ 0.7f * 2.5f, 0.7f * 2.5f,0.7f * 2.5f });
		m_pAnimator->Play_Animation(L"SunGolem_Dirty_BummerLeftHand", true);
	}switch (m_iIndex)
	{
	case 0:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-1.439f, -1.65f, -0.013f), fTimeDelta, 2.f);
			m_fRotationAngle = 23.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.767f, -0.2f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > -35.f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.9f, 2.1f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > -67.f)
				m_fRotationAngle -= 55.f * fTimeDelta;
		}
		break;
	case 1:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-1.21797f, -1.45396f, -0.013f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 25.3339f)
				m_fRotationAngle += 35.f * fTimeDelta;
			else
				m_fRotationAngle = 5.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.0014f, -0.598564f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < -8.25457)
				m_fRotationAngle += 61.f * fTimeDelta;
			else
				m_fRotationAngle = -8.25457;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.07986f, 0.758278f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < -40.7589)
				m_fRotationAngle += 55.f * fTimeDelta;
			else
				m_fRotationAngle = -40.7589;
		}
		break;
	case 2:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-2.1f, -2.4f, -0.013f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 5.f)
				m_fRotationAngle -= 35.f * fTimeDelta;
			else
				m_fRotationAngle = 5.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-3.8f, 0.f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 299.2f - 360.f)
				m_fRotationAngle -= 61.f * fTimeDelta;
			else
				m_fRotationAngle = 299.2f - 360.f;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-3.29f, 2.794f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 266.2f - 360.f)
				m_fRotationAngle -= 55.f * fTimeDelta;
			else
				m_fRotationAngle = 266.2f - 360.f;
		}
		break;

	default:
		break;
	}
}

void CGolemLeftHand::Update_Move(_float fTimeDelta)
{
	if (m_bExhale)
	{
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-2.1f, -1.7f, -0.012f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 361.f)
				m_fRotationAngle += 43.1f * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.8f, -0.2f, -0.011f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 400.f)
				m_fRotationAngle += 27.f * 2.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.49f, 1.f, -0.01f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 400.f)
				m_fRotationAngle += 27.f * 2.f * fTimeDelta;
		}
	}
	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemLeftHand::Update_Attack(_float fTimeDelta)
{
	if (m_bExhale)
	{
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-2.1f, -1.7f, -0.012f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 361)
				m_fRotationAngle += 43.1 * fTimeDelta * 2.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.8f, -0.2f, -0.011f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 400.f)
				m_fRotationAngle += 27.f * fTimeDelta * 2.f;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.49f, 1.f, -0.01f), fTimeDelta, 3.f);
			if (m_fRotationAngle < 400.f)
				m_fRotationAngle += 27.f * fTimeDelta * 2.f;
		}
	}
	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemLeftHand::Update_Die(_float fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	if (vPos.y > 0.5f)
		m_vOffset.y -= 2.f * fTimeDelta;
	else
		m_fAlpha -= 100.f * fTimeDelta;
	if (m_fAlpha <= 0.f)
		Set_Active(false);
}

void CGolemLeftHand::Update_Regen(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		m_vOffset = { -0.9f,	 - 1.2f,	0.009f };
		m_fRotationAngle = 25.f;
		break;
	case 1:
		Move_Offset(_vec3(-0.9f, - 1.f, m_vOffset.z), fTimeDelta, 2.f);
		break;
	case 2:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-1.439f, -1.65f, -0.013f), fTimeDelta, 2.f);
			m_fRotationAngle = 23.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.767f, -0.2f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > -35.f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.9f, 2.1f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > -67.f)
				m_fRotationAngle -= 55.f * fTimeDelta;
		}
		break;
	case 3:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-1.439f, -1.65f, -0.013f), fTimeDelta, 2.f);
			m_fRotationAngle = 23.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.767f, -0.2f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > -35.f)
				m_fRotationAngle -= 35.f * fTimeDelta;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.9f, 2.1f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > -67.f)
				m_fRotationAngle -= 55.f * fTimeDelta;
		}
		break;
	case 4:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-1.21797f, -1.45396f, -0.013f), fTimeDelta, 2.f);
			if (m_fRotationAngle < 25.3339f)
				m_fRotationAngle += 35.f * fTimeDelta;
			else
				m_fRotationAngle = 5.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-2.0014f, -0.598564f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle < -8.25457)
				m_fRotationAngle += 61.f * fTimeDelta;
			else
				m_fRotationAngle = -8.25457;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-2.07986f, 0.758278f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle < -40.7589)
				m_fRotationAngle += 55.f * fTimeDelta;
			else
				m_fRotationAngle = -40.7589;
		}
		break;
	case 5:
		if (m_iArmNum == 0)
		{
			Move_Offset(_vec3(-2.1f, -2.4f, -0.013f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 5.f)
				m_fRotationAngle -= 35.f * fTimeDelta;
			else
				m_fRotationAngle = 5.f;
		}
		else if (m_iArmNum == 1)
		{
			Move_Offset(_vec3(-3.8f, 0.f, -0.012f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 299.2f-360.f)
				m_fRotationAngle -= 61.f * fTimeDelta;
			else
				m_fRotationAngle = 299.2f - 360.f;
		}
		else if (m_iArmNum == 2)
		{
			Move_Offset(_vec3(-3.29f, 2.794f, -0.011f), fTimeDelta, 2.f);
			if (m_fRotationAngle > 266.2f - 360.f)
				m_fRotationAngle -= 55.f * fTimeDelta;
			else
				m_fRotationAngle = 266.2f - 360.f;
		}
		break;

	default:
		break;
	}
}
CGolemLeftHand* CGolemLeftHand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemLeftHand* pInstance = new CGolemLeftHand(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemLeftHand Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemLeftHand::Free()
{
	__super::Free();
}
