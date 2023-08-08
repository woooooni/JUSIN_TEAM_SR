#include "Effect_Trail.h"


#include "Export_Function.h"
#include "Bullet.h"
#include "Collider.h"

#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"

CEffect_Trail::CEffect_Trail(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)
{
}

CEffect_Trail::CEffect_Trail(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_Trail::~CEffect_Trail()
{
}

HRESULT CEffect_Trail::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	Set_Active(true);

	m_fAccPushTime = 0.0f;
	m_fPushTime = 0.01f;

	m_iMaxSize = 30;


	m_vColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	m_fOffsetSpeed = 0.0f;

	m_fPlusTime = 0.0f;

	return S_OK;
}

_int CEffect_Trail::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;

	Engine::Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	m_fAccPushTime += fTimeDelta;
	if (m_fAccPushTime >= m_fPushTime)
	{
		if (!m_bEnd)
		{
			m_fPlusTime = fTimeDelta / 0.09f;
			m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &m_vPos);
			D3DXVec3CatmullRom(&m_vCurrOffset, &m_vOffset[0], &m_vOffset[1], &m_vOffset[2], &m_vOffset[3], m_fOffsetSpeed);
			m_vPos += m_vCurrOffset;
			m_fOffsetSpeed += m_fPlusTime;
			if (m_fOffsetSpeed > 1.0f)
				m_fOffsetSpeed = 1.0f;
		}
		VTXPoint tPoint;


		if (m_deqPoint.empty())
		{
			tPoint.vPos[0] = m_vPos + _vec3(m_fScale, 0.0f, 0.0f);
			tPoint.vPos[1] = m_vPos - _vec3(m_fScale, 0.0f, 0.0f);
			tPoint.vPos[2] = m_vPos;

			m_deqPoint.push_front(tPoint);
			m_bPush = true;
		}
		else if (m_vPos != m_deqPoint.front().vPos[2])
		{
			CGameObject* pCamera = Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera");

			_vec3 vCameraPos;
			pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vCameraPos);
			_vec3 vPrevPos = m_deqPoint.front().vPos[2];

			_vec3 vCamDir = vCameraPos - m_vPos;
			_vec3 vPrevDir = vPrevPos - m_vPos;

			_vec3 vDir;
			D3DXVec3Cross(&vDir, &vPrevDir, &vCamDir);
			D3DXVec3Normalize(&vDir, &vDir);


			tPoint.vPos[0] = m_vPos + (vDir * m_fScale);
			tPoint.vPos[1] = m_vPos + (vDir * -m_fScale);
			tPoint.vPos[2] = m_vPos;

			m_deqPoint.push_front(tPoint);
			m_bPush = true;
		}
		else if (m_vPos == m_deqPoint.front().vPos[2])
		{
			m_deqPoint.pop_back();
			m_bPush = true;

			if (m_bEnd)
			{
				m_vColor.a -= 0.02f;
				if (m_vColor.a < 0.0f)
					m_vColor.a = 0.0f;


				if (m_deqPoint.empty())
					Set_Active(false);
			
			}
				
		}
		else
			m_bPush = false;

		m_fAccPushTime = 0.0f;
	}
	else
		m_bPush = false;


	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CEffect_Trail::LateUpdate_Object(void)
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
		if (!m_bPush)
		{
			if (!m_bEnd)
			{
				m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &m_vPos);
				//D3DXVec3CatmullRom(&m_vCurrOffset, &m_vOffset[0], &m_vOffset[1], &m_vOffset[2], &m_vOffset[3], m_fOffsetSpeed);
				m_vPos += m_vCurrOffset;
				/*m_fOffsetSpeed += m_fPlusTime;
				if (m_fOffsetSpeed > 1.0f)
					m_fOffsetSpeed = 1.0f;*/

			}

			CGameObject* pCamera = Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera");

			_vec3 vCameraPos;
			pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vCameraPos);
			_vec3 vPrevPos = m_deqPoint.front().vPos[2];

			_vec3 vCamDir = vCameraPos - m_vPos;
			_vec3 vPrevDir = vPrevPos - m_vPos;

			_vec3 vDir;
			D3DXVec3Cross(&vDir, &vPrevDir, &vCamDir);
			D3DXVec3Normalize(&vDir, &vDir);


			m_deqPoint[0].vPos[0] = m_vPos + (vDir * m_fScale);
			m_deqPoint[0].vPos[1] = m_vPos + (vDir * -m_fScale);
			m_deqPoint[0].vPos[2] = m_vPos;
		}


		m_pTransformCom->Set_Info(INFO_POS, &m_vPos);
	}

	

	__super::LateUpdate_Object();
}


void CEffect_Trail::Render_Object(void)
{
	if (!Is_Active())
		return;

	if (m_deqPoint.empty())
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

void CEffect_Trail::Return_Pool(void)
{
	CPool<CEffect_Trail>::Return_Obj(this);
}

CEffect_Trail* CEffect_Trail::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_Trail* pInstance = new CEffect_Trail(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_Trail Create Failed");
		return nullptr;
	}

	return pInstance;
}



HRESULT CEffect_Trail::Add_Component(void)
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




	return S_OK;
}

void CEffect_Trail::Free()
{
	__super::Free();
}
