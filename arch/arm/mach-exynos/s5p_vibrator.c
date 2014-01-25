/* include/asm/mach-msm/htc_pwrsink.h
 *
 * Copyright (C) 2012 Samsung Semiconductor China R&D
 * Copyright (C) 2008 HTC Corporation.
 * Copyright (C) 2007 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/io.h>
#include <linux/gpio.h>
#include <plat/gpio-cfg.h>
#include <mach/regs-gpio.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/err.h>
#include <linux/hrtimer.h>
#include <../../../drivers/staging/android/timed_output.h>
#include <linux/sched.h>

#if 0// Porting from MSM
#include <mach/msm_rpcrouter.h>

#define PM_LIBPROG        0x30000061
#if defined(CONFIG_ARCH_MSM7X30)
#define PM_LIBVERS        0x00030001
#elif defined(CONFIG_MSM_LEGACY_7X00A_AMSS)
#define PM_LIBVERS        0xfb837d0b
#else
#define PM_LIBVERS        MSM_RPC_VERS(1,1)
#endif

#if defined(CONFIG_ARCH_QSD8X50)  || defined(CONFIG_ARCH_MSM7X30)
#define HTC_PROCEDURE_SET_VIB_ON_OFF    22
#else
#define HTC_PROCEDURE_SET_VIB_ON_OFF    21
#endif
#define PMIC_VIBRATOR_LEVEL     (3000)
#endif

// GPIO definition
// Cellon modify start, Ted Shi, 2012/08/29, for vibrator gpio change
//#define GPIO_VIBCTL		EXYNOS4_GPD0(0)
#define GPIO_VIBCTL		EXYNOS4212_GPM0(1)

// Cellon modify start, Ted Shi, 2013/01/23, for add eng log switch
//#define VIBRATOR_DEBUG
#ifdef CONFIG_CELLON_ENG_LOG
#define VIBRATOR_DEBUG
#endif
// Cellon modify end, Ted Shi, 2013/01/23

#ifdef VIBRATOR_DEBUG
#define VIB_DBG(fmt, ...) printk(pr_fmt(fmt), ##__VA_ARGS__)
#else
#define VIB_DBG(fmt, ...)
#endif
// Cellon modify end, Ted Shi, 2012/08/29
// Cellon delete start, Ted Shi, 2012/09/13, for vibrator sometimes mix
//static struct work_struct vibrator_work;
// Cellon delete end, Ted Shi, 2012/09/13
static struct hrtimer vibe_timer;
static spinlock_t vibe_lock;
// Cellon modify start, Ted Shi, 2012/09/28, for hapticfeedback vibrate enable time
static spinlock_t vib_start_lock;
// Cellon modify end, Ted Shi, 2012/09/28
static int vibe_state;

void cfg_vib_ctrl(void)
{
	gpio_request(GPIO_VIBCTL, "MOTOR_PWM");
	s3c_gpio_cfgpin(GPIO_VIBCTL, S3C_GPIO_OUTPUT); 
	s3c_gpio_setpull(GPIO_VIBCTL, S3C_GPIO_PULL_NONE); 
}

void vib_start(int on)
{
// Cellon modify start, Ted Shi, 2012/09/28, for hapticfeedback vibrate enable time
	unsigned long   flags;
	spin_lock_irqsave(&vib_start_lock , flags);
	//if (on)
	//	gpio_set_value(GPIO_VIBCTL, 1);
	//else
	//	gpio_set_value(GPIO_VIBCTL, 0);
	if (on){
		VIB_DBG(" vibrator ===open===, jiffies = %lx \n",jiffies);
		gpio_set_value(GPIO_VIBCTL, 1);
	}else{
		VIB_DBG(" vibrator ===off===, jiffies = %lx \n",jiffies);
		gpio_set_value(GPIO_VIBCTL, 0);
	}
	spin_unlock_irqrestore(&vib_start_lock , flags);
// Cellon modify end, Ted Shi, 2012/09/28
}

// Cellon delete start, Ted Shi, 2012/09/28, for hapticfeedback vibrate enable time
/*
static void set_pmic_vibrator(int on)
{
#if 0// Porting from MSM
        static struct msm_rpc_endpoint *vib_endpoint;
        struct set_vib_on_off_req {
                struct rpc_request_hdr hdr;
                uint32_t data;
        } req;

        if (!vib_endpoint) {
                vib_endpoint = msm_rpc_connect(PM_LIBPROG, PM_LIBVERS, 0);
                if (IS_ERR(vib_endpoint)) {
                        printk(KERN_ERR "init vib rpc failed!\n");
                        vib_endpoint = 0;
                        return;
                }
        }

        if (on)
                req.data = cpu_to_be32(PMIC_VIBRATOR_LEVEL);
        else
                req.data = cpu_to_be32(0);

        msm_rpc_call(vib_endpoint, HTC_PROCEDURE_SET_VIB_ON_OFF, &req,
                sizeof(req), 5 * HZ);
#endif
	vib_start(on);
}
*/
// Cellon delete end, Ted Shi, 2012/09/28
// Cellon delete start, Ted Shi, 2012/09/13, for vibrator sometimes mix
/*
static void update_vibrator(struct work_struct *work)
{
        set_pmic_vibrator(vibe_state);
}
*/
// Cellon delete end, Ted Shi, 2012/09/13
static void vibrator_enable(struct timed_output_dev *dev, int value)
{
        unsigned long   flags;

        spin_lock_irqsave(&vibe_lock, flags);
        hrtimer_cancel(&vibe_timer);
// Cellon modify start, Ted Shi, 2012/09/13, for vibrator sometimes mix
	 VIB_DBG("%s: value %d , %d seconds \n",__func__,value,value/1000);
// Cellon modify end, Ted Shi, 2012/09/13

        if (value == 0)
                vibe_state = 0;
        else {
// Cellon modify start, Ted Shi, 2012/11/07, for vibrator time too short 
// Cellon modify start, Ted Shi, 2012/11/01, for vibrator time 
//                value += 20;
                value += 40;
// Cellon modify end, Ted Shi, 2012/11/01
// Cellon modify end, Ted Shi, 2012/11/07
                value = (value > 15000 ? 15000 : value);
                vibe_state = 1;
                hrtimer_start(&vibe_timer,
                        ktime_set(value / 1000, (value % 1000) * 1000000),
                        HRTIMER_MODE_REL);
        }
        spin_unlock_irqrestore(&vibe_lock, flags);
// Cellon modify start, Ted Shi, 2012/09/13, for vibrator sometimes mix
//        schedule_work(&vibrator_work);
	vib_start(vibe_state);
// Cellon modify end, Ted Shi, 2012/09/13
}

