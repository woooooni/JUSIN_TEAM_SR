#include "../Include/stdafx.h"
#include "Export_Function.h"
#include "SunGollem.h"
#include "GolemParts.h"
#include "SludgeWave.h"
#include "PushStone.h"
#include "GameMgr.h"
#include "MonkeyBarrelCleaner.h"
#include "Effect_Smoke.h"
#include "Pool.h"
#include "CutSceneMgr.h"

CSunGollem::CSunGollem(LPDIRECT3DDEVICE9 pGraphicDev) 
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER, OBJ_ID::SUN_GOLLEM)
	, m_eState(SUNGOLEM_STATE::REGEN)
	, m_fSpeed(20.f)
{
}
CSunGollem::CSunGollem(const CSunGollem& rhs)
	: Engine::CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_eState(rhs.m_eState)
{

}

CSunGollem::~CSunGollem()
{
}


HRESULT CSunGollem::Ready_Object(void)
{
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"SunGolem_Idle_Body", L"Proto_Texture_SunGolem_Idle_Body", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Body", L"Proto_Texture_SunGolem_Dirty_Body", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_Body", true);

	memset(m_bAttack, 1, sizeof(bool)*6);
	memset(m_bSummon, 1, sizeof(bool) * 3);
	m_vVerticalDir = { 0.f, 1.f ,0.f };
	m_pTransformCom->Set_Pos(&_vec3(9.0f, 3.f, 14.0f));
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_vRandomPos[0] = { vPos.x - 5, vPos.y, vPos.z };
	m_vRandomPos[1] = { vPos.x , vPos.y, vPos.z };
	m_vRandomPos[2] = { vPos.x + 5, vPos.y, vPos.z };
	m_pTransformCom->Set_Scale({ 3.f, 4.f, 3.f });
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({3.f, 4.f, 3.f });
	m_fSpeed = 5.f;
	Set_State(SUNGOLEM_STATE::REGEN);
	m_tStat = { 6,6,1 };
	m_pMonsterAim = CMonsterAim::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pMonsterAim, E_FAIL);
	m_pMonsterAim->Set_Red(100);
	m_pMonsterAim->Set_Active(true);
	FAILED_CHECK_RETURN(Ready_Parts(), E_FAIL);


	m_pUIBack = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_BACK);
	NULL_CHECK_RETURN(m_pUIBack, E_FAIL);
	m_pUIGauge = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_GAUGE);
	NULL_CHECK_RETURN(m_pUIGauge, E_FAIL);
	m_pUIFrame = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_FRAME);
	NULL_CHECK_RETURN(m_pUIFrame, E_FAIL);
	m_pUIGauge->Set_Name(BOSSNAME::SUNGOLLEM);


	m_fMinHeight = 3.0f;

	return S_OK;
}

