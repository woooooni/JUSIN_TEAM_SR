#include "LightPuzzleBase.h"
#include "Export_Function.h"

CLightPuzzleBase::CLightPuzzleBase(LPDIRECT3DDEVICE9 pGraphicDev) : CLightPuzzle(pGraphicDev), m_bMakeLight(false)
{
	
}

CLightPuzzleBase::CLightPuzzleBase(const CLightPuzzleBase& rhs) : CLightPuzzle(rhs), m_bMakeLight(rhs.m_bMakeLight)
{
}

CLightPuzzleBase::~CLightPuzzleBase()
{
}

HRESULT CLightPuzzleBase::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_MoonPuzzleBaseTile", 0.1f);
	m_pAnimator->Add_Animation(L"Vertical", L"Proto_Tex_MoonPuzzleBaseTile_Vert", 0.1f);
	m_pAnimator->Add_Animation(L"Corner", L"Proto_Tex_MoonPuzzleBaseTileCorner", 0.1f);
	m_pAnimator->Add_Animation(L"Three", L"Proto_Tex_MoonPuzzleBaseTile_Three", 0.1f);
	m_pAnimator->Add_Animation(L"Three_Up", L"Proto_Tex_MoonPuzzleBaseTile_Three_Up", 0.1f);

	FAILED_CHECK( m_pAnimator->Play_Animation(L"Base", false));

	return S_OK;
}

_int CLightPuzzleBase::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);

	return __super::Update_Object(fTimeDelta);
}

void CLightPuzzleBase::LateUpdate_Object(void)
{
}

void CLightPuzzleBase::Render_Object(void)
{
	_matrix mat = *m_pTransformCom->Get_WorldMatrix();

	
	__super::Render_Object();
	if (m_bIsLighting)
	{
		mat._42 += 0.01f;

		LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

		CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
		if (pCamera == nullptr)
			return;

		_vec3 vPos;
		pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		D3DVECTOR vCamPos = vPos;


		pEffect->SetMatrix("g_WorldMatrix", &mat);
		pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
		pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
		pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
		pEffect->SetFloat("g_AlphaRef", 0.0f);


		IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(0);
		pEffect->SetTexture("g_Texture", pTexture);


		CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

		MATERIAL.material.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
		MATERIAL.material.Diffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
		MATERIAL.material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
		MATERIAL.material.Emissive = { 0.0f, 0.0f, 0.0f, 0.0f };
		MATERIAL.material.Power = 0.0f;

		pEffect->SetValue("g_Material", &MATERIAL.material, sizeof(D3DMATERIAL9));

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(0);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}

	


}

