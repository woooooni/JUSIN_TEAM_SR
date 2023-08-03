#include "../Include/stdafx.h"
#include "Player_State_Idle.h"
#include "Export_Function.h"

#include "Animator.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Transform.h"
#include "Player_State_Skill.h"
#include "Item_Hat.h"

//Å×½ºÆ®
#include "Particle_FixedLeaf.h"
#include "Particle_MovingLeaf.h"
#include "Pool.h"
#include "Effect_Leaf.h"
#include "Effect_Shadow.h"
#include "Particle_MapCircle.h"
#include "Effect_StoneParticle.h"
#include "Effect_DieSmoke.h"
#include "Effect_Explosion.h"
#include "Effect_LightningGround.h"
#include "Effect_Block.h"
#include "Effect_Hit.h"
#include "Effect_Stun.h"
#include "Effect_CatapultHit.h"
#include "Effect_Dig.h"
#include "Effect_Smoke.h"
#include "Effect_Trail.h"
#include "Effect_NueTrail.h"

CPlayer_State_Idle::CPlayer_State_Idle(CGameObject* _pOwner)
	:CPlayer_State(_pOwner)
{
}

CPlayer_State_Idle::~CPlayer_State_Idle()
{
}

HRESULT CPlayer_State_Idle::Ready_State(void)
{
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
	m_eDir = m_pOwner->GetObj_Dir();
	m_pOwner->Get_TransformCom()->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));

	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();
		dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(1.0f);

		_vec3 vPos;
		m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		vPos.y += 0.3f;
		vPos.z -= 0.0001f;
		dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);
	}

	Stop_Sound(CHANNELID::SOUND_EFFECT_PLAYER);


	return S_OK;

	
}

_int CPlayer_State_Idle::Update_State(const _float& fTimeDelta)
{
	if (m_eDir != m_pOwner->GetObj_Dir())
	{
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
	}
	


	if(!dynamic_cast<CPlayer*>(m_pOwner)->Is_Stop())
		Key_Input(fTimeDelta);
	return 0;
}

void CPlayer_State_Idle::LateUpdate_State(void)
{
	if (dynamic_cast<CPlayer*>(m_pOwner)->Is_GetItem())
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::GETTIEM);
	}

	if (dynamic_cast<CPlayer*>(m_pOwner)->Is_BalloonFly())
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::BALLOONFLY);
	}

	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();

}

void CPlayer_State_Idle::Render_State(void)
{
}

void CPlayer_State_Idle::Reset_State(void)
{
}

void CPlayer_State_Idle::Update_Hat()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.005f;
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);
}

void CPlayer_State_Idle::Key_Input(const _float& fTimeDelta)
{
	if (KEY_HOLD(KEY::LEFT_ARROW) || 
		KEY_HOLD(KEY::RIGHT_ARROW) ||
		KEY_HOLD(KEY::UP_ARROW) ||
		KEY_HOLD(KEY::DOWN_ARROW))
	{
		if (KEY_HOLD(KEY::SHIFT))
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::RUN);
		else if (dynamic_cast<CPlayer*>(m_pOwner)->Is_Push())
		{
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::PUSH);
		}
		else
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::MOVE);
	}


	if (KEY_TAP(KEY::X))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::ROLL);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::JUMP);
	}

	if (KEY_HOLD(KEY::A))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::SWING);
	}


	if (KEY_HOLD(KEY::G))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::LIFT);
	}

	if (KEY_HOLD(KEY::D))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::DANCE);
	}

	


	/*if (KEY_TAP(KEY::W))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Set_BalloonFly(true);
	}*/

	if (KEY_TAP(KEY::E))
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::DRAWING);
	}

	if (KEY_TAP(KEY::Q))
	{
		if(dynamic_cast<CPlayer*>(m_pOwner)->Is_HaveSkill())
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::SKILL);
	}

	if (KEY_TAP(KEY::H))
	{
		CEffect_NueTrail* pEffect = CEffect_NueTrail::Create(Get_Device());
		pEffect->Ready_Object();
		_vec3 vPos;
		_vec3 vDir = _vec3(1.0f, 0.0f, 0.0f);
		m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		vPos += vDir;

		pEffect->Set_Effect(m_pOwner, 0.1f, vDir, _vec3(2.0f, 0.0f, 0.0f), _vec3(0.0f, 2.0f, 0.0f), 0.2f);
		Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"NueTrail", pEffect);



		pEffect = CEffect_NueTrail::Create(Get_Device());
		pEffect->Ready_Object();

		pEffect->Set_Effect(m_pOwner, 0.1f, vDir, _vec3(2.0f, 0.0f, 0.0f), _vec3(0.0f, -2.0f, 0.0f), 0.2f);
		Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"NueTrail", pEffect);



		pEffect = CEffect_NueTrail::Create(Get_Device());
		pEffect->Ready_Object();

		pEffect->Set_Effect(m_pOwner, 0.1f, vDir, _vec3(2.0f, 0.0f, 0.0f), _vec3(0.0f, 0.0f, -2.0f), 0.2f);
		Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"NueTrail", pEffect);



		pEffect = CEffect_NueTrail::Create(Get_Device());
		pEffect->Ready_Object();

		pEffect->Set_Effect(m_pOwner, 0.1f, vDir, _vec3(2.0f, 0.0f, 0.0f), _vec3(0.0f, 0.0f, 2.0f), 0.2f);
		Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"NueTrail", pEffect);
	}


}