_int CSunGollem::Update_Object	(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::NUM_1))
	{
		m_tStat.iHp -= 5;
	}

	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);;
	if(m_eState!=SUNGOLEM_STATE::DIE)
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BOSS);
	if(!m_bStop)
		m_fTime += 1.2f*fTimeDelta;
	if (!m_bStop&& m_eState == SUNGOLEM_STATE::DIE)
		m_fTime += 0.8f * fTimeDelta;
	m_iIndex = (int)m_fTime;
	switch (m_eState)
	{
	case SUNGOLEM_STATE::IDLE:
		Update_Idle(fTimeDelta);
		break;
	case SUNGOLEM_STATE::MOVE:
		Update_Move(fTimeDelta);
		break;
	case SUNGOLEM_STATE::REGEN:
		Update_Regen(fTimeDelta);
		break;
	case SUNGOLEM_STATE::ATTACK:
		Update_Attack(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIE:
		Update_Die(fTimeDelta);
		break;
	case SUNGOLEM_STATE::DIRTY:
		Update_Dirty(fTimeDelta);
		break;

	}
	_vec3 vPos = {};
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	if (m_eState != SUNGOLEM_STATE::MOVE)
	{
		m_fMinHeight = (m_pParts[LEFTLEG]->Get_Offset().y * -1.f) + 1.f;
		vPos.y = m_fMinHeight;
		m_pTransformCom->Set_Pos(&vPos);
	}
	if (m_bDirty)
	{
		_vec3 vOffset = m_pParts[HEAD]->Get_Offset();
		vOffset += {0.f, 0.15f, -0.1f};
		m_pParts[FACE]->Set_Offset(vOffset);
	}
	if(m_pParts!=nullptr)
	for (auto i = 0; i != PARTSEND; i++)
	{
		if (m_pParts[i]->Is_Active())
		{
			m_pParts[i]->Set_Index(m_iIndex);
			m_pParts[i]->Set_State(m_eState);
			m_pParts[i]->Update_Object(fTimeDelta);
			m_pParts[i]->Rotate();
			if (m_eState != SUNGOLEM_STATE::REGEN && m_eState != SUNGOLEM_STATE::DIRTY)
			{
				_vec3 vOffset = m_pParts[i]->Get_Offset();
				vOffset.y = m_vPartPos[i].y;
			}
			m_pParts[i]->Move_to_Offset(vPos);
		}
	}


	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Update_Object(fTimeDelta);
		m_pUIGauge->Update_Object(fTimeDelta);
		m_pUIFrame->Update_Object(fTimeDelta);
	}


	m_pMonsterAim->Update_Object(fTimeDelta);
	return iExit;
}

void CSunGollem::LateUpdate_Object(void)
{
	for (auto i = 0; i != PARTSEND; i++)
	{
		if (m_pParts[i]->Is_Active())
			m_pParts[i]->LateUpdate_Object();
	}
	m_pMonsterAim->LateUpdate_Object();
	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->LateUpdate_Object();
		m_pUIGauge->LateUpdate_Object();
		m_pUIFrame->LateUpdate_Object();
	}

	if (m_bDirty == true && m_tStat.iHp == 0)
	{
		m_pUIBack->Set_Active(false);
		m_pUIGauge->Set_Active(false);
		m_pUIFrame->Set_Active(false);
	}

}

void CSunGollem::Render_Object(void)
{
	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, 1.f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 50.0f);


	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);


	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();
	
	for (auto i = 0; i != PARTSEND; i++)
	{
		if (m_pParts[i]->Is_Active())
			m_pParts[i]->Render_Object();
	}

	m_pMonsterAim->Render_Object();

	if (m_pUIBack->Is_Active() &&
		m_pUIGauge->Is_Active() &&
		m_pUIFrame->Is_Active())
	{
		m_pUIBack->Render_Object();
		m_pUIGauge->Render_Object();
		m_pUIFrame->Render_Object();
	}

}

HRESULT CSunGollem::Add_Component(void)
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

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);
	
	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);


	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

