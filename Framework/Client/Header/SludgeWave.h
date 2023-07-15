#pragma once
#include "GameObject.h"


BEGIN(Engine)

class CRcTex;
class CTransform;

END

class CSludgeWave : public Engine::CGameObject
{
	CLONE(CSludgeWave)

protected:
	explicit CSludgeWave(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSludgeWave(const CSludgeWave& rhs);
	virtual ~CSludgeWave();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);


public:
	
	static CSludgeWave* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Set_Atk(_int _iAtk) { m_iAtk = _iAtk; }
	void Set_Wave(_int _iWave) { m_iWave = _iWave; }

	void Create_Wave();
private:
	_int m_iWave;
	_int m_iAtk = 0;
	_bool m_bDuplicate=true;
protected:
	_float m_fMoveTime;
protected:
	virtual void Free() override;

};