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
```bash
../ssa-viewer 02-map-operations.go
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

## 下一步
学习更复杂的控制流结构，如循环