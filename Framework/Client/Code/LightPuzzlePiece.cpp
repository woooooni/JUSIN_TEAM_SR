#include "LightPuzzlePiece.h"
#include "Export_Function.h"

CLightPuzzlePiece::CLightPuzzlePiece(LPDIRECT3DDEVICE9 pGraphicDev) : CLightPuzzle(pGraphicDev)
{
	m_tInfo.m_bIsPushable = true;
}

CLightPuzzlePiece::CLightPuzzlePiece(const CLightPuzzlePiece& rhs) : CLightPuzzle(rhs)
{
}

CLightPuzzlePiece::~CLightPuzzlePiece()
{
}

HRESULT CLightPuzzlePiece::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_MoonPuzzleTile", 0.f);

	m_pAnimator->Play_Animation(L"Base", false);
	

	return S_OK;
}

_int CLightPuzzlePiece::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);

	return __super::Update_Object(fTimeDelta);
}

void CLightPuzzlePiece::LateUpdate_Object(void)
{
}

void CLightPuzzlePiece::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CLightPuzzlePiece::Free()
{
}

CLightPuzzlePiece* CLightPuzzlePiece::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos)
{
	CLightPuzzlePiece* ret = new CLightPuzzlePiece(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("LightPuzzlePiece Create Failed");
		return nullptr;
	}
	ret->m_pTransformCom->Set_Pos(&_vec3(p_Pos.x, 0.001f, p_Pos.z));
	ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));
	return ret;
}
