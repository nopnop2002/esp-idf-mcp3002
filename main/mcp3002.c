#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <driver/spi_master.h>
#include <driver/gpio.h>
#include "esp_log.h"

#include "mcp3002.h"

#define TAG "MCP3002"

#if 0
#ifdef CONFIG_IDF_TARGET_ESP32
#define LCD_HOST HSPI_HOST
#elif defined CONFIG_IDF_TARGET_ESP32S2
#define LCD_HOST SPI2_HOST
#elif defined CONFIG_IDF_TARGET_ESP32C3
#define LCD_HOST SPI2_HOST
#endif
#endif

// SPI Stuff
#if CONFIG_SPI2_HOST
#define HOST_ID SPI2_HOST
#elif CONFIG_SPI3_HOST
#define HOST_ID SPI3_HOST
#endif

void mcpInit(MCP_t * dev, int16_t model, int16_t GPIO_MISO, int16_t GPIO_MOSI, int16_t GPIO_SCLK, int16_t GPIO_CS, int16_t input)
{
	esp_err_t ret;

	ESP_LOGI(TAG, "GPIO_CS=%d",GPIO_CS);
	gpio_reset_pin( GPIO_CS );
	gpio_set_direction( GPIO_CS, GPIO_MODE_OUTPUT );
	gpio_set_level( GPIO_CS, 1 );

	ESP_LOGI(TAG, "GPIO_MOSI=%d",GPIO_MOSI);
	ESP_LOGI(TAG, "GPIO_SCLK=%d",GPIO_SCLK);
	spi_bus_config_t buscfg = {
		.sclk_io_num = GPIO_SCLK,
		.mosi_io_num = GPIO_MOSI,
		.miso_io_num = GPIO_MISO,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1
	};

	ret = spi_bus_initialize( HOST_ID, &buscfg, SPI_DMA_CH_AUTO );
	ESP_LOGD(TAG, "spi_bus_initialize=%d",ret);
	assert(ret==ESP_OK);

	spi_device_interface_config_t devcfg={
		.clock_speed_hz = SPI_MASTER_FREQ_1M,
		.spics_io_num = GPIO_CS,
		.queue_size = 7,
		.mode = 0,
		.flags = SPI_DEVICE_NO_DUMMY,
	};

	spi_device_handle_t handle;
	ret = spi_bus_add_device( HOST_ID, &devcfg, &handle);
	ESP_LOGD(TAG, "spi_bus_add_device=%d",ret);
	assert(ret==ESP_OK);
	dev->_handle = handle;
	dev->_model = model;
	dev->_input = input;
	if (model == MCP3002) {
		dev->_bits = 10;
		dev->_channels = 2;
	} else if (model == MCP3004) {
		dev->_bits = 10;
		dev->_channels = 4;
	} else if (model == MCP3008) {
		dev->_bits = 10;
		dev->_channels = 8;
	} else if (model == MCP3201) {
		dev->_bits = 12;
		dev->_channels = 1;
	} else if (model == MCP3202) {
		dev->_bits = 12;
		dev->_channels = 2;
	} else if (model == MCP3204) {
		dev->_bits = 12;
		dev->_channels = 4;
	} else if (model == MCP3208) {
		dev->_bits = 12;
		dev->_channels = 8;
	} else if (model == MCP3302) {
		dev->_bits = 13;
		dev->_channels = 2;
	} else if (model == MCP3304) {
		dev->_bits = 13;
		dev->_channels = 4;
	}
}

