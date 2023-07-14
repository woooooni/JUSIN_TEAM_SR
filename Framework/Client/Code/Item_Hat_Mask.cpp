#include "Item_Hat_Mask.h"
#include "Animator.h"


CItem_Hat_Mask::CItem_Hat_Mask(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
	: CItem_Hat(pGraphicDev, _pPlayer)
{
}

CItem_Hat_Mask::CItem_Hat_Mask(const CItem_Hat& rhs)
	: CItem_Hat(rhs)
{
}

CItem_Hat_Mask::~CItem_Hat_Mask()
{
}

HRESULT CItem_Hat_Mask::Ready_Object(void)
{
	__super::Ready_Object();


	m_pAnimator->Add_Animation(L"Mask_Up", L"Proto_Texture_Item_MaskHat_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Mask_Down", L"Proto_Texture_Item_MaskHat_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Mask_Left", L"Proto_Texture_Item_MaskHat_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Mask_Right", L"Proto_Texture_Item_MaskHat_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Mask_LeftUp", L"Proto_Texture_Item_MaskHat_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Mask_LeftDown", L"Proto_Texture_Item_MaskHat_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Mask_RightUp", L"Proto_Texture_Item_MaskHat_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Mask_RightDown", L"Proto_Texture_Item_MaskHat_RightDown", 0.1f);


	m_vecAnimation[(_uint)OBJ_DIR::DIR_D] =  L"Mask_Down";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_U] =  L"Mask_Up";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_L] =  L"Mask_Left";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_R] =  L"Mask_Right";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LU] = L"Mask_LeftUp";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LD] = L"Mask_LeftDown";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RU] = L"Mask_RightUp";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RD] = L"Mask_RightDown";

	m_vScale = { 0.8f, 0.6f, 0.6f };
	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(&_vec3(0.0f, 0.0f, 0.0f));

	m_vecOffset[(_uint)OBJ_DIR::DIR_D] = { 0.0f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_U] = { 0.0f, 0.05f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_L] = { -0.1f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_R] = { 0.1f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_LD] = { -0.065f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_LU] = { -0.05f, 0.05f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_RD] = { 0.065f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_RU] = { 0.05f, 0.05f, 0.0f };

	m_bLoop = true;

	m_eSkill = PLAYER_SKILL::LIGHTNING;

	return S_OK;
}

_int CItem_Hat_Mask::Update_Object(const _float& fTimeDelta)
{
	_int Result = __super::Update_Object(fTimeDelta);
	return Result;
}

void CItem_Hat_Mask::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CItem_Hat_Mask::Render_Object(void)
{
	__super::Render_Object();
}

CItem_Hat_Mask* CItem_Hat_Mask::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
{
	CItem_Hat_Mask* pInstance = new CItem_Hat_Mask(pGraphicDev, _pPlayer);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Hat_Mask Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CItem_Hat_Mask::Add_Component(void)
{
	return E_NOTIMPL;
}

void CItem_Hat_Mask::Free()
{
	__super::Free();
}
