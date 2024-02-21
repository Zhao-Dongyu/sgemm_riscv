make &&
adb push test_bl_sgemm_step8.x ./. &&
adb shell "./test_bl_sgemm_step8.x"