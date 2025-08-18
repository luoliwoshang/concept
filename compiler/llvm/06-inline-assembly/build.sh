#!/bin/bash
# LLVM内联汇编教学示例编译脚本

CC="clang"
CFLAGS="-O1 -g -Wall"
SOURCES=(01-basic.c 02-operands.c 03-arm64-specific.c 04-constraints.c)

# 颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 帮助信息
show_help() {
    echo "LLVM内联汇编教学脚本"
    echo ""
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  build       - 编译所有示例程序"
    echo "  test        - 运行所有示例程序"
    echo "  llvm-ir     - 生成LLVM IR文件 (*.ll)"
    echo "  asm         - 生成汇编文件 (*.s)"
    echo "  clean       - 清理生成的文件"
    echo "  help        - 显示此帮助信息"
    echo ""
    echo "示例文件:"
    echo "  01-basic.c         - 基础内联汇编"
    echo "  02-operands.c      - 操作数占位符"
    echo "  03-arm64-specific.c - ARM64特定指令"
    echo "  04-constraints.c   - 约束类型详解"
}

# 编译所有示例
build_all() {
    echo -e "${BLUE}=== 编译所有示例程序 ===${NC}"
    
    for source in "${SOURCES[@]}"; do
        target="${source%.c}"
        echo -e "${YELLOW}编译: $source -> $target${NC}"
        
        if $CC $CFLAGS -o "$target" "$source"; then
            echo -e "${GREEN}✓ 编译成功: $target${NC}"
        else
            echo -e "${RED}✗ 编译失败: $source${NC}"
            return 1
        fi
        echo ""
    done
    
    echo -e "${GREEN}=== 所有程序编译完成 ===${NC}"
}

# 运行所有测试
run_tests() {
    echo -e "${BLUE}=== 运行所有内联汇编测试 ===${NC}"
    
    for source in "${SOURCES[@]}"; do
        target="${source%.c}"
        
        if [[ -f "$target" ]]; then
            echo -e "${YELLOW}--- 运行: $target ---${NC}"
            ./"$target"
            echo ""
        else
            echo -e "${RED}✗ 程序不存在: $target (请先运行 build)${NC}"
            return 1
        fi
    done
    
    echo -e "${GREEN}=== 所有测试完成 ===${NC}"
}

# 生成LLVM IR文件
generate_llvm_ir() {
    echo -e "${BLUE}=== 生成LLVM IR文件 ===${NC}"
    
    for source in "${SOURCES[@]}"; do
        ll_file="${source%.c}.ll"
        echo -e "${YELLOW}生成: $source -> $ll_file${NC}"
        
        if $CC -S -emit-llvm "$source" -o "$ll_file"; then
            echo -e "${GREEN}✓ 生成成功: $ll_file${NC}"
        else
            echo -e "${RED}✗ 生成失败: $source${NC}"
            return 1
        fi
    done
    
    echo -e "${GREEN}=== LLVM IR文件生成完成 ===${NC}"
}

# 生成汇编文件
generate_asm() {
    echo -e "${BLUE}=== 生成汇编文件 ===${NC}"
    
    for source in "${SOURCES[@]}"; do
        asm_file="${source%.c}.s"
        echo -e "${YELLOW}生成: $source -> $asm_file${NC}"
        
        if $CC -S "$source" -o "$asm_file"; then
            echo -e "${GREEN}✓ 生成成功: $asm_file${NC}"
        else
            echo -e "${RED}✗ 生成失败: $source${NC}"
            return 1
        fi
    done
    
    echo -e "${GREEN}=== 汇编文件生成完成 ===${NC}"
}

# 查看内联汇编代码
check_inline_asm() {
    echo -e "${BLUE}=== 检查生成的内联汇编 ===${NC}"
    
    # 检查汇编文件中的内联汇编
    for source in "${SOURCES[@]}"; do
        asm_file="${source%.c}.s"
        if [[ -f "$asm_file" ]]; then
            echo -e "${YELLOW}=== $asm_file 中的内联汇编 ===${NC}"
            grep -A3 -B1 "#APP\|#NO_APP" "$asm_file" || echo "未找到内联汇编标记"
            echo ""
        fi
    done
}

# 检查LLVM IR中的内联汇编
check_llvm_inline_asm() {
    echo -e "${BLUE}=== 检查LLVM IR中的内联汇编 ===${NC}"
    
    for source in "${SOURCES[@]}"; do
        ll_file="${source%.c}.ll"
        if [[ -f "$ll_file" ]]; then
            echo -e "${YELLOW}=== $ll_file 中的内联汇编 ===${NC}"
            grep -A2 -B1 "call.*asm" "$ll_file" || echo "未找到内联汇编"
            echo ""
        fi
    done
}

# 清理生成的文件
clean_all() {
    echo -e "${BLUE}=== 清理生成的文件 ===${NC}"
    
    # 清理可执行文件
    for source in "${SOURCES[@]}"; do
        target="${source%.c}"
        if [[ -f "$target" ]]; then
            rm "$target"
            echo -e "${GREEN}✓ 删除: $target${NC}"
        fi
    done
    
    # 清理LLVM IR文件
    rm -f *.ll
    echo -e "${GREEN}✓ 删除: *.ll${NC}"
    
    # 清理汇编文件
    rm -f *.s
    echo -e "${GREEN}✓ 删除: *.s${NC}"
    
    # 清理目标文件
    rm -f *.o
    echo -e "${GREEN}✓ 删除: *.o${NC}"
    
    echo -e "${GREEN}=== 清理完成 ===${NC}"
}

# 主函数
main() {
    case "$1" in
        "build")
            build_all
            ;;
        "test")
            run_tests
            ;;
        "llvm-ir")
            generate_llvm_ir
            ;;
        "asm")
            generate_asm
            ;;
        "check-asm")
            generate_asm
            check_inline_asm
            ;;
        "check-llvm-ir")
            generate_llvm_ir
            check_llvm_inline_asm
            ;;
        "clean")
            clean_all
            ;;
        "help"|"")
            show_help
            ;;
        *)
            echo -e "${RED}未知选项: $1${NC}"
            echo ""
            show_help
            exit 1
            ;;
    esac
}

# 运行主函数
main "$@"