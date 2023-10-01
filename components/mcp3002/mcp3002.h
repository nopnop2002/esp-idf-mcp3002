#ifndef MAIN_MCP3002_H_
#define MAIN_MCP3002_H_

#include "driver/spi_master.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int _model;
	int	_bits;
	int	_channels;
	int _input;
	spi_device_handle_t _handle;
} MCP_t;

enum MCP_MODEL {
	MCP3001,
	MCP3002,
	MCP3004,
	MCP3008,
	MCP3201,
	MCP3202,
	MCP3204,
	MCP3208,
	MCP3301,
	MCP3302,
	MCP3304
};

enum MCP_INPUT {
	MCP_SINGLE,
	MCP_DIFF
};

#define SPI_MASTER_FREQ_1M (APB_CLK_FREQ/80)
#define SPI_MASTER_FREQ_2M (APB_CLK_FREQ/40)
#define SPI_MASTER_FREQ_4M (APB_CLK_FREQ/20)

void mcpInit(MCP_t * dev, int16_t model, int16_t GPIO_MISO, int16_t GPIO_MOSI, int16_t GPIO_SCLK, int16_t GPIO_CS, int16_t input);
int16_t mcpReadData(MCP_t * dev, int16_t channel);

#ifdef __cplusplus
}
#endif

#endif /* MAIN_MCP3002_H_ */
