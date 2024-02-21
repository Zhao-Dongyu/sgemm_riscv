make &&
adb push test_bl_sgemm_step9.x ./. &&
adb shell "./test_bl_sgemm_step9.x"