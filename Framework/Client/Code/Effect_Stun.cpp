#include "Effect_Stun.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"

CEffect_Stun::CEffect_Stun(LPDIRECT3DDEVICE9 pGraphicDev)
    :CEffect(pGraphicDev)
{
}

CEffect_Stun::CEffect_Stun(const CEffect& rhs)
    : CEffect(rhs)
{
}

CEffect_Stun::~CEffect_Stun()
{
}

HRESULT CEffect_Stun::Ready_Object(void)
{
    FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

    m_pAnimator->Add_Animation(L"StunEffect", L"Proto_Texture_Effect_Stun", 0.05f);

    m_pAnimator->Play_Animation(L"StunEffect", true);

    Set_Active(false);

    return S_OK;
}

_int CEffect_Stun::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;



	if (*m_fEffectTime <= 0.0f || !m_pOwner->Is_Active())
	{
		Set_Active(false);
		CPool<CEffect_Stun>::Return_Obj(this);
	}


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_Stun::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	_vec3 vPos;
	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

	vPos += m_vOffSet;

	m_pTransformCom->Set_Pos(&vPos);


	__super::LateUpdate_Object();
}

void CEffect_Stun::Render_Object(void)
{
	if (!Is_Active())
		return;

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();
}

void CEffect_Stun::Return_Pool(void)
{
	Set_Active(false);
	CPool<CEffect_Stun>::Return_Obj(this);
}

CEffect_Stun* CEffect_Stun::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Stun* pInstance = new CEffect_Stun(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Hit Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_Stun::Get_Effect(CGameObject* _pObj, _vec3& _vOffSet, _vec3& _vScale, _float* fTime)
{
	m_fEffectTime = fTime;

	m_pOwner = _pObj;

	m_vOffSet = _vOffSet;

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	for (_uint i = 0; INFO_END > i; ++i)
	{
		_vec3 vInfo;
		memcpy(&vInfo, &matWorld.m[i][0], sizeof(_vec3));
		m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
	}

	m_pTransformCom->Set_Scale(_vScale);

	m_pAnimator->GetCurrAnimation()->Set_Idx(0);
	m_pAnimator->GetCurrAnimation()->Set_Finished(false);
	Set_Active(true);
	Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"StunEffect", this);
}

HRESULT CEffect_Stun::Add_Component(void)
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

void CEffect_Stun::Free()
{
	__super::Free();
}
