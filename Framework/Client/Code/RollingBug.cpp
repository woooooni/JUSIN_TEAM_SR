#include "RollingBug.h"
#include "Export_Function.h"


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
	Set_Speed(1.f);

	m_tStat = { 3, 3, 1 }; // MaxHp, Hp, Attack

	_vec3 vScale = _vec3(0.8f, 0.8f, 0.8f);
	m_pTransformCom->Set_Scale(vScale);

	return S_OK;
}

_int CRollingBug::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player");
	if (nullptr == pTarget)
		return S_OK;

	Set_Target(pTarget);

	_vec3 vPlayerPos, vOriginPos, vPos;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vOriginPos = m_tBugInfo.vDefaultPos;

	m_vBugDir = vOriginPos - vPos;
	m_vPlayerDir = vPlayerPos - vPos;

	if (D3DXVec3Length(&m_vPlayerDir) <= 7.f)
		Set_State(MONSTER_STATE::ATTACK);

	if (D3DXVec3Length(&m_vPlayerDir) > 7.f)
		Set_State(MONSTER_STATE::IDLE);

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CRollingBug::LateUpdate_Object(void)
{
	if (!Is_Active())
		return ;
	__super::LateUpdate_Object();
}

void CRollingBug::Render_Object(void)
{
	if (!Is_Active())
		return ;
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	__super::Render_Object();
}

void CRollingBug::Update_Idle(_float fTimeDelta)
{
	_vec3 vPos, vDir, vOriginPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vOriginPos = m_tBugInfo.vDefaultPos;

	vDir = vOriginPos - vPos;
	m_vBugDir = vDir;

	switch (m_tBugInfo.eType)
	{
	case BUGCOLORTYPE::PINK:
		m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Down", TRUE);
		break;

	case BUGCOLORTYPE::BLUE:
		m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Down", TRUE);
		break;

	case BUGCOLORTYPE::YELLOW:
		m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Down", TRUE);
		break;

	default:
		break;
	}

	if (D3DXVec3Length(&m_vPlayerDir) > 7.f)
	{
		D3DXVec3Normalize(&m_vBugDir, &m_vBugDir);
		m_pTransformCom->Move_Pos(&m_vBugDir, fTimeDelta, 1.5f * Get_Speed());

		if (m_fMoveTime > 10.f)
			m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;
}

void CRollingBug::Update_Die(_float fTimeDelta)
{

}

void CRollingBug::Update_Regen(_float fTimeDelta)
{

}

void CRollingBug::Update_Move(_float fTimeDelta)
{

}

void CRollingBug::Update_Attack(_float fTimeDelta)
{
	switch (m_tBugInfo.eType)
	{
	case BUGCOLORTYPE::PINK:
		m_pAnimator->Play_Animation(L"RollingBug_Pink_Attack_Down", TRUE);
		break;

	case BUGCOLORTYPE::BLUE:
		m_pAnimator->Play_Animation(L"RollingBug_Blue_Attack_Down", TRUE);
		break;

	case BUGCOLORTYPE::YELLOW:
		m_pAnimator->Play_Animation(L"RollingBug_Yellow_Attack_Right", TRUE);
		break;
	}

	Trace(fTimeDelta);

	if (D3DXVec3Length(&m_vPlayerDir) > 7.f)
	{
		D3DXVec3Normalize(&m_vBugDir, &m_vBugDir);
		m_pTransformCom->Move_Pos(&m_vBugDir, fTimeDelta, 1.5f * Get_Speed());

		if (D3DXVec3Length(&m_vPlayerDir) < 1.f)
		{
			Set_State(MONSTER_STATE::IDLE);

			switch (m_tBugInfo.eType)
			{
			case BUGCOLORTYPE::PINK:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Move_Down", TRUE);
				break;

			case BUGCOLORTYPE::BLUE:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Move_Down", TRUE);
				break;

			case BUGCOLORTYPE::YELLOW:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Move_Down", TRUE);
				break;

			default:
				break;
			}
		}

	}
}

void CRollingBug::Trace(_float fTimeDelta)
{
	_vec3 vTargetPos, vPos, vDir;

	vDir = m_vPlayerDir;
	vDir.y = 0.f;

	D3DXVec3Normalize(&vDir, &vDir);
	m_pTarget = nullptr;

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 2 * Get_Speed());

	if (m_fMoveTime > 10.f)
		m_fMoveTime = 0.f;

	m_fMoveTime += 10.f * fTimeDelta;
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

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	// Texture 일부만 제작 완료
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Idle_Down", L"Proto_Texture_RollingBug_Pink_Idle_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Idle_Down", L"Proto_Texture_RollingBug_Blue_Idle_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Idle_Down", L"Proto_Texture_RollingBug_Yellow_Idle_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Move_Down", L"Proto_Texture_RollingBug_Pink_Move_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Move_Down", L"Proto_Texture_RollingBug_Blue_Move_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Move_Down", L"Proto_Texture_RollingBug_Yellow_Move_Down", 0.1f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Attack_Down", L"Proto_Texture_RollingBug_Pink_Attack_UpDown", 0.08f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Attack_Down", L"Proto_Texture_RollingBug_Blue_Attack_UpDown", 0.08f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Attack_Right", L"Proto_Texture_RollingBug_Yellow_Attack_LeftRight", 0.08f), E_FAIL);

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