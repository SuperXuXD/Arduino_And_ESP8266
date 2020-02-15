#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET   //小爱同学

#include <Blinker.h>

char auth[] = "*******";
char ssid[] = "*******";
char pswd[] = "*******";

//定义按钮数据
BlinkerButton Button1("turn_on");     //只开
BlinkerButton Button2("turn_off");    //只关
BlinkerButton Button3("on_off");      //可开可关
BlinkerButton Button4("change");      //按一次切换一次开关状态

bool oState = false;   

void miotPowerState(const String & state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON) {     //小爱同学控制开命令
        digitalWrite(0, !digitalRead(0));     
        
        BlinkerMIOT.powerState("on");

        BlinkerMIOT.print();

        oState = true;
    }
    else if (state == BLINKER_CMD_OFF) {   //小爱同学控制关命令
        digitalWrite(0,!digitalRead(0));

        BlinkerMIOT.powerState("off");

        BlinkerMIOT.print();

        oState = false;
    }
}

void miotQuery(int32_t queryCode)      //小爱设备查询接口：当小爱同学向设备发起控制, 设备端需要有对应控制处理函数
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_ALL_NUMBER :        //Blinker文档中缺失该部分，网络查询所得
            BLINKER_LOG("MIOT Query All");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
            BLINKER_LOG("MIOT Query Power State");
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.powerState(oState ? "on" : "off");
            BlinkerMIOT.print();
            break;
    }
}


void dataRead(const String & data)      // 如果未绑定的组件被触发，则会执行其中内容
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();
    
    uint32_t BlinkerTime = millis();
    
    Blinker.print("millis", BlinkerTime);
}


void button1_callback(const String & state)     //点灯app内控制按键触发低电平、开灯
{
    
    digitalWrite(0,LOW);
    BLINKER_LOG("get button state: ", state);


}
void button2_callback(const String & state)     //点灯app内控制按键触发高电平、关灯
{
    
    digitalWrite(0,HIGH);
    BLINKER_LOG("get button state: ", state);


}

void button3_callback(const String & state)     //点灯app内控制按键指定开关灯状态
{
    if(state=="on")
    {
      digitalWrite(0,LOW);
     }
    else if(state =="off")
    {
      digitalWrite(0,HIGH);
    }
    
    BLINKER_LOG("get button state: ", state);
    Blinker.print(state);

}

void button4_callback(const String & state)     //点灯app内控制按键触发开/关切换
{  

    digitalWrite(0,!digitalRead(0));         //开的时候关，关的时候开
    BLINKER_LOG("get button state: ", state);

}


void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);

    pinMode(0, OUTPUT);              //定义GPIO0口为输出
    digitalWrite(0, LOW);            //定义GPIO0默认为低电平，防止断网

    Blinker.begin(auth, ssid, pswd); //连接WiFi和设备上线
    Blinker.attachData(dataRead);    //如果没有被绑定的组件被触发就执行这个回调
    
    BlinkerMIOT.attachPowerState(miotPowerState);  //小爱开关
    BlinkerMIOT.attachQuery(miotQuery);            //小爱查询
    Button1.attach(button1_callback);  //Button1这个组件被触发就执行这个回调
    Button2.attach(button2_callback);  //Button2这个组件被触发就执行这个回调   
    Button3.attach(button3_callback);  //Button3这个组件被触发就执行这个回调 
    Button4.attach(button4_callback);  //Button4这个组件被触发就执行这个回调 
}

void loop()
{
    Blinker.run();
}
