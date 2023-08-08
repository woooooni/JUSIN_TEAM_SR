#pragma once
#include "GameObject.h"

class CTest_Cube : public Engine::CGameObject
{
	CLONE(CTest_Cube)

private:
	explicit CTest_Cube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTest_Cube(const CTest_Cube& rhs);
	virtual ~CTest_Cube();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

private:
	HRESULT Ready_Component(void);

public:
	static CTest_Cube* Create(LPDIRECT3DDEVICE9 _pGraphicDev);
	virtual void Free() override;
};

