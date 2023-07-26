#include "../Include/stdafx.h"
#include "Player_State_BallonFly.h"
#include "Player.h"
#include "Texture.h"
#include "Transform.h"
#include "KeyMgr.h"


Player_State_BallonFly::Player_State_BallonFly(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
}

Player_State_BallonFly::~Player_State_BallonFly()
{
}

HRESULT Player_State_BallonFly::Ready_State(void)
{
	m_pOwner->SetObj_Dir(OBJ_DIR::DIR_D);
	dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"BalloonFly", FALSE);

	m_bFinished = false;
	m_eFlyState = BALLOONFLY_STATE::FLYREADY;

	if(dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Active(false);

	return S_OK;
}

_int Player_State_BallonFly::Update_State(const _float& fTimeDelta)
{
	switch (m_eFlyState)
	{
	case BALLOONFLY_STATE::FLYREADY:
		Update_FlyReady(fTimeDelta);
		break;
	case BALLOONFLY_STATE::FLYING:
		Update_Flying(fTimeDelta);
		if (KEY_TAP(KEY::W))
			dynamic_cast<CPlayer*>(m_pOwner)->Set_BalloonFly(false);
		break;
	case BALLOONFLY_STATE::FLYINGEND:
		Update_FlyEnd(fTimeDelta);
		break;
	}

	

	return 0;
}

void Player_State_BallonFly::LateUpdate_State(void)
{
}

void Player_State_BallonFly::Render_State(void)
{
}

void Player_State_BallonFly::Reset_State(void)
{
	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Active(true);
}

_int Player_State_BallonFly::Update_FlyReady(const _float& fTimeDelta)
{
	if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() > 12)
		m_pOwner->Get_TransformCom()->Move_Pos(&_vec3(0.0f, 1.0f, 0.0f), 10.0f, fTimeDelta);

	if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() > 16)
		m_eFlyState = BALLOONFLY_STATE::FLYING;
	return 0;
}

_int Player_State_BallonFly::Update_Flying(const _float& fTimeDelta)
{
	if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() == 18)
		m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Idx(17);

	m_pOwner->Get_TransformCom()->Move_Pos(&_vec3(0.0f, 1.0f, 0.0f), 10.0f, fTimeDelta);

	if (!dynamic_cast<CPlayer*>(m_pOwner)->Is_BalloonFly())
		m_eFlyState = BALLOONFLY_STATE::FLYINGEND;

	return 0;
}

_int Player_State_BallonFly::Update_FlyEnd(const _float& fTimeDelta)
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	if (vPos.y > m_pOwner->Get_MinHeight())
	{
		m_pOwner->Get_TransformCom()->Move_Pos(&_vec3(0.0f, -1.0f, 0.0f), 10.0f, fTimeDelta);

		if (vPos.y > 3.0f)
		{
			if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() == 18)
				m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Idx(17);
		}

	}
	else
	{
		vPos.y = m_pOwner->Get_MinHeight();
		m_pOwner->Get_TransformCom()->Set_Pos(&vPos);

		if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Is_Finished())
		{
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
			//dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Active(true);
		}

	}

	return 0;
}



