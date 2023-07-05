#include "../Include/stdafx.h"
#include "Player_State_Drawing.h"
#include "Transform.h"
#include "Player.h"
#include "Texture.h"
#include "KeyMgr.h"

CPlayer_State_Drawing::CPlayer_State_Drawing(CGameObject* _pOwner)
	:CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fDrawTime(2.5f)
{
}

CPlayer_State_Drawing::~CPlayer_State_Drawing()
{
}

HRESULT CPlayer_State_Drawing::Ready_State(void)
{
	m_pOwner->SetObj_Dir(OBJ_DIR::DIR_D);
	dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Drawing", FALSE);

	m_bFinished = false;

	return S_OK;
}

_int CPlayer_State_Drawing::Update_State(const _float& fTimeDelta)
{
	if (!m_bFinished)
	{
		m_fAccTime += fTimeDelta;
		
		if (m_fAccTime > m_fDrawTime)
		{
			m_bFinished = true;
			m_fAccTime = 0.0f;
		}
		else
		{
			if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Is_Finished())
			{
				m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Idx(15);
				m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Finished(false);
			}
		}
	}


	if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Is_Finished() && m_bFinished)
	{
		if (KEY_TAP(KEY::E))
		{
			dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		}
	}


	return 0;
}

void CPlayer_State_Drawing::LateUpdate_State(void)
{
}

void CPlayer_State_Drawing::Render_State(void)
{
}
