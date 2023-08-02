#include "Effect_EyeTrail.h"


#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"

CEffect_EyeTrail::CEffect_EyeTrail(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)
{
}

CEffect_EyeTrail::CEffect_EyeTrail(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_EyeTrail::~CEffect_EyeTrail()
{
}

HRESULT CEffect_EyeTrail::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_Active(true);


	m_fAccPushTime = 0.0f;
	m_fPushTime = 0.01f;

	m_iMaxSize = 30;

	m_iAlpha = 0;

	m_vColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	//m_tMaterial.Emissive = { 0.5f, 0.5f, 0.5f, 0.0f };

	return S_OK;
}

_int CEffect_EyeTrail::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	if (m_bStart)
	{
		m_iEyeAlpha += 10;
		if (m_iEyeAlpha >= 255)
		{
			m_iEyeAlpha = 255;
			m_bStart = false;

		}
		_vec3 vPos;
		m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		vPos += m_vOffset;
		m_pTransformCom->Set_Info(INFO_POS, &vPos);
	}
	else
	{
		m_fAccPushTime += fTimeDelta;
		if (m_fAccPushTime >= m_fPushTime)
		{
			if (!m_bEnd)
			{
				m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &m_vPos);
				m_vPos += m_vOffset;
			}
			VTXPoint tPoint;


			if (m_deqPoint.empty())
			{
				tPoint.vPos[0] = m_vPos + _vec3(m_fScale, 0.0f, 0.0f);
				tPoint.vPos[1] = m_vPos - _vec3(m_fScale, 0.0f, 0.0f);
				tPoint.vPos[2] = m_vPos;
				tPoint.vPos[0].z += 0.01f;
				tPoint.vPos[1].z += 0.01f;
				tPoint.vPos[2].z += 0.01f;

				m_deqPoint.push_front(tPoint);
				m_bPush = true;
			}
			else if (m_vPos != m_deqPoint.front().vPos[2])
			{
				_vec3 vPrevPos = m_deqPoint.front().vPos[2];

				_vec3 vCrossDir = { 0.0f, 0.0f, -1.0f };
				_vec3 vPrevDir = vPrevPos - m_vPos;

				_vec3 vDir;
				D3DXVec3Cross(&vDir, &vPrevDir, &vCrossDir);
				D3DXVec3Normalize(&vDir, &vDir);


				tPoint.vPos[0] = m_vPos + (vDir * m_fScale);
				tPoint.vPos[1] = m_vPos + (vDir * -m_fScale);
				tPoint.vPos[2] = m_vPos;
				tPoint.vPos[0].z += 0.01f;
				tPoint.vPos[1].z += 0.01f;
				tPoint.vPos[2].z += 0.01f;

				m_deqPoint.push_front(tPoint);
				m_bPush = true;
			}
			else if (m_vPos == m_deqPoint.front().vPos[2])
			{
				m_deqPoint.pop_back();
				m_bPush = true;
			}
			else
				m_bPush = false;

			m_fAccPushTime = 0.0f;
		}
		else
			m_bPush = false;


		if (m_bEnd)
		{
			m_vColor.a -= 0.02f;
			if (m_vColor.a < 0.0f)
				m_vColor.a = 0.0f;

			m_iEyeAlpha -= 1;


			if (m_iEyeAlpha <= 0)
			{
				m_iEyeAlpha = 0;
				Set_Active(false);
			}
		}
	}

	

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_EyeTrail::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	if (m_deqPoint.size() > m_iMaxSize)
	{
		for (_uint i = 0; i < m_deqPoint.size() - m_iMaxSize; ++i)
		{
			m_deqPoint.pop_back();
		}
	}

	if (!m_deqPoint.empty())
	{
		m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &m_vPos);
		m_vPos += m_vOffset;

		if (!m_bPush)
		{
			_vec3 vPrevPos = m_deqPoint.front().vPos[2];

			_vec3 vCrossDir = { 0.0f, 0.0, -1.0f };
			_vec3 vPrevDir = vPrevPos - m_vPos;

			_vec3 vDir;
			D3DXVec3Cross(&vDir, &vPrevDir, &vCrossDir);
			D3DXVec3Normalize(&vDir, &vDir);

			m_deqPoint[0].vPos[0] = m_vPos + (vDir * m_fScale);
			m_deqPoint[0].vPos[1] = m_vPos + (vDir * -m_fScale);
			m_deqPoint[0].vPos[2] = m_vPos;
			m_deqPoint[0].vPos[0].z += 0.01f;
			m_deqPoint[0].vPos[1].z += 0.01f;
			m_deqPoint[0].vPos[2].z += 0.01f;
		}

		m_pTransformCom->Set_Info(INFO_POS, &m_vPos);
	}

	__super::LateUpdate_Object();
}


void CEffect_EyeTrail::Render_Object(void)
{
	if (!Is_Active())
		return;

	__super::Render_Object();

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();
	IDirect3DBaseTexture9* pTexture = nullptr;

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;



	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());

	if (!m_deqPoint.empty())
	{
		pEffect->SetValue("g_Color", &m_vColor, sizeof(D3DCOLORVALUE));

		pTexture = m_pTextureCom->Get_Texture(0);
		pEffect->SetTexture("g_Texture", pTexture);


		for (_uint i = 0; i < m_deqPoint.size() - 1; ++i)
		{
			m_tTrailInfo.PointLeft = m_deqPoint[i].vPos[0];
			m_tTrailInfo.PointRight = m_deqPoint[i].vPos[1];

			m_tTrailInfo.NextLeft = m_deqPoint[i + 1].vPos[0];
			m_tTrailInfo.NextRight = m_deqPoint[i + 1].vPos[1];

			m_tTrailInfo.fSize = (float)(m_deqPoint.size() - 1);
			m_tTrailInfo.fIndex = (float)i;

			pEffect->SetValue("g_Trail", &m_tTrailInfo, sizeof(TRAILINFO));

			pEffect->Begin(nullptr, 0);
			pEffect->BeginPass(3);

			m_pBufferCom->Render_Buffer();

			pEffect->EndPass();
			pEffect->End();
		}

	}


	//Eye
	


	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;


	D3DCOLORVALUE vColor = m_vColor;
	vColor.a = m_iEyeAlpha / 255.0f;


	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 0.0f);
	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));



	pTexture = m_pEyeTex->Get_Texture(0);
	pEffect->SetTexture("g_Texture", pTexture);

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(2);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();




	
}

void CEffect_EyeTrail::Return_Pool(void)
{
	CPool<CEffect_EyeTrail>::Return_Obj(this);
}

CEffect_EyeTrail* CEffect_EyeTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_EyeTrail* pInstance = new CEffect_EyeTrail(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Trail Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_EyeTrail::Set_Effect(CGameObject* _pOwner, _vec3& _vOffset, _float _fScale)
{
	m_pOwner = _pOwner;
	m_vOffset = _vOffset;
	m_fScale = _fScale * 0.4f;
	m_bStart = true;
	m_iAlpha = 0;

	m_pTransformCom->Set_Scale(_vec3(_fScale, _fScale, 0.1f));
}



HRESULT CEffect_EyeTrail::Add_Component(void)
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


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Effect_Trail"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pShader = dynamic_cast<CShader*>(Engine::Clone_Proto(L"Proto_Shader"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_SHADER, pComponent);



	pComponent = m_pEyeTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Texture_Effect_Eye"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);
	

	return S_OK;
}

void CEffect_EyeTrail::Free()
{
	__super::Free();
}
