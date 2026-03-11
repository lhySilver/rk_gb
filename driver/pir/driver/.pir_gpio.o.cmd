cmd_/home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/pir_gpio.o := /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/scripts/gcc-wrapper.py arm-rockchip830-linux-uclibcgnueabihf-gcc -Wp,-MMD,/home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/.pir_gpio.o.d  -nostdinc -isystem /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin/../lib/gcc/arm-rockchip830-linux-uclibcgnueabihf/8.3.0/include -I/home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include -I./arch/arm/include/generated -I/home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include -I./include -I/home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi -I./arch/arm/include/generated/uapi -I/home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi -I./include/generated/uapi -include /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kconfig.h -include /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/compiler_types.h -D__KERNEL__ -mlittle-endian -fmacro-prefix-map=/home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/= -Wall -Wundef -Werror=strict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE -Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Wno-format-security -std=gnu89 -fno-dwarf2-cfi-asm -fno-ipa-sra -mabi=aapcs-linux -mfpu=vfp -funwind-tables -mthumb -Wa,-mimplicit-it=always -Wa,-mno-warn-deprecated -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -fno-delete-null-pointer-checks -Wno-frame-address -Wno-format-truncation -Wno-format-overflow -Os --param=allow-store-data-races=0 -Wframe-larger-than=1024 -fstack-protector -Werror -Wimplicit-fallthrough -Wno-unused-but-set-variable -Wno-unused-const-variable -fomit-frame-pointer -g -Wdeclaration-after-statement -Wvla -Wno-pointer-sign -Wno-stringop-truncation -Wno-array-bounds -Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized -fno-strict-overflow -fno-stack-check -fconserve-stack -Werror=date-time -Werror=incompatible-pointer-types -Werror=designated-init -Wno-packed-not-aligned  -DMODULE  -DKBUILD_BASENAME='"pir_gpio"' -DKBUILD_MODNAME='"pir_gpio"' -D__KBUILD_MODNAME=kmod_pir_gpio -c -o /home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/pir_gpio.o /home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/pir_gpio.c

source_/home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/pir_gpio.o := /home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/pir_gpio.c

