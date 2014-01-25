/*
 * BQ275xx battery driver
 *
 * Based on a previous work by Copyright (C) 2008 Texas Instruments, Inc.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#include <linux/module.h>
#include <linux/param.h>
#include <linux/jiffies.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <linux/idr.h>
#include <linux/i2c.h>
#include <asm/unaligned.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <plat/gpio-cfg.h>

//#define DEBUG_BATTERY_DATA			//show bqfs files data,need long time 		
//#define DEBUG_BATTERY				//show debug infor
#define bq27425CMD_CNTL_LSB  0x00
#define bq27425CMD_CNTL_MSB  0x01
#define bq27425CMD_TEMP_LSB    0x02
#define bq27425CMD_TEMP_MSB    0x03
#define bq27425CMD_VOLT_LSB 0x04
#define bq27425CMD_VOLT_MSB 0x05
#define bq27425CMD_FLAGS_LSB  0x06
#define bq27425CMD_FLAGS_MSB  0x07
#define bq27425CMD_NAC_LSB  0x08
#define bq27425CMD_NAC_MSB  0x09
#define bq27425CMD_FAC_LSB 0x0A
#define bq27425CMD_FAC_MSB 0x0B
#define bq27425CMD_RM_LSB   0x0C
#define bq27425CMD_RM_MSB   0x0D
#define bq27425CMD_FCC_LSB   0x0E
#define bq27425CMD_FCC_MSB   0x0F
#define bq27425CMD_AI_LSB    0x10
#define bq27425CMD_AI_MSB    0x11
#define bq27425CMD_SI_LSB   0x12
#define bq27425CMD_SI_MSB   0x13
#define bq27425CMD_MLI_LSB    0x14
#define bq27425CMD_MLI_MSB    0x15
#define bq27425CMD_AP_LSB   0x18
#define bq27425CMD_AP_MSB   0x19
#define bq27425CMD_SOC_LSB   0x1c
#define bq27425CMD_SOC_MSB   0x1d
#define bq27425CMD_ITEMP_LSB    0x1e
#define bq27425CMD_ITEMP_MSB    0x1f
#define bq27425CMD_SOH_LSB  0x20
#define bq27425CMD_SOH_MSB  0x21

//extended command
#define bq27425CMD_OPCFG_LSB  0x3a
#define bq27425CMD_OPCFG_MSB  0x3b
#define bq27425CMD_DCAP_LSB  0x3C
#define bq27425CMD_DCAP_MSB  0x3D
#define bq27425CMD_DFCLS     0x3E
#define bq27425CMD_DFBLK     0x3F
#define bq27425CMD_DFD       0x40
#define bq27425CMD_DFDCKS    0x60
#define bq27425CMD_DFDCNTL   0x61
#define bq27425CMD_DNAMELEN  0x62
#define bq27425CMD_DNAME     0x63

/*
#define bq27425CMD_MLI_LSB   0x1E
#define bq27425CMD_MLI_MSB   0x1F
#define bq27425CMD_MLTTE_LSB 0x20
#define bq27425CMD_MLTTE_MSB 0x21
#define bq27425CMD_AE_LSB    0x22
#define bq27425CMD_AE_MSB    0x23
#define bq27425CMD_AP_LSB    0x24
#define bq27425CMD_AP_MSB    0x25
#define bq27425CMD_TTECP_LSB 0x26
#define bq27425CMD_TTECP_MSB 0x27
#define bq27425CMD_RSVD_LSB  0x28
#define bq27425CMD_RSVD_MSB  0x29
#define bq27425CMD_CC_LSB    0x2A
#define bq27425CMD_CC_MSB    0x2B
#define bq27425CMD_SOC_LSB   0x2C
#define bq27425CMD_SOC_MSB   0x2D
#define bq27425CMD_DCAP_LSB  0x3C
#define bq27425CMD_DCAP_MSB  0x3D
#define bq27425CMD_DFCLS     0x3E
#define bq27425CMD_DFBLK     0x3F
#define bq27425CMD_ADF       0x40
#define bq27425CMD_ACKSDFD   0x54
#define bq27425CMD_DFDCKS    0x60
#define bq27425CMD_DFDCNTL   0x61
#define bq27425CMD_DNAMELEN  0x62
#define bq27425CMD_DNAME     0x63
#define bq27425CMD_APPSTAT   0x6A
*/
#define MAX_BUFFER_SIZE   20
#define MAX_DATA_SIZE   10
#define ROMMODEADDR   0x0b    //0x16 =8bit 0x0B =7bit
#define MAX_FAILURE_COUNT  3

