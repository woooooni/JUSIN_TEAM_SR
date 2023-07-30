#include "InventoryUI.h"
#include	"Export_Function.h"
#include		"InvenTabButton.h"
#include	"InventoryMgr.h"
#include "QuickSlot.h"
#include "UIMgr.h"

CInventoryUI::CInventoryUI(LPDIRECT3DDEVICE9 pGraphicDev)
	: CUI(pGraphicDev)
	, m_iCurPageIdx(0)
	, m_iCurItemIdx(0)
	, m_tCursorInfo({ 0.f, 0.f, 0.f,0.f })
	, m_tBtnInfo({ 0.f, 0.f, 0.f,0.f })
	, m_bIsRenderCurs(false)
{

}

CInventoryUI::CInventoryUI(const CInventoryUI& rhs)
	: CUI(rhs)
	, m_iCurPageIdx(rhs.m_iCurPageIdx)
	, m_iCurItemIdx(rhs.m_iCurItemIdx)
	, m_tCursorInfo(rhs.m_tCursorInfo)
	, m_bIsRenderCurs(rhs.m_bIsRenderCurs)
	, m_pCursurTex(rhs.m_pCursurTex)
	, m_pUseBtnTex(rhs.m_pUseBtnTex)
	, m_tBtnInfo(rhs.m_tBtnInfo)
{
}

CInventoryUI::~CInventoryUI()
{
}

HRESULT CInventoryUI::Ready_Object(void)
{
	FAILED_CHECK(__super::Ready_Object());

	CComponent* pCom = m_pTextureCom = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Frame"));
	NULL_CHECK_RETURN(pCom, E_FAIL);
	m_mapComponent[ID_STATIC].insert({ COMPONENT_TYPE::COM_TEXTURE, pCom });

	m_pCursurTex = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Cursor"));
	NULL_CHECK_RETURN(m_pCursurTex, E_FAIL);

	m_tCursorInfo = { 310.f, 310.f, 100.f, 100.f };
	m_tBtnInfo = { WINCX - 100.f, WINCY - 100.f, 150.f, 75.f };
	m_tExplainInfo = { WINCX * 0.5f + 250.f + 150.f, WINCY * 0.5f, 300.f, 500.f };

	m_pUseBtnTex = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Button"));
	m_pCursurTex = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Texture_Shop_Cursor"));

	return S_OK;
} 

_int CInventoryUI::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_UI, this);
	//Debug_Input();
	for (auto& iter : m_vecTabButton)
	{
		iter->Update_Object(fTimeDelta);
	}

	if (CKeyMgr::GetInstance()->GetKeyState(KEY::LBTN) == KEY_STATE::TAP)
	{
		if (m_bIsRenderCurs)
		{
			auto& pt = CKeyMgr::GetInstance()->GetMousePos();

			if (pt.x > m_tBtnInfo.fX - m_tBtnInfo.fCX * 0.5f && pt.x < m_tBtnInfo.fX + m_tBtnInfo.fCX * 0.5f
				&& pt.y > m_tBtnInfo.fY - m_tBtnInfo.fCY * 0.5f && pt.y < m_tBtnInfo.fY + m_tBtnInfo.fCY * 0.5f)
			{
				CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Use_Item();
				if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_InvenCount() <= 0)
				{
					CInventoryMgr::GetInstance()->Erase_Item((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx, m_iCurItemIdx);
					m_bIsRenderCurs = false;
				}

			}
		}

		for (_uint i = 0; i < CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx).size(); ++i)
		{
			if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[i]->Check_Clicked())
			{
				m_iCurItemIdx = i;
				m_tCursorInfo = { (i % 5) * 115.f + 310.f, (i / 5) * 115.f + 310.f , 100.f, 100.f };
				m_bIsRenderCurs = true;
				break;
			}
		}
	}

	if (CKeyMgr::GetInstance()->GetKeyState(KEY::ENTER) == KEY_STATE::TAP && m_bIsRenderCurs)
	{
		CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Use_Item();
		if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_InvenCount() <= 0)
		{
			CInventoryMgr::GetInstance()->Erase_Item((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx, m_iCurItemIdx);
			m_bIsRenderCurs = false;
		}
	}

	return __super::Update_Object(fTimeDelta);
}

void CInventoryUI::LateUpdate_Object(void)
{
	for (auto& iter : m_vecTabButton)
	{
		iter->LateUpdate_Object();
	}
	
	Register_QuickSlot();
	
	__super::LateUpdate_Object();
}

