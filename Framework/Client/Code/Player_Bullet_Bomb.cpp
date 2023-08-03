#include "Player_Bullet_Bomb.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"
#include "Effect_Explosion.h"
#include "Effect_Smoke.h"


CPlayer_Bullet_Bomb::CPlayer_Bullet_Bomb(LPDIRECT3DDEVICE9 pGraphicDev)
	: CBullet(pGraphicDev, OBJ_ID::PLAYER_SKILL),
	m_fMovePower(0.0f)
{
}

CPlayer_Bullet_Bomb::CPlayer_Bullet_Bomb(const CPlayer_Bullet_Bomb& rhs)
	: CBullet(rhs),
	m_fMovePower(rhs.m_fMovePower)
{
}

CPlayer_Bullet_Bomb::~CPlayer_Bullet_Bomb()
{
}

HRESULT CPlayer_Bullet_Bomb::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);


	m_pTransformCom->Set_Scale(_vec3(0.5f, 0.5f, 0.0f));

	m_pAnimator->Add_Animation(L"Bomb_Up", L"Proto_Texture_Player_Skill_Bomb_Up", 0.05f);
	m_pAnimator->Add_Animation(L"Bomb_Down", L"Proto_Texture_Player_Skill_Bomb_Down", 0.05f);
	m_pAnimator->Add_Animation(L"Bomb_Left", L"Proto_Texture_Player_Skill_Bomb_Left", 0.05f);
	m_pAnimator->Add_Animation(L"Bomb_Right", L"Proto_Texture_Player_Skill_Bomb_Right", 0.05f);


	Set_Active(false);

	m_fMinHeight = 0.4f;

	m_pRigidBodyCom->SetGravity(false);
	m_fOriginMaxVel = m_fMaxVel = 10.0f;
	m_pRigidBodyCom->SetMaxVelocity(m_fMaxVel);
	m_pRigidBodyCom->SetFricCoeff(0.0f);
	m_pRigidBodyCom->SetMass(1.0f);
	Set_Atk(1);

	m_fBulletTime = 1.5f;

	m_fAccEffectTime = 0.0f;
	m_fEffectTime = 0.001f;

	return S_OK;
}

_int CPlayer_Bullet_Bomb::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;


	m_fAccTime += fTimeDelta;
	if (m_fAccTime > m_fBulletTime)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		CGameObject* pExplosion = CPool<CEffect_Explosion>::Get_Obj();
		if (pExplosion)
			dynamic_cast<CEffect_Explosion*>(pExplosion)->Get_Effect(vPos, _vec3(0.7f, 0.7f, 0.7f));
		else
		{
			pExplosion = dynamic_cast<CEffect_Explosion*>(pExplosion)->Create(Engine::Get_Device());
			if (pExplosion)
				dynamic_cast<CEffect_Explosion*>(pExplosion)->Get_Effect(vPos, _vec3(0.7f, 0.7f, 0.7f));
		}

		m_pRigidBodyCom->SetVelocity(_vec3(0.0f, 0.0f, 0.0f));

		CPool<CPlayer_Bullet_Bomb>::Return_Obj(this);
	}

	if (m_pTarget)
		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &m_vTargetPos);


	_vec3 vDir, vPos, vEffectPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = m_vTargetPos - vPos;
	vEffectPos = vPos;

	m_fAccEffectTime += fTimeDelta;
	if (m_fAccEffectTime > m_fEffectTime)
	{
		m_fAccEffectTime = 0.0f;
		CGameObject* pEffect = CPool<CEffect_Smoke>::Get_Obj();
		if (!pEffect)
		{
			pEffect = CEffect_Smoke::Create(m_pGraphicDev);
			pEffect->Ready_Object();
		}
		dynamic_cast<CEffect_Smoke*>(pEffect)->Get_Effect(vEffectPos, _vec3(0.2f, 0.2f, 0.2f), 255, 255, 255);
	}


	if (!m_pTarget || !m_pTarget->Is_Active())
	{
		m_pTarget = nullptr;
		if (D3DXVec3Length(&vDir) < 0.2f)
		{
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			CGameObject* pExplosion = CPool<CEffect_Explosion>::Get_Obj();
			if (pExplosion)
				dynamic_cast<CEffect_Explosion*>(pExplosion)->Get_Effect(vPos, _vec3(0.7f, 0.7f, 0.7f));
			else
			{
				pExplosion = dynamic_cast<CEffect_Explosion*>(pExplosion)->Create(Engine::Get_Device());
				if (pExplosion)
					dynamic_cast<CEffect_Explosion*>(pExplosion)->Get_Effect(vPos, _vec3(0.7f, 0.7f, 0.7f));
			}

			m_pRigidBodyCom->SetVelocity(_vec3(0.0f, 0.0f, 0.0f));

			CPool<CPlayer_Bullet_Bomb>::Return_Obj(this);

		}
	}
	

	if (D3DXVec3Length(&vDir) < 2.5f)
	{
		if (m_vScale.x < 0.5f)
		{
			m_vScale += _vec3(1.0f, 1.0f, 1.0f) * 0.05f;
			m_pTransformCom->Set_Scale(m_vScale);
		}
		
		if (m_fMaxVel > 5.0f)
		{
			m_fMaxVel -= 0.5f;
			m_pRigidBodyCom->SetMaxVelocity(m_fMaxVel);
		}
	}

	m_fMovePower += 0.5f;
	D3DXVec3Normalize(&vDir, &vDir);
	m_pRigidBodyCom->AddForce(vDir * m_fMovePower);
	_vec3 vVel = m_pRigidBodyCom->GetVelocity();


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CPlayer_Bullet_Bomb::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;


	



	__super::LateUpdate_Object();
}

