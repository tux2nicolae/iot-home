#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial GSM(7, 8); // RX, TX

String response = "";

void sendGSM(const char *msg, int timeout = 500)
{
  auto start = millis();

  GSM.println(msg);

  String str;
  while (1)
  {
    if (millis() - start >= timeout)
    {
      break;
    }

    while (GSM.available())
    {
      str += (char)GSM.read();
    }
  }

  str.trim();

  response = static_cast<String &&>(str);
  Serial.println(response);
}

void setup()
{
  GSM.begin(9600);
  Serial.begin(9600);

  sendGSM("ATE1");
  sendGSM("ATE1");

  sendGSM("AT+CMGF=1");
  sendGSM("AT+SAPBR=3,1,\"APN\",\"broadband\"");
  sendGSM("AT+SAPBR=1,1");
  sendGSM("AT+HTTPINIT");
  sendGSM("AT+HTTPPARA=\"CID\",1");
  sendGSM("AT+HTTPPARA=\"URL\",\"https://localhost:3000/gsm\"");

  // work();
}

void work()
{
  sendGSM("AT+HTTPACTION=1", 5000);
  const String contentLengthStr = response.substring(response.lastIndexOf(',') + 1);

  sendGSM(("AT+HTTPREAD=0," + contentLengthStr).c_str());

  int foundIndex = response.indexOf(':');
  foundIndex = response.indexOf('\n', foundIndex);
  response = response.substring(foundIndex, response.length() - 3);
  response.trim();

  DynamicJsonDocument jsonDocument(400);
  if (deserializeJson(jsonDocument, response) != DeserializationError::Ok)
  {
    Serial.println("Deserialization failed:" + response);
    return;
  }

  const String smsPhoneNumber = jsonDocument.containsKey(F("phoneNumber")) ? jsonDocument[F("phoneNumber")].as<String>() : String();
  const String smsText = jsonDocument.containsKey(F("text")) ? jsonDocument[F("text")].as<String>() : String();

  if (smsPhoneNumber.length() && smsText.length())
  {
    const String message = smsText + ((char)26);
    sendGSM(("AT+CMGS=\"" + smsPhoneNumber + "\"").c_str());
    Serial.println("---------------");
    sendGSM(message.c_str(), 4000);
  }
  else
  {
    Serial.println("No more messages... Waiting 10 seconds");
    delay(10000);
  }
}

void loop()
{
  // work();
}
