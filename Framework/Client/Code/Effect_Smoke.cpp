#include "Effect_Smoke.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"

CEffect_Smoke::CEffect_Smoke(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)

{
}

CEffect_Smoke::CEffect_Smoke(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_Smoke::~CEffect_Smoke()
{
}

HRESULT CEffect_Smoke::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"Smoke", L"Proto_Texture_Effect_Smoke", 0.05f);

	m_pAnimator->Play_Animation(L"Smoke", true);

	Set_Active(false);

	return S_OK;
}

_int CEffect_Smoke::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	if (m_fScale < 1.0f)
	{
		m_fScale += 0.08f;
	}
	else
		m_fScale = 1.0f;

	m_pTransformCom->Move_Pos(&m_vDir, 0.3f, fTimeDelta);


	m_pTransformCom->Set_Scale(m_vScale * m_fScale);

	if (m_fAccTime > m_fEffectTime)
	{
		m_iAlpha -= 1;
		if (m_iAlpha < 10)
		{
			m_iAlpha = 0;
			Set_Active(false);
			CPool<CEffect_Smoke>::Return_Obj(this);
		}
	}
	else
	{
		m_fAccTime += fTimeDelta;
		if (m_iAlpha < 180)
		{
			m_iAlpha += 1;
		}
		else
			m_iAlpha = 180;
	}


	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_Smoke::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;


	__super::LateUpdate_Object();
}

void CEffect_Smoke::Render_Object(void)
{
	if (!Is_Active())
		return;

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, m_iR, m_iG, m_iB));

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CEffect_Smoke* CEffect_Smoke::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Smoke* pInstance = new CEffect_Smoke(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Smoke Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_Smoke::Get_Effect(_vec3& _vPos, _vec3& _vScale, _uint _iR, _uint _iG, _uint _iB)
{
	_vPos.z -= 0.01f;
	m_pTransformCom->Set_Pos(&_vPos);
	m_vScale = _vScale;
	m_fAccTime = 0.0f;
	m_fEffectTime = 0.5f;
	m_iAlpha = 70;
	m_iR = _iR;
	m_iG = _iG;
	m_iB = _iB;

	m_fScale = 0.5f;

	

	_float fX = (rand() % 11) * 0.1f;
	_float fY = (rand() % 11 + 1) * 0.1f;

	_uint i = rand() % 2;

	if (i == 0)
		fX *= -1.0f;

	m_vDir = { fX, fY, 0.0f };

	D3DXVec3Normalize(&m_vDir, &m_vDir);



	Set_Active(true);
	Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"SmokeEffect", this);

}

HRESULT CEffect_Smoke::Add_Component(void)
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

void CEffect_Smoke::Free()
{
	__super::Free();
}
