/*
 * RTC subsystem, initialize system time on startup
 *
 * Copyright (C) 2005 Tower Technologies
 * Author: Alessandro Zummo <a.zummo@towertech.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/rtc.h>

/* IMPORTANT: the RTC only stores whole seconds. It is arbitrary
 * whether it stores the most close value or the value with partial
 * seconds truncated. However, it is important that we use it to store
 * the truncated value. This is because otherwise it is necessary,
 * in an rtc sync function, to read both xtime.tv_sec and
 * xtime.tv_nsec. On some processors (i.e. ARM), an atomic read
 * of >32bits is not possible. So storing the most close value would
 * slow down the sync API. So here we have the truncated value and
 * the best guess is to add 0.5s.
 */

int rtc_hctosys_ret = -ENODEV;

//use AP RTC
#if 1
static int __init rtc_hctosys(void)
{
	int err = -ENODEV;
	struct rtc_time tm;
	struct timespec tv = {
	        .tv_nsec = NSEC_PER_SEC >> 1,
	    };
	    struct rtc_device *rtc = rtc_class_open(CONFIG_RTC_HCTOSYS_DEVICE);

	    if (rtc == NULL) {
	            pr_err("%s: unable to open rtc device (%s)\n",
	                    __FILE__, CONFIG_RTC_HCTOSYS_DEVICE);
	            goto err_open;
	    }

	    {
	            if(!strcmp("rtc1",CONFIG_RTC_HCTOSYS_DEVICE))
	            {
	                    struct rtc_device *rtc0 = rtc_class_open("rtc0");
	                    if(rtc0 == NULL)
	                    {
	                            pr_err("!!!!!!Robin: %s: unable to open rtc0 device!!!!!\n",
	                      __FILE__);
	                    }
				printk("read rtc1 time\n");	
	                    err = rtc_read_time(rtc, &tm);
				printk("set  rtc1 time 2 rtc0\n");	
	                    rtc_set_time(rtc0,&tm);
	                    rtc_class_close(rtc0);
	            }
	    }
	    if (err)
	    {
	            err = rtc_read_time(rtc, &tm);
			printk("rtc_read_time error\n");	
	
	            if (err) {
	                    dev_err(rtc->dev.parent,
	                                    "hctosys: unable to read the hardware clock\n");
	                    goto err_read;

	            }
	    }

	    err = rtc_valid_tm(&tm);
	    if (err) {
	            dev_err(rtc->dev.parent,
	                    "hctosys: invalid date/time\n");
	            goto err_invalid;
	    }

	    rtc_tm_to_time(&tm, &tv.tv_sec);

	    do_settimeofday(&tv);

	    dev_info(rtc->dev.parent,
	            "setting system clock to "
	            "%d-%02d-%02d %02d:%02d:%02d UTC (%u)\n",
	            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,

	            tm.tm_hour, tm.tm_min, tm.tm_sec,
	            (unsigned int) tv.tv_sec);

	err_invalid:
	err_read:
	    rtc_class_close(rtc);

	err_open:
	    rtc_hctosys_ret = err;

	    return err;
}



#else
static int __init rtc_hctosys(void)
{
	int err = -ENODEV;
	struct rtc_time tm;
	struct timespec tv = {
		.tv_nsec = NSEC_PER_SEC >> 1,
	};
	struct rtc_device *rtc = rtc_class_open(CONFIG_RTC_HCTOSYS_DEVICE);

	if (rtc == NULL) {
		pr_err("%s: unable to open rtc device (%s)\n",
			__FILE__, CONFIG_RTC_HCTOSYS_DEVICE);
		goto err_open;
	}

	{
		if(strcmp("rtc0",CONFIG_RTC_HCTOSYS_DEVICE))
		{
			struct rtc_device *rtc0 = rtc_class_open("rtc0");
			if(rtc0 == NULL)
			{
				pr_err("!!!!!!Robin: %s: unable to open rtc0 device!!!!!\n",
			  __FILE__);
			}
			err = rtc_read_time(rtc, &tm);
			rtc_set_time(rtc0,&tm);
			rtc_class_close(rtc0);
		}	
	}
	if (err)
	{
		err = rtc_read_time(rtc, &tm);
		if (err) {
			dev_err(rtc->dev.parent,
					"hctosys: unable to read the hardware clock\n");
			goto err_read;

		}
	}

	err = rtc_valid_tm(&tm);
	if (err) {
		dev_err(rtc->dev.parent,
			"hctosys: invalid date/time\n");
		goto err_invalid;
	}

	rtc_tm_to_time(&tm, &tv.tv_sec);

	do_settimeofday(&tv);

	dev_info(rtc->dev.parent,
		"setting system clock to "
		"%d-%02d-%02d %02d:%02d:%02d UTC (%u)\n",
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec,
		(unsigned int) tv.tv_sec);

err_invalid:
err_read:
	rtc_class_close(rtc);

err_open:
	rtc_hctosys_ret = err;

	return err;
}
#endif
late_initcall(rtc_hctosys);
