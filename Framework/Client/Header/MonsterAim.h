#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;
class CAnimator;

END

class CMonsterAim : public CGameObject
{
	CLONE(CMonsterAim)
private:
	explicit CMonsterAim(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMonsterAim(const CMonsterAim& rhs);
	virtual ~CMonsterAim();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Ready_Component(void);


private:
	
public:
	static CMonsterAim* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Set_Red(_uint _iRed);
private:
	_uint m_iRed = 255;
private:
	virtual void Free() override;


};

