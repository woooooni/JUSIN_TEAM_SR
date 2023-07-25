#pragma once
#include "Effect.h"



class CEffect_MothFlyLine :
	public CEffect
{

	CLONE(CEffect_MothFlyLine)

protected:
	explicit CEffect_MothFlyLine(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_MothFlyLine(const CEffect_MothFlyLine& rhs);
	virtual ~CEffect_MothFlyLine();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	void Set_Front(_bool _bFront) { m_bFront = _bFront; };
	void Set_Render(_bool _bRender) { m_bRender = _bRender; };

	void Set_Owner(CGameObject* _pOwner) { m_pOwner = _pOwner; }
	void Set_ARGB( int _iAlpha, int _iRed, int _iGreen, int _iBlue) {m_iRed = _iRed; m_iGreen = _iGreen; m_iBlue = _iBlue; m_iAlpha = _iAlpha;}
	void Set_Dir(_vec3 _vDir) { m_vDir = _vDir; }

private:
	HRESULT	Add_Component(void);
	_bool	m_bFront;
	_bool m_bRender = true;
	CGameObject* m_pOwner=nullptr;
	int m_iRed = 255.f;
	int m_iGreen = 255.f;
	int m_iBlue = 255.f;
	int m_iAlpha = 255.f;
	_vec3 m_vDir;
public:
	static CEffect_MothFlyLine* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};