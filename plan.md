好——下面给出一个**逐步、可执行、非常细化**的阶段计划（Phase），覆盖你此前列出的目标与目录结构。每个阶段包含：目标、具体任务清单、关键文件变更、测试/验收标准、常见问题及调试建议、以及推荐的 commit 信息（便于你满足“至少 5 个不同日期的 commits”要求）。

> 注意：当出现专业名词时我会先展示英文再解释（例如 `Token（标记）`），按你要求执行。

# Phase 0：准备与初始化（Day 1 — Commit#1）

**目标**：建立项目骨架与构建环境，保证能在 Qt 上编译并运行空窗口。

**要做的事**

1. 创建仓库并初始化 Git：

   * `git init`
   * 添加 `.gitignore`（Qt/CMake 或 qmake 特有项、build 目录等）。
2. 建立目录结构（与你之前给出的一致）：

   * `gui/`, `core/`, `runtime/`
   * `main.cpp`、`CMakeLists.txt` 或 `.pro`
3. 编写最小可运行的 `main.cpp`：

   * 初始化 `QApplication`，创建 `MainWindow`，显示窗口并退出。
4. 创建 `mainwindow.ui`（用 Qt Designer 新建基本窗口）并生成 `mainwindow.h/cpp` 的空壳（信号槽占位）。
5. 编译并运行，确保空界面能弹出。

**关键文件变更**

* `main.cpp`
* `gui/mainwindow.{h,cpp,ui}`
* `CMakeLists.txt` / `.pro`
* `.gitignore`

**验收标准 / 测试**

* 能成功构建并运行，显示主窗口（按钮/文本区可空）。
* 执行 `git log` 有第一条 commit。

**推荐 commit 信息**

* `Initial project skeleton: Qt app, directories, CMake/qmake, main window`

**调试提示**

* 编译问题先看包含路径和 Qt 版本；若使用 CMake，确认 `find_package(Qt6 COMPONENTS Widgets REQUIRED)`（或 Qt5）。
* UI 更改后若编译器报 moc 问题，确认头文件中有 `Q_OBJECT` 的类在 .pro/CMake 中被正确处理。

---

# Phase 1：GUI 完整化与基本交互（Day 2 — Commit#2）

**目标**：实现用户能输入/编辑程序文本、保存/加载文件，以及能触发“Run/Step/Save/Load”的基本 UI 事件。

**要做的事**

1. 在主界面加入控件：

   * 代码输入区：`QTextEdit`（或 `QPlainTextEdit`）
   * 输出/控制台区：`QTextBrowser` / `QPlainTextEdit`（只读）
   * 控制按钮：`Run`, `Step`, `Stop`, `Load`, `Save`, `Clear`
   * 行号显示（可选）或在后期添加。
2. 为按钮建立槽（slot）函数：

   * `onRunClicked()`, `onStepClicked()`, `onLoadClicked()`, `onSaveClicked()`
   * 槽仅做事件转发：例如 `onSaveClicked()` 获取文本并调用 `Program->saveToFile(path)`（若尚未实现，可先保存到临时文件）。
3. 实现文件读写（简单版）：

   * `Program` 类暂可用简单文本保存：每行为 `lineNumber statement`。
4. 状态栏显示当前文件名 / 运行状态。
5. UI 单元测试（手工）：

   * 编辑文本 -> Save -> 清空 -> Load -> 文本应恢复。

**关键文件变更**

* `gui/mainwindow.{h,cpp,ui}`
* 可能添加临时 `core/program.h/cpp` 的简单接口占位

**验收标准 / 测试**

* 可以保存并加载文本文件（.bas 或 .txt）。
* `Run` 等按钮存在并能触发相应槽（目前可打印日志到输出区）。

**推荐 commit 信息**

* `Add main UI: editor, console, control buttons; file load/save hooks`

**调试提示**

* 文件读写异常处理要做：权限、编码（建议使用 UTF-8）。
* 界面线程安全：长时间运行的任务（后续解释器执行）不要阻塞主线程，暂时可以同步执行并在后面优化。

---

# Phase 2：词法分析（Tokenizer）与基础表达式解析（Day 3 — Commit#3）

**目标**：实现 `Tokenizer`（词法分析器）和 `Parser` 的表达式部分，使得单个表达式可以被解析为 AST 并求值（基于简单环境）。

**要做的事**

1. 设计 `Token（标记）` 类型：

   * 类型举例：`NUMBER`, `IDENTIFIER`, `KEYWORD`, `OPERATOR`, `LPAREN`, `RPAREN`, `END`
2. 实现 `tokenizer.h/cpp`：

   * 支持识别整数、变量名（字母开头+字母数字_）、运算符 `+ - * / ^`、括号 `()`
   * 提供 `peek()`、`next()`、`putBack()` 等接口。
3. 设计 `Expression（表达式）` AST 类型（在 `core/exp.h`）：

   * `Exp` 抽象基类
   * `ConstantExp`（常量）
   * `IdentifierExp`（变量引用）
   * `CompoundExp`（二元表达式）
