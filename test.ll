declare i32 @printf(i8*, ...)

@fmt = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %argc_val = icmp eq i32 0, 0
  %argc = zext i1 %argc_val to i32
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fmt, i64 0, i64 0), i32 %argc)
  ret i32 0
}