#include "Item_Hat.h"
#include "Export_Function.h"

CItem_Hat::CItem_Hat(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer)
    :CItem(pGraphicDev, ITEM_TYPE::EQUIPMENT, OBJ_ID::ITEM)
	, m_pPlayer(_pPlayer)
{
}

CItem_Hat::CItem_Hat(const CItem_Hat& rhs) : CItem(rhs), m_pPlayer(rhs.m_pPlayer)
{
	m_vecAnimation = rhs.m_vecAnimation;
	m_vecOffset = rhs.m_vecOffset;
	m_vScale = rhs.m_vScale;
	m_bLoop = rhs.m_bLoop;
	m_eSkill = rhs.m_eSkill;
}

CItem_Hat::~CItem_Hat()
{
}

void CItem_Hat::Free()
{
	__super::Free();
}

void CItem_Hat::Set_Hat(void)
{
	if (m_eDir != m_pPlayer->GetObj_Dir())
	{
		m_eDir = m_pPlayer->GetObj_Dir();
		m_pAnimator->Play_Animation(m_vecAnimation[(_uint)m_eDir],m_bLoop);
	}
}


HRESULT CItem_Hat::Ready_Object(void)
{
	FAILED_CHECK_RETURN(CItem_Hat::Add_Component(), E_FAIL);
	
	
	m_vecAnimation.resize((_uint)OBJ_DIR::DIR_END, L"");
	m_vecOffset.resize((_uint)OBJ_DIR::DIR_END, { 0.0f,0.0f,0.0f });

	m_eDir = OBJ_DIR::DIR_END;

	

    return S_OK;
}

_int CItem_Hat::Update_Object(const _float& fTimeDelta)
{
	Set_Hat();

	_int Result = __super::Update_Object(fTimeDelta);


	return Result;
}

void CItem_Hat::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CItem_Hat::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	
	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	
}

HRESULT CItem_Hat::Add_Component(void)
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
