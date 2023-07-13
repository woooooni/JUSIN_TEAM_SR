
#include "SilkWorm.h"
#include "BugBall.h"
#include "Export_Function.h"
#include "RedBeatle.h"
#include "BlueBeatle.h"
#include "GreenBeatle.h"
CSilkWorm::CSilkWorm(LPDIRECT3DDEVICE9 pGraphicDev) : Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER, OBJ_ID::SILK_WORM)
{
}

CSilkWorm::CSilkWorm(const CSilkWorm& rhs) : CGameObject(rhs)
{
}

CSilkWorm::~CSilkWorm()
{
}


HRESULT CSilkWorm::Ready_Object(void)
{
	m_fMoveTime = 0.f;

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"BugBoss_Phase1_Idle", L"Proto_Texture_BugBoss_Phase1_Idle", 0.15f);
	m_pAnimator->Add_Animation(L"BugBoss_Phase1_Regen", L"Proto_Texture_BugBoss_Phase1_Regen", 0.15f);
	m_pAnimator->Add_Animation(L"BugBoss_Phase1_Ready", L"Proto_Texture_BugBoss_Phase1_Ready", 0.1f);
	m_pAnimator->Add_Animation(L"BugBoss_Phase1_Attack", L"Proto_Texture_BugBoss_Phase1_Attack", 0.15f);
	m_pAnimator->Add_Animation(L"BugBoss_Phase2_Death", L"Proto_Texture_BugBoss_Phase2_Death", 0.1f);
	m_pAnimator->Add_Animation(L"BugBoss_Phase2_Regen", L"Proto_Texture_BugBoss_Phase2_Regen", 0.1f);
	m_pAnimator->Add_Animation(L"BugBoss_Phase2_Down", L"Proto_Texture_BugBoss_Phase2_Down", 0.1f);
	m_pAnimator->Add_Animation(L"BugBoss_Phase2_Attack", L"Proto_Texture_BugBoss_Phase2_Attack", 0.2f);

	m_pTransformCom->Set_Scale({ 3,3,3 });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({3.f, 3.f, 3.f });
	m_pTransformCom->Set_Info(INFO_POS, &_vec3(10.0f, 2.0f, 10.f));
	m_bPhase2 = true;
	m_pAnimator->Play_Animation(L"BugBoss_Phase2_Regen", false);
	Set_State(SILKWORM_STATE::IDLE);
	_int iInterval =5;
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_vOrigin = vPos;
	m_vRandomPos[0] = { vPos.x -iInterval, vPos.y, vPos.z - iInterval };
	m_vRandomPos[1] = { vPos.x -iInterval, vPos.y, vPos.z };
	m_vRandomPos[2] = { vPos.x -iInterval, vPos.y, vPos.z + iInterval };
	m_vRandomPos[3] = { vPos.x +iInterval, vPos.y, vPos.z + iInterval };
	m_vRandomPos[4] = { vPos.x +iInterval, vPos.y, vPos.z };
	m_vRandomPos[5] = { vPos.x +iInterval, vPos.y, vPos.z - iInterval };
	m_vRandomPos[6] = { vPos.x , vPos.y, vPos.z + iInterval };
	m_vRandomPos[7] = { vPos.x , vPos.y, vPos.z - iInterval };
	m_fMinHeight = 2.0f;
	m_eCOLORPATTERN = COLOR_BLUE;
	m_tStat = { 25,25,1 };

	return S_OK;
}

_int CSilkWorm::Update_Object(const _float& fTimeDelta)
{

	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BOSS);
	switch (m_eState)
	{
	case Engine::SILKWORM_STATE::REGEN:	
		Update_Regen(fTimeDelta);
		break;
	case Engine::SILKWORM_STATE::IDLE:
		Update_Idle(fTimeDelta);
		break;
	case Engine::SILKWORM_STATE::READY:
		Update_Ready(fTimeDelta);
		break;
	case Engine::SILKWORM_STATE::DOWN:
		Update_Down(fTimeDelta);
		break;
	case Engine::SILKWORM_STATE::ATTACK:
		Update_Attack(fTimeDelta);
		break;
	case Engine::SILKWORM_STATE::DIE:
		Update_Die(fTimeDelta);
		break;
	}
	if (m_bSpawn)
		for (int i = 0; i < COLOR_END; i++)
			if (m_pBeatles[i]->Is_Active())
			m_pBeatles[i]->Update_Object(fTimeDelta);
	return iExit;
}
void CSilkWorm::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();	
	if (m_bSpawn)
		for (int i = 0; i < COLOR_END; i++)
			if (m_pBeatles[i]->Is_Active())
				m_pBeatles[i]->LateUpdate_Object();
}
void CSilkWorm::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	

	__super::Render_Object();
	m_pBufferCom->Render_Buffer();
	if (m_bSpawn)
		for (int i = 0; i < COLOR_END; i++)
			if (m_pBeatles[i]->Is_Active())
			m_pBeatles[i]->Render_Object();
	
}


