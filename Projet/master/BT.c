#include <includes_msp.c>
#include <msp430.h>
#include <string.h>

/*
 

// Display basic settings.  Address, Name, UART Settings, Security, Pin code, Bonding, Remote Address.
void bt_display_settings()
{
    uart_send_words("D");
}

 

// Temporary UART Change,  will change the serial parameters immediately, but not store them.
// Command will return “AOK” at current settings, then automatically exit command mode, and switch to new baudrate.
void bt_change_baud_rate(uint8_t baud_rate)
{
    uart_send_words("U,");
    uart_send_byte(baud_rate);
    uart_send_words(",<E,O,N>");
}

 

// Connectand immediately go into FAST data modeusing the LAST address found from the Inquirycommand.
// NOTE: you will not be able to enter command mode while connected.
// PIO6 can still be used to disconnect.
void bt_connect_fast_data_mode()
{
    uart_send_words("CFI");
}

 

// Attempt to connect tothe REMOTE stored address.
void bt_connect_device_from_list()
{
    uart_send_words("C");
}

 

// Connect to the address specified in hex format.
// The address is also stored as the REMOTE address.
void bt_connect_device(size_t device_address)
{
    uart_send_words("C,<");
    uart_send_words(device_address);
    uart_send_words(">");
}

 

// Go into fastdata mode, ends configuration immediately.
void bt_fast_data_mode()
{
    uart_send_words("F,1");
}

 

// Enter command mode Characters are passed as data until this exact sequence is seen.
// If any bytes are seen before or after the $$$ characters in a 1 second window, command mode will not be entered and these bytes will be passed on to other side.
// NOTE: The device will only enter command mode if it is within the configuration timer window (60 seconds from power up by default).
// In master mode the configuration timer is set to zero.
// The character string to enter command mode is configurable using the S$command
// Use---exit command mode. Exit command mode.
// “END” will be displayed.
void bt_enter_command_mode()
{
    uart_send_words("$$$");
}

 

// Exit command mode
void bt_exit_command_mode()
{
    uart_send_words("---");
}

 

// Forces a complete reboot of the device (similar to a power cycle).
void bt_reboot()
{
    uart_send_words("R,1");
}

 

// Causes device to be non-discoverable and non-connectable (temporarily).
// Does not survive a power cycle or reset.
// Used with the Z command below.
// Use the “W” command to re-enable.
// This command will return “Quiet” as a response.
void bt_disable_connexion()
{
    uart_send_words("Q");
}

 

// Re-enables discovery and connection.  This command reloads the stored value of the Inquiry and Page Window to re-enable.
// For example, to turn off Discovery but still allow connections,  send an “SI,0000”command, and follow it with “W”command.
// This command returns “Wake” as a response.
void bt_re_enable_connexion()
{
    uart_send_words("W");
}

 

// Kill (disconnect) from the current connection.
// The characters KILL<cr><lf> will be echoed to the local UART once the connection is broken.
void kill_connexion()
{
    uart_send_words("K");
}*/