void CInventoryUI::Render_Object(void)
{
	Ready_TransWorld();
	m_pTextureCom->Render_Texture();
	m_pBufferCom->Render_Buffer();

	for (_uint i = 0; i < CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx).size(); ++i)
	{

		CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[i]->Set_UIPos({ (i % 5) * 115.f + 310.f, (i / 5) * 115.f + 310.f });
		CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[i]->Render_UI();
		auto iter = Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_BOLD);
		RECT rc = { (i % 5) * 115.f + 310.f + 30.f , (i / 5) * 115.f + 310.f + 30.f, (i % 5) * 115.f + 310.f + 40.f , (i / 5) * 115.f + 310.f + 40.f };
		wstring tmp = to_wstring(CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[i]->Get_InvenCount());

		if (tmp == L"1")
			continue;

		iter->DrawTextW(NULL, tmp.c_str(), tmp.length(), &rc, DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	if (m_bIsRenderCurs)
	{
		_matrix	 mat = Trans_Matrix(m_tCursorInfo);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
		m_pCursurTex->Render_Texture();
		m_pBufferCom->Render_Buffer();


		m_pGraphicDev->SetTransform(D3DTS_WORLD, &Trans_Matrix(m_tExplainInfo));
		m_pUseBtnTex->Render_Texture();
		m_pBufferCom->Render_Buffer();

		RECT rc = { WINCX * 0.5f + 250 + 30, WINCY * 0.5f - 150, WINCX * 0.5f + 250 + 270, WINCY * 0.5f + 150 };

		CItem* pItem = CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx];
		if (!pItem)
			return;

		ITEM_CODE code = CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode();

		if (code == ITEM_CODE::ITEM_END)
			return;
		wstring src = CItem::Get_Explain(code);

		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawTextW(NULL, src.c_str(), src.length(), &rc, DT_CENTER | DT_WORDBREAK, D3DCOLOR_ARGB(255, 255, 255, 255));


		

		rc = { WINCX - 100 - 50, WINCY - 100 - 50, WINCX - 100 + 50, WINCY - 100 + 50 };
		wstring tmp;

		switch (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemType())
		{
		case ITEM_TYPE::CONSUMPTION:
			tmp = L"사용하기";
			break;

		case ITEM_TYPE::EQUIPMENT:
			tmp = L"장착하기";
			break;

		case ITEM_TYPE::ETC:
		case ITEM_TYPE::IMPORTANT:
			return;

		default:
			break;
		}
		m_pGraphicDev->SetTransform(D3DTS_WORLD, &Trans_Matrix(m_tBtnInfo));
		m_pUseBtnTex->Render_Texture();
		m_pBufferCom->Render_Buffer();

		Engine::Get_Font(FONT_TYPE::CAFE24_SURROUND_AIR)->DrawTextW(NULL, tmp.c_str(), tmp.length(), &rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

}

CInventoryUI* CInventoryUI::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInventoryUI* ret = new CInventoryUI(pGraphicDev);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("InvenUI Create Failed");
		return nullptr;
	}

	ret->m_tInfo.fX = WINCX * 0.5f - 100.f;
	ret->m_tInfo.fY = WINCY * 0.5f;
	ret->m_tInfo.fCX = 624.f;
	ret->m_tInfo.fCY = 400.f;


	for (_uint i = (_uint)ITEM_TYPE::EQUIPMENT; i < (_uint)ITEM_TYPE::COIN; i++)
	{
		CInvenTabButton* inven = CInvenTabButton::Create(pGraphicDev, ret, { i * 60.f + 270.f, 195.f, 1.f });
		inven->Get_TextureCom()->Set_Idx(i);
		ret->m_vecTabButton.push_back(inven);

	}

	return ret;
}

void CInventoryUI::Set_ButClicked(CInvenTabButton* pInv)
{
	for (_uint i = 0; i < m_vecTabButton.size(); i++)
	{
		if (pInv == m_vecTabButton[i])
		{
			m_iCurPageIdx = i;
			break;
		}
	}
	m_iCurItemIdx = 0;
	m_bIsRenderCurs = false;

}

