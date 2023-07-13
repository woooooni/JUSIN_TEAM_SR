#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "SunGollem.h"
#include "GolemParts.h"
#include "PushStone.h"
CSunGollem::CSunGollem(LPDIRECT3DDEVICE9 pGraphicDev) 
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER, OBJ_ID::SUN_GOLLEM)
	, m_eState(SUNGOLEM_STATE::REGEN)
	, m_fSpeed(5.f)
{
}
CSunGollem::CSunGollem(const CSunGollem& rhs)
	: Engine::CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_eState(rhs.m_eState)
{

}

CSunGollem::~CSunGollem()
{
}


HRESULT CSunGollem::Ready_Object(void)
{
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_Body", L"Proto_Texture_SunGolem_Idle_Head", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Body", L"Proto_Texture_SunGolem_Dirty_Body", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_Body", true);

	memset(m_bAttack, 1, sizeof(bool)*6);
	m_vVerticalDir = { 0.f, 1.f ,0.f }; m_vVerticalDir = { 0.f, 1.f ,0.f };
	m_pTransformCom->Set_Pos(&_vec3(4.0f, 2.0f, 4.0f));
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_vRandomPos[0] = { vPos.x - 5, vPos.y, vPos.z };
	m_vRandomPos[1] = { vPos.x , vPos.y, vPos.z };
	m_vRandomPos[2] = { vPos.x + 5, vPos.y, vPos.z };
	m_pTransformCom->Set_Scale({ 2,2,2 });	
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({2.f, 2.f, 2.f });
	m_fSpeed = 5.f;
	Set_State(SUNGOLEM_STATE::REGEN);
	m_tStat = { 6,6,1 };
	FAILED_CHECK_RETURN(Ready_Parts(), E_FAIL);

	return S_OK;
}

_int CSunGollem::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BOSS);
	switch (m_eState)
	{
	case SUNGOLEM_STATE::IDLE:
		Update_Idle(fTimeDelta);
		break;
	case SUNGOLEM_STATE::MOVE:
		Update_Move(fTimeDelta);
		break;
	case SUNGOLEM_STATE::REGEN:
		Update_Regen(fTimeDelta);
		break;
	case SUNGOLEM_STATE::ATTACK:
		Update_Attack(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIE:
		Update_Die(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIRTY:
		Update_Dirty(fTimeDelta);
		break;
	}

	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
	{
		if ((*iter)->Is_Active())
		(*iter)->Update_Object(fTimeDelta);
	}
	return iExit;
}

void CSunGollem::LateUpdate_Object(void)
{
	
	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
	{
		if ((*iter)->Is_Active())
			(*iter)->LateUpdate_Object();
	}
}

void CSunGollem::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
	{
		if((*iter)->Is_Active())
		(*iter)->Render_Object(); 
	}
		
}

HRESULT CSunGollem::Add_Component(void)
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
	return S_OK;
}

