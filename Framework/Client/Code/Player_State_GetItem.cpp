#include "../Include/stdafx.h"
#include "Player_State_GetItem.h"
#include "Transform.h"
#include "Player.h"
#include "Texture.h"
#include "KeyMgr.h"


CPlayer_State_GetItem::CPlayer_State_GetItem(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
}

CPlayer_State_GetItem::~CPlayer_State_GetItem()
{
}

HRESULT CPlayer_State_GetItem::Ready_State(void)
{
	m_pOwner->SetObj_Dir(OBJ_DIR::DIR_D);
	dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"GetItem", FALSE);

	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &m_vStartPos);
	m_bFinished = false;

	return S_OK;
}

_int CPlayer_State_GetItem::Update_State(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::Q))
		dynamic_cast<CPlayer*>(m_pOwner)->Set_GetItem(false);



	if (!dynamic_cast<CPlayer*>(m_pOwner)->Is_GetItem())
	{
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
		m_pOwner->Get_TransformCom()->Set_Pos(&m_vStartPos);
	}
	
	if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() > 2 && !m_bFinished)
	{
		m_pOwner->Get_TransformCom()->Move_Pos(&_vec3(0.0f, 1.0f, 0.0f), 10.0f, fTimeDelta);
		if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Is_Finished())
			m_bFinished = true;
	}
	

	return 0;
}

void CPlayer_State_GetItem::LateUpdate_State(void)
{
}

void CPlayer_State_GetItem::Render_State(void)
{
}