void CLightPuzzleBase::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER
		|| _eCollisionGroup == COLLISION_GROUP::COLLIDE_MONSTER || _eCollisionGroup == COLLISION_GROUP::COLLIDE_PUSH)
	{
		_float fDeltaX = 0.f, fDeltaY = 0.f, fDeltaZ = 0.f;

		_vec3 vPos, vOtherPos;
		_vec3 vMin, vMax;
		_vec3 vOtherMin, vOtherMax;

		CTransform* pTransform = m_pTransformCom;
		CTransform* pOtherTransform = pCollider->GetOwner()->Get_TransformCom();

		CBoxCollider* pBoxCollider = dynamic_cast<CBoxCollider*>(m_pColliderCom);
		CBoxCollider* pOtherBoxCollider = dynamic_cast<CBoxCollider*>(pCollider);


		NULL_CHECK_RETURN(pTransform, );
		NULL_CHECK_RETURN(pOtherTransform, );
		NULL_CHECK_RETURN(pBoxCollider, );
		NULL_CHECK_RETURN(pOtherBoxCollider, );

		pTransform->Get_Info(INFO_POS, &vPos);
		pOtherTransform->Get_Info(INFO_POS, &vOtherPos);

		_vec3 vDir = vOtherPos - vPos;
		_float fX = (pBoxCollider->Get_Scale().x * 0.5f) + (pOtherBoxCollider->Get_Scale().x * 0.5f);
		_float fY = (pBoxCollider->Get_Scale().y * 0.5f) + (pOtherBoxCollider->Get_Scale().y * 0.5f);
		_float fZ = (pBoxCollider->Get_Scale().z * 0.5f) + (pOtherBoxCollider->Get_Scale().z * 0.5f);


		if (fabs(vDir.x) >= fX)
			return;

		if (fabs(vDir.y) >= fY)
			return;

		if (fabs(vDir.z) >= fZ)
			return;



		if (fX - fabs(vDir.x) < fZ - fabs(vDir.z) && fX - fabs(vDir.x) < fY - fabs(vDir.y))
		{
			if (vDir.x < 0.f)
			{
				vDir.x -= (fX - fabs(vDir.x));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				vDir.x += (fX - fabs(vDir.x));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}
		else if (fY - fabs(vDir.y) < fZ - fabs(vDir.z) && fY - fabs(vDir.y) < fX - fabs(vDir.x))
		{
			if (vDir.y < 0.f)
			{
				vDir.y -= (fY - fabs(vDir.y));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				vDir.y += (fY - fabs(vDir.y));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}
		else if (fZ - fabs(vDir.z) < fX - fabs(vDir.x) && fZ - fabs(vDir.z) < fY - fabs(vDir.y))
		{
			if (vDir.z < 0.f)
			{
				vDir.z -= (fZ - fabs(vDir.z));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
			else
			{
				vDir.z += (fZ - fabs(vDir.z));
				vOtherPos = vPos + vDir;
				pOtherTransform->Set_Pos(&vOtherPos);
			}
		}


	}

}

void CLightPuzzleBase::Set_Lighting(const _bool& pBool)
{
	if (m_bIsLighting != pBool)
	{
		m_bIsLighting = pBool;
		if (!m_bMakeLight)
		{
			if (m_eCutSceneType != CCutSceneMgr::CUTSCENE_TYPE::TYPE_END)
			{
				CCutSceneMgr::GetInstance()->Set_EventNum(m_iMyEvent);
				CCutSceneMgr::GetInstance()->Start_CutScene(m_eCutSceneType);
			}
			else
				Check_Event_Start(m_iMyEvent);
		}
	}
}

void CLightPuzzleBase::Reset_Event()
{
	if (m_bIsLighting != m_bMakeLight)
	{
		m_bIsLighting = m_bMakeLight;
	}
}

void CLightPuzzleBase::Free()
{
	__super::Free();
}

CLightPuzzleBase* CLightPuzzleBase::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos, const _tchar* p_Type)
{
	CLightPuzzleBase* ret = new CLightPuzzleBase(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("LightPuzzleBase Create Failed");
		return nullptr;
	}

	ret->m_pTransformCom->Set_Pos(&_vec3(p_Pos.x, 0.013f, p_Pos.z));
	ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));
	ret->m_pAnimator->Play_Animation(p_Type, false);
	ret->Set_MinHeight(0.013f);

	if (p_Type == L"Base")
	{
		ret->Add_LightDir({ 1, 0 });
		CComponent* pComponent = ret->m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_LightEffect_Start_Hor"));
		NULL_CHECK_RETURN_MSG(pComponent, nullptr, L"LightPuzzleEffect Create Failed");
		pComponent->SetOwner(ret);
		ret->m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

		
	}
	else if (p_Type == L"Corner")
	{
		ret->Add_LightDir({ 0, -1 });
		CComponent* pComponent = ret->m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_LightEffect_Start"));
		NULL_CHECK_RETURN_MSG(pComponent, nullptr, L"LightPuzzleEffect Create Failed");
		pComponent->SetOwner(ret);
		ret->m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	}
	else if (p_Type == L"Vertical")
	{
		ret->Add_LightDir({ 0, -1 });
		CComponent* pComponent = ret->m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_LightEffect_Start"));
		NULL_CHECK_RETURN_MSG(pComponent, nullptr, L"LightPuzzleEffect Create Failed");
		pComponent->SetOwner(ret);
		ret->m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	}
	else if (p_Type == L"Three")
	{
		ret->Add_LightDir({ 0, -1 });
		ret->Add_LightDir({ 0, 1 });
		ret->Add_LightDir({ -1, 0 });

		CComponent* pComponent = ret->m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_LightEffect_Base"));
		NULL_CHECK_RETURN_MSG(pComponent, nullptr, L"LightPuzzleEffect Create Failed");
		pComponent->SetOwner(ret);
		ret->m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	}

	ret->m_iMyEvent = p_EventNum;

	return ret;
}
