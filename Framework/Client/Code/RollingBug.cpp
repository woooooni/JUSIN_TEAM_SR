#include "RollingBug.h"
#include "Export_Function.h"
#include "GameMgr.h"
#include "InteractionMgr.h"
#include "LightFlower.h"
#include "Pool.h"
#include "Effect_Stun.h"

CRollingBug::CRollingBug(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev, OBJ_ID::ROLLING_BUG), m_fMoveTime(0.f)
{
}

CRollingBug::CRollingBug(const CRollingBug& rhs)
	: CMonster(rhs), m_fMoveTime(rhs.m_fMoveTime)
{
}

CRollingBug::~CRollingBug()
{
}

HRESULT CRollingBug::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_State(MONSTER_STATE::IDLE);
	Set_Speed(3.5f);

	m_tStat = { 3, 3, 1 };

	_vec3 vScale = _vec3(1.f, 1.f, 1.f);
	m_pTransformCom->Set_Scale(vScale);

	m_pUIBack = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_BACK);
	if (m_pUIBack != nullptr)
		m_pUIBack->Set_Owner(this);

	m_pUIGauge = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_GAUGE);
	if (m_pUIGauge != nullptr)
		m_pUIGauge->Set_Owner(this);

	m_pUIFrame = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_FRAME);
	if (m_pUIFrame != nullptr)
		m_pUIFrame->Set_Owner(this);

	m_fMinHeight = 0.5f;

	return S_OK;
}

_int CRollingBug::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	__super::Update_Object(fTimeDelta);
	Engine::Add_CollisionGroup(m_pColliderCom, COLLIDE_STATE::COLLIDE_MONSTER);

	CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();

	if (nullptr == pTarget)
		return S_OK;

	Set_Target(pTarget);

	_vec3 vPlayerPos, vOriginPos, vPos;
	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (Get_State() != MONSTER_STATE::STUN)
	{
		
		vOriginPos = m_tBugInfo.vDefaultPos;
		m_vBugDir = vOriginPos - vPos;
		m_vPlayerDir = vPlayerPos - vPos;

		if (D3DXVec3Length(&m_vPlayerDir) <= 15.f)
		{
			m_vLook = m_vPlayerDir;
			Set_State(MONSTER_STATE::ATTACK);
		}

		if (D3DXVec3Length(&m_vPlayerDir) > 15.f)
		{
			m_vLook = m_vBugDir;
			Set_State(MONSTER_STATE::MOVE);

			if (D3DXVec3Length(&m_vBugDir) < 1.f)
			{
				m_vLook = _vec3(0.f, 0.f, -1.f);
				Set_State(MONSTER_STATE::IDLE);
			}
		}
	}

	vPos.y += 0.5f;
	vPos.z -= 0.01f;

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Update_Object(fTimeDelta);
		m_pUIBack->Get_TransformCom()->Set_Pos(&vPos);

		vPos.z -= 0.005f;
		m_pUIGauge->Update_Object(fTimeDelta);

		if (m_tStat.iHp == m_tStat.iMaxHp)
			m_pUIGauge->Get_TransformCom()->Set_Pos(&vPos);

		else if (m_tStat.iHp > 0 && m_tStat.iHp < m_tStat.iMaxHp)
		{
			_vec3 vMovePos = vPos;

			_float fMaxHP = _float(m_tStat.iMaxHp);
			_float fCurHP = _float(m_tStat.iHp);
			_float fHP = fCurHP / fMaxHP;

			_float fOriginWidth = _float(m_pUIGauge->Get_TextureCom()->Get_TextureDesc(0).Width);
			_float fWidth = fOriginWidth - fOriginWidth * fHP;

			_float fIndex = fWidth * 0.004f * 0.5f;

			vMovePos = _vec3((vMovePos.x - fIndex), vMovePos.y, vMovePos.z);
			m_pUIGauge->Get_TransformCom()->Set_Pos(&vMovePos);
		}

		vPos.z -= 0.005f;
		m_pUIFrame->Update_Object(fTimeDelta);
		m_pUIFrame->Get_TransformCom()->Set_Pos(&vPos);
	}

	
	return S_OK;
}