#ifdef CONFIG_CELLON_ENG_LOG
#define BQ27425_LOG_ON 1
#else 
#define BQ27425_LOG_ON 0
#endif

char *bqfs_file="/battery.bqfs";
static int rsoc_now = 0;
int first = 1;
static int temp_now=0;
static int vol_now = 0;
static struct work_struct bq27425_work;
static struct timer_list bq27425_timer;
int updateflag = 0;
/* If the system has several batteries we need a different name for each
 * of them...
 */
static DEFINE_IDR(battery_id);
static DEFINE_MUTEX(battery_mutex);

struct bq275xx_device_info;
struct bq275xx_access_methods {
	int (*read)(u8 reg, int *rt_value, int b_single,
		struct bq275xx_device_info *di);
};

struct bq275xx_device_info {
	struct device 		*dev;
	int			id;
	int			voltage_uV;
	int			current_uA;
	int			temp_C;
	int			charge_rsoc;
	struct bq275xx_access_methods	*bus;
	struct power_supply	bat;

	struct i2c_client	*client;
};

static enum power_supply_property bq275xx_battery_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_HEALTH,
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,
	POWER_SUPPLY_PROP_CURRENT_NOW,
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_TEMP,
	POWER_SUPPLY_PROP_MANUFACTURER,
};


struct bq275xx_device_info *bq_this_chip;
/*
 * Common code for BQ275xx devices
 */
static int  __init bq27425_gpio_init(void)
{
	int ret = 0;
 	ret=gpio_request(EXYNOS4_GPX0(0), "GPX0(0)");
	if(ret<0){
			printk("gpio requst fail for charger: S5PV310_GPX0(0) (cen) pin!! ");
		return ret;
		}
	
	s3c_gpio_cfgpin(EXYNOS4_GPX0(0), S3C_GPIO_SFN(0xF));
	s3c_gpio_setpull(EXYNOS4_GPX0(0), S3C_GPIO_PULL_UP);	
	#ifdef DEBUG_BATTERY
	printk("-----bq27425_EXYNOS4_GPX0(0): %d-----\n",gpio_get_value(EXYNOS4_GPX0(0)));
	#endif
	return ret;

}
static irqreturn_t bq27425_int_handler(int irq, void *data)
{
       #ifdef DEBUG_BATTERY
       printk("bq27425_int_handler start!\n");
	printk("-----bq27425_EXYNOS4_GPX0(0): %d-----\n",gpio_get_value(EXYNOS4_GPX0(0)));
       #endif
	
	schedule_work(&bq27425_work);
	
	#ifdef DEBUG_BATTERY
	 printk("bq27425_int_handler end!\n");
	#endif
	return IRQ_HANDLED;
}

static int bq275xx_read(u8 reg, int *rt_value, int b_single,
			struct bq275xx_device_info *di)
{
	int ret;

	ret = di->bus->read(reg, rt_value, b_single, di);
	*rt_value = be16_to_cpu(*rt_value);

	return ret;
}

/*
 * Return the battery temperature in Celsius degrees
 * Or < 0 if something fails.
 */
static int bq275xx_battery_temperature(struct bq275xx_device_info *di)
{
	int ret;
	int temp = 0;
	int i = 0;
       for(i = 0; i < 10; i++)
	{   
	ret = bq275xx_read(bq27425CMD_TEMP_LSB, &temp, 0, di);
	if (ret) {
		dev_err(di->dev, "error reading temperature\n");
		return ret;
	}
	    temp  -= 2730;
	    if(temp >= 0 && temp <= 700)
           {
                     temp_now = temp;
			 break;
           }
	    else
	    {
	           continue;
	    }		 

       }
	return temp_now;		//0.1C
}

/*
 * Return the battery Voltage in milivolts
 * Or < 0 if something fails.
 */
static int bq275xx_battery_voltage(struct bq275xx_device_info *di)
{
	int ret;
	int volt = 0;

	     ret = bq275xx_read(bq27425CMD_VOLT_LSB, &volt, 0, di);
	     if (volt < 0 ) {
		dev_err(di->dev, "error reading voltage\n");
		return ret;
	     }
	     else if(volt > 0)
             {
              vol_now = volt;
              return volt;
    	     }
	//printk("The battery voltage now is %d***\n",vol_now);   
	return 0;
}


