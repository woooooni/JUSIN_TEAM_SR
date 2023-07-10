#include "../Include/stdafx.h"
#include "Player_State_Lift.h"
#include "Transform.h"
#include "Texture.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Collider.h"
#include "KeyMgr.h"

CPlayer_State_Lift::CPlayer_State_Lift(CGameObject* _pOwner)
	: CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fKeyDelayTime(0.05f), m_fLiftTime(0.05f)
{
	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos_LiftUp[i].resize(8, { 0.0f,0.0f,0.0f });
	}
	Set_Hat_LiftUp();

	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos_LiftIdle[i] = { 0.0f,0.0f,0.0f };
	}
	Set_Hat_LiftIdle();

	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos_LiftWalk[i].resize(10, { 0.0f,0.0f,0.0f });
	}
	Set_Hat_LiftWalk();

	for (_uint i = 0; (_uint)OBJ_DIR::DIR_END > i; ++i)
	{
		m_vecHatPos_LiftDown[i].resize(10, { 0.0f,0.0f,0.0f });
	}
	Set_Hat_LiftDown();
}

CPlayer_State_Lift::~CPlayer_State_Lift()
{
}

HRESULT CPlayer_State_Lift::Ready_State(void)
{
	m_eLiftState = LIFT_STATE::LIFTREADY;

	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Offset(_vec3(0.0f, 0.0f, 0.5f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Active(true);
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Offset(_vec3(0.0f, 0.0f, -0.5f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Active(true);
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Offset(_vec3(-0.5f, 0.0f, 0.0f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Active(true);
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Offset(_vec3(0.5f, 0.0f, 0.0f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Active(true);
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Offset(_vec3(-0.5f, 0.0f, -0.5f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Active(true);
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Offset(_vec3(-0.5f, 0.0f, 0.5f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Active(true);
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Offset(_vec3(0.5f, 0.0f, 0.5f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Active(true);
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Offset(_vec3(0.5f, 0.0f, -0.5f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Active(true);
		break;
	}


	m_fAccTime = 0.0f;
	return S_OK;
}

_int CPlayer_State_Lift::Update_State(const _float& fTimeDelta)
{
	switch (m_eLiftState)
	{
	case LIFT_STATE::LIFTREADY:
		Update_LiftReady(fTimeDelta);
		break;
	case LIFT_STATE::LIFTUP:
		Update_LiftUp(fTimeDelta);
		break;
	case LIFT_STATE::LIFTIDLE:
		Update_LiftIdle(fTimeDelta);
		break;
	case LIFT_STATE::LIFTWALK:
		Update_LiftWalk(fTimeDelta);
		break;
	case LIFT_STATE::LIFTDOWN:
		Update_LiftDown(fTimeDelta);
		break;
	}

	return 0;
}

void CPlayer_State_Lift::LateUpdate_State(void)
{
	switch (m_eLiftState)
	{
	case LIFT_STATE::LIFTUP:
		Update_Hat_LiftUp();
		break;
	case LIFT_STATE::LIFTIDLE:
		Update_Hat_LiftIdle();
		break;
	case LIFT_STATE::LIFTWALK:
		Update_Hat_LiftWalk();
		break;
	case LIFT_STATE::LIFTDOWN:
		Update_Hat_LiftDown();
		break;
	}
}

void CPlayer_State_Lift::Render_State(void)
{
}

_int CPlayer_State_Lift::Update_LiftReady(const _float& fTimeDelta)
{

	if (dynamic_cast<CPlayer*>(m_pOwner)->Is_Grab())
	{
		m_eLiftState = LIFT_STATE::LIFTUP;

		switch (m_pOwner->GetObj_Dir())
		{
		case OBJ_DIR::DIR_U:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_Up", FALSE);
			m_vTargetDir = {0.0f, 0.0f, 0.9f};
			break;
		case OBJ_DIR::DIR_D:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_Down", FALSE);
			m_vTargetDir = { 0.0f, 0.0f, -0.9f };
			break;
		case OBJ_DIR::DIR_L:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_Left", FALSE);
			m_vTargetDir = { -0.9f, 0.0f, 0.0f };
			break;
		case OBJ_DIR::DIR_R:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_Right", FALSE);
			m_vTargetDir = { 0.9f, 0.0f, 0.0f };
			break;
		case OBJ_DIR::DIR_LD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_LeftDown", FALSE);
			m_vTargetDir = { -0.9f, 0.0f, -0.9f };
			break;
		case OBJ_DIR::DIR_LU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_LeftUp", FALSE);
			m_vTargetDir = { -0.9f, 0.0f, 0.9f };
			break;
		case OBJ_DIR::DIR_RU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_RightUp", FALSE);
			m_vTargetDir = { 0.9f, 0.0f, 0.9f };
			break;
		case OBJ_DIR::DIR_RD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftUp_RightDown", FALSE);
			m_vTargetDir = { 0.9f, 0.0f, -0.9f };
			break;
		}

		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Offset(_vec3(0.0f, 0.0f, 0.0f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Active(false);
		dynamic_cast<CPlayer*>(m_pOwner)->Set_Grab(false);
		D3DXVec3Cross(&m_vRot, &_vec3(0.0f, 1.0f, 0.0f), &m_vTargetDir);
		m_fAccTime = 0.0f;
	}

	if (m_fAccTime > fTimeDelta)
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Offset(_vec3(0.0f, 0.0f, 0.0f));
		dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerCol(COLLIDER_PLAYER::COLLIDER_GRAB)->Set_Active(false);
		dynamic_cast<CPlayer*>(m_pOwner)->Set_Grab(false);
		m_fAccTime = 0.0f;
	}
	else
	{
		++m_fAccTime += fTimeDelta;
	}

	return 0;
}

_int CPlayer_State_Lift::Update_LiftUp(const _float& fTimeDelta)
{
	_uint iIndex = dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Get_Idx();
	_vec3 vGrabPos;
	dynamic_cast<CPlayer*>(m_pOwner)->Get_TransformCom()->Get_Info(INFO_POS, &vGrabPos);
	_vec3 vTargetPos;
	dynamic_cast<CPlayer*>(m_pOwner)->Get_LiftObj()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);

	

	if (dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Is_Finished())
	{
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		m_eLiftState = LIFT_STATE::LIFTIDLE;
		switch (m_pOwner->GetObj_Dir())
		{
		case OBJ_DIR::DIR_U:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Up", FALSE);
			break;
		case OBJ_DIR::DIR_D:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Down", FALSE);
			break;
		case OBJ_DIR::DIR_L:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Left", FALSE);
			break;
		case OBJ_DIR::DIR_R:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Right", FALSE);
			break;
		case OBJ_DIR::DIR_LD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_LeftDown", FALSE);
			break;
		case OBJ_DIR::DIR_LU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_LeftUp", FALSE);
			break;
		case OBJ_DIR::DIR_RU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_RightUp", FALSE);
			break;
		case OBJ_DIR::DIR_RD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_RightDown", FALSE);
			break;
		}

		vGrabPos += _vec3(0.0f, 0.9f, 0.0f);
		dynamic_cast<CPlayer*>(m_pOwner)->Get_LiftObj()->Get_TransformCom()->Set_Pos(&vGrabPos);
	}
	else
	{
		if (iIndex < 4)
		{
			vGrabPos += m_vTargetDir;
			D3DXVec3Lerp(&vTargetPos, &vTargetPos, &vGrabPos, 0.25f);
			dynamic_cast<CPlayer*>(m_pOwner)->Get_LiftObj()->Get_TransformCom()->Set_Pos(&vTargetPos);

		}
		else if (iIndex == 4)
		{
			vGrabPos += m_vTargetDir;
			dynamic_cast<CPlayer*>(m_pOwner)->Get_LiftObj()->Get_TransformCom()->Set_Pos(&vGrabPos);
		}
		else if (iIndex > 4)
		{
			D3DXQUATERNION  qRot;
			_matrix matRot;
			m_vTargetDir;
			D3DXQuaternionRotationAxis(&qRot, &m_vRot, -0.08f);
			D3DXMatrixRotationQuaternion(&matRot, &qRot);
			D3DXVec3TransformNormal(&m_vTargetDir, &m_vTargetDir, &matRot);

			vGrabPos += m_vTargetDir;
			dynamic_cast<CPlayer*>(m_pOwner)->Get_LiftObj()->Get_TransformCom()->Set_Pos(&vGrabPos);
		}
	}
	return 0;
}

_int CPlayer_State_Lift::Update_LiftIdle(const _float& fTimeDelta)
{
	_vec3 vGrabPos;
	dynamic_cast<CPlayer*>(m_pOwner)->Get_TransformCom()->Get_Info(INFO_POS, &vGrabPos);
	vGrabPos += _vec3(0.0f, 0.9f, 0.0f);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_LiftObj()->Get_TransformCom()->Set_Pos(&vGrabPos);
	


	if (GetAsyncKeyState(VK_UP) & 0x8000 ||
		GetAsyncKeyState(VK_RIGHT) & 0x8000 ||
		GetAsyncKeyState(VK_DOWN) & 0x8000 ||
		GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		m_eLiftState = LIFT_STATE::LIFTWALK;

		m_fAccTime = m_fKeyDelayTime + 1.0f;



		switch (m_pOwner->GetObj_Dir())
		{
		case OBJ_DIR::DIR_U:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_Up", TRUE);
			break;
		case OBJ_DIR::DIR_D:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_Down", TRUE);
			break;
		case OBJ_DIR::DIR_L:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_Left", TRUE);
			break;
		case OBJ_DIR::DIR_R:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_Right", TRUE);
			break;
		case OBJ_DIR::DIR_LD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_LeftDown", TRUE);
			break;
		case OBJ_DIR::DIR_LU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_LeftUp", TRUE);
			break;
		case OBJ_DIR::DIR_RU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_RightUp", TRUE);
			break;
		case OBJ_DIR::DIR_RD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftWalk_RightDown", TRUE);
			break;
		}
	}

	if (KEY_TAP(KEY::G))
	{
		m_eLiftState = LIFT_STATE::LIFTDOWN;


		switch (m_pOwner->GetObj_Dir())
		{
		case OBJ_DIR::DIR_U:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Up", FALSE);
			m_vTargetDir = { 0.0f, 0.0f, 0.9f };
			break;
		case OBJ_DIR::DIR_D:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Down", FALSE);
			m_vTargetDir = { 0.0f, 0.0f, -0.9f };
			break;
		case OBJ_DIR::DIR_L:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Left", FALSE);
			m_vTargetDir = { -0.9f, 0.0f, 0.0f };
			break;
		case OBJ_DIR::DIR_R:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Right", FALSE);
			m_vTargetDir = { 0.9f, 0.0f, 0.0f };
			break;
		case OBJ_DIR::DIR_LD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_LeftDown", FALSE);
			m_vTargetDir = { -0.9f, 0.0f, -0.9f };
			break;
		case OBJ_DIR::DIR_LU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_LeftUp", FALSE);
			m_vTargetDir = { -0.9f, 0.0f, 0.9f };
			break;
		case OBJ_DIR::DIR_RU:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_RightUp", FALSE);
			m_vTargetDir = { 0.9f, 0.0f, 0.9f };
			break;
		case OBJ_DIR::DIR_RD:
			dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_RightDown", FALSE);
			m_vTargetDir = { 0.9f, 0.0f, -0.9f };
			break;
		}
		D3DXVec3Cross(&m_vRot, &_vec3(0.0f, 1.0f, 0.0f), &m_vTargetDir);
		m_vCurrDir = {0.0f, 1.0f, 0.0f};
	}
	return 0;
}

_int CPlayer_State_Lift::Update_LiftDown(const _float& fTimeDelta)
{
	_vec3 vGrabPos;
	dynamic_cast<CPlayer*>(m_pOwner)->Get_TransformCom()->Get_Info(INFO_POS, &vGrabPos);

	if (dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Is_Finished())
	{
		vGrabPos += m_vTargetDir;
		dynamic_cast<CPlayer*>(m_pOwner)->Get_LiftObj()->Get_TransformCom()->Set_Pos(&vGrabPos);
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
		dynamic_cast<CPlayer*>(m_pOwner)->Reset_LiftObj();
	}
	else
	{
	
		

		D3DXQUATERNION  qRot;
		_matrix matRot;
		D3DXQuaternionRotationAxis(&qRot, &m_vRot, 0.038f);
		D3DXMatrixRotationQuaternion(&matRot, &qRot);
		D3DXVec3TransformNormal(&m_vCurrDir, &m_vCurrDir, &matRot);

		vGrabPos += m_vCurrDir;
		dynamic_cast<CPlayer*>(m_pOwner)->Get_LiftObj()->Get_TransformCom()->Set_Pos(&vGrabPos);
	}
	return 0;
}

_int CPlayer_State_Lift::Update_LiftWalk(const _float& fTimeDelta)
{
	dynamic_cast<CTransform*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC))->Move_Pos(m_pOwner->GetObj_Dir(), 2.5f, fTimeDelta);
	_vec3 vGrabPos;
	dynamic_cast<CPlayer*>(m_pOwner)->Get_TransformCom()->Get_Info(INFO_POS, &vGrabPos);
	vGrabPos += _vec3(0.0f, 0.9f, 0.0f);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_LiftObj()->Get_TransformCom()->Set_Pos(&vGrabPos);

	if (m_fAccTime > m_fKeyDelayTime)
	{
		m_fAccTime = 0.0f;
		if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::LEFT_ARROW))
		{
			Change_Dir(OBJ_DIR::DIR_LU, L"LiftWalk_LeftUp");
		}
		else if (KEY_HOLD(KEY::UP_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW))
		{
			Change_Dir(OBJ_DIR::DIR_RU, L"LiftWalk_RightUp");
		}
		else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::RIGHT_ARROW))
		{
			Change_Dir(OBJ_DIR::DIR_RD, L"LiftWalk_RightDown");
		}
		else if (KEY_HOLD(KEY::DOWN_ARROW) && KEY_HOLD(KEY::LEFT_ARROW))
		{
			Change_Dir(OBJ_DIR::DIR_LD, L"LiftWalk_LeftDown");
		}
		else if (KEY_HOLD(KEY::UP_ARROW))
		{
			Change_Dir(OBJ_DIR::DIR_U, L"LiftWalk_Up");
		}
		else if (KEY_HOLD(KEY::DOWN_ARROW))
		{
			Change_Dir(OBJ_DIR::DIR_D, L"LiftWalk_Down");
		}
		else if (KEY_HOLD(KEY::LEFT_ARROW))
		{
			Change_Dir(OBJ_DIR::DIR_L, L"LiftWalk_Left");
		}
		else if (KEY_HOLD(KEY::RIGHT_ARROW))
		{
			Change_Dir(OBJ_DIR::DIR_R, L"LiftWalk_Right");
		}
		else
		{
			m_eLiftState = LIFT_STATE::LIFTIDLE;
			switch (m_pOwner->GetObj_Dir())
			{
			case OBJ_DIR::DIR_U:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Up", FALSE);
				break;
			case OBJ_DIR::DIR_D:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Down", FALSE);
				break;
			case OBJ_DIR::DIR_L:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Left", FALSE);
				break;
			case OBJ_DIR::DIR_R:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_Right", FALSE);
				break;
			case OBJ_DIR::DIR_LD:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_LeftDown", FALSE);
				break;
			case OBJ_DIR::DIR_LU:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_LeftUp", FALSE);
				break;
			case OBJ_DIR::DIR_RU:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_RightUp", FALSE);
				break;
			case OBJ_DIR::DIR_RD:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Lift_RightDown", FALSE);
				break;
			}
		}

		if (KEY_TAP(KEY::G))
		{
			m_eLiftState = LIFT_STATE::LIFTDOWN;


			switch (m_pOwner->GetObj_Dir())
			{
			case OBJ_DIR::DIR_U:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Up", FALSE);
				m_vTargetDir = { 0.0f, 0.0f, 0.9f };
				break;
			case OBJ_DIR::DIR_D:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Down", FALSE);
				m_vTargetDir = { 0.0f, 0.0f, -0.9f };
				break;
			case OBJ_DIR::DIR_L:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Left", FALSE);
				m_vTargetDir = { -0.9f, 0.0f, 0.0f };
				break;
			case OBJ_DIR::DIR_R:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_Right", FALSE);
				m_vTargetDir = { 0.9f, 0.0f, 0.0f };
				break;
			case OBJ_DIR::DIR_LD:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_LeftDown", FALSE);
				m_vTargetDir = { -0.9f, 0.0f, -0.9f };
				break;
			case OBJ_DIR::DIR_LU:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_LeftUp", FALSE);
				m_vTargetDir = { -0.9f, 0.0f, 0.9f };
				break;
			case OBJ_DIR::DIR_RU:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_RightUp", FALSE);
				m_vTargetDir = { 0.9f, 0.0f, 0.9f };
				break;
			case OBJ_DIR::DIR_RD:
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"LiftDown_RightDown", FALSE);
				m_vTargetDir = { 0.9f, 0.0f, -0.9f };
				break;
			}
			D3DXVec3Cross(&m_vRot, &_vec3(0.0f, 1.0f, 0.0f), &m_vTargetDir);
			m_vCurrDir = { 0.0f, 1.0f, 0.0f };
		}
	}
	else
	{
		m_fAccTime += fTimeDelta;
	}


	

	return 0;
}



void CPlayer_State_Lift::Update_Hat_LiftUp()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.0001f;
	vPos += m_vecHatPos_LiftUp[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();

	float fScale = m_fScale_LiftUp[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(fScale);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle_LiftUp[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);
}

void CPlayer_State_Lift::Set_Hat_LiftUp()
{

	//포지션

	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_U][0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_U][1] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_U][2] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_U][3] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_U][4] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_U][5] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_U][6] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_U][7] = { 0.0f, 0.05f, 0.0f };

	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_D][0] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_D][1] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_D][2] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_D][3] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_D][4] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_D][5] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_D][6] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_D][7] = { 0.0f, 0.04f, 0.0f };

	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_L][0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_L][1] = { -0.02f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_L][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_L][3] = { -0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_L][4] = { -0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_L][5] = { 0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_L][6] = { 0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_L][7] = { 0.02f, 0.0f, 0.0f };

	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_R][0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_R][1] = { 0.02f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_R][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_R][3] = { 0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_R][4] = { 0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_R][5] = { -0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_R][6] = { -0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_R][7] = { -0.02f, 0.0f, 0.0f };

	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LU][0] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LU][1] = { -0.07f, 0.03f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LU][2] = { -0.04f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LU][3] = { -0.04f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LU][4] = { -0.06f, 0.01f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LU][5] = { -0.05f, 0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LU][6] = { -0.03f, 0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LU][7] = { 0.1f, -0.09f, 0.0f };

	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LD][0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LD][1] = { 0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LD][2] = { -0.03f, -0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LD][3] = { 0.01f, -0.03f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LD][4] = { -0.01f, -0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LD][5] = { 0.01f, -0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LD][6] = { 0.01f, -0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_LD][7] = { 0.02f, 0.02f, 0.0f };

	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RU][0] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RU][1] = { 0.07f, 0.03f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RU][2] = { 0.04f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RU][3] = { 0.04f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RU][4] = { 0.06f, 0.01f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RU][5] = { 0.05f, 0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RU][6] = { 0.03f, 0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RU][7] = { -0.1f, -0.09f, 0.0f };

	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RD][0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RD][1] = { -0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RD][2] = { 0.03f, -0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RD][3] = { -0.01f, -0.03f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RD][4] = { 0.01f, -0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RD][5] = { -0.01f, -0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RD][6] = { -0.01f, -0.02f, 0.0f };
	m_vecHatPos_LiftUp[(_uint)OBJ_DIR::DIR_RD][7] = { -0.02f, 0.02f, 0.0f };



	//각도
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_U][0] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_U][1] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_U][2] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_U][3] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_U][4] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_U][5] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_U][6] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_U][7] = 0.0f;

	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_D][0] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_D][1] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_D][2] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_D][3] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_D][4] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_D][5] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_D][6] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_D][7] = 0.0f;

	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_L][0] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_L][1] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_L][2] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_L][3] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_L][4] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_L][5] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_L][6] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_L][7] = 0.0f;

	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_R][0] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_R][1] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_R][2] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_R][3] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_R][4] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_R][5] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_R][6] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_R][7] = 0.0f;

	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LU][0] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LU][1] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LU][2] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LU][3] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LU][4] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LU][5] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LU][6] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LU][7] = 0.0f;

	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LD][0] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LD][1] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LD][2] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LD][3] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LD][4] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LD][5] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LD][6] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;

	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RU][0] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RU][1] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RU][2] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RU][3] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RU][4] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RU][5] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RU][6] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RU][7] = 0.0f;

	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RD][0] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RD][1] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RD][2] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RD][3] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RD][4] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RD][5] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RD][6] = 0.0f;
	m_fAngle_LiftUp[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;



	//스케일
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_U][0] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_U][1] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_U][2] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_U][3] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_U][4] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_U][5] = 0.98f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_U][6] = 0.9f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_U][7] = 0.85f;

	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_D][0] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_D][1] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_D][2] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_D][3] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_D][4] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_D][5] = 0.98f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_D][6] = 0.93f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_D][7] = 0.9f;

	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_L][0] = 0.85f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_L][1] = 0.75f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_L][2] = 0.75f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_L][3] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_L][4] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_L][5] = 0.75f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_L][6] = 0.75f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_L][7] = 0.8f;

	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_R][0] = 0.85f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_R][1] = 0.75f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_R][2] = 0.75f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_R][3] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_R][4] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_R][5] = 0.75f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_R][6] = 0.75f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_R][7] = 0.8f;

	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LU][0] = 0.95f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LU][1] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LU][2] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LU][3] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LU][4] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LU][5] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LU][6] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LU][7] = 0.8f;

	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LD][0] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LD][1] = 0.9f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LD][2] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LD][3] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LD][4] = 0.85f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LD][5] = 0.85f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LD][6] = 0.85f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_LD][7] = 0.85f;

	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RU][0] = 0.95f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RU][1] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RU][2] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RU][3] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RU][4] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RU][5] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RU][6] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RU][7] = 0.8f;

	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RD][0] = 1.0f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RD][1] = 0.9f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RD][2] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RD][3] = 0.8f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RD][4] = 0.85f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RD][5] = 0.85f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RD][6] = 0.85f;
	m_fScale_LiftUp[(_uint)OBJ_DIR::DIR_RD][7] = 0.85f;

}

