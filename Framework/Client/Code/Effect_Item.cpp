#include "Effect_Item.h"

#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"

CEffect_Item::CEffect_Item(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)
{
}

CEffect_Item::CEffect_Item(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_Item::~CEffect_Item()
{
}

HRESULT CEffect_Item::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"GetItem", L"Proto_Texture_Effect_GetItem", 0.1f);

	m_pAnimator->Play_Animation(L"GetItem", false);

	m_pTextureCom->Set_Idx(0);

	Set_Active(false);

	return S_OK;
}

_int CEffect_Item::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	_int iExit = __super::Update_Object(fTimeDelta);

	switch (m_iTurn)
	{
	case 0:
		m_vScale += _vec3(0.2f, 0.2f, 0.2f);
		if (m_vScale.x > 1.5f)
			m_iTurn = 1;
		break;
	case 1:
		if (m_vScale.x > 1.3f)
			m_vScale -= _vec3(0.05f, 0.05f, 0.05f);
		else
			m_iTurn = 2;
		break;
	case 2:
		m_pTransformCom->Move_Pos(&_vec3(0.0f, 1.0f, 0.0f), fTimeDelta, 0.5f);

		_vec3 vPos;
		m_pTransformCom->Get_Info(INFO_POS, &vPos);
		if (vPos.y > 3.0f)
		{
			if (m_vScale.x <= 0.01f)
			{
				CPool<CEffect_Item>::Return_Obj(this);
			}
			else
				m_vScale -= _vec3(0.1f, 0.1f, 0.1f);
		}
		
		break;
	}
	m_pTransformCom->Set_Scale(m_vScale);





	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	

	return iExit;
}

void CEffect_Item::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	__super::LateUpdate_Object();
}

void CEffect_Item::Render_Object(void)
{
	if (!Is_Active())
		return;

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	matWorld._11 *= 0.4f;
	matWorld._22 *= 0.4f;
	matWorld._33 *= 0.4f;
	matWorld._43 -= 0.005f;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);
	m_pTextureCom->Render_Texture();

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

CEffect_Item* CEffect_Item::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Item* pInstance = new CEffect_Item(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Item Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_Item::Get_Effect(_vec3& _vPos, ITEM_CODE _eItemCode)
{
	m_iTurn = 0;

	_vPos.y += 2.0f;

	_matrix matWorld;
	D3DXMatrixIdentity(&matWorld);
	for (_uint i = 0; INFO_END > i; ++i)
	{
		_vec3 vInfo;
		memcpy(&vInfo, &matWorld.m[i][0], sizeof(_vec3));
		m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
	}

	m_pTransformCom->Set_Scale(_vec3(0.001f, 0.001f, 0.001f));
	m_vScale = { 0.001f, 0.001f, 0.001f };

	m_pTransformCom->Set_Pos(&_vPos);
	Set_Active(true);

	m_pTextureCom->Set_Idx((_uint)_eItemCode);
	Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"ItemEffect", this);
}

HRESULT CEffect_Item::Add_Component(void)
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


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Items"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	return S_OK;
}

void CEffect_Item::Free()
{
	__super::Free();
}