void CRollingBug::LateUpdate_Object(void)
{
	if (!Is_Active())
		return ;

	Set_Animation();
	__super::LateUpdate_Object();

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->LateUpdate_Object();
		m_pUIGauge->LateUpdate_Object();
		m_pUIFrame->LateUpdate_Object();
	}

	CGameObject* pLightFlower = Engine::Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Find_GameObject(L"LightFlower");
	
	_vec3 vFlowerPos, vPos, vDir;
	pLightFlower->Get_TransformCom()->Get_Info(INFO_POS, &vFlowerPos);

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vDir = vFlowerPos - vPos;

	_bool bIsOpened = dynamic_cast<CLightFlower*>(pLightFlower)->m_bIsOpened;

	if (bIsOpened && D3DXVec3Length(&vDir) <= 5.f)
	{
		if (JELLY_COLOR::BLUE == dynamic_cast<CLightFlower*>(pLightFlower)->m_eColor)
			if (m_tBugInfo.eType == BUGCOLORTYPE::BLUE)
				Set_Stun(0.1f);

		if (JELLY_COLOR::MAGENTA == dynamic_cast<CLightFlower*>(pLightFlower)->m_eColor)
			if (m_tBugInfo.eType == BUGCOLORTYPE::PINK)
				Set_Stun(0.1f);

		if (JELLY_COLOR::YELLOW == dynamic_cast<CLightFlower*>(pLightFlower)->m_eColor)
			if (m_tBugInfo.eType == BUGCOLORTYPE::YELLOW)
				Set_Stun(0.1f);

	}

	if (!bIsOpened)
	{
		if (Get_State() == MONSTER_STATE::STUN)
		{
			m_fStunTime = 0.0f;
			Set_State(MONSTER_STATE::IDLE);
		}
	}		

}

void CRollingBug::Render_Object(void)
{
	if (!Is_Active())
		return ;

	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;


	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

	MATERIAL.material.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	MATERIAL.material.Diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	MATERIAL.material.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	MATERIAL.material.Power = 0.0f;

	pEffect->SetValue("g_Material", &MATERIAL.material, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(0);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Render_Object();
		m_pUIGauge->Render_Object();
		m_pUIFrame->Render_Object();
	}
}

void CRollingBug::Update_Idle(_float fTimeDelta)
{
	_vec3 vPos, vDir, vOriginPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vOriginPos = m_tBugInfo.vDefaultPos;

	vDir = vOriginPos - vPos;
	m_vBugDir = vDir;

	if (D3DXVec3Length(&m_vPlayerDir) <= 15.f)
	{
		D3DXVec3Normalize(&m_vBugDir, &m_vBugDir);
		m_pTransformCom->Move_Pos(&m_vBugDir, fTimeDelta, 1.5f * Get_Speed());
	}

	m_fMoveTime += 10.f * fTimeDelta;

}

void CRollingBug::Update_Regen(_float fTimeDelta)
{
}

void CRollingBug::Update_Move(_float fTimeDelta)
{
	_vec3 vPos, vDir, vOriginPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vOriginPos = m_tBugInfo.vDefaultPos;

	vDir = vOriginPos - vPos;
	m_vBugDir = vDir;

	if (D3DXVec3Length(&m_vPlayerDir) > 15.f)
	{
		D3DXVec3Normalize(&m_vBugDir, &m_vBugDir);
		m_pTransformCom->Move_Pos(&m_vBugDir, fTimeDelta, 0.5f * Get_Speed());

		if (m_fMoveTime > 10.f)
			m_fMoveTime = 0.f;
	}
	m_fMoveTime += 10.f * fTimeDelta;
}

void CRollingBug::Update_Attack(_float fTimeDelta)
{

	Trace(fTimeDelta);

	if (D3DXVec3Length(&m_vPlayerDir) > 15.f)
	{
		D3DXVec3Normalize(&m_vBugDir, &m_vBugDir);
		m_pTransformCom->Move_Pos(&m_vBugDir, fTimeDelta, 1.5f * Get_Speed());

		if (D3DXVec3Length(&m_vPlayerDir) < 1.f)
		{

			Set_State(MONSTER_STATE::IDLE);

		}

	}
}

void CRollingBug::Update_Stun(_float fTimeDelta)
{
	switch (m_tBugInfo.eType)
	{
	case BUGCOLORTYPE::PINK:
		m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Up", true);
		break;

	case BUGCOLORTYPE::YELLOW:
		m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Up", true);
		break;

	case BUGCOLORTYPE::BLUE:
		m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Up", true);
		break;


	default:
		break;
	}
}

void CRollingBug::Update_Die(_float fTimeDelta)
{
	if (Is_Active())
	{
		Set_Active(false);
		On_Death();

		int iSound = rand() % 5 + (_uint)CHANNELID::SOUND_EFFECT_MONSTER;
		Stop_Sound((CHANNELID)iSound);
		Play_Sound(L"SFX_84_MonsterBugRolling_Death.wav", (CHANNELID)iSound, 0.5f);
	}
}

