# ZRXSymbolTable 编译问题修复记录

## 项目背景

本项目是基于中望 CAD（ZWCAD）ObjectARX SDK（ZRX SDK）开发的插件 DLL，使用 MFC 框架，功能为操作符号表（Symbol Table）中的图层表（Layer Table）。

---

## 遇到的编译/链接错误及根本原因

### 错误一：`IUnknown` 未声明（C2061 / C2065）

```
zacpublishuireactors.h: error C2061: 语法错误: 标识符"IUnknown"
zacprofile.h: error C2061: 语法错误: 标识符"IUnknown"
```

**根本原因**：ZWCAD SDK 的部分头文件（如 `zacpublishuireactors.h`）中使用了 COM 接口 `IUnknown`，该类型定义在 `<unknwn.h>` 中。由于在包含这些 SDK 头文件之前，`IUnknown` 尚未声明，导致连锁报错。

这是 **错误三** 的次生问题，解决头文件顺序后自然消失。

---

### 错误二：MFC 与 `Windows.h` 包含冲突（C1189 fatal error）

```
afxv_w32.h: fatal error C1189: WINDOWS.H already included.
MFC apps must not #include <Windows.h>
```

**根本原因**：MFC 有一条强制规则——**MFC 头文件必须在 `<Windows.h>` 之前被包含**，由 MFC 自己负责管理 `Windows.h` 的引入。

触发冲突的路径有两条：

1. `framework.h` 中定义了 `WIN32_LEAN_AND_MEAN`，随后 ATL 的 `<atlstr.h>` 被包含，而 `<atlstr.h>` 不受 MFC 管理，会直接 `#include <Windows.h>`，导致后续 `<afx.h>` 报错。

2. `SymbolTable.h` 直接包含了 `<aced.h>`（ZWCAD SDK 头文件），该文件内部会包含 `<Windows.h>`。当 `SymbolTable.cpp` 先包含 `SymbolTable.h`，再间接包含 MFC 时，MFC 看到 `Windows.h` 已存在，触发报错。

---

### 错误三：`dllmain.cpp` 中 Windows 类型未定义（C4430 / C2146）

```
dllmain.cpp: error C4430: 缺少类型说明符
dllmain.cpp: error C2146: 语法错误: 缺少";"(在标识符"APIENTRY"的前面)
```

**根本原因**：`dllmain.cpp` 只包含了 `pch.h`，而 `pch.h` → `framework.h` 里没有包含任何实际的头文件（仅有一个宏定义），导致 `BOOL`、`HMODULE`、`APIENTRY` 等 Windows 类型全部未定义。

---

### 错误四：`CStdioFile::ReadString` 参数类型不匹配（C2664）

```
CLayerTableOpts.cpp: error C2664: "BOOL CStdioFile::ReadString(CString &)":
无法将参数 1 从"ZcString"转换为"CString &"
```

**根本原因**：MFC 的 `CStdioFile::ReadString` 只接受 `CString&` 类型的参数，而代码中传入的是 ZWCAD SDK 的 `AcString`（即 `ZcString`），两者不兼容，无法隐式转换。

---

### 错误五：`DllMain` 重复定义（LNK2005 / LNK1169）

```
dllmain.obj : error LNK2005: DllMain 已经在 mfcs140u.lib(dllmodul.obj) 中定义
fatal error LNK1169: 找到一个或多个多重定义的符号
```

**根本原因**：引入 MFC（`<afxwin.h>`）后，MFC 运行时库（`mfcs140u.lib`）已经内置了一个 `DllMain` 实现，用于 MFC DLL 的初始化。而 `dllmain.cpp` 中又手动定义了一个 `DllMain`，造成符号重复定义。

对于 ZRXSDK 插件，DLL 的应用层入口是 `acrxEntryPoint`（已在 `SymbolTable.cpp` 中实现），不需要也不能自定义 `DllMain`。

---

## 修复方案

### 修改 1：`framework.h` — 删除 `WIN32_LEAN_AND_MEAN`

**修改前：**
```cpp
#pragma once
#define WIN32_LEAN_AND_MEAN   // 从 Windows 头文件中排除极少使用的内容
```

