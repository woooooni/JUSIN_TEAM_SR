#include "Effect_LightningGround.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"


CEffect_LightningGround::CEffect_LightningGround(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)
{
}

CEffect_LightningGround::CEffect_LightningGround(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_LightningGround::~CEffect_LightningGround()
{
}

HRESULT CEffect_LightningGround::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"LightningGround", L"Proto_Texture_Effect_LightningGround", 0.1f);

	m_pAnimator->Play_Animation(L"LightningGround", false);

	Set_Active(false);

	return S_OK;
}

_int CEffect_LightningGround::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_Active(false);
		CPool<CEffect_LightningGround>::Return_Obj(this);
	}


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_LightningGround::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;


	__super::LateUpdate_Object();
}

void CEffect_LightningGround::Render_Object(void)
{
	if (!Is_Active())
		return;

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();
}

CEffect_LightningGround* CEffect_LightningGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_LightningGround* pInstance = new CEffect_LightningGround(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_LightningGround Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_LightningGround::Get_Effect(_vec3& _vPos, _vec3& _vScale)
{
	_vPos.z -= 0.001f;
	_vPos.y = 0.008f;

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

	m_pTransformCom->Set_Pos(&_vPos);
	m_pTransformCom->Set_Scale(_vScale);
	m_pTransformCom->RotationAxis(_vec3(1.0f, 0.0f, 0.0f), m_fAngle);

	m_pAnimator->GetCurrAnimation()->Set_Idx(0);
	m_pAnimator->GetCurrAnimation()->Set_Finished(false);
	Set_Active(true);
	Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"DieSmokeEffect", this);
}

HRESULT CEffect_LightningGround::Add_Component(void)
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

void CEffect_LightningGround::Free()
{
	__super::Free();
}
