Open Arduino IDE, open the "Project" drop-down menu -> "Load Library" -> "Add a .ZIP ibrary".Select the EPD1IN54.zip library file to add.Open the "File" drop-down menu -> "Examples", select the epd1in54-demo routine, compile and upload to the development board.

Wiring:

UNO/2560---------------------------E-PAPER
  D7--------------------------------Busy
  D8--------------------------------Reset
  D9--------------------------------D/C
  D10-------------------------------CS
  D11-------------------------------SDI
  D13-------------------------------SCLK
  VCC-------------------------------VCC
  GND------------------------------GND

According to the supply voltage, set the toggle switch to the correct gear. The SPI interface uses a 4-wire interface by default.You can find the required function from the library folder keywords.txt document.