#include "..\Header\Player.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Player_State_Walk.h"
#include "Player_State_Idle.h"
#include "Player_State_Run.h"
#include "Player_State_Rolling.h"
#include "Player_State_Jump.h"
#include "Player_State_Swing.h"
#include "Player_State_Hit.h"
#include "Player_State_Lift.h"
#include "Player_State_Down.h"
#include "Player_State_GameOver.h"
#include "Player_State_Push.h"
#include "Player_State_GetItem.h"
#include "Player_State_BallonFly.h"
#include "Player_State_Drawing.h"
#include "FieldObject.h"
#include "Player_State_Skill.h"
#include "Player_State_Dance.h"
#include "Effect_Shadow.h"
#include	"InventoryMgr.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PLAYER, OBJ_ID::PLAYER)
	, m_fSpeed(5.f)
	, m_vDest(0.f, 0.f, 0.f)
	, m_eState(PLAYER_STATE::IDLE)
	, m_bStateChange(false)
{

}
CPlayer::CPlayer(const CPlayer& rhs)
	: Engine::CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_vDest(0.f, 0.f, 0.f)
	, m_bStateChange(false)
	, m_eState(PLAYER_STATE::IDLE)
{

}
CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"Idle_Down", L"Proto_Texture_Player_Idle_Down",0.3f);
	m_pAnimator->Add_Animation(L"Idle_Up", L"Proto_Texture_Player_Idle_Up", 0.3f);
	m_pAnimator->Add_Animation(L"Idle_Left", L"Proto_Texture_Player_Idle_Left", 0.3f);
	m_pAnimator->Add_Animation(L"Idle_Right", L"Proto_Texture_Player_Idle_Right", 0.3f);
	m_pAnimator->Add_Animation(L"Idle_LeftUp", L"Proto_Texture_Player_Idle_LeftUp", 0.3f);
	m_pAnimator->Add_Animation(L"Idle_LeftDown", L"Proto_Texture_Player_Idle_LeftDown", 0.3f);
	m_pAnimator->Add_Animation(L"Idle_RightUp", L"Proto_Texture_Player_Idle_RightUp", 0.3f);
	m_pAnimator->Add_Animation(L"Idle_RightDown", L"Proto_Texture_Player_Idle_RightDown", 0.3f);

	m_pAnimator->Add_Animation(L"Walk_Down", L"Proto_Texture_Player_Walk_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Walk_Up", L"Proto_Texture_Player_Walk_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Walk_Left", L"Proto_Texture_Player_Walk_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Walk_Right", L"Proto_Texture_Player_Walk_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Walk_LeftUp", L"Proto_Texture_Player_Walk_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Walk_LeftDown", L"Proto_Texture_Player_Walk_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Walk_RightUp", L"Proto_Texture_Player_Walk_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Walk_RightDown", L"Proto_Texture_Player_Walk_RightDown", 0.1f);

	m_pAnimator->Add_Animation(L"Run_Down", L"Proto_Texture_Player_Run_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Run_Up", L"Proto_Texture_Player_Run_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Run_Left", L"Proto_Texture_Player_Run_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Run_Right", L"Proto_Texture_Player_Run_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Run_LeftUp", L"Proto_Texture_Player_Run_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Run_LeftDown", L"Proto_Texture_Player_Run_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Run_RightUp", L"Proto_Texture_Player_Run_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Run_RightDown", L"Proto_Texture_Player_Run_RightDown", 0.1f);

	m_pAnimator->Add_Animation(L"Rolling_Down", L"Proto_Texture_Player_Rolling_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Rolling_Up", L"Proto_Texture_Player_Rolling_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Rolling_Left", L"Proto_Texture_Player_Rolling_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Rolling_Right", L"Proto_Texture_Player_Rolling_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Rolling_LeftUp", L"Proto_Texture_Player_Rolling_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Rolling_LeftDown", L"Proto_Texture_Player_Rolling_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Rolling_RightUp", L"Proto_Texture_Player_Rolling_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Rolling_RightDown", L"Proto_Texture_Player_Rolling_RightDown", 0.1f);

	m_pAnimator->Add_Animation(L"Jump_Down", L"Proto_Texture_Player_Jump_Down", 0.05f);
	m_pAnimator->Add_Animation(L"Jump_Up", L"Proto_Texture_Player_Jump_Up", 0.05f);
	m_pAnimator->Add_Animation(L"Jump_Left", L"Proto_Texture_Player_Jump_Left", 0.05f);
	m_pAnimator->Add_Animation(L"Jump_Right", L"Proto_Texture_Player_Jump_Right", 0.05f);
	m_pAnimator->Add_Animation(L"Jump_LeftUp", L"Proto_Texture_Player_Jump_LeftUp", 0.05f);
	m_pAnimator->Add_Animation(L"Jump_LeftDown", L"Proto_Texture_Player_Jump_LeftDown", 0.05f);
	m_pAnimator->Add_Animation(L"Jump_RightUp", L"Proto_Texture_Player_Jump_RightUp", 0.05f);
	m_pAnimator->Add_Animation(L"Jump_RightDown", L"Proto_Texture_Player_Jump_RightDown", 0.05f);

	m_pAnimator->Add_Animation(L"Swing_Down", L"Proto_Texture_Player_Swing_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_Up", L"Proto_Texture_Player_Swing_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_Left", L"Proto_Texture_Player_Swing_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_Right", L"Proto_Texture_Player_Swing_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_LeftUp", L"Proto_Texture_Player_Swing_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_LeftDown", L"Proto_Texture_Player_Swing_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_RightUp", L"Proto_Texture_Player_Swing_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_RightDown", L"Proto_Texture_Player_Swing_RightDown", 0.1f);

	m_pAnimator->Add_Animation(L"Hit_Down", L"Proto_Texture_Player_Hit_Down", 0.2f);
	m_pAnimator->Add_Animation(L"Hit_Up", L"Proto_Texture_Player_Hit_Up", 0.2f);
	m_pAnimator->Add_Animation(L"Hit_Left", L"Proto_Texture_Player_Hit_Left", 0.2f);
	m_pAnimator->Add_Animation(L"Hit_Right", L"Proto_Texture_Player_Hit_Right", 0.2f);
	m_pAnimator->Add_Animation(L"Hit_LeftUp", L"Proto_Texture_Player_Hit_LeftUp", 0.2f);
	m_pAnimator->Add_Animation(L"Hit_LeftDown", L"Proto_Texture_Player_Hit_LeftDown", 0.2f);
	m_pAnimator->Add_Animation(L"Hit_RightUp", L"Proto_Texture_Player_Hit_RightUp", 0.2f);
	m_pAnimator->Add_Animation(L"Hit_RightDown", L"Proto_Texture_Player_Hit_RightDown", 0.2f);

	m_pAnimator->Add_Animation(L"LiftUp_Down", L"Proto_Texture_Player_LiftUp_Down", 0.1f);
	m_pAnimator->Add_Animation(L"LiftUp_Up", L"Proto_Texture_Player_LiftUp_Up", 0.1f);
	m_pAnimator->Add_Animation(L"LiftUp_Left", L"Proto_Texture_Player_LiftUp_Left", 0.1f);
	m_pAnimator->Add_Animation(L"LiftUp_Right", L"Proto_Texture_Player_LiftUp_Right", 0.1f);
	m_pAnimator->Add_Animation(L"LiftUp_LeftUp", L"Proto_Texture_Player_LiftUp_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"LiftUp_LeftDown", L"Proto_Texture_Player_LiftUp_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"LiftUp_RightUp", L"Proto_Texture_Player_LiftUp_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"LiftUp_RightDown", L"Proto_Texture_Player_LiftUp_RightDown", 0.1f);

	m_pAnimator->Add_Animation(L"LiftDown_Down", L"Proto_Texture_Player_LiftDown_Down", 0.1f);
	m_pAnimator->Add_Animation(L"LiftDown_Up", L"Proto_Texture_Player_LiftDown_Up", 0.1f);
	m_pAnimator->Add_Animation(L"LiftDown_Left", L"Proto_Texture_Player_LiftDown_Left", 0.1f);
	m_pAnimator->Add_Animation(L"LiftDown_Right", L"Proto_Texture_Player_LiftDown_Right", 0.1f);
	m_pAnimator->Add_Animation(L"LiftDown_LeftUp", L"Proto_Texture_Player_LiftDown_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"LiftDown_LeftDown", L"Proto_Texture_Player_LiftDown_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"LiftDown_RightUp", L"Proto_Texture_Player_LiftDown_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"LiftDown_RightDown", L"Proto_Texture_Player_LiftDown_RightDown", 0.1f);

	m_pAnimator->Add_Animation(L"LiftWalk_Down", L"Proto_Texture_Player_LiftWalk_Down", 0.1f);
	m_pAnimator->Add_Animation(L"LiftWalk_Up", L"Proto_Texture_Player_LiftWalk_Up", 0.1f);
	m_pAnimator->Add_Animation(L"LiftWalk_Left", L"Proto_Texture_Player_LiftWalk_Left", 0.1f);
	m_pAnimator->Add_Animation(L"LiftWalk_Right", L"Proto_Texture_Player_LiftWalk_Right", 0.1f);
	m_pAnimator->Add_Animation(L"LiftWalk_LeftUp", L"Proto_Texture_Player_LiftWalk_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"LiftWalk_LeftDown", L"Proto_Texture_Player_LiftWalk_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"LiftWalk_RightUp", L"Proto_Texture_Player_LiftWalk_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"LiftWalk_RightDown", L"Proto_Texture_Player_LiftWalk_RightDown", 0.1f);

	m_pAnimator->Add_Animation(L"Lift_Down", L"Proto_Texture_Player_Lift_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Lift_Up", L"Proto_Texture_Player_Lift_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Lift_Left", L"Proto_Texture_Player_Lift_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Lift_Right", L"Proto_Texture_Player_Lift_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Lift_LeftUp", L"Proto_Texture_Player_Lift_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Lift_LeftDown", L"Proto_Texture_Player_Lift_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Lift_RightUp", L"Proto_Texture_Player_Lift_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Lift_RightDown", L"Proto_Texture_Player_Lift_RightDown", 0.1f);


	m_pAnimator->Add_Animation(L"Push_Down", L"Proto_Texture_Player_Push_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Push_Up", L"Proto_Texture_Player_Push_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Push_Left", L"Proto_Texture_Player_Push_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Push_Right", L"Proto_Texture_Player_Push_Right", 0.1f);

	m_pAnimator->Add_Animation(L"Down", L"Proto_Texture_Player_Down", 0.1f);
	m_pAnimator->Add_Animation(L"GameOver", L"Proto_Texture_Player_GameOver", 0.1f);
	m_pAnimator->Add_Animation(L"Dance", L"Proto_Texture_Player_Dance", 0.1f);

	m_pAnimator->Add_Animation(L"GetItem", L"Proto_Texture_Player_GetItem", 0.1f);
	m_pAnimator->Add_Animation(L"BalloonFly", L"Proto_Texture_Player_BalloonFly", 0.1f);
	m_pAnimator->Add_Animation(L"Drawing", L"Proto_Texture_Player_Drawing", 0.1f);

	m_pAnimator->Add_Animation(L"Drill", L"Proto_Texture_Player_Drill", 0.1f);


	m_pAnimator->Play_Animation(L"Idle_Down", true);

	m_eState = PLAYER_STATE::IDLE;
	m_eDir = OBJ_DIR::DIR_D;

	m_vecState.reserve(10);

	m_vecState.push_back(new CPlayer_State_Idle(this));
	m_vecState.push_back(new CPlayer_State_Walk(this));
	m_vecState.push_back(new CPlayer_State_Run(this));
	m_vecState.push_back(new CPlayer_State_Rolling(this));
	m_vecState.push_back(new CPlayer_State_Jump(this));
	m_vecState.push_back(new CPlayer_State_Swing(this));
	m_vecState.push_back(new CPlayer_State_Hit(this));
	m_vecState.push_back(new CPlayer_State_Lift(this));
	m_vecState.push_back(new CPlayer_State_Down(this));
	m_vecState.push_back(new CPlayer_State_GameOver(this));
	m_vecState.push_back(new CPlayer_State_Push(this));
	m_vecState.push_back(new CPlayer_State_GetItem(this));
	m_vecState.push_back(new Player_State_BallonFly(this));
	m_vecState.push_back(new CPlayer_State_Drawing(this));
	m_vecState.push_back(new CPlayer_State_Skill(this));
	m_vecState.push_back(new CPlayer_State_Dance(this));

	m_pTransformCom->Set_Pos(&_vec3(0.0f, 0.5f, 0.0f));
	m_fMinHeight = 0.5f;

	m_tPlayerStat.iMaxHp = m_tPlayerStat.iHp = 5;
	m_tPlayerStat.iMaxMp = m_tPlayerStat.iMp = 5;
	m_tPlayerStat.iAttack = 5;

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PLAYER);
	Engine::Add_CollisionGroup(m_pCollider[(_uint)COLLIDER_PLAYER::COLLIDER_GRAB], COLLISION_GROUP::COLLIDE_GRAB);
	Engine::Add_CollisionGroup(m_pCollider[(_uint)COLLIDER_PLAYER::COLLIDER_ATTACK], COLLISION_GROUP::COLLIDE_SWING);


	if (m_bStateChange)
	{
		m_vecState[(_uint)m_eState]->Reset_State();
		m_eState = m_eChangeState;
		m_vecState[(_uint)m_eState]->Ready_State();
		m_bStateChange = false;
	}

	m_vecState[(_uint)m_eState]->Update_State(fTimeDelta);

	for (int i = 0; (_uint)COLLIDER_PLAYER::COLLIDER_END > i; ++i)
	{
		m_pCollider[i]->Update_Component(fTimeDelta);
	}

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{
	m_vecState[(_uint)m_eState]->LateUpdate_State();
	for (int i = 0; (_uint)COLLIDER_PLAYER::COLLIDER_END > i; ++i)
	{
		m_pCollider[i]->LateUpdate_Component();
	}
	__super::LateUpdate_Object();
}

void CPlayer::Render_Object(void)
{
	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	for (int i = 0; (_uint)COLLIDER_PLAYER::COLLIDER_END > i; ++i)
	{
		m_pCollider[i]->Render_Component();
	}
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}


HRESULT CPlayer::Ready_Component(void)
{
	CComponent*			pComponent = nullptr;

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
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));

	for (int i = 0; (_uint)COLLIDER_PLAYER::COLLIDER_END > i; ++i)
	{
		m_pCollider[i] = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		m_pCollider[i]->SetOwner(this);
	}
	

	dynamic_cast<CBoxCollider*>(m_pCollider[(_uint)COLLIDER_PLAYER::COLLIDER_GRAB])->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));
	m_pCollider[(_uint)COLLIDER_PLAYER::COLLIDER_GRAB]->Set_Active(false);
	m_pCollider[(_uint)COLLIDER_PLAYER::COLLIDER_ATTACK]->Set_Active(false);



	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	return S_OK;
}



