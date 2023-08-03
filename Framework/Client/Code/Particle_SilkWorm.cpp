#include "Particle_SilkWorm.h"

#include "Export_Function.h"
#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"
#include "GameMgr.h"


CParticle_SilkWorm::CParticle_SilkWorm(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev, OBJ_TYPE::OBJ_EFFECT, OBJ_ID::EFFECT)
{
}

CParticle_SilkWorm::CParticle_SilkWorm(const CParticle_SilkWorm& rhs)
    : CGameObject(rhs)
{
}

CParticle_SilkWorm::~CParticle_SilkWorm()
{
}

HRESULT CParticle_SilkWorm::Ready_Object(void)
{
    FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

    m_pTransformCom->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));
	m_pAnimator->Add_Animation(L"SilkWorm", L"Proto_Texture_Effect_SilkWorm", 0.1f);
	m_pAnimator->Play_Animation(L"SilkWorm", false);

	m_iR = m_iG = m_iB = 255;
	m_iAlpha = 255;

	m_fAccTime = 0.0f;
	m_fEndTime = 3.0f;
	m_fAccEffectTime = 0.0f;
	m_fEffectTime = 0.5f;

	Set_Active(true);

    return S_OK;
}

_int CParticle_SilkWorm::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	m_pTransformCom->Move_Pos(&m_vDir, 0.5f, fTimeDelta);

	Update_Move(fTimeDelta);

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CParticle_SilkWorm::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	__super::LateUpdate_Object();
}

void CParticle_SilkWorm::Render_Object(void)
{
	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	// 노란 계열 색상
	_float m_fR = 255.0f;
	_float m_fG = rand() % 26 + 230;
	_float m_fB = rand() % 72;
	_float m_fAlpha = 255.0f;

	D3DCOLORVALUE vColor = { m_fR, m_fG, m_fB, m_fAlpha };

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

void CParticle_SilkWorm::Random_Particle(_vec3& _vPos)
{
	m_fAccTime = 0.0f;

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);

	for (_uint i = 0; INFO_END > i; ++i)
	{
		_vec3 vInfo;
		memcpy(&vInfo, &matWorld.m[i][0], sizeof(_vec3));
		m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
	}

	_vPos.z += 0.1f;
	_float fScale = 0.1f + ((rand() % 10) * 0.05f);
	_float fAngle = _float(rand() % 360);

	m_pTransformCom->Set_Scale(_vec3(fScale, fScale, 0.0f));
	m_pTransformCom->Set_Pos(&_vPos);

//	_float fX = (rand() % 11) * 0.1f;
//	_float fY = (rand() % 11 + 1) * 0.1f;
//
//	_uint i = 
}

void CParticle_SilkWorm::Update_Move(const _float& fTimeDelta)
{
	if (m_fAccEffectTime > m_fEffectTime)
	{

	}
}

HRESULT CParticle_SilkWorm::Ready_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Effect_SilkWorm"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

CParticle_SilkWorm* CParticle_SilkWorm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticle_SilkWorm* pInstance = new CParticle_SilkWorm(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Particle SilkWorm Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CParticle_SilkWorm::Free()
{
    __super::Free();
}
