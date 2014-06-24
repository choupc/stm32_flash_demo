USART3
==============
Ready for USART3, so you can connect rs-232 to the discovery board.

[Usage]rs232.tx->PC11; rs232.rx->PC10
  
And completed printf function by usart3.(can't print float)

The printf source code from 《stm32庫開發實踐指南》.


main
==============
flash在執行初始化的過程中，printf會印出目前正在執行的初始化動作。例如erase sector,flash read/write等等。
LED燈會依不同執行動作而有不同的閃爍變化。
GREEN : FLASH is unlocked.
BLUE : FLASH is writing.
RED : FLASH is erasing.
YELLOW: FLASH is reading.


教學網站(ncku wiki)
==============
http://wiki.csie.ncku.edu.tw/embedded/Flash
