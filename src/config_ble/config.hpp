#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <Arduino.h>
#include <HardwareSerial.h>

// uart 1 : rx,tx
HardwareSerial debug(PA10,PA9);
// uart 2 : rx,tx 
HardwareSerial port_ble(PA3,PA2);
/**
* Param:(2 to 8)
    2：2400
    3：4800
    4：9600
    5：19200
    6：38400
    7：57600
    8：115200
*/
#define MODE_BAUD                   8
#define BAUD_DEFAULT_OF_JDY         9600

#define BAUD_BLE                    115200
#define BAUD_NORMAL                 115200
/**
* mode kết nối nhập mật khẩu BLE 
* return 0: kết nối không dùng password
* 1: nhập mật khẩu mỗi khi kết nối ble
* 2: nhập mật khẩu một lần cho lần đầu kết nối 
*/
#define MODE_TYPE                   0

#define COMMAND_AT                  "AT\r\n"
#define COMMAND_AT_NAME             "AT+NAME\r\n"
#define COMMAND_AT_TYPE             "AT+TYPE\r\n"
#define COMMAND_AT_BAUD             "AT+BAUD\r\n"
#define COMMAND_AT_RESET            "AT+RESET\r\n"

#define SET_TYPE_BLE(x)             "AT+TYPE"#x"\r\n"
#define SET_BAUD                    "AT+BAUD8\r\n"

class Config_ble
{
public:
    Config_ble(int baudBle = BAUD_DEFAULT_OF_JDY, int baudNormal = BAUD_NORMAL) 
                : _baudBle(baudBle), _baudNormal(baudNormal)
                {}
    void setNameBLE(const char *names);
    void resetBLE();
    void setBaud();
    void setType();
    void configBLE(const char *names);
    void initBLE();
    void sendJDY_24M(char *message);
    void receiveDataFromJDY();
    
private:
    int _data;
    int _baudBle;
    int _baudNormal;
    bool updateBaud=false;
    // 115200
    int baud_ble = 115200;
};



void Config_ble::initBLE()
{
    debug.begin(this->_baudNormal);
    port_ble.begin(this->_baudBle);
    debug.printf("hello world\n");
    // PIN EN MODULE BLE 
    pinMode(PA8,OUTPUT);
    digitalWrite(PA8,0);
    HAL_Delay(500);
    // ban đầu baud sẽ mặc định là 9600 
    this->sendJDY_24M(COMMAND_AT);HAL_Delay(100);
    this->sendJDY_24M(COMMAND_AT_NAME);HAL_Delay(100);
    this->sendJDY_24M(COMMAND_AT_TYPE);HAL_Delay(100);
    this->sendJDY_24M(COMMAND_AT_BAUD);HAL_Delay(100);

}
void Config_ble::sendJDY_24M(char *message)
{
    // xem những lệnh được gửi đến jdy-24m 
    //debug.printf("debug message:= [%s]\n",message);
    port_ble.printf("%s",message);
}
void Config_ble::resetBLE()
{
    this->sendJDY_24M(COMMAND_AT_RESET); HAL_Delay(100);
}

void Config_ble::setBaud()
{
    this->sendJDY_24M(SET_BAUD); HAL_Delay(100);
    this->updateBaud = true;
}

void Config_ble::setType()
{
    this->sendJDY_24M(SET_TYPE_BLE(MODE_TYPE));
}
void Config_ble::setNameBLE(const char *names)
{
    port_ble.printf("AT+NAME%s\r\n",names); HAL_Delay(100);
    //this->sendJDY_24M(SET_NAME_BLE(names)); HAL_Delay(100);
}
void Config_ble::receiveDataFromJDY()
{
    while(port_ble.available())
    {
        this->_data = port_ble.read();
        debug.write(this->_data);
    }

}

void Config_ble::configBLE(const char *names)
{
    this->setNameBLE(names);HAL_Delay(100);
    this->setBaud();HAL_Delay(500);
    this->setType();HAL_Delay(100);
    this->resetBLE();HAL_Delay(500);
    // update baud lên 115200 
    if(this->updateBaud)
    {
        //port_ble.end();
        port_ble.begin(this->baud_ble);
        // update names and baud khi tốc độ baudrate đã được config thành 115200
        this->setNameBLE(names);HAL_Delay(100);
        this->resetBLE();HAL_Delay(500);
        this->sendJDY_24M(COMMAND_AT);HAL_Delay(100);
        this->sendJDY_24M(COMMAND_AT_NAME);HAL_Delay(100);
        this->sendJDY_24M(COMMAND_AT_TYPE);HAL_Delay(100);
        this->sendJDY_24M(COMMAND_AT_BAUD);HAL_Delay(100);
    }

}



#endif