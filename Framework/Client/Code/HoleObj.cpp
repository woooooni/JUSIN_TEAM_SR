#include "HoleObj.h"
#include	"Export_Function.h"
#include	"PushStone.h"
#include	"Pool.h"

CHoleObj::CHoleObj(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::BALPAN_OBJ), m_bIn(false), m_pStone(nullptr)
{
}

CHoleObj::CHoleObj(const CHoleObj& rhs) : CFieldObject(rhs), m_bIn(rhs.m_bIn), m_pStone(rhs.m_pStone)
{
}

CHoleObj::~CHoleObj()
{
}

HRESULT CHoleObj::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Idle", L"Proto_Tex_Hole", 0.f);
	m_pAnimator->Add_Animation(L"In", L"Proto_Tex_Hole_In", 0.1f);

	m_pAnimator->Play_Animation(L"Idle", false);

	return S_OK;
}

_int CHoleObj::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	if (!m_bIn)
	{
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);
	}

	return __super::Update_Object(fTimeDelta);
}

void CHoleObj::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CHoleObj::Render_Object(void)
{
	__super::Render_Object();
}

void CHoleObj::Free()
{
	__super::Free();
}

void CHoleObj::Reset_Event()
{
	if (m_bIn)
	{
		m_bIn = false;
		m_pAnimator->Play_Animation(L"Idle", false);
		m_pStone->Reset_Event();
		m_pStone = nullptr;
	}
}

CHoleObj* CHoleObj::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos)
{
	CHoleObj* ret = new CHoleObj(p_Dev);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("holeobj create failed");
		return nullptr;

	}

	ret->m_pTransformCom->Set_Pos(&p_Pos);
	ret->Set_MinHeight(0.01f);
	ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));
	return ret;
}

void CHoleObj::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	CPushStone* tmp;
	if (tmp = dynamic_cast<CPushStone*>(pCollider->GetOwner()))
	{
		m_bIn = true;
		tmp->Set_Off();
		m_pAnimator->Play_Animation(L"In", false);
		m_pStone = tmp;
		Stop_Sound(CHANNELID::SOUND_EFFECT_INTERACTION);
		Play_Sound(L"SFX_41_Catapult_StoneHit.wav", CHANNELID::SOUND_EFFECT_ENVIRONMENT, .5f);

	}
}

void CHoleObj::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER
		|| _eCollisionGroup == COLLISION_GROUP::COLLIDE_MONSTER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_PUSH)
	{
		_float fDeltaX = 0.f, fDeltaY = 0.f, fDeltaZ = 0.f;

		_vec3 vPos, vOtherPos;
		_vec3 vMin, vMax;
		_vec3 vOtherMin, vOtherMax;

		CTransform* pTransform = m_pTransformCom;
		CTransform* pOtherTransform = pCollider->GetOwner()->Get_TransformCom();

		CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pColliderCom);
		CBoxCollider* pOtherBoxCollider = dynamic_cast<CBoxCollider*>(pCollider);


		NULL_CHECK_RETURN(pTransform, );
		NULL_CHECK_RETURN(pOtherTransform, );
		NULL_CHECK_RETURN(pBoxCollider, );
		NULL_CHECK_RETURN(pOtherBoxCollider, );

		pTransform->Get_Info(INFO_POS, &vPos);
		pOtherTransform->Get_Info(INFO_POS, &vOtherPos);

		_vec3 vDir = vOtherPos - vPos;
		_float fX = (pBoxCollider->Get_Scale().x * 0.5f) + (pOtherBoxCollider->Get_Scale().x * 0.5f);
		_float fY = (pBoxCollider->Get_Scale().y * 0.5f) + (pOtherBoxCollider->Get_Scale().y * 0.5f);
		_float fZ = (pBoxCollider->Get_Scale().z * 0.5f) + (pOtherBoxCollider->Get_Scale().z * 0.5f);


		if (fabs(vDir.x) >= fX)
			return;

		if (fabs(vDir.y) >= fY)
			return;

		if (fabs(vDir.z) >= fZ)
			return;



		if (fX - fabs(vDir.x) < fZ - fabs(vDir.z) && fX - fabs(vDir.x) < fY - fabs(vDir.y))
		{
			if (vDir.x < 0.f)
			{
				vDir.x -= (fX - fabs(vDir.x));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				vDir.x += (fX - fabs(vDir.x));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}
		else if (fY - fabs(vDir.y) < fZ - fabs(vDir.z) && fY - fabs(vDir.y) < fX - fabs(vDir.x))
		{
			if (vDir.y < 0.f)
			{
				vDir.y -= (fY - fabs(vDir.y));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				vDir.y += (fY - fabs(vDir.y));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}
		else if (fZ - fabs(vDir.z) < fX - fabs(vDir.x) && fZ - fabs(vDir.z) < fY - fabs(vDir.y))
		{
			if (vDir.z < 0.f)
			{
				vDir.z -= (fZ - fabs(vDir.z));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				vDir.z += (fZ - fabs(vDir.z));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}


	}

}

void CHoleObj::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}
