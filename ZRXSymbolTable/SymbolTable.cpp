#include "pch.h"
#include "SymbolTable.h"
#include "CLayerTableOpts.h"
#include "CTextStyleOpts.h"
#include "CDimStyleOpts.h"

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