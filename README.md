# STM32_DataWatchpointTrigger

Diese Bibliothek stellt einem die grundlegenden Funktionen für den Data Watchdog Trigger (DWT) zur
Verfügung. Dabei ist zu beachten, dass nicht jeder STM32 einen DWT verbaut hat. Es ist somit
nicht möglich diese Bibliothek bei jedem Mikrocontroller anzuwenden.

Mit diesen Funktionen lassen sich Programminterne Timing messen, um nachzuvollziehen wie schnell
der Mikrocontroller den Code ausführt und ob etwas vom Timing her kritisch werden würde.