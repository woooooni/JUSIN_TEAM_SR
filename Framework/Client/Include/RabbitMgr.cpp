#include "RabbitMgr.h"
#include	"Export_Function.h"


IMPLEMENT_SINGLETON(CRabbitMgr)

void CRabbitMgr::Free()
{
}

HRESULT CRabbitMgr::Ready_Object(const _int& pRabbitNumX, const _int& pRabbitNumY)
{
	m_iRabbitCountX = pRabbitNumX;
	m_iRabbitCountY = pRabbitNumY;

	FAILED_CHECK(Ready_Rabits());

	return S_OK;
}

_int CRabbitMgr::Update_Object(const _float& fTimeDelta)
{
	return _int();
}

HRESULT CRabbitMgr::Ready_Rabits()
{



	return S_OK;
}