void CPlayer_State_Lift::Update_Hat_LiftIdle()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.0001f;
	vPos += m_vecHatPos_LiftIdle[(_uint)m_pOwner->GetObj_Dir()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();

	float fScale = m_fScale_LiftIdle[(_uint)m_pOwner->GetObj_Dir()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(fScale);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle_LiftIdle[(_uint)m_pOwner->GetObj_Dir()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);
}

void CPlayer_State_Lift::Set_Hat_LiftIdle()
{
	m_vecHatPos_LiftIdle[(_uint)OBJ_DIR::DIR_U] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos_LiftIdle[(_uint)OBJ_DIR::DIR_D] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos_LiftIdle[(_uint)OBJ_DIR::DIR_L] = { -0.01f, -0.03f, 0.0f };
	m_vecHatPos_LiftIdle[(_uint)OBJ_DIR::DIR_R] = { 0.01f, -0.03f, 0.0f };
	m_vecHatPos_LiftIdle[(_uint)OBJ_DIR::DIR_LU] = { 0.03f, -0.02f, 0.0f };
	m_vecHatPos_LiftIdle[(_uint)OBJ_DIR::DIR_LD] = { 0.01f, -0.02f, 0.0f };
	m_vecHatPos_LiftIdle[(_uint)OBJ_DIR::DIR_RU] = { -0.03f, -0.02f, 0.0f };
	m_vecHatPos_LiftIdle[(_uint)OBJ_DIR::DIR_RD] = { -0.01f, -0.02f, 0.0f };



	m_fAngle_LiftIdle[(_uint)OBJ_DIR::DIR_U] = 0.0f;
	m_fAngle_LiftIdle[(_uint)OBJ_DIR::DIR_D] = 0.0f;
	m_fAngle_LiftIdle[(_uint)OBJ_DIR::DIR_L] = 0.0f;
	m_fAngle_LiftIdle[(_uint)OBJ_DIR::DIR_R] = 0.0f;
	m_fAngle_LiftIdle[(_uint)OBJ_DIR::DIR_LU] = 0.0f;
	m_fAngle_LiftIdle[(_uint)OBJ_DIR::DIR_LD] = 0.0f;
	m_fAngle_LiftIdle[(_uint)OBJ_DIR::DIR_RU] = 0.0f;
	m_fAngle_LiftIdle[(_uint)OBJ_DIR::DIR_RD] = 0.0f;


	m_fScale_LiftIdle[(_uint)OBJ_DIR::DIR_U] = 0.95f;
	m_fScale_LiftIdle[(_uint)OBJ_DIR::DIR_D] = 0.95f;
	m_fScale_LiftIdle[(_uint)OBJ_DIR::DIR_L] = 0.85f;
	m_fScale_LiftIdle[(_uint)OBJ_DIR::DIR_R] = 0.85f;
	m_fScale_LiftIdle[(_uint)OBJ_DIR::DIR_LU] = 0.8f;
	m_fScale_LiftIdle[(_uint)OBJ_DIR::DIR_LD] = 0.95f;
	m_fScale_LiftIdle[(_uint)OBJ_DIR::DIR_RU] = 0.8f;
	m_fScale_LiftIdle[(_uint)OBJ_DIR::DIR_RD] = 0.95f;
}


void CPlayer_State_Lift::Update_Hat_LiftWalk()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.0001f;
	vPos += m_vecHatPos_LiftWalk[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();

	float fScale = m_fScale_LiftWalk[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(fScale);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle_LiftWalk[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);
}

void CPlayer_State_Lift::Set_Hat_LiftWalk()
{
	//포지션

	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_U][0] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_U][1] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_U][2] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_U][3] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_U][4] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_U][5] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_U][6] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_U][7] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_U][8] = { 0.01f, 0.05f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_U][9] = { 0.0f, 0.04f, 0.0f };

	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_D][0] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_D][1] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_D][2] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_D][3] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_D][4] = { 0.01f, 0.05f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_D][5] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_D][6] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_D][7] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_D][8] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_D][9] = { 0.0f, 0.04f, 0.0f };

	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_L][0] = { 0.0f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_L][1] = { -0.02f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_L][2] = { -0.04f, 0.0f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_L][3] = { -0.04f, 0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_L][4] = { -0.05f, 0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_L][5] = { -0.01f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_L][6] = { -0.03f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_L][7] = { -0.05f, -0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_L][8] = { -0.05f, -0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_L][9] = { -0.04f, 0.0f, 0.0f };

	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_R][0] = { 0.0f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_R][1] = { 0.02f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_R][2] = { 0.04f, 0.0f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_R][3] = { 0.04f, 0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_R][4] = { 0.05f, 0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_R][5] = { 0.01f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_R][6] = { 0.03f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_R][7] = { 0.05f, -0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_R][8] = { 0.05f, -0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_R][9] = { 0.04f, 0.0f, 0.0f };

	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LU][0] = { 0.05f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LU][1] = { 0.0f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LU][2] = { 0.0f, -0.04f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LU][3] = { 0.01f, -0.04f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LU][4] = { 0.02f, -0.04f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LU][5] = { 0.03f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LU][6] = { -0.02f, 0.0f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LU][7] = { 0.02f, 0.0f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LU][8] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LU][9] = { 0.03f, 0.01f, 0.0f };

	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LD][0] = { 0.0f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LD][1] = { -0.03f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LD][2] = { -0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LD][3] = { 0.0f, -0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LD][4] = { -0.01f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LD][5] = { -0.01f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LD][6] = { 0.0f, -0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LD][7] = { 0.0f, -0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LD][8] = { -0.01f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_LD][9] = { -0.02f, -0.04f, 0.0f };

	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RD][0] = { 0.0f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RD][1] = { 0.03f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RD][2] = { 0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RD][3] = { 0.0f, -0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RD][4] = { 0.01f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RD][5] = { 0.01f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RD][6] = { 0.0f, -0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RD][7] = { 0.0f, -0.01f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RD][8] = { 0.01f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RD][9] = { 0.02f, -0.04f, 0.0f };

	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RU][0] = { -0.05f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RU][1] = { 0.0f, -0.02f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RU][2] = { 0.0f, -0.04f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RU][3] = { -0.01f, -0.04f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RU][4] = { -0.02f, -0.04f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RU][5] = { -0.03f, -0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RU][6] = { 0.02f, 0.0f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RU][7] = { -0.02f, 0.0f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RU][8] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftWalk[(_uint)OBJ_DIR::DIR_RU][9] = { -0.03f, 0.01f, 0.0f };


	



	//각도
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_U][0] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_U][1] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_U][2] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_U][3] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_U][4] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_U][5] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_U][6] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_U][7] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_U][8] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_U][9] = 0.0f;

	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_D][0] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_D][1] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_D][2] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_D][3] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_D][4] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_D][5] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_D][6] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_D][7] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_D][8] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_D][9] = 0.0f;

	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_L][0] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_L][1] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_L][2] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_L][3] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_L][4] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_L][5] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_L][6] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_L][7] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_L][8] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_L][9] = 0.0f;

	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_R][0] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_R][1] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_R][2] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_R][3] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_R][4] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_R][5] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_R][6] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_R][7] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_R][8] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_R][9] = 0.0f;

	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LU][0] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LU][1] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LU][2] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LU][3] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LU][4] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LU][5] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LU][6] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LU][7] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LU][8] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LU][9] = 0.0f;

	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LD][0] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LD][1] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LD][2] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LD][3] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LD][4] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LD][5] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LD][6] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LD][8] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_LD][9] = 0.0f;

	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RU][0] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RU][1] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RU][2] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RU][3] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RU][4] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RU][5] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RU][6] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RU][7] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RU][8] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RU][9] = 0.0f;

	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RD][0] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RD][1] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RD][2] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RD][3] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RD][4] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RD][5] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RD][6] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RD][8] = 0.0f;
	m_fAngle_LiftWalk[(_uint)OBJ_DIR::DIR_RD][9] = 0.0f;


	//스케일
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_U][0] = 0.95f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_U][1] = 0.95f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_U][2] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_U][3] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_U][4] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_U][5] = 0.95f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_U][6] = 0.95f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_U][7] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_U][8] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_U][9] = 0.9f;

	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_D][0] = 0.95f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_D][1] = 0.95f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_D][2] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_D][3] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_D][4] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_D][5] = 0.95f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_D][6] = 0.95f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_D][7] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_D][8] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_D][9] = 0.9f;

	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_L][0] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_L][1] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_L][2] = 0.75f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_L][3] = 0.75f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_L][4] = 0.75f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_L][5] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_L][6] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_L][7] = 0.75f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_L][8] = 0.75f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_L][9] = 0.8f;

	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_R][0] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_R][1] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_R][2] = 0.75f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_R][3] = 0.75f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_R][4] = 0.75f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_R][5] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_R][6] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_R][7] = 0.75f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_R][8] = 0.75f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_R][9] = 0.8f;

	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LU][0] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LU][1] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LU][2] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LU][3] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LU][4] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LU][5] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LU][6] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LU][7] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LU][8] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LU][9] = 0.8f;


	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LD][0] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LD][1] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LD][2] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LD][3] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LD][4] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LD][5] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LD][6] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LD][7] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LD][8] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_LD][9] = 0.9f;

	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RU][0] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RU][1] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RU][2] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RU][3] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RU][4] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RU][5] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RU][6] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RU][7] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RU][8] = 0.8f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RU][9] = 0.8f;

	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RD][0] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RD][1] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RD][2] = 0.85f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RD][3] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RD][4] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RD][5] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RD][6] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RD][7] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RD][8] = 0.9f;
	m_fScale_LiftWalk[(_uint)OBJ_DIR::DIR_RD][9] = 0.9f;


	
}

