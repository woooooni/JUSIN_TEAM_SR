#include "..\Header\Monster.h"
#include "Export_Function.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER)
	, m_eState(MONSTER_STATE::REGEN)
	, m_fSpeed(5.f)
{

}
CMonster::CMonster(const CMonster& rhs)
	: Engine::CGameObject(rhs)
	, m_fSpeed(rhs.m_fSpeed)
	, m_eState(MONSTER_STATE::REGEN)
	, m_pTarget(nullptr), m_tStat(rhs.m_tStat)
{

}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

Engine::_int CMonster::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	switch (m_eState)
	{
	case MONSTER_STATE::IDLE:
		Update_Idle(fTimeDelta);
		break;
	case MONSTER_STATE::MOVE:
		Update_Move(fTimeDelta);
		break;
	case MONSTER_STATE::REGEN:
		Update_Regen(fTimeDelta);
		break;
	case MONSTER_STATE::ATTACK:
		Update_Attack(fTimeDelta);
		break;
	case MONSTER_STATE::DIE:
		Update_Die(fTimeDelta);
		break;
	}

	return iExit;
}

void CMonster::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CMonster::Render_Object(void)
{
	__super::Render_Object();
}

HRESULT CMonster::Add_Component(void)
{
	return S_OK;
}

void CMonster::Trace(_float fTimeDelta)
{
}

void CMonster::Free()
{
	__super::Free();
}

