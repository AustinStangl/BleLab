#include "ts.h"
#include "main.h"

extern ADC_HandleTypeDef hadc1;
extern uint32_t potx;
extern uint32_t poty;

Pen_Holder Pen_Point;

unsigned char flag=0;
uint32_t x=0;

int GUI_TOUCH_X_MeasureX()
{
	  unsigned short i;
	  unsigned short sum=0;
	  for(i=0;i<8;i++);
//	   	sum+=TPReadX();
	  return sum>>3;

}

int GUI_TOUCH_X_MeasureY()
{
	  unsigned short i;
	  unsigned short sum=0;
	  for(i=0;i<8;i++)
	   	sum+=TPReadY();
	  return sum>>3;
}

void GUI_TOUCH_X_MeasureXY(uint32_t *sumx, uint32_t *sumy)
{
	  uint32_t i;
	  uint32_t x=0;
	  uint32_t y=0;

	  for(i=0;i<8;i++)
	  {
	   	TPReadX(&x, &y);
	   	(*sumx)+=x;
	   	(*sumy)+=y;
	  }

	  *sumx = (*sumx)>>3;
	  *sumy = (*sumy)>>3;
}

char pfGetPENIRQ()
{

// Y1_Pin LCD_CS_Pin //A3 	need two analog inputs
// X1_Pin LCD_RS_Pin //A2
// Y2_Pin LCD_D1_Pin //9
// X2_Pin LCD_D0_Pin //8

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	uint8_t touch=0;

	//test for touch
//	HAL_GPIO_WritePin(Y1_PORT,Y1_Pin, GPIO_PIN_SET);

	//Configure GPIO pins :Y1_Pin digital input - no pullup
	GPIO_InitStruct.Pin = Y1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Y1_PORT, &GPIO_InitStruct);

	//Configure GPIO pins :Y2_Pin digital input - pullup
//	GPIO_InitStruct.Pin = Y2_Pin;
//	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	HAL_GPIO_Init(Y2_PORT, &GPIO_InitStruct);


	HAL_GPIO_WritePin(X1_PORT,X1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(X2_PORT,X2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Y2_PORT,Y2_Pin, GPIO_PIN_SET);


	touch = HAL_GPIO_ReadPin(Y1_PORT, Y1_Pin);


	//Configure GPIO pins :Y2_Pin digital output
	GPIO_InitStruct.Pin = Y1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Y1_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(X1_PORT,X1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(X2_PORT,X2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Y2_PORT,Y2_Pin, GPIO_PIN_RESET);

	return touch; //active lo  - returns 0 if pressed

}




unsigned short int TPReadY(void)
{
	unsigned short int y=0;


	return y;
}


/* USER CODE BEGIN 4 */

// Y1_Pin LCD_CS_Pin //A3 	need two analog inputs
// X1_Pin LCD_RS_Pin //A2
// Y2_Pin LCD_D1_Pin //9
// X2_Pin LCD_D0_Pin //8


void TPReadX(uint32_t *X, uint32_t *Y)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	ADC_ChannelConfTypeDef sConfig = {0};

	//setup for touch

	//X

	  /** Configure Regular Channel */
	  sConfig.Channel = ADC_CHANNEL_3;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_247CYCLES_5;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }


	  GPIO_InitStruct.Pin = Y1_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  //Configure GPIO pins : Y2 input with pullups
	  GPIO_InitStruct.Pin = Y2_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  //Configure GPIO pins : X1_Pin output
	  GPIO_InitStruct.Pin = X1_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	  //Configure GPIO pins : X2_Pin output
	  GPIO_InitStruct.Pin = X2_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  HAL_GPIO_WritePin(GPIOC,X1_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOB,X2_Pin, GPIO_PIN_SET);

	  HAL_Delay(1);

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 200);
	  *X = HAL_ADC_GetValue(&hadc1);


	  //Configure GPIO pins : Y1_Pin output
	  GPIO_InitStruct.Pin = Y1_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


	  //Configure GPIO pins : Y2 output
	  GPIO_InitStruct.Pin = Y2_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	  //Y

	  /** Configure Regular Channel */
	  sConfig.Channel = ADC_CHANNEL_4;
	  sConfig.Rank = ADC_REGULAR_RANK_1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }



	  GPIO_InitStruct.Pin = X1_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


	  //Configure GPIO pins : X2 input with pullups
	  GPIO_InitStruct.Pin = X2_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  HAL_GPIO_WritePin(GPIOC,Y1_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOA,Y2_Pin, GPIO_PIN_SET);

	  HAL_Delay(1);

	  HAL_ADC_Start(&hadc1);
	  HAL_ADC_PollForConversion(&hadc1, 200);
	  *Y = HAL_ADC_GetValue(&hadc1);


	  //Configure GPIO pins : X1_Pin output
	  GPIO_InitStruct.Pin = X1_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


	  //Configure GPIO pins : X2 output
	  GPIO_InitStruct.Pin = X2_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


}


#define TSLEFT 3520
#define TSRIGHT 450
#define TSTOP 3515
#define TSBOTTOM 580

//hard code these so not to recalculate every time
//#define XDIVISOR (TSRIGHT-TSLEFT)/320
//#define YDIVISOR (TSBOTTOM-TSTOP)/240


#define XDIVISOR 9.59	//(TSRIGHT-TSLEFT)/320   	(3520-450)/320=9.59
#define YDIVISOR 12.23	//(TSBOTTOM-TSTOP)/240		(3515-580)/240=12.23


void Convert_Pos(void)
{
//	Pen_Point.X=GUI_TOUCH_X_MeasureX();
//	Pen_Point.Y=GUI_TOUCH_X_MeasureY();

	Pen_Point.X=potx;
	Pen_Point.Y=poty;


	Pen_Point.X0=(int)320-((Pen_Point.Y-TSRIGHT)/XDIVISOR); //widescreen xy, 00 upper left
	Pen_Point.Y0=(int)240-((Pen_Point.X-TSBOTTOM)/YDIVISOR);

	pstate.x = Pen_Point.X0;
	pstate.y = Pen_Point.Y0;

	if(pstate.x || pstate.y)
		pstate.Pressed = 1;		//pressed active lo   pfGetPENIRQ() works for button
	else
		pstate.Pressed = 0;

	GUI_PID_StoreState(&pstate);



}