void CPlayer_State_Lift::Update_Hat_LiftDown()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.0001f;
	vPos += m_vecHatPos_LiftDown[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();

	float fScale = m_fScale_LiftDown[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(fScale);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle_LiftDown[(_uint)m_pOwner->GetObj_Dir()][m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);
}

void CPlayer_State_Lift::Set_Hat_LiftDown()
{

	//포지션

	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_U][0] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_U][1] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_U][2] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_U][3] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_U][4] = { 0.01f, 0.05f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_U][5] = { 0.01f, 0.05f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_U][6] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_U][7] = { 0.0f, 0.05f, 0.0f };

	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_D][0] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_D][1] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_D][2] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_D][3] = { 0.0f, 0.04f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_D][4] = { 0.0f, 0.05f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_D][5] = { -0.01f, 0.05f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_D][6] = { -0.01f, 0.05f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_D][7] = { -0.01f, 0.03f, 0.0f };

	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_L][0] = { -0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_L][1] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_L][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_L][3] = { -0.03f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_L][4] = { -0.03f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_L][5] = { -0.03f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_L][6] = { -0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_L][7] = { 0.01f, 0.01f, 0.0f };

	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_R][0] = { 0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_R][1] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_R][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_R][3] = { 0.03f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_R][4] = { 0.03f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_R][5] = { 0.03f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_R][6] = { 0.01f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_R][7] = { -0.01f, 0.01f, 0.0f };

	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LU][0] = { 0.02f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LU][1] = { 0.01f, -0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LU][2] = { 0.01f, -0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LU][3] = { -0.03f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LU][4] = { -0.02f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LU][5] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LU][6] = { 0.01f, 0.04f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LU][7] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LD][0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LD][1] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LD][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LD][3] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LD][4] = { 0.02f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LD][5] = { 0.02f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LD][6] = { 0.04f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_LD][7] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RU][0] = { -0.02f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RU][1] = { -0.01f, -0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RU][2] = { -0.01f, -0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RU][3] = { 0.03f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RU][4] = { 0.02f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RU][5] = { 0.0f, 0.03f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RU][6] = { -0.01f, 0.04f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RU][7] = { 0.0f, 0.0f, 0.0f };

	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RD][0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RD][1] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RD][2] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RD][3] = { 0.0f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RD][4] = { -0.02f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RD][5] = { -0.02f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RD][6] = { -0.04f, 0.01f, 0.0f };
	m_vecHatPos_LiftDown[(_uint)OBJ_DIR::DIR_RD][7] = { 0.0f, 0.0f, 0.0f };



	//각도
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_U][0] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_U][1] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_U][2] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_U][3] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_U][4] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_U][5] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_U][6] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_U][7] = 0.0f;

	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_D][0] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_D][1] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_D][2] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_D][3] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_D][4] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_D][5] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_D][6] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_D][7] = 0.0f;

	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_L][0] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_L][1] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_L][2] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_L][3] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_L][4] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_L][5] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_L][6] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_L][7] = 0.0f;

	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_R][0] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_R][1] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_R][2] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_R][3] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_R][4] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_R][5] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_R][6] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_R][7] = 0.0f;

	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LU][0] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LU][1] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LU][2] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LU][3] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LU][4] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LU][5] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LU][6] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LU][7] = 0.0f;

	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LD][0] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LD][1] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LD][2] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LD][3] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LD][4] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LD][5] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LD][6] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;

	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RU][0] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RU][1] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RU][2] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RU][3] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RU][4] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RU][5] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RU][6] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RU][7] = 0.0f;

	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RD][0] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RD][1] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RD][2] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RD][3] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RD][4] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RD][5] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RD][6] = 0.0f;
	m_fAngle_LiftDown[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;



	//스케일
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_U][0] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_U][1] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_U][2] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_U][3] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_U][4] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_U][5] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_U][6] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_U][7] = 0.95f;

	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_D][0] = 0.95f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_D][1] = 0.95f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_D][2] = 0.95f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_D][3] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_D][4] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_D][5] = 0.95f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_D][6] = 0.95f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_D][7] = 0.95f;

	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_L][0] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_L][1] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_L][2] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_L][3] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_L][4] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_L][5] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_L][6] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_L][7] = 0.9f;

	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_R][0] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_R][1] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_R][2] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_R][3] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_R][4] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_R][5] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_R][6] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_R][7] = 0.9f;

	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LU][0] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LU][1] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LU][2] = 0.75f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LU][3] = 0.7f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LU][4] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LU][5] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LU][6] = 1.0f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LU][7] = 0.0f;

	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LD][0] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LD][1] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LD][2] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LD][3] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LD][4] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LD][5] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LD][6] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_LD][7] = 0.0f;

	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RU][0] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RU][1] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RU][2] = 0.75f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RU][3] = 0.7f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RU][4] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RU][5] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RU][6] = 1.0f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RU][7] = 0.0f;

	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RD][0] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RD][1] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RD][2] = 0.9f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RD][3] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RD][4] = 0.8f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RD][5] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RD][6] = 0.85f;
	m_fScale_LiftDown[(_uint)OBJ_DIR::DIR_RD][7] = 0.0f;

}
