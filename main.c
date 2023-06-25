/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//TUDO ISSO EH DA SERIAL
#define TCMD 1 //TAMANHO DO COMANDO
#define TMSG 200 //TAMANHO DA MENSAGEM
#define TOUT 1000
uint8_t comando[TCMD];
//200 = TMSG
char msg[200]= "\n\rDigite L para a sequencia aparecer em ordem, R para a sequencia aparecer de tras para frente, S para parar e A para continuar\r\n";
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//VARIAVEIS DE CONTROLE
int conta=0, b1=0, b2=0, a=0, b=0, c=0, d=0, e=0, f=0, g=0, p=1;
int direita=0, esquerda=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void testeConta()
{
	if(conta > 6) conta = 1;
	if(conta < 1) conta = 6;
}
//PARA FORMAR OS TRAÇOS
void linhas()
{
	switch(conta){
		case 1:
		a=0; b=1; c=1; d=1; e=1; f=1; g=1; //0 liga 1 desliga
		break;

		case 2:
		a=1; b=1; c=1; d=1; e=1; f=1; g=0;
		break;

		case 3:
		a=1; b=1; c=1; d=0; e=1; f=1; g=1;
		break;

		case 4:
		a=1; b=1; c=0; d=1; e=1; f=1; g=1;
		break;

		case 5:
		a=1; b=1; c=1; d=1; e=1; f=1; g=0;
		break;

		case 6:
		a=1; b=1; c=1; d=1; e=1; f=0; g=1;
		break;

		default:
		a=1; b=1; c=1; d=1; e=1; f=1; g=1;
}
	//ALTERA O ESTADO DOS SEGMENTOS
	HAL_GPIO_WritePin(A_GPIO_Port, A_Pin, a);
	HAL_GPIO_WritePin(B_GPIO_Port, B_Pin, b);
	HAL_GPIO_WritePin(C_GPIO_Port, C_Pin, c);
	HAL_GPIO_WritePin(D_GPIO_Port, D_Pin, d);
	HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, e);
	HAL_GPIO_WritePin(F_GPIO_Port, F_Pin, f);
	HAL_GPIO_WritePin(G_GPIO_Port, G_Pin, g);
}

//FUNCAO PARA MUDAR O ESTADO DO PONTO
void dot()
{
	p^=1;
	HAL_GPIO_WritePin(ponto_GPIO_Port, ponto_Pin, p);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM10_Init();
  MX_TIM11_Init();
  /* USER CODE BEGIN 2 */

  //PRO TIMER FUNCIONAR COM INTERRUPCAO
  HAL_TIM_Base_Start_IT(&htim11);
  HAL_TIM_Base_Start_IT(&htim10);

  //manda a mensagem
  HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);
  //recebe a mensagem
  HAL_UART_Receive_IT(&huart2, comando, TCMD);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(msg[0]!='\0'){
		  HAL_UART_Transmit(&huart2, msg, strlen(msg), TOUT);
		  msg[0]='\0';
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

//QUANDO RECEBE MENSAGEM PELA SERIAL VEM PRA CA
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	comando[0]=toupper(comando[0]);

	switch(comando[0]){
		case 'L':
			direita=0; esquerda=1;
			dot();
			strcpy(msg,"\r\nesquerda para direita\r\n");
			break;

		case 'R':
			direita=1; esquerda=0;
			dot();
			strcpy(msg,"\r\ndireita para esquerda\r\n");
			break;

		case 'S':
			HAL_TIM_Base_Stop_IT(&htim11); //desliga o timer
			dot();
			break;

		case 'A':
			HAL_TIM_Base_Start_IT(&htim11); //liga o timer
			dot();
			break;

		default:
			strcpy(msg,"\r\nParametro invalido\r\n");
	}
	HAL_UART_Receive_IT(&huart2, comando,TCMD); //pronto para receber outra mensagem
}

//QUANDO O TIMER ESTOURA VEM PRA CA
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM10){
		if(HAL_GPIO_ReadPin(botao1_GPIO_Port, botao1_Pin)){
			b1++;

			if(b1==1){
				HAL_TIM_Base_Start_IT(&htim11); //liga o timer
				__HAL_TIM_SET_AUTORELOAD(&htim11,CEM); //muda a frequencia do timer
				strcpy(msg,"\r\n100ms\r\n");
			}

			if(b1==2){
				__HAL_TIM_SET_AUTORELOAD(&htim11,TREZENTOS);
				strcpy(msg,"\r\n300ms\r\n");
			}

			if(b1==3){
				__HAL_TIM_SET_AUTORELOAD(&htim11,SETECENTOS);
				strcpy(msg,"\r\n700ms\r\n");
			}

			if(b1==4){
				__HAL_TIM_SET_AUTORELOAD(&htim11,UM);
				strcpy(msg,"\r\n1s\r\n");
				b1=0;
			}
		}

		if(HAL_GPIO_ReadPin(botao2_GPIO_Port, botao2_Pin)){
			b2^=1;

			if(b2==0){
				HAL_TIM_Base_Stop_IT(&htim11);
			}

			else{
				HAL_TIM_Base_Start_IT(&htim11);

				if(esquerda==1)conta=1;

				if(direita==1)conta=6;

				strcpy(msg,"\r\nrecomecando...\r\n");
			}
			linhas();
		}

  }

	if(htim->Instance==TIM11){

		if(direita==1){
			conta--;
		}
		
		else{
			conta++;
		}

		testeConta();
		linhas();
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */