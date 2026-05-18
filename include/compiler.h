#ifndef SHIFTOS_COMPILER_H
#define SHIFTOS_COMPILER_H
#define SHIFTOS_NORETURN  __attribute__((noreturn))
#define SHIFTOS_USED      __attribute__((used))
#define SHIFTOS_PACKED    __attribute__((packed))
#define SHIFTOS_ALIGNED(x) __attribute__((aligned(x)))
#define SHIFTOS_SECTION(x) __attribute__((section(x)))
#define SHIFTOS_SYSV      __attribute__((sysv_abi))
#define SHIFTOS_CALL      __attribute__((sysv_abi))
#define SHIFTOS_UNUSED    __attribute__((unused))
#define SHIFTOS_INLINE static inline __attribute__((always_inline))
#endif
