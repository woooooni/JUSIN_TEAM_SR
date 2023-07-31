#include "Effect_CircleBlur.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"


CEffect_CircleBlur::CEffect_CircleBlur(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)

{
}

CEffect_CircleBlur::CEffect_CircleBlur(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_CircleBlur::~CEffect_CircleBlur()
{
}

HRESULT CEffect_CircleBlur::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_Active(false);

	return S_OK;
}

_int CEffect_CircleBlur::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_CircleBlur::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;


	__super::LateUpdate_Object();
}

void CEffect_CircleBlur::Render_Object(void)
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

	D3DCOLORVALUE vColor = { m_iR / 255.0f, m_iG / 255.0f, m_iB / 255.0f, m_iAlpha / 255.0f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(0);
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

}

void CEffect_CircleBlur::Return_Pool(void)
{
	Set_Active(false);
	CPool<CEffect_CircleBlur>::Return_Obj(this);
}

CEffect_CircleBlur* CEffect_CircleBlur::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_CircleBlur* pInstance = new CEffect_CircleBlur(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Smoke Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_CircleBlur::Set_Effect(_vec3& _vPos, _vec3& _vScale, D3DCOLORVALUE& vColor, D3DCOLORVALUE& vEmissive)
{
	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	for (_uint i = 0; INFO_END > i; ++i)
	{
		_vec3 vInfo;
		memcpy(&vInfo, &matWorld.m[i][0], sizeof(_vec3));
		m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
	}

	m_pTransformCom->Set_Pos(&_vPos);
	m_pTransformCom->Set_Scale(_vScale);

	m_tMaterial.Emissive = vEmissive;

	Set_Color(vColor);

	Set_Active(true);
}

HRESULT CEffect_CircleBlur::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Effect_CircleBlur"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

void CEffect_CircleBlur::Free()
{
	__super::Free();
}
