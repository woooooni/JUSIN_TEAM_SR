#include "Text.h"
#include "Export_Function.h"

CNPCText::CNPCText(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev), m_tInfo{}
{
}

CNPCText::CNPCText(const CNPCText& rhs)
	: CUI(rhs), m_tInfo(rhs.m_tInfo)
{
}

CNPCText::~CNPCText()
{
}

HRESULT CNPCText::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	// 소 아저씨
	m_vecText.push_back({ TEXTTYPE::COW, L"" });
	m_vecText.push_back({ TEXTTYPE::COW, L"오늘도 날씨가 따스하고 좋네\n이런 날은 놀러 나가야 하는데... " });

	// 양 아줌마
	m_vecText.push_back({ TEXTTYPE::SHEEP, L"" });
	m_vecText.push_back({ TEXTTYPE::SHEEP, L"놀러 나가는 건 좋지만, 아무나 따라가면 큰일 난다~ " });

	// 돼지 삼촌
	m_vecText.push_back({ TEXTTYPE::PIG, L"" });
	m_vecText.push_back({ TEXTTYPE::PIG, L"어린아이라면 자고로 열심히 뛰고\n열심히 먹고 열심히 놀아야지. " });

	return S_OK;
}

_int CNPCText::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	m_fAccTime += fTimeDelta * m_fTextSpeed;
	
	if (KEY_TAP(KEY::Z))
		Next_Text();

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CNPCText::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNPCText::Render_Object(void)
{
	_uint iIndex = 0;

	if (m_bShown)
	{
		// Name Tag 위치
		RECT rcName = { -1 * (WINCX / 4) + 70, (WINCY / 4) - 50, 3 * (WINCX / 4), WINCY };
		// Text 출력 위치
		RECT rc = { 0, WINCY / 2, WINCX, WINCY };

		switch (m_tInfo.eType)
		{
		case TEXTTYPE::SHEEP:
			//m_vecText.push_back({ TEXTTYPE::SHEEP, L"놀러 나가는 건 좋지만, 아무나 따라가면 큰일 난다~ " });
			CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
				L"양 아줌마", lstrlen(L"양 아줌마"), &rcName, DT_CENTER | DT_VCENTER | DT_NOCLIP,
				D3DCOLOR_ARGB(100, 0, 0, 0));
			break;

		case TEXTTYPE::COW:
			//m_vecText.push_back({ TEXTTYPE::COW, L"오늘도 날씨가 따스하고 좋네\n이런 날은 놀러 나가야 하는데... " });
			CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
				L"소 아저씨", lstrlen(L"소 아저씨"), &rcName, DT_CENTER | DT_VCENTER | DT_NOCLIP,
				D3DCOLOR_ARGB(100, 0, 0, 0));
			break;

		default:
			break;
		}

		if ((_uint)m_fAccTime < m_vecText[m_iIndex].strDesc.size())
		{
			m_strCurrDesc = L"";
			for (_uint i = 0; i < (_uint)m_fAccTime; ++i)
				m_strCurrDesc += m_vecText[m_iIndex].strDesc[i];
		}

		CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
			m_strCurrDesc.c_str(), m_strCurrDesc.size(), &rc,
			DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));

//		tagTextInfo tagDst;
//		tagDst.eType = TEXTTYPE::COW;
//		vector<tagTextInfo>::iterator iter;
//		iter = find_if(m_vecText.begin(), m_vecText.end(), tagDst);
//
//		if (iter != m_vecText.end())
//		{
//			if ((_uint)m_fAccTime < m_vecText[m_iIndex].strDesc.size())
//			{
//				m_strCurrDesc = L"";
//				for (_uint i = 0; i < (_uint)m_fAccTime; ++i)
//					m_strCurrDesc += m_vecText[m_iIndex].strDesc[i];
//			}
//		}

		// 구조체가 들어간 벡터
//		if ((_uint)m_fAccTime < m_vecText[m_iIndex].strDesc.size())
//		{
//			m_strCurrDesc = L"";
//			for (_uint i = 0; i < (_uint)m_fAccTime; ++i)
//				m_strCurrDesc += m_vecText[m_iIndex].strDesc[i];
//		}
//
//		CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
//			m_strCurrDesc.c_str(), m_strCurrDesc.size(), &rc,
//			DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CNPCText::Set_Type(TEXTTYPE eType)
{
	m_tInfo.eType = eType;
}

void CNPCText::Next_Text()
{
	m_fAccTime = 0.f;
	m_strCurrDesc = L"";

	if (m_vecText.size() - 1 == m_iIndex)
	{
		//
	}
	else
		++m_iIndex;
}

HRESULT CNPCText::Add_Component(void)
{
//	switch (m_tInfo.eType)
//	{
//	case TEXTTYPE::COW:
//		m_vecText.push_back({ L"오늘도 날씨가 따스하고 좋네\n이런 날은 놀러 나가야 하는데..." });
//		break;
//
//	case TEXTTYPE::SHEEP:
//		m_vecText.push_back({ L"놀러 나가는 건 좋지만, 아무나 따라가면 큰일 난다~" });
//		m_vecText.push_back({ L"조심히 놀다 오렴~" });
//		break;
//
//	case TEXTTYPE::PIG:
//		m_vecText.push_back({ L"어린아이라면 자고로 열심히 뛰고\n열심히 먹고 열심히 놀아야지." });
//		break;
//
//	case TEXTTYPE::DOOGEE:
//		m_vecText.push_back({ L"안녕, 아기 오구야, 어디 가 ?" });
//		m_vecText.push_back({ L"오늘도 모험을 떠나는 거니 ?" });
//		m_vecText.push_back({ L"몸조심하고 무슨 일 있으면 나를 부르도록 해!" });
//		break;
//
//	default:
//		break;
//	}

	return S_OK;
}

