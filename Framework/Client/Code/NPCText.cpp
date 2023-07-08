#include "NPCText.h"
#include "Export_Function.h"
#include "../Include/stdafx.h"

CNPCText::CNPCText(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
{
}

CNPCText::CNPCText(const CNPCText& rhs)
	: CUI(rhs)
{
}

CNPCText::~CNPCText()
{
}

HRESULT CNPCText::Ready_Object(void)
{	
	// NameTag 추가
//	Engine::CLayer* pLayer = Engine::CLayer::Create();
//	NULL_CHECK_RETURN(pLayer, E_FAIL);
//
//	CUI_NameTag* pNameTag = CUI_NameTag::Create(m_pGraphicDev);
//	NULL_CHECK_RETURN(pNameTag, E_FAIL);
//	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"NPC_NameTag", pNameTag), E_FAIL);
	//

	CComponent* pComponent = nullptr;

	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_TextBox"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);
	
	return S_OK;
}

_int CNPCText::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	// Key_Input 만들어서 넣기 (대화창 닫히게끔)

	__super::Update_Object(fTimeDelta);

	return S_OK;
}

void CNPCText::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CNPCText::Render_Object(void)
{
	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

//	_vec3 vPos = { ((2 * (WINCX / 2)) / WINCX - 1) *  (1 / m_matProj._11) ,
//		((-2 * (WINCY / 2)) / WINCY + 1)  * (1 / m_matProj._22), 0.f };
	_vec3 vPos = { ((2 * (WINCX / 2)) / WINCX - 1) * (1 / m_matProj._11) ,
		((-2 * (WINCY / 2)) / WINCY + 0.5f) * (1 / m_matProj._22), 0.f };

	m_pTransformCom->Set_Pos(&vPos);

	_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
	_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);

	_float fRatio = _float(WINCY) / _float(WINCX);
//	_vec3 vScale = _vec3(fWidth * fRatio, fHeight * fRatio, 0.f);
	_vec3 vScale = _vec3(fWidth * fRatio * 0.8, fHeight * fRatio, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	m_pTextureCom->Render_Texture(0);
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);

////////////////
//	RECT rc = { 0, WINCY / 2, WINCX, WINCY };
//	//GetClientRect(g_hWnd, &rc);
//
//	TCHAR szBuf[256] = L"";
//	swprintf_s(szBuf, L"Hello Ogu World!");
//	CGraphicDev::GetInstance()->Get_Font()->DrawText(NULL,
//		szBuf, lstrlen(szBuf), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP, D3DCOLOR_ARGB(100, 255, 255, 255));
//	// NULL, 출력할 Text, Text길이(혹은 -1 -> 자동으로 문자열 길이 계산),
//	// 글꼴 출력 사각 영역, 출력 서식, 색상
////////////////

	__super::Render_Object();
}

CNPCText* CNPCText::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CNPCText* pInstance = new CNPCText(pGraphicDev);
	
	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("NPC TextBox Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CNPCText::Free()
{
}
