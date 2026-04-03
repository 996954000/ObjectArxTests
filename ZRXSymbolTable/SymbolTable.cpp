#include "pch.h"
#include "SymbolTable.h"
#include "CLayerTableOpts.h"
#include "CTextStyleOpts.h"
#include "CDimStyleOpts.h"
#include "CViewTableOpts.h"
#include "CViewPortOpts.h"
#include "CUCSOpts.h"

void AddLayerTable()
{
	CLayerTableOpts::AddLayerTable();
}

void SetLayerColor()
{
	CLayerTableOpts::SetLayerColor();
}

void DeleteLayer(){
	CLayerTableOpts::DeleteLayer();
}

void ExportLayerMessage(){
	CLayerTableOpts::ExportLayerMessage();
}

void ImportLayerMessage(){
	CLayerTableOpts::ImportLayerMessage();
}

void AddTextStyle(){
	CTextStyleOpts::AddTextStyle();
}
void AddDimStyle(){
	CDimStyleOpts::AddDimStyle();
}

void ZoomCurViewScale(){
	CViewTableOpts::ZoomScale();
}	

void CreateNewUCS(){
	CUCSOpts::CreateNewUCS();
}

void SetCurUCS(){
	CUCSOpts::SetCurUCS();
}

void MoveUCSOrigin(){
	CUCSOpts::MoveUCSOrigin();
}

void AddEntityInUCS(){
	CUCSOpts::AddEntityInUCS();
}

void Create4ViewPorts(){
	CViewPortOpts::Create4ViewPorts();
}

void initApp()
{
	acutPrintf(L"\n[SymbolTable] initApp called, registering command...");
	int retAddLayer = acedRegCmds->addCommand(L"SymTablesOperation", L"AddLayerTable",
		L"AddLayerTable",
		ACRX_CMD_MODAL,
		AddLayerTable);
	acutPrintf(L"\n[AddLayerTable] addCommand result: %d", retAddLayer);

	int retSetLayerColor = acedRegCmds->addCommand(L"SymTablesOperation", L"SetLayerColor",
		L"SetLayerColor",
		ACRX_CMD_MODAL,
		SetLayerColor);
	acutPrintf(L"\n[SetLayerColor] addCommand result: %d", retSetLayerColor);

	int retDeleteLayer = acedRegCmds->addCommand(L"SymTablesOperation", L"DeleteLayer",
		L"DeleteLayer",
		ACRX_CMD_MODAL,
		DeleteLayer);
	acutPrintf(L"\n[DeleteLayer] addCommand result: %d", retDeleteLayer);

	int retExportLayerMessage = acedRegCmds->addCommand(L"SymTablesOperation", L"ExportLayerMessage",
		L"ExportLayerMessage",
		ACRX_CMD_MODAL,
		ExportLayerMessage);
	acutPrintf(L"\n[ExportLayerMessage] addCommand result: %d", retExportLayerMessage);

	int retImportLayerMessage = acedRegCmds->addCommand(L"SymTablesOperation", L"ImportLayerMessage",
		L"ImportLayerMessage",
		ACRX_CMD_MODAL,
		ImportLayerMessage);
	acutPrintf(L"\n[ImportLayerMessage] addCommand result: %d", retImportLayerMessage);

	int retAddTextStyle = acedRegCmds->addCommand(L"SymTablesOperation", L"AddTextStyle",
		L"AddTextStyle",
		ACRX_CMD_MODAL,
		AddTextStyle);
	acutPrintf(L"\n[AddTextStyle] addCommand result: %d", retAddTextStyle);

	int retAddDimStyle = acedRegCmds->addCommand(L"SymTablesOperation", L"AddDimStyle",
		L"AddDimStyle",
		ACRX_CMD_MODAL,
		AddDimStyle);
	acutPrintf(L"\n[AddDimStyle] addCommand result: %d", retAddDimStyle);

	int retZoomCurViewScale = acedRegCmds->addCommand(L"SymTablesOperation", L"ZoomCurViewScale",
		L"ZoomCurViewScale",
		ACRX_CMD_MODAL,
		ZoomCurViewScale);
	acutPrintf(L"\n[ZoomCurViewScale] addCommand result: %d", retZoomCurViewScale);

	// UCS相关命令
	int retCreateNewUCS = acedRegCmds->addCommand(L"SymTablesOperation", L"CreateNewUCS",
		L"CreateNewUCS",
		ACRX_CMD_MODAL,
		CreateNewUCS);
	acutPrintf(L"\n[CreateNewUCS] addCommand result: %d", retCreateNewUCS);

	int retSetCurUCS = acedRegCmds->addCommand(L"SymTablesOperation", L"SetCurUCS",
		L"SetCurUCS",
		ACRX_CMD_MODAL,
		SetCurUCS);
	acutPrintf(L"\n[SetCurUCS] addCommand result: %d", retSetCurUCS);

	int retMoveUCSOrigin = acedRegCmds->addCommand(L"SymTablesOperation", L"MoveUCSOrigin",
		L"MoveUCSOrigin",
		ACRX_CMD_MODAL,
		MoveUCSOrigin);
	acutPrintf(L"\n[MoveUCSOrigin] addCommand result: %d", retMoveUCSOrigin);

	int retAddEntityInUCS = acedRegCmds->addCommand(L"SymTablesOperation", L"AddEntityInUCS",
		L"AddEntityInUCS",
		ACRX_CMD_MODAL,
		AddEntityInUCS);
	acutPrintf(L"\n[AddEntityInUCS] addCommand result: %d", retAddEntityInUCS);

	int retCreate4ViewPorts = acedRegCmds->addCommand(L"SymTablesOperation", L"CreateViewPorts",
		L"CreateViewPorts",
		ACRX_CMD_MODAL,
		Create4ViewPorts);
	acutPrintf(L"\n[Create4ViewPorts] addCommand result: %d", retCreate4ViewPorts);
}

void unloadApp()
{
	//删除命令组
	acedRegCmds->removeGroup(L"SymTablesOperation");
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