void CSilkWorm::Update_Idle(_float fTimeDelta)
{
	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 4)
			Set_State(SILKWORM_STATE::READY);
		m_fMoveTime = 0.f;
	}
	
	m_fMoveTime += 10.f * fTimeDelta;
}

void CSilkWorm::Update_Die(_float fTimeDelta)
{
	m_fMoveTime += 20.f * fTimeDelta;
	if (m_fMoveTime > 100.f)
	{
		if (Is_Active())
			Set_Active(false);
	}
	if ((int)(m_fMoveTime*0.1f)%2==0)
	{
		m_pAnimator->GetCurrAnimation()->Set_Idx(2);
	}
	else
		m_pAnimator->GetCurrAnimation()->Set_Idx(3);
}

void CSilkWorm::Update_Regen(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		if (m_bPhase2)
		{
			
			m_pAnimator->Play_Animation(L"BugBoss_Phase2_Attack", false);
			Set_State(SILKWORM_STATE::ATTACK);
		}
		else
		{
			m_pAnimator->Play_Animation(L"BugBoss_Phase1_Idle", true);
			Set_State(SILKWORM_STATE::IDLE);
		}
	}
}

void CSilkWorm::Update_Ready(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
			m_pAnimator->Play_Animation(L"BugBoss_Phase1_Attack", false);
			Set_State(SILKWORM_STATE::ATTACK);
	}
}


void CSilkWorm::Update_Attack(_float fTimeDelta)
{
	
	if (!m_bPhase2)
	{
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 3 && m_bShoot)
		{
			_float fAngle = 18.f;
			_matrix matRotationY;
			_vec3 vDir = { 1,0,0 };
			D3DXMatrixRotationY(&matRotationY, D3DXToRadian(-fAngle));
			D3DXVec3TransformNormal(&vDir, &vDir, &matRotationY);
			for (int i = 0; i < 10; i++)
			{
				D3DXMatrixRotationY(&matRotationY, D3DXToRadian(fAngle));
				D3DXVec3TransformNormal(&vDir, &vDir, &matRotationY);
				CBugBall* pBugBall = CBugBall::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pBugBall, );
				_vec3 BulletPos;
				m_pTransformCom->Get_Info(INFO_POS, &BulletPos);
				BulletPos.y -= 0.5f;
				pBugBall->Get_TransformCom()->Set_Pos(&BulletPos);
				pBugBall->Set_Dir(vDir);
				pBugBall->Set_Shooter(this);
				pBugBall->Set_Atk(m_tStat.iAttack);
				CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
				pLayer->Add_GameObject(L"BugBall", pBugBall);
				m_bShoot = false;
			}
		}
		if (m_pAnimator->GetCurrAnimation()->Is_Finished())
		{
				m_pAnimator->Play_Animation(L"BugBoss_Phase1_Idle", true);
				Set_State(SILKWORM_STATE::IDLE);
				m_bShoot = true;
		}
	}
	else
	{

		if (m_pAnimator->GetCurrAnimation()->Get_Idx()>2)
		{
			Trace(fTimeDelta);
		}
		else
		{
			if (!m_bSpawn)
			{
				for (int i = 0; i < COLOR_END; i++)
					m_pBeatles[i]=nullptr;
				_vec3 vRedPos, vGreenPos, vBluePos;
				vRedPos = { m_vOrigin.x + float(rand() % 6) - 3.f,
				m_vOrigin.y + float(rand() % 6) - 3.f,m_vOrigin.z + float(rand() % 6) - 3.f };
				vGreenPos = { m_vOrigin.x + float(rand() % 6) - 3.f,
				m_vOrigin.y + float(rand() % 6) - 3.f,m_vOrigin.z + float(rand() % 6) - 3.f };
				vBluePos = { m_vOrigin.x + float(rand() % 6) - 3.f,
				m_vOrigin.y + float(rand() % 6) - 3.f,m_vOrigin.z + float(rand() % 6) - 3.f };
				CRedBeatle* pRedBeatle = CRedBeatle::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pRedBeatle, );
				pRedBeatle->Get_TransformCom()->Set_Pos(&vRedPos);
				CBlueBeatle* pBlueBeatle = CBlueBeatle::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pBlueBeatle, );
				pBlueBeatle->Get_TransformCom()->Set_Pos(&vBluePos);
				CGreenBeatle* pGreenBeatle = CGreenBeatle::Create(m_pGraphicDev);
				NULL_CHECK_RETURN(pGreenBeatle, );
				pGreenBeatle->Get_TransformCom()->Set_Pos(&vGreenPos);
				m_pBeatles[COLOR_RED]= pRedBeatle;
				m_pBeatles[COLOR_BLUE] = pBlueBeatle;
				m_pBeatles[COLOR_GREEN] = pGreenBeatle;
				m_bSpawn = true;
			}
			_vec3 vTargetPos, vPos, vDir;
			CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
			NULL_CHECK_RETURN(pTarget, );
			m_pTarget = pTarget;
			m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vDir = vTargetPos - vPos;
			m_vDir = vTargetPos - vPos;
		}
	}
}
void CSilkWorm::Update_Down(_float fTimeDelta)
{
	if (m_iHit > 6)
	{
	
		m_fMoveTime += 4.f*fTimeDelta;
		m_pAnimator->GetCurrAnimation()->Set_Idx(6-int(m_fMoveTime));
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 0)
		{
			m_fMoveTime = 0.f;
			m_pAnimator->Play_Animation(L"BugBoss_Phase2_Attack", false);
			Set_State(SILKWORM_STATE::ATTACK);
			m_pTransformCom->Set_Pos(&m_vOrigin);
			m_iHit = 0;
		}
	}
}
HRESULT CSilkWorm::Add_Component(void)
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

	return S_OK;
}

