#include "Effect_NueTrail.h"


#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"

CEffect_NueTrail::CEffect_NueTrail(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)
{
}

CEffect_NueTrail::CEffect_NueTrail(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_NueTrail::~CEffect_NueTrail()
{
}

HRESULT CEffect_NueTrail::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_Active(true);



	m_vColor = { 1.0f, 1.0f, 1.0f, 1.0f };


	m_iFirstMax = 20;
	m_iSecondMax = 400;

	return S_OK;
}

_int CEffect_NueTrail::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &m_vOwnerPos);

	switch (m_eTrailState)
	{
	case NUETRAIL_STATE::TRAIL_START:
		Update_Start(fTimeDelta);
		break;
	case NUETRAIL_STATE::TRAIL_MOVE:
		Update_Move(fTimeDelta);
		break;
	case NUETRAIL_STATE::TRAIL_END:
		Update_End(fTimeDelta);
		break;
	}


	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_NueTrail::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	_uint iSize = m_deqSecondPoint.size();

	if (iSize > m_iSecondMax)
	{
		for (_int i = 0; i < iSize - m_iSecondMax; ++i)
		{
			m_deqSecondPoint.pop_back();
		}
	}



	__super::LateUpdate_Object();
}


void CEffect_NueTrail::Render_Object(void)
{
	if (!Is_Active())
		return;


	__super::Render_Object();



	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_Color", &m_vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(0);
	pEffect->SetTexture("g_Texture", pTexture);

	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	_int iFirstSize = m_deqFirstPoint.size() - 1;

	for (_int i = 0; i < iFirstSize; ++i)
	{
		if (i == iFirstSize - 1)
		{
			m_tTrailInfo.PointLeft = m_deqFirstPoint[i].vPos[0] + m_vOwnerPos;
			m_tTrailInfo.PointRight = m_deqFirstPoint[i].vPos[1] + m_vOwnerPos;

			if (m_deqSecondPoint.size() > 0)
			{
				m_tTrailInfo.NextLeft = m_deqSecondPoint[0].vPos[0];
				m_tTrailInfo.NextRight = m_deqSecondPoint[0].vPos[1];
			}
			else
			{
				m_tTrailInfo.NextLeft = m_deqFirstPoint[i + 1].vPos[0] + m_vOwnerPos;
				m_tTrailInfo.NextRight = m_deqFirstPoint[i + 1].vPos[1] + m_vOwnerPos;
			}

			m_tTrailInfo.fSize = (float)(iFirstSize);
			m_tTrailInfo.fIndex = (float)i;
			
		}
		else
		{
			m_tTrailInfo.PointLeft = m_deqFirstPoint[i].vPos[0] + m_vOwnerPos;
			m_tTrailInfo.PointRight = m_deqFirstPoint[i].vPos[1] + m_vOwnerPos;

			m_tTrailInfo.NextLeft = m_deqFirstPoint[i + 1].vPos[0] + m_vOwnerPos;
			m_tTrailInfo.NextRight = m_deqFirstPoint[i + 1].vPos[1] + m_vOwnerPos;

			m_tTrailInfo.fSize = (float)(iFirstSize);
			m_tTrailInfo.fIndex = (float)i;
		}
		

		pEffect->SetValue("g_Trail", &m_tTrailInfo, sizeof(TRAILINFO));

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(3);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}

	
	_int iSecondSize = m_deqSecondPoint.size() - 1;

	for (_int i = 0; i < iSecondSize; ++i)
	{
		m_tTrailInfo.PointLeft = m_deqSecondPoint[i].vPos[0];
		m_tTrailInfo.PointRight = m_deqSecondPoint[i].vPos[1];

		m_tTrailInfo.NextLeft = m_deqSecondPoint[i + 1].vPos[0];
		m_tTrailInfo.NextRight = m_deqSecondPoint[i + 1].vPos[1];

		m_tTrailInfo.fSize = (float)(iSecondSize);
		m_tTrailInfo.fIndex = (float)i;
	

		pEffect->SetValue("g_Trail", &m_tTrailInfo, sizeof(TRAILINFO));

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(3);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}

	

}

void CEffect_NueTrail::Return_Pool(void)
{
	CPool<CEffect_NueTrail>::Return_Obj(this);
}

CEffect_NueTrail* CEffect_NueTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_NueTrail* pInstance = new CEffect_NueTrail(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Trail Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_NueTrail::Set_Effect(CGameObject* _pOwner, _float _fScale, _vec3& _vFowardDir, _vec3& _vOffset, _vec3& _vUpOffset, _float _fAngleSpeed)
{
	m_pOwner = _pOwner;
	m_fScale = _fScale;
	m_vFowardDir = _vFowardDir;
	m_vFront[1] = _vOffset;
	m_vFront[2] = _vUpOffset;

	_vec3 vDir = m_vFront[1] - m_vFront[2];

	D3DXVec3Normalize(&vDir, &vDir);

	m_vFront[0] = m_vFront[1] + (vDir * 5.0f);
	m_vFront[3] = m_vFront[2] - (_vFowardDir * 5.0f);
	m_fAngleSpeed = _fAngleSpeed;

	m_eTrailState = NUETRAIL_STATE::TRAIL_START;


	m_deqFirstPoint.clear();
	m_deqSecondPoint.clear();

	m_vColor = { 1.0f, 1.0f, 1.0f, 0.7f };
}


void CEffect_NueTrail::Move_Start()
{
	m_vLinePoint[0] = m_vFront[2] + (m_vFowardDir * 2.0f) + m_vOwnerPos;
	m_vLinePoint[1] = m_vFront[2] + m_vOwnerPos;
	m_vLinePoint[2] = m_vFront[2] + m_vOwnerPos;
	m_vLinePoint[3] = m_vFront[3] + m_vOwnerPos;


	m_eTrailState = NUETRAIL_STATE::TRAIL_MOVE;
}

HRESULT CEffect_NueTrail::Add_Component(void)
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


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Effect_NueTrail"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);




	return S_OK;
}