void CPlayer::Player_Move(_float fTimeDelta)
{
	_float fDist = D3DXVec3Length(&m_vDest);
	if (fDist > 5.f)
	{
		_vec3 vDir, vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		vDir = m_vDest - vPos;
		D3DXVec3Normalize(&vDir, &vDir);

		m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 10.f);
	}
}
void CPlayer::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	//MSG_BOX("충돌 Enter");
	if (_iColliderID == m_pCollider[(_uint)COLLIDER_PLAYER::COLLIDER_GRAB]->Get_Id())
	{
		Collision_Enter_Grab(pCollider, _eCollisionGroup, _iColliderID);
	}

	if (m_pColliderCom->Get_Id() == _iColliderID && pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
	{
		Collision_Enter_Hit(pCollider, _eCollisionGroup, _iColliderID);
	}


}
void CPlayer::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	//MSG_BOX("충돌 Stay");
	if (_iColliderID == m_pColliderCom->Get_Id())
	{
		CFieldObject* src;
		if((src = dynamic_cast<CFieldObject*>(pCollider->GetOwner())) && src->Get_ObjInfo().m_bIsPushable)
			Collision_Stay_Push(pCollider, _eCollisionGroup, _iColliderID);
	}	
}
void CPlayer::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	
}

void CPlayer::Free()
{
	for (_uint i = 0; i < m_vecState.size(); ++i)
		Safe_Delete(m_vecState[i]);

	__super::Free();
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*	pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Player Create Failed");
		return nullptr;
	}

	CInventoryMgr::GetInstance()->Set_Player(pInstance);

	return pInstance;
}

