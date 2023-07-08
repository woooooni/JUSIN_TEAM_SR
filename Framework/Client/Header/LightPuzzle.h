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

protected:
	list<_vec2>		m_lightDir;
};

