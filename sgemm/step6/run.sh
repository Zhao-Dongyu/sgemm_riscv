make &&
adb push test_bl_sgemm_step6.x ./. &&
adb shell "./test_bl_sgemm_step6.x"