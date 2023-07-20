#include "Player_Skill_Drill.h"
#include "Texture.h"
#include "Transform.h"
#include "Player.h"
#include "KeyMgr.h"
#include "RigidBody.h"
#include "Export_Function.h"

CPlayer_Skill_Drill::CPlayer_Skill_Drill(CGameObject* _pOwner)
    : CPlayer_State(_pOwner), m_fSpeed(10.0f), m_fMinHeight(0.0f)
{
    m_eState = DRILL_STATE::START;
}

CPlayer_Skill_Drill::~CPlayer_Skill_Drill()
{
}

HRESULT CPlayer_Skill_Drill::Ready_State(void)
{
    m_pOwner->Get_AnimatorCom()->Play_Animation(L"Drill", false);
    m_eState = DRILL_STATE::START;
    m_fMinHeight = m_pOwner->Get_MinHeight();
    m_pOwner->Set_MinHeight(-1.0f);
    m_bJump = false;

    dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Active(false);

    return S_OK;
}

_int CPlayer_Skill_Drill::Update_State(const _float& fTimeDelta)
{
    switch (m_eState)
    {
    case DRILL_STATE::START:
        Update_Start(fTimeDelta);
        break;
    case DRILL_STATE::INGROUND:
        Update_InGround(fTimeDelta);
        break;
    case DRILL_STATE::OUTGROUND:
        Update_OutGround(fTimeDelta);
        break;
    }

    return 0;
}

void CPlayer_Skill_Drill::LateUpdate_State(void)
{
    switch (m_eState)
    {
    case DRILL_STATE::START:
        LateUpdate_Start();
        break;
    case DRILL_STATE::INGROUND:
        LateUpdate_InGround();
        break;
    case DRILL_STATE::OUTGROUND:
        LateUpdate_OutGround();
        break;
    }

}

void CPlayer_Skill_Drill::Render_State(void)
{
}

void CPlayer_Skill_Drill::Reset_State(void)
{
    m_pOwner->Set_MinHeight(m_fMinHeight);
    dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Active(true);
}

void CPlayer_Skill_Drill::Update_Start(const _float& fTimeDelta)
{
    if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() > 5)
    {
        _vec3 vPos;
        m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPos);

        if (vPos.y > -1.0f)
        {
            vPos.y -= 2.0f * fTimeDelta;
            m_pOwner->Get_TransformCom()->Set_Pos(&vPos);
            if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() > 18)
            {
                m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Idx(11);
            }
        }
        else
        {
            vPos.y = -1.0f;
            m_pOwner->Get_TransformCom()->Set_Pos(&vPos);
            m_eState = DRILL_STATE::INGROUND;

            CGameObject* pAim = dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim();
            pAim->Set_Active(true);
            Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"SkillAim", pAim);

            vPos.y = 0.1f;
            dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim()->Get_TransformCom()->Set_Info(INFO_POS, &vPos);

        }

    }
}

void CPlayer_Skill_Drill::Update_InGround(const _float& fTimeDelta)
{
    Key_Input(fTimeDelta);
}

void CPlayer_Skill_Drill::Update_OutGround(const _float& fTimeDelta)
{

}

void CPlayer_Skill_Drill::LateUpdate_Start()
{


}

void CPlayer_Skill_Drill::LateUpdate_InGround()
{
    _vec3 vPlayerPos;
    m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
    vPlayerPos.y = 0.1f;
    dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim()->Get_TransformCom()->Set_Info(INFO_POS, &vPlayerPos);
    m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Idx(19);
}

void CPlayer_Skill_Drill::LateUpdate_OutGround()
{
    if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() == 20)
    {
        _vec3 vPlayerPos;
        m_pOwner->Get_TransformCom()->Get_Info(INFO_POS, &vPlayerPos);
        vPlayerPos.y = -0.2f;
        m_pOwner->Get_TransformCom()->Set_Pos(&vPlayerPos);
    }
    if (m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() == 21 && !m_bJump)
    {
        m_pOwner->Get_RigidBodyCom()->AddForce(_vec3(0.0f, 1.0f, 0.0f) * 150.0f);
        m_bJump = true;
        m_pOwner->Get_RigidBodyCom()->SetGround(false);
       
    }

    if (m_bJump && m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Get_Idx() > 21)
    {

        if (m_pOwner->Get_RigidBodyCom()->GetVelocity().y > 0.0f)
        {
            m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Idx(21);
        }
        else
        {
            m_pOwner->Set_MinHeight(m_fMinHeight);
            m_pOwner->Get_AnimatorCom()->GetCurrAnimation()->Set_Idx(22);
        }

        if (m_pOwner->Get_RigidBodyCom()->IsGround())
        {
            dynamic_cast<CPlayer*>(m_pOwner)->Change_State_Now(PLAYER_STATE::IDLE);
            dynamic_cast<CPlayer*>(m_pOwner)->Get_Hat()->Set_Active(true);
        }
    }


}

void CPlayer_Skill_Drill::Key_Input(const _float& fTimeDelta)
{
    if (KEY_HOLD(KEY::RIGHT_ARROW))
    {
        m_pOwner->Get_TransformCom()->Move_Pos(OBJ_DIR::DIR_R, m_fSpeed, fTimeDelta);
    }
    if (KEY_HOLD(KEY::LEFT_ARROW))
    {
        m_pOwner->Get_TransformCom()->Move_Pos(OBJ_DIR::DIR_L, m_fSpeed, fTimeDelta);
    }
    if (KEY_HOLD(KEY::UP_ARROW))
    {
        m_pOwner->Get_TransformCom()->Move_Pos(OBJ_DIR::DIR_U, m_fSpeed, fTimeDelta);
    }
    if (KEY_HOLD(KEY::DOWN_ARROW))
    {
        m_pOwner->Get_TransformCom()->Move_Pos(OBJ_DIR::DIR_D, m_fSpeed, fTimeDelta);
    }

    if (KEY_TAP(KEY::Q))
    {
        m_eState = DRILL_STATE::OUTGROUND;
        dynamic_cast<CPlayer*>(m_pOwner)->Get_Aim()->Set_Active(false);
    }
}

