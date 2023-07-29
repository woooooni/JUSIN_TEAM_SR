#include "Particle_MapCircle.h"

#include "Export_Function.h"
#include "Scene.h"
#include "Terrain.h"
#include "Pool.h"
#include "GameMgr.h"

CParticle_MapCircle::CParticle_MapCircle(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev, OBJ_TYPE::OBJ_EFFECT, OBJ_ID::EFFECT)
{
}

CParticle_MapCircle::CParticle_MapCircle(const CParticle_MapCircle& rhs)
    : CGameObject(rhs)
{
}

CParticle_MapCircle::~CParticle_MapCircle()
{
}

HRESULT CParticle_MapCircle::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

	m_pTransformCom->Set_Scale(_vec3(0.2f, 0.2f, 0.2f));

	m_pAnimator->Add_Animation(L"MapCircle", L"Proto_Texture_Effect_MapCircle", 0.1f);

	m_pAnimator->Play_Animation(L"MapCircle", false);

	Set_Active(false);

	m_fMinHeight = 0.0f;
	m_pRigidBodyCom->SetGravity(-0.001f);

	m_fAccTime = 0.0f;
	m_fEndTime = 3.0f;
	m_fStartTime = 1.0f;
	m_fMoveTime = 0.5f;
	m_fAccMoveTime = 0.0f;
	

	m_iR = m_iG = m_iB = 255;
	m_iAlpha = 0;

	return S_OK;
}

_int CParticle_MapCircle::Update_Object(const _float& fTimeDelta)
{
	if (!Is_Active())
		return S_OK;


	Update_Move(fTimeDelta);
	Update_Show(fTimeDelta);
	Update_Reset(fTimeDelta);

	Engine::Add_RenderGroup(RENDERID::RENDER_EFFECT, this);

	_int iExit = __super::Update_Object(fTimeDelta);

	return iExit;
}

void CParticle_MapCircle::LateUpdate_Object(void)
{
	if (!Is_Active())
		return;

	BillBoard();


	__super::LateUpdate_Object();
}

void CParticle_MapCircle::Render_Object(void)
{
	if (!Is_Active())
		return;

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(m_iAlpha, m_iR, m_iG, m_iB));

	_matrix matWorld = *(m_pTransformCom->Get_WorldMatrix());
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());


	__super::Render_Object();
	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CParticle_MapCircle::Ready_Component(void)
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

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	return S_OK;
}

void CParticle_MapCircle::Random_Particle(_vec3& _vMax, _uint _iCount, _uint iR, _uint iG, _uint iB, _uint _iMaxAlpha)
{
	m_vMax = _vMax;
	m_iAlpha = 0;
	m_iMaxAlpha = _iMaxAlpha;
	m_iR = 255;
	m_iG = 255;
	m_iB = 255;
	m_iCount = _iCount;
	m_fAccMoveTime = 0.0f;
	m_fAccTime = 0.0f;

	_float fX = m_vMax.x / (_float)_iCount;
	_float fY = m_vMax.y / (_float)_iCount;
	_float fZ = m_vMax.z / (_float)_iCount;


	int iTemp = rand() % (_iCount + 1);
	fX *= (_float)iTemp;

	iTemp = rand() % (_iCount + 1);
	fY *= (_float)iTemp * 0.25f;

	iTemp = rand() % (_iCount + 1);
	fZ *= (_float)iTemp;


	iTemp = rand() % 2;
	if (iTemp == 1)
		fX *= -1.0f;

	iTemp = rand() % 2;
	if (iTemp == 1)
		fZ *= -1.0f;

	_vec3 vPlayerPos;
	CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);

	_vec3 vPos = { vPlayerPos.x + fX, vPlayerPos.y + fY, vPlayerPos.z + fZ };
	
	
	
	m_pTransformCom->Set_Pos(&vPos);


	m_pRigidBodyCom->SetGround(false);

	Set_Active(true);
	m_pRigidBodyCom->SetVelocity(_vec3(0.0f, 0.0f, 0.0f));
}

