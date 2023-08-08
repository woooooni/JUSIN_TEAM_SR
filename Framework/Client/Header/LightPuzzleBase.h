#pragma once
#include "LightPuzzle.h"
class CLightPuzzleBase :    public CLightPuzzle
{
	CLONE(CLightPuzzleBase)
protected:
	CLightPuzzleBase(LPDIRECT3DDEVICE9 pGraphicDev);
	CLightPuzzleBase(const CLightPuzzleBase& rhs);
	virtual ~CLightPuzzleBase();
public:

	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID) override;

public:
	void		Set_MakeLight() { m_bMakeLight = true; }
	const _bool& Get_MakeLight() { return m_bMakeLight; }
	virtual		void	Set_Lighting(const _bool& pBool) override;

	virtual void		Reset_Event()override;



protected:
	_bool		m_bMakeLight;
	_uint		m_iMyEvent;

public:
	virtual void    Free() override;
	static			CLightPuzzleBase* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 }, const _tchar* p_Type = L"Base");

};

