#include "..\Header\Scene_TutorialVillage.h"
#include "Export_Function.h"

CScene_TutorialVillage::CScene_TutorialVillage(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev, SCENE_TYPE::TUTORIAL_VILLAGE)
{
}

CScene_TutorialVillage::~CScene_TutorialVillage()
{
}

HRESULT CScene_TutorialVillage::Ready_Scene()
{
	return S_OK;
}

_int CScene_TutorialVillage::Update_Scene(const _float& fTimeDelta)
{
	return S_OK;
}

void CScene_TutorialVillage::LateUpdate_Scene()
{
}

void CScene_TutorialVillage::Render_Scene()
{
}

HRESULT CScene_TutorialVillage::Ready_Prototype()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Player()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Camera()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Terrrain()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Environment()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Monster()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_InterationObj()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_Effect()
{
	return S_OK;
}

HRESULT CScene_TutorialVillage::Ready_Layer_UI()
{
	return S_OK;
}

CScene_TutorialVillage* CScene_TutorialVillage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CScene_TutorialVillage* pInstance = new CScene_TutorialVillage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);

		MSG_BOX("TutorialVillage Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CScene_TutorialVillage::Free()
{
	__super::Free();
}
