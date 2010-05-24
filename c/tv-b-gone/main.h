struct codeElement {
  uint16_t onTime;   // duration of "On" time
  uint16_t offTime;  // duration of "Off" time
};

struct powercode {
  uint8_t timer_val; // not the actual frequency, but the timer value to generate the frequency
  struct codeElement codes[];  // flexible number of on/off codes
};
