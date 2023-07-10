#include "Turret.h"
#include	"Export_Function.h"
CTurret::CTurret(LPDIRECT3DDEVICE9 pGraphicDev) : CFieldObject(pGraphicDev), m_wstrCurState(L""), m_pTarget(nullptr), m_fEnergy(0)
{}


CTurret::CTurret(const CTurret& rhs) : CFieldObject(rhs), m_wstrCurState(rhs.m_wstrCurState), m_pTarget(rhs.m_pTarget), m_fEnergy(rhs.m_fEnergy)
{
}

CTurret::~CTurret()
{
}

HRESULT CTurret::Ready_Object(void) 
{
	FAILED_CHECK(Ready_Component());

	FAILED_CHECK(m_pAnimator->Add_Animation(L"Up", L"Proto_Tex_Turret_Up", 0.15f));
	FAILED_CHECK(m_pAnimator->Add_Animation(L"Down", L"Proto_Tex_Turret_Down", 0.15f));
	FAILED_CHECK(m_pAnimator->Add_Animation(L"LeftUp", L"Proto_Tex_Turret_LeftUp", 0.15f));
	FAILED_CHECK(m_pAnimator->Add_Animation(L"RightUp", L"Proto_Tex_Turret_RightUp", 0.15f));
	FAILED_CHECK(m_pAnimator->Add_Animation(L"LeftDown", L"Proto_Tex_Turret_LeftDown", 0.15f));
	FAILED_CHECK(m_pAnimator->Add_Animation(L"RightDown", L"Proto_Tex_Turret_RightDown", 0.15f));
	FAILED_CHECK(m_pAnimator->Add_Animation(L"Left", L"Proto_Tex_Turret_Left", 0.15f));
	FAILED_CHECK(m_pAnimator->Add_Animation(L"Right", L"Proto_Tex_Turret_Right", 0.15f));

	m_wstrCurState = L"Up";
	m_pAnimator->Play_Animation(L"Up", false);
	Set_Idle();

	return S_OK;
}

_int CTurret::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);

	if (m_fEnergy > 0.f)
	{
		m_fEnergy -= fTimeDelta * 10.f;
		Find_Enemy();
		if (!m_pTarget)
			Set_Idle();
		else
		{
			_vec3 LookEnemy, mypos, enemyPos;
			wstring src;

			m_pTransformCom->Get_Info(INFO_POS, &mypos);
			m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &enemyPos);
			LookEnemy = enemyPos - mypos;
			LookEnemy.y = 0.f;
			D3DXVec3Normalize(&LookEnemy, &LookEnemy);
			
			if ((src = Get_Direction(LookEnemy)) != m_wstrCurState)
			{
				m_wstrCurState = src;
				m_pAnimator->Play_Animation(m_wstrCurState, true);
			}
			else if (!m_pAnimator->GetCurrAnimation()->Is_Loop())
			{
				m_pAnimator->GetCurrAnimation()->Set_Loop(true);
			}
		}
	}
	else if (m_pAnimator->GetCurrAnimation()->Is_Loop())
	{
		m_fEnergy = 0.f;
		m_pTarget = nullptr;
		Set_Idle();
	}

	return __super::Update_Object(fTimeDelta);
}

void CTurret::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CTurret::Render_Object(void)
{
	__super::Render_Object();

}

void CTurret::Add_Subscribe(_uint iNum)
{
	Engine::Add_Subscribe(iNum, this);
}

void CTurret::Free()
{
}

CTurret* CTurret::Create(LPDIRECT3DDEVICE9 p_Dev,  const _uint& p_EventNum, const _vec3 p_Pos)
{
	CTurret* ret = new	CTurret(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Turret Create Failed");
		return nullptr;
	}

	ret->Add_Subscribe(p_EventNum);
	ret->m_pTransformCom->Set_Pos(&p_Pos);

	return ret;
}

