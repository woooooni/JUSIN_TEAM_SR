#include "Item_Hat_Turtle.h"
#include "Animator.h"


CItem_Hat_Turtle::CItem_Hat_Turtle(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
	: CItem_Hat(pGraphicDev, _pPlayer)
{
}

CItem_Hat_Turtle::CItem_Hat_Turtle(const CItem_Hat& rhs)
	: CItem_Hat(rhs)
{
}

CItem_Hat_Turtle::~CItem_Hat_Turtle()
{
}

HRESULT CItem_Hat_Turtle::Ready_Object(void)
{
	__super::Ready_Object();


	m_pAnimator->Add_Animation(L"Turtle_Up", L"Proto_Texture_Item_TurtleHat_Up", 0.1f);
	m_pAnimator->Add_Animation(L"Turtle_Down", L"Proto_Texture_Item_TurtleHat_Down", 0.1f);
	m_pAnimator->Add_Animation(L"Turtle_Left", L"Proto_Texture_Item_TurtleHat_Left", 0.1f);
	m_pAnimator->Add_Animation(L"Turtle_Right", L"Proto_Texture_Item_TurtleHat_Right", 0.1f);
	m_pAnimator->Add_Animation(L"Turtle_LeftUp", L"Proto_Texture_Item_TurtleHat_LeftUp", 0.1f);
	m_pAnimator->Add_Animation(L"Turtle_LeftDown", L"Proto_Texture_Item_TurtleHat_LeftDown", 0.1f);
	m_pAnimator->Add_Animation(L"Turtle_RightUp", L"Proto_Texture_Item_TurtleHat_RightUp", 0.1f);
	m_pAnimator->Add_Animation(L"Turtle_RightDown", L"Proto_Texture_Item_TurtleHat_RightDown", 0.1f);


	m_vecAnimation[(_uint)OBJ_DIR::DIR_D] =  L"Turtle_Down";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_U] =  L"Turtle_Up";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_L] =  L"Turtle_Left";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_R] =  L"Turtle_Right";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LU] = L"Turtle_LeftUp";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_LD] = L"Turtle_LeftDown";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RU] = L"Turtle_RightUp";
	m_vecAnimation[(_uint)OBJ_DIR::DIR_RD] = L"Turtle_RightDown";

	m_vScale = { 0.8f, 0.6f, 0.6f };
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

	m_eSkill = PLAYER_SKILL::TURTLE;

	return S_OK;
}

_int CItem_Hat_Turtle::Update_Object(const _float& fTimeDelta)
{
	_int Result = __super::Update_Object(fTimeDelta);
	return Result;
}

void CItem_Hat_Turtle::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CItem_Hat_Turtle::Render_Object(void)
{
	__super::Render_Object();
}

CItem_Hat_Turtle* CItem_Hat_Turtle::Create(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
{
	CItem_Hat_Turtle* pInstance = new CItem_Hat_Turtle(pGraphicDev, _pPlayer);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Hat_Turtle Create Failed");
		return nullptr;
	}

	return pInstance;
}

HRESULT CItem_Hat_Turtle::Add_Component(void)
{
	return E_NOTIMPL;
}

void CItem_Hat_Turtle::Free()
{
	__super::Free();
}
