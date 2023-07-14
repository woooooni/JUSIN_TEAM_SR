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

	// �� ������
	m_vecText.push_back({ TEXTTYPE::COW, L"" });
	m_vecText.push_back({ TEXTTYPE::COW, L"���õ� ������ �����ϰ� ����\n�̷� ���� � ������ �ϴµ�... " });

	// �� ���ܸ�
	m_vecText.push_back({ TEXTTYPE::SHEEP, L"" });
	m_vecText.push_back({ TEXTTYPE::SHEEP, L"� ������ �� ������, �ƹ��� ���󰡸� ū�� ����~ " });

	// ���� ����
	m_vecText.push_back({ TEXTTYPE::PIG, L"" });
	m_vecText.push_back({ TEXTTYPE::PIG, L"����̶�� �ڰ�� ������ �ٰ�\n������ �԰� ������ ��ƾ���. " });

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
		// Name Tag ��ġ
		RECT rcName = { -1 * (WINCX / 4) + 70, (WINCY / 4) - 50, 3 * (WINCX / 4), WINCY };
		// Text ��� ��ġ
		RECT rc = { 0, WINCY / 2, WINCX, WINCY };

		switch (m_tInfo.eType)
		{
		case TEXTTYPE::SHEEP:
			//m_vecText.push_back({ TEXTTYPE::SHEEP, L"� ������ �� ������, �ƹ��� ���󰡸� ū�� ����~ " });
			CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
				L"�� ���ܸ�", lstrlen(L"�� ���ܸ�"), &rcName, DT_CENTER | DT_VCENTER | DT_NOCLIP,
				D3DCOLOR_ARGB(100, 0, 0, 0));
			break;

		case TEXTTYPE::COW:
			//m_vecText.push_back({ TEXTTYPE::COW, L"���õ� ������ �����ϰ� ����\n�̷� ���� � ������ �ϴµ�... " });
			CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
				L"�� ������", lstrlen(L"�� ������"), &rcName, DT_CENTER | DT_VCENTER | DT_NOCLIP,
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

		// ����ü�� �� ����
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
//		m_vecText.push_back({ L"���õ� ������ �����ϰ� ����\n�̷� ���� � ������ �ϴµ�..." });
//		break;
//
//	case TEXTTYPE::SHEEP:
//		m_vecText.push_back({ L"� ������ �� ������, �ƹ��� ���󰡸� ū�� ����~" });
//		m_vecText.push_back({ L"������ ��� ����~" });
//		break;
//
//	case TEXTTYPE::PIG:
//		m_vecText.push_back({ L"����̶�� �ڰ�� ������ �ٰ�\n������ �԰� ������ ��ƾ���." });
//		break;
//
//	case TEXTTYPE::DOOGEE:
//		m_vecText.push_back({ L"�ȳ�, �Ʊ� ������, ��� �� ?" });
//		m_vecText.push_back({ L"���õ� ������ ������ �Ŵ� ?" });
//		m_vecText.push_back({ L"�������ϰ� ���� �� ������ ���� �θ����� ��!" });
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
	// �ϳ��� ���ڿ� ���� ������ �� �����.

	int iCount = 0;
	CString strTemp;

	// ���� �κ� ���ڿ��� CString ���� ��ü�� ���� ����
	// ������ ���ڿ��� ��ü �ؽ�Ʈ�� �����ϴ� ���ڿ�
	// �����Һκ� ���ڿ��� �ε���
	// �κ� ���ڿ��� �����ϴ� �� ���Ǵ� ���� ��ȣ ����
	while (AfxExtractSubString(strTemp, lpszTemp, iCount++, szSep))
	{
		strTemp.TrimLeft();
		strTemp.TrimRight();
		strArr.Add(strTemp);
	}

	//CStringArray strArray;
	//m_util.Split_String(card_confirm_protocol, "*", strArray);

	return iCount; // ���ҵ� ����
}

_int CTextBox::Splits_String(CString strTemp, CString strSep, CStringArray& strArr)
{
	// ���ڿ��� ������ �� �����.

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
{ L'��', L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��' };

static const wchar_t wcJung[] =
{ L'��', L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��', L'��', L'��' };

static const wchar_t wcJungMix[][3] =
{ {L'��', L'��', L'��'}, {L'��', L'��', L'��'},
	{L'��', L'��', L'��'}, {L'��', L'��', L'��'},
	{L'��', L'��', L'��'}, {L'��', L'��', L'��'},
	{L'��', L'��', L'��'}, };

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
{ L' ', L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��', L'��',
L'��', L'��', L'��', L'��', L'��',
L'��', L'��', L'��' };

static const wchar_t wcjongMix[][3] =
{ {L'��', L'��', L'��'}, {L'��', L'��', L'��'},
	{L'��', L'��', L'��'}, {L'��', L'��', L'��'},
	{L'��', L'��', L'��'}, {L'��', L'��', L'��'},
	{L'��', L'��', L'��'}, {L'��', L'��', L'��'},
	{L'��', L'��', L'��'}, {L'��', L'��', L'��'},
	{L'��', L'��', L'��'}, };

// Index
{
	for (int i = 0; i < sizeof(wcCho) / sizeof(wchar_t); i++)
	{
		if (wcCho[i] == ch)
			return i;
	}  

	return -1;
}

//�ѱ� ����
//Ÿ���� �ϵ��� ��ü

*/