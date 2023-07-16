#include    "../Include/stdafx.h"
#include "Grass.h"
#include    "Export_Function.h"
#include    "DefaultItem.h"
#include    "UseItem.h"
#include    "Pool.h"
#include    <time.h>
#include    "Effect_Leaf.h"

CGrass::CGrass(LPDIRECT3DDEVICE9 pGr) : CFieldObject(pGr, OBJ_ID::GRASS), m_eGrassType(GRASS_TYPE::GRASS_END)
, m_fCurMoveTime(0.f)
, m_fMaxMoveTime(0.f)
, m_bIsReverse(false)
{
    m_tInfo.m_bIsAttackable = true;
}

CGrass::CGrass(const CGrass& rhs) : CFieldObject(rhs),
m_GrassList(rhs.m_GrassList),
m_fCurMoveTime(rhs.m_fCurMoveTime),
m_fMaxMoveTime(rhs.m_fMaxMoveTime)
, m_bIsReverse(rhs.m_bIsReverse)
, m_eGrassType(rhs.m_eGrassType)

{
}

CGrass::~CGrass()
{
}

HRESULT CGrass::Ready_Object(void)
{
    FAILED_CHECK(Ready_Component());



    return S_OK;
}

_int CGrass::Update_Object(const _float& fTimeDelta)
{
    Add_RenderGroup(RENDER_ALPHA, this);
    Add_CollisionGroup(m_pColliderCom, COLLISION_GROUP::COLLIDE_BREAK);
    if (m_fMaxMoveTime > 0.f)
    {
        for (auto& iter : m_GrassList)
        {
            if (!m_bIsReverse)
                iter->m_fAngle += iter->m_vPosByCenter.x >= 0 ? iter->m_fMaxAngle * 5 * fTimeDelta : -1.f * iter->m_fMaxAngle * 5 * fTimeDelta;
            else
                iter->m_fAngle += iter->m_vPosByCenter.x >= 0 ? -1.f * iter->m_fMaxAngle * 5 * fTimeDelta : iter->m_fMaxAngle * 5 * fTimeDelta;
        }
        m_fCurMoveTime += fTimeDelta;
        if (m_fCurMoveTime >= 0.2f)
        {
            m_fMaxMoveTime -= 0.2f;
            m_fCurMoveTime = 0.f;
            m_bIsReverse = !m_bIsReverse;
        }
    }
    else
    {
        if (m_fMaxMoveTime != 0.f)
            m_fMaxMoveTime = 0.f;

        if (m_bIsReverse == true)
            m_bIsReverse = false;

        if (m_fCurMoveTime != 0.f)
            m_fCurMoveTime = 0.f;

        for (auto& iter : m_GrassList)
        {
            if (iter->m_fAngle != iter->m_fOriginAngle)
                iter->m_fAngle = iter->m_fOriginAngle;
        }
    }


    return __super::Update_Object(fTimeDelta);
}

void CGrass::LateUpdate_Object(void)
{
    __super::LateUpdate_Object();
}

void CGrass::Render_Object(void)
{


    _matrix mat = *m_pTransformCom->Get_WorldMatrix();
    _matrix src;
    _matrix rot;

    _vec3   tmp;

    m_pGraphicDev->SetTransform(D3DTS_WORLD, &mat);
    m_pColliderCom->Render_Component();
    for (auto& iter : m_GrassList)
    {
        D3DXMatrixScaling(&src, iter->m_fScale.x, iter->m_fScale.y, iter->m_fScale.z);
        D3DXMatrixRotationZ(&rot, D3DXToRadian(iter->m_fAngle));

        src = (src * rot);
        D3DXVec3TransformCoord(&tmp, &iter->m_vRenderPos, &rot);

        if (iter->m_iIsReverse)
        {
            D3DXMatrixRotationY(&rot, D3DXToRadian(180.f));
            src *= rot;
            D3DXVec3TransformCoord(&tmp, &tmp, &rot);

        }

        src *= mat;

        src._41 += iter->m_vPosByCenter.x + tmp.x;
        src._42 += tmp.y;
        src._43 += iter->m_vPosByCenter.y + tmp.z;

        m_pGraphicDev->SetTransform(D3DTS_WORLD, &src);
        iter->m_pTexture->Set_Idx(iter->m_iTextureIndex);
        iter->m_pTexture->Render_Texture();
        m_pBufferCom->Render_Buffer();
    }
}

