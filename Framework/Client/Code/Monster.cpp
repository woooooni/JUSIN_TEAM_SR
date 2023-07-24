#include "..\Header\Monster.h"
#include "Export_Function.h"
#include "Bullet.h"
#include "Pool.h"
#include "Effect_DieSmoke.h"
#include "Effect_Stun.h"
#include "Player.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eObjId)
	: Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_MONSTER, _eObjId)
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

_int CMonster::Update_Object(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Object(fTimeDelta);		
	if (m_tStat.iHp < 1||m_tStat.iMaxHp<m_tStat.iHp)
		Set_State(MONSTER_STATE::DIE);
	if (Is_Active())
	{	
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
		case MONSTER_STATE::STUN:
			Update_Stun(fTimeDelta);
			break;
		}
	}

//	_vec3 vPos;
//	m_pTransformCom->Get_Info(INFO_POS, &vPos);
//	
//	vPos.y += 0.8f;
//	vPos.z -= 0.01f;
//
//	if (m_pUIBack->Is_Active() &&
//		m_pUIGauge->Is_Active() &&
//		m_pUIFrame->Is_Active())
//	{
//		m_pUIBack->Update_Object(fTimeDelta);
//		m_pUIBack->Get_TransformCom()->Set_Pos(&vPos);
//
//		vPos.z -= 0.005f;
//		m_pUIGauge->Update_Object(fTimeDelta);
//
//		if (m_tStat.iHp == m_tStat.iMaxHp)
//			m_pUIGauge->Get_TransformCom()->Set_Pos(&vPos);
//		else if (m_tStat.iHp > 0 && m_tStat.iHp < m_tStat.iMaxHp)
//		{
//			_vec3 vMovePos = vPos;
//
//			_float fMaxHP = _float(m_tStat.iMaxHp);
//			_float fCurHP = _float(m_tStat.iHp);
//			_float fHP = fCurHP / fMaxHP;
//
//			_float fOriginWidth = _float(m_pUIGauge->Get_TextureCom()->Get_TextureDesc(0).Width);
//			_float fWidth = fOriginWidth - fOriginWidth * fHP;
//
//			_float fIndex = fWidth * 0.004f * 0.5f;
//
//			vMovePos = _vec3((vMovePos.x - fIndex), vMovePos.y, vMovePos.z);
//			m_pUIGauge->Get_TransformCom()->Set_Pos(&vMovePos);
//		}
//
//		vPos.z -= 0.005f;
//		m_pUIFrame->Update_Object(fTimeDelta);
//		m_pUIFrame->Get_TransformCom()->Set_Pos(&vPos);
//	}

	return iExit;
}

void CMonster::LateUpdate_Object(void)
{
//	if (m_pUIBack->Is_Active() &&
//		m_pUIGauge->Is_Active() &&
//		m_pUIFrame->Is_Active())
//	{
//		m_pUIBack->LateUpdate_Object();
//		m_pUIGauge->LateUpdate_Object();
//		m_pUIFrame->LateUpdate_Object();
//	}

	__super::LateUpdate_Object();
}

void CMonster::Render_Object(void)
{
	if (Is_Active())
		__super::Render_Object();

}

HRESULT CMonster::Add_Component(void)
{
	return S_OK;
}

void CMonster::Trace(_float fTimeDelta)
{
}

void CMonster::Set_Stun(_float _fStunTime)
{
	m_fStunTime = _fStunTime;

	m_eState = MONSTER_STATE::STUN;

	CGameObject* pEffect = CPool<CEffect_Stun>::Get_Obj();

	if (!pEffect)
	{
		pEffect = CEffect_Stun::Create(m_pGraphicDev);
		pEffect->Ready_Object();
	}
	_float fScaleY = m_pTransformCom->Get_Scale().y * 0.5f;
	dynamic_cast<CEffect_Stun*>(pEffect)->Get_Effect(this, _vec3(0.0f, fScaleY * 1.0f, 0.0f), _vec3(1.0f, 0.5f, 1.0f), &m_fStunTime);


}

void CMonster::Update_Stun(_float fTimeDelta)
{
	m_fStunTime -= fTimeDelta;

	if (m_fStunTime < 0.0f)
	{
		m_eState = MONSTER_STATE::IDLE;
	}
}

void CMonster::Free()
{
	__super::Free();
}

