===============
DAC RTOS driver
===============

A DAC (Digital-to-Analog Converter) converts digital value to an analog voltage.
The digital value from 0 to the highest represent the output voltage value.
The digital highest value possible with the bit resolution supported by the
DAC. For example a 10 bit resolution DAC hardware, the highest value is 1024.
The highest output voltage possible with the reference voltage.

The convert functions of DAC RTOS driver are optimized for RTOS support.
That is, the convert functions will not work without RTOS, the convert
functions should only be called in a RTOS task or thread.

The driver can convert a serials digital value. The pre-defined data should
be an data array. Application can invoke dac_os_write to start the conversation.
the task/thread will blocked until conversation done.

During data conversation, the DAC convert process is not protected, so that a more
flexible way can be chosen in application.

Features
--------
* Initialization and de-initialization
* Enabling and disabling
* Support for DACs with channels
* Writing buffers with multiple samples to DAC

Applications
------------
* Generate audio signals by connecting the DAC output to a speaker
* Generate a reference voltage

Dependencies
------------
* DAC hardware with conversion done/buffer empty interrupt and error interrupt
* RTOS

Concurrency
-----------
N/A

Limitations
-----------
N/A

Knows issues and workarounds
----------------------------
N/A

