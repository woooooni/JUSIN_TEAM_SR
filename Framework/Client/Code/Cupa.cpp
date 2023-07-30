#include "Cupa.h"
#include "Export_Function.h"
#include "GameMgr.h"

CCupa::CCupa(LPDIRECT3DDEVICE9 pGraphicDev)
	:CMonster(pGraphicDev, OBJ_ID::CUPA), m_fMoveTime(0.f)
{
}

CCupa::CCupa(const CCupa& rhs)
	: CMonster(rhs), m_fMoveTime(rhs.m_fMoveTime)
{
}

CCupa::~CCupa()
{
}

HRESULT CCupa::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_State(MONSTER_STATE::IDLE);
	Set_Speed(3.f);

	_vec3 vPos = _vec3(1.f, 0.5f, 1.f);
	_vec3 vScale = _vec3(1.f, 1.f, 1.f);
	m_pTransformCom->Set_Pos(&vPos);
	m_pTransformCom->Set_Scale(vScale);

	m_tStat = { 3, 3, 1 };
	m_fMinHeight = 0.5f;

	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale(_vec3(0.6f, 0.6f, 0.6f));

	m_pUIBack = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_BACK);
	if (m_pUIBack != nullptr)
		m_pUIBack->Set_Owner(this);

	m_pUIGauge = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_GAUGE);
	if (m_pUIGauge != nullptr)
		m_pUIGauge->Set_Owner(this);

	m_pUIFrame = CUI_MonsterHP::Create(m_pGraphicDev, MONSTERHP::UI_FRAME);
	if (m_pUIFrame != nullptr)
		m_pUIFrame->Set_Owner(this);

	return S_OK;
}

_int CCupa::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	Engine::Add_CollisionGroup(m_pColliderCom, COLLIDE_STATE::COLLIDE_MONSTER);

	_vec3 vPlayerPos, vPos, vDir;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	if (Get_State() != MONSTER_STATE::REGEN &&
		Get_State() != MONSTER_STATE::ATTACK)
	{
		CGameObject* pTarget = CGameMgr::GetInstance()->Get_Player();
		if (nullptr == pTarget)
			return S_OK;

		Set_Target(pTarget);

		m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);

		vDir = vPlayerPos - vPos;
		m_vDir = vDir;

		if (D3DXVec3Length(&vDir) < 5.f)
		{
			Set_State(MONSTER_STATE::REGEN);
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

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CCupa::LateUpdate_Object(void)
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
}

