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

	
	//플레이어를 계속 추격하는 몬스터
	//몬스터 색상에 해당하는 빛이 켜지면 추격을 멈춘다.
	

	// 공끼리 겹치지 않게 할 것
	// 일정거리 이상으로 가면 IDLE상태로 변경 (움직임도 없음 -> 랜덤위치로 움직이게 수정할것)

Set_State(MONSTER_STATE::IDLE);
Set_Speed(1.f);

_vec3 vScale = _vec3(0.8f, 0.8f, 0.8f);

m_pTransformCom->Set_Scale(vScale);

// Texture 일부만 제작 완료
FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Idle_Down", L"Proto_Texture_RollingBug_Pink_Idle_Down", 0.1f), E_FAIL);
FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Idle_Down", L"Proto_Texture_RollingBug_Blue_Idle_Down", 0.1f), E_FAIL);
FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Idle_Down", L"Proto_Texture_RollingBug_Yellow_Idle_Down", 0.1f), E_FAIL);
FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Move_Down", L"Proto_Texture_RollingBug_Move_Down", 0.1f), E_FAIL);
FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Pink_Attack_Down", L"Proto_Texture_RollingBug_Pink_Attack_UpDown", 0.08f), E_FAIL);
FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Blue_Attack_Down", L"Proto_Texture_RollingBug_Blue_Attack_UpDown", 0.08f), E_FAIL);
FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"RollingBug_Yellow_Attack_Right", L"Proto_Texture_RollingBug_Yellow_Attack_LeftRight", 0.08f), E_FAIL);

switch (m_tBugInfo.eType)
{
case BUGCOLORTYPE::PINK:
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Down", TRUE), E_FAIL);
	break;

case BUGCOLORTYPE::BLUE:
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Down", TRUE), E_FAIL);
	break;

case BUGCOLORTYPE::YELLOW:
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Down", TRUE), E_FAIL);
	break;

default:
	break;
}

return S_OK;
}

_int CRollingBug::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	CGameObject* pTarget = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT)->Find_GameObject(L"Player");
	NULL_CHECK_RETURN(pTarget, E_FAIL);

	Set_Target(pTarget);

	_vec3 vPlayerPos, vPos, vDir;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	vDir = vPlayerPos - vPos; // DIrection Vec
	m_vDir = vDir;

	if (D3DXVec3Length(&vDir) <= 7.f &&
		D3DXVec3Length(&vDir) >= 1.5f)
	{
		Set_State(MONSTER_STATE::ATTACK);

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
	}

	if (D3DXVec3Length(&vDir) > 7.f)
	{
		// 원래 본인 position으로 돌아간다
		_vec3 vOriginPos = m_tBugInfo.vDefaultPos;
		vDir = vOriginPos - vPos;
		m_vDir = vDir;
	}

	if (D3DXVec3Length(&vDir) < 1.5f)
		Set_State(MONSTER_STATE::IDLE);

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CRollingBug::LateUpdate_Object(void)
{

	__super::LateUpdate_Object();
}

void CRollingBug::Render_Object(void)
{
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
	m_vDir = vDir;

	if (m_fMoveTime > 10.f)
	{

		m_fMoveTime = 0.f;
	}
	if (D3DXVec3Length(&vDir) < 1.5f);
	{
		Set_State(MONSTER_STATE::IDLE);

		switch (m_tBugInfo.eType)
		{
		case BUGCOLORTYPE::PINK:
			m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Down", TRUE);
			break;

		case BUGCOLORTYPE::BLUE:
			m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Down", TRUE);
			break;

		case BUGCOLORTYPE::YELLOW:
			m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Down", TRUE);
			break;

		default:
			break;
		}

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
	_vec3 vPlayerPos, vDir, vPos, vDst;

	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	m_vDir = vPlayerPos - vPos;

	if (m_fMoveTime > 5.f)
	{
		//		if (rand() % 10 > 8)
		//		{
		//			Set_State(MONSTER_STATE::MOVE);
		//			m_pAnimator->Play_Animation(L"RollingBug_Move_Down", TRUE);
		//		}

				//vDst = { float(rand() % 10) - 5.f, 0.f, float(rand() % 10) - 5.f };
		vDst = m_vDir;

		if (vDst != m_vDst)
			m_vDst = vDst;

		m_fMoveTime = 0.f;
	}

	m_fMoveTime += 10.f * fTimeDelta;

	vDir = m_vDst;
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, Get_Speed());
}

void CRollingBug::Update_Attack(_float fTimeDelta)
{
	Trace(fTimeDelta);

	_vec3 vPos, vDir, vOriginPos;

	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	vOriginPos = m_tBugInfo.vDefaultPos;

	vDir = vOriginPos - vPos;
	m_vDir = vDir;

	if (D3DXVec3Length(&vDir) > 7.f)
	{
		// 원래 본인 position으로 돌아간다
		//	_vec3 vOriginPos = m_tBugInfo.vDefaultPos;
		//	vDir = vOriginPos - vPos;
		//	m_vDir = vDir;

		if (D3DXVec3Length(&vDir) < 1.5f);
		{

			Set_State(MONSTER_STATE::IDLE);

			switch (m_tBugInfo.eType)
			{
			case BUGCOLORTYPE::PINK:
				m_pAnimator->Play_Animation(L"RollingBug_Pink_Idle_Down", TRUE);
				break;

			case BUGCOLORTYPE::BLUE:
				m_pAnimator->Play_Animation(L"RollingBug_Blue_Idle_Down", TRUE);
				break;

			case BUGCOLORTYPE::YELLOW:
				m_pAnimator->Play_Animation(L"RollingBug_Yellow_Idle_Down", TRUE);
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

	vDir = m_vDir;
	vDir.y = 0.f;

	D3DXVec3Normalize(&vDir, &vDir);
	m_pTarget = nullptr;

	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 2 * Get_Speed());

	if (m_fMoveTime > 10.f)
	{

		Set_State(MONSTER_STATE::ATTACK);

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

		default:
			break;
		}

		m_fMoveTime = 0.f;

	}

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