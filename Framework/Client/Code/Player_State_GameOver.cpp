#include "../Include/stdafx.h"
#include "Player_State_GameOver.h"
#include "Transform.h"
#include "Player.h"
#include "Texture.h"
#include "Export_Function.h"

CPlayer_State_GameOver::CPlayer_State_GameOver(CGameObject* _pOwner)
	:CPlayer_State(_pOwner)
{
}

CPlayer_State_GameOver::~CPlayer_State_GameOver()
{
}

HRESULT CPlayer_State_GameOver::Ready_State(void)
{
	dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"GameOver", FALSE);

	Stop_Sound(CHANNELID::SOUND_EFFECT_PLAYER);
	Play_Sound(L"SFX_238_OguGameRestart.wav", CHANNELID::SOUND_EFFECT_PLAYER, 0.7f);


	return S_OK;
}

_int CPlayer_State_GameOver::Update_State(const _float& fTimeDelta)
{
	if (dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Is_Finished())
	{
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerStat().iHp = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerStat().iMaxHp;
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);

		if (nullptr != dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
			dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Active(true);
	
		m_pOwner->SetObj_Dir(OBJ_DIR::DIR_U);
	}
	return 0;
}

void CPlayer_State_GameOver::LateUpdate_State(void)
{
}

void CPlayer_State_GameOver::Render_State(void)
{
}

void CPlayer_State_GameOver::Reset_State(void)
{
}