/*
 * Return the battery average current
 * Note that current can be negative signed as well
 * Or 0 if something fails.
 */
static int bq275xx_battery_current(struct bq275xx_device_info *di)
{
	int ret;
	int curr = 0;

	ret = bq275xx_read(bq27425CMD_AI_LSB, &curr, 0, di);
	if (ret) {
		dev_err(di->dev, "error reading current\n");
		return 0;
	}
	if (curr&(0x1<<15)) {
		dev_dbg(di->dev, "negative current!\n");		
		//return -curr;
		return (65535-curr);
	}
	return curr;
}

static int bq275xx_battery_flags_hi(struct bq275xx_device_info *di);

/*
 * Return the battery Relative State-of-Charge
 * Or < 0 if something fails.
 */

static int bq275xx_battery_rsoc(struct bq275xx_device_info *di)
{
	int ret;
	int rsoc = 0;
	int sub = 0;
       int i = 0;
	
	ret = bq275xx_read(bq27425CMD_SOC_LSB, &rsoc, 1, di);
	if (ret) {
			dev_err(di->dev, "error reading relative State-of-Charge\n");
			return ret;
	}
	rsoc = rsoc >> 8;
	
	if(!(bq275xx_battery_flags_hi(di) & 0x2) && (rsoc == 100)) rsoc = 99;


	printk("The battery capacity now is %d***\n",rsoc);   
	return rsoc;
}

static int bq275xx_battery_flags(struct bq275xx_device_info *di)
{
       int ret = 0;
	int flags = 0;
	ret = bq275xx_read(bq27425CMD_FLAGS_LSB, &flags, 0, di); 
	if (ret) {
		dev_err(di->dev, "error reading flags\n");
		return ret;
	}
	return flags;
}

static int bq275xx_battery_flags_hi(struct bq275xx_device_info *di)
{
       int ret = 0;
	int flags = 0;
	ret = bq275xx_read(bq27425CMD_FLAGS_MSB, &flags, 0, di); 
	if (ret) {
		dev_err(di->dev, "error reading flags\n");
		return ret;
	}
	
	return flags;
}

static int bq275xx_battery_status(struct bq275xx_device_info *di)
{
	int ret = POWER_SUPPLY_STATUS_UNKNOWN;

	if(gpio_get_value(EXYNOS4_GPX0(4)) == 0x0) 
	{
		ret = POWER_SUPPLY_STATUS_NOT_CHARGING;
	}

	if(gpio_get_value(EXYNOS4_GPX0(4)) == 0x1)  //charger int detect
	{
		if(bq275xx_battery_rsoc(di) == 100)
		{
	     		ret = POWER_SUPPLY_STATUS_FULL;
       	}
		else
			ret = POWER_SUPPLY_STATUS_CHARGING;
	}	
       else if(bq275xx_battery_flags(di) & 0x1)
       {
       	ret = POWER_SUPPLY_STATUS_DISCHARGING;
       }
       return ret;
}

extern int bat_ID_value;
extern int bat_temp;
static char * bq27425_get_bat_ID(void)
{
	char * bat_ID;
	//printk("bat_ID_value is %d\n",bat_ID_value);
	if((bat_ID_value>1820 && bat_ID_value<2275)||(bat_ID_value>683 && bat_ID_value<1365))
		bat_ID="GOOD_BAT";
	else
		bat_ID = "BAD_BAT";

	return bat_ID;
	
}

#define to_bq275xx_device_info(x) container_of((x), \
				struct bq275xx_device_info, bat);

static int bq275xx_battery_get_property(struct power_supply *psy,
					enum power_supply_property psp,
					union power_supply_propval *val)
{
	struct bq275xx_device_info *di = to_bq275xx_device_info(psy);
	
