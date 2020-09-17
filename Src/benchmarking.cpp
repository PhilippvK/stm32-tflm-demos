/**
  ******************************************************************************
  * @file           : benchmarking.c
  * @brief          : Small benchmarking suite
  ******************************************************************************
  * @attention
  *
  * TODO: License
  *
  ******************************************************************************
  */

/* Private includes ----------------------------------------------------------*/
#include "benchmarking.h"
#include <cstdio>

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
ticks_info ticks_infos[TYPE_COUNT];
const char *tick_labels[] = {
    "Populate", "Invoke", "Respond", "Other",
};

/* Private function prototypes -----------------------------------------------*/

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  TODO
  * @parm   TODO
  * @retval void
  */
void print_summary(int types)
{
  int mask = 1, index = 0;

  fprintf(stderr, "TIMESTAMP: %ld ms\n\r", HAL_GetTick());
  while (index < TYPE_COUNT) {
    if (mask & types) {
      fprintf(stderr, "Ticks for %s (cur/min/max/avg): %ld/%ld/%ld/%ld\n\r", tick_labels[index], ticks_infos[index].cur, ticks_infos[index].min, ticks_infos[index].max, ticks_infos[index].avg);
    }
    index++;
    mask <<= 1;
  }
}

/**
  * @brief  TODO
  * @parm   TODO
  * @retval void
  */
void update_avg_ticks(int type, int32_t ticks)
{
  int mask = 1, index = 0;
  while (index < TYPE_COUNT) {
    if (mask & type) {
      break;
    }
    index++;
    mask <<= 1;
  }
  ticks_infos[index].cur = ticks;
  if (ticks > ticks_infos[index].max) {
    ticks_infos[index].max = ticks;
  } else if (ticks < ticks_infos[index].min) {
    ticks_infos[index].min = ticks;
  }
  // average windowing
  if (ticks_infos[index].count < AVG_LEN) {
    ticks_infos[index].count++;
  } else {
    ticks_infos[index].sum -= ticks_infos[index].ticks[ticks_infos[index].index];
  }
  ticks_infos[index].ticks[ticks_infos[index].index] = ticks;
  ticks_infos[index].sum += ticks;
  if (ticks_infos[index].index == (AVG_LEN - 1)) {
    ticks_infos[index].index = 0;
  } else {
    ticks_infos[index].index++;
  }
  ticks_infos[index].avg = ticks_infos[index].sum / ticks_infos[index].count;
}

/**
  * @brief  TODO
  * @parm   TODO
  * @retval void
  */
int32_t get_cur_ticks(int type)
{
  int mask = 1, index = 0;
  while (index < TYPE_COUNT) {
    if (mask & type) {
      break;
    }
    index++;
    mask <<= 1;
  }
  return ticks_infos[index].cur;
}

/**
  * @brief  TODO
  * @parm   TODO
  * @retval void
  */
int32_t get_avg_ticks(int type)
{
  int mask = 1, index = 0;
  while (index < TYPE_COUNT) {
    if (mask & type) {
      break;
    }
    index++;
    mask <<= 1;
  }
  return ticks_infos[index].avg;
}

/**
  * @brief  TODO
  * @parm   TODO
  * @retval void
  */
int32_t get_min_ticks(int type)
{
  int mask = 1, index = 0;
  while (index < TYPE_COUNT) {
    if (mask & type) {
      break;
    }
    index++;
    mask <<= 1;
  }
  return ticks_infos[index].min;
}

/**
  * @brief  TODO
  * @parm   TODO
  * @retval void
  */
int32_t get_max_ticks(int type)
{
  int mask = 1, index = 0;
  while (index < TYPE_COUNT) {
    if (mask & type) {
      break;
    }
    index++;
    mask <<= 1;
  }
  return ticks_infos[index].max;
}
