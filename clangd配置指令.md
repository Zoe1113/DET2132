# 为 Sonix SN8 嵌入式工程配置 clangd 跳转（可复用指令）

> 适用：使用 Sonix SN8C 编译器（sn8pc）的 C 工程，因 sfr/sbit/bit/__asm/__interrupt 等非标准语法导致 clangd"转到定义"失效。
> 原理：clangd 后端是 clang，不认识这些语法，会把芯片头当语法错误 → 整个工程符号索引失败。方案用 `__clangd__` 宏隔离：编译走原语法，clangd 走兼容版。

---

## 第 1 步：收集工程信息

1. Read 工程根的 `makefile`，提取：
   - **编译器 include 目录**：`-I` 标志里指向 sn8pc 安装目录的路径（通常含 `SN8P*.h` 和 `stdlib.h`，路径形如 `...\SN8_C_~1\data\C\include`）
   - **项目 include 目录**：如 `-I.\Include`、`-ISrc` 等
   - **全局宏**：所有 `-D` 标志（如 `-DICE_Mode=0`、`-DSN8P2988=1`）
   - **芯片型号**：从 `-target=SN8Pxxxx` 或 `SN8Pxxxx=1` 宏提取
2. Glob 确认芯片头文件名（`SN8Pxxxx.h`）在项目 Include 目录或编译器 include 目录的位置
3. Glob 检查是否已存在 `.clangd` / `compile_flags.txt` / `compile_commands.json`（有则先备份再覆盖）
4. Read 项目**总头文件**（通常 `Include/Include.h` 或 `main.h`——即 `#include` 了芯片头和其他所有 .h 的那个），记下 `#include "SN8Pxxxx.h"` 所在行
5. Grep 统计 `.c` 中 Sonix 关键字使用情况：`__asm`、`interrupt`、`__interrupt`、`sfr`、`sbit`、`bit`、`data`、`idata`、`xdata`、`pdata`、`code`、`using`、`at`、`reentrant`（用于决定兼容宏覆盖范围）

## 第 2 步：生成 clangd 兼容版芯片头

1. 在工程根创建 `ClangdCompat/` 目录
2. 把原芯片头 `SN8Pxxxx.h` 内容复制为 `ClangdCompat/SN8Pxxxx_clangd.h`
3. 用 Edit `replace_all` 做以下转换（顺序无关，互不冲突）：
   - `sfr\t` → `extern volatile unsigned char `
   - `sbit\t` → `extern volatile unsigned char `
   - `:0;` → `;`
   - `:1;` → `;`
   - `:2;` → `;`
   - `:3;` → `;`
   - `:4;` → `;`
   - `:5;` → `;`
   - `:6;` → `;`
   - `:7;` → `;`
   - `__asm{` → `(void)0 /*`
   - `}` → `*/`（**替换前必须 Grep 确认 `}` 仅出现在 `__asm{}` 宏内**，否则跳过此步并逐行处理宏）
4. Read 文件开头和结尾验证：无残留 `sfr`/`sbit`/`__asm{`/`:N;`

## 第 3 步：创建全局兼容宏头

创建 `ClangdCompat/clangd_compat.h`：

```c
#ifndef CLANGD_COMPAT_H
#define CLANGD_COMPAT_H
/* clangd 索引兼容头：把 Sonix 非标准关键字映射为标准 C，不参与固件编译 */
#define sfr     extern volatile unsigned char
#define sbit    extern volatile unsigned char
#define bit     unsigned char
#define __asm
#define __interrupt
#define interrupt
#define data
#define idata
#define xdata
#define pdata
#define code
#endif
```

若第 1 步发现其他 Sonix 关键字（`using`/`at`/`reentrant` 等），补充对应 `#define`。

## 第 4 步：在总头文件加守卫

把总头文件里的 `#include "SN8Pxxxx.h"` 改为：

```c
#ifdef __clangd__
#include "SN8Pxxxx_clangd.h"
#else
#include "SN8Pxxxx.h"
#endif
```

（sn8pc 编译时不定义 `__clangd__`，走原版，固件编译完全不受影响）

## 第 5 步：创建 compile_flags.txt（工程根）

```
-IClangdCompat
-I<项目Include目录>
-I<编译器include目录>
-D<宏1>=<值>
-D<宏2>=<值>
-D__clangd__=1
-include
ClangdCompat/clangd_compat.h
```

- 路径相对工程根；编译器短名路径（如 `SN8_C_~1`）可直接用，避免空格
- `-D__clangd__=1` 和 `-include` 两行**必须**有

## 第 6 步：创建 .clangd（工程根）

```yaml
Diagnostics:
  ClangTidy:
    Remove: '*'
  UnusedIncludes: None
```

## 第 7 步：清理与交付

- 删除任何损坏的遗留 clangd 配置文件（如只有孤立 `#endif` 的半成品）
- 告知用户：重启 clangd（`Ctrl+Shift+P` → `clangd: Restart Language Server`），等 background index 完成（1~3 分钟）
- 告知维护规则：新增普通函数/变量/文件无需改配置；仅新增 include 目录、全局条件编译宏、或未映射的 Sonix 关键字时才需更新对应文件

---

## 常见坑

- **sbit 的 `addr:bit` 冒号语法纯宏无法处理**，必须物理改写芯片头（第 2 步的 `:N;`→`;` 替换）
- **`}` → `*/` 替换前务必 Grep 确认** `}` 仅在 `__asm{}` 宏内，否则误伤
- 若 `.vscode` 可写，可加 `settings.json`：`{"clangd.arguments": ["--background-index"]}` 确保跨文件跳转到 .c 定义
- 中断函数 `__interrupt[0x08]` 的方括号修饰符 clangd 解析可能不完美，该函数本身跳转或许受影响，不影响同文件其他函数
