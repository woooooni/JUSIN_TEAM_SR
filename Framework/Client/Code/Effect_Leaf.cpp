#include "..\Header\Effect_Leaf.h"
#include "Particle_FixedLeaf.h"
#include "Particle_MovingLeaf.h"
#include "Pool.h"
#include "Export_Function.h"

CEffect_Leaf::CEffect_Leaf(LPDIRECT3DDEVICE9 pGraphicDev)
    :CEffect(pGraphicDev)
{
}

CEffect_Leaf::CEffect_Leaf(const CEffect& rhs)
    : CEffect(rhs)
{
}

CEffect_Leaf::~CEffect_Leaf()
{
}

HRESULT CEffect_Leaf::Ready_Object(void)
{
    Set_Active(false);

    return S_OK;
}

_int CEffect_Leaf::Update_Object(const _float& fTimeDelta)
{
    Set_Active(false);
    return 0;
}

void CEffect_Leaf::LateUpdate_Object(void)
{
}

void CEffect_Leaf::Render_Object(void)
{
}

CEffect_Leaf* CEffect_Leaf::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CEffect_Leaf* pInstance = new CEffect_Leaf(pGraphicDev);

    if (FAILED(pInstance->Ready_Object()))
    {
        Safe_Release(pInstance);

        MSG_BOX("Effect_Leaf_Create_Failed");
        return nullptr;
    }

    return pInstance;
}


void CEffect_Leaf::Get_Effect(_vec3& _vPos, _vec3& _vScale, _uint _iCount)
{
    _uint iFixedCount = _iCount * 0.8f;
    _uint iMovingCount = _iCount - iFixedCount;
    int iTemp;

    for (_uint i = 0; iFixedCount > i; ++i)
    {
        _vec3 vPos;

        vPos.x = (rand() % (_uint)(_vScale.x * 100.0f)) * 0.005f;
        vPos.y = (rand() % (_uint)(_vScale.y * 100.0f)) * 0.005f;
        vPos.z = (rand() % 1000) * 0.00001f;

        iTemp = rand() % 2;
        if (iTemp == 1)
            vPos.x *= -1.0f;

        iTemp = rand() % 2;
        if (iTemp == 1)
            vPos.y *= -1.0f;

        vPos += _vPos;

        CGameObject* pLeaf = CPool<CParticle_FixedLeaf>::Get_Obj();
        if (pLeaf)
        {
            dynamic_cast<CParticle_FixedLeaf*>(pLeaf)->Random_Particle(vPos);
            pLeaf->Set_Active(true);
            Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"FixedLeaf", pLeaf);
        }
        else
        {
            pLeaf = dynamic_cast<CParticle_FixedLeaf*>(pLeaf)->Create(Engine::Get_Device());
            if (pLeaf)
            {
                pLeaf->Set_Active(true);
                dynamic_cast<CParticle_FixedLeaf*>(pLeaf)->Random_Particle(vPos);
                Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"FixedLeaf", pLeaf);
            }
        }


    }

    for (_uint i = 0; iMovingCount > i; ++i)
    {
        _vec3 vPos;

        vPos.x = (rand() % (_uint)(_vScale.x * 100.0f)) * 0.005f;
        vPos.y = (rand() % (_uint)(_vScale.y * 100.0f)) * 0.005f;
        vPos.z = (rand() % 1000) * 0.00001f;

        iTemp = rand() % 2;
        if (iTemp == 1)
            vPos.x *= -1.0f;

        vPos.y *= 0.5f;

        vPos += _vPos;

        CGameObject* pLeaf = CPool<CParticle_MovingLeaf>::Get_Obj();
        if (pLeaf)
        {
            dynamic_cast<CParticle_MovingLeaf*>(pLeaf)->Random_Particle(vPos);
            pLeaf->Set_Active(true);
            Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"MovingLeaf", pLeaf);
        }
        else
        {
            pLeaf = dynamic_cast<CParticle_MovingLeaf*>(pLeaf)->Create(Engine::Get_Device());
            if (pLeaf)
            {
                pLeaf->Set_Active(true);
                dynamic_cast<CParticle_MovingLeaf*>(pLeaf)->Random_Particle(vPos);
                Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"MovingLeaf", pLeaf);
            }
        }
    }

}

HRESULT CEffect_Leaf::Add_Component(void)
{
    return S_OK;
}

void CEffect_Leaf::Free()
{
    __super::Free();
}
