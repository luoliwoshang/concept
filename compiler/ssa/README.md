# SSA 核心哲学：盲目性与约定机制

基于对TinyGo内联汇编机制的深入调研，揭示SSA (Static Single Assignment) 形式的核心设计哲学。

## 核心发现：Go函数签名在SSA阶段"不可见"

### 关键问题
**在SSA指令实现阶段，无法获取Go函数签名中的返回值信息**。

```go
// Go函数签名
func device.Asm(instruction string)  // 看起来无返回值

// 但在SSA阶段的指令实现中：
func implementDeviceAsm(call *ssa.CallCommon) llvm.Value {
    // ❌ 无法直接获取到Go函数的返回值类型！
    // ❌ 无法知道调用者期望什么返回值！
    // ❌ 只能看到SSA层面的调用结构！
}
```

### TinyGo的解决方案：约定机制

```go
// compiler/inlineasm.go - 实际实现
func (c *compilerContext) createInlineAssembly(call *ssa.CallCommon) llvm.Value {
    // 约定1：假设函数无返回值（基于函数名识别）
    // 约定2：直接生成void类型的内联汇编
    return c.builder.CreateCall(
        llvm.InlineAsm(..., instruction, "sideeffect"),
        []llvm.Value{}, // 无返回值约定
    )
}
```

## SSA盲目性原则

### 1. 指令实现的完全盲目性

每个SSA指令在实现时完全"看不到"上层语义：

```llvm
; SSA指令实现者只能看到：
%0 = call void @device.Asm(ptr @.str.wfi)

; 完全看不到：
; - Go函数的原始签名
; - 用户的使用意图  
; - 函数的语义含义
; - 返回值的预期用途
```

### 2. 从TinyGo看盲目性的完整流程

#### 阶段1：Go源码 → 初始SSA（盲目翻译）
```go
// 用户代码
device.Asm("wfi")

// 编译器盲目按语法翻译，不理解语义
```

#### 阶段2：初始LLVM IR（仍然盲目）
```llvm
; 编译器只能生成通用的函数调用
call void @device.Asm(ptr @.str.wfi)
; 此时完全不知道这个调用的特殊性！
```

#### 阶段3：约定识别（特殊处理）
```llvm
; 特殊pass识别函数名约定，转换为：
call void asm sideeffect "wfi", ""()
; 这时才"理解"了语义！
```

## 约定机制：连接盲目组件的纽带

### 问题：既然都是盲目的，如何协同工作？
**答案：分层约定系统**

### 约定层次1：语法约定
```go
// 约定：特定包名+函数名表示内联汇编
device.Asm("instruction")
```

### 约定层次2：编译器识别约定
```go
// compiler/compiler.go:1865
if functionName == "device.Asm" {
    // 约定：转交给内联汇编处理器
    return createInlineAssembly(call)
}
```

### 约定层次3：IR生成约定
```llvm
; 约定：生成特定格式的内联汇编IR
call void asm sideeffect "INSTRUCTION", ""()
```

### 约定层次4：后端代码约定
```assembly
; 约定：直接嵌入汇编指令
wfi
```

## 为什么Go函数签名在SSA阶段不可见

### 1. 信息丢失的必然性
```go
// Go层面：
func Add(a, b int) int { return a + b }

// SSA层面只剩下：
%result = call i32 @Add(i32 %a, i32 %b)
;                 ↑ 只有LLVM类型，没有Go语义
```

### 2. 类型系统的转换
```go
// Go类型系统 → LLVM类型系统的映射是有损的：
Go: func(string) error  → LLVM: ptr @func(ptr) ptr
Go: func() (int, bool) → LLVM: {i32, i1} @func()

// 原始的Go函数签名信息丢失！
```

### 3. 优化的需要
```llvm
; 如果保留Go函数签名信息，会阻碍优化：
; 优化器需要自由重写函数调用，不应被上层语义束缚
%inlined = add i32 %a, %b  ; 原来是函数调用，被内联了
```

## 实际例子：返回值处理的盲目性

### 问题场景
```go
// 如果device.Asm需要返回值怎么办？
func GetProcessorID() uint32 {
    // 假设需要内联汇编返回处理器ID
    return device.AsmWithReturn("mrs x0, mpidr_el1")  // 假设的函数
}
```

### SSA实现的困境
```go
func implementAsmWithReturn(call *ssa.CallCommon) llvm.Value {
    // ❌ 问题：无法从函数名得知返回值类型！
    // ❌ 问题：无法知道应该生成什么类型的内联汇编！
    
    // 只能依赖约定：
    // 约定1：从SSA调用的目标类型推断
    targetType := call.Type()  // 只能获取SSA层面的类型
    
    // 约定2：生成对应类型的内联汇编
    return c.builder.CreateCall(
        llvm.InlineAsm(targetType, "mrs $0, mpidr_el1", "=r"),
        []llvm.Value{},
    )
}
```

## 核心洞察：约定 vs 依赖

### 传统方式（依赖）
```c
// ❌ 如果指令需要"知道"上下文
void generate_call(Context* ctx, Function* func, Call* call) {
    if (func->go_signature.return_type == "error") {
        // 生成错误处理代码
    }
    // 需要依赖上层语义信息！
}
```

### SSA方式（约定）
```c
// ✅ 完全盲目的指令生成
SSAValue* generate_call(SSAFunction* target, SSAValue* args[]) {
    // 只管生成调用，不管语义
    return create_call_instruction(target, args);
    
    // 完全不需要知道：
    // - Go函数的原始签名
    // - 返回值的Go类型
    // - 错误处理约定
}
```

## 盲目性带来的好处

### 1. 极致的优化自由度
```llvm
; 优化器可以自由变换，因为不受上层语义约束：
; 原始：
%result = call i32 @Add(i32 %a, i32 %b)

; 优化后：
%result = add i32 %a, %b  ; 直接内联，函数调用消失
```

### 2. 平台无关性
```llvm
; 相同的SSA表示在不同平台生成不同代码：
call void asm sideeffect "wfi", ""()

; ARM64: wfi
; RISC-V: wfi  
; x86: hlt
```

### 3. 语言无关性
```llvm
; 相同的SSA可以来自不同语言：
%result = add i32 %a, %b

; 可能来自：Go的 a + b
; 可能来自：C的 a + b  
; 可能来自：Rust的 a + b
; SSA层面完全一致！
```

## 总结

**关键发现**：
1. **Go函数签名在SSA阶段确实获取不到** - 这是设计特性，不是缺陷
2. **盲目性是SSA的核心哲学** - 让每个组件专注于自己的职责
3. **约定机制连接盲目组件** - 通过分层约定实现协同工作

**TinyGo的device.Asm完美诠释了这个原则**：
- 编译器无法从函数名获取Go层面的类型信息
- 只能通过约定（函数名识别）进行特殊处理
- 生成的代码完全基于约定，而非依赖上层语义

**这就是现代编译器的核心智慧**：通过盲目性获得灵活性，通过约定确保正确性！