	switch (psp) {
		case POWER_SUPPLY_PROP_STATUS:
		val->intval = bq275xx_battery_status(di);
		 break;		
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		val->intval = bq275xx_battery_voltage(di);
		break;
	case POWER_SUPPLY_PROP_PRESENT:
		//val->intval = bq275xx_battery_flags(di);
		//val->intval = ((val->intval >> 3) & 0x1);
		val->intval = 1;
		break;
	case POWER_SUPPLY_PROP_HEALTH:
		val->intval = POWER_SUPPLY_HEALTH_GOOD;
		break;
	case POWER_SUPPLY_PROP_CURRENT_NOW:
		val->intval = bq275xx_battery_current(di);
		break;
	case POWER_SUPPLY_PROP_CAPACITY:
		val->intval = bq275xx_battery_rsoc(di);
		break;
	case POWER_SUPPLY_PROP_TEMP:
		val->intval = bat_temp*10; //bq275xx_battery_temperature(di);
		break;
	case POWER_SUPPLY_PROP_TECHNOLOGY:
		val->intval = POWER_SUPPLY_TECHNOLOGY_LION;	
		break;
	case POWER_SUPPLY_PROP_MANUFACTURER:
		val->strval = bq27425_get_bat_ID();
		break;		
	default:
		return -EINVAL;
	}
#ifdef DEBUG_BATTERY
	printk("++++bq275xx_battery_get_property :power_supply_property=%d, val->intval =%d\n",psp,val->intval);
#endif
	return 0;
}

static void bq275xx_powersupply_init(struct bq275xx_device_info *di)
{
	di->bat.name = "battery";
	di->bat.type = POWER_SUPPLY_TYPE_BATTERY;
	di->bat.properties = bq275xx_battery_props;
	di->bat.num_properties = ARRAY_SIZE(bq275xx_battery_props);
	di->bat.get_property = bq275xx_battery_get_property;
	di->bat.external_power_changed = NULL;
}

/*
 * bq27425 specific code
 */

static int bq27425_read(u8 reg, int *rt_value, int b_single,
			struct bq275xx_device_info *di)
{
	struct i2c_client *client = di->client;
	struct i2c_msg msg[1];
	unsigned char data[2];
	int err;

	if (!client->adapter)
		return -ENODEV;

	msg->addr = client->addr;
	msg->flags = 0;
	msg->len = 1;
	msg->buf = data;

	data[0] = reg;
	err = i2c_transfer(client->adapter, msg, 1);

	if (err >= 0) {
		if (!b_single)
			msg->len = 2;
		else
			msg->len = 1;

		msg->flags = I2C_M_RD;
		err = i2c_transfer(client->adapter, msg, 1);
		if (err >= 0) {
			if (!b_single)
				*rt_value = get_unaligned_be16(data);
			else
				*rt_value = data[0];

			return 0;
		}
	}
	return err;
}


/*
This function only used in ROM updated mode and the i2cAddr is not the same of normal mode
*/
static int bq27425_i2c_read(u8 i2cAddr,u8 reg, u8 *buf, int len,
							struct bq275xx_device_info *di)
{
	int i;
	int err;
	struct i2c_client *client = di->client;
	struct i2c_msg msgs[2];
       /*
	if(i2cAddr == 0xAB)
	{
		i2cAddr = ROMMODEADDR;
	}
      */
       if(i2cAddr == 0x16)
       {
       	i2cAddr = 0xB;
       }
	msgs[0].addr = i2cAddr;
	msgs[0].flags = 0;
	msgs[0].len = 1;
	msgs[0].buf = &reg;

	msgs[1].addr = i2cAddr;
	msgs[1].flags = I2C_M_RD;
	msgs[1].len = len;
	msgs[1].buf = buf;

	for(i=0;i<MAX_FAILURE_COUNT;i++)
	{
		err = i2c_transfer(client->adapter, msgs, 2);
		if(err>=0)
		{
			break;
		}
		mdelay(10);
	}
	if(i>=MAX_FAILURE_COUNT)
	{
		printk("%s:retry over %d times\n",__FUNCTION__,MAX_FAILURE_COUNT);
		return -EIO;
	}
	return 0;
}

static int bq27425_i2c_write(u8 i2cAddr, u8 *buf, int len,
							struct bq275xx_device_info *di)
{
	int i;
	int err;
	struct i2c_client *client = di->client;
	struct i2c_msg msgs[1];

	if(i2cAddr == 0xAA)
	{
		i2cAddr = 0x55;
	}
       //Cellon add start,Fengying Zhang,2012/09/12
      	if(i2cAddr == 0x16)
	{
		i2cAddr = 0xB;
	}
 
