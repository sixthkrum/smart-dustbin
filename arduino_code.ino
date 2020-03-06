int led_pin_ir1 = 2;
int led_pin_ir2 = 3;
int led_pin_ir3 = 4;
int ir_pin1 = 5;
int ir_pin2 = 6;
int ir_pin3 = 7;
int rx_pin = 19;
int tx_pin = 18;
int baud_rate = 19200;
bool preferred_state_irpin1 = 0;
bool preferred_state_irpin2 = 0;
bool preferred_state_irpin3 = 0;
bool persistence_check_status_irpin1 = 0;
bool persistence_check_status_irpin2 = 0;
bool persistence_check_status_irpin3 = 0;
std::string target_mobile_number = "+91xxxxxxxxxx";
int last_status_code = 0;
int cycle_number = 0;

void setup()

{

Serial1.begin(baud_rate);
delay(5000);
pinMode(led_pin_ir1 , OUTPUT);
pinMode(led_pin_ir2 , OUTPUT);
pinMode(led_pin_ir3 , OUTPUT);
pinMode(ir_pin1 , INPUT);
pinMode(ir_pin2 , INPUT);
pinMode(ir_pin3 , INPUT);

}

int persistent_digitalRead(int pin_num , bool &persistence_check_status , bool preferred_state)

{

if(persistence_check_status == 1)

{

return preferred_state;

}

int i;

for(i = 0 ; i < 60 ; i ++)

{

if(digitalRead(pin_num) != preferred_state)

{

persistence_check_status = 0;
return !preferred_state;

}

delay(500);

}

persistence_check_status = 1;
return preferred_state;

}

int ir_status()

{

int count_high_irpins = !persistent_digitalRead(ir_pin1 , preferred_state_irpin1 , persistence_check_status_irpin1) + !persistent_digitalRead(ir_pin2 , preferred_state_irpin2 , persistence_check_status_irpin2) +
!persistent_digitalRead(ir_pin3 , preferred_state_irpin3 , persistence_check_status_irpin3);

return count_high_irpins;

}

void led_response()

{

digitalWrite(led_pin_ir1 , digitalRead(ir_pin1));
digitalWrite(led_pin_ir2 , digitalRead(ir_pin2));
digitalWrite(led_pin_ir3 , digitalRead(ir_pin3));

}

int send_sms(std::string msg)

{

char rx_capture;
Serial1.println("AT+CMGF=1\r");
delay(3000);

while(Serial1.available())

{

rx_capture = Serial1.read();

}

Serial1.println("AT+CSCS=\"GSM\"\r");
delay(3000);

while(Serial1.available())

{

rx_capture = Serial1.read();

}

Serial1.println("AT+CMGS=\"+918699721065\"\r");
delay(3000);

while(Serial1.available())

{

rx_capture = Serial1.read();

}

Serial1.println(msg + "\r");
delay(3000);

while(Serial1.available())

{

rx_capture = Serial1.read();

}

Serial1.write((char)26);
delay(3000);

while(Serial1.available())

{

rx_capture = Serial1.read();

}

}

void loop()

{

int count_high_irpins;
count_high_irpins = ir_status();
led_response();

switch (count_high_irpins)

{

case 1:

if(last_status_code != 1)

{

send_sms("Dustbin is 50% full");

}

last_status_code = 1;

break;


case 2:

if(last_status_code != 2)

{

send_sms("Dustbin is 75% full");

}

last_status_code = 2;

break;


case 3:

if(last_status_code != 3)

{

send_sms("Dustbin is 95% full");

}

last_status_code = 3;

break;


default:

break;

}

cycle_number++;

if(cycle_number == 10)

{

cycle_number = 0;
persistence_check_status_irpin1 = 0;
persistence_check_status_irpin2 = 0;
persistence_check_status_irpin3 = 0;

}
}
