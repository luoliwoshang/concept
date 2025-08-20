# Go SSA 学习课程

通过一系列课程学习Go编译器的SSA (Static Single Assignment) 中间表示。

## 环境准备
```bash
go mod tidy
go build -o ssa-viewer cmd/ssa-viewer.go
```

## 工具

### 基础工具
- `ssa-viewer` - 查看SSA表示的命令行工具

### 编程分析工具 
- `pkg/ssautil` - SSA分析的Go包，提供编程接口
- 每个lesson的 `cmd/` 目录 - 针对性的分析工具

## 课程目录

### [课程1: 简单函数的SSA表示](lesson-01-simple-add/)
学习最基础的函数调用和算术运算的SSA形式

### [课程2: Map操作的SSA表示](lesson-02-map-operations/)
学习map的创建、读取、写入操作以及多返回值和条件分支的SSA表示
**包含TinyGo编译器真实应用案例** - 展示Referrers在实际编译器中的用途

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

## 编程分析

你可以使用 `pkg/ssautil` 包来编写自己的SSA分析程序：

```go
package main

import (
    "fmt"
    "log"
    "ssa-demo/pkg/ssautil"
)

func main() {
    // 解析Go文件
    analyzer, err := ssautil.ParseFile("example.go")
    if err != nil {
        log.Fatal(err)
    }

    // 找到main函数并分析
    mainFunc := analyzer.FindFunction("main")
    if mainFunc != nil {
        // 查找所有MakeMap指令
        makeMaps := analyzer.FindInstructionsByType(mainFunc, "MakeMap")
        for _, makeMap := range makeMaps {
            // 分析引用者
            referrers := analyzer.AnalyzeReferrers(makeMap.Instruction.(ssa.Value))
            fmt.Printf("MakeMap有%d个引用者\n", len(referrers))
        }
    }
}
```

更多示例请查看 `examples/` 目录。