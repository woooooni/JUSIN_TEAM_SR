#include "Player_Skill_Missile.h"

#include "Transform.h"
#include "Player.h"
#include "KeyMgr.h"
#include "Export_Function.h"
#include "Player_Bullet_Bomb.h"
#include "Pool.h"

CPlayer_Skill_Missile::CPlayer_Skill_Missile(CGameObject* _pOwner)
	:CPlayer_State(_pOwner),
	m_fAccShootTime(0.0f),
	m_fShootTime(0.1f),
	m_iMaxBombCount(6),
	m_iBombCount(0),
	m_fMaxLength(6.0f)
{
}

CPlayer_Skill_Missile::~CPlayer_Skill_Missile()
{
}

HRESULT CPlayer_Skill_Missile::Ready_State(void)
{
	m_pTarget = nullptr;

	if (Find_Target())
	{
		Set_PlayerDir();
	}

	switch (m_pOwner->GetObj_Dir())
	{
	case OBJ_DIR::DIR_U:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Up", true);
		m_vDir = { 0.0f, 0.0f, 1.0f };
		break;
	case OBJ_DIR::DIR_D:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Down", true);
		m_vDir = { 0.0f, 0.0f, -1.0f };
		break;
	case OBJ_DIR::DIR_L:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Left", true);
		m_vDir = { -1.0f, 0.0f, 0.0f };
		break;
	case OBJ_DIR::DIR_R:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_Right", true);
		m_vDir = { 1.0f,0.0f,0.0f };
		break;
	case OBJ_DIR::DIR_LD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_LeftDown", true);
		m_vDir = { -1.0f,0.0f,-1.0f };
		break;
	case OBJ_DIR::DIR_LU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_LeftUp", true);
		m_vDir = { -1.0f,0.0f,1.0f };
		break;
	case OBJ_DIR::DIR_RU:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_RightUp", true);
		m_vDir = { 1.0f,0.0f,1.0f };
		break;
	case OBJ_DIR::DIR_RD:
		dynamic_cast<CAnimator*>(m_pOwner->Get_Component(COMPONENT_TYPE::COM_ANIMATOR, ID_DYNAMIC))->Play_Animation(L"Idle_RightDown", true);
		m_vDir = { 1.0f,0.0f,-1.0f };
		break;
	}
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vDir *= 0.5f;

	m_iBombCount = 0;
	m_fAccShootTime = 0.0f;


	if (dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat())
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Reset();
		dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Scale(1.0f);

		_vec3 vPos;
		m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		vPos.y += 0.3f;
		vPos.z -= 0.0001f;
		dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Pos(vPos);
	}

	return S_OK;
}

_int CPlayer_Skill_Missile::Update_State(const _float& fTimeDelta)
{
	if (!m_pTarget)
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
		return 0;
	}
		


	if (m_iMaxBombCount > m_iBombCount)
	{
		m_fAccShootTime += fTimeDelta;
		if (m_fAccShootTime > m_fShootTime)
		{
			Shoot();
			m_fAccShootTime = 0.0f;
		}
	}
	else
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Change_State(PLAYER_STATE::IDLE);
	}
	

    return 0;
}

void CPlayer_Skill_Missile::LateUpdate_State(void)
{
}

void CPlayer_Skill_Missile::Render_State(void)
{
}

void CPlayer_Skill_Missile::Reset_State(void)
{
}

