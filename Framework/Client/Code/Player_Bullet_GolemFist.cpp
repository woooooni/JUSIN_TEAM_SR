#include "Player_Bullet_GolemFist.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"
#include "Effect_GolemFist.h"
#include "Effect_Smoke.h"
#include "Particle_LargeStone.h"

CPlayer_Bullet_GolemFist::CPlayer_Bullet_GolemFist(LPDIRECT3DDEVICE9 pGraphicDev)
    : CBullet(pGraphicDev, OBJ_ID::PLAYER_SKILL),
	m_fStopTime(0.1f),
	m_fAccTime(0.0f)
{
}

CPlayer_Bullet_GolemFist::CPlayer_Bullet_GolemFist(const CPlayer_Bullet_GolemFist& rhs)
    : CBullet(rhs),
	m_fStopTime(rhs.m_fStopTime),
	m_fAccTime(rhs.m_fAccTime)
{
}

CPlayer_Bullet_GolemFist::~CPlayer_Bullet_GolemFist()
{
}

HRESULT CPlayer_Bullet_GolemFist::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);


	m_pTransformCom->Set_Scale(_vec3(1.2f, 1.5f, 0.8f));

	m_pAnimator->Add_Animation(L"Fist", L"Proto_Texture_SunGolem_Idle_Fist", 0.1f);

	m_pAnimator->Play_Animation(L"Fist", false);

	Set_Active(false);

	m_pRigidBodyCom->SetMaxVelocity(100.0f);

	m_fMinHeight = 0.70f;
	Set_Atk(5);

	return S_OK;
}

_int CPlayer_Bullet_GolemFist::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	if (m_pRigidBodyCom->IsGround())
	{
		m_fAccTime += fTimeDelta;
		if (!m_bEffect)
		{
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			CGameObject* pEffect = CPool<CEffect_GolemFist>::Get_Obj();
			if (!pEffect)
			{
				pEffect = CEffect_GolemFist::Create(m_pGraphicDev);
				pEffect->Ready_Object();
			}
			dynamic_cast<CEffect_GolemFist*>(pEffect)->Get_Effect(vPos, _vec3(3.0f, 3.0f, 3.0f));

			FistSmokeEffect();
			m_bEffect = true;

			CCamera* pCamera = dynamic_cast<CCamera*>(Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
			NULL_CHECK_RETURN(pCamera, E_FAIL);
			pCamera->CamShake(0.5f, 2.0f);
		}

		if (m_fAccTime > m_fStopTime)
		{
			m_fAccTime = 0.0f;
			Set_Active(false);
			m_fEffectTime = 0.05f;
			m_bEffect = false;
		}
	}
	else
	{
		m_fEffectTime -= fTimeDelta;
		if (m_fEffectTime < 0.0f)
		{
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vPos.y += 1.0f;

			for (_uint i = 0; 5 > i; ++i)
			{
				CGameObject* pEffect = CPool<CEffect_Smoke>::Get_Obj();

				if (!pEffect)
				{
					pEffect = CEffect_Smoke::Create(m_pGraphicDev);
					pEffect->Ready_Object();
				}
				dynamic_cast<CEffect_Smoke*>(pEffect)->Get_Effect(vPos, _vec3(1.5f, 1.5f, 1.5f));
			}
			m_fEffectTime = 0.05f;
		}
	}


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CPlayer_Bullet_GolemFist::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	
		

	__super::LateUpdate_Object();
}

void CPlayer_Bullet_GolemFist::Render_Object(void)
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
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
}

HRESULT CPlayer_Bullet_GolemFist::Ready_Component(void)
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

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

void CPlayer_Bullet_GolemFist::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CPlayer_Bullet_GolemFist::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CPlayer_Bullet_GolemFist::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

CPlayer_Bullet_GolemFist* CPlayer_Bullet_GolemFist::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Bullet_GolemFist* pInstance = new CPlayer_Bullet_GolemFist(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player_Bullet_GolemFist_Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Bullet_GolemFist::FistSmokeEffect()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	_vec3 vDir = { 0.0f, -0.4f, 1.0f };

	_float fAngle;
	for (_int i = 0; 10 > i; ++i)
	{
		_vec3 vEffectPos = vDir;
		fAngle = 36.0f * i;

		fAngle = D3DXToRadian(fAngle);

		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot, &_vec3(0.0f, 1.0f, 0.0f), fAngle);

		D3DXVec3TransformNormal(&vEffectPos, &vEffectPos, &matRot);
		vEffectPos += vPos;


		CGameObject* pEffect = CPool<CEffect_Smoke>::Get_Obj();
		if (!pEffect)
		{
			pEffect = CEffect_Smoke::Create(m_pGraphicDev);
			pEffect->Ready_Object();
		}
		dynamic_cast<CEffect_Smoke*>(pEffect)->Get_Effect(vEffectPos, _vec3(1.0f, 1.0f, 1.0f), 186, 132, 72);
	}


	CParticle_LargeStone::Get_Effect(vPos, _vec3(1.0f, 0.5f, 1.0f), 20);
}

void CPlayer_Bullet_GolemFist::Free()
{
	__super::Free();
}
