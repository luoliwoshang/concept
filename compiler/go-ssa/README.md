# Go SSA 学习课程

通过一系列课程学习Go编译器的SSA (Static Single Assignment) 中间表示。

## 环境准备
```bash
go mod tidy
go build -o ssa-viewer cmd/ssa-viewer.go
```

## 工具
- `ssa-viewer` - 使用 `go/ssa` 包查看SSA表示的可执行工具

## 课程目录

### [课程1: 简单函数的SSA表示](lesson-01-simple-add/)
学习最基础的函数调用和算术运算的SSA形式

### [课程2: Map操作的SSA表示](lesson-02-map-operations/)
学习map的创建、读取、写入操作以及多返回值和条件分支的SSA表示

### 即将推出
- 课程3: 循环结构 (for)
- 课程4: 复杂控制流
- 课程5: 接口和方法调用

## 使用方法
每个课程目录包含：
- 源码文件
- 详细的README说明
- 关键观察点和学习目标

在每个课程目录中运行：
```bash
../ssa-viewer [源码文件]
```