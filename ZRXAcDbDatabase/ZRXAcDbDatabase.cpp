#include "pch.h"
#include "ZRXAcDbDatabase.h"
#include "CDatabaseOpts.h"
#include "CDataTrans.h"
#include "CTransactionOpts.h"

void CreateDWG()
{
	CDatabaseOpts::CreateDWG();
}

void ReadDWG() {
	CDatabaseOpts::ReadDWG();
}

void ImportBlk(){
	CDataTrans::ImportBlk();
}

void CreateBlock() {
	CTransactionOpts::CreateBlock();
}
void EditBlock() {
	CTransactionOpts::EditBlock();
}

void initApp()
{
	acutPrintf(L"\n[ZRXAcDbDatabase] initApp called, registering command...");
	int createDWGret = acedRegCmds->addCommand(L"DatabaseOpts", L"CreateDWG",
		L"CreateDWG",
		ACRX_CMD_MODAL,
		CreateDWG);
	acutPrintf(L"\n[ZRXAcDbDatabase] addCommand result: %d", createDWGret);

	int readDWGret = acedRegCmds->addCommand(L"DatabaseOpts", L"ReadDWG",
		L"ReadDWG",
		ACRX_CMD_MODAL,
		ReadDWG);
	acutPrintf(L"\n[ZRXAcDbDatabase] addCommand result: %d", readDWGret);

	int importBlkret = acedRegCmds->addCommand(L"DatabaseOpts", L"ImportBlk",
		L"ImportBlk",
		ACRX_CMD_MODAL,
		ImportBlk);
	acutPrintf(L"\n[ZRXAcDbDatabase] addCommand result: %d", importBlkret);

	int createBlockret = acedRegCmds->addCommand(L"TransactionOpts", L"CreateBlock",
		L"CreateBlock",
		ACRX_CMD_MODAL,
		CreateBlock);
	acutPrintf(L"\n[ZRXAcDbDatabase] addCommand result: %d", createBlockret);

	int editBlockret = acedRegCmds->addCommand(L"TransactionOpts", L"EditBlock",
		L"EditBlock",
		ACRX_CMD_MODAL,
		EditBlock);
	acutPrintf(L"\n[ZRXAcDbDatabase] addCommand result: %d", editBlockret);
}

void unloadApp()
{
	//删除命令组
	acedRegCmds->removeGroup(L"Hello1");
}

extern "C" AcRx::AppRetCode
acrxEntryPoint(AcRx::AppMsgCode msg, void* pkt)
{
	switch (msg)
	{
	case AcRx::kInitAppMsg:
		acrxDynamicLinker->unlockApplication(pkt);
		acrxRegisterAppMDIAware(pkt);
		initApp();
		break;
	case AcRx::kUnloadAppMsg:
		unloadApp();
		break;
	default:
		break;
	}
	return AcRx::kRetOK;
}