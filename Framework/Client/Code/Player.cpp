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
#include "InventoryMgr.h"

#include "Item_Hat_Drill.h"
#include "Item_Hat_Light.h"
#include "Item_Hat_Mask.h"
#include "Item_Hat_Missile.h"
#include "Item_Hat_Monkey.h"
#include "Item_Hat_Turtle.h"
#include "KeyMgr.h"
#include "Player_Skill_Aim.h"
#include "Player_Skill_Range.h"
#include "Effect_Item.h"
#include "Pool.h"
#include "Effect_Shadow.h"
#include "Effect_Block.h"
#include "Effect_Hit.h"
#include "LightMgr.h"


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

	m_pAnimator->Add_Animation(L"Walk_Down", L"Proto_Texture_Player_Walk_Down", 0.07f);
	m_pAnimator->Add_Animation(L"Walk_Up", L"Proto_Texture_Player_Walk_Up", 0.07f);
	m_pAnimator->Add_Animation(L"Walk_Left", L"Proto_Texture_Player_Walk_Left", 0.07f);
	m_pAnimator->Add_Animation(L"Walk_Right", L"Proto_Texture_Player_Walk_Right", 0.07f);
	m_pAnimator->Add_Animation(L"Walk_LeftUp", L"Proto_Texture_Player_Walk_LeftUp", 0.07f);
	m_pAnimator->Add_Animation(L"Walk_LeftDown", L"Proto_Texture_Player_Walk_LeftDown", 0.07f);
	m_pAnimator->Add_Animation(L"Walk_RightUp", L"Proto_Texture_Player_Walk_RightUp", 0.07f);
	m_pAnimator->Add_Animation(L"Walk_RightDown", L"Proto_Texture_Player_Walk_RightDown", 0.07f);

	m_pAnimator->Add_Animation(L"Run_Down", L"Proto_Texture_Player_Run_Down", 0.09f);
	m_pAnimator->Add_Animation(L"Run_Up", L"Proto_Texture_Player_Run_Up", 0.09f);
	m_pAnimator->Add_Animation(L"Run_Left", L"Proto_Texture_Player_Run_Left", 0.09f);
	m_pAnimator->Add_Animation(L"Run_Right", L"Proto_Texture_Player_Run_Right", 0.09f);
	m_pAnimator->Add_Animation(L"Run_LeftUp", L"Proto_Texture_Player_Run_LeftUp", 0.09f);
	m_pAnimator->Add_Animation(L"Run_LeftDown", L"Proto_Texture_Player_Run_LeftDown", 0.09f);
	m_pAnimator->Add_Animation(L"Run_RightUp", L"Proto_Texture_Player_Run_RightUp", 0.09f);
	m_pAnimator->Add_Animation(L"Run_RightDown", L"Proto_Texture_Player_Run_RightDown", 0.09f);

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

	m_pAnimator->Add_Animation(L"Swing_Down", L"Proto_Texture_Player_Swing_Down", 0.07f);
	m_pAnimator->Add_Animation(L"Swing_Up", L"Proto_Texture_Player_Swing_Up", 0.07f);
	m_pAnimator->Add_Animation(L"Swing_Left", L"Proto_Texture_Player_Swing_Left", 0.07f);
	m_pAnimator->Add_Animation(L"Swing_Right", L"Proto_Texture_Player_Swing_Right", 0.07f);
	m_pAnimator->Add_Animation(L"Swing_LeftUp", L"Proto_Texture_Player_Swing_LeftUp", 0.07f);
	m_pAnimator->Add_Animation(L"Swing_LeftDown", L"Proto_Texture_Player_Swing_LeftDown", 0.07f);
	m_pAnimator->Add_Animation(L"Swing_RightUp", L"Proto_Texture_Player_Swing_RightUp", 0.07f);
	m_pAnimator->Add_Animation(L"Swing_RightDown", L"Proto_Texture_Player_Swing_RightDown", 0.07f);

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

	m_pAnimator->Add_Animation(L"LiftWalk_Down", L"Proto_Texture_Player_LiftWalk_Down", 0.07f);
	m_pAnimator->Add_Animation(L"LiftWalk_Up", L"Proto_Texture_Player_LiftWalk_Up", 0.07f);
	m_pAnimator->Add_Animation(L"LiftWalk_Left", L"Proto_Texture_Player_LiftWalk_Left", 0.07f);
	m_pAnimator->Add_Animation(L"LiftWalk_Right", L"Proto_Texture_Player_LiftWalk_Right", 0.07f);
	m_pAnimator->Add_Animation(L"LiftWalk_LeftUp", L"Proto_Texture_Player_LiftWalk_LeftUp", 0.07f);
	m_pAnimator->Add_Animation(L"LiftWalk_LeftDown", L"Proto_Texture_Player_LiftWalk_LeftDown", 0.07f);
	m_pAnimator->Add_Animation(L"LiftWalk_RightUp", L"Proto_Texture_Player_LiftWalk_RightUp", 0.07f);
	m_pAnimator->Add_Animation(L"LiftWalk_RightDown", L"Proto_Texture_Player_LiftWalk_RightDown", 0.07f);

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



	CGameObject* pHat = nullptr;
	m_eHat = PLAYER_HAT::NONE;

	m_vecHats.push_back(pHat);


	pHat = CItem_Hat_Light::Create(m_pGraphicDev, this);
	if (pHat)
		m_vecHats.push_back(pHat);

	pHat = CItem_Hat_Monkey::Create(m_pGraphicDev, this);
	if (pHat)
		m_vecHats.push_back(pHat);

	pHat = CItem_Hat_Mask::Create(m_pGraphicDev, this);
	if (pHat)
		m_vecHats.push_back(pHat);

	pHat = CItem_Hat_Turtle::Create(m_pGraphicDev, this);
	if (pHat)
		m_vecHats.push_back(pHat);

	pHat = CItem_Hat_Drill::Create(m_pGraphicDev, this);
	if (pHat)
		m_vecHats.push_back(pHat);
	
	pHat = CItem_Hat_Missile::Create(m_pGraphicDev, this);
	if (pHat)
		m_vecHats.push_back(pHat);
	



	CGameObject* pAim = CPlayer_Skill_Aim::Create(m_pGraphicDev);
	if (pAim)
	{
		pAim->Ready_Object();
		m_pAim = pAim;
	}
		

	CGameObject* pRange = CPlayer_Skill_Range::Create(m_pGraphicDev);
	if (pRange)
	{
		pRange->Ready_Object();
		m_pSkillRange = pRange;
	}
		

	CGameObject* pShadow = CEffect_Shadow::Create(m_pGraphicDev);
	if (pShadow)
	{
		pShadow->Ready_Object();
		dynamic_cast<CEffect_Shadow*>(pShadow)->Set_Shadow(this, _vec3(1.2f, 1.0f, 1.0f));
		m_pShadow = pShadow; 
	}
		

	m_iAlpha = 255;

	m_fAccInvinTime = 0.0f;
	m_fInvinTime = 2.0f;
	m_fBlinkTime = 0.05;

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PLAYER);
	Engine::Add_CollisionGroup(m_pCollider[(_uint)COLLIDER_PLAYER::COLLIDER_GRAB], COLLISION_GROUP::COLLIDE_GRAB);
	Engine::Add_CollisionGroup(m_pCollider[(_uint)COLLIDER_PLAYER::COLLIDER_ATTACK], COLLISION_GROUP::COLLIDE_SWING);

	Key_Input(fTimeDelta);

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

	if (m_pShadow && m_pShadow->Is_Active())
		m_pShadow->Update_Object(fTimeDelta);


	//모자 테스트
	if(m_vecHats[(_uint)m_eHat] && m_vecHats[(_uint)m_eHat]->Is_Active())
		m_vecHats[(_uint)m_eHat]->Update_Object(fTimeDelta);


	if (m_bInvincible)
	{
		m_fAccInvinTime += fTimeDelta;
		m_fBlinkTime -= fTimeDelta;

		if (m_fBlinkTime < 0.0f)
		{
			if (m_iAlpha == 255)
				m_iAlpha = 50;
			else
				m_iAlpha = 255;

			m_fBlinkTime = 0.05f;
		}

		

		if (m_fAccInvinTime > m_fInvinTime)
		{
			m_bInvincible = false;
			m_iAlpha = 255;
		}
	}


	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{
	m_vecState[(_uint)m_eState]->LateUpdate_State();

	for (int i = 0; (_uint)COLLIDER_PLAYER::COLLIDER_END > i; ++i)
		m_pCollider[i]->LateUpdate_Component();


	if (m_pShadow && m_pShadow->Is_Active())
		m_pShadow->LateUpdate_Object();


	if (m_vecHats[(_uint)m_eHat] && m_vecHats[(_uint)m_eHat]->Is_Active())
		m_vecHats[(_uint)m_eHat]->LateUpdate_Object();

	__super::LateUpdate_Object();
}

