#include "Effect_GetItem.h"


#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"

CEffect_GetItem::CEffect_GetItem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)
{
}

CEffect_GetItem::CEffect_GetItem(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_GetItem::~CEffect_GetItem()
{
}

HRESULT CEffect_GetItem::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"GetItem", L"Proto_Texture_Effect_GetItem", 0.1f);

	m_pAnimator->Play_Animation(L"GetItem", false);

	Set_Active(false);

	return S_OK;
}

_int CEffect_GetItem::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;


	switch (m_iTurn)
	{
	case 0:
		m_vScale += _vec3(0.1f, 0.1f, 0.1f);
		if (m_vScale.x > 2.0f)
			m_iTurn = 1;
		break;
	case 1:
		if (m_vScale.x > 1.8f)
			m_vScale -= _vec3(0.05f, 0.05f, 0.05f);
		break;
	case 2:
		m_vScale -= _vec3(0.2f, 0.2f, 0.2f);
		if (m_vScale.x <= 0.01f)
			Set_Active(false);
		break;
	}
	m_pTransformCom->Set_Scale(m_vScale);





	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_GetItem::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	__super::LateUpdate_Object();
}


void CEffect_GetItem::Render_Object(void)
{
	if (!Is_Active())
		return;

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(50, 255, 255, 255));

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CEffect_GetItem* CEffect_GetItem::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_GetItem* pInstance = new CEffect_GetItem(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_GetItem Create Failed");
		return nullptr;
	}

	return pInstance;
}


void CEffect_GetItem::Get_Effect(_vec3& _vPos, CGameObject* _pItem)
{
	m_iTurn = 0;
	m_pTransformCom->Set_Scale(_vec3(0.001f, 0.001f, 0.001f));
	m_vScale = { 0.001f, 0.001f, 0.001f };

	m_pTransformCom->Set_Pos(&_vPos);
	Set_Active(true);
}

HRESULT CEffect_GetItem::Add_Component(void)
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

void CEffect_GetItem::Free()
{
}
