target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
  target triple = "arm64-apple-darwin"

  

  define i32 @main(i32 %argc, i8** %argv) {
        ret i32 %argc
  }

  define void @app_main() {
       %argc_val = icmp eq i32 1, 0
	  %argc = zext i1 %argc_val to i32
        %argv = inttoptr i64 0 to i8**
      call i32 @main(i32 %argc, i8** %argv)
      ret void
  }