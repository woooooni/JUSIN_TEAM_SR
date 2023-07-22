#include "Effect_Block.h"
#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"

CEffect_Block::CEffect_Block(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)
{
}

CEffect_Block::CEffect_Block(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_Block::~CEffect_Block()
{
}

HRESULT CEffect_Block::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"Block", L"Proto_Texture_Effect_Block", 0.05f);

	m_pAnimator->Play_Animation(L"Block", false);

	Set_Active(false);

	return S_OK;
}

_int CEffect_Block::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	if (m_pAnimator->GetCurrAnimation()->Is_Finished())
	{
		Set_Active(false);
		CPool<CEffect_Block>::Return_Obj(this);
	}


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_Block::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	Set_Billboard();

	__super::LateUpdate_Object();
}

void CEffect_Block::Render_Object(void)
{
	if (!Is_Active())
		return;

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();
}

CEffect_Block* CEffect_Block::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Block* pInstance = new CEffect_Block(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Block Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_Block::Get_Effect(_vec3& _vPos, _vec3& _vScale)
{
	_vPos.z -= 0.001f;
	m_pTransformCom->Set_Pos(&_vPos);
	m_pTransformCom->Set_Scale(_vScale);
	m_pAnimator->GetCurrAnimation()->Set_Idx(0);
	m_pAnimator->GetCurrAnimation()->Set_Finished(false);
	Set_Active(true);
	Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"BlockEffect", this);
}

HRESULT CEffect_Block::Add_Component(void)
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

void CEffect_Block::Free()
{
	__super::Free();
}
