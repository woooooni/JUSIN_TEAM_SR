#pragma once
#include "GameObject.h"

class CBullet : public Engine::CGameObject
{
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID _eID);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;


	CGameObject*	Get_Owner() { return m_pOwner; }
	void			Set_Owner(CGameObject* _pOwner) { m_pOwner = _pOwner; }
public:
	void Set_Atk(_uint _iAtk) { if (m_iAtk == _iAtk) return; m_iAtk = _iAtk; }
	_uint Get_Atk() { return  m_iAtk; }
protected:
	_uint m_iAtk=0;
protected:
	virtual void Free() override;

	CGameObject* m_pOwner = nullptr;
};