void CSunGollem::Update_Idle(_float fTimeDelta)
{
	if (!m_bExhale)
	{
		for (_int i = 0; i < PARTSEND; i++)
		{
			m_pParts[i]->Move_Offset(m_vPartPos[i], fTimeDelta,3.f);
			m_pParts[i]->Set_RotationAngle(m_fPartAngleArray[i]);

		}
	}
	else
	{

	}
	if (m_iIndex > 1)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
		{
			m_bExhale = true;
		}
		if(rand()%10>4)
				Set_State(SUNGOLEM_STATE::MOVE);
		else if(rand() % 10 < 5)
				Set_State(SUNGOLEM_STATE::ATTACK);
		m_fSpeed = 5.f;
		m_fMoveTime = 0.f;
		m_fTime = 0.f;
		if (m_tStat.iHp < 1.f )
			if (!m_bDirty)
			{
				Set_State(SUNGOLEM_STATE::DIRTY);
				
				m_pAnimator->Play_Animation(L"SunGolem_Dirty_Body", true);
			
				Create_Monkey();
				m_fTime = 0.f;
			}
			else
			{
				m_fTime = 0.f;
				Set_State(SUNGOLEM_STATE::DIE);
				Stop_Sound(CHANNELID::SOUND_BOSS);
				Play_Sound(L"SFX_166_SunGolemDeath.wav", CHANNELID::SOUND_BOSS, .5f);
				CCutSceneMgr::GetInstance()->Start_CutScene(CCutSceneMgr::CUTSCENE_TYPE::BOSS_SUNGOLEM_DIE);
			}
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CSunGollem::Update_Dirty(_float fTimeDelta)
{

	m_tStat.iMaxHp = 30;
	m_tStat.iAttack = 1;
	m_bDirty = true;
	if(!m_pParts[FACE]->Is_Active())
	m_pParts[FACE]->Set_Active(true);

	if (m_tStat.iMaxHp > m_iDirtyHp)
		m_iDirtyHp += _uint(100 * fTimeDelta);


	

	m_pUIBack = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_BACK);
	if (m_pUIBack == nullptr)
		return;
	m_pUIGauge = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_GAUGE);
	if (m_pUIGauge == nullptr)
		return;
	m_pUIFrame = CUI_BossHP::Create(m_pGraphicDev, BOSSHP::UI_FRAME);
	if (m_pUIFrame == nullptr)
		return;

	m_pUIGauge->Set_Name(BOSSNAME::SUNGOLLEM_REBIRTH);
	switch (m_iIndex)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		if (m_bScream)
		{
			dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"))->CamShake(1.5f);
			m_bScream = false;
			Stop_Sound(CHANNELID::SOUND_BOSS);
			Play_Sound(L"SFX_337_DirtyGolemActivate_0.wav", CHANNELID::SOUND_BOSS, .5f);
		}
		break;
	case 3:
		break;
	case 4:
		if (m_tStat.iMaxHp <= m_iDirtyHp)
		{
			m_iDirtyHp = m_tStat.iMaxHp;
			m_fTime = 0.f;
			Set_State(SUNGOLEM_STATE::IDLE);
		}
		for (_int i = 0; i < PARTSEND; i++)
		{
			m_pParts[i]->Set_Offset(m_vPartPos[i]);
			m_pParts[i]->Set_RotationAngle(m_fPartAngleArray[i]);
		}
		break;
	default:
		break;
	}
	m_tStat.iHp = m_iDirtyHp;
	m_fMoveTime += 10.f * fTimeDelta;
}

void CSunGollem::Update_Move(_float fTimeDelta)
{

	_vec3 vPos;
	_vec3 vDir;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vDir = m_vRandomPos[m_iRand] - vPos;
	vDir.y = 0.f;

	m_pTransformCom->Move_Pos(&vDir,5.f,fTimeDelta );
	if (!m_bJump)
	{
		Stop_Sound(CHANNELID::SOUND_BOSS);
		Play_Sound(L"SFX__SunGolemJumpNormal_0.wav", CHANNELID::SOUND_BOSS, .5f);
		m_pRigidBodyCom->AddForce(_vec3(0.0f, 150.0f, 0.0f));
		m_pRigidBodyCom->SetGround(false);
		m_bJump = true;
	}
	if (m_pRigidBodyCom->IsGround() && m_pRigidBodyCom->GetVelocity().y <= 0.0f)
	{
		Stop_Sound(CHANNELID::SOUND_BOSS);
		Play_Sound(L"SFX__SunGolemJumpNormal_1.wav", CHANNELID::SOUND_BOSS, .5f);
		dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"))->CamShake(0.2f);
		m_vVerticalDir = { 0.f, 1.f ,0.f };
		m_iRand = rand() % 3;
		m_fMoveTime = 0.f;
		D3DXVec3Normalize(&vDir, &vDir);
		m_pRigidBodyCom->AddForce(vDir *80.f);
		Set_State(SUNGOLEM_STATE::ATTACK);
		if (m_bDirty && m_tStat.iHp < 15)
			Create_Wave(vPos);
		CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::INTERACTION_OBJ);
		vector<Engine::CGameObject*> ObjectVec = pLayer->Get_GameObjectVec();
		_uint iStoneNum(0);
		for (_uint i = 0; i < ObjectVec.size(); ++i)
		{
			if (ObjectVec[i]->Get_Name() == L"Stone")
				iStoneNum++;
		}
		if (iStoneNum < 6)
		{
			Create_Stone();
			Create_Stone();

		}
		for (int i = 0; i < 15; i++)
		{
			Create_Effect();
		}
		m_bJump = false;
	}
	if (!m_bExhale)
	{
		for (_int i = 0; i < PARTSEND; i++)
		{
			m_pParts[i]->Move_Offset(m_vPartPos[i], fTimeDelta, 2.f);
			m_pParts[i]->Set_RotationAngle(m_fPartAngleArray[i]);
		}
	}
	else
	{

	}
	if (m_iIndex > 1)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
		{
			m_bExhale = true;
		}
		m_fSpeed = 5.f;
		m_fTime = 0.f;
	}
}

