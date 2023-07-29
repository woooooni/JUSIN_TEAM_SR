
#include "SilkWorm.h"
#include "BugBall.h"
#include "Export_Function.h"
#include "RedBeatle.h"
#include "BlueBeatle.h"
#include "GreenBeatle.h"
#include "GameMgr.h"
#include "TrashPrist.h"
#include "Effect_Smoke.h"
#include "Pool.h"
#include "Effect_MothFlyLine.h"
#include "NueFlower.h"

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

	m_pTransformCom->Set_Scale({ 2.5f,3.f,2.5f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 2.5f, 3.f, 2.5f });
	m_pTransformCom->Set_Info(INFO_POS, &_vec3(53.f, 1.5f, 26.f));
	m_bPhase2 = false;
	m_pAnimator->Play_Animation(L"BugBoss_Phase1_Idle", false);
	Set_State(SILKWORM_STATE::IDLE);
	 m_fiInterval = 45.f;
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_vOrigin = vPos;
	m_vRandomPos[0] = { vPos.x -m_fiInterval, vPos.y, vPos.z - m_fiInterval };
	m_vRandomPos[1] = { vPos.x -m_fiInterval, vPos.y, vPos.z };
	m_vRandomPos[2] = { vPos.x -m_fiInterval, vPos.y, vPos.z + m_fiInterval };
	m_vRandomPos[3] = { vPos.x +m_fiInterval, vPos.y, vPos.z + m_fiInterval };
	m_vRandomPos[4] = { vPos.x +m_fiInterval, vPos.y, vPos.z };
	m_vRandomPos[5] = { vPos.x +m_fiInterval, vPos.y, vPos.z - m_fiInterval };
	m_vRandomPos[6] = { vPos.x , vPos.y, vPos.z + m_fiInterval };
	m_vRandomPos[7] = { vPos.x , vPos.y, vPos.z - m_fiInterval };
	m_fMinHeight = 1.5f;
	m_eCOLORPATTERN = COLOR_BLUE;
	m_tStat = { 25,25,1 };

	m_pUIBack = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_BACK);
	NULL_CHECK_RETURN(m_pUIBack, E_FAIL);
	m_pUIGauge = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_GAUGE);
	NULL_CHECK_RETURN(m_pUIGauge, E_FAIL);
	m_pUIFrame = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_FRAME);
	NULL_CHECK_RETURN(m_pUIFrame, E_FAIL);

	m_pUIGauge->Set_Name(BOSSNAME::SILKWORM);

	return S_OK;
}

_int CSilkWorm::Update_Object(const _float& fTimeDelta)
{

	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BOSS);
	_vec3 vTargetPos;
	
	if (m_tStat.iHp < 1.f)
	{
		m_pAnimator->Play_Animation(L"BugBoss_Phase2_Death", true);
		Set_State(SILKWORM_STATE::DIE);
	}
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
	{
		for (int i = 0; i < COLOR_END; i++)
		{
			if (nullptr != m_pBeatles[i] && m_pBeatles[i]->Is_Active())
			{
				_vec3 vPos, vDir = {};
				m_pBeatles[i]->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
				if (vPos.x < m_vRandomPos[0].x*0.7F || vPos.z < m_vRandomPos[0].z *0.7f || vPos.x > m_vRandomPos[3].x * 0.7f || vPos.z > m_vRandomPos[3].z * 0.7f)
				{
					vDir = m_vOrigin - vPos;
					m_pBeatles[i]->Get_TransformCom()->Move_Pos(D3DXVec3Normalize(&vDir, &vDir), 10.f, fTimeDelta);
				}
			}
		}
	}
	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Update_Object(fTimeDelta);
		m_pUIGauge->Update_Object(fTimeDelta);
		m_pUIFrame->Update_Object(fTimeDelta);
	}

	return iExit;
}
void CSilkWorm::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();	

	if (m_bSpawn)
		for (int i = 0; i < COLOR_END; i++)
		{		
		 if (nullptr != m_pBeatles[i] && !m_pBeatles[i]->Is_Active())
				m_pBeatles[i] = nullptr;
		}
	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->LateUpdate_Object();
		m_pUIGauge->LateUpdate_Object();
		m_pUIFrame->LateUpdate_Object();
	}

}

void CSilkWorm::Render_Object(void)
{
	if (Is_Active())
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


		__super::Render_Object();
		m_pBufferCom->Render_Buffer();
		if (m_bSpawn)
			for (int i = 0; i < COLOR_END; i++)
				if (nullptr != m_pBeatles[i] && m_pBeatles[i]->Is_Active())
					m_pBeatles[i]->Render_Object();
	}
	
	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Render_Object();
		m_pUIGauge->Render_Object();
		m_pUIFrame->Render_Object();
	}

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
	for (int i = 0; i < COLOR_END; i++)
	{

		if (nullptr != m_pBeatles[i] && m_pBeatles[i]->Is_Active())
			m_pBeatles[i]->Set_Active(false);
	}
}

