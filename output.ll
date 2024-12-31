; ModuleID = 'test'
source_filename = "test"

define i32 @main() {
entry:
  %temp = alloca i32, align 4
  %digitCount = alloca i32, align 4
  %isEven5 = alloca i32, align 4
  %isEven = alloca i32, align 4
  %i = alloca i32, align 4
  %factorial = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, ptr %n, align 4
  store i32 1, ptr %factorial, align 4
  store i32 1, ptr %i, align 4
  br label %loop

loop:                                             ; preds = %loopbody, %entry
  %loadLeftTmp = load i32, ptr %i, align 4
  %loadRightTmp = load i32, ptr %n, align 4
  %letmp = icmp sle i32 %loadLeftTmp, %loadRightTmp
  br i1 %letmp, label %loopbody, label %loopexit

loopbody:                                         ; preds = %loop
  %loadLeftTmp1 = load i32, ptr %factorial, align 4
  %loadRightTmp2 = load i32, ptr %i, align 4
  %multmp = mul i32 %loadLeftTmp1, %loadRightTmp2
  store i32 %multmp, ptr %factorial, align 4
  %loadLeftTmp3 = load i32, ptr %i, align 4
  %addtmp = add i32 %loadLeftTmp3, 1
  store i32 %addtmp, ptr %i, align 4
  br label %loop

loopexit:                                         ; preds = %loop
  %loadLeftTmp4 = load i32, ptr %factorial, align 4
  %modtmp = srem i32 %loadLeftTmp4, 2
  %eqtmp = icmp eq i32 %modtmp, 0
  br i1 %eqtmp, label %then, label %else

then:                                             ; preds = %loopexit
  store i32 1, ptr %isEven, align 4
  br label %ifcont

else:                                             ; preds = %loopexit
  store i32 0, ptr %isEven5, align 4
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  store i32 0, ptr %digitCount, align 4
  %temp6 = load i32, ptr %factorial, align 4
  store i32 %temp6, ptr %temp, align 4
  br label %loop7

loop7:                                            ; preds = %loopbody8, %ifcont
  %loadLeftTmp10 = load i32, ptr %temp, align 4
  %gttmp = icmp sgt i32 %loadLeftTmp10, 0
  br i1 %gttmp, label %loopbody8, label %loopexit9

loopbody8:                                        ; preds = %loop7
  %loadLeftTmp11 = load i32, ptr %temp, align 4
  %divtmp = sdiv i32 %loadLeftTmp11, 10
  store i32 %divtmp, ptr %temp, align 4
  %loadLeftTmp12 = load i32, ptr %digitCount, align 4
  %addtmp13 = add i32 %loadLeftTmp12, 1
  store i32 %addtmp13, ptr %digitCount, align 4
  br label %loop7

loopexit9:                                        ; preds = %loop7
  %loadLeftTmp14 = load i32, ptr %digitCount, align 4
  %gttmp15 = icmp sgt i32 %loadLeftTmp14, 2
  br i1 %gttmp15, label %then16, label %else17

then16:                                           ; preds = %loopexit9
  %loadtmp = load i32, ptr %factorial, align 4
  ret i32 %loadtmp
  br label %ifcont18

else17:                                           ; preds = %loopexit9
  br label %ifcont18

ifcont18:                                         ; preds = %else17, %then16
  %loadtmp19 = load i32, ptr %digitCount, align 4
  ret i32 %loadtmp19
}