void CSunGollem::Update_Attack(_float fTimeDelta)
{

	CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();
	if (nullptr == pTarget)
		return;
	Set_Target(pTarget);

	_vec3 vTargetPos, vDir;
	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	if (m_bLockon == false)
	{
		m_vTargetPos = vTargetPos;
		m_pMonsterAim->Set_Active(true);
		m_pMonsterAim->Get_TransformCom()->Set_Pos(&_vec3{ vTargetPos.x,0.001f,vTargetPos.z });
		m_bLockon = true;
	}
	else
	{
		vDir = vTargetPos - m_vTargetPos;
		D3DXVec3Normalize(&vDir, &vDir);
		m_vTargetPos += vDir * fTimeDelta *5.f;
		m_pMonsterAim->Get_TransformCom()->Set_Pos(&_vec3{ m_vTargetPos.x,0.001f,m_vTargetPos.z });
	}
	if (m_iActiveArm > 5)
	{
		m_pParts[LEFTARM0]->Set_Active(false);
		m_pParts[RIGHTARM0]->Set_Active(false);
		m_pParts[LEFTHAND0]->Set_Active(false);
		m_pParts[RIGHTHAND0]->Set_Active(false);
	}
	if (m_iActiveArm > 3)
	{
		m_pParts[LEFTARM1]->Set_Active(false);
		m_pParts[RIGHTARM1]->Set_Active(false);
		m_pParts[LEFTHAND1]->Set_Active(false);
		m_pParts[RIGHTHAND1]->Set_Active(false);
	}
	if (m_iActiveArm > 1)
	{
		m_pParts[LEFTARM2]->Set_Active(false);
		m_pParts[RIGHTARM2]->Set_Active(false);
		m_pParts[LEFTHAND2]->Set_Active(false);
		m_pParts[RIGHTHAND2]->Set_Active(false);
	}
	if (m_fMoveTime > 10.f)
	{

	
		if (m_iActiveArm > 1)
		{
			if (m_bAttack[0]&&m_fMoveTime > 10.f)
			{
				Create_Fist(false, 5);
				m_bAttack[0] = false;
			}
			if (m_bAttack[1] && m_fMoveTime > 15.f)
			{
				Create_Fist(false, 5);
				m_bAttack[1] = false;
			}
		}
			if (m_iActiveArm > 3)
			{
				if (m_bAttack[2] && m_fMoveTime > 20.f)
				{
					Create_Fist(true, 5);
					m_bAttack[2] = false;
				}
				if (m_bAttack[3] && m_fMoveTime > 25.f)
				{
					Create_Fist(true, 5);
					m_bAttack[3] = false;
				}
			}
			if (m_iActiveArm > 5 && m_fMoveTime > 30.f)
			{
				if (m_bAttack[4])
				{
					Create_Fist(false, 5) ;
					m_bAttack[4] = false;
				}
				if (m_bAttack[5] && m_fMoveTime > 35.f)
				{
					Create_Fist(false, 5);
					m_bAttack[5] = false;
				}
			}
			
		
		if (m_fMoveTime > 10.f+ 6.f* (float)m_iActiveArm)
		{
			m_fMoveTime = 0.f;
			Set_State(SUNGOLEM_STATE::IDLE);
			memset(m_bAttack, 1, sizeof(bool) * 6);
			m_iActiveArm += 2;
			m_bLockon = false;
			m_pMonsterAim->Set_Active(false);
			if (m_iActiveArm > 5)
			{
				m_pParts[LEFTARM0]->Set_Active(true);
				m_pParts[RIGHTARM0]->Set_Active(true);
				m_pParts[LEFTHAND0]->Set_Active(true);
				m_pParts[RIGHTHAND0]->Set_Active(true);
			}
			if (m_iActiveArm > 3)
			{
				m_pParts[LEFTARM1]->Set_Active(true);
				m_pParts[RIGHTARM1]->Set_Active(true);
				m_pParts[LEFTHAND1]->Set_Active(true);
				m_pParts[RIGHTHAND1]->Set_Active(true);
			}
			if (m_iActiveArm > 1)
			{
				m_pParts[LEFTARM2]->Set_Active(true);
				m_pParts[RIGHTARM2]->Set_Active(true);
				m_pParts[LEFTHAND2]->Set_Active(true);
				m_pParts[RIGHTHAND2]->Set_Active(true);

			}
			if (m_iActiveArm > 6)
			{
				m_iActiveArm = 2;

			}
		}
	}
	m_fMoveTime += 10.f * fTimeDelta;
	if (!m_bExhale)
	{
		for (_int i = 0; i < PARTSEND; i++)
		{
			m_pParts[i]->Move_Offset(m_vPartPos[i], fTimeDelta, 2.f);
			m_pParts[i]->Set_RotationAngle(m_fPartAngleArray[i]);


		}
	}
	else
	{

	}
	if (m_iIndex > 1)
	{
		if (m_bExhale)
			m_bExhale = false;
		else
		{
			m_bExhale = true;


		}
		m_fSpeed = 5.f;
		m_fTime = 0.f;
	}
}

