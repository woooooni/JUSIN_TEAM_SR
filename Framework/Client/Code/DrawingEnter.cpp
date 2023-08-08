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

CDrawingEnter::CDrawingEnter(LPDIRECT3DDEVICE9 pGraphicDev, CItem* pItem)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_PORTAL, OBJ_ID::PORTAL)
	, m_pItem(pItem)
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

	if (D3DXVec3Length(&vDir) < 5.0f && !m_bFinish)
	{
		if (m_bStart)
		{
			m_iAlpha = 0;
			if (pPlayer->Get_State() == PLAYER_STATE::DRAWING && pPlayer->Is_DrawFinish())
			{
				m_iAlpha = 255;
				m_bFinish = true;
				pPlayer->Change_State(PLAYER_STATE::IDLE);

				/*CGameObject* pItem = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, ITEM_CODE::DRAWING_COLORS);
				NULL_CHECK_RETURN(pItem, E_FAIL);*/
				CInventoryMgr::GetInstance()->Add_Item(m_pItem->Clone());
				CEventMgr::GetInstance()->DeleteObjEvt(this);
			}
		}

		if (!m_bStart && D3DXVec3Length(&vDir) <= 1.0f)
		{
			CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(true);
			if (KEY_TAP(KEY::Z))
			{
				m_bStart = true;
				pPlayer->Change_State(PLAYER_STATE::DRAWING);
				CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
			}
		}
		else if (D3DXVec3Length(&vDir) > 1.0f)
			CUIMgr::GetInstance()->Get_ShortcutKey()->Set_Active(false);
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
	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, m_iAlpha / 255.0f };

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(0);
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);



	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

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


	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);

	return S_OK;
}

CDrawingEnter* CDrawingEnter::Create(LPDIRECT3DDEVICE9 pGraphicDev, CItem* pItem)
{
	CDrawingEnter* pInstance = new CDrawingEnter(pGraphicDev, pItem);
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
