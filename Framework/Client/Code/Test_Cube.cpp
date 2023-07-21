#include "..\Header\Test_Cube.h"
#include "Export_Function.h"
#include "Transform.h"

CTest_Cube::CTest_Cube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev, OBJ_TYPE::OBJ_END, OBJ_ID::BLOCK_OBJ)
{
}

CTest_Cube::CTest_Cube(const CTest_Cube& rhs)
	: CGameObject(rhs)
{
}

CTest_Cube::~CTest_Cube()
{
}

HRESULT CTest_Cube::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	
	return S_OK;
}

_int CTest_Cube::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	m_pTransformCom->Set_Scale(_vec3(.5f, .2f, 1.f));
	return __super::Update_Object(fTimeDelta);
}

void CTest_Cube::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CTest_Cube::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();

	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}

HRESULT CTest_Cube::Ready_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"Proto_RcCube"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Cube_Captapult"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	return  S_OK;
}

CTest_Cube* CTest_Cube::Create(LPDIRECT3DDEVICE9 _pGraphicDev)
{

	CTest_Cube* pInstance = new CTest_Cube(_pGraphicDev);
	NULL_CHECK_RETURN(pInstance, nullptr);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Test_Cube Create Failed");
		return nullptr;
	}
	return pInstance;
}



void CTest_Cube::Free()
{
	__super::Free();
}