void CSunGollem::Update_Idle(_float fTimeDelta)
{
	_vec3 vDir;
	if (m_bBreath)
		vDir = { 0.,1.f ,0.f };
	else
		vDir = { 0.f,-1.f ,0.f };

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 0.05f);

	if (m_fMoveTime > 10.f)
	{
		if (m_bBreath)
			m_bBreath = false;
		else
			m_bBreath = true;
	
		if(rand()%10>4)
			Set_State(SUNGOLEM_STATE::MOVE);
		else if(rand() % 10 < 5)
			Set_State(SUNGOLEM_STATE::ATTACK);
		if (m_tStat.iHp < 1)
		{
			if (!m_bDirty)
			{
				Set_State(SUNGOLEM_STATE::DIRTY);
				m_bDirty = true;
				m_pAnimator->Play_Animation(L"SunGolem_Dirty_Body", true);
				m_vecParts[FACE]->Set_Active(true);
			}
			else
			{
				Set_State(SUNGOLEM_STATE::DIE);
			}
		}
		m_fSpeed = 5.f;
		m_fMoveTime = 0.f;

	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CSunGollem::Update_Dirty(_float fTimeDelta)
{
	_vec3 vDir;

	if (m_bBreath)
		vDir = { 0.,1.f ,0.f };
	else
		vDir = { 0.f,-1.f ,0.f };

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 0.05f);
	m_tStat = { 8 ,8 ,2 };
	if (m_fMoveTime > 10.f)
	{
		if (m_bBreath)
			m_bBreath = false;
		else
			m_bBreath = true;
		Set_State(SUNGOLEM_STATE::IDLE);
		m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CSunGollem::Update_Move(_float fTimeDelta)
{

	

	m_fSpeed -= m_fMoveTime * m_fMoveTime*0.01f;


	_vec3 vPos;
	_vec3 vDir;

	m_pTransformCom->Move_Pos(&m_vVerticalDir, fTimeDelta, m_fSpeed);
	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
		(*iter)->Get_TransformCom()->Move_Pos(&m_vVerticalDir, fTimeDelta, m_fSpeed);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	
	vDir = m_vRandomPos[m_iRand] - vPos;
	vDir.y = 0.f;

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 1.f);
	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
		(*iter)->Get_TransformCom()->Move_Pos(&vDir, fTimeDelta, 1.f);

	if (vPos.y < 2.f&&m_fSpeed<0)
	{
		//Engine::CCameraMgr::GetInstance()->GetMainCamera()->CamShake(0.5f);
			m_vVerticalDir= {0.f, 1.f ,0.f };
			m_fSpeed = 15.f;
			m_iRand = rand() % 3 ;
		m_fMoveTime = 0.f;
		Set_State(SUNGOLEM_STATE::ATTACK);
	}
	m_fMoveTime += 10 * fTimeDelta;

}

void CSunGollem::Update_Attack(_float fTimeDelta)
{
	CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
	NULL_CHECK_RETURN(pTarget, );
	Set_Target(pTarget);
	_vec3 vTargetPos, vDir;
	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	if (m_bLockon == false)
	{
		m_vTargetPos = vTargetPos;
		m_bLockon = true;
	}
	else
	{
		vDir = vTargetPos - m_vTargetPos;
		D3DXVec3Normalize(&vDir, &vDir);
		m_vTargetPos += vDir * fTimeDelta * 5.f;
	}
	if (m_iActiveArm > 5)
	{
		m_vecParts[LEFTARM0]->Set_Active(false);
		m_vecParts[RIGHTARM0]->Set_Active(false);
		m_vecParts[LEFTHAND0]->Set_Active(false);
		m_vecParts[RIGHTHAND0]->Set_Active(false);
	}
	if (m_iActiveArm > 3)
	{
		m_vecParts[LEFTARM1]->Set_Active(false);
		m_vecParts[RIGHTARM1]->Set_Active(false);
		m_vecParts[LEFTHAND1]->Set_Active(false);
		m_vecParts[RIGHTHAND1]->Set_Active(false);
	}
	if (m_iActiveArm > 1)
	{
		m_vecParts[LEFTARM2]->Set_Active(false);
		m_vecParts[RIGHTARM2]->Set_Active(false);
		m_vecParts[LEFTHAND2]->Set_Active(false);
		m_vecParts[RIGHTHAND2]->Set_Active(false);
	}
	if (m_fMoveTime > 10.f)
	{

	
		if (m_iActiveArm > 1)
		{
			if (m_bAttack[0]&&m_fMoveTime > 10.f)
			{
				Create_Fist(false, 5);
				m_bAttack[0] = false;
			}
			if (m_bAttack[1] && m_fMoveTime > 15.f)
			{
				Create_Fist(false, 5);
				m_bAttack[1] = false;
			}
		}
			if (m_iActiveArm > 3)
			{
				if (m_bAttack[2] && m_fMoveTime > 20.f)
				{
					Create_Fist(true, 5);
					m_bAttack[2] = false;
				}
				if (m_bAttack[3] && m_fMoveTime > 25.f)
				{
					Create_Fist(true, 5);
					m_bAttack[3] = false;
				}
			}
			if (m_iActiveArm > 5 && m_fMoveTime > 30.f)
			{
				if (m_bAttack[4])
				{
					Create_Fist(false, 5) ;
					m_bAttack[4] = false;
				}
				if (m_bAttack[5] && m_fMoveTime > 35.f)
				{
					Create_Fist(false, 5);
					m_bAttack[5] = false;
				}
			}
			
		
		if (m_fMoveTime > 20.f +5.f* (float)m_iActiveArm)
		{
			m_fMoveTime = 0.f;
			Set_State(SUNGOLEM_STATE::IDLE);
			memset(m_bAttack, 1, sizeof(bool) * 6);
			m_iActiveArm += 2;
			m_bLockon = false;
			//m_tStat.iHp -= 2;
			if (m_iActiveArm > 5)
			{
				m_vecParts[LEFTARM0]->Set_Active(true);
				m_vecParts[RIGHTARM0]->Set_Active(true);
				m_vecParts[LEFTHAND0]->Set_Active(true);
				m_vecParts[RIGHTHAND0]->Set_Active(true);
			}
			if (m_iActiveArm > 3)
			{
				m_vecParts[LEFTARM1]->Set_Active(true);
				m_vecParts[RIGHTARM1]->Set_Active(true);
				m_vecParts[LEFTHAND1]->Set_Active(true);
				m_vecParts[RIGHTHAND1]->Set_Active(true);
			}
			if (m_iActiveArm > 1)
			{
				m_vecParts[LEFTARM2]->Set_Active(true);
				m_vecParts[RIGHTARM2]->Set_Active(true);
				m_vecParts[LEFTHAND2]->Set_Active(true);
				m_vecParts[RIGHTHAND2]->Set_Active(true);

			}
			if (m_iActiveArm > 6)
			{
				m_iActiveArm = 2;

			}
		}
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CSunGollem::Update_Die(_float fTimeDelta)
{
	for (auto iter = m_vecParts.begin(); iter != m_vecParts.end(); iter++)
	{
		if ((*iter)->Is_Active())
			(*iter)->Set_Active(false);
	}
	Set_Active(false);
}

void CSunGollem::Update_Regen(_float fTimeDelta)
{
	_vec3 vDir, vPos;
	vDir = { 0.f, 1.f * fTimeDelta, 0.f };
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	
	if (vPos.y > 2.f)
	{
		vPos.y = 2.f;
		m_pTransformCom->Set_Info(INFO_POS, &vPos);
		Set_State(SUNGOLEM_STATE::IDLE);
		m_fMoveTime = 0.f;
	}
}
CSunGollem* CSunGollem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSunGollem* pInstance = new CSunGollem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SunGollem Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CSunGollem::Free()
{
	__super::Free();
}
void CSunGollem::Create_Fist(bool _BummerFist, _int _iSrc)
{
	CGolemFist* pGolemFist = CGolemFist::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemFist, );
	m_vTargetPos.y = 7.f;
	pGolemFist->Get_TransformCom()->Set_Pos(&m_vTargetPos);
	pGolemFist->Set_Dirty(m_bDirty);
	pGolemFist->Set_Bummer(_BummerFist);
	pGolemFist->Set_Atk(m_tStat.iAttack);
	CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
	pLayer->Add_GameObject(L"GolemFist", pGolemFist);

}
HRESULT CSunGollem::Ready_Parts(void) 
{
	_vec3 vPos, vAxisZ;
	vAxisZ = {0.f,0.f,1.f};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_vPartPos[HEAD] = { vPos.x,vPos.y + 0.5f,vPos.z - 0.01f };
	m_vPartPos[UPPERJAW] = { vPos.x,vPos.y + 0.1f,vPos.z - 0.011f };
	m_vPartPos[LOWERJAW] = { vPos.x,vPos.y - 0.2f,vPos.z - 0.0105f };
	m_vPartPos[LEFTLEG] = { vPos.x-0.6f,vPos.y - 1.f,vPos.z - 0.01f };
	m_vPartPos[RIGHTLEG] = { vPos.x + 0.6f,vPos.y - 1.f,vPos.z - 0.01f };
	m_vPartPos[LEFTARM0] = { vPos.x - 1.5f,vPos.y - 0.5f,vPos.z - 0.0103f };
	m_vPartPos[LEFTARM1] = { vPos.x - 1.75f,vPos.y + 0.5f,vPos.z - 0.0102f };
	m_vPartPos[LEFTARM2] = { vPos.x - 1.5f,vPos.y + 1.5f,vPos.z - 0.0101f };
	m_vPartPos[RIGHTARM0] = { vPos.x + 1.5f,vPos.y - 0.5f,vPos.z - 0.0103f };
	m_vPartPos[RIGHTARM1] = { vPos.x + 1.75f,vPos.y + 0.5f,vPos.z - 0.0102f };
	m_vPartPos[RIGHTARM2] = { vPos.x + 1.5f,vPos.y + 1.5f,vPos.z - 0.0101f };
	m_vPartPos[LEFTHAND0] = { vPos.x - 1.9f,vPos.y - 0.5f,vPos.z - 0.0104f };
	m_vPartPos[LEFTHAND1] = { vPos.x - 2.15f,vPos.y + 0.5f,vPos.z - 0.0103f };
	m_vPartPos[LEFTHAND2] = { vPos.x - 1.9f,vPos.y + 1.5f,vPos.z - 0.0102f };
	m_vPartPos[RIGHTHAND0] = { vPos.x + 1.9f,vPos.y - 0.5f,vPos.z - 0.0104f };
	m_vPartPos[RIGHTHAND1] = { vPos.x + 2.15f,vPos.y + 0.5f,vPos.z - 0.0103f };
	m_vPartPos[RIGHTHAND2] = { vPos.x + 1.9f,vPos.y + 1.5f,vPos.z - 0.0102f };
	m_vPartPos[FACE] = { vPos.x,vPos.y + 0.5f,vPos.z - 0.012f };
	//	HEAD, LOWERJAW, UPPERJAW, LEFTLEG, RIGHTLEG, LEFTARM0, LEFTARM1,
	//	LEFTARM2, RIGHTARM0, RIGHTARM1, RIGHTARM2, LEFTHAND0,
	//	LEFTHAND1, LEFTHAND2, RIGHTHAND0, RIGHTHAND1, RIGHTHAND2
	m_vecParts.reserve(BODYPARTS::PARTSEND);
	CGolemHead* pGolemHead = CGolemHead::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemHead, E_FAIL);
	pGolemHead->Get_TransformCom()->Set_Pos(&m_vPartPos[HEAD]);
	m_vecParts.push_back(pGolemHead);
	CGolemUpperJaw* pGolemUpperJaw = CGolemUpperJaw::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemUpperJaw, E_FAIL);
	pGolemUpperJaw->Get_TransformCom()->Set_Pos(&m_vPartPos[UPPERJAW]);
	m_vecParts.push_back(pGolemUpperJaw);
	CGolemLowerJaw* pGolemLowerJaw = CGolemLowerJaw::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemLowerJaw, E_FAIL);
	pGolemLowerJaw->Get_TransformCom()->Set_Pos(&m_vPartPos[LOWERJAW]);
	m_vecParts.push_back(pGolemLowerJaw);
	CGolemLeftLeg* pGolemLeftLeg = CGolemLeftLeg::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemLeftLeg, E_FAIL);
	pGolemLeftLeg->Get_TransformCom()->Set_Pos(&m_vPartPos[LEFTLEG]);
	m_vecParts.push_back(pGolemLeftLeg);
	CGolemRightLeg* pGolemRightLeg = CGolemRightLeg::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemRightLeg, E_FAIL);
	pGolemRightLeg->Get_TransformCom()->Set_Pos(&m_vPartPos[RIGHTLEG]);
	m_vecParts.push_back(pGolemRightLeg);
	for (_int i = LEFTARM0; i < LEFTARM2+1; i++)
	{
		CGolemLeftArm* pGolemLeftArm = CGolemLeftArm::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGolemLeftArm, E_FAIL);
		pGolemLeftArm->Set_ArmNum(i - LEFTARM0);
		pGolemLeftArm->Get_TransformCom()->RotationAxis(vAxisZ, D3DXToRadian(-45.f*(i - LEFTARM1)));
		pGolemLeftArm->Get_TransformCom()->Set_Pos(&m_vPartPos[i]);
		m_vecParts.push_back(pGolemLeftArm);
	}
	for (_int i = RIGHTARM0; i < RIGHTARM2 + 1; i++)
	{
		CGolemRightArm* pGolemRightArm = CGolemRightArm::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGolemRightArm, E_FAIL);
		pGolemRightArm->Set_ArmNum(i - RIGHTARM0);
		pGolemRightArm->Get_TransformCom()->RotationAxis(vAxisZ, D3DXToRadian(45.f * (i - RIGHTARM1)));
		pGolemRightArm->Get_TransformCom()->Set_Pos(&m_vPartPos[i]);
		m_vecParts.push_back(pGolemRightArm);
	}
	for (_int i = LEFTHAND0; i < LEFTHAND2 + 1; i++)
	{
		CGolemLeftHand* pGolemLeftHand = CGolemLeftHand::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGolemLeftHand, E_FAIL);
		pGolemLeftHand->Set_ArmNum(i - LEFTHAND0);
		pGolemLeftHand->Get_TransformCom()->Set_Pos(&m_vPartPos[i]);
		pGolemLeftHand->Get_TransformCom()->RotationAxis(vAxisZ, D3DXToRadian(-90.f));
		m_vecParts.push_back(pGolemLeftHand);
	}
	for (_int i = RIGHTHAND0; i < RIGHTHAND2 + 1; i++)
	{
		CGolemRightHand* pGolemRightHand = CGolemRightHand::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pGolemRightHand, E_FAIL);
		pGolemRightHand->Set_ArmNum(i - RIGHTHAND0);
		pGolemRightHand->Get_TransformCom()->Set_Pos(&m_vPartPos[i]);
		pGolemRightHand->Get_TransformCom()->RotationAxis(vAxisZ,D3DXToRadian( 90.f));
		m_vecParts.push_back(pGolemRightHand);
	}
	CGolemFace* pGolemFace = CGolemFace::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemFace, E_FAIL);
	pGolemFace->Get_TransformCom()->Set_Pos(&m_vPartPos[FACE]);
	m_vecParts.push_back(pGolemFace);
	return S_OK;
}
void CSunGollem::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{

	if (m_eState == SUNGOLEM_STATE::MOVE|| m_eState == SUNGOLEM_STATE::DIRTY)
		return;
	if (dynamic_cast<CPushStone*>(pCollider->GetOwner()))
	{
		if (dynamic_cast<CPushStone*>(pCollider->GetOwner())->Is_Flying() == true)
		{
			m_tStat.iHp -= 1;
		}
	}



}