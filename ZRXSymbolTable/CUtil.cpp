#include "pch.h"
#include "CUtil.h"

// 解析LayerMessage
bool CUtil::ParseLayerMessage(const AcString lineStr, AcStringArray& fields) {
    fields.removeAll();
    int nPos = 0;
    AcString str(lineStr); // 用AcString替代CString
    AcString token;
    int curPos = 0;
    while ((token = str.tokenize(_T(","), curPos)) != _T("")) {
        fields.append(token);
    }
    return (fields.length() > 0);
}