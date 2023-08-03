#pragma once
#include "Effect.h"
class CEffect_NueTrail : public CEffect
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

	CLONE(CEffect_NueTrail)
protected:
	explicit CEffect_NueTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CEffect_NueTrail(const CEffect& rhs);
	virtual ~CEffect_NueTrail();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Return_Pool(void)							override;


	static CEffect_NueTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	
public:
	void		Set_Effect(CGameObject* _pOwner, _float _fScale, _vec3& _vFowardDir, _vec3& _vOffset, _vec3& _vUpOffset, _float _fAngleSpeed);

	void	Set_Color(_uint _iR, _uint _iG, _uint _iB, _uint _iA) 
	{
		m_vColor.r = _iR / 255.0f;
		m_vColor.g = _iG / 255.0f;
		m_vColor.b = _iB / 255.0f;
		m_vColor.a = _iA / 255.0f;
	}

	void	Set_End() { m_bEnd = true; }


	void		Move_Start();
protected:
	virtual HRESULT	Add_Component(void);


protected:
	virtual void Free() override;

private:
	void			Update_Start(const _float& fTimeDelta);
	void			Update_Move(const _float& fTimeDelta);
	void			Update_End(const _float& fTimeDelta);


	void			Rotate_Trail(void);


private:
	deque<VTXPoint> m_deqSecondPoint;
	deque<VTXPoint> m_deqFirstPoint;


	TRAILINFO m_tTrailInfo;

	_float m_fSumAngle;
	_float m_fAngle;
	_float m_fAngleSpeed;


	_vec3 m_vLinePoint[4];
	_vec3 m_vFront[4];
	_vec3 m_vFowardDir;

	_float m_fScale;
	CGameObject* m_pOwner = nullptr;


	D3DCOLORVALUE m_vColor;

	_bool m_bEnd = false;

	_vec3 m_vOwnerPos;


	NUETRAIL_STATE m_eTrailState;

	_uint m_iFirstMax;
	_uint m_iSecondMax;
};

