#include "max6675.h"
#include "stdint.h"
max6675_tc *tc_init(SPI_HandleTypeDef *hspi, GPIO_TypeDef *CS_PORT, uint16_t CS_PIN)
{
    max6675_tc *sensor = (max6675_tc *)malloc(sizeof(max6675_tc));
    sensor->hspi = hspi;
    sensor->CS_PORT = CS_PORT;
    sensor->CS_PIN = CS_PIN;
    return sensor;
}

float tc_readTemp(max6675_tc *sensor)
{
    uint8_t buffer[2];
    uint16_t rawValue;
    if (!sensor || !sensor->CS_PORT || !sensor->hspi)
    {
        return 0.0f;
    }
    HAL_GPIO_WritePin(sensor->CS_PORT, sensor->CS_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
    if (HAL_SPI_Receive(sensor->hspi, buffer, 2, 100) != HAL_OK)
    {
        HAL_GPIO_WritePin(sensor->CS_PORT, sensor->CS_PIN, GPIO_PIN_SET);
        return sensor->tempCelcius;
    }
    HAL_GPIO_WritePin(sensor->CS_PORT, sensor->CS_PIN, GPIO_PIN_SET);

    rawValue = (buffer[0] | (buffer[1] << 8)) >> 3;
    float celcius = rawValue * HIGH_TEMP_C / DATA_OUTPUT_RES;
    sensor->tempCelcius = celcius;

    return celcius;
}

void tc_free(max6675_tc *sensor)
{
    free(sensor);
}