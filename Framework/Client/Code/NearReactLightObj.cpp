#include "../Include/stdafx.h"
#include "NearReactLightObj.h"

CNearReactLightObj::CNearReactLightObj(LPDIRECT3DDEVICE9 pGraphicDev) : CNearReactObj(pGraphicDev)
{
}

CNearReactLightObj::CNearReactLightObj(const CNearReactObj& rhs) : CNearReactObj(rhs)
{
}

CNearReactLightObj::~CNearReactLightObj()
{
}

HRESULT CNearReactLightObj::Ready_Object(void)
{
	return E_NOTIMPL;
}

_int CNearReactLightObj::Update_Object(const _float& fTimeDelta)
{
	return _int();
}

void CNearReactLightObj::LateUpdate_Object(void)
{
}

void CNearReactLightObj::Render_Object(void)
{
}

void CNearReactLightObj::Free()
{
}

CNearReactObj* CNearReactLightObj::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3 p_pos)
{
	return nullptr;
}

HRESULT CNearReactLightObj::Ready_Component(void)
{
	return E_NOTIMPL;
}
