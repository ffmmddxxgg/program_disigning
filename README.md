# 3D Rubik's Cube

一个基于 **C 语言 + OpenGL** 实现的 3D 魔方程序。项目使用 GLFW 创建窗口与处理输入，使用 GLAD 加载 OpenGL 函数，并通过贴图图集渲染魔方六个面的颜色。程序支持键盘旋转魔方面、鼠标拖拽观察视角、滚轮缩放、复位和旋转音效。

## 项目功能

- 3D 魔方实时渲染
- 支持 3×3×3 魔方结构显示
- 支持六个面的单层旋转
- 支持顺时针 / 逆时针旋转
- 支持旋转动画
- 支持鼠标拖拽切换观察视角
- 支持鼠标滚轮缩放
- 支持一键复位魔方状态
- 支持旋转音效播放
- 使用 UV 贴图图集渲染魔方面颜色

## 技术栈

- C 语言
- OpenGL 3.3 Core Profile
- GLFW
- GLAD
- stb_image
- Windows API
- MSYS2 MinGW64 / GCC

## 项目结构

```text
.
├── main.c              # 程序入口：初始化窗口、OpenGL、魔方、音频和主循环
├── cube.c / cube.h     # 魔方数据结构、状态初始化、旋转逻辑、复位逻辑
├── render.c / render.h # OpenGL 渲染、着色器、贴图加载、绘制魔方
├── input.c / input.h   # 键盘、鼠标、窗口尺寸变化等输入回调
├── animation.c / .h    # 魔方旋转动画逻辑
├── zoom.c / zoom.h     # 鼠标滚轮缩放逻辑
├── math3d.c / .h       # 3D 数学计算、矩阵构建等工具函数
├── audio.c / audio.h   # 旋转音效播放
├── glad.c / glad.h     # OpenGL 函数加载
├── glfw3.h             # GLFW 头文件
├── stb_image.h         # 图片加载库
├── khrplatform.h       # OpenGL 相关平台头文件
└── .vscode/
    ├── tasks.json      # VS Code 编译任务
    └── launch.json     # VS Code 调试配置
```

> 说明：`render_old.c` 是旧版渲染代码，当前编译任务使用的是 `render.c`。

## 资源文件要求

程序运行时会从项目根目录读取以下资源文件：

```text
atlas.png
rubik_cube_rotate.wav
```

其中：

- `atlas.png`：魔方六个面的贴图图集，当前代码按“1 行 6 列”的方式读取 UV 区域。
- `rubik_cube_rotate.wav`：魔方旋转时播放的音效文件。

如果运行后贴图缺失或没有音效，请确认这两个文件位于项目根目录，也就是和 `main.c`、`main.exe` 同一级目录。

## 环境准备

### 1. 安装 MSYS2

建议在 Windows 上使用 MSYS2 MinGW64 环境编译。

安装完成后，确保 GCC 路径类似于：

```text
C:\msys64\mingw64\bin\gcc.exe
```

项目中的 VS Code 编译任务默认使用这个路径。

### 2. 安装 MinGW64 依赖

在 MSYS2 MinGW64 终端中安装 GCC 和 GLFW：

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-glfw
```

如果系统提示需要更新 MSYS2 环境，可以先执行：

```bash
pacman -Syu
```

更新完成后重新打开 MSYS2 MinGW64 终端，再安装依赖。

### 3. 配置 VS Code

建议安装 VS Code 插件：

- C/C++
- CodeLLDB（可选）

项目已经包含 `.vscode/tasks.json` 和 `.vscode/launch.json`，正常情况下可以直接使用 VS Code 的构建与调试功能。

## 编译运行

### 方法一：使用 VS Code 编译运行

1. 用 VS Code 打开项目文件夹。
2. 确认 `C:\msys64\mingw64\bin\gcc.exe` 存在。
3. 按下：

```text
Ctrl + Shift + B
```

4. 编译完成后会在项目根目录生成：

```text
main.exe
```

5. 运行 `main.exe`。

也可以直接按 `F5` 使用 `.vscode/launch.json` 中的调试配置启动程序。

### 方法二：手动命令行编译

在项目根目录执行：

```bash
gcc -g main.c glad.c cube.c input.c math3d.c render.c zoom.c audio.c animation.c \
  -o main.exe \
  -L C:/msys64/mingw64/lib \
  -lglfw3 -lopengl32 -lgdi32 -lwinmm
