#include "Export_Function.h"
#include "GolemFist.h"
#include "SunGollem.h"
#include "FistEffect.h"
#include "TrashBummer.h"
#include "Effect_StoneSpike.h"

CGolemFist::CGolemFist(LPDIRECT3DDEVICE9 pGraphicDev) : Engine::CGameObject(pGraphicDev, OBJ_TYPE::OBJ_BULLET, OBJ_ID::MONSTER_SKILL)
{
}
CGolemFist::CGolemFist(const CGolemFist& rhs)
	: Engine::CGameObject(rhs)
{

}

CGolemFist::~CGolemFist()
{
}

HRESULT CGolemFist::Ready_Object(void)
{
	//227 / 315
	m_fMoveTime = 0.f;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pAnimator->Add_Animation(L"SunGolem_Idle_Fist", L"Proto_Texture_SunGolem_Idle_Fist", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_Fist", L"Proto_Texture_SunGolem_Dirty_Fist", 0.1f);
	m_pAnimator->Add_Animation(L"SunGolem_Dirty_BummerFist", L"Proto_Texture_SunGolem_Dirty_BummerFist", 0.1f);
	m_pAnimator->Play_Animation(L"SunGolem_Idle_Fist", true);

	m_pTransformCom->Set_Pos(&_vec3(2.0f, 2.0f, 2.0f));
	m_pTransformCom->Set_Scale({ 0.6f, 1.f, 1.f });
	m_pMonsterAim = CMonsterAim::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(m_pMonsterAim, E_FAIL);
	m_pMonsterAim->Set_Active(true);
	return S_OK;
}

_int CGolemFist::Update_Object(const _float& fTimeDelta)
{
	int iExit = __super::Update_Object(fTimeDelta);
	Add_RenderGroup(RENDERID::RENDER_ALPHA, this);

	_vec3 vDir = { 0.f, -1.f, 0.f };


	if (m_bDirty == true)
	{
		if (m_bBummer == true)
			m_pAnimator->Play_Animation(L"SunGolem_Dirty_BummerFist", true);
		else
			m_pAnimator->Play_Animation(L"SunGolem_Dirty_Fist", true);
	}


	m_pTransformCom->Move_Pos(&vDir, fTimeDelta, 20.f);
	
	m_pMonsterAim->Update_Object(fTimeDelta);

	return iExit;
}

void CGolemFist::LateUpdate_Object(void)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pMonsterAim->Get_TransformCom()->Set_Pos(&_vec3{ vPos.x,0.01f,vPos.z });
	m_pMonsterAim->Set_Red(int(vPos.y/7.f*255.f));
	__super::LateUpdate_Object();
	if (vPos.y < -1.f)
	{
		if (Is_Active())
		{			
			if (m_bDirty == true)
			{
				if (m_bBummer == true && rand() % 99 < 33)
				{
					CTrashBummer* pTrashBummer = CTrashBummer::Create(m_pGraphicDev);
					NULL_CHECK_RETURN(pTrashBummer, );
					vPos.y = 0.5f;
					pTrashBummer->Get_TransformCom()->Set_Pos(&vPos);
					CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::MONSTER);
					pLayer->Add_GameObject(L"GolemFist", pTrashBummer);
				}
				else
				{
					CEffect_StoneSpike* pFistEffect = CEffect_StoneSpike::Create(m_pGraphicDev);
					NULL_CHECK_RETURN(pFistEffect, );
					vPos.y = -0.5f;
					pFistEffect->Get_TransformCom()->Set_Pos(&vPos);
					pFistEffect->Set_Atk(m_iAtk);
					CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
					pLayer->Add_GameObject(L"GolemFist", pFistEffect);
					Set_Active(false);
					m_pMonsterAim->Set_Active(false);
				}
			}
			CFistEffect* pFistEffect = CFistEffect::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pFistEffect, );
			vPos.y = 0.001f;
			pFistEffect->Get_TransformCom()->Set_Pos(&vPos);
			pFistEffect->Set_Atk(m_iAtk);
			CLayer* pLayer = Engine::GetCurrScene()->Get_Layer(LAYER_TYPE::ENVIRONMENT);
			pLayer->Add_GameObject(L"GolemFist", pFistEffect);
			Set_Active(false);
			m_pMonsterAim->Set_Active(false);
		}
	}
	m_pMonsterAim->LateUpdate_Object();
}

void CGolemFist::Render_Object(void)
{
	if (Is_Active())
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());

		__super::Render_Object();
		m_pBufferCom->Render_Buffer();
		m_pMonsterAim->Render_Object();
	}
	
}

HRESULT CGolemFist::Add_Component(void)
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

	pComponent = m_pAnimator = dynamic_cast<CAnimator*>(Engine::Clone_Proto(L"Proto_Animator"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_ANIMATOR, pComponent);
	return S_OK;
}




CGolemFist* CGolemFist::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGolemFist* pInstance = new CGolemFist(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("GolemFist Create Failed");
		return nullptr;
	}

	return pInstance;
}
void CGolemFist::Free()
{
	__super::Free();
}
