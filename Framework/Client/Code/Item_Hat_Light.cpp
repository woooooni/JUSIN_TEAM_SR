#include "Item_Hat_Light.h"
#include "Animator.h"
#include "Export_Function.h"
#include "KeyMgr.h"

CItem_Hat_Light::CItem_Hat_Light(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
	: CItem_Hat(pGraphicDev, _pPlayer)
{
	m_eCode = ITEM_CODE::HAT_LIGHT;

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

	Add_Component();

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

	m_vScale = { 0.6f, 0.5f, 0.5f };
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


	D3DLIGHT9 tHatLight;
	ZeroMemory(&tHatLight, sizeof(D3DLIGHT9));

	tHatLight.Type = D3DLIGHTTYPE::D3DLIGHT_POINT;
	tHatLight.Ambient = { 0.0f, 0.0f, 0.0f, 0.0f };
	tHatLight.Diffuse = { 0.3f, 0.3f, 0.3f, 0.3f };
	tHatLight.Specular = { 0.5f, 0.5f, 0.5f, 0.5f};
	tHatLight.Range = 50.0f;
	tHatLight.Attenuation0 = 0.01f; // 鼻熱 馬潸 啗熱
	tHatLight.Attenuation1 = 0.06f; // 摹⑽ 馬潸 啗熱
	tHatLight.Attenuation2 = 0.03f; // 檜離 馬潸 啗熱


	Ready_Light(m_pGraphicDev, &tHatLight, (_uint)LIGHT_TYPE::LIGHT_SPOTLIGHT_HAT);
	
	CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_SPOTLIGHT_HAT)->Set_LightOff();



	return S_OK;
}

_int CItem_Hat_Light::Update_Object(const _float& fTimeDelta)
{
	_int Result = __super::Update_Object(fTimeDelta);

	if (KEY_TAP(KEY::Q))
	{
		if (CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_SPOTLIGHT_HAT)->Is_LightOn())
			CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_SPOTLIGHT_HAT)->Set_LightOff();
		else
			CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_SPOTLIGHT_HAT)->Set_LightOn();
	}


	return Result;
}

void CItem_Hat_Light::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();

	if (CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_SPOTLIGHT_HAT)->Is_LightOn())
	{
		D3DLIGHT9& tHatLight = CLightMgr::GetInstance()->Get_Light(LIGHT_TYPE::LIGHT_SPOTLIGHT_HAT)->Get_LightInfo();

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);

		if (m_pPlayer->GetObj_Dir() == OBJ_DIR::DIR_U)
		{
			vPos.z += 0.2f;
		}
		else if (m_pPlayer->GetObj_Dir() == OBJ_DIR::DIR_D)
		{
			vPos.z -= 0.2f;
		}
		else if (m_pPlayer->GetObj_Dir() == OBJ_DIR::DIR_L)
		{
			vPos.x -= 0.2f;
		}
		else if (m_pPlayer->GetObj_Dir() == OBJ_DIR::DIR_R)
		{
			vPos.x += 0.2f;
		}
		else if (m_pPlayer->GetObj_Dir() == OBJ_DIR::DIR_LU)
		{
			vPos.x -= 0.2f;
			vPos.z += 0.2f;
		}
		else if (m_pPlayer->GetObj_Dir() == OBJ_DIR::DIR_RU)
		{
			vPos.x += 0.2f;
			vPos.z += 0.2f;
		}
		else if (m_pPlayer->GetObj_Dir() == OBJ_DIR::DIR_LD)
		{
			vPos.x -= 0.2f;
			vPos.z -= 0.2f;
		}
		else if (m_pPlayer->GetObj_Dir() == OBJ_DIR::DIR_RD)
		{
			vPos.x += 0.2f;
			vPos.z -= 0.2f;
		}
		tHatLight.Position = vPos;
	}
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
	CComponent* pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Item_LightHat_Down"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

void CItem_Hat_Light::Free()
{
	__super::Free();
}
