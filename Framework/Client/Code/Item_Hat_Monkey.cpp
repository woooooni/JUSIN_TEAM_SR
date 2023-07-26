#include "Item_Hat_Monkey.h"
#include "Export_Function.h"
#include "Texture.h"
#include "Export_Function.h"

CItem_Hat_Monkey::CItem_Hat_Monkey(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
	: CItem_Hat(pGraphicDev, _pPlayer)
{
	m_eCode = ITEM_CODE::HAT_MONKEY;

}

CItem_Hat_Monkey::CItem_Hat_Monkey(const CItem_Hat& rhs)
	:CItem_Hat(rhs)
{
}

CItem_Hat_Monkey::~CItem_Hat_Monkey()
{
}

HRESULT CItem_Hat_Monkey::Ready_Object(void)
{
	__super::Ready_Object();

	Add_Component();

	m_pAnimator->Add_Animation(L"Monkey_Up", L"Proto_Texture_Item_MonkeyHat_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Monkey_Down", L"Proto_Texture_Item_MonkeyHat_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Monkey_Left", L"Proto_Texture_Item_MonkeyHat_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Monkey_Right", L"Proto_Texture_Item_MonkeyHat_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Monkey_LeftUp", L"Proto_Texture_Item_MonkeyHat_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Monkey_LeftDown", L"Proto_Texture_Item_MonkeyHat_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Monkey_RightUp", L"Proto_Texture_Item_MonkeyHat_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Monkey_RightDown", L"Proto_Texture_Item_MonkeyHat_RightDown", 0.1f);


	m_vecAnimation[(_uint)OBJ_DIR::DIR_D] =  L"Monkey_Down";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_U] =  L"Monkey_Up";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_L] =  L"Monkey_Left";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_R] =  L"Monkey_Right";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LU] = L"Monkey_LeftUp";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LD] = L"Monkey_LeftDown";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RU] = L"Monkey_RightUp";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RD] = L"Monkey_RightDown";

	m_vScale = { 0.8f, 0.6f, 0.6f };
	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(&_vec3(0.0f, 0.0f, 0.0f));

	m_vecOffset[(_uint)OBJ_DIR::DIR_D] = {0.0f, 0.05f, 0.0f};
	m_vecOffset[(_uint)OBJ_DIR::DIR_U] = { 0.0f, 0.05f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_L] = { -0.05f, 0.05f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_R] = { 0.05f, 0.05f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_LD] = { -0.065f, 0.05f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_LU] = { -0.05f, 0.05f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_RD] = { 0.065f, 0.05f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_RU] = { 0.05f, 0.05f, 0.0f };

	m_bLoop = true;

	m_eSkill = PLAYER_SKILL::GOLEMFIST;

	return S_OK;
}

_int CItem_Hat_Monkey::Update_Object(const _float& fTimeDelta)
{
	_int Result = __super::Update_Object(fTimeDelta);
	return Result;
}

void CItem_Hat_Monkey::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CItem_Hat_Monkey::Render_Object(void)
{
	__super::Render_Object();
}

CItem_Hat_Monkey* CItem_Hat_Monkey::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
{
	CItem_Hat_Monkey* pInstance = new CItem_Hat_Monkey(pGraphicDev, _pPlayer);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Hat_Monkey Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CItem_Hat_Monkey::Add_Component(void)
{
	CComponent* pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Item_MonkeyHat_Down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

void CItem_Hat_Monkey::Free()
{
	__super::Free();
}
