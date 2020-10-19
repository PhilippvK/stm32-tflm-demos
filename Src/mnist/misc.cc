// TODO

#include "misc.h"
#include "main.h"
#include <cstring>

// MNIST (TODO: mode to misc.cc)
signed char prev_value = -1;
bool touched = false;
static int32_t x0 = -1, y0 = -1; // TODO
TS_StateTypeDef TS_State;
static uint16_t TSInputImage[INPUT_IMAGE_SIZE_PIXEL][INPUT_IMAGE_SIZE_PIXEL] = {
  0
};

static uint8_t NNInputImage[INPUT_IMAGE_SIZE][INPUT_IMAGE_SIZE] = { 0 };
static const uint32_t image_size = INPUT_IMAGE_SIZE;
static const uint32_t image_size_pixel = INPUT_IMAGE_SIZE_PIXEL;

void MNISTReset() {
  touched = false;
  x0 = -1;
  y0 = -1;
}

uint8_t* MNISTGetNNInputImage() {
  return (uint8_t*) NNInputImage;
}

// TODO
uint16_t AverageImageBlock(uint32_t block_size, uint32_t x, uint32_t y)
{
  uint32_t ret[3] = { 0 };

  for (int i = 0; i < block_size; i++)
    for (int j = 0; j < block_size; j++) {
      ret[0] += TSInputImage[y * block_size + i]
                [x * block_size + j] &
          0x1F;
      ret[1] += (TSInputImage[y * block_size + i]
                 [x * block_size + j] >>
           5) &
          0x3F;
      ret[2] += (TSInputImage[y * block_size + i]
                 [x * block_size + j] >>
           11) &
          0x1F;
    }

  return (((ret[0] / (block_size * block_size)) & 0x1F) |
    (((ret[1] / (block_size * block_size)) & 0x3F) << 5) |
    (((ret[2] / (block_size * block_size)) & 0x1F) << 11));
}

void PreprocessImage(void)
{
  uint8_t scale_factor = image_size_pixel / image_size;

  for (int y = 0; y < image_size; y++)
    for (int x = 0; x < image_size; x++) {
      NNInputImage[y][x] = 255 -
        AverageImageBlock(scale_factor, x, y);
    }
}

uint8_t ConvertHighColorToGS(uint16_t pixel)
{
  uint8_t r = pixel >> 11 & 0x1F;
  uint8_t g = pixel >> 5 & 0x3F;
  uint8_t b = pixel & 0x1F;
  uint8_t luminance = (r + (g>>1) + b) / 3;
  uint8_t gs = luminance * (255 / 31.0);
  return gs;
}

static void SendImageUART(void *image, size_t pixel_size, size_t image_size,
        char flags)
{
  char *buffer = (char *)malloc((image_size + 3) * sizeof(char));
  memset(buffer, 0, (image_size + 3) * sizeof(char));
  fprintf(stderr, "**********IMAGE START**********\n\r");
  for (int i = 0; i < image_size; i++) {
    buffer[0] = '|';
    for (int j = 0; j < image_size; j++) {
      if (flags & 0b1) {
        buffer[j + 1] = (*((char *)image +
               (i * image_size + j) *
                 pixel_size) == 255) ?
              '.' :
              '#';
      } else {
        int index = j+1;
        int offset = (i * image_size + j) * pixel_size; 
        char value = 255-*((char *)image + offset);
        char cond = value == 255;
        if (cond) {
          buffer[index] = '.';
        } else {
          char mod = value % 26;
          char ch = 'A' + mod;
          buffer[index] = ch;
        }
      }
    }
    buffer[image_size] = '|';
    fprintf(stderr, "%s\n\r", buffer);
  }
  fprintf(stderr, "**********IMAGE STOP**********\n\r");
  free(buffer);
}

static void DrawShrunkImage(int x, int y)
{
  for (int i = 0; i < INPUT_IMAGE_SIZE; i++) {
    for (int j = 0; j < INPUT_IMAGE_SIZE; j++) {
      BSP_LCD_DrawPixel(x + j, y + i, GS2RGB(NNInputImage[i][j]));
    }
  }
}

void DrawInputBox(void) {
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect(INPUT_BOX_X_OUTER, INPUT_BOX_Y_OUTER, INPUT_BOX_WIDTH,
       INPUT_BOX_WIDTH);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_FillRect(INPUT_BOX_X_INNER, INPUT_BOX_Y_INNER,
       INPUT_IMAGE_SIZE_PIXEL, INPUT_IMAGE_SIZE_PIXEL);
}

