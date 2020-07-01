
#include "HX711.h"
#include <Time.h>

#define DOUT  3
#define CLK  2

HX711 scale;

float calibration_factor = -50050; //-7050 worked for my 440lb max scale setup
int idContenedor = 1;
int direccionCll = 100; int direccionKr = 1;

void setup() {
  Serial.begin(9600);
  //Serial.println("HX711 calibration sketch");
  //Serial.println("Remove all weight from scale");
  //Serial.println("After readings begin, place known weight on scale");
  //Serial.println("Press + or a to increase calibration factor");
  //Serial.println("Press - or z to decrease calibration factor");
  setTime(21,35,00,40,06,2020);

  scale.begin(DOUT, CLK);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  //Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  //Serial.println(zero_factor);
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  if (idContenedor < 100)
    {idContenedor++; direccionCll--; direccionKr++;}
  else if (idContenedor == 100)
    {idContenedor = 1; direccionKr = 1; direccionCll = 100;}
  String idC = String(idContenedor); 
  

             
  String Cll = String(direccionCll); String Kr = String(direccionKr);
  int  peso = scale.get_units()*100;
  double pesoo = peso;

  time_t t = now();
  Serial.print("{");
  if (idContenedor >= 10 && idContenedor <100)
  {Serial.print("0"+idC);}
  else if (idContenedor < 10)
  {Serial.print("00"+idC);}
  else if (idContenedor == 100)
  {Serial.print(idC);}
  Serial.print(", ");
  if(pesoo*0.0144 != 0)
  {/*Serial.print(String(abs(pesoo*0.0144)));*/
  if(pesoo*0.0144 < 10)
  {Serial.print("00"+String(abs(pesoo*0.0144)));}
  else if (pesoo*0.0144 >= 10 && pesoo*0.0144 < 100)
  {Serial.print("0"+String(abs(pesoo*0.0144)));}
  else if (pesoo*0.0144 >= 100)
  {Serial.print(String(abs(pesoo*0.0144)));}}
  else if (pesoo*0.0144 == 0)
  {Serial.print("0.0000");}
  
  Serial.print(", ");
  Serial.print(String(day(t)));
  Serial.print(+ "/") ;
  Serial.print(String(month(t)));
  Serial.print(+ "/") ;
  Serial.print(String(year(t))); 
  Serial.print( ", ") ;
  Serial.print(String(hour(t)));  
  Serial.print(+ ":") ;
  Serial.print(String(minute(t)));
  Serial.print(":") ;
  if(second(t) >= 10)
  {Serial.print(String(second(t)));}
  else if (second(t) <10)
  {Serial.print("0"+String(second(t)));}
  Serial.print(", "); 
  Serial.print("Cll:");
  if (direccionCll < 10)
  {Serial.print("00"+Cll);}                 //+"-Kr:"+Kr);
  else if (direccionCll >= 10 && direccionCll !=100)
  {Serial.print("0"+Cll);}
  else if (direccionCll == 100)
  {Serial.print(Cll);}
  Serial.print("-Kr");
  if(direccionKr < 10)
  {Serial.print("00"+Kr);}
  else if(direccionKr >= 10 && direccionKr != 100)
  {Serial.print("0"+Kr);}
  else if(direccionKr == 100)
  {Serial.print(Kr);}
  Serial.println("}");
  delay(2000);
 
  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 10;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 10;
  }
}
