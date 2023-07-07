#include "LightPuzzleTerrain.h"
#include	"Export_Function.h"
#include	"RcPuzzleBuff.h"

CLightPuzzleTerrain::CLightPuzzleTerrain(LPDIRECT3DDEVICE9 p_Dev) : CGameObject(p_Dev, OBJ_TYPE::OBJ_BACKGROUND), m_vTileCenterPos(nullptr)
{
}

CLightPuzzleTerrain::CLightPuzzleTerrain(const CLightPuzzleTerrain& rhs) : CGameObject(rhs), m_vTileCenterPos(nullptr)
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
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();

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
			ret->m_vTileCenterPos[tileY * i + j] = _vec3(j + 0.5f, 0, i + 0.5f);
			ret->m_vecLightMap.push_back(new LIGHT_INFO);
		}
	}
	
	_vec2 src;

	if (tileX % 2 == 0)
	{
		src.x = (tileX / 2);
	}
	else
	{
		src.x = (tileX / 2);
		src.x += 0.5f;
	}

	if (tileY % 2 == 0)
	{
		src.y = (tileY / 2);
	}
	else
	{
		src.y = (tileY / 2);
		src.y += 0.5f;
	}

	ret->m_pColliderCom->Set_Offset({ src.x , 0, src.y });
	dynamic_cast<CBoxCollider*>(ret->m_pColliderCom)->Set_Scale({ (float)tileX * 0.5f, 1.f, (float)tileY * 0.5f });
	ret->m_pTransformCom->Set_Pos(& _vec3( p_Pos.x, 0.001f, p_Pos.z ));

	return ret;
}

void CLightPuzzleTerrain::Collision_Enter(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

void CLightPuzzleTerrain::Collision_Stay(CGameObject* pCollisionObj, UINT _iColliderID)
{
}

void CLightPuzzleTerrain::Collision_Exit(CGameObject* pCollisionObj, UINT _iColliderID)
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


	return S_OK;
}