void CSunGollem::Update_Die(_float fTimeDelta)
{
	CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::MONSTER);
	for (auto iter = pLayer->Get_GameObjectVec().begin(); iter != pLayer->Get_GameObjectVec().end(); iter++)
	{
		if ((*iter) != this&& dynamic_cast<CMonster*>((*iter)))
		dynamic_cast<CMonster*>((*iter))->Set_State(MONSTER_STATE::DIE);
	}
}

void CSunGollem::Update_Regen(_float fTimeDelta)
{

	switch (m_iIndex)
	{
	case 0:
		break;
	case 1:
		if (!m_bSoundPlay)
		{
			m_bSoundPlay = true;
			Stop_Sound(CHANNELID::SOUND_BOSS);
			Play_Sound(L"SFX_37_SunGolemActivate_0.wav", CHANNELID::SOUND_BOSS, .5f);
		}
		break;
	case 2:
		m_bSoundPlay = false;
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		if (!m_bSoundPlay)
		{
			m_bSoundPlay = true;
			Stop_Sound(CHANNELID::SOUND_BOSS);
			Play_Sound(L"SFX_37_SunGolemActivate_1.wav", CHANNELID::SOUND_BOSS, .5f);
		}break;
	case 6:
		m_bSoundPlay = false;
		break;
	case 7:
		Set_State(SUNGOLEM_STATE::IDLE);
		for (_int i = 0; i < PARTSEND; i++)
		{
			m_pParts[i]->Set_Offset(m_vPartPos[i]);
			m_pParts[i]->Set_RotationAngle(m_fPartAngleArray[i]);
		}
		if (m_fMinHeight < 3.f)
			m_fMinHeight += 3.f * fTimeDelta;
		else
			m_fMinHeight = 3.f;
	
		break;
	default:
		m_fTime = 0.f;
		break;
	}
}
CSunGollem* CSunGollem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSunGollem* pInstance = new CSunGollem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("SunGollem Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CSunGollem::Free()
{
	__super::Free();
}
void CSunGollem::Create_Fist(bool _BummerFist, _int _iSrc)
{
	CGolemFist* pGolemFist = CGolemFist::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGolemFist, );
	m_vTargetPos.y = 7.f;
	pGolemFist->Get_TransformCom()->Set_Pos(&m_vTargetPos);
	pGolemFist->Set_Dirty(m_bDirty);
	pGolemFist->Set_Bummer(_BummerFist);
	pGolemFist->Set_Atk(m_tStat.iAttack); 
	pGolemFist->Set_Owner(this);
	CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::MONSTER);
	pLayer->Add_GameObject(L"GolemFist", pGolemFist);

}
void CSunGollem::Create_Wave(_vec3 vPos)
{
	vPos.y = 0.01f;
	CSludgeWave* pSludgeWave = CSludgeWave::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pSludgeWave, );
	pSludgeWave->Get_TransformCom()->Set_Pos(&vPos);
	pSludgeWave->Set_Atk(m_tStat.iAttack);
	pSludgeWave->Set_Owner(this);
	pSludgeWave->Set_Wave(40);
	CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::MONSTER);
	pLayer->Add_GameObject(L"SludgeWave", pSludgeWave);
	Stop_Sound(CHANNELID::SOUND_EFFECT_MONSTER5);
	Play_Sound(L"SFX_335_DirtyWave.wav", CHANNELID::SOUND_EFFECT_MONSTER5, .5f);
}

