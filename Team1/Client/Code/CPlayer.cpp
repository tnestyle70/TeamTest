#include "pch.h"
#include "CPlayer.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CPlayer::CPlayer(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	return iExit;
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);

	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlayer::Render_GameObject()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	m_pGraphicDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHAREF, 0xc0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// TriCol
	pComponent = m_pBufferCom = dynamic_cast<CRcCol*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcCol"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// RcCol
	pComponent = m_pTransformCom = dynamic_cast<CTransform*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));

	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{
	if (GetAsyncKeyState('Q'))
		m_pTransformCom->Rotation(ROT_X, 180.f * fTimeDelta);

	if (GetAsyncKeyState('A'))
		m_pTransformCom->Rotation(ROT_X, -180.f * fTimeDelta);

	if (GetAsyncKeyState('W'))
		m_pTransformCom->Rotation(ROT_Y, 180.f * fTimeDelta);

	if (GetAsyncKeyState('S'))
		m_pTransformCom->Rotation(ROT_Y, -180.f * fTimeDelta);

	if (GetAsyncKeyState('E'))
		m_pTransformCom->Rotation(ROT_Z, 180.f * fTimeDelta);

	if (GetAsyncKeyState('D'))
		m_pTransformCom->Rotation(ROT_Z, -180.f * fTimeDelta);

	_vec3	vUp;
	m_pTransformCom->Get_Info(INFO_UP, &vUp);

	if (GetAsyncKeyState(VK_UP))
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), 10.f, fTimeDelta);

	if (GetAsyncKeyState(VK_DOWN))
		m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vUp, &vUp), -10.f, fTimeDelta);

}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pBackGround = new CPlayer(pGraphicDev);

	if (FAILED(pBackGround->Ready_GameObject()))
	{
		Safe_Release(pBackGround);
		MSG_BOX("pBackGround Create Failed");
		return nullptr;
	}

	return pBackGround;
}

void CPlayer::Free()
{
	CGameObject::Free();
}
