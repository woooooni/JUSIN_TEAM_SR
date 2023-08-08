#pragma once
#include "GameObject.h"
#include "UI_BossHP.h"

class CNexus : public CGameObject
{
	CLONE(CNexus)
private:
	explicit CNexus(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNexus(const CNexus& rhs);
	virtual ~CNexus();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;


	virtual void Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;
	virtual void Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

private:
	HRESULT			Ready_Component(void);


public:
	static CNexus* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void		Start_Defence();
	
	_int		Get_MaxHp() { return m_iMaxHp; }
	_int		Get_Hp() { return m_iHp; }

	void		Reset_Defence();

	void		Reset() 
	{
		m_bClear = false;
		m_bFail = false;
		m_bStart = false;
		m_fDefenceAccTime = 0.0f;
		m_bOpen = false;
		m_iHp = m_iMaxHp;
		m_iAlpha = 255;
	}


	void		Add_Spawner(CGameObject* _pSpawner) { m_vecSpawner.push_back(_pSpawner); }
private:
	_int m_iHp;
	_int m_iMaxHp;
	_float m_fDefenceTime;
	_float m_fDefenceAccTime;

	_uint m_iAlpha;


	bool m_bStart = false;
	bool m_bFail = false;
	bool m_bClear = false;
	bool m_bOpen = false;

private:
	virtual void Free() override;


	vector<CGameObject*> m_vecSpawner;

	vector<CGameObject*> m_vecHpBar;
};

