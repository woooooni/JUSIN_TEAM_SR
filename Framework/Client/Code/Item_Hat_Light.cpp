#include "Item_Hat_Light.h"
#include "Animator.h"

CItem_Hat_Light::CItem_Hat_Light(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
	: CItem_Hat(pGraphicDev, _pPlayer)
{
}

CItem_Hat_Light::CItem_Hat_Light(const CItem_Hat& rhs)
	: CItem_Hat(rhs)
{
}

CItem_Hat_Light::~CItem_Hat_Light()
{
}

HRESULT CItem_Hat_Light::Ready_Object(void)
{
	__super::Ready_Object();


	m_pAnimator->Add_Animation(L"Light_Up", L"Proto_Texture_Item_LightHat_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Light_Down", L"Proto_Texture_Item_LightHat_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Light_Left", L"Proto_Texture_Item_LightHat_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Light_Right", L"Proto_Texture_Item_LightHat_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Light_LeftUp", L"Proto_Texture_Item_LightHat_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Light_LeftDown", L"Proto_Texture_Item_LightHat_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Light_RightUp", L"Proto_Texture_Item_LightHat_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Light_RightDown", L"Proto_Texture_Item_LightHat_RightDown", 0.1f);


	m_vecAnimation[(_uint)OBJ_DIR::DIR_D] =  L"Light_Down";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_U] =  L"Light_Up";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_L] =  L"Light_Left";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_R] =  L"Light_Right";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LU] = L"Light_LeftUp";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LD] = L"Light_LeftDown";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RU] = L"Light_RightUp";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RD] = L"Light_RightDown";

	m_vScale = { 0.6f, 0.5f, 0.0f };
	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(&_vec3(0.0f, 0.0f, 0.0f));

	m_vecOffset[(_uint)OBJ_DIR::DIR_D] = { 0.0f, 0.1f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_U] = { 0.0f, 0.1f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_L] = { -0.05f, 0.1f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_R] = { 0.05f, 0.1f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_LD] = { -0.065f, 0.1f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_LU] = { -0.05f, 0.1f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_RD] = { 0.065f, 0.1f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_RU] = { 0.05f, 0.1f, 0.0f };

	m_bLoop = true;

	return S_OK;
}

_int CItem_Hat_Light::Update_Object(const _float& fTimeDelta)
{
	_int Result = __super::Update_Object(fTimeDelta);
	return Result;
}

void CItem_Hat_Light::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CItem_Hat_Light::Render_Object(void)
{
	__super::Render_Object();
}

CItem_Hat_Light* CItem_Hat_Light::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
{
	CItem_Hat_Light* pInstance = new CItem_Hat_Light(pGraphicDev, _pPlayer);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Hat_Light Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CItem_Hat_Light::Add_Component(void)
{
	return E_NOTIMPL;
}

void CItem_Hat_Light::Free()
{
	__super::Free();
}