int16_t mcpReadData(MCP_t * dev, int16_t channel)
{
	char rbuf[3];
	char wbuf[3];
	int16_t val = 0;

	if (channel > dev->_channels) {
		ESP_LOGE(TAG, "Illegal channel %d", channel);
		return 0;
	}

	memset(wbuf, 0, sizeof(rbuf));
	memset(rbuf, 0, sizeof(rbuf));
	if (dev->_model == MCP3002 || dev->_model == MCP3202) {
		// MCP3002
		// [IN]  00 00 00 START SGL/DIFF ODD/SIGN MSBF 
		// [OUT] -- -- -- ----- -------- -------- ---- 00 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0
		// MCP3202
		// [IN]  00 00 00 START SGL/DIFF ODD/SIGN MSBF 
		// [OUT] -- -- -- ----- -------- -------- ---- 00 B11 B10 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0
		if (dev->_input == MCP_SINGLE) {
			wbuf[0] = 0x18 | channel << 2;
		} else {
			wbuf[0] = 0x10 | channel << 2;
		}
	} else if (dev->_model == MCP3004 || dev->_model == MCP3008 || dev->_model == MCP3204 || dev->_model == MCP3208 || dev->_model == MCP3302 || dev->_model == MCP3304) {
		// Need DMY bit
		// MCP300x
		// [IN]  00 START SGL/DIFF D2 D1 D0 DMY
		// [OUT] -- ----- -------- -- -- -- --- 00 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0
		// MCP320x
		// [IN]  00 START SGL/DIFF D2 D1 D0 DMY
		// [OUT] -- ----- -------- -- -- -- --- 00 B11 B10 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0
		// MCP330x
		// [IN]  00 START SGL/DIFF D2 D1 D0 DMY
		// [OUT] -- ----- -------- -- -- -- --- 00 SB B11 B10 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0
		if (dev->_input == MCP_SINGLE) {
			wbuf[0] = 0x60 | channel << 2;
		} else {
			wbuf[0] = 0x40 | channel << 2;
		}
	} else if (dev->_model == MCP3001 || dev->_model == MCP3201 || dev->_model == MCP3301) {
		// There is no START bit
		// There is no SINGLE/DIFF
		// 3'rd bit is NULL bit
		// MCP3001
		// [IN]  XX XX
		// [OUT] -- -- 00 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0
		// MCP3201
		// [IN]  XX XX
		// [OUT] -- -- 00 B11 B10 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0
		// MCP3301
		// [IN]  XX XX
		// [OUT] -- -- 00 SB B11 B10 B9 B8 B7 B6 B5 B4 B3 B2 B1 B0
		wbuf[0] = 0x00;
	}
	ESP_LOGD(TAG, "wbuf=0x%02X 0x%02X", wbuf[0], wbuf[1]);
	spi_transaction_t SPITransaction;
	esp_err_t ret;

	memset( &SPITransaction, 0, sizeof( spi_transaction_t ) );
	SPITransaction.length = 3 * 8;
	SPITransaction.tx_buffer = wbuf;
	SPITransaction.rx_buffer = rbuf;
#if 1
	ret = spi_device_transmit( dev->_handle, &SPITransaction );
#else
	ret = spi_device_polling_transmit( dev->_handle, &SPITransaction );
#endif
	assert(ret==ESP_OK); 
	ESP_LOGD(TAG, "rbuf[0]=%02X rbuf[1]=%02X rbuf[2]=%02X", rbuf[0], rbuf[1], rbuf[2]);
	if (dev->_model == MCP3001) {
		val = ((rbuf[0]&0x1F)<<5)+(rbuf[1]>>3);
	} else if (dev->_model == MCP3201) {
		val = ((rbuf[0]&0x1F)<<7)+(rbuf[1]>>1);
	} else if (dev->_model == MCP3301) {
		val = ((rbuf[0]&0x0F)<<8)+(rbuf[1]);
		// Sign bit ON
		if ((rbuf[0]&0x10) == 0x10) {
			val = val - 0xFFF;
		}
	} else {
		if (dev->_bits == 10) {
			val = (rbuf[1]<<2)+(rbuf[2]>>6);
		} else if (dev->_bits == 12) {
			val = (rbuf[1]<<4)+(rbuf[2]>>4);
		} else if (dev->_bits == 13) {
			val = ((rbuf[1]&0x7F)<<5)+(rbuf[2]>>3);
			//val = (rbuf[1]<<5)+(rbuf[2]>>3);
			// Sign bit ON
			if ((rbuf[1]&0x80) == 0x80) {
				val = val - 0xFFF;
			}
		}
	}
	ESP_LOGD(TAG, "val=%02X", val);
	return(val);
}