void CSilkWorm::Update_Regen(_float fTimeDelta)
{
	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		if (m_bPhase2)
		{
			_vec3 vTargetPos, vPos, vDir;
			CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();
			if (nullptr == pTarget)
				return;
			m_pTarget = pTarget;
			m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vDir = vTargetPos - vPos;
			m_vDir = vTargetPos - vPos;
			Create_Line();
			Create_NueFlower();
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
	if (!m_bPhase2)
	{
		if (m_pAnimator->GetCurrAnimation()->Is_Finished())
		{
			m_pAnimator->Play_Animation(L"BugBoss_Phase1_Attack", false);
			Set_State(SILKWORM_STATE::ATTACK);
		}
	}
	else
	{
		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 0)
			m_pAnimator->GetCurrAnimation()->Set_Idx(2);
		if (m_pAnimator->GetCurrAnimation()->Is_Finished()&&vPos.y <= 1.5)
		{
			m_pAnimator->Play_Animation(L"BugBoss_Phase2_Attack", false);
			Set_State(SILKWORM_STATE::ATTACK);
			_vec3 vTargetPos, vPos, vDir;
			CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();
			if (nullptr == pTarget)
				return;
			m_pTarget = pTarget;
			m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vDir = vTargetPos - vPos;
			m_vDir = vTargetPos - vPos;
			Create_Line();
		}
		else
		{
			m_pTransformCom->Move_Pos(&_vec3(0, -1, 0), 5.f, fTimeDelta);
		}
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
				BulletPos.y = 0.5f;
				pBugBall->Get_TransformCom()->Set_Pos(&BulletPos);
				pBugBall->Set_Dir(vDir);
				pBugBall->Set_Owner(this);
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
	
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() ==2)
		{
			if (!m_bRotate[0])
			{
				_vec3  vUp, vCurrentRight;
				m_pTransformCom->Get_Info(INFO_RIGHT, &vCurrentRight);
				m_pTransformCom->RotationAxis(vCurrentRight, D3DXToRadian(30.f));
				m_bRotate[0] = true;

			}
		}
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 3)
		{
			if (!m_bRotate[1])
			{
				_vec3  vUp, vCurrentRight;
				m_pTransformCom->Get_Info(INFO_RIGHT, &vCurrentRight);
				m_pTransformCom->RotationAxis(vCurrentRight, D3DXToRadian(30.f));
				m_bRotate[1] = true;

			}
		}
		if (m_pAnimator->GetCurrAnimation()->Get_Idx()==4)
		{
			if (!m_bRotate[2])
			{
				_vec3  vUp, vCurrentRight,vLook;
				m_pTransformCom->Get_Info(INFO_RIGHT, &vCurrentRight);
				m_pTransformCom->RotationAxis(vCurrentRight,D3DXToRadian(30.f));
				m_bRotate[2] = true;
				m_pTransformCom->Get_Info(INFO_UP, &vUp);
				m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
				m_pTransformCom->Get_Info(INFO_RIGHT, &vCurrentRight);
				m_vDir.y = 0.f;
				float fAngle = acosf(D3DXVec3Dot(D3DXVec3Normalize(&m_vDir, &m_vDir), D3DXVec3Normalize(&vUp, &-vUp)));
				
				if (D3DXVec3Dot(&vCurrentRight, &m_vDir) > 0)
				{
					fAngle *= -1.f;
				}
				m_pTransformCom->RotationAxis(-vLook, fAngle);
			}	
			Trace(fTimeDelta);

		}
		else
		{
			if (!m_bSpawn)
			{
				for (int i = 0; i < COLOR_END; i++)
					m_pBeatles[i]=nullptr;
				_vec3 vRedPos, vGreenPos, vBluePos;
				vRedPos = (m_vOrigin + m_vRandomPos[rand() % 8]) * 0.5f;
				vGreenPos = (m_vOrigin + m_vRandomPos[rand() % 8]) * 0.5f;
				vBluePos = (m_vOrigin + m_vRandomPos[rand() % 8]) * 0.5f;
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
				CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::MONSTER);
				pLayer->Add_GameObject(L"RedBeatle", pRedBeatle);
				pLayer->Add_GameObject(L"BlueBeatle", pBlueBeatle);
				pLayer->Add_GameObject(L"GreenBeatle", pGreenBeatle);
				m_bSpawn = true;
			}
			
		}
	}
}
void CSilkWorm::Update_Down(_float fTimeDelta)
{
	if (m_iHit > 6)
	{
		if (m_pAnimator->GetCurrAnimation()->Get_Idx() == 0)
		{
			m_pAnimator->GetCurrAnimation()->Set_Idx(0);
			m_pTransformCom->Move_Pos(&_vec3(0.f,1.f,0.f) , 5.f, fTimeDelta);
			_vec3 vPos;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			if (vPos.y > 5.f)
			{
				m_fMoveTime = 0.f;
				m_pAnimator->Play_Animation(L"BugBoss_Phase2_Regen", true);
				Set_State(SILKWORM_STATE::READY);
				m_pTransformCom->Set_Pos(&_vec3(m_vOrigin.x, 5.f, m_vOrigin.z));
				m_iHit = 0;
			}
		}
		else
		{
			m_fMoveTime += 4.f * fTimeDelta;
			m_pAnimator->GetCurrAnimation()->Set_Idx(6 - int(m_fMoveTime));
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
	_vec3 vTargetPos, vPos, vDir,vEffectPos,vEffectDir;
	_matrix matRot;
	vDir = m_vDir;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	m_fMoveTime += fTimeDelta;
	m_fEffectCoolTime += fTimeDelta;
	float fAccel = 1.5f*m_fMoveTime* m_fMoveTime;
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 15.f* fAccel);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	if (m_fEffectCoolTime >= 0.3f)
	{
		for (int i = 0; i < 5; i++)
		{
			D3DXVec3TransformNormal(&vEffectDir, &vDir, D3DXMatrixRotationY(&matRot, D3DXToRadian(40.f - (float)i * 20.f)));
			vEffectPos = vPos - vEffectDir * 3.f;
			Create_Effect(vEffectPos);
		}
		m_fEffectCoolTime = 0.f;

	}
	if (vPos.x < m_vRandomPos[0].x|| vPos.z < m_vRandomPos[0].z|| vPos.x > m_vRandomPos[3].x || vPos.z > m_vRandomPos[3].z)
	{
		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		m_pAnimator->GetCurrAnimation()->Set_Idx(0);
		m_pAnimator->Play_Animation(L"BugBoss_Phase2_Attack", false);
		m_pTransformCom->Set_Pos(&m_vRandomPos[rand() % 8]);
		vDir = vTargetPos - vPos;
		m_vDir = vTargetPos - vPos;
		if (m_bRotate[2])
		{
			_vec3 vScale = m_pTransformCom->Get_Scale();
			m_pTransformCom->Set_Info(INFO_LOOK, &_vec3(0.f, 0.f, vScale.z));
			m_pTransformCom->Set_Info(INFO_UP, &_vec3(0.f, vScale.y, 0.f));
			m_pTransformCom->Set_Info(INFO_RIGHT, &_vec3(vScale.x, 0.f, 0.f));
			ZeroMemory(m_bRotate, sizeof (bool)* 3);
		}
		m_fMoveTime = 0.f;

		dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_Render(false);
		_vec3 vTargetPos, vPos, vDir;
		CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();
		if (nullptr == pTarget)
			return;
		m_pTarget = pTarget;
		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		vDir = vTargetPos - vPos;
		m_vDir = vTargetPos - vPos;
		Create_Line();
	}
	 if (nullptr == m_pBeatles[m_eCOLORPATTERN]&& (vPos.x < m_vRandomPos[0].x * 0.5f || vPos.z < m_vRandomPos[0].z * 0.5f || vPos.x > m_vRandomPos[3].x * 0.5f || vPos.z > m_vRandomPos[3].z * 0.5f))
	{
		 dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_Render(false);
		 if (m_bRotate[2])
		 {
			_vec3 vScale = m_pTransformCom->Get_Scale();
			m_pTransformCom->Set_Info(INFO_LOOK,& _vec3(0.f, 0.f, vScale.z));
			m_pTransformCom->Set_Info(INFO_UP, &_vec3(0.f, vScale.y, 0.f));
			m_pTransformCom->Set_Info(INFO_RIGHT, &_vec3(vScale.x, 0.f, 0.f));
			ZeroMemory(m_bRotate, sizeof(bool) * 3);
		}
		for (int i = 0; i < 2; i++)
		{
			CTrashPrist* pTrashPrist = CTrashPrist::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pTrashPrist, );
			vPos += float(rand() % 2+1) * (*D3DXVec3Normalize(& _vec3(),&m_vRandomPos[rand() % 8]));
			pTrashPrist->Get_TransformCom()->Set_Pos(&vPos);
			CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::MONSTER);
			pLayer->Add_GameObject(L"RedBeatle", pTrashPrist);
		}

		Set_State(SILKWORM_STATE::DOWN);
		m_fMoveTime = 0.f;
		m_pAnimator->Play_Animation(L"BugBoss_Phase2_Down", false);
		for (int i = 0; i < COLOR_END; i++)
		{
		
			if(nullptr != m_pBeatles[m_eCOLORPATTERN] && m_pBeatles[i]->Is_Active())
			m_pBeatles[i]->Set_Active(false);
		}
		m_bSpawn = false;
		if (m_eCOLORPATTERN == COLOR_RED)
		{
			dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_ARGB(150, 100, 255, 100);
			m_eCOLORPATTERN = COLOR_GREEN;
		}
		else if (m_eCOLORPATTERN == COLOR_BLUE)
		{
			m_eCOLORPATTERN = COLOR_GREEN;
			dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_ARGB(150, 100, 255, 100);
		}
			
		else if (m_eCOLORPATTERN == COLOR_GREEN)
		{
			dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_ARGB(150, 255, 100, 100);
			m_eCOLORPATTERN = COLOR_RED;
		}
		m_iHit = 0;
	}
}
void CSilkWorm::Create_Effect(_vec3 vPos )
{
	for (int i = 0; i < 6; i++)
	{
		CGameObject* pSmoke = CPool<CEffect_Smoke>::Get_Obj();
		if (pSmoke)
			dynamic_cast<CEffect_Smoke*>(pSmoke)->Get_Effect(vPos, _vec3(2.f, 2.f, 2.f), 148, 150, 148);
		else
		{
			pSmoke = dynamic_cast<CEffect_Smoke*>(pSmoke)->Create(Engine::Get_Device());
			if (pSmoke)
				dynamic_cast<CEffect_Smoke*>(pSmoke)->Get_Effect(vPos, _vec3(2.f, 2.f, 2.f), 148, 150, 148);
		}
	}
}

