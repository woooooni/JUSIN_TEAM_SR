#pragma once
#include "GameObject.h"



class CCactusNeedle :
	public CGameObject
{

	CLONE(CCactusNeedle)

protected:
	explicit CCactusNeedle(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCactusNeedle(const CCactusNeedle& rhs);
	virtual ~CCactusNeedle();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

protected:
	HRESULT	Add_Component(void);



public:

	void Set_Poison(_bool _bPoison) { if (m_bPoison == _bPoison) return; m_bPoison = _bPoison; }
	_bool Is_Poison() { return m_bPoison; }
	void Set_Spin(_bool _bSpin) { if (m_bSpin == _bSpin) return; m_bSpin = _bSpin; }
	_bool Is_Spin() { return m_bSpin; }

	static CCactusNeedle* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	OBJ_DIR m_eDir;
	_vec3 m_vDir;
	_bool	m_bPoison = false;
	_bool m_bSpin = true;
protected:
	_float m_fMoveTime=20.f;
protected:
	virtual void Free() override;

};