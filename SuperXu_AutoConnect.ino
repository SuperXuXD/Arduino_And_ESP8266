/*
 * smartConfig：可通过ESPTouch等APP或者 “安信可科技” 微信公众号进行WiFi密码配置
 */

#include <ESP8266WiFi.h>
int count=0;
int i=0;

void smartConfig()
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig...");
  WiFi.beginSmartConfig();//等待手机端发出的用户名与密码
  while (1)
  {
    Serial.print("."); 
    digitalWrite(LED_BUILTIN, HIGH);  //连接过程中指示灯每2秒一闪
    delay(1000);                      
    digitalWrite(LED_BUILTIN, LOW);    
    delay(1000);                      
    if (WiFi.smartConfigDone())//退出等待
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      for(i=0;i<3;i++)  //连接成功后指示灯快速连闪三下
      {
            digitalWrite(LED_BUILTIN, HIGH);  
            delay(150);                      
            digitalWrite(LED_BUILTIN, LOW);    
            delay(150);   
        }
      break;
    }
  }
}
void setup() 
{
  Serial.begin(115200);
  delay(100);  //开机后等待100ms
  pinMode(LED_BUILTIN, OUTPUT);


}


void loop() 
{
  
  //主程序1：连接网络、断网重连
  while(WiFi.status()!=WL_CONNECTED)
  {
      Serial.println("\r\n正在尝试重新连接");
      Serial.print(".");
      digitalWrite(LED_BUILTIN, HIGH);   //重新连接过程中每1秒一闪
      delay(500);                       
      digitalWrite(LED_BUILTIN, LOW);    
      delay(500);                 
      count++;
      
      if(count>=10) //10s后仍未连接成功
      {
          Serial.println("WiFi连接自动失败，请用手机重新进行配网"); 
          smartConfig();  //调用WiFi配置程序
          Serial.println("连接成功");  
          Serial.print("IP:");
          Serial.println(WiFi.localIP());
          count=0;
      }
   }

   //主程序1结束


}
