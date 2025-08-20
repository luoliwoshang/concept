package main

import (
	"fmt"
	"log"
	"os"
	"strings"

	"ssa-demo/pkg/ssautil"
	"golang.org/x/tools/go/ssa"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("用法: go run cmd/analyze-referrers.go <go源文件>")
		fmt.Println("示例: go run cmd/analyze-referrers.go 02-map-operations.go")
		os.Exit(1)
	}

	filename := os.Args[1]
	
	// 解析文件
	analyzer, err := ssautil.ParseFile(filename)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Printf("=== 分析 %s 的Referrers ===\n\n", filename)

	// 找到所有函数
	functions := analyzer.GetFunctions()
	for _, fn := range functions {
		fmt.Printf("分析函数: %s\n", fn.Name())
		
		// 查找所有 MakeMap 指令
		makeMaps := analyzer.FindInstructionsByType(fn, "MakeMap")
		for _, mapInstr := range makeMaps {
			makeMap := mapInstr.Instruction.(*ssa.MakeMap)
			fmt.Printf("\n找到 MakeMap: %s\n", makeMap.String())
			
			// 分析这个MakeMap的所有引用者
			referrers := analyzer.AnalyzeReferrers(makeMap)
			fmt.Printf("引用者数量: %d\n", len(referrers))
			
			for i, ref := range referrers {
				fmt.Printf("  [%d] %s: %s\n", i, ref.Type, ref.Instruction.String())
				if ref.Description != ref.Instruction.String() {
					fmt.Printf("      → %s\n", ref.Description)
				}
			}
		}
		
		fmt.Println(strings.Repeat("-", 50))
	}
}