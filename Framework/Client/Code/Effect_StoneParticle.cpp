#include "Effect_StoneParticle.h"
#include "CParticle_Stone.h"

#include "Pool.h"
#include "Export_Function.h"

CEffect_StoneParticle::CEffect_StoneParticle(LPDIRECT3DDEVICE9 pGraphicDev)
	:CEffect(pGraphicDev)
{
}

CEffect_StoneParticle::CEffect_StoneParticle(const CEffect& rhs)
	: CEffect(rhs)
{
}

CEffect_StoneParticle::~CEffect_StoneParticle()
{
}

HRESULT CEffect_StoneParticle::Ready_Object(void)
{
	Set_Active(false);

	return S_OK;
}

_int CEffect_StoneParticle::Update_Object(const _float& fTimeDelta)
{
	return 0;
}

void CEffect_StoneParticle::LateUpdate_Object(void)
{
}

void CEffect_StoneParticle::Render_Object(void)
{
}

CEffect_StoneParticle* CEffect_StoneParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CEffect_StoneParticle* pInstance = new CEffect_StoneParticle(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Effect_StoneParticle_Create_Failed");
		return nullptr;
	}

	return pInstance;
}

void CEffect_StoneParticle::Get_Effect(_vec3& _vPos, _vec3& _vScale, _uint _iCount)
{
    _uint iStoneCount = _iCount;
    int iTemp;

    _vScale = _vScale * 0.01f;

    for (_uint i = 0; iStoneCount > i; ++i)
    {
        _vec3 vPos;

        vPos.x = (rand() % 100) * _vScale.x;
        vPos.y = (rand() % 100) * _vScale.y;
        vPos.z = (rand() % 100) * 0.0001f;

        iTemp = rand() % 2;
        if (iTemp == 1)
            vPos.x *= -1.0f;

        iTemp = rand() % 2;
        if (iTemp == 1)
            vPos.y *= -1.0f;

        vPos += _vPos;

        CGameObject* pStone = CPool<CParticle_Stone>::Get_Obj();
        if (pStone)
        {
            dynamic_cast<CParticle_Stone*>(pStone)->Random_Particle(vPos);
            pStone->Set_Active(true);
            Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"StoneParticle", pStone);
        }
        else
        {
            pStone = dynamic_cast<CParticle_Stone*>(pStone)->Create(Engine::Get_Device());
            if (pStone)
            {
                pStone->Set_Active(true);
                dynamic_cast<CParticle_Stone*>(pStone)->Random_Particle(vPos);
                Engine::Get_Layer(LAYER_TYPE::EFFECT)->Add_GameObject(L"StoneParticle", pStone);
            }
        }


    }
}

HRESULT CEffect_StoneParticle::Add_Component(void)
{
	return S_OK;
}

void CEffect_StoneParticle::Free()
{
	__super::Free();
}
