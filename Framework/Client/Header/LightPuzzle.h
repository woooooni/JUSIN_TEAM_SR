#pragma once
#include "FieldObject.h"
class CLightPuzzle : public CFieldObject
{
protected:
	CLightPuzzle(LPDIRECT3DDEVICE9 pGraphicDev);
	CLightPuzzle(const CLightPuzzle& rhs);
	virtual ~CLightPuzzle();

public:
	virtual void Free() override;

	void		Reverse_Puzzle(_bool isX);

	void		Add_LightDir(const _vec2& dir) { m_lightDir.push_back(dir); }

	const	list<_vec2>& Get_LightDir() { return m_lightDir; }

	virtual		void	Set_Lighting(const _bool& pBool) { m_bIsLighting = pBool; }
	virtual		const _bool& Get_Lighting() { return m_bIsLighting; }

protected:
	list<_vec2>		m_lightDir;
	_bool			m_bIsLighting;
};

