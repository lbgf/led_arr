/*
控制16*16的灯阵，主控板是esp32 d1，使用蓝牙进行通讯
*/

#include "BluetoothSerial.h"
#include <FastLED.h>
#include <Adafruit_GFX.h>  // 绘图库 
#include <FastLED_NeoMatrix.h>  
#include <Fonts/TomThumb.h>  // 字体库

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#define LED_PIN     12 // 控制线使用的端口
#define NUM_LEDS    256 // 一共256个led灯

CRGB leds[NUM_LEDS];
FastLED_NeoMatrix *matrix;

String ctrlMsg = ""; // 保存控制信息

// test:s_0005ffcc00abcdefgh@f_@ c_@f_@

// 初始化矩阵显示屏相关参数
void initMatrix() {
  matrix = new FastLED_NeoMatrix(leds, 16, 16, NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, 256).setCorrection(TypicalLEDStrip);
  matrix->begin();  // 屏幕初始化  
  matrix->setTextWrap(true);  // 文字换行
  matrix->setBrightness(30);  // 亮度
  matrix->setFont(&TomThumb);  // 字体
}

void setup() {
  Serial.begin(9600);
  SerialBT.begin("16_16_bt"); // Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  initMatrix(); // 初始化矩阵显示屏相关参数
  matrix->clear();
  matrix->show();
}

void loop() {

  if (SerialBT.available() || Serial.available()) {
    String inBuffer = "";
    if (SerialBT.available()) { // 蓝牙获取信息
      inBuffer =  SerialBT.readStringUntil('@');
    } else { // 串口获取信息（方便调试）
      inBuffer =  Serial.readStringUntil('@');
    }
    ctrlMsg = inBuffer;
    // Serial.println(ctrlMsg);
    String cmd = ctrlMsg.substring(0, 2);
    // Serial.println(cmd);
    
    if (cmd.indexOf("f_") >= 0) { // 结束并显示

      matrix->show();
      
    } else if(cmd.indexOf("c_") >= 0) { // 清屏
      
      matrix->clear();
      // matrix->show();
      
    } else if(cmd.indexOf("p_") >= 0) { // 画点

      String base = ctrlMsg.substring(2, 12);
      unsigned char out[5];
      int outlen = 0;
      char *token0 = const_cast<char *>(base.c_str());
      stringToHex(token0, out, &outlen);
      if(outlen == 5) {
        matrix->drawPixel(out[0], out[1], matrix->Color(out[2], out[3], out[4])); 
      }
      
    } else if(cmd.indexOf("s_") >= 0) { // 字符

      String base = ctrlMsg.substring(2, 12);
      String str = ctrlMsg.substring(12, ctrlMsg.length());
      // Serial.println(str);
      unsigned char out[5];
      int outlen = 0;
      char *token0 = const_cast<char *>(base.c_str());
      stringToHex(token0, out, &outlen);
      
      if(outlen == 5) {
        matrix->setTextColor(matrix->Color(out[2], out[3], out[4]));  // 文本颜色
        matrix->setCursor(out[0], out[1]);  // 文本位置 
        matrix->print(str);
      }
      
    } else if(cmd.indexOf("v_") >= 0) { // 画垂直线

      String content = ctrlMsg.substring(2, 14);
      unsigned char out[6];
      int outlen = 0;
      char *token0 = const_cast<char *>(content.c_str());
      stringToHex(token0, out, &outlen);
      if(outlen == 6) {
        matrix->drawFastVLine(out[0], out[1], out[2], matrix->Color(out[3], out[4], out[5])); 
      }
      
    } else if(cmd.indexOf("h_") >= 0) { // 画水平线

      String content = ctrlMsg.substring(2, 14);
      unsigned char out[6];
      int outlen = 0;
      char *token0 = const_cast<char *>(content.c_str());
      stringToHex(token0, out, &outlen);
      if(outlen == 6) {
        matrix->drawFastHLine(out[0], out[1], out[2], matrix->Color(out[3], out[4], out[5])); 
      }
      
    }
  }

}

// 字符转数字
int stringToHex(char *str, unsigned char *out, int *outlen) {
  char *p = str;
  char high = 0, low = 0;
  int tmplen = strlen(p), cnt = 0;
  tmplen = strlen(p);
  while (cnt < (tmplen / 2)) {
    high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
    low = (*(++p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p) - 48 - 7 : *(p) - 48;
    out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
    p++;
    cnt++;
  }
  if (tmplen % 2 != 0) { 
    out[cnt] = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;
  }
  if (outlen != NULL) { 
    *outlen = tmplen / 2 + tmplen % 2;
  }
  return tmplen / 2 + tmplen % 2;
}
