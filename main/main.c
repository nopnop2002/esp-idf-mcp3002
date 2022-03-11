/* The example of ESP-IDF
 *
 * This sample code is in the public domain.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"

#include "mcp3002.h"


void app_main()
{
	int model;
#if CONFIG_MODEL_MCP3001
	model = MCP3001;
	#define TAG "MCP3001"
#elif CONFIG_MODEL_MCP3002
	model = MCP3002;
	#define TAG "MCP3002"
#elif CONFIG_MODEL_MCP3004
	model = MCP3004;
	#define TAG "MCP3004"
#elif CONFIG_MODEL_MCP3008
	model = MCP3008;
	#define TAG "MCP3008"
#elif CONFIG_MODEL_MCP3201
	model = MCP3201;
	#define TAG "MCP3201"
#elif CONFIG_MODEL_MCP3202
	model = MCP3202;
	#define TAG "MCP3202"
#elif CONFIG_MODEL_MCP3204
	model = MCP3204;
	#define TAG "MCP3204"
#elif CONFIG_MODEL_MCP3208
	model = MCP3208;
	#define TAG "MCP3208"
#elif CONFIG_MODEL_MCP3301
	model = MCP3301;
	#define TAG "MCP3301"
#elif CONFIG_MODEL_MCP3302
	model = MCP3302;
	#define TAG "MCP3302"
#elif CONFIG_MODEL_MCP3304
	model = MCP3304;
	#define TAG "MCP3304"
#endif

	ESP_LOGI(TAG, "CONFIG_MISO_GPIO=%d", CONFIG_MISO_GPIO);
	ESP_LOGI(TAG, "CONFIG_MOSI_GPIO=%d", CONFIG_MOSI_GPIO);
	ESP_LOGI(TAG, "CONFIG_SCLK_GPIO=%d", CONFIG_SCLK_GPIO);
	ESP_LOGI(TAG, "CONFIG_CS_GPIO=%d", CONFIG_CS_GPIO);
	MCP_t dev;
#if CONFIG_INPUT_SINGLE
	ESP_LOGI(TAG, "Single-ended");
	mcpInit(&dev, model, CONFIG_MISO_GPIO, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, MCP_SINGLE);
#else
	ESP_LOGI(TAG, "Differential");
	mcpInit(&dev, model, CONFIG_MISO_GPIO, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, MCP_DIFF);	
#endif

	int16_t value;
	while(1) {
#if CONFIG_CHANNEL0
		value = mcpReadData(&dev, 0);
#if CONFIG_INPUT_SINGLE
		ESP_LOGI(TAG, "Single_ended ch0 value=%d", value);
#else
		ESP_LOGI(TAG, "Differential ch0-ch1 value=%d", value);
#endif
#endif

#if CONFIG_CHANNEL1
		value = mcpReadData(&dev, 1);
#if CONFIG_INPUT_SINGLE
		ESP_LOGI(TAG, "Single_ended ch1 value=%d", value);
#else
		ESP_LOGI(TAG, "Differential ch1-ch0 value=%d", value);
#endif
#endif

#if CONFIG_CHANNEL2
		value = mcpReadData(&dev, 2);
#if CONFIG_INPUT_SINGLE
		ESP_LOGI(TAG, "Single_ended ch2 value=%d", value);
#else
		ESP_LOGI(TAG, "Differential ch2-ch3 value=%d", value);
#endif
#endif

#if CONFIG_CHANNEL3
		value = mcpReadData(&dev, 3);
#if CONFIG_INPUT_SINGLE
		ESP_LOGI(TAG, "Single_ended ch3 value=%d", value);
#else
		ESP_LOGI(TAG, "Differential ch3-ch2 value=%d", value);
#endif
#endif

#if CONFIG_CHANNEL4
		value = mcpReadData(&dev, 4);
#if CONFIG_INPUT_SINGLE
		ESP_LOGI(TAG, "Single_ended ch4 value=%d", value);
#else
		ESP_LOGI(TAG, "Differential ch4-ch5 value=%d", value);
#endif
#endif

#if CONFIG_CHANNEL5
		value = mcpReadData(&dev, 5);
#if CONFIG_INPUT_SINGLE
		ESP_LOGI(TAG, "Single_ended ch5 value=%d", value);
#else
		ESP_LOGI(TAG, "Differential ch5-ch4 value=%d", value);
#endif
#endif

#if CONFIG_CHANNEL6
		value = mcpReadData(&dev, 6);
#if CONFIG_INPUT_SINGLE
		ESP_LOGI(TAG, "Single_ended ch6 value=%d", value);
#else
		ESP_LOGI(TAG, "Differential ch6-ch7 value=%d", value);
#endif
#endif

#if CONFIG_CHANNEL7
		value = mcpReadData(&dev, 7);
#if CONFIG_INPUT_SINGLE
		ESP_LOGI(TAG, "Single_ended ch7 value=%d", value);
#else
		ESP_LOGI(TAG, "Differential ch7-ch6 value=%d", value);
#endif
#endif
		vTaskDelay(100);
	}
}
