#include "Text.h"
#include "Export_Function.h"

CTextBox::CTextBox(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev), m_tInfo{}
{
}

CTextBox::CTextBox(const CTextBox& rhs)
	: CUI(rhs), m_tInfo(rhs.m_tInfo)
{
}

CTextBox::~CTextBox()
{
}

HRESULT CTextBox::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	/*
	1. NPC���� �ޱ�
	2. NPC�� �´� ��ȭ���� Ǫ��
	3. Text��� (���� �������̰� �� ��)
	4. ��ȭ ������ Text, TextBox ��� ����, ���� ������ �� ����.

	+

	5. Ÿ���� �ϵ� Text ���
	*/
	

	return S_OK;
}

_int CTextBox::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	m_fAccTime += fTimeDelta * m_fTextSpeed;
	
	if (KEY_TAP(KEY::Z))
		Next_Text();

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CTextBox::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CTextBox::Render_Object(void)
{
	//if (m_bShown)
	//{
		/*
		switch (m_tInfo.eType)
		{
		case TEXTTYPE::SHEEP:
			break;

		case TEXTTYPE::PIG:
			break;

		case TEXTTYPE::COW:
			break;

		case TEXTTYPE::DOOGEE:
			break;

		default:
			break;

		}
		*/
		// Name Tag
		RECT rcName = { -1 * (WINCX / 4) + 70, (WINCY / 4) - 50, 3 * (WINCX / 4), WINCY };

		
		TCHAR szNameBuf[128] = L"�� ���ܸ�";
		//CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
		//	szNameBuf, lstrlen(szNameBuf), &rcName, DT_CENTER | DT_VCENTER | DT_NOCLIP,
		//	D3DCOLOR_ARGB(100, 0, 0, 0));

		//TCHAR szNameBuf[128] = L"�� ������";
		
		CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
			szNameBuf, lstrlen(szNameBuf), &rcName, DT_CENTER | DT_VCENTER | DT_NOCLIP,
			D3DCOLOR_ARGB(100, 0, 0, 0));

		// ��ȭâ
		RECT rc = { 0, WINCY / 2, WINCX, WINCY };
			//GetClientRect(g_hWnd, &rc);
		//TCHAR szBuf[256] = L"";
		//swprintf_s(szBuf, L"� ������ �� ������, �ƹ��� ���󰡸� ū�� ����~\n�� �ٲ� �׽�Ʈ");
		
		//CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
		//	szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		/*
		RECT rc = { 0, WINCY / 2, WINCX, WINCY };
		TCHAR szBuf[256] = L"";
		TCHAR szTemp[256] = L"";
		CHAR* szScript = "� ������ �� ������,";
		int iLength = strlen(szScript);

		for (int i = 0; i < iLength; i++)
		{
			CHAR temp = szScript[i];
			//szTemp = szScript[i]
			//swprintf_s(szBuf, szScript[i]);
			fflush(stdout);
		}

		CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
			szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
		*/

		// ����ü�� �� ����
		if ((_uint)m_fAccTime < m_vecText[m_iIndex].strDesc.size())
		{
			m_strCurrDesc = L"";
			for (_uint i = 0; i < (_uint)m_fAccTime; ++i)
				m_strCurrDesc += m_vecText[m_iIndex].strDesc[i];
		}

		CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
			m_strCurrDesc.c_str(), m_strCurrDesc.size(), &rc,
			DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	//}
}

void CTextBox::Set_Type(TEXTTYPE eType)
{
	//m_tInfo.eType = eType;
}

void CTextBox::Next_Text()
{
	m_fAccTime = 0.f;
	m_strCurrDesc = L"";

	if (m_vecText.size() - 1 == m_iIndex)
	{

	}
	else
		++m_iIndex;
}

HRESULT CTextBox::Add_Component(void)
{
	switch (m_tInfo.eType)
	{
	case TEXTTYPE::COW:
		m_vecText.push_back({ L"���õ� ������ �����ϰ� ����\n�̷� ���� � ������ �ϴµ�..." });
		break;

	case TEXTTYPE::SHEEP:
		m_vecText.push_back({ L"� ������ �� ������, �ƹ��� ���󰡸� ū�� ����~" });
		m_vecText.push_back({ L"������ ��� ����~" });
		break;

	case TEXTTYPE::PIG:
		m_vecText.push_back({ L"����̶�� �ڰ�� ������ �ٰ�\n������ �԰� ������ ��ƾ���." });
		break;

	case TEXTTYPE::DOOGEE:
		m_vecText.push_back({ L"�ȳ�, �Ʊ� ������, ��� �� ?" });
		m_vecText.push_back({ L"���õ� ������ ������ �Ŵ� ?" });
		m_vecText.push_back({ L"�������ϰ� ���� �� ������ ���� �θ����� ��!" });
		break;

	default:
		break;
	}

	return S_OK;
}

CTextBox* CTextBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTTYPE eType)
{
	CTextBox* pInstance = new CTextBox(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC Text Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CTextBox::Free()
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