static int vibrator_get_time(struct timed_output_dev *dev)
{
	struct timeval tv;
        if (hrtimer_active(&vibe_timer)) {
                ktime_t r = hrtimer_get_remaining(&vibe_timer);
#if 1
		tv = ktime_to_timeval(r);
		return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#else
                return r.tv.sec * 1000 + r.tv.nsec / 1000000;
#endif
        } else
                return 0;
}

static enum hrtimer_restart vibrator_timer_func(struct hrtimer *timer)
{
        vibe_state = 0;
// Cellon modify start, Ted Shi, 2012/09/13, for vibrator sometimes mix
//        schedule_work(&vibrator_work);
	vib_start(vibe_state);
// Cellon modify end, Ted Shi, 2012/09/13
        return HRTIMER_NORESTART;
}

static struct timed_output_dev pmic_vibrator = {
        .name = "vibrator",
        .get_time = vibrator_get_time,
        .enable = vibrator_enable,
};

void __init s5p_init_pmic_vibrator(void)
{
	VIB_DBG("%s()\n", __FUNCTION__);
	cfg_vib_ctrl();
// Cellon delete start, Ted Shi, 2012/09/13, for vibrator sometimes mix
//        INIT_WORK(&vibrator_work, update_vibrator);
// Cellon delete end, Ted Shi, 2012/09/13
        spin_lock_init(&vibe_lock);
        vibe_state = 0;
        hrtimer_init(&vibe_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        vibe_timer.function = vibrator_timer_func;

        timed_output_dev_register(&pmic_vibrator);
}

void __exit s5p_exit_pmic_vibrator(void)
{
	gpio_free(GPIO_VIBCTL);
	timed_output_dev_unregister(&pmic_vibrator);
}

module_init(s5p_init_pmic_vibrator);
module_exit(s5p_exit_pmic_vibrator);

MODULE_DESCRIPTION("timed output pmic vibrator device");
MODULE_LICENSE("GPL");
