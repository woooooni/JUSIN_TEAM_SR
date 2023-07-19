#include "Prop.h"
#include "Export_Function.h"

CProp::CProp(LPDIRECT3DDEVICE9 pGraphicDev)
	: CFixedObj(pGraphicDev, OBJ_ID::PROP)
{
	
}

CProp::CProp(const CProp& rhs)
	: CFixedObj(rhs)
{

}

CProp::~CProp()
{

}

HRESULT CProp::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	return S_OK;
}

_int CProp::Update_Object(const _float& fTimeDelta)
{
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CProp::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CProp::Render_Object(void)
{
	//Set_Billboard();

	__super::Render_Object();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	
}

void CProp::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	
	
}

void CProp::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
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

		/*if (vDir.x < vDir.z)
		{
			if (vDir.x < 0.f)
			{
				vPos = { vPos.x - pBoxCollider->Get_Scale().x / 2.f, vOtherPos.y, vOtherPos.z };
				pOtherTransform->Set_Pos(&vPos);
			}
			else if (vDir.x > 0.f)
			{
				vPos = { vPos.x + pBoxCollider->Get_Scale().x / 2.f, vPos.y, vPos.z };
				pOtherTransform->Set_Pos(&vPos);
			}
		}
		
		else
		{
			if (vDir.z < 0.f)
			{
				vPos = { vPos.x, vPos.y, vPos.z - pBoxCollider->Get_Scale().z / 2.f };
				pOtherTransform->Set_Pos(&vPos);
			}
			else if (vDir.z > 0.f)
			{
				vPos = { vPos.x, vPos.y, vPos.z + pBoxCollider->Get_Scale().z / 2.f };
				pOtherTransform->Set_Pos(&vPos);
			}
		}*/
		
		
	}

}

void CProp::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{

}

HRESULT CProp::Ready_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Prop"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);


	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	return S_OK;
}

CProp* CProp::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProp* pInstance = new CProp(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Prop Object Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CProp::Free()
{
}
