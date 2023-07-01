#pragma once
#include "Component.h"


BEGIN(Engine)
class CTexture;
class ENGINE_DLL CAnimator : public CComponent
{
public:
	explicit CAnimator();
	explicit CAnimator(LPDIRECT3DDEVICE9 _pDevice);
	explicit CAnimator(const CAnimator& rhs);
	virtual ~CAnimator();

public:
	HRESULT			Ready_Animator();
	virtual	_int	Update_Component(const _float& fTimeDelta) override;
	virtual void	LateUpdate_Component() override;
	virtual void	Render_Component()	override;

public:
	HRESULT		Add_Animation(const wstring& _strKey, const wstring& _strProtoTexture);
	HRESULT		Play_Animation(const wstring& _strKey);
	CTexture*	GetCurrAnimation()							{ return m_pCurAnimation; }
	_bool		IsFinished()								{ return m_bFinished; }


private:
	map<wstring, CTexture*>			m_mapTexture;
	CTexture*						m_pCurAnimation;
	_bool							m_bFinished;

	_float							m_fAccTime;
	_float							m_fChangeTime;

public:
	static CAnimator*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void			Free();
	virtual CComponent *	Clone(void) override;
};

END


