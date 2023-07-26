
#include "MothOrb.h"

#include "Export_Function.h"

CMothOrb::CMothOrb(LPDIRECT3DDEVICE9 pGraphicDev)
	: CItem(pGraphicDev, ITEM_TYPE::ETC, OBJ_ID::ITEM)
{

}

CMothOrb::CMothOrb(const CMothOrb& rhs) : CItem(rhs)
{
}

CMothOrb::~CMothOrb()
{
}



HRESULT CMothOrb::Ready_Object(void)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pAnimator->Add_Animation(L"MothMageOrb", L"Proto_Texture_MothMageOrb", 0.1f);

	m_pTransformCom->Set_Info(INFO_POS, &_vec3(4.0f, 1.0f, 4.0f));
	m_pTransformCom->Set_Scale({ 0.5,0.5,0.5 });
	m_pAnimator->Play_Animation(L"MothMageOrb", true);


	return S_OK;
}

_int CMothOrb::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);
	if (!Is_Active())
		return S_OK;
	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}
void CMothOrb::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}
void CMothOrb::Render_Object(void)
{
	if (!Is_Active())
		return ;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();
	
}

HRESULT CMothOrb::Add_Component(void)
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

CMothOrb* CMothOrb::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMothOrb* pInstance = new CMothOrb(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("MothOrb Create Failed");
		return nullptr;
	}

	return pInstance;
}