HRESULT CPlayer_Skill_Missile::Shoot(void)
{
	_vec3 vPos;
	_vec3 vTarget;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTarget);
	vPos -= m_vDir;
	
	_vec3 vUp = {0.0f, 1.0f, 0.0f};
	_vec3 vLook = m_vDir;
	D3DXVec3Normalize(&vLook, &vLook);
	_vec3 vRight;
	D3DXVec3Cross(&vRight, &vUp, &vLook);
	

	_vec3 vDir = vUp + vRight;
	D3DXVec3Normalize(&vDir, &vDir);

	_float fAngle = (rand() % 11) * 9.0f;
	fAngle = D3DXToRadian(fAngle);
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &vLook, fAngle);
	D3DXVec3TransformNormal(&vDir, &vDir, &matRot);

	vLook *= (rand() % 11 + 5) * -0.1f;
	vDir += vLook;
	D3DXVec3Normalize(&vDir, &vDir);
	

	CGameObject* pBomb = CPool<CPlayer_Bullet_Bomb>::Get_Obj();

	if (!pBomb)
	{
		pBomb = CPlayer_Bullet_Bomb::Create(Engine::Get_Device());
		NULL_CHECK_RETURN(pBomb, E_FAIL);
		pBomb->Set_Active(true);
	}
	FAILED_CHECK_RETURN(Engine::Get_Layer(LAYER_TYPE::PLAYER)->Add_GameObject(L"Bomb", pBomb), E_FAIL);
	dynamic_cast<CPlayer_Bullet_Bomb*>(pBomb)->Shoot(m_pTarget, vDir, 25.0f, vPos);
	dynamic_cast<CBullet*>(pBomb)->Set_Owner(m_pOwner);
	++m_iBombCount;


	return 0;
}

bool CPlayer_Skill_Missile::Find_Target(void)
{
	vector<CGameObject*>& vecMonsters = Engine::Get_Layer(LAYER_TYPE::MONSTER)->Get_GameObjectVec();

	_vec3 vPos;
	_vec3 vDir;
	_vec3 vTargetPos;
	_float fTargetLength = 0.0f;

	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

	bool bFind = false;

	for (auto iter : vecMonsters)
	{
		if (iter->GetObj_Type() != OBJ_TYPE::OBJ_MONSTER)
			continue;

		CTransform* pTranform = iter->Get_TransformCom();
		pTranform->Get_Info(INFO_POS, &vTargetPos);
		vDir = vTargetPos - vPos;

		if (D3DXVec3Length(&vDir) < m_fMaxLength)
		{
			if (m_pTarget)
			{
				if (D3DXVec3Length(&vDir) < fTargetLength)
				{
					m_pTarget = iter;
					fTargetLength = D3DXVec3Length(&vDir);
				}
			}
			else
			{
				m_pTarget = iter;
				fTargetLength = D3DXVec3Length(&vDir);
			}
		}
	}

	if (m_pTarget)
		return true;
	else
		return false;


	return false;
}

void CPlayer_Skill_Missile::Set_PlayerDir()
{
	OBJ_DIR eTargetDir = OBJ_DIR::DIR_END;
	_vec3 vTargetPos;
	_vec3 vPos;
	_vec3 vDir;
	_vec3 vAxis = { 0.0f, 0.0f, 1.0f };
	m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &vTargetPos);
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	vDir = vTargetPos - vPos;
	vDir.y = 0.0f;
	D3DXVec3Normalize(&vDir, &vDir);

	_float fAngle = D3DXVec3Dot(&vDir, &vAxis);
	fAngle = acosf(fAngle);

	if (vDir.x < 0.0f)
		fAngle = D3DX_PI * 2 - fAngle;


	fAngle = D3DXToDegree(fAngle);



	_uint iDir = fAngle / 22.5f;

	if (iDir == 0 || iDir == 15 || iDir == 16)
	{
		eTargetDir = OBJ_DIR::DIR_U;
	}
	else if (iDir == 1 || iDir == 2)
	{
		eTargetDir = OBJ_DIR::DIR_RU;
	}
	else if (iDir == 3 || iDir == 4)
	{
		eTargetDir = OBJ_DIR::DIR_R;
	}
	else if (iDir == 5 || iDir == 6)
	{
		eTargetDir = OBJ_DIR::DIR_RD;
	}
	else if (iDir == 7 || iDir == 8)
	{
		eTargetDir = OBJ_DIR::DIR_D;
	}
	else if (iDir == 9 || iDir == 10)
	{
		eTargetDir = OBJ_DIR::DIR_LD;
	}
	else if (iDir == 11 || iDir == 12)
	{
		eTargetDir = OBJ_DIR::DIR_L;
	}
	else if (iDir == 13 || iDir == 14)
	{
		eTargetDir = OBJ_DIR::DIR_LU;
	}

	m_pOwner->SetObj_Dir(eTargetDir);
}
