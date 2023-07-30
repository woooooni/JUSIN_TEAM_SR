#include	"BreakStone.h"
#include	"Export_Function.h"
#include	"Pool.h"

CBreakStone::CBreakStone(LPDIRECT3DDEVICE9 p_Dev) : CFieldObject(p_Dev, OBJ_ID::PUSH_STONE) , m_eColor(JELLY_COLOR::JELLY_END), m_bBreak(false)
{
}

CBreakStone::CBreakStone(const CBreakStone& rhs) : CFieldObject(rhs), m_eColor(rhs.m_eColor), m_bBreak(rhs.m_bBreak)
{
}

CBreakStone::~CBreakStone()
{
}


HRESULT		CBreakStone::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	m_pAnimator->Add_Animation(L"Yellow", L"Proto_Tex_BreakStone_Yellow", 0.f);
	m_pAnimator->Add_Animation(L"Red", L"Proto_Tex_BreakStone_Red", 0.f);
	m_pAnimator->Add_Animation(L"Green", L"Proto_Tex_BreakStone_Green", 0.f);
	m_pAnimator->Add_Animation(L"Breaking", L"Proto_Tex_BreakStone_Breaking", 0.1f);

	return S_OK;
}


_int CBreakStone::Update_Object(const _float& fTimeDelta)
{
	if (m_bBreak && m_pAnimator->GetCurrAnimation()->Get_Idx() == (m_pAnimator->GetCurrAnimation()->Get_Size() - 1))
	{

	}
	else
	{
		Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_WALL);
		Add_RenderGroup(RENDER_ALPHA, this);
	}


	return __super::Update_Object(fTimeDelta);
}

void CBreakStone::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CBreakStone::Render_Object(void)
{
	D3DCOLORVALUE vColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	if (m_bBreak)
	{
		switch (m_eColor)
		{
		case JELLY_COLOR::YELLOW:
			vColor = { 1.0f, 1.0f, 0.0f, 1.0f };
			break;
		case JELLY_COLOR::GREEN:
			vColor = { 0.0f, 1.0f, 0.0f, 1.0f };
			break;
		case JELLY_COLOR::RED:
			vColor = { 1.0f, 0.0f, 0.0f, 1.0f };
			break;
		default:
			break;
		}

	}

	LPD3DXEFFECT pEffect = m_pShader->Get_Effect();

	CCamera* pCamera = dynamic_cast<CCamera*>(Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::CAMERA)->Find_GameObject(L"MainCamera"));
	if (pCamera == nullptr)
		return;

	_vec3 vPos;
	pCamera->Get_TransformCom()->Get_Info(INFO_POS, &vPos);
	D3DVECTOR vCamPos = vPos;

	pEffect->SetMatrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix());
	pEffect->SetMatrix("g_ViewMatrix", &pCamera->GetViewMatrix());
	pEffect->SetMatrix("g_ProjMatrix", &pCamera->GetProjectionMatrix());
	pEffect->SetValue("g_CamPos", &vCamPos, sizeof(D3DVECTOR));
	pEffect->SetValue("g_Color", &vColor, sizeof(D3DCOLORVALUE));
	pEffect->SetFloat("g_AlphaRef", 0.0f);


	IDirect3DBaseTexture9* pTexture = m_pAnimator->GetCurrAnimation()->Get_Texture(m_pAnimator->GetCurrAnimation()->Get_Idx());
	pEffect->SetTexture("g_Texture", pTexture);


	CLightMgr::GetInstance()->Set_LightToEffect(pEffect);



	pEffect->SetValue("g_Material", &m_tMaterial, sizeof(D3DMATERIAL9));

	pEffect->Begin(nullptr, 0);
	pEffect->BeginPass(1);

	m_pBufferCom->Render_Buffer();

	pEffect->EndPass();
	pEffect->End();

}

void CBreakStone::Add_Subscribe(_uint iNum)
{
	Engine::Add_Subscribe(iNum, this);
}

void CBreakStone::Free()
{
	__super::Free();
}

CBreakStone* CBreakStone::Create(LPDIRECT3DDEVICE9 p_Dev, JELLY_COLOR pColor, const _uint& p_EventNum, const _vec3 p_Pos)
{
	if (pColor != JELLY_COLOR::RED && pColor != JELLY_COLOR::GREEN && pColor != JELLY_COLOR::YELLOW)
		return nullptr;

	CBreakStone* ret = new CBreakStone(p_Dev);

	NULL_CHECK_RETURN(ret, nullptr);

	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Breakstone creata failed");
		return nullptr;
	}

	ret->m_eColor = pColor;

	switch (ret->m_eColor)
	{	
	case JELLY_COLOR::YELLOW:
		ret->m_pAnimator->Play_Animation(L"Yellow", false);
		break;
	case JELLY_COLOR::GREEN:
		ret->m_pAnimator->Play_Animation(L"Green", false);
		break;
	case JELLY_COLOR::RED:
		ret->m_pAnimator->Play_Animation(L"Red", false);
		break;

	default:
		break;
	}

	ret->Add_Subscribe(p_EventNum);
	ret->Get_TransformCom()->Set_Pos(&p_Pos);
	ret->Set_MinHeight(0.5f);
	return ret;
}

void CBreakStone::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CBreakStone::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
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

void CBreakStone::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{

}

void CBreakStone::Event_Start(_uint iEventNum)
{
	m_bBreak = true;
	m_pAnimator->Play_Animation(L"Breaking", false);
}

void CBreakStone::Event_End(_uint iEventNum)
{
	m_bBreak = true;
	m_pAnimator->Play_Animation(L"Breaking", false);

}

void CBreakStone::Reset_Event()
{
	m_bBreak = false;
	switch (m_eColor)
	{
	case JELLY_COLOR::YELLOW:
		m_pAnimator->Play_Animation(L"Yellow", false);
		break;
	case JELLY_COLOR::GREEN:
		m_pAnimator->Play_Animation(L"Green", false);
		break;
	case JELLY_COLOR::RED:
		m_pAnimator->Play_Animation(L"Red", false);
		break;

	default:
		break;
	}

}
