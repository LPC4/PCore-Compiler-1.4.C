; ModuleID = 'sinCalculator'
source_filename = "sinCalculator"

define float @castIntToFloat(i32 %iNum) {
entry:
  %fNum = alloca float, align 4
  %iNum.addr = alloca i32, align 4
  store i32 %iNum, ptr %iNum.addr, align 4
  %fNum1 = load i32, ptr %iNum.addr, align 4
  %fNum.castIntToFloatTmp = sitofp i32 %fNum1 to float
  store float %fNum.castIntToFloatTmp, ptr %fNum, align 4
  %loadTmp = load float, ptr %fNum, align 4
  ret float %loadTmp
}

define i32 @castFloatToInt(float %fNum) {
entry:
  %iNum = alloca i32, align 4
  %fNum.addr = alloca float, align 4
  store float %fNum, ptr %fNum.addr, align 4
  %iNum1 = load float, ptr %fNum.addr, align 4
  %iNum.castFloatToIntTmp = fptosi float %iNum1 to i32
  store i32 %iNum.castFloatToIntTmp, ptr %iNum, align 4
  %loadTmp = load i32, ptr %iNum, align 4
  ret i32 %loadTmp
}

define float @power(float %x, i32 %exponent) {
entry:
  %x.addr = alloca float, align 4
  store float %x, ptr %x.addr, align 4
  %exponent.addr = alloca i32, align 4
  store i32 %exponent, ptr %exponent.addr, align 4
  %loadLeftTmp = load i32, ptr %exponent.addr, align 4
  %eqTmp = icmp eq i32 %loadLeftTmp, 0
  br i1 %eqTmp, label %then, label %else

then:                                             ; preds = %entry
  ret float 1.000000e+00
  br label %ifCont

else:                                             ; preds = %entry
  br label %ifCont

ifCont:                                           ; preds = %else, %then
  %loadLeftTmp1 = load float, ptr %x.addr, align 4
  %loadArgTmp = load float, ptr %x.addr, align 4
  %loadLeftTmp2 = load i32, ptr %exponent.addr, align 4
  %subTmp = sub i32 %loadLeftTmp2, 1
  %callResultTmp = call float @power(float %loadArgTmp, i32 %subTmp)
  %mulTmp = fmul float %loadLeftTmp1, %callResultTmp
  ret float %mulTmp
}

define i32 @factorial(i32 %n) {
entry:
  %n.addr = alloca i32, align 4
  store i32 %n, ptr %n.addr, align 4
  %loadLeftTmp = load i32, ptr %n.addr, align 4
  %eqTmp = icmp eq i32 %loadLeftTmp, 0
  %loadLeftTmp1 = load i32, ptr %n.addr, align 4
  %eqTmp2 = icmp eq i32 %loadLeftTmp1, 1
  %orTmp = or i1 %eqTmp, %eqTmp2
  br i1 %orTmp, label %then, label %else

then:                                             ; preds = %entry
  ret i32 1
  br label %ifCont

else:                                             ; preds = %entry
  br label %ifCont

ifCont:                                           ; preds = %else, %then
  %loadLeftTmp3 = load i32, ptr %n.addr, align 4
  %loadLeftTmp4 = load i32, ptr %n.addr, align 4
  %subTmp = sub i32 %loadLeftTmp4, 1
  %callResultTmp = call i32 @factorial(i32 %subTmp)
  %mulTmp = mul i32 %loadLeftTmp3, %callResultTmp
  ret i32 %mulTmp
}

define float @sin(float %x) {
entry:
  %term = alloca float, align 4
  %i = alloca i32, align 4
  %terms = alloca i32, align 4
  %result = alloca float, align 4
  %x.addr = alloca float, align 4
  store float %x, ptr %x.addr, align 4
  store float 0.000000e+00, ptr %result, align 4
  store i32 10, ptr %terms, align 4
  store i32 0, ptr %i, align 4
  br label %loop

loop:                                             ; preds = %ifCont, %entry
  %loadLeftTmp = load i32, ptr %i, align 4
  %loadRightTmp = load i32, ptr %terms, align 4
  %ltTmp = icmp slt i32 %loadLeftTmp, %loadRightTmp
  br i1 %ltTmp, label %loopBody, label %loopExit

loopBody:                                         ; preds = %loop
  %loadArgTmp = load float, ptr %x.addr, align 4
  %loadRightTmp1 = load i32, ptr %i, align 4
  %mulTmp = mul i32 2, %loadRightTmp1
  %addTmp = add i32 %mulTmp, 1
  %callResultTmp = call float @power(float %loadArgTmp, i32 %addTmp)
  %loadRightTmp2 = load i32, ptr %i, align 4
  %mulTmp3 = mul i32 2, %loadRightTmp2
  %addTmp4 = add i32 %mulTmp3, 1
  %callResultTmp5 = call i32 @factorial(i32 %addTmp4)
  %callResultTmp6 = call float @castIntToFloat(i32 %callResultTmp5)
  %divTmp = fdiv float %callResultTmp, %callResultTmp6
  store float %divTmp, ptr %term, align 4
  %loadLeftTmp7 = load i32, ptr %i, align 4
  %modTmp = srem i32 %loadLeftTmp7, 2
  %eqTmp = icmp eq i32 %modTmp, 0
  br i1 %eqTmp, label %then, label %else

loopExit:                                         ; preds = %loop
  %loadTmp = load float, ptr %result, align 4
  ret float %loadTmp

then:                                             ; preds = %loopBody
  %loadLeftTmp8 = load float, ptr %result, align 4
  %loadRightTmp9 = load float, ptr %term, align 4
  %addTmp10 = fadd float %loadLeftTmp8, %loadRightTmp9
  store float %addTmp10, ptr %result, align 4
  br label %ifCont

else:                                             ; preds = %loopBody
  %loadLeftTmp11 = load float, ptr %result, align 4
  %loadRightTmp12 = load float, ptr %term, align 4
  %subTmp = fsub float %loadLeftTmp11, %loadRightTmp12
  store float %subTmp, ptr %result, align 4
  br label %ifCont

ifCont:                                           ; preds = %else, %then
  %loadLeftTmp13 = load i32, ptr %i, align 4
  %addTmp14 = add i32 %loadLeftTmp13, 1
  store i32 %addTmp14, ptr %i, align 4
  br label %loop
}

define i32 @main() {
entry:
  %sineValue = alloca float, align 4
  %angle = alloca float, align 4
  store float 1.000000e+00, ptr %angle, align 4
  %loadArgTmp = load float, ptr %angle, align 4
  %callResultTmp = call float @sin(float %loadArgTmp)
  store float %callResultTmp, ptr %sineValue, align 4
  %loadLeftTmp = load float, ptr %sineValue, align 4
  %mulTmp = fmul float %loadLeftTmp, 1.000000e+06
  %callResultTmp1 = call i32 @castFloatToInt(float %mulTmp)
  ret i32 %callResultTmp1
}
