; ModuleID = 'quadraticSolver'
source_filename = "quadraticSolver"

define float @castIntToFloat(i32 %iNum) {
entry:
  %fNum = alloca float, align 4
  %iNum.addr = alloca i32, align 4
  store i32 %iNum, ptr %iNum.addr, align 4
  %fNum1 = load i32, ptr %iNum.addr, align 4
  %fNum.sitofpTmp = sitofp i32 %fNum1 to float
  store float %fNum.sitofpTmp, ptr %fNum, align 4
  %loadTmp = load float, ptr %fNum, align 4
  ret float %loadTmp
}

define i32 @castFloatToInt(float %fNum) {
entry:
  %iNum = alloca i32, align 4
  %fNum.addr = alloca float, align 4
  store float %fNum, ptr %fNum.addr, align 4
  %iNum1 = load float, ptr %fNum.addr, align 4
  %iNum.fptosiTmp = fptosi float %iNum1 to i32
  store i32 %iNum.fptosiTmp, ptr %iNum, align 4
  %loadTmp = load i32, ptr %iNum, align 4
  ret i32 %loadTmp
}

define float @sqrt(float %x) {
entry:
  %newGuess = alloca float, align 4
  %shouldBreak = alloca i32, align 4
  %diff = alloca float, align 4
  %guess = alloca float, align 4
  %tolerance = alloca float, align 4
  %x.addr = alloca float, align 4
  store float %x, ptr %x.addr, align 4
  %loadLeftTmp = load float, ptr %x.addr, align 4
  %ltTmp = fcmp olt float %loadLeftTmp, 0.000000e+00
  br i1 %ltTmp, label %then, label %else

then:                                             ; preds = %entry
  ret float -1.000000e+00
  br label %ifCont

else:                                             ; preds = %entry
  br label %ifCont

ifCont:                                           ; preds = %else, %then
  store float 0x3EE4F8B580000000, ptr %tolerance, align 4
  %loadLeftTmp1 = load float, ptr %x.addr, align 4
  %divTmp = fdiv float %loadLeftTmp1, 2.000000e+00
  store float %divTmp, ptr %guess, align 4
  store float 0.000000e+00, ptr %diff, align 4
  store i32 0, ptr %shouldBreak, align 4
  br label %loop

loop:                                             ; preds = %ifCont18, %ifCont
  %loadLeftTmp2 = load i32, ptr %shouldBreak, align 4
  %eqTmp = icmp eq i32 %loadLeftTmp2, 0
  br i1 %eqTmp, label %loopBody, label %loopExit

loopBody:                                         ; preds = %loop
  %loadLeftTmp3 = load float, ptr %guess, align 4
  %loadLeftTmp4 = load float, ptr %x.addr, align 4
  %loadRightTmp = load float, ptr %guess, align 4
  %divTmp5 = fdiv float %loadLeftTmp4, %loadRightTmp
  %addTmp = fadd float %loadLeftTmp3, %divTmp5
  %divTmp6 = fdiv float %addTmp, 2.000000e+00
  store float %divTmp6, ptr %newGuess, align 4
  %loadLeftTmp7 = load float, ptr %newGuess, align 4
  %loadRightTmp8 = load float, ptr %guess, align 4
  %subTmp = fsub float %loadLeftTmp7, %loadRightTmp8
  store float %subTmp, ptr %diff, align 4
  %loadLeftTmp9 = load float, ptr %diff, align 4
  %ltTmp10 = fcmp olt float %loadLeftTmp9, 0.000000e+00
  br i1 %ltTmp10, label %then11, label %else12

loopExit:                                         ; preds = %loop
  %loadTmp19 = load float, ptr %guess, align 4
  ret float %loadTmp19

then11:                                           ; preds = %loopBody
  %loadOperandTmp = load float, ptr %diff, align 4
  %fnegTmp = fneg float %loadOperandTmp
  store float %fnegTmp, ptr %diff, align 4
  br label %ifCont13

else12:                                           ; preds = %loopBody
  br label %ifCont13

ifCont13:                                         ; preds = %else12, %then11
  %loadLeftTmp14 = load float, ptr %diff, align 4
  %loadRightTmp15 = load float, ptr %tolerance, align 4
  %leTmp = fcmp ole float %loadLeftTmp14, %loadRightTmp15
  br i1 %leTmp, label %then16, label %else17

then16:                                           ; preds = %ifCont13
  store i32 1, ptr %shouldBreak, align 4
  br label %ifCont18

else17:                                           ; preds = %ifCont13
  br label %ifCont18

ifCont18:                                         ; preds = %else17, %then16
  %loadTmp = load float, ptr %newGuess, align 4
  store float %loadTmp, ptr %guess, align 4
  br label %loop
}

define float @calculateDiscriminant(float %a, float %b, float %c) {
entry:
  %a.addr = alloca float, align 4
  store float %a, ptr %a.addr, align 4
  %b.addr = alloca float, align 4
  store float %b, ptr %b.addr, align 4
  %c.addr = alloca float, align 4
  store float %c, ptr %c.addr, align 4
  %loadLeftTmp = load float, ptr %b.addr, align 4
  %loadRightTmp = load float, ptr %b.addr, align 4
  %mulTmp = fmul float %loadLeftTmp, %loadRightTmp
  %loadRightTmp1 = load float, ptr %a.addr, align 4
  %mulTmp2 = fmul float 4.000000e+00, %loadRightTmp1
  %loadRightTmp3 = load float, ptr %c.addr, align 4
  %mulTmp4 = fmul float %mulTmp2, %loadRightTmp3
  %subTmp = fsub float %mulTmp, %mulTmp4
  ret float %subTmp
}