void CPlayer::Render_Object(void)
{
	// _matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	// m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	// m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	// __super::Render_Object();
	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;
	
	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());

	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);
	
	const D3DLIGHT9& tLight = CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_DIRECTION)->Get_LightInfo();
	pEffect->SetValue("g_Light", &tLight, sizeof(D3DLIGHT9));

	
	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	if (m_pShadow && m_pShadow->Is_Active())
		m_pShadow->Render_Object();

	if (m_vecHats[(_uint)m_eHat] && m_vecHats[(_uint)m_eHat]->Is_Active())
		m_vecHats[(_uint)m_eHat]->Render_Object();
	
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

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

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

	if (m_pColliderCom->Get_Id() == _iColliderID && 
		pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_BULLET &&
		dynamic_cast<CBullet*>(pCollider->GetOwner())->Get_Owner()->GetObj_Type() == OBJ_TYPE::OBJ_MONSTER)
	{
		Collision_Enter_BulletHit(pCollider, _eCollisionGroup, _iColliderID);
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

void CPlayer::Key_Input(const _float& fTimeDelta)
{

	if (fTimeDelta == 0)
		return;

	if (KEY_TAP(KEY::NUM_1))
	{
		m_eHat = PLAYER_HAT::LIGHT;
		Set_Hat(m_vecHats[(_uint)m_eHat]);
	}
	else if (KEY_TAP(KEY::NUM_2))
	{
		m_eHat = PLAYER_HAT::GOLEMFIST;
		Set_Hat(m_vecHats[(_uint)m_eHat]);
	}
	else if (KEY_TAP(KEY::NUM_3))
	{
		m_eHat = PLAYER_HAT::LIGHTNING;
		Set_Hat(m_vecHats[(_uint)m_eHat]);
	}
	else if (KEY_TAP(KEY::NUM_4))
	{
		m_eHat = PLAYER_HAT::TURTLE;
		Set_Hat(m_vecHats[(_uint)m_eHat]);
	}
	else if (KEY_TAP(KEY::NUM_5))
	{
		m_eHat = PLAYER_HAT::DRILL;
		Set_Hat(m_vecHats[(_uint)m_eHat]);
	}
	else if (KEY_TAP(KEY::NUM_6))
	{
		m_eHat = PLAYER_HAT::MISSILE;
		Set_Hat(m_vecHats[(_uint)m_eHat]);
	}
}

void CPlayer::Set_ItemEffect(ITEM_CODE eItemCode)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	CGameObject* pEffect = CPool<CEffect_Item>::Get_Obj();

	if (!pEffect)
		pEffect = CEffect_Item::Create(m_pGraphicDev);

	dynamic_cast<CEffect_Item*>(pEffect)->Get_Effect(vPos, eItemCode);

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

	_vec3 vPos, vOtherPos;

	CTransform* pTransform = m_pTransformCom;
	CTransform* pOtherTransform = pCollider->GetOwner()->Get_TransformCom();

	CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pColliderCom);
	CBoxCollider* pOtherBoxCollider = dynamic_cast<CBoxCollider*>(pCollider);


	NULL_CHECK_RETURN(pTransform, );
	NULL_CHECK_RETURN(pOtherTransform, );
	NULL_CHECK_RETURN(pBoxCollider, );
	NULL_CHECK_RETURN(pOtherBoxCollider, );

	pTransform->Get_Info(INFO_POS, &vPos);
	pOtherTransform->Get_Info(INFO_POS, &vOtherPos);

	_vec3 vDir = vOtherPos - vPos;
	_float fX = (pBoxCollider->Get_Scale().x * 0.5f) + (pOtherBoxCollider->Get_Scale().x * 0.5f);
	_float fY = (pBoxCollider->Get_Scale().y * 0.5f) + (pOtherBoxCollider->Get_Scale().y * 0.5f);
	_float fZ = (pBoxCollider->Get_Scale().z * 0.5f) + (pOtherBoxCollider->Get_Scale().z * 0.5f);


	if (fabs(vDir.x) >= fX)
		return;

	if (fabs(vDir.y) >= fY)
		return;

	if (fabs(vDir.z) >= fZ)
		return;

	


	if (m_eState == PLAYER_STATE::PUSH)
	{
		if (fX - fabs(vDir.x) < fZ - fabs(vDir.z))
		{
			if (vDir.x < 0.f)
			{
				eTargetDir = OBJ_DIR::DIR_L;
				vDir.x -= (fX - fabs(vDir.x));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				eTargetDir = OBJ_DIR::DIR_R;
				vDir.x += (fX - fabs(vDir.x));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}
		/*else if (fY - fabs(vDir.y) < fZ - fabs(vDir.z) && fY - fabs(vDir.y) < fX - fabs(vDir.x))
		{
			if (vDir.y < 0.f)
			{
				vDir.y -= (fY - fabs(vDir.y));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				vDir.y += (fY - fabs(vDir.y));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}*/
		else if (fZ - fabs(vDir.z) < fX - fabs(vDir.x))
		{
			if (vDir.z < 0.f)
			{
				eTargetDir = OBJ_DIR::DIR_D;
				vDir.z -= (fZ - fabs(vDir.z));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				eTargetDir = OBJ_DIR::DIR_U;
				vDir.z += (fZ - fabs(vDir.z));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}

		//사운드

		if(pCollider->GetOwner()->GetObj_Id() == OBJ_ID::PUSH_STONE)
			Play_Sound(L"SFX_26_StonePushable_Push.wav", CHANNELID::SOUND_EFFECT_INTERACTION, 0.5f);
		else if (pCollider->GetOwner()->GetObj_Id() == OBJ_ID::CATAPULT)
			Play_Sound(L"SFX_30_Catapult_Push.wav", CHANNELID::SOUND_EFFECT_INTERACTION, 0.5f);
		else if (pCollider->GetOwner()->GetObj_Id() == OBJ_ID::JELLY_STONE || 
			pCollider->GetOwner()->GetObj_Id() == OBJ_ID::JELLY_COMBINED)
			Play_Sound(L"SFX_44_Jelly_Push.wav", CHANNELID::SOUND_EFFECT_INTERACTION, 0.5f);




	}
	else if (pCollider->GetOwner() == m_pLiftObj)
	{

	}
	else
	{
		if (fX - fabs(vDir.x) < fZ - fabs(vDir.z))
		{
			if (vDir.x < 0.f)
			{
				eTargetDir = OBJ_DIR::DIR_L;
				vDir.x -= (fX - fabs(vDir.x));
				vDir *= -1.0f;
				vPos = vOtherPos + vDir;
				pTransform->Set_Pos(&vPos);
			}
			else
			{
				eTargetDir = OBJ_DIR::DIR_R;
				vDir.x += (fX - fabs(vDir.x));
				vDir *= -1.0f;
				vPos = vOtherPos + vDir;
				pTransform->Set_Pos(&vPos);
			}
		}
		/*else if (fY - fabs(vDir.y) < fZ - fabs(vDir.z) && fY - fabs(vDir.y) < fX - fabs(vDir.x))
		{
			if (vDir.y < 0.f)
			{
				vDir.y -= (fY - fabs(vDir.y));
				vDir *= -1.0f;
				vPos = vOtherPos + vDir;
				pTransform->Set_Pos(&vPos);
			}
			else
			{
				vDir.y += (fY - fabs(vDir.y));
				vDir *= -1.0f;
				vPos = vOtherPos + vDir;
				pTransform->Set_Pos(&vPos);
			}
		}*/
		else if (fZ - fabs(vDir.z) < fX - fabs(vDir.x))
		{
			if (vDir.z < 0.f)
			{
				eTargetDir = OBJ_DIR::DIR_D;
				vDir.z -= (fZ - fabs(vDir.z));
				vDir *= -1.0f;
				vPos = vOtherPos + vDir;
				pTransform->Set_Pos(&vPos);
			}
			else
			{
				eTargetDir = OBJ_DIR::DIR_U;
				vDir.z += (fZ - fabs(vDir.z));
				vDir *= -1.0f;
				vPos = vOtherPos + vDir;
				pTransform->Set_Pos(&vPos);
			}
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
	if (m_eState == PLAYER_STATE::HIT || 
		m_eState == PLAYER_STATE::DOWN || 
		m_eState == PLAYER_STATE::GAMEOVER || 
		m_bInvincible)
		return;


	if (pCollider->GetOwner()->GetObj_Id() == OBJ_ID::BLUE_BEATLE ||
		pCollider->GetOwner()->GetObj_Id() == OBJ_ID::RED_BEATLE ||
		pCollider->GetOwner()->GetObj_Id() == OBJ_ID::GREEN_BEATLE)
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
		vDir = vTargetPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		_vec3 vEffectPos = vPos + (vDir * 0.5f);
		CGameObject* pEffect = CPool<CEffect_Block>::Get_Obj();
		if (!pEffect)
		{
			pEffect = CEffect_Block::Create(m_pGraphicDev);
			pEffect->Ready_Object();
		}
		dynamic_cast<CEffect_Block*>(pEffect)->Get_Effect(vEffectPos, _vec3(2.0f, 2.0f, 2.0f));

		m_pRigidBodyCom->AddForce(vDir * -20.0f);
		pCollider->GetOwner()->Get_RigidBodyCom()->AddForce(vDir * 120.0f);


		Stop_Sound(CHANNELID::SOUND_SKILL);
		Play_Sound(L"SFX_204_Blocked.wav", CHANNELID::SOUND_SKILL, 0.5f);
	}
	else
	{
		vDir = vTargetPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		_vec3 vEffectPos = vPos + (vDir * 0.5f);
		CGameObject* pEffect = CPool<CEffect_Hit>::Get_Obj();
		if (!pEffect)
		{
			pEffect = CEffect_Hit::Create(m_pGraphicDev);
			pEffect->Ready_Object();
		}
		dynamic_cast<CEffect_Hit*>(pEffect)->Get_Effect(vEffectPos, _vec3(2.0f, 2.0f, 2.0f), 255, 0, 0);
		Change_State_Now(PLAYER_STATE::HIT);
	}
		

}

void CPlayer::Collision_Enter_BulletHit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (m_eState == PLAYER_STATE::HIT ||
		m_eState == PLAYER_STATE::DOWN ||
		m_eState == PLAYER_STATE::GAMEOVER ||
		m_bInvincible)
		return;



	OBJ_DIR eTargetDir = OBJ_DIR::DIR_END;
	_vec3 vTargetPos;
	_vec3 vPos;
	_vec3 vDir;
	_vec3 vAxis = { 0.0f, 0.0f, 1.0f };
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
		vDir = vTargetPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		_vec3 vEffectPos = vPos + (vDir * 0.5f);
		CGameObject* pEffect = CPool<CEffect_Block>::Get_Obj();
		if (!pEffect)
		{
			pEffect = CEffect_Block::Create(m_pGraphicDev);
			pEffect->Ready_Object();
		}
		dynamic_cast<CEffect_Block*>(pEffect)->Get_Effect(vEffectPos, _vec3(2.0f, 2.0f, 2.0f));

		m_pRigidBodyCom->AddForce(vDir * -20.0f);
		//pCollider->GetOwner()->Get_RigidBodyCom()->AddForce(vDir * 120.0f);

		Stop_Sound(CHANNELID::SOUND_SKILL);
		Play_Sound(L"SFX_204_Blocked.wav", CHANNELID::SOUND_SKILL, 0.5f);

	}
	else
	{
		vDir = vTargetPos - vPos;
		D3DXVec3Normalize(&vDir, &vDir);
		_vec3 vEffectPos = vPos + (vDir * 0.5f);
		CGameObject* pEffect = CPool<CEffect_Hit>::Get_Obj();
		if (!pEffect)
		{
			pEffect = CEffect_Hit::Create(m_pGraphicDev);
			pEffect->Ready_Object();
		}
		dynamic_cast<CEffect_Hit*>(pEffect)->Get_Effect(vEffectPos, _vec3(2.0f, 2.0f, 2.0f), 255, 0, 0);
		Change_State_Now(PLAYER_STATE::HIT);
	}
}

