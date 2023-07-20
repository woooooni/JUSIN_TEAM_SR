#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CCollider;
class CTransform;
class CAnimator;
//class CUI_ExclamationMark;
//class CUI_QuestionMark;

END

// Stage�� ENUM������ ���� �ٸ��� -> �浹(?)�� ��ȭâ �ٸ��� ������
class CNpc
	: public CGameObject
{
	CLONE(CNpc)

protected:
	explicit CNpc(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CNpc(const CNpc& rhs);
	virtual ~CNpc();

public:
	virtual HRESULT		Ready_Object(void) override;
	virtual _int		Update_Object(const _float& fTimeDelta) override;
	virtual void		LateUpdate_Object(void) override;
	virtual void		Render_Object(void) override;

//protected:
//	CUI_ExclamationMark* m_pExclamation = nullptr;
//	CUI_QuestionMark*	 m_pQuestion = nullptr;

public:
	static CNpc* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 vPos);

protected:
	virtual void Free() override;
};