void CPlayer::Collision_Stay_Push(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	OBJ_DIR eTargetDir = OBJ_DIR::DIR_END;
	_vec3 vTargetPos;
	_vec3 vPos;
	_vec3 vDir;
	pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = vTargetPos - vPos;

	if (vDir.x > 0.0f && fabs(vDir.x) > fabs(vDir.z))
	{
		eTargetDir = OBJ_DIR::DIR_R;
	}
	else if (vDir.z > 0.0f && fabs(vDir.z) > fabs(vDir.x))
	{
		eTargetDir = OBJ_DIR::DIR_U;
	}
	else if (vDir.x < 0.0f && fabs(vDir.x) > fabs(vDir.z))
	{
		eTargetDir = OBJ_DIR::DIR_L;
	}
	else if (vDir.z < 0.0f && fabs(vDir.x) < fabs(vDir.z))
	{
		eTargetDir = OBJ_DIR::DIR_D;
	}

	if (m_eState == PLAYER_STATE::PUSH)
	{
		if (eTargetDir == OBJ_DIR::DIR_R && fabs(vDir.x) < 0.9f)
		{
			vTargetPos = { vPos.x + 0.9f, vTargetPos.y, vTargetPos.z };
			pCollider->GetOwner()->Get_TransformCom()->Set_Pos(&vTargetPos);
		}
		else if (eTargetDir == OBJ_DIR::DIR_L && fabs(vDir.x) < 0.9f)
		{
			vTargetPos = { vPos.x - 0.9f, vTargetPos.y, vTargetPos.z };
			pCollider->GetOwner()->Get_TransformCom()->Set_Pos(&vTargetPos);
		}
		else if (eTargetDir == OBJ_DIR::DIR_U && fabs(vDir.z) < 0.9f)
		{
			vTargetPos = { vTargetPos.x, vTargetPos.y, vPos.z + 0.9f };
			pCollider->GetOwner()->Get_TransformCom()->Set_Pos(&vTargetPos);
		}
		else if (eTargetDir == OBJ_DIR::DIR_D && fabs(vDir.z) < 0.9f)
		{
			vTargetPos = { vTargetPos.x, vTargetPos.y, vPos.z - 0.9f };
			pCollider->GetOwner()->Get_TransformCom()->Set_Pos(&vTargetPos);
		}
	}
	else if (pCollider->GetOwner() == m_pLiftObj)
	{

	}
	else
	{
		if (eTargetDir == OBJ_DIR::DIR_R && fabs(vDir.x) < 0.9f)
		{
			vPos = { vTargetPos.x - 0.9f, vPos.y, vPos.z };
			m_pTransformCom->Set_Pos(&vPos);
		}
		else if (eTargetDir == OBJ_DIR::DIR_L && fabs(vDir.x) < 0.9f)
		{
			vPos = { vTargetPos.x + 0.9f, vPos.y, vPos.z };
			m_pTransformCom->Set_Pos(&vPos);
		}
		else if (eTargetDir == OBJ_DIR::DIR_U && fabs(vDir.z) < 0.9f)
		{
			vPos = { vPos.x, vPos.y, vTargetPos.z - 0.9f };
			m_pTransformCom->Set_Pos(&vPos);
		}
		else if (eTargetDir == OBJ_DIR::DIR_D && fabs(vDir.z) < 0.9f)
		{
			vPos = { vPos.x, vPos.y, vTargetPos.z + 0.9f };
			m_pTransformCom->Set_Pos(&vPos);
		}

		if (m_eDir == eTargetDir)
			m_bPush = true;

	}
}

