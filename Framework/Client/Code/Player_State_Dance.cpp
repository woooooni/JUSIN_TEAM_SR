#include "Player_State_Dance.h"

#include "Export_Function.h"
#include "Player_State_Drawing.h"
#include "Transform.h"
#include "Player.h"
#include "Texture.h"
#include "KeyMgr.h"

#include "Pool.h"

CPlayer_State_Dance::CPlayer_State_Dance(CGameObject* _pOwner)
	:CPlayer_State(_pOwner), m_fHealTime(1.0f)
{
	m_vecHatPos.resize(14, { 0.0f,0.0f,0.0f });
	Set_Hat();
}

CPlayer_State_Dance::~CPlayer_State_Dance()
{
}

HRESULT CPlayer_State_Dance::Ready_State(void)
{
	m_pOwner->SetObj_Dir(OBJ_DIR::DIR_D);
	dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Dance", true);

	m_bFinished = false;

	m_pOwner->Get_TransformCom()->Set_Scale(_vec3(1.7f, 1.7f, 1.7f));
	dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"))->Set_TargetObj(nullptr);

	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.z -= 0.05f;
	vPos.y += 0.3f;

	m_pEffect = CPool<CEffect_CircleBlur>::Get_Obj();
	if (!m_pEffect)
	{
		m_pEffect = CEffect_CircleBlur::Create(Get_Device());
		NULL_CHECK_RETURN(m_pEffect, E_FAIL);
		m_pEffect->Ready_Object();
	}

	D3DCOLORVALUE vColor = { 32.0f / 255.0f, 222.0f / 255.0f, 7.0f / 255.0f, 0.0f};
	D3DCOLORVALUE vEmissive = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_pEffect->Set_Effect(vPos, _vec3(3.0f, 2.5f, 3.0f), vColor, vEmissive);
	Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"CircleBlur", m_pEffect);

	m_bDown = false;
	m_fAccTime = 0.0f;



	return S_OK;
}

_int CPlayer_State_Dance::Update_State(const _float& fTimeDelta)
{
	_int iAlpha = m_pEffect->Get_Alpha();

	if (!m_bDown)
	{
		iAlpha += 3;
		if (iAlpha >= 200)
		{
			iAlpha = 200;
			m_bDown = true;
		}
	}
	else
	{
		iAlpha -= 3;
		if (iAlpha <= 50)
		{
			iAlpha = 50;
			m_bDown = false;
		}
	}

	m_pEffect->Set_Alpha(iAlpha);

	if (m_fAccTime >= m_fHealTime)
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Add_HP(1);
		m_fAccTime = 0.0f;
	}
	else
		m_fAccTime += fTimeDelta;


	Key_Input(fTimeDelta);
	return 0;
}

void CPlayer_State_Dance::LateUpdate_State(void)
{
	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();

	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y = m_pOwner->Get_MinHeight() + 0.1f;
	m_pOwner->Get_TransformCom()->Set_Pos(&vPos);
}

void CPlayer_State_Dance::Render_State(void)
{
}

void CPlayer_State_Dance::Reset_State(void)
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y = m_pOwner->Get_MinHeight();
	m_pOwner->Get_TransformCom()->Set_Pos(&vPos);
	dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"))->Set_TargetObj(m_pOwner);

	m_pEffect->Set_Active(false);
	CPool<CEffect_CircleBlur>::Return_Obj(m_pEffect);

	m_pOwner->Get_TransformCom()->Set_Scale(_vec3(1.0f, 1.0f, 1.0f));
}

void CPlayer_State_Dance::Key_Input(const _float& fTimeDelta)
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
}

void CPlayer_State_Dance::Update_Hat()
{
	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vPos.y += 0.3f;
	vPos.z -= 0.005f;
	vPos += m_vecHatPos[m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();

	float fScale = m_fScale[m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()];
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(fScale);
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Angle(D3DXToRadian(m_fAngle[m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx()]));
	dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);
}

void CPlayer_State_Dance::Set_Hat()
{
	m_vecHatPos[0] =  { 0.0f, 0.08f, 0.0f };
	m_vecHatPos[1] =  { -0.01f, -0.02f, 0.0f };
	m_vecHatPos[2] =  { 0.0f, -0.07f, 0.0f };
	m_vecHatPos[3] =  { 0.01f, -0.1f, 0.0f };
	m_vecHatPos[4] =  { -0.025f, 0.25f, 0.0f };
	m_vecHatPos[5] =  { -0.025f, 0.34f, 0.0f };
	m_vecHatPos[6] =  { -0.025f, 0.23f, 0.0f };
	m_vecHatPos[7] =  { 0.025f, 0.09f, 0.0f };
	m_vecHatPos[8] =  { 0.025f, 0.0f, 0.0f };
	m_vecHatPos[9] =  { 0.0f, -0.05f, 0.0f };
	m_vecHatPos[10] = { 0.0f, -0.1f, 0.0f };
	m_vecHatPos[11] = { 0.0f, 0.26f, 0.0f };
	m_vecHatPos[12] = { 0.0f, 0.34f, 0.0f };
	m_vecHatPos[13] = { 0.0f, 0.23f, 0.0f };


	m_fAngle[0] = 5.0f;
	m_fAngle[1] = 5.0f;
	m_fAngle[2] = 3.0f;
	m_fAngle[3] = 0.0f;
	m_fAngle[4] = 0.0f;
	m_fAngle[5] = 0.0f;
	m_fAngle[6] = 0.0f;
	m_fAngle[7] = -5.0f;
	m_fAngle[8] = -5.0f;
	m_fAngle[9] = -5.0f;
	m_fAngle[10] = -3.0f;
	m_fAngle[11] = 0.0f;
	m_fAngle[12] = 0.0f;
	m_fAngle[13] = 0.0f;



	m_fScale[0] =  0.98f;
	m_fScale[1] =  0.98f;
	m_fScale[2] =  0.98f;
	m_fScale[3] =  0.98f;
	m_fScale[4] =  0.98f;
	m_fScale[5] =  0.98f;
	m_fScale[6] =  0.98f;
	m_fScale[7] =  0.98f;
	m_fScale[8] =  0.98f;
	m_fScale[9] =  0.98f;
	m_fScale[10] = 0.98f;
	m_fScale[11] = 0.98f;
	m_fScale[12] = 0.98f;
	m_fScale[13] = 0.98f;

}
