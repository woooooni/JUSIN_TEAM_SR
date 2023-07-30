#include "Effect_CatapultHit.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"

CEffect_CatapultHit::CEffect_CatapultHit(LPDIRECT3DDEVICE9 pGraphicDev)
    :CEffect(pGraphicDev)
{
}

CEffect_CatapultHit::CEffect_CatapultHit(const CEffect& rhs)
    : CEffect(rhs)
{
}

CEffect_CatapultHit::~CEffect_CatapultHit()
{
}

HRESULT CEffect_CatapultHit::Ready_Object(void)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pAnimator->Add_Animation(L"CatapultHit", L"Proto_Texture_Effect_CatapultHit", 0.05f);

    m_pAnimator->Play_Animation(L"CatapultHit", false);

    Set_Active(false);

    return S_OK;
}

_int CEffect_CatapultHit::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_Active(false);
		CPool<CEffect_CatapultHit>::Return_Obj(this);
	}


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_CatapultHit::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	Set_Billboard();

	__super::LateUpdate_Object();
}


void CEffect_CatapultHit::Render_Object(void)
{
	if (!Is_Active())
		return;

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

void CEffect_CatapultHit::Return_Pool(void)
{
	Set_Active(false);
	CPool<CEffect_CatapultHit>::Return_Obj(this);
}

CEffect_CatapultHit* CEffect_CatapultHit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_CatapultHit* pInstance = new CEffect_CatapultHit(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_CatapultHit Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_CatapultHit::Get_Effect(_vec3& _vPos, _vec3& _vScale)
{
	_vPos.z -= 0.001f;
	m_pTransformCom->Set_Pos(&_vPos);
	m_pTransformCom->Set_Scale(_vScale);
	m_pAnimator->GetCurrAnimation()->Set_Idx(0);
	m_pAnimator->GetCurrAnimation()->Set_Finished(false);
	Set_Active(true);
	Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"CatapultHit", this);
}

HRESULT CEffect_CatapultHit::Add_Component(void)
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

void CEffect_CatapultHit::Free()
{
	__super::Free();
}
