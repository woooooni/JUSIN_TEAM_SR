#pragma once
#include "CUI.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CAnimator;

END

class CUI_Cursor : public CUI
{
private:
	explicit CUI_Cursor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUI_Cursor(const CUI_Cursor& rhs);
	virtual ~CUI_Cursor();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

public:
	_uint	Get_CursorXPos() { return m_iCursorX; }
	_uint	Get_CursorYPos() { return m_iCursorY; }

private:
	HRESULT Ready_Component();
	void	Key_Input();

private:
	_bool	m_bShown = true; // ������ �������� �ִ����� ���� �Һ���
	_bool	m_bMoveCursor = true; // Ŀ���� �ű� �� �ִ� ���������� ���� �Һ���
	_vec3	m_vDefaultPos;
	_uint	m_iCursorX = 0; // X������ �̵��ϴ� Cursor
	_uint	m_iCursorY = 0; // Y������ �̵��ϴ� Cursor

	_vec3	m_vOriginPos, m_vMovePos;
	_matrix m_matPreView, m_matPreProj;

public:
	static  CUI_Cursor* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void		Free() override;

};