deps_/home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/pir_gpio.o := \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kconfig.h \
    $(wildcard include/config/cc/version/text.h) \
    $(wildcard include/config/cpu/big/endian.h) \
    $(wildcard include/config/booger.h) \
    $(wildcard include/config/foo.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/compiler_types.h \
    $(wildcard include/config/have/arch/compiler/h.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/cc/has/asm/inline.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/compiler_attributes.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/compiler-gcc.h \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/retpoline.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
    $(wildcard include/config/kcov.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/module.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/sysfs.h) \
    $(wildcard include/config/modules/tree/lookup.h) \
    $(wildcard include/config/livepatch.h) \
    $(wildcard include/config/cfi/clang.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tree/srcu.h) \
    $(wildcard include/config/bpf/events.h) \
    $(wildcard include/config/jump/label.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/kprobes.h) \
    $(wildcard include/config/have/static/call/inline.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/function/error/injection.h) \
    $(wildcard include/config/module/sig.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/types.h \
    $(wildcard include/config/have/uid16.h) \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/int-ll64.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/int-ll64.h \
  arch/arm/include/generated/uapi/asm/bitsperlong.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitsperlong.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/bitsperlong.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/posix_types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/stddef.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/stddef.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/compiler_types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/posix_types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/posix_types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/const.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/const.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/const.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/preempt/rt.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/panic/timeout.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/limits.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/limits.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/limits.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/linkage.h \
    $(wildcard include/config/arch/use/sym/annotations.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/stringify.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/export.h \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/module/rel/crcs.h) \
    $(wildcard include/config/have/arch/prel32/relocations.h) \
    $(wildcard include/config/trim/unused/ksyms.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/linkage.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/stack/validation.h) \
  arch/arm/include/generated/asm/rwonce.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/rwonce.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kasan-checks.h \
    $(wildcard include/config/kasan/generic.h) \
    $(wildcard include/config/kasan/sw/tags.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kcsan-checks.h \
    $(wildcard include/config/kcsan.h) \
    $(wildcard include/config/kcsan/ignore/atomics.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/bitops.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/bits.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/bits.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/build_bug.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/bitops.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/typecheck.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/irqflags.h \
    $(wildcard include/config/cpu/v7m.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/arm/thumb.h) \
    $(wildcard include/config/thumb2/kernel.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/hwcap.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/hwcap.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/irqflags.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/percpu.h \
    $(wildcard include/config/cpu/v6.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/percpu.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
    $(wildcard include/config/amd/mem/encrypt.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/barrier.h \
    $(wildcard include/config/cpu/32v6k.h) \
    $(wildcard include/config/cpu/xsc3.h) \
    $(wildcard include/config/cpu/fa526.h) \
    $(wildcard include/config/arm/heavy/mb.h) \
    $(wildcard include/config/arm/dma/mem/bufferable.h) \
    $(wildcard include/config/cpu/spectre.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/barrier.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/non-atomic.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/builtin-__fls.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/builtin-__ffs.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/builtin-fls.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/builtin-ffs.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/ffz.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/fls64.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/sched.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/hweight.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/arch_hweight.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/const_hweight.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/lock.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/atomic.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/atomic.h \
    $(wildcard include/config/generic/atomic64.h) \
    $(wildcard include/config/arm/lpae.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/prefetch.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/processor.h \
    $(wildcard include/config/have/hw/breakpoint.h) \
    $(wildcard include/config/binfmt/elf/fdpic.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/hw_breakpoint.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/unified.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/vdso/processor.h \
    $(wildcard include/config/arm/errata/754327.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/cache.h \
    $(wildcard include/config/arm/l1/cache/shift.h) \
    $(wildcard include/config/aeabi.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/cmpxchg.h \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/cmpxchg-local.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/atomic-fallback.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/atomic-long.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/find.h \
    $(wildcard include/config/generic/find/first/bit.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/le.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/byteorder.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/byteorder/little_endian.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/byteorder/little_endian.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/swab.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/swab.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/swab.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/swab.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/byteorder/generic.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bitops/ext2-atomic-setbit.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/minmax.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/printk.h \
    $(wildcard include/config/message/loglevel/default.h) \
    $(wildcard include/config/console/loglevel/default.h) \
    $(wildcard include/config/console/loglevel/quiet.h) \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk/nmi.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/dynamic/debug/core.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/init.h \
    $(wildcard include/config/strict/kernel/rwx.h) \
    $(wildcard include/config/strict/module/rwx.h) \
    $(wildcard include/config/lto/clang.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kern_levels.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/ratelimit_types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/param.h \
  arch/arm/include/generated/uapi/asm/param.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/param.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/spinlock_types.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/spinlock_types_up.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/lockdep_types.h \
    $(wildcard include/config/prove/raw/lock/nesting.h) \
    $(wildcard include/config/preempt/lock.h) \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rwlock_types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/div64.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/compiler.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/div64.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/kernel.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/sysinfo.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/stat.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/stat.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/stat.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
    $(wildcard include/config/posix/timers.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/math64.h \
    $(wildcard include/config/arch/supports/int128.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/math64.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/time64.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/time64.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/time.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/time_types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/time32.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/timex.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/timex.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/timex.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/time32.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/time.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/uidgid.h \
    $(wildcard include/config/multiuser.h) \
    $(wildcard include/config/user/ns.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/highuid.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kmod.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/umh.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/gfp.h \
    $(wildcard include/config/cma.h) \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/zone/device.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/pm/sleep.h) \
    $(wildcard include/config/contig/alloc.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/mmdebug.h \
    $(wildcard include/config/debug/vm.h) \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/debug/vm/pgflags.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/bug.h \
    $(wildcard include/config/bug/on/data/corruption.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/bug.h \
    $(wildcard include/config/debug/bugverbose.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/opcodes.h \
    $(wildcard include/config/cpu/endian/be32.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/instrumentation.h \
    $(wildcard include/config/debug/entry.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/isolation.h) \
    $(wildcard include/config/shadow/call/stack.h) \
    $(wildcard include/config/memcg.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/transparent/hugepage.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/page/extension.h) \
    $(wildcard include/config/deferred/struct/page/init.h) \
    $(wildcard include/config/have/memoryless/nodes.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/memory/hotremove.h) \
    $(wildcard include/config/have/arch/pfn/valid.h) \
    $(wildcard include/config/holes/in/zone.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/spinlock.h \
    $(wildcard include/config/preemption.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/preempt.h \
    $(wildcard include/config/preempt/count.h) \
    $(wildcard include/config/trace/preempt/toggle.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  arch/arm/include/generated/asm/preempt.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/preempt.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/thread_info.h \
    $(wildcard include/config/thread/info/in/task.h) \
    $(wildcard include/config/have/arch/within/stack/frames.h) \
    $(wildcard include/config/hardened/usercopy.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/restart_block.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/errno.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/errno.h \
  arch/arm/include/generated/uapi/asm/errno.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/errno.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/errno-base.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/thread_info.h \
    $(wildcard include/config/stackprotector/per/task.h) \
    $(wildcard include/config/crunch.h) \
    $(wildcard include/config/arm/thumbee.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/fpstate.h \
    $(wildcard include/config/vfpv3.h) \
    $(wildcard include/config/iwmmxt.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/page.h \
    $(wildcard include/config/cpu/copy/v4wt.h) \
    $(wildcard include/config/cpu/copy/v4wb.h) \
    $(wildcard include/config/cpu/copy/feroceon.h) \
    $(wildcard include/config/cpu/copy/fa.h) \
    $(wildcard include/config/cpu/xscale.h) \
    $(wildcard include/config/cpu/copy/v6.h) \
    $(wildcard include/config/kuser/helpers.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/glue.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/pgtable-2level-types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/memory.h \
    $(wildcard include/config/need/mach/memory/h.h) \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/dram/base.h) \
    $(wildcard include/config/dram/size.h) \
    $(wildcard include/config/xip/kernel.h) \
    $(wildcard include/config/have/tcm.h) \
    $(wildcard include/config/arm/patch/phys/virt.h) \
    $(wildcard include/config/phys/offset.h) \
    $(wildcard include/config/xip/phys/addr.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sizes.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/memory_model.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pfn.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/getorder.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/bottom_half.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/lockdep.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/smp.h \
    $(wildcard include/config/up/late/init.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/bitmap.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
    $(wildcard include/config/fortify/source.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/string.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/string.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/smp_types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/llist.h \
    $(wildcard include/config/arch/have/nmi/safe/cmpxchg.h) \
  arch/arm/include/generated/asm/mmiowb.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/mmiowb.h \
    $(wildcard include/config/mmiowb.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/spinlock_up.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rwlock.h \
    $(wildcard include/config/preempt.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/spinlock_api_up.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/wait.h \
  arch/arm/include/generated/asm/current.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/current.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/wait.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
    $(wildcard include/config/numa/keep/meminfo.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/seqlock.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/mutex.h \
    $(wildcard include/config/mutex/spin/on/owner.h) \
    $(wildcard include/config/debug/mutexes.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/osq_lock.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/debug_locks.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/android_vendor.h \
    $(wildcard include/config/android/vendor/oem/data.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/ww_mutex.h \
    $(wildcard include/config/debug/ww/mutex/slowpath.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/nodemask.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/page-flags-layout.h \
    $(wildcard include/config/numa/balancing.h) \
    $(wildcard include/config/kasan/hw/tags.h) \
  include/generated/bounds.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/mm_types.h \
    $(wildcard include/config/have/aligned/struct/page.h) \
    $(wildcard include/config/userfaultfd.h) \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/speculative/page/fault.h) \
    $(wildcard include/config/have/arch/compat/mmap/bases.h) \
    $(wildcard include/config/membarrier.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
    $(wildcard include/config/arch/want/batched/unmap/tlb/flush.h) \
    $(wildcard include/config/iommu/support.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/mm_types_task.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/arch/enable/split/pmd/ptlock.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/auxvec.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/auxvec.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/auxvec.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/auxvec.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rbtree.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rcupdate.h \
    $(wildcard include/config/preempt/rcu.h) \
    $(wildcard include/config/tiny/rcu.h) \
    $(wildcard include/config/tasks/rcu/generic.h) \
    $(wildcard include/config/rcu/stall/common.h) \
    $(wildcard include/config/no/hz/full.h) \
    $(wildcard include/config/rcu/nocb/cpu.h) \
    $(wildcard include/config/tasks/rcu.h) \
    $(wildcard include/config/tasks/trace/rcu.h) \
    $(wildcard include/config/tasks/rude/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
    $(wildcard include/config/debug/objects/rcu/head.h) \
    $(wildcard include/config/prove/rcu.h) \
    $(wildcard include/config/rcu/boost.h) \
    $(wildcard include/config/arch/weak/release/acquire.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rcutiny.h \
    $(wildcard include/config/srcu.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rwsem.h \
    $(wildcard include/config/rwsem/spin/on/owner.h) \
    $(wildcard include/config/debug/rwsems.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/err.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/completion.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/swait.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/uprobes.h \
    $(wildcard include/config/uprobes.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/workqueue.h \
    $(wildcard include/config/debug/objects/work.h) \
    $(wildcard include/config/freezer.h) \
    $(wildcard include/config/wq/watchdog.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/timer.h \
    $(wildcard include/config/debug/objects/timers.h) \
    $(wildcard include/config/no/hz/common.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/ktime.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/jiffies.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/jiffies.h \
  include/generated/timeconst.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/ktime.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/timekeeping.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/timekeeping32.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/android_kabi.h \
    $(wildcard include/config/android/kabi/reserve.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/mmu.h \
    $(wildcard include/config/cpu/has/asid.h) \
    $(wildcard include/config/vdso.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/page-flags.h \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/page/idle/flag.h) \
    $(wildcard include/config/thp/swap.h) \
    $(wildcard include/config/ksm.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/memory_hotplug.h \
    $(wildcard include/config/arch/has/add/pages.h) \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/have/bootmem/info/node.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/notifier.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/srcu.h \
    $(wildcard include/config/tiny/srcu.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rcu_segcblist.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/srcutiny.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/topology.h \
    $(wildcard include/config/use/percpu/numa/node/id.h) \
    $(wildcard include/config/sched/smt.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/arch_topology.h \
    $(wildcard include/config/generic/arch/topology.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/percpu.h \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/topology.h \
    $(wildcard include/config/arm/cpu/topology.h) \
    $(wildcard include/config/bl/switcher.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/topology.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sysctl.h \
    $(wildcard include/config/sysctl.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/sysctl.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/elf.h \
    $(wildcard include/config/arch/use/gnu/property.h) \
    $(wildcard include/config/arch/have/elf/prot.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/elf.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/vdso_datapage.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/datapage.h \
    $(wildcard include/config/arch/has/vdso/data.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/errno-base.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/clocksource.h \
    $(wildcard include/config/generic/gettimeofday.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/vdso/processor.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/vdso/gettimeofday.h \
    $(wildcard include/config/arm/arch/timer.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/unistd.h \
    $(wildcard include/config/oabi/compat.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/unistd.h \
  arch/arm/include/generated/uapi/asm/unistd-eabi.h \
  arch/arm/include/generated/uapi/asm/unistd-common.h \
  arch/arm/include/generated/asm/unistd-nr.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/vdso/cp15.h \
    $(wildcard include/config/cpu/cp15.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/user.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/elf.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/elf-em.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kobject.h \
    $(wildcard include/config/uevent/helper.h) \
    $(wildcard include/config/debug/kobject/release.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sysfs.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kernfs.h \
    $(wildcard include/config/kernfs.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/idr.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/radix-tree.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/xarray.h \
    $(wildcard include/config/xarray/multi.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kconfig.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/local_lock.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/local_lock_internal.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kobject_ns.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kref.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/refcount.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/jump_label.h \
    $(wildcard include/config/have/arch/jump/label/relative.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/jump_label.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rbtree_latch.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/error-injection.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/error-injection.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/tracepoint-defs.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/static_key.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/static_call_types.h \
    $(wildcard include/config/have/static/call.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/cfi.h \
    $(wildcard include/config/cfi/clang/shadow.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/module.h \
    $(wildcard include/config/arm/unwind.h) \
    $(wildcard include/config/arm/module/plts.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/module.h \
    $(wildcard include/config/have/mod/arch/specific.h) \
    $(wildcard include/config/modules/use/elf/rel.h) \
    $(wildcard include/config/modules/use/elf/rela.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/fs.h \
    $(wildcard include/config/read/only/thp/for/fs.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/security.h) \
    $(wildcard include/config/cgroup/writeback.h) \
    $(wildcard include/config/ima.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/fs/encryption.h) \
    $(wildcard include/config/fs/verity.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/unicode.h) \
    $(wildcard include/config/compat.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fs/dax.h) \
    $(wildcard include/config/mandatory/file/locking.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/migration.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/wait_bit.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kdev_t.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/kdev_t.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/dcache.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rculist.h \
    $(wildcard include/config/prove/rcu/list.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rculist_bl.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/list_bl.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/bit_spinlock.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/lockref.h \
    $(wildcard include/config/arch/use/cmpxchg/lockref.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/stringhash.h \
    $(wildcard include/config/dcache/word/access.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/hash.h \
    $(wildcard include/config/have/arch/hash.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/path.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/list_lru.h \
    $(wildcard include/config/memcg/kmem.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/shrinker.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pid.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/capability.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/capability.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/semaphore.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/fcntl.h \
    $(wildcard include/config/arch/32bit/off/t.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/fcntl.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/fcntl.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/fcntl.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/openat2.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/migrate_mode.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/percpu-rwsem.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rcuwait.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched/signal.h \
    $(wildcard include/config/sched/autogroup.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/stack/growsup.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/signal.h \
    $(wildcard include/config/proc/fs.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/signal_types.h \
    $(wildcard include/config/old/sigaction.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/signal.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/signal.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/signal.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/signal-defs.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/uapi/asm/sigcontext.h \
  arch/arm/include/generated/uapi/asm/siginfo.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/siginfo.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched.h \
    $(wildcard include/config/virt/cpu/accounting/native.h) \
    $(wildcard include/config/sched/info.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/uclamp/task.h) \
    $(wildcard include/config/uclamp/buckets/count.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/psi.h) \
    $(wildcard include/config/compat/brk.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/blk/cgroup.h) \
    $(wildcard include/config/stackprotector.h) \
    $(wildcard include/config/arch/has/scaled/cputime.h) \
    $(wildcard include/config/cpu/freq/times.h) \
    $(wildcard include/config/virt/cpu/accounting/gen.h) \
    $(wildcard include/config/posix/cputimers.h) \
    $(wildcard include/config/posix/cpu/timers/task/work.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/io/uring.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/ubsan.h) \
    $(wildcard include/config/ubsan/trap.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/x86/cpu/resctrl.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/rseq.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/kunit.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/bcache.h) \
    $(wildcard include/config/vmap/stack.h) \
    $(wildcard include/config/gcc/plugin/stackleak.h) \
    $(wildcard include/config/x86/mce.h) \
    $(wildcard include/config/rt/softint/optimization.h) \
    $(wildcard include/config/arch/task/struct/on/stack.h) \
    $(wildcard include/config/debug/rseq.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/sched.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sem.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/sem.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/ipc.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rhashtable-types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/ipc.h \
  arch/arm/include/generated/uapi/asm/ipcbuf.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/ipcbuf.h \
  arch/arm/include/generated/uapi/asm/sembuf.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/sembuf.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/shm.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/shm.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/hugetlb_encode.h \
  arch/arm/include/generated/uapi/asm/shmbuf.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/shmbuf.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/shmparam.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kcov.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/kcov.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/plist.h \
    $(wildcard include/config/debug/plist.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
    $(wildcard include/config/time/low/res.h) \
    $(wildcard include/config/timerfd.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/hrtimer_defs.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/timerqueue.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
    $(wildcard include/config/have/arch/seccomp/filter.h) \
    $(wildcard include/config/seccomp/filter.h) \
    $(wildcard include/config/checkpoint/restore.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/seccomp.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/seccomp.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/seccomp.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/unistd.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/resource.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/resource.h \
  arch/arm/include/generated/uapi/asm/resource.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/resource.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/resource.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/latencytop.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched/prio.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched/types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/posix-timers.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/alarmtimer.h \
    $(wildcard include/config/rtc/class.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/task_work.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/rseq.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kcsan.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched/jobctl.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched/task.h \
    $(wildcard include/config/have/exit/thread.h) \
    $(wildcard include/config/arch/wants/dynamic/task/struct.h) \
    $(wildcard include/config/have/arch/thread/struct/whitelist.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/uaccess.h \
    $(wildcard include/config/set/fs.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/fault-inject-usercopy.h \
    $(wildcard include/config/fault/injection/usercopy.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/instrumented.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/uaccess.h \
    $(wildcard include/config/cpu/sw/domain/pan.h) \
    $(wildcard include/config/cpu/use/domains.h) \
    $(wildcard include/config/uaccess/with/memcpy.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/domain.h \
    $(wildcard include/config/io/36.h) \
    $(wildcard include/config/cpu/cp15/mmu.h) \
  arch/arm/include/generated/asm/extable.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/extable.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/key.h \
    $(wildcard include/config/key/notifications.h) \
    $(wildcard include/config/net.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/assoc_array.h \
    $(wildcard include/config/associative/array.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched/user.h \
    $(wildcard include/config/fanotify.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/bpf/syscall.h) \
    $(wildcard include/config/watch/queue.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/ratelimit.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/rcu_sync.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/delayed_call.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/uuid.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/uuid.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/errseq.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/ioprio.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched/rt.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/iocontext.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/fs_types.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/fs.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/ioctl.h \
  arch/arm/include/generated/uapi/asm/ioctl.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/ioctl.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/ioctl.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/quota.h \
    $(wildcard include/config/quota/netlink/interface.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/percpu_counter.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/dqblk_xfs.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/dqblk_v1.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/dqblk_v2.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/dqblk_qtree.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/projid.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/quota.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/nfs_fs_i.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/energy/model.h) \
    $(wildcard include/config/generic/msi/irq/domain.h) \
    $(wildcard include/config/pinctrl.h) \
    $(wildcard include/config/generic/msi/irq.h) \
    $(wildcard include/config/dma/ops.h) \
    $(wildcard include/config/dma/declare/coherent.h) \
    $(wildcard include/config/dma/cma.h) \
    $(wildcard include/config/arch/has/sync/dma/for/device.h) \
    $(wildcard include/config/arch/has/sync/dma/for/cpu.h) \
    $(wildcard include/config/arch/has/sync/dma/for/cpu/all.h) \
    $(wildcard include/config/dma/ops/bypass.h) \
    $(wildcard include/config/of.h) \
    $(wildcard include/config/devtmpfs.h) \
    $(wildcard include/config/sysfs/deprecated.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/dev_printk.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/energy_model.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched/cpufreq.h \
    $(wildcard include/config/cpu/freq.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched/topology.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/sched/mc.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/sched/idle.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/ioport.h \
    $(wildcard include/config/io/strict/devmem.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/klist.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pm.h \
    $(wildcard include/config/vt/console/sleep.h) \
    $(wildcard include/config/pm.h) \
    $(wildcard include/config/pm/clk.h) \
    $(wildcard include/config/pm/generic/domains.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/overflow.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/device/bus.h \
    $(wildcard include/config/acpi.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/device/class.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/device/driver.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/device.h \
    $(wildcard include/config/dmabounce.h) \
    $(wildcard include/config/arm/dma/use/iommu.h) \
    $(wildcard include/config/arch/omap.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pm_wakeup.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/slab.h \
    $(wildcard include/config/debug/slab.h) \
    $(wildcard include/config/failslab.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/have/hardened/usercopy/allocator.h) \
    $(wildcard include/config/rockchip/mini/kernel.h) \
    $(wildcard include/config/slab.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/percpu-refcount.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kasan.h \
    $(wildcard include/config/kasan/stack.h) \
    $(wildcard include/config/kasan/vmalloc.h) \
    $(wildcard include/config/kasan/inline.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/kasan-enabled.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/cdev.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/gpio.h \
    $(wildcard include/config/gpiolib.h) \
    $(wildcard include/config/arch/have/custom/gpio/h.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/gpio.h \
    $(wildcard include/config/arch/nr/gpio.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/gpio.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/gpio/driver.h \
    $(wildcard include/config/irq/domain/hierarchy.h) \
    $(wildcard include/config/gpio/generic.h) \
    $(wildcard include/config/gpiolib/irqchip.h) \
    $(wildcard include/config/of/gpio.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/irq.h \
    $(wildcard include/config/generic/irq/effective/aff/mask.h) \
    $(wildcard include/config/generic/irq/ipi.h) \
    $(wildcard include/config/generic/irq/migration.h) \
    $(wildcard include/config/generic/pending/irq.h) \
    $(wildcard include/config/hardirqs/sw/resend.h) \
    $(wildcard include/config/generic/irq/legacy/alloc/hwirq.h) \
    $(wildcard include/config/generic/irq/legacy.h) \
    $(wildcard include/config/generic/irq/multi/handler.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/irqhandler.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/irqreturn.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/irqnr.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/irqnr.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/io.h \
    $(wildcard include/config/have/arch/huge/vmap.h) \
    $(wildcard include/config/has/ioport/map.h) \
    $(wildcard include/config/pci.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/io.h \
    $(wildcard include/config/need/mach/io/h.h) \
    $(wildcard include/config/pcmcia/soc/common.h) \
    $(wildcard include/config/isa.h) \
    $(wildcard include/config/pccard.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/pci_iomap.h \
    $(wildcard include/config/no/generic/pci/ioport/map.h) \
    $(wildcard include/config/generic/pci/iomap.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/io.h \
    $(wildcard include/config/generic/iomap.h) \
    $(wildcard include/config/generic/ioremap.h) \
    $(wildcard include/config/virt/to/bus.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/logic_pio.h \
    $(wildcard include/config/indirect/pio.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/fwnode.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/vmalloc.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/vmalloc.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/irq.h \
    $(wildcard include/config/sparse/irq.h) \
  arch/arm/include/generated/asm/irq_regs.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/asm-generic/irq_regs.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/irqdesc.h \
    $(wildcard include/config/generic/irq/debugfs.h) \
    $(wildcard include/config/handle/domain/irq.h) \
    $(wildcard include/config/irq/domain.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/hw_irq.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/irqchip/chained_irq.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/irqdomain.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/of.h \
    $(wildcard include/config/of/dynamic.h) \
    $(wildcard include/config/sparc.h) \
    $(wildcard include/config/of/promtree.h) \
    $(wildcard include/config/of/kobj.h) \
    $(wildcard include/config/of/numa.h) \
    $(wildcard include/config/of/overlay.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/mod_devicetable.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/property.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pinctrl/pinctrl.h \
    $(wildcard include/config/generic/pinconf.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/seq_file.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pinctrl/pinctrl-state.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pinctrl/devinfo.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pinctrl/consumer.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pinctrl/pinconf-generic.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/pinctrl/machine.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/gpio/consumer.h \
    $(wildcard include/config/gpio/sysfs.h) \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/delay.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/arch/arm/include/asm/delay.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/miscdevice.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/major.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/linux/poll.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/poll.h \
  arch/arm/include/generated/uapi/asm/poll.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/asm-generic/poll.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/SDK/RV1106_IPC_SDK/sysdrv/source/kernel/include/uapi/linux/eventpoll.h \
  /home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/pir_gpio.h \

/home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/pir_gpio.o: $(deps_/home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/pir_gpio.o)

$(deps_/home/ale/workspace/Rockchip/RV1106_RV1103/Project/RC0240/rc0240/driver/pir/driver/pir_gpio.o):
