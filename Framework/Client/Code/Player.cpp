#include "..\Header\Player.h"

#include "Export_Function.h"
#include "Bullet.h"

#include "Scene.h"
#include "Terrain.h"
#include "Player_State_Walk.h"
#include "Player_State_Idle.h"
#include "Player_State_Run.h"
#include "Player_State_Rolling.h"
#include "Player_State_Jump.h"
#include "Player_State_Swing.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PLAYER)
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

	m_pAnimator->Add_Animation(L"Idle_Down", L"Proto_Texture_Player_Idle_Down",0.1f);
	m_pAnimator->Add_Animation(L"Idle_Up", L"Proto_Texture_Player_Idle_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Idle_Left", L"Proto_Texture_Player_Idle_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Idle_Right", L"Proto_Texture_Player_Idle_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Idle_LeftUp", L"Proto_Texture_Player_Idle_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Idle_LeftDown", L"Proto_Texture_Player_Idle_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Idle_RightUp", L"Proto_Texture_Player_Idle_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Idle_RightDown", L"Proto_Texture_Player_Idle_RightDown", 0.1f);

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

	m_pAnimator->Add_Animation(L"Jump_Down", L"Proto_Texture_Player_Jump_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Jump_Up", L"Proto_Texture_Player_Jump_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Jump_Left", L"Proto_Texture_Player_Jump_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Jump_Right", L"Proto_Texture_Player_Jump_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Jump_LeftUp", L"Proto_Texture_Player_Jump_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Jump_LeftDown", L"Proto_Texture_Player_Jump_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Jump_RightUp", L"Proto_Texture_Player_Jump_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Jump_RightDown", L"Proto_Texture_Player_Jump_RightDown", 0.1f);

	m_pAnimator->Add_Animation(L"Swing_Down", L"Proto_Texture_Player_Swing_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_Up", L"Proto_Texture_Player_Swing_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_Left", L"Proto_Texture_Player_Swing_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_Right", L"Proto_Texture_Player_Swing_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_LeftUp", L"Proto_Texture_Player_Swing_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_LeftDown", L"Proto_Texture_Player_Swing_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_RightUp", L"Proto_Texture_Player_Swing_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Swing_RightDown", L"Proto_Texture_Player_Swing_RightDown", 0.1f);


	m_pAnimator->Play_Animation(L"Idle_Down");

	m_eState = PLAYER_STATE::IDLE;
	m_eDir = OBJ_DIR::DIR_D;

	m_vecState.reserve(10);

	m_vecState.push_back(new CPlayer_State_Idle(this));
	m_vecState.push_back(new CPlayer_State_Walk(this));
	m_vecState.push_back(new CPlayer_State_Run(this));
	m_vecState.push_back(new CPlayer_State_Rolling(this));
	m_vecState.push_back(new CPlayer_State_Jump(this));
	m_vecState.push_back(new CPlayer_State_Swing(this));

	m_pTransformCom->Set_Pos(&_vec3(0.0f, 1.0f, 0.0f));

	return S_OK;
}

Engine::_int CPlayer::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	
	if (m_bStateChange)
	{
		m_eState = m_eState;
		m_vecState[(_uint)m_eState]->Ready_State();
		m_bStateChange = false;
	}

	m_vecState[(_uint)m_eState]->Update_State(fTimeDelta);


	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CPlayer::LateUpdate_Object(void)
{
	m_vecState[(_uint)m_eState]->LateUpdate_State();
	__super::LateUpdate_Object();
}

void CPlayer::Render_Object(void)
{
	Set_Billboard();
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

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
void CPlayer::Free()
{
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

	return pInstance;
}
