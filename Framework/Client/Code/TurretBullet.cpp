#include "TurretBullet.h"
#include	"Export_Function.h"
#include	"Pool.h"

CTurretBullet::CTurretBullet(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID) 
	: CBullet(pGraphicDev, _eID)
	, m_vDirection({1, 0, 0})
{
}

CTurretBullet::CTurretBullet(const CTurretBullet& rhs) 
	: CBullet(rhs)
	, m_vDirection(rhs.m_vDirection)
{
}

CTurretBullet::~CTurretBullet()
{
}

HRESULT CTurretBullet::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_Turret_Bullet", 0.f);
	m_pAnimator->Play_Animation(L"Base", false);
	dynamic_cast<CBoxCollider*>(m_pColliderCom)->Set_Scale({ 0.5f, 0.5f, 0.5f });

	Set_MinHeight(0.5f);

    return S_OK;
}

_int CTurretBullet::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	_vec3	pos;

	m_pTransformCom->Get_Info(INFO_POS, &pos);

	pos += m_vDirection * fTimeDelta * 10.f;

	m_pTransformCom->Set_Pos(&pos);

    return __super::Update_Object(fTimeDelta);
}

void CTurretBullet::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CTurretBullet::Render_Object(void)
{
	_matrix mat = *m_pTransformCom->Get_WorldMatrix();
	_vec3 pos, right;
	m_pTransformCom->Get_Info(INFO_POS, &pos);
	m_pTransformCom->Get_Info(INFO_RIGHT, &right);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	ZeroMemory(&mat.m[3][0], sizeof(_vec3));

	_matrix rot;
	D3DXMatrixRotationAxis(&rot, &right, D3DXToRadian(90.f));

	mat *= rot;

	memcpy(&mat.m[3][0], &pos, sizeof(_vec3));

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();


}

void CTurretBullet::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (pCollider->GetOwner()->GetObj_Id() != OBJ_ID::PLAYER)
	{
		m_pTransformCom->Set_Info(INFO_RIGHT, &_vec3(1.f, 0.f, 0.f));
		m_pTransformCom->Set_Info(INFO_UP, &_vec3(0.f, 0.f, 1.f));
		m_pTransformCom->Set_Info(INFO_LOOK, &_vec3(0.f, -1.f, 0.f));


		CPool<CTurretBullet>::Return_Obj(this);
	}
}

void CTurretBullet::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CTurretBullet::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

CTurretBullet* CTurretBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3& pPos, const _vec3& pDir)
{

	CTurretBullet* ret = new CTurretBullet(pGraphicDev, OBJ_ID::BUG_BALL);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("TurBul Create Failed");
		return nullptr;
	}


	ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(-90.f));
	ret->Set_Start(pPos, pDir);
	ret->Set_Atk(1);

	return ret;
}

void CTurretBullet::Set_Start(const _vec3& pPos, const _vec3& pDir)
{
	_vec3 src = { 1, 0, 0 };

	m_pTransformCom->Set_Pos(&pPos);
	m_vDirection = pDir;
	m_vDirection.y = 0.f;
	_vec3 tmp;
	
	if(D3DXVec3Cross(&tmp, &src, &m_vDirection)->y < 0)
		m_pTransformCom->RotationAxis({ 0, 1, 0 }, -acosf(D3DXVec3Dot(&src, D3DXVec3Normalize(&m_vDirection, &m_vDirection))));
	else
	{
		m_pTransformCom->RotationAxis({ 0, 1, 0 }, acosf(D3DXVec3Dot(&src, D3DXVec3Normalize(&m_vDirection, &m_vDirection))));
	}
}


void CTurretBullet::Free()
{
	__super::Free();
}

HRESULT CTurretBullet::Ready_Component()
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
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	return S_OK;
}
