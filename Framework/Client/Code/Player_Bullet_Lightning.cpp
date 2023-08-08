#include "Player_Bullet_Lightning.h"

#include "Export_Function.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"
#include "Effect_LightningGround.h"


CPlayer_Bullet_Lightning::CPlayer_Bullet_Lightning(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev, OBJ_ID::PLAYER_SKILL),
	m_fLightningTime(0.1f),
	m_fAccLightningTime(0.0f),
	m_fNextTime(0.2f),
	m_fAccNextTime(0.0f)
{

}

CPlayer_Bullet_Lightning::CPlayer_Bullet_Lightning(const CPlayer_Bullet_Lightning& rhs)
	: CBullet(rhs),
	m_fLightningTime(rhs.m_fLightningTime),
	m_fAccLightningTime(rhs.m_fAccLightningTime),
	m_fNextTime(rhs.m_fNextTime),
	m_fAccNextTime(rhs.m_fAccNextTime)
{
}

CPlayer_Bullet_Lightning::~CPlayer_Bullet_Lightning()
{
}

HRESULT CPlayer_Bullet_Lightning::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"Lightning", L"Proto_Texture_Player_Skill_Lightning", 0.1f);
	m_pAnimator->Play_Animation(L"Lightning", false);

	Set_Active(false);

	Set_Atk(1.f);

	m_pTransformCom->Set_Scale(_vec3(1.5f, 5.5f, 0.0f));

	m_vDir = { 0.0f, 0.0f, 0.0f };

	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(_vec3(1.5f, 7.0f, 1.5f));

	m_pColliderCom->Set_Active(false);

	return S_OK;
}

_int CPlayer_Bullet_Lightning::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 1)
		m_pColliderCom->Set_Active(true);

	if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 4)
		m_pColliderCom->Set_Active(false);


	if (m_pAnimator->GetCurrAnimation()->Is_Finished() && m_bFinished)
	{
		CPool<CPlayer_Bullet_Lightning>::Return_Obj(this);
	}

	

	if (m_iMax > m_iCurr && !m_bFinished)
	{
		if (m_fAccNextTime > m_fNextTime)
		{
			ShootNext();
		}
		else
			m_fAccNextTime += fTimeDelta;
	}
	else
		m_bFinished = true;



	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CPlayer_Bullet_Lightning::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	

	__super::LateUpdate_Object();
}

void CPlayer_Bullet_Lightning::Render_Object(void)
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



	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
}

HRESULT CPlayer_Bullet_Lightning::Ready_Component(void)
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

void CPlayer_Bullet_Lightning::Shoot(_vec3& _vPos, _vec3& _vDir, _uint _iMax, _uint _iCurr)
{
	m_pColliderCom->Set_Active(false);
	m_pTransformCom->Set_Pos(&_vPos);
	m_pAnimator->GetCurrAnimation()->Set_Idx(0);
	m_pAnimator->GetCurrAnimation()->Set_Finished(false);
	m_bFinished = false;
	m_vDir = _vDir;

	m_fAccNextTime = 0.0f;
	m_fAccLightningTime = 0.0f;

	m_iCurr = _iCurr + 1;
	m_iMax = _iMax;

	CGameObject* pGruond = CPool<CEffect_LightningGround>::Get_Obj();
	if (pGruond)
		dynamic_cast<CEffect_LightningGround*>(pGruond)->Get_Effect(_vPos, _vec3(2.0f, 2.0f, 2.0f));
	else
	{
		pGruond = dynamic_cast<CEffect_LightningGround*>(pGruond)->Create(Engine::Get_Device());
		if (pGruond)
			dynamic_cast<CEffect_LightningGround*>(pGruond)->Get_Effect(_vPos, _vec3(2.0f, 2.0f, 2.0f));
	}
}

HRESULT CPlayer_Bullet_Lightning::ShootNext()
{
	CGameObject* pLightning = CPool<CPlayer_Bullet_Lightning>::Get_Obj();

	if (!pLightning)
	{
		pLightning = Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pLightning, E_FAIL);
		pLightning->Set_Active(true);
	}
	dynamic_cast<CBullet*>(pLightning)->Set_Owner(m_pOwner);
	FAILED_CHECK_RETURN(Engine::Get_Layer(LAYER_TYPE::PLAYER)->Add_GameObject(L"Lightning", pLightning), E_FAIL);

	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos += m_vDir;
	dynamic_cast<CPlayer_Bullet_Lightning*>(pLightning)->Shoot(vPos,m_vDir, m_iMax, m_iCurr);

	m_bFinished = true;

	return 0;
}


void CPlayer_Bullet_Lightning::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CPlayer_Bullet_Lightning::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CPlayer_Bullet_Lightning::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

CPlayer_Bullet_Lightning* CPlayer_Bullet_Lightning::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Bullet_Lightning* pInstance = new CPlayer_Bullet_Lightning(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player_Bullet_Lightning_Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Bullet_Lightning::Free()
{
}
