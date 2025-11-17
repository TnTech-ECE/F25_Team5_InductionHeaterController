#include "ff.h"
#include "user_diskio.h"
#include "stdbool.h"
#include "save.h"

// chat gpt wrote this
static FATFS s_fs;

static bool mounted = false;
bool mountSD(void)
{

	if (mounted)
		return true;
	if (f_mount(&s_fs, "", 1) == FR_OK)
	{
		mounted = true;
		return true;
	}
	return false;
}
// Binary save of a single struct

bool saveControllerDataSD(const char *path, const struct ControllerData *data)
{
	if (!mountSD())
		return false;

	FIL f;
	UINT wr;
	FRESULT fr = f_open(&f, path, FA_WRITE | FA_CREATE_ALWAYS);
	if (fr != FR_OK)
		return false;

	fr = f_write(&f, data, sizeof(*data), &wr);
	f_close(&f);
	unmountSD();
	return (fr == FR_OK && wr == sizeof(*data));
}

// Binary load of a single struct
bool loadControllerDataSD(const char *path, struct ControllerData *out)
{
	if (!mountSD())
		return false;

	FIL f;
	UINT rd;
	FRESULT fr = f_open(&f, path, FA_READ);
	if (fr != FR_OK)
		return false;

	fr = f_read(&f, out, sizeof(*out), &rd);
	f_close(&f);
	unmountSD();
	return (fr == FR_OK && rd == sizeof(*out));
}

// Append another record (for logging multiple samples)
bool appendControllerDataSD(const char *path, const struct ControllerData *data)
{
	if (!mountSD())
		return false;

	FIL f;
	UINT wr;
	FRESULT fr = f_open(&f, path, FA_OPEN_APPEND | FA_WRITE);
	if (fr != FR_OK)
		return false;

	fr = f_write(&f, data, sizeof(*data), &wr);
	f_close(&f);
	unmountSD();
	return (fr == FR_OK && wr == sizeof(*data));
}
bool unmountSD()
{
	if (!mounted)
		return true;
	if (f_mount(NULL, "", 0) == FR_OK)
	{
		mounted = false;
		return true;
	}
	return false;
}