#include "LightPuzzleTerrain.h"
#include	"Export_Function.h"
#include	"RcPuzzleBuff.h"
#include	"LightPuzzleBase.h"
#include	"LightPuzzlePiece.h"

CLightPuzzleTerrain::CLightPuzzleTerrain(LPDIRECT3DDEVICE9 p_Dev) : CGameObject(p_Dev, OBJ_TYPE::OBJ_INTERACTION), m_vTileCenterPos(nullptr), m_pSubBuffer(nullptr), tileX(0), tileY(0), m_bIsCanUpdate(true)
{
}

CLightPuzzleTerrain::CLightPuzzleTerrain(const CLightPuzzleTerrain& rhs) : CGameObject(rhs), m_vTileCenterPos(nullptr), m_pSubBuffer(rhs.m_pSubBuffer), tileX(rhs.tileX), tileY(rhs.tileY), m_bIsCanUpdate(rhs.m_bIsCanUpdate)
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
	if (m_bIsCanUpdate)
	{
		Add_RenderGroup(RENDER_ALPHA, this);
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_TRIGGER);

		for (size_t i = 0; i < m_vecLightMap.size(); i++)
		{
			if (m_vecLightMap[i]->m_pOnTileObj)
			{
				_vec3 src, tmp;
				m_vecLightMap[i]->m_pOnTileObj->Get_TransformCom()->Get_Info(INFO_POS, &src);
				m_pTransformCom->Get_Info(INFO_POS, &tmp);
				src.y = 0;
				if (D3DXVec3Length(&(src - (tmp + m_vTileCenterPos[i]))) > 0.35f)
				{
					m_vecLightMap[i]->m_bIsOnTile = false;
					m_vecLightMap[i]->m_bIsLighting = false;
					m_vecLightMap[i]->m_pOnTileObj->Set_Lighting(false);
					m_vecLightMap[i]->m_pOnTileObj = nullptr;
					Make_LightRoute();
				}
			}
		}

	}




	return __super::Update_Object(fTimeDelta);
}

void CLightPuzzleTerrain::LateUpdate_Object(void)
{
	for (auto& iter : m_vecLightMap)
	{
		if (iter->m_bIsOnTile && !iter->m_pOnTileObj)
		{
			iter->m_bIsOnTile = false;
		}
	}

	Make_LightRoute();

	m_bIsCanUpdate = true;
}

void CLightPuzzleTerrain::Render_Object(void)
{
	_matrix mat = *m_pTransformCom->Get_WorldMatrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);

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
			ret->m_vecLightMap.back()->iIndexX = j;
			ret->m_vecLightMap.back()->iIndexY = i;

		}
	}
	
	ret->tileX = tileX;
	ret->tileY = tileY;
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
	CLightPuzzle* src;



	if (src = dynamic_cast<CLightPuzzle*>(pCollider->GetOwner()))
	{
		CLightPuzzleBase* puzzleBase = dynamic_cast<CLightPuzzleBase*>(src);

		_uint iIndex;
		_vec3 tmp, myPos;

		src->Get_TransformCom()->Get_Info(INFO_POS, &tmp);
		m_pTransformCom->Get_Info(INFO_POS, &myPos);

		for (size_t i = 0; i < m_vecLightMap.size(); i++)
		{
			if (i == 0)
			{
				iIndex = i;
				continue;
			}


			if (D3DXVec3Length(&(myPos + m_vTileCenterPos[iIndex] - tmp)) > D3DXVec3Length(&(myPos + m_vTileCenterPos[i] - tmp)))
				iIndex = i;
		}

		_vec3 dst = myPos + m_vTileCenterPos[iIndex] - tmp;
		dst.y = 0;

		if (D3DXVec3Length(&dst) > 0.02f/*수치조정 필요*/ && D3DXVec3Length(&dst) < 0.35f)
		{
			src->Get_TransformCom()->Move_Pos(D3DXVec3Normalize(&dst, &dst), 0.3f, Get_TimeDelta(L"Timer_FPS60"));
		}
		else if (D3DXVec3Length(&dst) < 0.1f)
		{
			m_vecLightMap[iIndex]->m_bIsOnTile = true;

			m_vecLightMap[iIndex]->m_pOnTileObj = src;
			m_vecLightMap[iIndex]->m_listLightDir.clear();
			for (auto& iter : src->Get_LightDir())
			{
				m_vecLightMap[iIndex]->m_listLightDir.push_back(iter);
				
			}


			for (size_t i = 0; i < m_vecLightMap.size(); i++)
			{
				if (m_vecLightMap[i]->m_pOnTileObj == src && i != iIndex)
					m_vecLightMap[i]->m_pOnTileObj = nullptr;
			}
			

		}

	}
}

