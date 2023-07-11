#include "Player_Skill_Turtle.h"
#include "Player_Skill_Barrier.h"
#include "Transform.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Export_Function.h"


CPlayer_Skill_Turtle::CPlayer_Skill_Turtle(CGameObject* _pOwner)
	:CPlayer_State(_pOwner)
{
	
}

CPlayer_Skill_Turtle::~CPlayer_Skill_Turtle()
{
}

HRESULT CPlayer_Skill_Turtle::Ready_State(void)
{
	if (!m_pBarrier)
	{
		CPlayer_Skill_Barrier* pBarrier = CPlayer_Skill_Barrier::Create(Engine::Get_Device());
		Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"Player_Barrier", pBarrier);
		m_pBarrier = pBarrier;
	}

	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Up", true);
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Down", true);
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Left", true);
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Right", true);
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_LeftDown", true);
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_LeftUp", true);
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_RightUp", true);
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_RightDown", true);
		break;
	}
	m_pBarrier->Set_Active(true);



	return S_OK;
}

_int CPlayer_Skill_Turtle::Update_State(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	return 0;
}

void CPlayer_Skill_Turtle::LateUpdate_State(void)
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.z -= 0.01f;
	vPos.y += 0.1f;
	m_pBarrier->Get_TransformCom()->Set_Pos(&vPos);
}

void CPlayer_Skill_Turtle::Render_State(void)
{
}

void CPlayer_Skill_Turtle::Key_Input(const _float& fTimeDelta)
{
	if (KEY_HOLD(KEY::K))
	{

	}
	else
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
		m_pBarrier->Set_Active(false);
	}
}