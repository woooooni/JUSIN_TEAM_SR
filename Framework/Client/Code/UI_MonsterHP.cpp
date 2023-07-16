#include "UI_MonsterHP.h"
#include "Export_Function.h"

// 몬스터를 때리면 나타난다.

CUI_MonsterHP::CUI_MonsterHP(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_MonsterHP::CUI_MonsterHP(const CUI_MonsterHP& rhs) : CUI(rhs)
{
}

CUI_MonsterHP::~CUI_MonsterHP()
{
}

HRESULT CUI_MonsterHP::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_vDefaultPos = { 0.f, 0.f, 0.f };

	return S_OK;
}

_int CUI_MonsterHP::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	// Player가 공격한 몬스터를 찾아 MaxHp와 Hp를 얻어온다.
	// 우선, Test용으로 RollingBug를 잡겠다.
	CGameObject* pMonster = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::MONSTER)->Find_GameObject(L"Monster_Rolling_Pink");
	
	if (pMonster != nullptr)
	{
		_uint iMaxHp = dynamic_cast<CMonster*>(pMonster)->Get_Stat().iMaxHp;
		_uint iHp = dynamic_cast<CMonster*>(pMonster)->Get_Stat().iHp;

		m_iMaxHP = iMaxHp;
		m_iHP = iHp;

		_vec3 vBarPos, vPos, vDir;
		pMonster->Get_TransformCom()->Get_Info(INFO_POS, &vPos);


		if ((m_iMaxHP != m_iHP) && (m_iHP != 0))
			m_bShown = true;
	}

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_MonsterHP::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_MonsterHP::Render_Object(void)
{

	__super::Render_Object();
}

HRESULT CUI_MonsterHP::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	switch (m_eUIType)
	{
	case MONSTERHP::UI_FRAME:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_HPFrame"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case MONSTERHP::UI_BACK:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_HPBack"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	case MONSTERHP::UI_GAUGE:
		pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_UI_HPGauge"));
		NULL_CHECK_RETURN(pComponent, E_FAIL);
		pComponent->SetOwner(this);
		m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
		break;

	default:
		break;
	}

	return S_OK;
}

void CUI_MonsterHP::Set_Type(MONSTERHP eType)
{
	m_eUIType = eType;
}

CUI_MonsterHP* CUI_MonsterHP::Create(LPDIRECT3DDEVICE9 pGraphicDev, MONSTERHP eType)
{
	CUI_MonsterHP* pInstance = new CUI_MonsterHP(pGraphicDev);

	pInstance->Set_Type(eType);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Monster HP Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_MonsterHP::Free()
{
}
