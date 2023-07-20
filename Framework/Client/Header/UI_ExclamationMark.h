#pragma once
#include "CUI.h"
#include "Npc.h"

BEGIN(Engine)
class CRcTex;
class CTransform;
class CTexture;
END;

class CUI_ExclamationMark : public CUI
{
private:
	CUI_ExclamationMark(LPDIRECT3DDEVICE9 pGraphicDev);
	CUI_ExclamationMark(const CUI_ExclamationMark& rhs);
	virtual ~CUI_ExclamationMark();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	HRESULT	Add_Component(void);
	void	Set_Owner(CNpc* _pOwner) { m_pOwner = _pOwner; }

public:
	CNpc* m_pOwner = nullptr;

public:
	static  CUI_ExclamationMark* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

