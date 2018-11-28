// ----------------------------------------------------------------------------
// [Title of project] for use with the [Lesson Plan Title] lesson plan 
// available from Microsoft Education Workshop at http://aka.ms/hackingSTEM 
// 
// This project uses an Arduino UNO microcontroller board. More information can
// be found by visiting the Arduino website: 
// https://www.arduino.cc/en/main/arduinoBoardUno 
//  
// [Several sentences describing code function in accessible verbiage] 
// 
// Comments, contributions, suggestions, bug reports, and feature requests 
// are welcome! For source code and bug reports see: 
// http://github.com/[TODO github path to Hacking STEM] 
// 
// Copyright [year], [your name] Microsoft EDU Workshop - HackingSTEM 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE. 
// ----------------------------------------------------------------------------

// Program variables ----------------------------------------------------------
int exampleVariable = 0;
int sensorPin = A0;

// Excel variables ------------------------------------------------------------
float incomingExcelFloat = 0;
int incomingExcelInteger = 0;
String incomingExcelString = "";

// Serial data variables ------------------------------------------------------
const byte kNumberOfChannelsFromExcel = 6; //Incoming Serial Data Array
// IMPORTANT: This must be equal to number of channels set in Data Streamer
String incomingSerialData[kNumberOfChannelsFromExcel];

const String kDelimiter = ",";    // Data Streamer expects a comma delimeter
//char inputString[64];          // String variable to hold incoming data
boolean stringComplete = false;   // Indicates complete string (newline found)
const int kSerialInterval = 50;   // Interval between serial writes
unsigned long serialPreviousTime; // Timestamp to track serial interval

//char arr[kNumberOfChannelsFromExcel][10];

char* arr[kNumberOfChannelsFromExcel];
// SETUP ----------------------------------------------------------------------
void setup() {
  // Initializations occur here
  Serial.begin(9600);  
}

// START OF MAIN LOOP --------------------------------------------------------- 
void loop()
{
  // Process sensors
  processSensors();
  delay(1000);

  // Read Excel variables from serial port (Data Streamer)
  processIncomingSerial();

  // Process and send data to Excel via serial port (Data Streamer)
  // processOutgoingSerial();

  if ( strcmp ("Apple", arr[0]) == 0){ // Compares STR1 to STR2 returns 0 if true.
      Serial.println("working");
  }
}


// SENSOR INPUT CODE-----------------------------------------------------------
void processSensors() 
{
  // Add sensor processing code here
  exampleVariable = analogRead( sensorPin );
}

// Add any specialized methods and processing code here

// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------
// Process serial data inputString from Data Streamer
void ParseSerialData()
{
  if (stringComplete) {     
    //Build an array of values from comma delimited string from Data Streamer
    //BuildDataArray(inputString);

    // Set vavariables based on array index referring to columns:
    // Data Out column A5 = 0, B5 = 1, C5 = 2, etc.
    incomingExcelFloat = incomingSerialData[0].toFloat(); // Data Out column A5
    incomingExcelInteger = incomingSerialData[1].toInt(); // Data Out column B5
    incomingExcelString = incomingSerialData[2]; // Data Out column C5

    //inputString = ""; // reset inputString
    stringComplete = false; // reset stringComplete flag
  }
}

// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------
void sendDataToSerial()
{
  // Send data out separated by a comma (kDelimiter)
  // Repeat next 2 lines of code for each variable sent:

  Serial.print(exampleVariable);
  Serial.print(kDelimiter);
  
  // Example test for incoming Excel variables
  Serial.print(incomingExcelFloat, 4); // second parameter = decimal places
  Serial.print(kDelimiter);
  
  Serial.print(incomingExcelInteger);
  Serial.print(kDelimiter);
  
  Serial.print(incomingExcelString);
  Serial.print(kDelimiter);
  
  Serial.println(); // Add final line ending character only once
}

//-----------------------------------------------------------------------------
// DO NOT EDIT ANYTHING BELOW THIS LINE
//-----------------------------------------------------------------------------

// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------
void processOutgoingSerial()
{
   // Enter into this only when serial interval has elapsed
  if((millis() - serialPreviousTime) > kSerialInterval) 
  {
    serialPreviousTime = millis(); // Reset serial interval timestamp
    sendDataToSerial(); 
  }
}

// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------
void processIncomingSerial()
{
  if(Serial.available()){
  GetSerialData();
  //parseData();
  }
  //ParseSerialData();
}

// Gathers bits from serial port to build inputString
void GetSerialData()
{
  char inputString[64] = {0};
  while (Serial.available()){
    
    Serial.readBytesUntil('\n', inputString, 64);
    
    //inputString = Serial.readStringUntil('\n');
    Serial.println("Serial Read Good");
    Serial.println(inputString);
    stringComplete =true;
  }
  parseData(inputString);
}

void parseData(char data[])
{
    char *token = strtok(data, ",");
    Serial.println(token);
    int j = 0;
    while (token != NULL){
        Serial.println(strlen(token));
        arr[j] = token;
      // for(int i = 0; i < strlen(token); i++){
      //   arr[j][i] = *token;
      //   //Serial.println(arr[j][i]);
      //   token++;
      // }
     // Serial.print(token);
     // Serial.print(" ");
      token = strtok(NULL, ",");
      Serial.print("Token in Array is ");
      Serial.println(arr[j]);
      j++;
    }
    // delete token;
    // delete arr;
}

// Takes the comma delimited string from Data Streamer
// and splits the fields into an indexed array
void BuildDataArray(String data)
{
  return ParseLine(data);
}

// Parses a single string of comma delimited values with line ending character
void ParseLine(String data) 
{
    int charIndex = 0; // Tracks the character we are looking at
    int arrayIndex = 0; // Tracks the array index to set values into
    while(arrayIndex < kNumberOfChannelsFromExcel) // Loop until full
    {
        String field = ParseNextField(data, charIndex);  // Parse next field
        incomingSerialData[arrayIndex] = field; // Add field to array
        arrayIndex++;   // Increment index
    }
}

// Parses the next value field in between the comma delimiters
String ParseNextField(String data, int &charIndex)
{
    if (charIndex >= data.length() )
    {
      return ""; //end of data
    }
    
    String field = "";
    bool hitDelimiter = false; // flag for delimiter detection 
    while (hitDelimiter == false) // loop characters until next delimiter
    {
        if (charIndex >= data.length() )
        {
          break; //end of data
        }

        if (String(data[charIndex]) == "\n") // if character is a line break
        {
          break; // end of data
        }
        
       if(String(data[charIndex]) == kDelimiter) // if we hit a delimiter
        {
          hitDelimiter = true;  // flag the delimiter hit
          charIndex++; // set iterator after delimiter so we skip next comma
          break;
        }
        else
        {        
          field += data[charIndex]; // add character to field string
          charIndex++; // increment to next character in data
        }
    }
    return field;
}