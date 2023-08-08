#include "Export_Function.h"
#include "CUI.h"

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev, OBJ_TYPE::OBJ_UI, OBJ_ID::UI)
{
	ZeroMemory(&m_tInfo, sizeof(UI_INFO));
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
{
	m_tInfo = rhs.m_tInfo;
}

CUI::~CUI()
{
}

HRESULT CUI::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);


	m_pTransformCom = dynamic_cast<CTransform*>(Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(m_pTransformCom, E_FAIL);

	return S_OK;
}


_int CUI::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	CGameObject::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI::LateUpdate_Object(void)
{
	CGameObject::LateUpdate_Object();
}

void CUI::Render_Object(void)
{

	CGameObject::Render_Object();
}

void CUI::Debug_Input()
{
	if (KEY_TAP(KEY::UP_ARROW))
	{
		m_tInfo.fY -= 1.f;
	}

	if (KEY_TAP(KEY::DOWN_ARROW))
	{
		m_tInfo.fY += 1.f;
	}

	if (KEY_TAP(KEY::LEFT_ARROW))
	{
		m_tInfo.fX -= 1.f;
	}

	if (KEY_TAP(KEY::RIGHT_ARROW))
	{
		m_tInfo.fX += 1.f;
	}

	if (KEY_HOLD(KEY::UP_ARROW))
	{
		m_tInfo.fY -= 1.f;
	}

	if (KEY_HOLD(KEY::DOWN_ARROW))
	{
		m_tInfo.fY += 1.f;
	}

	if (KEY_HOLD(KEY::LEFT_ARROW))
	{
		m_tInfo.fX -= 1.f;
	}

	if (KEY_HOLD(KEY::RIGHT_ARROW))
	{
		m_tInfo.fX += 1.f;
	}
}



void CUI::Ready_TransWorld()
{
	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);


	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };

	m_pTransformCom->Set_Pos(&vPos);

	_vec3 vScale = _vec3(m_tInfo.fCX, m_tInfo.fCY, 0.f);

	m_pTransformCom->Set_Scale(vScale);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

}

_matrix CUI::Trans_Matrix(const UI_INFO& pUI)
{
	_matrix mat;

	D3DXMatrixIdentity(&mat);

	_vec3 vPos = { ((2 * (pUI.fX)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (pUI.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };

	memcpy(&mat.m[3][0], &vPos, sizeof(_vec3));


	mat._11 *= pUI.fCX;
	mat._22 *= pUI.fCY;


	return mat;

}

_vec3 CUI::Trans_WinPos(_vec3 origin)
{
	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	_vec3 vPos = { ((2 * (m_tInfo.fX)) / WINCX - 1) * (1 / m_matProj._11) , ((-2 * (m_tInfo.fY)) / WINCY + 1) * (1 / m_matProj._22), 0.f };

	return 	vPos;

}

void CUI::Free()
{
	CGameObject::Free();
}
