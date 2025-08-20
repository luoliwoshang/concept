package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"go/types"
	"log"
	"os"
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
			fmt.Println("\n" + strings.Repeat("=", 60) + "\n")
		}
	}

	// 6. 显示包级别信息
	fmt.Printf("包名: %s\n", ssaPkg.Pkg.Name())
	fmt.Printf("成员数量: %d\n", len(ssaPkg.Members))
}