```

编译成功后运行：

```bash
./main.exe
```

## 操作说明

程序启动后会弹出操作说明窗口。主要控制方式如下：

| 操作 | 功能 |
| --- | --- |
| `Q` / `E` | 旋转上 / 下面 |
| `A` / `D` | 旋转左 / 右面 |
| `W` / `S` | 旋转前 / 后面 |
| `Shift + 旋转键` | 反方向旋转 |
| 鼠标左键拖拽 | 旋转观察视角 |
| 鼠标滚轮 | 缩放魔方 |
| `TAB` | 复位魔方 |
| `F1` | 显示帮助说明 |
| `ESC` | 退出程序 |

## 核心实现说明

### 1. 魔方状态

项目将魔方拆分为 27 个小方块，通过数据结构记录每个小方块的位置、朝向和颜色信息。旋转某一层时，会更新对应小方块的位置和朝向，从而保持魔方状态的连续变化。

### 2. 渲染流程

程序启动后会初始化 GLFW 窗口、加载 GLAD、开启深度测试，并编译顶点着色器和片段着色器。每一帧都会清空画面、更新动画状态、构建投影矩阵与观察矩阵，然后绘制魔方。

### 3. UV 贴图

当前渲染方式不是直接给每个面设置纯色，而是通过 `atlas.png` 贴图图集采样。贴图图集按 1 行 6 列划分，每个颜色占图集宽度的 `1/6`。

### 4. 交互控制

键盘输入由 GLFW key callback 处理。按下 `Q/E/A/D/W/S` 时，程序会根据按键映射判断旋转轴、层和方向；按住 `Shift` 时会反向旋转。鼠标左键拖拽用于更新观察角度，滚轮用于更新缩放距离。

### 5. 音效播放

旋转魔方时会调用 Windows API 的 `PlaySound` 播放 `rubik_cube_rotate.wav`。因此该项目目前主要面向 Windows 环境。

## 常见问题

### 1. 编译时报找不到 `glfw3` 或链接失败

请确认已经安装 MSYS2 MinGW64 版本的 GLFW：

```bash
pacman -S mingw-w64-x86_64-glfw
```

并确认编译命令中包含：

```bash
-lglfw3 -lopengl32 -lgdi32 -lwinmm
```

### 2. 运行后魔方没有贴图

请确认 `atlas.png` 放在项目根目录。程序默认用相对路径读取：

```c
loadCubeTexture("atlas.png");
```

### 3. 旋转时没有声音

请确认 `rubik_cube_rotate.wav` 放在项目根目录。程序默认读取：

```c
#define SOUND_FILE "rubik_cube_rotate.wav"
```

### 4. VS Code 无法编译

检查 `.vscode/tasks.json` 中的 GCC 路径是否和本机一致。默认路径为：

```text
C:\msys64\mingw64\bin\gcc.exe
```

如果 MSYS2 安装在其他目录，需要同步修改 `tasks.json` 和 `launch.json`。

## 后续可改进方向

- 增加随机打乱功能
- 增加自动还原算法
- 增加旋转历史记录与撤销功能
- 增加更完整的 UI 面板
- 增加跨平台构建方式，例如 CMake
- 增加 macOS / Linux 兼容支持
- 增加更精细的光照、阴影和材质效果

## 许可证

当前仓库尚未提供明确的开源许可证。如果需要公开分发或允许他人复用代码，建议后续补充 `LICENSE` 文件。
