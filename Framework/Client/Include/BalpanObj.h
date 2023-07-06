#pragma once
#include "FieldObject.h"
class CBalpanObj : public CFieldObject
{
protected:
	CBalpanObj(LPDIRECT3DDEVICE9 p_Dev);
	CBalpanObj(const CBalpanObj& rhs);
	virtual ~CBalpanObj();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

	virtual void    Free() override;

	static			CBalpanObj* Create(LPDIRECT3DDEVICE9 p_Dev, const _vec3 p_Pos = { 0, 0, 0 });

protected:
	virtual HRESULT		Ready_Component();

};

