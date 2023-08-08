#pragma once
#include "GameObject.h"



BEGIN(Engine)

class CRcTex;
class CTransform;
class CCollider;

END

class CItem :
	public CGameObject
{

protected:
	explicit CItem(LPDIRECT3DDEVICE9 pGraphicDev, ITEM_TYPE _eItemType, OBJ_ID _eID);
	explicit CItem(const CItem& rhs);
	virtual ~CItem();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;
	virtual void	Render_UI();

	const ITEM_TYPE& Get_ItemType() { return m_eItemType; }
	const	_uint& Get_InvenCount() { return m_iInvenCount; }
	void	Set_InvenCount(_bool p_isPlus = true, _uint	p_iCount = 1)
	{
		if (p_isPlus)
			m_iInvenCount += p_iCount;
		else
		{
			if (m_iInvenCount < p_iCount)
				return;
			
			m_iInvenCount -= p_iCount;
		}
	}

	CLONE(CItem);

	virtual	CGameObject* Get_ByPool() { return nullptr; };
	virtual void				Add_Pool() {};

	static	wstring		Get_Explain(const ITEM_CODE& pCode);
	static	wstring		Get_Name(const ITEM_CODE& pCode);

	const ITEM_CODE& Get_ItemCode() { return m_eCode; }

	void		Set_UIPos(const _vec2& pV) 
	{
		m_tInfo.fX = pV.x; m_tInfo.fY = pV.y;
		m_tInfo.fCX = 80.f; m_tInfo.fCY = 80.f;
	}

	virtual HRESULT		Use_Item() { return S_OK; }

	_bool		Check_Clicked();

	void		Set_InInven() { m_bIsInInven = true; }

	CTexture* Get_Texture();

protected:
	virtual HRESULT	Add_Component(void);

protected:
	ITEM_TYPE m_eItemType;
	UI_INFO		m_tInfo;

	_bool		m_bIsInInven;

private:
	_uint		m_iInvenCount;

protected:
	virtual void Free() override;
	void Ready_TransWorld();
	ITEM_CODE	m_eCode;
};