       //Cellon add end,Fengying Zhang,2012/09/12
	msgs[0].addr = i2cAddr;
	msgs[0].flags = 0;
	msgs[0].len = len;
	msgs[0].buf = buf;

	for(i=0;i<MAX_FAILURE_COUNT;i++)
	{
		err = i2c_transfer(client->adapter, msgs, 1);
		if(err>=0)
		{
			break;
		}
		mdelay(10);
	}
	if(i>=MAX_FAILURE_COUNT)
	{
		printk("%s:retry over %d times\n",__FUNCTION__,MAX_FAILURE_COUNT);
		return -EIO;
	}
	return 0;
}


static void string_ParseTo_char(const char *orig, size_t len, unsigned char *result)
{
	int i = 0;				//result data num
	int j = 0;				//orig data num
	unsigned char val,c;
	for(i=0; i<len ; i++)
	{
		 //two ASCII conbine to a char
		c = orig[j]; 		//first bit
		
		if (c >= '0' && c <= '9') val = c - '0';
	    	else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
	    	else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
	    	else {
	       		 printk("\n%s: value is invalid\n", __FUNCTION__);
	        	 break;
	    	}
		result[i] =  val <<4;	//first high bit,should *16

		j++;					//second  bit
		c = orig[j];	
		
		if (c >= '0' && c <= '9') val = c - '0';
	    	else if (c >= 'a' && c <= 'f') val = c - 'a' + 10;
	    	else if (c >= 'A' && c <= 'F') val = c - 'A' + 10;
	    	else {
	       		 printk("\n%s: value is invalid\n", __FUNCTION__);
	        	 break;
	    	}
		result[i] += val;

		j = j+2;				//ignore space
	}

}



static void bq27425_timer_func(void)
{
	power_supply_changed(&bq_this_chip->bat);

	#ifdef DEBUG_BATTERY
	printk("bq27425  start timer again\n");
	printk("-----bq27425_EXYNOS4_GPX0(0): %d-----\n",gpio_get_value(EXYNOS4_GPX0(0)));
	#endif
       mod_timer(&bq27425_timer,jiffies + msecs_to_jiffies(9000));
}



static ssize_t update_show(struct device *dev, struct device_attribute *attr,
		char *buf)
{
	if(updateflag == 2)
	       return sprintf(buf, "success\n");
	else if(updateflag == 1)
		return sprintf(buf, "fail\n");
	else
		return sprintf(buf, "not update\n");
}

static ssize_t update_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf, size_t count)
{
/*	unsigned int state;

	sscanf(buf, "%d", &state);
	#if BQ27425_LOG_ON
	printk("bq27425 :update eprom = %s \n",state?"enable":"disable");
	#endif
	if(state)
	{
		if(bq27425_rom_update(bq_this_chip) == 0)
	       updateflag = 2; //update success
		else
		updateflag = 1;//update fail
	}
	else
		updateflag = 0; //not update
	return count;
	*/
	return 0;
}

static DEVICE_ATTR(update, S_IRUGO | S_IWUSR, update_show, update_store);

