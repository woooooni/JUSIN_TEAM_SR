#include "ButterFly.h"
#include	"Export_Function.h"
#include	<time.h>
#include	"InventoryMgr.h"
#include	"Pool.h"

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
	m_pTextureCom->Render_Texture();
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

HRESULT CButterFly::Change_Item(const ITEM_CODE& pCode)
{
	if (pCode >= ITEM_CODE::ITEM_END)
		return E_FAIL;


	m_eCode = pCode;

	m_pTextureCom->Set_Idx((_uint)m_eCode);

	return S_OK;

}


CButterFly* CButterFly::Create(LPDIRECT3DDEVICE9 p_Dev, ITEM_CODE p_Code, const _vec3 p_Pos)
{
	if (p_Code < ITEM_CODE::HP_SMALL || p_Code >= ITEM_CODE::ITEM_END)
		return nullptr;


	CButterFly* ret = new CButterFly(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Butterfly Create Failed");
		return nullptr;
	}

	ret->m_pTextureCom->Set_Idx(_uint(p_Code));
	ret->m_pTransformCom->RotationAxis({ 0, 0, 1 }, D3DXToRadian(-89.5f));
	ret->m_pTransformCom->Set_Pos(&p_Pos);
	

	return ret;
}

void CButterFly::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING)
	{
		CInventoryMgr::GetInstance()->Add_Item(this);

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

CGameObject* CButterFly::Get_ByPool()
{
	return CPool<CButterFly>::Get_Obj();
}

void CButterFly::Add_Pool()
{
	CPool<CButterFly>::Return_Obj(this);
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Items"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);




	return S_OK;

}
