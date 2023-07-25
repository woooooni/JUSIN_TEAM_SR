#pragma once
#include	"Base.h"
#include	"Engine_Define.h"
#include	"RabitObj.h"

class CRabbitMgr : public CBase
{
private:
	DECLARE_SINGLETON(CRabbitMgr)

	CRabbitMgr() : m_iPoints(0)
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

	HRESULT	Ready_Object(const LPDIRECT3DDEVICE9& pGraphicDev, const _int& pRabbitNumX, const _int& pRabbitNumY, const _vec3& pStartPos);
	_int Update_Object(const _float& fTimeDelta);
	void		Add_RabbitAppear(const RABBIT_TIMER& pRab)
	{
		m_vecTimer.push_back(pRab);
	}

	void		Start_Rabbits() { m_bIsStart = true; }

	void		Add_Point()
	{
		m_iPoints++;
	}

protected:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	vector<CRabitObj*> m_vecRabbits;

	_int		m_iRabbitCountX, m_iRabbitCountY;

	vector<RABBIT_TIMER>	m_vecTimer;

	HRESULT		Ready_Rabits();

	_vec3		m_vStartPos;

	_bool		m_bIsStart;

	_float		m_fCurTime;

	_int		m_iPoints;

};

