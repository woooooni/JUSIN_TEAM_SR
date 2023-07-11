#pragma once
#include "Item.h"
#include "Transform.h"

class CItem_Hat : public CItem
{
	CLONE(CItem_Hat)

protected:
	explicit CItem_Hat(LPDIRECT3DDEVICE9 pGraphicDev, CGameObject* _pPlayer);
	explicit CItem_Hat(const CItem_Hat& rhs);
	virtual ~CItem_Hat();

public:
	virtual HRESULT Ready_Object(void)							override;
	virtual _int	Update_Object(const _float& fTimeDelta)		override;
	virtual void	LateUpdate_Object(void)						override;
	virtual void	Render_Object(void)							override;

	PLAYER_SKILL	Get_Skill() { return m_eSkill; }


	void			Set_Offset(OBJ_DIR _eDir ,_vec3& _vOffset) {m_vecOffset[(_uint)_eDir] = _vOffset; }
	void			Set_Pos(_vec3& _vPos)
	{
		m_pTransformCom->Set_Pos(&(_vPos + m_vecOffset[(_uint)m_pPlayer->GetObj_Dir()])); 
	}

	void			Set_Scale(float _fScale) { m_pTransformCom->Set_Scale(m_vScale * _fScale); }

	void			Reset()
	{
		_matrix matWorld;
		D3DXMatrixIdentity(&matWorld);

		for (_uint i = 0; INFO_END > i; ++i)
		{
			_vec3 vInfo;
			memcpy(&vInfo, &matWorld.m[i][0], sizeof(_vec3));
			m_pTransformCom->Set_Info((MATRIX_INFO)i, &vInfo);
		}
	}

	void			Set_Angle(float _fAngle) { m_pTransformCom->RotationAxis(_vec3(0.0f, 0.0f, 1.0f), _fAngle); }
protected:
	HRESULT	Add_Component(void);



protected:
	virtual void Free() override;


	vector<wstring> m_vecAnimation;
	vector<_vec3> m_vecOffset;
	CGameObject* m_pPlayer;
	_vec3		m_vScale;

	bool	m_bLoop = false;

	PLAYER_SKILL m_eSkill;
private:
	void	Set_Hat(void);
};

