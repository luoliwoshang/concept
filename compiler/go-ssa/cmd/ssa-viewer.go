package main

import (
	"fmt"
	"log"
	"os"
	"strings"

	"ssa-demo/pkg/ssautil"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("用法: ssa-viewer <go源文件>")
		fmt.Println("示例: ssa-viewer 01-simple-add.go")
		os.Exit(1)
	}

	filename := os.Args[1]
	
	// 使用SSA分析器
	analyzer, err := ssautil.ParseFile(filename)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("=== 分析文件: %s ===\n\n", filename)

	// 输出所有函数的SSA
	functions := analyzer.GetFunctions()
	for _, fn := range functions {
		analyzer.PrintFunction(fn)
		fmt.Println(strings.Repeat("=", 60) + "\n")
	}

	// 显示包级别信息
	fmt.Printf("包名: %s\n", analyzer.Package.Pkg.Name())
	fmt.Printf("成员数量: %d\n", len(analyzer.Package.Members))
}