void CGrass::Add_Subscribe(_uint iNum)
{
    Engine::Add_Subscribe(iNum, this);
}

void CGrass::Free()
{
    for_each(m_GrassList.begin(), m_GrassList.end(), [&](GrassTexture* iter)
        {
            Safe_Delete(iter);
        });
    __super::Free();
}

CGrass* CGrass::Create(LPDIRECT3DDEVICE9 p_Dev, const GRASS_TYPE& p_Type, const _uint& p_EventNum, const _vec3 p_Pos)
{
    CGrass* ret = new CGrass(p_Dev);
    NULL_CHECK_RETURN(ret, nullptr);
    if (FAILED(ret->Ready_Object()))
    {
        Safe_Release(ret);
        MSG_BOX("Grass Create Failed");
        return nullptr;
    }
    ret->m_eGrassType = p_Type;

    if (FAILED(ret->Set_Grass()))
    {
        Safe_Release(ret);
        MSG_BOX("Grass Create Failed");
        return nullptr;
    }
    ret->m_pTransformCom->Set_Scale({ 1.5f, 1.5f, 1.5f });
    ret->m_pTransformCom->Set_Pos(&p_Pos);
    
    ret->m_pColliderCom->Set_Offset({ 0, 0.75f, 0 });

    return ret;
}

void CGrass::Collision_Enter(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{

    _vec3 myPos;
    m_pTransformCom->Get_Info(INFO_POS, &myPos);

    if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_SWING)
    {
        Set_Active(false);
        srand(unsigned(time(NULL)));

        _vec3 vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);
        CGameObject* pLeaf = CPool<CEffect_Leaf>::Get_Obj();
        if (pLeaf)
            dynamic_cast<CEffect_Leaf*>(pLeaf)->Get_Effect(vPos, _vec3(1.2f, 2.5f, 1.5f), 40);
        else
        {
            pLeaf = dynamic_cast<CEffect_Leaf*>(pLeaf)->Create(Engine::Get_Device());
            if (pLeaf)
                dynamic_cast<CEffect_Leaf*>(pLeaf)->Get_Effect(vPos, _vec3(1.2f, 2.5f, 1.5f), 40);
        }



        for (auto& iter : m_dropItemMap)
        {
            if (rand() % 100 < iter.second)
            {

                if (iter.first >= ITEM_CODE::HP_SMALL && iter.first <= ITEM_CODE::SPEED_BIG)
                {
                    CUseItem* src = dynamic_cast<CUseItem*>(CPool<CUseItem>::Get_Obj());

                    if (src)
                    {
                        src->Change_Item(iter.first);
                    }
                    else
                    {
                        src = CUseItem::Create(m_pGraphicDev, OBJ_ID::ITEM, iter.first);
                    }

                    src->Get_TransformCom()->Set_Pos(&myPos);
                    Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"Item", src);
                    return;

                }
                else if (iter.first >= ITEM_CODE::LEAF && iter.first <= ITEM_CODE::TWIG)
                {
                    CDefaultItem* src = dynamic_cast<CDefaultItem*>(CPool<CDefaultItem>::Get_Obj());

                    if (src)
                    {
                        src->Change_Item(iter.first);
                    }
                    else
                    {
                        src = CDefaultItem::Create(m_pGraphicDev, OBJ_ID::ITEM, iter.first);
                    }

                    src->Get_TransformCom()->Set_Pos(&myPos);
                    Get_Layer(LAYER_TYPE::INTERACTION_OBJ)->Add_GameObject(L"Item", src);
                    return;
                }
            }
        }
    }
    else if (_eCollisionGroup == COLLISION_GROUP::COLLIDE_PLAYER)
    {
        m_fMaxMoveTime = 0.5f;

        CGameObject* pLeaf = CPool<CEffect_Leaf>::Get_Obj();
        if (pLeaf)
            dynamic_cast<CEffect_Leaf*>(pLeaf)->Get_Effect(myPos, _vec3(1.5f, 4.f, 2.f), 10);
        else
        {
            pLeaf = dynamic_cast<CEffect_Leaf*>(pLeaf)->Create(Engine::Get_Device());
            if (pLeaf)
                dynamic_cast<CEffect_Leaf*>(pLeaf)->Get_Effect(myPos, _vec3(1.5f, 4.f, 2.f), 10);
        }

    }
}

