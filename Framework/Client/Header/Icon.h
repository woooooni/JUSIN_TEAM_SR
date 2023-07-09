#pragma once
#include "CUI.h"

// 아무 기능 없는 메인 컴포넌트 아이콘 클래스
// QuickSlot -> 따로 클래스 생성

BEGIN(Engine)

class CRcTex;
class CTransform;
class CTexture;

END

typedef enum class IconType
{
	PLAYERHP_FRAME,
	HEART, KEYBOARD, QUEST,
	KEYBUTTON,
	PLAYERHP, PLAYERMAXHP,

	ICONTYPE_END

}ICONTYPE;

struct tagIconInfo
{
	ICONTYPE		eType;
};

class CIcon : public CUI
{
	CLONE(CIcon)
private:
	CIcon(LPDIRECT3DDEVICE9 pGraphicDev);
	CIcon(const CIcon& rhs);
	virtual ~CIcon();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

private:
	HRESULT			Add_Component(void);
	void			Set_Type(ICONTYPE eType);

private:
	tagIconInfo			m_tInfo;

public:
	static  CIcon* Create(LPDIRECT3DDEVICE9 pGraphicDev, ICONTYPE eType);

private:
	virtual void		Free() override;
};

