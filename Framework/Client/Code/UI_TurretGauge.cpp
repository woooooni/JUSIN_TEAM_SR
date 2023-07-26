#include "UI_TurretGauge.h"
#include	"Export_Function.h"

CUI_TurretGauge::CUI_TurretGauge(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_TurretGauge::CUI_TurretGauge(const CUI_TurretGauge& rhs) : CUI(rhs)
{
}

CUI_TurretGauge::~CUI_TurretGauge()
{
}

HRESULT CUI_TurretGauge::Ready_Object(void)
{
	FAILED_CHECK(Add_Component());



	return S_OK;
}

_int CUI_TurretGauge::Update_Object(const _float& fTimeDelta)
{
	m_fEnergy = m_pOwner->Get_Energy();

	return __super::Update_Object(fTimeDelta);
}

void CUI_TurretGauge::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CUI_TurretGauge::Render_Object(void)
{
	_matrix	mat = *m_pTransformCom->Get_WorldMatrix();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pBack->Render_Texture();
	m_pBufferCom->Render_Buffer();

	mat._43 -= 0.001f;

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();


	mat._43 -= 0.001f;

	_vec3 right, up;

	memcpy(&right, &mat.m[0][0], sizeof(_vec3));
	memcpy(&up, &mat.m[1][0], sizeof(_vec3));

	right *= 0.9f * m_fEnergy * 0.01f;
	up *= 0.8f;
	memcpy(&mat.m[0][0], &right, sizeof(_vec3));
	memcpy(&mat.m[1][0], &up, sizeof(_vec3));

	mat._41 -= (0.5f - m_fEnergy * 0.01f * 0.5f) * 0.9f;


	m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
	m_pAnimator->Render_Component();
	m_pBufferCom->Render_Buffer();


}

HRESULT CUI_TurretGauge::Add_Component(void)
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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_HPFrame"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);



	pComponent = m_pBack = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Icon_HPBack"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);

	m_pAnimator->Add_Animation(L"Base", L"Proto_Texture_UI_HPGauge", 1.f);
	m_pAnimator->Play_Animation(L"Base", false);

	m_tInfo.fCX = m_pTextureCom->Get_TextureDesc(0).Width;
	m_tInfo.fCY = m_pTextureCom->Get_TextureDesc(0).Height;

	m_pTransformCom->Set_Scale({ 1, m_tInfo.fCY / m_tInfo.fCX, 1 });
	
	return S_OK;

}

CUI_TurretGauge* CUI_TurretGauge::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTurret* pOwner)
{
	NULL_CHECK_RETURN(pOwner, nullptr);

	CUI_TurretGauge* ret = new CUI_TurretGauge(pGraphicDev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("CUI_TurretGauge failed");
		return nullptr;
	}

	ret->m_pOwner = pOwner;
	_vec3 pos;
	pOwner->Get_TransformCom()->Get_Info(INFO_POS, &pos);
	pos.y += 1;
	ret->m_pTransformCom->Set_Pos(&pos);

	return ret;
}

void CUI_TurretGauge::Free()
{
	__super::Free();
}
