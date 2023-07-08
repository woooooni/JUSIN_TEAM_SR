#pragma once
#include "LightPuzzle.h"
class CLightPuzzleBase :    public CLightPuzzle
{
protected:
	CLightPuzzleBase(LPDIRECT3DDEVICE9 pGraphicDev);
	CLightPuzzleBase(const CLightPuzzleBase& rhs);
	virtual ~CLightPuzzleBase();
public:

	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CLightPuzzleBase* Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum = 0, const _vec3 p_Pos = { 0, 0, 0 });

public:
	void		Set_MakeLight() { m_bMakeLight = true; }
	const _bool& Get_MakeLight() { return m_bMakeLight; }

protected:
	_bool		m_bMakeLight;

};

