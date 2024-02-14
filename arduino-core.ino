//Declare Serial Read variables
int CmdCount=1;
byte inData;
char inChar;
String BuildINString="";
String DisplayString="";
long DisplayValue;
String SentMessage="";
int ByteCount=0;
long A;
int B;
int WorkingVal;
String WorkingString="";
//Declare Buttons
int MenuID=0;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
void setup() 
{
    // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  Bootup();
  Retry:
  lcd.setCursor(0, 0);
  lcd.print("Connecting......    ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  Serial.begin(38400);
  delay(500);
  //Send a test message to see if the ELM327 Chip is responding
  SentMessage = "ATI";
  Serial.println("ATI");delay(500);ReadData();
  if (BuildINString.substring(1,7)=="ELM327")
    {
      lcd.setCursor(0, 0);
      lcd.print("Welcome...            ");
      lcd.setCursor(0, 1);
      lcd.print("Connection OK         ");
      delay(1500);
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Error             ");
      lcd.setCursor(0, 1);
      lcd.print("No Connection!         ");
      delay(1500);
      goto Retry;
    }
  delay(1500);
}

void loop() {
int x;
x = analogRead (0);
//Serial.println(x);
lcd.setCursor(10,1);
if (x > 800 and x < 820){lcd.print ("Select  ");}
if (x > 620 and x < 630){if (MenuID>0){MenuID--;}delay(250);}//Left
if (x > 400 and x < 415){lcd.print ("Down    ");}
if (x > 190 and x < 215){lcd.print ("Up      ");}
if (x > -10 and x < 10){if (MenuID<11){MenuID++;}delay(250);} //Right
  if (MenuID==1){lcd.setCursor(0, 0);lcd.print("02 IAT Temp        ");lcd.setCursor(0, 1);lcd.print(DisplayString);SentMessage = "01 0F";Serial.println("01 0F");delay(300);ReadData();}
  if (MenuID==2){lcd.setCursor(0, 0);lcd.print("03 Ambient Temp    ");lcd.setCursor(0, 1);lcd.print(DisplayString);SentMessage = "01 46";Serial.println("01 46");delay(300);ReadData();}
  if (MenuID==3){lcd.setCursor(0, 0);lcd.print("04 Throttle %      ");lcd.setCursor(0, 1);lcd.print(DisplayString);SentMessage = "01 11";Serial.println("01 11");delay(300);ReadData();}
  if (MenuID==4){lcd.setCursor(0, 0);lcd.print("05 CAT 1 Temp      ");lcd.setCursor(0, 1);lcd.print("Not Implemented ");}
  if (MenuID==5){lcd.setCursor(0, 0);lcd.print("06 CAT 2 Temp      ");lcd.setCursor(0, 1);lcd.print("Not Implemented ");}
  if (MenuID==6){lcd.setCursor(0, 0);lcd.print("07 CAT 3 Temp      ");lcd.setCursor(0, 1);lcd.print("Not Implemented ");}
  if (MenuID==7){lcd.setCursor(0, 0);lcd.print("08 CAT 4 Temp      ");lcd.setCursor(0, 1);lcd.print("Not Implemented ");}
  if (MenuID==8){lcd.setCursor(0, 0);lcd.print("09 RPM             ");lcd.setCursor(0, 1);lcd.print(DisplayString);SentMessage = "01 0C";Serial.println("01 0C");delay(300);ReadData();}
  if (MenuID==9){lcd.setCursor(0, 0);lcd.print("10 Vehicle Speed   ");lcd.setCursor(0, 1);lcd.print(DisplayString);SentMessage = "01 0D";Serial.println("01 0D");delay(300);ReadData();}
  if (MenuID==10){lcd.setCursor(0, 0);lcd.print("11 Air Flow Rate  ");lcd.setCursor(0, 1);lcd.print(DisplayString);SentMessage = "01 10";Serial.println("01 10");delay(300);ReadData();}
  if (MenuID==11){lcd.setCursor(0, 0);lcd.print("12 Barometric     ");lcd.setCursor(0, 1);lcd.print(DisplayString);SentMessage = "01 33";Serial.println("01 33");delay(300);ReadData();}
}
//Read Data and act accordingly
void ReadData()
{
  BuildINString="";  
  while(Serial.available() > 0)
  {
    inData=0;
    inChar=0;
    inData = Serial.read();
    inChar=char(inData);
    BuildINString = BuildINString + inChar;
  }
  
  //if(BuildINString!=""){Serial.print(BuildINString);}
  BuildINString.replace(SentMessage,"");
  BuildINString.replace(">","");
  BuildINString.replace("OK","");
  BuildINString.replace("STOPPED","");
  BuildINString.replace("SEARCHING","");
  BuildINString.replace("NO DATA","");
  BuildINString.replace("?","");
  BuildINString.replace(",","");
  //Serial.print(BuildINString);
  
  //Check which OBD Command was sent and calculate VALUE 
  //Calculate RPM I.E Returned bytes wil be 41 0C 1B E0 
  if (SentMessage=="01 0C")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
    WorkingString = BuildINString.substring(11,13);
    B = strtol(WorkingString.c_str(),NULL,0);
   
   DisplayValue = ((A * 256)+B)/4;
   DisplayString = String(DisplayValue) + " rpm          ";
   lcd.setCursor(0, 1);
   lcd.print(DisplayString);
  }
  //Calculate Vehicle speed I.E Returned bytes wil be 41 0C 1B E0 
  if (SentMessage=="01 0D")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
   DisplayValue = A;
   DisplayString = String(DisplayValue) + " km/h          ";
   lcd.setCursor(0, 1);
   lcd.print(DisplayString); 
  }
  
  //Coolant Temp 
  if (SentMessage=="01 05")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
   DisplayValue = A;
   DisplayString = String(DisplayValue) + " C            ";
   lcd.setCursor(0, 1);
   lcd.print(DisplayString); 
  }
  
    //IAT Temp 
  if (SentMessage=="01 0F")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
   DisplayValue = A;
   DisplayString = String(DisplayValue) + " C            ";
   lcd.setCursor(0, 1);
   lcd.print(DisplayString); 
  }
  
  //Air flow Rate
  if (SentMessage=="01 10")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
    WorkingString = BuildINString.substring(11,13);
    B = strtol(WorkingString.c_str(),NULL,0);
   
   DisplayValue = ((A * 256)+B)/100;
   DisplayString = String(DisplayValue) + " g/s          ";
   lcd.setCursor(0, 1);
   lcd.print(DisplayString); 
  }
  
  //Ambient Temp 
  if (SentMessage=="01 46")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
   DisplayValue = A;
   DisplayString = String(DisplayValue) + " C            ";
   lcd.setCursor(0, 1);
   lcd.print(DisplayString); 
  }
  
  //Throttle position 
  if (SentMessage=="01 11")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
   DisplayValue = A;
   DisplayString = String(DisplayValue) + " %            ";
   lcd.setCursor(0, 1);
   lcd.print(DisplayString); 
  }
  //Barometric pressure
  if (SentMessage=="01 33")
  {
    WorkingString = BuildINString.substring(7,9);
    A = strtol(WorkingString.c_str(),NULL,0);
   DisplayValue = A;
   DisplayString = String(DisplayValue) + " kpa            ";
   lcd.setCursor(0, 1);
   lcd.print(DisplayString); 
  }  
}
void Bootup()
{
lcd.print("WisperChip V2.00 ");
     for (int i=0; i <= 5; i++)
     {
       for (int j=1; j <= 4; j++)// -/|\-
       {
        if(j==1){lcd.setCursor(0, 1);lcd.print ("-");delay(200);} 
        if(j==2){lcd.setCursor(0, 1);lcd.print ("/");delay(200);}
        if(j==3){lcd.setCursor(0, 1);lcd.print ("|");delay(200);} 
        if(j==4){lcd.setCursor(0, 1);lcd.print ("\\");delay(200);} 
       }   
     }
     
delay(1000);  
}