void CSilkWorm::Create_Line()
{
	if(m_pLine)
		dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_Render(true);
	else
	{
		m_pLine = CEffect_MothFlyLine::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pLine, );
		dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_Owner(this);
		CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::EFFECT);
		pLayer->Add_GameObject(L"Effect_MothFlyLine", m_pLine);
	}
	_vec3 vPos, vUp, vLook, vRight;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vPos.y = 0.09f;	
	m_pLine->Get_TransformCom()->Set_Pos(&vPos);
	_vec3 vScale  = m_pLine->Get_TransformCom()->Get_Scale();
	vScale.y = sqrtf(((m_fiInterval * 2.f)*(m_fiInterval * 2.f))*2.f);
	m_pLine->Get_TransformCom()->Set_Scale(vScale);
	m_vDir.y = 0.f;
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	vPos -= m_vDir * vScale.y*0.5f;
	m_pLine->Get_TransformCom()->Set_Pos(&vPos);
	vPos += m_vDir * vScale.y;
	dynamic_cast<CEffect_MothFlyLine*>(m_pLine)->Set_Dst(vPos);
	m_pLine->Get_TransformCom()->Get_Info(INFO_UP, &vUp);
	m_pLine->Get_TransformCom()->Get_Info(INFO_LOOK, &vLook);
	m_pLine->Get_TransformCom()->Get_Info(INFO_RIGHT, &vRight);

	float fAngle = acosf(D3DXVec3Dot(D3DXVec3Normalize(&m_vDir, &m_vDir), D3DXVec3Normalize(&vUp, &-vUp)));
	if (D3DXVec3Dot(&vRight, &m_vDir) > 0)
	{
		fAngle *= -1.f;
	}
	m_pLine->Get_TransformCom()->RotationAxis(-vLook, fAngle);

	
}

void CSilkWorm::Create_NueFlower()
{
	_vec3 vDir, vFlowerDir,vFlowerPos,vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	_matrix matRot;
	vDir = { 1.f,0.f,0.f };
	for (int i = 0; i < 6; i++)
	{
		
		D3DXVec3TransformNormal(&vFlowerDir, &vDir, D3DXMatrixRotationY(&matRot, D3DXToRadian((float)i * 60.f)));

		m_pNueFlower[i] = CNueFlower::Create(m_pGraphicDev, vPos, vFlowerDir,2.f,25.f);
		NULL_CHECK_RETURN(m_pNueFlower[i], );
		CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::EFFECT);
		pLayer->Add_GameObject(L"NueFlower", m_pNueFlower[i]);
	}
}

void CSilkWorm::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
		if (m_eState == SILKWORM_STATE::DIE)
		return;
		
		if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BULLET && dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Owner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
		{
			if (m_eState == SILKWORM_STATE::DOWN && m_bPhase2)
			{
				m_iHit += dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Atk();
				m_tStat.iHp -= dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Atk();
			}
			if (!m_bPhase2)
				m_tStat.iHp -= dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Atk();
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
	
		}
}