void CRollingBug::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;

	vDir = m_vPlayerDir;
	vDir.y = 0.f;

	D3DXVec3Normalize(&vDir, &vDir);
	m_pTarget = nullptr;

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());

	if (m_fMoveTime > 10.f)
		m_fMoveTime = 0.f;

	m_fMoveTime += 10.f * fTimeDelta;

	int iSound = rand() % 3 + (_uint)CHANNELID::SOUND_EFFECT_MONSTER;
	Play_Sound(L"SFX_82_MonsterBugRolling_Rolling.wav", (CHANNELID)iSound, 0.5f);
}

void CRollingBug::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (Get_State() == MONSTER_STATE::DIE)
		return;

	__super::Collision_Enter(pCollider, _eCollisionGroup, _iColliderID);

	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING &&
		pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
	{
		_vec3 vPlayerPos, vPos, vDir;
		pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		vDir = vPos - vPlayerPos;
		vDir.y = 0.0f;
		D3DXVec3Normalize(&vDir, &vDir);

		m_tStat.iHp -= 1;
		m_pRigidBodyCom->AddForce(vDir * 100.0f);

		if (m_tStat.iHp < 1.f)
			Set_State(MONSTER_STATE::DIE);

		int iSound = rand() % 5 + (_uint)CHANNELID::SOUND_EFFECT_MONSTER;
		Stop_Sound((CHANNELID)iSound);
		Play_Sound(L"SFX_83_MonsterBugRolling_Hit.wav", (CHANNELID)iSound, 0.5f);
	}
}

void CRollingBug::Set_Animation()
{
	OBJ_DIR eDir = OBJ_DIR::DIR_END;
	D3DXVec3Normalize(&m_vLook, &m_vLook);

	_vec3 vAxis(0.f, 0.f, 1.f);
	_float fAngle = D3DXVec3Dot(&m_vLook, &vAxis);
	fAngle = acosf(fAngle);

	if (m_vLook.x < 0.0f)
		fAngle = D3DX_PI * 2 - fAngle;

	fAngle = D3DXToDegree(fAngle);
	_uint iDir = fAngle / 22.5f;

	if (iDir == 0 || iDir == 15 || iDir == 16)
	{
		eDir = OBJ_DIR::DIR_U;
	}
	else if (iDir == 1 || iDir == 2)
	{
		eDir = OBJ_DIR::DIR_RU;
	}
	else if (iDir == 3 || iDir == 4)
	{
		eDir = OBJ_DIR::DIR_R;
	}
	else if (iDir == 5 || iDir == 6)
	{
		eDir = OBJ_DIR::DIR_RD;
	}
	else if (iDir == 7 || iDir == 8)
	{
		eDir = OBJ_DIR::DIR_D;
	}
	else if (iDir == 9 || iDir == 10)
	{
		eDir = OBJ_DIR::DIR_LD;
	}
	else if (iDir == 11 || iDir == 12)
	{
		eDir = OBJ_DIR::DIR_L;
	}
	else if (iDir == 13 || iDir == 14)
	{
		eDir = OBJ_DIR::DIR_LU;
	}
	else
		return;

	MONSTER_STATE eState = Get_State();


	if (m_ePreviousState == eState && eDir == m_eDir)
		return;


	_uint iIndex = m_pAnimator->GetCurrAnimation()->Get_Idx();

	switch (eState)
	{
	case Engine::MONSTER_STATE::IDLE:
		if (m_tBugInfo.eType == BUGCOLORTYPE::PINK)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}

		if (m_tBugInfo.eType == BUGCOLORTYPE::BLUE)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}

		if (m_tBugInfo.eType == BUGCOLORTYPE::YELLOW)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}



	case Engine::MONSTER_STATE::MOVE:
		if (m_tBugInfo.eType == BUGCOLORTYPE::PINK)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}

		if (m_tBugInfo.eType == BUGCOLORTYPE::BLUE)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}

		if (m_tBugInfo.eType == BUGCOLORTYPE::YELLOW)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}

	case Engine::MONSTER_STATE::REGEN:
		if (m_tBugInfo.eType == BUGCOLORTYPE::PINK)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}

		if (m_tBugInfo.eType == BUGCOLORTYPE::BLUE)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}

		if (m_tBugInfo.eType == BUGCOLORTYPE::YELLOW)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Up", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Down", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBugYellow_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBugYellow_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Left", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBugYellow_Idle_Right", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}


	case Engine::MONSTER_STATE::ATTACK:
		if (m_tBugInfo.eType == BUGCOLORTYPE::PINK)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Attack_LeftRight", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Attack_LeftRight", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;
	
			default:
				break;
			}
			break;
		}

		if (m_tBugInfo.eType == BUGCOLORTYPE::BLUE)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Attack_LeftRight", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Attack_LeftRight", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}

		if (m_tBugInfo.eType == BUGCOLORTYPE::YELLOW)
		{
			switch (eDir)
			{
			case Engine::OBJ_DIR::DIR_U:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_D:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_L:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Attack_LeftRight", true);
				break;
			case Engine::OBJ_DIR::DIR_R:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Attack_LeftRight", true);
				break;
			case Engine::OBJ_DIR::DIR_LU:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_RU:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_LD:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_RD:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Attack_UpDown", true);
				break;
			case Engine::OBJ_DIR::DIR_END:
				return;

			default:
				break;
			}
			break;
		}


	case Engine::MONSTER_STATE::DIE:
		break;

	default:
		break;
	}

	if (m_ePreviousState == eState)
		m_pAnimator->GetCurrAnimation()->Set_Idx(iIndex);

	m_ePreviousState = eState;
	m_eDir = eDir;
}

