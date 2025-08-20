package ssautil

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"go/types"
	"os"
	"reflect"

	"golang.org/x/tools/go/ssa"
)

// SSAAnalyzer 提供SSA分析功能
type SSAAnalyzer struct {
	Program *ssa.Program
	Package *ssa.Package
	FileSet *token.FileSet
}

// ParseFile 解析Go源文件并构建SSA
func ParseFile(filename string) (*SSAAnalyzer, error) {
	// 1. 解析源文件
	fset := token.NewFileSet()
	file, err := parser.ParseFile(fset, filename, nil, parser.ParseComments)
	if err != nil {
		return nil, fmt.Errorf("解析文件失败: %w", err)
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
		return nil, fmt.Errorf("类型检查失败: %w", err)
	}

	// 3. 构建SSA
	prog := ssa.NewProgram(fset, ssa.SanityCheckFunctions)
	ssaPkg := prog.CreatePackage(pkg, []*ast.File{file}, info, true)
	ssaPkg.Build()

	return &SSAAnalyzer{
		Program: prog,
		Package: ssaPkg,
		FileSet: fset,
	}, nil
}

// GetFunctions 获取所有函数
func (a *SSAAnalyzer) GetFunctions() []*ssa.Function {
	var functions []*ssa.Function
	for _, member := range a.Package.Members {
		if fn, ok := member.(*ssa.Function); ok {
			functions = append(functions, fn)
		}
	}
	return functions
}

// FindFunction 按名字查找函数
func (a *SSAAnalyzer) FindFunction(name string) *ssa.Function {
	for _, fn := range a.GetFunctions() {
		if fn.Name() == name {
			return fn
		}
	}
	return nil
}

// InstructionInfo 指令信息
type InstructionInfo struct {
	Index       int
	Type        string
	Instruction ssa.Instruction
	Description string
}

// AnalyzeFunction 分析函数的所有指令
func (a *SSAAnalyzer) AnalyzeFunction(fn *ssa.Function) [][]InstructionInfo {
	var blocks [][]InstructionInfo
	
	for _, block := range fn.Blocks {
		var instrs []InstructionInfo
		for j, instr := range block.Instrs {
			info := InstructionInfo{
				Index:       j,
				Type:        reflect.TypeOf(instr).Elem().Name(),
				Instruction: instr,
				Description: a.describeInstruction(instr),
			}
			instrs = append(instrs, info)
		}
		blocks = append(blocks, instrs)
	}
	return blocks
}

func (a *SSAAnalyzer) describeInstruction(instr ssa.Instruction) string {
	switch v := instr.(type) {
	case *ssa.MakeMap:
		return fmt.Sprintf("创建map，类型=%s", v.Type())
	case *ssa.MapUpdate:
		return fmt.Sprintf("更新map[%s] = %s", v.Key, v.Value)
	case *ssa.Lookup:
		return fmt.Sprintf("从map查找，key=%s，返回(value, ok)", v.Index)
	case *ssa.Extract:
		return fmt.Sprintf("提取元组第%d个元素", v.Index)
	case *ssa.If:
		return fmt.Sprintf("条件分支，条件=%s", v.Cond)
	case *ssa.Call:
		if v.Call.IsInvoke() {
			return fmt.Sprintf("接口方法调用 %s", v.Call.Method)
		} else {
			return fmt.Sprintf("函数调用 %s", v.Call.Value)
		}
	case *ssa.BinOp:
		return fmt.Sprintf("二元运算 %s %s %s", v.X, v.Op, v.Y)
	case *ssa.Return:
		if len(v.Results) > 0 {
			return fmt.Sprintf("返回值 %v", v.Results)
		} else {
			return "无返回值"
		}
	default:
		return instr.String()
	}
}

// FindInstructionsByType 按类型查找指令
func (a *SSAAnalyzer) FindInstructionsByType(fn *ssa.Function, instrType string) []InstructionInfo {
	var results []InstructionInfo
	blocks := a.AnalyzeFunction(fn)
	
	for blockIndex, block := range blocks {
		for _, info := range block {
			if info.Type == instrType {
				info.Index = blockIndex*1000 + info.Index // 编码block和指令索引
				results = append(results, info)
			}
		}
	}
	return results
}

// ReferrerInfo 引用者信息
type ReferrerInfo struct {
	Instruction ssa.Instruction
	Type        string
	Description string
}

// AnalyzeReferrers 分析一个值的所有引用者
func (a *SSAAnalyzer) AnalyzeReferrers(value ssa.Value) []ReferrerInfo {
	var results []ReferrerInfo
	
	if referrers := value.Referrers(); referrers != nil {
		for _, ref := range *referrers {
			info := ReferrerInfo{
				Instruction: ref,
				Type:        reflect.TypeOf(ref).Elem().Name(),
				Description: a.describeInstruction(ref),
			}
			results = append(results, info)
		}
	}
	
	return results
}

// PrintFunction 打印函数的SSA表示（保持兼容性）
func (a *SSAAnalyzer) PrintFunction(fn *ssa.Function) {
	fmt.Printf("函数: %s\n", fn.Name())
	fmt.Printf("签名: %s\n", fn.Signature)
	fmt.Printf("SSA表示:\n")
	fn.WriteTo(os.Stdout)
	
	fmt.Printf("\n--- 指令类型分析 ---\n")
	blocks := a.AnalyzeFunction(fn)
	for i, block := range blocks {
		fmt.Printf("基本块 %d:\n", i)
		for _, info := range block {
			fmt.Printf("  [%d] %s: %s\n", info.Index, info.Type, info.Instruction.String())
			if info.Description != info.Instruction.String() {
				fmt.Printf("      → %s\n", info.Description)
			}
		}
		fmt.Println()
	}
}