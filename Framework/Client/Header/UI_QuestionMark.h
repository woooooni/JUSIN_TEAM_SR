#pragma once

class CNpc;
class CUI_QuestionMark : public CUI
{
private:
	explicit CUI_QuestionMark(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_QuestionMark(const CUI_QuestionMark& rhs);
	virtual ~CUI_QuestionMark();

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
	static  CUI_QuestionMark* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;
};

