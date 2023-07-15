#include "Effect_Shadow.h"


#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"


CEffect_Shadow::CEffect_Shadow(LPDIRECT3DDEVICE9 pGraphicDev)
	: CEffect(pGraphicDev)
{
}

CEffect_Shadow::CEffect_Shadow(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_Shadow::~CEffect_Shadow()
{
}

HRESULT CEffect_Shadow::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"Shadow", L"Proto_Texture_Effect_Shadow", 0.1f);

	m_pAnimator->Play_Animation(L"Shadow", false);

	Set_Active(false);

	Engine::Get_Layer(LAYER_TYPE::ENVIRONMENT)->Add_GameObject(L"Shadow", this);

	return S_OK;
}

_int CEffect_Shadow::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_Shadow::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	_vec3 vPos;

	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

	vPos.y = 0.001f;
	m_pTransformCom->Set_Pos(&vPos);

	__super::LateUpdate_Object();
}

void CEffect_Shadow::Render_Object(void)
{
	if (!Is_Active())
		return;

	
	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(100, 255, 255, 255));


	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

}

CEffect_Shadow* CEffect_Shadow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Shadow* pInstance = new CEffect_Shadow(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Shadow_Create_Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_Shadow::Set_Shadow(CGameObject* _pObj, _vec3& _vScale)
{
	m_pOwner = _pObj;

	_float m_fAngle;

	m_fAngle = D3DXToRadian(90.0f);

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	for (_uint i = 0; INFO_END > i; ++i)
	{
		_vec3 vInfo;
		memcpy(&vInfo, &matWorld.m[i][0], sizeof(_vec3));
		m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
	}

	m_pTransformCom->Set_Scale(_vScale);
	m_pTransformCom->RotationAxis(_vec3(1.0f, 0.0f, 0.0f), m_fAngle);

	Set_Active(true);
}

HRESULT CEffect_Shadow::Add_Component(void)
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

void CEffect_Shadow::Free()
{
	__super::Free();
}