CParticle_MapCircle* CParticle_MapCircle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CParticle_MapCircle* pInstance = new CParticle_MapCircle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Particle_MapCircle_Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CParticle_MapCircle::Update_Show(const _float& fTimeDelta)
{
	if (m_bStop)
		return;

	if (m_bShow)
	{
		if (m_fAccTime > m_fEndTime)
		{
			m_iAlpha -= 1;
			if (m_iAlpha <= 0)
			{
				m_iAlpha = 0;
				m_bShow = false;
				m_fAccTime = 0.0f;
			}
		}
		else
			m_fAccTime += fTimeDelta;
	}
	else
	{
		if (m_fAccTime > m_fStartTime)
		{
			m_iAlpha += 1;
			if (m_iAlpha >= m_iMaxAlpha)
			{
				m_iAlpha = m_iMaxAlpha;
				m_bShow = true;
				m_fAccTime = 0.0f;
			}
		}
		else
			m_fAccTime += fTimeDelta;
	}
	
}

void CParticle_MapCircle::Update_Reset(const _float& fTimeDelta)
{
	_vec3 vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);


	if (vPos.y < 0.2f)
	{
		m_bStop = true;
		m_iAlpha -= 1;
		if (m_iAlpha <= 0)
		{
			m_bShow = false;
			Random_Particle(m_vMax, m_iCount, m_iR, m_iG, m_iB, m_iMaxAlpha);
			m_bStop = false;
			m_fAccTime = m_fStartTime;
		}
	}

	_vec3 vPlayerPos;
	CGameMgr::GetInstance()->Get_Player()->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);


	if (D3DXVec3Length(&(vPlayerPos - vPos)) > D3DXVec3Length(&m_vMax))
	{
		m_bShow = false;
		Random_Particle(m_vMax, m_iCount, m_iR, m_iG, m_iB, m_iMaxAlpha);
		m_bStop = false;
		m_fAccTime = m_fStartTime;
	}
}

void CParticle_MapCircle::Update_Move(const _float& fTimeDelta)
{
	if (m_fAccMoveTime > m_fMoveTime)
	{
		int iTemp = rand() % 100;
		_float fX = iTemp * 0.1f;
		iTemp = rand() % 100;
		_float fY = iTemp * 0.1f;
		
		iTemp = rand() % 2;
		if (iTemp == 1)
			fX *= -1.0f;

		iTemp = rand() % 2;
		if (iTemp == 1)
			fY *= -1.0f;

		m_pRigidBodyCom->AddForce(_vec3(fX, fY, 0.0f));
		m_fAccMoveTime = 0.0f;
	}
	else
		m_fAccMoveTime += fTimeDelta;
}

void CParticle_MapCircle::BillBoard()
{
	if (m_pTransformCom == nullptr)
		return;

	_vec3 fScale = m_pTransformCom->Get_Scale();

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	// 카메라의 월드행렬.
	D3DXMatrixInverse(&matView, nullptr, &matView);

	_vec3 vCameraPos = _vec3(matView._41, matView._42, matView._43);
	_vec3 vObjPos, vObjUp = { 0.f, 1.f, 0.f };
	m_pTransformCom->Get_Info(INFO_UP, &vObjUp);
	m_pTransformCom->Get_Info(INFO_POS, &vObjPos);

	_vec3 vDir = vObjPos - vCameraPos;
	_vec3 vRight, vLook;

	D3DXVec3Normalize(&vDir, &vDir);
	D3DXVec3Cross(&vRight, &vObjUp, &vDir);
	D3DXVec3Cross(&vLook, &vRight, &vObjUp);

	m_pTransformCom->Set_Info(INFO_RIGHT, &(vRight));
	m_pTransformCom->Set_Info(INFO_LOOK, &(vLook));

}

void CParticle_MapCircle::Free()
{
	__super::Free();
}
