#include "FixedObj.h"
#include "Export_Function.h"

CFixedObj::CFixedObj(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT, OBJ_ID::FIXED_OBJ)
{
}

CFixedObj::CFixedObj(const CFixedObj& rhs)
    :CGameObject(rhs)
{
}

CFixedObj::~CFixedObj()
{
}

HRESULT CFixedObj::Ready_Object(void)
{
    return S_OK;
}

_int CFixedObj::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	__super::Update_Object(fTimeDelta);
    return S_OK;
}

void CFixedObj::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CFixedObj::Render_Object(void)
{
	__super::Render_Object();
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();
}


void CFixedObj::Free()
{
	__super::Free();
}
