#ifndef _CMOS_MPU9250_H_
#define _CMOS_MPU9250_H_

#include "stm32f4xx_hal_conf.h"
   
#define TS_I2C_ADDRESS                      0x82
#define MPU9250_I2C_ADDRESS                 0xD0

/*############################### I2Cx #######################################*/
/* User can use this section to tailor I2Cx instance used and associated 
   resources */
#define DISCOVERY_I2Cx                          I2C3
#define DISCOVERY_I2Cx_CLOCK_ENABLE()           __I2C3_CLK_ENABLE()
#define DISCOVERY_I2Cx_FORCE_RESET()            __I2C3_FORCE_RESET()
#define DISCOVERY_I2Cx_RELEASE_RESET()          __I2C3_RELEASE_RESET()
#define DISCOVERY_I2Cx_SDA_GPIO_CLK_ENABLE()    __GPIOC_CLK_ENABLE()
#define DISCOVERY_I2Cx_SCL_GPIO_CLK_ENABLE()    __GPIOA_CLK_ENABLE() 
#define DISCOVERY_I2Cx_SDA_GPIO_CLK_DISABLE()   __GPIOC_CLK_DISABLE()

/* Definition for DISCO I2Cx Pins */
#define DISCOVERY_I2Cx_SCL_PIN                  GPIO_PIN_8
#define DISCOVERY_I2Cx_SCL_GPIO_PORT            GPIOA
#define DISCOVERY_I2Cx_SCL_SDA_AF               GPIO_AF4_I2C3
#define DISCOVERY_I2Cx_SDA_PIN                  GPIO_PIN_9
#define DISCOVERY_I2Cx_SDA_GPIO_PORT            GPIOC

/* Definition for IOE I2Cx's NVIC */
#define DISCOVERY_I2Cx_EV_IRQn                  I2C3_EV_IRQn
#define DISCOVERY_I2Cx_ER_IRQn                  I2C3_ER_IRQn

#define BSP_I2C_SPEED                           100000

#define I2Cx_TIMEOUT_MAX                        0x3000

/*################################ IOE #######################################*/
/* Definition for external IT for STMPE811 */
#define STMPE811_INT_PIN                        GPIO_PIN_15
#define STMPE811_INT_GPIO_PORT                  GPIOA
#define STMPE811_INT_CLK_ENABLE()               __GPIOA_CLK_ENABLE()
#define STMPE811_INT_CLK_DISABLE()              __GPIOA_CLK_DISABLE()
#define STMPE811_INT_EXTI                       EXTI15_10_IRQn
#define STMPE811_INT_EXTIHandler                EXTI15_10_IRQHandler

/* I2Cx bus function */
void    I2Cx_Init(void);
void    I2Cx_ITConfig(void);
void    I2Cx_WriteData(uint8_t Addr, uint8_t Reg, uint8_t Value);
void    I2Cx_WriteBuffer(uint8_t Addr, uint8_t Reg,  uint8_t *pBuffer, uint16_t Length);
uint8_t I2Cx_ReadData(uint8_t Addr, uint8_t Reg);
uint8_t I2Cx_ReadBuffer(uint8_t Addr, uint8_t Reg, uint8_t *pBuffer, uint16_t Length);

#endif /* _CMOS_MPU9250_H_ */