void CSunGollem::Create_Stone()
{

	_vec3 vPos = { m_vRandomPos[1].x+(rand()%10)-5,7.f,m_vRandomPos[1].z + (rand() % 6) - 3 - 7.f};
	CPushStone* pPushStone = CPushStone::Create(vPos,m_pGraphicDev);
	NULL_CHECK_RETURN(pPushStone, );
	CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::INTERACTION_OBJ);
	pLayer->Add_GameObject(L"Stone", pPushStone);
}
void CSunGollem::Create_Effect()
{
	_vec3 vPos = { m_vRandomPos[1].x + float((rand() % 20) - 10),0.5f,m_vRandomPos[1].z + float(rand() % 20)- 10.f };
	for (int i = 0; i < 6; i++)
	{
		CGameObject* pSmoke = CPool<CEffect_Smoke>::Get_Obj();
		if (pSmoke)
			dynamic_cast<CEffect_Smoke*>(pSmoke)->Get_Effect(vPos, _vec3(2.f, 2.f, 2.f), 148, 150, 148);
		else
		{
			pSmoke = dynamic_cast<CEffect_Smoke*>(pSmoke)->Create(Engine::Get_Device());
			if (pSmoke)
				dynamic_cast<CEffect_Smoke*>(pSmoke)->Get_Effect(vPos, _vec3(2.f, 2.f, 2.f), 148, 150, 148);
		}
	}
}

void CSunGollem::Create_Monkey()
{
	_vec3 vPos[5] ={_vec3(-3.45f, 1.f,	4.75f), 
					_vec3(-3.45f, 1.f,	8.5f), 
					_vec3(21.45f, 1.f,	8.5f),
					_vec3(21.45f, 1.f,	6.5f),
					_vec3(21.45f, 1.f,	2.5f) };
	for (int i = 0; i < 5; i++)
	{
		CMonkeyBarrelCleaner* pMonkeyBarrelCleaner = CMonkeyBarrelCleaner::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(pMonkeyBarrelCleaner, );
		if (i < 2)
			pMonkeyBarrelCleaner->Set_Right(true);
		pMonkeyBarrelCleaner->Get_TransformCom()->Set_Pos(&vPos[i]);
		CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
		pLayer->Add_GameObject(L"MonkeyBarrelCleaner", pMonkeyBarrelCleaner);
	}
}