void CCupa::Render_Object(void)
{
	if (!Is_Active())
		return;

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

void CCupa::Update_Idle(_float fTimeDelta)
{

	if (m_fMoveTime > 10.f)
	{
		if (rand() % 10 > 6)
		{
			Set_State(MONSTER_STATE::MOVE);
		}
		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CCupa::Update_Die(_float fTimeDelta)
{
	if (Is_Active()) 
	{
		Set_Active(false);
		On_Death();
	}
}

void CCupa::Update_Regen(_float fTimeDelta)
{
	_vec3 vPlayerPos, vPos, vDir;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_vDir = vPlayerPos - vPos;
	m_vLook = m_vDir;

	if (m_fMoveTime > 15.f)
	{
		Set_State(MONSTER_STATE::ATTACK);
		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CCupa::Update_Move(_float fTimeDelta)
{
	_vec3 vPlayerPos, vDir, vPos, vDst;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_vDir = vPlayerPos - vPos;

	if (m_fMoveTime > 5.f)
	{
		if (rand() % 10 > 8)
		{
			Set_State(MONSTER_STATE::IDLE);
		}

		vDst = { float(rand() % 10) - 5.f, 0.f, float(rand() % 5) - 5.f };
		
		if (vDst != m_vDst)
			m_vDst = vDst;
		
		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;

	vDir = m_vDst;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);
	m_vLook = vDir;
	m_vDir = vDir;
	m_pTarget = nullptr;
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed() / 2.f);
}

void CCupa::Update_Attack(_float fTimeDelta)
{
	Trace(fTimeDelta);
}

void CCupa::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;

	vDir = m_vDir;
	vDir.y = 0.f;

	D3DXVec3Normalize(&vDir, &vDir);
	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 1.5f * Get_Speed()); // Default Speed = 5.f

//	m_pTransformCom->Get_Info(INFO_POS, &vPos);
//	vPos += vDir * fTimeDelta * 1.5f * Get_Speed();
//	m_vArrived = vPos;

	if (m_fMoveTime > 10.f)
	{
		Set_State(MONSTER_STATE::IDLE);
		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CCupa::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
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
		vDir.y = 0.5f;
		D3DXVec3Normalize(&vDir, &vDir);

		m_tStat.iHp -= 1;
		m_pRigidBodyCom->AddForce(vDir * 60.0f);

		if (m_tStat.iHp < 1.f)
			Set_State(MONSTER_STATE::DIE);
	}

//	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER &&
//		pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
//	{
////		_vec3 vPos;
////		m_pTransformCom->Get_Info(INFO_POS, &vPos);
////
////		if (m_vArrived == vPos)
////		{
////		_vec3 vDir;
////		vDir = m_vDst;
////		vDir.y = 0.f;
////		D3DXVec3Normalize(&vDir, &vDir);
////		m_vLook = vDir;
//		_vec3 vScale = _vec3(0.5f, 0.5f, 0.5f);
//		m_pTransformCom->Set_Scale(vScale);
//
//		m_pAnimator->Play_Animation(L"Cupa_Reaction_Down", TRUE);
//		//m_pAnimator->Play_Animation(L"Cupa_Regen_Down", TRUE);
//
////			if (!m_bJump)
////			{
////				m_pRigidBodyCom->AddForce(vDir * 80.0f);
////				m_pRigidBodyCom->AddForce(_vec3(0.0f, 50.0f, 0.0f));
////				m_pRigidBodyCom->SetGround(FALSE);
////				m_bJump = TRUE;
////			}
//
//			if (m_pAnimator->GetCurrAnimation()->Is_Finished())
//			{
//				//m_bJump = FALSE;
//				Set_State(MONSTER_STATE::IDLE);
//			}
////		}
//	}

}

void CCupa::Set_Animation()
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
		switch (eDir)
		{
		case Engine::OBJ_DIR::DIR_U:
			m_pAnimator->Play_Animation(L"Cupa_Idle_Up", true);
			break;
		case Engine::OBJ_DIR::DIR_D:
			m_pAnimator->Play_Animation(L"Cupa_Idle_Down", true);
			break;
		case Engine::OBJ_DIR::DIR_L:
			m_pAnimator->Play_Animation(L"Cupa_Idle_Left", true);
			break;
		case Engine::OBJ_DIR::DIR_R:
			m_pAnimator->Play_Animation(L"Cupa_Idle_Right", true);
			break;
		case Engine::OBJ_DIR::DIR_LU:
			m_pAnimator->Play_Animation(L"Cupa_Idle_LeftUp", true);
			break;
		case Engine::OBJ_DIR::DIR_RU:
			m_pAnimator->Play_Animation(L"Cupa_Idle_RightUp", true);
			break;
		case Engine::OBJ_DIR::DIR_LD:
			m_pAnimator->Play_Animation(L"Cupa_Idle_LeftDown", true);
			break;
		case Engine::OBJ_DIR::DIR_RD:
			m_pAnimator->Play_Animation(L"Cupa_Idle_RightUp", true);
			break;
		case Engine::OBJ_DIR::DIR_END:
			return;
		default:
			break;
		}
		break;

	case Engine::MONSTER_STATE::MOVE:
		switch (eDir)
		{
		case Engine::OBJ_DIR::DIR_U:
			m_pAnimator->Play_Animation(L"Cupa_Move_Up", true);
			break;
		case Engine::OBJ_DIR::DIR_D:
			m_pAnimator->Play_Animation(L"Cupa_Move_Down", true);
			break;
		case Engine::OBJ_DIR::DIR_L:
			m_pAnimator->Play_Animation(L"Cupa_Move_Left", true);
			break;
		case Engine::OBJ_DIR::DIR_R:
			m_pAnimator->Play_Animation(L"Cupa_Move_Right", true);
			break;
		case Engine::OBJ_DIR::DIR_LU:
			m_pAnimator->Play_Animation(L"Cupa_Move_LeftUp", true);
			break;
		case Engine::OBJ_DIR::DIR_RU:
			m_pAnimator->Play_Animation(L"Cupa_Move_RightUp", true);
			break;
		case Engine::OBJ_DIR::DIR_LD:
			m_pAnimator->Play_Animation(L"Cupa_Move_LeftDown", true);
			break;
		case Engine::OBJ_DIR::DIR_RD:
			m_pAnimator->Play_Animation(L"Cupa_Move_RightDown", true);
			break;
		case Engine::OBJ_DIR::DIR_END:
			return;
		default:
			break;
		}
		break;

	case Engine::MONSTER_STATE::REGEN:
		switch (eDir)
		{
//		case Engine::OBJ_DIR::DIR_U:
//			m_pAnimator->Play_Animation(L"DesertRhino_Ready_Up", true);
//			break;
//		case Engine::OBJ_DIR::DIR_D:
//			m_pAnimator->Play_Animation(L"DesertRhino_Ready_Down", true);
//			break;
//		case Engine::OBJ_DIR::DIR_L:
//			m_pAnimator->Play_Animation(L"DesertRhino_Ready_Left", true);
//			break;
//		case Engine::OBJ_DIR::DIR_R:
//			m_pAnimator->Play_Animation(L"DesertRhino_Ready_Right", true);
//			break;
//		case Engine::OBJ_DIR::DIR_LU:
//			m_pAnimator->Play_Animation(L"DesertRhino_Ready_LeftUp", true);
//			break;
//		case Engine::OBJ_DIR::DIR_RU:
//			m_pAnimator->Play_Animation(L"DesertRhino_Ready_RightUp", true);
//			break;
//		case Engine::OBJ_DIR::DIR_LD:
//			m_pAnimator->Play_Animation(L"DesertRhino_Ready_LeftDown", true);
//			break;
//		case Engine::OBJ_DIR::DIR_RD:
//			m_pAnimator->Play_Animation(L"DesertRhino_Ready_RightDown", true);
//			break;
//		case Engine::OBJ_DIR::DIR_END:
//			return;

		default:
			m_pAnimator->Play_Animation(L"Cupa_Regen_Down", true);
			break;
		}
		break;

	case Engine::MONSTER_STATE::ATTACK:
		switch (eDir)
		{
		case Engine::OBJ_DIR::DIR_U:
			m_pAnimator->Play_Animation(L"Cupa_Attack_Up", true);
			break;
		case Engine::OBJ_DIR::DIR_D:
			m_pAnimator->Play_Animation(L"Cupa_Attack_Down", true);
			break;
		case Engine::OBJ_DIR::DIR_L:
			m_pAnimator->Play_Animation(L"Cupa_Attack_Left", true);
			break;
		case Engine::OBJ_DIR::DIR_R:
			m_pAnimator->Play_Animation(L"Cupa_Attack_Right", true);
			break;
		case Engine::OBJ_DIR::DIR_LU:
			m_pAnimator->Play_Animation(L"Cupa_Attack_LeftUp", true);
			break;
		case Engine::OBJ_DIR::DIR_RU:
			m_pAnimator->Play_Animation(L"Cupa_Attack_RightUp", true);
			break;
		case Engine::OBJ_DIR::DIR_LD:
			m_pAnimator->Play_Animation(L"Cupa_Attack_LeftDown", true);
			break;
		case Engine::OBJ_DIR::DIR_RD:
			m_pAnimator->Play_Animation(L"Cupa_Attack_RightDown", true);
			break;
		case Engine::OBJ_DIR::DIR_END:
			return;

		default:
			break;
		}
		break;

	case Engine::MONSTER_STATE::DIE:
		switch (eDir)
		{
//		case Engine::OBJ_DIR::DIR_U:
//			m_pAnimator->Play_Animation(L"DesertRhino_Idle_Up", true);
//			break;
//		case Engine::OBJ_DIR::DIR_D:
//			m_pAnimator->Play_Animation(L"DesertRhino_Idle_Down", true);
//			break;
//		case Engine::OBJ_DIR::DIR_L:
//			m_pAnimator->Play_Animation(L"DesertRhino_Idle_Left", true);
//			break;
//		case Engine::OBJ_DIR::DIR_R:
//			m_pAnimator->Play_Animation(L"DesertRhino_Idle_Right", true);
//			break;
//		case Engine::OBJ_DIR::DIR_LU:
//			m_pAnimator->Play_Animation(L"DesertRhino_Idle_LeftUp", true);
//			break;
//		case Engine::OBJ_DIR::DIR_RU:
//			m_pAnimator->Play_Animation(L"DesertRhino_Idle_RightUp", true);
//			break;
//		case Engine::OBJ_DIR::DIR_LD:
//			m_pAnimator->Play_Animation(L"DesertRhino_Idle_LeftDown", true);
//			break;
//		case Engine::OBJ_DIR::DIR_RD:
//			m_pAnimator->Play_Animation(L"DesertRhino_Idle_RightDown", true);
//			break;
//		case Engine::OBJ_DIR::DIR_END:
//			return;
		default:
			break;
		}
		break;

	default:
		break;
	}

	if (m_ePreviousState == eState)
		m_pAnimator->GetCurrAnimation()->Set_Idx(iIndex);

	m_ePreviousState = eState;
	m_eDir = eDir;
}

