# PS-WTX500 brain transplant

Simple control of a Multi-channel Digital Audio Processor Pulsus PS9829B

The Samsung PS-WTX500 subwoofer is part of a 5.1 audio system (HT-TX500) that includes a receiver with DVD player.


<p align="center"><img src=/images/HT-TX500.jpeg></p>

The receiver does not have the sound amplifiers. The PS-WTX500 is who has six class D amplifiers included, but you cannot use them if you do not have the receiver connected. The receiver send the audio (analog) and signal control to PS-WTX500 to enable y config the procesing audio.

Medium and treble speakers (5 pieces) and the subwoofer are of acceptable quality. You could disconnect all the electronics and buy an amplifier and connect it to the speakers. But why not use the amplifiers already included in the subwoofer?

<p align="center"><img src=/images/DSC_0040.JPG></p>

# Hardware

The PS-WTX500 service manual and the PS9829B datasheet are not available to the public :(
But it is possible to find your pinout in some service manuals of other audio systems that use this audio processor.

<p align="center"><img src=/images/PS9829B_pinout.png width="500"></p>

Fortunately it was possible to find the datasheet of one of his brothers, the [Pulsus PS9818](datasheet/PS9818.pdf). Checking the pinout of PS9829B vs PS9818 there is only one difference at first sight: the PLL, which for our purposes does not matter.

In the tests, the reading registers of the DAP (PS9829B) was implemented and fortunately it was verified that the default values of the registers coincided with the datasheet of his brother PS9818.

Disassembly and with a little reverse engineering, you find that the signals that arrive from the receiver are analog, each of them reaches an operational amplifier [S4560](datasheet/S4560.pdf), for later be sampled by [AK5384](datasheet/AK5384.pdf) (4 ch) and [AK5381](datasheet/AK5381.pdf) (2 ch).

The ADCs deliver their work to the Pulsus PS9829B audio processor through the following connections:

|PS9829B | ADC        |
|--------|------------|
|MSDIN0  |AK5384 SDTO1|
|MSDIN1  |AK5384 SDTO2|
|MSDIN2  |AK5381 SDTO |

The mapping of signal inputs and physical connections is as follows:

|SDIN                       |   Input Channel linked |
|---------------------------|------------------------|
|Left channel from SDIN0    | Front Left             |
|Right channel from SDIN0   | Front Right            |
|Left channel from SDIN1    | Surround Left          |
|Right channel from SDIN1   | Surround Right         |
|Left channel from SDIN2    | LFE                    |
|Right channel from SDIN2   | Center                 |

The DAP can generate eight PWWs, but this equipment only use six and deliver them to six [TAS5152](datasheet/TAS5152.pdf) amplifiers from Texas Instruments.

The physical connections between the PWM's and the outputs to the speakers are as follows:

| PS9829B | SPEAKER |
|---------|---------|
| PWM1    | RR      |
| PWM2    | RL      |
| PWM3    | SW      |
| PWM4    | C       |
| PWM5    | FR      |
| PWM6    | FL      |

# SPI 9-bit

The PS9829B DAP is controlled by the Samsung [S3P70F4](datasheet/S3P70F4.pdf) microcontroller.

<p align="center"><img src=/images/DSC_0039.JPG></p>

So far everything normal, but sniffing the SPI flow between the uC and the DAP there is something weird.

The *SPI is 9 bits* (Bus Pirate goes trash ...) and other digital analyzers did not do a good job.

<p align="center"><img src=/images/SPI.png></p>

By tracking tracks and through-hole, the following connections of interest are obtained:

| S3C70F4       | Conection     | S3C70F4       | Conection     |
| ------------- | ------------- | ------------- | ------------- |
| Pin 01        | VSS           | Pin 17        | MISO          |
| Pin 02        |               | Pin 18        | Encoder       |
| Pin 03        |               | Pin 19        |               |
| Pin 04        |               | Pin 20        |               |
| Pin 05        |               | Pin 21        | P-ON          |
| Pin 06        |               | Pin 22        |               |
| Pin 07        |               | Pin 23        |               | 
| Pin 08        |               | Pin 24        |               |
| Pin 09        |               | Pin 25        | PDN           |
| Pin 10        |               | Pin 26        |               |
| Pin 11        |               | Pin 27        | CS            |
| Pin 12        |               | Pin 28        | SCK           |
| Pin 13        |               | Pin 29        | MOSI          |
| Pin 14        |               | Pin 30        |               |
| Pin 15        | Encoder       | Pin 31        |               |
| Pin 16        | EXT_MUTE      | Pin 32        | VDD           |

P-ON: enables 30V, 5V and 3.3V power
PDN: Reset of ADCs

The encoder is used to control the crossover.

# The new brain

The S3C70F4 is removed from the PCB to replace it with a [Pro Mini 328](https://www.sparkfun.com/products/11114) - 3.3V/8MHz version of Sparkfun, which thanks to the sky is not red color and apparently does not rust so fast.

**Most likely the PS9829B is 5V tolerant but until you have the datasheet ...**

<p align="center"><img src=/images/DSC_0001.JPG></p>

Brain transplantation looks like this with the new connections:

| S3C70F4 (footprint) | Pro Mini 328  | S3C70F4 (footprint) | Pro Mini 328  |
| -------------- | ------------- | -------------- | ------------- |
| Pin 01         | GND           | Pin 17         | 12            |
| Pin 02         |               | Pin 18         | 7             |
| Pin 03         |               | Pin 19         |               |
| Pin 04         |               | Pin 20         |               |
| Pin 05         |               | Pin 21         | 9             |
| Pin 06         |               | Pin 22         |               |
| Pin 07         |               | Pin 23         |               | 
| Pin 08         |               | Pin 24         |               |
| Pin 09         |               | Pin 25         | 8             |
| Pin 10         |               | Pin 26         |               |
| Pin 11         |               | Pin 27         | 10            |
| Pin 12         |               | Pin 28         | 13            |
| Pin 13         |               | Pin 29         | 11            |
| Pin 14         |               | Pin 30         |               |
| Pin 15         | 6             | Pin 31         |               |
| Pin 16         |               | Pin 32         | RAW           |

<p align="center"><img src=/images/DSC_0005.JPG></p>

you can use only two speakers and the subwoofer, *the PULSUS can do the sum of bass and send it to the subwoffer*, it works great. 

You can also add an IR receiver :)

<img src=/images/DSC_0763.JPG>

# video

<a href="http://www.youtube.com/watch?feature=player_embedded&v=8eDIVF7A_ak" target="_blank"><img src="http://img.youtube.com/vi/8eDIVF7A_ak/0.jpg" 
alt="YouTube" width="400" border="10" /></a>
