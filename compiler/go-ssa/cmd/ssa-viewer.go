package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"go/types"
	"log"
	"os"
	"reflect"
	"strings"

	"golang.org/x/tools/go/ssa"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("用法: go run ssa-viewer.go <go源文件>")
		fmt.Println("示例: go run ssa-viewer.go 01-simple-add.go")
		os.Exit(1)
	}

	filename := os.Args[1]

	// 1. 解析源文件
	fset := token.NewFileSet()
	file, err := parser.ParseFile(fset, filename, nil, parser.ParseComments)
	if err != nil {
		log.Fatal("解析文件失败:", err)
	}

	// 2. 类型检查
	info := &types.Info{
		Types: make(map[ast.Expr]types.TypeAndValue),
		Defs:  make(map[*ast.Ident]types.Object),
		Uses:  make(map[*ast.Ident]types.Object),
	}
	conf := types.Config{}
	pkg, err := conf.Check("demo", fset, []*ast.File{file}, info)
	if err != nil {
		log.Fatal("类型检查失败:", err)
	}

	// 3. 构建SSA
	prog := ssa.NewProgram(fset, ssa.SanityCheckFunctions)
	ssaPkg := prog.CreatePackage(pkg, []*ast.File{file}, info, true)

	// 4. 构建所有函数的SSA
	ssaPkg.Build()

	fmt.Printf("=== 分析文件: %s ===\n\n", filename)

	// 5. 输出所有函数的SSA
	for _, member := range ssaPkg.Members {
		if fn, ok := member.(*ssa.Function); ok {
			fmt.Printf("函数: %s\n", fn.Name())
			fmt.Printf("签名: %s\n", fn.Signature)
			fmt.Printf("SSA表示:\n")
			fn.WriteTo(os.Stdout)
			
			// 显示详细的指令类型分析
			fmt.Printf("\n--- 指令类型分析 ---\n")
			printDetailedInstructions(fn)
			fmt.Println("\n" + strings.Repeat("=", 60) + "\n")
		}
	}

	// 6. 显示包级别信息
	fmt.Printf("包名: %s\n", ssaPkg.Pkg.Name())
	fmt.Printf("成员数量: %d\n", len(ssaPkg.Members))
}

func printDetailedInstructions(fn *ssa.Function) {
	for i, block := range fn.Blocks {
		fmt.Printf("基本块 %d:\n", i)
		for j, instr := range block.Instrs {
			instrType := reflect.TypeOf(instr).Elem().Name()
			fmt.Printf("  [%d] %s: %s\n", j, instrType, instr.String())
			
			// 对特殊指令类型提供额外说明
			switch v := instr.(type) {
			case *ssa.MakeMap:
				fmt.Printf("      → MakeMap: 创建map，类型=%s\n", v.Type())
			case *ssa.MapUpdate:
				fmt.Printf("      → MapUpdate: 更新map[%s] = %s\n", v.Key, v.Value)
			case *ssa.Lookup:
				fmt.Printf("      → Lookup: 从map查找，key=%s，返回(value, ok)\n", v.Index)
			case *ssa.Extract:
				fmt.Printf("      → Extract: 提取元组第%d个元素\n", v.Index)
			case *ssa.If:
				fmt.Printf("      → If: 条件分支，条件=%s\n", v.Cond)
			case *ssa.Call:
				if v.Call.IsInvoke() {
					fmt.Printf("      → Call: 接口方法调用 %s\n", v.Call.Method)
				} else {
					fmt.Printf("      → Call: 函数调用 %s\n", v.Call.Value)
				}
			case *ssa.BinOp:
				fmt.Printf("      → BinOp: 二元运算 %s %s %s\n", v.X, v.Op, v.Y)
			case *ssa.Return:
				if len(v.Results) > 0 {
					fmt.Printf("      → Return: 返回值 %v\n", v.Results)
				} else {
					fmt.Printf("      → Return: 无返回值\n")
				}
			}
		}
		fmt.Println()
	}
}