4. 在 `parser.h/cpp` 中实现表达式解析（递归下降）：

   * 规则（示例）：

     * `expression -> term { ('+'|'-') term }`
     * `term -> factor { ('*'|'/') factor }`
     * `factor -> NUMBER | IDENTIFIER | '(' expression ')'`
5. 实现 `Exp::eval(EvalState&)` 的占位（EvalState 尚未完全实现，可用 `std::map<string,int>` 临时实现）。
6. 编写单元测试 / 手工测试：

   * 输入字符串 `"3+4*2"`，parse -> eval -> 输出 `11`
   * `" (1+2) * (3+4) "` -> `21`
   * `"A + 2"` 在未定义 A 时抛出或返回错误（由 EvalState 控制）。

**关键文件变更**

* `runtime/tokenizer.{h,cpp}`
* `runtime/parser.{h,cpp}`
* `core/exp.{h,cpp}`
* 简单 `runtime/evalstate.h/cpp` 占位（key-value map）

**验收标准 / 测试**

* 表达式解析正确（基本四则且支持括号）。
* 错误位置能被基本捕获（如未闭合括号、非法符号）。
* 单元测试覆盖至少这些例子。

**推荐 commit 信息**

* `Implement tokenizer and expression parser (AST + eval), add basic EvalState placeholder`

**调试提示**

* 对于解析错误，打印 **位置/剩余字符串** 便于定位。
* 处理空格与注释（例如用 `REM`）时，Tokenizer 应跳过空白。

---

# Phase 3：语句层实现（Let / Print / END / INPUT 基础）（Day 4 — Commit#4）

**目标**：实现 `Statement` 抽象层及最少两个语句：`LET` 和 `PRINT`，并把它们能从文本行解析成对象。

**要做的事**

1. 设计 `Statement（语句）` 抽象基类（在 `core/statement.h`）：

   * `virtual void execute(EvalState& state) = 0;`
   * `static Statement* parse(string line);`（或 parser 负责）
2. 实现具体语句类：

   * `LetStatement`：`LET var = expression`，执行时在 `EvalState` 中 `setValue(var, value)`
   * `PrintStatement`：`PRINT expression`，执行时把结果写入输出（回调到 GUI 的 console）
   * `EndStatement`：`END`（使程序执行终止）
   * `InputStatement`（可选先做占位）
3. 在 `parser` 中增加语句解析入口：

   * 根据首个 token 判断语句类型（关键字 `LET`, `PRINT`, `END`, `INPUT`）。
4. 连接 AST 与语句：

   * `LetStatement` 持有 `IdentifierExp` 和 `Exp*`（右侧表达式）
   * `PrintStatement` 持有 `Exp*`
5. 实现 `Program` 的存储结构（在 `core/program.h/cpp`）：

   * 使用 `std::map<int, Statement*> lines;`（行号→语句）
   * 提供 `insertLine(int lineno, string content)`、`removeLine(int lineno)`、`run(EvalState&)`
6. 在 GUI 上连接 `Run` 按钮：

   * 从编辑器获取文本（每行可能 `linenumber statement`），先解析并存入 `Program`，再 `program.run()`。
7. 测试用例：

   * 输入：

     ```
     10 LET A = 5
     20 LET B = A * 2 + 3
     30 PRINT B
     40 END
     ```

     运行后 console 应输出 `13`。

**关键文件变更**

* `core/statement.{h,cpp}`
* `core/program.{h,cpp}`
* 修改 `runtime/parser` 用于语句解析
* GUI `mainwindow.cpp`：绑定 `Run` 到 `Program::run`

**验收标准 / 测试**

* 能解析并执行 LET 与 PRINT，并在 GUI 控制台显示正确结果。
* `Program` 能接受多行输入并按行号顺序执行。
* END 能中止执行流程。

**推荐 commit 信息**

* `Add Statement hierarchy and Program storage; implement LET and PRINT execution`

**调试提示**

* 执行流程发生异常时，在 console 输出当前行号与异常原因。
* `Program::run` 应保护死循环（GOTO 未实现前不会发生，但后续实现 GOTO 需谨慎）。

---

# Phase 4：控制流与更多语句（GOTO / IF / INPUT）与单步执行（Day 5 — Commit#5）

**目标**：实现 GOTO、IF...THEN、INPUT 等控制流语句，并支持单步（Step）执行与简单断点。

**要做的事**

1. 实现 `GotoStatement`：

   * `GOTO lineNumber`，执行时改变 `Program` 的当前执行指针。
2. 实现 `IfStatement`：

   * `IF expr1 relop expr2 THEN lineno`（relop: `=`, `<`, `>`, `<=`, `>=`, `<>`）
   * 需要解析比较运算与条件判断。
3. 实现 `InputStatement`：

   * `INPUT var`，在执行时从 GUI 弹出对话框或在 console 等待用户输入。
   * 注意：交互式输入不可阻塞 GUI 主线程，建议用模态对话框或事件循环处理（最简单可用阻塞输入对话框）。
4. `Program` 增强：

   * 维护 `currentLine` 指示器
   * `run()` 支持 while loop，读取 `currentLine` 指向 Statement 执行后更新到下一行或被 GOTO/IF 修改。
   * 支持 `step()`：执行单条语句并返回，使 GUI 可以实现“单步调试”按钮。