CNPCText* CNPCText::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTTYPE eType)
{
	CNPCText* pInstance = new CNPCText(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC Text Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNPCText::Free()
{
}

/*
_int CTextBox::Split_String(LPCTSTR lpszTemp, TCHAR szSep, CStringArray& strArr)
{
	// 하나의 문자에 대해 분할할 때 사용함.

	int iCount = 0;
	CString strTemp;

	// 개별 부분 문자열을 CString 받을 개체에 대한 참조
	// 추출할 문자열의 전체 텍스트를 포함하는 문자열
	// 추출할부분 문자열의 인덱스
	// 부분 문자열을 구분하는 데 사용되는 구분 기호 문자
	while (AfxExtractSubString(strTemp, lpszTemp, iCount++, szSep))
	{
		strTemp.TrimLeft();
		strTemp.TrimRight();
		strArr.Add(strTemp);
	}

	//CStringArray strArray;
	//m_util.Split_String(card_confirm_protocol, "*", strArray);

	return iCount; // 분할된 개수
}

_int CTextBox::Splits_String(CString strTemp, CString strSep, CStringArray& strArr)
{
	// 문자열을 분할할 때 사용함.

	int iCount = 0;

	while (strTemp.GetLength() > 0)
	{
		int iPos = strTemp.Find(strSep);

		if (iPos != -1)
		{
			strArr.Add(strTemp.Left(iPos));
			strTemp = strTemp.Mid(iPos + strSep.GetLength());
			iCount++;

			if (strTemp.IsEmpty())
			{
				strArr.Add("");
				iCount++;
			}
		}

		else
		{
			strArr.Add(strTemp);
			strTemp = "";
			iCount++;
		}
	}

	//CStringArray strArray;
	//m_util.Splits_String(is_parm, "|||", strArray);

	return iCount;
}
*/

/*
static const wchar_t wcCho[] =
{ L'ㄱ', L'ㄲ', L'ㄴ', L'ㄷ', L'ㄸ',
L'ㄹ', L'ㅁ', L'ㅂ', L'ㅃ', L'ㅅ',
L'ㅆ', L'ㅇ', L'ㅈ', L'ㅉ', L'ㅊ',
L'ㅋ', L'ㅌ', L'ㅍ', L'ㅎ' };

static const wchar_t wcJung[] =
{ L'ㅏ', L'ㅐ', L'ㅑ', L'ㅒ', L'ㅓ',
L'ㅔ', L'ㅕ', L'ㅖ', L'ㅗ', L'ㅘ',
L'ㅙ', L'ㅚ', L'ㅛ', L'ㅜ', L'ㅝ',
L'ㅞ', L'ㅟ', L'ㅠ', L'ㅡ', L'ㅢ', L'ㅣ' };

static const wchar_t wcJungMix[][3] =
{ {L'ㅗ', L'ㅏ', L'ㅘ'}, {L'ㅗ', L'ㅐ', L'ㅙ'},
	{L'ㅗ', L'ㅑ', L'ㅚ'}, {L'ㅜ', L'ㅓ', L'ㅝ'},
	{L'ㅜ', L'ㅔ', L'ㅞ'}, {L'ㅜ', L'ㅣ', L'ㅟ'},
	{L'ㅡ', L'ㅣ', L'ㅢ'}, };

BOOL CTextBox::IsMidMix(wchar_t wc, __out wchar_t& pFirst, __out wchar_t& pSeconde)
{
	for (int i = 0; i < sizeof(wcMidMix) / sizeof(*wcMidMix); i++)
	{
		if (wcMidMix[i][2] == wc)
		{
			pFirst = wcMidMix[i][0]; pSeconde = wcMidMix[i][1];
			return TRUE;
		}
	}
	return FALSE;
}

static const wchar_t wcjong[] =
{ L' ', L'ㄱ', L'ㄲ', L'ㄳ', L'ㄴ',
L'ㄵ', L'ㄶ', L'ㄷ', L'ㄹ', L'ㄺ',
L'ㄻ', L'ㄼ', L'ㄽ', L'ㄾ', L'ㄿ',
L'ㅀ', L'ㅁ', L'ㅂ', L'ㅄ', L'ㅅ',
L'ㅆ', L'ㅇ', L'ㅈ', L'ㅊ', L'ㅋ',
L'ㅌ', L'ㅍ', L'ㅎ' };

static const wchar_t wcjongMix[][3] =
{ {L'ㄱ', L'ㅅ', L'ㄳ'}, {L'ㄴ', L'ㅈ', L'ㄵ'},
	{L'ㄴ', L'ㅎ', L'ㄶ'}, {L'ㄹ', L'ㄱ', L'ㄺ'},
	{L'ㄹ', L'ㅁ', L'ㄻ'}, {L'ㄹ', L'ㅂ', L'ㄼ'},
	{L'ㄹ', L'ㅅ', L'ㄽ'}, {L'ㄹ', L'ㅌ', L'ㄾ'},
	{L'ㄹ', L'ㅍ', L'ㄿ'}, {L'ㄹ', L'ㅎ', L'ㅀ'},
	{L'ㅂ', L'ㅅ', L'ㅄ'}, };

// Index
{
	for (int i = 0; i < sizeof(wcCho) / sizeof(wchar_t); i++)
	{
		if (wcCho[i] == ch)
			return i;
	}  

	return -1;
}

//한글 분해
//타이핑 하듯이 해체

*/