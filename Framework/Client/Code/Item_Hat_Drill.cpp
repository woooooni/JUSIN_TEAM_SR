#include "Item_Hat_Drill.h"
#include "Animator.h"
#include "Export_Function.h"

CItem_Hat_Drill::CItem_Hat_Drill(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
	: CItem_Hat(pGraphicDev, _pPlayer)
{
}

CItem_Hat_Drill::CItem_Hat_Drill(const CItem_Hat& rhs)
	: CItem_Hat(rhs)
{
}

CItem_Hat_Drill::~CItem_Hat_Drill()
{
}

HRESULT CItem_Hat_Drill::Ready_Object(void)
{
	__super::Ready_Object();

	Add_Component();

	m_pAnimator->Add_Animation(L"Drill", L"Proto_Texture_Item_DrillHat", 0.1f);


	m_vecAnimation[(_uint)OBJ_DIR::DIR_D] =  L"Drill";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_U] =  L"Drill";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_L] =  L"Drill";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_R] =  L"Drill";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LU] = L"Drill";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LD] = L"Drill";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RU] = L"Drill";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RD] = L"Drill";

	m_vScale = { 0.6f, 0.6f, 0.0f };
	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(&_vec3(0.0f, 0.0f, 0.0f));

	m_vecOffset[(_uint)OBJ_DIR::DIR_D] = { 0.0f, 0.2f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_U] = { 0.0f, 0.2f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_L] = { -0.05f, 0.2f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_R] = { 0.05f, 0.2f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_LD] = { -0.07f, 0.2f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_LU] = { -0.05f, 0.2f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_RD] = { 0.07f, 0.2f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_RU] = { 0.05f, 0.2f, 0.0f };

	m_bLoop = true;

	m_eSkill = PLAYER_SKILL::DRILL;

	return S_OK;
}

_int CItem_Hat_Drill::Update_Object(const _float& fTimeDelta)
{
	_int Result = __super::Update_Object(fTimeDelta);
	return Result;
}

void CItem_Hat_Drill::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CItem_Hat_Drill::Render_Object(void)
{
	__super::Render_Object();
}

CItem_Hat_Drill* CItem_Hat_Drill::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
{
	CItem_Hat_Drill* pInstance = new CItem_Hat_Drill(pGraphicDev, _pPlayer);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Hat_Drill Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CItem_Hat_Drill::Add_Component(void)
{
	CComponent* pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Item_DrillHat"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

void CItem_Hat_Drill::Free()
{
	__super::Free();
}