HRESULT CCupa::Add_Component(void)
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
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Idle_Down", L"Proto_Texture_Cupa_Idle_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Idle_Up", L"Proto_Texture_Cupa_Idle_Up", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Idle_Left", L"Proto_Texture_Cupa_Idle_Left", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Idle_Right", L"Proto_Texture_Cupa_Idle_Right", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Idle_LeftUp", L"Proto_Texture_Cupa_Idle_LeftUp", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Idle_LeftDown", L"Proto_Texture_Cupa_Idle_LeftDown", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Idle_RightDown", L"Proto_Texture_Cupa_Idle_RightDown", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Idle_RightUp", L"Proto_Texture_Cupa_Idle_RightUp", 0.1f), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Move_Down", L"Proto_Texture_Cupa_Move_Down", 0.3f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Move_Up", L"Proto_Texture_Cupa_Move_Up", 0.3f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Move_Left", L"Proto_Texture_Cupa_Move_Left", 0.3f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Move_Right", L"Proto_Texture_Cupa_Move_Right", 0.3f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Move_LeftUp", L"Proto_Texture_Cupa_Move_LeftUp", 0.3f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Move_LeftDown", L"Proto_Texture_Cupa_Move_LeftDown", 0.3f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Move_RightDown", L"Proto_Texture_Cupa_Move_RightDown", 0.3f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Move_RightUp", L"Proto_Texture_Cupa_Move_RightUp", 0.3f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Attack_Down", L"Proto_Texture_Cupa_Attack_Down", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Attack_Up", L"Proto_Texture_Cupa_Attack_Up", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Attack_Left", L"Proto_Texture_Cupa_Attack_Left", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Attack_Right", L"Proto_Texture_Cupa_Attack_Right", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Attack_LeftUp", L"Proto_Texture_Cupa_Attack_LeftUp", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Attack_LeftDown", L"Proto_Texture_Cupa_Attack_LeftDown", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Attack_RightDown", L"Proto_Texture_Cupa_Attack_RightDown", 0.2f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Attack_RightUp", L"Proto_Texture_Cupa_Attack_RightUp", 0.2f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Regen_Down", L"Proto_Texture_Cupa_Regen_Down", 0.1f), E_FAIL);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"Cupa_Reaction_Down", L"Proto_Texture_Cupa_Reaction_Down", 0.5f), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"Cupa_Idle_Down", TRUE), E_FAIL);

	return S_OK;
}

CCupa* CCupa::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCupa* pInstance = new CCupa(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Monster Cupa Create Failed");
		return nullptr;
	}

	return pInstance;
}
