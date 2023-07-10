#include "Catapult.h"
#include "Export_Function.h"
#include	"Player.h"
#include	"PushStone.h"

CCatapult::CCatapult(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev), m_pThrowingStone(nullptr), m_vStonePos(0.f, -0.f, -0.5f), m_pRevTexture(nullptr), m_vCenterPos(0, 0.5f, 0), m_fThrowAngle(-90.f), m_bIsThrowing(false)
{
	m_tInfo.m_bIsPushable = true;
	m_tInfo.m_bIsBreakable = true;
}

CCatapult::CCatapult(const CCatapult& rhs) : CFieldObject(rhs), m_pThrowingStone(rhs.m_pThrowingStone), m_vStonePos(rhs.m_vStonePos), m_pRevTexture(rhs.m_pRevTexture), m_vCenterPos(rhs.m_vCenterPos), m_fThrowAngle(rhs.m_fThrowAngle), m_bIsThrowing(rhs.m_bIsThrowing)
{
}

CCatapult::~CCatapult()
{
}

HRESULT CCatapult::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Normal", L"Proto_Tex_Catapult_Idle", 0.1f);
	m_pAnimator->Add_Animation(L"Throw", L"Proto_Tex_Catapult_Fire", 0.1f);
	m_pAnimator->Add_Animation(L"Body", L"Proto_Tex_Catapult_Body", 0.f);

	m_pAnimator->Play_Animation(L"Body", false);

	CComponent* pComponent;
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Catapult_Scoop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent->insert({ COMPONENT_TYPE::COM_TEXTURE, pComponent });

	pComponent = m_pRevTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Catapult_Scoop_Rev"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);



	return S_OK;
}

_int CCatapult::Update_Object(const _float& fTimeDelta)
{


	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);

	if (m_pThrowingStone)
	{
		_vec3	src, tmp;
		_float dst;

		m_pThrowingStone->Get_TransformCom()->Get_Info(INFO_POS, &src);
		m_pTransformCom->Get_Info(INFO_POS, &tmp);
		tmp += m_vStonePos;
		if ((dst = D3DXVec3Length(&(tmp - src))) && dst > 0.5f )
			m_pThrowingStone->Get_TransformCom()->Move_Pos(D3DXVec3Normalize(&_vec3(), &(tmp - src)), 11, fTimeDelta);
		else
			m_pThrowingStone->Get_TransformCom()->Set_Pos(&(tmp + _vec3(0, 0.001f, 0)));

	}

	if (m_bIsThrowing && m_fThrowAngle < 90.f)
	{
		m_fThrowAngle += fTimeDelta * 720.f;
	}
	else if (!m_bIsThrowing && m_fThrowAngle > -90.f)
	{
		m_fThrowAngle -= fTimeDelta * 480.f;
	}
	else if (m_bIsThrowing &&  m_fThrowAngle >= 90.f)
	{
		m_bIsThrowing = false;
		m_fThrowAngle = 90.f;
	}
	else if (!m_bIsThrowing && m_fThrowAngle < -90.f)
	{
		m_fThrowAngle = -90.f;
	}

	return __super::Update_Object(fTimeDelta);
}

void CCatapult::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

}

void CCatapult::Render_Object(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	_matrix	mat;

	D3DXMatrixRotationX(&mat, D3DXToRadian(m_fThrowAngle));
	_vec3 vec, myPos;

	m_pTransformCom->Get_Info(INFO_POS, &myPos);

	D3DXVec3TransformCoord(&vec, &m_vCenterPos, &mat);
	mat._41 += vec.x + myPos.x ;
	mat._42 += vec.y + myPos.y + 0.02f;
	mat._43 += vec.z + myPos.z;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	vec = { mat._31, mat._32, mat._33 };

	vec *= -0.01f;

	mat._41 += vec.x;
	mat._42 += vec.y;
	mat._43 += vec.z;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);

	m_pRevTexture->Render_Texture();
	m_pBufferCom->Render_Buffer();


	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


}

void CCatapult::Free()
{
	__super::Free();
}

CCatapult* CCatapult::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos)
{
	CCatapult* ret = new CCatapult(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Catapult create Failed");
		return nullptr;
	}
	ret->m_pTransformCom->Set_Pos(&_vec3(p_Pos.x , 0.1f, p_Pos.z));
	ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));
	ret->m_pColliderCom->Set_Offset({ 0, 0.5f, 0 });

	return ret;
}

void CCatapult::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	CPlayer* src;
	CPushStone*	tmp;
	
	if ((_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING))
	{
		if (m_pThrowingStone)
		{
			Throw_Stone();
		}

	}
	else if ((tmp = dynamic_cast<CPushStone*>(pCollider->GetOwner())) && !tmp->Is_Flying())
	{
		m_pThrowingStone = tmp;
		m_pThrowingStone->Get_ColliderCom()->Set_Active(false);
	}
}

void CCatapult::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BALPAN || _eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING)
		return;

	CPushStone*	tmp; 
	if ((tmp = dynamic_cast<CPushStone*>(pCollider->GetOwner())) && tmp->Is_Flying())
	{
		return;
	}


	Push_Me(pCollider);
}

void CCatapult::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}



void CCatapult::Event_Start(_uint iEventNum)
{
}

void CCatapult::Event_End(_uint iEventNum)
{
}

void CCatapult::Set_SubscribeEvent(_uint pEvent)
{
}

void CCatapult::Throw_Stone()
{
	_vec3 src;
	m_pThrowingStone->Get_TransformCom()->Get_Info(INFO_POS, &src);
	src.y = 1.f;
	m_pThrowingStone->Get_TransformCom()->Set_Pos(&src);

	m_pThrowingStone->Get_RigidBodyCom()->AddForce({ 0, 100, 40.f });
	m_pThrowingStone->Get_ColliderCom()->Set_Active(true);
	m_pThrowingStone->Get_RigidBodyCom()->SetGravity(true);
	m_pThrowingStone->Get_RigidBodyCom()->SetGround(false);
	dynamic_cast<CPushStone*>(m_pThrowingStone)->Fire();
	m_pThrowingStone = nullptr;
	m_bIsThrowing = true;

}
