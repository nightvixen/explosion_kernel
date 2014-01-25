/* linux/drivers/media/video/s5k5cagx.h
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 * 		http://www.samsung.com/
 *
 * Driver for S5K5CAGX (UXGA camera) from Samsung Electronics
 * 1/4" 2.0Mp CMOS Image Sensor SoC with an Embedded Image Processor
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define S5K5CAGX_COMPLETE
#ifndef __S5K5CAGX_H__
#define __S5K5CAGX_H__

struct s5k5cagx_reg {
	unsigned char addr;
	unsigned char val;
};

struct s5k5cagx_regset_type {
	unsigned char *regset;
	int len;
};

/*
 * Macro
 */
#define REGSET_LENGTH(x)	(sizeof(x)/sizeof(s5k5cagx_reg))

/*
 * User defined commands
 */
/* S/W defined features for tune */
#define REG_DELAY	0xFF00	/* in ms */
#define REG_CMD		0xFFFF	/* Followed by command */

/* Following order should not be changed */
enum image_size_s5k5cagx {
	/* This SoC supports upto UXGA (1600*1200) */
#if 0
	QQVGA,	/* 160*120*/
	QCIF,	/* 176*144 */
	QVGA,	/* 320*240 */
	CIF,	/* 352*288 */
	VGA,	/* 640*480 */
#endif
	SVGA,	/* 800*600 */
#if 0
	HD720P,	/* 1280*720 */
	SXGA,	/* 1280*1024 */
	UXGA,	/* 1600*1200 */
#endif
};
enum s5k5cagx_prev_frmsize {
	S5K5CAGX_PREVIEW_QCIF,
	S5K5CAGX_PREVIEW_QCIF2,
	S5K5CAGX_PREVIEW_QVGA,
	S5K5CAGX_PREVIEW_VGA,
	S5K5CAGX_PREVIEW_D1,
	S5K5CAGX_PREVIEW_WVGA,
	S5K5CAGX_PREVIEW_720P,
	S5K5CAGX_PREVIEW_1080P,
	S5K5CAGX_PREVIEW_HDR,
};

enum s5k5cagx_cap_frmsize {
	S5K5CAGX_CAPTURE_VGA,	/* 640 x 480 */
	S5K5CAGX_CAPTURE_WVGA,	/* 800 x 480 */
	S5K5CAGX_CAPTURE_W1MP,	/* 1600 x 960 */
	S5K5CAGX_CAPTURE_2MP,	/* UXGA - 1600 x 1200 */
	S5K5CAGX_CAPTURE_W2MP,	/* 2048 x 1232 */
	S5K5CAGX_CAPTURE_3MP,	/* QXGA - 2048 x 1536 */
	S5K5CAGX_CAPTURE_W4MP,	/* WQXGA - 2560 x 1536 */
	S5K5CAGX_CAPTURE_5MP,	/* 2560 x 1920 */
	S5K5CAGX_CAPTURE_W6MP,	/* 3072 x 1856 */
	S5K5CAGX_CAPTURE_7MP,	/* 3072 x 2304 */
	S5K5CAGX_CAPTURE_W7MP,	/* WQXGA - 2560 x 1536 */
	S5K5CAGX_CAPTURE_8MP,	/* 3264 x 2448 */
};

struct s5k5cagx_control {
	u32 id;
	s32 value;
	s32 minimum;		/* Note signedness */
	s32 maximum;
	s32 step;
	s32 default_value;
};

struct s5k5cagx_frmsizeenum {
	unsigned int index;
	unsigned int width;
	unsigned int height;
	u8 reg_val;		/* a value for category parameter */
};

struct s5k5cagx_isp {
	wait_queue_head_t wait;
	unsigned int irq;	/* irq issued by ISP */
	unsigned int issued;
	unsigned int int_factor;
	unsigned int bad_fw:1;
};

struct s5k5cagx_jpeg {
	int quality;
	unsigned int main_size;	/* Main JPEG file size */
	unsigned int thumb_size;	/* Thumbnail file size */
	unsigned int main_offset;
	unsigned int thumb_offset;
	unsigned int postview_offset;
};

struct s5k5cagx_focus {
	unsigned int mode;
	unsigned int lock;
	unsigned int status;
	unsigned int touch;
	unsigned int pos_x;
	unsigned int pos_y;
};

struct s5k5cagx_exif {
	char unique_id[7];
	u32 exptime;		/* us */
	u16 flash;
	u16 iso;
	int tv;			/* shutter speed */
	int bv;			/* brightness */
	int ebv;		/* exposure bias */
};

struct s5k5cagx_state {
	struct s5k5cagx_platform_data *pdata;
	struct v4l2_subdev sd;

	struct s5k5cagx_isp isp;

	const struct s5k5cagx_frmsizeenum *preview;
	const struct s5k5cagx_frmsizeenum *capture;

	enum v4l2_pix_format_mode format_mode;
	enum v4l2_sensor_mode sensor_mode;
	enum v4l2_flash_mode flash_mode;
	int vt_mode;
	int beauty_mode;
	int zoom;

	unsigned int fps;
	struct s5k5cagx_focus focus;

	struct s5k5cagx_jpeg jpeg;
	struct s5k5cagx_exif exif;

	int check_dataline;
	char *fw_version;

#ifdef CONFIG_CAM_DEBUG
	u8 dbg_level;
#endif

};
/*
 * Following values describe controls of camera
 * in user aspect and must be match with index of s5k5cagx_regset[]
 * These values indicates each controls and should be used
 * to control each control
 */

#define S5K5CAGX_REGSET(x)	{	\
	.regset = x,			\
	.len = sizeof(x)/sizeof(s5k5cagx_reg),}


/*
 * User tuned register setting values
 */
	static unsigned short s5k5cagx_init_reg_short0[]={
	
	//=================================================================================================
	// * Name: S5K5CAGX EVT1.0 Initial Setfile 
	// * important : TnP and Analog Setting Version is 091216
	// * PLL mode: MCLK=24.5MHz / SYSCLK=40MHz / PCLK=60MHz
	// * FPS : Preview YUV QVGA 22-10fps / Capture YUV QXGA 7.5fps
	//=================================================================================================
	// ARM GO
	// Direct mode
	0xFCFC, 0xD000,
	0x0010, 0x0001, //Reset
	0x1030, 0x0000, //Clear host interrupt so main will wait
	0x0014, 0x0001 //ARM go
	//p10 //Min.10ms delay is required
};
	static unsigned short s5k5cagx_init_reg_short1_old[]={	
	// Start T&P part
	// DO NOT DELETE T&P SECTION COMMENTS! They are required to debug T&P related issues.
	// svn://transrdsrv/svn/svnroot/System/Software/tcevb/SDK+FW/ISP_5CA/Firmware
	// Rev: 32375-32375
	// Signature:
	// md5 78c1a0d32ef22ba270994f08d64a05a0 .btp
	// md5 6765ffc40fde4420aab81f0039a60c38 .htp
	// md5 956e8c724c34dd8b76dd297b92f59677 .RegsMap.h
	// md5 7db8e8f88de22128b8b909128f087a53 .RegsMap.bin
	// md5 506b4144bd48cdb79cbecdda4f7176ba .base.RegsMap.h
	// md5 fd8f92f13566c1a788746b23691c5f5f .base.RegsMap.bin
	//
	0x0028, 0x7000,
	0x002A, 0x2CF8,
	0x0F12, 0xB510,
	0x0F12, 0x4827,
	0x0F12, 0x21C0,
	0x0F12, 0x8041,
	0x0F12, 0x4825,
	0x0F12, 0x4A26,
	0x0F12, 0x3020,
	0x0F12, 0x8382,
	0x0F12, 0x1D12,
	0x0F12, 0x83C2,
	0x0F12, 0x4822,
	0x0F12, 0x3040,
	0x0F12, 0x8041,
	0x0F12, 0x4821,
	0x0F12, 0x4922,
	0x0F12, 0x3060,
	0x0F12, 0x8381,
	0x0F12, 0x1D09,
	0x0F12, 0x83C1,
	0x0F12, 0x4821,
	0x0F12, 0x491D,
	0x0F12, 0x8802,
	0x0F12, 0x3980,
	0x0F12, 0x804A,
	0x0F12, 0x8842,
	0x0F12, 0x808A,
	0x0F12, 0x8882,
	0x0F12, 0x80CA,
	0x0F12, 0x88C2,
	0x0F12, 0x810A,
	0x0F12, 0x8902,
	0x0F12, 0x491C,
	0x0F12, 0x80CA,
	0x0F12, 0x8942,
	0x0F12, 0x814A,
	0x0F12, 0x8982,
	0x0F12, 0x830A,
	0x0F12, 0x89C2,
	0x0F12, 0x834A,
	0x0F12, 0x8A00,
	0x0F12, 0x4918,
	0x0F12, 0x8188,
	0x0F12, 0x4918,
	0x0F12, 0x4819,
	0x0F12, 0xF000,
	0x0F12, 0xFA0C,
	0x0F12, 0x4918,
	0x0F12, 0x4819,
	0x0F12, 0x6341,
	0x0F12, 0x4919,
	0x0F12, 0x4819,
	0x0F12, 0xF000,
	0x0F12, 0xFA05,
	0x0F12, 0x4816,
	0x0F12, 0x4918,
	0x0F12, 0x3840,
	0x0F12, 0x62C1,
	0x0F12, 0x4918,
	0x0F12, 0x3880,
	0x0F12, 0x63C1,
	0x0F12, 0x4917,
	0x0F12, 0x6301,
	0x0F12, 0x4917,
	0x0F12, 0x3040,
	0x0F12, 0x6181,
	0x0F12, 0x4917,
	0x0F12, 0x4817,
	0x0F12, 0xF000,
	0x0F12, 0xF9F5,
	0x0F12, 0x4917,
	0x0F12, 0x4817,
	0x0F12, 0xF000,
	0x0F12, 0xF9F1,
	0x0F12, 0x4917,
	0x0F12, 0x4817,
	0x0F12, 0xF000,
	0x0F12, 0xF9ED,
	0x0F12, 0xBC10,
	0x0F12, 0xBC08,
	0x0F12, 0x4718,
	0x0F12, 0x1100,
	0x0F12, 0xD000,
	0x0F12, 0x267C,
	0x0F12, 0x0000,
	0x0F12, 0x2CE8,
	0x0F12, 0x0000,
	0x0F12, 0x3274,
	0x0F12, 0x7000,
	0x0F12, 0xF400,
	0x0F12, 0xD000,
	0x0F12, 0xF520,
	0x0F12, 0xD000,
	0x0F12, 0x2DF1,
	0x0F12, 0x7000,
	0x0F12, 0x89A9,
	0x0F12, 0x0000,
	0x0F12, 0x2E43,
	0x0F12, 0x7000,
	0x0F12, 0x0140,
	0x0F12, 0x7000,
	0x0F12, 0x2E75,
	0x0F12, 0x7000,
	0x0F12, 0xB4F7,
	0x0F12, 0x0000,
	0x0F12, 0x2EFF,
	0x0F12, 0x7000,
	0x0F12, 0x2F23,
	0x0F12, 0x7000,
	0x0F12, 0x2FCD,
	0x0F12, 0x7000,
	0x0F12, 0x2FE1,
	0x0F12, 0x7000,
	0x0F12, 0x2FB5,
	0x0F12, 0x7000,
	0x0F12, 0x013D,
	0x0F12, 0x0001,
	0x0F12, 0x3067,
	0x0F12, 0x7000,
	0x0F12, 0x5823,
	0x0F12, 0x0000,
	0x0F12, 0x30B5,
	0x0F12, 0x7000,
	0x0F12, 0xD789,
	0x0F12, 0x0000,
	0x0F12, 0xB570,
	0x0F12, 0x6804,
	0x0F12, 0x6845,
	0x0F12, 0x6881,
	0x0F12, 0x6840,
	0x0F12, 0x2900,
	0x0F12, 0x6880,
	0x0F12, 0xD007,
	0x0F12, 0x49C2,
	0x0F12, 0x8949,
	0x0F12, 0x084A,
	0x0F12, 0x1880,
	0x0F12, 0xF000,
	0x0F12, 0xF9B8,
	0x0F12, 0x80A0,
	0x0F12, 0xE000,
	0x0F12, 0x80A0,
	0x0F12, 0x88A0,
	0x0F12, 0x2800,
	0x0F12, 0xD010,
	0x0F12, 0x68A9,
	0x0F12, 0x6828,
	0x0F12, 0x084A,
	0x0F12, 0x1880,
	0x0F12, 0xF000,
	0x0F12, 0xF9AC,
	0x0F12, 0x8020,
	0x0F12, 0x1D2D,
	0x0F12, 0xCD03,
	0x0F12, 0x084A,
	0x0F12, 0x1880,
	0x0F12, 0xF000,
	0x0F12, 0xF9A5,
	0x0F12, 0x8060,
	0x0F12, 0xBC70,
	0x0F12, 0xBC08,
	0x0F12, 0x4718,
	0x0F12, 0x2000,
	0x0F12, 0x8060,
	0x0F12, 0x8020,
	0x0F12, 0xE7F8,
	0x0F12, 0xB510,
	0x0F12, 0xF000,
	0x0F12, 0xF9A0,
	0x0F12, 0x48B1,
	0x0F12, 0x49B2,
	0x0F12, 0x8800,
	0x0F12, 0x4AB2,
	0x0F12, 0x2805,
	0x0F12, 0xD003,
	0x0F12, 0x4BB1,
	0x0F12, 0x795B,
	0x0F12, 0x2B00,
	0x0F12, 0xD005,
	0x0F12, 0x2001,
	0x0F12, 0x8008,
	0x0F12, 0x8010,
	0x0F12, 0xBC10,
	0x0F12, 0xBC08,
	0x0F12, 0x4718,
	0x0F12, 0x2800,
	0x0F12, 0xD1FA,
	0x0F12, 0x2000,
	0x0F12, 0x8008,
	0x0F12, 0x8010,
	0x0F12, 0xE7F6,
	0x0F12, 0xB5F8,
	0x0F12, 0x2407,
	0x0F12, 0x2C06,
	0x0F12, 0xD035,
	0x0F12, 0x2C07,
	0x0F12, 0xD033,
	0x0F12, 0x48A3,
	0x0F12, 0x8BC1,
	0x0F12, 0x2900,
	0x0F12, 0xD02A,
	0x0F12, 0x00A2,
	0x0F12, 0x1815,
	0x0F12, 0x4AA4,
	0x0F12, 0x6DEE,
	0x0F12, 0x8A92,
	0x0F12, 0x4296,
	0x0F12, 0xD923,
	0x0F12, 0x0028,
	0x0F12, 0x3080,
	0x0F12, 0x0007,
	0x0F12, 0x69C0,
	0x0F12, 0xF000,
	0x0F12, 0xF96D,
	0x0F12, 0x1C71,
	0x0F12, 0x0280,
	0x0F12, 0xF000,
	0x0F12, 0xF969,
	0x0F12, 0x0006,
	0x0F12, 0x4898,
	0x0F12, 0x0061,
	0x0F12, 0x1808,
	0x0F12, 0x8D80,
	0x0F12, 0x0A01,
	0x0F12, 0x0600,
	0x0F12, 0x0E00,
	0x0F12, 0x1A08,
	0x0F12, 0xF000,
	0x0F12, 0xF96C,
	0x0F12, 0x0002,
	0x0F12, 0x6DE9,
	0x0F12, 0x6FE8,
	0x0F12, 0x1A08,
	0x0F12, 0x4351,
	0x0F12, 0x0300,
	0x0F12, 0x1C49,
	0x0F12, 0xF000,
	0x0F12, 0xF955,
	0x0F12, 0x0401,
	0x0F12, 0x0430,
	0x0F12, 0x0C00,
	0x0F12, 0x4301,
	0x0F12, 0x61F9,
	0x0F12, 0xE004,
	0x0F12, 0x00A2,
	0x0F12, 0x4990,
	0x0F12, 0x1810,
	0x0F12, 0x3080,
	0x0F12, 0x61C1,
	0x0F12, 0x1E64,
	0x0F12, 0xD2C5,
	0x0F12, 0x2006,
	0x0F12, 0xF000,
	0x0F12, 0xF95B,
	0x0F12, 0x2007,
	0x0F12, 0xF000,
	0x0F12, 0xF958,
	0x0F12, 0xBCF8,
	0x0F12, 0xBC08,
	0x0F12, 0x4718,
	0x0F12, 0xB510,
	0x0F12, 0xF000,
	0x0F12, 0xF95A,
	0x0F12, 0x2800,
	0x0F12, 0xD00A,
	0x0F12, 0x4881,
	0x0F12, 0x8B81,
	0x0F12, 0x0089,
	0x0F12, 0x1808,
	0x0F12, 0x6DC1,
	0x0F12, 0x4883,
	0x0F12, 0x8A80,
	0x0F12, 0x4281,
	0x0F12, 0xD901,
	0x0F12, 0x2001,
	0x0F12, 0xE7A1,
	0x0F12, 0x2000,
	0x0F12, 0xE79F,
	0x0F12, 0xB5F8,
	0x0F12, 0x0004,
	0x0F12, 0x4F80,
	0x0F12, 0x227D,
	0x0F12, 0x8938,
	0x0F12, 0x0152,
	0x0F12, 0x4342,
	0x0F12, 0x487E,
	0x0F12, 0x9000,
	0x0F12, 0x8A01,
	0x0F12, 0x0848,
	0x0F12, 0x1810,
	0x0F12, 0xF000,
	0x0F12, 0xF91F,
	0x0F12, 0x210F,
	0x0F12, 0xF000,
	0x0F12, 0xF942,
	0x0F12, 0x497A,
	0x0F12, 0x8C49,
	0x0F12, 0x090E,
	0x0F12, 0x0136,
	0x0F12, 0x4306,
	0x0F12, 0x4979,
	0x0F12, 0x2C00,
	0x0F12, 0xD003,
	0x0F12, 0x2001,
	0x0F12, 0x0240,
	0x0F12, 0x4330,
	0x0F12, 0x8108,
	0x0F12, 0x4876,
	0x0F12, 0x2C00,
	0x0F12, 0x8D00,
	0x0F12, 0xD001,
	0x0F12, 0x2501,
	0x0F12, 0xE000,
	0x0F12, 0x2500,
	0x0F12, 0x4972,
	0x0F12, 0x4328,
	0x0F12, 0x8008,
	0x0F12, 0x207D,
	0x0F12, 0x00C0,
	0x0F12, 0xF000,
	0x0F12, 0xF930,
	0x0F12, 0x2C00,
	0x0F12, 0x496E,
	0x0F12, 0x0328,
	0x0F12, 0x4330,
	0x0F12, 0x8108,
	0x0F12, 0x88F8,
	0x0F12, 0x2C00,
	0x0F12, 0x01AA,
	0x0F12, 0x4310,
	0x0F12, 0x8088,
	0x0F12, 0x2C00,
	0x0F12, 0xD00B,
	0x0F12, 0x9800,
	0x0F12, 0x8A01,
	0x0F12, 0x4869,
	0x0F12, 0xF000,
	0x0F12, 0xF8F1,
	0x0F12, 0x4969,
	0x0F12, 0x8809,
	0x0F12, 0x4348,
	0x0F12, 0x0400,
	0x0F12, 0x0C00,
	0x0F12, 0xF000,
	0x0F12, 0xF918,
	0x0F12, 0x0020,
	0x0F12, 0xF000,
	0x0F12, 0xF91D,
	0x0F12, 0x4865,
	0x0F12, 0x7004,
	0x0F12, 0xE7A1,
	0x0F12, 0xB510,
	0x0F12, 0x0004,
	0x0F12, 0xF000,
	0x0F12, 0xF91E,
	0x0F12, 0x6020,
	0x0F12, 0x4962,
	0x0F12, 0x8B49,
	0x0F12, 0x0789,
	0x0F12, 0xD001,
	0x0F12, 0x0040,
	0x0F12, 0x6020,
	0x0F12, 0xE74A,
	0x0F12, 0xB510,
	0x0F12, 0xF000,
	0x0F12, 0xF91B,
	0x0F12, 0x485E,
	0x0F12, 0x8880,
	0x0F12, 0x0601,
	0x0F12, 0x4853,
	0x0F12, 0x1609,
	0x0F12, 0x8141,
	0x0F12, 0xE740,
	0x0F12, 0xB5F8,
	0x0F12, 0x000F,
	0x0F12, 0x4C54,
	0x0F12, 0x3420,
	0x0F12, 0x2500,
	0x0F12, 0x5765,
	0x0F12, 0x0039,
	0x0F12, 0xF000,
	0x0F12, 0xF913,
	0x0F12, 0x9000,
	0x0F12, 0x2600,
	0x0F12, 0x57A6,
	0x0F12, 0x4C4B,
	0x0F12, 0x42AE,
	0x0F12, 0xD01B,
	0x0F12, 0x4D53,
	0x0F12, 0x8AE8,
	0x0F12, 0x2800,
	0x0F12, 0xD013,
	0x0F12, 0x484C,
	0x0F12, 0x8A01,
	0x0F12, 0x8B80,
	0x0F12, 0x4378,
	0x0F12, 0xF000,
	0x0F12, 0xF8B5,
	0x0F12, 0x89A9,
	0x0F12, 0x1A41,
	0x0F12, 0x484D,
	0x0F12, 0x3820,
	0x0F12, 0x8AC0,
	0x0F12, 0x4348,
	0x0F12, 0x17C1,
	0x0F12, 0x0D89,
	0x0F12, 0x1808,
	0x0F12, 0x1280,
	0x0F12, 0x8961,
	0x0F12, 0x1A08,
	0x0F12, 0x8160,
	0x0F12, 0xE003,
	0x0F12, 0x88A8,
	0x0F12, 0x0600,
	0x0F12, 0x1600,
	0x0F12, 0x8160,
	0x0F12, 0x200A,
	0x0F12, 0x5E20,
	0x0F12, 0x42B0,
	0x0F12, 0xD011,
	0x0F12, 0xF000,
	0x0F12, 0xF8AB,
	0x0F12, 0x1D40,
	0x0F12, 0x00C3,
	0x0F12, 0x1A18,
	0x0F12, 0x214B,
	0x0F12, 0xF000,
	0x0F12, 0xF897,
	0x0F12, 0x211F,
	0x0F12, 0xF000,
	0x0F12, 0xF8BA,
	0x0F12, 0x210A,
	0x0F12, 0x5E61,
	0x0F12, 0x0FC9,
	0x0F12, 0x0149,
	0x0F12, 0x4301,
	0x0F12, 0x483C,
	0x0F12, 0x81C1,
	0x0F12, 0x9800,
	0x0F12, 0xE748,
	0x0F12, 0xB5F1,
	0x0F12, 0xB082,
	0x0F12, 0x2500,
	0x0F12, 0x4839,
	0x0F12, 0x9001,
	0x0F12, 0x2400,
	0x0F12, 0x2028,
	0x0F12, 0x4368,
	0x0F12, 0x4A39,
	0x0F12, 0x4937,
	0x0F12, 0x1887,
	0x0F12, 0x1840,
	0x0F12, 0x9000,
	0x0F12, 0x9800,
	0x0F12, 0x0066,
	0x0F12, 0x9A01,
	0x0F12, 0x1980,
	0x0F12, 0x218C,
	0x0F12, 0x5A09,
	0x0F12, 0x8A80,
	0x0F12, 0x8812,
	0x0F12, 0xF000,
	0x0F12, 0xF8CA,
	0x0F12, 0x53B8,
	0x0F12, 0x1C64,
	0x0F12, 0x2C14,
	0x0F12, 0xDBF1,
	0x0F12, 0x1C6D,
	0x0F12, 0x2D03,
	0x0F12, 0xDBE6,
	0x0F12, 0x9802,
	0x0F12, 0x6800,
	0x0F12, 0x0600,
	0x0F12, 0x0E00,
	0x0F12, 0xF000,
	0x0F12, 0xF8C5,
	0x0F12, 0xBCFE,
	0x0F12, 0xBC08,
	0x0F12, 0x4718,
	0x0F12, 0xB570,
	0x0F12, 0x6805,
	0x0F12, 0x2404,
	0x0F12, 0xF000,
	0x0F12, 0xF8C5,
	0x0F12, 0x2800,
	0x0F12, 0xD103,
	0x0F12, 0xF000,
	0x0F12, 0xF8C9,
	0x0F12, 0x2800,
	0x0F12, 0xD000,
	0x0F12, 0x2400,
	0x0F12, 0x3540,
	0x0F12, 0x88E8,
	0x0F12, 0x0500,
	0x0F12, 0xD403,
	0x0F12, 0x4822,
	0x0F12, 0x89C0,
	0x0F12, 0x2800,
	0x0F12, 0xD002,
	0x0F12, 0x2008,
	0x0F12, 0x4304,
	0x0F12, 0xE001,
	0x0F12, 0x2010,
	0x0F12, 0x4304,
	0x0F12, 0x481F,
	0x0F12, 0x8B80,
	0x0F12, 0x0700,
	0x0F12, 0x0F81,
	0x0F12, 0x2001,
	0x0F12, 0x2900,
	0x0F12, 0xD000,
	0x0F12, 0x4304,
	0x0F12, 0x491C,
	0x0F12, 0x8B0A,
	0x0F12, 0x42A2,
	0x0F12, 0xD004,
	0x0F12, 0x0762,
	0x0F12, 0xD502,
	0x0F12, 0x4A19,
	0x0F12, 0x3220,
	0x0F12, 0x8110,
	0x0F12, 0x830C,
	0x0F12, 0xE693,
	0x0F12, 0x0C3C,
	0x0F12, 0x7000,
	0x0F12, 0x26E8,
	0x0F12, 0x7000,
	0x0F12, 0x6100,
	0x0F12, 0xD000,
	0x0F12, 0x6500,
	0x0F12, 0xD000,
	0x0F12, 0x1A7C,
	0x0F12, 0x7000,
	0x0F12, 0x1120,
	0x0F12, 0x7000,
	0x0F12, 0xFFFF,
	0x0F12, 0x0000,
	0x0F12, 0x3374,
	0x0F12, 0x7000,
	0x0F12, 0x1D6C,
	0x0F12, 0x7000,
	0x0F12, 0x167C,
	0x0F12, 0x7000,
	0x0F12, 0xF400,
	0x0F12, 0xD000,
	0x0F12, 0x2C2C,
	0x0F12, 0x7000,
	0x0F12, 0x40A0,
	0x0F12, 0x00DD,
	0x0F12, 0xF520,
	0x0F12, 0xD000,
	0x0F12, 0x2C29,
	0x0F12, 0x7000,
	0x0F12, 0x1A54,
	0x0F12, 0x7000,
	0x0F12, 0x1564,
	0x0F12, 0x7000,
	0x0F12, 0xF2A0,
	0x0F12, 0xD000,
	0x0F12, 0x2440,
	0x0F12, 0x7000,
	0x0F12, 0x3274,
	0x0F12, 0x7000,
	0x0F12, 0x05A0,
	0x0F12, 0x7000,
	0x0F12, 0x2894,
	0x0F12, 0x7000,
	0x0F12, 0x1224,
	0x0F12, 0x7000,
	0x0F12, 0xB000,
	0x0F12, 0xD000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0x1A3F,
	0x0F12, 0x0001,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xF004,
	0x0F12, 0xE51F,
	0x0F12, 0x1F48,
	0x0F12, 0x0001,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0x24BD,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0x36DD,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0xB4CF,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0xB5D7,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0x36ED,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0xF53F,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0xF5D9,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0x013D,
	0x0F12, 0x0001,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0xF5C9,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0xFAA9,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0x3723,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0x5823,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0xD771,
	0x0F12, 0x0000,
	0x0F12, 0x4778,
	0x0F12, 0x46C0,
	0x0F12, 0xC000,
	0x0F12, 0xE59F,
	0x0F12, 0xFF1C,
	0x0F12, 0xE12F,
	0x0F12, 0xD75B,
	0x0F12, 0x0000,
	0x0F12, 0x7E77,
	0x0F12, 0x0000,
	// End T&P part
};
static unsigned short s5k5cagx_init_reg_short1[]={	

// Start T&P part
// DO NOT DELETE T&P SECTION COMMENTS! They are required to debug T&P related issues.
// svn://transrdsrv/svn/svnroot/System/Software/tcevb/SDK+FW/ISP_5CA/Firmware
// Rev: 32375-32375
// Signature:
// md5 78c1a0d32ef22ba270994f08d64a05a0 .btp
// md5 6765ffc40fde4420aab81f0039a60c38 .htp
// md5 956e8c724c34dd8b76dd297b92f59677 .RegsMap.h
// md5 7db8e8f88de22128b8b909128f087a53 .RegsMap.bin
// md5 506b4144bd48cdb79cbecdda4f7176ba .base.RegsMap.h
// md5 fd8f92f13566c1a788746b23691c5f5f .base.RegsMap.bin
//
0x0028, 0x7000,
0x002A, 0x2CF8,
0x0F12, 0xB510,
0x0F12, 0x4827,
0x0F12, 0x21C0,
0x0F12, 0x8041,
0x0F12, 0x4825,
0x0F12, 0x4A26,
0x0F12, 0x3020,
0x0F12, 0x8382,
0x0F12, 0x1D12,
0x0F12, 0x83C2,
0x0F12, 0x4822,
0x0F12, 0x3040,
0x0F12, 0x8041,
0x0F12, 0x4821,
0x0F12, 0x4922,
0x0F12, 0x3060,
0x0F12, 0x8381,
0x0F12, 0x1D09,
0x0F12, 0x83C1,
0x0F12, 0x4821,
0x0F12, 0x491D,
0x0F12, 0x8802,
0x0F12, 0x3980,
0x0F12, 0x804A,
0x0F12, 0x8842,
0x0F12, 0x808A,
0x0F12, 0x8882,
0x0F12, 0x80CA,
0x0F12, 0x88C2,
0x0F12, 0x810A,
0x0F12, 0x8902,
0x0F12, 0x491C,
0x0F12, 0x80CA,
0x0F12, 0x8942,
0x0F12, 0x814A,
0x0F12, 0x8982,
0x0F12, 0x830A,
0x0F12, 0x89C2,
0x0F12, 0x834A,
0x0F12, 0x8A00,
0x0F12, 0x4918,
0x0F12, 0x8188,
0x0F12, 0x4918,
0x0F12, 0x4819,
0x0F12, 0xF000,
0x0F12, 0xF9F4,
0x0F12, 0x4918,
0x0F12, 0x4819,
0x0F12, 0x6341,
0x0F12, 0x4919,
0x0F12, 0x4819,
0x0F12, 0xF000,
0x0F12, 0xF9ED,
0x0F12, 0x4816,
0x0F12, 0x4918,
0x0F12, 0x3840,
0x0F12, 0x62C1,
0x0F12, 0x4918,
0x0F12, 0x3880,
0x0F12, 0x63C1,
0x0F12, 0x4917,
0x0F12, 0x6301,
0x0F12, 0x4917,
0x0F12, 0x3040,
0x0F12, 0x6181,
0x0F12, 0x4917,
0x0F12, 0x4817,
0x0F12, 0xF000,
0x0F12, 0xF9DD,
0x0F12, 0x4917,
0x0F12, 0x4817,
0x0F12, 0xF000,
0x0F12, 0xF9D9,
0x0F12, 0x4917,
0x0F12, 0x4817,
0x0F12, 0xF000,
0x0F12, 0xF9D5,
0x0F12, 0xBC10,
0x0F12, 0xBC08,
0x0F12, 0x4718,
0x0F12, 0x1100,
0x0F12, 0xD000,
0x0F12, 0x267C,
0x0F12, 0x0000,
0x0F12, 0x2CE8,
0x0F12, 0x0000,
0x0F12, 0x3274,
0x0F12, 0x7000,
0x0F12, 0xF400,
0x0F12, 0xD000,
0x0F12, 0xF520,
0x0F12, 0xD000,
0x0F12, 0x2DF1,
0x0F12, 0x7000,
0x0F12, 0x89A9,
0x0F12, 0x0000,
0x0F12, 0x2E43,
0x0F12, 0x7000,
0x0F12, 0x0140,
0x0F12, 0x7000,
0x0F12, 0x2E7D,
0x0F12, 0x7000,
0x0F12, 0xB4F7,
0x0F12, 0x0000,
0x0F12, 0x2F07,
0x0F12, 0x7000,
0x0F12, 0x2F2B,
0x0F12, 0x7000,
0x0F12, 0x2FD1,
0x0F12, 0x7000,
0x0F12, 0x2FE5,
0x0F12, 0x7000,
0x0F12, 0x2FB9,
0x0F12, 0x7000,
0x0F12, 0x013D,
0x0F12, 0x0001,
0x0F12, 0x306B,
0x0F12, 0x7000,
0x0F12, 0x5823,
0x0F12, 0x0000,
0x0F12, 0x30B9,
0x0F12, 0x7000,
0x0F12, 0xA565,
0x0F12, 0x0000,
0x0F12, 0xB570,
0x0F12, 0x6804,
0x0F12, 0x6845,
0x0F12, 0x6881,
0x0F12, 0x6840,
0x0F12, 0x2900,
0x0F12, 0x6880,
0x0F12, 0xD007,
0x0F12, 0x49B8,
0x0F12, 0x8949,
0x0F12, 0x084A,
0x0F12, 0x1880,
0x0F12, 0xF000,
0x0F12, 0xF9A0,
0x0F12, 0x80A0,
0x0F12, 0xE000,
0x0F12, 0x80A0,
0x0F12, 0x88A0,
0x0F12, 0x2800,
0x0F12, 0xD010,
0x0F12, 0x68A9,
0x0F12, 0x6828,
0x0F12, 0x084A,
0x0F12, 0x1880,
0x0F12, 0xF000,
0x0F12, 0xF994,
0x0F12, 0x8020,
0x0F12, 0x1D2D,
0x0F12, 0xCD03,
0x0F12, 0x084A,
0x0F12, 0x1880,
0x0F12, 0xF000,
0x0F12, 0xF98D,
0x0F12, 0x8060,
0x0F12, 0xBC70,
0x0F12, 0xBC08,
0x0F12, 0x4718,
0x0F12, 0x2000,
0x0F12, 0x8060,
0x0F12, 0x8020,
0x0F12, 0xE7F8,
0x0F12, 0xB510,
0x0F12, 0xF000,
0x0F12, 0xF988,
0x0F12, 0x48A7,
0x0F12, 0x8A40,
0x0F12, 0x2800,
0x0F12, 0xD00C,
0x0F12, 0x48A6,
0x0F12, 0x49A7,
0x0F12, 0x8800,
0x0F12, 0x4AA7,
0x0F12, 0x2805,
0x0F12, 0xD003,
0x0F12, 0x4BA6,
0x0F12, 0x795B,
0x0F12, 0x2B00,
0x0F12, 0xD005,
0x0F12, 0x2001,
0x0F12, 0x8008,
0x0F12, 0x8010,
0x0F12, 0xBC10,
0x0F12, 0xBC08,
0x0F12, 0x4718,
0x0F12, 0x2800,
0x0F12, 0xD1FA,
0x0F12, 0x2000,
0x0F12, 0x8008,
0x0F12, 0x8010,
0x0F12, 0xE7F6,
0x0F12, 0xB5F8,
0x0F12, 0x2407,
0x0F12, 0x2C06,
0x0F12, 0xD035,
0x0F12, 0x2C07,
0x0F12, 0xD033,
0x0F12, 0x4898,
0x0F12, 0x8BC1,
0x0F12, 0x2900,
0x0F12, 0xD02A,
0x0F12, 0x00A2,
0x0F12, 0x1815,
0x0F12, 0x4A99,
0x0F12, 0x6DEE,
0x0F12, 0x8A92,
0x0F12, 0x4296,
0x0F12, 0xD923,
0x0F12, 0x0028,
0x0F12, 0x3080,
0x0F12, 0x0007,
0x0F12, 0x69C0,
0x0F12, 0xF000,
0x0F12, 0xF951,
0x0F12, 0x1C71,
0x0F12, 0x0280,
0x0F12, 0xF000,
0x0F12, 0xF94D,
0x0F12, 0x0006,
0x0F12, 0x488D,
0x0F12, 0x0061,
0x0F12, 0x1808,
0x0F12, 0x8D80,
0x0F12, 0x0A01,
0x0F12, 0x0600,
0x0F12, 0x0E00,
0x0F12, 0x1A08,
0x0F12, 0xF000,
0x0F12, 0xF950,
0x0F12, 0x0002,
0x0F12, 0x6DE9,
0x0F12, 0x6FE8,
0x0F12, 0x1A08,
0x0F12, 0x4351,
0x0F12, 0x0300,
0x0F12, 0x1C49,
0x0F12, 0xF000,
0x0F12, 0xF939,
0x0F12, 0x0401,
0x0F12, 0x0430,
0x0F12, 0x0C00,
0x0F12, 0x4301,
0x0F12, 0x61F9,
0x0F12, 0xE004,
0x0F12, 0x00A2,
0x0F12, 0x4985,
0x0F12, 0x1810,
0x0F12, 0x3080,
0x0F12, 0x61C1,
0x0F12, 0x1E64,
0x0F12, 0xD2C5,
0x0F12, 0x2006,
0x0F12, 0xF000,
0x0F12, 0xF93F,
0x0F12, 0x2007,
0x0F12, 0xF000,
0x0F12, 0xF93C,
0x0F12, 0xBCF8,
0x0F12, 0xBC08,
0x0F12, 0x4718,
0x0F12, 0xB510,
0x0F12, 0xF000,
0x0F12, 0xF93E,
0x0F12, 0x2800,
0x0F12, 0xD00A,
0x0F12, 0x4876,
0x0F12, 0x8B81,
0x0F12, 0x0089,
0x0F12, 0x1808,
0x0F12, 0x6DC1,
0x0F12, 0x4878,
0x0F12, 0x8A80,
0x0F12, 0x4281,
0x0F12, 0xD901,
0x0F12, 0x2001,
0x0F12, 0xE7A1,
0x0F12, 0x2000,
0x0F12, 0xE79F,
0x0F12, 0xB5F8,
0x0F12, 0x0004,
0x0F12, 0x4F75,
0x0F12, 0x227D,
0x0F12, 0x8938,
0x0F12, 0x0152,
0x0F12, 0x4342,
0x0F12, 0x4873,
0x0F12, 0x9000,
0x0F12, 0x8A01,
0x0F12, 0x0848,
0x0F12, 0x1810,
0x0F12, 0xF000,
0x0F12, 0xF903,
0x0F12, 0x210F,
0x0F12, 0xF000,
0x0F12, 0xF926,
0x0F12, 0x496F,
0x0F12, 0x8C49,
0x0F12, 0x090E,
0x0F12, 0x0136,
0x0F12, 0x4306,
0x0F12, 0x496E,
0x0F12, 0x2C00,
0x0F12, 0xD003,
0x0F12, 0x2001,
0x0F12, 0x0240,
0x0F12, 0x4330,
0x0F12, 0x8108,
0x0F12, 0x486B,
0x0F12, 0x2C00,
0x0F12, 0x8D00,
0x0F12, 0xD001,
0x0F12, 0x2501,
0x0F12, 0xE000,
0x0F12, 0x2500,
0x0F12, 0x4967,
0x0F12, 0x4328,
0x0F12, 0x8008,
0x0F12, 0x207D,
0x0F12, 0x00C0,
0x0F12, 0xF000,
0x0F12, 0xF914,
0x0F12, 0x2C00,
0x0F12, 0x4963,
0x0F12, 0x0328,
0x0F12, 0x4330,
0x0F12, 0x8108,
0x0F12, 0x88F8,
0x0F12, 0x2C00,
0x0F12, 0x01AA,
0x0F12, 0x4310,
0x0F12, 0x8088,
0x0F12, 0x9800,
0x0F12, 0x8A01,
0x0F12, 0x485F,
0x0F12, 0xF000,
0x0F12, 0xF8D7,
0x0F12, 0x495F,
0x0F12, 0x8809,
0x0F12, 0x4348,
0x0F12, 0x0400,
0x0F12, 0x0C00,
0x0F12, 0xF000,
0x0F12, 0xF8FE,
0x0F12, 0x0020,
0x0F12, 0xF000,
0x0F12, 0xF903,
0x0F12, 0x485B,
0x0F12, 0x7004,
0x0F12, 0xE7A3,
0x0F12, 0xB510,
0x0F12, 0x0004,
0x0F12, 0xF000,
0x0F12, 0xF904,
0x0F12, 0x6020,
0x0F12, 0x4958,
0x0F12, 0x8B49,
0x0F12, 0x0789,
0x0F12, 0xD001,
0x0F12, 0x0040,
0x0F12, 0x6020,
0x0F12, 0xE74C,
0x0F12, 0xB510,
0x0F12, 0xF000,
0x0F12, 0xF901,
0x0F12, 0x4854,
0x0F12, 0x8880,
0x0F12, 0x0601,
0x0F12, 0x4849,
0x0F12, 0x1609,
0x0F12, 0x8141,
0x0F12, 0xE742,
0x0F12, 0xB5F8,
0x0F12, 0x000F,
0x0F12, 0x4C4A,
0x0F12, 0x3420,
0x0F12, 0x2500,
0x0F12, 0x5765,
0x0F12, 0x0039,
0x0F12, 0xF000,
0x0F12, 0xF8F9,
0x0F12, 0x9000,
0x0F12, 0x2600,
0x0F12, 0x57A6,
0x0F12, 0x4C41,
0x0F12, 0x42AE,
0x0F12, 0xD01B,
0x0F12, 0x4D49,
0x0F12, 0x8AE8,
0x0F12, 0x2800,
0x0F12, 0xD013,
0x0F12, 0x4842,
0x0F12, 0x8A01,
0x0F12, 0x8B80,
0x0F12, 0x4378,
0x0F12, 0xF000,
0x0F12, 0xF89B,
0x0F12, 0x89A9,
0x0F12, 0x1A41,
0x0F12, 0x4843,
0x0F12, 0x3820,
0x0F12, 0x8AC0,
0x0F12, 0x4348,
0x0F12, 0x17C1,
0x0F12, 0x0D89,
0x0F12, 0x1808,
0x0F12, 0x1280,
0x0F12, 0x8961,
0x0F12, 0x1A08,
0x0F12, 0x8160,
0x0F12, 0xE003,
0x0F12, 0x88A8,
0x0F12, 0x0600,
0x0F12, 0x1600,
0x0F12, 0x8160,
0x0F12, 0x200A,
0x0F12, 0x5E20,
0x0F12, 0x42B0,
0x0F12, 0xD011,
0x0F12, 0xF000,
0x0F12, 0xF891,
0x0F12, 0x1D40,
0x0F12, 0x00C3,
0x0F12, 0x1A18,
0x0F12, 0x214B,
0x0F12, 0xF000,
0x0F12, 0xF87D,
0x0F12, 0x211F,
0x0F12, 0xF000,
0x0F12, 0xF8A0,
0x0F12, 0x210A,
0x0F12, 0x5E61,
0x0F12, 0x0FC9,
0x0F12, 0x0149,
0x0F12, 0x4301,
0x0F12, 0x4832,
0x0F12, 0x81C1,
0x0F12, 0x9800,
0x0F12, 0xE74A,
0x0F12, 0xB5F1,
0x0F12, 0xB082,
0x0F12, 0x2500,
0x0F12, 0x482F,
0x0F12, 0x9001,
0x0F12, 0x2400,
0x0F12, 0x2028,
0x0F12, 0x4368,
0x0F12, 0x4A2E,
0x0F12, 0x491A,
0x0F12, 0x1887,
0x0F12, 0x1840,
0x0F12, 0x9000,
0x0F12, 0x9800,
0x0F12, 0x0066,
0x0F12, 0x9A01,
0x0F12, 0x1980,
0x0F12, 0x218C,
0x0F12, 0x5A09,
0x0F12, 0x8A80,
0x0F12, 0x8812,
0x0F12, 0xF000,
0x0F12, 0xF8B0,
0x0F12, 0x53B8,
0x0F12, 0x1C64,
0x0F12, 0x2C14,
0x0F12, 0xDBF1,
0x0F12, 0x1C6D,
0x0F12, 0x2D03,
0x0F12, 0xDBE6,
0x0F12, 0x9802,
0x0F12, 0x6800,
0x0F12, 0x0600,
0x0F12, 0x0E00,
0x0F12, 0xF000,
0x0F12, 0xF8AB,
0x0F12, 0xBCFE,
0x0F12, 0xBC08,
0x0F12, 0x4718,
0x0F12, 0xB510,
0x0F12, 0x0004,
0x0F12, 0x6861,
0x0F12, 0x4811,
0x0F12, 0x8880,
0x0F12, 0x2800,
0x0F12, 0xD009,
0x0F12, 0x481A,
0x0F12, 0x000B,
0x0F12, 0x3860,
0x0F12, 0x6E82,
0x0F12, 0x481A,
0x0F12, 0x2105,
0x0F12, 0xF000,
0x0F12, 0xF8A1,
0x0F12, 0x6020,
0x0F12, 0xE6C7,
0x0F12, 0x6820,
0x0F12, 0xF000,
0x0F12, 0xF8A4,
0x0F12, 0x6020,
0x0F12, 0xE6C2,
0x0F12, 0x0C3C,
0x0F12, 0x7000,
0x0F12, 0x3274,
0x0F12, 0x7000,
0x0F12, 0x26E8,
0x0F12, 0x7000,
0x0F12, 0x6100,
0x0F12, 0xD000,
0x0F12, 0x6500,
0x0F12, 0xD000,
0x0F12, 0x1A7C,
0x0F12, 0x7000,
0x0F12, 0x1120,
0x0F12, 0x7000,
0x0F12, 0xFFFF,
0x0F12, 0x0000,
0x0F12, 0x3374,
0x0F12, 0x7000,
0x0F12, 0x1D6C,
0x0F12, 0x7000,
0x0F12, 0x167C,
0x0F12, 0x7000,
0x0F12, 0xF400,
0x0F12, 0xD000,
0x0F12, 0x2C2C,
0x0F12, 0x7000,
0x0F12, 0x40A0,
0x0F12, 0x00DD,
0x0F12, 0xF520,
0x0F12, 0xD000,
0x0F12, 0x2C29,
0x0F12, 0x7000,
0x0F12, 0x1A54,
0x0F12, 0x7000,
0x0F12, 0x1564,
0x0F12, 0x7000,
0x0F12, 0xF2A0,
0x0F12, 0xD000,
0x0F12, 0x2440,
0x0F12, 0x7000,
0x0F12, 0x05A0,
0x0F12, 0x7000,
0x0F12, 0x0764,
0x0F12, 0x7000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0x1A3F,
0x0F12, 0x0001,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xF004,
0x0F12, 0xE51F,
0x0F12, 0x1F48,
0x0F12, 0x0001,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0x24BD,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0x36DD,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0xB4CF,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0xB5D7,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0x36ED,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0xF53F,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0xF5D9,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0x013D,
0x0F12, 0x0001,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0xF5C9,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0xFAA9,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0x3723,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0x5823,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0xA48B,
0x0F12, 0x0000,
0x0F12, 0x4778,
0x0F12, 0x46C0,
0x0F12, 0xC000,
0x0F12, 0xE59F,
0x0F12, 0xFF1C,
0x0F12, 0xE12F,
0x0F12, 0xA565,
0x0F12, 0x0000,
0x0F12, 0x8403,
0x0F12, 0x0000,

// End T&P part
0x002A, 0x0572,     //skl_usConfigStbySettings  2011/01/10  
0x0F12, 0x0007,


// End T&P part
};
	static unsigned short s5k5cagx_init_reg_short2[]={		
	//============================================================
	// CIS/APS/Analog setting- 400LSBSYSCLK 45MHz -091127 
	//============================================================
	// This registers are for FACTORY ONLY. If you change it without prior notification, 
	// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
	//============================================================
	0x0028, 0x7000,
	0x002A, 0x157A,
	0x0F12, 0x0001,
	0x002A, 0x1578,
	0x0F12, 0x0001,
	0x002A, 0x1576,
	0x0F12, 0x0020,
	0x002A, 0x1574,
	0x0F12, 0x0006,
	0x002A, 0x156E,
	0x0F12, 0x0001, // Slope calibration tolerance in units of 1/256
	0x002A, 0x1568,
	0x0F12, 0x00FC,
	//ADC control
	0x002A, 0x155A,
	0x0F12, 0x01CC, //ADC SAT of 450mV for 10bit default in EVT1
	0x002A, 0x157E,
	0x0F12, 0x0C80, // 3200 Max. Reset ramp DCLK counts (default 2048 0x800)
	0x0F12, 0x0578, // 1400 Max. Reset ramp DCLK counts for x3.5
	0x002A, 0x157C,
	0x0F12, 0x0190, // 400 Reset ramp for x1 in DCLK counts
	0x002A, 0x1570,
	0x0F12, 0x00A0, // 256 LSB
	0x0F12, 0x0010, // reset threshold
	0x002A, 0x12C4,
	0x0F12, 0x006A, // 106 additional timing columns.
	0x002A, 0x12C8,
	0x0F12, 0x08AC, // 2220 ADC columns in normal mode including Hold & Latch
	0x0F12, 0x0050, // 80 addition of ADC columns in Y-ave mode (default 244 0x74)
	//WRITE #senHal_ForceModeType 0001 // Long exposure mode
	0x002A, 0x1696,
	0x0F12, 0x0000, // based on APS guidelines
	0x0F12, 0x0000, // based on APS guidelines
	0x0F12, 0x00C6, // default. 1492 used for ADC dark characteristics
	0x0F12, 0x00C6,
	0x002A, 0x12B8,
	0x0F12, 0x1000, //disable CINTR 0
	0x002A, 0x1690,
	0x0F12, 0x0001, // when set double sampling is activated - requires different set of pointers
	0x002A, 0x12B0,
	0x0F12, 0x0055, // comp and pixel bias control 0xF40E - default for EVT1
	0x0F12, 0x005A, // comp and pixel bias control 0xF40E for binning mode
	0x002A, 0x337A,
	0x0F12, 0x0006, // [7] - is used for rest-only mode (EVT0 value is 0xD and HW 0x6)
	0x0F12, 0x0068, // 104M
	0x002A, 0x327C,
	0x0F12, 0x1000, // [11]: Enable DBLR Regulation
	0x0F12, 0x6998, // [3:0]: VPIX ~2.8V ****
	0x0F12, 0x0078, // [0]: Static RC-filter
	0x0F12, 0x04FE, // [7:4]: Full RC-filter
	0x0F12, 0x8800, // [11]: Add load to CDS block
	0x002A, 0x3274,
	0x0F12, 0x0155, //#Tune_TP_IO_DrivingCurrent_D0_D4_cs10Set IO driving current 
	0x0F12, 0x0155, //#Tune_TP_IO_DrivingCurrent_D9_D5_cs10Set IO driving current
	0x0F12, 0x1555, //#Tune_TP_IO_DrivingCurrent_GPIO_cd10 Set IO driving current
	0x0F12, 0x0555, //#Tune_TP_IO_DrivingCurrent_CLKs_cd10 Set IO driving current
	0x002A, 0x169E,
	0x0F12, 0x0007, // [3:0]- specifies the target (default 7)- DCLK = 64MHz instead of 116MHz.
	0x002A, 0x0BF6,
	0x0F12, 0x0000, //from ALEX //Enable Bayer Downscaler
	//Asserting CDS pointers - Long exposure MS Normal
	//Conditions: 10bit, ADC_SAT = 450mV ; ramp_del = 40 ; ramp_start = 60
	0x002A, 0x12D2,
	0x0F12, 0x0003, //#senHal_pContSenModesRegsArray[0][0]2 700012D2
	0x0F12, 0x0003, //#senHal_pContSenModesRegsArray[0][1]2 700012D4
	0x0F12, 0x0003, //#senHal_pContSenModesRegsArray[0][2]2 700012D6
	0x0F12, 0x0003, //#senHal_pContSenModesRegsArray[0][3]2 700012D8
	0x0F12, 0x0884, //#senHal_pContSenModesRegsArray[1][0]2 700012DA
	0x0F12, 0x08CF, //#senHal_pContSenModesRegsArray[1][1]2 700012DC
	0x0F12, 0x0500, //#senHal_pContSenModesRegsArray[1][2]2 700012DE
	0x0F12, 0x054B, //#senHal_pContSenModesRegsArray[1][3]2 700012E0
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[2][0]2 700012E2
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[2][1]2 700012E4
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[2][2]2 700012E6
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[2][3]2 700012E8
	0x0F12, 0x0885, //#senHal_pContSenModesRegsArray[3][0]2 700012EA
	0x0F12, 0x0467, //#senHal_pContSenModesRegsArray[3][1]2 700012EC
	0x0F12, 0x0501, //#senHal_pContSenModesRegsArray[3][2]2 700012EE
	0x0F12, 0x02A5, //#senHal_pContSenModesRegsArray[3][3]2 700012F0
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[4][0]2 700012F2
	0x0F12, 0x046A, //#senHal_pContSenModesRegsArray[4][1]2 700012F4
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[4][2]2 700012F6
	0x0F12, 0x02A8, //#senHal_pContSenModesRegsArray[4][3]2 700012F8
	0x0F12, 0x0885, //#senHal_pContSenModesRegsArray[5][0]2 700012FA
	0x0F12, 0x08D0, //#senHal_pContSenModesRegsArray[5][1]2 700012FC
	0x0F12, 0x0501, //#senHal_pContSenModesRegsArray[5][2]2 700012FE
	0x0F12, 0x054C, //#senHal_pContSenModesRegsArray[5][3]2 70001300
	0x0F12, 0x0006, //#senHal_pContSenModesRegsArray[6][0]2 70001302
	0x0F12, 0x0020, //#senHal_pContSenModesRegsArray[6][1]2 70001304
	0x0F12, 0x0006, //#senHal_pContSenModesRegsArray[6][2]2 70001306
	0x0F12, 0x0020, //#senHal_pContSenModesRegsArray[6][3]2 70001308
	0x0F12, 0x0881, //#senHal_pContSenModesRegsArray[7][0]2 7000130A
	0x0F12, 0x0463, //#senHal_pContSenModesRegsArray[7][1]2 7000130C
	0x0F12, 0x04FD, //#senHal_pContSenModesRegsArray[7][2]2 7000130E
	0x0F12, 0x02A1, //#senHal_pContSenModesRegsArray[7][3]2 70001310
	0x0F12, 0x0006, //#senHal_pContSenModesRegsArray[8][0]2 70001312
	0x0F12, 0x0489, //#senHal_pContSenModesRegsArray[8][1]2 70001314
	0x0F12, 0x0006, //#senHal_pContSenModesRegsArray[8][2]2 70001316
	0x0F12, 0x02C7, //#senHal_pContSenModesRegsArray[8][3]2 70001318
	0x0F12, 0x0881, //#senHal_pContSenModesRegsArray[9][0]2 7000131A
	0x0F12, 0x08CC, //#senHal_pContSenModesRegsArray[9][1]2 7000131C
	0x0F12, 0x04FD, //#senHal_pContSenModesRegsArray[9][2]2 7000131E
	0x0F12, 0x0548, //#senHal_pContSenModesRegsArray[9][3]2 70001320
	0x0F12, 0x03A2, //#senHal_pContSenModesRegsArray[10][0] 2 70001322
	0x0F12, 0x01D3, //#senHal_pContSenModesRegsArray[10][1] 2 70001324
	0x0F12, 0x01E0, //#senHal_pContSenModesRegsArray[10][2] 2 70001326
	0x0F12, 0x00F2, //#senHal_pContSenModesRegsArray[10][3] 2 70001328
	0x0F12, 0x03F2, //#senHal_pContSenModesRegsArray[11][0] 2 7000132A
	0x0F12, 0x0223, //#senHal_pContSenModesRegsArray[11][1] 2 7000132C
	0x0F12, 0x0230, //#senHal_pContSenModesRegsArray[11][2] 2 7000132E
	0x0F12, 0x0142, //#senHal_pContSenModesRegsArray[11][3] 2 70001330
	0x0F12, 0x03A2, //#senHal_pContSenModesRegsArray[12][0] 2 70001332
	0x0F12, 0x063C, //#senHal_pContSenModesRegsArray[12][1] 2 70001334
	0x0F12, 0x01E0, //#senHal_pContSenModesRegsArray[12][2] 2 70001336
	0x0F12, 0x0399, //#senHal_pContSenModesRegsArray[12][3] 2 70001338
	0x0F12, 0x03F2, //#senHal_pContSenModesRegsArray[13][0] 2 7000133A
	0x0F12, 0x068C, //#senHal_pContSenModesRegsArray[13][1] 2 7000133C
	0x0F12, 0x0230, //#senHal_pContSenModesRegsArray[13][2] 2 7000133E
	0x0F12, 0x03E9, //#senHal_pContSenModesRegsArray[13][3] 2 70001340
	0x0F12, 0x0002, //#senHal_pContSenModesRegsArray[14][0] 2 70001342
	0x0F12, 0x0002, //#senHal_pContSenModesRegsArray[14][1] 2 70001344
	0x0F12, 0x0002, //#senHal_pContSenModesRegsArray[14][2] 2 70001346
	0x0F12, 0x0002, //#senHal_pContSenModesRegsArray[14][3] 2 70001348
	0x0F12, 0x003C, //#senHal_pContSenModesRegsArray[15][0] 2 7000134A
	0x0F12, 0x003C, //#senHal_pContSenModesRegsArray[15][1] 2 7000134C
	0x0F12, 0x003C, //#senHal_pContSenModesRegsArray[15][2] 2 7000134E
	0x0F12, 0x003C, //#senHal_pContSenModesRegsArray[15][3] 2 70001350
	0x0F12, 0x01D3, //#senHal_pContSenModesRegsArray[16][0] 2 70001352
	0x0F12, 0x01D3, //#senHal_pContSenModesRegsArray[16][1] 2 70001354
	0x0F12, 0x00F2, //#senHal_pContSenModesRegsArray[16][2] 2 70001356
	0x0F12, 0x00F2, //#senHal_pContSenModesRegsArray[16][3] 2 70001358
	0x0F12, 0x020B, //#senHal_pContSenModesRegsArray[17][0] 2 7000135A
	0x0F12, 0x024A, //#senHal_pContSenModesRegsArray[17][1] 2 7000135C
	0x0F12, 0x012A, //#senHal_pContSenModesRegsArray[17][2] 2 7000135E
	0x0F12, 0x0169, //#senHal_pContSenModesRegsArray[17][3] 2 70001360
	0x0F12, 0x0002, //#senHal_pContSenModesRegsArray[18][0] 2 70001362
	0x0F12, 0x046B, //#senHal_pContSenModesRegsArray[18][1] 2 70001364
	0x0F12, 0x0002, //#senHal_pContSenModesRegsArray[18][2] 2 70001366
	0x0F12, 0x02A9, //#senHal_pContSenModesRegsArray[18][3] 2 70001368
	0x0F12, 0x0419, //#senHal_pContSenModesRegsArray[19][0] 2 7000136A
	0x0F12, 0x04A5, //#senHal_pContSenModesRegsArray[19][1] 2 7000136C
	0x0F12, 0x0257, //#senHal_pContSenModesRegsArray[19][2] 2 7000136E
	0x0F12, 0x02E3, //#senHal_pContSenModesRegsArray[19][3] 2 70001370
	0x0F12, 0x0630, //#senHal_pContSenModesRegsArray[20][0] 2 70001372
	0x0F12, 0x063C, //#senHal_pContSenModesRegsArray[20][1] 2 70001374
	0x0F12, 0x038D, //#senHal_pContSenModesRegsArray[20][2] 2 70001376
	0x0F12, 0x0399, //#senHal_pContSenModesRegsArray[20][3] 2 70001378
	0x0F12, 0x0668, //#senHal_pContSenModesRegsArray[21][0] 2 7000137A
	0x0F12, 0x06B3, //#senHal_pContSenModesRegsArray[21][1] 2 7000137C
	0x0F12, 0x03C5, //#senHal_pContSenModesRegsArray[21][2] 2 7000137E
	0x0F12, 0x0410, //#senHal_pContSenModesRegsArray[21][3] 2 70001380
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[22][0] 2 70001382
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[22][1] 2 70001384
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[22][2] 2 70001386
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[22][3] 2 70001388
	0x0F12, 0x03A2, //#senHal_pContSenModesRegsArray[23][0] 2 7000138A
	0x0F12, 0x01D3, //#senHal_pContSenModesRegsArray[23][1] 2 7000138C
	0x0F12, 0x01E0, //#senHal_pContSenModesRegsArray[23][2] 2 7000138E
	0x0F12, 0x00F2, //#senHal_pContSenModesRegsArray[23][3] 2 70001390
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[24][0] 2 70001392
	0x0F12, 0x0461, //#senHal_pContSenModesRegsArray[24][1] 2 70001394
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[24][2] 2 70001396
	0x0F12, 0x029F, //#senHal_pContSenModesRegsArray[24][3] 2 70001398
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[25][0] 2 7000139A
	0x0F12, 0x063C, //#senHal_pContSenModesRegsArray[25][1] 2 7000139C
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[25][2] 2 7000139E
	0x0F12, 0x0399, //#senHal_pContSenModesRegsArray[25][3] 2 700013A0
	0x0F12, 0x003D, //#senHal_pContSenModesRegsArray[26][0] 2 700013A2
	0x0F12, 0x003D, //#senHal_pContSenModesRegsArray[26][1] 2 700013A4
	0x0F12, 0x003D, //#senHal_pContSenModesRegsArray[26][2] 2 700013A6
	0x0F12, 0x003D, //#senHal_pContSenModesRegsArray[26][3] 2 700013A8
	0x0F12, 0x01D0, //#senHal_pContSenModesRegsArray[27][0] 2 700013AA
	0x0F12, 0x01D0, //#senHal_pContSenModesRegsArray[27][1] 2 700013AC
	0x0F12, 0x00EF, //#senHal_pContSenModesRegsArray[27][2] 2 700013AE
	0x0F12, 0x00EF, //#senHal_pContSenModesRegsArray[27][3] 2 700013B0
	0x0F12, 0x020C, //#senHal_pContSenModesRegsArray[28][0] 2 700013B2
	0x0F12, 0x024B, //#senHal_pContSenModesRegsArray[28][1] 2 700013B4
	0x0F12, 0x012B, //#senHal_pContSenModesRegsArray[28][2] 2 700013B6
	0x0F12, 0x016A, //#senHal_pContSenModesRegsArray[28][3] 2 700013B8
	0x0F12, 0x039F, //#senHal_pContSenModesRegsArray[29][0] 2 700013BA
	0x0F12, 0x045E, //#senHal_pContSenModesRegsArray[29][1] 2 700013BC
	0x0F12, 0x01DD, //#senHal_pContSenModesRegsArray[29][2] 2 700013BE
	0x0F12, 0x029C, //#senHal_pContSenModesRegsArray[29][3] 2 700013C0
	0x0F12, 0x041A, //#senHal_pContSenModesRegsArray[30][0] 2 700013C2
	0x0F12, 0x04A6, //#senHal_pContSenModesRegsArray[30][1] 2 700013C4
	0x0F12, 0x0258, //#senHal_pContSenModesRegsArray[30][2] 2 700013C6
	0x0F12, 0x02E4, //#senHal_pContSenModesRegsArray[30][3] 2 700013C8
	0x0F12, 0x062D, //#senHal_pContSenModesRegsArray[31][0] 2 700013CA
	0x0F12, 0x0639, //#senHal_pContSenModesRegsArray[31][1] 2 700013CC
	0x0F12, 0x038A, //#senHal_pContSenModesRegsArray[31][2] 2 700013CE
	0x0F12, 0x0396, //#senHal_pContSenModesRegsArray[31][3] 2 700013D0
	0x0F12, 0x0669, //#senHal_pContSenModesRegsArray[32][0] 2 700013D2
	0x0F12, 0x06B4, //#senHal_pContSenModesRegsArray[32][1] 2 700013D4
	0x0F12, 0x03C6, //#senHal_pContSenModesRegsArray[32][2] 2 700013D6
	0x0F12, 0x0411, //#senHal_pContSenModesRegsArray[32][3] 2 700013D8
	0x0F12, 0x087C, //#senHal_pContSenModesRegsArray[33][0] 2 700013DA
	0x0F12, 0x08C7, //#senHal_pContSenModesRegsArray[33][1] 2 700013DC
	0x0F12, 0x04F8, //#senHal_pContSenModesRegsArray[33][2] 2 700013DE
	0x0F12, 0x0543, //#senHal_pContSenModesRegsArray[33][3] 2 700013E0
	0x0F12, 0x0040, //#senHal_pContSenModesRegsArray[34][0] 2 700013E2
	0x0F12, 0x0040, //#senHal_pContSenModesRegsArray[34][1] 2 700013E4
	0x0F12, 0x0040, //#senHal_pContSenModesRegsArray[34][2] 2 700013E6
	0x0F12, 0x0040, //#senHal_pContSenModesRegsArray[34][3] 2 700013E8
	0x0F12, 0x01D0, //#senHal_pContSenModesRegsArray[35][0] 2 700013EA
	0x0F12, 0x01D0, //#senHal_pContSenModesRegsArray[35][1] 2 700013EC
	0x0F12, 0x00EF, //#senHal_pContSenModesRegsArray[35][2] 2 700013EE
	0x0F12, 0x00EF, //#senHal_pContSenModesRegsArray[35][3] 2 700013F0
	0x0F12, 0x020F, //#senHal_pContSenModesRegsArray[36][0] 2 700013F2
	0x0F12, 0x024E, //#senHal_pContSenModesRegsArray[36][1] 2 700013F4
	0x0F12, 0x012E, //#senHal_pContSenModesRegsArray[36][2] 2 700013F6
	0x0F12, 0x016D, //#senHal_pContSenModesRegsArray[36][3] 2 700013F8
	0x0F12, 0x039F, //#senHal_pContSenModesRegsArray[37][0] 2 700013FA
	0x0F12, 0x045E, //#senHal_pContSenModesRegsArray[37][1] 2 700013FC
	0x0F12, 0x01DD, //#senHal_pContSenModesRegsArray[37][2] 2 700013FE
	0x0F12, 0x029C, //#senHal_pContSenModesRegsArray[37][3] 2 70001400
	0x0F12, 0x041D, //#senHal_pContSenModesRegsArray[38][0] 2 70001402
	0x0F12, 0x04A9, //#senHal_pContSenModesRegsArray[38][1] 2 70001404
	0x0F12, 0x025B, //#senHal_pContSenModesRegsArray[38][2] 2 70001406
	0x0F12, 0x02E7, //#senHal_pContSenModesRegsArray[38][3] 2 70001408
	0x0F12, 0x062D, //#senHal_pContSenModesRegsArray[39][0] 2 7000140A
	0x0F12, 0x0639, //#senHal_pContSenModesRegsArray[39][1] 2 7000140C
	0x0F12, 0x038A, //#senHal_pContSenModesRegsArray[39][2] 2 7000140E
	0x0F12, 0x0396, //#senHal_pContSenModesRegsArray[39][3] 2 70001410
	0x0F12, 0x066C, //#senHal_pContSenModesRegsArray[40][0] 2 70001412
	0x0F12, 0x06B7, //#senHal_pContSenModesRegsArray[40][1] 2 70001414
	0x0F12, 0x03C9, //#senHal_pContSenModesRegsArray[40][2] 2 70001416
	0x0F12, 0x0414, //#senHal_pContSenModesRegsArray[40][3] 2 70001418
	0x0F12, 0x087C, //#senHal_pContSenModesRegsArray[41][0] 2 7000141A
	0x0F12, 0x08C7, //#senHal_pContSenModesRegsArray[41][1] 2 7000141C
	0x0F12, 0x04F8, //#senHal_pContSenModesRegsArray[41][2] 2 7000141E
	0x0F12, 0x0543, //#senHal_pContSenModesRegsArray[41][3] 2 70001420
	0x0F12, 0x0040, //#senHal_pContSenModesRegsArray[42][0] 2 70001422
	0x0F12, 0x0040, //#senHal_pContSenModesRegsArray[42][1] 2 70001424
	0x0F12, 0x0040, //#senHal_pContSenModesRegsArray[42][2] 2 70001426
	0x0F12, 0x0040, //#senHal_pContSenModesRegsArray[42][3] 2 70001428
	0x0F12, 0x01D0, //#senHal_pContSenModesRegsArray[43][0] 2 7000142A
	0x0F12, 0x01D0, //#senHal_pContSenModesRegsArray[43][1] 2 7000142C
	0x0F12, 0x00EF, //#senHal_pContSenModesRegsArray[43][2] 2 7000142E
	0x0F12, 0x00EF, //#senHal_pContSenModesRegsArray[43][3] 2 70001430
	0x0F12, 0x020F, //#senHal_pContSenModesRegsArray[44][0] 2 70001432
	0x0F12, 0x024E, //#senHal_pContSenModesRegsArray[44][1] 2 70001434
	0x0F12, 0x012E, //#senHal_pContSenModesRegsArray[44][2] 2 70001436
	0x0F12, 0x016D, //#senHal_pContSenModesRegsArray[44][3] 2 70001438
	0x0F12, 0x039F, //#senHal_pContSenModesRegsArray[45][0] 2 7000143A
	0x0F12, 0x045E, //#senHal_pContSenModesRegsArray[45][1] 2 7000143C
	0x0F12, 0x01DD, //#senHal_pContSenModesRegsArray[45][2] 2 7000143E
	0x0F12, 0x029C, //#senHal_pContSenModesRegsArray[45][3] 2 70001440
	0x0F12, 0x041D, //#senHal_pContSenModesRegsArray[46][0] 2 70001442
	0x0F12, 0x04A9, //#senHal_pContSenModesRegsArray[46][1] 2 70001444
	0x0F12, 0x025B, //#senHal_pContSenModesRegsArray[46][2] 2 70001446
	0x0F12, 0x02E7, //#senHal_pContSenModesRegsArray[46][3] 2 70001448
	0x0F12, 0x062D, //#senHal_pContSenModesRegsArray[47][0] 2 7000144A
	0x0F12, 0x0639, //#senHal_pContSenModesRegsArray[47][1] 2 7000144C
	0x0F12, 0x038A, //#senHal_pContSenModesRegsArray[47][2] 2 7000144E
	0x0F12, 0x0396, //#senHal_pContSenModesRegsArray[47][3] 2 70001450
	0x0F12, 0x066C, //#senHal_pContSenModesRegsArray[48][0] 2 70001452
	0x0F12, 0x06B7, //#senHal_pContSenModesRegsArray[48][1] 2 70001454
	0x0F12, 0x03C9, //#senHal_pContSenModesRegsArray[48][2] 2 70001456
	0x0F12, 0x0414, //#senHal_pContSenModesRegsArray[48][3] 2 70001458
	0x0F12, 0x087C, //#senHal_pContSenModesRegsArray[49][0] 2 7000145A
	0x0F12, 0x08C7, //#senHal_pContSenModesRegsArray[49][1] 2 7000145C
	0x0F12, 0x04F8, //#senHal_pContSenModesRegsArray[49][2] 2 7000145E
	0x0F12, 0x0543, //#senHal_pContSenModesRegsArray[49][3] 2 70001460
	0x0F12, 0x003D, //#senHal_pContSenModesRegsArray[50][0] 2 70001462
	0x0F12, 0x003D, //#senHal_pContSenModesRegsArray[50][1] 2 70001464
	0x0F12, 0x003D, //#senHal_pContSenModesRegsArray[50][2] 2 70001466
	0x0F12, 0x003D, //#senHal_pContSenModesRegsArray[50][3] 2 70001468
	0x0F12, 0x01D2, //#senHal_pContSenModesRegsArray[51][0] 2 7000146A
	0x0F12, 0x01D2, //#senHal_pContSenModesRegsArray[51][1] 2 7000146C
	0x0F12, 0x00F1, //#senHal_pContSenModesRegsArray[51][2] 2 7000146E
	0x0F12, 0x00F1, //#senHal_pContSenModesRegsArray[51][3] 2 70001470
	0x0F12, 0x020C, //#senHal_pContSenModesRegsArray[52][0] 2 70001472
	0x0F12, 0x024B, //#senHal_pContSenModesRegsArray[52][1] 2 70001474
	0x0F12, 0x012B, //#senHal_pContSenModesRegsArray[52][2] 2 70001476
	0x0F12, 0x016A, //#senHal_pContSenModesRegsArray[52][3] 2 70001478
	0x0F12, 0x03A1, //#senHal_pContSenModesRegsArray[53][0] 2 7000147A
	0x0F12, 0x0460, //#senHal_pContSenModesRegsArray[53][1] 2 7000147C
	0x0F12, 0x01DF, //#senHal_pContSenModesRegsArray[53][2] 2 7000147E
	0x0F12, 0x029E, //#senHal_pContSenModesRegsArray[53][3] 2 70001480
	0x0F12, 0x041A, //#senHal_pContSenModesRegsArray[54][0] 2 70001482
	0x0F12, 0x04A6, //#senHal_pContSenModesRegsArray[54][1] 2 70001484
	0x0F12, 0x0258, //#senHal_pContSenModesRegsArray[54][2] 2 70001486
	0x0F12, 0x02E4, //#senHal_pContSenModesRegsArray[54][3] 2 70001488
	0x0F12, 0x062F, //#senHal_pContSenModesRegsArray[55][0] 2 7000148A
	0x0F12, 0x063B, //#senHal_pContSenModesRegsArray[55][1] 2 7000148C
	0x0F12, 0x038C, //#senHal_pContSenModesRegsArray[55][2] 2 7000148E
	0x0F12, 0x0398, //#senHal_pContSenModesRegsArray[55][3] 2 70001490
	0x0F12, 0x0669, //#senHal_pContSenModesRegsArray[56][0] 2 70001492
	0x0F12, 0x06B4, //#senHal_pContSenModesRegsArray[56][1] 2 70001494
	0x0F12, 0x03C6, //#senHal_pContSenModesRegsArray[56][2] 2 70001496
	0x0F12, 0x0411, //#senHal_pContSenModesRegsArray[56][3] 2 70001498
	0x0F12, 0x087E, //#senHal_pContSenModesRegsArray[57][0] 2 7000149A
	0x0F12, 0x08C9, //#senHal_pContSenModesRegsArray[57][1] 2 7000149C
	0x0F12, 0x04FA, //#senHal_pContSenModesRegsArray[57][2] 2 7000149E
	0x0F12, 0x0545, //#senHal_pContSenModesRegsArray[57][3] 2 700014A0
	0x0F12, 0x03A2, //#senHal_pContSenModesRegsArray[58][0] 2 700014A2
	0x0F12, 0x01D3, //#senHal_pContSenModesRegsArray[58][1] 2 700014A4
	0x0F12, 0x01E0, //#senHal_pContSenModesRegsArray[58][2] 2 700014A6
	0x0F12, 0x00F2, //#senHal_pContSenModesRegsArray[58][3] 2 700014A8
	0x0F12, 0x03AF, //#senHal_pContSenModesRegsArray[59][0] 2 700014AA
	0x0F12, 0x01E0, //#senHal_pContSenModesRegsArray[59][1] 2 700014AC
	0x0F12, 0x01ED, //#senHal_pContSenModesRegsArray[59][2] 2 700014AE
	0x0F12, 0x00FF, //#senHal_pContSenModesRegsArray[59][3] 2 700014B0
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[60][0] 2 700014B2
	0x0F12, 0x0461, //#senHal_pContSenModesRegsArray[60][1] 2 700014B4
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[60][2] 2 700014B6
	0x0F12, 0x029F, //#senHal_pContSenModesRegsArray[60][3] 2 700014B8
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[61][0] 2 700014BA
	0x0F12, 0x046E, //#senHal_pContSenModesRegsArray[61][1] 2 700014BC
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[61][2] 2 700014BE
	0x0F12, 0x02AC, //#senHal_pContSenModesRegsArray[61][3] 2 700014C0
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[62][0] 2 700014C2
	0x0F12, 0x063C, //#senHal_pContSenModesRegsArray[62][1] 2 700014C4
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[62][2] 2 700014C6
	0x0F12, 0x0399, //#senHal_pContSenModesRegsArray[62][3] 2 700014C8
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[63][0] 2 700014CA
	0x0F12, 0x0649, //#senHal_pContSenModesRegsArray[63][1] 2 700014CC
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[63][2] 2 700014CE
	0x0F12, 0x03A6, //#senHal_pContSenModesRegsArray[63][3] 2 700014D0
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[64][0] 2 700014D2
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[64][1] 2 700014D4
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[64][2] 2 700014D6
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[64][3] 2 700014D8
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[65][0] 2 700014DA
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[65][1] 2 700014DC
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[65][2] 2 700014DE
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[65][3] 2 700014E0
	0x0F12, 0x03AA, //#senHal_pContSenModesRegsArray[66][0] 2 700014E2
	0x0F12, 0x01DB, //#senHal_pContSenModesRegsArray[66][1] 2 700014E4
	0x0F12, 0x01E8, //#senHal_pContSenModesRegsArray[66][2] 2 700014E6
	0x0F12, 0x00FA, //#senHal_pContSenModesRegsArray[66][3] 2 700014E8
	0x0F12, 0x03B7, //#senHal_pContSenModesRegsArray[67][0] 2 700014EA
	0x0F12, 0x01E8, //#senHal_pContSenModesRegsArray[67][1] 2 700014EC
	0x0F12, 0x01F5, //#senHal_pContSenModesRegsArray[67][2] 2 700014EE
	0x0F12, 0x0107, //#senHal_pContSenModesRegsArray[67][3] 2 700014F0
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[68][0] 2 700014F2
	0x0F12, 0x0469, //#senHal_pContSenModesRegsArray[68][1] 2 700014F4
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[68][2] 2 700014F6
	0x0F12, 0x02A7, //#senHal_pContSenModesRegsArray[68][3] 2 700014F8
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[69][0] 2 700014FA
	0x0F12, 0x0476, //#senHal_pContSenModesRegsArray[69][1] 2 700014FC
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[69][2] 2 700014FE
	0x0F12, 0x02B4, //#senHal_pContSenModesRegsArray[69][3] 2 70001500
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[70][0] 2 70001502
	0x0F12, 0x0644, //#senHal_pContSenModesRegsArray[70][1] 2 70001504
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[70][2] 2 70001506
	0x0F12, 0x03A1, //#senHal_pContSenModesRegsArray[70][3] 2 70001508
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[71][0] 2 7000150A
	0x0F12, 0x0651, //#senHal_pContSenModesRegsArray[71][1] 2 7000150C
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[71][2] 2 7000150E
	0x0F12, 0x03AE, //#senHal_pContSenModesRegsArray[71][3] 2 70001510
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[72][0] 2 70001512
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[72][1] 2 70001514
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[72][2] 2 70001516
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[72][3] 2 70001518
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[73][0] 2 7000151A
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[73][1] 2 7000151C
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[73][2] 2 7000151E
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[73][3] 2 70001520
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[74][0] 2 70001522
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[74][1] 2 70001524
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[74][2] 2 70001526
	0x0F12, 0x0001, //#senHal_pContSenModesRegsArray[74][3] 2 70001528
	0x0F12, 0x000F, //#senHal_pContSenModesRegsArray[75][0] 2 7000152A
	0x0F12, 0x000F, //#senHal_pContSenModesRegsArray[75][1] 2 7000152C
	0x0F12, 0x000F, //#senHal_pContSenModesRegsArray[75][2] 2 7000152E
	0x0F12, 0x000F, //#senHal_pContSenModesRegsArray[75][3] 2 70001530
	0x0F12, 0x05AD, //#senHal_pContSenModesRegsArray[76][0] 2 70001532
	0x0F12, 0x03DE, //#senHal_pContSenModesRegsArray[76][1] 2 70001534
	0x0F12, 0x030A, //#senHal_pContSenModesRegsArray[76][2] 2 70001536
	0x0F12, 0x021C, //#senHal_pContSenModesRegsArray[76][3] 2 70001538
	0x0F12, 0x062F, //#senHal_pContSenModesRegsArray[77][0] 2 7000153A
	0x0F12, 0x0460, //#senHal_pContSenModesRegsArray[77][1] 2 7000153C
	0x0F12, 0x038C, //#senHal_pContSenModesRegsArray[77][2] 2 7000153E
	0x0F12, 0x029E, //#senHal_pContSenModesRegsArray[77][3] 2 70001540
	0x0F12, 0x07FC, //#senHal_pContSenModesRegsArray[78][0] 2 70001542
	0x0F12, 0x0847, //#senHal_pContSenModesRegsArray[78][1] 2 70001544
	0x0F12, 0x0478, //#senHal_pContSenModesRegsArray[78][2] 2 70001546
	0x0F12, 0x04C3, //#senHal_pContSenModesRegsArray[78][3] 2 70001548
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[79][0] 2 7000154A
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[79][1] 2 7000154C
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[79][2] 2 7000154E
	0x0F12, 0x0000, //#senHal_pContSenModesRegsArray[79][3] 2 70001550
	//============================================================
	// Analog Setting END
	//============================================================


};

static unsigned short s5k5cagx_init_reg_short3[]={	
	//============================================================
	// AF Interface setting
	//============================================================ 
	0x0F12, 0x7000,
	0x002A, 0x01D4,
	0x0F12, 0x0000, //REG_TC_IPRM_AuxGpios : 0 - no Flash
	0x002A, 0x01DE,
	0x0F12, 0x0003, //REG_TC_IPRM_CM_Init_AfModeType : 3 - AFD_VCM_I2C
	0x0F12, 0x0000, //REG_TC_IPRM_CM_Init_PwmConfig1 : 0 - no PWM
	0x002A, 0x01E4,
	0x0F12, 0x0000, //REG_TC_IPRM_CM_Init_GpioConfig1 : 0 - no GPIO
	0x002A, 0x01E8,
	0x0F12, 0x200C, //REG_TC_IPRM_CM_Init_Mi2cBits : MSCL - GPIO1 MSDA - GPIO2 Device ID (0C)
	0x0F12, 0x0190, //REG_TC_IPRM_CM_Init_Mi2cRateKhz : MI2C Speed - 400KHz
	
	//#============================================================
	//#AF Parameter setting
	//#============================================================
	//#AF Window Settings
	0x002A, 0x025A,
	
	0x0F12, 0x0100, //#REG_TC_AF_FstWinStartX //#2nd : 1200-900(OUT)_640-480(IN) window setting
	0x0F12, 0x00E3, //#REG_TC_AF_FstWinStartY
	
	0x0F12, 0x0200, //#REG_TC_AF_FstWinSizeX 512
	0x0F12, 0x0238, //#REG_TC_AF_FstWinSizeY
	
	0x0F12, 0x018C, //#REG_TC_AF_ScndWinStartX
	0x0F12, 0x0166, //#REG_TC_AF_ScndWinStartY
	
	0x0F12, 0x00E6, //#REG_TC_AF_ScndWinSizeX
	0x0F12, 0x0132, //#REG_TC_AF_ScndWinSizeY
	
	
	0x0F12, 0x0001, //#REG_TC_AF_WinSizesUpdated
	
	//#AF Setot Settings
	0x002A, 0x0586,
	0x0F12, 0x00FF, //#skl_af_StatOvlpExpFactor
	//#AF Scene Settings
	0x002A, 0x115E,
	0x0F12, 0x0003, //#af_scene_usSaturatedScene
	//#AF Fine Search Settings
	0x002A, 0x10D4,
	0x0F12, 0x1000, //#FineSearch Disable //#af_search_usSingleAfFlags
	0x002A, 0x10DE,
	0x0F12, 0x0004, //#af_search_usFinePeakCount
	0x002A, 0x106C,
	0x0F12, 0x0202, //#af_pos_usFineStepNumSize
	//#AF Peak Threshold Setting
	0x002A, 0x10CA, //#af_search_usPeakThr
	0x0F12, 0x00C0,
	//#AF Default Position 
	0x002A, 0x1060,
	0x0F12, 0x0000, //#af_pos_usHomePos
	0x0F12, 0x783C, //#af_pos_usLowConfPos
	//#AF LowConfThr Setting
	0x002A, 0x10F4, //#LowEdgeBoth GRAD
	0x0F12, 0x0280,
	0x002A, 0x1100, //#LowLight HPF
	0x0F12, 0x03A0,
	0x0F12, 0x0320,
	0x002A, 0x1134,
	0x0F12, 0x0030, //#af_stat_usMinStatVal
	//#AF low Br Th
	0x002A, 0x1154, //#normBrThr
	0x0F12, 0x0060,
	//#AF Policy
	0x002A, 0x10E2,
	0x0F12, 0x0000, //#af_search_usCapturePolicy: Focus_Priority, 0002 : Shutter_Priority_Fixed, 0001 : Shutter_Priority_Last_BFP 0000: Shutter_Priority_Current
	0x002A, 0x1072,
	0x0F12, 0x003C, //#af_pos_usCaptureFixedPos//#0x0008
	//#AF Lens Position Table Settings
	0x002A, 0x1074,
	0x0F12, 0x0010, //0015 //#af_pos_usTableLastInd//#22 Steps
	0x0F12, 0x0032, //002E //#af_pos_usTable_0_//#af_pos_usTable
	0x0F12, 0x0036, //0030 //#af_pos_usTable_1_
	0x0F12, 0x003A, //0033 //#af_pos_usTable_2_
	0x0F12, 0x003E, //0036 //#af_pos_usTable_3_
	0x0F12, 0x0042, //0039 //#af_pos_usTable_4_
	0x0F12, 0x0046, //003C //#af_pos_usTable_5_
	0x0F12, 0x004A, //003F //#af_pos_usTable_6_
	0x0F12, 0x004E, //0042 //#af_pos_usTable_7_
	0x0F12, 0x0053, //0045 //#af_pos_usTable_8_
	0x0F12, 0x0058, //0048 //#af_pos_usTable_9_
	0x0F12, 0x005D, //004B //#af_pos_usTable_10_
	0x0F12, 0x0062, //004E //#af_pos_usTable_11_
	0x0F12, 0x0066, //0051 //#af_pos_usTable_12_
	0x0F12, 0x006A, //0054 //#af_pos_usTable_13_
	0x0F12, 0x006D, //0057 //#af_pos_usTable_14_
	0x0F12, 0x0070, //005A //#af_pos_usTable_15_
	0x0F12, 0x0073, //005E //#af_pos_usTable_16_
	0x002A, 0x0252,
	0x0F12, 0x0003, //#init  //AF
	
	//============================================================
	// ISP-FE Setting
	//============================================================
	0x002A, 0x158A,
	0x0F12, 0xEAF0,
	0x002A, 0x15C6,
	0x0F12, 0x0020,
	0x0F12, 0x0060,
	0x002A, 0x15BC,
	0x0F12, 0x0200,
	
	//Analog Offset for MSM
	0x002A, 0x1608,
	0x0F12, 0x0100, //#gisp_msm_sAnalogOffset[0] 
	0x0F12, 0x0100, //#gisp_msm_sAnalogOffset[1]
	0x0F12, 0x0100, //#gisp_msm_sAnalogOffset[2]
	0x0F12, 0x0100, //#gisp_msm_sAnalogOffset[3]
	
	//============================================================
	// ISP-FE Setting END
	//============================================================
	
	//============================================================
	// Frame rate setting 
	//============================================================
	// How to set
	// 1. Exposure value
	// dec2hex((1 / (frame rate you want(ms))) * 100d * 4d)
	// 2. Analog Digital gain
	// dec2hex((Analog gain you want) * 256d)
	//============================================================
	// Set preview exposure time
	0x002A, 0x0530, 
	0x0F12, 0x3E80, //#lt_uMaxExp1 40ms 
	0x0F12, 0x0000, 
	0x0F12, 0x6D60, //#lt_uMaxExp2 70ms
	0x0F12, 0x0000, 
	0x002A, 0x167C, 
	0x0F12, 0x8CA0, //#evt1_lt_uMaxExp3 90ms
	0x0F12, 0x0000,
	0x0F12, 0x9C40, //#evt1_lt_uMaxExp4 100ms
	0x0F12, 0x0000,
	 
	// Set capture exposure time
	0x002A, 0x0538,
	0x0F12, 0x3E80, //#lt_uCapMaxExp1 40ms
	0x0F12, 0x0000,
	0x0F12, 0x6D60, //#lt_uCapMaxExp2 70ms 
	0x0F12, 0x0000,
	0x002A, 0x1684,
	0x0F12, 0x8CA0, //#evt1_lt_uCapMaxExp3 90ms
	0x0F12, 0x0000,
	0x0F12, 0x9C40, //#evt1_lt_uCapMaxExp4 100ms
	0x0F12, 0x0000,
	
	// Set gain
	0x002A, 0x0540,
	0x0F12, 0x0150, //#lt_uMaxAnGain1
	0x0F12, 0x0280, //#lt_uMaxAnGain2
	0x002A, 0x168C,
	0x0F12, 0x02A0, //#evt1_lt_uMaxAnGain3
	0x0F12, 0x0800, //#evt1_lt_uMaxAnGain4 
	0x002A, 0x0544,
	0x0F12, 0x0100, //#lt_uMaxDigGain
	0x0F12, 0x0A00, //#lt_uMaxTotGain
	0x002A, 0x1694,
	0x0F12, 0x0001, //#evt1_senHal_bExpandForbid
	0x002A, 0x051A,
	0x0F12, 0x0111, //#lt_uLimitHigh 
	0x0F12, 0x00F0, //#lt_uLimitLow
	
	
	//================================================================================================
	// SET AE
	//================================================================================================
	// AE target 
	0x002A, 0x0F70,
	0x0F12, 0x003B, //#TVAR_ae_BrAve 091222
	// AE mode
	0x002A, 0x0F76,
	0x0F12, 0x000F, //Disable illumination & contrast// #ae_StatMode
	// AE weight
	0x002A, 0x0F7E,
	0x0F12, 0x0101, //#ae_WeightTbl_16_0_
	0x0F12, 0x0101, //#ae_WeightTbl_16_1_
	0x0F12, 0x0101, //#ae_WeightTbl_16_2_
	0x0F12, 0x0101, //#ae_WeightTbl_16_3_
	0x0F12, 0x0101, //#ae_WeightTbl_16_4_
	0x0F12, 0x0101, //#ae_WeightTbl_16_5_
	0x0F12, 0x0101, //#ae_WeightTbl_16_6_
	0x0F12, 0x0101, //#ae_WeightTbl_16_7_
	0x0F12, 0x0101, //#ae_WeightTbl_16_8_
	0x0F12, 0x0303, //#ae_WeightTbl_16_9_
	0x0F12, 0x0303, //#ae_WeightTbl_16_10
	0x0F12, 0x0101, //#ae_WeightTbl_16_11
	0x0F12, 0x0101, //#ae_WeightTbl_16_12
	0x0F12, 0x0303, //#ae_WeightTbl_16_13
	0x0F12, 0x0303, //#ae_WeightTbl_16_14
	0x0F12, 0x0101, //#ae_WeightTbl_16_15
	0x0F12, 0x0101, //#ae_WeightTbl_16_16
	0x0F12, 0x0303, //#ae_WeightTbl_16_17
	0x0F12, 0x0303, //#ae_WeightTbl_16_18
	0x0F12, 0x0101, //#ae_WeightTbl_16_19
	0x0F12, 0x0101, //#ae_WeightTbl_16_20
	0x0F12, 0x0303, //#ae_WeightTbl_16_21
	0x0F12, 0x0303, //#ae_WeightTbl_16_22
	0x0F12, 0x0101, //#ae_WeightTbl_16_23
	0x0F12, 0x0101, //#ae_WeightTbl_16_24
	0x0F12, 0x0101, //#ae_WeightTbl_16_25
	0x0F12, 0x0101, //#ae_WeightTbl_16_26
	0x0F12, 0x0101, //#ae_WeightTbl_16_27
	0x0F12, 0x0101, //#ae_WeightTbl_16_28
	0x0F12, 0x0101, //#ae_WeightTbl_16_29
	0x0F12, 0x0101, //#ae_WeightTbl_16_30
	0x0F12, 0x0101, //#ae_WeightTbl_16_31
	
	//================================================================================================
	// SET FLICKER
	//================================================================================================
#if 0
	0x002A, 0x04D2,
	0x0F12, 0x065F, //#REG_TC_DBG_AutoAlgEnBits
	0x002A, 0x04BA,
	0x0F12, 0x0002, //#REG_SF_USER_FlickerQuant
	0x0F12, 0x0001, //#REG_SF_USER_FlickerQuantChanged
#else 

//Flicker Auto (Start from 60Hz)
0x0028, 0x7000,
0x002A, 0x0C18,
0x0F12, 0x0001, //#AFC_Default60Hz 1:60Hz 0: 50Hz
0x002A, 0x04D2,
0x0F12, 0x067F, //#REG_TC_DBG_AutoAlgEnBits
#endif
};

static unsigned short s5k5cagx_init_reg_short4[]={		

	//#================================================================================================
	//#SETGAS
	//#================================================================================================
	//#GASalpha
	//#RGrGbBperlightsource
	0x0F12, 0x7000,
	0x002A, 0x06CE,
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[0][0] Hor
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[0][1]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[0][2]
	0x0F12, 0x00E0, //#TVAR_ash_GASAlpha[0][3]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[1][0] Inca
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[1][1]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[1][2]
	0x0F12, 0x00E0, //#TVAR_ash_GASAlpha[1][3]
	0x0F12, 0x0110, //#TVAR_ash_GASAlpha[2][0] CWF, TL84
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[2][1]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[2][2]
	0x0F12, 0x00F0, //#TVAR_ash_GASAlpha[2][3]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[3][0] Indoor
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[3][1]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[3][2]
	0x0F12, 0x00F0, //#TVAR_ash_GASAlpha[3][3]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[4][0] D50,DNP
	0x0F12, 0x0101, //#TVAR_ash_GASAlpha[4][1]
	0x0F12, 0x0101, //#TVAR_ash_GASAlpha[4][2]
	0x0F12, 0x00E0, //#TVAR_ash_GASAlpha[4][3]
	0x0F12, 0x0118, //#TVAR_ash_GASAlpha[5][0] D65
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[5][1]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[5][2]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[5][3]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[6][0] D75
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[6][1]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[6][2]
	0x0F12, 0x0100, //#TVAR_ash_GASAlpha[6][3]
	0x0F12, 0x0100, //#TVAR_ash_GASOutdoorAlpha[0]//#Outdoor //##Only, applied when 0x06B4=[1] 
	0x0F12, 0x0100, //#TVAR_ash_GASOutdoorAlpha[1]
	0x0F12, 0x0100, //#TVAR_ash_GASOutdoorAlpha[2]
	0x0F12, 0x0100, //#TVAR_ash_GASOutdoorAlpha[3]
	//#GASbeta
	0x0F12, 0x0000, //#ash_GASBeta[0]//#Horizon
	0x0F12, 0x0000, //#ash_GASBeta[1]
	0x0F12, 0x0000, //#ash_GASBeta[2]
	0x0F12, 0x0000, //#ash_GASBeta[3]
	0x0F12, 0x0000, //#ash_GASBeta[4]//#IncandA
	0x0F12, 0x0000, //#ash_GASBeta[5]
	0x0F12, 0x0000, //#ash_GASBeta[6]
	0x0F12, 0x0000, //#ash_GASBeta[7]
	0x0F12, 0x0000, //#ash_GASBeta[8]//#CWF, TL84 
	0x0F12, 0x0000, //#ash_GASBeta[9]
	0x0F12, 0x0000, //#ash_GASBeta[10]
	0x0F12, 0x0000, //#ash_GASBeta[11]
	0x0F12, 0x0000, //#ash_GASBeta[12]//#Indoor
	0x0F12, 0x0000, //#ash_GASBeta[13]
	0x0F12, 0x0000, //#ash_GASBeta[14]
	0x0F12, 0x0000, //#ash_GASBeta[15]
	0x0F12, 0x0000, //#ash_GASBeta[16]//#D50,DNP
	0x0F12, 0x0000, //#ash_GASBeta[17]
	0x0F12, 0x0000, //#ash_GASBeta[18]
	0x0F12, 0x0000, //#ash_GASBeta[19]
	0x0F12, 0x0000, //#ash_GASBeta[20]//#D65
	0x0F12, 0x0000, //#ash_GASBeta[21]
	0x0F12, 0x0000, //#ash_GASBeta[22]
	0x0F12, 0x0000, //#ash_GASBeta[23]
	0x0F12, 0x0000, //#ash_GASBeta[24]//#D75
	0x0F12, 0x0000, //#ash_GASBeta[25]
	0x0F12, 0x0000, //#ash_GASBeta[26]
	0x0F12, 0x0000, //#ash_GASBeta[27]
	0x0F12, 0x0000, //#ash_GASOutdoorBeta[0]//#Outdoor
	0x0F12, 0x0000, //#ash_GASOutdoorBeta[1]
	0x0F12, 0x0000, //#ash_GASOutdoorBeta[2]
	0x0F12, 0x0000, //#ash_GASOutdoorBeta[3]
	0x002A, 0x06B4,
	0x0F12, 0x0001, //#wbt_bUseOutdoorASHON:1OFF:0 //##Outdoor GAS_Alpha Enable[1] / Disable[0]
	//#Parabloicfunction
	0x002A, 0x075A,
	0x0F12, 0x0001, //#ash_bParabolicEstimation
	0x0F12, 0x0400, //#ash_uParabolicCenterX
	0x0F12, 0x0300, //#ash_uParabolicCenterY
	0x0F12, 0x0010, //#ash_uParabolicScalingA
	0x0F12, 0x0011, //#ash_uParabolicScalingB
	0x002A, 0x06C6,
	0x0F12, 0x0100, //#ash_CGrasAlphas_0_
	0x0F12, 0x0100, //#ash_CGrasAlphas_1_
	0x0F12, 0x0100, //#ash_CGrasAlphas_2_
	0x0F12, 0x0100, //#ash_CGrasAlphas_3_
	0x002A, 0x0E3C,
	0x0F12, 0x00C0, //#awbb_Alpha_Comp_Mode
	0x002A, 0x074E,
	0x0F12, 0x0000, //#ash_bLumaMode//#useBeta:0001notuseBeta:0000
	//#GASLUTstartaddress//#7000_347C
	0x002A, 0x0754,
	0x0F12, 0x347C,
	0x0F12, 0x7000,
};
	static unsigned short s5k5cagx_init_reg_short4_2[]={		

	//#GASLUT
	0x0F12, 0x7000,
	0x002A, 0x347C,
	0x0F12, 0x01C6, //#TVAR_ash_pGAS[0]
	0x0F12, 0x0190, //#TVAR_ash_pGAS[1]
	0x0F12, 0x0151, //#TVAR_ash_pGAS[2]
	0x0F12, 0x0119, //#TVAR_ash_pGAS[3]
	0x0F12, 0x00EC, //#TVAR_ash_pGAS[4]
	0x0F12, 0x00D4, //#TVAR_ash_pGAS[5]
	0x0F12, 0x00CA, //#TVAR_ash_pGAS[6]
	0x0F12, 0x00D0, //#TVAR_ash_pGAS[7]
	0x0F12, 0x00E5, //#TVAR_ash_pGAS[8]
	0x0F12, 0x010B, //#TVAR_ash_pGAS[9]
	0x0F12, 0x013F, //#TVAR_ash_pGAS[10]
	0x0F12, 0x0179, //#TVAR_ash_pGAS[11]
	0x0F12, 0x01AC, //#TVAR_ash_pGAS[12]
	0x0F12, 0x01AB, //#TVAR_ash_pGAS[13]
	0x0F12, 0x016E, //#TVAR_ash_pGAS[14]
	0x0F12, 0x0123, //#TVAR_ash_pGAS[15]
	0x0F12, 0x00E3, //#TVAR_ash_pGAS[16]
	0x0F12, 0x00B5, //#TVAR_ash_pGAS[17]
	0x0F12, 0x0099, //#TVAR_ash_pGAS[18]
	0x0F12, 0x008F, //#TVAR_ash_pGAS[19]
	0x0F12, 0x0096, //#TVAR_ash_pGAS[20]
	0x0F12, 0x00AD, //#TVAR_ash_pGAS[21]
	0x0F12, 0x00D5, //#TVAR_ash_pGAS[22]
	0x0F12, 0x0110, //#TVAR_ash_pGAS[23]
	0x0F12, 0x0154, //#TVAR_ash_pGAS[24]
	0x0F12, 0x018C, //#TVAR_ash_pGAS[25]
	0x0F12, 0x017A, //#TVAR_ash_pGAS[26]
	0x0F12, 0x0135, //#TVAR_ash_pGAS[27]
	0x0F12, 0x00E3, //#TVAR_ash_pGAS[28]
	0x0F12, 0x00A1, //#TVAR_ash_pGAS[29]
	0x0F12, 0x0073, //#TVAR_ash_pGAS[30]
	0x0F12, 0x0058, //#TVAR_ash_pGAS[31]
	0x0F12, 0x004E, //#TVAR_ash_pGAS[32]
	0x0F12, 0x0055, //#TVAR_ash_pGAS[33]
	0x0F12, 0x006C, //#TVAR_ash_pGAS[34]
	0x0F12, 0x0096, //#TVAR_ash_pGAS[35]
	0x0F12, 0x00D0, //#TVAR_ash_pGAS[36]
	0x0F12, 0x011A, //#TVAR_ash_pGAS[37]
	0x0F12, 0x0160, //#TVAR_ash_pGAS[38]
	0x0F12, 0x0154, //#TVAR_ash_pGAS[39]
	0x0F12, 0x0108, //#TVAR_ash_pGAS[40]
	0x0F12, 0x00B6, //#TVAR_ash_pGAS[41]
	0x0F12, 0x0074, //#TVAR_ash_pGAS[42]
	0x0F12, 0x0047, //#TVAR_ash_pGAS[43]
	0x0F12, 0x002D, //#TVAR_ash_pGAS[44]
	0x0F12, 0x0023, //#TVAR_ash_pGAS[45]
	0x0F12, 0x002A, //#TVAR_ash_pGAS[46]
	0x0F12, 0x0042, //#TVAR_ash_pGAS[47]
	0x0F12, 0x0069, //#TVAR_ash_pGAS[48]
	0x0F12, 0x00A3, //#TVAR_ash_pGAS[49]
	0x0F12, 0x00F1, //#TVAR_ash_pGAS[50]
	0x0F12, 0x013C, //#TVAR_ash_pGAS[51]
	0x0F12, 0x0138, //#TVAR_ash_pGAS[52]
	0x0F12, 0x00EA, //#TVAR_ash_pGAS[53]
	0x0F12, 0x0097, //#TVAR_ash_pGAS[54]
	0x0F12, 0x0057, //#TVAR_ash_pGAS[55]
	0x0F12, 0x002C, //#TVAR_ash_pGAS[56]
	0x0F12, 0x0012, //#TVAR_ash_pGAS[57]
	0x0F12, 0x0009, //#TVAR_ash_pGAS[58]
	0x0F12, 0x0010, //#TVAR_ash_pGAS[59]
	0x0F12, 0x0027, //#TVAR_ash_pGAS[60]
	0x0F12, 0x004F, //#TVAR_ash_pGAS[61]
	0x0F12, 0x0088, //#TVAR_ash_pGAS[62]
	0x0F12, 0x00D7, //#TVAR_ash_pGAS[63]
	0x0F12, 0x0125, //#TVAR_ash_pGAS[64]
	0x0F12, 0x012C, //#TVAR_ash_pGAS[65]
	0x0F12, 0x00DE, //#TVAR_ash_pGAS[66]
	0x0F12, 0x008A, //#TVAR_ash_pGAS[67]
	0x0F12, 0x004C, //#TVAR_ash_pGAS[68]
	0x0F12, 0x0021, //#TVAR_ash_pGAS[69]
	0x0F12, 0x0008, //#TVAR_ash_pGAS[70]
	0x0F12, 0x0000, //#TVAR_ash_pGAS[71]
	0x0F12, 0x0007, //#TVAR_ash_pGAS[72]
	0x0F12, 0x001E, //#TVAR_ash_pGAS[73]
	0x0F12, 0x0045, //#TVAR_ash_pGAS[74]
	0x0F12, 0x007F, //#TVAR_ash_pGAS[75]
	0x0F12, 0x00CE, //#TVAR_ash_pGAS[76]
	0x0F12, 0x011C, //#TVAR_ash_pGAS[77]
	0x0F12, 0x012B, //#TVAR_ash_pGAS[78]
	0x0F12, 0x00DF, //#TVAR_ash_pGAS[79]
	0x0F12, 0x008E, //#TVAR_ash_pGAS[80]
	0x0F12, 0x0051, //#TVAR_ash_pGAS[81]
	0x0F12, 0x0027, //#TVAR_ash_pGAS[82]
	0x0F12, 0x000F, //#TVAR_ash_pGAS[83]
	0x0F12, 0x0007, //#TVAR_ash_pGAS[84]
	0x0F12, 0x000F, //#TVAR_ash_pGAS[85]
	0x0F12, 0x0025, //#TVAR_ash_pGAS[86]
	0x0F12, 0x004D, //#TVAR_ash_pGAS[87]
	0x0F12, 0x0087, //#TVAR_ash_pGAS[88]
	0x0F12, 0x00D5, //#TVAR_ash_pGAS[89]
	0x0F12, 0x0123, //#TVAR_ash_pGAS[90]
	0x0F12, 0x013B, //#TVAR_ash_pGAS[91]
	0x0F12, 0x00F0, //#TVAR_ash_pGAS[92]
	0x0F12, 0x00A3, //#TVAR_ash_pGAS[93]
	0x0F12, 0x0067, //#TVAR_ash_pGAS[94]
	0x0F12, 0x003D, //#TVAR_ash_pGAS[95]
	0x0F12, 0x0025, //#TVAR_ash_pGAS[96]
	0x0F12, 0x001E, //#TVAR_ash_pGAS[97]
	0x0F12, 0x0027, //#TVAR_ash_pGAS[98]
	0x0F12, 0x003E, //#TVAR_ash_pGAS[99]
	0x0F12, 0x0066, //#TVAR_ash_pGAS[100]
	0x0F12, 0x00A1, //#TVAR_ash_pGAS[101]
	0x0F12, 0x00ED, //#TVAR_ash_pGAS[102]
	0x0F12, 0x0136, //#TVAR_ash_pGAS[103]
	0x0F12, 0x0158, //#TVAR_ash_pGAS[104]
	0x0F12, 0x0113, //#TVAR_ash_pGAS[105]
	0x0F12, 0x00C9, //#TVAR_ash_pGAS[106]
	0x0F12, 0x008F, //#TVAR_ash_pGAS[107]
	0x0F12, 0x0064, //#TVAR_ash_pGAS[108]
	0x0F12, 0x004D, //#TVAR_ash_pGAS[109]
	0x0F12, 0x0046, //#TVAR_ash_pGAS[110]
	0x0F12, 0x0050, //#TVAR_ash_pGAS[111]
	0x0F12, 0x0067, //#TVAR_ash_pGAS[112]
	0x0F12, 0x0091, //#TVAR_ash_pGAS[113]
	0x0F12, 0x00CA, //#TVAR_ash_pGAS[114]
	0x0F12, 0x0114, //#TVAR_ash_pGAS[115]
	0x0F12, 0x0157, //#TVAR_ash_pGAS[116]
	0x0F12, 0x017C, //#TVAR_ash_pGAS[117]
	0x0F12, 0x0146, //#TVAR_ash_pGAS[118]
	0x0F12, 0x0100, //#TVAR_ash_pGAS[119]
	0x0F12, 0x00C7, //#TVAR_ash_pGAS[120]
	0x0F12, 0x009E, //#TVAR_ash_pGAS[121]
	0x0F12, 0x0087, //#TVAR_ash_pGAS[122]
	0x0F12, 0x0082, //#TVAR_ash_pGAS[123]
	0x0F12, 0x008C, //#TVAR_ash_pGAS[124]
	0x0F12, 0x00A5, //#TVAR_ash_pGAS[125]
	0x0F12, 0x00CF, //#TVAR_ash_pGAS[126]
	0x0F12, 0x0106, //#TVAR_ash_pGAS[127]
	0x0F12, 0x014B, //#TVAR_ash_pGAS[128]
	0x0F12, 0x0184, //#TVAR_ash_pGAS[129]
	0x0F12, 0x019B, //#TVAR_ash_pGAS[130]
	0x0F12, 0x016C, //#TVAR_ash_pGAS[131]
	0x0F12, 0x0132, //#TVAR_ash_pGAS[132]
	0x0F12, 0x00FD, //#TVAR_ash_pGAS[133]
	0x0F12, 0x00D9, //#TVAR_ash_pGAS[134]
	0x0F12, 0x00C6, //#TVAR_ash_pGAS[135]
	0x0F12, 0x00C2, //#TVAR_ash_pGAS[136]
	0x0F12, 0x00CE, //#TVAR_ash_pGAS[137]
	0x0F12, 0x00E4, //#TVAR_ash_pGAS[138]
	0x0F12, 0x010B, //#TVAR_ash_pGAS[139]
	0x0F12, 0x013F, //#TVAR_ash_pGAS[140]
	0x0F12, 0x017A, //#TVAR_ash_pGAS[141]
	0x0F12, 0x01AF, //#TVAR_ash_pGAS[142]
	0x0F12, 0x0175, //#TVAR_ash_pGAS[143]
	0x0F12, 0x0146, //#TVAR_ash_pGAS[144]
	0x0F12, 0x0118, //#TVAR_ash_pGAS[145]
	0x0F12, 0x00EA, //#TVAR_ash_pGAS[146]
	0x0F12, 0x00C6, //#TVAR_ash_pGAS[147]
	0x0F12, 0x00AF, //#TVAR_ash_pGAS[148]
	0x0F12, 0x00A5, //#TVAR_ash_pGAS[149]
	0x0F12, 0x00A8, //#TVAR_ash_pGAS[150]
	0x0F12, 0x00B9, //#TVAR_ash_pGAS[151]
	0x0F12, 0x00D7, //#TVAR_ash_pGAS[152]
	0x0F12, 0x0102, //#TVAR_ash_pGAS[153]
	0x0F12, 0x0131, //#TVAR_ash_pGAS[154]
	0x0F12, 0x015E, //#TVAR_ash_pGAS[155]
	0x0F12, 0x015F, //#TVAR_ash_pGAS[156]
	0x0F12, 0x012D, //#TVAR_ash_pGAS[157]
	0x0F12, 0x00F2, //#TVAR_ash_pGAS[158]
	0x0F12, 0x00BF, //#TVAR_ash_pGAS[159]
	0x0F12, 0x009A, //#TVAR_ash_pGAS[160]
	0x0F12, 0x0083, //#TVAR_ash_pGAS[161]
	0x0F12, 0x0079, //#TVAR_ash_pGAS[162]
	0x0F12, 0x007C, //#TVAR_ash_pGAS[163]
	0x0F12, 0x008D, //#TVAR_ash_pGAS[164]
	0x0F12, 0x00AD, //#TVAR_ash_pGAS[165]
	0x0F12, 0x00DE, //#TVAR_ash_pGAS[166]
	0x0F12, 0x0115, //#TVAR_ash_pGAS[167]
	0x0F12, 0x0146, //#TVAR_ash_pGAS[168]
	0x0F12, 0x0139, //#TVAR_ash_pGAS[169]
	0x0F12, 0x00FD, //#TVAR_ash_pGAS[170]
	0x0F12, 0x00BC, //#TVAR_ash_pGAS[171]
	0x0F12, 0x0089, //#TVAR_ash_pGAS[172]
	0x0F12, 0x0064, //#TVAR_ash_pGAS[173]
	0x0F12, 0x004E, //#TVAR_ash_pGAS[174]
	0x0F12, 0x0044, //#TVAR_ash_pGAS[175]
	0x0F12, 0x0047, //#TVAR_ash_pGAS[176]
	0x0F12, 0x0059, //#TVAR_ash_pGAS[177]
	0x0F12, 0x007A, //#TVAR_ash_pGAS[178]
	0x0F12, 0x00A9, //#TVAR_ash_pGAS[179]
	0x0F12, 0x00E5, //#TVAR_ash_pGAS[180]
	0x0F12, 0x0126, //#TVAR_ash_pGAS[181]
	0x0F12, 0x011A, //#TVAR_ash_pGAS[182]
	0x0F12, 0x00D7, //#TVAR_ash_pGAS[183]
	0x0F12, 0x0095, //#TVAR_ash_pGAS[184]
	0x0F12, 0x0063, //#TVAR_ash_pGAS[185]
	0x0F12, 0x003F, //#TVAR_ash_pGAS[186]
	0x0F12, 0x002A, //#TVAR_ash_pGAS[187]
	0x0F12, 0x0020, //#TVAR_ash_pGAS[188]
	0x0F12, 0x0024, //#TVAR_ash_pGAS[189]
	0x0F12, 0x0036, //#TVAR_ash_pGAS[190]
	0x0F12, 0x0057, //#TVAR_ash_pGAS[191]
	0x0F12, 0x0085, //#TVAR_ash_pGAS[192]
	0x0F12, 0x00C3, //#TVAR_ash_pGAS[193]
	0x0F12, 0x0108, //#TVAR_ash_pGAS[194]
	0x0F12, 0x0102, //#TVAR_ash_pGAS[195]
	0x0F12, 0x00BC, //#TVAR_ash_pGAS[196]
	0x0F12, 0x007B, //#TVAR_ash_pGAS[197]
	0x0F12, 0x004B, //#TVAR_ash_pGAS[198]
	0x0F12, 0x0028, //#TVAR_ash_pGAS[199]
	0x0F12, 0x0012, //#TVAR_ash_pGAS[200]
	0x0F12, 0x0009, //#TVAR_ash_pGAS[201]
	0x0F12, 0x000D, //#TVAR_ash_pGAS[202]
	0x0F12, 0x0020, //#TVAR_ash_pGAS[203]
	0x0F12, 0x0041, //#TVAR_ash_pGAS[204]
	0x0F12, 0x0070, //#TVAR_ash_pGAS[205]
	0x0F12, 0x00B0, //#TVAR_ash_pGAS[206]
	0x0F12, 0x00F6, //#TVAR_ash_pGAS[207]
	0x0F12, 0x00F4, //#TVAR_ash_pGAS[208]
	0x0F12, 0x00AF, //#TVAR_ash_pGAS[209]
	0x0F12, 0x006F, //#TVAR_ash_pGAS[210]
	0x0F12, 0x0040, //#TVAR_ash_pGAS[211]
	0x0F12, 0x001D, //#TVAR_ash_pGAS[212]
	0x0F12, 0x0008, //#TVAR_ash_pGAS[213]
	0x0F12, 0x0000, //#TVAR_ash_pGAS[214]
	0x0F12, 0x0005, //#TVAR_ash_pGAS[215]
	0x0F12, 0x0018, //#TVAR_ash_pGAS[216]
	0x0F12, 0x0039, //#TVAR_ash_pGAS[217]
	0x0F12, 0x0069, //#TVAR_ash_pGAS[218]
	0x0F12, 0x00AA, //#TVAR_ash_pGAS[219]
	0x0F12, 0x00F2, //#TVAR_ash_pGAS[220]
	0x0F12, 0x00F5, //#TVAR_ash_pGAS[221]
	0x0F12, 0x00B0, //#TVAR_ash_pGAS[222]
	0x0F12, 0x0071, //#TVAR_ash_pGAS[223]
	0x0F12, 0x0042, //#TVAR_ash_pGAS[224]
	0x0F12, 0x0020, //#TVAR_ash_pGAS[225]
	0x0F12, 0x000C, //#TVAR_ash_pGAS[226]
	0x0F12, 0x0005, //#TVAR_ash_pGAS[227]
	0x0F12, 0x000B, //#TVAR_ash_pGAS[228]
	0x0F12, 0x001F, //#TVAR_ash_pGAS[229]
	0x0F12, 0x0040, //#TVAR_ash_pGAS[230]
	0x0F12, 0x0070, //#TVAR_ash_pGAS[231]
	0x0F12, 0x00B2, //#TVAR_ash_pGAS[232]
	0x0F12, 0x00F8, //#TVAR_ash_pGAS[233]
	0x0F12, 0x0101, //#TVAR_ash_pGAS[234]
	0x0F12, 0x00BD, //#TVAR_ash_pGAS[235]
	0x0F12, 0x0080, //#TVAR_ash_pGAS[236]
	0x0F12, 0x0052, //#TVAR_ash_pGAS[237]
	0x0F12, 0x0031, //#TVAR_ash_pGAS[238]
	0x0F12, 0x001D, //#TVAR_ash_pGAS[239]
	0x0F12, 0x0016, //#TVAR_ash_pGAS[240]
	0x0F12, 0x001D, //#TVAR_ash_pGAS[241]
	0x0F12, 0x0032, //#TVAR_ash_pGAS[242]
	0x0F12, 0x0054, //#TVAR_ash_pGAS[243]
	0x0F12, 0x0085, //#TVAR_ash_pGAS[244]
	0x0F12, 0x00C6, //#TVAR_ash_pGAS[245]
	0x0F12, 0x010A, //#TVAR_ash_pGAS[246]
	0x0F12, 0x0115, //#TVAR_ash_pGAS[247]
	0x0F12, 0x00D6, //#TVAR_ash_pGAS[248]
	0x0F12, 0x009C, //#TVAR_ash_pGAS[249]
	0x0F12, 0x006F, //#TVAR_ash_pGAS[250]
	0x0F12, 0x004E, //#TVAR_ash_pGAS[251]
	0x0F12, 0x003B, //#TVAR_ash_pGAS[252]
	0x0F12, 0x0035, //#TVAR_ash_pGAS[253]
	0x0F12, 0x003C, //#TVAR_ash_pGAS[254]
	0x0F12, 0x0051, //#TVAR_ash_pGAS[255]
	0x0F12, 0x0074, //#TVAR_ash_pGAS[256]
	0x0F12, 0x00A4, //#TVAR_ash_pGAS[257]
	0x0F12, 0x00E3, //#TVAR_ash_pGAS[258]
	0x0F12, 0x0125, //#TVAR_ash_pGAS[259]
	0x0F12, 0x012E, //#TVAR_ash_pGAS[260]
	0x0F12, 0x00FA, //#TVAR_ash_pGAS[261]
	0x0F12, 0x00C3, //#TVAR_ash_pGAS[262]
	0x0F12, 0x0097, //#TVAR_ash_pGAS[263]
	0x0F12, 0x0078, //#TVAR_ash_pGAS[264]
	0x0F12, 0x0066, //#TVAR_ash_pGAS[265]
	0x0F12, 0x0061, //#TVAR_ash_pGAS[266]
	0x0F12, 0x0069, //#TVAR_ash_pGAS[267]
	0x0F12, 0x007E, //#TVAR_ash_pGAS[268]
	0x0F12, 0x00A1, //#TVAR_ash_pGAS[269]
	0x0F12, 0x00D1, //#TVAR_ash_pGAS[270]
	0x0F12, 0x010B, //#TVAR_ash_pGAS[271]
	0x0F12, 0x0141, //#TVAR_ash_pGAS[272]
	0x0F12, 0x013E, //#TVAR_ash_pGAS[273]
	0x0F12, 0x0117, //#TVAR_ash_pGAS[274]
	0x0F12, 0x00E9, //#TVAR_ash_pGAS[275]
	0x0F12, 0x00BF, //#TVAR_ash_pGAS[276]
	0x0F12, 0x00A3, //#TVAR_ash_pGAS[277]
	0x0F12, 0x0094, //#TVAR_ash_pGAS[278]
	0x0F12, 0x008F, //#TVAR_ash_pGAS[279]
	0x0F12, 0x0099, //#TVAR_ash_pGAS[280]
	0x0F12, 0x00AF, //#TVAR_ash_pGAS[281]
	0x0F12, 0x00D1, //#TVAR_ash_pGAS[282]
	0x0F12, 0x00FD, //#TVAR_ash_pGAS[283]
	0x0F12, 0x012E, //#TVAR_ash_pGAS[284]
	0x0F12, 0x0160, //#TVAR_ash_pGAS[285]
	0x0F12, 0x0161, //#TVAR_ash_pGAS[286]
	0x0F12, 0x012F, //#TVAR_ash_pGAS[287]
	0x0F12, 0x0102, //#TVAR_ash_pGAS[288]
	0x0F12, 0x00DA, //#TVAR_ash_pGAS[289]
	0x0F12, 0x00BB, //#TVAR_ash_pGAS[290]
	0x0F12, 0x00A9, //#TVAR_ash_pGAS[291]
	0x0F12, 0x00A6, //#TVAR_ash_pGAS[292]
	0x0F12, 0x00B0, //#TVAR_ash_pGAS[293]
	0x0F12, 0x00C7, //#TVAR_ash_pGAS[294]
	0x0F12, 0x00E9, //#TVAR_ash_pGAS[295]
	0x0F12, 0x0114, //#TVAR_ash_pGAS[296]
	0x0F12, 0x0143, //#TVAR_ash_pGAS[297]
	0x0F12, 0x0179, //#TVAR_ash_pGAS[298]
	0x0F12, 0x0149, //#TVAR_ash_pGAS[299]
	0x0F12, 0x0113, //#TVAR_ash_pGAS[300]
	0x0F12, 0x00DE, //#TVAR_ash_pGAS[301]
	0x0F12, 0x00B0, //#TVAR_ash_pGAS[302]
	0x0F12, 0x0090, //#TVAR_ash_pGAS[303]
	0x0F12, 0x007C, //#TVAR_ash_pGAS[304]
	0x0F12, 0x0077, //#TVAR_ash_pGAS[305]
	0x0F12, 0x007F, //#TVAR_ash_pGAS[306]
	0x0F12, 0x0097, //#TVAR_ash_pGAS[307]
	0x0F12, 0x00BB, //#TVAR_ash_pGAS[308]
	0x0F12, 0x00EC, //#TVAR_ash_pGAS[309]
	0x0F12, 0x0122, //#TVAR_ash_pGAS[310]
	0x0F12, 0x0158, //#TVAR_ash_pGAS[311]
	0x0F12, 0x0127, //#TVAR_ash_pGAS[312]
	0x0F12, 0x00E9, //#TVAR_ash_pGAS[313]
	0x0F12, 0x00AD, //#TVAR_ash_pGAS[314]
	0x0F12, 0x007D, //#TVAR_ash_pGAS[315]
	0x0F12, 0x005C, //#TVAR_ash_pGAS[316]
	0x0F12, 0x0048, //#TVAR_ash_pGAS[317]
	0x0F12, 0x0042, //#TVAR_ash_pGAS[318]
	0x0F12, 0x004A, //#TVAR_ash_pGAS[319]
	0x0F12, 0x0061, //#TVAR_ash_pGAS[320]
	0x0F12, 0x0085, //#TVAR_ash_pGAS[321]
	0x0F12, 0x00B6, //#TVAR_ash_pGAS[322]
	0x0F12, 0x00F3, //#TVAR_ash_pGAS[323]
	0x0F12, 0x0134, //#TVAR_ash_pGAS[324]
	0x0F12, 0x010B, //#TVAR_ash_pGAS[325]
	0x0F12, 0x00C9, //#TVAR_ash_pGAS[326]
	0x0F12, 0x008C, //#TVAR_ash_pGAS[327]
	0x0F12, 0x005C, //#TVAR_ash_pGAS[328]
	0x0F12, 0x003B, //#TVAR_ash_pGAS[329]
	0x0F12, 0x0026, //#TVAR_ash_pGAS[330]
	0x0F12, 0x001F, //#TVAR_ash_pGAS[331]
	0x0F12, 0x0026, //#TVAR_ash_pGAS[332]
	0x0F12, 0x003D, //#TVAR_ash_pGAS[333]
	0x0F12, 0x0060, //#TVAR_ash_pGAS[334]
	0x0F12, 0x008F, //#TVAR_ash_pGAS[335]
	0x0F12, 0x00CE, //#TVAR_ash_pGAS[336]
	0x0F12, 0x0115, //#TVAR_ash_pGAS[337]
	0x0F12, 0x00FA, //#TVAR_ash_pGAS[338]
	0x0F12, 0x00B5, //#TVAR_ash_pGAS[339]
	0x0F12, 0x0078, //#TVAR_ash_pGAS[340]
	0x0F12, 0x0048, //#TVAR_ash_pGAS[341]
	0x0F12, 0x0026, //#TVAR_ash_pGAS[342]
	0x0F12, 0x0010, //#TVAR_ash_pGAS[343]
	0x0F12, 0x0008, //#TVAR_ash_pGAS[344]
	0x0F12, 0x0010, //#TVAR_ash_pGAS[345]
	0x0F12, 0x0025, //#TVAR_ash_pGAS[346]
	0x0F12, 0x0048, //#TVAR_ash_pGAS[347]
	0x0F12, 0x0077, //#TVAR_ash_pGAS[348]
	0x0F12, 0x00B6, //#TVAR_ash_pGAS[349]
	0x0F12, 0x00FE, //#TVAR_ash_pGAS[350]
	0x0F12, 0x00F6, //#TVAR_ash_pGAS[351]
	0x0F12, 0x00B0, //#TVAR_ash_pGAS[352]
	0x0F12, 0x0072, //#TVAR_ash_pGAS[353]
	0x0F12, 0x0042, //#TVAR_ash_pGAS[354]
	0x0F12, 0x0020, //#TVAR_ash_pGAS[355]
	0x0F12, 0x0009, //#TVAR_ash_pGAS[356]
	0x0F12, 0x0001, //#TVAR_ash_pGAS[357]
	0x0F12, 0x0008, //#TVAR_ash_pGAS[358]
	0x0F12, 0x001D, //#TVAR_ash_pGAS[359]
	0x0F12, 0x003F, //#TVAR_ash_pGAS[360]
	0x0F12, 0x006E, //#TVAR_ash_pGAS[361]
	0x0F12, 0x00AC, //#TVAR_ash_pGAS[362]
	0x0F12, 0x00F5, //#TVAR_ash_pGAS[363]
	0x0F12, 0x00FE, //#TVAR_ash_pGAS[364]
	0x0F12, 0x00B7, //#TVAR_ash_pGAS[365]
	0x0F12, 0x0079, //#TVAR_ash_pGAS[366]
	0x0F12, 0x0049, //#TVAR_ash_pGAS[367]
	0x0F12, 0x0027, //#TVAR_ash_pGAS[368]
	0x0F12, 0x0010, //#TVAR_ash_pGAS[369]
	0x0F12, 0x0007, //#TVAR_ash_pGAS[370]
	0x0F12, 0x000E, //#TVAR_ash_pGAS[371]
	0x0F12, 0x0022, //#TVAR_ash_pGAS[372]
	0x0F12, 0x0043, //#TVAR_ash_pGAS[373]
	0x0F12, 0x0073, //#TVAR_ash_pGAS[374]
	0x0F12, 0x00B1, //#TVAR_ash_pGAS[375]
	0x0F12, 0x00F8, //#TVAR_ash_pGAS[376]
	0x0F12, 0x0110, //#TVAR_ash_pGAS[377]
	0x0F12, 0x00CD, //#TVAR_ash_pGAS[378]
	0x0F12, 0x008F, //#TVAR_ash_pGAS[379]
	0x0F12, 0x005F, //#TVAR_ash_pGAS[380]
	0x0F12, 0x003B, //#TVAR_ash_pGAS[381]
	0x0F12, 0x0024, //#TVAR_ash_pGAS[382]
	0x0F12, 0x001B, //#TVAR_ash_pGAS[383]
	0x0F12, 0x0021, //#TVAR_ash_pGAS[384]
	0x0F12, 0x0034, //#TVAR_ash_pGAS[385]
	0x0F12, 0x0056, //#TVAR_ash_pGAS[386]
	0x0F12, 0x0085, //#TVAR_ash_pGAS[387]
	0x0F12, 0x00C4, //#TVAR_ash_pGAS[388]
	0x0F12, 0x0108, //#TVAR_ash_pGAS[389]
	0x0F12, 0x012C, //#TVAR_ash_pGAS[390]
	0x0F12, 0x00ED, //#TVAR_ash_pGAS[391]
	0x0F12, 0x00B1, //#TVAR_ash_pGAS[392]
	0x0F12, 0x0080, //#TVAR_ash_pGAS[393]
	0x0F12, 0x005B, //#TVAR_ash_pGAS[394]
	0x0F12, 0x0044, //#TVAR_ash_pGAS[395]
	0x0F12, 0x003B, //#TVAR_ash_pGAS[396]
	0x0F12, 0x003F, //#TVAR_ash_pGAS[397]
	0x0F12, 0x0053, //#TVAR_ash_pGAS[398]
	0x0F12, 0x0074, //#TVAR_ash_pGAS[399]
	0x0F12, 0x00A3, //#TVAR_ash_pGAS[400]
	0x0F12, 0x00E0, //#TVAR_ash_pGAS[401]
	0x0F12, 0x0121, //#TVAR_ash_pGAS[402]
	0x0F12, 0x014D, //#TVAR_ash_pGAS[403]
	0x0F12, 0x0115, //#TVAR_ash_pGAS[404]
	0x0F12, 0x00DD, //#TVAR_ash_pGAS[405]
	0x0F12, 0x00AD, //#TVAR_ash_pGAS[406]
	0x0F12, 0x0088, //#TVAR_ash_pGAS[407]
	0x0F12, 0x0071, //#TVAR_ash_pGAS[408]
	0x0F12, 0x0068, //#TVAR_ash_pGAS[409]
	0x0F12, 0x006C, //#TVAR_ash_pGAS[410]
	0x0F12, 0x007F, //#TVAR_ash_pGAS[411]
	0x0F12, 0x00A1, //#TVAR_ash_pGAS[412]
	0x0F12, 0x00CE, //#TVAR_ash_pGAS[413]
	0x0F12, 0x0107, //#TVAR_ash_pGAS[414]
	0x0F12, 0x013E, //#TVAR_ash_pGAS[415]
	0x0F12, 0x0161, //#TVAR_ash_pGAS[416]
	0x0F12, 0x0137, //#TVAR_ash_pGAS[417]
	0x0F12, 0x0104, //#TVAR_ash_pGAS[418]
	0x0F12, 0x00D8, //#TVAR_ash_pGAS[419]
	0x0F12, 0x00B5, //#TVAR_ash_pGAS[420]
	0x0F12, 0x00A0, //#TVAR_ash_pGAS[421]
	0x0F12, 0x0098, //#TVAR_ash_pGAS[422]
	0x0F12, 0x009C, //#TVAR_ash_pGAS[423]
	0x0F12, 0x00AE, //#TVAR_ash_pGAS[424]
	0x0F12, 0x00CE, //#TVAR_ash_pGAS[425]
	0x0F12, 0x00F9, //#TVAR_ash_pGAS[426]
	0x0F12, 0x012A, //#TVAR_ash_pGAS[427]
	0x0F12, 0x015E, //#TVAR_ash_pGAS[428]
	0x0F12, 0x0134, //#TVAR_ash_pGAS[429]
	0x0F12, 0x010D, //#TVAR_ash_pGAS[430]
	0x0F12, 0x00DF, //#TVAR_ash_pGAS[431]
	0x0F12, 0x00BC, //#TVAR_ash_pGAS[432]
	0x0F12, 0x00A5, //#TVAR_ash_pGAS[433]
	0x0F12, 0x0097, //#TVAR_ash_pGAS[434]
	0x0F12, 0x0094, //#TVAR_ash_pGAS[435]
	0x0F12, 0x009B, //#TVAR_ash_pGAS[436]
	0x0F12, 0x00A9, //#TVAR_ash_pGAS[437]
	0x0F12, 0x00C2, //#TVAR_ash_pGAS[438]
	0x0F12, 0x00E7, //#TVAR_ash_pGAS[439]
	0x0F12, 0x0113, //#TVAR_ash_pGAS[440]
	0x0F12, 0x013A, //#TVAR_ash_pGAS[441]
	0x0F12, 0x011E, //#TVAR_ash_pGAS[442]
	0x0F12, 0x00F1, //#TVAR_ash_pGAS[443]
	0x0F12, 0x00BD, //#TVAR_ash_pGAS[444]
	0x0F12, 0x0096, //#TVAR_ash_pGAS[445]
	0x0F12, 0x007D, //#TVAR_ash_pGAS[446]
	0x0F12, 0x006F, //#TVAR_ash_pGAS[447]
	0x0F12, 0x006C, //#TVAR_ash_pGAS[448]
	0x0F12, 0x0072, //#TVAR_ash_pGAS[449]
	0x0F12, 0x0081, //#TVAR_ash_pGAS[450]
	0x0F12, 0x009A, //#TVAR_ash_pGAS[451]
	0x0F12, 0x00C1, //#TVAR_ash_pGAS[452]
	0x0F12, 0x00F1, //#TVAR_ash_pGAS[453]
	0x0F12, 0x011D, //#TVAR_ash_pGAS[454]
	0x0F12, 0x00F6, //#TVAR_ash_pGAS[455]
	0x0F12, 0x00C3, //#TVAR_ash_pGAS[456]
	0x0F12, 0x008D, //#TVAR_ash_pGAS[457]
	0x0F12, 0x0066, //#TVAR_ash_pGAS[458]
	0x0F12, 0x004C, //#TVAR_ash_pGAS[459]
	0x0F12, 0x003F, //#TVAR_ash_pGAS[460]
	0x0F12, 0x003B, //#TVAR_ash_pGAS[461]
	0x0F12, 0x0040, //#TVAR_ash_pGAS[462]
	0x0F12, 0x004E, //#TVAR_ash_pGAS[463]
	0x0F12, 0x0068, //#TVAR_ash_pGAS[464]
	0x0F12, 0x008D, //#TVAR_ash_pGAS[465]
	0x0F12, 0x00C0, //#TVAR_ash_pGAS[466]
	0x0F12, 0x00F4, //#TVAR_ash_pGAS[467]
	0x0F12, 0x00DA, //#TVAR_ash_pGAS[468]
	0x0F12, 0x00A4, //#TVAR_ash_pGAS[469]
	0x0F12, 0x006D, //#TVAR_ash_pGAS[470]
	0x0F12, 0x0046, //#TVAR_ash_pGAS[471]
	0x0F12, 0x002D, //#TVAR_ash_pGAS[472]
	0x0F12, 0x001F, //#TVAR_ash_pGAS[473]
	0x0F12, 0x001A, //#TVAR_ash_pGAS[474]
	0x0F12, 0x001F, //#TVAR_ash_pGAS[475]
	0x0F12, 0x002D, //#TVAR_ash_pGAS[476]
	0x0F12, 0x0044, //#TVAR_ash_pGAS[477]
	0x0F12, 0x006A, //#TVAR_ash_pGAS[478]
	0x0F12, 0x009F, //#TVAR_ash_pGAS[479]
	0x0F12, 0x00D4, //#TVAR_ash_pGAS[480]
	0x0F12, 0x00C8, //#TVAR_ash_pGAS[481]
	0x0F12, 0x0091, //#TVAR_ash_pGAS[482]
	0x0F12, 0x005B, //#TVAR_ash_pGAS[483]
	0x0F12, 0x0034, //#TVAR_ash_pGAS[484]
	0x0F12, 0x001A, //#TVAR_ash_pGAS[485]
	0x0F12, 0x000B, //#TVAR_ash_pGAS[486]
	0x0F12, 0x0007, //#TVAR_ash_pGAS[487]
	0x0F12, 0x000B, //#TVAR_ash_pGAS[488]
	0x0F12, 0x0018, //#TVAR_ash_pGAS[489]
	0x0F12, 0x0031, //#TVAR_ash_pGAS[490]
	0x0F12, 0x0054, //#TVAR_ash_pGAS[491]
	0x0F12, 0x0089, //#TVAR_ash_pGAS[492]
	0x0F12, 0x00C0, //#TVAR_ash_pGAS[493]
	0x0F12, 0x00C5, //#TVAR_ash_pGAS[494]
	0x0F12, 0x008E, //#TVAR_ash_pGAS[495]
	0x0F12, 0x0056, //#TVAR_ash_pGAS[496]
	0x0F12, 0x002F, //#TVAR_ash_pGAS[497]
	0x0F12, 0x0015, //#TVAR_ash_pGAS[498]
	0x0F12, 0x0005, //#TVAR_ash_pGAS[499]
	0x0F12, 0x0000, //#TVAR_ash_pGAS[500]
	0x0F12, 0x0004, //#TVAR_ash_pGAS[501]
	0x0F12, 0x0011, //#TVAR_ash_pGAS[502]
	0x0F12, 0x0029, //#TVAR_ash_pGAS[503]
	0x0F12, 0x004C, //#TVAR_ash_pGAS[504]
	0x0F12, 0x0083, //#TVAR_ash_pGAS[505]
	0x0F12, 0x00BA, //#TVAR_ash_pGAS[506]
	0x0F12, 0x00CF, //#TVAR_ash_pGAS[507]
	0x0F12, 0x0097, //#TVAR_ash_pGAS[508]
	0x0F12, 0x005F, //#TVAR_ash_pGAS[509]
	0x0F12, 0x0036, //#TVAR_ash_pGAS[510]
	0x0F12, 0x001C, //#TVAR_ash_pGAS[511]
	0x0F12, 0x000C, //#TVAR_ash_pGAS[512]
	0x0F12, 0x0007, //#TVAR_ash_pGAS[513]
	0x0F12, 0x000A, //#TVAR_ash_pGAS[514]
	0x0F12, 0x0018, //#TVAR_ash_pGAS[515]
	0x0F12, 0x002F, //#TVAR_ash_pGAS[516]
	0x0F12, 0x0053, //#TVAR_ash_pGAS[517]
	0x0F12, 0x0089, //#TVAR_ash_pGAS[518]
	0x0F12, 0x00BF, //#TVAR_ash_pGAS[519]
	0x0F12, 0x00E4, //#TVAR_ash_pGAS[520]
	0x0F12, 0x00AD, //#TVAR_ash_pGAS[521]
	0x0F12, 0x0074, //#TVAR_ash_pGAS[522]
	0x0F12, 0x004C, //#TVAR_ash_pGAS[523]
	0x0F12, 0x0030, //#TVAR_ash_pGAS[524]
	0x0F12, 0x0020, //#TVAR_ash_pGAS[525]
	0x0F12, 0x0019, //#TVAR_ash_pGAS[526]
	0x0F12, 0x001D, //#TVAR_ash_pGAS[527]
	0x0F12, 0x002A, //#TVAR_ash_pGAS[528]
	0x0F12, 0x0041, //#TVAR_ash_pGAS[529]
	0x0F12, 0x0067, //#TVAR_ash_pGAS[530]
	0x0F12, 0x009C, //#TVAR_ash_pGAS[531]
	0x0F12, 0x00D1, //#TVAR_ash_pGAS[532]
	0x0F12, 0x0102, //#TVAR_ash_pGAS[533]
	0x0F12, 0x00CD, //#TVAR_ash_pGAS[534]
	0x0F12, 0x0096, //#TVAR_ash_pGAS[535]
	0x0F12, 0x006C, //#TVAR_ash_pGAS[536]
	0x0F12, 0x004E, //#TVAR_ash_pGAS[537]
	0x0F12, 0x003C, //#TVAR_ash_pGAS[538]
	0x0F12, 0x0036, //#TVAR_ash_pGAS[539]
	0x0F12, 0x0039, //#TVAR_ash_pGAS[540]
	0x0F12, 0x0046, //#TVAR_ash_pGAS[541]
	0x0F12, 0x005F, //#TVAR_ash_pGAS[542]
	0x0F12, 0x0085, //#TVAR_ash_pGAS[543]
	0x0F12, 0x00B8, //#TVAR_ash_pGAS[544]
	0x0F12, 0x00EB, //#TVAR_ash_pGAS[545]
	0x0F12, 0x0126, //#TVAR_ash_pGAS[546]
	0x0F12, 0x00F9, //#TVAR_ash_pGAS[547]
	0x0F12, 0x00C2, //#TVAR_ash_pGAS[548]
	0x0F12, 0x0097, //#TVAR_ash_pGAS[549]
	0x0F12, 0x0079, //#TVAR_ash_pGAS[550]
	0x0F12, 0x0065, //#TVAR_ash_pGAS[551]
	0x0F12, 0x005E, //#TVAR_ash_pGAS[552]
	0x0F12, 0x0060, //#TVAR_ash_pGAS[553]
	0x0F12, 0x006F, //#TVAR_ash_pGAS[554]
	0x0F12, 0x008A, //#TVAR_ash_pGAS[555]
	0x0F12, 0x00AF, //#TVAR_ash_pGAS[556]
	0x0F12, 0x00E2, //#TVAR_ash_pGAS[557]
	0x0F12, 0x010F, //#TVAR_ash_pGAS[558]
	0x0F12, 0x013B, //#TVAR_ash_pGAS[559]
	0x0F12, 0x0119, //#TVAR_ash_pGAS[560]
	0x0F12, 0x00E8, //#TVAR_ash_pGAS[561]
	0x0F12, 0x00BD, //#TVAR_ash_pGAS[562]
	0x0F12, 0x00A0, //#TVAR_ash_pGAS[563]
	0x0F12, 0x008F, //#TVAR_ash_pGAS[564]
	0x0F12, 0x0087, //#TVAR_ash_pGAS[565]
	0x0F12, 0x008B, //#TVAR_ash_pGAS[566]
	0x0F12, 0x0099, //#TVAR_ash_pGAS[567]
	0x0F12, 0x00B3, //#TVAR_ash_pGAS[568]
	0x0F12, 0x00D7, //#TVAR_ash_pGAS[569]
	0x0F12, 0x0105, //#TVAR_ash_pGAS[570]
	0x0F12, 0x0130, //#TVAR_ash_pGAS[571]
	0x002A, 0x0D30,
	0x0F12, 0x02A7, //#awbb_GLocusR
	0x0F12, 0x0343, //#awbb_GLocusB
	0x002A, 0x06B8, //##GAS_Alpha separated
	0x0F12, 0x00D0, //#TVAR_ash_AwbAshCord_0_Hor
	0x0F12, 0x00F0, //#TVAR_ash_AwbAshCord_1_Inca
	0x0F12, 0x0120, //#TVAR_ash_AwbAshCord_2_CWF,TL84
	0x0F12, 0x0150, //#TVAR_ash_AwbAshCord_3_실내
	0x0F12, 0x0158, //0160 //#TVAR_ash_AwbAshCord_4_D50, DNP
	0x0F12, 0x016A, //#TVAR_ash_AwbAshCord_5_D65
	0x0F12, 0x01A0, //#TVAR_ash_AwbAshCord_6_D75
	
	0x002A, 0x0688, //#seti_bUseEfuseCorrection
	0x0F12, 0x0000, //# 0 : Not use, 1 : Use

	};
	
	static unsigned short s5k5cagx_init_reg_short5_old[]={	
	//#================================================================================================
	//#SET CCM
	//#================================================================================================ 
	//#CCM start address //#7000_33A4
	0x0F12, 0x7000,
	0x002A, 0x0698,
	0x0F12, 0x33A4,
	0x0F12, 0x7000,
	//#Horizon[0]
	0x002A, 0x33A4,
	0x0F12, 0x01FE, //#TVAR_wbt_pBaseCcms [01]
	0x0F12, 0xFF50, //#TVAR_wbt_pBaseCcms [02]
	0x0F12, 0xFFDC, //#TVAR_wbt_pBaseCcms [03]
	0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
	0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
	0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
	0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
	0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
	0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
	0x0F12, 0x0120, //#TVAR_wbt_pBaseCcms [10]
	0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
	0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
	0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
	0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
	0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
	0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
	0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
	0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
	//#Inca[1]
	0x0F12, 0x01FE, //#TVAR_wbt_pBaseCcms [01]
	0x0F12, 0xFF50, //#TVAR_wbt_pBaseCcms [02]
	0x0F12, 0xFFDC, //#TVAR_wbt_pBaseCcms [03]
	0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
	0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
	0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
	0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
	0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
	0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
	0x0F12, 0x0120, //#TVAR_wbt_pBaseCcms [10]
	0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
	0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
	0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
	0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
	0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
	0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
	0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
	0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
	//#TL84[2]
	0x0F12, 0x01FE, //#TVAR_wbt_pBaseCcms [01]
	0x0F12, 0xFF50, //#TVAR_wbt_pBaseCcms [02]
	0x0F12, 0xFFDC, //#TVAR_wbt_pBaseCcms [03]
	0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
	0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
	0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
	0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
	0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
	0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
	0x0F12, 0x0120, //#TVAR_wbt_pBaseCcms [10]
	0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
	0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
	0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
	0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
	0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
	0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
	0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
	0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
	//#CWF[3]
	0x0F12, 0x01FE, //#TVAR_wbt_pBaseCcms [01]
	0x0F12, 0xFF50, //#TVAR_wbt_pBaseCcms [02]
	0x0F12, 0xFFDC, //#TVAR_wbt_pBaseCcms [03]
	0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
	0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
	0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
	0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
	0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
	0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
	0x0F12, 0x0120, //#TVAR_wbt_pBaseCcms [10]
	0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
	0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
	0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
	0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
	0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
	0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
	0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
	0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
	//#D50[4]
	0x0F12, 0x01FE, //#TVAR_wbt_pBaseCcms [01]
	0x0F12, 0xFF50, //#TVAR_wbt_pBaseCcms [02]
	0x0F12, 0xFFDC, //#TVAR_wbt_pBaseCcms [03]
	0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
	0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
	0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
	0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
	0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
	0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
	0x0F12, 0x0120, //#TVAR_wbt_pBaseCcms [10]
	0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
	0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
	0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
	0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
	0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
	0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
	0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
	0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
	//#D65[5]
	0x0F12, 0x01FE, //#TVAR_wbt_pBaseCcms [01]
	0x0F12, 0xFF50, //#TVAR_wbt_pBaseCcms [02]
	0x0F12, 0xFFDC, //#TVAR_wbt_pBaseCcms [03]
	0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
	0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
	0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
	0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
	0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
	0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
	0x0F12, 0x0120, //#TVAR_wbt_pBaseCcms [10]
	0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
	0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
	0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
	0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
	0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
	0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
	0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
	0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
	//#Outdoor CCM address //#7000_3380
	0x002A, 0x06A0,
	0x0F12, 0x3380,
	0x0F12, 0x7000,
	//#Outdoor CCM
	0x002A, 0x3380,
	0x0F12, 0x01FE, //#TVAR_wbt_pOutdoorCcm [01]
	0x0F12, 0xFF50, //#TVAR_wbt_pOutdoorCcm [02]
	0x0F12, 0xFFDC, //#TVAR_wbt_pOutdoorCcm [03]
	0x0F12, 0xFF21, //#TVAR_wbt_pOutdoorCcm [04]
	0x0F12, 0x0210, //#TVAR_wbt_pOutdoorCcm [05]
	0x0F12, 0xFF15, //#TVAR_wbt_pOutdoorCcm [06]
	0x0F12, 0xFFCA, //#TVAR_wbt_pOutdoorCcm [07]
	0x0F12, 0xFF9B, //#TVAR_wbt_pOutdoorCcm [08]
	0x0F12, 0x021F, //#TVAR_wbt_pOutdoorCcm [09]
	0x0F12, 0x0120, //#TVAR_wbt_pOutdoorCcm [10]
	0x0F12, 0x0156, //#TVAR_wbt_pOutdoorCcm [11]
	0x0F12, 0xFEFF, //#TVAR_wbt_pOutdoorCcm [12]
	0x0F12, 0x0161, //#TVAR_wbt_pOutdoorCcm [13]
	0x0F12, 0xFF88, //#TVAR_wbt_pOutdoorCcm [14]
	0x0F12, 0x01C8, //#TVAR_wbt_pOutdoorCcm [15]
	0x0F12, 0xFE58, //#TVAR_wbt_pOutdoorCcm [16]
	0x0F12, 0x0206, //#TVAR_wbt_pOutdoorCcm [17]
	0x0F12, 0x0175, //#TVAR_wbt_pOutdoorCcm [18]
	0x002A, 0x06A6,
	0x0F12, 0x00D8, //#SARR_AwbCcmCord_0_Horizon (00E7)
	0x0F12, 0x0100, //#SARR_AwbCcmCord_1_Inca (0111)
	0x0F12, 0x0120, //#SARR_AwbCcmCord_2_TL84 (012F)
	0x0F12, 0x0140, //#SARR_AwbCcmCord_3_CWF (0145)
	0x0F12, 0x0160, //#SARR_AwbCcmCord_4_D50 (0177)
	0x0F12, 0x0180, //#SARR_AwbCcmCord_5_D65 (0178) 
};
static unsigned short s5k5cagx_init_reg_short5[]={	
//#================================================================================================
//#SET CCM
//#================================================================================================ 
//#CCM start address //#7000_33A4
0x002A, 0x0698,
0x0F12, 0x33A4,
0x0F12, 0x7000,
//#Horizon[0]
0x002A, 0x33A4,
0x0F12, 0x01A0, //#TVAR_wbt_pBaseCcms [01]
0x0F12, 0xFF80, //#TVAR_wbt_pBaseCcms [02]
0x0F12, 0xFFCC, //#TVAR_wbt_pBaseCcms [03]
0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
0x0F12, 0x0080, //#TVAR_wbt_pBaseCcms [10]
0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
//#Inca[1]
0x0F12, 0x01A0, //#TVAR_wbt_pBaseCcms [01]
0x0F12, 0xFF80, //#TVAR_wbt_pBaseCcms [02]
0x0F12, 0xFFCC, //#TVAR_wbt_pBaseCcms [03]
0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
0x0F12, 0x0080, //#TVAR_wbt_pBaseCcms [10]
0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
//#TL84[2]
0x0F12, 0x01A0, //#TVAR_wbt_pBaseCcms [01]
0x0F12, 0xFF80, //#TVAR_wbt_pBaseCcms [02]
0x0F12, 0xFFCC, //#TVAR_wbt_pBaseCcms [03]
0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
0x0F12, 0x0080, //#TVAR_wbt_pBaseCcms [10]
0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
//#CWF[3]
0x0F12, 0x01A0, //01FE //#TVAR_wbt_pBaseCcms [01]
0x0F12, 0xFF80, //FF50 //#TVAR_wbt_pBaseCcms [02]
0x0F12, 0xFFCC, //FFDC //#TVAR_wbt_pBaseCcms [03]
0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
0x0F12, 0x0080, //0120 //#TVAR_wbt_pBaseCcms [10]
0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
//#D50[4]
0x0F12, 0x01A0, //#TVAR_wbt_pBaseCcms [01]
0x0F12, 0xFF80, //#TVAR_wbt_pBaseCcms [02]
0x0F12, 0xFFCC, //#TVAR_wbt_pBaseCcms [03]
0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
0x0F12, 0x0080, //#TVAR_wbt_pBaseCcms [10]
0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
//#D65[5]
0x0F12, 0x01A0, //#TVAR_wbt_pBaseCcms [01]
0x0F12, 0xFF80, //#TVAR_wbt_pBaseCcms [02]
0x0F12, 0xFFCC, //#TVAR_wbt_pBaseCcms [03]
0x0F12, 0xFF21, //#TVAR_wbt_pBaseCcms [04]
0x0F12, 0x0210, //#TVAR_wbt_pBaseCcms [05]
0x0F12, 0xFF15, //#TVAR_wbt_pBaseCcms [06]
0x0F12, 0xFFCA, //#TVAR_wbt_pBaseCcms [07]
0x0F12, 0xFF9B, //#TVAR_wbt_pBaseCcms [08]
0x0F12, 0x021F, //#TVAR_wbt_pBaseCcms [09]
0x0F12, 0x0080, //#TVAR_wbt_pBaseCcms [10]
0x0F12, 0x0156, //#TVAR_wbt_pBaseCcms [11]
0x0F12, 0xFEFF, //#TVAR_wbt_pBaseCcms [12]
0x0F12, 0x0161, //#TVAR_wbt_pBaseCcms [13]
0x0F12, 0xFF88, //#TVAR_wbt_pBaseCcms [14]
0x0F12, 0x01C8, //#TVAR_wbt_pBaseCcms [15]
0x0F12, 0xFE58, //#TVAR_wbt_pBaseCcms [16]
0x0F12, 0x0206, //#TVAR_wbt_pBaseCcms [17]
0x0F12, 0x0175, //#TVAR_wbt_pBaseCcms [18]
//#Outdoor CCM address //#7000_3380
0x002A, 0x06A0,
0x0F12, 0x3380,
0x0F12, 0x7000,
//#Outdoor CCM
0x002A, 0x3380,
0x0F12, 0x01A0, //#TVAR_wbt_pOutdoorCcm [01]
0x0F12, 0xFF80, //#TVAR_wbt_pOutdoorCcm [02]
0x0F12, 0xFFCC, //#TVAR_wbt_pOutdoorCcm [03]
0x0F12, 0xFF21, //#TVAR_wbt_pOutdoorCcm [04]
0x0F12, 0x0210, //#TVAR_wbt_pOutdoorCcm [05]
0x0F12, 0xFF15, //#TVAR_wbt_pOutdoorCcm [06]
0x0F12, 0xFFCA, //#TVAR_wbt_pOutdoorCcm [07]
0x0F12, 0xFF9B, //#TVAR_wbt_pOutdoorCcm [08]
0x0F12, 0x021F, //#TVAR_wbt_pOutdoorCcm [09]
0x0F12, 0x0080, //#TVAR_wbt_pOutdoorCcm [10]
0x0F12, 0x0156, //#TVAR_wbt_pOutdoorCcm [11]
0x0F12, 0xFEFF, //#TVAR_wbt_pOutdoorCcm [12]
0x0F12, 0x0161, //#TVAR_wbt_pOutdoorCcm [13]
0x0F12, 0xFF88, //#TVAR_wbt_pOutdoorCcm [14]
0x0F12, 0x01C8, //#TVAR_wbt_pOutdoorCcm [15]
0x0F12, 0xFE58, //#TVAR_wbt_pOutdoorCcm [16]
0x0F12, 0x0206, //#TVAR_wbt_pOutdoorCcm [17]
0x0F12, 0x0175, //#TVAR_wbt_pOutdoorCcm [18]

0x002A, 0x06A6,
0x0F12, 0x00D8, //#SARR_AwbCcmCord_0_Horizon (00E7)
0x0F12, 0x0100, //#SARR_AwbCcmCord_1_Inca (0111)
0x0F12, 0x0120, //#SARR_AwbCcmCord_2_TL84 (012F)
0x0F12, 0x0140, //#SARR_AwbCcmCord_3_CWF (0145)
0x0F12, 0x0160, //#SARR_AwbCcmCord_4_D50 (0177)
0x0F12, 0x0180, //#SARR_AwbCcmCord_5_D65 (0178) 
};
static unsigned short s5k5cagx_init_reg_short6[]={		

	//================================================================================================
	// SET AWB
	//================================================================================================
	// Indoor boundary
	0x0F12, 0x7000,
	0x002A, 0x0C48,
	0x0F12, 0x03C8,  //034C //awbb_IndoorGrZones_m_BGrid[0] 
	0x0F12, 0x03E2,  //037E //awbb_IndoorGrZones_m_BGrid[1] 
	0x0F12, 0x038A,  //0307 //awbb_IndoorGrZones_m_BGrid[2] 
	0x0F12, 0x03F4,  //037C //awbb_IndoorGrZones_m_BGrid[3] 
	0x0F12, 0x034E,  //02D5 //awbb_IndoorGrZones_m_BGrid[4] 
	0x0F12, 0x03E2,  //035F //awbb_IndoorGrZones_m_BGrid[5] 
	0x0F12, 0x030A,  //02A9 //awbb_IndoorGrZones_m_BGrid[6] 
	0x0F12, 0x03B8,  //033A //awbb_IndoorGrZones_m_BGrid[7] 
	0x0F12, 0x02C8,  //0283 //awbb_IndoorGrZones_m_BGrid[8] 
	0x0F12, 0x038A,  //031A //awbb_IndoorGrZones_m_BGrid[9] 
	0x0F12, 0x029C,  //0261 //awbb_IndoorGrZones_m_BGrid[10]
	0x0F12, 0x034E,  //02FE //awbb_IndoorGrZones_m_BGrid[11]
	0x0F12, 0x0286,  //0244 //awbb_IndoorGrZones_m_BGrid[12]
	0x0F12, 0x0310,  //02E3 //awbb_IndoorGrZones_m_BGrid[13]
	0x0F12, 0x026C,  //0225 //awbb_IndoorGrZones_m_BGrid[14]
	0x0F12, 0x02E4,  //02D2 //awbb_IndoorGrZones_m_BGrid[15]
	0x0F12, 0x0254,  //0211 //awbb_IndoorGrZones_m_BGrid[16]
	0x0F12, 0x02CA,  //02BE //awbb_IndoorGrZones_m_BGrid[17]
	0x0F12, 0x023E,  //01FD //awbb_IndoorGrZones_m_BGrid[18]
	0x0F12, 0x02B8,  //02AC //awbb_IndoorGrZones_m_BGrid[19]
	0x0F12, 0x022E,  //01F2 //awbb_IndoorGrZones_m_BGrid[20]
	0x0F12, 0x02A4,  //0299 //awbb_IndoorGrZones_m_BGrid[21]
	0x0F12, 0x0226,  //01E3 //awbb_IndoorGrZones_m_BGrid[22]
	0x0F12, 0x0294,  //0286 //awbb_IndoorGrZones_m_BGrid[23]
	0x0F12, 0x0220,  //01D8 //awbb_IndoorGrZones_m_BGrid[24]
	0x0F12, 0x027E,  //026B //awbb_IndoorGrZones_m_BGrid[25]
	0x0F12, 0x022A,  //01D8 //awbb_IndoorGrZones_m_BGrid[26]
	0x0F12, 0x025E,  //0257 //awbb_IndoorGrZones_m_BGrid[27]
	0x0F12, 0x0000,  //01F2 //awbb_IndoorGrZones_m_BGrid[28]
	0x0F12, 0x0000,  //0230 //awbb_IndoorGrZones_m_BGrid[29]
	0x0F12, 0x0000,  //0000 //awbb_IndoorGrZones_m_BGrid[30]
	0x0F12, 0x0000,  //0000 //awbb_IndoorGrZones_m_BGrid[31]
	0x0F12, 0x0000,  //0000 //awbb_IndoorGrZones_m_BGrid[32]
	0x0F12, 0x0000,  //0000 //awbb_IndoorGrZones_m_BGrid[33]
	0x0F12, 0x0000,  //0000 //awbb_IndoorGrZones_m_BGrid[34]
	0x0F12, 0x0000,  //0000 //awbb_IndoorGrZones_m_BGrid[35]
	0x0F12, 0x0000,  //0000 //awbb_IndoorGrZones_m_BGrid[36]
	0x0F12, 0x0000,  //0000 //awbb_IndoorGrZones_m_BGrid[37]
	0x0F12, 0x0000,  //0000 //awbb_IndoorGrZones_m_BGrid[38]
	0x0F12, 0x0000,  //0000 //awbb_IndoorGrZones_m_BGrid[39]
	0x0F12, 0x0005,  //0005 // #awbb_IndoorGrZones_m_GridStep
	0x002A, 0x0CA0, //0CA0
	0x0F12, 0x00FE, //0169 // #awbb_IndoorGrZones_m_Boffs
		 
	// Outdoor boundary
	0x002A, 0x0CA4,
	0x0F12, 0x027E,  //0281 // #awbb_OutdoorGrZones_m_BGrid[0] 
	0x0F12, 0x02AE,  //02BC // #awbb_OutdoorGrZones_m_BGrid[1] 
	0x0F12, 0x025C,  //0258 // #awbb_OutdoorGrZones_m_BGrid[2] 
	0x0F12, 0x02B2,  //02C9 // #awbb_OutdoorGrZones_m_BGrid[3] 
	0x0F12, 0x0244,  //023E // #awbb_OutdoorGrZones_m_BGrid[4] 
	0x0F12, 0x02A0,  //02C3 // #awbb_OutdoorGrZones_m_BGrid[5] 
	0x0F12, 0x0236,  //022F // #awbb_OutdoorGrZones_m_BGrid[6] 
	0x0F12, 0x0290,  //02B5 // #awbb_OutdoorGrZones_m_BGrid[7] 
	0x0F12, 0x0230,  //022F // #awbb_OutdoorGrZones_m_BGrid[8] 
	0x0F12, 0x027A,  //029A // #awbb_OutdoorGrZones_m_BGrid[9] 
	0x0F12, 0x0236,  //0251 // #awbb_OutdoorGrZones_m_BGrid[10]
	0x0F12, 0x025E,  //0272 // #awbb_OutdoorGrZones_m_BGrid[11]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[12]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[13]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[14]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[15]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[16]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[17]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[18]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[19]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[20]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[21]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[22]
	0x0F12, 0x0000,  //0000 // #awbb_OutdoorGrZones_m_BGrid[23]
	0x0F12, 0x0005,  //0005 // #awbb_OutdoorGrZones_m_GridStep
	0x002A, 0x0CDC,  //0CDC
	0x0F12, 0x01EE,  //0216 // #awbb_OutdoorGrZones_m_Boffs
	 
	// Outdoor detection zone??
	0x002A, 0x0D88,
	0x0F12, 0xFFB6, //#awbb_OutdoorDetectionZone_m_BGrid[0]_m_left
	0x0F12, 0x00B6, //#awbb_OutdoorDetectionZone_m_BGrid[0]_m_right
	0x0F12, 0xFF38, //#awbb_OutdoorDetectionZone_m_BGrid[1]_m_left
	0x0F12, 0x0118, //#awbb_OutdoorDetectionZone_m_BGrid[1]_m_right
	0x0F12, 0xFEF1, //#awbb_OutdoorDetectionZone_m_BGrid[2]_m_left
	0x0F12, 0x015F, //#awbb_OutdoorDetectionZone_m_BGrid[2]_m_right
	0x0F12, 0xFEC0, //#awbb_OutdoorDetectionZone_m_BGrid[3]_m_left
	0x0F12, 0x0199, //#awbb_OutdoorDetectionZone_m_BGrid[3]_m_right
	0x0F12, 0xFE91, //#awbb_OutdoorDetectionZone_m_BGrid[4]_m_left
	0x0F12, 0x01CF, //#awbb_OutdoorDetectionZone_m_BGrid[4]_m_right
	0x0F12, 0x1388, //#awbb_OutdoorDetectionZone_ZInfo_m_AbsGridStep
	0x0F12, 0x0000,
	0x0F12, 0x0005, //#awbb_OutdoorDetectionZone_ZInfo_m_GridSz
	0x0F12, 0x0000,
	0x0F12, 0x1387, //#awbb_OutdoorDetectionZone_ZInfo_m_NBoffs
	0x0F12, 0x0000,
	0x0F12, 0x1388, //#awbb_OutdoorDetectionZone_ZInfo_m_MaxNB 
	0x0F12, 0x0000,
	
	// LowBr boundary
	0x002A, 0x0CE0,
	0x0F12, 0x03EA,  //03C6 //#awbb_LowBrGrZones_m_BGrid[0]
	0x0F12, 0x044E,  //03F1 //#awbb_LowBrGrZones_m_BGrid[1]
	0x0F12, 0x035E,  //0329 //#awbb_LowBrGrZones_m_BGrid[2]
	0x0F12, 0x044C,  //03F6 //#awbb_LowBrGrZones_m_BGrid[3]
	0x0F12, 0x02FA,  //02BF //#awbb_LowBrGrZones_m_BGrid[4]
	0x0F12, 0x0434,  //03B6 //#awbb_LowBrGrZones_m_BGrid[5]
	0x0F12, 0x02AA,  //0268 //#awbb_LowBrGrZones_m_BGrid[6]
	0x0F12, 0x03F2,  //037A //#awbb_LowBrGrZones_m_BGrid[7]
	0x0F12, 0x0266,  //022A //#awbb_LowBrGrZones_m_BGrid[8]
	0x0F12, 0x03AE,  //0332 //#awbb_LowBrGrZones_m_BGrid[9]
	0x0F12, 0x022C,  //01F6 //#awbb_LowBrGrZones_m_BGrid[10]
	0x0F12, 0x035A,  //02F9 //#awbb_LowBrGrZones_m_BGrid[11]
	0x0F12, 0x020E,  //01D6 //#awbb_LowBrGrZones_m_BGrid[12]
	0x0F12, 0x0314,  //02CC //#awbb_LowBrGrZones_m_BGrid[13]
	0x0F12, 0x01F4,  //01C2 //#awbb_LowBrGrZones_m_BGrid[14]
	0x0F12, 0x02E0,  //02A7 //#awbb_LowBrGrZones_m_BGrid[15]
	0x0F12, 0x01E2,  //01B2 //#awbb_LowBrGrZones_m_BGrid[16]
	0x0F12, 0x02AA,  //0280 //#awbb_LowBrGrZones_m_BGrid[17]
	0x0F12, 0x01E6,  //01BD //#awbb_LowBrGrZones_m_BGrid[18]
	0x0F12, 0x0264,  //024E //#awbb_LowBrGrZones_m_BGrid[19]
	0x0F12, 0x0000,  //01E2 //#awbb_LowBrGrZones_m_BGrid[20]
	0x0F12, 0x0000,  //020C //#awbb_LowBrGrZones_m_BGrid[21]
	0x0F12, 0x0000,  //0000 //#awbb_LowBrGrZones_m_BGrid[22]
	0x0F12, 0x0000,  //0000 //#awbb_LowBrGrZones_m_BGrid[23]
	0x0F12, 0x0006,  //0006 //#awbb_LowBrGrZones_m_GridStep
	0x002A, 0x0D18,  //0D18
	0x0F12, 0x009A,  //00FE //#awbb_LowBrGrZones_m_Boffs
	
	// AWB ETC
	0x002A, 0x0D1C,
	0x0F12, 0x036C, //#awbb_CrclLowT_R_c
	0x002A, 0x0D20,
	0x0F12, 0x011D, //#awbb_CrclLowT_B_c
	0x002A, 0x0D24,
	0x0F12, 0x62B8, //#awbb_CrclLowT_Rad_c
	
	0x002A, 0x0D2C,  //0D2C
	0x0F12, 0x0135,  //0141 //#awbb_IntcR
	0x0F12, 0x012B,  //013B //#awbb_IntcB
	
	0x002A, 0x0D28,  //0D28
	0x0F12, 0x023B,  //0269 //#awbb_OutdoorWP_r
	0x0F12, 0x0274,  //0240 //#awbb_OutdoorWP_b
	
	0x002A, 0x0E4C,
	0x0F12, 0x0000, //#awbboost_useBoosting4Outdoor
	0x002A, 0x0D4C,  //0D4C
	0x0F12, 0x05C6,  //0187 //#awbb_GamutWidthThr1
	0x0F12, 0x02DF,  //00CF //#awbb_GamutHeightThr1
	0x0F12, 0x0030,  //000D //#awbb_GamutWidthThr2
	0x0F12, 0x0020,  //000A //#awbb_GamutHeightThr2
	
	0x002A, 0x0D5C,
	0x0F12, 0x7FFF, //#awbb_LowTempRB
	0x0F12, 0x0050, //#awbb_LowTemp_RBzone
	
	0x002A, 0x0D46, //0D46
	0x0F12, 0x0553, //0420//#awbb_MvEq_RBthresh
	
	0x002A, 0x0D4A,
	0x0F12, 0x000A, //#awbb_MovingScale10
	
	0x002A, 0x0E3E,
	0x0F12, 0x0000, //#awbb_rpl_InvalidOutdoor off
	//002A 22DE
	//0F12 0004 //#Mon_AWB_ByPassMode // [0]Outdoor [1]LowBr [2]LowTemp
	
	//002A 337C
	//0F12 00B3 //#Tune_TP_ChMoveToNearR
	//0F12 0040 //#Tune_TP_AvMoveToGamutDist
	
	// AWB initial point
	0x002A, 0x0E44,
	0x0F12, 0x053C, //#define awbb_GainsInit_0_
	0x0F12, 0x0400, //#define awbb_GainsInit_1_
	0x0F12, 0x055C, //#define awbb_GainsInit_2_
	// Set AWB global offset
	0x002A, 0x0E36,
	0x0F12, 0x0000, //#awbb_RGainOff
	0x0F12, 0x0000, //#awbb_BGainOff
	0x0F12, 0x0000, //#awbb_GGainOff
	
	//================================================================================================
	// SET GRID OFFSET
	//================================================================================================
	// Not used
	0x002A, 0x0E4A,
	0x0F12, 0x0000, // #awbb_GridEnable
	
	//================================================================================================
	// SET GAMMA
	//================================================================================================
	0x002A, 0x3288, 
	0x0F12, 0x0000, //#SARR_usDualGammaLutRGBIndoor_0__0_ 0x70003288
	0x0F12, 0x0004, //#SARR_usDualGammaLutRGBIndoor_0__1_ 0x7000328A
	0x0F12, 0x0010, //#SARR_usDualGammaLutRGBIndoor_0__2_ 0x7000328C
	0x0F12, 0x002A, //#SARR_usDualGammaLutRGBIndoor_0__3_ 0x7000328E
	0x0F12, 0x0062, //#SARR_usDualGammaLutRGBIndoor_0__4_ 0x70003290
	0x0F12, 0x00D5, //#SARR_usDualGammaLutRGBIndoor_0__5_ 0x70003292
	0x0F12, 0x0138, //#SARR_usDualGammaLutRGBIndoor_0__6_ 0x70003294
	0x0F12, 0x0161, //#SARR_usDualGammaLutRGBIndoor_0__7_ 0x70003296
	0x0F12, 0x0186, //#SARR_usDualGammaLutRGBIndoor_0__8_ 0x70003298
	0x0F12, 0x01BC, //#SARR_usDualGammaLutRGBIndoor_0__9_ 0x7000329A
	0x0F12, 0x01E8, //#SARR_usDualGammaLutRGBIndoor_0__10_ 0x7000329C
	0x0F12, 0x020F, //#SARR_usDualGammaLutRGBIndoor_0__11_ 0x7000329E
	0x0F12, 0x0232, //#SARR_usDualGammaLutRGBIndoor_0__12_ 0x700032A0
	0x0F12, 0x0273, //#SARR_usDualGammaLutRGBIndoor_0__13_ 0x700032A2
	0x0F12, 0x02AF, //#SARR_usDualGammaLutRGBIndoor_0__14_ 0x700032A4
	0x0F12, 0x0309, //#SARR_usDualGammaLutRGBIndoor_0__15_ 0x700032A6
	0x0F12, 0x0355, //#SARR_usDualGammaLutRGBIndoor_0__16_ 0x700032A8
	0x0F12, 0x0394, //#SARR_usDualGammaLutRGBIndoor_0__17_ 0x700032AA
	0x0F12, 0x03CE, //#SARR_usDualGammaLutRGBIndoor_0__18_ 0x700032AC
	0x0F12, 0x03FF, //#SARR_usDualGammaLutRGBIndoor_0__19_ 0x700032AE
	0x0F12, 0x0000, //#SARR_usDualGammaLutRGBIndoor_1__0_ 0x700032B0
	0x0F12, 0x0004, //#SARR_usDualGammaLutRGBIndoor_1__1_ 0x700032B2
	0x0F12, 0x0010, //#SARR_usDualGammaLutRGBIndoor_1__2_ 0x700032B4
	0x0F12, 0x002A, //#SARR_usDualGammaLutRGBIndoor_1__3_ 0x700032B6
	0x0F12, 0x0062, //#SARR_usDualGammaLutRGBIndoor_1__4_ 0x700032B8
	0x0F12, 0x00D5, //#SARR_usDualGammaLutRGBIndoor_1__5_ 0x700032BA
	0x0F12, 0x0138, //#SARR_usDualGammaLutRGBIndoor_1__6_ 0x700032BC
	0x0F12, 0x0161, //#SARR_usDualGammaLutRGBIndoor_1__7_ 0x700032BE
	0x0F12, 0x0186, //#SARR_usDualGammaLutRGBIndoor_1__8_ 0x700032C0
	0x0F12, 0x01BC, //#SARR_usDualGammaLutRGBIndoor_1__9_ 0x700032C2
	0x0F12, 0x01E8, //#SARR_usDualGammaLutRGBIndoor_1__10_ 0x700032C4
	0x0F12, 0x020F, //#SARR_usDualGammaLutRGBIndoor_1__11_ 0x700032C6
	0x0F12, 0x0232, //#SARR_usDualGammaLutRGBIndoor_1__12_ 0x700032C8
	0x0F12, 0x0273, //#SARR_usDualGammaLutRGBIndoor_1__13_ 0x700032CA
	0x0F12, 0x02AF, //#SARR_usDualGammaLutRGBIndoor_1__14_ 0x700032CC
	0x0F12, 0x0309, //#SARR_usDualGammaLutRGBIndoor_1__15_ 0x700032CE
	0x0F12, 0x0355, //#SARR_usDualGammaLutRGBIndoor_1__16_ 0x700032D0
	0x0F12, 0x0394, //#SARR_usDualGammaLutRGBIndoor_1__17_ 0x700032D2
	0x0F12, 0x03CE, //#SARR_usDualGammaLutRGBIndoor_1__18_ 0x700032D4
	0x0F12, 0x03FF, //#SARR_usDualGammaLutRGBIndoor_1__19_ 0x700032D6
	0x0F12, 0x0000, //#SARR_usDualGammaLutRGBIndoor_2__0_ 0x700032D8
	0x0F12, 0x0004, //#SARR_usDualGammaLutRGBIndoor_2__1_ 0x700032DA
	0x0F12, 0x0010, //#SARR_usDualGammaLutRGBIndoor_2__2_ 0x700032DC
	0x0F12, 0x002A, //#SARR_usDualGammaLutRGBIndoor_2__3_ 0x700032DE
	0x0F12, 0x0062, //#SARR_usDualGammaLutRGBIndoor_2__4_ 0x700032E0
	0x0F12, 0x00D5, //#SARR_usDualGammaLutRGBIndoor_2__5_ 0x700032E2
	0x0F12, 0x0138, //#SARR_usDualGammaLutRGBIndoor_2__6_ 0x700032E4
	0x0F12, 0x0161, //#SARR_usDualGammaLutRGBIndoor_2__7_ 0x700032E6
	0x0F12, 0x0186, //#SARR_usDualGammaLutRGBIndoor_2__8_ 0x700032E8
	0x0F12, 0x01BC, //#SARR_usDualGammaLutRGBIndoor_2__9_ 0x700032EA
	0x0F12, 0x01E8, //#SARR_usDualGammaLutRGBIndoor_2__10_ 0x700032EC
	0x0F12, 0x020F, //#SARR_usDualGammaLutRGBIndoor_2__11_ 0x700032EE
	0x0F12, 0x0232, //#SARR_usDualGammaLutRGBIndoor_2__12_ 0x700032F0
	0x0F12, 0x0273, //#SARR_usDualGammaLutRGBIndoor_2__13_ 0x700032F2
	0x0F12, 0x02AF, //#SARR_usDualGammaLutRGBIndoor_2__14_ 0x700032F4
	0x0F12, 0x0309, //#SARR_usDualGammaLutRGBIndoor_2__15_ 0x700032F6
	0x0F12, 0x0355, //#SARR_usDualGammaLutRGBIndoor_2__16_ 0x700032F8
	0x0F12, 0x0394, //#SARR_usDualGammaLutRGBIndoor_2__17_ 0x700032FA
	0x0F12, 0x03CE, //#SARR_usDualGammaLutRGBIndoor_2__18_ 0x700032FC
	0x0F12, 0x03FF, //#SARR_usDualGammaLutRGBIndoor_2__19_ 0x700032FE
	
	0x0F12, 0x0000, //#SARR_usDualGammaLutRGBOutdoor_0__0_ 0x70003300
	0x0F12, 0x0004, //#SARR_usDualGammaLutRGBOutdoor_0__1_ 0x70003302
	0x0F12, 0x0010, //#SARR_usDualGammaLutRGBOutdoor_0__2_ 0x70003304
	0x0F12, 0x002A, //#SARR_usDualGammaLutRGBOutdoor_0__3_ 0x70003306
	0x0F12, 0x0062, //#SARR_usDualGammaLutRGBOutdoor_0__4_ 0x70003308
	0x0F12, 0x00D5, //#SARR_usDualGammaLutRGBOutdoor_0__5_ 0x7000330A
	0x0F12, 0x0138, //#SARR_usDualGammaLutRGBOutdoor_0__6_ 0x7000330C
	0x0F12, 0x0161, //#SARR_usDualGammaLutRGBOutdoor_0__7_ 0x7000330E
	0x0F12, 0x0186, //#SARR_usDualGammaLutRGBOutdoor_0__8_ 0x70003310
	0x0F12, 0x01BC, //#SARR_usDualGammaLutRGBOutdoor_0__9_ 0x70003312
	0x0F12, 0x01E8, //#SARR_usDualGammaLutRGBOutdoor_0__10_0x70003314
	0x0F12, 0x020F, //#SARR_usDualGammaLutRGBOutdoor_0__11_0x70003316
	0x0F12, 0x0232, //#SARR_usDualGammaLutRGBOutdoor_0__12_0x70003318
	0x0F12, 0x0273, //#SARR_usDualGammaLutRGBOutdoor_0__13_0x7000331A
	0x0F12, 0x02AF, //#SARR_usDualGammaLutRGBOutdoor_0__14_0x7000331C
	0x0F12, 0x0309, //#SARR_usDualGammaLutRGBOutdoor_0__15_0x7000331E
	0x0F12, 0x0355, //#SARR_usDualGammaLutRGBOutdoor_0__16_0x70003320
	0x0F12, 0x0394, //#SARR_usDualGammaLutRGBOutdoor_0__17_0x70003322
	0x0F12, 0x03CE, //#SARR_usDualGammaLutRGBOutdoor_0__18_0x70003324
	0x0F12, 0x03FF, //#SARR_usDualGammaLutRGBOutdoor_0__19_0x70003326
	0x0F12, 0x0000, //#SARR_usDualGammaLutRGBOutdoor_1__0_ 0x70003328
	0x0F12, 0x0004, //#SARR_usDualGammaLutRGBOutdoor_1__1_ 0x7000332A
	0x0F12, 0x0010, //#SARR_usDualGammaLutRGBOutdoor_1__2_ 0x7000332C
	0x0F12, 0x002A, //#SARR_usDualGammaLutRGBOutdoor_1__3_ 0x7000332E
	0x0F12, 0x0062, //#SARR_usDualGammaLutRGBOutdoor_1__4_ 0x70003330
	0x0F12, 0x00D5, //#SARR_usDualGammaLutRGBOutdoor_1__5_ 0x70003332
	0x0F12, 0x0138, //#SARR_usDualGammaLutRGBOutdoor_1__6_ 0x70003334
	0x0F12, 0x0161, //#SARR_usDualGammaLutRGBOutdoor_1__7_ 0x70003336
	0x0F12, 0x0186, //#SARR_usDualGammaLutRGBOutdoor_1__8_ 0x70003338
	0x0F12, 0x01BC, //#SARR_usDualGammaLutRGBOutdoor_1__9_ 0x7000333A
	0x0F12, 0x01E8, //#SARR_usDualGammaLutRGBOutdoor_1__10_0x7000333C
	0x0F12, 0x020F, //#SARR_usDualGammaLutRGBOutdoor_1__11_0x7000333E
	0x0F12, 0x0232, //#SARR_usDualGammaLutRGBOutdoor_1__12_0x70003340
	0x0F12, 0x0273, //#SARR_usDualGammaLutRGBOutdoor_1__13_0x70003342
	0x0F12, 0x02AF, //#SARR_usDualGammaLutRGBOutdoor_1__14_0x70003344
	0x0F12, 0x0309, //#SARR_usDualGammaLutRGBOutdoor_1__15_0x70003346
	0x0F12, 0x0355, //#SARR_usDualGammaLutRGBOutdoor_1__16_0x70003348
	0x0F12, 0x0394, //#SARR_usDualGammaLutRGBOutdoor_1__17_0x7000334A
	0x0F12, 0x03CE, //#SARR_usDualGammaLutRGBOutdoor_1__18_0x7000334C
	0x0F12, 0x03FF, //#SARR_usDualGammaLutRGBOutdoor_1__19_0x7000334E
	0x0F12, 0x0000, //#SARR_usDualGammaLutRGBOutdoor_2__0_ 0x70003350
	0x0F12, 0x0004, //#SARR_usDualGammaLutRGBOutdoor_2__1_ 0x70003352
	0x0F12, 0x0010, //#SARR_usDualGammaLutRGBOutdoor_2__2_ 0x70003354
	0x0F12, 0x002A, //#SARR_usDualGammaLutRGBOutdoor_2__3_ 0x70003356
	0x0F12, 0x0062, //#SARR_usDualGammaLutRGBOutdoor_2__4_ 0x70003358
	0x0F12, 0x00D5, //#SARR_usDualGammaLutRGBOutdoor_2__5_ 0x7000335A
	0x0F12, 0x0138, //#SARR_usDualGammaLutRGBOutdoor_2__6_ 0x7000335C
	0x0F12, 0x0161, //#SARR_usDualGammaLutRGBOutdoor_2__7_ 0x7000335E
	0x0F12, 0x0186, //#SARR_usDualGammaLutRGBOutdoor_2__8_ 0x70003360
	0x0F12, 0x01BC, //#SARR_usDualGammaLutRGBOutdoor_2__9_ 0x70003362
	0x0F12, 0x01E8, //#SARR_usDualGammaLutRGBOutdoor_2__10_0x70003364
	0x0F12, 0x020F, //#SARR_usDualGammaLutRGBOutdoor_2__11_0x70003366
	0x0F12, 0x0232, //#SARR_usDualGammaLutRGBOutdoor_2__12_0x70003368
	0x0F12, 0x0273, //#SARR_usDualGammaLutRGBOutdoor_2__13_0x7000336A
	0x0F12, 0x02AF, //#SARR_usDualGammaLutRGBOutdoor_2__14_0x7000336C
	0x0F12, 0x0309, //#SARR_usDualGammaLutRGBOutdoor_2__15_0x7000336E
	0x0F12, 0x0355, //#SARR_usDualGammaLutRGBOutdoor_2__16_0x70003370
	0x0F12, 0x0394, //#SARR_usDualGammaLutRGBOutdoor_2__17_0x70003372
	0x0F12, 0x03CE, //#SARR_usDualGammaLutRGBOutdoor_2__18_0x70003374
	0x0F12, 0x03FF, //#SARR_usDualGammaLutRGBOutdoor_2__19_0x70003376 
};
static unsigned short s5k5cagx_init_reg_short7[]={		

	//================================================================================================
	// SET AFIT
	//================================================================================================
	// Noise index
	0x0F12, 0x7000,
	0x002A, 0x0764,
	0x0F12, 0x0041, //#afit_uNoiseIndInDoor[0] // 65
	0x0F12, 0x0063, //#afit_uNoiseIndInDoor[1] // 99
	0x0F12, 0x00C8, //#afit_uNoiseIndInDoor[2] // 187
	0x0F12, 0x01F4, //#afit_uNoiseIndInDoor[3] // 403->380
	0x0F12, 0x028A, //#afit_uNoiseIndInDoor[4] // 700
	// AFIT table start address // 7000_07C4
	0x002A, 0x0770,
	0x0F12, 0x07C4,
	0x0F12, 0x7000,
	// AFIT table (Variables)
	0x002A, 0x07C4,
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[0]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[1]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[2]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[3]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[4]
	0x0F12, 0x00C4, //#TVAR_afit_pBaseVals[5]
	0x0F12, 0x03FF, //#TVAR_afit_pBaseVals[6]
	0x0F12, 0x009C, //#TVAR_afit_pBaseVals[7]
	0x0F12, 0x017C, //#TVAR_afit_pBaseVals[8]
	0x0F12, 0x03FF, //#TVAR_afit_pBaseVals[9]
	0x0F12, 0x000C, //#TVAR_afit_pBaseVals[10]
	0x0F12, 0x0010, //#TVAR_afit_pBaseVals[11]
	0x0F12, 0x012C, //#TVAR_afit_pBaseVals[12]
	0x0F12, 0x03E8, //#TVAR_afit_pBaseVals[13]
	0x0F12, 0x0046, //#TVAR_afit_pBaseVals[14]
	0x0F12, 0x005A, //#TVAR_afit_pBaseVals[15]
	0x0F12, 0x0070, //#TVAR_afit_pBaseVals[16]
	0x0F12, 0x0019, //#TVAR_afit_pBaseVals[17]
	0x0F12, 0x0019, //#TVAR_afit_pBaseVals[18]
	0x0F12, 0x01AA, //#TVAR_afit_pBaseVals[19]
	0x0F12, 0x0064, //#TVAR_afit_pBaseVals[20]
	0x0F12, 0x0064, //#TVAR_afit_pBaseVals[21]
	0x0F12, 0x000A, //#TVAR_afit_pBaseVals[22]
	0x0F12, 0x000A, //#TVAR_afit_pBaseVals[23]
	0x0F12, 0x0096, //#TVAR_afit_pBaseVals[24]
	0x0F12, 0x0019, //#TVAR_afit_pBaseVals[25]
	0x0F12, 0x002A, //#TVAR_afit_pBaseVals[26]
	0x0F12, 0x0024, //#TVAR_afit_pBaseVals[27]
	0x0F12, 0x002A, //#TVAR_afit_pBaseVals[28]
	0x0F12, 0x0024, //#TVAR_afit_pBaseVals[29]
	0x0F12, 0x0A24, //#TVAR_afit_pBaseVals[30]
	0x0F12, 0x1701, //#TVAR_afit_pBaseVals[31]
	0x0F12, 0x0229, //#TVAR_afit_pBaseVals[32]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[33]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[34]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[35]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[36]
	0x0F12, 0x00FF, //#TVAR_afit_pBaseVals[37]
	0x0F12, 0x043B, //#TVAR_afit_pBaseVals[38]
	0x0F12, 0x1414, //#TVAR_afit_pBaseVals[39]
	0x0F12, 0x0301, //#TVAR_afit_pBaseVals[40]
	0x0F12, 0xFF07, //#TVAR_afit_pBaseVals[41]
	0x0F12, 0x051E, //#TVAR_afit_pBaseVals[42]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[43]
	0x0F12, 0x0F0F, //#TVAR_afit_pBaseVals[44]
	0x0F12, 0x0A05, //#TVAR_afit_pBaseVals[45]
	0x0F12, 0x143C, //#TVAR_afit_pBaseVals[46]
	0x0F12, 0x0A14, //#TVAR_afit_pBaseVals[47]
	0x0F12, 0x0002, //#TVAR_afit_pBaseVals[48]
	0x0F12, 0x00FF, //#TVAR_afit_pBaseVals[49]
	0x0F12, 0x1102, //#TVAR_afit_pBaseVals[50]
	0x0F12, 0x001B, //#TVAR_afit_pBaseVals[51]
	0x0F12, 0x0900, //#TVAR_afit_pBaseVals[52]
	0x0F12, 0x0600, //#TVAR_afit_pBaseVals[53]
	0x0F12, 0x0504, //#TVAR_afit_pBaseVals[54]
	0x0F12, 0x0305, //#TVAR_afit_pBaseVals[55]
	0x0F12, 0x3C03, //#TVAR_afit_pBaseVals[56]
	0x0F12, 0x006E, //#TVAR_afit_pBaseVals[57]
	0x0F12, 0x0078, //#TVAR_afit_pBaseVals[58]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[59]
	0x0F12, 0x1414, //#TVAR_afit_pBaseVals[60]
	0x0F12, 0x0101, //#TVAR_afit_pBaseVals[61]
	0x0F12, 0x5002, //#TVAR_afit_pBaseVals[62]
	0x0F12, 0x7850, //#TVAR_afit_pBaseVals[63]
	0x0F12, 0x2878, //#TVAR_afit_pBaseVals[64]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[65]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[66]
	0x0F12, 0x1E0C, //#TVAR_afit_pBaseVals[67]
	0x0F12, 0x070A, //#TVAR_afit_pBaseVals[68]
	0x0F12, 0x32FF, //#TVAR_afit_pBaseVals[69]
	0x0F12, 0x4104, //#TVAR_afit_pBaseVals[70]
	0x0F12, 0x123C, //#TVAR_afit_pBaseVals[71]
	0x0F12, 0x4012, //#TVAR_afit_pBaseVals[72]
	0x0F12, 0x0204, //#TVAR_afit_pBaseVals[73]
	0x0F12, 0x1E03, //#TVAR_afit_pBaseVals[74]
	0x0F12, 0x011E, //#TVAR_afit_pBaseVals[75]
	0x0F12, 0x0201, //#TVAR_afit_pBaseVals[76]
	0x0F12, 0x5050, //#TVAR_afit_pBaseVals[77]
	0x0F12, 0x3C3C, //#TVAR_afit_pBaseVals[78]
	0x0F12, 0x0028, //#TVAR_afit_pBaseVals[79]
	0x0F12, 0x030A, //#TVAR_afit_pBaseVals[80]
	0x0F12, 0x0714, //#TVAR_afit_pBaseVals[81]
	0x0F12, 0x0A1E, //#TVAR_afit_pBaseVals[82]
	0x0F12, 0xFF07, //#TVAR_afit_pBaseVals[83]
	0x0F12, 0x0432, //#TVAR_afit_pBaseVals[84]
	0x0F12, 0x4050, //#TVAR_afit_pBaseVals[85]
	0x0F12, 0x0F0F, //#TVAR_afit_pBaseVals[86]
	0x0F12, 0x0440, //#TVAR_afit_pBaseVals[87]
	0x0F12, 0x0302, //#TVAR_afit_pBaseVals[88]
	0x0F12, 0x1E1E, //#TVAR_afit_pBaseVals[89]
	0x0F12, 0x0101, //#TVAR_afit_pBaseVals[90]
	0x0F12, 0x5002, //#TVAR_afit_pBaseVals[91]
	0x0F12, 0x3C50, //#TVAR_afit_pBaseVals[92]
	0x0F12, 0x283C, //#TVAR_afit_pBaseVals[93]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[94]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[95]
	0x0F12, 0x1E07, //#TVAR_afit_pBaseVals[96]
	0x0F12, 0x070A, //#TVAR_afit_pBaseVals[97]
	0x0F12, 0x32FF, //#TVAR_afit_pBaseVals[98]
	0x0F12, 0x5004, //#TVAR_afit_pBaseVals[99]
	0x0F12, 0x0F40, //#TVAR_afit_pBaseVals[100]
	0x0F12, 0x400F, //#TVAR_afit_pBaseVals[101]
	0x0F12, 0x0204, //#TVAR_afit_pBaseVals[102]
	0x0F12, 0x0003, //#TVAR_afit_pBaseVals[103]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[104]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[105]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[106]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[107]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[108]
	0x0F12, 0x00C4, //#TVAR_afit_pBaseVals[109]
	0x0F12, 0x03FF, //#TVAR_afit_pBaseVals[110]
	0x0F12, 0x009C, //#TVAR_afit_pBaseVals[111]
	0x0F12, 0x017C, //#TVAR_afit_pBaseVals[112]
	0x0F12, 0x03FF, //#TVAR_afit_pBaseVals[113]
	0x0F12, 0x000C, //#TVAR_afit_pBaseVals[114]
	0x0F12, 0x0010, //#TVAR_afit_pBaseVals[115]
	0x0F12, 0x012C, //#TVAR_afit_pBaseVals[116]
	0x0F12, 0x03E8, //#TVAR_afit_pBaseVals[117]
	0x0F12, 0x0046, //#TVAR_afit_pBaseVals[118]
	0x0F12, 0x005A, //#TVAR_afit_pBaseVals[119]
	0x0F12, 0x0070, //#TVAR_afit_pBaseVals[120]
	0x0F12, 0x000F, //#TVAR_afit_pBaseVals[121]
	0x0F12, 0x000F, //#TVAR_afit_pBaseVals[122]
	0x0F12, 0x01AA, //#TVAR_afit_pBaseVals[123]
	0x0F12, 0x003C, //#TVAR_afit_pBaseVals[124]
	0x0F12, 0x003C, //#TVAR_afit_pBaseVals[125]
	0x0F12, 0x0005, //#TVAR_afit_pBaseVals[126]
	0x0F12, 0x0005, //#TVAR_afit_pBaseVals[127]
	0x0F12, 0x0096, //#TVAR_afit_pBaseVals[128]
	0x0F12, 0x0023, //#TVAR_afit_pBaseVals[129]
	0x0F12, 0x002A, //#TVAR_afit_pBaseVals[130]
	0x0F12, 0x0024, //#TVAR_afit_pBaseVals[131]
	0x0F12, 0x002A, //#TVAR_afit_pBaseVals[132]
	0x0F12, 0x0024, //#TVAR_afit_pBaseVals[133]
	0x0F12, 0x0A24, //#TVAR_afit_pBaseVals[134]
	0x0F12, 0x1701, //#TVAR_afit_pBaseVals[135]
	0x0F12, 0x0229, //#TVAR_afit_pBaseVals[136]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[137]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[138]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[139]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[140]
	0x0F12, 0x00FF, //#TVAR_afit_pBaseVals[141]
	0x0F12, 0x043B, //#TVAR_afit_pBaseVals[142]
	0x0F12, 0x1414, //#TVAR_afit_pBaseVals[143]
	0x0F12, 0x0301, //#TVAR_afit_pBaseVals[144]
	0x0F12, 0xFF07, //#TVAR_afit_pBaseVals[145]
	0x0F12, 0x051E, //#TVAR_afit_pBaseVals[146]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[147]
	0x0F12, 0x0F0F, //#TVAR_afit_pBaseVals[148]
	0x0F12, 0x0A05, //#TVAR_afit_pBaseVals[149]
	0x0F12, 0x143C, //#TVAR_afit_pBaseVals[150]
	0x0F12, 0x0A14, //#TVAR_afit_pBaseVals[151]
	0x0F12, 0x0002, //#TVAR_afit_pBaseVals[152]
	0x0F12, 0x00FF, //#TVAR_afit_pBaseVals[153]
	0x0F12, 0x1102, //#TVAR_afit_pBaseVals[154]
	0x0F12, 0x001B, //#TVAR_afit_pBaseVals[155]
	0x0F12, 0x0900, //#TVAR_afit_pBaseVals[156]
	0x0F12, 0x0600, //#TVAR_afit_pBaseVals[157]
	0x0F12, 0x0504, //#TVAR_afit_pBaseVals[158]
	0x0F12, 0x0305, //#TVAR_afit_pBaseVals[159]
	0x0F12, 0x4603, //#TVAR_afit_pBaseVals[160]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[161]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[162]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[163]
	0x0F12, 0x1919, //#TVAR_afit_pBaseVals[164]
	0x0F12, 0x0101, //#TVAR_afit_pBaseVals[165]
	0x0F12, 0x3C02, //#TVAR_afit_pBaseVals[166]
	0x0F12, 0x643C, //#TVAR_afit_pBaseVals[167]
	0x0F12, 0x2864, //#TVAR_afit_pBaseVals[168]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[169]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[170]
	0x0F12, 0x1E0C, //#TVAR_afit_pBaseVals[171]
	0x0F12, 0x070A, //#TVAR_afit_pBaseVals[172]
	0x0F12, 0x32FF, //#TVAR_afit_pBaseVals[173]
	0x0F12, 0x4104, //#TVAR_afit_pBaseVals[174]
	0x0F12, 0x123C, //#TVAR_afit_pBaseVals[175]
	0x0F12, 0x4012, //#TVAR_afit_pBaseVals[176]
	0x0F12, 0x0204, //#TVAR_afit_pBaseVals[177]
	0x0F12, 0x1E03, //#TVAR_afit_pBaseVals[178]
	0x0F12, 0x011E, //#TVAR_afit_pBaseVals[179]
	0x0F12, 0x0201, //#TVAR_afit_pBaseVals[180]
	0x0F12, 0x3232, //#TVAR_afit_pBaseVals[181]
	0x0F12, 0x3C3C, //#TVAR_afit_pBaseVals[182]
	0x0F12, 0x0028, //#TVAR_afit_pBaseVals[183]
	0x0F12, 0x030A, //#TVAR_afit_pBaseVals[184]
	0x0F12, 0x0714, //#TVAR_afit_pBaseVals[185]
	0x0F12, 0x0A1E, //#TVAR_afit_pBaseVals[186]
	0x0F12, 0xFF07, //#TVAR_afit_pBaseVals[187]
	0x0F12, 0x0432, //#TVAR_afit_pBaseVals[188]
	0x0F12, 0x4050, //#TVAR_afit_pBaseVals[189]
	0x0F12, 0x0F0F, //#TVAR_afit_pBaseVals[190]
	0x0F12, 0x0440, //#TVAR_afit_pBaseVals[191]
	0x0F12, 0x0302, //#TVAR_afit_pBaseVals[192]
	0x0F12, 0x1E1E, //#TVAR_afit_pBaseVals[193]
	0x0F12, 0x0101, //#TVAR_afit_pBaseVals[194]
	0x0F12, 0x3202, //#TVAR_afit_pBaseVals[195]
	0x0F12, 0x3C32, //#TVAR_afit_pBaseVals[196]
	0x0F12, 0x283C, //#TVAR_afit_pBaseVals[197]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[198]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[199]
	0x0F12, 0x1E07, //#TVAR_afit_pBaseVals[200]
	0x0F12, 0x070A, //#TVAR_afit_pBaseVals[201]
	0x0F12, 0x32FF, //#TVAR_afit_pBaseVals[202]
	0x0F12, 0x5004, //#TVAR_afit_pBaseVals[203]
	0x0F12, 0x0F40, //#TVAR_afit_pBaseVals[204]
	0x0F12, 0x400F, //#TVAR_afit_pBaseVals[205]
	0x0F12, 0x0204, //#TVAR_afit_pBaseVals[206]
	0x0F12, 0x0003, //#TVAR_afit_pBaseVals[207]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[208]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[209]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[210]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[211]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[212]
	0x0F12, 0x00C4, //#TVAR_afit_pBaseVals[213]
	0x0F12, 0x03FF, //#TVAR_afit_pBaseVals[214]
	0x0F12, 0x009C, //#TVAR_afit_pBaseVals[215]
	0x0F12, 0x017C, //#TVAR_afit_pBaseVals[216]
	0x0F12, 0x03FF, //#TVAR_afit_pBaseVals[217]
	0x0F12, 0x000C, //#TVAR_afit_pBaseVals[218]
	0x0F12, 0x0010, //#TVAR_afit_pBaseVals[219]
	0x0F12, 0x012C, //#TVAR_afit_pBaseVals[220]
	0x0F12, 0x03E8, //#TVAR_afit_pBaseVals[221]
	0x0F12, 0x0046, //#TVAR_afit_pBaseVals[222]
	0x0F12, 0x0078, //#TVAR_afit_pBaseVals[223]
	0x0F12, 0x0070, //#TVAR_afit_pBaseVals[224]
	0x0F12, 0x0004, //#TVAR_afit_pBaseVals[225]
	0x0F12, 0x0004, //#TVAR_afit_pBaseVals[226]
	0x0F12, 0x01AA, //#TVAR_afit_pBaseVals[227]
	0x0F12, 0x001E, //#TVAR_afit_pBaseVals[228]
	0x0F12, 0x001E, //#TVAR_afit_pBaseVals[229]
	0x0F12, 0x0005, //#TVAR_afit_pBaseVals[230]
	0x0F12, 0x0005, //#TVAR_afit_pBaseVals[231]
	0x0F12, 0x0096, //#TVAR_afit_pBaseVals[232]
	0x0F12, 0x0023, //#TVAR_afit_pBaseVals[233]
	0x0F12, 0x002A, //#TVAR_afit_pBaseVals[234]
	0x0F12, 0x0024, //#TVAR_afit_pBaseVals[235]
	0x0F12, 0x002A, //#TVAR_afit_pBaseVals[236]
	0x0F12, 0x0024, //#TVAR_afit_pBaseVals[237]
	0x0F12, 0x0A24, //#TVAR_afit_pBaseVals[238]
	0x0F12, 0x1701, //#TVAR_afit_pBaseVals[239]
	0x0F12, 0x0229, //#TVAR_afit_pBaseVals[240]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[241]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[242]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[243]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[244]
	0x0F12, 0x00FF, //#TVAR_afit_pBaseVals[245]
	0x0F12, 0x043B, //#TVAR_afit_pBaseVals[246]
	0x0F12, 0x1414, //#TVAR_afit_pBaseVals[247]
	0x0F12, 0x0301, //#TVAR_afit_pBaseVals[248]
	0x0F12, 0xFF07, //#TVAR_afit_pBaseVals[249]
	0x0F12, 0x051E, //#TVAR_afit_pBaseVals[250]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[251]
	0x0F12, 0x0F0F, //#TVAR_afit_pBaseVals[252]
	0x0F12, 0x0A05, //#TVAR_afit_pBaseVals[253]
	0x0F12, 0x143C, //#TVAR_afit_pBaseVals[254]
	0x0F12, 0x0514, //#TVAR_afit_pBaseVals[255]
	0x0F12, 0x0002, //#TVAR_afit_pBaseVals[256]
	0x0F12, 0x00FF, //#TVAR_afit_pBaseVals[257]
	0x0F12, 0x1102, //#TVAR_afit_pBaseVals[258]
	0x0F12, 0x001B, //#TVAR_afit_pBaseVals[259]
	0x0F12, 0x0900, //#TVAR_afit_pBaseVals[260]
	0x0F12, 0x0600, //#TVAR_afit_pBaseVals[261]
	0x0F12, 0x0504, //#TVAR_afit_pBaseVals[262]
	0x0F12, 0x0305, //#TVAR_afit_pBaseVals[263]
	0x0F12, 0x4603, //#TVAR_afit_pBaseVals[264]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[265]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[266]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[267]
	0x0F12, 0x2323, //#TVAR_afit_pBaseVals[268]
	0x0F12, 0x0101, //#TVAR_afit_pBaseVals[269]
	0x0F12, 0x2A02, //#TVAR_afit_pBaseVals[270]
	0x0F12, 0x462A, //#TVAR_afit_pBaseVals[271]
	0x0F12, 0x2846, //#TVAR_afit_pBaseVals[272]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[273]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[274]
	0x0F12, 0x1E0C, //#TVAR_afit_pBaseVals[275]
	0x0F12, 0x070A, //#TVAR_afit_pBaseVals[276]
	0x0F12, 0x32FF, //#TVAR_afit_pBaseVals[277]
	0x0F12, 0x4B04, //#TVAR_afit_pBaseVals[278]
	0x0F12, 0x0F40, //#TVAR_afit_pBaseVals[279]
	0x0F12, 0x400F, //#TVAR_afit_pBaseVals[280]
	0x0F12, 0x0204, //#TVAR_afit_pBaseVals[281]
	0x0F12, 0x2303, //#TVAR_afit_pBaseVals[282]
	0x0F12, 0x0123, //#TVAR_afit_pBaseVals[283]
	0x0F12, 0x0201, //#TVAR_afit_pBaseVals[284]
	0x0F12, 0x262A, //#TVAR_afit_pBaseVals[285]
	0x0F12, 0x2C2C, //#TVAR_afit_pBaseVals[286]
	0x0F12, 0x0028, //#TVAR_afit_pBaseVals[287]
	0x0F12, 0x030A, //#TVAR_afit_pBaseVals[288]
	0x0F12, 0x0714, //#TVAR_afit_pBaseVals[289]
	0x0F12, 0x0A1E, //#TVAR_afit_pBaseVals[290]
	0x0F12, 0xFF07, //#TVAR_afit_pBaseVals[291]
	0x0F12, 0x0432, //#TVAR_afit_pBaseVals[292]
	0x0F12, 0x4050, //#TVAR_afit_pBaseVals[293]
	0x0F12, 0x0F0F, //#TVAR_afit_pBaseVals[294]
	0x0F12, 0x0440, //#TVAR_afit_pBaseVals[295]
	0x0F12, 0x0302, //#TVAR_afit_pBaseVals[296]
	0x0F12, 0x2323, //#TVAR_afit_pBaseVals[297]
	0x0F12, 0x0101, //#TVAR_afit_pBaseVals[298]
	0x0F12, 0x2A02, //#TVAR_afit_pBaseVals[299]
	0x0F12, 0x2C26, //#TVAR_afit_pBaseVals[300]
	0x0F12, 0x282C, //#TVAR_afit_pBaseVals[301]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[302]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[303]
	0x0F12, 0x1E07, //#TVAR_afit_pBaseVals[304]
	0x0F12, 0x070A, //#TVAR_afit_pBaseVals[305]
	0x0F12, 0x32FF, //#TVAR_afit_pBaseVals[306]
	0x0F12, 0x5004, //#TVAR_afit_pBaseVals[307]
	0x0F12, 0x0F40, //#TVAR_afit_pBaseVals[308]
	0x0F12, 0x400F, //#TVAR_afit_pBaseVals[309]
	0x0F12, 0x0204, //#TVAR_afit_pBaseVals[310]
	0x0F12, 0x0003, //#TVAR_afit_pBaseVals[311]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[312]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[313]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[314]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[315]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[316]
	0x0F12, 0x00C4, //#TVAR_afit_pBaseVals[317]
	0x0F12, 0x03FF, //#TVAR_afit_pBaseVals[318]
	0x0F12, 0x009C, //#TVAR_afit_pBaseVals[319]
	0x0F12, 0x017C, //#TVAR_afit_pBaseVals[320]
	0x0F12, 0x03FF, //#TVAR_afit_pBaseVals[321]
	0x0F12, 0x000C, //#TVAR_afit_pBaseVals[322]
	0x0F12, 0x0010, //#TVAR_afit_pBaseVals[323]
	0x0F12, 0x00C8, //#TVAR_afit_pBaseVals[324]
	0x0F12, 0x0384, //#TVAR_afit_pBaseVals[325]
	0x0F12, 0x0046, //#TVAR_afit_pBaseVals[326]
	0x0F12, 0x0082, //#TVAR_afit_pBaseVals[327]
	0x0F12, 0x0070, //#TVAR_afit_pBaseVals[328]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[329]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[330]
	0x0F12, 0x01AA, //#TVAR_afit_pBaseVals[331]
	0x0F12, 0x001E, //#TVAR_afit_pBaseVals[332]
	0x0F12, 0x001E, //#TVAR_afit_pBaseVals[333]
	0x0F12, 0x000A, //#TVAR_afit_pBaseVals[334]
	0x0F12, 0x000A, //#TVAR_afit_pBaseVals[335]
	0x0F12, 0x010E, //#TVAR_afit_pBaseVals[336]
	0x0F12, 0x0028, //#TVAR_afit_pBaseVals[337]
	0x0F12, 0x0032, //#TVAR_afit_pBaseVals[338]
	0x0F12, 0x0028, //#TVAR_afit_pBaseVals[339]
	0x0F12, 0x0032, //#TVAR_afit_pBaseVals[340]
	0x0F12, 0x0028, //#TVAR_afit_pBaseVals[341]
	0x0F12, 0x0A24, //#TVAR_afit_pBaseVals[342]
	0x0F12, 0x1701, //#TVAR_afit_pBaseVals[343]
	0x0F12, 0x0229, //#TVAR_afit_pBaseVals[344]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[345]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[346]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[347]
	0x0F12, 0x0504, //#TVAR_afit_pBaseVals[348]
	0x0F12, 0x00FF, //#TVAR_afit_pBaseVals[349]
	0x0F12, 0x043B, //#TVAR_afit_pBaseVals[350]
	0x0F12, 0x1414, //#TVAR_afit_pBaseVals[351]
	0x0F12, 0x0301, //#TVAR_afit_pBaseVals[352]
	0x0F12, 0xFF07, //#TVAR_afit_pBaseVals[353]
	0x0F12, 0x051E, //#TVAR_afit_pBaseVals[354]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[355]
	0x0F12, 0x0F0F, //#TVAR_afit_pBaseVals[356]
	0x0F12, 0x0A05, //#TVAR_afit_pBaseVals[357]
	0x0F12, 0x143C, //#TVAR_afit_pBaseVals[358]
	0x0F12, 0x0514, //#TVAR_afit_pBaseVals[359]
	0x0F12, 0x0002, //#TVAR_afit_pBaseVals[360]
	0x0F12, 0x00FF, //#TVAR_afit_pBaseVals[361]
	0x0F12, 0x1002, //#TVAR_afit_pBaseVals[362]
	0x0F12, 0x001E, //#TVAR_afit_pBaseVals[363]
	0x0F12, 0x0900, //#TVAR_afit_pBaseVals[364]
	0x0F12, 0x0600, //#TVAR_afit_pBaseVals[365]
	0x0F12, 0x0504, //#TVAR_afit_pBaseVals[366]
	0x0F12, 0x0305, //#TVAR_afit_pBaseVals[367]
	0x0F12, 0x4602, //#TVAR_afit_pBaseVals[368]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[369]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[370]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[371]
	0x0F12, 0x2328, //#TVAR_afit_pBaseVals[372]
	0x0F12, 0x0101, //#TVAR_afit_pBaseVals[373]
	0x0F12, 0x2A02, //#TVAR_afit_pBaseVals[374]
	0x0F12, 0x2228, //#TVAR_afit_pBaseVals[375]
	0x0F12, 0x2822, //#TVAR_afit_pBaseVals[376]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[377]
	0x0F12, 0x1903, //#TVAR_afit_pBaseVals[378]
	0x0F12, 0x1E0F, //#TVAR_afit_pBaseVals[379]
	0x0F12, 0x070A, //#TVAR_afit_pBaseVals[380]
	0x0F12, 0x32FF, //#TVAR_afit_pBaseVals[381]
	0x0F12, 0x9604, //#TVAR_afit_pBaseVals[382]
	0x0F12, 0x0F42, //#TVAR_afit_pBaseVals[383]
	0x0F12, 0x400F, //#TVAR_afit_pBaseVals[384]
	0x0F12, 0x0504, //#TVAR_afit_pBaseVals[385]
	0x0F12, 0x2805, //#TVAR_afit_pBaseVals[386]
	0x0F12, 0x0123, //#TVAR_afit_pBaseVals[387]
	0x0F12, 0x0201, //#TVAR_afit_pBaseVals[388]
	0x0F12, 0x2024, //#TVAR_afit_pBaseVals[389]
	0x0F12, 0x1C1C, //#TVAR_afit_pBaseVals[390]
	0x0F12, 0x0028, //#TVAR_afit_pBaseVals[391]
	0x0F12, 0x030A, //#TVAR_afit_pBaseVals[392]
	0x0F12, 0x0A0A, //#TVAR_afit_pBaseVals[393]
	0x0F12, 0x0A2D, //#TVAR_afit_pBaseVals[394]
	0x0F12, 0xFF07, //#TVAR_afit_pBaseVals[395]
	0x0F12, 0x0432, //#TVAR_afit_pBaseVals[396]
	0x0F12, 0x4050, //#TVAR_afit_pBaseVals[397]
	0x0F12, 0x0F0F, //#TVAR_afit_pBaseVals[398]
	0x0F12, 0x0440, //#TVAR_afit_pBaseVals[399]
	0x0F12, 0x0302, //#TVAR_afit_pBaseVals[400]
	0x0F12, 0x2328, //#TVAR_afit_pBaseVals[401]
	0x0F12, 0x0101, //#TVAR_afit_pBaseVals[402]
	0x0F12, 0x3C02, //#TVAR_afit_pBaseVals[403]
	0x0F12, 0x1C3C, //#TVAR_afit_pBaseVals[404]
	0x0F12, 0x281C, //#TVAR_afit_pBaseVals[405]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[406]
	0x0F12, 0x0A03, //#TVAR_afit_pBaseVals[407]
	0x0F12, 0x2D0A, //#TVAR_afit_pBaseVals[408]
	0x0F12, 0x070A, //#TVAR_afit_pBaseVals[409]
	0x0F12, 0x32FF, //#TVAR_afit_pBaseVals[410]
	0x0F12, 0x5004, //#TVAR_afit_pBaseVals[411]
	0x0F12, 0x0F40, //#TVAR_afit_pBaseVals[412]
	0x0F12, 0x400F, //#TVAR_afit_pBaseVals[413]
	0x0F12, 0x0204, //#TVAR_afit_pBaseVals[414]
	0x0F12, 0x0003, //#TVAR_afit_pBaseVals[415]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[416]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[417]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[418]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[419]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[420]
	0x0F12, 0x00C4, //#TVAR_afit_pBaseVals[421]
	0x0F12, 0x03FF, //#TVAR_afit_pBaseVals[422]
	0x0F12, 0x009C, //#TVAR_afit_pBaseVals[423]
	0x0F12, 0x017C, //#TVAR_afit_pBaseVals[424]
	0x0F12, 0x03FF, //#TVAR_afit_pBaseVals[425]
	0x0F12, 0x000C, //#TVAR_afit_pBaseVals[426]
	0x0F12, 0x0010, //#TVAR_afit_pBaseVals[427]
	0x0F12, 0x00C8, //#TVAR_afit_pBaseVals[428]
	0x0F12, 0x0320, //#TVAR_afit_pBaseVals[429]
	0x0F12, 0x0046, //#TVAR_afit_pBaseVals[430]
	0x0F12, 0x015E, //#TVAR_afit_pBaseVals[431]
	0x0F12, 0x0070, //#TVAR_afit_pBaseVals[432]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[433]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[434]
	0x0F12, 0x01AA, //#TVAR_afit_pBaseVals[435]
	0x0F12, 0x0014, //#TVAR_afit_pBaseVals[436]
	0x0F12, 0x0014, //#TVAR_afit_pBaseVals[437]
	0x0F12, 0x000A, //#TVAR_afit_pBaseVals[438]
	0x0F12, 0x000A, //#TVAR_afit_pBaseVals[439]
	0x0F12, 0x0140, //#TVAR_afit_pBaseVals[440]
	0x0F12, 0x003C, //#TVAR_afit_pBaseVals[441]
	0x0F12, 0x0032, //#TVAR_afit_pBaseVals[442]
	0x0F12, 0x0023, //#TVAR_afit_pBaseVals[443]
	0x0F12, 0x0023, //#TVAR_afit_pBaseVals[444]
	0x0F12, 0x0032, //#TVAR_afit_pBaseVals[445]
	0x0F12, 0x0A24, //#TVAR_afit_pBaseVals[446]
	0x0F12, 0x1701, //#TVAR_afit_pBaseVals[447]
	0x0F12, 0x0229, //#TVAR_afit_pBaseVals[448]
	0x0F12, 0x1403, //#TVAR_afit_pBaseVals[449]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[450]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[451]
	0x0F12, 0x0505, //#TVAR_afit_pBaseVals[452]
	0x0F12, 0x00FF, //#TVAR_afit_pBaseVals[453]
	0x0F12, 0x043B, //#TVAR_afit_pBaseVals[454]
	0x0F12, 0x1414, //#TVAR_afit_pBaseVals[455]
	0x0F12, 0x0301, //#TVAR_afit_pBaseVals[456]
	0x0F12, 0xFF07, //#TVAR_afit_pBaseVals[457]
	0x0F12, 0x051E, //#TVAR_afit_pBaseVals[458]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[459]
	0x0F12, 0x0000, //#TVAR_afit_pBaseVals[460]
	0x0F12, 0x0A05, //#TVAR_afit_pBaseVals[461]
	0x0F12, 0x143C, //#TVAR_afit_pBaseVals[462]
	0x0F12, 0x0514, //#TVAR_afit_pBaseVals[463]
	0x0F12, 0x0002, //#TVAR_afit_pBaseVals[464]
	0x0F12, 0x0096, //#TVAR_afit_pBaseVals[465]
	0x0F12, 0x1002, //#TVAR_afit_pBaseVals[466]
	0x0F12, 0x001E, //#TVAR_afit_pBaseVals[467]
	0x0F12, 0x0900, //#TVAR_afit_pBaseVals[468]
	0x0F12, 0x0600, //#TVAR_afit_pBaseVals[469]
	0x0F12, 0x0504, //#TVAR_afit_pBaseVals[470]
	0x0F12, 0x0305, //#TVAR_afit_pBaseVals[471]
	0x0F12, 0x6402, //#TVAR_afit_pBaseVals[472]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[473]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[474]
	0x0F12, 0x0080, //#TVAR_afit_pBaseVals[475]
	0x0F12, 0x5050, //#TVAR_afit_pBaseVals[476]
	0x0F12, 0x0101, //#TVAR_afit_pBaseVals[477]
	0x0F12, 0x1C02, //#TVAR_afit_pBaseVals[478]
	0x0F12, 0x191C, //#TVAR_afit_pBaseVals[479]
	0x0F12, 0x2819, //#TVAR_afit_pBaseVals[480]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[481]
	0x0F12, 0x1E03, //#TVAR_afit_pBaseVals[482]
	0x0F12, 0x1E0F, //#TVAR_afit_pBaseVals[483]
	0x0F12, 0x0508, //#TVAR_afit_pBaseVals[484]
	0x0F12, 0x32FF, //#TVAR_afit_pBaseVals[485]
	0x0F12, 0xAA04, //#TVAR_afit_pBaseVals[486]
	0x0F12, 0x1452, //#TVAR_afit_pBaseVals[487]
	0x0F12, 0x4015, //#TVAR_afit_pBaseVals[488]
	0x0F12, 0x0604, //#TVAR_afit_pBaseVals[489]
	0x0F12, 0x5006, //#TVAR_afit_pBaseVals[490]
	0x0F12, 0x0150, //#TVAR_afit_pBaseVals[491]
	0x0F12, 0x0201, //#TVAR_afit_pBaseVals[492]
	0x0F12, 0x1E1E, //#TVAR_afit_pBaseVals[493]
	0x0F12, 0x1212, //#TVAR_afit_pBaseVals[494]
	0x0F12, 0x0028, //#TVAR_afit_pBaseVals[495]
	0x0F12, 0x030A, //#TVAR_afit_pBaseVals[496]
	0x0F12, 0x0A10, //#TVAR_afit_pBaseVals[497]
	0x0F12, 0x0819, //#TVAR_afit_pBaseVals[498]
	0x0F12, 0xFF05, //#TVAR_afit_pBaseVals[499]
	0x0F12, 0x0432, //#TVAR_afit_pBaseVals[500]
	0x0F12, 0x4052, //#TVAR_afit_pBaseVals[501]
	0x0F12, 0x1514, //#TVAR_afit_pBaseVals[502]
	0x0F12, 0x0440, //#TVAR_afit_pBaseVals[503]
	0x0F12, 0x0302, //#TVAR_afit_pBaseVals[504]
	0x0F12, 0x5050, //#TVAR_afit_pBaseVals[505]
	0x0F12, 0x0101, //#TVAR_afit_pBaseVals[506]
	0x0F12, 0x1E02, //#TVAR_afit_pBaseVals[507]
	0x0F12, 0x121E, //#TVAR_afit_pBaseVals[508]
	0x0F12, 0x2812, //#TVAR_afit_pBaseVals[509]
	0x0F12, 0x0A00, //#TVAR_afit_pBaseVals[510]
	0x0F12, 0x1003, //#TVAR_afit_pBaseVals[511]
	0x0F12, 0x190A, //#TVAR_afit_pBaseVals[512]
	0x0F12, 0x0508, //#TVAR_afit_pBaseVals[513]
	0x0F12, 0x32FF, //#TVAR_afit_pBaseVals[514]
	0x0F12, 0x5204, //#TVAR_afit_pBaseVals[515]
	0x0F12, 0x1440, //#TVAR_afit_pBaseVals[516]
	0x0F12, 0x4015, //#TVAR_afit_pBaseVals[517]
	0x0F12, 0x0204, //#TVAR_afit_pBaseVals[518]
	0x0F12, 0x0003, //#TVAR_afit_pBaseVals[519]
	
	// AFIT table (Constants)
	0x0F12, 0x7F7A, //#afit_pConstBaseVals[0]
	0x0F12, 0x7FBD, //#afit_pConstBaseVals[1]
	0x0F12, 0xBEFC, //#afit_pConstBaseVals[2]
	0x0F12, 0xF7BC, //#afit_pConstBaseVals[3]
	0x0F12, 0x7E06, //#afit_pConstBaseVals[4]
	0x0F12, 0x0053, //#afit_pConstBaseVals[5]
	
	// Update Changed Registers
	0x002A, 0x0664,
	0x0F12, 0x013E, //seti_uContrastCenter


};

static unsigned short s5k5cagx_init_reg_short8[]={	
#if 0	
	//================================================================================================
	// SET PLL
	//================================================================================================
	// How to set
	// 1. MCLK
	//hex(CLK you want) * 1000)
	//// 2. System CLK
	//hex((CLK you want) * 1000 / 4)
	//// 3. PCLK
	//hex((CLK you want) * 1000 / 4)
	//================================================================================================
	// Set input CLK // 24.5MHz
	0x0F12, 0x7000,
	
	0x002A, 0x01CC,
	0x0F12, 0x5DC0, //#REG_TC_IPRM_InClockLSBs
	0x0F12, 0x0000, //#REG_TC_IPRM_InClockMSBs
	0x002A, 0x01EE,
	0x0F12, 0x0001, //#REG_TC_IPRM_UseNPviClocks // Number of PLL setting
	#if 1
	// Set system CLK // 45MHz
	0x002A, 0x01F6,
	0x0F12, 0x2EE0, //#REG_TC_IPRM_OpClk4KHz_0
	// Set pixel CLK // 60MHz (0x3A98)
	0x0F12, 0x3A8B, //#REG_TC_IPRM_MinOutRate4KHz_0
	0x0F12, 0x3AA4, //#REG_TC_IPRM_MaxOutRate4KHz_0
	#else
	// Set system CLK // 45MHz
	0x002A, 0x01F6,
	0x0F12, 0x2EE0+10000/4, //#REG_TC_IPRM_OpClk4KHz_0
	// Set pixel CLK // 60MHz (0x3A98)
	0x0F12, 0x3A8B, //#REG_TC_IPRM_MinOutRate4KHz_0
	0x0F12, 0x3AA4, //#REG_TC_IPRM_MaxOutRate4KHz_0
	
	#endif
	// Update PLL
	0x002A, 0x0208,
	0x0F12, 0x0001, //#REG_TC_IPRM_InitParamsUpdated
#else 
//================================================================================================
// SET PLL
//================================================================================================
// How to set
// 1. MCLK
//hex(CLK you want) * 1000)
//// 2. System CLK
//hex((CLK you want) * 1000 / 4)
//// 3. PCLK
//hex((CLK you want) * 1000 / 4)
//================================================================================================
// Set input CLK // 24MHz
0x002A, 0x01CC,
0x0F12, 0x5DC0, //#REG_TC_IPRM_InClockLSBs
0x0F12, 0x0000, //#REG_TC_IPRM_InClockMSBs
0x002A, 0x01EE,
0x0F12, 0x0001, //#REG_TC_IPRM_UseNPviClocks // Number of PLL setting
// Set system CLK // 53MHz
0x002A, 0x01F6,
0x0F12, 0x33C2, //#REG_TC_IPRM_OpClk4KHz_0
// Set pixel CLK // 65MHz
0x0F12, 0x3F04, //#REG_TC_IPRM_MinOutRate4KHz_0 // 64.528MHz
0x0F12, 0x3FFF, //#REG_TC_IPRM_MaxOutRate4KHz_0 // 65.532MHz
// Update PLL
0x002A, 0x0208,
0x0F12, 0x0001, //#REG_TC_IPRM_InitParamsUpdated
#endif
};
static unsigned short s5k5cagx_init_reg_short8_1_2048x1536[]={	

// Input crop
//WRITE	#REG_TC_IPRM_InputWidthSize	  0500	//1280
//WRITE	#REG_TC_IPRM_InputHeightSize	02D0	//720
//WRITE	#REG_TC_IPRM_InputWidthOfs	  0180	//1280
//WRITE	#REG_TC_IPRM_InputHeightOfs	  0198	//720

0x0028, 0x7000,
0x002A, 0x01D6,
0x0F12, 2048, //#REG_TC_IPRM_InputWidthSize	
0x0F12, 1536, //#REG_TC_IPRM_InputHeightSize
0x0F12, 0, //#REG_TC_IPRM_InputWidthOfs	
0x0F12, 0, //#REG_TC_IPRM_InputHeightOfs	
};

static unsigned short s5k5cagx_init_reg_short8_1_1280x720[]={	

// Input crop
//WRITE	#REG_TC_IPRM_InputWidthSize	  0500	//1280
//WRITE	#REG_TC_IPRM_InputHeightSize	02D0	//720
//WRITE	#REG_TC_IPRM_InputWidthOfs	  0180	//1280
//WRITE	#REG_TC_IPRM_InputHeightOfs	  0198	//720

0x0028, 0x7000,
0x002A, 0x01D6,
0x0F12, 0x0500, //#REG_TC_IPRM_InputWidthSize	
0x0F12, 0x02D0, //#REG_TC_IPRM_InputHeightSize
0x0F12, 0x0180, //#REG_TC_IPRM_InputWidthOfs	
0x0F12, 0x0198, //#REG_TC_IPRM_InputHeightOfs	
};

static unsigned short s5k5cagx_init_reg_short8_1280x720[]={	
//================================================================================================
// SET PREVIEW CONFIGURATION_0
// # Foramt : YUV422
// # Size: HD
// # FPS : 22~10fps
//================================================================================================
0x002A, 0x026C,
0x0F12, 0x0500, //#REG_0TC_PCFG_usWidth//1280 
0x0F12, 0x02D0, //#REG_0TC_PCFG_usHeight//720
0x0F12, 0x0005, //#REG_0TC_PCFG_Format
0x0F12, 0x3FFF, //#REG_0TC_PCFG_usMaxOut4KHzRate
0x0F12, 0x3F04, //#REG_0TC_PCFG_usMinOut4KHzRate
0x0F12, 0x0100, //#REG_0TC_PCFG_OutClkPerPix88
0x0F12, 0x0800, //#REG_0TC_PCFG_uMaxBpp88
0x0F12, 0x0042,//0x0052, //#REG_0TC_PCFG_PVIMask //s0050 = FALSE in MSM6290 : s0052 = TRUE in MSM6800 //reg
0x0F12, 0x4000, //#REG_0TC_PCFG_OIFMask
0x0F12, 0x01E0, //#REG_0TC_PCFG_usJpegPacketSize
0x0F12, 0x0000, //#REG_0TC_PCFG_usJpegTotalPackets
0x0F12, 0x0000, //#REG_0TC_PCFG_uClockInd
0x0F12, 0x0000, //#REG_0TC_PCFG_usFrTimeType
0x0F12, 0x0002, //#REG_0TC_PCFG_FrRateQualityType
0x0F12, 10000/20, //#REG_0TC_PCFG_usMaxFrTimeMsecMult10 //30fps
0x0F12, 10000/27, //#REG_0TC_PCFG_usMinFrTimeMsecMult10 //30fps

		0x0F12, 0x0000, //#REG_0TC_PCFG_bSmearOutput
		0x0F12, 0x0000, //#REG_0TC_PCFG_sSaturation
		0x0F12, 0x0000, //#REG_0TC_PCFG_sSharpBlur
		0x0F12, 0x0000, //#REG_0TC_PCFG_sColorTemp
		0x0F12, 0x0000, //#REG_0TC_PCFG_uDeviceGammaIndex
#ifdef CONFIG_BOARD_ODROID_A4
		0x0F12, 0x0003, //#REG_0TC_PCFG_uPrevMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#else	
		0x0F12, 0x0000, //#REG_0TC_PCFG_uPrevMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#endif

#if 0
//ZOOM
//WRITE #REG_TC_PZOOM_ZoomInputWidth  0500  // 1280
//WRITE #REG_TC_PZOOM_ZoomInputHeight 02D0  // 720

0x002A, 0x046C,
0x0F12, 0x0500, //#REG_TC_PZOOM_ZoomInputWidth 
0x0F12, 0x02D0, //#REG_TC_PZOOM_ZoomInputHeight

//Window Settings for 720p
//WRITE #REG_TC_GP_ReqInputWidth  0500      // 1280
//WRITE #REG_TC_GP_ReqInputHeight 02D0      // 720
//WRITE #REG_TC_GP_InputWidthOfs  0180       // 384
//WRITE #REG_TC_GP_InputHeightOfs 0198      // 408

0x002A, 0x0232,
0x0F12, 0x0500,  //#REG_TC_GP_ReqInputWidth 
0x0F12, 0x02D0,  //#REG_TC_GP_ReqInputHeight
0x0F12, 0x0180,  //#REG_TC_GP_InputWidthOfs 
0x0F12, 0x0198,  //#REG_TC_GP_InputHeightOfs
#endif
};
static unsigned short s5k5cagx_init_reg_short8_640x480[]={	
//================================================================================================
// SET PREVIEW CONFIGURATION_0
// # Foramt : YUV422
// # Size: HD
// # FPS : 22~10fps
//================================================================================================
0x002A, 0x026C,
0x0F12, 640, //#REG_0TC_PCFG_usWidth//1280 
0x0F12, 480, //#REG_0TC_PCFG_usHeight//720
0x0F12, 0x0005, //#REG_0TC_PCFG_Format
0x0F12, 0x3FFF, //#REG_0TC_PCFG_usMaxOut4KHzRate
0x0F12, 0x3F04, //#REG_0TC_PCFG_usMinOut4KHzRate
0x0F12, 0x0100, //#REG_0TC_PCFG_OutClkPerPix88
0x0F12, 0x0800, //#REG_0TC_PCFG_uMaxBpp88
0x0F12, 0x0042,//0x0052, //#REG_0TC_PCFG_PVIMask //s0050 = FALSE in MSM6290 : s0052 = TRUE in MSM6800 //reg
0x0F12, 0x4000, //#REG_0TC_PCFG_OIFMask
0x0F12, 0x01E0, //#REG_0TC_PCFG_usJpegPacketSize
0x0F12, 0x0000, //#REG_0TC_PCFG_usJpegTotalPackets
0x0F12, 0x0000, //#REG_0TC_PCFG_uClockInd
0x0F12, 0x0000, //#REG_0TC_PCFG_usFrTimeType
0x0F12, 0x0000, //#REG_0TC_PCFG_FrRateQualityType
0x0F12, 10000/26, //#REG_0TC_PCFG_usMaxFrTimeMsecMult10 //30fps
0x0F12, 10000/30, //#REG_0TC_PCFG_usMinFrTimeMsecMult10 //30fps

		0x0F12, 0x0000, //#REG_0TC_PCFG_bSmearOutput
		0x0F12, 0x0000, //#REG_0TC_PCFG_sSaturation
		0x0F12, 0x0000, //#REG_0TC_PCFG_sSharpBlur
		0x0F12, 0x0000, //#REG_0TC_PCFG_sColorTemp
		0x0F12, 0x0000, //#REG_0TC_PCFG_uDeviceGammaIndex
#ifdef CONFIG_BOARD_ODROID_A4
		0x0F12, 0x0003, //#REG_0TC_PCFG_uPrevMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#else	
		0x0F12, 0x0000, //#REG_0TC_PCFG_uPrevMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#endif

#if 0
//ZOOM
//WRITE #REG_TC_PZOOM_ZoomInputWidth  0500  // 1280
//WRITE #REG_TC_PZOOM_ZoomInputHeight 02D0  // 720

0x002A, 0x046C,
0x0F12, 0x0500, //#REG_TC_PZOOM_ZoomInputWidth 
0x0F12, 0x02D0, //#REG_TC_PZOOM_ZoomInputHeight

//Window Settings for 720p
//WRITE #REG_TC_GP_ReqInputWidth  0500      // 1280
//WRITE #REG_TC_GP_ReqInputHeight 02D0      // 720
//WRITE #REG_TC_GP_InputWidthOfs  0180       // 384
//WRITE #REG_TC_GP_InputHeightOfs 0198      // 408

0x002A, 0x0232,
0x0F12, 0x0500,  //#REG_TC_GP_ReqInputWidth 
0x0F12, 0x02D0,  //#REG_TC_GP_ReqInputHeight
0x0F12, 0x0180,  //#REG_TC_GP_InputWidthOfs 
0x0F12, 0x0198,  //#REG_TC_GP_InputHeightOfs
#endif
};
static unsigned short s5k5cagx_init_reg_short8_800x480[]={	
//================================================================================================
// SET PREVIEW CONFIGURATION_0
// # Foramt : YUV422
// # Size: HD
// # FPS : 22~10fps
//================================================================================================
0x002A, 0x026C,
0x0F12, 800, //#REG_0TC_PCFG_usWidth//1280 
0x0F12, 480, //#REG_0TC_PCFG_usHeight//720
0x0F12, 0x0005, //#REG_0TC_PCFG_Format
0x0F12, 0x3FFF, //#REG_0TC_PCFG_usMaxOut4KHzRate
0x0F12, 0x3F04, //#REG_0TC_PCFG_usMinOut4KHzRate
0x0F12, 0x0100, //#REG_0TC_PCFG_OutClkPerPix88
0x0F12, 0x0800, //#REG_0TC_PCFG_uMaxBpp88
0x0F12, 0x0042,//0x0052, //#REG_0TC_PCFG_PVIMask //s0050 = FALSE in MSM6290 : s0052 = TRUE in MSM6800 //reg
0x0F12, 0x4000, //#REG_0TC_PCFG_OIFMask
0x0F12, 0x01E0, //#REG_0TC_PCFG_usJpegPacketSize
0x0F12, 0x0000, //#REG_0TC_PCFG_usJpegTotalPackets
0x0F12, 0x0000, //#REG_0TC_PCFG_uClockInd
0x0F12, 0x0000, //#REG_0TC_PCFG_usFrTimeType
0x0F12, 0x0000, //#REG_0TC_PCFG_FrRateQualityType
0x0F12, 10000/26, //#REG_0TC_PCFG_usMaxFrTimeMsecMult10 //30fps
0x0F12, 10000/30, //#REG_0TC_PCFG_usMinFrTimeMsecMult10 //30fps

		0x0F12, 0x0000, //#REG_0TC_PCFG_bSmearOutput
		0x0F12, 0x0000, //#REG_0TC_PCFG_sSaturation
		0x0F12, 0x0000, //#REG_0TC_PCFG_sSharpBlur
		0x0F12, 0x0000, //#REG_0TC_PCFG_sColorTemp
		0x0F12, 0x0000, //#REG_0TC_PCFG_uDeviceGammaIndex
#ifdef CONFIG_BOARD_ODROID_A4
		0x0F12, 0x0003, //#REG_0TC_PCFG_uPrevMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#else	
		0x0F12, 0x0000, //#REG_0TC_PCFG_uPrevMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#endif

#if 0
//ZOOM
//WRITE #REG_TC_PZOOM_ZoomInputWidth  0500  // 1280
//WRITE #REG_TC_PZOOM_ZoomInputHeight 02D0  // 720

0x002A, 0x046C,
0x0F12, 0x0500, //#REG_TC_PZOOM_ZoomInputWidth 
0x0F12, 0x02D0, //#REG_TC_PZOOM_ZoomInputHeight

//Window Settings for 720p
//WRITE #REG_TC_GP_ReqInputWidth  0500      // 1280
//WRITE #REG_TC_GP_ReqInputHeight 02D0      // 720
//WRITE #REG_TC_GP_InputWidthOfs  0180       // 384
//WRITE #REG_TC_GP_InputHeightOfs 0198      // 408

0x002A, 0x0232,
0x0F12, 0x0500,  //#REG_TC_GP_ReqInputWidth 
0x0F12, 0x02D0,  //#REG_TC_GP_ReqInputHeight
0x0F12, 0x0180,  //#REG_TC_GP_InputWidthOfs 
0x0F12, 0x0198,  //#REG_TC_GP_InputHeightOfs
#endif
};


static unsigned short s5k5cagx_init_reg_short8_640x480_old[]={	
	//================================================================================================
	// SET PREVIEW CONFIGURATION_0
	// # Foramt : YUV422
	// # Size: VGA
	// # FPS : 22~10fps
	//================================================================================================
	0x002A, 0x026C,
	0x0F12, 640, //#REG_0TC_PCFG_usWidth//640 
	0x0F12, 480, //#REG_0TC_PCFG_usHeight//480
	0x0F12, 0x0005, //#REG_0TC_PCFG_Format
	0x0F12, 0x3AA4, //#REG_0TC_PCFG_usMaxOut4KHzRate
	0x0F12, 0x3A8B, //#REG_0TC_PCFG_usMinOut4KHzRate
	0x0F12, 0x0100, //#REG_0TC_PCFG_OutClkPerPix88
	0x0F12, 0x0800, //#REG_0TC_PCFG_uMaxBpp88
	0x0F12, 0x0042, //#REG_0TC_PCFG_PVIMask //s0050 = FALSE in MSM6290 : s0052 = TRUE in MSM6800 //reg
	0x0F12, 0x4000, //#REG_0TC_PCFG_OIFMask
	0x0F12, 0x01E0, //#REG_0TC_PCFG_usJpegPacketSize
	0x0F12, 0x0000, //#REG_0TC_PCFG_usJpegTotalPackets
	0x0F12, 0x0000, //#REG_0TC_PCFG_uClockInd
	0x0F12, 0x0000, //#REG_0TC_PCFG_usFrTimeType
	0x0F12, 0x0001, //#REG_0TC_PCFG_FrRateQualityType
	0x0F12, 0x03E8, //#REG_0TC_PCFG_usMaxFrTimeMsecMult10 //10fps
	0x0F12, 333,//0x01A0, //#REG_0TC_PCFG_usMinFrTimeMsecMult10 //24fps //30fps
	0x0F12, 0x0000, //#REG_0TC_PCFG_bSmearOutput
	0x0F12, 0x0000, //#REG_0TC_PCFG_sSaturation
	0x0F12, 0x0000, //#REG_0TC_PCFG_sSharpBlur
	0x0F12, 0x0000, //#REG_0TC_PCFG_sColorTemp
	0x0F12, 0x0000, //#REG_0TC_PCFG_uDeviceGammaIndex
#ifdef CONFIG_BOARD_ODROID_A4
	0x0F12, 0x0003, //#REG_0TC_PCFG_uPrevMirror
	0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
	0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#else	
	0x0F12, 0x0000, //#REG_0TC_PCFG_uPrevMirror
	0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
	0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#endif
};

static unsigned short s5k5cagx_init_reg_short8_1000x562[]={	
	//================================================================================================
	// SET PREVIEW CONFIGURATION_0
	// # Foramt : YUV422
	// # Size: VGA
	// # FPS : 22~10fps
	//================================================================================================
	0x002A, 0x026C,

	0x0F12, 1000, //#REG_0TC_PCFG_usWidth//640 
	0x0F12, 562, //#REG_0TC_PCFG_usHeight//480

	0x0F12, 0x0005, //#REG_0TC_PCFG_Format
	0x0F12, 0x3AA4, //#REG_0TC_PCFG_usMaxOut4KHzRate
	0x0F12, 0x3A8B, //#REG_0TC_PCFG_usMinOut4KHzRate
	0x0F12, 0x0100, //#REG_0TC_PCFG_OutClkPerPix88
	0x0F12, 0x0800, //#REG_0TC_PCFG_uMaxBpp88
	0x0F12, 0x0042, //#REG_0TC_PCFG_PVIMask //s0050 = FALSE in MSM6290 : s0052 = TRUE in MSM6800 //reg
	0x0F12, 0x4000, //#REG_0TC_PCFG_OIFMask
	0x0F12, 0x01E0, //#REG_0TC_PCFG_usJpegPacketSize
	0x0F12, 0x0000, //#REG_0TC_PCFG_usJpegTotalPackets
	0x0F12, 0x0000, //#REG_0TC_PCFG_uClockInd
	0x0F12, 0x0000, //#REG_0TC_PCFG_usFrTimeType
	0x0F12, 0x0001, //#REG_0TC_PCFG_FrRateQualityType
	0x0F12, 0x03E8, //#REG_0TC_PCFG_usMaxFrTimeMsecMult10 //10fps
	0x0F12, 333,//0x01A0, //#REG_0TC_PCFG_usMinFrTimeMsecMult10 //24fps //30fps
	0x0F12, 0x0000, //#REG_0TC_PCFG_bSmearOutput
	0x0F12, 0x0000, //#REG_0TC_PCFG_sSaturation
	0x0F12, 0x0000, //#REG_0TC_PCFG_sSharpBlur
	0x0F12, 0x0000, //#REG_0TC_PCFG_sColorTemp
	0x0F12, 0x0000, //#REG_0TC_PCFG_uDeviceGammaIndex
#ifdef CONFIG_BOARD_ODROID_A4
	0x0F12, 0x0003, //#REG_0TC_PCFG_uPrevMirror
	0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
	0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#else	
	0x0F12, 0x0000, //#REG_0TC_PCFG_uPrevMirror
	0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
	0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#endif
};
static unsigned short s5k5cagx_init_reg_short9[]={	

// Start T&P part
// DO NOT DELETE T&P SECTION COMMENTS! They are required to debug T&P related issues.
// svn://transrdsrv/svn/svnroot/System/Software/tcevb/SDK+FW/ISP_5CA/Firmware
// Rev: 32375-32375
// Signature:
// md5 78c1a0d32ef22ba270994f08d64a05a0 .btp
// md5 6765ffc40fde4420aab81f0039a60c38 .htp
// md5 956e8c724c34dd8b76dd297b92f59677 .RegsMap.h
// md5 7db8e8f88de22128b8b909128f087a53 .RegsMap.bin
// md5 506b4144bd48cdb79cbecdda4f7176ba .base.RegsMap.h
// md5 fd8f92f13566c1a788746b23691c5f5f .base.RegsMap.bin
0x0028,0x7000,
0x002A,0x2CF8,
0x0F12,0xB510,
0x0F12,0x4827,
0x0F12,0x21C0,
0x0F12,0x8041,
0x0F12,0x4825,
0x0F12,0x4A26,
0x0F12,0x3020,
0x0F12,0x8382,
0x0F12,0x1D12,
0x0F12,0x83C2,
0x0F12,0x4822,
0x0F12,0x3040,
0x0F12,0x8041,
0x0F12,0x4821,
0x0F12,0x4922,
0x0F12,0x3060,
0x0F12,0x8381,
0x0F12,0x1D09,
0x0F12,0x83C1,
0x0F12,0x4821,
0x0F12,0x491D,
0x0F12,0x8802,
0x0F12,0x3980,
0x0F12,0x804A,
0x0F12,0x8842,
0x0F12,0x808A,
0x0F12,0x8882,
0x0F12,0x80CA,
0x0F12,0x88C2,
0x0F12,0x810A,
0x0F12,0x8902,
0x0F12,0x491C,
0x0F12,0x80CA,
0x0F12,0x8942,
0x0F12,0x814A,
0x0F12,0x8982,
0x0F12,0x830A,
0x0F12,0x89C2,
0x0F12,0x834A,
0x0F12,0x8A00,
0x0F12,0x4918,
0x0F12,0x8188,
0x0F12,0x4918,
0x0F12,0x4819,
0x0F12,0xF000,
0x0F12,0xFA0C,
0x0F12,0x4918,
0x0F12,0x4819,
0x0F12,0x6341,
0x0F12,0x4919,
0x0F12,0x4819,
0x0F12,0xF000,
0x0F12,0xFA05,
0x0F12,0x4816,
0x0F12,0x4918,
0x0F12,0x3840,
0x0F12,0x62C1,
0x0F12,0x4918,
0x0F12,0x3880,
0x0F12,0x63C1,
0x0F12,0x4917,
0x0F12,0x6301,
0x0F12,0x4917,
0x0F12,0x3040,
0x0F12,0x6181,
0x0F12,0x4917,
0x0F12,0x4817,
0x0F12,0xF000,
0x0F12,0xF9F5,
0x0F12,0x4917,
0x0F12,0x4817,
0x0F12,0xF000,
0x0F12,0xF9F1,
0x0F12,0x4917,
0x0F12,0x4817,
0x0F12,0xF000,
0x0F12,0xF9ED,
0x0F12,0xBC10,
0x0F12,0xBC08,
0x0F12,0x4718,
0x0F12,0x1100,
0x0F12,0xD000,
0x0F12,0x267C,
0x0F12,0x0000,
0x0F12,0x2CE8,
0x0F12,0x0000,
0x0F12,0x3274,
0x0F12,0x7000,
0x0F12,0xF400,
0x0F12,0xD000,
0x0F12,0xF520,
0x0F12,0xD000,
0x0F12,0x2DF1,
0x0F12,0x7000,
0x0F12,0x89A9,
0x0F12,0x0000,
0x0F12,0x2E43,
0x0F12,0x7000,
0x0F12,0x0140,
0x0F12,0x7000,
0x0F12,0x2E75,
0x0F12,0x7000,
0x0F12,0xB4F7,
0x0F12,0x0000,
0x0F12,0x2EFF,
0x0F12,0x7000,
0x0F12,0x2F23,
0x0F12,0x7000,
0x0F12,0x2FCD,
0x0F12,0x7000,
0x0F12,0x2FE1,
0x0F12,0x7000,
0x0F12,0x2FB5,
0x0F12,0x7000,
0x0F12,0x013D,
0x0F12,0x0001,
0x0F12,0x3067,
0x0F12,0x7000,
0x0F12,0x5823,
0x0F12,0x0000,
0x0F12,0x30B5,
0x0F12,0x7000,
0x0F12,0xD789,
0x0F12,0x0000,
0x0F12,0xB570,
0x0F12,0x6804,
0x0F12,0x6845,
0x0F12,0x6881,
0x0F12,0x6840,
0x0F12,0x2900,
0x0F12,0x6880,
0x0F12,0xD007,
0x0F12,0x49C2,
0x0F12,0x8949,
0x0F12,0x084A,
0x0F12,0x1880,
0x0F12,0xF000,
0x0F12,0xF9B8,
0x0F12,0x80A0,
0x0F12,0xE000,
0x0F12,0x80A0,
0x0F12,0x88A0,
0x0F12,0x2800,
0x0F12,0xD010,
0x0F12,0x68A9,
0x0F12,0x6828,
0x0F12,0x084A,
0x0F12,0x1880,
0x0F12,0xF000,
0x0F12,0xF9AC,
0x0F12,0x8020,
0x0F12,0x1D2D,
0x0F12,0xCD03,
0x0F12,0x084A,
0x0F12,0x1880,
0x0F12,0xF000,
0x0F12,0xF9A5,
0x0F12,0x8060,
0x0F12,0xBC70,
0x0F12,0xBC08,
0x0F12,0x4718,
0x0F12,0x2000,
0x0F12,0x8060,
0x0F12,0x8020,
0x0F12,0xE7F8,
0x0F12,0xB510,
0x0F12,0xF000,
0x0F12,0xF9A0,
0x0F12,0x48B1,
0x0F12,0x49B2,
0x0F12,0x8800,
0x0F12,0x4AB2,
0x0F12,0x2805,
0x0F12,0xD003,
0x0F12,0x4BB1,
0x0F12,0x795B,
0x0F12,0x2B00,
0x0F12,0xD005,
0x0F12,0x2001,
0x0F12,0x8008,
0x0F12,0x8010,
0x0F12,0xBC10,
0x0F12,0xBC08,
0x0F12,0x4718,
0x0F12,0x2800,
0x0F12,0xD1FA,
0x0F12,0x2000,
0x0F12,0x8008,
0x0F12,0x8010,
0x0F12,0xE7F6,
0x0F12,0xB5F8,
0x0F12,0x2407,
0x0F12,0x2C06,
0x0F12,0xD035,
0x0F12,0x2C07,
0x0F12,0xD033,
0x0F12,0x48A3,
0x0F12,0x8BC1,
0x0F12,0x2900,
0x0F12,0xD02A,
0x0F12,0x00A2,
0x0F12,0x1815,
0x0F12,0x4AA4,
0x0F12,0x6DEE,
0x0F12,0x8A92,
0x0F12,0x4296,
0x0F12,0xD923,
0x0F12,0x0028,
0x0F12,0x3080,
0x0F12,0x0007,
0x0F12,0x69C0,
0x0F12,0xF000,
0x0F12,0xF96D,
0x0F12,0x1C71,
0x0F12,0x0280,
0x0F12,0xF000,
0x0F12,0xF969,
0x0F12,0x0006,
0x0F12,0x4898,
0x0F12,0x0061,
0x0F12,0x1808,
0x0F12,0x8D80,
0x0F12,0x0A01,
0x0F12,0x0600,
0x0F12,0x0E00,
0x0F12,0x1A08,
0x0F12,0xF000,
0x0F12,0xF96C,
0x0F12,0x0002,
0x0F12,0x6DE9,
0x0F12,0x6FE8,
0x0F12,0x1A08,
0x0F12,0x4351,
0x0F12,0x0300,
0x0F12,0x1C49,
0x0F12,0xF000,
0x0F12,0xF955,
0x0F12,0x0401,
0x0F12,0x0430,
0x0F12,0x0C00,
0x0F12,0x4301,
0x0F12,0x61F9,
0x0F12,0xE004,
0x0F12,0x00A2,
0x0F12,0x4990,
0x0F12,0x1810,
0x0F12,0x3080,
0x0F12,0x61C1,
0x0F12,0x1E64,
0x0F12,0xD2C5,
0x0F12,0x2006,
0x0F12,0xF000,
0x0F12,0xF95B,
0x0F12,0x2007,
0x0F12,0xF000,
0x0F12,0xF958,
0x0F12,0xBCF8,
0x0F12,0xBC08,
0x0F12,0x4718,
0x0F12,0xB510,
0x0F12,0xF000,
0x0F12,0xF95A,
0x0F12,0x2800,
0x0F12,0xD00A,
0x0F12,0x4881,
0x0F12,0x8B81,
0x0F12,0x0089,
0x0F12,0x1808,
0x0F12,0x6DC1,
0x0F12,0x4883,
0x0F12,0x8A80,
0x0F12,0x4281,
0x0F12,0xD901,
0x0F12,0x2001,
0x0F12,0xE7A1,
0x0F12,0x2000,
0x0F12,0xE79F,
0x0F12,0xB5F8,
0x0F12,0x0004,
0x0F12,0x4F80,
0x0F12,0x227D,
0x0F12,0x8938,
0x0F12,0x0152,
0x0F12,0x4342,
0x0F12,0x487E,
0x0F12,0x9000,
0x0F12,0x8A01,
0x0F12,0x0848,
0x0F12,0x1810,
0x0F12,0xF000,
0x0F12,0xF91F,
0x0F12,0x210F,
0x0F12,0xF000,
0x0F12,0xF942,
0x0F12,0x497A,
0x0F12,0x8C49,
0x0F12,0x090E,
0x0F12,0x0136,
0x0F12,0x4306,
0x0F12,0x4979,
0x0F12,0x2C00,
0x0F12,0xD003,
0x0F12,0x2001,
0x0F12,0x0240,
0x0F12,0x4330,
0x0F12,0x8108,
0x0F12,0x4876,
0x0F12,0x2C00,
0x0F12,0x8D00,
0x0F12,0xD001,
0x0F12,0x2501,
0x0F12,0xE000,
0x0F12,0x2500,
0x0F12,0x4972,
0x0F12,0x4328,
0x0F12,0x8008,
0x0F12,0x207D,
0x0F12,0x00C0,
0x0F12,0xF000,
0x0F12,0xF930,
0x0F12,0x2C00,
0x0F12,0x496E,
0x0F12,0x0328,
0x0F12,0x4330,
0x0F12,0x8108,
0x0F12,0x88F8,
0x0F12,0x2C00,
0x0F12,0x01AA,
0x0F12,0x4310,
0x0F12,0x8088,
0x0F12,0x2C00,
0x0F12,0xD00B,
0x0F12,0x9800,
0x0F12,0x8A01,
0x0F12,0x4869,
0x0F12,0xF000,
0x0F12,0xF8F1,
0x0F12,0x4969,
0x0F12,0x8809,
0x0F12,0x4348,
0x0F12,0x0400,
0x0F12,0x0C00,
0x0F12,0xF000,
0x0F12,0xF918,
0x0F12,0x0020,
0x0F12,0xF000,
0x0F12,0xF91D,
0x0F12,0x4865,
0x0F12,0x7004,
0x0F12,0xE7A1,
0x0F12,0xB510,
0x0F12,0x0004,
0x0F12,0xF000,
0x0F12,0xF91E,
0x0F12,0x6020,
0x0F12,0x4962,
0x0F12,0x8B49,
0x0F12,0x0789,
0x0F12,0xD001,
0x0F12,0x0040,
0x0F12,0x6020,
0x0F12,0xE74A,
0x0F12,0xB510,
0x0F12,0xF000,
0x0F12,0xF91B,
0x0F12,0x485E,
0x0F12,0x8880,
0x0F12,0x0601,
0x0F12,0x4853,
0x0F12,0x1609,
0x0F12,0x8141,
0x0F12,0xE740,
0x0F12,0xB5F8,
0x0F12,0x000F,
0x0F12,0x4C54,
0x0F12,0x3420,
0x0F12,0x2500,
0x0F12,0x5765,
0x0F12,0x0039,
0x0F12,0xF000,
0x0F12,0xF913,
0x0F12,0x9000,
0x0F12,0x2600,
0x0F12,0x57A6,
0x0F12,0x4C4B,
0x0F12,0x42AE,
0x0F12,0xD01B,
0x0F12,0x4D53,
0x0F12,0x8AE8,
0x0F12,0x2800,
0x0F12,0xD013,
0x0F12,0x484C,
0x0F12,0x8A01,
0x0F12,0x8B80,
0x0F12,0x4378,
0x0F12,0xF000,
0x0F12,0xF8B5,
0x0F12,0x89A9,
0x0F12,0x1A41,
0x0F12,0x484D,
0x0F12,0x3820,
0x0F12,0x8AC0,
0x0F12,0x4348,
0x0F12,0x17C1,
0x0F12,0x0D89,
0x0F12,0x1808,
0x0F12,0x1280,
0x0F12,0x8961,
0x0F12,0x1A08,
0x0F12,0x8160,
0x0F12,0xE003,
0x0F12,0x88A8,
0x0F12,0x0600,
0x0F12,0x1600,
0x0F12,0x8160,
0x0F12,0x200A,
0x0F12,0x5E20,
0x0F12,0x42B0,
0x0F12,0xD011,
0x0F12,0xF000,
0x0F12,0xF8AB,
0x0F12,0x1D40,
0x0F12,0x00C3,
0x0F12,0x1A18,
0x0F12,0x214B,
0x0F12,0xF000,
0x0F12,0xF897,
0x0F12,0x211F,
0x0F12,0xF000,
0x0F12,0xF8BA,
0x0F12,0x210A,
0x0F12,0x5E61,
0x0F12,0x0FC9,
0x0F12,0x0149,
0x0F12,0x4301,
0x0F12,0x483C,
0x0F12,0x81C1,
0x0F12,0x9800,
0x0F12,0xE748,
0x0F12,0xB5F1,
0x0F12,0xB082,
0x0F12,0x2500,
0x0F12,0x4839,
0x0F12,0x9001,
0x0F12,0x2400,
0x0F12,0x2028,
0x0F12,0x4368,
0x0F12,0x4A39,
0x0F12,0x4937,
0x0F12,0x1887,
0x0F12,0x1840,
0x0F12,0x9000,
0x0F12,0x9800,
0x0F12,0x0066,
0x0F12,0x9A01,
0x0F12,0x1980,
0x0F12,0x218C,
0x0F12,0x5A09,
0x0F12,0x8A80,
0x0F12,0x8812,
0x0F12,0xF000,
0x0F12,0xF8CA,
0x0F12,0x53B8,
0x0F12,0x1C64,
0x0F12,0x2C14,
0x0F12,0xDBF1,
0x0F12,0x1C6D,
0x0F12,0x2D03,
0x0F12,0xDBE6,
0x0F12,0x9802,
0x0F12,0x6800,
0x0F12,0x0600,
0x0F12,0x0E00,
0x0F12,0xF000,
0x0F12,0xF8C5,
0x0F12,0xBCFE,
0x0F12,0xBC08,
0x0F12,0x4718,
0x0F12,0xB570,
0x0F12,0x6805,
0x0F12,0x2404,
0x0F12,0xF000,
0x0F12,0xF8C5,
0x0F12,0x2800,
0x0F12,0xD103,
0x0F12,0xF000,
0x0F12,0xF8C9,
0x0F12,0x2800,
0x0F12,0xD000,
0x0F12,0x2400,
0x0F12,0x3540,
0x0F12,0x88E8,
0x0F12,0x0500,
0x0F12,0xD403,
0x0F12,0x4822,
0x0F12,0x89C0,
0x0F12,0x2800,
0x0F12,0xD002,
0x0F12,0x2008,
0x0F12,0x4304,
0x0F12,0xE001,
0x0F12,0x2010,
0x0F12,0x4304,
0x0F12,0x481F,
0x0F12,0x8B80,
0x0F12,0x0700,
0x0F12,0x0F81,
0x0F12,0x2001,
0x0F12,0x2900,
0x0F12,0xD000,
0x0F12,0x4304,
0x0F12,0x491C,
0x0F12,0x8B0A,
0x0F12,0x42A2,
0x0F12,0xD004,
0x0F12,0x0762,
0x0F12,0xD502,
0x0F12,0x4A19,
0x0F12,0x3220,
0x0F12,0x8110,
0x0F12,0x830C,
0x0F12,0xE693,
0x0F12,0x0C3C,
0x0F12,0x7000,
0x0F12,0x26E8,
0x0F12,0x7000,
0x0F12,0x6100,
0x0F12,0xD000,
0x0F12,0x6500,
0x0F12,0xD000,
0x0F12,0x1A7C,
0x0F12,0x7000,
0x0F12,0x1120,
0x0F12,0x7000,
0x0F12,0xFFFF,
0x0F12,0x0000,
0x0F12,0x3374,
0x0F12,0x7000,
0x0F12,0x1D6C,
0x0F12,0x7000,
0x0F12,0x167C,
0x0F12,0x7000,
0x0F12,0xF400,
0x0F12,0xD000,
0x0F12,0x2C2C,
0x0F12,0x7000,
0x0F12,0x40A0,
0x0F12,0x00DD,
0x0F12,0xF520,
0x0F12,0xD000,
0x0F12,0x2C29,
0x0F12,0x7000,
0x0F12,0x1A54,
0x0F12,0x7000,
0x0F12,0x1564,
0x0F12,0x7000,
0x0F12,0xF2A0,
0x0F12,0xD000,
0x0F12,0x2440,
0x0F12,0x7000,
0x0F12,0x3274,
0x0F12,0x7000,
0x0F12,0x05A0,
0x0F12,0x7000,
0x0F12,0x2894,
0x0F12,0x7000,
0x0F12,0x1224,
0x0F12,0x7000,
0x0F12,0xB000,
0x0F12,0xD000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0x1A3F,
0x0F12,0x0001,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xF004,
0x0F12,0xE51F,
0x0F12,0x1F48,
0x0F12,0x0001,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0x24BD,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0x36DD,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0xB4CF,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0xB5D7,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0x36ED,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0xF53F,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0xF5D9,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0x013D,
0x0F12,0x0001,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0xF5C9,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0xFAA9,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0x3723,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0x5823,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0xD771,
0x0F12,0x0000,
0x0F12,0x4778,
0x0F12,0x46C0,
0x0F12,0xC000,
0x0F12,0xE59F,
0x0F12,0xFF1C,
0x0F12,0xE12F,
0x0F12,0xD75B,
0x0F12,0x0000,
0x0F12,0x7E77,
0x0F12,0x0000,
// End T&P part
//============================================================  
// CIS/APS/Analog setting        - 400LSB  SYSCLK 32MHz                                               
//============================================================  
// This registers are for FACTORY ONLY. If you change it without prior notification, 
// YOU are RESPONSIBLE for the FAILURE that will happen in the future.
//============================================================
0x0028,0x7000,                                                    
0x002A,0x157A,                                                    
0x0F12,0x0001, //#senHal_Calib_bCorrectOfs                                                    
0x002A,0x1578,                                                    
0x0F12,0x0001, //#senHal_Calib_bCorrectSlope                                                   
0x002A,0x1576,                                                    
0x0F12,0x0020, //#senHal_Calib_SigCalibRef	                                                    
0x002A,0x1574,                                                    
0x0F12,0x0006, //#senHal_Calib_RstCalibRef	                                                   
0x002A,0x156E,                                                    
0x0F12,0x0001, //#senHal_Calib_Factor_diff_thr // Slope calibration tolerance in units of 1/256   
0x002A,0x1568,                                                    
0x0F12,0x00FC, //#senHal_Calib_Ofs_Mv	 
//============================================================                                                   
//ADC control
//============================================================ 
0x002A,0x155A,                                                                                     
0x0F12,0x01CC, //#senHal_Calib_AdcSat //ADC SAT of 450mV for 10bit default in EVT1                                        
0x002A,0x157E,                                                                                     
0x0F12,0x0C80, //#senHal_AigMaxRampNormalMode// 3200 Max. Reset ramp DCLK counts (default 2048 0x800)                            
0x0F12,0x0578, //#senHal_AigMaxRampShExpMode //1400 Max. Reset ramp DCLK counts for x3.5                                        
0x002A,0x157C,                                                                                     
0x0F12,0x0190, //#senHal_AigResetForAgain1 //400 Reset ramp for x1 in DCLK counts                                             
0x002A,0x1570,                                                                                     
0x0F12,0x00A0, //#senHal_Calib_OptimalResetAvAg1 // 256 LSB                                                                          
0x0F12,0x0010, //#senHal_Calib_Ofs_diff_thr // reset threshold                                                                  
0x002A,0x12C4,                                                                                     
0x0F12,0x006A, //#senHal_uAddColsNoBin //106 additional timing columns.                                                   
0x002A,0x12C8,                                                                                    
0x0F12,0x08AC, //#senHal_uMinColsNoHorBin// 2220 ADC columns in normal mode including Hold & Latch                           
0x0F12,0x0050, //#senHal_uMinColsAddAnalogBin// 80 addition of ADC columns in Y-ave mode (default 244 0x74)                      
//WRITE #senHal_ForceModeType   0001 // Long exposure mode                                    
0x002A,0x1696,                                                                                     
0x0F12,0x0000, //#evt1_senHal_BlstEnNoBin// based on APS guidelines                                                          
0x0F12,0x0000, //#evt1_senHal_BlstEnBin// based on APS guidelines                                                          
0x0F12,0x00C6, //#evt1_senHal_AigMain1ValueNoBin// default. 1492 used for ADC dark characteristics                                  
0x0F12,0x00C6, //#evt1_senHal_AigMain1ValueBin	                                                                                    
0x002A,0x12B8,                                                                                     
0x0F12,0x0B00, //#senHal_ExpMinPixels //disable CINTR 0  
0x002A,0x1690,                                                                                     
0x0F12,0x0001, //#evt1_senHal_bDoubleSampling// when set double sampling is activated - requires different set of pointers       
0x002A,0x12B0,                                                                                     
0x0F12,0x0055, //#senHal_BiasNormal// comp and pixel bias control 0xF40E - default for EVT1                            
0x0F12,0x005A, //#senHal_BiasSfShort	// comp and pixel bias control 0xF40E for binning mode                              
0x002A,0x337A,                                                                                     
0x0F12,0x0006, //#Tune_TP_atop_dbus_reg// [7] - is used for rest-only mode (EVT0 value is 0xD and HW 0x6)                  
0x0F12,0x0068, //104M
0x002A,0x327C,                                                                                     
0x0F12,0x1000, //#Tune_TP_atop_dblr_reg_1 //[11]: Enable DBLR Regulation                                                     
0x0F12,0x6998, //#Tune_TP_atop_dblr_reg_3 //[3:0]: VPIX ~2.8V ****                                                           
0x0F12,0x0078, //#Tune_TP_atop_ramp_reg_1 //[0]: Static RC-filter                                                            
0x0F12,0x04FE, //#Tune_TP_atop_ramp_reg_2 //[7:4]: Full RC-filter                                                            
0x0F12,0x8800, //#Tune_TP_atop_rmp_offset_sig// [11]: Add load to CDS block                                                      
0x002A,0x3274,                                                                                     
0x0F12,0x0155,	//#Tune_TP_IO_DrivingCurrent_D0_D4_cs10Set IO driving current                       
0x0F12,0x0155,	//#Tune_TP_IO_DrivingCurrent_D9_D5_cs10Set IO driving current                       
0x0F12,0x1555,	//#Tune_TP_IO_DrivingCurrent_GPIO_cd10 Set IO driving current                       
0x0F12,0x0555,	//#Tune_TP_IO_DrivingCurrent_CLKs_cd10 Set IO driving current                       
0x002A,0x169E,
0x0F12,0x0007, //evt1_senHal_Dblr2Value //	 [3:0]- specifies the target (default 7)- DCLK = 64MHz instead of 116MHz. 
0x002A,0x0BF6,                                                                                     
0x0F12,0x0000, //from ALEX //Enable Bayer Downscaler                                               
//Asserting CDS pointers - Long exposure MS Normal
// Conditions: 10bit, ADC_SAT = 450mV ; ramp_del = 40 ; ramp_start = 60  
0x0028,0x7000,                                                      
0x002A,0x12D2,                                                      
0x0F12,0x0003, //#senHal_pContSenModesRegsArray[0][0]2 700012D2     
0x0F12,0x0003, //#senHal_pContSenModesRegsArray[0][1]2 700012D4     
0x0F12,0x0003, //#senHal_pContSenModesRegsArray[0][2]2 700012D6     
0x0F12,0x0003, //#senHal_pContSenModesRegsArray[0][3]2 700012D8     
0x0F12,0x0884, //#senHal_pContSenModesRegsArray[1][0]2 700012DA     
0x0F12,0x08CF, //#senHal_pContSenModesRegsArray[1][1]2 700012DC     
0x0F12,0x0500, //#senHal_pContSenModesRegsArray[1][2]2 700012DE     
0x0F12,0x054B, //#senHal_pContSenModesRegsArray[1][3]2 700012E0     
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[2][0]2 700012E2     
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[2][1]2 700012E4     
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[2][2]2 700012E6     
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[2][3]2 700012E8     
0x0F12,0x0885, //#senHal_pContSenModesRegsArray[3][0]2 700012EA     
0x0F12,0x0467, //#senHal_pContSenModesRegsArray[3][1]2 700012EC     
0x0F12,0x0501, //#senHal_pContSenModesRegsArray[3][2]2 700012EE     
0x0F12,0x02A5, //#senHal_pContSenModesRegsArray[3][3]2 700012F0     
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[4][0]2 700012F2     
0x0F12,0x046A, //#senHal_pContSenModesRegsArray[4][1]2 700012F4     
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[4][2]2 700012F6     
0x0F12,0x02A8, //#senHal_pContSenModesRegsArray[4][3]2 700012F8     
0x0F12,0x0885, //#senHal_pContSenModesRegsArray[5][0]2 700012FA     
0x0F12,0x08D0, //#senHal_pContSenModesRegsArray[5][1]2 700012FC     
0x0F12,0x0501, //#senHal_pContSenModesRegsArray[5][2]2 700012FE     
0x0F12,0x054C, //#senHal_pContSenModesRegsArray[5][3]2 70001300     
0x0F12,0x0006, //#senHal_pContSenModesRegsArray[6][0]2 70001302     
0x0F12,0x0020, //#senHal_pContSenModesRegsArray[6][1]2 70001304     
0x0F12,0x0006, //#senHal_pContSenModesRegsArray[6][2]2 70001306     
0x0F12,0x0020, //#senHal_pContSenModesRegsArray[6][3]2 70001308     
0x0F12,0x0881, //#senHal_pContSenModesRegsArray[7][0]2 7000130A     
0x0F12,0x0463, //#senHal_pContSenModesRegsArray[7][1]2 7000130C     
0x0F12,0x04FD, //#senHal_pContSenModesRegsArray[7][2]2 7000130E     
0x0F12,0x02A1, //#senHal_pContSenModesRegsArray[7][3]2 70001310     
0x0F12,0x0006, //#senHal_pContSenModesRegsArray[8][0]2 70001312     
0x0F12,0x0489, //#senHal_pContSenModesRegsArray[8][1]2 70001314     
0x0F12,0x0006, //#senHal_pContSenModesRegsArray[8][2]2 70001316     
0x0F12,0x02C7, //#senHal_pContSenModesRegsArray[8][3]2 70001318     
0x0F12,0x0881, //#senHal_pContSenModesRegsArray[9][0]2 7000131A     
0x0F12,0x08CC, //#senHal_pContSenModesRegsArray[9][1]2 7000131C     
0x0F12,0x04FD, //#senHal_pContSenModesRegsArray[9][2]2 7000131E     
0x0F12,0x0548, //#senHal_pContSenModesRegsArray[9][3]2 70001320     
0x0F12,0x03A2, //#senHal_pContSenModesRegsArray[10][0] 2 70001322   
0x0F12,0x01D3, //#senHal_pContSenModesRegsArray[10][1] 2 70001324   
0x0F12,0x01E0, //#senHal_pContSenModesRegsArray[10][2] 2 70001326   
0x0F12,0x00F2, //#senHal_pContSenModesRegsArray[10][3] 2 70001328   
0x0F12,0x03F2, //#senHal_pContSenModesRegsArray[11][0] 2 7000132A   
0x0F12,0x0223, //#senHal_pContSenModesRegsArray[11][1] 2 7000132C   
0x0F12,0x0230, //#senHal_pContSenModesRegsArray[11][2] 2 7000132E   
0x0F12,0x0142, //#senHal_pContSenModesRegsArray[11][3] 2 70001330   
0x0F12,0x03A2, //#senHal_pContSenModesRegsArray[12][0] 2 70001332   
0x0F12,0x063C, //#senHal_pContSenModesRegsArray[12][1] 2 70001334   
0x0F12,0x01E0, //#senHal_pContSenModesRegsArray[12][2] 2 70001336   
0x0F12,0x0399, //#senHal_pContSenModesRegsArray[12][3] 2 70001338   
0x0F12,0x03F2, //#senHal_pContSenModesRegsArray[13][0] 2 7000133A   
0x0F12,0x068C, //#senHal_pContSenModesRegsArray[13][1] 2 7000133C   
0x0F12,0x0230, //#senHal_pContSenModesRegsArray[13][2] 2 7000133E   
0x0F12,0x03E9, //#senHal_pContSenModesRegsArray[13][3] 2 70001340   
0x0F12,0x0002, //#senHal_pContSenModesRegsArray[14][0] 2 70001342   
0x0F12,0x0002, //#senHal_pContSenModesRegsArray[14][1] 2 70001344   
0x0F12,0x0002, //#senHal_pContSenModesRegsArray[14][2] 2 70001346   
0x0F12,0x0002, //#senHal_pContSenModesRegsArray[14][3] 2 70001348   
0x0F12,0x003C, //#senHal_pContSenModesRegsArray[15][0] 2 7000134A   
0x0F12,0x003C, //#senHal_pContSenModesRegsArray[15][1] 2 7000134C   
0x0F12,0x003C, //#senHal_pContSenModesRegsArray[15][2] 2 7000134E   
0x0F12,0x003C, //#senHal_pContSenModesRegsArray[15][3] 2 70001350   
0x0F12,0x01D3, //#senHal_pContSenModesRegsArray[16][0] 2 70001352   
0x0F12,0x01D3, //#senHal_pContSenModesRegsArray[16][1] 2 70001354   
0x0F12,0x00F2, //#senHal_pContSenModesRegsArray[16][2] 2 70001356   
0x0F12,0x00F2, //#senHal_pContSenModesRegsArray[16][3] 2 70001358   
0x0F12,0x020B, //#senHal_pContSenModesRegsArray[17][0] 2 7000135A   
0x0F12,0x024A, //#senHal_pContSenModesRegsArray[17][1] 2 7000135C   
0x0F12,0x012A, //#senHal_pContSenModesRegsArray[17][2] 2 7000135E   
0x0F12,0x0169, //#senHal_pContSenModesRegsArray[17][3] 2 70001360   
0x0F12,0x0002, //#senHal_pContSenModesRegsArray[18][0] 2 70001362   
0x0F12,0x046B, //#senHal_pContSenModesRegsArray[18][1] 2 70001364   
0x0F12,0x0002, //#senHal_pContSenModesRegsArray[18][2] 2 70001366   
0x0F12,0x02A9, //#senHal_pContSenModesRegsArray[18][3] 2 70001368   
0x0F12,0x0419, //#senHal_pContSenModesRegsArray[19][0] 2 7000136A   
0x0F12,0x04A5, //#senHal_pContSenModesRegsArray[19][1] 2 7000136C   
0x0F12,0x0257, //#senHal_pContSenModesRegsArray[19][2] 2 7000136E   
0x0F12,0x02E3, //#senHal_pContSenModesRegsArray[19][3] 2 70001370   
0x0F12,0x0630, //#senHal_pContSenModesRegsArray[20][0] 2 70001372   
0x0F12,0x063C, //#senHal_pContSenModesRegsArray[20][1] 2 70001374   
0x0F12,0x038D, //#senHal_pContSenModesRegsArray[20][2] 2 70001376   
0x0F12,0x0399, //#senHal_pContSenModesRegsArray[20][3] 2 70001378   
0x0F12,0x0668, //#senHal_pContSenModesRegsArray[21][0] 2 7000137A   
0x0F12,0x06B3, //#senHal_pContSenModesRegsArray[21][1] 2 7000137C   
0x0F12,0x03C5, //#senHal_pContSenModesRegsArray[21][2] 2 7000137E   
0x0F12,0x0410, //#senHal_pContSenModesRegsArray[21][3] 2 70001380   
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[22][0] 2 70001382   
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[22][1] 2 70001384   
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[22][2] 2 70001386   
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[22][3] 2 70001388   
0x0F12,0x03A2, //#senHal_pContSenModesRegsArray[23][0] 2 7000138A   
0x0F12,0x01D3, //#senHal_pContSenModesRegsArray[23][1] 2 7000138C   
0x0F12,0x01E0, //#senHal_pContSenModesRegsArray[23][2] 2 7000138E   
0x0F12,0x00F2, //#senHal_pContSenModesRegsArray[23][3] 2 70001390   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[24][0] 2 70001392   
0x0F12,0x0461, //#senHal_pContSenModesRegsArray[24][1] 2 70001394   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[24][2] 2 70001396   
0x0F12,0x029F, //#senHal_pContSenModesRegsArray[24][3] 2 70001398   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[25][0] 2 7000139A   
0x0F12,0x063C, //#senHal_pContSenModesRegsArray[25][1] 2 7000139C   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[25][2] 2 7000139E   
0x0F12,0x0399, //#senHal_pContSenModesRegsArray[25][3] 2 700013A0   
0x0F12,0x003D, //#senHal_pContSenModesRegsArray[26][0] 2 700013A2   
0x0F12,0x003D, //#senHal_pContSenModesRegsArray[26][1] 2 700013A4   
0x0F12,0x003D, //#senHal_pContSenModesRegsArray[26][2] 2 700013A6   
0x0F12,0x003D, //#senHal_pContSenModesRegsArray[26][3] 2 700013A8   
0x0F12,0x01D0, //#senHal_pContSenModesRegsArray[27][0] 2 700013AA   
0x0F12,0x01D0, //#senHal_pContSenModesRegsArray[27][1] 2 700013AC   
0x0F12,0x00EF, //#senHal_pContSenModesRegsArray[27][2] 2 700013AE   
0x0F12,0x00EF, //#senHal_pContSenModesRegsArray[27][3] 2 700013B0   
0x0F12,0x020C, //#senHal_pContSenModesRegsArray[28][0] 2 700013B2   
0x0F12,0x024B, //#senHal_pContSenModesRegsArray[28][1] 2 700013B4   
0x0F12,0x012B, //#senHal_pContSenModesRegsArray[28][2] 2 700013B6   
0x0F12,0x016A, //#senHal_pContSenModesRegsArray[28][3] 2 700013B8   
0x0F12,0x039F, //#senHal_pContSenModesRegsArray[29][0] 2 700013BA   
0x0F12,0x045E, //#senHal_pContSenModesRegsArray[29][1] 2 700013BC   
0x0F12,0x01DD, //#senHal_pContSenModesRegsArray[29][2] 2 700013BE   
0x0F12,0x029C, //#senHal_pContSenModesRegsArray[29][3] 2 700013C0   
0x0F12,0x041A, //#senHal_pContSenModesRegsArray[30][0] 2 700013C2   
0x0F12,0x04A6, //#senHal_pContSenModesRegsArray[30][1] 2 700013C4   
0x0F12,0x0258, //#senHal_pContSenModesRegsArray[30][2] 2 700013C6   
0x0F12,0x02E4, //#senHal_pContSenModesRegsArray[30][3] 2 700013C8   
0x0F12,0x062D, //#senHal_pContSenModesRegsArray[31][0] 2 700013CA   
0x0F12,0x0639, //#senHal_pContSenModesRegsArray[31][1] 2 700013CC   
0x0F12,0x038A, //#senHal_pContSenModesRegsArray[31][2] 2 700013CE   
0x0F12,0x0396, //#senHal_pContSenModesRegsArray[31][3] 2 700013D0   
0x0F12,0x0669, //#senHal_pContSenModesRegsArray[32][0] 2 700013D2   
0x0F12,0x06B4, //#senHal_pContSenModesRegsArray[32][1] 2 700013D4   
0x0F12,0x03C6, //#senHal_pContSenModesRegsArray[32][2] 2 700013D6   
0x0F12,0x0411, //#senHal_pContSenModesRegsArray[32][3] 2 700013D8   
0x0F12,0x087C, //#senHal_pContSenModesRegsArray[33][0] 2 700013DA   
0x0F12,0x08C7, //#senHal_pContSenModesRegsArray[33][1] 2 700013DC   
0x0F12,0x04F8, //#senHal_pContSenModesRegsArray[33][2] 2 700013DE   
0x0F12,0x0543, //#senHal_pContSenModesRegsArray[33][3] 2 700013E0   
0x0F12,0x0040, //#senHal_pContSenModesRegsArray[34][0] 2 700013E2   
0x0F12,0x0040, //#senHal_pContSenModesRegsArray[34][1] 2 700013E4   
0x0F12,0x0040, //#senHal_pContSenModesRegsArray[34][2] 2 700013E6   
0x0F12,0x0040, //#senHal_pContSenModesRegsArray[34][3] 2 700013E8   
0x0F12,0x01D0, //#senHal_pContSenModesRegsArray[35][0] 2 700013EA   
0x0F12,0x01D0, //#senHal_pContSenModesRegsArray[35][1] 2 700013EC   
0x0F12,0x00EF, //#senHal_pContSenModesRegsArray[35][2] 2 700013EE   
0x0F12,0x00EF, //#senHal_pContSenModesRegsArray[35][3] 2 700013F0   
0x0F12,0x020F, //#senHal_pContSenModesRegsArray[36][0] 2 700013F2   
0x0F12,0x024E, //#senHal_pContSenModesRegsArray[36][1] 2 700013F4   
0x0F12,0x012E, //#senHal_pContSenModesRegsArray[36][2] 2 700013F6   
0x0F12,0x016D, //#senHal_pContSenModesRegsArray[36][3] 2 700013F8   
0x0F12,0x039F, //#senHal_pContSenModesRegsArray[37][0] 2 700013FA   
0x0F12,0x045E, //#senHal_pContSenModesRegsArray[37][1] 2 700013FC   
0x0F12,0x01DD, //#senHal_pContSenModesRegsArray[37][2] 2 700013FE   
0x0F12,0x029C, //#senHal_pContSenModesRegsArray[37][3] 2 70001400   
0x0F12,0x041D, //#senHal_pContSenModesRegsArray[38][0] 2 70001402   
0x0F12,0x04A9, //#senHal_pContSenModesRegsArray[38][1] 2 70001404   
0x0F12,0x025B, //#senHal_pContSenModesRegsArray[38][2] 2 70001406   
0x0F12,0x02E7, //#senHal_pContSenModesRegsArray[38][3] 2 70001408   
0x0F12,0x062D, //#senHal_pContSenModesRegsArray[39][0] 2 7000140A   
0x0F12,0x0639, //#senHal_pContSenModesRegsArray[39][1] 2 7000140C   
0x0F12,0x038A, //#senHal_pContSenModesRegsArray[39][2] 2 7000140E   
0x0F12,0x0396, //#senHal_pContSenModesRegsArray[39][3] 2 70001410   
0x0F12,0x066C, //#senHal_pContSenModesRegsArray[40][0] 2 70001412   
0x0F12,0x06B7, //#senHal_pContSenModesRegsArray[40][1] 2 70001414   
0x0F12,0x03C9, //#senHal_pContSenModesRegsArray[40][2] 2 70001416   
0x0F12,0x0414, //#senHal_pContSenModesRegsArray[40][3] 2 70001418   
0x0F12,0x087C, //#senHal_pContSenModesRegsArray[41][0] 2 7000141A   
0x0F12,0x08C7, //#senHal_pContSenModesRegsArray[41][1] 2 7000141C   
0x0F12,0x04F8, //#senHal_pContSenModesRegsArray[41][2] 2 7000141E   
0x0F12,0x0543, //#senHal_pContSenModesRegsArray[41][3] 2 70001420   
0x0F12,0x0040, //#senHal_pContSenModesRegsArray[42][0] 2 70001422   
0x0F12,0x0040, //#senHal_pContSenModesRegsArray[42][1] 2 70001424   
0x0F12,0x0040, //#senHal_pContSenModesRegsArray[42][2] 2 70001426   
0x0F12,0x0040, //#senHal_pContSenModesRegsArray[42][3] 2 70001428   
0x0F12,0x01D0, //#senHal_pContSenModesRegsArray[43][0] 2 7000142A   
0x0F12,0x01D0, //#senHal_pContSenModesRegsArray[43][1] 2 7000142C   
0x0F12,0x00EF, //#senHal_pContSenModesRegsArray[43][2] 2 7000142E   
0x0F12,0x00EF, //#senHal_pContSenModesRegsArray[43][3] 2 70001430   
0x0F12,0x020F, //#senHal_pContSenModesRegsArray[44][0] 2 70001432   
0x0F12,0x024E, //#senHal_pContSenModesRegsArray[44][1] 2 70001434   
0x0F12,0x012E, //#senHal_pContSenModesRegsArray[44][2] 2 70001436   
0x0F12,0x016D, //#senHal_pContSenModesRegsArray[44][3] 2 70001438   
0x0F12,0x039F, //#senHal_pContSenModesRegsArray[45][0] 2 7000143A   
0x0F12,0x045E, //#senHal_pContSenModesRegsArray[45][1] 2 7000143C   
0x0F12,0x01DD, //#senHal_pContSenModesRegsArray[45][2] 2 7000143E   
0x0F12,0x029C, //#senHal_pContSenModesRegsArray[45][3] 2 70001440   
0x0F12,0x041D, //#senHal_pContSenModesRegsArray[46][0] 2 70001442   
0x0F12,0x04A9, //#senHal_pContSenModesRegsArray[46][1] 2 70001444   
0x0F12,0x025B, //#senHal_pContSenModesRegsArray[46][2] 2 70001446   
0x0F12,0x02E7, //#senHal_pContSenModesRegsArray[46][3] 2 70001448   
0x0F12,0x062D, //#senHal_pContSenModesRegsArray[47][0] 2 7000144A   
0x0F12,0x0639, //#senHal_pContSenModesRegsArray[47][1] 2 7000144C   
0x0F12,0x038A, //#senHal_pContSenModesRegsArray[47][2] 2 7000144E   
0x0F12,0x0396, //#senHal_pContSenModesRegsArray[47][3] 2 70001450   
0x0F12,0x066C, //#senHal_pContSenModesRegsArray[48][0] 2 70001452   
0x0F12,0x06B7, //#senHal_pContSenModesRegsArray[48][1] 2 70001454   
0x0F12,0x03C9, //#senHal_pContSenModesRegsArray[48][2] 2 70001456   
0x0F12,0x0414, //#senHal_pContSenModesRegsArray[48][3] 2 70001458   
0x0F12,0x087C, //#senHal_pContSenModesRegsArray[49][0] 2 7000145A   
0x0F12,0x08C7, //#senHal_pContSenModesRegsArray[49][1] 2 7000145C   
0x0F12,0x04F8, //#senHal_pContSenModesRegsArray[49][2] 2 7000145E   
0x0F12,0x0543, //#senHal_pContSenModesRegsArray[49][3] 2 70001460   
0x0F12,0x003D, //#senHal_pContSenModesRegsArray[50][0] 2 70001462   
0x0F12,0x003D, //#senHal_pContSenModesRegsArray[50][1] 2 70001464   
0x0F12,0x003D, //#senHal_pContSenModesRegsArray[50][2] 2 70001466   
0x0F12,0x003D, //#senHal_pContSenModesRegsArray[50][3] 2 70001468   
0x0F12,0x01D2, //#senHal_pContSenModesRegsArray[51][0] 2 7000146A   
0x0F12,0x01D2, //#senHal_pContSenModesRegsArray[51][1] 2 7000146C   
0x0F12,0x00F1, //#senHal_pContSenModesRegsArray[51][2] 2 7000146E   
0x0F12,0x00F1, //#senHal_pContSenModesRegsArray[51][3] 2 70001470   
0x0F12,0x020C, //#senHal_pContSenModesRegsArray[52][0] 2 70001472   
0x0F12,0x024B, //#senHal_pContSenModesRegsArray[52][1] 2 70001474   
0x0F12,0x012B, //#senHal_pContSenModesRegsArray[52][2] 2 70001476   
0x0F12,0x016A, //#senHal_pContSenModesRegsArray[52][3] 2 70001478   
0x0F12,0x03A1, //#senHal_pContSenModesRegsArray[53][0] 2 7000147A   
0x0F12,0x0460, //#senHal_pContSenModesRegsArray[53][1] 2 7000147C   
0x0F12,0x01DF, //#senHal_pContSenModesRegsArray[53][2] 2 7000147E   
0x0F12,0x029E, //#senHal_pContSenModesRegsArray[53][3] 2 70001480   
0x0F12,0x041A, //#senHal_pContSenModesRegsArray[54][0] 2 70001482   
0x0F12,0x04A6, //#senHal_pContSenModesRegsArray[54][1] 2 70001484   
0x0F12,0x0258, //#senHal_pContSenModesRegsArray[54][2] 2 70001486   
0x0F12,0x02E4, //#senHal_pContSenModesRegsArray[54][3] 2 70001488   
0x0F12,0x062F, //#senHal_pContSenModesRegsArray[55][0] 2 7000148A   
0x0F12,0x063B, //#senHal_pContSenModesRegsArray[55][1] 2 7000148C   
0x0F12,0x038C, //#senHal_pContSenModesRegsArray[55][2] 2 7000148E   
0x0F12,0x0398, //#senHal_pContSenModesRegsArray[55][3] 2 70001490   
0x0F12,0x0669, //#senHal_pContSenModesRegsArray[56][0] 2 70001492   
0x0F12,0x06B4, //#senHal_pContSenModesRegsArray[56][1] 2 70001494   
0x0F12,0x03C6, //#senHal_pContSenModesRegsArray[56][2] 2 70001496   
0x0F12,0x0411, //#senHal_pContSenModesRegsArray[56][3] 2 70001498   
0x0F12,0x087E, //#senHal_pContSenModesRegsArray[57][0] 2 7000149A   
0x0F12,0x08C9, //#senHal_pContSenModesRegsArray[57][1] 2 7000149C   
0x0F12,0x04FA, //#senHal_pContSenModesRegsArray[57][2] 2 7000149E   
0x0F12,0x0545, //#senHal_pContSenModesRegsArray[57][3] 2 700014A0   
0x0F12,0x03A2, //#senHal_pContSenModesRegsArray[58][0] 2 700014A2   
0x0F12,0x01D3, //#senHal_pContSenModesRegsArray[58][1] 2 700014A4   
0x0F12,0x01E0, //#senHal_pContSenModesRegsArray[58][2] 2 700014A6   
0x0F12,0x00F2, //#senHal_pContSenModesRegsArray[58][3] 2 700014A8   
0x0F12,0x03AF, //#senHal_pContSenModesRegsArray[59][0] 2 700014AA   
0x0F12,0x01E0, //#senHal_pContSenModesRegsArray[59][1] 2 700014AC   
0x0F12,0x01ED, //#senHal_pContSenModesRegsArray[59][2] 2 700014AE   
0x0F12,0x00FF, //#senHal_pContSenModesRegsArray[59][3] 2 700014B0   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[60][0] 2 700014B2   
0x0F12,0x0461, //#senHal_pContSenModesRegsArray[60][1] 2 700014B4   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[60][2] 2 700014B6   
0x0F12,0x029F, //#senHal_pContSenModesRegsArray[60][3] 2 700014B8   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[61][0] 2 700014BA   
0x0F12,0x046E, //#senHal_pContSenModesRegsArray[61][1] 2 700014BC   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[61][2] 2 700014BE   
0x0F12,0x02AC, //#senHal_pContSenModesRegsArray[61][3] 2 700014C0   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[62][0] 2 700014C2   
0x0F12,0x063C, //#senHal_pContSenModesRegsArray[62][1] 2 700014C4   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[62][2] 2 700014C6   
0x0F12,0x0399, //#senHal_pContSenModesRegsArray[62][3] 2 700014C8   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[63][0] 2 700014CA   
0x0F12,0x0649, //#senHal_pContSenModesRegsArray[63][1] 2 700014CC   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[63][2] 2 700014CE   
0x0F12,0x03A6, //#senHal_pContSenModesRegsArray[63][3] 2 700014D0   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[64][0] 2 700014D2   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[64][1] 2 700014D4   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[64][2] 2 700014D6   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[64][3] 2 700014D8   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[65][0] 2 700014DA   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[65][1] 2 700014DC   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[65][2] 2 700014DE   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[65][3] 2 700014E0   
0x0F12,0x03AA, //#senHal_pContSenModesRegsArray[66][0] 2 700014E2   
0x0F12,0x01DB, //#senHal_pContSenModesRegsArray[66][1] 2 700014E4   
0x0F12,0x01E8, //#senHal_pContSenModesRegsArray[66][2] 2 700014E6   
0x0F12,0x00FA, //#senHal_pContSenModesRegsArray[66][3] 2 700014E8   
0x0F12,0x03B7, //#senHal_pContSenModesRegsArray[67][0] 2 700014EA   
0x0F12,0x01E8, //#senHal_pContSenModesRegsArray[67][1] 2 700014EC   
0x0F12,0x01F5, //#senHal_pContSenModesRegsArray[67][2] 2 700014EE   
0x0F12,0x0107, //#senHal_pContSenModesRegsArray[67][3] 2 700014F0   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[68][0] 2 700014F2   
0x0F12,0x0469, //#senHal_pContSenModesRegsArray[68][1] 2 700014F4   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[68][2] 2 700014F6   
0x0F12,0x02A7, //#senHal_pContSenModesRegsArray[68][3] 2 700014F8   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[69][0] 2 700014FA   
0x0F12,0x0476, //#senHal_pContSenModesRegsArray[69][1] 2 700014FC   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[69][2] 2 700014FE   
0x0F12,0x02B4, //#senHal_pContSenModesRegsArray[69][3] 2 70001500   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[70][0] 2 70001502   
0x0F12,0x0644, //#senHal_pContSenModesRegsArray[70][1] 2 70001504   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[70][2] 2 70001506   
0x0F12,0x03A1, //#senHal_pContSenModesRegsArray[70][3] 2 70001508   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[71][0] 2 7000150A   
0x0F12,0x0651, //#senHal_pContSenModesRegsArray[71][1] 2 7000150C   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[71][2] 2 7000150E   
0x0F12,0x03AE, //#senHal_pContSenModesRegsArray[71][3] 2 70001510   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[72][0] 2 70001512   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[72][1] 2 70001514   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[72][2] 2 70001516   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[72][3] 2 70001518   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[73][0] 2 7000151A   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[73][1] 2 7000151C   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[73][2] 2 7000151E   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[73][3] 2 70001520   
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[74][0] 2 70001522   
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[74][1] 2 70001524   
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[74][2] 2 70001526   
0x0F12,0x0001, //#senHal_pContSenModesRegsArray[74][3] 2 70001528   
0x0F12,0x000F, //#senHal_pContSenModesRegsArray[75][0] 2 7000152A   
0x0F12,0x000F, //#senHal_pContSenModesRegsArray[75][1] 2 7000152C   
0x0F12,0x000F, //#senHal_pContSenModesRegsArray[75][2] 2 7000152E   
0x0F12,0x000F, //#senHal_pContSenModesRegsArray[75][3] 2 70001530   
0x0F12,0x05AD, //#senHal_pContSenModesRegsArray[76][0] 2 70001532   
0x0F12,0x03DE, //#senHal_pContSenModesRegsArray[76][1] 2 70001534   
0x0F12,0x030A, //#senHal_pContSenModesRegsArray[76][2] 2 70001536   
0x0F12,0x021C, //#senHal_pContSenModesRegsArray[76][3] 2 70001538   
0x0F12,0x062F, //#senHal_pContSenModesRegsArray[77][0] 2 7000153A   
0x0F12,0x0460, //#senHal_pContSenModesRegsArray[77][1] 2 7000153C   
0x0F12,0x038C, //#senHal_pContSenModesRegsArray[77][2] 2 7000153E   
0x0F12,0x029E, //#senHal_pContSenModesRegsArray[77][3] 2 70001540   
0x0F12,0x07FC, //#senHal_pContSenModesRegsArray[78][0] 2 70001542   
0x0F12,0x0847, //#senHal_pContSenModesRegsArray[78][1] 2 70001544   
0x0F12,0x0478, //#senHal_pContSenModesRegsArray[78][2] 2 70001546   
0x0F12,0x04C3, //#senHal_pContSenModesRegsArray[78][3] 2 70001548   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[79][0] 2 7000154A   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[79][1] 2 7000154C   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[79][2] 2 7000154E   
0x0F12,0x0000, //#senHal_pContSenModesRegsArray[79][3] 2 70001550   
                                                               
//============================================================  
// Analog Setting END                                                          
//============================================================  
//============================================================                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
// AF Interface setting                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
//============================================================                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
0x0028,0x7000,
0x002A,0x01D4,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x0000, //REG_TC_IPRM_AuxGpios : 0 - no Flash                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
0x002A,0x01DE,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x0003, //REG_TC_IPRM_CM_Init_AfModeType : 3 - AFD_VCM_I2C                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
0x0F12,0x0000, //REG_TC_IPRM_CM_Init_PwmConfig1 : 0 - no PWM                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
0x002A,0x01E4,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x0041,//REG_TC_IPRM_CM_Init_GpioConfig1 : 1 -  GPIO4                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x002A,0x01E8,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x2A0C, //REG_TC_IPRM_CM_Init_Mi2cBits : MSCL - GPIO5 MSDA - GPIO6 Device ID (0C)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
0x0F12,0x0190, //REG_TC_IPRM_CM_Init_Mi2cRateKhz : MI2C Speed - 400KHz                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
0x002A,0x1196,  //afd_usFlags   0100=low,0000=high                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
0x0F12,0x0000,                                    
//============================================================                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
// AF Parameter setting                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
//============================================================                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
// AF Window Settings                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
0x002A,0x025A,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x0100, //#REG_TC_AF_FstWinStartX                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
0x0F12,0x00E3, //#REG_TC_AF_FstWinStartY                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
0x0F12,0x0200, //#REG_TC_AF_FstWinSizeX                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
0x0F12,0x0238, //#REG_TC_AF_FstWinSizeY                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
0x0F12,0x018C, //#REG_TC_AF_ScndWinStartX                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
0x0F12,0x0166, //#REG_TC_AF_ScndWinStartY                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
0x0F12,0x00E6, //#REG_TC_AF_ScndWinSizeX                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
0x0F12,0x0132, //#REG_TC_AF_ScndWinSizeY                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
0x0F12,0x0001, //#REG_TC_AF_WinSizesUpdated                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
// AF Setot Settings                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
0x002A,0x0586,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x00FF, //#skl_af_StatOvlpExpFactor                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
// AF Scene Settings                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
0x002A,0x115E,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x0003, //#af_scene_usSaturatedScene                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
// AF Fine Search Settings                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
0x002A,0x10D4,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x1000, //FineSearch Disable //#af_search_usSingleAfFlags                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
0x002A,0x10DE,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x0004, //#af_search_usFinePeakCount                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
0x002A,0x106C,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x0202, //#af_pos_usFineStepNumSize                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
// AF Peak Threshold Setting                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
0x002A,0x10CA, //#af_search_usPeakThr                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
0x0F12,0x00C0,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
// AF ,0xDefault Position                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
0x002A,0x1060,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x003C, //#af_pos_usHomePos                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
0x0F12,0x783C, //#af_pos_usLowConfPos                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
// AF ,0xLowConfThr Setting                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
0x002A,0x10F4, //LowEdgeBoth GRAD                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
0x0F12,0x0280,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x002A,0x1100, //LowLight HPF                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
0x0F12,0x03A0,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x0320,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
//                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
0x002A,0x1134,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
0x0F12,0x0030, //af_stat_usMinStatVal                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
// AF ,0xlow Br Th                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
0x002A,0x1154, // normBrThr                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
0x0F12,0x0060,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
// AF ,0xPolicy                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
0x002A,0x10E2,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x0000, //#af_search_usCapturePolicy: Focus_Priority, 0002 : Shutter_Priority_Fixed, 0001 : Shutter_Priority_Last_BFP 0000: Shutter_Priority_Current                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
0x002A,0x1072,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x003C, //#af_pos_usCaptureFixedPos// 0x0008                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
// AF Lens Position Table Settings                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
0x002A,0x1074,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x000D,    //0010 //#af_pos_usTableLastInd// 16 Steps 091222                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
0x0F12,0x0020,    //003C //#af_pos_usTable_0_// af_pos_usTable                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
0x0F12,0x0028,    //003F //#af_pos_usTable_1_     
0x0F12,0x0030,    //0042 //#af_pos_usTable_2_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
0x0F12,0x0038,    //0045 //#af_pos_usTable_3_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
0x0F12,0x003E,    //0048 //#af_pos_usTable_4_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
0x0F12,0x0044,    //004B //#af_pos_usTable_5_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
0x0F12,0x004A,    //004E //#af_pos_usTable_6_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
0x0F12,0x0050,    //0051 //#af_pos_usTable_7_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
0x0F12,0x0056,    //0054 //#af_pos_usTable_8_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
0x0F12,0x005C,    //0057 //#af_pos_usTable_9_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
0x0F12,0x0062,    //005A //#af_pos_usTable_10_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
0x0F12,0x0068,    //005E //#af_pos_usTable_11_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
0x0F12,0x0078,    //0061 //#af_pos_usTable_12_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
0x0F12,0x008A,    //0064 //#af_pos_usTable_13_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
//0F12       0064 //#af_pos_usTable_13_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
//0F12       0068 //#af_pos_usTable_14_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
//0F12       006C //#af_pos_usTable_15_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
//0F12       0078 //#af_pos_usTable_16_                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    


0x002A,0x0252,                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
0x0F12,0x0003, //init                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
//============================================================  
// ISP-FE Setting                                                          
//============================================================  
0x002A,0x158A,                  
0x0F12,0xEAF0, //#gisp_dadlc_config                 
0x002A,0x15C6,                  
0x0F12,0x0020, //#gisp_dcdstat_LeftColW88		                   
0x0F12,0x0060, // #gisp_dcdstat_RightColW88		               
0x002A,0x15BC,                  
0x0F12,0x0200, //gisp_dcdstat_Pedestal		// added by Shy.
//                          
0x002A,0x1608,                  
0x0F12,0x0100,  //gisp_msm_sAnalogOffset[0]                
0x0F12,0x0100,  //gisp_msm_sAnalogOffset[1]                
0x0F12,0x0100,  //gisp_msm_sAnalogOffset[2]                
0x0F12,0x0100,  //gisp_msm_sAnalogOffset[3]                
//============================================================  
// ISP-FE Setting END                                                          
//============================================================  
//============================================================    
// Frame rate setting                                                           
//============================================================    
//	How to set
//	1. Exposure value
//			dec2hex((1 / (frame rate you want(ms))) * 100d * 4d)
//	2. Analog Digital gain
//			dec2hex((Analog gain you want) * 256d)
//============================================================
// Set preview exposure time           
0x002A,0x0530,                              
0x0F12,0x5DC0, //#lt_uMaxExp1 60ms          
0x0F12,0x0000,                              
0x0F12,0x6D60, //#lt_uMaxExp2 70ms          
0x0F12,0x0000,                             
0x002A,0x167C,                              
0x0F12,0x9C40, //#evt1_lt_uMaxExp3 100ms    
0x0F12,0x0000,                              
0x0F12,0x3880,//BB80 //#evt1_lt_uMaxExp4 120ms    
0x0F12,0x0001,//0000                              
// Set capture exposure time           
0x002A,0x0538,                              
0x0F12,0x5DC0, //#lt_uCapMaxExp160ms        
0x0F12,0x0000,                              
0x0F12,0x6D60, //#lt_uCapMaxExp270ms        
0x0F12,0x0000,                              
0x002A,0x1684,                              
0x0F12,0x9C40, //#evt1_lt_uCapMaxExp3 100ms 
0x0F12,0x0000,                              
0x0F12,0xBB80, //#evt1_lt_uCapMaxExp4 120ms 
0x0F12,0x0000,                              
// Set gain                         
0x002A,0x0540,                           
0x0F12,0x0150, //#lt_uMaxAnGain1         
0x0F12,0x0280, //#lt_uMaxAnGain2         
0x002A,0x168C,                           
0x0F12,0x02A0, //#evt1_lt_uMaxAnGain3    
0x0F12,0x0800, //#evt1_lt_uMaxAnGain4    
                      
0x002A,0x0544,                           
0x0F12,0x0100, //#lt_uMaxDigGain         
0x0F12,0x0A00, //#lt_uMaxTotGain         
      
0x002A,0x1694,                                 
0x0F12,0x0001, //#evt1_senHal_bExpandForbid    //expand forbidde zone

//002A 051A                      
//0F12 0111 //#lt_uLimitHigh          
//0F12 00F0 //#lt_uLimitLow     

//============================================================           
//Tuning part                          
//============================================================  
//Calibrations go here
//============================================================
//Anti-Shading
//============================================================
//	param_start	TVAR_ash_GASAlpha
0x0028,0x7000,
0x002A,0x06CE,
0x0F12,0x00FE,	//TVAR_ash_GASAlpha[0]
0x0F12,0x00E0,	//TVAR_ash_GASAlpha[1]
0x0F12,0x00F0,	//TVAR_ash_GASAlpha[2]
0x0F12,0x00FE,	//TVAR_ash_GASAlpha[3]
0x0F12,0x00F0,	//TVAR_ash_GASAlpha[4]
0x0F12,0x00E0,	//TVAR_ash_GASAlpha[5]
0x0F12,0x00C0,	//TVAR_ash_GASAlpha[6]
0x0F12,0x00E0,	//TVAR_ash_GASAlpha[7]
0x0F12,0x00EF,	//TVAR_ash_GASAlpha[8]
0x0F12,0x00FF,	//TVAR_ash_GASAlpha[9]
0x0F12,0x00F9,	//TVAR_ash_GASAlpha[10]
0x0F12,0x00E0,	//TVAR_ash_GASAlpha[11]
0x0F12,0x00CE,	//TVAR_ash_GASAlpha[12]
0x0F12,0x00FF,	//TVAR_ash_GASAlpha[13]
0x0F12,0x0100,	//TVAR_ash_GASAlpha[14]
0x0F12,0x00E0,	//TVAR_ash_GASAlpha[15]
0x0F12,0x00DF,	//TVAR_ash_GASAlpha[16]
0x0F12,0x00F4,	//TVAR_ash_GASAlpha[17]
0x0F12,0x00F3,	//TVAR_ash_GASAlpha[18]
0x0F12,0x00F5,	//TVAR_ash_GASAlpha[19]
0x0F12,0x00E0, //TVAR_ash_GASAlpha[20]
0x0F12,0x00E9,	//TVAR_ash_GASAlpha[21]
0x0F12,0x00E6,	//TVAR_ash_GASAlpha[22]
0x0F12,0x00F6,	//TVAR_ash_GASAlpha[23]
0x0F12,0x00CE,	//TVAR_ash_GASAlpha[24]
0x0F12,0x00E9,	//TVAR_ash_GASAlpha[25]
0x0F12,0x00E6,	//TVAR_ash_GASAlpha[26]
0x0F12,0x00F6,	//TVAR_ash_GASAlpha[27]
//	param_end	TVAR_ash_GASAlpha
//	param_start	TVAR_ash_GASOutdoorAlpha 
0x002A,0x0706,
0x0F12,0x00E4,	//TVAR_ash_GASOutdoorAlpha[0]
0x0F12,0x00E9,	//TVAR_ash_GASOutdoorAlpha[1]
0x0F12,0x00E6,	//TVAR_ash_GASOutdoorAlpha[2]
0x0F12,0x00F6,	//TVAR_ash_GASOutdoorAlpha[3]
//	param_end	TVAR_ash_GASOutdoorAlpha
//	param_start	ash_GASBeta
0x002A,0x070E,
0x0F12,0x0060,	//ash_GASBeta[0]
0x0F12,0x002B,	//ash_GASBeta[1]
0x0F12,0x002F,	//ash_GASBeta[2]
0x0F12,0x0000,	//ash_GASBeta[3]
0x0F12,0x0052,	//ash_GASBeta[4]
0x0F12,0x0026,	//ash_GASBeta[5]
0x0F12,0x0029,	//ash_GASBeta[6]
0x0F12,0x0000,	//ash_GASBeta[7]
0x0F12,0x0010,	//ash_GASBeta[8]
0x0F12,0x0026,	//ash_GASBeta[9]
0x0F12,0x0028,	//ash_GASBeta[10]
0x0F12,0x0000,	//ash_GASBeta[11]
0x0F12,0x0010,	//ash_GASBeta[12]
0x0F12,0x0026,	//ash_GASBeta[13]
0x0F12,0x0027,	//ash_GASBeta[14]
0x0F12,0x0000,	//ash_GASBeta[15]
0x0F12,0x0023,	//ash_GASBeta[16]
0x0F12,0x001F,	//ash_GASBeta[17]
0x0F12,0x001F,	//ash_GASBeta[18]
0x0F12,0x0000,	//ash_GASBeta[19]
0x0F12,0x0025,	//ash_GASBeta[20]
0x0F12,0x0017,	//ash_GASBeta[21]
0x0F12,0x0016,	//ash_GASBeta[22]
0x0F12,0x0000,	//ash_GASBeta[23]
0x0F12,0x0025,	//ash_GASBeta[24]
0x0F12,0x0017,	//ash_GASBeta[25]
0x0F12,0x0016,	//ash_GASBeta[26]
0x0F12,0x0000,	//ash_GASBeta[27]
//	param_end	ash_GASBeta
//	param_start	ash_GASOutdoorBeta 
0x002A,0x0746,
0x0F12,0x0025,	//ash_GASOutdoorBeta[0]
0x0F12,0x0017,	//ash_GASOutdoorBeta[1]
0x0F12,0x0016,	//ash_GASOutdoorBeta[2]
0x0F12,0x0000,	//ash_GASOutdoorBeta[3]
//	param_end	ash_GASOutdoorBeta
0x002A,0x075A,
0x0F12,0x0000,	//ash_bParabolicEstimation
0x0F12,0x0400,	//ash_uParabolicCenterX
0x0F12,0x0300,	//ash_uParabolicCenterY
0x0F12,0x0010,	//ash_uParabolicScalingA
0x0F12,0x0011,	//ash_uParabolicScalingB
//	param_start	TVAR_ash_pGAS
0x002A,0x347C,
0x0F12,0x025B,	//TVAR_ash_pGAS[0]
0x0F12,0x01B4,	//TVAR_ash_pGAS[1]
0x0F12,0x0156,	//TVAR_ash_pGAS[2]
0x0F12,0x0112,	//TVAR_ash_pGAS[3]
0x0F12,0x00EE,	//TVAR_ash_pGAS[4]
0x0F12,0x00D8,	//TVAR_ash_pGAS[5]
0x0F12,0x00CE,	//TVAR_ash_pGAS[6]
0x0F12,0x00D4,	//TVAR_ash_pGAS[7]
0x0F12,0x00E6,	//TVAR_ash_pGAS[8]
0x0F12,0x010B,	//TVAR_ash_pGAS[9]
0x0F12,0x0141,	//TVAR_ash_pGAS[10]
0x0F12,0x019F,	//TVAR_ash_pGAS[11]
0x0F12,0x027A,	//TVAR_ash_pGAS[12]
0x0F12,0x01CD,	//TVAR_ash_pGAS[13]
0x0F12,0x0174,	//TVAR_ash_pGAS[14]
0x0F12,0x0115,	//TVAR_ash_pGAS[15]
0x0F12,0x00D8,	//TVAR_ash_pGAS[16]
0x0F12,0x00B2,	//TVAR_ash_pGAS[17]
0x0F12,0x009C,	//TVAR_ash_pGAS[18]
0x0F12,0x0092,	//TVAR_ash_pGAS[19]
0x0F12,0x009A,	//TVAR_ash_pGAS[20]
0x0F12,0x00B0,	//TVAR_ash_pGAS[21]
0x0F12,0x00D9,	//TVAR_ash_pGAS[22]
0x0F12,0x0111,	//TVAR_ash_pGAS[23]
0x0F12,0x015F,	//TVAR_ash_pGAS[24]
0x0F12,0x01DD,	//TVAR_ash_pGAS[25]
0x0F12,0x0176,	//TVAR_ash_pGAS[26]
0x0F12,0x0128,	//TVAR_ash_pGAS[27]
0x0F12,0x00D1,	//TVAR_ash_pGAS[28]
0x0F12,0x0097,	//TVAR_ash_pGAS[29]
0x0F12,0x006F,	//TVAR_ash_pGAS[30]
0x0F12,0x0056,	//TVAR_ash_pGAS[31]
0x0F12,0x004D,	//TVAR_ash_pGAS[32]
0x0F12,0x0057,	//TVAR_ash_pGAS[33]
0x0F12,0x0073,	//TVAR_ash_pGAS[34]
0x0F12,0x009E,	//TVAR_ash_pGAS[35]
0x0F12,0x00D9,	//TVAR_ash_pGAS[36]
0x0F12,0x0122,	//TVAR_ash_pGAS[37]
0x0F12,0x0173,	//TVAR_ash_pGAS[38]
0x0F12,0x0144,	//TVAR_ash_pGAS[39]
0x0F12,0x00FE,	//TVAR_ash_pGAS[40]
0x0F12,0x00A9,	//TVAR_ash_pGAS[41]
0x0F12,0x006F,	//TVAR_ash_pGAS[42]
0x0F12,0x0043,	//TVAR_ash_pGAS[43]
0x0F12,0x0028,	//TVAR_ash_pGAS[44]
0x0F12,0x001F,	//TVAR_ash_pGAS[45]
0x0F12,0x002A,	//TVAR_ash_pGAS[46]
0x0F12,0x0049,	//TVAR_ash_pGAS[47]
0x0F12,0x0079,	//TVAR_ash_pGAS[48]
0x0F12,0x00B6,	//TVAR_ash_pGAS[49]
0x0F12,0x0102,	//TVAR_ash_pGAS[50]
0x0F12,0x014F,	//TVAR_ash_pGAS[51]
0x0F12,0x012D,	//TVAR_ash_pGAS[52]
0x0F12,0x00E7,	//TVAR_ash_pGAS[53]
0x0F12,0x0094,	//TVAR_ash_pGAS[54]
0x0F12,0x0056,	//TVAR_ash_pGAS[55]
0x0F12,0x0028,	//TVAR_ash_pGAS[56]
0x0F12,0x000E,	//TVAR_ash_pGAS[57]
0x0F12,0x0007,	//TVAR_ash_pGAS[58]
0x0F12,0x0011,	//TVAR_ash_pGAS[59]
0x0F12,0x0030,	//TVAR_ash_pGAS[60]
0x0F12,0x0063,	//TVAR_ash_pGAS[61]
0x0F12,0x00A6,	//TVAR_ash_pGAS[62]
0x0F12,0x00F2,	//TVAR_ash_pGAS[63]
0x0F12,0x013D,	//TVAR_ash_pGAS[64]
0x0F12,0x0126,	//TVAR_ash_pGAS[65]
0x0F12,0x00E1,	//TVAR_ash_pGAS[66]
0x0F12,0x008D,	//TVAR_ash_pGAS[67]
0x0F12,0x004E,	//TVAR_ash_pGAS[68]
0x0F12,0x0022,	//TVAR_ash_pGAS[69]
0x0F12,0x0007,	//TVAR_ash_pGAS[70]
0x0F12,0x0000,	//TVAR_ash_pGAS[71]
0x0F12,0x000A,	//TVAR_ash_pGAS[72]
0x0F12,0x0028,	//TVAR_ash_pGAS[73]
0x0F12,0x005E,	//TVAR_ash_pGAS[74]
0x0F12,0x00A3,	//TVAR_ash_pGAS[75]
0x0F12,0x00F0,	//TVAR_ash_pGAS[76]
0x0F12,0x013B,	//TVAR_ash_pGAS[77]
0x0F12,0x0133,	//TVAR_ash_pGAS[78]
0x0F12,0x00EE,	//TVAR_ash_pGAS[79]
0x0F12,0x0099,	//TVAR_ash_pGAS[80]
0x0F12,0x005A,	//TVAR_ash_pGAS[81]
0x0F12,0x002D,	//TVAR_ash_pGAS[82]
0x0F12,0x0013,	//TVAR_ash_pGAS[83]
0x0F12,0x000B,	//TVAR_ash_pGAS[84]
0x0F12,0x0016,	//TVAR_ash_pGAS[85]
0x0F12,0x0035,	//TVAR_ash_pGAS[86]
0x0F12,0x006C,	//TVAR_ash_pGAS[87]
0x0F12,0x00B2,	//TVAR_ash_pGAS[88]
0x0F12,0x0100,	//TVAR_ash_pGAS[89]
0x0F12,0x0148,	//TVAR_ash_pGAS[90]
0x0F12,0x0153,	//TVAR_ash_pGAS[91]
0x0F12,0x0107,	//TVAR_ash_pGAS[92]
0x0F12,0x00B4,	//TVAR_ash_pGAS[93]
0x0F12,0x0078,	//TVAR_ash_pGAS[94]
0x0F12,0x004B,	//TVAR_ash_pGAS[95]
0x0F12,0x0030,	//TVAR_ash_pGAS[96]
0x0F12,0x002A,	//TVAR_ash_pGAS[97]
0x0F12,0x0036,	//TVAR_ash_pGAS[98]
0x0F12,0x0057,	//TVAR_ash_pGAS[99]
0x0F12,0x008B,	//TVAR_ash_pGAS[100]
0x0F12,0x00D1,	//TVAR_ash_pGAS[101]
0x0F12,0x0121,	//TVAR_ash_pGAS[102]
0x0F12,0x016A,	//TVAR_ash_pGAS[103]
0x0F12,0x0189,	//TVAR_ash_pGAS[104]
0x0F12,0x0138,	//TVAR_ash_pGAS[105]
0x0F12,0x00DF,	//TVAR_ash_pGAS[106]
0x0F12,0x00A8,	//TVAR_ash_pGAS[107]
0x0F12,0x007E,	//TVAR_ash_pGAS[108]
0x0F12,0x0065,	//TVAR_ash_pGAS[109]
0x0F12,0x005F,	//TVAR_ash_pGAS[110]
0x0F12,0x006A,	//TVAR_ash_pGAS[111]
0x0F12,0x008C,	//TVAR_ash_pGAS[112]
0x0F12,0x00BF,	//TVAR_ash_pGAS[113]
0x0F12,0x00FE,	//TVAR_ash_pGAS[114]
0x0F12,0x0150,	//TVAR_ash_pGAS[115]
0x0F12,0x019E,	//TVAR_ash_pGAS[116]
0x0F12,0x01D5,	//TVAR_ash_pGAS[117]
0x0F12,0x0181,	//TVAR_ash_pGAS[118]
0x0F12,0x0125,	//TVAR_ash_pGAS[119]
0x0F12,0x00EA,	//TVAR_ash_pGAS[120]
0x0F12,0x00C7,	//TVAR_ash_pGAS[121]
0x0F12,0x00B2,	//TVAR_ash_pGAS[122]
0x0F12,0x00AC,	//TVAR_ash_pGAS[123]
0x0F12,0x00BA,	//TVAR_ash_pGAS[124]
0x0F12,0x00D5,	//TVAR_ash_pGAS[125]
0x0F12,0x0100,	//TVAR_ash_pGAS[126]
0x0F12,0x0140,	//TVAR_ash_pGAS[127]
0x0F12,0x019B,	//TVAR_ash_pGAS[128]
0x0F12,0x0217,	//TVAR_ash_pGAS[129]
0x0F12,0x0280,	//TVAR_ash_pGAS[130]
0x0F12,0x01CC,	//TVAR_ash_pGAS[131]
0x0F12,0x0176,	//TVAR_ash_pGAS[132]
0x0F12,0x0131,	//TVAR_ash_pGAS[133]
0x0F12,0x010E,	//TVAR_ash_pGAS[134]
0x0F12,0x00FF,	//TVAR_ash_pGAS[135]
0x0F12,0x00FC,	//TVAR_ash_pGAS[136]
0x0F12,0x0103,	//TVAR_ash_pGAS[137]
0x0F12,0x011B,	//TVAR_ash_pGAS[138]
0x0F12,0x0146,	//TVAR_ash_pGAS[139]
0x0F12,0x018B,	//TVAR_ash_pGAS[140]
0x0F12,0x01F8,	//TVAR_ash_pGAS[141]
0x0F12,0x02DD,	//TVAR_ash_pGAS[142]
0x0F12,0x0232,	//TVAR_ash_pGAS[143]
0x0F12,0x018A,	//TVAR_ash_pGAS[144]
0x0F12,0x0127,	//TVAR_ash_pGAS[145]
0x0F12,0x00EF,	//TVAR_ash_pGAS[146]
0x0F12,0x00CA,	//TVAR_ash_pGAS[147]
0x0F12,0x00B4,	//TVAR_ash_pGAS[148]
0x0F12,0x00AB,	//TVAR_ash_pGAS[149]
0x0F12,0x00B1,	//TVAR_ash_pGAS[150]
0x0F12,0x00C3,	//TVAR_ash_pGAS[151]
0x0F12,0x00E5,	//TVAR_ash_pGAS[152]
0x0F12,0x0118,	//TVAR_ash_pGAS[153]
0x0F12,0x0168,	//TVAR_ash_pGAS[154]
0x0F12,0x023A,	//TVAR_ash_pGAS[155]
0x0F12,0x019A,	//TVAR_ash_pGAS[156]
0x0F12,0x013E,	//TVAR_ash_pGAS[157]
0x0F12,0x00E5,	//TVAR_ash_pGAS[158]
0x0F12,0x00B1,	//TVAR_ash_pGAS[159]
0x0F12,0x008E,	//TVAR_ash_pGAS[160]
0x0F12,0x0079,	//TVAR_ash_pGAS[161]
0x0F12,0x0072,	//TVAR_ash_pGAS[162]
0x0F12,0x0078,	//TVAR_ash_pGAS[163]
0x0F12,0x008E,	//TVAR_ash_pGAS[164]
0x0F12,0x00B2,	//TVAR_ash_pGAS[165]
0x0F12,0x00E1,	//TVAR_ash_pGAS[166]
0x0F12,0x0121,	//TVAR_ash_pGAS[167]
0x0F12,0x019A,	//TVAR_ash_pGAS[168]
0x0F12,0x014D,	//TVAR_ash_pGAS[169]
0x0F12,0x0100,	//TVAR_ash_pGAS[170]
0x0F12,0x00AE,	//TVAR_ash_pGAS[171]
0x0F12,0x007F,	//TVAR_ash_pGAS[172]
0x0F12,0x005B,	//TVAR_ash_pGAS[173]
0x0F12,0x0043,	//TVAR_ash_pGAS[174]
0x0F12,0x003B,	//TVAR_ash_pGAS[175]
0x0F12,0x0045,	//TVAR_ash_pGAS[176]
0x0F12,0x005E,	//TVAR_ash_pGAS[177]
0x0F12,0x0083,	//TVAR_ash_pGAS[178]
0x0F12,0x00B4,	//TVAR_ash_pGAS[179]
0x0F12,0x00EF,	//TVAR_ash_pGAS[180]
0x0F12,0x0139,	//TVAR_ash_pGAS[181]
0x0F12,0x0120,	//TVAR_ash_pGAS[182]
0x0F12,0x00DB,	//TVAR_ash_pGAS[183]
0x0F12,0x008D,	//TVAR_ash_pGAS[184]
0x0F12,0x005D,	//TVAR_ash_pGAS[185]
0x0F12,0x0037,	//TVAR_ash_pGAS[186]
0x0F12,0x001F,	//TVAR_ash_pGAS[187]
0x0F12,0x0017,	//TVAR_ash_pGAS[188]
0x0F12,0x0022,	//TVAR_ash_pGAS[189]
0x0F12,0x003D,	//TVAR_ash_pGAS[190]
0x0F12,0x0066,	//TVAR_ash_pGAS[191]
0x0F12,0x009A,	//TVAR_ash_pGAS[192]
0x0F12,0x00D7,	//TVAR_ash_pGAS[193]
0x0F12,0x011A,	//TVAR_ash_pGAS[194]
0x0F12,0x0108,	//TVAR_ash_pGAS[195]
0x0F12,0x00C7,	//TVAR_ash_pGAS[196]
0x0F12,0x007B,	//TVAR_ash_pGAS[197]
0x0F12,0x004A,	//TVAR_ash_pGAS[198]
0x0F12,0x0023,	//TVAR_ash_pGAS[199]
0x0F12,0x000B,	//TVAR_ash_pGAS[200]
0x0F12,0x0005,	//TVAR_ash_pGAS[201]
0x0F12,0x000E,	//TVAR_ash_pGAS[202]
0x0F12,0x002A,	//TVAR_ash_pGAS[203]
0x0F12,0x0055,	//TVAR_ash_pGAS[204]
0x0F12,0x008D,	//TVAR_ash_pGAS[205]
0x0F12,0x00C9,	//TVAR_ash_pGAS[206]
0x0F12,0x0104,	//TVAR_ash_pGAS[207]
0x0F12,0x0100,	//TVAR_ash_pGAS[208]
0x0F12,0x00C0,	//TVAR_ash_pGAS[209]
0x0F12,0x0076,	//TVAR_ash_pGAS[210]
0x0F12,0x0044,	//TVAR_ash_pGAS[211]
0x0F12,0x001E,	//TVAR_ash_pGAS[212]
0x0F12,0x0006,	//TVAR_ash_pGAS[213]
0x0F12,0x0000,	//TVAR_ash_pGAS[214]
0x0F12,0x0009,	//TVAR_ash_pGAS[215]
0x0F12,0x0024,	//TVAR_ash_pGAS[216]
0x0F12,0x0052,	//TVAR_ash_pGAS[217]
0x0F12,0x008B,	//TVAR_ash_pGAS[218]
0x0F12,0x00C8,	//TVAR_ash_pGAS[219]
0x0F12,0x0104,	//TVAR_ash_pGAS[220]
0x0F12,0x0109,	//TVAR_ash_pGAS[221]
0x0F12,0x00C9,	//TVAR_ash_pGAS[222]
0x0F12,0x007F,	//TVAR_ash_pGAS[223]
0x0F12,0x004D,	//TVAR_ash_pGAS[224]
0x0F12,0x0027,	//TVAR_ash_pGAS[225]
0x0F12,0x0010,	//TVAR_ash_pGAS[226]
0x0F12,0x000A,	//TVAR_ash_pGAS[227]
0x0F12,0x0014,	//TVAR_ash_pGAS[228]
0x0F12,0x002F,	//TVAR_ash_pGAS[229]
0x0F12,0x005D,	//TVAR_ash_pGAS[230]
0x0F12,0x0098,	//TVAR_ash_pGAS[231]
0x0F12,0x00D7,	//TVAR_ash_pGAS[232]
0x0F12,0x010E,	//TVAR_ash_pGAS[233]
0x0F12,0x0125,	//TVAR_ash_pGAS[234]
0x0F12,0x00E0,	//TVAR_ash_pGAS[235]
0x0F12,0x0094,	//TVAR_ash_pGAS[236]
0x0F12,0x0066,	//TVAR_ash_pGAS[237]
0x0F12,0x0040,	//TVAR_ash_pGAS[238]
0x0F12,0x002A,	//TVAR_ash_pGAS[239]
0x0F12,0x0024,	//TVAR_ash_pGAS[240]
0x0F12,0x002F,	//TVAR_ash_pGAS[241]
0x0F12,0x004C,	//TVAR_ash_pGAS[242]
0x0F12,0x0078,	//TVAR_ash_pGAS[243]
0x0F12,0x00B3,	//TVAR_ash_pGAS[244]
0x0F12,0x00F4,	//TVAR_ash_pGAS[245]
0x0F12,0x012F,	//TVAR_ash_pGAS[246]
0x0F12,0x0153,	//TVAR_ash_pGAS[247]
0x0F12,0x0108,	//TVAR_ash_pGAS[248]
0x0F12,0x00B9,	//TVAR_ash_pGAS[249]
0x0F12,0x008D,	//TVAR_ash_pGAS[250]
0x0F12,0x006B,	//TVAR_ash_pGAS[251]
0x0F12,0x0055,	//TVAR_ash_pGAS[252]
0x0F12,0x0050,	//TVAR_ash_pGAS[253]
0x0F12,0x005D,	//TVAR_ash_pGAS[254]
0x0F12,0x007A,	//TVAR_ash_pGAS[255]
0x0F12,0x00A4,	//TVAR_ash_pGAS[256]
0x0F12,0x00D9,	//TVAR_ash_pGAS[257]
0x0F12,0x011D,	//TVAR_ash_pGAS[258]
0x0F12,0x015E,	//TVAR_ash_pGAS[259]
0x0F12,0x019C,	//TVAR_ash_pGAS[260]
0x0F12,0x0144,	//TVAR_ash_pGAS[261]
0x0F12,0x00F3,	//TVAR_ash_pGAS[262]
0x0F12,0x00C2,	//TVAR_ash_pGAS[263]
0x0F12,0x00A4,	//TVAR_ash_pGAS[264]
0x0F12,0x0093,	//TVAR_ash_pGAS[265]
0x0F12,0x008F,	//TVAR_ash_pGAS[266]
0x0F12,0x009C,	//TVAR_ash_pGAS[267]
0x0F12,0x00B5,	//TVAR_ash_pGAS[268]
0x0F12,0x00DA,	//TVAR_ash_pGAS[269]
0x0F12,0x010E,	//TVAR_ash_pGAS[270]
0x0F12,0x015E,	//TVAR_ash_pGAS[271]
0x0F12,0x01C9,	//TVAR_ash_pGAS[272]
0x0F12,0x0239,	//TVAR_ash_pGAS[273]
0x0F12,0x0186,	//TVAR_ash_pGAS[274]
0x0F12,0x0137,	//TVAR_ash_pGAS[275]
0x0F12,0x00FF,	//TVAR_ash_pGAS[276]
0x0F12,0x00DE,	//TVAR_ash_pGAS[277]
0x0F12,0x00CF,	//TVAR_ash_pGAS[278]
0x0F12,0x00CE,	//TVAR_ash_pGAS[279]
0x0F12,0x00D5,	//TVAR_ash_pGAS[280]
0x0F12,0x00ED,	//TVAR_ash_pGAS[281]
0x0F12,0x0113,	//TVAR_ash_pGAS[282]
0x0F12,0x014E,	//TVAR_ash_pGAS[283]
0x0F12,0x01B0,	//TVAR_ash_pGAS[284]
0x0F12,0x0285,	//TVAR_ash_pGAS[285]
0x0F12,0x020F,	//TVAR_ash_pGAS[286]
0x0F12,0x0173,	//TVAR_ash_pGAS[287]
0x0F12,0x0112,	//TVAR_ash_pGAS[288]
0x0F12,0x00DE,	//TVAR_ash_pGAS[289]
0x0F12,0x00BF,	//TVAR_ash_pGAS[290]
0x0F12,0x00AD,	//TVAR_ash_pGAS[291]
0x0F12,0x00A8,	//TVAR_ash_pGAS[292]
0x0F12,0x00B3,	//TVAR_ash_pGAS[293]
0x0F12,0x00CC,	//TVAR_ash_pGAS[294]
0x0F12,0x00F2,	//TVAR_ash_pGAS[295]
0x0F12,0x0126,	//TVAR_ash_pGAS[296]
0x0F12,0x0176,	//TVAR_ash_pGAS[297]
0x0F12,0x024B,	//TVAR_ash_pGAS[298]
0x0F12,0x0180,	//TVAR_ash_pGAS[299]
0x0F12,0x012F,	//TVAR_ash_pGAS[300]
0x0F12,0x00D5,	//TVAR_ash_pGAS[301]
0x0F12,0x00A7,	//TVAR_ash_pGAS[302]
0x0F12,0x0087,	//TVAR_ash_pGAS[303]
0x0F12,0x0075,	//TVAR_ash_pGAS[304]
0x0F12,0x0072,	//TVAR_ash_pGAS[305]
0x0F12,0x007E,	//TVAR_ash_pGAS[306]
0x0F12,0x0096,	//TVAR_ash_pGAS[307]
0x0F12,0x00BE,	//TVAR_ash_pGAS[308]
0x0F12,0x00EF,	//TVAR_ash_pGAS[309]
0x0F12,0x012F,	//TVAR_ash_pGAS[310]
0x0F12,0x01A7,	//TVAR_ash_pGAS[311]
0x0F12,0x013C,	//TVAR_ash_pGAS[312]
0x0F12,0x00F5,	//TVAR_ash_pGAS[313]
0x0F12,0x00A5,	//TVAR_ash_pGAS[314]
0x0F12,0x0078,	//TVAR_ash_pGAS[315]
0x0F12,0x0057,	//TVAR_ash_pGAS[316]
0x0F12,0x0042,	//TVAR_ash_pGAS[317]
0x0F12,0x003D,	//TVAR_ash_pGAS[318]
0x0F12,0x0049,	//TVAR_ash_pGAS[319]
0x0F12,0x0066,	//TVAR_ash_pGAS[320]
0x0F12,0x008F,	//TVAR_ash_pGAS[321]
0x0F12,0x00C0,	//TVAR_ash_pGAS[322]
0x0F12,0x00FD,	//TVAR_ash_pGAS[323]
0x0F12,0x0144,	//TVAR_ash_pGAS[324]
0x0F12,0x0115,	//TVAR_ash_pGAS[325]
0x0F12,0x00D6,	//TVAR_ash_pGAS[326]
0x0F12,0x0089,	//TVAR_ash_pGAS[327]
0x0F12,0x005B,	//TVAR_ash_pGAS[328]
0x0F12,0x0036,	//TVAR_ash_pGAS[329]
0x0F12,0x001F,	//TVAR_ash_pGAS[330]
0x0F12,0x0018,	//TVAR_ash_pGAS[331]
0x0F12,0x0024,	//TVAR_ash_pGAS[332]
0x0F12,0x0043,	//TVAR_ash_pGAS[333]
0x0F12,0x006F,	//TVAR_ash_pGAS[334]
0x0F12,0x00A5,	//TVAR_ash_pGAS[335]
0x0F12,0x00E0,	//TVAR_ash_pGAS[336]
0x0F12,0x0122,	//TVAR_ash_pGAS[337]
0x0F12,0x0104,	//TVAR_ash_pGAS[338]
0x0F12,0x00C7,	//TVAR_ash_pGAS[339]
0x0F12,0x007B,	//TVAR_ash_pGAS[340]
0x0F12,0x004A,	//TVAR_ash_pGAS[341]
0x0F12,0x0023,	//TVAR_ash_pGAS[342]
0x0F12,0x000B,	//TVAR_ash_pGAS[343]
0x0F12,0x0005,	//TVAR_ash_pGAS[344]
0x0F12,0x0010,	//TVAR_ash_pGAS[345]
0x0F12,0x002E,	//TVAR_ash_pGAS[346]
0x0F12,0x005B,	//TVAR_ash_pGAS[347]
0x0F12,0x0094,	//TVAR_ash_pGAS[348]
0x0F12,0x00D0,	//TVAR_ash_pGAS[349]
0x0F12,0x010C,	//TVAR_ash_pGAS[350]
0x0F12,0x0105,	//TVAR_ash_pGAS[351]
0x0F12,0x00C5,	//TVAR_ash_pGAS[352]
0x0F12,0x007A,	//TVAR_ash_pGAS[353]
0x0F12,0x0047,	//TVAR_ash_pGAS[354]
0x0F12,0x0021,	//TVAR_ash_pGAS[355]
0x0F12,0x0008,	//TVAR_ash_pGAS[356]
0x0F12,0x0000,	//TVAR_ash_pGAS[357]
0x0F12,0x0009,	//TVAR_ash_pGAS[358]
0x0F12,0x0026,	//TVAR_ash_pGAS[359]
0x0F12,0x0054,	//TVAR_ash_pGAS[360]
0x0F12,0x008E,	//TVAR_ash_pGAS[361]
0x0F12,0x00CA,	//TVAR_ash_pGAS[362]
0x0F12,0x0106,	//TVAR_ash_pGAS[363]
0x0F12,0x0116,	//TVAR_ash_pGAS[364]
0x0F12,0x00D3,	//TVAR_ash_pGAS[365]
0x0F12,0x0088,	//TVAR_ash_pGAS[366]
0x0F12,0x0054,	//TVAR_ash_pGAS[367]
0x0F12,0x002C,	//TVAR_ash_pGAS[368]
0x0F12,0x0012,	//TVAR_ash_pGAS[369]
0x0F12,0x0009,	//TVAR_ash_pGAS[370]
0x0F12,0x0013,	//TVAR_ash_pGAS[371]
0x0F12,0x002E,	//TVAR_ash_pGAS[372]
0x0F12,0x005C,	//TVAR_ash_pGAS[373]
0x0F12,0x0095,	//TVAR_ash_pGAS[374]
0x0F12,0x00D4,	//TVAR_ash_pGAS[375]
0x0F12,0x010D,	//TVAR_ash_pGAS[376]
0x0F12,0x013A,	//TVAR_ash_pGAS[377]
0x0F12,0x00EE,	//TVAR_ash_pGAS[378]
0x0F12,0x00A1,	//TVAR_ash_pGAS[379]
0x0F12,0x006F,	//TVAR_ash_pGAS[380]
0x0F12,0x0047,	//TVAR_ash_pGAS[381]
0x0F12,0x002C,	//TVAR_ash_pGAS[382]
0x0F12,0x0023,	//TVAR_ash_pGAS[383]
0x0F12,0x002D,	//TVAR_ash_pGAS[384]
0x0F12,0x0047,	//TVAR_ash_pGAS[385]
0x0F12,0x0072,	//TVAR_ash_pGAS[386]
0x0F12,0x00AB,	//TVAR_ash_pGAS[387]
0x0F12,0x00EB,	//TVAR_ash_pGAS[388]
0x0F12,0x0127,	//TVAR_ash_pGAS[389]
0x0F12,0x016C,	//TVAR_ash_pGAS[390]
0x0F12,0x011B,	//TVAR_ash_pGAS[391]
0x0F12,0x00CA,	//TVAR_ash_pGAS[392]
0x0F12,0x0099,	//TVAR_ash_pGAS[393]
0x0F12,0x0074,	//TVAR_ash_pGAS[394]
0x0F12,0x0059,	//TVAR_ash_pGAS[395]
0x0F12,0x0050,	//TVAR_ash_pGAS[396]
0x0F12,0x0057,	//TVAR_ash_pGAS[397]
0x0F12,0x0072,	//TVAR_ash_pGAS[398]
0x0F12,0x0099,	//TVAR_ash_pGAS[399]
0x0F12,0x00CB,	//TVAR_ash_pGAS[400]
0x0F12,0x0111,	//TVAR_ash_pGAS[401]
0x0F12,0x0155,	//TVAR_ash_pGAS[402]
0x0F12,0x01BA,	//TVAR_ash_pGAS[403]
0x0F12,0x015D,	//TVAR_ash_pGAS[404]
0x0F12,0x0107,	//TVAR_ash_pGAS[405]
0x0F12,0x00D3,	//TVAR_ash_pGAS[406]
0x0F12,0x00AE,	//TVAR_ash_pGAS[407]
0x0F12,0x0099,	//TVAR_ash_pGAS[408]
0x0F12,0x008F,	//TVAR_ash_pGAS[409]
0x0F12,0x0096,	//TVAR_ash_pGAS[410]
0x0F12,0x00AB,	//TVAR_ash_pGAS[411]
0x0F12,0x00CD,	//TVAR_ash_pGAS[412]
0x0F12,0x00FE,	//TVAR_ash_pGAS[413]
0x0F12,0x014D,	//TVAR_ash_pGAS[414]
0x0F12,0x01BC,	//TVAR_ash_pGAS[415]
0x0F12,0x025D,	//TVAR_ash_pGAS[416]
0x0F12,0x01A6,	//TVAR_ash_pGAS[417]
0x0F12,0x0151,	//TVAR_ash_pGAS[418]
0x0F12,0x0115,	//TVAR_ash_pGAS[419]
0x0F12,0x00F0,	//TVAR_ash_pGAS[420]
0x0F12,0x00DA,	//TVAR_ash_pGAS[421]
0x0F12,0x00D0,	//TVAR_ash_pGAS[422]
0x0F12,0x00D3,	//TVAR_ash_pGAS[423]
0x0F12,0x00E5,	//TVAR_ash_pGAS[424]
0x0F12,0x0108,	//TVAR_ash_pGAS[425]
0x0F12,0x013F,	//TVAR_ash_pGAS[426]
0x0F12,0x019E,	//TVAR_ash_pGAS[427]
0x0F12,0x0271,	//TVAR_ash_pGAS[428]
0x0F12,0x01B8,	//TVAR_ash_pGAS[429]
0x0F12,0x013B,	//TVAR_ash_pGAS[430]
0x0F12,0x00E6,	//TVAR_ash_pGAS[431]
0x0F12,0x00BB,	//TVAR_ash_pGAS[432]
0x0F12,0x00A5,	//TVAR_ash_pGAS[433]
0x0F12,0x009A,	//TVAR_ash_pGAS[434]
0x0F12,0x0096,	//TVAR_ash_pGAS[435]
0x0F12,0x00A0,	//TVAR_ash_pGAS[436]
0x0F12,0x00B1,	//TVAR_ash_pGAS[437]
0x0F12,0x00D0,	//TVAR_ash_pGAS[438]
0x0F12,0x00FE,	//TVAR_ash_pGAS[439]
0x0F12,0x0148,	//TVAR_ash_pGAS[440]
0x0F12,0x01EF,	//TVAR_ash_pGAS[441]
0x0F12,0x0133,	//TVAR_ash_pGAS[442]
0x0F12,0x00F9,	//TVAR_ash_pGAS[443]
0x0F12,0x00B0,	//TVAR_ash_pGAS[444]
0x0F12,0x008A,	//TVAR_ash_pGAS[445]
0x0F12,0x0074,	//TVAR_ash_pGAS[446]
0x0F12,0x0069,	//TVAR_ash_pGAS[447]
0x0F12,0x0068,	//TVAR_ash_pGAS[448]
0x0F12,0x0071,	//TVAR_ash_pGAS[449]
0x0F12,0x0085,	//TVAR_ash_pGAS[450]
0x0F12,0x00A4,	//TVAR_ash_pGAS[451]
0x0F12,0x00CC,	//TVAR_ash_pGAS[452]
0x0F12,0x0106,	//TVAR_ash_pGAS[453]
0x0F12,0x0161,	//TVAR_ash_pGAS[454]
0x0F12,0x00F6,	//TVAR_ash_pGAS[455]
0x0F12,0x00C0,	//TVAR_ash_pGAS[456]
0x0F12,0x0080,	//TVAR_ash_pGAS[457]
0x0F12,0x005F,	//TVAR_ash_pGAS[458]
0x0F12,0x0048,	//TVAR_ash_pGAS[459]
0x0F12,0x003B,	//TVAR_ash_pGAS[460]
0x0F12,0x0039,	//TVAR_ash_pGAS[461]
0x0F12,0x0043,	//TVAR_ash_pGAS[462]
0x0F12,0x0059,	//TVAR_ash_pGAS[463]
0x0F12,0x0078,	//TVAR_ash_pGAS[464]
0x0F12,0x00A0,	//TVAR_ash_pGAS[465]
0x0F12,0x00D5,	//TVAR_ash_pGAS[466]
0x0F12,0x0107,	//TVAR_ash_pGAS[467]
0x0F12,0x00CE,	//TVAR_ash_pGAS[468]
0x0F12,0x00A0,	//TVAR_ash_pGAS[469]
0x0F12,0x0063,	//TVAR_ash_pGAS[470]
0x0F12,0x0043,	//TVAR_ash_pGAS[471]
0x0F12,0x0029,	//TVAR_ash_pGAS[472]
0x0F12,0x001B,	//TVAR_ash_pGAS[473]
0x0F12,0x0018,	//TVAR_ash_pGAS[474]
0x0F12,0x0020,	//TVAR_ash_pGAS[475]
0x0F12,0x0037,	//TVAR_ash_pGAS[476]
0x0F12,0x0059,	//TVAR_ash_pGAS[477]
0x0F12,0x0082,	//TVAR_ash_pGAS[478]
0x0F12,0x00B6,	//TVAR_ash_pGAS[479]
0x0F12,0x00E5,	//TVAR_ash_pGAS[480]
0x0F12,0x00BC,	//TVAR_ash_pGAS[481]
0x0F12,0x008F,	//TVAR_ash_pGAS[482]
0x0F12,0x0054,	//TVAR_ash_pGAS[483]
0x0F12,0x0031,	//TVAR_ash_pGAS[484]
0x0F12,0x0017,	//TVAR_ash_pGAS[485]
0x0F12,0x0008,	//TVAR_ash_pGAS[486]
0x0F12,0x0006,	//TVAR_ash_pGAS[487]
0x0F12,0x000D,	//TVAR_ash_pGAS[488]
0x0F12,0x0022,	//TVAR_ash_pGAS[489]
0x0F12,0x0044,	//TVAR_ash_pGAS[490]
0x0F12,0x0070,	//TVAR_ash_pGAS[491]
0x0F12,0x00A3,	//TVAR_ash_pGAS[492]
0x0F12,0x00CF,	//TVAR_ash_pGAS[493]
0x0F12,0x00B7,	//TVAR_ash_pGAS[494]
0x0F12,0x008A,	//TVAR_ash_pGAS[495]
0x0F12,0x0051,	//TVAR_ash_pGAS[496]
0x0F12,0x002D,	//TVAR_ash_pGAS[497]
0x0F12,0x0013,	//TVAR_ash_pGAS[498]
0x0F12,0x0005,	//TVAR_ash_pGAS[499]
0x0F12,0x0000,	//TVAR_ash_pGAS[500]
0x0F12,0x0006,	//TVAR_ash_pGAS[501]
0x0F12,0x001A,	//TVAR_ash_pGAS[502]
0x0F12,0x003B,	//TVAR_ash_pGAS[503]
0x0F12,0x0069,	//TVAR_ash_pGAS[504]
0x0F12,0x009A,	//TVAR_ash_pGAS[505]
0x0F12,0x00C8,	//TVAR_ash_pGAS[506]
0x0F12,0x00C3,	//TVAR_ash_pGAS[507]
0x0F12,0x0093,	//TVAR_ash_pGAS[508]
0x0F12,0x005A,	//TVAR_ash_pGAS[509]
0x0F12,0x0036,	//TVAR_ash_pGAS[510]
0x0F12,0x001B,	//TVAR_ash_pGAS[511]
0x0F12,0x000D,	//TVAR_ash_pGAS[512]
0x0F12,0x0008,	//TVAR_ash_pGAS[513]
0x0F12,0x000D,	//TVAR_ash_pGAS[514]
0x0F12,0x0020,	//TVAR_ash_pGAS[515]
0x0F12,0x0041,	//TVAR_ash_pGAS[516]
0x0F12,0x006C,	//TVAR_ash_pGAS[517]
0x0F12,0x00A0,	//TVAR_ash_pGAS[518]
0x0F12,0x00CC,	//TVAR_ash_pGAS[519]
0x0F12,0x00DE,	//TVAR_ash_pGAS[520]
0x0F12,0x00AA,	//TVAR_ash_pGAS[521]
0x0F12,0x0070,	//TVAR_ash_pGAS[522]
0x0F12,0x004D,	//TVAR_ash_pGAS[523]
0x0F12,0x0032,	//TVAR_ash_pGAS[524]
0x0F12,0x0023,	//TVAR_ash_pGAS[525]
0x0F12,0x001E,	//TVAR_ash_pGAS[526]
0x0F12,0x0024,	//TVAR_ash_pGAS[527]
0x0F12,0x0036,	//TVAR_ash_pGAS[528]
0x0F12,0x0055,	//TVAR_ash_pGAS[529]
0x0F12,0x007E,	//TVAR_ash_pGAS[530]
0x0F12,0x00B3,	//TVAR_ash_pGAS[531]
0x0F12,0x00E3,	//TVAR_ash_pGAS[532]
0x0F12,0x010B,	//TVAR_ash_pGAS[533]
0x0F12,0x00D0,	//TVAR_ash_pGAS[534]
0x0F12,0x0091,	//TVAR_ash_pGAS[535]
0x0F12,0x0070,	//TVAR_ash_pGAS[536]
0x0F12,0x0058,	//TVAR_ash_pGAS[537]
0x0F12,0x0048,	//TVAR_ash_pGAS[538]
0x0F12,0x0044,	//TVAR_ash_pGAS[539]
0x0F12,0x0048,	//TVAR_ash_pGAS[540]
0x0F12,0x005A,	//TVAR_ash_pGAS[541]
0x0F12,0x0075,	//TVAR_ash_pGAS[542]
0x0F12,0x009A,	//TVAR_ash_pGAS[543]
0x0F12,0x00D2,	//TVAR_ash_pGAS[544]
0x0F12,0x010B,	//TVAR_ash_pGAS[545]
0x0F12,0x014F,	//TVAR_ash_pGAS[546]
0x0F12,0x010B,	//TVAR_ash_pGAS[547]
0x0F12,0x00C5,	//TVAR_ash_pGAS[548]
0x0F12,0x00A0,	//TVAR_ash_pGAS[549]
0x0F12,0x008A,	//TVAR_ash_pGAS[550]
0x0F12,0x007F,	//TVAR_ash_pGAS[551]
0x0F12,0x0079,	//TVAR_ash_pGAS[552]
0x0F12,0x007D,	//TVAR_ash_pGAS[553]
0x0F12,0x008A,	//TVAR_ash_pGAS[554]
0x0F12,0x00A1,	//TVAR_ash_pGAS[555]
0x0F12,0x00C6,	//TVAR_ash_pGAS[556]
0x0F12,0x0107,	//TVAR_ash_pGAS[557]
0x0F12,0x0164,	//TVAR_ash_pGAS[558]
0x0F12,0x01E0,	//TVAR_ash_pGAS[559]
0x0F12,0x014B,	//TVAR_ash_pGAS[560]
0x0F12,0x0107,	//TVAR_ash_pGAS[561]
0x0F12,0x00D7,	//TVAR_ash_pGAS[562]
0x0F12,0x00BF,	//TVAR_ash_pGAS[563]
0x0F12,0x00B2,	//TVAR_ash_pGAS[564]
0x0F12,0x00AE,	//TVAR_ash_pGAS[565]
0x0F12,0x00AF,	//TVAR_ash_pGAS[566]
0x0F12,0x00B8,	//TVAR_ash_pGAS[567]
0x0F12,0x00D1,	//TVAR_ash_pGAS[568]
0x0F12,0x00FE,	//TVAR_ash_pGAS[569]
0x0F12,0x014D,	//TVAR_ash_pGAS[570]
0x0F12,0x01FD,	//TVAR_ash_pGAS[571]
//	param_end	TVAR_ash_pGAS
0x002A,0x074E,
0x0F12,0x0001,	//ash_bLumaMode 
0x002A,0x0D30,
0x0F12,0x025F,	//awbb_GLocusR
0x002A,0x0D32,
0x0F12,0x0376,	//awbb_GLocusB
//	param_start	TVAR_ash_AwbAshCord
0x002A,0x06B8,
0x0F12,0x00C0,	//TVAR_ash_AwbAshCord[0]
0x0F12,0x00E0,	//TVAR_ash_AwbAshCord[1]
0x0F12,0x00FA,	//TVAR_ash_AwbAshCord[2]
0x0F12,0x011D,	//TVAR_ash_AwbAshCord[3]
0x0F12,0x0144,	//TVAR_ash_AwbAshCord[4]
0x0F12,0x0173,	//TVAR_ash_AwbAshCord[5]
0x0F12,0x0180,	//TVAR_ash_AwbAshCord[6]
//	param_end	TVAR_ash_AwbAshCord
0x002A,0x0664,
0x0F12,0x013E,	//seti_uContrastCenter
      
//ash_CGrasAlphas
0x002A,0x06C6,
0x0F12,0x010B,    	//ash_CGrasAlphas[0]
0x0F12,0x0103,    	//ash_CGrasAlphas[1]
0x0F12,0x00FC,    	//ash_CGrasAlphas[2]
0x0F12,0x010C,    	//ash_CGrasAlphas[3]
//============================================================
//End GAS 
//============================================================
//White Balance
//============================================================
//	param_start	awbb_IndoorGrZones_m_BGrid
0x002A,0x0C48,
0x0F12,0x03C9,	//awbb_IndoorGrZones_m_BGrid[0]
0x0F12,0x040A,	//awbb_IndoorGrZones_m_BGrid[1]
0x0F12,0x038B,	//awbb_IndoorGrZones_m_BGrid[2]
0x0F12,0x0405,	//awbb_IndoorGrZones_m_BGrid[3]
0x0F12,0x0356,	//awbb_IndoorGrZones_m_BGrid[4]
0x0F12,0x0400,	//awbb_IndoorGrZones_m_BGrid[5]
0x0F12,0x0322,	//awbb_IndoorGrZones_m_BGrid[6]
0x0F12,0x03DF,	//awbb_IndoorGrZones_m_BGrid[7]
0x0F12,0x02E9,	//awbb_IndoorGrZones_m_BGrid[8]
0x0F12,0x03B0,	//awbb_IndoorGrZones_m_BGrid[9]
0x0F12,0x02B6,	//awbb_IndoorGrZones_m_BGrid[10]
0x0F12,0x0380,	//awbb_IndoorGrZones_m_BGrid[11]
0x0F12,0x0296,	//awbb_IndoorGrZones_m_BGrid[12]
0x0F12,0x034F,	//awbb_IndoorGrZones_m_BGrid[13]
0x0F12,0x027F,	//awbb_IndoorGrZones_m_BGrid[14]
0x0F12,0x031A,	//awbb_IndoorGrZones_m_BGrid[15]
0x0F12,0x026C,	//awbb_IndoorGrZones_m_BGrid[16]
0x0F12,0x02F5,	//awbb_IndoorGrZones_m_BGrid[17]
0x0F12,0x0256,	//awbb_IndoorGrZones_m_BGrid[18]
0x0F12,0x02DA,	//awbb_IndoorGrZones_m_BGrid[19]
0x0F12,0x0241,	//awbb_IndoorGrZones_m_BGrid[20]
0x0F12,0x02C4,	//awbb_IndoorGrZones_m_BGrid[21]
0x0F12,0x0228,	//awbb_IndoorGrZones_m_BGrid[22]
0x0F12,0x02AB,	//awbb_IndoorGrZones_m_BGrid[23]
0x0F12,0x0213,	//awbb_IndoorGrZones_m_BGrid[24]
0x0F12,0x0292,	//awbb_IndoorGrZones_m_BGrid[25]
0x0F12,0x01FF,	//awbb_IndoorGrZones_m_BGrid[26]
0x0F12,0x0278,	//awbb_IndoorGrZones_m_BGrid[27]
0x0F12,0x01F4,	//awbb_IndoorGrZones_m_BGrid[28]
0x0F12,0x025F,	//awbb_IndoorGrZones_m_BGrid[29]
0x0F12,0x0202,	//awbb_IndoorGrZones_m_BGrid[30]
0x0F12,0x0234,	//awbb_IndoorGrZones_m_BGrid[31]
0x0F12,0x0000,	//awbb_IndoorGrZones_m_BGrid[32]
0x0F12,0x0000,	//awbb_IndoorGrZones_m_BGrid[33]
0x0F12,0x0000,	//awbb_IndoorGrZones_m_BGrid[34]
0x0F12,0x0000,	//awbb_IndoorGrZones_m_BGrid[35]
0x0F12,0x0000,	//awbb_IndoorGrZones_m_BGrid[36]
0x0F12,0x0000,	//awbb_IndoorGrZones_m_BGrid[37]
0x0F12,0x0000,	//awbb_IndoorGrZones_m_BGrid[38]
0x0F12,0x0000,	//awbb_IndoorGrZones_m_BGrid[39]
//	param_end	awbb_IndoorGrZones_m_BGrid
0x002A,0x0C98,
0x0F12,0x0005,
0x0F12,0x0000,	//awbb_IndoorGrZones_m_GridStep
0x002A,0x0CA0,
0x0F12,0x00E8,
0x0F12,0x0000,	//awbb_IndoorGrZones_m_Boffs
//	param_start	awbb_LowBrGrZones_m_BGrid
0x002A,0x0CE0,
0x0F12,0x03D4,	//awbb_LowBrGrZones_m_BGrid[0]
0x0F12,0x043E,	//awbb_LowBrGrZones_m_BGrid[1]
0x0F12,0x035C,	//awbb_LowBrGrZones_m_BGrid[2]
0x0F12,0x0438,	//awbb_LowBrGrZones_m_BGrid[3]
0x0F12,0x02F0,	//awbb_LowBrGrZones_m_BGrid[4]
0x0F12,0x042D,	//awbb_LowBrGrZones_m_BGrid[5]
0x0F12,0x029A,	//awbb_LowBrGrZones_m_BGrid[6]
0x0F12,0x03EF,	//awbb_LowBrGrZones_m_BGrid[7]
0x0F12,0x025E,	//awbb_LowBrGrZones_m_BGrid[8]
0x0F12,0x0395,	//awbb_LowBrGrZones_m_BGrid[9]
0x0F12,0x022E,	//awbb_LowBrGrZones_m_BGrid[10]
0x0F12,0x0346,	//awbb_LowBrGrZones_m_BGrid[11]
0x0F12,0x0200,	//awbb_LowBrGrZones_m_BGrid[12]
0x0F12,0x02F6,	//awbb_LowBrGrZones_m_BGrid[13]
0x0F12,0x01CE,	//awbb_LowBrGrZones_m_BGrid[14]
0x0F12,0x02C8,	//awbb_LowBrGrZones_m_BGrid[15]
0x0F12,0x01BB,	//awbb_LowBrGrZones_m_BGrid[16]
0x0F12,0x0287,	//awbb_LowBrGrZones_m_BGrid[17]
0x0F12,0x01E2,	//awbb_LowBrGrZones_m_BGrid[18]
0x0F12,0x0239,	//awbb_LowBrGrZones_m_BGrid[19]
0x0F12,0x0000,	//awbb_LowBrGrZones_m_BGrid[20]
0x0F12,0x0000,	//awbb_LowBrGrZones_m_BGrid[21]
0x0F12,0x0000,	//awbb_LowBrGrZones_m_BGrid[22]
0x0F12,0x0000,	//awbb_LowBrGrZones_m_BGrid[23]
//	param_end	awbb_LowBrGrZones_m_BGrid  
0x002A,0x0D10,
0x0F12,0x0006,
0x0F12,0x0000,	//awbb_LowBrGrZones_m_GridStep
0x002A,0x0D18,
0x0F12,0x00AE,
0x0F12,0x0000,	//awbb_LowBrGrZones_m_Boffs
//	param_start	awbb_OutdoorGrZones_m_BGrid
0x002A,0x0CA4,
0x0F12,0x031A,//02A8	//awbb_OutdoorGrZones_m_BGrid[0]
0x0F12,0x0382,//02C2	//awbb_OutdoorGrZones_m_BGrid[1]
0x0F12,0x02E3,//0270	//awbb_OutdoorGrZones_m_BGrid[2]
0x0F12,0x038B,//02B4	//awbb_OutdoorGrZones_m_BGrid[3]
0x0F12,0x02B7,//025C	//awbb_OutdoorGrZones_m_BGrid[4]
0x0F12,0x035D,//02A0	//awbb_OutdoorGrZones_m_BGrid[5]
0x0F12,0x0297,//0240	//awbb_OutdoorGrZones_m_BGrid[6]
0x0F12,0x032A,//0288	//awbb_OutdoorGrZones_m_BGrid[7]
0x0F12,0x0285,//0230	//awbb_OutdoorGrZones_m_BGrid[8]
0x0F12,0x0302,//026E	//awbb_OutdoorGrZones_m_BGrid[9]
0x0F12,0x0271,//0222	//awbb_OutdoorGrZones_m_BGrid[10]
0x0F12,0x02DF,//025A	//awbb_OutdoorGrZones_m_BGrid[11]
0x0F12,0x025D,//0220	//awbb_OutdoorGrZones_m_BGrid[12]
0x0F12,0x02C7,//023A	//awbb_OutdoorGrZones_m_BGrid[13]
0x0F12,0x0241,//0000	//awbb_OutdoorGrZones_m_BGrid[14]
0x0F12,0x02B5,//0000	//awbb_OutdoorGrZones_m_BGrid[15]
0x0F12,0x0229,//0000	//awbb_OutdoorGrZones_m_BGrid[16]
0x0F12,0x029B,//0000	//awbb_OutdoorGrZones_m_BGrid[17]
0x0F12,0x0212,//0000	//awbb_OutdoorGrZones_m_BGrid[18]
0x0F12,0x0280,//0000	//awbb_OutdoorGrZones_m_BGrid[19]
0x0F12,0x0205,//0000	//awbb_OutdoorGrZones_m_BGrid[20]
0x0F12,0x026A,//0000	//awbb_OutdoorGrZones_m_BGrid[21]
0x0F12,0x020A,//0000	//awbb_OutdoorGrZones_m_BGrid[22]
0x0F12,0x023F,//0000	//awbb_OutdoorGrZones_m_BGrid[23]
//	param_end	awbb_OutdoorGrZones_m_BGrid 
0x002A,0x0CD4,
0x0F12,0x0005,
0x0F12,0x0000,	//awbb_OutdoorGrZones_m_GridStep
0x002A,0x0CDC,
0x0F12,0x0157,  //01FE
0x0F12,0x0000,	//awbb_OutdoorGrZones_m_Boffs
//Low illumiantion
0x002A,0x0D1C,
0x0F12,0x037C,
0x0F12,0x0000,	//awbb_CrclLowT_R_c
0x002A,0x0D20,
0x0F12,0x0157,
0x0F12,0x0000,	//awbb_CrclLowT_B_c
0x002A,0x0D24,
0x0F12,0x3EB8,
0x0F12,0x0000,	//awbb_CrclLowT_Rad_c
//White locus 
0x002A,0x0D2C,
0x0F12,0x013D,	//awbb_IntcR
0x0F12,0x011E,	//awbb_IntcB
0x002A,0x0D46,
0x0F12,0x0396,	//04A2	//awbb_MvEq_RBthresh
0x002A,0x0D5C,
0x0F12,0x0584,	//awbb_LowTempRB
      
//Grid,0x Correction
//	pa,0xram_start	awbb_GridCorr_R
0x002A,0x0DD4,
0x0F12,0x003C,	//awbb_GridCorr_R[0]
0x0F12,0x0050,	//awbb_GridCorr_R[1]
0x0F12,0x0064,	//awbb_GridCorr_R[2]
0x0F12,0x0000,	//awbb_GridCorr_R[3]
0x0F12,0x0014,	//awbb_GridCorr_R[4]
0x0F12,0x0014,	//awbb_GridCorr_R[5]
0x0F12,0x003C,	//awbb_GridCorr_R[6]
0x0F12,0x0050,	//awbb_GridCorr_R[7]
0x0F12,0x0064,	//awbb_GridCorr_R[8]
0x0F12,0x0000,	//awbb_GridCorr_R[9]
0x0F12,0x0014,	//awbb_GridCorr_R[10]
0x0F12,0x0014,	//awbb_GridCorr_R[11]
0x0F12,0x003C,	//awbb_GridCorr_R[12]
0x0F12,0x0050,	//awbb_GridCorr_R[13]
0x0F12,0x0064,	//awbb_GridCorr_R[14]
0x0F12,0x0000,	//awbb_GridCorr_R[15]
0x0F12,0x0014,	//awbb_GridCorr_R[16]
0x0F12,0x0014,	//awbb_GridCorr_R[17]
//	pa,0xram_end	awbb_GridCorr_R
//	pa,0xram_start	awbb_GridCorr_B
0x002A,0x0DF8,
0x0F12,0xFFEC,	//awbb_GridCorr_B[0]
0x0F12,0xFFEC,	//awbb_GridCorr_B[1]
0x0F12,0x0078,	//awbb_GridCorr_B[2]
0x0F12,0xFFE2,	//awbb_GridCorr_B[3]
0x0F12,0xFFE2,	//awbb_GridCorr_B[4]
0x0F12,0xFFB0,	//awbb_GridCorr_B[5]
0x0F12,0xFFEC,	//awbb_GridCorr_B[6]
0x0F12,0xFFEC,	//awbb_GridCorr_B[7]
0x0F12,0x0078,	//awbb_GridCorr_B[8]
0x0F12,0xFFE2,	//awbb_GridCorr_B[9]
0x0F12,0xFFE2,	//awbb_GridCorr_B[10]
0x0F12,0xFFB0,	//awbb_GridCorr_B[11]
0x0F12,0xFFEC,	//awbb_GridCorr_B[12]
0x0F12,0xFFEC,	//awbb_GridCorr_B[13]
0x0F12,0x0078,	//awbb_GridCorr_B[14]
0x0F12,0xFFE2,	//awbb_GridCorr_B[15]
0x0F12,0xFFE2,	//awbb_GridCorr_B[16]
0x0F12,0xFFB0,	//awbb_GridCorr_B[17]
//	pa,0xram_end	awbb_GridCorr_B
//	pa,0xram_start	awbb_GridConst_1 
0x002A,0x0E1C,
0x0F12,0x02D9,	//awbb_GridConst_1[0]
0x0F12,0x0357,	//awbb_GridConst_1[1]
0x0F12,0x03D1,	//awbb_GridConst_1[2]
//	pa,0xram_end	awbb_GridConst_1
//	pa,0xram_start	awbb_GridConst_2
0x002A,0x0E22,
0x0F12,0x0DE9,	//awbb_GridConst_2[0]
0x0F12,0x0EDD,	//awbb_GridConst_2[1]
0x0F12,0x0F42,	//awbb_GridConst_2[2]
0x0F12,0x0F54,	//awbb_GridConst_2[3]
0x0F12,0x0FAE,	//awbb_GridConst_2[4]
0x0F12,0x1011,	//awbb_GridConst_2[5]
//	pa,0xram_end	awbb_GridConst_2 
0x002A,0x0E2E,
0x0F12,0x00BA,	//awbb_GridCoeff_R_1
0x0F12,0x00AF,	//awbb_GridCoeff_B_1
0x0F12,0x0049,	//awbb_GridCoeff_R_2
0x0F12,0x00F5,	//awbb_GridCoeff_B_2
      
0x002A,0x0E4A,
0x0F12,0x0002,    	//awbb_GridEnable
//====,0x========================================================
//End ,0xAWB
//====,0x========================================================
//AE  ,0x
//====,0x========================================================
//AE v,0xariation
0x002A,0x051A,
0x0F12,0x010E,	//lt_uLimitHigh
0x0F12,0x00F5,	//lt_uLimitLow
//disa,0xble contrast, enable illumination
0x002A,0x0F76,
0x0F12,0x0001,  //
        
0x002A,0x1034, 
0x0F12,0x00C0,	//SARR_IllumType[0]
0x0F12,0x00E0,  //SARR_IllumType[1]
0x0F12,0x00F0,  //SARR_IllumType[2]
0x0F12,0x0129,  //SARR_IllumType[3]
0x0F12,0x0156,  //SARR_IllumType[4]
0x0F12,0x017F,  //SARR_IllumType[5]
0x0F12,0x018F,  //SARR_IllumType[6]
        
0x002A,0x1042,
0x0F12,0x0120,    //SARR_IllumTypeF[0]
0x0F12,0x0120,    //SARR_IllumTypeF[1]
0x0F12,0x0120,    //SARR_IllumTypeF[2]
0x0F12,0x0100,    //SARR_IllumTypeF[3]
0x0F12,0x0100,    //SARR_IllumTypeF[4]
0x0F12,0x0100,    //SARR_IllumTypeF[5]
0x0F12,0x0100,    //SARR_IllumTypeF[6]
      
//AE t,0xarget  
0x002A,0x0F70,
0x0F12,0x0046,  //TVAR_ae_BrAve
//AE W,0xeight               
0x0028,0x7000,                
0x002A,0x0f7e,    //AE weight/
0x0F12,0x0101,//0000                
0x0F12,0x0101,//0000                
0x0F12,0x0101,//0000                
0x0F12,0x0101,//0000                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0303,//0101                
0x0F12,0x0303,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0303,//0401                
0x0F12,0x0303,//0104                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0303,//0401                
0x0F12,0x0303,//0104                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0303,//0201                
0x0F12,0x0303,//0102                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0201                
0x0F12,0x0101,//0102                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
0x0F12,0x0101,//0101                
//============================================================   
//Gamm,0xa
//============================================================     
0x002A,0x3288,
0x0F12,0x0000, //#SARR_usDualGammaLutRGBIndoor[0][0]		0000		
0x0F12,0x0008, //#SARR_usDualGammaLutRGBIndoor[0][1]		0008		
0x0F12,0x0013, //#SARR_usDualGammaLutRGBIndoor[0][2]		0013		
0x0F12,0x002C, //#SARR_usDualGammaLutRGBIndoor[0][3]		002C		
0x0F12,0x0061, //#SARR_usDualGammaLutRGBIndoor[0][4]		0061		
0x0F12,0x00C8, //#SARR_usDualGammaLutRGBIndoor[0][5]		00C8		
0x0F12,0x0113, //#SARR_usDualGammaLutRGBIndoor[0][6]		0113		
0x0F12,0x0132, //#SARR_usDualGammaLutRGBIndoor[0][7]		0132		
0x0F12,0x014C, //#SARR_usDualGammaLutRGBIndoor[0][8]		014C		
0x0F12,0x0179, //#SARR_usDualGammaLutRGBIndoor[0][9]		0179		
0x0F12,0x01A4, //#SARR_usDualGammaLutRGBIndoor[0][10]	  01A4			
0x0F12,0x01CD, //#SARR_usDualGammaLutRGBIndoor[0][11]	  01CD			
0x0F12,0x01F4, //#SARR_usDualGammaLutRGBIndoor[0][12]	  01F4			
0x0F12,0x0239, //#SARR_usDualGammaLutRGBIndoor[0][13]	  0239			
0x0F12,0x0278, //#SARR_usDualGammaLutRGBIndoor[0][14]	  0278			
0x0F12,0x02E0, //#SARR_usDualGammaLutRGBIndoor[0][15]	  02E0			
0x0F12,0x0333, //#SARR_usDualGammaLutRGBIndoor[0][16]	  0333			
0x0F12,0x037B, //#SARR_usDualGammaLutRGBIndoor[0][17]	  037B			
0x0F12,0x03BF, //#SARR_usDualGammaLutRGBIndoor[0][18]	  03BF			
0x0F12,0x03FF, //#SARR_usDualGammaLutRGBIndoor[0][19]	  03FF			
0x0F12,0x0000, //#SARR_usDualGammaLutRGBIndoor[1][0]		0000		
0x0F12,0x0008, //#SARR_usDualGammaLutRGBIndoor[1][1]		0008		
0x0F12,0x0013, //#SARR_usDualGammaLutRGBIndoor[1][2]		0013		
0x0F12,0x002C, //#SARR_usDualGammaLutRGBIndoor[1][3]		002C		
0x0F12,0x0061, //#SARR_usDualGammaLutRGBIndoor[1][4]		0061		
0x0F12,0x00C8, //#SARR_usDualGammaLutRGBIndoor[1][5]		00C8		
0x0F12,0x0113, //#SARR_usDualGammaLutRGBIndoor[1][6]		0113		
0x0F12,0x0132, //#SARR_usDualGammaLutRGBIndoor[1][7]		0132		
0x0F12,0x014C, //#SARR_usDualGammaLutRGBIndoor[1][8]		014C		
0x0F12,0x0179, //#SARR_usDualGammaLutRGBIndoor[1][9]		0179		
0x0F12,0x01A4, //#SARR_usDualGammaLutRGBIndoor[1][10]	  01A4			
0x0F12,0x01CD, //#SARR_usDualGammaLutRGBIndoor[1][11]	  01CD			
0x0F12,0x01F4, //#SARR_usDualGammaLutRGBIndoor[1][12]	  01F4			
0x0F12,0x0239, //#SARR_usDualGammaLutRGBIndoor[1][13]	  0239			
0x0F12,0x0278, //#SARR_usDualGammaLutRGBIndoor[1][14]	  0278			
0x0F12,0x02E0, //#SARR_usDualGammaLutRGBIndoor[1][15]	  02E0			
0x0F12,0x0333, //#SARR_usDualGammaLutRGBIndoor[1][16]	  0333			
0x0F12,0x037B, //#SARR_usDualGammaLutRGBIndoor[1][17]	  037B			
0x0F12,0x03BF, //#SARR_usDualGammaLutRGBIndoor[1][18]	  03BF			
0x0F12,0x03FF, //#SARR_usDualGammaLutRGBIndoor[1][19]	  03FF			
0x0F12,0x0000, //#SARR_usDualGammaLutRGBIndoor[2][0]		0000		
0x0F12,0x0008, //#SARR_usDualGammaLutRGBIndoor[2][1]		0008		
0x0F12,0x0013, //#SARR_usDualGammaLutRGBIndoor[2][2]		0013		
0x0F12,0x002C, //#SARR_usDualGammaLutRGBIndoor[2][3]		002C		
0x0F12,0x0061, //#SARR_usDualGammaLutRGBIndoor[2][4]		0061		
0x0F12,0x00C8, //#SARR_usDualGammaLutRGBIndoor[2][5]		00C8		
0x0F12,0x0113, //#SARR_usDualGammaLutRGBIndoor[2][6]		0113		
0x0F12,0x0132, //#SARR_usDualGammaLutRGBIndoor[2][7]		0132		
0x0F12,0x014C, //#SARR_usDualGammaLutRGBIndoor[2][8]		014C		
0x0F12,0x0179, //#SARR_usDualGammaLutRGBIndoor[2][9]		0179		
0x0F12,0x01A4, //#SARR_usDualGammaLutRGBIndoor[2][10]	  01A4			
0x0F12,0x01CD, //#SARR_usDualGammaLutRGBIndoor[2][11]	  01CD			
0x0F12,0x01F4, //#SARR_usDualGammaLutRGBIndoor[2][12]	  01F4			
0x0F12,0x0239, //#SARR_usDualGammaLutRGBIndoor[2][13]	  0239			
0x0F12,0x0278, //#SARR_usDualGammaLutRGBIndoor[2][14]	  0278			
0x0F12,0x02E0, //#SARR_usDualGammaLutRGBIndoor[2][15]	  02E0			
0x0F12,0x0333, //#SARR_usDualGammaLutRGBIndoor[2][16]	  0333			
0x0F12,0x037B, //#SARR_usDualGammaLutRGBIndoor[2][17]	  037B			
0x0F12,0x03BF, //#SARR_usDualGammaLutRGBIndoor[2][18]	  03BF			
0x0F12,0x03FF, //#SARR_usDualGammaLutRGBIndoor[2][19]	  03FF			
      
0x002A,0x3300,
0x0F12,0x0000,	 //#SARR_usDualGammaLutRGBOutdoor[0][0]	0000			
0x0F12,0x0008,	 //#SARR_usDualGammaLutRGBOutdoor[0][1]	0008			
0x0F12,0x0013,	 //#SARR_usDualGammaLutRGBOutdoor[0][2]	0013			
0x0F12,0x002C,	 //#SARR_usDualGammaLutRGBOutdoor[0][3]	002C			
0x0F12,0x0061,	 //#SARR_usDualGammaLutRGBOutdoor[0][4]	0061			
0x0F12,0x00C8,	 //#SARR_usDualGammaLutRGBOutdoor[0][5]	00C8			
0x0F12,0x0113,	 //#SARR_usDualGammaLutRGBOutdoor[0][6]	0113			
0x0F12,0x0132,	 //#SARR_usDualGammaLutRGBOutdoor[0][7]	0132			
0x0F12,0x014C,	 //#SARR_usDualGammaLutRGBOutdoor[0][8]	014C			
0x0F12,0x0179,	 //#SARR_usDualGammaLutRGBOutdoor[0][9]	0179			
0x0F12,0x01A4,	 //#SARR_usDualGammaLutRGBOutdoor[0][10]	01A4		
0x0F12,0x01CD,	 //#SARR_usDualGammaLutRGBOutdoor[0][11]	01CD		
0x0F12,0x01F4,	 //#SARR_usDualGammaLutRGBOutdoor[0][12]	01F4		
0x0F12,0x0239,	 //#SARR_usDualGammaLutRGBOutdoor[0][13]	0239		
0x0F12,0x0278,	 //#SARR_usDualGammaLutRGBOutdoor[0][14]	0278		
0x0F12,0x02E0,	 //#SARR_usDualGammaLutRGBOutdoor[0][15]	02E0		
0x0F12,0x0333,	 //#SARR_usDualGammaLutRGBOutdoor[0][16]	0333		
0x0F12,0x037B,	 //#SARR_usDualGammaLutRGBOutdoor[0][17]	037B		
0x0F12,0x03BF,	 //#SARR_usDualGammaLutRGBOutdoor[0][18]	03BF		
0x0F12,0x03FF,	 //#SARR_usDualGammaLutRGBOutdoor[0][19]	03FF		
0x0F12,0x0000,	 //#SARR_usDualGammaLutRGBOutdoor[1][0]	0000			
0x0F12,0x0008,	 //#SARR_usDualGammaLutRGBOutdoor[1][1]	0008			
0x0F12,0x0013,	 //#SARR_usDualGammaLutRGBOutdoor[1][2]	0013			
0x0F12,0x002C,	 //#SARR_usDualGammaLutRGBOutdoor[1][3]	002C			
0x0F12,0x0061,	 //#SARR_usDualGammaLutRGBOutdoor[1][4]	0061			
0x0F12,0x00C8,	 //#SARR_usDualGammaLutRGBOutdoor[1][5]	00C8			
0x0F12,0x0113,	 //#SARR_usDualGammaLutRGBOutdoor[1][6]	0113			
0x0F12,0x0132,	 //#SARR_usDualGammaLutRGBOutdoor[1][7]	0132			
0x0F12,0x014C,	 //#SARR_usDualGammaLutRGBOutdoor[1][8]	014C			
0x0F12,0x0179,	 //#SARR_usDualGammaLutRGBOutdoor[1][9]	0179			
0x0F12,0x01A4,	 //#SARR_usDualGammaLutRGBOutdoor[1][10]	01A4		
0x0F12,0x01CD,	 //#SARR_usDualGammaLutRGBOutdoor[1][11]	01CD		
0x0F12,0x01F4,	 //#SARR_usDualGammaLutRGBOutdoor[1][12]	01F4		
0x0F12,0x0239,	 //#SARR_usDualGammaLutRGBOutdoor[1][13]	0239		
0x0F12,0x0278,	 //#SARR_usDualGammaLutRGBOutdoor[1][14]	0278		
0x0F12,0x02E0,	 //#SARR_usDualGammaLutRGBOutdoor[1][15]	02E0		
0x0F12,0x0333,	 //#SARR_usDualGammaLutRGBOutdoor[1][16]	0333		
0x0F12,0x037B,	 //#SARR_usDualGammaLutRGBOutdoor[1][17]	037B		
0x0F12,0x03BF,	 //#SARR_usDualGammaLutRGBOutdoor[1][18]	03BF		
0x0F12,0x03FF,	 //#SARR_usDualGammaLutRGBOutdoor[1][19]	03FF		
0x0F12,0x0000,	 //#SARR_usDualGammaLutRGBOutdoor[2][0]	0000			
0x0F12,0x0008,	 //#SARR_usDualGammaLutRGBOutdoor[2][1]	0008			
0x0F12,0x0013,	 //#SARR_usDualGammaLutRGBOutdoor[2][2]	0013			
0x0F12,0x002C,	 //#SARR_usDualGammaLutRGBOutdoor[2][3]	002C			
0x0F12,0x0061,	 //#SARR_usDualGammaLutRGBOutdoor[2][4]	0061			
0x0F12,0x00C8,	 //#SARR_usDualGammaLutRGBOutdoor[2][5]	00C8			
0x0F12,0x0113,	 //#SARR_usDualGammaLutRGBOutdoor[2][6]	0113			
0x0F12,0x0132,	 //#SARR_usDualGammaLutRGBOutdoor[2][7]	0132			
0x0F12,0x014C,	 //#SARR_usDualGammaLutRGBOutdoor[2][8]	014C			
0x0F12,0x0179,	 //#SARR_usDualGammaLutRGBOutdoor[2][9]	0179			
0x0F12,0x01A4,	 //#SARR_usDualGammaLutRGBOutdoor[2][10]	01A4		
0x0F12,0x01CD,	 //#SARR_usDualGammaLutRGBOutdoor[2][11]	01CD		
0x0F12,0x01F4,	 //#SARR_usDualGammaLutRGBOutdoor[2][12]	01F4		
0x0F12,0x0239,	 //#SARR_usDualGammaLutRGBOutdoor[2][13]	0239		
0x0F12,0x0278,	 //#SARR_usDualGammaLutRGBOutdoor[2][14]	0278		
0x0F12,0x02E0,	 //#SARR_usDualGammaLutRGBOutdoor[2][15]	02E0		
0x0F12,0x0333,	 //#SARR_usDualGammaLutRGBOutdoor[2][16]	0333		
0x0F12,0x037B,	 //#SARR_usDualGammaLutRGBOutdoor[2][17]	037B		
0x0F12,0x03BF,	 //#SARR_usDualGammaLutRGBOutdoor[2][18]	03BF		
0x0F12,0x03FF,	 //#SARR_usDualGammaLutRGBOutdoor[2][19]	03FF		
      
//====,0x========================================================
//CCM ,0x
//====,0x======================================================== 
0x002A,0x06A6,
0x0F12,0x00C0,    	//SARR_AwbCcmCord[0]                        	   
0x0F12,0x00F8,    	//SARR_AwbCcmCord[1]                        	   
0x0F12,0x0112,    	//SARR_AwbCcmCord[2]                        	   
0x0F12,0x014A,    	//SARR_AwbCcmCord[3]                        	   
0x0F12,0x0156,    	//SARR_AwbCcmCord[4]                        	   
0x0F12,0x017F,    	//SARR_AwbCcmCord[5]    
      
//	pa,0xram_start	TVAR_wbt_pBaseCcms 
// Hor,0xizon
0x002A,0x33A4,
0x0F12,0x01C3,//02D5	//TVAR_wbt_pBaseCcms[0]
0x0F12,0xFF89,//FF53	//TVAR_wbt_pBaseCcms[1]
0x0F12,0xFFE5,//FF83	//TVAR_wbt_pBaseCcms[2]
0x0F12,0xFF26,//FEE9	//TVAR_wbt_pBaseCcms[3]
0x0F12,0x028E,//01A2	//TVAR_wbt_pBaseCcms[4]
0x0F12,0xFF80,//FDFE	//TVAR_wbt_pBaseCcms[5]
0x0F12,0x0002,//FFFC	//TVAR_wbt_pBaseCcms[6]
0x0F12,0xFFA8,//FF5F	//TVAR_wbt_pBaseCcms[7]
0x0F12,0x01F0,//02AD	//TVAR_wbt_pBaseCcms[8]
0x0F12,0x0125,//0125	//TVAR_wbt_pBaseCcms[9]
0x0F12,0x0119,//0119	//TVAR_wbt_pBaseCcms[10]
0x0F12,0xFE5A,//FE5A	//TVAR_wbt_pBaseCcms[11]
0x0F12,0x0179,//00D9	//TVAR_wbt_pBaseCcms[12]
0x0F12,0xFF8A,//FF26	//TVAR_wbt_pBaseCcms[13]
0x0F12,0x0180,//013C	//TVAR_wbt_pBaseCcms[14]
0x0F12,0xFEC2,//FEC0	//TVAR_wbt_pBaseCcms[15]
0x0F12,0x0176,//01F3	//TVAR_wbt_pBaseCcms[16]
0x0F12,0x0094,//0109	//TVAR_wbt_pBaseCcms[17] 
//inca,0x                                       
0x0F12,0x01C3,//022B	//TVAR_wbt_pBaseCcms[18]
0x0F12,0xFF89,//FF73	//TVAR_wbt_pBaseCcms[19]
0x0F12,0xFFE5,//FFC3	//TVAR_wbt_pBaseCcms[20]
0x0F12,0xFF26,//FEED	//TVAR_wbt_pBaseCcms[21]
0x0F12,0x028E,//01B9	//TVAR_wbt_pBaseCcms[22]
0x0F12,0xFF80,//FF0D	//TVAR_wbt_pBaseCcms[23]
0x0F12,0x0002,//FFE7	//TVAR_wbt_pBaseCcms[24]
0x0F12,0xFFA8,//FFD3	//TVAR_wbt_pBaseCcms[25]
0x0F12,0x01F0,//022F	//TVAR_wbt_pBaseCcms[26]
0x0F12,0x0125,//0118	//TVAR_wbt_pBaseCcms[27]
0x0F12,0x0119,//012C	//TVAR_wbt_pBaseCcms[28]
0x0F12,0xFE5A,//FED0	//TVAR_wbt_pBaseCcms[29]
0x0F12,0x0179,//0212	//TVAR_wbt_pBaseCcms[30]
0x0F12,0xFF8A,//FF46	//TVAR_wbt_pBaseCcms[31]
0x0F12,0x0180,//01F2	//TVAR_wbt_pBaseCcms[32]
0x0F12,0xFEC2,//FED4	//TVAR_wbt_pBaseCcms[33]
0x0F12,0x0176,//018C	//TVAR_wbt_pBaseCcms[34]
0x0F12,0x0094,//013C	//TVAR_wbt_pBaseCcms[35]
//WW  ,0x
0x0F12,0x01CA,//0121	//TVAR_wbt_pBaseCcms[36]
0x0F12,0xFF89,//FF8D	//TVAR_wbt_pBaseCcms[37]
0x0F12,0xFFE0,//FFA7	//TVAR_wbt_pBaseCcms[38]
0x0F12,0xFF26,//FF3D	//TVAR_wbt_pBaseCcms[39]
0x0F12,0x028E,//02D0	//TVAR_wbt_pBaseCcms[40]
0x0F12,0xFF80,//FE31	//TVAR_wbt_pBaseCcms[41]
0x0F12,0x0020,//0016	//TVAR_wbt_pBaseCcms[42]
0x0F12,0xFFF8,//003B	//TVAR_wbt_pBaseCcms[43]
0x0F12,0x01E0,//02E1	//TVAR_wbt_pBaseCcms[44]
0x0F12,0x0120,//0175	//TVAR_wbt_pBaseCcms[45]
0x0F12,0x00FA,//009C	//TVAR_wbt_pBaseCcms[46]
0x0F12,0xFF12,//FE3A	//TVAR_wbt_pBaseCcms[47]
0x0F12,0x0179,//0189	//TVAR_wbt_pBaseCcms[48]
0x0F12,0xFF8A,//FF6A	//TVAR_wbt_pBaseCcms[49]
0x0F12,0x0180,//0298	//TVAR_wbt_pBaseCcms[50]
0x0F12,0xFEC2,//FF14	//TVAR_wbt_pBaseCcms[51]
0x0F12,0x0176,//01BD	//TVAR_wbt_pBaseCcms[52]
0x0F12,0x0094,//00FB	//TVAR_wbt_pBaseCcms[53]
//CWF ,0x                                      
0x0F12,0x01CA,//018A//0262	//TVAR_wbt_pBaseCcms[54]
0x0F12,0xFF89,//FFC9//FFAB	//TVAR_wbt_pBaseCcms[55]
0x0F12,0xFFE0,//0005//FFFB	//TVAR_wbt_pBaseCcms[56]
0x0F12,0xFF26,//FFC1	//TVAR_wbt_pBaseCcms[57]
0x0F12,0x028E,//0292	//TVAR_wbt_pBaseCcms[58]
0x0F12,0xFF80,//FF81	//TVAR_wbt_pBaseCcms[59]
0x0F12,0x0020,//0069	//TVAR_wbt_pBaseCcms[60]
0x0F12,0xFFF8,//0057	//TVAR_wbt_pBaseCcms[61]
0x0F12,0x01E0,//0296	//TVAR_wbt_pBaseCcms[62]
0x0F12,0x0120,//00FF//015F	//TVAR_wbt_pBaseCcms[63]
0x0F12,0x00FA,//00E2//0112	//TVAR_wbt_pBaseCcms[64]
0x0F12,0xFF12,//FF4D//FF17	//TVAR_wbt_pBaseCcms[65]
0x0F12,0x0179,//024E	//TVAR_wbt_pBaseCcms[66]
0x0F12,0xFF8A,//0001	//TVAR_wbt_pBaseCcms[67]
0x0F12,0x0180,//0276	//TVAR_wbt_pBaseCcms[68]
0x0F12,0xFEC2,//FEE2	//TVAR_wbt_pBaseCcms[69]
0x0F12,0x0176,//0236	//TVAR_wbt_pBaseCcms[70]
0x0F12,0x0094,//014A	//TVAR_wbt_pBaseCcms[71] 
//D50 ,0x
0x0F12,0x018A,//01C5	//TVAR_wbt_pBaseCcms[72]
0x0F12,0xFFC9,//FFA8	//TVAR_wbt_pBaseCcms[73]
0x0F12,0x0005,//FFBA	//TVAR_wbt_pBaseCcms[74]
0x0F12,0xFF56,//FF06	//TVAR_wbt_pBaseCcms[75]
0x0F12,0x0230,//0258	//TVAR_wbt_pBaseCcms[76]
0x0F12,0xFFA0,//FF52	//TVAR_wbt_pBaseCcms[77]
0x0F12,0x0020,//0036	//TVAR_wbt_pBaseCcms[78]
0x0F12,0xFFF8,//FFFA	//TVAR_wbt_pBaseCcms[79]
0x0F12,0x01E0,//01D7	//TVAR_wbt_pBaseCcms[80]
0x0F12,0x00FF,//00DE	//TVAR_wbt_pBaseCcms[81]
0x0F12,0x00E2,//013B	//TVAR_wbt_pBaseCcms[82]
0x0F12,0xFF4D,//FF30	//TVAR_wbt_pBaseCcms[83]
0x0F12,0x0179,//0168	//TVAR_wbt_pBaseCcms[84]
0x0F12,0xFF8A,//FF6F	//TVAR_wbt_pBaseCcms[85]
0x0F12,0x0180,//0164	//TVAR_wbt_pBaseCcms[86]
0x0F12,0xFEC2,//FEDB	//TVAR_wbt_pBaseCcms[87]
0x0F12,0x0176,//0195	//TVAR_wbt_pBaseCcms[88]
0x0F12,0x0094,//00E7	//TVAR_wbt_pBaseCcms[89]
//D65 ,0x
0x0F12,0x018A,//019E	//TVAR_wbt_pBaseCcms[90]
0x0F12,0xFFC9,//FF8A	//TVAR_wbt_pBaseCcms[91]
0x0F12,0x0005,//FFCE	//TVAR_wbt_pBaseCcms[92]
0x0F12,0xFF56,//FF06	//TVAR_wbt_pBaseCcms[93]
0x0F12,0x0230,//0258	//TVAR_wbt_pBaseCcms[94]
0x0F12,0xFFA0,//FF52	//TVAR_wbt_pBaseCcms[95]
0x0F12,0x0020,//0036	//TVAR_wbt_pBaseCcms[96]
0x0F12,0xFFF8,//FFFA	//TVAR_wbt_pBaseCcms[97]
0x0F12,0x01E0,//01D7	//TVAR_wbt_pBaseCcms[98]
0x0F12,0x00FF,//00DE	//TVAR_wbt_pBaseCcms[99]
0x0F12,0x00E2,//013B	//TVAR_wbt_pBaseCcms[100]
0x0F12,0xFF4D,//FF30	//TVAR_wbt_pBaseCcms[101]
0x0F12,0x0179,//0168	//TVAR_wbt_pBaseCcms[102]
0x0F12,0xFF8A,//FF6F	//TVAR_wbt_pBaseCcms[103]
0x0F12,0x0180,//0164	//TVAR_wbt_pBaseCcms[104]
0x0F12,0xFEC2,//FEDB	//TVAR_wbt_pBaseCcms[105]
0x0F12,0x0176,//0195	//TVAR_wbt_pBaseCcms[106]
0x0F12,0x0094,//00E7	//TVAR_wbt_pBaseCcms[107]
//	pa,0xram_end	TVAR_wbt_pBaseCcms
//	pa,0xram_start	TVAR_wbt_pOutdoorCcm  
0x002A,0x3380,
0x0F12,0x018A,//01C7	//TVAR_wbt_pOutdoorCcm[0]
0x0F12,0xFFC9,//FFA0	//TVAR_wbt_pOutdoorCcm[1]
0x0F12,0x0005,//FFE8	//TVAR_wbt_pOutdoorCcm[2]
0x0F12,0xFF26,//FF11	//TVAR_wbt_pOutdoorCcm[3]
0x0F12,0x028E,//01F4	//TVAR_wbt_pOutdoorCcm[4]
0x0F12,0xFF80,//FF38	//TVAR_wbt_pOutdoorCcm[5]
0x0F12,0x0020,//FFDF	//TVAR_wbt_pOutdoorCcm[6]
0x0F12,0xFFF8,//FFD3	//TVAR_wbt_pOutdoorCcm[7]
0x0F12,0x01E0,//01CC	//TVAR_wbt_pOutdoorCcm[8]
0x0F12,0x00FF,//011D	//TVAR_wbt_pOutdoorCcm[9]
0x0F12,0x00E2,//0157	//TVAR_wbt_pOutdoorCcm[10]
0x0F12,0xFF4D,//FF16	//TVAR_wbt_pOutdoorCcm[11]
0x0F12,0x0179,//01DA	//TVAR_wbt_pOutdoorCcm[12]
0x0F12,0xFF8A,//FF3A	//TVAR_wbt_pOutdoorCcm[13]
0x0F12,0x0180,//01B6	//TVAR_wbt_pOutdoorCcm[14]
0x0F12,0xFEC2,//FF2A	//TVAR_wbt_pOutdoorCcm[15]
0x0F12,0x0176,//0176	//TVAR_wbt_pOutdoorCcm[16]
0x0F12,0x0094,//0114	//TVAR_wbt_pOutdoorCcm[17]
//	pa,0xram_end	TVAR_wbt_pOutdoorCcm
//====,0x========================================================
//AFIT,0x
//====,0x========================================================
//	pa,0xram_start	afit_uNoiseIndInDoor
//====,0x========================================================  
0x002A,0x0764,
0x0F12,0x0041,	//afit_uNoiseIndInDoor[0]
0x0F12,0x00A5,	//afit_uNoiseIndInDoor[1]
0x0F12,0x016B,	//afit_uNoiseIndInDoor[2]
0x0F12,0x0226,	//afit_uNoiseIndInDoor[3]
0x0F12,0x02EA,	//afit_uNoiseIndInDoor[4]
//	pa,0xram_end	afit_uNoiseIndInDoor
//	pa,0xram_start	TVAR_afit_pBaseVals
0x002A,0x07C4,
0x0F12,0x0000,	//TVAR_afit_pBaseVals[0]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[1]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[2]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[3]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[4]
0x0F12,0x001E,	//TVAR_afit_pBaseVals[5]
0x0F12,0x03FF,	//TVAR_afit_pBaseVals[6]
0x0F12,0x009C,	//TVAR_afit_pBaseVals[7]
0x0F12,0x017C,	//TVAR_afit_pBaseVals[8]
0x0F12,0x03FF,	//TVAR_afit_pBaseVals[9]
0x0F12,0x000C,	//TVAR_afit_pBaseVals[10]
0x0F12,0x0010,	//TVAR_afit_pBaseVals[11]
0x0F12,0x0104,	//TVAR_afit_pBaseVals[12]
0x0F12,0x03E8,	//TVAR_afit_pBaseVals[13]
0x0F12,0x0023,	//TVAR_afit_pBaseVals[14]
0x0F12,0x012C,	//TVAR_afit_pBaseVals[15]
0x0F12,0x0070,	//TVAR_afit_pBaseVals[16]
0x0F12,0x0004,	//TVAR_afit_pBaseVals[17]
0x0F12,0x0004,	//TVAR_afit_pBaseVals[18]
0x0F12,0x01AA,	//TVAR_afit_pBaseVals[19]
0x0F12,0x0064,	//TVAR_afit_pBaseVals[20]
0x0F12,0x0064,	//TVAR_afit_pBaseVals[21]
0x0F12,0x000A,	//TVAR_afit_pBaseVals[22]
0x0F12,0x000A,	//TVAR_afit_pBaseVals[23]
0x0F12,0x002A,	//TVAR_afit_pBaseVals[24]
0x0F12,0x0024,	//TVAR_afit_pBaseVals[25]
0x0F12,0x002A,	//TVAR_afit_pBaseVals[26]
0x0F12,0x0024,	//TVAR_afit_pBaseVals[27]
0x0F12,0x002A,	//TVAR_afit_pBaseVals[28]
0x0F12,0x0024,	//TVAR_afit_pBaseVals[29]
0x0F12,0x0A0F,	//TVAR_afit_pBaseVals[30]
0x0F12,0x1701,	//TVAR_afit_pBaseVals[31]
0x0F12,0x0229,	//TVAR_afit_pBaseVals[32]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[33]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[34]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[35]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[36]
0x0F12,0x00FF,	//TVAR_afit_pBaseVals[37]
0x0F12,0x053B,	//TVAR_afit_pBaseVals[38]
0x0F12,0x0505,	//TVAR_afit_pBaseVals[39]
0x0F12,0x0301,	//TVAR_afit_pBaseVals[40]
0x0F12,0x8007,	//TVAR_afit_pBaseVals[41]
0x0F12,0x051E,	//TVAR_afit_pBaseVals[42]
0x0F12,0x0A1E,	//TVAR_afit_pBaseVals[43]
0x0F12,0x0F0F,	//TVAR_afit_pBaseVals[44]
0x0F12,0x0A05,	//TVAR_afit_pBaseVals[45]
0x0F12,0x103C,	//TVAR_afit_pBaseVals[46]
0x0F12,0x0A28,	//TVAR_afit_pBaseVals[47]
0x0F12,0x0002,	//TVAR_afit_pBaseVals[48]
0x0F12,0x00FF,	//TVAR_afit_pBaseVals[49]
0x0F12,0x1002,	//TVAR_afit_pBaseVals[50]
0x0F12,0x001D,	//TVAR_afit_pBaseVals[51]
0x0F12,0x0900,	//TVAR_afit_pBaseVals[52]
0x0F12,0x0600,	//TVAR_afit_pBaseVals[53]
0x0F12,0x0504,	//TVAR_afit_pBaseVals[54]
0x0F12,0x0305,	//TVAR_afit_pBaseVals[55]
0x0F12,0x5A03,	//TVAR_afit_pBaseVals[56]
0x0F12,0x006E,	//TVAR_afit_pBaseVals[57]
0x0F12,0x0A78,	//TVAR_afit_pBaseVals[58]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[59]
0x0F12,0x3232,	//TVAR_afit_pBaseVals[60]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[61]
0x0F12,0x5001,	//TVAR_afit_pBaseVals[62]
0x0F12,0x7850,	//TVAR_afit_pBaseVals[63]
0x0F12,0x2878,	//TVAR_afit_pBaseVals[64]
0x0F12,0x0A00,	//TVAR_afit_pBaseVals[65]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[66]
0x0F12,0x1E07,	//TVAR_afit_pBaseVals[67]
0x0F12,0x070A,	//TVAR_afit_pBaseVals[68]
0x0F12,0x32FF,	//TVAR_afit_pBaseVals[69]
0x0F12,0x5004,	//TVAR_afit_pBaseVals[70]
0x0F12,0x0F40,	//TVAR_afit_pBaseVals[71]
0x0F12,0x400F,	//TVAR_afit_pBaseVals[72]
0x0F12,0x0204,	//TVAR_afit_pBaseVals[73]
0x0F12,0x3203,	//TVAR_afit_pBaseVals[74]
0x0F12,0x0132,	//TVAR_afit_pBaseVals[75]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[76]
0x0F12,0x5050,	//TVAR_afit_pBaseVals[77]
0x0F12,0x7878,	//TVAR_afit_pBaseVals[78]
0x0F12,0x0028,	//TVAR_afit_pBaseVals[79]
0x0F12,0x030A,	//TVAR_afit_pBaseVals[80]
0x0F12,0x0714,	//TVAR_afit_pBaseVals[81]
0x0F12,0x0A1E,	//TVAR_afit_pBaseVals[82]
0x0F12,0xFF07,	//TVAR_afit_pBaseVals[83]
0x0F12,0x0432,	//TVAR_afit_pBaseVals[84]
0x0F12,0x4050,	//TVAR_afit_pBaseVals[85]
0x0F12,0x0F0F,	//TVAR_afit_pBaseVals[86]
0x0F12,0x0440,	//TVAR_afit_pBaseVals[87]
0x0F12,0x0302,	//TVAR_afit_pBaseVals[88]
0x0F12,0x3232,	//TVAR_afit_pBaseVals[89]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[90]
0x0F12,0x5001,	//TVAR_afit_pBaseVals[91]
0x0F12,0x7850,	//TVAR_afit_pBaseVals[92]
0x0F12,0x2878,	//TVAR_afit_pBaseVals[93]
0x0F12,0x0A00,	//TVAR_afit_pBaseVals[94]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[95]
0x0F12,0x1E07,	//TVAR_afit_pBaseVals[96]
0x0F12,0x070A,	//TVAR_afit_pBaseVals[97]
0x0F12,0x32FF,	//TVAR_afit_pBaseVals[98]
0x0F12,0x5004,	//TVAR_afit_pBaseVals[99]
0x0F12,0x0F40,	//TVAR_afit_pBaseVals[100]
0x0F12,0x400F,	//TVAR_afit_pBaseVals[101]
0x0F12,0x0204,	//TVAR_afit_pBaseVals[102]
0x0F12,0x0003,	//TVAR_afit_pBaseVals[103]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[104]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[105]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[106]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[107]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[108]
0x0F12,0x001E,	//TVAR_afit_pBaseVals[109]
0x0F12,0x03FF,	//TVAR_afit_pBaseVals[110]
0x0F12,0x009C,	//TVAR_afit_pBaseVals[111]
0x0F12,0x017C,	//TVAR_afit_pBaseVals[112]
0x0F12,0x03FF,	//TVAR_afit_pBaseVals[113]
0x0F12,0x000C,	//TVAR_afit_pBaseVals[114]
0x0F12,0x0010,	//TVAR_afit_pBaseVals[115]
0x0F12,0x0104,	//TVAR_afit_pBaseVals[116]
0x0F12,0x03E8,	//TVAR_afit_pBaseVals[117]
0x0F12,0x0023,	//TVAR_afit_pBaseVals[118]
0x0F12,0x012C,	//TVAR_afit_pBaseVals[119]
0x0F12,0x0070,	//TVAR_afit_pBaseVals[120]
0x0F12,0x0004,	//TVAR_afit_pBaseVals[121]
0x0F12,0x0004,	//TVAR_afit_pBaseVals[122]
0x0F12,0x01AA,	//TVAR_afit_pBaseVals[123]
0x0F12,0x001E,	//TVAR_afit_pBaseVals[124]
0x0F12,0x001E,	//TVAR_afit_pBaseVals[125]
0x0F12,0x0005,	//TVAR_afit_pBaseVals[126]
0x0F12,0x0005,	//TVAR_afit_pBaseVals[127]
0x0F12,0x002A,	//TVAR_afit_pBaseVals[128]
0x0F12,0x0024,	//TVAR_afit_pBaseVals[129]
0x0F12,0x002A,	//TVAR_afit_pBaseVals[130]
0x0F12,0x0024,	//TVAR_afit_pBaseVals[131]
0x0F12,0x002A,	//TVAR_afit_pBaseVals[132]
0x0F12,0x0024,	//TVAR_afit_pBaseVals[133]
0x0F12,0x0A0F,	//TVAR_afit_pBaseVals[134]
0x0F12,0x1701,	//TVAR_afit_pBaseVals[135]
0x0F12,0x0229,	//TVAR_afit_pBaseVals[136]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[137]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[138]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[139]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[140]
0x0F12,0x00FF,	//TVAR_afit_pBaseVals[141]
0x0F12,0x053B,	//TVAR_afit_pBaseVals[142]
0x0F12,0x0505,	//TVAR_afit_pBaseVals[143]
0x0F12,0x0301,	//TVAR_afit_pBaseVals[144]
0x0F12,0x8007,	//TVAR_afit_pBaseVals[145]
0x0F12,0x051E,	//TVAR_afit_pBaseVals[146]
0x0F12,0x0A1E,	//TVAR_afit_pBaseVals[147]
0x0F12,0x0F0F,	//TVAR_afit_pBaseVals[148]
0x0F12,0x0A04,	//TVAR_afit_pBaseVals[149]
0x0F12,0x103C,	//TVAR_afit_pBaseVals[150]
0x0F12,0x0A28,	//TVAR_afit_pBaseVals[151]
0x0F12,0x0002,	//TVAR_afit_pBaseVals[152]
0x0F12,0x00FF,	//TVAR_afit_pBaseVals[153]
0x0F12,0x1002,	//TVAR_afit_pBaseVals[154]
0x0F12,0x001D,	//TVAR_afit_pBaseVals[155]
0x0F12,0x0900,	//TVAR_afit_pBaseVals[156]
0x0F12,0x0600,	//TVAR_afit_pBaseVals[157]
0x0F12,0x0504,	//TVAR_afit_pBaseVals[158]
0x0F12,0x0305,	//TVAR_afit_pBaseVals[159]
0x0F12,0x5F03,	//TVAR_afit_pBaseVals[160]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[161]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[162]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[163]
0x0F12,0x3232,	//TVAR_afit_pBaseVals[164]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[165]
0x0F12,0x2A01,	//TVAR_afit_pBaseVals[166]
0x0F12,0x3232,	//TVAR_afit_pBaseVals[167]
0x0F12,0x2830,	//TVAR_afit_pBaseVals[168]
0x0F12,0x0A00,	//TVAR_afit_pBaseVals[169]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[170]
0x0F12,0x1E07,	//TVAR_afit_pBaseVals[171]
0x0F12,0x070A,	//TVAR_afit_pBaseVals[172]
0x0F12,0x32FF,	//TVAR_afit_pBaseVals[173]
0x0F12,0x5004,	//TVAR_afit_pBaseVals[174]
0x0F12,0x0F40,	//TVAR_afit_pBaseVals[175]
0x0F12,0x400F,	//TVAR_afit_pBaseVals[176]
0x0F12,0x0204,	//TVAR_afit_pBaseVals[177]
0x0F12,0x3203,	//TVAR_afit_pBaseVals[178]
0x0F12,0x0132,	//TVAR_afit_pBaseVals[179]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[180]
0x0F12,0x262A,	//TVAR_afit_pBaseVals[181]
0x0F12,0x3032,	//TVAR_afit_pBaseVals[182]
0x0F12,0x0028,	//TVAR_afit_pBaseVals[183]
0x0F12,0x030A,	//TVAR_afit_pBaseVals[184]
0x0F12,0x0714,	//TVAR_afit_pBaseVals[185]
0x0F12,0x0A1E,	//TVAR_afit_pBaseVals[186]
0x0F12,0xFF07,	//TVAR_afit_pBaseVals[187]
0x0F12,0x0432,	//TVAR_afit_pBaseVals[188]
0x0F12,0x4050,	//TVAR_afit_pBaseVals[189]
0x0F12,0x0F0F,	//TVAR_afit_pBaseVals[190]
0x0F12,0x0440,	//TVAR_afit_pBaseVals[191]
0x0F12,0x0302,	//TVAR_afit_pBaseVals[192]
0x0F12,0x3232,	//TVAR_afit_pBaseVals[193]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[194]
0x0F12,0x2A01,	//TVAR_afit_pBaseVals[195]
0x0F12,0x3226,	//TVAR_afit_pBaseVals[196]
0x0F12,0x2830,	//TVAR_afit_pBaseVals[197]
0x0F12,0x0A00,	//TVAR_afit_pBaseVals[198]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[199]
0x0F12,0x1E07,	//TVAR_afit_pBaseVals[200]
0x0F12,0x070A,	//TVAR_afit_pBaseVals[201]
0x0F12,0x32FF,	//TVAR_afit_pBaseVals[202]
0x0F12,0x5004,	//TVAR_afit_pBaseVals[203]
0x0F12,0x0F40,	//TVAR_afit_pBaseVals[204]
0x0F12,0x400F,	//TVAR_afit_pBaseVals[205]
0x0F12,0x0204,	//TVAR_afit_pBaseVals[206]
0x0F12,0x0003,	//TVAR_afit_pBaseVals[207]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[208]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[209]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[210]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[211]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[212]
0x0F12,0x001E,	//TVAR_afit_pBaseVals[213]
0x0F12,0x03FF,	//TVAR_afit_pBaseVals[214]
0x0F12,0x009C,	//TVAR_afit_pBaseVals[215]
0x0F12,0x017C,	//TVAR_afit_pBaseVals[216]
0x0F12,0x03FF,	//TVAR_afit_pBaseVals[217]
0x0F12,0x000C,	//TVAR_afit_pBaseVals[218]
0x0F12,0x0010,	//TVAR_afit_pBaseVals[219]
0x0F12,0x012C,	//TVAR_afit_pBaseVals[220]
0x0F12,0x03E8,	//TVAR_afit_pBaseVals[221]
0x0F12,0x0041,	//TVAR_afit_pBaseVals[222]
0x0F12,0x005A,	//TVAR_afit_pBaseVals[223]
0x0F12,0x0070,	//TVAR_afit_pBaseVals[224]
0x0F12,0x0004,	//TVAR_afit_pBaseVals[225]
0x0F12,0x0004,	//TVAR_afit_pBaseVals[226]
0x0F12,0x01AA,	//TVAR_afit_pBaseVals[227]
0x0F12,0x001E,	//TVAR_afit_pBaseVals[228]
0x0F12,0x001E,	//TVAR_afit_pBaseVals[229]
0x0F12,0x000A,	//TVAR_afit_pBaseVals[230]
0x0F12,0x000A,	//TVAR_afit_pBaseVals[231]
0x0F12,0x0032,	//TVAR_afit_pBaseVals[232]
0x0F12,0x0028,	//TVAR_afit_pBaseVals[233]
0x0F12,0x0032,	//TVAR_afit_pBaseVals[234]
0x0F12,0x0028,	//TVAR_afit_pBaseVals[235]
0x0F12,0x0032,	//TVAR_afit_pBaseVals[236]
0x0F12,0x0028,	//TVAR_afit_pBaseVals[237]
0x0F12,0x0A0F,	//TVAR_afit_pBaseVals[238]
0x0F12,0x1701,	//TVAR_afit_pBaseVals[239]
0x0F12,0x0229,	//TVAR_afit_pBaseVals[240]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[241]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[242]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[243]
0x0F12,0x0404,	//TVAR_afit_pBaseVals[244]
0x0F12,0x00FF,	//TVAR_afit_pBaseVals[245]
0x0F12,0x043B,	//TVAR_afit_pBaseVals[246]
0x0F12,0x0505,	//TVAR_afit_pBaseVals[247]
0x0F12,0x0301,	//TVAR_afit_pBaseVals[248]
0x0F12,0x8007,	//TVAR_afit_pBaseVals[249]
0x0F12,0x051E,	//TVAR_afit_pBaseVals[250]
0x0F12,0x0A1E,	//TVAR_afit_pBaseVals[251]
0x0F12,0x0F0F,	//TVAR_afit_pBaseVals[252]
0x0F12,0x0A03,	//TVAR_afit_pBaseVals[253]
0x0F12,0x0A3C,	//TVAR_afit_pBaseVals[254]
0x0F12,0x0528,	//TVAR_afit_pBaseVals[255]
0x0F12,0x0002,	//TVAR_afit_pBaseVals[256]
0x0F12,0x00FF,	//TVAR_afit_pBaseVals[257]
0x0F12,0x1002,	//TVAR_afit_pBaseVals[258]
0x0F12,0x001D,	//TVAR_afit_pBaseVals[259]
0x0F12,0x0900,	//TVAR_afit_pBaseVals[260]
0x0F12,0x0600,	//TVAR_afit_pBaseVals[261]
0x0F12,0x0504,	//TVAR_afit_pBaseVals[262]
0x0F12,0x0305,	//TVAR_afit_pBaseVals[263]
0x0F12,0x6902,	//TVAR_afit_pBaseVals[264]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[265]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[266]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[267]
0x0F12,0x2D2D,	//TVAR_afit_pBaseVals[268]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[269]
0x0F12,0x2001,	//TVAR_afit_pBaseVals[270]
0x0F12,0x2026,	//TVAR_afit_pBaseVals[271]
0x0F12,0x281E,	//TVAR_afit_pBaseVals[272]
0x0F12,0x0A00,	//TVAR_afit_pBaseVals[273]
0x0F12,0x0A03,	//TVAR_afit_pBaseVals[274]
0x0F12,0x1E0A,	//TVAR_afit_pBaseVals[275]
0x0F12,0x070A,	//TVAR_afit_pBaseVals[276]
0x0F12,0x32FF,	//TVAR_afit_pBaseVals[277]
0x0F12,0x5004,	//TVAR_afit_pBaseVals[278]
0x0F12,0x0F40,	//TVAR_afit_pBaseVals[279]
0x0F12,0x400F,	//TVAR_afit_pBaseVals[280]
0x0F12,0x0204,	//TVAR_afit_pBaseVals[281]
0x0F12,0x3203,	//TVAR_afit_pBaseVals[282]
0x0F12,0x0132,	//TVAR_afit_pBaseVals[283]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[284]
0x0F12,0x1C20,	//TVAR_afit_pBaseVals[285]
0x0F12,0x1C1E,	//TVAR_afit_pBaseVals[286]
0x0F12,0x0028,	//TVAR_afit_pBaseVals[287]
0x0F12,0x030A,	//TVAR_afit_pBaseVals[288]
0x0F12,0x0A0A,	//TVAR_afit_pBaseVals[289]
0x0F12,0x0A1E,	//TVAR_afit_pBaseVals[290]
0x0F12,0xFF07,	//TVAR_afit_pBaseVals[291]
0x0F12,0x0432,	//TVAR_afit_pBaseVals[292]
0x0F12,0x4050,	//TVAR_afit_pBaseVals[293]
0x0F12,0x0F0F,	//TVAR_afit_pBaseVals[294]
0x0F12,0x0440,	//TVAR_afit_pBaseVals[295]
0x0F12,0x0302,	//TVAR_afit_pBaseVals[296]
0x0F12,0x3232,	//TVAR_afit_pBaseVals[297]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[298]
0x0F12,0x2001,	//TVAR_afit_pBaseVals[299]
0x0F12,0x1E1C,	//TVAR_afit_pBaseVals[300]
0x0F12,0x281C,	//TVAR_afit_pBaseVals[301]
0x0F12,0x0A00,	//TVAR_afit_pBaseVals[302]
0x0F12,0x0A03,	//TVAR_afit_pBaseVals[303]
0x0F12,0x1E0A,	//TVAR_afit_pBaseVals[304]
0x0F12,0x070A,	//TVAR_afit_pBaseVals[305]
0x0F12,0x32FF,	//TVAR_afit_pBaseVals[306]
0x0F12,0x5004,	//TVAR_afit_pBaseVals[307]
0x0F12,0x0F40,	//TVAR_afit_pBaseVals[308]
0x0F12,0x400F,	//TVAR_afit_pBaseVals[309]
0x0F12,0x0204,	//TVAR_afit_pBaseVals[310]
0x0F12,0x0003,	//TVAR_afit_pBaseVals[311]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[312]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[313]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[314]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[315]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[316]
0x0F12,0x001E,	//TVAR_afit_pBaseVals[317]
0x0F12,0x03FF,	//TVAR_afit_pBaseVals[318]
0x0F12,0x009C,	//TVAR_afit_pBaseVals[319]
0x0F12,0x017C,	//TVAR_afit_pBaseVals[320]
0x0F12,0x03FF,	//TVAR_afit_pBaseVals[321]
0x0F12,0x000C,	//TVAR_afit_pBaseVals[322]
0x0F12,0x0010,	//TVAR_afit_pBaseVals[323]
0x0F12,0x012C,	//TVAR_afit_pBaseVals[324]
0x0F12,0x03E8,	//TVAR_afit_pBaseVals[325]
0x0F12,0x0050,	//TVAR_afit_pBaseVals[326]
0x0F12,0x00C8,	//TVAR_afit_pBaseVals[327]
0x0F12,0x0070,	//TVAR_afit_pBaseVals[328]
0x0F12,0x0004,	//TVAR_afit_pBaseVals[329]
0x0F12,0x0004,	//TVAR_afit_pBaseVals[330]
0x0F12,0x01AA,	//TVAR_afit_pBaseVals[331]
0x0F12,0x0014,	//TVAR_afit_pBaseVals[332]
0x0F12,0x0014,	//TVAR_afit_pBaseVals[333]
0x0F12,0x000A,	//TVAR_afit_pBaseVals[334]
0x0F12,0x000A,	//TVAR_afit_pBaseVals[335]
0x0F12,0x0032,	//TVAR_afit_pBaseVals[336]
0x0F12,0x0023,	//TVAR_afit_pBaseVals[337]
0x0F12,0x0032,	//TVAR_afit_pBaseVals[338]
0x0F12,0x0028,	//TVAR_afit_pBaseVals[339]
0x0F12,0x0032,	//TVAR_afit_pBaseVals[340]
0x0F12,0x0028,	//TVAR_afit_pBaseVals[341]
0x0F12,0x0A0F,	//TVAR_afit_pBaseVals[342]
0x0F12,0x1701,	//TVAR_afit_pBaseVals[343]
0x0F12,0x0229,	//TVAR_afit_pBaseVals[344]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[345]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[346]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[347]
0x0F12,0x0404,	//TVAR_afit_pBaseVals[348]
0x0F12,0x00FF,	//TVAR_afit_pBaseVals[349]
0x0F12,0x033B,	//TVAR_afit_pBaseVals[350]
0x0F12,0x0505,	//TVAR_afit_pBaseVals[351]
0x0F12,0x0301,	//TVAR_afit_pBaseVals[352]
0x0F12,0x8007,	//TVAR_afit_pBaseVals[353]
0x0F12,0x051E,	//TVAR_afit_pBaseVals[354]
0x0F12,0x0A1E,	//TVAR_afit_pBaseVals[355]
0x0F12,0x0F0F,	//TVAR_afit_pBaseVals[356]
0x0F12,0x0A03,	//TVAR_afit_pBaseVals[357]
0x0F12,0x0A3C,	//TVAR_afit_pBaseVals[358]
0x0F12,0x0828,	//TVAR_afit_pBaseVals[359]
0x0F12,0x0002,	//TVAR_afit_pBaseVals[360]
0x0F12,0x00FF,	//TVAR_afit_pBaseVals[361]
0x0F12,0x1002,	//TVAR_afit_pBaseVals[362]
0x0F12,0x001D,	//TVAR_afit_pBaseVals[363]
0x0F12,0x0900,	//TVAR_afit_pBaseVals[364]
0x0F12,0x0600,	//TVAR_afit_pBaseVals[365]
0x0F12,0x0504,	//TVAR_afit_pBaseVals[366]
0x0F12,0x0305,	//TVAR_afit_pBaseVals[367]
0x0F12,0x6F02,	//TVAR_afit_pBaseVals[368]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[369]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[370]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[371]
0x0F12,0x323C,	//TVAR_afit_pBaseVals[372]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[373]
0x0F12,0x1A01,	//TVAR_afit_pBaseVals[374]
0x0F12,0x1A1E,	//TVAR_afit_pBaseVals[375]
0x0F12,0x2818,	//TVAR_afit_pBaseVals[376]
0x0F12,0x0A00,	//TVAR_afit_pBaseVals[377]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[378]
0x0F12,0x1905,	//TVAR_afit_pBaseVals[379]
0x0F12,0x060E,	//TVAR_afit_pBaseVals[380]
0x0F12,0x32FF,	//TVAR_afit_pBaseVals[381]
0x0F12,0x5004,	//TVAR_afit_pBaseVals[382]
0x0F12,0x1440,	//TVAR_afit_pBaseVals[383]
0x0F12,0x4015,	//TVAR_afit_pBaseVals[384]
0x0F12,0x0204,	//TVAR_afit_pBaseVals[385]
0x0F12,0x3C03,	//TVAR_afit_pBaseVals[386]
0x0F12,0x013C,	//TVAR_afit_pBaseVals[387]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[388]
0x0F12,0x141A,	//TVAR_afit_pBaseVals[389]
0x0F12,0x181A,	//TVAR_afit_pBaseVals[390]
0x0F12,0x0028,	//TVAR_afit_pBaseVals[391]
0x0F12,0x030A,	//TVAR_afit_pBaseVals[392]
0x0F12,0x0614,	//TVAR_afit_pBaseVals[393]
0x0F12,0x0A19,	//TVAR_afit_pBaseVals[394]
0x0F12,0xFF06,	//TVAR_afit_pBaseVals[395]
0x0F12,0x0432,	//TVAR_afit_pBaseVals[396]
0x0F12,0x4050,	//TVAR_afit_pBaseVals[397]
0x0F12,0x1514,	//TVAR_afit_pBaseVals[398]
0x0F12,0x0440,	//TVAR_afit_pBaseVals[399]
0x0F12,0x0302,	//TVAR_afit_pBaseVals[400]
0x0F12,0x3C3C,	//TVAR_afit_pBaseVals[401]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[402]
0x0F12,0x1A01,	//TVAR_afit_pBaseVals[403]
0x0F12,0x1A14,	//TVAR_afit_pBaseVals[404]
0x0F12,0x2818,	//TVAR_afit_pBaseVals[405]
0x0F12,0x0A00,	//TVAR_afit_pBaseVals[406]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[407]
0x0F12,0x1906,	//TVAR_afit_pBaseVals[408]
0x0F12,0x060A,	//TVAR_afit_pBaseVals[409]
0x0F12,0x32FF,	//TVAR_afit_pBaseVals[410]
0x0F12,0x5004,	//TVAR_afit_pBaseVals[411]
0x0F12,0x1440,	//TVAR_afit_pBaseVals[412]
0x0F12,0x4015,	//TVAR_afit_pBaseVals[413]
0x0F12,0x0204,	//TVAR_afit_pBaseVals[414]
0x0F12,0x0003,	//TVAR_afit_pBaseVals[415]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[416]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[417]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[418]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[419]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[420]
0x0F12,0x001E,	//TVAR_afit_pBaseVals[421]
0x0F12,0x03FF,	//TVAR_afit_pBaseVals[422]
0x0F12,0x009C,	//TVAR_afit_pBaseVals[423]
0x0F12,0x017C,	//TVAR_afit_pBaseVals[424]
0x0F12,0x03FF,	//TVAR_afit_pBaseVals[425]
0x0F12,0x000C,	//TVAR_afit_pBaseVals[426]
0x0F12,0x0010,	//TVAR_afit_pBaseVals[427]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[428]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[429]
0x0F12,0x0046,	//TVAR_afit_pBaseVals[430]
0x0F12,0x0050,	//TVAR_afit_pBaseVals[431]
0x0F12,0x0070,	//TVAR_afit_pBaseVals[432]
0x0F12,0x0004,	//TVAR_afit_pBaseVals[433]
0x0F12,0x0004,	//TVAR_afit_pBaseVals[434]
0x0F12,0x01AA,	//TVAR_afit_pBaseVals[435]
0x0F12,0x0014,	//TVAR_afit_pBaseVals[436]
0x0F12,0x0014,	//TVAR_afit_pBaseVals[437]
0x0F12,0x000A,	//TVAR_afit_pBaseVals[438]
0x0F12,0x000A,	//TVAR_afit_pBaseVals[439]
0x0F12,0x002D,	//TVAR_afit_pBaseVals[440]
0x0F12,0x0019,	//TVAR_afit_pBaseVals[441]
0x0F12,0x0023,	//TVAR_afit_pBaseVals[442]
0x0F12,0x0023,	//TVAR_afit_pBaseVals[443]
0x0F12,0x0023,	//TVAR_afit_pBaseVals[444]
0x0F12,0x0023,	//TVAR_afit_pBaseVals[445]
0x0F12,0x0A0F,	//TVAR_afit_pBaseVals[446]
0x0F12,0x1701,	//TVAR_afit_pBaseVals[447]
0x0F12,0x0229,	//TVAR_afit_pBaseVals[448]
0x0F12,0x1403,	//TVAR_afit_pBaseVals[449]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[450]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[451]
0x0F12,0x0606,	//TVAR_afit_pBaseVals[452]
0x0F12,0x00FF,	//TVAR_afit_pBaseVals[453]
0x0F12,0x033B,	//TVAR_afit_pBaseVals[454]
0x0F12,0x0505,	//TVAR_afit_pBaseVals[455]
0x0F12,0x0301,	//TVAR_afit_pBaseVals[456]
0x0F12,0x8007,	//TVAR_afit_pBaseVals[457]
0x0F12,0x051E,	//TVAR_afit_pBaseVals[458]
0x0F12,0x0A1E,	//TVAR_afit_pBaseVals[459]
0x0F12,0x0000,	//TVAR_afit_pBaseVals[460]
0x0F12,0x0A03,	//TVAR_afit_pBaseVals[461]
0x0F12,0x1E3C,	//TVAR_afit_pBaseVals[462]
0x0F12,0x1028,	//TVAR_afit_pBaseVals[463]
0x0F12,0x0002,	//TVAR_afit_pBaseVals[464]
0x0F12,0x00FF,	//TVAR_afit_pBaseVals[465]
0x0F12,0x1002,	//TVAR_afit_pBaseVals[466]
0x0F12,0x001E,	//TVAR_afit_pBaseVals[467]
0x0F12,0x0900,	//TVAR_afit_pBaseVals[468]
0x0F12,0x0600,	//TVAR_afit_pBaseVals[469]
0x0F12,0x0504,	//TVAR_afit_pBaseVals[470]
0x0F12,0x0305,	//TVAR_afit_pBaseVals[471]
0x0F12,0x8002,	//TVAR_afit_pBaseVals[472]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[473]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[474]
0x0F12,0x0080,	//TVAR_afit_pBaseVals[475]
0x0F12,0x4646,	//TVAR_afit_pBaseVals[476]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[477]
0x0F12,0x1801,	//TVAR_afit_pBaseVals[478]
0x0F12,0x141C,	//TVAR_afit_pBaseVals[479]
0x0F12,0x2812,	//TVAR_afit_pBaseVals[480]
0x0F12,0x0A00,	//TVAR_afit_pBaseVals[481]
0x0F12,0x1003,	//TVAR_afit_pBaseVals[482]
0x0F12,0x1405,	//TVAR_afit_pBaseVals[483]
0x0F12,0x050C,	//TVAR_afit_pBaseVals[484]
0x0F12,0x32FF,	//TVAR_afit_pBaseVals[485]
0x0F12,0x5204,	//TVAR_afit_pBaseVals[486]
0x0F12,0x1440,	//TVAR_afit_pBaseVals[487]
0x0F12,0x4015,	//TVAR_afit_pBaseVals[488]
0x0F12,0x0204,	//TVAR_afit_pBaseVals[489]
0x0F12,0x5003,	//TVAR_afit_pBaseVals[490]
0x0F12,0x0150,	//TVAR_afit_pBaseVals[491]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[492]
0x0F12,0x1418,	//TVAR_afit_pBaseVals[493]
0x0F12,0x1214,	//TVAR_afit_pBaseVals[494]
0x0F12,0x0028,	//TVAR_afit_pBaseVals[495]
0x0F12,0x030A,	//TVAR_afit_pBaseVals[496]
0x0F12,0x0A10,	//TVAR_afit_pBaseVals[497]
0x0F12,0x0819,	//TVAR_afit_pBaseVals[498]
0x0F12,0xFF05,	//TVAR_afit_pBaseVals[499]
0x0F12,0x0432,	//TVAR_afit_pBaseVals[500]
0x0F12,0x4052,	//TVAR_afit_pBaseVals[501]
0x0F12,0x1514,	//TVAR_afit_pBaseVals[502]
0x0F12,0x0440,	//TVAR_afit_pBaseVals[503]
0x0F12,0x0302,	//TVAR_afit_pBaseVals[504]
0x0F12,0x5050,	//TVAR_afit_pBaseVals[505]
0x0F12,0x0101,	//TVAR_afit_pBaseVals[506]
0x0F12,0x1801,	//TVAR_afit_pBaseVals[507]
0x0F12,0x1414,	//TVAR_afit_pBaseVals[508]
0x0F12,0x2812,	//TVAR_afit_pBaseVals[509]
0x0F12,0x0A00,	//TVAR_afit_pBaseVals[510]
0x0F12,0x1003,	//TVAR_afit_pBaseVals[511]
0x0F12,0x190A,	//TVAR_afit_pBaseVals[512]
0x0F12,0x0508,	//TVAR_afit_pBaseVals[513]
0x0F12,0x32FF,	//TVAR_afit_pBaseVals[514]
0x0F12,0x5204,	//TVAR_afit_pBaseVals[515]
0x0F12,0x1440,	//TVAR_afit_pBaseVals[516]
0x0F12,0x4015,	//TVAR_afit_pBaseVals[517]
0x0F12,0x0204,	//TVAR_afit_pBaseVals[518]
0x0F12,0x0003,	//TVAR_afit_pBaseVals[519]
//	pa,0xram_end	TVAR_afit_pBaseVals
//	pa,0xram_start	afit_pConstBaseVals 
0x002A,0x0BD4,
0x0F12,0x7F7A,	//afit_pConstBaseVals[0]
0x0F12,0x779D,	//afit_pConstBaseVals[1]
0x0F12,0xBE7E,	//afit_pConstBaseVals[2]
0x0F12,0xF7BC,	//afit_pConstBaseVals[3]
0x0F12,0x7E06,	//afit_pConstBaseVals[4]
0x0F12,0x0053,	//afit_pConstBaseVals[5]
//	pa,0xram_end	afit_pConstBaseVals
      
// Fil,0xl RAM with alternative op-codes
0x0028,0x7000,	// start add MSW
0x002A,0x2CE8,	// start add LSW
0x0F12,0x0007,	// Modify LSB to control AWBB_YThreshLow
0x0F12,0x00e2,	//
0x0F12,0x0005,	// Modify LSB to control AWBB_YThreshLowBrLow
0x0F12,0x00E2,	//
//////,0x////////////////////////////////////////////////////////////////////
//====,0x========================================================         
//END ,0xTuning part                                                          
//====,0x======================================================== 
//====,0x========================================================
// Ini,0xt Parameters
//====,0x========================================================
//clk ,0xSettings
0x002A,0x01CC,
0x0F12,0x5DC0, //#REG_TC_IPRM_InClockLSBs			//input clock=24MHz 6590 26MHz
0x0F12,0x0000, //	#REG_TC_IPRM_InClockMSBs		
0x002A,0x01EE,
0x0F12,0x0000, //#REG_TC_IPRM_UseNPviClocks		//1 PLL configurations
0x0F12,0x0001, //REG_TC_IPRM_UseNMipiClocks					0x700001F0
0x0F12,0x0001, //REG_TC_IPRM_NumberOfMipiLanes				0x700001F2
      
0x002A,0x01F6,
0x0F12,0x36B0, //EE0 //1F40 //#REG_TC_IPRM_OpClk4KHz_0		//1st system CLK 1F40=32MHz 36B0=56M  2EE0=48MHz
0x0F12,0x5208, //A88 //#REG_TC_IPRM_MinOutRate4KHz_0	//PVI clock 3A88 = 60MHz 5208 = 85M
0x0F12,0x5218, //AA8 //#REG_TC_IPRM_MaxOutRate4KHz_0		
0x002A,0x0208,
0x0F12,0x0001, //#REG_TC_IPRM_InitParamsUpdated	0001
// sle,0xep(100) p100 // Wait100mSec  
      
//Auto,0x Flicker 60Hz Start                                                                          
0x0028,0x7000,                                                                                           
0x002A,0x0C18,  //#AFC_Default60Hz                                                                       
0x0F12,0x0001,  // #AFC_Default60Hz  1: Auto Flicker 60Hz start 0: Auto Flicker 50Hz start               
0x002A,0x04D2,  // #REG_TC_DBG_AutoAlgEnBits                                                             
0x0F12,0x067F,                                                                                           
      
//====,0x============================================================================================
// SET,0x PREVIEW CONFIGURATION_0
// # F,0xoramt : YUV422
// # S,0xize: 1024*768
// # F,0xPS :10-15fps(using normal_mode preview)
//====,0x============================================================================================
0x002A,0x026C,
0x0F12,0x0280, //0400//0800//#REG_0TC_PCFG_usWidth 
0x0F12,0x01E0, //0300 //0600 //#REG_0TC_PCFG_usHeight 
0x0F12,0x0005, //#REG_0TC_PCFG_Format            
0x0F12,0x5218, //AA8 //#REG_0TC_PCFG_usMaxOut4KHzRate  
0x0F12,0x5208, //A88 //#REG_0TC_PCFG_usMinOut4KHzRate  
0x002A,0x027A,
0x0F12,0x0010, //#REG_0TC_PCFG_PVIMask //s0050 = FALSE in MSM6290 : s0052 = TRUE in MSM6800 //reg 027A
0x002A,0x0282,
0x0F12,0x0000, //#REG_0TC_PCFG_uClockInd
0x0F12,0x0000, //#REG_0TC_PCFG_usFrTimeType
0x0F12,0x0001, //#REG_0TC_PCFG_FrRateQualityType
0x0F12,0x029A, //03E8  //#REG_0TC_PCFG_usMaxFrTimeMsecMult10 //10fps
0x0F12,0x014D, //029A  //#REG_0TC_PCFG_usMinFrTimeMsecMult10 //15fps
                                                                                                   
//====,0x============================================================================================                                                                                          
// SET,0x Capture CONFIGURATION_0                                                                                                                                                              
// # F,0xoramt : YUV422                                                                                                                                                                        
// # S,0xize: QXGA,                                         
// # F,0xPS :7.5fps                                                
//====,0x============================================================================================  
0x002A,0x035E,                                                    
0x0F12,0x0800, //#REG_0TC_CCFG_usWidth                            
0x0F12,0x0600, //#REG_0TC_CCFG_usHeight                           
0x0F12,0x0005, //#REG_0TC_CCFG_Format//5:YUV9:JPEG                
0x0F12,0x5218, //AA8//#REG_0TC_CCFG_usMaxOut4KHzRate       
0x0F12,0x5208, //A88//#REG_0TC_CCFG_usMinOut4KHzRate       
0x002A,0x036C,                                                    
0x0F12,0x0010, //#REG_0TC_CCFG_PVIMask                            
0x002A,0x0374,                                                    
0x0F12,0x0000,//#REG_0TC_CCFG_uClockInd                          
0x0F12,0x0000, //#REG_0TC_CCFG_usFrTimeType                       
0x0F12,0x0002, //#REG_0TC_CCFG_FrRateQualityType                  
0x0F12,0x0535,//0535//#REG_0TC_CCFG_usMaxFrTimeMsecMult10 //5fps  
0x0F12,0x0535, //#REG_0TC_CCFG_usMinFrTimeMsecMult10 //6.5fps     
//====,0x============================================================================================
// APP,0xLY PREVIEW CONFIGURATION & RUN PREVIEW 
//====,0x============================================================================================
0x002A,0x023C,                                                                                                                                                                                                     
0x0F12,0x0000,// #REG_TC_GP_ActivePrevConfig // Select preview configuration_0                                                                                                                                     
0x002A,0x0240,                                                                                                                                                                                                     
0x0F12,0x0001, // #REG_TC_GP_PrevOpenAfterChange                                                                                                                                                                   
0x002A,0x0230,                                                                                                                                                                                                     
0x0F12,0x0001, // #REG_TC_GP_NewConfigSync // Update preview configuration                                                                                                                                         
0x002A,0x023E,                                                                                                                                                                                                     
0x0F12,0x0001, // #REG_TC_GP_PrevConfigChanged                                                                                                                                                                     
0x002A,0x0220,                                                                                                                                                                                                     
0x0F12,0x0001, // #REG_TC_GP_EnablePreview // Start preview                                                                                                                                                        
0x0F12,0x0001, // #REG_TC_GP_EnablePreviewChanged                                                                                                                                                                  
//====,0x============================================================================================                                                                                                                     
0x0028,0xD000,     
0x002A,0x1000,  // Set host interrupt so main start run          
0x0F12,0x0001,     
//p10    //    p10	// Wait10mSec    

	/*
		//================================================================================================
		// SET PREVIEW CONFIGURATION_1
		// # Foramt : YUV422
		// # Size: 
		// # FPS : 
		//================================================================================================
		0x002A, 0x029C,
		0x0F12, 0x800, //#REG_0TC_PCFG_usWidth//640 
		0x0F12, 0x600, //#REG_0TC_PCFG_usHeight//480
		0x0F12, 0x0005, //#REG_0TC_PCFG_Format
		0x0F12, 0x3AA4, //#REG_0TC_PCFG_usMaxOut4KHzRate
		0x0F12, 0x3A8B, //#REG_0TC_PCFG_usMinOut4KHzRate
		0x0F12, 0x0100, //#REG_0TC_PCFG_OutClkPerPix88
		0x0F12, 0x0800, //#REG_0TC_PCFG_uMaxBpp88
		0x0F12, 0x0042, //#REG_0TC_PCFG_PVIMask //s0050 = FALSE in MSM6290 : s0052 = TRUE in MSM6800 //reg
		0x0F12, 0x4000, //#REG_0TC_PCFG_OIFMask
		0x0F12, 0x01E0, //#REG_0TC_PCFG_usJpegPacketSize
		0x0F12, 0x0000, //#REG_0TC_PCFG_usJpegTotalPackets
		0x0F12, 0x0000, //#REG_0TC_PCFG_uClockInd
		0x0F12, 0x0000, //#REG_0TC_PCFG_usFrTimeType
		0x0F12, 0x0001, //#REG_0TC_PCFG_FrRateQualityType
		0x0F12, 0x03E8*10, //#REG_0TC_PCFG_usMaxFrTimeMsecMult10 //10fps
		0x0F12, 0x01A0, //#REG_0TC_PCFG_usMinFrTimeMsecMult10 //24fps
		0x0F12, 0x0000, //#REG_0TC_PCFG_bSmearOutput
		0x0F12, 0x0000, //#REG_0TC_PCFG_sSaturation
		0x0F12, 0x0000, //#REG_0TC_PCFG_sSharpBlur
		0x0F12, 0x0000, //#REG_0TC_PCFG_sColorTemp
		0x0F12, 0x0000, //#REG_0TC_PCFG_uDeviceGammaIndex
#ifdef CONFIG_BOARD_ODROID_A4
		0x0F12, 0x0003, //#REG_0TC_PCFG_uPrevMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#else	
		0x0F12, 0x0000, //#REG_0TC_PCFG_uPrevMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uCaptureMirror
		0x0F12, 0x0000, //#REG_0TC_PCFG_uRotation
#endif

	//================================================================================================
	// APPLY PREVIEW CONFIGURATION & RUN PREVIEW
	//================================================================================================
	0x002A, 0x023C,
	0x0F12, 0x0000, //#REG_TC_GP_ActivePrevConfig // Select preview configuration_0
	0x002A, 0x0240,
	0x0F12, 0x0001, //#REG_TC_GP_PrevOpenAfterChange
	0x002A, 0x0230,
	0x0F12, 0x0001, //#REG_TC_GP_NewConfigSync // Update preview configuration
	0x002A, 0x023E,
	0x0F12, 0x0001, //#REG_TC_GP_PrevConfigChanged
	0x002A, 0x0220,
	0x0F12, 0x0001, //#REG_TC_GP_EnablePreview // Start preview
	0x0F12, 0x0001, //#REG_TC_GP_EnablePreviewChanged
	
	//================================================================================================
	// SET CAPTURE CONFIGURATION_0
	// # Foramt :JPEG
	// # Size: QXGA
	// # FPS : 7.5fps
	//================================================================================================
	0x002A, 0x035C,
	0x0F12, 0x0000, //#REG_0TC_CCFG_uCaptureModeJpEG
	0x0F12, 2048, //#REG_0TC_CCFG_usWidth 
	0x0F12, 1536, //#REG_0TC_CCFG_usHeight
	0x0F12, 0x0009, //#REG_0TC_CCFG_Format//5:YUV 9:JPEG 
	0x0F12, 0x3FFF,//AA4, //#REG_0TC_CCFG_usMaxOut4KHzRate
	0x0F12, 0x3F04,//A8B, //#REG_0TC_CCFG_usMinOut4KHzRate
	0x0F12, 0x0100, //#REG_0TC_CCFG_OutClkPerPix88
	0x0F12, 0x0800, //#REG_0TC_CCFG_uMaxBpp88 
	0x0F12, 0x0042, //#REG_0TC_CCFG_PVIMask //s0050 = FALSE in MSM6290 : s0052 = TRUE in MSM6800
	0x0F12, 0x0050, //#REG_0TC_CCFG_OIFMask 
	0x0F12, 0x01E0, //#REG_0TC_CCFG_usJpegPacketSize
	0x0F12, 0x0000, //#REG_0TC_CCFG_usJpegTotalPackets
	0x0F12, 0x0000, //#REG_0TC_CCFG_uClockInd 
	0x0F12, 0x0000, //#REG_0TC_CCFG_usFrTimeType
	0x0F12, 0x0002, //#REG_0TC_CCFG_FrRateQualityType 
	0x0F12, 0x0535, //#REG_0TC_CCFG_usMaxFrTimeMsecMult10 //7.5fps
	0x0F12, 0x0535, //#REG_0TC_CCFG_usMinFrTimeMsecMult10 //7.5fps 
	0x0F12, 0x0000, //#REG_0TC_CCFG_bSmearOutput
	0x0F12, 0x0000, //#REG_0TC_CCFG_sSaturation 
	0x0F12, 0x0000, //#REG_0TC_CCFG_sSharpBlur
	0x0F12, 0x0000, //#REG_0TC_CCFG_sColorTemp
	0x0F12, 0x0000, //#REG_0TC_CCFG_uDeviceGammaIndex
	0x002A, 0x021A,
	0x0F12, 0x0000, //disable //#REG_TC_bUseMBR 
	
	// Fill RAM with alternative op-codes
	0x0028, 0x7000, //start add MSW
	0x002A, 0x2CE8, //start add LSW
	0x0F12, 0x0007, //Modify LSB to control AWBB_YThreshLow
	0x0F12, 0x00e2,
	0x0F12, 0x0005, //Modify LSB to control AWBB_YThreshLowBrLow
	0x0F12, 0x00e2,
	
	0x0028, 0xD000,
	0x002A, 0x1000,
	0x0F12, 0x0001,
	
	0x0028, 0x7000,

// Set IO driving current
0x0028, 0xD000, //sensor default
0x002A, 0x1082,
0x0F12, 0x0155, // [9:8] D4, [7:6] D3, [5:4] D2, [3:2] D1, [1:0] D0
0x0F12, 0x0155, // [9:8] D9, [7:6] D8, [5:4] D7, [3:2] D6, [1:0] D5
0x0F12, 0x1555, // [5:4] GPIO3, [3:2] GPIO2, [1:0] GPIO1
0x0F12, 0x0555, // [11:10] SDA, [9:8] SCA, [7:6] PCLK, [3:2] VSYNC, [1:0] HSYNC

	0xffff, 200, // delay

	0x0028, 0x7000, //sensor default
	0x002A, 0x0252,
	0x0F12, 0x0006, //#init  //AF
*/
	};

// capture preset
static unsigned short  s5k5cagx_capture_preset_0[] = { 
	//This code is for 3M Full YUV Capture Mode.
	//If you want initial(Preview)mode, comment below code.
	
	0x0028 ,0x7000,
	0x002a ,0x0244,
	0x0f12 ,0x0000, // #REG_TC_GP_ActiveCapConfig//capture config
	0x0f12 ,0x0001, // #REG_TC_GP_CapConfigChanged 
	0x002a ,0x0230,
	0x0f12 ,0x0001, // #REG_TC_GP_NewConfigSync//config ��?�Ʃ�Au��e 
	0x002a ,0x0224,
	0x0f12 ,0x0001, // #REG_TC_GP_EnableCapture 
	0x0f12 ,0x0001, // #REG_TC_GP_EnableCaptureChanged

};

static unsigned short s5k5cagx_preview_preset_0[]={	
	// Select and apply preview configuration

	0x0028 ,0x7000,
	0x002A, 0x026C,
	0x0F12, 640, //#REG_0TC_PCFG_usWidth
	0x0F12, 480, //#REG_0TC_PCFG_usHeight
	
	0x002A ,0x168E,
	0x0f12 ,0x0800,
	0x002A ,0x023C,		//#REG_TC_GP_ActivePrevConfig
	0x0F12 ,0x0000,
	0x002A ,0x0240,		//#REG_TC_GP_PrevOpenAfterChange
	0x0F12 ,0x0001,
	0x002A ,0x0230,		//#REG_TC_GP_NewConfigSync
	0x0F12 ,0x0001,
	0x002A ,0x023E,		//#REG_TC_GP_PrevConfigChanged
	0x0F12 ,0x0001,

	0x002A, 0x0252,
	0x0F12, 0x0006, //  //AF

};
static unsigned short s5k5cagx_preview_preset_1000x562[]={	
	// Select and apply preview configuration

	0x0028 ,0x7000,
	0x002A, 0x026C,
	0x0F12, 1000, //#REG_0TC_PCFG_usWidth
	0x0F12, 562, //#REG_0TC_PCFG_usHeight
	
	0x002A ,0x168E,
	0x0f12 ,0x0800,
	0x002A ,0x023C,		//#REG_TC_GP_ActivePrevConfig
	0x0F12 ,0x0000,
	0x002A ,0x0240,		//#REG_TC_GP_PrevOpenAfterChange
	0x0F12 ,0x0001,
	0x002A ,0x0230,		//#REG_TC_GP_NewConfigSync
	0x0F12 ,0x0001,
	0x002A ,0x023E,		//#REG_TC_GP_PrevConfigChanged
	0x0F12 ,0x0001,

	0x002A, 0x0252,
	0x0F12, 0x0006, //  //AF

};

static unsigned short s5k5cagx_movie_preset_0[]={	
	// Select and apply preview configuration

	0x0028 ,0x7000,
	0x002A, 0x026C,
	0x0F12, 1000, //#REG_0TC_PCFG_usWidth
	0x0F12, 600, //#REG_0TC_PCFG_usHeight
	
	0x002A ,0x168E,
	0x0f12 ,0x0800,
	0x002A ,0x023C,		//#REG_TC_GP_ActivePrevConfig
	0x0F12 ,0x0000,
	0x002A ,0x0240,		//#REG_TC_GP_PrevOpenAfterChange
	0x0F12 ,0x0001,
	0x002A ,0x0230,		//#REG_TC_GP_NewConfigSync
	0x0F12 ,0x0001,
	0x002A ,0x023E,		//#REG_TC_GP_PrevConfigChanged
	0x0F12 ,0x0001,

	0x002A, 0x0252,
	0x0F12, 0x0005, //  //MF
};

static unsigned short s5k5cagx_preview_preset_800x480[]={	
	// Select and apply preview configuration

	0x0028 ,0x7000,

	0x002A, 0x026C,
	0x0F12, 800, //#REG_0TC_PCFG_usWidth
	0x0F12, 480, //#REG_0TC_PCFG_usHeight
	
	0x002A ,0x168E,
	0x0f12 ,0x0800,
	0x002A ,0x023C,		//#REG_TC_GP_ActivePrevConfig
	0x0F12 ,0x0000,
	0x002A ,0x0240,		//#REG_TC_GP_PrevOpenAfterChange
	0x0F12 ,0x0001,
	0x002A ,0x0230,		//#REG_TC_GP_NewConfigSync
	0x0F12 ,0x0001,
	0x002A ,0x023E,		//#REG_TC_GP_PrevConfigChanged
	0x0F12 ,0x0001,

	0x002A, 0x0252,
	0x0F12, 0x0006, //  //AF
};


static unsigned short s5k5cagx_capture_jpeg_2048x1536[]={	
	//This code is for 3M Full YUV Capture Mode.
	//If you want initial(Preview)mode, comment below code.
	0x0028 ,0x7000,
	0x002A ,0x0388,
	0x0F12 ,0x0000, //#REG_0TC_CCFG_uCaptureModeJpEG
	0x032A ,0x038E,
	0x0F12 ,0x0009, //#REG_0TC_CCFG_Format//5:YUV 9:JPEG 

	0x002a ,0x0244,
	0x0f12 ,0x0001, // #REG_TC_GP_ActiveCapConfig//capture config
	0x0f12 ,0x0001, // #REG_TC_GP_CapConfigChanged 
	0x002a ,0x0230,
	0x0f12 ,0x0001, // #REG_TC_GP_NewConfigSync//config ��?�Ʃ�Au��e 
	0x002a ,0x0224,
	0x0f12 ,0x0001, // #REG_TC_GP_EnableCapture 
	0x0f12 ,0x0001, // #REG_TC_GP_EnableCaptureChanged
};
static unsigned short s5k5cagx_capture_jpeg_2048x1536_off[]={	
	//This code is for 3M Full YUV Capture Mode.
	//If you want initial(Preview)mode, comment below code.
	0x0028 ,0x7000,
	0x002A ,0x0388,
	0x0F12 ,0x0000, //#REG_0TC_CCFG_uCaptureModeJpEG
	0x032A ,0x038E,
	0x0F12 ,0x0009, //#REG_0TC_CCFG_Format//5:YUV 9:JPEG 

	0x002a ,0x0244,
	0x0f12 ,0x0000, // #REG_TC_GP_ActiveCapConfig//capture config
	0x0f12 ,0x0001, // #REG_TC_GP_CapConfigChanged 
	0x002a ,0x0230,
	0x0f12 ,0x0001, // #REG_TC_GP_NewConfigSync//config ��?�Ʃ�Au��e 
	0x002a ,0x0224,
	0x0f12 ,0x0000, // #REG_TC_GP_EnableCapture 
	0x0f12 ,0x0001, // #REG_TC_GP_EnableCaptureChanged
};

static unsigned short s5k5cagx_preview_on[]={
	// Select and apply preview configuration

	0x0028, 0x7000,
	0x002A, 0x168E,
	0x0f12, 0x0800,
	0x002A, 0x023C,		//#REG_TC_GP_ActivePrevConfig
	0x0F12, 0x0000,
	0x002A, 0x0240,		//#REG_TC_GP_PrevOpenAfterChange
	0x0F12, 0x0001,
	0x002A, 0x0230,		//#REG_TC_GP_NewConfigSync
	0x0F12, 0x0001,
	0x002A, 0x023E,		//#REG_TC_GP_PrevConfigChanged
	0x0F12, 0x0001,
};
static unsigned short s5k5cagx_preview_off[]={
	// Select and apply preview configuration

	0x0028, 0x7000,
	0x002A, 0x168E,
	0x0f12, 0x0800,
	0x002A, 0x023C,		//#REG_TC_GP_ActivePrevConfig
	0x0F12, 0x0000,
	0x002A, 0x0240,		//#REG_TC_GP_PrevOpenAfterChange
	0x0F12, 0x0000,
	0x002A, 0x0230,		//#REG_TC_GP_NewConfigSync
	0x0F12, 0x0001,
	0x002A, 0x023E,		//#REG_TC_GP_PrevConfigChanged
	0x0F12, 0x0001,
};


#define S5K5CAGX_INIT_REGS0	\
		(sizeof(s5k5cagx_init_reg_short0) / sizeof(s5k5cagx_init_reg_short0[0]))

#define S5K5CAGX_INIT_REGS1	\
		(sizeof(s5k5cagx_init_reg_short1) / sizeof(s5k5cagx_init_reg_short1[0]))

#define S5K5CAGX_INIT_REGS2	\
		(sizeof(s5k5cagx_init_reg_short2) / sizeof(s5k5cagx_init_reg_short2[0]))

#define S5K5CAGX_INIT_REGS3	\
		(sizeof(s5k5cagx_init_reg_short3) / sizeof(s5k5cagx_init_reg_short3[0]))

#define S5K5CAGX_INIT_REGS4	\
		(sizeof(s5k5cagx_init_reg_short4) / sizeof(s5k5cagx_init_reg_short4[0]))

#define S5K5CAGX_INIT_REGS4_2	\
		(sizeof(s5k5cagx_init_reg_short4_2) / sizeof(s5k5cagx_init_reg_short4_2[0]))


#define S5K5CAGX_INIT_REGS5	\
		(sizeof(s5k5cagx_init_reg_short5) / sizeof(s5k5cagx_init_reg_short5[0]))

#define S5K5CAGX_INIT_REGS6	\
		(sizeof(s5k5cagx_init_reg_short6) / sizeof(s5k5cagx_init_reg_short6[0]))

#define S5K5CAGX_INIT_REGS7	\
		(sizeof(s5k5cagx_init_reg_short7) / sizeof(s5k5cagx_init_reg_short7[0]))

#define S5K5CAGX_INIT_REGS8	\
		(sizeof(s5k5cagx_init_reg_short8) / sizeof(s5k5cagx_init_reg_short8[0]))

#define S5K5CAGX_INIT_REGS8_1	\
		(sizeof(s5k5cagx_init_reg_short8_1_2048x1536) / sizeof(s5k5cagx_init_reg_short8_1_2048x1536[0]))

#define S5K5CAGX_INIT_REGS8_640x480	\
			(sizeof(s5k5cagx_init_reg_short8_640x480) / sizeof(s5k5cagx_init_reg_short8_640x480[0]))

#define S5K5CAGX_INIT_REGS8_1280x720	\
			(sizeof(s5k5cagx_init_reg_short8_1280x720) / sizeof(s5k5cagx_init_reg_short8_1280x720[0]))

#define S5K5CAGX_INIT_REGS8_1000x562	\
			(sizeof(s5k5cagx_preview_preset_1000x562) / sizeof(s5k5cagx_preview_preset_1000x562[0]))

#define S5K5CAGX_INIT_REGS9	\
			(sizeof(s5k5cagx_init_reg_short9) / sizeof(s5k5cagx_init_reg_short9[0]))

	
	

#define S5K5CAGX_CAPTURE_PRESET_0	\
		(sizeof(s5k5cagx_capture_preset_0) / sizeof(s5k5cagx_capture_preset_0[0]))

#define S5K5CAGX_PREVIEW_PRESET_0	\
		(sizeof(s5k5cagx_preview_preset_0) / sizeof(s5k5cagx_preview_preset_0[0]))
		
#define S5K5CAGX_PREVIEW_PRESET_800x480	\
				(sizeof(s5k5cagx_preview_preset_800x480) / sizeof(s5k5cagx_preview_preset_800x480[0]))

#define S5K5CAGX_MOVIE_PRESET_0	\
			(sizeof(s5k5cagx_movie_preset_0) / sizeof(s5k5cagx_movie_preset_0[0]))

#define S5K5CAGX_CAPTURE_JPEG_2048x1536	\
		(sizeof(s5k5cagx_capture_jpeg_2048x1536) / sizeof(s5k5cagx_capture_jpeg_2048x1536[0]))

#define S5K5CAGX_PREVIEW_ON	\
		(sizeof(s5k5cagx_preview_on) / sizeof(s5k5cagx_preview_on[0]))

#define S5K5CAGX_PREVIEW_OFF	\
		(sizeof(s5k5cagx_preview_off) / sizeof(s5k5cagx_preview_off[0]))


/*
 * EV bias
 */

static const struct s5k5cagx_reg s5k5cagx_ev_m6[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_m5[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_m4[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_m3[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_m2[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_m1[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_default[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_p1[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_p2[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_p3[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_p4[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_p5[] = {
};

static const struct s5k5cagx_reg s5k5cagx_ev_p6[] = {
};

#ifdef S5K5CAGX_COMPLETE
/* Order of this array should be following the querymenu data */
static const unsigned char *s5k5cagx_regs_ev_bias[] = {
	(unsigned char *)s5k5cagx_ev_m6, (unsigned char *)s5k5cagx_ev_m5,
	(unsigned char *)s5k5cagx_ev_m4, (unsigned char *)s5k5cagx_ev_m3,
	(unsigned char *)s5k5cagx_ev_m2, (unsigned char *)s5k5cagx_ev_m1,
	(unsigned char *)s5k5cagx_ev_default, (unsigned char *)s5k5cagx_ev_p1,
	(unsigned char *)s5k5cagx_ev_p2, (unsigned char *)s5k5cagx_ev_p3,
	(unsigned char *)s5k5cagx_ev_p4, (unsigned char *)s5k5cagx_ev_p5,
	(unsigned char *)s5k5cagx_ev_p6,
};
/* auto focus */

unsigned short s5k5cagx_focus_auto[] = {
		0x0028, 0x7000,
		0x002A, 0x0254, //REG_TC_AF_AfCmdParam
		0x0F12, 0x0000, //write lens position from 0000 to 00FF. 
				  //0000 means infinity and 00FF means macro
	//	P133	  //Delay 133ms
		0xffff, 133,
		0x002A, 0x0252, //REG_TC_AF_AfCmd
		0x0F12, 0x0004, //0004 - Manual AF
		
		//P200	  //Delay 200ms
		0xffff, 200,
		0x002A, 0x0252, //REG_TC_AF_AfCmd
		0x0F12, 0x0006, //0005 - Single AF

};

unsigned short s5k5cagx_zoom_fixed[][4] = {
		0x0028, 0x7000,
		0x002A, 0x0254, //REG_TC_AF_AfCmdParam
		0x0F12, 0x0000, //write lens position from 0000 to 00FF. 
				  //0000 means infinity and 00FF means macro
	//	P133	  //Delay 133ms
		0xffff, 133,
		0x002A, 0x0252, //REG_TC_AF_AfCmd
		0x0F12, 0x0004, //0004 - Manual AF
		
		//P200	  //Delay 200ms
		0xffff, 200,
		0x002A, 0x0252, //REG_TC_AF_AfCmd
		0x0F12, 0x0005, //0005 - Single AF
};
unsigned short s5k5cagx_zoom_infinity[][4] = {
		0x0028, 0x7000,
		0x002A, 0x0254, //REG_TC_AF_AfCmdParam
		0x0F12, 0x0000, //write lens position from 0000 to 00FF. 
				  //0000 means infinity and 00FF means macro
	//	P133	  //Delay 133ms
		0xffff, 133,
		0x002A, 0x0252, //REG_TC_AF_AfCmd
		0x0F12, 0x0004, //0004 - Manual AF
		
		//P200	  //Delay 200ms
		0xffff, 200,
		0x002A, 0x0252, //REG_TC_AF_AfCmd
		0x0F12, 0x0005, //0005 - Single AF
};
unsigned short s5k5cagx_zoom_macro[][4] = {
		0x0028, 0x7000,
		0x002A, 0x0254, //REG_TC_AF_AfCmdParam
		0x0F12, 0x0000, //write lens position from 0000 to 00FF. 
				  //0000 means infinity and 00FF means macro
	//	P133	  //Delay 133ms
		0xffff, 133,
		0x002A, 0x0252, //REG_TC_AF_AfCmd
		0x0F12, 0x0004, //0004 - Manual AF
		
		//P200	  //Delay 200ms
		0xffff, 200,
		0x002A, 0x0252, //REG_TC_AF_AfCmd
		0x0F12, 0x0005, //0005 - Single AF
};

unsigned short s5k5cagx_focus_init[][4] = {
		0x0028, 0x7000,
		0x002A, 0x0254, //REG_TC_AF_AfCmdParam
		0x0F12, 0x0000, //write lens position from 0000 to 00FF. 
				  //0000 means infinity and 00FF means macro
	//	P133	  //Delay 133ms
		0xffff, 133,
		0x002A, 0x0252, //REG_TC_AF_AfCmd
		0x0F12, 0x0004, //0004 - Manual AF
		
		//P200	  //Delay 200ms
		0xffff, 200,
		0x002A, 0x0252, //REG_TC_AF_AfCmd
		0x0F12, 0x0003, //0005 - init
};

#define S5K5CAGX_FOCUS	(sizeof(s5k5cagx_focus_auto) / sizeof(s5k5cagx_focus_auto[0]))

static const unsigned short *s5k5cagx_regs_focus[] = {
	(unsigned short *)s5k5cagx_focus_auto,
	(unsigned short *)s5k5cagx_zoom_fixed,
	(unsigned short *)s5k5cagx_zoom_infinity,
	(unsigned short *)s5k5cagx_zoom_macro,
	(unsigned short *)s5k5cagx_focus_init,
};


/*
 * Auto White Balance configure
 */
static const struct s5k5cagx_reg s5k5cagx_awb_off[] = {
};

static const struct s5k5cagx_reg s5k5cagx_awb_on[] = {
};

static const unsigned char *s5k5cagx_regs_awb_enable[] = {
	(unsigned char *)s5k5cagx_awb_off,
	(unsigned char *)s5k5cagx_awb_on,
};

/*
 * Manual White Balance (presets)
 */

static unsigned short s5k5cagx_wb_tungsten[] = {
//================================================================================================

// SET MWB_Incandescent

//================================================================================================

// Indoor boundary

0x0028 ,0x7000,
0x002A ,0x246E,
0x0F12 ,0x0000, //#Mon_AAIO_bAWB_Off
0x002A ,0x22CA,
0x0F12 ,0x0433, //#Mon_AWB_GainsOut_R
0x0F12 ,0x0400, //#Mon_AWB_GainsOut_G
0x0F12 ,0x0800, //#Mon_AWB_GainsOut_B
0x002A ,0x0C48,
0x0F12 ,0x032E, //#awbb_IndoorGrZones_m_BGrid[0]
0x0F12 ,0x035A, //#awbb_IndoorGrZones_m_BGrid[1]
0x0F12 ,0x0311, //#awbb_IndoorGrZones_m_BGrid[2]
0x0F12 ,0x0356, //#awbb_IndoorGrZones_m_BGrid[3]
0x0F12 ,0x030E, //#awbb_IndoorGrZones_m_BGrid[4]
0x0F12 ,0x033B, //#awbb_IndoorGrZones_m_BGrid[5]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[6]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[7]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[8]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[9]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[10]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[11]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[12]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[13]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[14]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[15]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[16]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[17]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[18]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[19]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[20]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[21]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[22]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[23]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[24]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[25]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[26]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[27]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[28]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[29]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[30]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[31]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[32]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[33]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[34]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[35]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[36]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[37]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[38]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[39]
0x0F12 ,0x0005, //#awbb_IndoorGrZones_m_GridStep
0x002A ,0x0CA0,
0x0F12 ,0x017F, //#awbb_IndoorGrZones_m_Boffs
0xffff ,200,
0x002A ,0x246E,
0x0F12 ,0x0001, //#Mon_AAIO_bAWB_On
};


static unsigned short s5k5cagx_wb_fluorescent[] = {
//================================================================================================

// SET MWB_Fluorescent

//================================================================================================

// Indoor boundary

0x0028 ,0x7000,
0x002A ,0x246E,
0x0F12 ,0x0000, //#Mon_AAIO_bAWB_Off
0x002A ,0x22CA,
0x0F12 ,0x047A, //#Mon_AWB_GainsOut_R
0x0F12 ,0x0400, //#Mon_AWB_GainsOut_G
0x0F12 ,0x06EB, //#Mon_AWB_GainsOut_B
0x002A ,0x0C48,
0x0F12 ,0x02F2, //#awbb_IndoorGrZones_m_BGrid[0]
0x0F12 ,0x031E, //#awbb_IndoorGrZones_m_BGrid[1]
0x0F12 ,0x02D5, //#awbb_IndoorGrZones_m_BGrid[2]
0x0F12 ,0x031A, //#awbb_IndoorGrZones_m_BGrid[3]
0x0F12 ,0x02D2, //#awbb_IndoorGrZones_m_BGrid[4]
0x0F12 ,0x02FF, //#awbb_IndoorGrZones_m_BGrid[5]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[6]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[7]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[8]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[9]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[10]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[11]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[12]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[13]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[14]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[15]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[16]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[17]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[18]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[19]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[20]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[21]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[22]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[23]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[24]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[25]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[26]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[27]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[28]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[29]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[30]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[31]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[32]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[33]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[34]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[35]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[36]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[37]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[38]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[39]
0x0F12 ,0x0005, //#awbb_IndoorGrZones_m_GridStep
0x002A ,0x0CA0,
0x0F12 ,0x01B2, //#awbb_IndoorGrZones_m_Boffs
0xffff ,200,
0x002A ,0x246E,
0x0F12 ,0x0001, //#Mon_AAIO_bAWB_On
};


static unsigned short s5k5cagx_wb_sunny[] = {
//================================================================================================

// SET MWB_DayLight

//================================================================================================

// Indoor boundary

0x0028 ,0x7000,
0x002A ,0x246E,
0x0F12 ,0x0000, //#Mon_AAIO_bAWB_Off
0x002A ,0x22CA,
0x0F12 ,0x0547, //#Mon_AWB_GainsOut_R
0x0F12 ,0x0400, //#Mon_AWB_GainsOut_G
0x0F12 ,0x0585, //#Mon_AWB_GainsOut_B
0x002A ,0x0C48,
0x0F12 ,0x026C, //#awbb_IndoorGrZones_m_BGrid[0]
0x0F12 ,0x0298, //#awbb_IndoorGrZones_m_BGrid[1]
0x0F12 ,0x024F, //#awbb_IndoorGrZones_m_BGrid[2]
0x0F12 ,0x0295, //#awbb_IndoorGrZones_m_BGrid[3]
0x0F12 ,0x024C, //#awbb_IndoorGrZones_m_BGrid[4]
0x0F12 ,0x0279, //#awbb_IndoorGrZones_m_BGrid[5]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[6]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[7]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[8]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[9]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[10]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[11]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[12]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[13]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[14]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[15]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[16]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[17]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[18]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[19]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[20]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[21]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[22]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[23]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[24]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[25]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[26]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[27]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[28]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[29]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[30]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[31]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[32]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[33]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[34]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[35]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[36]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[37]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[38]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[39]
0x0F12 ,0x0005, //#awbb_IndoorGrZones_m_GridStep
0x002A ,0x0CA0,
0x0F12 ,0x0234, //#awbb_IndoorGrZones_m_Boffs
0xffff,200,
0x002A ,0x246E,
0x0F12 ,0x0001, //#Mon_AAIO_bAWB_On
};


static unsigned short s5k5cagx_wb_cloudy[] = {
//================================================================================================

// SET MWB_Cloudy

//================================================================================================

// Indoor boundary

0x0028 ,0x7000,
0x002A ,0x246E,
0x0F12 ,0x0000, //#Mon_AAIO_bAWB_Off
0x002A ,0x22CA,
0x0F12 ,0x057A, //#Mon_AWB_GainsOut_R
0x0F12 ,0x0400, //#Mon_AWB_GainsOut_G
0x0F12 ,0x05A3, //#Mon_AWB_GainsOut_B
0x002A ,0x0C48,
0x0F12 ,0x022C, //#awbb_IndoorGrZones_m_BGrid[0]
0x0F12 ,0x0258, //#awbb_IndoorGrZones_m_BGrid[1]
0x0F12 ,0x020F, //#awbb_IndoorGrZones_m_BGrid[2]
0x0F12 ,0x0255, //#awbb_IndoorGrZones_m_BGrid[3]
0x0F12 ,0x020C, //#awbb_IndoorGrZones_m_BGrid[4]
0x0F12 ,0x0239, //#awbb_IndoorGrZones_m_BGrid[5]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[6]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[7]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[8]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[9]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[10]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[11]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[12]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[13]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[14]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[15]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[16]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[17]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[18]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[19]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[20]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[21]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[22]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[23]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[24]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[25]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[26]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[27]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[28]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[29]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[30]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[31]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[32]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[33]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[34]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[35]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[36]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[37]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[38]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[39]
0x0F12 ,0x0005, //#awbb_IndoorGrZones_m_GridStep
0x002A ,0x0CA0,
0x0F12 ,0x0273, //#awbb_IndoorGrZones_m_Boffs
0xffff,200,
0x002A ,0x246E,
0x0F12 ,0x0001, //#Mon_AAIO_bAWB_On

};

static unsigned short s5k5cagx_wb_auto[] = {

//================================================================================================

// SET MWB_Auto

//================================================================================================

// Indoor boundary

0x0028 ,0x7000,
0x002A ,0x246E,
0x0F12 ,0x0000, //#Mon_AAIO_bAWB_Off
0x002A ,0x22CA,
0x0F12 ,0x053C, //#Mon_AWB_GainsOut_R
0x0F12 ,0x0400, //#Mon_AWB_GainsOut_G
0x0F12 ,0x055C, //#Mon_AWB_GainsOut_B
0x002A ,0x0C48,
0x0F12 ,0x0384, //#awbb_IndoorGrZones_m_BGrid[0]
0x0F12 ,0x03A3, //#awbb_IndoorGrZones_m_BGrid[1]
0x0F12 ,0x034C, //#awbb_IndoorGrZones_m_BGrid[2]
0x0F12 ,0x037B, //#awbb_IndoorGrZones_m_BGrid[3]
0x0F12 ,0x0311, //#awbb_IndoorGrZones_m_BGrid[4]
0x0F12 ,0x0355, //#awbb_IndoorGrZones_m_BGrid[5]
0x0F12 ,0x02E7, //#awbb_IndoorGrZones_m_BGrid[6]
0x0F12 ,0x0334, //#awbb_IndoorGrZones_m_BGrid[7]
0x0F12 ,0x02BD, //#awbb_IndoorGrZones_m_BGrid[8]
0x0F12 ,0x0312, //#awbb_IndoorGrZones_m_BGrid[9]
0x0F12 ,0x029B, //#awbb_IndoorGrZones_m_BGrid[10]
0x0F12 ,0x02F6, //#awbb_IndoorGrZones_m_BGrid[11]
0x0F12 ,0x0278, //#awbb_IndoorGrZones_m_BGrid[12]
0x0F12 ,0x02DC, //#awbb_IndoorGrZones_m_BGrid[13]
0x0F12 ,0x025E, //#awbb_IndoorGrZones_m_BGrid[14]
0x0F12 ,0x02BB, //#awbb_IndoorGrZones_m_BGrid[15]
0x0F12 ,0x0246, //#awbb_IndoorGrZones_m_BGrid[16]
0x0F12 ,0x02AA, //#awbb_IndoorGrZones_m_BGrid[17]
0x0F12 ,0x022C, //#awbb_IndoorGrZones_m_BGrid[18]
0x0F12 ,0x029B, //#awbb_IndoorGrZones_m_BGrid[19]
0x0F12 ,0x0214, //#awbb_IndoorGrZones_m_BGrid[20]
0x0F12 ,0x028E, //#awbb_IndoorGrZones_m_BGrid[21]
0x0F12 ,0x01FF, //#awbb_IndoorGrZones_m_BGrid[22]
0x0F12 ,0x0286, //#awbb_IndoorGrZones_m_BGrid[23]
0x0F12 ,0x01E3, //#awbb_IndoorGrZones_m_BGrid[24]
0x0F12 ,0x0272, //#awbb_IndoorGrZones_m_BGrid[25]
0x0F12 ,0x01D8, //#awbb_IndoorGrZones_m_BGrid[26]
0x0F12 ,0x025C, //#awbb_IndoorGrZones_m_BGrid[27]
0x0F12 ,0x01D8, //#awbb_IndoorGrZones_m_BGrid[28]
0x0F12 ,0x0248, //#awbb_IndoorGrZones_m_BGrid[29]
0x0F12 ,0x01F2, //#awbb_IndoorGrZones_m_BGrid[30]
0x0F12 ,0x0230, //#awbb_IndoorGrZones_m_BGrid[31]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[32]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[33]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[34]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[35]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[36]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[37]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[38]
0x0F12 ,0x0000, //#awbb_IndoorGrZones_m_BGrid[39]
0x0F12 ,0x0005, //#awbb_IndoorGrZones_m_GridStep
0x002A ,0x0CA0,
0x0F12 ,0x0149, //#awbb_IndoorGrZones_m_Boffs
0xffff,200,
0x002A ,0x246E,
0x0F12 ,0x0001, //#Mon_AAIO_bAWB_On
};

/* Order of this array should be following the querymenu data */
static const unsigned short *s5k5cagx_regs_wb_preset[] = {
	(unsigned short *)s5k5cagx_wb_auto,
	(unsigned short *)s5k5cagx_wb_sunny,
	(unsigned short *)s5k5cagx_wb_cloudy,
	(unsigned short *)s5k5cagx_wb_tungsten,
	(unsigned short *)s5k5cagx_wb_fluorescent,
};
#define S5K5CAGX_WB_PRESET	(sizeof(s5k5cagx_wb_auto) / sizeof(s5k5cagx_wb_auto[0]))

/*
 * Color Effect (COLORFX)
 */
 

static unsigned short s5k5cagx_color_none[] = {
	0x0028 ,0x7000,	// set page
	0x002A ,0x021E,	// set address
	0x0F12 ,0x0000,	// No effect
};

static unsigned short s5k5cagx_color_sepia[] = {
	0x0028 ,0x7000,	// set page
	0x002A ,0x021E,	// set address
	0x0F12 ,0x0004,	// Sepia
};

static unsigned short s5k5cagx_color_aqua[] = {
	0x0028 ,0x7000,	// set page
	0x002A ,0x021E,	// set address
	0x0F12 ,0x0005,	// Aqua
};

static unsigned short s5k5cagx_color_monochrome[] = {
	0x0028 ,0x7000,	// set page
	0x002A ,0x021E,	// set address
	0x0F12 ,0x0001,	// Mono (Black & White)
};

static unsigned short s5k5cagx_color_negative[] = {
	0x0028 ,0x7000,	// set page
	0x002A ,0x021E,	// set address
	0x0F12 ,0x0003,	// Color + Negative
};

static unsigned short s5k5cagx_color_sketch[] = {
	0x0028 ,0x7000,	// set page
	0x002A ,0x021E,	// set address	
	0x0F12 ,0x0006,	// Sketch
};

/* Order of this array should be following the querymenu data */
static const unsigned short *s5k5cagx_regs_color_effect[] = {
	(unsigned short *)s5k5cagx_color_none,
	(unsigned short *)s5k5cagx_color_sepia,
	(unsigned short *)s5k5cagx_color_aqua,
	(unsigned short *)s5k5cagx_color_monochrome,
	(unsigned short *)s5k5cagx_color_negative,
	(unsigned short *)s5k5cagx_color_sketch,
};
#define S5K5CAGX_COLOR_FX	(sizeof(s5k5cagx_color_none) / sizeof(s5k5cagx_color_none[0]))


/*
 * Contrast bias
 */
static const struct s5k5cagx_reg s5k5cagx_contrast_m2[] = {
};

static const struct s5k5cagx_reg s5k5cagx_contrast_m1[] = {
};

static const struct s5k5cagx_reg s5k5cagx_contrast_default[] = {
};

static const struct s5k5cagx_reg s5k5cagx_contrast_p1[] = {
};

static const struct s5k5cagx_reg s5k5cagx_contrast_p2[] = {
};

static const unsigned char *s5k5cagx_regs_contrast_bias[] = {
	(unsigned char *)s5k5cagx_contrast_m2,
	(unsigned char *)s5k5cagx_contrast_m1,
	(unsigned char *)s5k5cagx_contrast_default,
	(unsigned char *)s5k5cagx_contrast_p1,
	(unsigned char *)s5k5cagx_contrast_p2,
};

/*
 * Saturation bias
 */
static const struct s5k5cagx_reg s5k5cagx_saturation_m2[] = {
};

static const struct s5k5cagx_reg s5k5cagx_saturation_m1[] = {
};

static const struct s5k5cagx_reg s5k5cagx_saturation_default[] = {
};

static const struct s5k5cagx_reg s5k5cagx_saturation_p1[] = {
};

static const struct s5k5cagx_reg s5k5cagx_saturation_p2[] = {
};

static const unsigned char *s5k5cagx_regs_saturation_bias[] = {
	(unsigned char *)s5k5cagx_saturation_m2,
	(unsigned char *)s5k5cagx_saturation_m1,
	(unsigned char *)s5k5cagx_saturation_default,
	(unsigned char *)s5k5cagx_saturation_p1,
	(unsigned char *)s5k5cagx_saturation_p2,
};

/*
 * Sharpness bias
 */
static const struct s5k5cagx_reg s5k5cagx_sharpness_m2[] = {
};

static const struct s5k5cagx_reg s5k5cagx_sharpness_m1[] = {
};

static const struct s5k5cagx_reg s5k5cagx_sharpness_default[] = {
};

static const struct s5k5cagx_reg s5k5cagx_sharpness_p1[] = {
};

static const struct s5k5cagx_reg s5k5cagx_sharpness_p2[] = {
};

static const unsigned char *s5k5cagx_regs_sharpness_bias[] = {
	(unsigned char *)s5k5cagx_sharpness_m2,
	(unsigned char *)s5k5cagx_sharpness_m1,
	(unsigned char *)s5k5cagx_sharpness_default,
	(unsigned char *)s5k5cagx_sharpness_p1,
	(unsigned char *)s5k5cagx_sharpness_p2,
};
#endif /* S5K5CAGX_COMPLETE */

#endif