static int bq27425_g1_support_4350mv(struct bq275xx_device_info *di)
{        
       unsigned char valData[11];
       unsigned int valData_old[32] = {0};
       unsigned int valData_new[11] = {0};

       int ret = 0;
       int checksum = 0;
        int i =0;
        

         valData[0] = 0x0; //reg addr
         valData[1] = 0x13;
         valData[2] = 0x00;
         ret = bq27425_i2c_write(di->client->addr,valData,3,di); //Get into update mode
        if(ret<0)
        {
                printk("Get into update mode failed\n");
                 return -1;
        }
        mdelay(200);
        
        #ifdef DEBUG_BATTERY
        ret = bq275xx_read(0x6, &checksum, 1, di);
        if(ret<0)
        {
                printk("Read 0x6 failed\n");
                 return -1;
        }
       printk("Read 0x6 is : %x\n",(checksum>>8));
#endif
        
        valData[0] = bq27425CMD_DFCLS;
        valData[1] = 0x56;
           ret = bq27425_i2c_write(di->client->addr,valData,2,di); //Set the Data Class ID: 86
        if(ret<0)
        {
                printk("Set the Data Class ID: 0x56 failed\n");        
                 return -1;
        }

        valData[0] = 0x3F;
        valData[1] = 0x00;
           ret = bq27425_i2c_write(di->client->addr,valData,2,di); //Set offset 0x0
        if(ret<0)
        {
                printk("Set offset 0x0 failed\n");
                 return -1;
        }
       mdelay(200);
        
        #ifdef DEBUG_BATTERY
        for(i = 0; i < 32; i++)
        {
        ret = bq275xx_read((0x40+i), &valData_old[i], 1, di);
        if(ret<0)
        {
                printk("Read old data failed\n");
                 return -1;
        }
              printk("Read old data is : %x\n",valData_old[i] >> 8);
        }
      
        ret = bq275xx_read(bq27425CMD_DFDCKS, &checksum, 1, di);
        if(ret<0)
        {
                printk("Read old checksum failed\n");
                 return -1;
        }
              printk("Read old checksum is : %x\n",(checksum>>8));
#endif

        valData[0] = 0x40;
        valData[1] = 0x10;
        valData[2] = 0xF4;
        valData[3] = 0xFF;
        valData[4] = 0xCE;
        valData[5] = 0xFF;
        valData[6] = 0xCE;
       valData[7] = 0x00;
        valData[8] = 0x01;
        valData[9] = 0x02;
        valData[10] = 0xBC;
           ret = bq27425_i2c_write(di->client->addr,valData,11,di); //Set the data (4350 mV in Hex)
        if(ret<0)
        {
                printk("Set the data (4350 mV in Hex) failed\n");
                 return -1;
        }        
                
        valData[0] = 0x60;
        valData[1] = 0xA2;
           ret = bq27425_i2c_write(di->client->addr,valData,2,di); //Send the checksum
        if(ret<0)
        {
                printk("Send the checksum failed\n");
                 return -1;
        }

         valData[0] = 0x00; //reg addr
         valData[1] = 0x42;
         valData[2] = 0x00;
         ret = bq27425_i2c_write(di->client->addr,valData,3,di); //Get into update mode
        if(ret<0)
        {
                printk("Exit update mode failed\n");
                 return -1;
        }
        mdelay(1000);

        #ifdef DEBUG_BATTERY
        ret = bq275xx_read(0x6, &checksum, 1, di);
        if(ret<0)
        {
                printk("Read 0x6 failed\n");
                 return -1;
        }
       printk("Read 0x6 is : %x\n",(checksum>>8));

        for(i = 0; i < 10; i++)
        {
        ret = bq275xx_read((0x40+i), &valData_new[i], 1, di);
        if(ret<0)
        {
                printk("Read new data failed\n");
                 return -1;
        }
              printk("Read new data is : %x\n",valData_new[i] >> 8);
        }
      #endif
//        #if BQ27425_LOG_ON
        printk("%s success\n",__FUNCTION__);
//        #endif
}


static void bq27425_charger_work_func()
{
       power_supply_changed(&bq_this_chip->bat);
}