void CLightPuzzleTerrain::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	for (auto& iter :m_vecLightMap)
	{
		if (iter->m_pOnTileObj == pCollider->GetOwner())
			iter->m_pOnTileObj = nullptr;
	}
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

const _vec3& CLightPuzzleTerrain::Get_TilePos(_uint indexX, _uint indexY)
{
	if (indexX >= tileX || indexY >= tileY)
		return { 0, 0, 0 };
	_vec3 src;
	m_pTransformCom->Get_Info(INFO_POS, &src);
	return src + m_vTileCenterPos[indexY * tileX + indexX];
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
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_MoonPuzzleBase"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pSubBuffer = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);


	return S_OK;
}

HRESULT CLightPuzzleTerrain::Make_LightRoute()
{
	list<LIGHT_INFO*>	roundingList;

	for (auto& iter : m_vecLightMap)
	{
		iter->m_bIsLighting = false;
		if (iter->m_pOnTileObj)
			iter->m_pOnTileObj->Set_Lighting(false);
	}

	for (auto& iter : m_vecLightMap)
	{
		if (iter->m_pOnTileObj && dynamic_cast<CLightPuzzleBase*>(iter->m_pOnTileObj) && dynamic_cast<CLightPuzzleBase*>(iter->m_pOnTileObj)->Get_MakeLight())
		{
			iter->m_pOnTileObj->Set_Lighting(true);
			iter->m_bIsLighting = true;
			roundingList.push_back(iter);
			break;
		}
	}

	if (roundingList.empty())
		return E_FAIL;

	while (!roundingList.empty())
	{
		LIGHT_INFO* src = roundingList.front();
		roundingList.pop_front();

		for (auto& iter : src->m_listLightDir)
		{
			if (iter == _vec2(1, 0) && src->iIndexX < tileX)
			{
				LIGHT_INFO* tmp = m_vecLightMap[tileX * src->iIndexY + src->iIndexX + 1];

				if (tmp->m_bIsLighting || !tmp->m_pOnTileObj)
					continue;

				for (auto& iter2 : tmp->m_listLightDir)
				{
					if (iter2 == _vec2(-1, 0))
					{
						tmp->m_pOnTileObj->Set_Lighting(true);
						tmp->m_bIsLighting = true;
						roundingList.push_back(tmp);
						break;
					}
				}
			}
			else if (iter == _vec2(-1, 0) && src->iIndexX > 0)
			{
				LIGHT_INFO* tmp = m_vecLightMap[tileX * src->iIndexY + src->iIndexX - 1];

				if (tmp->m_bIsLighting || !tmp->m_pOnTileObj)
					continue;

				for (auto& iter2 : tmp->m_listLightDir)
				{
					if (iter2 == _vec2(1, 0))
					{
						tmp->m_pOnTileObj->Set_Lighting(true);
						tmp->m_bIsLighting = true;
						roundingList.push_back(tmp);
						break;
					}
				}

			}
			else if (iter == _vec2(0, 1) && src->iIndexY < tileY)
			{
				LIGHT_INFO* tmp = m_vecLightMap[tileX * (src->iIndexY + 1) + src->iIndexX];

				if (tmp->m_bIsLighting || !tmp->m_pOnTileObj)
					continue;

				for (auto& iter2 : tmp->m_listLightDir)
				{
					if (iter2 == _vec2(0, -1))
					{
						tmp->m_pOnTileObj->Set_Lighting(true);
						tmp->m_bIsLighting = true;
						roundingList.push_back(tmp);
						break;
					}
				}

			}
			else if (iter == _vec2(0, -1) && src->iIndexY > 0)
			{
				LIGHT_INFO* tmp = m_vecLightMap[tileX * (src->iIndexY - 1) + src->iIndexX];

				if (tmp->m_bIsLighting || !tmp->m_pOnTileObj)
					continue;

				for (auto& iter2 : tmp->m_listLightDir)
				{
					if (iter2 == _vec2(0, 1))
					{
						tmp->m_pOnTileObj->Set_Lighting(true);
						tmp->m_bIsLighting = true;
						roundingList.push_back(tmp);
						break;
					}
				}

			}

		}



	}

	return S_OK;
}