void CPlayer::Collision_Enter_Grab(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	m_pLiftObj = dynamic_cast<CFieldObject*>(pCollider->GetOwner())->Get_GrabObj();
	if (!m_pLiftObj || !dynamic_cast<CFieldObject*>(m_pLiftObj)->Get_ObjInfo().m_bIsGrabbable)
	{
		m_pLiftObj = nullptr;
		return;
	}


	m_pLiftObj->Get_ColliderCom()->Set_Active(false);
	if(m_pLiftObj != nullptr)
		m_bGrab = true;
}

void CPlayer::Collision_Enter_Hit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (m_eState == PLAYER_STATE::HIT)
		return;

	OBJ_DIR eTargetDir = OBJ_DIR::DIR_END;
	_vec3 vTargetPos;
	_vec3 vPos;
	_vec3 vDir;
	_vec3 vAxis = {0.0f, 0.0f, 1.0f};
	pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = vTargetPos - vPos;
	vDir.y = 0.0f;
	D3DXVec3Normalize(&vDir, &vDir);

	_float fAngle = D3DXVec3Dot(&vDir, &vAxis);
	fAngle = acosf(fAngle);

	if (vDir.x < 0.0f)
		fAngle = D3DX_PI * 2 - fAngle;

	
	fAngle = D3DXToDegree(fAngle);
	


	_uint iDir = fAngle / 22.5f;

	if (iDir == 0 || iDir == 15 || iDir == 16)
	{
		eTargetDir = OBJ_DIR::DIR_U;
	}
	else if (iDir == 1 || iDir == 2)
	{
		eTargetDir = OBJ_DIR::DIR_RU;
	}
	else if (iDir == 3 || iDir == 4)
	{
		eTargetDir = OBJ_DIR::DIR_R;
	}
	else if (iDir == 5 || iDir == 6)
	{
		eTargetDir = OBJ_DIR::DIR_RD;
	}
	else if (iDir == 7 || iDir == 8)
	{
		eTargetDir = OBJ_DIR::DIR_D;
	}
	else if (iDir == 9 || iDir == 10)
	{
		eTargetDir = OBJ_DIR::DIR_LD;
	}
	else if (iDir == 11 || iDir == 12)
	{
		eTargetDir = OBJ_DIR::DIR_L;
	}
	else if (iDir == 13 || iDir == 14)
	{
		eTargetDir = OBJ_DIR::DIR_LU;
	}

	m_eDir = eTargetDir;


	if (m_eState == PLAYER_STATE::SKILL && dynamic_cast<CPlayer_State_Skill*>(m_vecState[(_uint)PLAYER_STATE::SKILL])->Get_Skill() == PLAYER_SKILL::TURTLE)
	{

	}
	else
		Change_State_Now(PLAYER_STATE::HIT);

}

