#include "..\..\Header\Scene.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev, SCENE_TYPE _eType)
	: m_pGraphicDev(pGraphicDev)
	, m_eType(_eType)
	, m_hVideoHandle(nullptr)
{
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CLayer * CScene::Get_Layer(LAYER_TYPE _eType)
{
	auto		iter = m_mapLayer.find(_eType);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second;
}

HRESULT CScene::Ready_Scene()
{
	return S_OK;
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	_int		iResult = 0;

	for (auto& iter : m_mapLayer)
	{
		iResult = iter.second->Update_Layer(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}
	
	return iResult;
}

void CScene::LateUpdate_Scene()
{
	for (auto& iter : m_mapLayer)
		iter.second->LateUpdate_Layer();
}
void CScene::Free()
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDeleteMap());
	m_mapLayer.clear();

	Safe_Release(m_pGraphicDev);
}

void CScene::PlayVideo(HWND _hWnd, const wstring& _strFilePath)
{
	m_hVideoHandle = MCIWndCreate(_hWnd,
		NULL,
		WS_CHILD |
		WS_VISIBLE |
		MCIWNDF_NOPLAYBAR, _strFilePath.c_str());

	MoveWindow(m_hVideoHandle, 0, 0, WINCX, WINCY, FALSE);

	MCIWndPlay(m_hVideoHandle);
	while (MCIWndGetLength(m_hVideoHandle) > MCIWndGetPosition(m_hVideoHandle))
	{
		if (GetAsyncKeyState(VK_RETURN))
		{
			MCIWndClose(m_hVideoHandle);
			break;
		}
	}

	MCIWndClose(m_hVideoHandle);
}
