#include "Item_Hat_Missile.h"
#include "Export_Function.h"


CItem_Hat_Missile::CItem_Hat_Missile(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
	: CItem_Hat(pGraphicDev, _pPlayer)
{
}

CItem_Hat_Missile::CItem_Hat_Missile(const CItem_Hat& rhs)
	: CItem_Hat(rhs)
{
}

CItem_Hat_Missile::~CItem_Hat_Missile()
	
{
}

HRESULT CItem_Hat_Missile::Ready_Object(void)
{
	__super::Ready_Object();


	m_pAnimator->Add_Animation(L"Missile_Up", L"Proto_Texture_Item_MissileHat_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Missile_Down", L"Proto_Texture_Item_MissileHat_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Missile_Left", L"Proto_Texture_Item_MissileHat_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Missile_Right", L"Proto_Texture_Item_MissileHat_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Missile_LeftUp", L"Proto_Texture_Item_MissileHat_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Missile_LeftDown", L"Proto_Texture_Item_MissileHat_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Missile_RightUp", L"Proto_Texture_Item_MissileHat_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Missile_RightDown", L"Proto_Texture_Item_MissileHat_RightDown", 0.1f);


	m_vecAnimation[(_uint)OBJ_DIR::DIR_D] =  L"Missile_Down";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_U] = L"Missile_Up";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_L] = L"Missile_Left";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_R] = L"Missile_Right";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LU] = L"Missile_LeftUp";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LD] = L"Missile_LeftDown";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RU] = L"Missile_RightUp";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RD] = L"Missile_RightDown";

	m_vScale = { 0.8f, 0.6f, 0.6f };
	m_pTransformCom->Set_Scale(m_vScale);
	m_pTransformCom->Set_Pos(&_vec3(0.0f, 0.0f, 0.0f));

	m_vecOffset[(_uint)OBJ_DIR::DIR_D] = { 0.0f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_U] = { 0.0f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_L] = { -0.05f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_R] = { 0.05f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_LD] = { -0.065f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_LU] = { -0.05f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_RD] = { 0.065f, 0.00f, 0.0f };
	m_vecOffset[(_uint)OBJ_DIR::DIR_RU] = { 0.05f, 0.00f, 0.0f };

	m_bLoop = true;

	m_eSkill = PLAYER_SKILL::MISSILE;

	return S_OK;
}

_int CItem_Hat_Missile::Update_Object(const _float& fTimeDelta)
{
	_int Result = __super::Update_Object(fTimeDelta);
	return Result;
}

void CItem_Hat_Missile::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CItem_Hat_Missile::Render_Object(void)
{
	__super::Render_Object();
}

CItem_Hat_Missile* CItem_Hat_Missile::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
{
	CItem_Hat_Missile* pInstance = new CItem_Hat_Missile(pGraphicDev, _pPlayer);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Hat_Missile_Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CItem_Hat_Missile::Add_Component(void)
{
	return E_NOTIMPL;
}

void CItem_Hat_Missile::Free()
{
	__super::Free();
}
