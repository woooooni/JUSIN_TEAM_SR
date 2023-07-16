#include "../Include/stdafx.h"
#include "Player_State_Down.h"
#include "KeyMgr.h"
#include "Player.h"
#include "Texture.h"

CPlayer_State_Down::CPlayer_State_Down(CGameObject* _pOwner)
    : CPlayer_State(_pOwner)
{
}

CPlayer_State_Down::~CPlayer_State_Down()
{
}

HRESULT CPlayer_State_Down::Ready_State(void)
{
    m_pOwner->SetObj_Dir(OBJ_DIR::DIR_D);
    dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Down", FALSE);

    return S_OK;
}

_int CPlayer_State_Down::Update_State(const _float& fTimeDelta)
{
    if (dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Is_Finished())
    {
        if (KEY_TAP(KEY::U))
        {
            dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::GAMEOVER);
        }
        return 0;
    }

    return 0;
}

void CPlayer_State_Down::LateUpdate_State(void)
{
}

void CPlayer_State_Down::Render_State(void)
{
}

void CPlayer_State_Down::Reset_State(void)
{
}
