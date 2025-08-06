; ModuleID = 'busy_wait_demo_volatile.c'
source_filename = "busy_wait_demo_volatile.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

@flag = global i32 0, align 4
@counter = local_unnamed_addr global i32 0, align 4
@.str.3 = private unnamed_addr constant [30 x i8] c"\E4\BB\8D\E5\9C\A8\E7\AD\89\E5\BE\85... (\E8\AE\A1\E6\95\B0: %d)\0A\00", align 1
@.str.4 = private unnamed_addr constant [41 x i8] c"flag\E5\8F\98\E4\B8\BA1\E4\BA\86\EF\BC\8C\E6\80\BB\E5\85\B1\E5\BE\AA\E7\8E\AF\E4\BA\86 %d \E6\AC\A1\0A\00", align 1
@str = private unnamed_addr constant [33 x i8] c"\0A\E4\BF\A1\E5\8F\B7\E5\A4\84\E7\90\86\E5\99\A8: \E8\AE\BE\E7\BD\AEflag\E4\B8\BA1\00", align 1
@str.5 = private unnamed_addr constant [30 x i8] c"\E6\8C\89 Ctrl+C \E6\9D\A5\E8\A7\A6\E5\8F\91\E4\BF\A1\E5\8F\B7...\00", align 1
@str.6 = private unnamed_addr constant [33 x i8] c"\E7\A8\8B\E5\BA\8F\E6\AD\A3\E5\9C\A8\E7\AD\89\E5\BE\85flag\E5\8F\98\E4\B8\BA1...\00", align 1

; Function Attrs: nofree nounwind ssp uwtable(sync)
define void @signal_handler(i32 %0) #0 {
  store volatile i32 1, ptr @flag, align 4, !tbaa !6
  %2 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: nounwind ssp uwtable(sync)
define noundef i32 @main() local_unnamed_addr #2 {
  %1 = tail call ptr @signal(i32 noundef 2, ptr noundef nonnull @signal_handler) #5
  %2 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.5)
  %3 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.6)
  %4 = load volatile i32, ptr @flag, align 4, !tbaa !6
  %5 = icmp eq i32 %4, 0
  %6 = load i32, ptr @counter, align 4, !tbaa !6
  br i1 %5, label %7, label %22

7:                                                ; preds = %0, %18
  %8 = phi i32 [ %19, %18 ], [ %6, %0 ]
  %9 = add nsw i32 %8, 1
  store i32 %9, ptr @counter, align 4, !tbaa !6
  %10 = freeze i32 %9
  %11 = sdiv i32 %10, 100000000
  %12 = mul i32 %11, 100000000
  %13 = sub i32 %10, %12
  %14 = icmp eq i32 %13, 0
  br i1 %14, label %15, label %18

15:                                               ; preds = %7
  %16 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.3, i32 noundef %11)
  %17 = load i32, ptr @counter, align 4, !tbaa !6
  br label %18

18:                                               ; preds = %15, %7
  %19 = phi i32 [ %17, %15 ], [ %9, %7 ]
  %20 = load volatile i32, ptr @flag, align 4, !tbaa !6
  %21 = icmp eq i32 %20, 0
  br i1 %21, label %7, label %22, !llvm.loop !10

22:                                               ; preds = %18, %0
  %23 = phi i32 [ %6, %0 ], [ %19, %18 ]
  %24 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.4, i32 noundef %23)
  ret i32 0
}

declare ptr @signal(i32 noundef, ptr noundef) local_unnamed_addr #3

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #4

attributes #0 = { nofree nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #1 = { nofree nounwind "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #2 = { nounwind ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #3 = { "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }
attributes #4 = { nofree nounwind }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 15, i32 5]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"Homebrew clang version 19.1.7"}
!6 = !{!7, !7, i64 0}
!7 = !{!"int", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
!10 = distinct !{!10, !11}
!11 = !{!"llvm.loop.mustprogress"}