CSilkWorm* CSilkWorm::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSilkWorm* pInstance = new CSilkWorm(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SilkWorm Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CSilkWorm::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;
	vDir = m_vDir;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, m_fSpeed*2.f);

	if (m_fMoveTime > 20.f)
	{
		m_pTransformCom->Set_Pos(&m_vRandomPos[rand() % 8]);

		m_fMoveTime = 0.f;
		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pAnimator->GetCurrAnimation()->Set_Idx(0);
		m_pAnimator->Play_Animation(L"BugBoss_Phase2_Attack", false);
		
		vDir = vTargetPos - vPos;
		m_vDir = vTargetPos - vPos;
	}

	if (false==(m_pBeatles[ m_eCOLORPATTERN]->Is_Active()))
	{
		Set_State(SILKWORM_STATE::DOWN);
		m_pAnimator->Play_Animation(L"BugBoss_Phase2_Down", false);
		for (int i = 0; i < COLOR_END; i++)
		{
		
			if(m_pBeatles[i]->Is_Active())
			m_pBeatles[i]->Set_Active(false);
		}
		m_bSpawn = false;
		if (m_eCOLORPATTERN == COLOR_RED)
			m_eCOLORPATTERN = COLOR_GREEN;
		else if (m_eCOLORPATTERN == COLOR_BLUE)
			m_eCOLORPATTERN = COLOR_GREEN;
		else if (m_eCOLORPATTERN == COLOR_GREEN)
			m_eCOLORPATTERN = COLOR_RED;
	}
	m_fMoveTime += 10.f * fTimeDelta;
	


}
void CSilkWorm::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
		if (m_eState == SILKWORM_STATE::DIE)
		return;
		

		if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BULLET && dynamic_cast<CBugBall*> (pCollider->GetOwner())->Get_Shooter()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
		{
			m_tStat.iHp -= 1;
			
			_vec3 vTargetPos;
			_vec3 vPos;
			_vec3 vDir;
			pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vDir = vPos - vTargetPos;
			vDir.y = 0.0f;
			D3DXVec3Normalize(&vDir, &vDir);
			
			
			if (m_tStat.iHp < 24.f && m_bPhase2 == false)
			{
				Set_State(SILKWORM_STATE::REGEN);
				m_bPhase2 = true;
				m_pAnimator->Play_Animation(L"BugBoss_Phase2_Regen", false);
				m_iHit = 0;
			}
		}
		if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING &&
			pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER && m_bPhase2&&
			m_eState == SILKWORM_STATE::DOWN)
		{

			m_iHit++;
			m_tStat.iHp -= 1;
			if (m_tStat.iHp < 1.f)
			{
				m_pAnimator->Play_Animation(L"BugBoss_Phase2_Death", true);
				Set_State(SILKWORM_STATE::DIE);
			}
		}
}