#include "sd_functions.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"
#include "sd_benchmark.h"
#include "stm32l4xx_hal.h"
#include "stm32l476xx.h"
#include "save.h"
uint8_t bufr[80];
UINT br;
int testnumber = 0;
void test()
{
	printf("test starting");
	FRESULT fr;

	char path[20];
	sprintf(path, "FILE%d.TXT", testnumber);
	fr = sd_write_file(path, "dwkjwdjkwkdj");
	printf("sd_write_file -> %d\r\n", fr);

	memset(bufr, 0, sizeof bufr);
	br = 0;
	fr = sd_read_file(path, bufr, sizeof bufr, &br);
	printf("sd_read_file -> %d, br=%u\r\n", fr, br);
	if (fr == FR_OK)
		printf("DATA: %s\r\n", bufr);

	sd_list_files();
	testnumber++;
	//	fr = sd_unmount();
	//	printf("sd_unmount -> %d\r\n", fr);
}
bool saveControllerDataSD(const char *path, const struct ControllerData *data)
{

	printf("writing COntrollerData:\r\n");
	// test();
	// return true;
	// if (!sd_mount())
	// 	return false;
	FIL file;
	UINT bw;
	FRESULT res = f_open(&file, path, FA_CREATE_ALWAYS | FA_WRITE);
	if (res != FR_OK)
		return res;

	res = f_write(&file, data, sizeof(struct ControllerData), &bw);
	f_sync(&file);
	f_close(&file);
	printf("Write %u bytes to %s\r\n", bw, path);
	//	if (!sd_unmount())
	//				return false;
	return (res == FR_OK && bw == sizeof(struct ControllerData)) ? true : false;
}

bool loadControllerDataSD(const char *path, struct ControllerData *out)
{
	// if (!sd_mount())
	// 	return false;

	FIL f;
	UINT rd = 0;
	FRESULT fr = f_open(&f, path, FA_READ);
	if (fr == FR_OK)
	{
		fr = f_read(&f, out, sizeof(*out), &rd);
		f_close(&f);
	}
	// sd_unmount();
	return (fr == FR_OK && rd == sizeof(*out));
	return false;
}
unsigned logId = 0;
bool appendControllerDataSD(const char *path, const struct ControllerData *data)
{
	//	 if (!sd_mount())
	//	 	return false;
	char printBuffer[500];
	sprintf(printBuffer, "=============\n\rLogID: %d\n\rD: %d\n\rT1: %d\n\rT2: %d\n\rE: %d\n\r============\n\r", logId++, (int)data->desiredTemperature, (int)data->t1, (int)data->t2, (int)(data->t1 - data->t2));
	FRESULT st = sd_append_file(path, printBuffer);
	//	 sd_unmount();
	return st == FR_OK;
}