void CInventoryUI::Register_QuickSlot()
{
	CQuickSlot* pSlots = CUIMgr::GetInstance()->Get_Slots();

	if (m_bIsRenderCurs)
	{
		if (m_iCurPageIdx == (_uint)CInventoryMgr::INVENTORY_TYPE::CONSUMPSION)
		{
			_bool bFilled_One = pSlots->Get_Filled(SLOTNUM::SLOT_ONE);
			_bool bFilled_Two = pSlots->Get_Filled(SLOTNUM::SLOT_TWO);
			_bool bFilled_Three = pSlots->Get_Filled(SLOTNUM::SLOT_THREE);
			_bool bFilled_Four = pSlots->Get_Filled(SLOTNUM::SLOT_FOUR);

			if (KEY_TAP(KEY::NUM_1))
			{
				if (!bFilled_One)
				{
					// 슬롯 2, 3, 4가 차있는지 확인하고 차있는 경우 아이템 코드를 받는다.
					// 이 아이템 코드와 지금 넣으려고하는 아이템이 같은것이면 넣지 않는다.

					if (bFilled_Two)
					{
						// 3번과 4번에도 같은 아이템이 있지는 않은지 확인해줘야한다.
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_TWO))
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_THREE))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_FOUR)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_ONE, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (bFilled_Three)
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_THREE))
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_TWO))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_FOUR)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_ONE, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (bFilled_Four)
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_FOUR))
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_TWO))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_THREE)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_ONE, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (!bFilled_Two && !bFilled_Three && !bFilled_Four) // 셋다 false면 아이템을 추가한다.
						pSlots->Set_Item(SLOTNUM::SLOT_ONE, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
				}

				//_bool bCanUse = pSlots->Get_Use(); // 230729 추가(수정 진행중)

				if (bFilled_One) // 여기에 조건 하나 더 추가해주어야함. 아이템창이 열려있는 상태여야한다. m_bCanUse 호출
				{
					if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
						== pSlots->Get_ItemCode(SLOTNUM::SLOT_ONE))
					{
						// 1Key에 이미 배정된 아이템을 선택한 상태에서 동일 키를 누를 경우 배치가 해제됨.
						pSlots->Set_Item(SLOTNUM::SLOT_ONE, m_iCurItemIdx, ITEM_CODE::ITEM_END);
						pSlots->Set_Filled(SLOTNUM::SLOT_ONE, false);
					}
				}
			}

			if (KEY_TAP(KEY::NUM_2))
			{
				if (!bFilled_Two) // 슬롯이 차지 않으면. 즉, false를 반환하면 OK
				{
					if (bFilled_One) // 슬롯1이 차있으면
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_ONE)) // 슬롯1의 아이템 코드를 얻어오고 지금 담을 아이템 코드를 얻어와 비교한다 -> 같지 않으면 슬롯에 추가한다
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_THREE))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_FOUR)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_TWO, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (bFilled_Three) // 슬롯3이 차있으면
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_THREE)) // 슬롯 3의 아이템 코드를 얻어오고 지금 담을 아이템 코드를 얻어와 비교한다 ->같지 않으면
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_ONE))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_FOUR)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_TWO, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (bFilled_Four)
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_FOUR))
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_ONE))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_THREE)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_TWO, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (!bFilled_One && !bFilled_Three && !bFilled_Four)
						pSlots->Set_Item(SLOTNUM::SLOT_TWO, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
				}

				if (bFilled_Two)
				{
					if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
						== pSlots->Get_ItemCode(SLOTNUM::SLOT_TWO))
					{
						pSlots->Set_Item(SLOTNUM::SLOT_TWO, m_iCurItemIdx, ITEM_CODE::ITEM_END);
						pSlots->Set_Filled(SLOTNUM::SLOT_TWO, false);
					}
				}
			}

			if (KEY_TAP(KEY::NUM_3))
			{
				if (!bFilled_Three)
				{
					if (bFilled_One)
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_ONE))
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_TWO))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_FOUR)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_THREE, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (bFilled_Two)
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_TWO))
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_ONE))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_FOUR)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_THREE, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (bFilled_Four)
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_FOUR))
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_ONE))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_TWO)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_THREE, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (!bFilled_One && !bFilled_Two && !bFilled_Four)
						pSlots->Set_Item(SLOTNUM::SLOT_THREE, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
				}

				if (bFilled_Three)
				{
					if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
						== pSlots->Get_ItemCode(SLOTNUM::SLOT_THREE))
					{
						pSlots->Set_Item(SLOTNUM::SLOT_THREE, m_iCurItemIdx, ITEM_CODE::ITEM_END);
						pSlots->Set_Filled(SLOTNUM::SLOT_THREE, false);
					}
				}
			}

			if (KEY_TAP(KEY::NUM_4))
			{
				if (!bFilled_Four)
				{
					if (bFilled_One)
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_ONE))
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_TWO))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_THREE)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_FOUR, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (bFilled_Two)
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_TWO))
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_ONE))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_THREE)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_FOUR, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (bFilled_Three)
					{
						if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
							!= pSlots->Get_ItemCode(SLOTNUM::SLOT_THREE))
						{
							if ((CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
								!= pSlots->Get_ItemCode(SLOTNUM::SLOT_ONE))
								&& (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
									!= pSlots->Get_ItemCode(SLOTNUM::SLOT_TWO)))
							{
								pSlots->Set_Item(SLOTNUM::SLOT_FOUR, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
							}
						}
					}

					if (!bFilled_One && !bFilled_Two && !bFilled_Three)
						pSlots->Set_Item(SLOTNUM::SLOT_FOUR, m_iCurItemIdx, CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode());
				}

				if (bFilled_Four)
				{
					if (CInventoryMgr::GetInstance()->Get_Inventory((CInventoryMgr::INVENTORY_TYPE)m_iCurPageIdx)[m_iCurItemIdx]->Get_ItemCode()
						== pSlots->Get_ItemCode(SLOTNUM::SLOT_FOUR))
					{
						pSlots->Set_Item(SLOTNUM::SLOT_FOUR, m_iCurItemIdx, ITEM_CODE::ITEM_END);
						pSlots->Set_Filled(SLOTNUM::SLOT_FOUR, false);
					}
				}
			}
		}
	}
}

void CInventoryUI::Free()
{
	for (auto& iter : m_vecTabButton)
	{
		iter->Free();
	}
	m_vecTabButton.clear();
	__super::Free();
}
