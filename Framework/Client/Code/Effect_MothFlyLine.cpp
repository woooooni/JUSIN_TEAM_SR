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
	m_pTransformCom->Set_Scale({ 5.f,70.f, 5.f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 5.f,45.f, 5.f });
	_vec3 AxisX = { 1,0,0 };
	Set_ARGB(150, 100, 100, 255);
	m_pTransformCom->RotationAxis(AxisX, D3DXToRadian(90.f));
	Set_Active(true);

	m_tMaterial.Emissive = { 0.3f,0.3f,0.3f, 0.3f };
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

	_vec3 vPos, vDir;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = m_vDst - vPos;
	_vec3 vScale = m_pTransformCom->Get_Scale();
	m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDir,&vDir), vScale.y * 0.5f, fTimeDelta);

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

		__super::Render_Object();

		LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

		CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
		if (pCamera == nullptr)
			return;

		_vec3 vPos;
		pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		D3DVECTOR vCamPos = vPos;

		D3DCOLORVALUE vColor = { m_iRed / 255.0f, m_iGreen / 255.0f, m_iBlue / 255.0f, m_iAlpha / 255.0f };

		pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
		pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
		pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
		pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
		pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
		pEffect->SetFloat("g_AlphaRef", 0.0f);


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

void CEffect_MothFlyLine::Return_Pool(void)
{
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


	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

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