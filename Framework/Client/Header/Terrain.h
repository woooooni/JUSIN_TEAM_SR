#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CTransform;
class CRcTerrain;

END

class CTerrain : public CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 _pDevice);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain();

private:
	HRESULT		Add_Component(void);

public:
	virtual HRESULT Ready_Object(void) override;
	virtual _int Update_Object(const _float& fTimeDelta) override;
	virtual void LateUpdate_Object(void) override;
	virtual void Render_Object(void) override;

public:
	HRESULT		SetY_Terrain(CGameObject* _pTarget, _float fTimeDelta);
	CRcTerrain*	GetTerrainVertex() { return m_pBufferCom; }
private:
	void		Load_HeightMapInfo(const wstring& _strFilePath);
	int			GetHeightMapEntry(int iRow, int iCol);
	void		SetHeightMapEntry(int iRow, int iCol, int value);

private:
	CRcTerrain*				m_pBufferCom = nullptr;
	CTexture*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;
	LPDIRECT3DTEXTURE9      m_pTexHeightMap = nullptr;

	vector<int>		m_vecHeightInfo;

	_float			m_fAccRandom;
	_float			m_fCoolTime;
	
public:
	static CTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free() override;
};

