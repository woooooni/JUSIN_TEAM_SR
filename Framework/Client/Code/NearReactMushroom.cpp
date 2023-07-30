#include "NearReactMushroom.h"
#include	"Export_Function.h"

CNearReactMushroom::CNearReactMushroom(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID eID) : CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT,eID)
, m_bHide(false)
, m_pSubAnimator(nullptr)
{
}

CNearReactMushroom::CNearReactMushroom(const CNearReactMushroom& rhs) : CGameObject(rhs)
, m_bHide(rhs.m_bHide)
, m_pSubAnimator(rhs.m_pSubAnimator)
{
	m_pSubAnimator->AddRef();
}

CNearReactMushroom::~CNearReactMushroom()
{
}

HRESULT CNearReactMushroom::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	FAILED_CHECK(m_pAnimator->Add_Animation(L"Hide", L"Proto_Tex_LightMushroom_Hide", 0.05f));
	FAILED_CHECK(m_pAnimator->Add_Animation(L"Out", L"Proto_Tex_LightMushroom_Out", 0.05f));
	FAILED_CHECK(m_pSubAnimator->Add_Animation(L"Hide", L"Proto_Tex_LightMushroom_Effect_Hide", 0.05f));
	FAILED_CHECK(m_pSubAnimator->Add_Animation(L"Out", L"Proto_Tex_LightMushroom_Effect_Out", 0.05f));

	m_pAnimator->Play_Animation(L"Out", false);
	m_pAnimator->GetCurrAnimation()->Set_Idx(m_pAnimator->GetCurrAnimation()->Get_Size() - 1);

	m_pSubAnimator->Play_Animation(L"Out", false);
	m_pSubAnimator->GetCurrAnimation()->Set_Idx(m_pAnimator->GetCurrAnimation()->Get_Size() - 1);


	return S_OK;
}

_int CNearReactMushroom::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	_vec3 playerPos, myPos;

	Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player")->Get_TransformCom()->Get_Info(INFO_POS, &playerPos);
	m_pTransformCom->Get_Info(INFO_POS, &myPos);

	playerPos.y = 0;
	myPos.y = 0;

	_float	range = D3DXVec3Length(&(playerPos - myPos));

	if (range <= 3.f)
	{
		if (m_bHide == false)
		{
			m_pAnimator->Play_Animation(L"Hide", false);
			m_pSubAnimator->Play_Animation(L"Hide", false);
			m_bHide = true;

		}
	}
	else if (range > 3.f)
	{
		if (m_bHide == true)
		{
			m_pAnimator->Play_Animation(L"Out", false);
			m_pSubAnimator->Play_Animation(L"Out", false);
			m_bHide = false;
		}


	}

	_int max = m_pAnimator->GetCurrAnimation()->Get_Size() - 1;
	_int cur = m_pAnimator->GetCurrAnimation()->Get_Idx();
	_float mySize;
	if (m_bHide)
	{
		mySize = (float)(max - cur) / (float)max;
	}
	else
	{
		mySize = (float)cur / (float)max;
	}

	if (mySize != m_pTransformCom->Get_Scale().x)
	{
		if (mySize == 0.f)
			mySize = 0.01f;
		m_pTransformCom->Set_Scale({ mySize, mySize, 1 });
	}
	Set_MinHeight(mySize * 0.5f);

	m_pSubAnimator->Update_Component(fTimeDelta);
	return CGameObject::Update_Object(fTimeDelta);
}

void CNearReactMushroom::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNearReactMushroom::Render_Object(void)
{
	_matrix src = *m_pTransformCom->Get_WorldMatrix();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;


	pEffect->SetMatrix("g_WorldMatrix", &src);
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pSubAnimator->GetCurrAnimation()->Get_Texture(m_pSubAnimator->GetCurrAnimation()->Get_Idx());
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

	_vec3 front;
	m_pTransformCom->Get_Info(INFO_LOOK, &front);

	src._41 -= front.x * 0.01f;
	src._42 -= front.y * 0.01f;
	src._43 -= front.z * 0.01f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &src);
	__super::Render_Object();

	pEffect->SetMatrix("g_WorldMatrix", &src);
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	pTexture = m_pSubAnimator->GetCurrAnimation()->Get_Texture(m_pSubAnimator->GetCurrAnimation()->Get_Idx());
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

void CNearReactMushroom::Free()
{
	Safe_Release(m_pSubAnimator);
	__super::Free();
}

CNearReactMushroom* CNearReactMushroom::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 p_pos)
{
	CNearReactMushroom* ret = new CNearReactMushroom(pGraphicDev, OBJ_ID::NEAR_REACT_MUSHROOM);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("NearMush Create Failed");
		return nullptr;
	}

	ret->m_pTransformCom->Set_Pos(&_vec3( p_pos.x, 0.5f, p_pos.z ));

	return ret;
}

HRESULT CNearReactMushroom::Ready_Component(void)
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
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	m_pSubAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);

	return S_OK;

}