void CPlayer_Bullet_Bomb::Render_Object(void)
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

HRESULT CPlayer_Bullet_Bomb::Ready_Component(void)
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

void CPlayer_Bullet_Bomb::Shoot(CGameObject* _pTarget, _vec3& _vDir, _float _fPower, _vec3& _vPos)
{
	m_fAccTime = 0.0f;
	m_pTarget = _pTarget;
	m_pTransformCom->Set_Pos(&_vPos);
	m_pRigidBodyCom->AddForce(_vDir * _fPower);

	_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &m_vTargetPos);

	_vec3 vDir = m_vTargetPos - _vPos;

	if (fabs(vDir.x) > fabs(vDir.z))
	{
		if (vDir.x > 0.0f)
		{
			m_eDir = OBJ_DIR::DIR_R;
		}
		else
		{
			m_eDir = OBJ_DIR::DIR_L;
		}
	}
	else if (fabs(vDir.x) < fabs(vDir.z))
	{
		if (vDir.z > 0.0f)
		{
			m_eDir = OBJ_DIR::DIR_U;
		}
		else
		{
			m_eDir = OBJ_DIR::DIR_D;
		}
	}
	else
	{
		if (vDir.z > 0.0f)
		{
			m_eDir = OBJ_DIR::DIR_U;
		}
		else
		{
			m_eDir = OBJ_DIR::DIR_D;
		}
	}

	switch (m_eDir)
	{
	case OBJ_DIR::DIR_U:
		m_pAnimator->Play_Animation(L"Bomb_Up",true);
		break;
	case OBJ_DIR::DIR_D:
		m_pAnimator->Play_Animation(L"Bomb_Down", true);
		break;
	case OBJ_DIR::DIR_L:
		m_pAnimator->Play_Animation(L"Bomb_Left", true);
		break;
	case OBJ_DIR::DIR_R:
		m_pAnimator->Play_Animation(L"Bomb_Right", true);
		break;
	}
	m_pAnimator->GetCurrAnimation()->Set_Idx(0);
	m_pAnimator->GetCurrAnimation()->Set_Finished(false);



	m_fMovePower = 0.0f;

	m_vScale = { 0.1f, 0.1f, 0.1f };
	m_pTransformCom->Set_Scale(m_vScale);

	m_fMaxVel = m_fOriginMaxVel;
	m_pRigidBodyCom->SetMaxVelocity(m_fMaxVel);
}

void CPlayer_Bullet_Bomb::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER )
		return;

	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_MONSTER)
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		CGameObject* pExplosion = CPool<CEffect_Explosion>::Get_Obj();


		vPos.x += ((rand() % 3) - 2) * 0.1f;
		vPos.y += ((rand() % 3) - 2) * 0.1f;

		if (pExplosion)
			dynamic_cast<CEffect_Explosion*>(pExplosion)->Get_Effect(vPos, _vec3(2.f, 2.f, 2.f));
		else
		{
			pExplosion = dynamic_cast<CEffect_Explosion*>(pExplosion)->Create(Engine::Get_Device());
			if (pExplosion)
				dynamic_cast<CEffect_Explosion*>(pExplosion)->Get_Effect(vPos, _vec3(2.f, 2.f, 2.f));
		}

		m_pRigidBodyCom->SetVelocity(_vec3(0.0f, 0.0f, 0.0f));

		Stop_Sound(CHANNELID::SOUND_SKILL);
		Play_Sound(L"SFX_550_Hat_Missile_Hit.wav", CHANNELID::SOUND_SKILL, 0.5f);

		CPool<CPlayer_Bullet_Bomb>::Return_Obj(this);
	}
}

void CPlayer_Bullet_Bomb::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CPlayer_Bullet_Bomb::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

CPlayer_Bullet_Bomb* CPlayer_Bullet_Bomb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer_Bullet_Bomb* pInstance = new CPlayer_Bullet_Bomb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player_Bullet_Bomb_Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CPlayer_Bullet_Bomb::Free()
{
	__super::Free();
}