define float @calculateRoot(float %a, float %b, float %discriminant, i32 %isPositiveRoot) {
entry:
  %a.addr = alloca float, align 4
  store float %a, ptr %a.addr, align 4
  %b.addr = alloca float, align 4
  store float %b, ptr %b.addr, align 4
  %discriminant.addr = alloca float, align 4
  store float %discriminant, ptr %discriminant.addr, align 4
  %isPositiveRoot.addr = alloca i32, align 4
  store i32 %isPositiveRoot, ptr %isPositiveRoot.addr, align 4
  %loadLeftTmp = load i32, ptr %isPositiveRoot.addr, align 4
  %eqTmp = icmp eq i32 %loadLeftTmp, 1
  br i1 %eqTmp, label %then, label %else

then:                                             ; preds = %entry
  %loadOperandTmp = load float, ptr %b.addr, align 4
  %fnegTmp = fneg float %loadOperandTmp
  %loadArgTmp = load float, ptr %discriminant.addr, align 4
  %callResultTmp = call float @sqrt(float %loadArgTmp)
  %addTmp = fadd float %fnegTmp, %callResultTmp
  %loadRightTmp = load float, ptr %a.addr, align 4
  %mulTmp = fmul float 2.000000e+00, %loadRightTmp
  %divTmp = fdiv float %addTmp, %mulTmp
  ret float %divTmp
  br label %ifCont

else:                                             ; preds = %entry
  br label %ifCont

ifCont:                                           ; preds = %else, %then
  %loadOperandTmp1 = load float, ptr %b.addr, align 4
  %fnegTmp2 = fneg float %loadOperandTmp1
  %loadArgTmp3 = load float, ptr %discriminant.addr, align 4
  %callResultTmp4 = call float @sqrt(float %loadArgTmp3)
  %subTmp = fsub float %fnegTmp2, %callResultTmp4
  %loadRightTmp5 = load float, ptr %a.addr, align 4
  %mulTmp6 = fmul float 2.000000e+00, %loadRightTmp5
  %divTmp7 = fdiv float %subTmp, %mulTmp6
  ret float %divTmp7
}

define i32 @main() {
entry:
  %intRoot2 = alloca i32, align 4
  %intRoot1 = alloca i32, align 4
  %root2 = alloca float, align 4
  %root1 = alloca float, align 4
  %discriminant = alloca float, align 4
  %c = alloca float, align 4
  %b = alloca float, align 4
  %a = alloca float, align 4
  store float 2.000000e+00, ptr %a, align 4
  store float 3.000000e+00, ptr %b, align 4
  store float -1.000000e+00, ptr %c, align 4
  %loadArgTmp = load float, ptr %a, align 4
  %loadArgTmp1 = load float, ptr %b, align 4
  %loadArgTmp2 = load float, ptr %c, align 4
  %callResultTmp = call float @calculateDiscriminant(float %loadArgTmp, float %loadArgTmp1, float %loadArgTmp2)
  store float %callResultTmp, ptr %discriminant, align 4
  %loadLeftTmp = load float, ptr %discriminant, align 4
  %ltTmp = fcmp olt float %loadLeftTmp, 0.000000e+00
  br i1 %ltTmp, label %then, label %else

then:                                             ; preds = %entry
  ret i32 -1
  br label %ifCont

else:                                             ; preds = %entry
  br label %ifCont

ifCont:                                           ; preds = %else, %then
  %loadArgTmp3 = load float, ptr %a, align 4
  %loadArgTmp4 = load float, ptr %b, align 4
  %loadArgTmp5 = load float, ptr %discriminant, align 4
  %callResultTmp6 = call float @calculateRoot(float %loadArgTmp3, float %loadArgTmp4, float %loadArgTmp5, i32 1)
  store float %callResultTmp6, ptr %root1, align 4
  %loadArgTmp7 = load float, ptr %a, align 4
  %loadArgTmp8 = load float, ptr %b, align 4
  %loadArgTmp9 = load float, ptr %discriminant, align 4
  %callResultTmp10 = call float @calculateRoot(float %loadArgTmp7, float %loadArgTmp8, float %loadArgTmp9, i32 0)
  store float %callResultTmp10, ptr %root2, align 4
  %loadLeftTmp11 = load float, ptr %root1, align 4
  %mulTmp = fmul float %loadLeftTmp11, 1.000000e+03
  %callResultTmp12 = call i32 @castFloatToInt(float %mulTmp)
  store i32 %callResultTmp12, ptr %intRoot1, align 4
  %loadLeftTmp13 = load float, ptr %root2, align 4
  %mulTmp14 = fmul float %loadLeftTmp13, 1.000000e+03
  %callResultTmp15 = call i32 @castFloatToInt(float %mulTmp14)
  store i32 %callResultTmp15, ptr %intRoot2, align 4
  %loadTmp = load i32, ptr %intRoot1, align 4
  ret i32 %loadTmp
}
