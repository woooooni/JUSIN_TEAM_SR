#include "UI_Ending.h"
#include "Export_Function.h"

CUI_Ending::CUI_Ending(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_Ending::CUI_Ending(const CUI_Ending& rhs) : CUI(rhs)
{
}

CUI_Ending::~CUI_Ending()
{
}

HRESULT CUI_Ending::Ready_Object(void)
{
	for (size_t i = 0; i < 6; i++)
	{
		m_vecPosition.push_back((WINCY + ((float)i + 0.5f) * 150.f));
	}


	m_vecNameTags.resize(6, L"");
	auto iter = m_vecNameTags.begin();
	*iter = L"팀장, 프레임워크\n\n김태원";
	++iter;

	*iter = L"플레이어, 이펙트\n\n황인범";
	++iter;

	*iter = L"기믹\n\n임동규";
	++iter;

	*iter = L"보스, 몬스터\n\n이종석";
	++iter;

	*iter = L"UI\n\n좌원혜";
	++iter;

	*iter = L"\n\n\n감사합니다.";

	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 50.f;

	return S_OK;
}

_int CUI_Ending::Update_Object(const _float& fTimeDelta)
{
	for (auto& iter : m_vecPosition)
	{
		iter -= fTimeDelta * 100.f;
	}

	if (m_vecPosition.back() < -100.f)
		Set_Active(false);
	else
		Add_RenderGroup(RENDER_UI, this);


	return 0;
}

void CUI_Ending::LateUpdate_Object(void)
{
}

void CUI_Ending::Render_Object(void)
{
	RECT rc;

	for (size_t i = 0; i < m_vecPosition.size(); i++)
	{
		rc = { (LONG)(0.5f * WINCX - m_tInfo.fCX), (LONG)(m_vecPosition[i] - m_tInfo.fCY),(LONG)(0.5f * WINCX + m_tInfo.fCX), (LONG)(m_vecPosition[i] + m_tInfo.fCY) };
	    auto iter = Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD)->DrawText(NULL, m_vecNameTags[i].c_str(), INT(m_vecNameTags[i].size()), &rc, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
		int k = 0;
	}
}

CUI_Ending* CUI_Ending::Create(LPDIRECT3DDEVICE9 p_GraphicDev)
{
	CUI_Ending* pInst = new CUI_Ending(p_GraphicDev);

	NULL_CHECK_RETURN(pInst, nullptr);
	if (FAILED(pInst->Ready_Object()))
	{
		Safe_Release(pInst);
		MSG_BOX("CUI_Ending Failed");
		return nullptr;
	}

	return pInst;
}
