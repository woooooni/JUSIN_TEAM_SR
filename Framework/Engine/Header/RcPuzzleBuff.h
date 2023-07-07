#include	"VIBuffer.h"


BEGIN(Engine)
class ENGINE_DLL CRcPuzzleBuff : public CVIBuffer
{
public:
	explicit CRcPuzzleBuff();
	explicit CRcPuzzleBuff(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CRcPuzzleBuff(const CRcPuzzleBuff& rhs);
	virtual ~CRcPuzzleBuff();

public:
	const _vec3* Get_VtxPos() { return m_pPos; }

public:
	HRESULT			Ready_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);
	void			Render_Buffer(void);

private:
	HANDLE			m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;
	_vec3* m_pPos;

public:
	static CRcPuzzleBuff* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _ulong& dwCntX = 6,
		const _ulong& dwCntZ = 5,
		const _ulong& dwVtxItv = VTXITV);

	virtual CComponent* Clone(void);

private:
	virtual	void	Free(void);

};

END