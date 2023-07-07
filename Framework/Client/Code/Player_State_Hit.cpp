#include "../Include/stdafx.h"
#include "Player_State_Hit.h"
#include "Texture.h"
#include "Player.h"
#include "Transform.h"
#include "RigidBody.h"

CPlayer_State_Hit::CPlayer_State_Hit(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
}

CPlayer_State_Hit::~CPlayer_State_Hit()
{

}

HRESULT CPlayer_State_Hit::Ready_State(void)
{
	_vec3 vDir;
	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_Up", FALSE);
		vDir = { 0.0f, 0.0f, -1.0f };
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_Down", FALSE);
		vDir = { 0.0f, 0.0f, 1.0f };
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_Left", FALSE);
		vDir = { 1.0f, 0.0f, 0.0f };
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_Right", FALSE);
		vDir = { -1.0f, 0.0f, 0.0f };
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_LeftDown", FALSE);
		vDir = { 1.0f, 0.0f, 1.0f };
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_LeftUp", FALSE);
		vDir = { 1.0f, 0.0f, -1.0f };
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_RightUp", FALSE);
		vDir = { -1.0f, 0.0f, -1.0f };
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Hit_RightDown", FALSE);
		vDir = { -1.0f, 0.0f, 1.0f };
		break;
	}

	D3DXVec3Normalize(&vDir, &vDir);

	m_pOwner->Get_RigidBodyCom()->AddForce(vDir * 80.0f);

	return S_OK;
}

_int CPlayer_State_Hit::Update_State(const _float& fTimeDelta)
{
	return 0;
}

void CPlayer_State_Hit::LateUpdate_State(void)
{
	if (dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Is_Finished())
	{
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
	}
}

void CPlayer_State_Hit::Render_State(void)
{
}
