#ifndef __pir_GPIO__
#define __pir_GPIO__

//#define DEBUG
#ifdef DEBUG
#define prdebug(fmt, ...) printk(KERN_DEBUG pr_fmt(fmt), ##__VA_ARGS__)
#else
#define prdebug(fmt, ...)
#endif


#define PIR_MAGIC         'p'
#define PIR_SET_GPIO           _IOW(PIR_MAGIC, 0xa0, struct PIR_GPIO )
#define PIR_SET_RUN            _IOW(PIR_MAGIC, 0xa1, int )  //0-stop 1-start
#define PIR_SET_SEN            _IOW(PIR_MAGIC, 0xa2, int )  //0-低 1-中 2-高
#define PIR_CHECK_READY        _IOW(PIR_MAGIC, 0xa3, int)   //check pir if is ready  1-not 0-ready

struct PIR_GPIO {
    int gpio_out;
    int gpio_sen;
};

#endif

