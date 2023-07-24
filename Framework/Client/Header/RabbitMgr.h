#pragma once
#include	"Base.h"
#include	"Engine_Define.h"
#include	"RabitObj.h"

class CRabbitMgr : public CBase
{
private:
	DECLARE_SINGLETON(CRabbitMgr)

	CRabbitMgr()
	{}
	~CRabbitMgr() { Free(); }

public:
	struct RABBIT_TIMER
	{
		_float m_fTime;
		list<_int> m_RabbitList;
	};

public:
	// CBase을(를) 통해 상속됨
	virtual void Free() override;

	HRESULT	Ready_Object(const _int& pRabbitNumX, const _int& pRabbitNumY);
	_int Update_Object(const _float& fTimeDelta);

protected:
	vector<CRabitObj*> m_vecRabbits;

	_int		m_iRabbitCountX, m_iRabbitCountY;

	vector<RABBIT_TIMER>	m_vecTimer;

	HRESULT		Ready_Rabits();

};

