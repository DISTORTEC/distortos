CMSIS
=====

- link: [CMSIS - Cortex Microcontroller Software Interface
Standard](http://www.arm.com/products/processors/cortex-m/cortex-microcontroller-software-interface-standard.php)
- package name: CMSIS_5-5.1.0
- package version: 5.1.0
- download date: 2017-09-05

Only headers from CMSIS_5-5.1.0/CMSIS/Core/Include were copied from the package.

Single change was applied to the code to make it warning-free:

```
diff --git a/external/CMSIS/mpu_armv7.h b/external/CMSIS/mpu_armv7.h
index fb1a339b..8465abb3 100644
--- a/external/CMSIS/mpu_armv7.h
+++ b/external/CMSIS/mpu_armv7.h
@@ -115,7 +115,7 @@ __STATIC_INLINE void ARM_MPU_Enable(uint32_t MPU_Control)
 
 /** Disable the MPU.
 */
-__STATIC_INLINE void ARM_MPU_Disable()
+__STATIC_INLINE void ARM_MPU_Disable(void)
 {
   __DSB();
   __ISB();
```

This is already fixed in the upstream repository of CMSIS 5.
