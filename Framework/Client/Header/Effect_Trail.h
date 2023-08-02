#pragma once
#include "Effect.h"
class CEffect_Trail : public CEffect
{
	typedef struct tVertexPoint
	{
		_vec3 vPos[3];
	}VTXPoint;

	struct TRAILINFO
	{
		D3DVECTOR PointLeft;
		D3DVECTOR PointRight;
		D3DVECTOR NextLeft;
		D3DVECTOR NextRight;

		float fSize;
		float fIndex;
	};

	CLONE(CEffect_Trail)
protected:
	explicit CEffect_Trail(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_Trail(const CEffect& rhs);
	virtual ~CEffect_Trail();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;

	static CEffect_Trail* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
public:
	void		Set_Effect(CGameObject* _pOwner, _float _fScale)
	{
		m_pOwner = _pOwner;
		m_fScale = _fScale;
		m_fOffsetSpeed = 0.0f;
	}

	void	Set_Offset(_vec3& _vOffset1, _vec3& _vOffset2, _vec3& _vOffset3, _vec3& _vOffset4) 
	{ 
		m_vOffset[0] = _vOffset1;
		m_vOffset[1] = _vOffset2;
		m_vOffset[2] = _vOffset3;
		m_vOffset[3] = _vOffset4;
		m_fOffsetSpeed = 0.0f;
	}
	void	Set_Max(_uint _iMax) { m_iMaxSize = _iMax; }
	void	Set_Color(_uint _iR, _uint _iG, _uint _iB, _uint _iA) 
	{
		m_vColor.r = _iR / 255.0f;
		m_vColor.g = _iG / 255.0f;
		m_vColor.b = _iB / 255.0f;
		m_vColor.a = _iA / 255.0f;
	}

	void	Set_End() { m_bEnd = true; }
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;




private:
	deque<VTXPoint> m_deqPoint;
	TRAILINFO m_tTrailInfo;

	_float m_fAccPushTime;
	_float m_fPushTime;

	_vec3 m_vCurrOffset;
	_vec3 m_vOffset[4];
	_float m_fOffsetSpeed;
	_float m_fPlusTime;

	_uint m_iMaxSize;

	_float m_fScale;
	CGameObject* m_pOwner = nullptr;

	_bool m_bPush = false;

	D3DCOLORVALUE m_vColor;

	_bool m_bEnd = false;

	_vec3 m_vPos;
};

