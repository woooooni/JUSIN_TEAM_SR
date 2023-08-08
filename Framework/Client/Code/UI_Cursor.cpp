#include "UI_Cursor.h"
#include "Export_Function.h"
#include "UIMgr.h"

CUI_Cursor::CUI_Cursor(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev)
{
}

CUI_Cursor::CUI_Cursor(const CUI_Cursor& rhs) : CUI(rhs)
{
}

CUI_Cursor::~CUI_Cursor()
{
}

HRESULT CUI_Cursor::Ready_Object(void)
{
	D3DXMatrixOrthoLH(&m_matProj, WINCX, WINCY, 0, 1);
	D3DXMatrixIdentity(&m_matView);

	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	
	m_tInfo.fX = WINCX / 2 - 490.f;
	m_tInfo.fY = WINCY / 2 - 83.f;

	m_tInfo.fCX = m_pTextureCom->Get_TextureDesc(0).Width;
	m_tInfo.fCY = m_pTextureCom->Get_TextureDesc(0).Height;

	m_vDefaultPos = { (2 * m_tInfo.fX / WINCX - 1) * (1 / m_matProj._11) ,
					(-2 * m_tInfo.fY / WINCY + 1) * (1 / m_matProj._22), 0.f };

	return S_OK;
}

_int CUI_Cursor::Update_Object(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDERID::RENDER_UI, this);

	Key_Input();

	if (!m_bMoveCursor)
		m_bMoveCursor = true;

	__super::Update_Object(fTimeDelta);
	return S_OK;
}

void CUI_Cursor::LateUpdate_Object(void)
{
	if (!m_bShown)
	{
		m_pTransformCom->Set_Pos(&m_vDefaultPos);
		m_iCursorX = 0;
		m_iCursorY = 0;
	}
	__super::LateUpdate_Object();
}

void CUI_Cursor::Render_Object(void)
{
	_matrix matPreView, matPreProj;

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matPreView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matPreProj);

	_vec3 vOriginPos, vMovePos;
	Get_TransformCom()->Get_Info(MATRIX_INFO::INFO_POS, &vOriginPos);

	if (m_bShown && m_bMoveCursor)
	{
		if (KEY_TAP(KEY::LEFT_ARROW) && m_iCursorX > 0)
		{
			m_iCursorX--;
			vMovePos = { vOriginPos.x - 154.f, vOriginPos.y, 0.f };
			Get_TransformCom()->Set_Pos(&vMovePos);
			vOriginPos = vMovePos;
			m_bMoveCursor = false;
		}

		else if (KEY_TAP(KEY::RIGHT_ARROW) && m_iCursorX < 4)
		{
			m_iCursorX++;
			vMovePos = { vOriginPos.x + 154.f, vOriginPos.y, 0.f };
			Get_TransformCom()->Set_Pos(&vMovePos);
			vOriginPos = vMovePos;
			m_bMoveCursor = false;
		}

		else if (KEY_TAP(KEY::DOWN_ARROW) && m_iCursorY < 2)
		{
			m_iCursorY++;
			vMovePos = { vOriginPos.x, vOriginPos.y - 157.f, 0.f };
			Get_TransformCom()->Set_Pos(&vMovePos);
			vOriginPos = vMovePos;
			m_bMoveCursor = false;
		}

		else if (KEY_TAP(KEY::UP_ARROW) && m_iCursorY > 0)
		{
			m_iCursorY--;
			vMovePos = { vOriginPos.x, vOriginPos.y + 157.f, 0.f };
			Get_TransformCom()->Set_Pos(&vMovePos);
			vOriginPos = vMovePos;
			m_bMoveCursor = false;
		}

		_float fWidth = _float(m_pTextureCom->Get_TextureDesc(0).Width);
		_float fHeight = _float(m_pTextureCom->Get_TextureDesc(0).Height);
		_float fRatio = _float(WINCY) / _float(WINCX);
		_vec3 vScale = _vec3(fWidth * fRatio * 1.5f, fHeight * fRatio * 1.5f, 0.f);

		m_pTransformCom->Set_Scale(vScale);
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		__super::Render_Object();

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetTransform(D3DTS_VIEW, &matPreView);
		m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matPreProj);
	}
}

HRESULT CUI_Cursor::Ready_Component()
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Clone_Proto(L"Proto_RcTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_BUFFER, pComponent);

	pComponent = m_pTransformCom = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Proto_Transform"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TRANSFORM, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Cursor"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);

	FAILED_CHECK_RETURN(m_pAnimator->Add_Animation(L"UI_Shop_Cursor", L"Proto_Texture_Shop_Cursor", 0.5f), E_FAIL);
	FAILED_CHECK_RETURN(m_pAnimator->Play_Animation(L"UI_Shop_Cursor", TRUE), E_FAIL);

	return S_OK;
}

void CUI_Cursor::Key_Input()
{
	if (KEY_TAP(KEY::LEFT_ARROW) || KEY_TAP(KEY::RIGHT_ARROW) ||
		KEY_TAP(KEY::DOWN_ARROW) || KEY_TAP(KEY::UP_ARROW))
	{
		Stop_Sound(CHANNELID::SOUND_UI);
		Play_Sound(L"SFX_69_UISlotBigMove.wav", CHANNELID::SOUND_UI, 0.5f);

//		_float fRandom = (_float)(rand() % 3);
//		if (fRandom = 0)
//			Play_Sound(L"SFX_69_UISlotBigMove.wav", CHANNELID::SOUND_UI, 0.5f);
//		else if (fRandom = 1)
//			Play_Sound(L"SFX_70_UISlotMediumMove.wav", CHANNELID::SOUND_UI, 0.5f);
//		else
//			Play_Sound(L"SFX_71_UISlotSmallMove.wav", CHANNELID::SOUND_UI, 0.5f);
	}
}

CUI_Cursor* CUI_Cursor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUI_Cursor* pInstance = new CUI_Cursor(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("UI Cursor Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CUI_Cursor::Free()
{
}
