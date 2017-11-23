#include <stdio.h>
#include <time.h>

#include "mgos.h"

#include "tft.h"
#include "touch.h"
#include "button.h"

// #include "spiffs_vfs.h"



static button_t *button1, *button2;
const char* btext1 = "Button1";
const char* btext2 = "Button2";

static uint8_t fg_r=100, fg_g=50;

void button1_cb(bool state, void *b);
void button2_cb(bool state, void *b);


void tft_demo(void)
{
	// ========  PREPARE DISPLAY INITIALIZATION  =========


	// === SET GLOBAL VARIABLES ==========================

	// ===================================================
	// ==== Set display type                         =====
//	tft_disp_type = mgos_sys_config_get_tft_disp_type();
//	tft_disp_type = DEFAULT_DISP_TYPE;
	//tft_disp_type = DISP_TYPE_ILI9341;
	//tft_disp_type = DISP_TYPE_ILI9488;
	//tft_disp_type = DISP_TYPE_ST7735B;
	// ===================================================

	// ===================================================
	// === Set display resolution if NOT using default ===
	// === DEFAULT_TFT_DISPLAY_WIDTH &                 ===
	// === DEFAULT_TFT_DISPLAY_HEIGHT                  ===
//	_width = mgos_sys_config_get_tft_width();  		// smaller dimension
//	_height = mgos_sys_config_get_tft_height();		// larger dimension
//	_width = DEFAULT_TFT_DISPLAY_WIDTH;  // smaller dimension
//	_height = DEFAULT_TFT_DISPLAY_HEIGHT; // larger dimension
	//_width = 128;  // smaller dimension
	//_height = 160; // larger dimension
	// ===================================================

	// ===================================================
	// ==== Set maximum spi clock for display read    ====
	//      operations, function 'find_rd_speed()'    ====
	//      can be used after display initialization  ====
//	max_rdclock = 8000000;
	// ===================================================

	// ====================================================================
	// === Pins MUST be initialized before SPI interface initialization ===
	// ====================================================================


	// ================================
	// ==== Initialize the Display ====


	#if USE_TOUCH == TOUCH_TYPE_STMPE610
	stmpe610_Init();
	vTaskDelay(10 / portTICK_RATE_MS);
	uint32_t tver = stmpe610_getID();
	printf("STMPE touch initialized, ver: %04x - %02x\r\n", tver >> 8, tver & 0xFF);
	#endif



	TFT_setFont(DEJAVU24_FONT, NULL);
	_fg = TFT_CYAN;

	TFT_print("Hello MOS!", 20, 16);

	TFT_setGammaCurve(DEFAULT_GAMMA_CURVE);

	TFT_jpg_image(CENTER, CENTER, 1, "mongoose-os.jpg", NULL, 0);
	TFT_jpg_image(200, 150, 2, "flower.jpg", NULL, 0);

	button1 = TFT_Button_init(32, 48, 100, 32, button1_cb);
	button1->r = 5;
	button1->label = btext1;
	button1->outlinecolor = &TFT_YELLOW;
	button1->fillcolor = &TFT_DARKGREY;
	button1->textcolor = &TFT_GREEN;
	TFT_Button_draw(button1, false);

	button2 = TFT_Button_init(170, 32, 140, 32, button2_cb);
	button2->label = btext2;
	button2->outlinecolor = &TFT_CYAN;
	button2->fillcolor = &TFT_LIGHTGREY;
	button2->textcolor = &TFT_RED;
	button2->font = COMIC24_FONT;
	TFT_Button_draw(button2, false);

	{
		char tmp_buff[64];

		double mg_now = mg_time();
		time_t now=time(0);
		struct tm* tm_info = localtime(&now);

		_fg = TFT_ORANGE;
		_bg = TFT_BLACK;
		TFT_setFont(MINYA24_FONT, NULL);

		printf("==== mg_time=%f\n", mg_now);
		sprintf(tmp_buff, "%2d.%2d  %02d:%02d:%02d", tm_info->tm_mday, 1+tm_info->tm_mon, tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
		TFT_print(tmp_buff, 32, 205);
	}

}


//
//
void button1_cb(bool state, void *b)
{
	fg_r += 30;
	printf("Button1 event %d %d\n", state, ((button_t *)b)->id);
}

void button2_cb(bool state, void *b)
{
	fg_g += 30;
	printf("Button2 event %d %d\n", state, ((button_t *)b)->id);
}

void mgos_tft_print_date( int x,  int y,  int font);


//
//
void sntp_cb(void *arg, double delta)
{
	(void)delta;

	mgos_tft_print_date(32, 205, MINYA24_FONT);
}


//
//
void mgos_tft_print_date( int x,  int y,  int font)
{
	char tmp_buff[32];

	double mg_now = mg_time();
	time_t now = 2*60*60 + time(0);
	struct tm* tm_info = gmtime(&now);
//	int ms = (int)((mg_now - (int)(mg_now)) * 1000);

	if (-1 != font) {
		TFT_setFont(font, NULL);
	}
	_fg = (color_t){ fg_r, fg_g, 192 };
	_bg = TFT_BLACK;

	printf("mgos_tft_print_date mg_time=%f\n", mg_now);

	sprintf(tmp_buff, "%2d.%2d  %02d:%02d:%02d", tm_info->tm_mday, 1+tm_info->tm_mon, tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec);
	TFT_clearStringRect(x, y, tmp_buff);
	TFT_print(tmp_buff, x, y);
}


//
//
enum mgos_app_init_result mgos_app_init(void)
{

	mgos_sntp_add_time_change_cb(sntp_cb, NULL);

	tft_demo();

	return MGOS_APP_INIT_SUCCESS;
}
