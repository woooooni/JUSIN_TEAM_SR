#include "..\..\Header\GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "VIBuffer.h"
#include "Collider.h"
#include "Animator.h"
#include "Texture.h"
#include "Export_Function.h"
#include "RigidBody.h"


CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_TYPE _eType, OBJ_ID _eID)
	: m_pGraphicDev(pGraphicDev)
	, m_eType(_eType)
	, m_eDir(OBJ_DIR::DIR_D)
	, m_bActive(true)
	, m_fViewZ(0.f)
	, m_pBufferCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_pColliderCom(nullptr)
	, m_pAnimator(nullptr)
	, m_pTextureCom(nullptr)
	, m_pRigidBodyCom(nullptr)
	, m_fMinHeight(0.006f)
	, m_eID(_eID)
	, m_iAlpha(255)
{
	m_pGraphicDev->AddRef();

	m_tMaterial.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_tMaterial.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_tMaterial.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_tMaterial.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_tMaterial.Power = 0.0f;
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_eType(rhs.m_eType)
	, m_eDir(rhs.m_eDir)
	, m_bActive(true)
	, m_fViewZ(rhs.m_fViewZ)
	, m_pBufferCom(rhs.m_pBufferCom)
	, m_pTransformCom(rhs.m_pTransformCom)
	, m_pColliderCom(rhs.m_pColliderCom)
	, m_pAnimator(rhs.m_pAnimator)
	, m_pTextureCom(rhs.m_pTextureCom)
	, m_pRigidBodyCom(rhs.m_pRigidBodyCom)
	, m_fMinHeight(rhs.m_fMinHeight)
	, m_eID(rhs.m_eID)
	, m_iAlpha(255)
{

	m_pGraphicDev->AddRef();


	if (nullptr != m_pBufferCom)
	{
		m_pBufferCom = dynamic_cast<CVIBuffer*>(m_pBufferCom->Clone());

		NULL_CHECK(m_pBufferCom);

		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, m_pBufferCom);
		
	}
		
		
	if (nullptr != m_pTransformCom)
	{
		m_pTransformCom = dynamic_cast<CTransform*>(m_pTransformCom->Clone());
		NULL_CHECK(m_pTransformCom);

		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, m_pTransformCom);
	}
		

	if (nullptr != m_pTextureCom)
	{
		m_pTextureCom = dynamic_cast<CTexture*>(m_pTextureCom->Clone());
		NULL_CHECK(m_pTextureCom);

		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, m_pTextureCom);
	}
		


	if (nullptr != m_pColliderCom)
	{
		m_pColliderCom = dynamic_cast<CCollider*>(m_pColliderCom->Clone());
		NULL_CHECK(m_pColliderCom);

		m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, m_pColliderCom);
	}
		

	if (nullptr != m_pAnimator)
	{
		m_pAnimator = dynamic_cast<CAnimator*>(m_pAnimator->Clone());
		NULL_CHECK(m_pAnimator);

		m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, m_pAnimator);
	}
		

	if (nullptr != m_pRigidBodyCom)
	{
		m_pRigidBodyCom = dynamic_cast<CRigidBody*>(m_pRigidBodyCom->Clone());
		NULL_CHECK(m_pRigidBodyCom);

		m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, m_pRigidBodyCom);
	}
		
	m_tMaterial = rhs.m_tMaterial;
}

CGameObject::~CGameObject()
{

}


HRESULT CGameObject::Ready_Object(void)
{
	return S_OK;
}

_int CGameObject::Update_Object(const _float & fTimeDelta)
{
	if (!m_bActive)
		return S_OK;

	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Update_Component(fTimeDelta);
	
	if (m_pTransformCom)
	{
		CCamera* pMainCamera = dynamic_cast<CCamera*>(Engine::Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
		if (pMainCamera)
		{
			_vec3 vCamPos, vPos, vScale;
			m_pTransformCom->Get_Info(INFO_POS, &vPos);
			vScale = m_pTransformCom->Get_Scale();

			if (m_eID == OBJ_ID::TILE)
			{
				vPos.z += vScale.y;
			}
			else
			{
				vPos.z += vScale.z * 0.001f;
			}
			
			pMainCamera->Get_TransformCom()->Get_Info(INFO_POS, &vCamPos);
			_vec3 vDir = vPos - vCamPos;
			float fLen = D3DXVec3Length(&vDir);
			m_fViewZ = vDir.z;
		}
	}

	return S_OK;
}

void CGameObject::LateUpdate_Object(void)
{
	if (!m_bActive)
		return;

	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->LateUpdate_Component();
}

void CGameObject::Render_Object(void)
{
	if (!m_bActive)
		return;

	for (auto& iter : m_mapComponent[ID_DYNAMIC])
		iter.second->Render_Component();
}

CComponent* CGameObject::Get_Component(COMPONENT_TYPE eType, COMPONENTID eID)
{
	CComponent* pComponent = Find_Component(eType, eID);

	NULL_CHECK_RETURN(pComponent, nullptr);

	return pComponent;
}

void CGameObject::Set_Billboard()
{
	if (m_pTransformCom == nullptr)
		return;

	_vec3 fScale = m_pTransformCom->Get_Scale();

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	// 카메라의 월드행렬.
	D3DXMatrixInverse(&matView, nullptr, &matView);

	_vec3 vCameraPos = _vec3(matView._41, matView._42, matView._43);
	_vec3 vObjPos, vObjUp = { 0.f, 1.f, 0.f };
	m_pTransformCom->Get_Info(INFO_UP, &vObjUp);
	m_pTransformCom->Get_Info(INFO_POS, &vObjPos);

	_vec3 vDir = vObjPos - vCameraPos;
	_vec3 vRight, vLook;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Cross(&vRight, &vObjUp, &vDir);
	D3DXVec3Cross(&vLook, &vRight, &vObjUp);

	m_pTransformCom->Set_Info(INFO_RIGHT, &(vRight));
	m_pTransformCom->Set_Info(INFO_LOOK, &(vLook));
	m_pTransformCom->Set_Info(INFO_UP, &(vObjUp));
}

CComponent * CGameObject::Find_Component(COMPONENT_TYPE eType, COMPONENTID eID)
{
	auto& iter = m_mapComponent[eID].find(eType);

	if (iter != m_mapComponent[eID].end())
		return iter->second;

	return nullptr;
}

void CGameObject::Free()
{
	for (size_t i = 0; i < ID_END; ++i)
	{
		for_each(m_mapComponent[i].begin(), m_mapComponent[i].end(), CDeleteMap());
		m_mapComponent[i].clear();
	}

	m_pBufferCom = nullptr;
	m_pTransformCom = nullptr;
	m_pColliderCom = nullptr;
	m_pAnimator = nullptr;
	m_pTextureCom = nullptr;
	m_pRigidBodyCom = nullptr;
}
