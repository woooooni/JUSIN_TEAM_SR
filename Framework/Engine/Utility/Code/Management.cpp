#include "Export_Utility.h"
#include "Engine_Struct.h"


IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement() 
	: m_pCurrScene(nullptr)
	, m_pReserveScene(nullptr)
{
}

CManagement::~CManagement()
{
	Free();
}

HRESULT CManagement::Set_Scene(CScene * pScene)
{
	if (m_pCurrScene != nullptr)
	{
		m_pCurrScene->Exit_Scene();
		Safe_Release(m_pCurrScene);
	}
	
	m_pCurrScene = pScene;
	m_pCurrScene->Enter_Scene();
	return S_OK;
}

_int CManagement::Update_Scene(const _float & fTimeDelta)
{
	if (m_pReserveScene)
	{
		Set_Scene(m_pReserveScene);
		m_pReserveScene = nullptr;
	}
		

	if (nullptr == m_pCurrScene)
		return -1;

	return m_pCurrScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Scene()
{
	if (nullptr == m_pCurrScene)
		return;

	m_pCurrScene->LateUpdate_Scene();
}

void CManagement::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Render_GameObject(pGraphicDev);

	if (m_pCurrScene)
		m_pCurrScene->Render_Scene();
}

void CManagement::Free()
{
	Safe_Release(m_pCurrScene);
}
