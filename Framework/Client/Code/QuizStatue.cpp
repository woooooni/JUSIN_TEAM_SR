#include "QuizStatue.h"
#include"Export_Function.h"

CQuizStatue::CQuizStatue(LPDIRECT3DDEVICE9 p_Dev) 
	: CFieldObject(p_Dev, OBJ_ID::BUG_STATUE)
	, m_iEventNum(0)
	,m_eAnswer(MONKEY_STATUE::STATUE_END)
	, m_eMonkey(MONKEY_STATUE::STATUE_END)
	,m_bIsCorrect(false)
	,m_bIsLocked(false)

{
}

CQuizStatue::CQuizStatue(const CQuizStatue& rhs) 
	: CFieldObject(rhs)
	,m_iEventNum(rhs.m_iEventNum)
	,m_eAnswer(rhs.m_eAnswer)
	,m_eMonkey(rhs.m_eMonkey)
	,m_bIsCorrect(rhs.m_bIsCorrect)
	,m_bIsLocked(rhs.m_bIsLocked)
{
}

CQuizStatue::~CQuizStatue()
{
}

HRESULT CQuizStatue::Ready_Object(void)
{
	FAILED_CHECK(Ready_Component());

	

	return S_OK;
}

_int CQuizStatue::Update_Object(const _float& fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);

	return __super::Update_Object(fTimeDelta);
}

void CQuizStatue::LateUpdate_Object(void)
{
	__super::LateUpdate_Object();
}

void CQuizStatue::Render_Object(void)
{
	m_pTextureCom->Render_Texture();
	__super::Render_Object();
}

void CQuizStatue::Free()
{
}

CQuizStatue* CQuizStatue::Create(LPDIRECT3DDEVICE9 p_Dev, const _uint& p_EventNum,const MONKEY_STATUE& answer, const _vec3 p_Pos, MONKEY_STATUE p_Monkey)
{
	if (p_Monkey >= MONKEY_STATUE::STATUE_END)
		return nullptr;

	CQuizStatue* ret = new CQuizStatue(p_Dev);
	NULL_CHECK_RETURN(ret, nullptr);
	if (FAILED(ret->Ready_Object()))
	{
		Safe_Release(ret);
		MSG_BOX("Quiz Statue Create Failed");
		return nullptr;
	}

	ret->m_iEventNum = p_EventNum;
	ret->m_pTransformCom->Set_Pos(&p_Pos);
	ret->Set_MinHeight(0.5f);
	
	ret->m_eMonkey = p_Monkey;
	ret->m_eAnswer = answer;

	ret->m_pTextureCom->Set_Idx((_uint)p_Monkey);

	ret->m_bIsCorrect = (ret->m_eAnswer == ret->m_eMonkey);

	if (ret->m_bIsCorrect)
		Check_Event_Start(ret->m_iEventNum);

	return ret;
}



void CQuizStatue::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
	if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING && !m_bIsLocked)
	{
		m_eMonkey = (MONKEY_STATUE)((_uint)m_eMonkey + 1);
		if (m_eMonkey == MONKEY_STATUE::STATUE_END)
			m_eMonkey = MONKEY_STATUE::EAR;
		m_pTextureCom->Set_Idx(_uint(m_eMonkey));

		if ((m_eAnswer == m_eMonkey) != m_bIsCorrect)
		{
			Engine::Check_Event_Start(m_iEventNum);
			m_bIsCorrect = !m_bIsCorrect;
		}
	}
}

void CQuizStatue::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CQuizStatue::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CQuizStatue::Event_Start(_uint iEventNum)
{
	m_bIsLocked = true;
}

HRESULT CQuizStatue::Ready_Component()
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

	pComponent = m_pColliderCom = dynamic_cast<CBoxCollider*>(Engine::Clone_Proto(L"Proto_BoxCollider"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_BOX_COLLIDER, pComponent);

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Proto_Tex_MonkeyStatue_Quiz"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_STATIC].emplace(COMPONENT_TYPE::COM_TEXTURE, pComponent);

	pComponent = m_pRigidBodyCom = dynamic_cast<CRigidBody*>(Engine::Clone_Proto(L"Proto_RigidBody"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	pComponent->SetOwner(this);
	m_mapComponent[ID_DYNAMIC].emplace(COMPONENT_TYPE::COM_RIGIDBODY, pComponent);

	return S_OK;

}

