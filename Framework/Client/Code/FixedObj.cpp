#include "FixedObj.h"
#include "Export_Function.h"

CFixedObj::CFixedObj(LPDIRECT3DDEVICE9 pGraphicDev, OBJ_ID eID)
    :CGameObject(pGraphicDev, OBJ_TYPE::OBJ_ENVIRONMENT, eID)
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
}


void CFixedObj::Free()
{
	__super::Free();
}