void SaveMNISTInput(void)
{
#ifndef FAKE_TOUCH
  volatile int marginX = INPUT_BOX_X_INNER;
  volatile int marginY = INPUT_BOX_Y_INNER;
  TSInputImage[0][0] = 1; // TODO: find out why?
  for (int y = 0; y < image_size_pixel; y++) {
    for (int x = 0; x < image_size_pixel; x++) {
      TSInputImage[y][x] = ConvertHighColorToGS(BSP_LCD_ReadPixel(x + marginX, y + marginY));
    }
  }

  PreprocessImage();
  // TODO: clear Touch input
#else
  volatile int marginX = INPUT_BOX_X_INNER + INPUT_IMAGE_SIZE_PIXEL/2 - image_size/2;
  volatile int marginY = INPUT_BOX_Y_INNER + INPUT_IMAGE_SIZE_PIXEL/2 - image_size/2;
  for (int y = 0; y < image_size; y++) {
    for (int x = 0; x < image_size; x++) {
      NNInputImage[y][x] = ConvertHighColorToGS(BSP_LCD_ReadPixel(x + marginX, y + marginY));
    }
  }
#endif /* FAKE_TOUCH */
  DrawInputBox();
  DrawShrunkImage(BSP_LCD_GetXSize() / 2 - INPUT_IMAGE_SIZE / 2,
      BSP_LCD_GetYSize() / 2 - INPUT_IMAGE_SIZE / 2);
  SendImageUART((void *)NNInputImage, sizeof(char), INPUT_IMAGE_SIZE, 0);
}

void DrawInputScreen(void)
{
  //BSP_LCD_SetFont(&Font16);
  //BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  //BSP_LCD_FillRect(INPUT_BOX_X_OUTER, INPUT_BOX_Y_OUTER, INPUT_BOX_WIDTH,
  //     INPUT_BOX_WIDTH);
  //BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  //BSP_LCD_FillRect(INPUT_BOX_X_INNER, INPUT_BOX_Y_INNER,
  //     INPUT_IMAGE_SIZE_PIXEL, INPUT_IMAGE_SIZE_PIXEL);
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  if (prev_value != -1) {
    char buf[2];
    sprintf(buf, "%c", prev_value);
    BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)buf, CENTER_MODE);
  } else {
    //BSP_LCD_DisplayStringAt(0, 200, (uint8_t *)"Press USER for classification", CENTER_MODE);
  }

}

char determine_char(int8_t *out_array)
{
  int index = 0;
  int8_t max_val = out_array[index];

  for (int i = 1; i < 10; i++)
    if (out_array[i] > max_val) {
      index = i;
      max_val = out_array[i];
    }

  return (char)index + 48;
}

void printPercentages(int8_t *output_array)
{
  char buff[10] = { 0 };
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  for (int i = 0; i < 10; i++) {
    sprintf(buff, "%d:%3d%%", i, ((int16_t)output_array[i]+128)*100/255);
    BSP_LCD_DisplayStringAt(INPUT_BOX_X_OUTER + 20,
          INPUT_BOX_Y_OUTER + 35 + i * 13,
          (uint8_t *)buff, LEFT_MODE);
  }
}

uint8_t DrawInBox(uint32_t x, uint32_t y, uint32_t dot_radius)
{
	if ((x < (INPUT_BOX_X_INNER + dot_radius)) ||
	    (x > (INPUT_BOX_X_INNER + INPUT_IMAGE_SIZE_PIXEL - dot_radius))) {
		return 0;
	}
	if ((y < (INPUT_BOX_Y_INNER + dot_radius)) ||
	    (y > (INPUT_BOX_Y_INNER + INPUT_IMAGE_SIZE_PIXEL - dot_radius))) {
		return 0;
	}

	return 1;
}

void PrintMNISTHint() {
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_LCD_DisplayStringAt(0, INPUT_BOX_Y_INNER+INPUT_IMAGE_SIZE_PIXEL-60, (uint8_t *)"Start Drawing and", CENTER_MODE); // TODO: Move to MNISTHint()
  BSP_LCD_DisplayStringAt(0, INPUT_BOX_Y_INNER+INPUT_IMAGE_SIZE_PIXEL-40, (uint8_t *)"press USER when done", CENTER_MODE);
}

bool GetTouchInput() {
  static const uint32_t dot_radius = TOUCH_DOT_RADIUS;
  static int32_t x = 0, y = 0;

  if (BSP_TS_GetState(&TS_State) != TS_OK) {
    return false;
  }


  if(CheckForUserInput()) {
    if (touched) {
      return true;
    } else {
      DrawInputBox();
      PrintMNISTHint();
      return false;;
    }
  } else if (TS_State.touchDetected) {
    x = TS_State.touchX[0];
    y = TS_State.touchY[0];

    if (DrawInBox(x, y, dot_radius)) {
      if (x0 >= 0 && y0 >=0) {
        /* If manhatten distance is smaller equals TOUCH_DOT_RADIUS*2
         * interpolate between two points to look less 'dotted' */
        if ((abs(x-x0)+abs(y-y0)) <= dot_radius*2) { 
          BSP_LCD_FillCircle((x+x0)/2, (y+y0)/2, dot_radius);
        }
      } else {
          DrawInputBox();
          touched = true;
      }
      BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
      BSP_LCD_FillCircle(x, y, dot_radius);
      x0 = x;
      y0 = y;
    }
  }
  return false;
}

void MNISTHandleOutput(int8_t* output_array) {
  prev_value = determine_char(output_array);
  char buff[10] = { 0 };
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  sprintf(buff, "Char: %c", prev_value);
  BSP_LCD_DisplayStringAt(0,
        INPUT_BOX_Y_OUTER + INPUT_IMAGE_SIZE - 20,
        (uint8_t *)buff, CENTER_MODE);

  printPercentages(output_array);
#ifdef FAKE_TOUCH
  HAL_Delay(1000);
#endif /* FAKE_TOUCH */
}
