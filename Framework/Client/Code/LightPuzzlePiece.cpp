#include "LightPuzzlePiece.h"
#include "Export_Function.h"

CLightPuzzlePiece::CLightPuzzlePiece(LPDIRECT3DDEVICE9 pGraphicDev) : CLightPuzzle(pGraphicDev)
{
	m_tInfo.m_bIsPushable = true;
}

CLightPuzzlePiece::CLightPuzzlePiece(const CLightPuzzlePiece& rhs) : CLightPuzzle(rhs)
{
}

CLightPuzzlePiece::~CLightPuzzlePiece()
{
}

HRESULT CLightPuzzlePiece::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Base", L"Proto_Tex_MoonPuzzleTile", 0.f);
	m_pAnimator->Add_Animation(L"Horizon", L"Proto_Tex_MoonPuzzleTile_Hor", 0.f);
	m_pAnimator->Add_Animation(L"Corner", L"Proto_Tex_MoonPuzzleTile_Corner", 0.f);
	m_pAnimator->Add_Animation(L"Vertical", L"Proto_Tex_MoonPuzzleTile_Vert", 0.f);
	m_pAnimator->Add_Animation(L"Base_Up", L"Proto_Tex_MoonPuzzleTile_Up", 0.f);

	

	return S_OK;
}

_int CLightPuzzlePiece::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_PUSH);

	return __super::Update_Object(fTimeDelta);
}

void CLightPuzzlePiece::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CLightPuzzlePiece::Render_Object(void)
{
	_matrix world = *(m_pTransformCom->Get_WorldMatrix());

	
	__super::Render_Object();
	world._42 += 0.05f;


	if (m_bIsLighting)
	{

		LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

		CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
		if (pCamera == nullptr)
			return;

		_vec3 vPos;
		pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
		D3DVECTOR vCamPos = vPos;


		pEffect->SetMatrix("g_WorldMatrix", &world);
		pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
		pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
		pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
		pEffect->SetFloat("g_AlphaRef", 0.0f);


		IDirect3DBaseTexture9* pTexture = m_pTextureCom->Get_Texture(0);
		pEffect->SetTexture("g_Texture", pTexture);


		CLightMgr::GetInstance()->Set_LightToEffect(pEffect);

		

		pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

		pEffect->Begin(nullptr, 0);
		pEffect->BeginPass(2);

		m_pBufferCom->Render_Buffer();

		pEffect->EndPass();
		pEffect->End();
	}


	

}

void CLightPuzzlePiece::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_BALPAN || _eCollisionGroup == COLLISION_GROUP::COLLIDE_TRIGGER)
		return;

	Push_Me(pCollider);

}

void CLightPuzzlePiece::Reset_Event()
{
	m_bIsLighting = false;
	m_pTransformCom->Set_Pos(&originPos);
}

void CLightPuzzlePiece::Free()
{
	__super::Free();
}

CLightPuzzlePiece* CLightPuzzlePiece::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum, const _vec3 p_Pos, const _tchar* p_FirstName)
{

	CLightPuzzlePiece* ret = new CLightPuzzlePiece(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("LightPuzzlePiece Create Failed");
		return nullptr;
	}
	ret->m_pTransformCom->Set_Pos(&_vec3(p_Pos.x, 0.02f, p_Pos.z));
	ret->originPos = _vec3(p_Pos.x, 0.02f, p_Pos.z);


	if (p_FirstName == L"Base")
	{
		ret->m_lightDir.push_back({ -1, 0, });
		ret->m_lightDir.push_back({ 0, -1 });
		ret->m_lightDir.push_back({ 0, 1 });

		CComponent* pComponent = ret->m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_LightEffect_Base"));
		NULL_CHECK_RETURN_MSG(pComponent, nullptr, L"LightPuzzleEffect Create Failed");
		pComponent->SetOwner(ret);
		ret->m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	}
	else if (p_FirstName == L"Horizon")
	{
		ret->m_lightDir.push_back({ -1, 0, });
		ret->m_lightDir.push_back({ 1, 0 });

		CComponent* pComponent = ret->m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_LightEffect_Hor"));
		NULL_CHECK_RETURN_MSG(pComponent, nullptr, L"LightPuzzleEffect Create Failed");
		pComponent->SetOwner(ret);
		ret->m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	}
	else if (p_FirstName == L"Corner")
	{
		ret->m_lightDir.push_back({ -1, 0, });
		ret->m_lightDir.push_back({ 0, -1 });

		CComponent* pComponent = ret->m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_LightEffect_Corner"));
		NULL_CHECK_RETURN_MSG(pComponent, nullptr, L"LightPuzzleEffect Create Failed");
		pComponent->SetOwner(ret);
		ret->m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	}
	else if (p_FirstName == L"Vertical")
	{
		ret->m_lightDir.push_back({ 0, 1 });
		ret->m_lightDir.push_back({ 0, -1 });

		CComponent* pComponent = ret->m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_LightEffect_Vert"));
		NULL_CHECK_RETURN_MSG(pComponent, nullptr, L"LightPuzzleEffect Create Failed");
		pComponent->SetOwner(ret);
		ret->m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	}
	else if (p_FirstName == L"Base_Up")
	{
		ret->m_lightDir.push_back({ 0, 1 });
		ret->m_lightDir.push_back({ 1, 0 });
		ret->m_lightDir.push_back({ -1, 0 });


		CComponent* pComponent = ret->m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_LightEffect_Base_Up"));
		NULL_CHECK_RETURN_MSG(pComponent, nullptr, L"LightPuzzleEffect Create Failed");
		pComponent->SetOwner(ret);
		ret->m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	}
	ret->m_pAnimator->Play_Animation(p_FirstName, false);
	ret->m_pTransformCom->RotationAxis({ 1, 0, 0 }, D3DXToRadian(90.f));
	ret->Set_MinHeight(0.02f);
	return ret;
}

void CLightPuzzlePiece::Set_Lighting(const _bool& pBool) 
{ 
	m_bIsLighting = pBool; 
	Play_Sound(L"SFX_99_MazePuzzle_Complete.wav", CHANNELID::SOUND_EFFECT_INTERACTION, .5f);

}
