#include "../Include/stdafx.h"
#include "Player_State_GetItem.h"
#include "Transform.h"
#include "Player.h"
#include "Texture.h"
#include "KeyMgr.h"
#include "Effect_GetItem.h"
#include "Export_Function.h"
#include "UI_NewItem.h"


CPlayer_State_GetItem::CPlayer_State_GetItem(CGameObject* _pOwner)
	: CPlayer_State(_pOwner)
{
	m_vecHatPos.resize(6, { 0.0f,0.0f,0.0f });
	Set_Hat();
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
	m_bEffect = false;
	m_bReverse = false;

	if (!m_pEffect)
	{
		m_pEffect = CEffect_GetItem::Create(Engine::Get_Device());
	}        
	//추가
	if (!m_pWindow)
		m_pWindow = CUI_NewItem::Create(Engine::Get_Device());
	//
	m_iIndex = 5;

	return S_OK;
}

_int CPlayer_State_GetItem::Update_State(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::Q))
		dynamic_cast<CPlayer*>(m_pOwner)->Set_GetItem(false);



	if (!dynamic_cast<CPlayer*>(m_pOwner)->Is_GetItem())
	{
		dynamic_cast<CEffect_GetItem*>(m_pEffect)->End_Effect();
		if (!m_pEffect->Is_Active())
		{
			m_pOwner->Get_TransformCom()->Move_Pos(&_vec3(0.0f, 1.0f, 0.0f), -10.0f, fTimeDelta);
			_vec3 vPos;
			m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
			
			if (m_iIndex > 0)
				--m_iIndex;
			m_bReverse = true;

			if (vPos.y <= m_vStartPos.y)
			{
				dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->GetCurrAnimation()->Set_Finished(false);
				dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
				m_pOwner->Get_TransformCom()->Set_Pos(&m_vStartPos);
			}
		}
	}
	
	if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() > 2 && !m_bEffect)
	{
		m_pOwner->Get_TransformCom()->Move_Pos(&_vec3(0.0f, 1.0f, 0.0f), 5.0f, fTimeDelta);
		if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Is_Finished())
			m_bEffect = true;
	}

	if (m_bEffect && !m_bFinished)
	{
		_vec3 vPos;

		m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		vPos.z -= 0.05f;
		vPos.y += 2.5f;
		dynamic_cast<CEffect_GetItem*>(m_pEffect)->Get_Effect(vPos, dynamic_cast<CPlayer*>(m_pOwner)->Get_GetItemCode());
		Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"Effect_GetItem", m_pEffect);
		//추가
		dynamic_cast<CUI_NewItem*>(m_pWindow)->Get_ItemInfo(dynamic_cast<CPlayer*>(m_pOwner)->Get_GetItemCode());
		Engine::Get_Layer(LAYER_TYPE::UI)->Add_GameObject(L"UI_GetItem", m_pWindow);
		//
		FMOD_RESULT pRes = FMOD_Channel_SetPaused(Get_Channel(CHANNELID::SOUND_BGM), true);

		Stop_Sound(CHANNELID::SOUND_EFFECT_PLAYER);
		Play_Sound(L"SFX_147_OguItemAdd.wav", CHANNELID::SOUND_EFFECT_PLAYER, .5f);
		m_bFinished = true;


	}
	

	return 0;
}

void CPlayer_State_GetItem::LateUpdate_State(void)
{
	if(m_bReverse)
		m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Idx(m_iIndex);

	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
		Update_Hat();
}

void CPlayer_State_GetItem::Render_State(void)
{
}

void CPlayer_State_GetItem::Reset_State(void)
{
}

void CPlayer_State_GetItem::Update_Hat()
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

void CPlayer_State_GetItem::Set_Hat()
{
	m_vecHatPos[0] = { 0.0f, 0.0f, 0.0f };
	m_vecHatPos[1] = { 0.0f, 0.02f, 0.0f };
	m_vecHatPos[2] = { 0.02f, 0.1f, 0.0f };
	m_vecHatPos[3] = { 0.02f, 0.16f, 0.0f };
	m_vecHatPos[4] = { 0.02f, 0.20f, 0.0f };
	m_vecHatPos[5] = { 0.02f, 0.35f, 0.0f };
	

	m_fAngle[0] = 0.0f;
	m_fAngle[1] = 0.0f;
	m_fAngle[2] = -5.0f;
	m_fAngle[3] = -10.0f;
	m_fAngle[4] = -10.0f;
	m_fAngle[5] = -10.0f;
	

	m_fScale[0] = 1.0f;
	m_fScale[1] = 1.0f;
	m_fScale[2] = 0.93f;
	m_fScale[3] = 0.93f;
	m_fScale[4] = 0.93f;
	m_fScale[5] = 0.93f;


}
