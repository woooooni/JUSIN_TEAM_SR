#include "ButterFly.h"
#include	"Export_Function.h"
#include	<time.h>


CButterFly::CButterFly(LPDIRECT3DDEVICE9 pDev) 
	: CItem(pDev, ITEM_TYPE::ETC, OBJ_ID::BUTTERFLY)
	, m_vMovingDir({-1, 0, 0})
	, m_fChangeTime(1.f)
{
}


CButterFly::CButterFly(const CButterFly& rhs) : CItem(rhs), m_vMovingDir(rhs.m_vMovingDir), m_fChangeTime(rhs.m_fChangeTime)
{
}

CButterFly::~CButterFly()
{
}

HRESULT CButterFly::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());
	m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_Kabuto", 0.f);
	m_pAnimator->Play_Animation(L"Base", false);
	

	return S_OK;
}

_int CButterFly::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);

	_vec3 playerPos, myPos;

	Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::PLAYER)->Find_GameObject(L"Player")->Get_TransformCom()->Get_Info(INFO_POS, &playerPos);
	m_pTransformCom->Get_Info(INFO_POS, &myPos);

	playerPos.y = 0;
	myPos.y = 0;

	if (D3DXVec3Length(&(playerPos - myPos)) < 5.f)
	{
		_vec3 dir;
		m_vMovingDir = *D3DXVec3Normalize(&dir, &((playerPos - myPos) * -1.f));

		_vec3 scale;
		m_pTransformCom->Get_Info(INFO_UP, &scale);

		if (m_vMovingDir.x >= 0.f && scale.y < 0)
			m_pTransformCom->Set_Scale({ 1, -1, 1 });
		else if (m_vMovingDir.x < 0.f && scale.y >= 0)
			m_pTransformCom->Set_Scale({ 1, -1, 1 });

	}

	if (m_fChangeTime >= 0)
	{
		m_fChangeTime -= fTimeDelta;
	}
	else
	{
		srand(unsigned(time(NULL)));

		_float tmp = (_float)(rand() % 91 - 45);

		_matrix mat;

		D3DXMatrixRotationY(&mat, D3DXToRadian(tmp));

		D3DXVec3TransformNormal(&m_vMovingDir, &m_vMovingDir, &mat);

		mat = *m_pTransformCom->Get_WorldMatrix();

		if (m_vMovingDir.x >= 0.f && mat._22 < 0)
			m_pTransformCom->Set_Scale({ 1, -1, 1 });
		else if (m_vMovingDir.x < 0.f && mat._22 >= 0)
			m_pTransformCom->Set_Scale({ 1, -1, 1 });

		m_fChangeTime = 1.f;
	}

	_vec3 pos;
	m_pTransformCom->Get_Info(INFO_POS, &pos);
	m_pTransformCom->Set_Pos(&(pos + (m_vMovingDir * fTimeDelta)));



	return __super::Update_Object(fTimeDelta);
}

void CButterFly::LateUpdate_Object(void)
{
	return __super::LateUpdate_Object();
}

void CButterFly::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

}

void CButterFly::Add_Subscribe(_uint iNum)
{
	Engine::Add_Subscribe(iNum, this);
}

void CButterFly::Free()
{
	__super::Free();
}

CButterFly* CButterFly::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos)
{
	CButterFly* ret = new CButterFly(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Butterfly Create Failed");
		return nullptr;
	}

	ret->Add_Subscribe(p_EventNum);
	ret->m_pTransformCom->RotationAxis({ 0, 0, 1 }, D3DXToRadian(-89.5f));
	ret->m_pTransformCom->Set_Pos(&p_Pos);
	

	return ret;
}

void CButterFly::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING)
	{
		//æ∆¿Ã≈€ »πµÊ«œ¥¬∞≈

	}
}

void CButterFly::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CButterFly::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CButterFly::Event_Start(_uint iEventNum)
{
}

void CButterFly::Event_End(_uint iEventNum)
{
}

HRESULT CButterFly::Ready_Component()
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
