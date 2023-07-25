#include "RabbitMgr.h"
#include	"Export_Function.h"


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
	Free();

	m_vecRabbits.reserve(_uint(pRabbitNumX * pRabbitNumY));

	FAILED_CHECK(Ready_Rabits());

	return S_OK;
}

_int CRabbitMgr::Update_Object(const _float& fTimeDelta)
{
	if (KEY_TAP(KEY::Z))
	{
		RABBIT_TIMER timer;
		timer.m_fTime = 1.f;
		timer.m_RabbitList.push_back(1);
		Add_RabbitAppear(timer);
		timer.m_RabbitList.clear();

		timer.m_fTime = 1.f;
		timer.m_RabbitList.push_back(3);
		timer.m_RabbitList.push_back(4);

		Add_RabbitAppear(timer);
		timer.m_RabbitList.clear();

		timer.m_fTime = 1.f;
		timer.m_RabbitList.push_back(6);
		timer.m_RabbitList.push_back(7);
		timer.m_RabbitList.push_back(8);
		timer.m_RabbitList.push_back(9);

		Add_RabbitAppear(timer);

		m_bIsStart = true;
	}

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
	if (m_bIsStart && m_vecTimer.empty())
		m_bIsStart = false;

	for (auto& iter : m_vecRabbits)
	{
		iter->Update_Object(fTimeDelta);
	}

	return 0;
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