void CMonster::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
		return;
	switch (pCollider->GetOwner()->GetObj_Type())
	{
	case Engine::OBJ_TYPE::OBJ_ENVIRONMENT:
		Push_Me(pCollider);
		break;
	case Engine::OBJ_TYPE::OBJ_SPAWNER:
		Push_Me(pCollider);
		break;
	case Engine::OBJ_TYPE::OBJ_MONSTER:
		Push_Me(pCollider);
		break;
	case Engine::OBJ_TYPE::OBJ_EFFECT:
		Push_Me(pCollider);
		break;
	case Engine::OBJ_TYPE::OBJ_INTERACTION:
		Push_Me(pCollider);
		break;
	case Engine::OBJ_TYPE::OBJ_BULLET:
		if ( dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Owner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
		{
			m_tStat.iHp -=	dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Atk(); 
			_vec3 vTargetPos;
			_vec3 vPos;
			_vec3 vDir;
			pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vDir = vPos - vTargetPos;
			vDir.y = 0.0f;
			D3DXVec3Normalize(&vDir, &vDir);
			if (m_bPushable)
			{
				m_pRigidBodyCom->SetVelocity(_vec3(0.0f, 0.0f, 0.0f));
				m_pRigidBodyCom->AddForce(vDir * 70.0f);
			}
				
			

			if (dynamic_cast<CPlayer*>(dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Owner())->Get_Skill() == PLAYER_SKILL::LIGHTNING)
			{
				Set_Stun(3.0f);
			}

		}
		if (dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Owner()->GetObj_Id() == OBJ_ID::TURRET)
		{
			m_tStat.iHp -= dynamic_cast<CBullet*> (pCollider->GetOwner())->Get_Atk();
			_vec3 vTargetPos;
			_vec3 vPos;
			_vec3 vDir;
			pCollider->GetOwner()->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vDir = vPos - vTargetPos;
			vDir.y = 0.0f;
			D3DXVec3Normalize(&vDir, &vDir);
			if (m_bPushable)
			{
				m_pRigidBodyCom->SetVelocity(_vec3(0.0f, 0.0f, 0.0f));
				m_pRigidBodyCom->AddForce(vDir * 70.0f);
			}
		}
		break;
	default:
		break;
	}

}

void CMonster::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && pCollider->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
		return;
	switch (pCollider->GetOwner()->GetObj_Type())
	{
	case Engine::OBJ_TYPE::OBJ_ENVIRONMENT:
		Push_Me(pCollider);
		break;
	case Engine::OBJ_TYPE::OBJ_SPAWNER:
		Push_Me(pCollider);
		break;
	case Engine::OBJ_TYPE::OBJ_MONSTER:
		Push_Me(pCollider);
		break;
	case Engine::OBJ_TYPE::OBJ_EFFECT:
		Push_Me(pCollider);
		break;
	case Engine::OBJ_TYPE::OBJ_INTERACTION:
		Push_Me(pCollider);
		break;
	default:
		break;
	}
}

void CMonster::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CMonster::Push_Me(CCollider* other)
{
	if (!other->Is_Active() || !m_pColliderCom->Is_Active())
		return;

	if (other->GetOwner()->GetObj_Type() == OBJ_TYPE::OBJ_PLAYER)
		return;

	if (!m_bPushable)
		return;	
	const _vec3& vLeftScale = ((CBoxCollider*)m_pColliderCom)->Get_Scale();
	const _vec3& vLeftPos = ((CBoxCollider*)m_pColliderCom)->Get_Pos();
	const _vec3& vRightScale = ((CBoxCollider*)other)->Get_Scale();
	const _vec3& vRightPos = ((CBoxCollider*)other)->Get_Pos();

	if (fabs(vRightPos.x - vLeftPos.x) < 0.5f * (vLeftScale.x + vRightScale.x)
		&& fabs(vRightPos.y - vLeftPos.y) < 0.5f * (vLeftScale.y + vRightScale.y)
		&& fabs(vRightPos.z - vLeftPos.z) < 0.5f * (vLeftScale.z + vRightScale.z))
	{
		float colX = 0.5f * (vLeftScale.x + vRightScale.x) - fabs(vRightPos.x - vLeftPos.x);
		float colY = 0.5f * (vLeftScale.y + vRightScale.y) - fabs(vRightPos.y - vLeftPos.y);
		float colZ = 0.5f * (vLeftScale.z + vRightScale.z) - fabs(vRightPos.z - vLeftPos.z);

		if (colX < colZ)
		{
			if (vLeftPos.x > vRightPos.x)
				m_pTransformCom->Move_Pos(&_vec3(colX * 0.5f, 0, 0), 1, 0.5);
			else
				m_pTransformCom->Move_Pos(&_vec3(colX * -0.5f, 0, 0),1, 0.5);
		}

		else if (colZ < colX)
		{
			if (vLeftPos.z > vRightPos.z)
				m_pTransformCom->Move_Pos( &_vec3(0, 0, 0.5f * colZ),1, 0.5);
			else
				m_pTransformCom->Move_Pos( &_vec3(0, 0, -0.5f * colZ),1, 0.5 );

		}
	}

}

void CMonster::On_Death()
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	CGameObject* pSmoke = CPool<CEffect_DieSmoke>::Get_Obj();
	if (pSmoke)
		dynamic_cast<CEffect_DieSmoke*>(pSmoke)->Get_Effect(vPos, _vec3(2.f,2.f, 2.f));
	else
	{
		pSmoke = dynamic_cast<CEffect_DieSmoke*>(pSmoke)->Create(Engine::Get_Device());
		if (pSmoke)
			dynamic_cast<CEffect_DieSmoke*>(pSmoke)->Get_Effect(vPos, _vec3(2.f, 2.f, 2.f));
	}
}
