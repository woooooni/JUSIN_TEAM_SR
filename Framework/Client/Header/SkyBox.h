#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTransform;
class CTexture;

END

class CSkyBox : public Engine::CGameObject
{
	CLONE(CSkyBox)
private:
	explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSkyBox(const CSkyBox& rhs);
	virtual ~CSkyBox();

public:

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

private:
	HRESULT		Add_Component(void);
public:
	void Set_Index(_int _iIndex) { m_iIndex = _iIndex; }
private:
	_int m_iIndex =0;
public:
	static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int _iIndex);
private:
	virtual void Free() override;

};