void CEffect_NueTrail::Free()
{
	__super::Free();
}

void CEffect_NueTrail::Update_Start(const _float& fTimeDelta)
{
	_uint iSize = m_deqFirstPoint.size();

	if (iSize <= m_iFirstMax)
	{
		_vec3 vRightDir, vPos;
		_vec3 vUpDir = m_vFront[2] - m_vFront[1];
		VTXPoint vPoint;

		D3DXVec3Cross(&vRightDir, &vUpDir, &m_vFowardDir);
		D3DXVec3Normalize(&vRightDir, &vRightDir);
		D3DXVec3CatmullRom(&vPos, &m_vFront[0], &m_vFront[1], &m_vFront[2], &m_vFront[3], (_float)iSize / (_float)m_iFirstMax);

		vPoint.vPos[0] = vPos + (vRightDir * m_fScale);
		vPoint.vPos[1] = vPos - (vRightDir * m_fScale);
		vPoint.vPos[2] = vPos;

		m_deqFirstPoint.push_back(vPoint);
	}
	else
		Move_Start();
}

void CEffect_NueTrail::Update_Move(const _float& fTimeDelta)
{
	Rotate_Trail();

	m_vLinePoint[0] = m_vFront[2] + (m_vFowardDir * 2.0f) + m_vOwnerPos;
	m_vLinePoint[1] = m_vFront[2] + m_vOwnerPos;

	
	_float fLength = D3DXVec3Length(&(m_vLinePoint[2] - m_vLinePoint[1]));

	if (fLength > 0.5f)
	{
		_int iCount = fLength / 0.1f;
		_vec3 vPos;
		_float fS = 1.0f / (_float)iCount;

		for (_int i = 0; i < iCount; ++i)
		{
			D3DXVec3CatmullRom(&vPos, &m_vLinePoint[3], &m_vLinePoint[2], &m_vLinePoint[1], &m_vLinePoint[0], fS * i);

			VTXPoint tPoint;
			_vec3 vRight, vUpDir;

			vUpDir = vPos - (m_vFront[1] + m_vOwnerPos);
			D3DXVec3Cross(&vRight, &vUpDir, &m_vFowardDir);
			D3DXVec3Normalize(&vRight, &vRight);


			tPoint.vPos[0] = vPos + (vRight * m_fScale);
			tPoint.vPos[1] = vPos - (vRight * m_fScale);
			tPoint.vPos[2] = vPos;

			m_deqSecondPoint.push_front(tPoint);
		}


		m_vLinePoint[3] = m_vLinePoint[2];
		m_vLinePoint[2] = m_vLinePoint[1];
	}
	else if (fLength > 0.0f)
	{
		_vec3 vPos;

		D3DXVec3CatmullRom(&vPos, &m_vLinePoint[3], &m_vLinePoint[2], &m_vLinePoint[1], &m_vLinePoint[0], 1.0f);

		VTXPoint tPoint;
		_vec3 vRight, vUpDir;

		vUpDir = vPos - (m_vFront[1] + m_vOwnerPos);
		D3DXVec3Cross(&vRight, &vUpDir, &m_vFowardDir);
		D3DXVec3Normalize(&vRight, &vRight);


		tPoint.vPos[0] = vPos + (vRight * m_fScale);
		tPoint.vPos[1] = vPos - (vRight * m_fScale);
		tPoint.vPos[2] = vPos;

		m_deqSecondPoint.push_front(tPoint);

		m_vLinePoint[3] = m_vLinePoint[2];
		m_vLinePoint[2] = m_vLinePoint[1];
	}



	

	
}

void CEffect_NueTrail::Update_End(const _float& fTimeDelta)
{
	m_vColor.a -= 0.01f;
	if (m_vColor.a <= 0.0f)
	{
		m_vColor.a = 0.0f;
		Return_Pool();
	}
}

void CEffect_NueTrail::Rotate_Trail(void)
{
	_matrix matRot;
	D3DXMatrixRotationAxis(&matRot, &m_vFowardDir, m_fAngleSpeed);


	for (auto& iter : m_deqFirstPoint)
	{
		D3DXVec3TransformNormal(&iter.vPos[0], &iter.vPos[0], &matRot);
		D3DXVec3TransformNormal(&iter.vPos[1], &iter.vPos[1], &matRot);
		D3DXVec3TransformNormal(&iter.vPos[2], &iter.vPos[2], &matRot);
	}

	for (_int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformNormal(&m_vFront[i], &m_vFront[i], &matRot);
	}
}



