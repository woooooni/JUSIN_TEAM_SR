#include "Item_Hat.h"
#include "Export_Function.h"

CItem_Hat::CItem_Hat(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
    :CItem(pGraphicDev, ITEM_TYPE::EQUIPMENT, OBJ_ID::ITEM)
	, m_pPlayer(_pPlayer)
{
}

CItem_Hat::CItem_Hat(const CItem_Hat& rhs) : CItem(rhs), m_pPlayer(rhs.m_pPlayer)
{
	m_vecAnimation = rhs.m_vecAnimation;
	m_vecOffset = rhs.m_vecOffset;
	m_vScale = rhs.m_vScale;
	m_bLoop = rhs.m_bLoop;
	m_eSkill = rhs.m_eSkill;
}

CItem_Hat::~CItem_Hat()
{
}

void CItem_Hat::Free()
{
	__super::Free();
}

void CItem_Hat::Set_Hat(void)
{
	if (m_eDir != m_pPlayer->GetObj_Dir())
	{
		m_eDir = m_pPlayer->GetObj_Dir();
		m_pAnimator->Play_Animation(m_vecAnimation[(_uint)m_eDir],m_bLoop);
	}
}


HRESULT CItem_Hat::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CItem_Hat::Add_Component(), E_FAIL);
	
	
	m_vecAnimation.resize((_uint)OBJ_DIR::DIR_END, L"");
	m_vecOffset.resize((_uint)OBJ_DIR::DIR_END, { 0.0f,0.0f,0.0f });

	m_eDir = OBJ_DIR::DIR_END;

	

    return S_OK;
}

_int CItem_Hat::Update_Object(const _float& fTimeDelta)
{
	Set_Hat();

	_int Result = __super::Update_Object(fTimeDelta);


	return Result;
}

void CItem_Hat::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CItem_Hat::Render_Object(void)
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

HRESULT CItem_Hat::Add_Component(void)
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
