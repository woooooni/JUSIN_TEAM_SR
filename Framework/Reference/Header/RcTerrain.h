#pragma once
#include "VIBuffer.h"


BEGIN(Engine)
class ENGINE_DLL CRcTerrain :
	public CVIBuffer
{
public:
	explicit CRcTerrain();
	explicit CRcTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcTerrain(const CRcTerrain& rhs);
	virtual ~CRcTerrain();

public:
	virtual HRESULT			Ready_Buffer();
	virtual void			Render_Buffer(void);

public:
	_uint m_iVertexCountRow; 
	_uint m_iVertexCountCol;
	_uint m_iCellSpacing;

	_uint m_iCellCountRow;
	_uint m_iCellCountCol;

	_uint m_iWidth;
	_uint m_iHeight;
	

	list<VTXTEX> m_listVertex;
	list<INDEX32> m_listIndex;

private:
	virtual	void	Free(void);

public:
	static CRcTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _uint iVertexRow, _uint iVertexCol, _uint iCellSpacing);
	virtual CComponent*		Clone(void);



};

END