void CTurret::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && m_fEnergy < 100.f)
	{
		m_fEnergy += 20.f;
		if (m_fEnergy > 100.f)
			m_fEnergy = 100.f;
	}
}

void CTurret::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CTurret::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CTurret::Event_Start(_uint iEventNum)
{
}

void CTurret::Event_End(_uint iEventNum)
{
}

void CTurret::Set_Idle()
{
	m_pAnimator->GetCurrAnimation()->Set_Loop(false);
	m_pAnimator->GetCurrAnimation()->Set_Idx(m_pAnimator->GetCurrAnimation()->Get_Size() - 1);
}

void CTurret::Find_Enemy()
{
	vector<CGameObject*>& src = Get_Layer(LAYER_TYPE::ENVIRONMENT)->Get_GameObjectVec();

	m_pTarget = nullptr;

	if (src.empty())
	{
		Set_Idle();
		return;
	}
	_vec3	myPos, enemyPos;

	float tmp;

	m_pTransformCom->Get_Info(INFO_POS, &myPos);

	myPos.y = 0;

	for (auto& iter : src)
	{
		if (iter->GetObj_Type() != OBJ_TYPE::OBJ_MONSTER)
			continue;

		iter->Get_TransformCom()->Get_Info(INFO_POS, &enemyPos);
		enemyPos.y = 0;

		if ((tmp = D3DXVec3Length(&(myPos - enemyPos))) && tmp < 5.f)
		{
			if (m_pTarget)
			{
				m_pTarget->Get_TransformCom()->Get_Info(INFO_POS, &enemyPos);
				enemyPos.y = 0;

				if (D3DXVec3Length(&(myPos - enemyPos)) > tmp)
					m_pTarget = iter;
			}
			else
				m_pTarget = iter;
		}
	}


}

wstring CTurret::Get_Direction(const _vec3& dir)
{
	_float		src = D3DXVec3Dot(&_vec3(1, 0, 0), &dir);
	src = D3DXToDegree(acosf(src));

	_vec3 tmp;
	D3DXVec3Cross(&tmp, &_vec3(1, 0, 0), &dir);

	if (src < 22.5f)
	{
		return L"Right";
	}
	else if (22.5f < src && src < 67.5f)
	{
		if (tmp.y > 0.f)
		{
			return L"RightDown";
		}
		else
			return L"RightUp";
	}
	else if (67.5f < src && src < 112.5f)
	{
		if (tmp.y > 0.f)
		{
			return L"Down";
		}
		else
			return L"Up";
	}
	else if (112.5f < src && src < 157.5f)
	{
		if (tmp.y > 0.f)
		{
			return L"LeftDown";
		}
		else
			return L"LeftUp";

	}
	else
		return L"Left";


}

_vec3 CTurret::Get_Direction(const wstring& dir)
{
	_vec3 ret = { 0, 0, 0 };

	if (m_wstrCurState == L"Left")
		ret = { -1, 0, 0 };
	else if (m_wstrCurState == L"Right")
		ret = { 1, 0, 0 };
	else if (m_wstrCurState == L"LeftUp")
	{
		ret = { -1, 0, 1 };
		D3DXVec3Normalize(&ret, &ret);
	}
	else if (m_wstrCurState == L"LeftDown")
	{
		ret = { -1, 0, -1 };
		D3DXVec3Normalize(&ret, &ret);
	}
	else if (m_wstrCurState == L"RightUp")
	{
		ret = { 1, 0, 1 };
		D3DXVec3Normalize(&ret, &ret);
	}
	else if (m_wstrCurState == L"RightDown")
	{
		ret = { 1, 0, -1 };
		D3DXVec3Normalize(&ret, &ret);
	}
	else if (m_wstrCurState == L"Up")
	{
		ret = { 0, 0, 1 };
	}
	else if (m_wstrCurState == L"Down")
	{
		ret = { 0, 0, -1 };
	}


	return ret;


}
