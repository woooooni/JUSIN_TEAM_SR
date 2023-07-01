#include "..\Header\Terrain.h"
#include "Export_Function.h"

#include "RcTerrain.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 _pDevice)
	: CGameObject(_pDevice, OBJ_TYPE::OBJ_ENVIRONMENT)
	, m_fAccRandom(0.f)
	, m_fCoolTime(1.f)
{
}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{

}

CTerrain::~CTerrain()
{

}



HRESULT CTerrain::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransformCom->Set_Pos(&_vec3(0.f, 0.f, 0.f));
	Load_HeightMapInfo(L"../Bin/Resource/Texture/Terrain/Height.bmp");
	m_pBufferCom->ReBuildHeightMap(m_pTexHeightMap, 10.f, false);
	
	return S_OK;
}

_int CTerrain::Update_Object(const _float & fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_NONALPHA, this);
	_int iResult = __super::Update_Object(fTimeDelta);
	//Key_Input(fTimeDelta);
	
	m_fAccRandom += fTimeDelta;
	if (m_fAccRandom >= m_fCoolTime)
	{
		m_fAccRandom = 0.f;
		//m_pBufferCom->ReBuildHeightMap(m_vecHeightInfo, 4.f, true);
	}
	return iResult;
}

void CTerrain::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CTerrain::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();
}

HRESULT CTerrain::SetY_Terrain(CGameObject * _pTarget, _float fTimeDelta)
{
	CTransform* pTargetTrans = dynamic_cast<CTransform*>(_pTarget->Get_Component(COMPONENT_TYPE::COM_TRANSFORM, ID_STATIC));
	NULL_CHECK_RETURN(pTargetTrans, E_FAIL);

	_vec3 vTargetPos;
	pTargetTrans->Get_Info(INFO_POS, &vTargetPos);
	
	_float fX = ((_float)m_pBufferCom->m_iWidth / 2.0f) + vTargetPos.x;
	_float fZ = ((_float)m_pBufferCom->m_iHeight / 2.0f) - vTargetPos.z;

	_int col = _int(fX);
	_int row = _int(fZ);

	_float dx = fX - col;
	_float dz = fZ - row;

	
	VTXTEX* pVB;
	LPDIRECT3DVERTEXBUFFER9 pBuffer = m_pBufferCom->GetBuffer();

	pBuffer->Lock(0, 0, (void**)&pVB, 0);

	_vec3 vVertex1 = pVB[row * m_pBufferCom->m_iVertexCountRow + col].vPosition;			// x : 0 z : 0
	_vec3 vVertex2 = pVB[row * m_pBufferCom->m_iVertexCountRow + col + 1].vPosition;		// x : 1 z : 0
	_vec3 vVertex3 = pVB[(row + 1) * m_pBufferCom->m_iVertexCountRow + col].vPosition;		// x : 0 z : -1
	_vec3 vVertex4 = pVB[(row + 1) * m_pBufferCom->m_iVertexCountRow + col + 1].vPosition;	// x : 1 z : -1

	D3DXPLANE plane;

	if (dz < 1.0f - dx) // 위쪽 삼각형 ABC
	{
		D3DXPlaneFromPoints(&plane, &vVertex1, &vVertex2, &vVertex3);
	}
	else
	{
		D3DXPlaneFromPoints(&plane, &vVertex2, &vVertex3, &vVertex4);
	}

	// 플레인 구해서 
	_float fHeight = -1.f * (plane.c*vTargetPos.z + plane.a*vTargetPos.x + plane.d) / plane.b;

	pBuffer->Unlock();


	vTargetPos.y = fHeight + 1.f;
	pTargetTrans->Set_Info(INFO_POS, &vTargetPos);

	return S_OK;
}


void CTerrain::Load_HeightMapInfo(const wstring& _strFilePath)
{
	_uint iWidth = m_pBufferCom->m_iCellCountRow;
	_uint iHeight = m_pBufferCom->m_iCellCountCol;

	if (FAILED(D3DXCreateTextureFromFileEx(m_pGraphicDev,
		_strFilePath.c_str(), D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, 0,
		D3DFMT_X8R8G8B8, D3DPOOL_MANAGED,
		D3DX_DEFAULT, D3DX_DEFAULT, 0,
		NULL, NULL, &m_pTexHeightMap
	)))
	{
		MSG_BOX("HeightMap Load Failed");
		return;
	}
	
	//m_pBufferCom->ReBuildHeightMap()
	return;
}

int CTerrain::GetHeightMapEntry(int iRow, int iCol)
{
	return m_vecHeightInfo[iRow * m_pBufferCom->m_iVertexCountRow + iCol];
}

void CTerrain::SetHeightMapEntry(int iRow, int iCol, int value)
{
	m_vecHeightInfo[iRow * m_pBufferCom->m_iVertexCountRow + iCol] = value;
}


HRESULT CTerrain::Add_Component(void)
{
	CComponent*			pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTerrain*>(Engine::Clone_Proto(L"Proto_RcTerrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pBufferCom->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pTextureCom->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_pTransformCom->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	return S_OK;
}


CTerrain * CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain* pInstance = new CTerrain(pGraphicDev);
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Terrain Create Failed");
		return nullptr;
	}
	return pInstance;
}

void CTerrain::Free()
{
	Safe_Release(m_pTexHeightMap);
	__super::Free();
}

