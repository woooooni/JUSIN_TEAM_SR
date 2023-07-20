#include "UI_MonsterHP.h"
#include "Export_Function.h"
#include "Monster.h"
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

	m_tInfo.fCX = m_pTextureCom->Get_TextureDesc(0).Width;
	m_tInfo.fCY = m_pTextureCom->Get_TextureDesc(0).Height;

	return S_OK;
}

_int CUI_MonsterHP::Update_Object(const _float& fTimeDelta)
{
	
	if (m_pOwner != nullptr && m_pOwner->Is_Active())
	{ 
		_uint iMaxHp = (m_pOwner)->Get_Stat().iMaxHp;
		_uint iHp = (m_pOwner)->Get_Stat().iHp;

		m_iMaxHP = iMaxHp;
		m_iHP = iHp;

		if ((m_iMaxHP != m_iHP) && (m_iHP != 0))
			Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);
	}

	else
		Set_Active(false);

	_int iExit = __super::Update_Object(fTimeDelta);
	return iExit;
}

void CUI_MonsterHP::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_MonsterHP::Render_Object(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

	_vec3 vScale = _vec3(m_tInfo.fCX * 0.004f, m_tInfo.fCY * 0.008f, 0.f);
	Get_TransformCom()->Set_Scale(vScale);

	if ((m_iMaxHP != m_iHP) && (m_iHP != 0))
	{
		//m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

//		if (m_eUIType == MONSTERHP::UI_FRAME ||
//			m_eUIType == MONSTERHP::UI_BACK ||
//			(m_eUIType == MONSTERHP::UI_GAUGE && m_iMaxHP == m_iHP))
//		{
//			_vec3 vScale = _vec3(m_tInfo.fCX * 0.004f, m_tInfo.fCY * 0.008f, 0.f);
//			Get_TransformCom()->Set_Scale(vScale);
//		}

		if ((m_eUIType == MONSTERHP::UI_GAUGE) &&
			(m_iMaxHP > m_iHP))
		{
			_float fMaxHP = _float(m_iMaxHP);
			_float fCurHP = _float(m_iHP);
			_float fHP = fCurHP / fMaxHP;

			_float fOriginWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
			_float fWidth = fOriginWidth * fHP;
			_float fX = fOriginWidth - fWidth;

			_vec3 vReScale = _vec3(fWidth, m_tInfo.fCY, 0.f);
			vReScale = _vec3(vReScale.x * 0.004f, vReScale.y * 0.008f, 0.f);

			m_pTransformCom->Set_Scale(vReScale);
		}
//		__super::Render_Object();

		m_pTextureCom->Render_Texture(0);
		m_pBufferCom->Render_Buffer();
	}
}

HRESULT CUI_MonsterHP::Add_Component(void)
{
	CComponent* pComponent = nullptr;

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