HRESULT CRollingBug::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Idle_Down", L"Proto_Texture_RollingBug_Pink_Idle_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Idle_Up", L"Proto_Texture_RollingBug_Pink_Idle_Up", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Idle_Right", L"Proto_Texture_RollingBug_Pink_Idle_Right", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Idle_Left", L"Proto_Texture_RollingBug_Pink_Idle_Left", 0.1f), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Move_Down", L"Proto_Texture_RollingBug_Pink_Move_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Move_Up", L"Proto_Texture_RollingBug_Pink_Move_Up", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Move_Right", L"Proto_Texture_RollingBug_Pink_Move_Right", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Move_Left", L"Proto_Texture_RollingBug_Pink_Move_Left", 0.1f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Attack_LeftRight", L"Proto_Texture_RollingBug_Pink_Attack_LeftRight", 0.08f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Attack_UpDown", L"Proto_Texture_RollingBug_Pink_Attack_UpDown", 0.08f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Idle_Down", L"Proto_Texture_RollingBug_Blue_Idle_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Idle_Up", L"Proto_Texture_RollingBug_Blue_Idle_Up", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Idle_Right", L"Proto_Texture_RollingBug_Blue_Idle_Right", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Idle_Left", L"Proto_Texture_RollingBug_Blue_Idle_Left", 0.1f), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Move_Down", L"Proto_Texture_RollingBug_Blue_Move_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Move_Up", L"Proto_Texture_RollingBug_Blue_Move_Up", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Move_Right", L"Proto_Texture_RollingBug_Blue_Move_Right", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Move_Left", L"Proto_Texture_RollingBug_Blue_Move_Left", 0.1f), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Attack_LeftRight", L"Proto_Texture_RollingBug_Blue_Attack_LeftRight", 0.08f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Attack_UpDown", L"Proto_Texture_RollingBug_Blue_Attack_UpDown", 0.08f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Idle_Down", L"Proto_Texture_RollingBug_Yellow_Idle_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Idle_Up", L"Proto_Texture_RollingBug_Yellow_Idle_Up", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Idle_Right", L"Proto_Texture_RollingBug_Yellow_Idle_Right", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Idle_Left", L"Proto_Texture_RollingBug_Yellow_Idle_Left", 0.1f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Move_Down", L"Proto_Texture_RollingBug_Yellow_Move_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Move_Up", L"Proto_Texture_RollingBug_Yellow_Move_Up", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Move_Right", L"Proto_Texture_RollingBug_Yellow_Move_Right", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Move_Left", L"Proto_Texture_RollingBug_Yellow_Move_Left", 0.1f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Attack_LeftRight", L"Proto_Texture_RollingBug_Yellow_Attack_LeftRight", 0.08f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Attack_UpDown", L"Proto_Texture_RollingBug_Yellow_Attack_UpDown", 0.08f), E_FAIL);



	switch (m_tBugInfo.eType)
	{
	case BUGCOLORTYPE::PINK:
		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Down", TRUE), E_FAIL);
		break;

	case BUGCOLORTYPE::BLUE:
		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Down", TRUE), E_FAIL);
		break;

	case BUGCOLORTYPE::YELLOW:
		FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Down", TRUE), E_FAIL);
		break;

	default:
		break;
	}

	return S_OK;
}

void CRollingBug::Set_Info(const _vec3 vPos, BUGCOLORTYPE eType)
{
	m_tBugInfo.eType = eType;
	m_tBugInfo.vDefaultPos = vPos;
}

CRollingBug* CRollingBug::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos, BUGCOLORTYPE eType)
{
	CRollingBug* pInstance = new CRollingBug(pGraphicDev);

	pInstance->Set_Info(vPos, eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Monster RollingBug Create Failed");
		return nullptr;
	}

	pInstance->Get_TransformCom()->Set_Pos(&vPos);

	return pInstance;
}