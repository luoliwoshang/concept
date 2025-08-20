# 课程2: Map操作的SSA表示

## 学习目标
理解Go中map操作如何在SSA中表示，包括map的创建、读取、写入和存在性检查

## 源码
```go
package demo

func getFromMap(m map[string]int, key string) int {
	value, exists := m[key]
	if exists {
		return value
	}
	return -1
}

func setToMap(m map[string]int, key string, value int) {
	m[key] = value
}

func main() {
	// 创建map
	scores := make(map[string]int)
	
	// 设置值
	setToMap(scores, "alice", 95)
	setToMap(scores, "bob", 87)
	
	// 获取值
	aliceScore := getFromMap(scores, "alice")
	charlieScore := getFromMap(scores, "charlie") // 不存在的key
	
	println("Alice's score:", aliceScore)
	println("Charlie's score:", charlieScore)
}
```

## 查看SSA

### 基础SSA查看
```bash
../ssa-viewer 02-map-operations.go
```

### 专门的Map引用分析
```bash
# 分析MakeMap的所有引用者 - 特别适合理解map的使用模式
go build -o analyze-referrers cmd/analyze-referrers.go
./analyze-referrers 02-map-operations.go
./analyze-referrers tinygo-style.go  # 分析TinyGo编译器案例
```

## 关键观察点

### 1. Map创建
- `make(map[string]int)` 在SSA中的表示
- 内存分配和初始化

### 2. Map读取 (`getFromMap`函数)
- `value, exists := m[key]` 的双返回值
- 条件分支基于存在性检查
- 两个不同的返回路径

### 3. Map写入 (`setToMap`函数)
- `m[key] = value` 的SSA表示
- 引用传递的体现

### 4. 控制流
- `if exists` 创建的基本块分支
- 不同返回路径的汇合

## SSA中的重要概念
1. **MapAccess**: map读取操作的SSA指令
2. **MapUpdate**: map写入操作的SSA指令
3. **MakeMap**: map创建的SSA指令
4. **多返回值**: Go特有的多返回值在SSA中的处理
5. **条件分支**: 基于布尔值的控制流

## 学习重点
- 观察map操作如何转换为SSA指令
- 理解多返回值的SSA表示
- 分析条件分支在SSA中的基本块结构

## 实际应用案例：TinyGo编译器

`tinygo-style.go` 展示了一个真实的编译器应用场景 - TinyGo如何在编译时分析map的使用模式。

### TinyGo的AsmFull函数
TinyGo编译器需要处理内联汇编，用户这样调用：
```go
regs := make(map[string]interface{})
regs["value"] = 42
regs["result"] = 100
AsmFull("mov {value}, {result}", regs)
```

### 编译器的SSA分析过程
1. **找到MakeMap指令** - 定位到register map的创建
2. **分析所有Referrers** - 通过 `registerMap.Referrers()` 找到所有使用
3. **提取MapUpdate操作** - 获取所有的 `map["key"] = value` 操作
4. **生成LLVM约束** - 将SSA分析结果转换为LLVM内联汇编

### 用我们的工具分析
```bash
./analyze-referrers tinygo-style.go
```

这个例子完美展示了SSA Referrers在实际编译器中的应用！

## 下一步
学习更复杂的控制流结构，如循环