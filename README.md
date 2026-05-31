# Qt-TextEdit

基于 Qt 6 的轻量级纯文本编辑器，提供类似记事本的基础编辑体验。

## 功能特性

- **新建 / 打开 / 保存 / 另存为** — 支持 `.txt` 文件的完整文件操作
- **字体设置** — 通过字体对话框自定义编辑区字体
- **插入时间日期** — 一键插入 `yyyy-MM-dd hh:mm` 格式的当前时间
- **状态栏实时信息** — 显示当前时间、文件路径、光标行/列位置
- **关闭前确认** — 内容已修改时提示是否保存

## 快捷键

| 操作 | 快捷键 |
|------|--------|
| 新建文件 | `Ctrl+N` |
| 打开文件 | `Ctrl+O` |
| 保存 | `Ctrl+S` |
| 另存为 | `Ctrl+Shift+S` |

## 构建要求

- **Qt 6.5** 或更高版本
- **CMake 3.19** 或更高版本
- Qt 模块：`Core`、`Widgets`

## 构建步骤

```bash
cmake -B build -DCMAKE_PREFIX_PATH=/path/to/Qt/6.x/gcc_64
cmake --build build
```

将 `/path/to/Qt/6.x/gcc_64` 替换为你的 Qt 安装路径。Windows 下使用 MSVC 或 MinGW 工具链时路径类似：

```bash
cmake -B build -DCMAKE_PREFIX_PATH=C:/Qt/6.x.x/msvc2022_64
cmake --build build --config Release
```

## 项目结构

```
Qt-TextEdit/
  CMakeLists.txt      # CMake 构建配置
  main.cpp            # 程序入口
  mainwindow.h        # MainWindow 类声明
  mainwindow.cpp      # MainWindow 类实现（核心逻辑）
  mainwindow.ui       # Qt Designer 界面文件（占位用）
  rec.qrc             # Qt 资源文件
  rec/
    windowIcon.jpeg   # 应用图标
```

## 界面语言

界面为简体中文，包括菜单项、对话框提示和状态栏文字。

## License

本项目暂未指定开源许可证。
