#include "LightPuzzleTerrain.h"
#include	"Export_Function.h"
#include	"RcPuzzleBuff.h"

CLightPuzzleTerrain::CLightPuzzleTerrain(LPDIRECT3DDEVICE9 p_Dev) : CGameObject(p_Dev, OBJ_TYPE::OBJ_BACKGROUND), m_vTileCenterPos(nullptr), m_pSubBuffer(nullptr)
{
}

CLightPuzzleTerrain::CLightPuzzleTerrain(const CLightPuzzleTerrain& rhs) : CGameObject(rhs), m_vTileCenterPos(nullptr), m_pSubBuffer(rhs.m_pSubBuffer)
{
}

CLightPuzzleTerrain::~CLightPuzzleTerrain()
{
}

HRESULT CLightPuzzleTerrain::Ready_Object(void)
{
	FAILED_CHECK(Ready_Compnent());

	m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_MoonPuzzleBase", 0.1f);
	m_pAnimator->Play_Animation(L"Base", false);

	return S_OK;
}

_int CLightPuzzleTerrain::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);

	return __super::Update_Object(fTimeDelta);
}

void CLightPuzzleTerrain::LateUpdate_Object(void)
{
}

void CLightPuzzleTerrain::Render_Object(void)
{
	_matrix mat = *m_pTransformCom->Get_WorldMatrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pAnimator->Render_Component();
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	

	/*for (size_t i = 0; i < m_vecLightMap.size(); i++)
	{
		_matrix src = mat;
		src.m[3][0] += m_vTileCenterPos[i].x;
		src.m[3][1] += m_vTileCenterPos[i].y;
		src.m[3][2] += m_vTileCenterPos[i].z;

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &src);
		m_pTextureCom->Render_Texture();
		m_pSubBuffer->Render_Buffer();

	}*/

}

void CLightPuzzleTerrain::Free()
{
	Safe_Delete_Array(m_vTileCenterPos);
	__super::Free();
}

CLightPuzzleTerrain* CLightPuzzleTerrain::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& tileX, const _uint& tileY, const _vec3 p_Pos)
{
	CLightPuzzleTerrain* ret = new CLightPuzzleTerrain(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("PuzzleTerrain Create Failed");
		return nullptr;
	}

	ret->m_vTileCenterPos = new _vec3[(tileX * tileY)];

	for (size_t i = 0; i < tileY; i++)
	{
		for (size_t j = 0; j < tileX; j++)
		{
			ret->m_vTileCenterPos[tileX * i + j] = _vec3(j + 0.5f, 0, i + 0.5f);
			ret->m_vecLightMap.push_back(new LIGHT_INFO);
		}
	}
	

	ret->m_pColliderCom->Set_Offset({(_float)tileX * 0.5f , 0, (_float)tileY * 0.5f});

	dynamic_cast<CBoxCollider*>(ret->m_pColliderCom)->Set_Scale({ (float)tileX, 1.f, (float)tileY });
	ret->m_pTransformCom->Set_Pos(& _vec3( p_Pos.x, 0.001f, p_Pos.z ));

	return ret;
}

void CLightPuzzleTerrain::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CLightPuzzleTerrain::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CLightPuzzleTerrain::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CLightPuzzleTerrain::Event_Start(_uint iEventNum)
{
}

void CLightPuzzleTerrain::Event_End(_uint iEventNum)
{
}

void CLightPuzzleTerrain::Set_SubscribeEvent(_uint pEvent)
{
}

HRESULT CLightPuzzleTerrain::Ready_Compnent()
{

	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcPuzzleBuff*>(Engine::Clone_Proto(L"Proto_LightPuzzle1"));
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
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_MoonPuzzleBase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pSubBuffer = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);


	return S_OK;
}