struct class *bq27425_class;
static int bq27425_battery_probe(struct i2c_client *client,
				 const struct i2c_device_id *id)
{
	char *name;
	int num;
	int retval = 0;
	struct bq275xx_device_info *di;
	struct bq275xx_access_methods *bus;
	struct device *bq27425_dev;

	/* Get new ID for the new battery device */
	retval = idr_pre_get(&battery_id, GFP_KERNEL);
	if (retval == 0)
		return -ENOMEM;
	mutex_lock(&battery_mutex);
	retval = idr_get_new(&battery_id, client, &num);
	mutex_unlock(&battery_mutex);
	if (retval < 0)
		return retval;

	name = kasprintf(GFP_KERNEL, "bq27425-%d", num);
	if (!name) {
		dev_err(&client->dev, "failed to allocate device name\n");
		retval = -ENOMEM;
		goto batt_failed_1;
	}

	di = kzalloc(sizeof(*di), GFP_KERNEL);
	if (!di) {
		dev_err(&client->dev, "failed to allocate device info data\n");
		retval = -ENOMEM;
		goto batt_failed_2;
	}
	di->id = num;

	bus = kzalloc(sizeof(*bus), GFP_KERNEL);
	if (!bus) {
		dev_err(&client->dev, "failed to allocate access method "
					"data\n");
		retval = -ENOMEM;
		goto batt_failed_3;
	}

      bq27425_gpio_init();
	i2c_set_clientdata(client, di);
	di->dev = &client->dev;
	di->bat.name = name;
	bus->read = &bq27425_read;
	di->bus = bus;
	di->client = client;
       bq_this_chip = di; 
	   
	//bq27425_first_run(di);

/*	retval = bq275xx_battery_flags(di);
	if((retval & 0x20) == 0x20)
	{     //power on reset 
	       bq27425_g1_support_4350mv(di);
	}
*/
   
	bq275xx_powersupply_init(di);

	retval = power_supply_register(&client->dev, &di->bat);

	if (retval) {
		dev_err(&client->dev, "failed to register battery\n");
		goto batt_failed_4;
	}

	setup_timer(&bq27425_timer, bq27425_timer_func, 0);	
	mod_timer(&bq27425_timer,		  
			jiffies + msecs_to_jiffies(1000));
	//init workqueue 
	 INIT_WORK(&bq27425_work, bq27425_charger_work_func);

       retval = request_irq(
				gpio_to_irq(EXYNOS4_GPX0(0)),
				bq27425_int_handler,
				IRQF_TRIGGER_FALLING,
				"bq27425_int_handler", NULL); 
		if (retval < 0)
		{
			printk("bq27425 request_irq(uok_irq_handler) failed due to %d !!!\n",retval);
		free_irq(gpio_to_irq(EXYNOS4_GPX0(0)),NULL);
		return -1;
	}
	enable_irq_wake(gpio_to_irq(EXYNOS4_GPX0(0)));	

	bq27425_class = class_create(THIS_MODULE, "fuel_gaugle");
	if (IS_ERR((void *)bq27425_class))
		return PTR_ERR((void *)bq27425_class);
	bq27425_dev = device_create(bq27425_class, NULL,
		MKDEV(0, 0), NULL, "bq27425");
	if (IS_ERR((void *)bq27425_dev))
		return PTR_ERR((void *)bq27425_dev);

	retval = device_create_file(bq27425_dev, &dev_attr_update);
	if (retval < 0)
		goto err_create_file_1;
	
	return 0;

err_create_file_1:
	device_destroy(bq27425_class, MKDEV(0, 0));
	printk(KERN_ERR "switch: Failed to register driver %s\n", "bq27425");
batt_failed_4:
	kfree(bus);
batt_failed_3:
	kfree(di);
batt_failed_2:
	kfree(name);
batt_failed_1:
	mutex_lock(&battery_mutex);
	idr_remove(&battery_id, num);
	mutex_unlock(&battery_mutex);

	return retval;
}

static int bq27425_battery_remove(struct i2c_client *client)
{
	struct bq275xx_device_info *di = i2c_get_clientdata(client);

	power_supply_unregister(&di->bat);

	kfree(di->bat.name);

	mutex_lock(&battery_mutex);
	idr_remove(&battery_id, di->id);
	mutex_unlock(&battery_mutex);

	kfree(di);

	return 0;
}

/*
 * Module stuff
 */

static int bq27425_suspend(struct i2c_client *client)
{
	return 0;
}

static int bq27425_resume(struct i2c_client *client)
{
	first = 1;
	return 0;
}

static const struct i2c_device_id bq27425_id[] = {
	{ "bq27425", 0 },
	{},
};

static struct i2c_driver bq27425_battery_driver = {
	.driver = {
		.name = "bq27425",
	},
	.probe = bq27425_battery_probe,
	.remove = bq27425_battery_remove,
	.suspend= bq27425_suspend,
	.resume = bq27425_resume,	
	.id_table = bq27425_id,
};

static int __init bq275xx_battery_init(void)
{
	int ret;
	#if BQ27425_LOG_ON
	printk( "bq275xx_battery_init : register bq27425 driver\n");
	#endif
	ret = i2c_add_driver(&bq27425_battery_driver);
	if (ret)
		printk(KERN_ERR "Unable to register bq27425 driver\n");

	return ret;
}
module_init(bq275xx_battery_init);

static void __exit bq275xx_battery_exit(void)
{
	i2c_del_driver(&bq27425_battery_driver);
}
module_exit(bq275xx_battery_exit);

MODULE_AUTHOR("bob yu@cellon.com");
MODULE_DESCRIPTION("bq27425 battery monitor driver");
MODULE_LICENSE("GPL");
