#pragma once

#include "Engine_Define.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev, COMPONENT_TYPE _eType);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual	_int Update_Component(const _float& fTimeDelta) { return 0; }
	virtual void LateUpdate_Component(void) {}
	virtual void Render_Component(void) {};
public:
	CGameObject*	GetOwner()						{ return m_pOwner; }
	void			SetOwner(CGameObject* _pOwner)	{ m_pOwner = _pOwner; }
	COMPONENT_TYPE	GetComponentType() { return m_eType; }

	_bool	Is_Active() { return m_bActive; }
	void	Set_Active(_bool _bActive) { m_bActive = _bActive; }

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	_bool						m_bClone;
	CGameObject*				m_pOwner;
	COMPONENT_TYPE				m_eType;

private:
	_bool m_bActive;
public:
	virtual CComponent*		Clone(void)		PURE;
	virtual void			Free(void)	override;
};

END