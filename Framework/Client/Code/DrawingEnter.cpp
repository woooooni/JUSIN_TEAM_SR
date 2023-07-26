#include "DrawingEnter.h"
#include "Export_Function.h"
#include "Loading.h"
#include "Scene_Loading.h"
#include "Nexus.h"
#include "GameMgr.h"
#include "BossDoor.h"
#include "GameMgr.h"
#include "UIMgr.h"
#include "DefaultItem.h"
#include "InventoryMgr.h"

CDrawingEnter::CDrawingEnter(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PORTAL, OBJ_ID::PORTAL)
{

}

CDrawingEnter::CDrawingEnter(const CDrawingEnter& rhs)
	: CGameObject(rhs)
{
}

CDrawingEnter::~CDrawingEnter()
{
}

HRESULT CDrawingEnter::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.5f, 0.5f, 0.5f));

	m_iAlpha = 255;

	return S_OK;
}

_int CDrawingEnter::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
 
	__super::Update_Object(fTimeDelta);

	CPlayer* pPlayer = CGameMgr::GetInstance()->Get_Player();
	NULL_CHECK_RETURN(pPlayer, E_FAIL);

	_vec3 vPlayerPos, vPos, vDir;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	pPlayer->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);

	vDir = vPlayerPos - vPos;
	if (D3DXVec3Length(&vDir) < 1.0f && !m_bFinish)
	{
		if (m_bStart)
		{
			m_iAlpha = 0;
			if (pPlayer->Get_State() == PLAYER_STATE::DRAWING && pPlayer->Is_DrawFinish())
			{
				m_iAlpha = 255;
				m_bFinish = true;
				pPlayer->Change_State(PLAYER_STATE::IDLE);

				CGameObject* pItem = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::DRAWING_COLORS);
				NULL_CHECK_RETURN(pItem, E_FAIL);
				CInventoryMgr::GetInstance()->Add_Item(pItem);
			}
		}

		if (!m_bStart)
		{
			CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(true);
			if (KEY_TAP(KEY::Z))
			{
				m_bStart = true;
				pPlayer->Change_State(PLAYER_STATE::DRAWING);
			}
		}
			
		
	}

	if (m_bFinish)
	{
		m_iAlpha -= 5;
		if (m_iAlpha <= 0)
			Set_Active(false);
	}

	return S_OK;
}

void CDrawingEnter::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CDrawingEnter::Render_Object(void)
{
	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, 255, 255, 255));

	__super::Render_Object();
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CDrawingEnter::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	
}

void CDrawingEnter::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CDrawingEnter::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

HRESULT CDrawingEnter::Ready_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_DrawingEnter"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

CDrawingEnter* CDrawingEnter::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDrawingEnter* pInstance = new CDrawingEnter(pGraphicDev);
	NULL_CHECK_RETURN(pInstance, nullptr);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);
		MSG_BOX("BossDoorEnter Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CDrawingEnter::Free()
{
	__super::Free();
}
