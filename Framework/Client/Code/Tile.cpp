#include "Tile.h"
#include "Export_Function.h"


CTile::CTile(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_TILE, OBJ_ID::TILE)
{

}
CTile::CTile(const CTile& rhs)
	: CGameObject(rhs)
{

}

CTile::~CTile()
{
}

HRESULT CTile::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pTransformCom->RotationAxis(_vec3(1.f, 0.f, 0.f), D3DXToRadian(90.f));

	_vec3 vPos(0.f, 0.005f, 0.f);
	m_pTransformCom->Set_Info(INFO_POS, &vPos);

	m_pColliderCom->SetEnable(false);

	return S_OK;
}

_int CTile::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CTile::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

}

void CTile::Render_Object(void)
{
	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, m_iAlpha / 255.0f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 90.f);


	IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(m_pTextureCom->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	MATERIAL.material.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	MATERIAL.material.Diffuse = { 0.1f, 0.1f, 0.1f, 1.0f };
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

HRESULT CTile::Ready_Component()
{
	m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_pBufferCom->SetOwner(this);
	m_mapComponent[ID_STATIC].insert({ COMPONENT_TYPE::COM_BUFFER, m_pBufferCom });

	m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);
	m_pTransformCom->SetOwner(this);
	m_mapComponent[ID_STATIC].insert({ COMPONENT_TYPE::COM_TRANSFORM, m_pTransformCom });

	m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Tile"));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_pTextureCom->SetOwner(this);
	m_mapComponent[ID_STATIC].insert({ COMPONENT_TYPE::COM_TEXTURE, m_pTextureCom });

	m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(m_pColliderCom, E_FAIL);
	m_pColliderCom->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].insert({ COMPONENT_TYPE::COM_BOX_COLLIDER, m_pColliderCom });

	m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	NULL_CHECK_RETURN(m_pShader, E_FAIL);
	m_pShader->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, m_pShader);

	return S_OK;
}

CTile* CTile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTile* pInstance = new CTile(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Tile Create Failed");
		return nullptr;
	}
	

	return pInstance;
}



void CTile::Free()
{
	__super::Free();
}
