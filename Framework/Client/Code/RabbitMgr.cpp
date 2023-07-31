#include "RabbitMgr.h"
#include	"Export_Function.h"
#include "GameMgr.h"


IMPLEMENT_SINGLETON(CRabbitMgr)

void CRabbitMgr::Free()
{
	for (auto& iter : m_vecRabbits)
	{
		Safe_Release(iter);
	}
	m_vecRabbits.clear();
	m_vecTimer.clear();
}

HRESULT CRabbitMgr::Ready_Object(const LPDIRECT3DDEVICE9& pGraphicDev, const _int& pRabbitNumX, const _int& pRabbitNumY, const _vec3& pStartPos)
{
	m_iRabbitCountX = pRabbitNumX;
	m_iRabbitCountY = pRabbitNumY;
	m_vStartPos = pStartPos;
	m_pGraphicDev = pGraphicDev;
	m_fGameTime = 0.f;
	m_fCurTime = 0.f;
	m_iPoints = 0;
	Free();

	m_vecRabbits.reserve(_uint(pRabbitNumX * pRabbitNumY));

	FAILED_CHECK(Ready_Rabits());

	return S_OK;
}

_int CRabbitMgr::Update_Object(const _float& fTimeDelta)
{

	//if (KEY_TAP(KEY::Z))
	//{
	//	Start_Game();
	//}

	for (auto& iter : m_vecRabbits)
	{
		iter->Update_Object(fTimeDelta);
	}


	if (!m_bIsStart)
		return 0;

	if (m_bIsStart && !m_vecTimer.empty())
	{
		m_fCurTime += fTimeDelta;
		if (m_fCurTime >= m_vecTimer.front().m_fTime)
		{
			for (auto& iter : m_vecTimer.front().m_RabbitList)
			{
				m_vecRabbits[iter]->Set_Up();
			}

			m_vecTimer.erase(m_vecTimer.begin());
			m_fCurTime = 0.f;
		}
	}



	m_fGameTime += fTimeDelta;

	if (m_bIsStart && m_vecTimer.empty() && m_fGameTime >= 66.f && m_iPoints > 25)
	{
		m_bIsStart = false;
		m_bIsClear = true;
		m_fGameTime = 0.f;
		m_fCurTime = 0.f;
		m_iPoints = 0;
		Check_Event_Start(34);
		return 0;

	}
	else if (m_fGameTime > 66.f)
	{
		CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Set_Pos(&_vec3(m_vStartPos.x - 1.f, 0.f, m_vStartPos.z - 1.f));
		m_bIsStart = false;
		m_fGameTime = 0.f;
		m_fCurTime = 0.f;
		m_iPoints = 0;

	}


	return 0;
}

void CRabbitMgr::Render_Object()
{
	if (m_bIsStart)
	{
		RECT rc = { WINCX / 2 - 50 + 1 , 60 , WINCX / 2 + 50 + 1 , 160 };
		wstring szBuf;
		szBuf = to_wstring(66.f - m_fGameTime);
		szBuf = szBuf.substr(0, 5);

		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			szBuf.c_str(), INT(szBuf.size()), &rc, DT_LEFT | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));


		rc = { WINCX / 2 - 50 + 1 , 170 , WINCX / 2 + 50 + 1 , 270 };
		szBuf = to_wstring(m_iPoints);

		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawText(NULL,
			szBuf.c_str(), INT(szBuf.size()), &rc, DT_VCENTER | DT_NOCLIP | DT_SINGLELINE | DT_CENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

void CRabbitMgr::Start_Game()
{
	m_vecTimer.clear();

	RABBIT_TIMER timer;
	timer.m_fTime = 0.f;
	timer.m_RabbitList.push_back(10);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(8);

	Add_RabbitAppear(timer);

	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(8);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(8);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(13);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(3);
	timer.m_RabbitList.push_back(5);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(4);
	timer.m_RabbitList.push_back(10);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(4);
	timer.m_RabbitList.push_back(10);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(2);
	timer.m_RabbitList.push_back(8);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(4);
	timer.m_RabbitList.push_back(14);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(6);
	timer.m_RabbitList.push_back(10);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(4);
	timer.m_RabbitList.push_back(14);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 3.f;
	timer.m_RabbitList.push_back(3);
	timer.m_RabbitList.push_back(14);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(1);
	timer.m_RabbitList.push_back(13);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 2.5f;
	timer.m_RabbitList.push_back(6);
	timer.m_RabbitList.push_back(9);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(8);
	timer.m_RabbitList.push_back(10);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 2.5f;
	timer.m_RabbitList.push_back(1);
	timer.m_RabbitList.push_back(9);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();


	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(3);
	timer.m_RabbitList.push_back(5);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 2.5f;
	timer.m_RabbitList.push_back(14);
	timer.m_RabbitList.push_back(9);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(4);
	timer.m_RabbitList.push_back(13);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 2.5f;
	timer.m_RabbitList.push_back(5);
	timer.m_RabbitList.push_back(12);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(11);
	timer.m_RabbitList.push_back(14);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 2.5f;
	timer.m_RabbitList.push_back(0);
	timer.m_RabbitList.push_back(14);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(13);
	timer.m_RabbitList.push_back(15);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 2.5f;
	timer.m_RabbitList.push_back(7);
	timer.m_RabbitList.push_back(8);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(11);
	timer.m_RabbitList.push_back(0);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(10);
	timer.m_RabbitList.push_back(12);

	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(14);
	timer.m_RabbitList.push_back(2);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 1.f;
	timer.m_RabbitList.push_back(1);
	timer.m_RabbitList.push_back(6);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(0);
	timer.m_RabbitList.push_back(13);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(3);
	timer.m_RabbitList.push_back(15);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(7);
	timer.m_RabbitList.push_back(8);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 1.f;
	timer.m_RabbitList.push_back(2);
	timer.m_RabbitList.push_back(12);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(6);
	timer.m_RabbitList.push_back(11);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(4);
	timer.m_RabbitList.push_back(14);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(0);
	timer.m_RabbitList.push_back(9);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 1.f;
	timer.m_RabbitList.push_back(7);
	timer.m_RabbitList.push_back(9);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(15);
	timer.m_RabbitList.push_back(12);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(5);
	timer.m_RabbitList.push_back(10);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	timer.m_fTime = 0.5f;
	timer.m_RabbitList.push_back(11);
	timer.m_RabbitList.push_back(13);
	Add_RabbitAppear(timer);
	timer.m_RabbitList.clear();

	m_bIsStart = true;

}

HRESULT CRabbitMgr::Ready_Rabits()
{
	for (_uint i = 0; i < m_iRabbitCountY; i++)
	{
		for (_uint j = 0; j < m_iRabbitCountX; j++)
		{
			CRabitObj* rab = CRabitObj::Create(m_pGraphicDev, { m_vStartPos.x + (j * 2) , 0 , m_vStartPos.z + (i * 2) });
			NULL_CHECK_RETURN(rab, E_FAIL);
			m_vecRabbits.push_back(rab);
		}
	}


	return S_OK;
}
