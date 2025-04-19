#include<LiquidCrystal.h> LiquidCrystal lcd(12,11,5,4,3,2); int led=6;
int led1=7; int relay1=8; int relay2=9;
float reading=0; float current=0; float power=0; float unit=0;
//int cur_sens=A0; String status1="X"; String status2="Z"; int vol_sens=A1; void setup()
{
Serial.begin(9600); pinMode(led,OUTPUT); pinMode(led1,OUTPUT); pinMode(relay1,OUTPUT); pinMode(relay2,OUTPUT);
//pinMode(cur_sens,INPUT);
//pinMode(vol_sens,INPUT); lcd.begin(16,2);
lcd.clear();


noInterrupts(); TCCR1A=0; TCCR1B=0; TCNT1=0;

OCR1A=31250;
TCCR1B |=(1 << WGM12); TCCR1B |=(1 << CS12); TIMSK1 |=(1 << OCIE1A);
interrupts();


} ISR(TIMER1_COMPA_vect)
{
digitalWrite(led1,digitalRead(led1)^1);


if(Serial.available()>0)
{
char rec_data=Serial.read(); if(rec_data=='A')
{
digitalWrite(relay1,HIGH); status1="W";
}
else if(rec_data=='B')
{
digitalWrite(relay1,LOW); status1="X";
}
else if(rec_data=='C')
{
digitalWrite(relay2,HIGH); status2="Y";
}
else if(rec_data=='D')
{
digitalWrite(relay2,LOW); status2="Z";

}
}
void loop()
{


lcd.clear();
int v=analogRead(A1); int vol_sens=(v/4)-25; lcd.setCursor(4,0); lcd.print("VOLTAGE"); lcd.setCursor(6,1);
lcd.print(String(vol_sens)+"V"); delay(3000); current=analogRead(A0); current=current/240; if(current<0.2)
{
current=0;
}
lcd.clear(); lcd.setCursor(4,0); lcd.print("CURRENT"); lcd.setCursor(6,1);
lcd.print(String(current)+"A"); delay(3000); power=(vol_sens*current); if(power>10)
{
power=(vol_sens*current)-20;
}
else
{
power=(vol_sens*current);

lcd.clear(); lcd.setCursor(4,0); lcd.print("POWER"); lcd.setCursor(6,1);
lcd.print(String(power)+"W"); delay(3000); if(power>40&&power<70)
{
unit=unit+1;
}
if(power>70)
{
unit=unit+2;
}
lcd.clear();
lcd.setCursor(2,0); lcd.print("CONSUMED UNIT"); lcd.setCursor(4,1); lcd.print(String(unit)+"KWh"); delay(3000);
String values=status1+","+status2+","+String(unit); writestring(values);

}
void writestring(String stringData)
{
for(int i=0;i<stringData.length();i++)
{
Serial.write(stringData[i]);
}
}
