#include "Export_Function.h"
#include "Effect_MotionTrail.h"
#include "EventMgr.h"
#include "Pool.h"
CEffect_MotionTrail::CEffect_MotionTrail(LPDIRECT3DDEVICE9 pDevice)
	: CEffect(pDevice)
	, m_pOwner(nullptr)
	, m_fDeleteTime(1.f)
	, m_fAccDelete(0.f)
{

}

CEffect_MotionTrail::~CEffect_MotionTrail()
{
}

HRESULT CEffect_MotionTrail::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	Set_Alpha(255);
	
	return S_OK;
}

_int CEffect_MotionTrail::Update_Object(const _float& fTimeDelta)
{
	m_fAccDelete += fTimeDelta;
	if (m_fAccDelete >= m_fDeleteTime)
	{
		m_fAccDelete = 0.f;
		Return_Pool();
		return S_OK;
	}

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	_int iResult = __super::Update_Object(fTimeDelta);
	m_fAlpha -= 20.f;
	

	

	return iResult;
}

void CEffect_MotionTrail::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CEffect_MotionTrail::Render_Object(void)
{

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
	pEffect->SetFloat("g_AlphaRef", 50.f);


	IDirect3DBaseTexture9* pTexture = m_pTex;
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
}

void CEffect_MotionTrail::Return_Pool(void)
{
	Set_Alpha(255);
	m_fAccDelete = 0.f;
	m_fAlpha = 255.f;
	CPool<CEffect_MotionTrail>::Return_Obj(this);
}

HRESULT CEffect_MotionTrail::Ready_Component(void)
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

CEffect_MotionTrail* CEffect_MotionTrail::Create(LPDIRECT3DDEVICE9 pDevice)
{
	
	CEffect_MotionTrail* pInstance = new CEffect_MotionTrail(pDevice);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_MotionTrail Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_MotionTrail::Free()
{
	__super::Free();
}
