#include "updateRTCbyNetworkTime.hpp"

bool gotNetworkTime = false;
bool showRTCTime = true;
bool setRTCTime = false;

void updateRTCbyNetworkTime()
{
  if (!gotNetworkTime)
  {
    for (;;)
    {
      getNetworkTime();

      if (NETWORK_TIME_GLOBAL != false)
      {
        Serial.println("got time from Modem");
        gotNetworkTime = true;
        // goto label;
        delay(1000);
        break;
      }
    }
  }

  if (!setRTCTime)
  {
    adjustRTC_Time(NETWORK_HOUR, NETWORK_MINUTE, NETWORK_SECOND, NETWORK_YEAR, NETWORK_MONTH, NETWORK_DAY);
    setRTCTime = true;
  }

  if (setRTCTime != false && gotNetworkTime != false && showRTCTime != false)
  {
    delay(100);
    getTimeRTC();
    delay(1000);
  }
}