**修改后：**
```cpp
#pragma once
```

> `WIN32_LEAN_AND_MEAN` 会裁剪部分 MFC 依赖的 Windows API，且会导致 ATL 等非 MFC 库绕过 MFC 直接包含 `Windows.h`，MFC 项目中禁止使用。

---

### 修改 2：`pch.h` — 加入 `<afxwin.h>`

**修改前：**
```cpp
#include "framework.h"
```

**修改后：**
```cpp
#include "framework.h"
#include <afxwin.h>   // MFC 核心，必须在所有 Windows/SDK 头文件之前包含
```

> `pch.h` 是所有 `.cpp` 文件的统一预编译头入口。在这里加入 `<afxwin.h>` 后，所有包含 `pch.h` 的文件（包括 `dllmain.cpp`）都能在 SDK 头文件之前完成 MFC 初始化，同时获得 Windows 类型定义。

---

### 修改 3：`SymbolTable.h` — 删除直接包含的 SDK 头文件

**修改前：**
```cpp
#pragma once
#include <aced.h>
#include <rxregsvc.h>
```

**修改后：**
```cpp
#pragma once
```

> `.h` 头文件中不应直接包含 ZWCAD SDK 的头文件，因为这会让包含该头文件的任何 `.cpp` 文件提前引入 `Windows.h`，破坏 MFC 的包含顺序。SDK 头文件统一由 `stdArx.h` 引入。

---

### 修改 4：`CLayerTableOpts.cpp` — `ReadString` 用 `CString` 中转

**修改前：**
```cpp
AcString strLineText;
while (f.ReadString(strLineText))
```

**修改后：**
```cpp
CString strLineTextCS;
while (f.ReadString(strLineTextCS))
{
    AcString strLineText(strLineTextCS);  // 在循环体内转换为 AcString
    ...
}
```

> `CStdioFile::ReadString` 只接受 `CString&`。先读入 `CString`，再在循环体内构造 `AcString`，两者可以通过隐式转换兼容。

---

### 修改 5：删除 `dllmain.cpp`

直接从项目中移除 `dllmain.cpp`。

> MFC DLL 的 `DllMain` 由 MFC 运行时（`mfcs140u.lib`）统一提供，插件只需实现 `acrxEntryPoint`。自定义 `DllMain` 会与 MFC 产生链接冲突。

---

## 正确的头文件包含结构

修复后，项目的头文件包含层次如下：

```
pch.h
├── framework.h          （空，仅 #pragma once）
└── afxwin.h             （MFC 核心，统一管理 Windows.h）

stdArx.h
├── pch.h                （MFC 已在此初始化）
├── afx.h
├── atlstr.h
├── aced.h               （ZWCAD SDK，此时 Windows.h 已由 MFC 包含，不冲突）
├── rxregsvc.h
├── arxHeaders.h
├── dbents.h
└── unknwn.h             （COM 接口）

CLayerTableOpts.h
└── stdArx.h

SymbolTable.h            （不包含任何 SDK/MFC 头文件）
```

**核心原则：MFC 头文件必须是整个项目中第一个被包含的 Windows 相关头文件。**

---

## 注意事项

1. **MFC DLL 项目禁止定义 `DllMain`**，MFC 运行时已提供，重复定义会导致链接失败。

2. **禁止在 `.h` 头文件中直接包含 ZWCAD/ObjectARX SDK 头文件**，应统一收口到 `stdArx.h` 或 `pch.h` 中管理。

3. **`WIN32_LEAN_AND_MEAN` 不适用于 MFC 项目**，仅适用于纯 Win32 API 项目。

4. **ATL 头文件（如 `<atlstr.h>`）不受 MFC 的 `Windows.h` 管理机制约束**，必须放在 `<afx.h>` 之后包含。

5. **`CStdioFile` 等 MFC 文件 I/O 类只接受 `CString`**，不直接兼容 `AcString`/`ZcString`，需要显式转换。

6. **链接警告 `LNK4099`（找不到 PDB）** 是 ZWCAD SDK 第三方库的问题，不影响生成结果，可以忽略。