5. GUI：实现 Step 按钮逻辑与显示当前行高亮（可选）。
6. 编写测试脚本：

   * 使用 GOTO 实现循环计数，测试 IF 跳转终止条件。
   * 例：

     ```
     10 LET I = 1
     20 IF I > 5 THEN 60
     30 PRINT I
     40 LET I = I + 1
     50 GOTO 20
     60 END
     ```

     输出 1..5

**关键文件变更**

* `core/statement.{h,cpp}`（增加 Goto/If/Input）
* `core/program.{h,cpp}`（增强执行控制）
* `gui/mainwindow.cpp`（Step 绑定、行高亮）

**验收标准 / 测试**

* GOTO 与 IF 正确修改执行流。
* Step 能在每次点击后执行下一条语句并更新 GUI（变量窗口/控制台）。
* INPUT 能正确读取用户输入并赋值到变量表。

**推荐 commit 信息**

* `Add GOTO/IF/INPUT; implement program control flow and single-step execution`

**调试提示**

* GOTO 指向不存在行号时需要报错或中止。
* IF 解析错误时明确指示比较运算符与操作数类型。
* 对于 INPUT，若采用阻塞方式说明可能阻塞 GUI；可在后续阶段把执行放到后台线程并用信号/槽回传输入值。

---

# Phase 5：完善、错误处理、测试、UI 美化与文档（Day 6+ — Commit#6、#7…）

**目标**：完善异常处理、边界情况、添加帮助文档、完善 Git 历史并准备验收材料（至少 5 个不同日期的 commits）。

**要做的事**

1. 错误与异常处理：

   * 语法错误（parse error）、运行时错误（除 0、未定义变量、GOTO 无行号）、用户输入错误。
   * 统一错误类 `InterpreterException`，包含行号与消息。
2. 单元测试：

   * 为 `tokenizer`, `parser`, `exp::eval`, `statement::execute`, `program::run` 写单元测试（可用 GoogleTest 或简单断言脚本）。
3. UI 改进：

   * 行号列（可用 `QPlainTextEdit` + 旁边自绘行号）
   * 当前执行行高亮
   * 变量面板（显示 `EvalState` 当前变量）
   * 日志窗口（错误/运行信息）
4. 性能与线程：

   * 若要支持长时间运行或阻塞 `INPUT`，把 `Program::run` 放到 `QThread` 中执行，用信号将输出回传主线程（GUI）。
5. 文档：

   * `README.md`：项目介绍、如何构建、运行示例、支持的语句与限制。
   * `CONTRIBUTING.md`（可选）。
6. Git 历史准备：

   * 确保在至少 5 个不同日期做 commit（你可以在开发过程中每天完成一个阶段并 commit）。
   * 提交示例：`Refactor: move parser to runtime/`, `Fix: expression precedence bug`, `Add tests for tokenizer` 等。
7. 打包/发布：

   * 生成可执行文件（release），或给出构建步骤。

**关键文件变更**

* 所有模块的完善、测试脚本、文档文件

**验收标准 / 测试**

* 所有主要语句（LET/PRINT/GOTO/IF/INPUT/END）在大多数合理场景下运行正确。
* 错误能被友好提示并记录到日志。
* 提交历史有 5 个不同日期的 commit（可在 `git log --pretty=oneline --date=short` 查看）。

**推荐 commit 信息**

* `Add tests and error handling; document supported language features`
* `Improve UI: variable panel and execution highlight`
* `Refactor runtime to worker thread, add signals for console output`

**调试提示**

* 使用断言与日志（例如 `qDebug()`）来调试执行流。
* 对重要模块（tokenizer/parser）写大量边界测试（空输入、奇怪空白、多重括号、非法标识符）。

---

# 附：每个阶段的关键验收用例（Quick Test Suite）

1. 表达式解析：

   * `"2+3*4"` -> `14`
   * `"(2+3)*4"` -> `20`
2. LET / PRINT：

   * 程序执行后变量值与输出正确（示例在 Phase 3）。
3. GOTO / IF：

   * 循环示例输出 1..N。
4. INPUT：

   * 用户输入 `7` -> 变量被赋值 -> PRINT 输出 `7`
5. 错误处理：

   * `10 LET A =` -> 报语法错误并指出行号
   * `20 GOTO 9999` -> 报错“目标行不存在”

---

# 最后一点建议（实用）

* **小步快跑**：每实现一个可运行的小功能就 commit；这样自然满足多日期 commits 的要求。
* **保持接口清晰**：GUI 仅做展示/事件转发，解析与执行逻辑全放到 `core/` 与 `runtime/`，便于单元测试。
* **先实现同步再优化异步**：一开始可以让 `Program::run` 同步运行，功能稳定后再迁移到 `QThread` 优化。

---

如果你愿意，我可以接下来 **为每个阶段生成具体的模板代码**（例如 `tokenizer.h/cpp`、`exp.h/cpp`、`statement.h` 的骨架），或者直接给出 Phase 3（表达式解析）完整可编译示例。你想先看哪个阶段的代码模板？
