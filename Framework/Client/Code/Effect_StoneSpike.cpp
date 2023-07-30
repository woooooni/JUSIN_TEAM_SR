#include "Effect_StoneSpike.h"
#include "Export_Function.h"

CEffect_StoneSpike::CEffect_StoneSpike(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CBullet(pGraphicDev,OBJ_ID::MONSTER_SKILL)
{
}
CEffect_StoneSpike::CEffect_StoneSpike(const CEffect_StoneSpike& rhs)
	: CBullet(rhs)
{

}

CEffect_StoneSpike::~CEffect_StoneSpike()
{
}

HRESULT CEffect_StoneSpike::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"StoneSpike", L"Proto_Texture_Effect_StoneSpike", 0.1f);
	m_pAnimator->Play_Animation(L"StoneSpike", true);

	m_pTransformCom->Set_Pos(&_vec3(-5.0f, -0.5f, 0.0f));
	m_pTransformCom->Set_Scale({ 0.5f, 0.5f, 0.5f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 1.f, 1.f, 1.f });
	Set_Active(true);
	return S_OK;
}

_int CEffect_StoneSpike::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	int iExit = __super::Update_Object(fTimeDelta);
	_vec3 vPos = {};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	if (vPos.y < 0.5f)
	{
		m_pTransformCom->Move_Pos(&_vec3(0, 1, 0), 1.f, fTimeDelta);
		if (vPos.y < 0.25f)
			m_pAnimator->GetCurrAnimation()->Set_Idx(0);
		else
		{
			m_pTransformCom->Set_Scale({1.0f, 1.0f, 1.0f });
			m_pAnimator->GetCurrAnimation()->Set_Idx(1);
		}
	}

	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_EFFECT);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		if (Is_Active())
			Set_Active(false);
	}

	return iExit;
}

void CEffect_StoneSpike::LateUpdate_Object(void)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	__super::LateUpdate_Object();


}

void CEffect_StoneSpike::Render_Object(void)
{

	if (Is_Active())
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
}

HRESULT CEffect_StoneSpike::Add_Component(void)
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


CEffect_StoneSpike* CEffect_StoneSpike::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_StoneSpike* pInstance = new CEffect_StoneSpike(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("StoneSpike Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CEffect_StoneSpike::Free()
{
	__super::Free();
}