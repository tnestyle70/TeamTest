#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CInputMgr : public CBase
{
	DECLARE_SINGLETON(CInputMgr)
private:
	explicit CInputMgr();
	virtual ~CInputMgr();
public:
	void Initialize();
private:
	virtual void Free();
};

END