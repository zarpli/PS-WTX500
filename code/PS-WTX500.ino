#define PWM_ON_OFF          0x00
#define SRC_STATUS          0x01
#define IN_CONTROL          0x02
#define MASTER_VOL_CONTROL  0x10
#define GLOBAL_MUTE         0x19
#define CH_MUTE_CONTROL     0x1A
#define CH_MUTE_STATUS      0x1B
#define INPUT_CH_MAPPING1   0x32
#define INPUT_CH_MAPPING2   0x33
#define BM_CONFIG           0x64
#define OUTPUT_CH_MAPPING1  0x85
#define OUTPUT_CH_MAPPING2  0x86

#define MASTER_VOLUME_MAX   0x000C    // Gain +18 dB
#define MASTER_VOLUME_0dB   0x0030    // Gain 000 dB
#define MASTER_VOLUME_MIN   0x00BC    // Gain -70 dB

#define MUTE_ON             0x0001    // Default = 0x0001 (Mute)
#define MUTE_OFF            0x0000

#define CS      10
#define MOSI    11
#define MISO    12
#define SCK     13

#define PDN     8                     // ADC Power-Down mode Pin
#define P_ON    9                     // Control 3.3v, 5.0v and 30v power supply

#define TIME_DELAY    1               // 1 ms

void setup() {

pinMode(CS, OUTPUT);
pinMode(MOSI, OUTPUT);
pinMode(MISO, INPUT);
pinMode(SCK, OUTPUT);

pinMode(P_ON, OUTPUT);
pinMode(PDN, OUTPUT);

digitalWrite(CS,HIGH);  
digitalWrite(MOSI,LOW);  
digitalWrite(SCK,LOW);  

digitalWrite(P_ON, HIGH);           // Enable power supply
delay(500);
digitalWrite(PDN, HIGH);            // Power-Up ADC

Serial.begin(9600);

// Input channel Mapping
write_ap(INPUT_CH_MAPPING2, 0b0111011001000101);
 
// Internal processing channel linking to PWMs
write_ap(OUTPUT_CH_MAPPING1, 0b0100010100100011);
write_ap(OUTPUT_CH_MAPPING2, 0b0110011100000001);

// Bass Sum is sent to Subwoofer output
write_ap(BM_CONFIG, 0b0001010000110000);      

// Enable PWMs
// write_ap(PWM_ON_OFF, 0x0000);              // ALL PWM1-8 POWER ON
write_ap(PWM_ON_OFF, 0b1100101111100000);     // Only PWM3, PWM5 and PWM6 Power ON [SW, FR and FL] set 2.1 ch

// Set Master Volume
write_ap(MASTER_VOL_CONTROL, MASTER_VOLUME_0dB);

// Disable MUTE
write_ap(GLOBAL_MUTE, MUTE_OFF);
}

void loop() {
  read_ap(SRC_STATUS);
  Serial.println("");
  delay(500);
}


// read register in audio processor
uint8_t read_ap(uint8_t address)
{
  // chip select audio processor
  digitalWrite(CS,LOW);

  // read operation bit
  digitalWrite(SCK,LOW);  
  delay(TIME_DELAY);
  digitalWrite(MOSI,LOW);    // read bit
  delay(TIME_DELAY);  
  digitalWrite(SCK,HIGH);  
  delay(TIME_DELAY);  

  // set register address
  for (uint8_t bit = 0u; bit < 8u; bit++)
  {
  digitalWrite(SCK,LOW);  
  delay(TIME_DELAY);  
  digitalWrite(MOSI, ((address & (1<<7u-bit)) ? HIGH : LOW));
  delay(TIME_DELAY);  
  digitalWrite(SCK,HIGH);      
  delay(TIME_DELAY);  
  }

  // data read 16 bits
  for (uint8_t bit = 0u; bit < 16u; bit++)
  {
  digitalWrite(SCK,LOW);  
  delay(TIME_DELAY);  
  if(digitalRead(MISO) == HIGH) Serial.print("1");
  else Serial.print("0");
  digitalWrite(SCK,HIGH);  
  delay(TIME_DELAY);  
  }

  digitalWrite(SCK,LOW);  
  delay(TIME_DELAY);

  digitalWrite(CS,HIGH);
  
  Serial.println("");
}


// write register in audio processor
uint8_t write_ap(uint8_t address, uint16_t data)
{
  // chip select audio processor
  digitalWrite(CS,LOW);
  
  // write operation bit
  digitalWrite(SCK,LOW);  
  delay(TIME_DELAY);
  digitalWrite(MOSI,HIGH);    // write bit
  delay(TIME_DELAY);  
  digitalWrite(SCK,HIGH);  
  delay(TIME_DELAY);  

  // set register address
  for (uint8_t bit = 0u; bit < 8u; bit++)
  {
  digitalWrite(SCK,LOW);  
  delay(TIME_DELAY);  
  digitalWrite(MOSI, ((address & (1<<7u-bit)) ? HIGH : LOW));
  delay(TIME_DELAY);  
  digitalWrite(SCK,HIGH);      
  delay(TIME_DELAY);  
  }
  
  // data write 16 bits
  for (uint8_t bit = 0u; bit < 16u; bit++)
  {
  digitalWrite(SCK,LOW);  
  delay(TIME_DELAY);  
  digitalWrite(MOSI, ((data & (1<<15u-bit)) ? HIGH : LOW));
  digitalWrite(SCK,HIGH);  
  delay(TIME_DELAY);  
  }

  digitalWrite(SCK,LOW);  
  delay(TIME_DELAY);
  
  digitalWrite(CS,HIGH);
}
