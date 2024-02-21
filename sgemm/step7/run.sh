make &&
adb push test_bl_sgemm_step7.x ./. &&
adb shell "./test_bl_sgemm_step7.x"