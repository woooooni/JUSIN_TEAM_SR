#include "Export_Function.h"
#include "SludgeWave.h"
CSludgeWave::CSludgeWave(LPDIRECT3DDEVICE9 pGraphicDev) : CBullet(pGraphicDev,  OBJ_ID::MONSTER_SKILL)
{
}
CSludgeWave::CSludgeWave(const CSludgeWave& rhs)
	: CBullet(rhs)
{

}

CSludgeWave::~CSludgeWave()
{
}

HRESULT CSludgeWave::Ready_Object(void)
{
	//227 / 315
	m_fMoveTime = 10.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"SludgeWave", L"Proto_Texture_SludgeWave", 0.1f);
	m_pAnimator->Play_Animation(L"SludgeWave", true);
	_vec3 AxisX = { 1,0,0 };

	m_pTransformCom->RotationAxis(AxisX, D3DXToRadian(90.f));
	m_pTransformCom->Set_Pos(&_vec3(-2.0f,0.01f, -2.0f));	

	m_pTransformCom->Set_Scale({ 3.0f, 1.f, 1.f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 3.f, 1.f, 1.f });
	return S_OK;
}

_int CSludgeWave::Update_Object(const _float& fTimeDelta)
{
	if (Is_Active())
	{
		int iExit = __super::Update_Object(fTimeDelta);
		if (Is_Active())
		{
			Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
			if (m_fMoveTime < 9.9f && m_bDuplicate)
			{
				Create_Wave();
				m_bDuplicate = false;
			}
			if (m_fMoveTime < 5.f)
			{
				Set_Atk(0.f);
			}
			if (m_fMoveTime < 0.f)
			{
				if (Is_Active())
					Set_Active(false);
				m_fMoveTime = 0.f;
			}
			m_fMoveTime -= 5.f * fTimeDelta;
		}
		return iExit;
	}
	return S_OK;
}

void CSludgeWave::LateUpdate_Object(void)
{
	if (Is_Active())
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		__super::LateUpdate_Object();
	}
}

void CSludgeWave::Render_Object(void)
{
	if (Is_Active())
	{
		_int iAlpha = int(254.f * 0.1f * m_fMoveTime);

		__super::Render_Object();

		LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

		CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
		if (pCamera == nullptr)
			return;

		_vec3 vPos;
		pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		D3DVECTOR vCamPos = vPos;

		D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, iAlpha / 255.0f };

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
}

HRESULT CSludgeWave::Add_Component(void)
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


	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);


	return S_OK;
}




CSludgeWave* CSludgeWave::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSludgeWave* pInstance = new CSludgeWave(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SludgeWave Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CSludgeWave::Create_Wave()
{
	if (m_iWave == 0)
		return;
	_vec3 vPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.z -= 0.5f;
	vPos.y +=  0.001f;
	CSludgeWave* pSludgeWave = CSludgeWave::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSludgeWave, );
	pSludgeWave->Get_TransformCom()->Set_Pos(&vPos);
	pSludgeWave->Set_Atk(m_iAtk);
	pSludgeWave->Set_Wave(m_iWave-1);
	pSludgeWave->Set_Owner(m_pOwner);
	CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
	pLayer->Add_GameObject(L"SludgeWave", pSludgeWave);
}
void CSludgeWave::Free()
{
	__super::Free();
}
