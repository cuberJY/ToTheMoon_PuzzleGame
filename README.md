# To The Moon Puzzle Game

南开大学 2026 C++ 课程大作业

## 游戏简介

灵感来源于游戏 *《To The Moon》* 中的拼图小游戏：每一块拼图碎片都有**正面（图案朝上）**和**反面（空白朝上）**两种状态。玩家每次可以翻转一整行、一整列、或者一条对角线上的所有碎片，目标是**将所有碎片翻转到正面**。

> 其操作机制与经典的 *Lights Out*（熄灯游戏）类似：每次操作会同时改变特定条件下格子的状态（正↔反），可以转换为求解异或方程组。本项目在 *《To The Moon》* 的基础上增加了随机模式、倒计时、连胜统计等扩展玩法。

## 核心特性

- 🎮 **两种游戏模式**：关卡模式（15 关）与随机模式（4 种难度）
- 🔢 **最优解求解**：基于 GF(2) 域高斯-约旦消元 + 位掩码枚举，求出最少步数解
- ⌨️ **键盘操作**：自定义焦点环（Tab 键遍历按钮），支持纯键盘游玩
- 💾 **本地存档**：连胜记录与关卡进度以 JSON 格式自动持久化，退出时保存、启动时恢复
- 🔄 **数据重置**：设置菜单提供一键重置，清空所有本地存档
- 🎨 **翻转动画**：带有淡入淡出和卡片翻转效果的逐格动画
- 🎵 **背景音乐与音效**：内置 *For River* 钢琴曲及操作音效，支持独立开关
- ⚙️ **丰富设置**：BGM/音效/动画特效可随时开关，关卡线性解锁

## 玩法说明

### 关卡模式

共 15 个关卡，难度逐步递增。完成当前关卡后解锁下一关。后期关卡同时限制步数与时间。

### 随机模式

系统随机生成棋盘，玩家需将所有碎片翻转到正面。四种难度：

| 难度 | 棋盘大小 | 对角线 | 步数限制 | 时间限制 |
|------|----------|--------|----------|----------|
| 简单 | 4×4 | 主 | 无 | 30s |
| 普通 | 5×4 | 主 | 无 | 25s |
| 困难 | 6×5 | 主+副 | 最优+2步 | 20s |
| 极限 | 6×6 | 主+副 | 必须最优 | 20s |

成功后计入连胜统计，失败则连胜中断。

## 技术栈

| 类别 | 技术 |
|------|------|
| 语言 | C++17 |
| GUI 框架 | Qt6 (Core / Gui / Widgets / Multimedia) |
| 构建系统 | CMake 3.16+ |
| 数据持久化 | JSON 读写，文件保存在可执行文件同级目录 |
| 架构模式 | MVC（core 层与 GUI 层分离） |

## 项目结构

```
ToTheMoon_PuzzleGame/
├── core/                         # Model 层：纯 C++ 游戏逻辑，不依赖 Qt GUI
│   ├── include/
│   │   ├── PuzzleBoard.h         # 拼图核心（状态、操作、最优解求解）
│   │   ├── GameMode.h            # 抽象游戏模式基类（原型模式）
│   │   ├── RandomMode.h          # 随机模式（简单/普通/困难/极限）
│   │   ├── LevelMode.h           # 关卡模式（15 关）
│   │   └── PlayerData.h          # 玩家数据（连胜、解锁进度）
│   └── src/                      # 对应实现
├── gui/                          # View + Controller 层
│   ├── include/
│   │   ├── MainWindow.h          # 主窗口（界面切换、设置菜单）
│   │   ├── BoardController.h     # 拼图控制器（Model ↔ View 桥接）
│   │   ├── BoardView.h           # 拼图视图（操作、动画、倒计时）
│   │   ├── Home.h                # 主页（随机/关卡模式选择）
│   │   ├── ChooseDiff.h          # 难度选择界面
│   │   ├── ChooseLevel.h         # 关卡选择界面
│   │   └── Tool.h                # 工具类（图片/音频/动画静态方法）
│   └── src/                      # 对应实现 + .ui 文件
└── resources/                    # 图片、音频及 Qt 资源清单
    ├── images/                   # 背景、棋盘拼图、按钮图标
    ├── audio/                    # BGM 与音效
    └── resources.qrc
```

## 构建与运行

### 环境要求

- CMake ≥ 3.16
- Qt 6（含 Multimedia 模块）
- 支持 C++17 的编译器（MSVC / GCC / Clang）

### 安装 Qt 6

**Windows**（推荐通过 Qt 官方在线安装器）：
1. 下载 [Qt Online Installer](https://www.qt.io/download-qt-installer)
2. 安装时勾选 `Qt 6.x.x` → 至少选中 `MSVC 2022 64-bit` 和 `Qt Multimedia`

**Linux**（以 Ubuntu/Debian 为例）：
```bash
sudo apt install qt6-base-dev qt6-multimedia-dev cmake g++
```

**macOS**（通过 Homebrew）：
```bash
brew install qt@6 cmake
```

### 构建步骤

**方式一：命令行（全平台通用）**
```bash
# Debug 构建
mkdir build && cd build
cmake ..
cmake --build .

# Release 构建（优化性能）
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

**方式二：VS Code**（已配置 `.vscode/tasks.json`）
1. 用 VS Code 打开项目根目录
2. 按 `Ctrl+Shift+B` 选择 `CMake: build` 即可编译
3. 按 `F5` 直接运行调试

**方式三：Qt Creator**
1. 打开 Qt Creator → `File` → `Open File or Project...`
2. 选择项目根目录下的 `CMakeLists.txt`
3. 配置 Kit（选择已安装的 Qt 6 + MSVC/GCC/Clang）
4. 点击左下角 ▶ 运行

### 运行

```bash
# 命令行
./build/ToTheMoonPuzzleGame        # Linux / macOS
.\build\ToTheMoonPuzzleGame.exe    # Windows (Debug)
.\build\Release\ToTheMoonPuzzleGame.exe  # Windows (Release)
```

### 常见问题

| 问题 | 解决方案 |
|------|----------|
| `find_package(Qt6)` 找不到 | 设置 `CMAKE_PREFIX_PATH` 指向 Qt6 安装路径，如 `cmake .. -DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/msvc2022_64` |
| `QtMultimedia` 找不到 | 确认安装 Qt 时勾选了 Multimedia 模块，Linux 需额外安装 `qt6-multimedia-dev` |
| 运行时提示缺少 DLL | Windows 下将 Qt 的 `bin` 目录加入环境变量，或使用 `windeployqt` 工具自动复制依赖 |
| 编译 C++17 报错 | 确保编译器支持 C++17（MSVC 2019+、GCC 8+、Clang 7+） |

## 存档说明

游戏数据自动保存在可执行文件所在目录下的 `playerData.json` 中，结构如下：

```json
{
    "currentStreak": 0,
    "maxStreakEasy": 3,
    "maxStreakNormal": 2,
    "maxStreakHard": 5,
    "maxStreakHardcore": 1,
    "maxLevel": 7
}
```

- **currentStreak**：当前连胜次数（失败或查看答案后重置）
- **maxStreakXxx**：各难度下的历史最高连胜
- **maxLevel**：已解锁的最高关卡编号（从 0 开始）

数据在游戏退出时自动保存，启动时自动加载。如需清空所有记录，可在设置菜单中点击「重置」。

## License

本项目为南开大学课程作业，仅供学习交流使用。