void CGrass::Collision_Stay(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CGrass::Collision_Exit(CCollider* pCollider, COLLISION_GROUP _eCollisionGroup, UINT _iColliderID)
{
}

void CGrass::Event_Start(_uint iEventNum)
{
}

void CGrass::Event_End(_uint iEventNum)
{
}

HRESULT CGrass::Set_Grass()
{
    GrassTexture* src;

    switch (m_eGrassType)
    {
    case Engine::GRASS_TYPE::JUNGLEBUSH_BRIGHT:
        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushBright"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.f, 0.f };
        src->m_vRenderPos = { 0.f, 0.75f, 0.f };
        src->m_fScale = { 1.08f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushBright"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.3f, -0.21f };
        src->m_vRenderPos = { 0.f, 0.57f, 0.f };
        src->m_fScale = { 0.43f, 0.55f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 1;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushBright"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.39f, -0.21f };
        src->m_vRenderPos = { 0.f, 0.69f, 0.f };
        src->m_fScale = { 0.47f, 0.42f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushBright"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.04f, -0.23f };
        src->m_vRenderPos = { 0.f, 0.38f, 0.f };
        src->m_fScale = { 0.35f, 0.5f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushBright"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.05f, -0.17f };
        src->m_vRenderPos = { 0.f, 1.04f, 0.f };
        src->m_fScale = { 0.95f, 0.56f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 4;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        break;
    case Engine::GRASS_TYPE::JUNGLEBUSH_DARK:

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushDark"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.02f, 0.08f };
        src->m_vRenderPos = { 0.f, 1.f, 0.f };
        src->m_fScale = { 1.f, 0.94f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushDark"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.03f, 0.04f };
        src->m_vRenderPos = { 0.f, 0.42f, 0.f };
        src->m_fScale = { 0.34f, 0.56f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 1;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushDark"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.33f, 0.0f };
        src->m_vRenderPos = { 0.f, 0.57f, 0.f };
        src->m_fScale = { 0.41f, 0.5f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushDark"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.33f, 0.0f };
        src->m_vRenderPos = { 0.f, 0.67f, 0.f };
        src->m_fScale = { 0.37f, 0.41f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushDark"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.37f, 0.01f };
        src->m_vRenderPos = { 0.f, 0.91f, 0.f };
        src->m_fScale = { 0.41f, 0.46f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 4;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushDark"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.36f, 0.02f };
        src->m_vRenderPos = { 0.f, 1.07f, 0.f };
        src->m_fScale = { 0.4f, 0.54f, 1.f };
        src->m_fAngle = 4.4f;
        src->m_fOriginAngle = 4.4f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 5;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_JungleBushDark"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.03f, 0.02f };
        src->m_vRenderPos = { 0.f, 1.28f, 0.f };
        src->m_fScale = { 0.61f, 0.79f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 6;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        break;
    case Engine::GRASS_TYPE::ROUNDBUSH:

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_RoundBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.17f, -0.17f };
        src->m_vRenderPos = { 0.f, 0.36f, 0.f };
        src->m_fScale = { 0.5f, 0.5f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 0.f;
        src->m_iTextureIndex = 4;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_RoundBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.39f, 0.43f };
        src->m_vRenderPos = { 0.f, 0.67f, 0.f };
        src->m_fScale = { 0.42f, 0.72f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_RoundBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.02f, 0.72f };
        src->m_vRenderPos = { 0.f, 0.85f, 0.f };
        src->m_fScale = { 0.71f, 0.89f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 7.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_RoundBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.42f, -0.03f };
        src->m_vRenderPos = { 0.f, 0.52f, 0.f };
        src->m_fScale = { 0.49f, 0.66f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 7.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_RoundBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.25f, 0.18f };
        src->m_vRenderPos = { 0.f, 0.7f, 0.f };
        src->m_fScale = { 0.58f, 0.8f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 1;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);





        break;
    case Engine::GRASS_TYPE::SHARP:

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_Sharp"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.47f, -0.24f };
        src->m_vRenderPos = { 0.f, 0.49f, 0.f };
        src->m_fScale = { 0.41f, 0.65f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_Sharp"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.43f, -0.31f };
        src->m_vRenderPos = { 0.f, 0.72f, 0.f };
        src->m_fScale = { 0.48f, 0.96f, 1.f };
        src->m_fAngle = 1.5f;
        src->m_fOriginAngle = 1.5f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 1;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_Sharp"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.2f, 0.22f };
        src->m_vRenderPos = { 0.f, 0.77f, 0.f };
        src->m_fScale = { 0.47f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_Sharp"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.15f, 0.73f };
        src->m_vRenderPos = { 0.f, 0.77f, 0.f };
        src->m_fScale = { 0.45f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_Sharp"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.53f, 0.66f };
        src->m_vRenderPos = { 0.f, 0.89f, 0.f };
        src->m_fScale = { 0.37f, 0.86f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 1;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_Sharp"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.29f, 0.34f };
        src->m_vRenderPos = { 0.f, 0.68f, 0.f };
        src->m_fScale = { 0.66f, 0.78f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_Sharp"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.f, -0.54f };
        src->m_vRenderPos = { 0.f, 0.67f, 0.f };
        src->m_fScale = { 0.56f, 0.86f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        break;
    case Engine::GRASS_TYPE::HEALTHBUSH:

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_HealthBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.19f, -0.63f };
        src->m_vRenderPos = { 0.f, 0.41f, 0.f };
        src->m_fScale = { 0.4f, 0.57f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_HealthBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.27f, -0.47f };
        src->m_vRenderPos = { 0.f, 0.37f, 0.f };
        src->m_fScale = { 0.36f, 0.51f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 1;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_HealthBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.42f, -0.26f };
        src->m_vRenderPos = { 0.f, 0.7f, 0.f };
        src->m_fScale = { 0.59f, 0.96f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_HealthBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.43f, -0.08f };
        src->m_vRenderPos = { 0.f, 0.77f, 0.f };
        src->m_fScale = { 0.5f, 0.83f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_HealthBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.06f, 0.48f };
        src->m_vRenderPos = { 0.f, 0.79f, 0.f };
        src->m_fScale = { 1.f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 4;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);


        break;
    case Engine::GRASS_TYPE::SPEEDBUSH:

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_SpeedBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.17f, -0.56f };
        src->m_vRenderPos = { 0.f, 0.54f, 0.f };
        src->m_fScale = { 0.55f, 0.72f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 1;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_SpeedBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.25f, -0.19f };
        src->m_vRenderPos = { 0.f, 0.63f, 0.f };
        src->m_fScale = { 0.6f, 0.84f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_SpeedBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.22f, 0.f };
        src->m_vRenderPos = { 0.f, 0.76f, 0.f };
        src->m_fScale = { 0.81f, 1.05f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_SpeedBush"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.f, 0.35f };
        src->m_vRenderPos = { 0.f, 0.57f, 0.f };
        src->m_fScale = { 1.f, 1.11f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        break;
    case Engine::GRASS_TYPE::LONGGRASS:

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_LongGrass"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.14f, 0.2f };
        src->m_vRenderPos = { 0.f, 0.69f, 0.f };
        src->m_fScale = { 0.23f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_LongGrass"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.08f, -0.49f };
        src->m_vRenderPos = { 0.f, 0.52f, 0.f };
        src->m_fScale = { 0.16f, 0.72f, 1.f };
        src->m_fAngle = 11.3f;
        src->m_fOriginAngle = 11.3f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 1;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_LongGrass"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.27f, -0.19f };
        src->m_vRenderPos = { 0.f, 0.55f, 0.f };
        src->m_fScale = { 0.18f, 0.77f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_LongGrass"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.34f, -0.25f };
        src->m_vRenderPos = { 0.f, 0.54f, 0.f };
        src->m_fScale = { 0.17f, 0.78f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_LongGrass"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.5f, 0.13f };
        src->m_vRenderPos = { 0.f, 0.68f, 0.f };
        src->m_fScale = { 0.25f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 4;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_LongGrass"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.16f, 0.38f };
        src->m_vRenderPos = { 0.f, 0.7f, 0.f };
        src->m_fScale = { 0.2f, 1.1f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 5;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_LongGrass"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.45f, 0.05f };
        src->m_vRenderPos = { 0.f, 0.74f, 0.f };
        src->m_fScale = { 0.22f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 6;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_LongGrass"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.06f, 0.f };
        src->m_vRenderPos = { 0.f, 0.65f, 0.f };
        src->m_fScale = { 0.25f, 0.88f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = 0.f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 6;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_LongGrass"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.45f, -0.6f };
        src->m_vRenderPos = { 0.f, 0.81f, 0.f };
        src->m_fScale = { 0.17f, 1.02f, 1.f };
        src->m_fAngle = 3.4f;
        src->m_fOriginAngle = 3.4f;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 1;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_LongGrass"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.54f, -0.64f };
        src->m_vRenderPos = { 0.f, 0.76f, 0.f };
        src->m_fScale = { 0.19f, 1.f, 1.f };
        src->m_fAngle = 1.9f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 1;

        m_GrassList.push_back(src);

        break;
    case Engine::GRASS_TYPE::GLOWING_REED:

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.35f, 0.22f };
        src->m_vRenderPos = { 0.f, 0.71f, 0.f };
        src->m_fScale = { 0.17f, 1.01f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.01f, -0.41f };
        src->m_vRenderPos = { 0.f, 0.69f, 0.f };
        src->m_fScale = { 0.14f, 0.93f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 1;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.35f, -0.04f };
        src->m_vRenderPos = { 0.f, 0.75f, 0.f };
        src->m_fScale = { 0.19f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.6f, 0.f };
        src->m_vRenderPos = { 0.f, 0.7f, 0.f };
        src->m_fScale = { 0.22f, 0.95f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.56f, -0.3f };
        src->m_vRenderPos = { 0.f, 0.7f, 0.f };
        src->m_fScale = { 0.18f, 0.94f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 4;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.14f, 0.3f };
        src->m_vRenderPos = { 0.f, 0.73f, 0.f };
        src->m_fScale = { 0.18f, 0.93f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 1;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.48f, 1.1f };
        src->m_vRenderPos = { 0.f, 0.56f, 0.f };
        src->m_fScale = { 0.25f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 1;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.23f, 0.87f };
        src->m_vRenderPos = { 0.f, 0.69f, 0.f };
        src->m_fScale = { 0.23f, 0.93f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 1;

        m_GrassList.push_back(src);



        break;
    case Engine::GRASS_TYPE::GLOWING_REED_RED:
        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReedRed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.35f, 0.22f };
        src->m_vRenderPos = { 0.f, 0.71f, 0.f };
        src->m_fScale = { 0.17f, 1.01f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReedRed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.01f, -0.41f };
        src->m_vRenderPos = { 0.f, 0.69f, 0.f };
        src->m_fScale = { 0.14f, 0.93f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 1;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReedRed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.35f, -0.04f };
        src->m_vRenderPos = { 0.f, 0.75f, 0.f };
        src->m_fScale = { 0.19f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReedRed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.6f, 0.f };
        src->m_vRenderPos = { 0.f, 0.7f, 0.f };
        src->m_fScale = { 0.22f, 0.95f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReedRed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.56f, -0.3f };
        src->m_vRenderPos = { 0.f, 0.7f, 0.f };
        src->m_fScale = { 0.18f, 0.94f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 4;
        src->m_iIsReverse = 0;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReedRed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.14f, 0.3f };
        src->m_vRenderPos = { 0.f, 0.73f, 0.f };
        src->m_fScale = { 0.18f, 0.93f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 0;
        src->m_iIsReverse = 1;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReedRed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { -0.48f, 1.1f };
        src->m_vRenderPos = { 0.f, 0.56f, 0.f };
        src->m_fScale = { 0.25f, 1.f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 3;
        src->m_iIsReverse = 1;

        m_GrassList.push_back(src);

        src = new GrassTexture;
        src->m_pTexture = dynamic_cast<CTexture*>(Clone_Proto(L"Proto_Tex_Grass_GlowingPlantsReedRed"));
        NULL_CHECK_RETURN(src->m_pTexture, E_FAIL);
        src->m_vPosByCenter = { 0.23f, 0.87f };
        src->m_vRenderPos = { 0.f, 0.69f, 0.f };
        src->m_fScale = { 0.23f, 0.93f, 1.f };
        src->m_fAngle = 0.f;
        src->m_fOriginAngle = src->m_fAngle;
        src->m_fMaxAngle = 3.f;
        src->m_iTextureIndex = 2;
        src->m_iIsReverse = 1;

        m_GrassList.push_back(src);

        break;
    case Engine::GRASS_TYPE::GRASS_END:
        return E_FAIL;
    default:
        return E_FAIL;
    }

    return S_OK;
}
