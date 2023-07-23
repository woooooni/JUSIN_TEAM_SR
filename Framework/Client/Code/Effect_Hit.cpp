#include "Effect_Hit.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"

CEffect_Hit::CEffect_Hit(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)
{
}

CEffect_Hit::CEffect_Hit(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_Hit::~CEffect_Hit()
{
}

HRESULT CEffect_Hit::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"HitEffect", L"Proto_Texture_Effect_Hit", 0.05f);

	m_pAnimator->Play_Animation(L"HitEffect", false);

	Set_Active(false);

	return S_OK;
}

_int CEffect_Hit::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_Active(false);
		CPool<CEffect_Hit>::Return_Obj(this);
	}


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_Hit::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	Set_Billboard();

	__super::LateUpdate_Object();
}

void CEffect_Hit::Render_Object(void)
{
	if (!Is_Active())
		return;
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, m_iR, m_iG, m_iB));

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CEffect_Hit* CEffect_Hit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Hit* pInstance = new CEffect_Hit(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Hit Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_Hit::Get_Effect(_vec3& _vPos, _vec3& _vScale, _uint _iR, _uint _iG, _uint _iB)
{
	_vPos.z -= 0.001f;
	m_iR = _iR;
	m_iG = _iG;
	m_iB = _iB;

	m_pTransformCom->Set_Pos(&_vPos);
	m_pTransformCom->Set_Scale(_vScale);
	m_pAnimator->GetCurrAnimation()->Set_Idx(0);
	m_pAnimator->GetCurrAnimation()->Set_Finished(false);
	Set_Active(true);
	Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"HitEffect", this);
}

HRESULT CEffect_Hit::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);


	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	return S_OK;
}

void CEffect_Hit::Free()
{
	__super::Free();
}
