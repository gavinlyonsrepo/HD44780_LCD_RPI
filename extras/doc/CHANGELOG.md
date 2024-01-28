# Changelog

* version 1.0.0 Feb 2022
	* first release.
* Version 1.1 March 2022
	* added print class for numerical Data
* Version 1.2 April 2022
	* Added support for 20x04 after part arrived and tested
* Version 1.3 Jan 2023
	* Added Example file for 20x04 LCD
	* Added Example file for clock demo see issue one on github
	* Added ability to change I2C bus speed in the constructor.
* Version 1.3.1 April 2023
	* Updates to match Arduino source port 1.3.0
* Version 1.3.2 Dec 2023 
	* Refactored I2C control to give user greater control
	and visibility over bus
	* Added version number and more debug messages. 
	* Minor various optimizations
	* Changed Makefile so it will compile on 64 bit OS 
	* Added Hello world example
* Version 1.3.3 Jan 2024
	* Minor update
	* Changed default I2C speed to deal with latest bcm2835 release v1.73 problems: Intermittent 
	timeout errors on 100K, 
	* Added user ability to set I2C error timeout and number of retries attempts.
