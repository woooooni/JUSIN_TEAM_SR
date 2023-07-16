#include "../Include/stdafx.h"
#include "Player_State_Drawing.h"
#include "Transform.h"
#include "Player.h"
#include "Texture.h"
#include "KeyMgr.h"

CPlayer_State_Drawing::CPlayer_State_Drawing(CGameObject* _pOwner)
	:CPlayer_State(_pOwner), m_fAccTime(0.0f), m_fDrawTime(2.5f)
{
	m_vecHatPos.resize(18, { 0.0f,0.0f,0.0f });
	Set_Hat();
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
	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();

}

void CPlayer_State_Drawing::Render_State(void)
{
}

void CPlayer_State_Drawing::Reset_State(void)
{
}

void CPlayer_State_Drawing::Update_Hat()
{

	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.0001f;
	vPos += m_vecHatPos[m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();

	float fScale = m_fScale[m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(fScale);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle[m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);

}

void CPlayer_State_Drawing::Set_Hat()
{
	m_vecHatPos[0] =  { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[1] =  { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[2] =  { 0.0f, 0.03f, 0.0f };
	m_vecHatPos[3] =  { 0.0f, 0.03f, 0.0f};
	m_vecHatPos[4] =  { 0.09f, -0.06f, 0.0f };
	m_vecHatPos[5] =  { 0.2f, -0.2f, 0.0f };
	m_vecHatPos[6] =  { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[7] =  { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[8] =  { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[9] =  { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[10] = { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[11] = { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[12] = { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[13] = { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[14] = { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[15] = { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[16] = { 0.65f, -0.7f, 0.0f };
	m_vecHatPos[17] = { 0.65f, -0.7f, 0.0f };

	m_fAngle[0]  = 0.0f;
	m_fAngle[1]  = 0.0f;
	m_fAngle[2]  = 0.0f;
	m_fAngle[3]  = 0.0f;
	m_fAngle[4]  = 0.0f;
	m_fAngle[5]  = 0.0f;
	m_fAngle[6]  = 0.0f;
	m_fAngle[7]  = 0.0f;
	m_fAngle[8]  = 0.0f;
	m_fAngle[9]  = 0.0f;
	m_fAngle[10] = 0.0f;
	m_fAngle[11] = 0.0f;
	m_fAngle[12] = 0.0f;
	m_fAngle[13] = 0.0f;
	m_fAngle[14] = 0.0f;
	m_fAngle[15] = 0.0f;
	m_fAngle[16] = 0.0f;
	m_fAngle[17] = 0.0f;


	m_fScale[0]  = 1.0f;
	m_fScale[1]  = 0.95f;
	m_fScale[2]  = 0.93f;
	m_fScale[3]  = 0.93f;
	m_fScale[4]  = 0.93f;
	m_fScale[5]  = 0.9f;
	m_fScale[6]  = 0.8f;
	m_fScale[7]  = 0.8f;
	m_fScale[8]  = 0.8f;
	m_fScale[9]  = 0.8f;
	m_fScale[10] = 0.8f;
	m_fScale[11] = 0.8f;
	m_fScale[12] = 0.8f;
	m_fScale[13] = 0.8f;
	m_fScale[14] = 0.8f;
	m_fScale[15] = 0.8f;
	m_fScale[16] = 0.8f;
	m_fScale[17] = 0.8f;

}
