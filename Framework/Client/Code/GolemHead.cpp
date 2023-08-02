#include "Export_Function.h"
#include "GolemHead.h"
#include "SunGollem.h"
#include "Pool.h"

CGolemHead::CGolemHead(LPDIRECT3DDEVICE9 pGraphicDev) : CGolemPart(pGraphicDev)


{
}
CGolemHead::CGolemHead(const CGolemHead& rhs)
	: CGolemPart(rhs)
{

}

CGolemHead::~CGolemHead()
{
}

HRESULT CGolemHead::Ready_Object(void)
{
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_Head", L"Proto_Texture_SunGolem_Idle_Head", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Head", L"Proto_Texture_SunGolem_Dirty_Head", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_Head", true);
	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 2.f * 1.5f,2.f * 2.f,2.f * 1.5f });

	Set_State(SUNGOLEM_STATE::REGEN);

	return S_OK;
}

_int CGolemHead::Update_Object(const _float& fTimeDelta)
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
		Generate_MotionTrail(fTimeDelta);
		break;
	}

	return iExit;
}

void CGolemHead::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CGolemHead::Render_Object(void)
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

HRESULT CGolemHead::Add_Component(void)
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

void CGolemHead::Update_Idle(_float fTimeDelta)
{
	_vec3 vDir;
	if (m_bExhale)
		Move_Offset(_vec3(0.f, 1.16633f,-0.05f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}


}

void CGolemHead::Update_Dirty(_float fTimeDelta)
{
	m_pAnimator->Play_Animation(L"SunGolem_Dirty_Head", true);
	switch (m_iIndex)
	{

		case 0:
			Move_Offset(_vec3(0.f, 1.2f, -0.05f), fTimeDelta, 2.f);
			break;
		case 1:
			Move_Offset(_vec3(0.f, 1.2f, -0.05f), fTimeDelta, 2.f);
			break;
		case 2:
			Move_Offset(_vec3(0.f, 1.16633f, -0.05f), fTimeDelta, 2.f);
			End_FirstEye();
			Set_SecondEye();
			break;

		default:
			break;
	}
}

void CGolemHead::Update_Move(_float fTimeDelta)
{
	if (m_bExhale)
		Move_Offset(_vec3(0.f, 1.16633f, -0.05f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemHead::Update_Attack(_float fTimeDelta)
{
	if (m_bExhale)
		Move_Offset(_vec3(0.f, 1.16633f, -0.05f), fTimeDelta, 3.f);


	if (m_iIndex == 2)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
			m_bExhale = true;
	}
}

void CGolemHead::Update_Die(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		Move_Offset(_vec3(0.f, 1.16633f, -0.05f), fTimeDelta, 3.f);
		End_SecondEye();
		break;
	case 1:
		Move_Offset(_vec3(0.f, 1.f, -0.05f), fTimeDelta, 2.f);
		break;
	case 2:
		Move_Offset(_vec3(0.f, 0.8f, -0.05f), fTimeDelta, 2.f);
		break;
	case 3:
		Move_Offset(_vec3(0.f, 0.6f, -0.05f), fTimeDelta, 2.f);

		break;
	case 4:
		Move_Offset(_vec3(0.f, 0.3f, -0.05f), fTimeDelta, 2.f);
		break;
	case 5:
		Move_Offset(_vec3(0.f, 0.f, -0.05f), fTimeDelta, 2.f);
		break;

	default:
		break;
	}
}

void CGolemHead::Update_Regen(_float fTimeDelta)
{
	switch (m_iIndex)
	{
	case 0:
		m_vOffset = { 0.f, 1.f,	-0.05f };
		break;
	case 1:
		Move_Offset(_vec3(0.f,	1.f, -0.05f), fTimeDelta, 2.f);
		Set_FirstEye();
		break;
	case 2:
		Move_Offset(_vec3(0.f, 1.f, -0.05f), fTimeDelta, 2.f);
		break;
	case 3:
		Move_Offset(_vec3(0.f, 1.2f, -0.05f), fTimeDelta, 2.f);

		break;
	case 4:
		Move_Offset(_vec3(0.f, 1.2f, -0.05f), fTimeDelta, 2.f);
		break;
	case 5:
		Move_Offset(_vec3(0.f, 1.16633f, -0.05f), fTimeDelta, 2.f);
		break;

	default:
		break;
	}
}
CGolemHead* CGolemHead::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemHead* pInstance = new CGolemHead(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemHead Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CGolemHead::Set_FirstEye()
{
	if (!m_pLeftEye)
	{
		m_pLeftEye = CPool<CEffect_EyeTrail>::Get_Obj();
		if (!m_pLeftEye)
		{
			m_pLeftEye = CEffect_EyeTrail::Create(m_pGraphicDev);
			m_pLeftEye->Ready_Object();
		}
		m_pLeftEye->Set_Effect(this, _vec3(-0.385f, -0.19f, -0.03f), 0.15f);
		m_pLeftEye->Set_Color(255, 250, 72, 125);
		Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"EyeTrail", m_pLeftEye);

	}
	
	if (!m_pRightEye)
	{
		m_pRightEye = CPool<CEffect_EyeTrail>::Get_Obj();
		if (!m_pRightEye)
		{
			m_pRightEye = CEffect_EyeTrail::Create(m_pGraphicDev);
			m_pRightEye->Ready_Object();
		}
		m_pRightEye->Set_Effect(this, _vec3(0.404f, -0.197f, -0.03f), 0.15f);
		m_pRightEye->Set_Color(255, 250, 72, 125);
		Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"EyeTrail", m_pRightEye);
	}
}

void CGolemHead::End_FirstEye()
{
	m_pRightEye->Set_End();
	m_pLeftEye->Set_End();

	m_pRightEye->Return_Pool();
	m_pLeftEye->Return_Pool();

	m_pRightEye = nullptr;
	m_pLeftEye = nullptr;

}

void CGolemHead::Set_SecondEye()
{
	if (!m_pLeftEye)
	{
		m_pLeftEye = CPool<CEffect_EyeTrail>::Get_Obj();
		if (!m_pLeftEye)
		{
			m_pLeftEye = CEffect_EyeTrail::Create(m_pGraphicDev);
			m_pLeftEye->Ready_Object();
		}
		m_pLeftEye->Set_Effect(this, _vec3(-0.38f, -0.185f, -0.15f), 0.15f);
		m_pLeftEye->Set_Color(255, 0, 0, 125);
		Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"EyeTrail", m_pLeftEye);

	}

	if (!m_pRightEye)
	{
		m_pRightEye = CPool<CEffect_EyeTrail>::Get_Obj();
		if (!m_pRightEye)
		{
			m_pRightEye = CEffect_EyeTrail::Create(m_pGraphicDev);
			m_pRightEye->Ready_Object();
		}
		m_pRightEye->Set_Effect(this, _vec3(0.445f, -0.19f, -0.15f), 0.15f);
		m_pRightEye->Set_Color(255, 0, 0, 125);
		Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"EyeTrail", m_pRightEye);
	}
}

void CGolemHead::End_SecondEye()
{
	if (m_pLeftEye && m_pRightEye)
	{
		m_pRightEye->Set_End();
		m_pLeftEye->Set_End();

		m_pRightEye = nullptr;
		m_pLeftEye = nullptr;
	}
}

void CGolemHead::Free()
{
	__super::Free();
}
