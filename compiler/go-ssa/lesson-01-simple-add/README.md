# 课程1: 简单函数的SSA表示

## 学习目标
理解最基础的Go函数如何转换为SSA形式

## 源码
```go
package demo

func add(a, b int) int {
	return a + b
}

func main() {
	result := add(5, 3)
	println(result)
}
```

## 查看SSA
```bash
../ssa-viewer 01-simple-add.go
```

## 关键观察点
1. **简单函数**: `add` 函数只有一个基本块，包含加法和返回
2. **函数调用**: `main` 中的 `add(5:int, 3:int)` 调用
3. **临时变量**: SSA中的 `t0`, `t1` 等临时变量
4. **类型信息**: 每个值都有明确的类型标注

## SSA输出解析
- `add` 函数: `t0 = a + b; return t0`
- `main` 函数: `t0 = add(5:int, 3:int); t1 = println(t0); return`
- `init` 函数: 包的初始化函数(自动生成)

## 下一步
学习条件分支如何在SSA中表示