HRESULT CSunGollem::Ready_Parts(void) 
{
	_vec3  vAxisZ;
	vAxisZ = {0.f,0.f,1.f};

	ZeroMemory(m_fPartAngleArray, sizeof(float) * PARTSEND);

	m_vPartPos[HEAD] = { 0.f,1.16633f, -0.05f };
	m_vPartPos[UPPERJAW] = { 0.f,0.4f, -0.055f };
	m_vPartPos[LOWERJAW] = { 0.f,-0.16f, -0.055f };
	m_vPartPos[LEFTLEG] = { - 0.9f,	- 2.f, - 0.01f };
	m_vPartPos[RIGHTLEG] = {0.9f, - 2.0f, - 0.01f };
	m_vPartPos[LEFTARM0] = { -2.069f,-0.8f, - 0.0103f };
	m_fPartAngleArray[LEFTARM0] = 52.026;
	m_vPartPos[LEFTARM1] = { -2.2f,0.498318f, - 0.0102f };
	m_fPartAngleArray[LEFTARM1] = 6.5;
	m_vPartPos[LEFTARM2] = { -1.79f,1.551f, - 0.0101f };
	m_fPartAngleArray[LEFTARM2] = 1.f;
	m_vPartPos[RIGHTARM0] = { 2.069f,-0.8f, - 0.0103f };
	m_fPartAngleArray[RIGHTARM0] = 308.156;
	m_vPartPos[RIGHTARM1] = { 2.2f,0.5f, - 0.0102f };
	m_fPartAngleArray[RIGHTARM1] = 352.1f;
	m_vPartPos[RIGHTARM2] = { 1.79f,1.551f, - 0.0101f };
	m_fPartAngleArray[RIGHTARM2] =	380.322f;
	m_vPartPos[LEFTHAND0] = { -2.5f,-1.6f, - 0.0106f };
	m_fPartAngleArray[LEFTHAND0] = 341.6f;
	m_vPartPos[LEFTHAND1] = { -3.f,0.2f, - 0.0105f };
	m_fPartAngleArray[LEFTHAND1] = -60.8f;
	m_vPartPos[LEFTHAND2] = { -2.59f,1.694f, - 0.0104f };
	m_fPartAngleArray[LEFTHAND2] = -93.8f;
	m_vPartPos[RIGHTHAND0] = { 2.5f,-1.6f, - 0.0106f };
	m_fPartAngleArray[RIGHTHAND0] = 22.5f;
	m_vPartPos[RIGHTHAND1] = { 3.f,0.2f, - 0.0105f };
	m_fPartAngleArray[RIGHTHAND1] = 60;
	m_vPartPos[RIGHTHAND2] = { 2.59f,1.694f, - 0.0104f };
	m_fPartAngleArray[RIGHTHAND2] = 85;
	//	HEAD, LOWERJAW, UPPERJAW, LEFTLEG, RIGHTLEG, LEFTARM0, LEFTARM1,
	//	LEFTARM2, RIGHTARM0, RIGHTARM1, RIGHTARM2, LEFTHAND0,
	//	LEFTHAND1, LEFTHAND2, RIGHTHAND0, RIGHTHAND1, RIGHTHAND2
	
	m_pParts[HEAD] = CGolemHead::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pParts[HEAD], E_FAIL);
	m_pParts[HEAD]->Set_Offset(m_vPartPos[HEAD]);

	m_pParts[UPPERJAW] = CGolemUpperJaw::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pParts[UPPERJAW], E_FAIL);
	m_pParts[UPPERJAW]->Set_Offset(m_vPartPos[UPPERJAW]);
	
	m_pParts[LOWERJAW] = CGolemLowerJaw::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pParts[LOWERJAW], E_FAIL);
	m_pParts[LOWERJAW]->Set_Offset(m_vPartPos[LOWERJAW]);
	
	 m_pParts[LEFTLEG] = CGolemLeftLeg::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pParts[LEFTLEG], E_FAIL);
	m_pParts[LEFTLEG]->Set_Offset(m_vPartPos[LEFTLEG]);

	m_pParts[RIGHTLEG] = CGolemRightLeg::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pParts[RIGHTLEG], E_FAIL);
	m_pParts[RIGHTLEG]->Set_Offset(m_vPartPos[RIGHTLEG]);

	for (_int i = LEFTARM0; i < LEFTARM2+1; i++)
	{
		m_pParts[i ] = CGolemLeftArm::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pParts[i], E_FAIL);
		dynamic_cast<CGolemLeftArm*>(	m_pParts[i ])->Set_ArmNum(i - LEFTARM0);
		m_pParts[i]->Set_Offset(m_vPartPos[i]);
	}
	for (_int i = RIGHTARM0; i < RIGHTARM2 + 1; i++)
	{
		m_pParts[i ] = CGolemRightArm::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pParts[i], E_FAIL);
		dynamic_cast<CGolemRightArm*>(m_pParts[i])->Set_ArmNum(i - RIGHTARM0);
		m_pParts[i ]->Set_Offset(m_vPartPos[i]);
	}
	for (_int i = LEFTHAND0; i < LEFTHAND2 + 1; i++)
	{
		m_pParts[i] = CGolemLeftHand::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pParts[i], E_FAIL);
		dynamic_cast<CGolemLeftHand*>(m_pParts[i])->Set_ArmNum(i - LEFTHAND0);
		m_pParts[i]->Set_Offset(m_vPartPos[i]);
	}
	for (_int i = RIGHTHAND0; i < RIGHTHAND2 + 1; i++)
	{
		m_pParts[i] = CGolemRightHand::Create(m_pGraphicDev);
		NULL_CHECK_RETURN(m_pParts[i], E_FAIL);
		dynamic_cast<CGolemRightHand*>(m_pParts[i])->Set_ArmNum(i - RIGHTHAND0);
		m_pParts[i]->Set_Offset(m_vPartPos[i]);
	
	}
	m_pParts[FACE] = CGolemFace::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pParts[FACE], E_FAIL);
	m_pParts[FACE]->Set_Offset(m_vPartPos[FACE]);
	m_vecGolemPart.reserve(PARTSEND);

	for (auto i = 0; i != PARTSEND; i++)
		{
		if (m_pParts[i]->Is_Active())
			m_vecGolemPart.push_back(m_pParts[i]);
		m_pParts[i]->Set_RotationAngle(m_fPartAngleArray[i]);
		}
	return S_OK;
}
void CSunGollem::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{

	if ( m_eState == SUNGOLEM_STATE::DIRTY || m_eState == SUNGOLEM_STATE::DIE)
		return;

	CCamera* pCamera = dynamic_cast<CCamera*>(Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	NULL_CHECK_RETURN(pCamera, );

	if (dynamic_cast<CPushStone*>(pCollider->GetOwner()))
	{
		if (dynamic_cast<CPushStone*>(pCollider->GetOwner())->Is_Flying()&& dynamic_cast<CPushStone*>(pCollider->GetOwner())->Is_Clean())
		{
			m_tStat.iHp -= 5;
			pCamera->CamShake(0.3f, 5.f);
		}
		if (dynamic_cast<CPushStone*>(pCollider->GetOwner())->Is_Flying() && !dynamic_cast<CPushStone*>(pCollider->GetOwner())->Is_Clean())
		{
			m_tStat.iHp -= 1;
			if(m_bDirty)
				pCamera->CamShake(0.3f, 1.f);
			else
				pCamera->CamShake(0.3f, 5.f);
		}
	}



}