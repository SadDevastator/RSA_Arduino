#ifndef APP_CFG_H
#define APP_CFG_H

// Select device mode: set to 1 for sender, 0 for receiver
#define MODE_SENDER 1

// Serial settings
#define SERIAL_BAUD 115200

// RSA demo keys (small example keys suitable for demonstration only)
// Classic example: p=61, q=53 -> n=3233, phi=3120, e=17, d=2753
#define RSA_N 3233u
#define RSA_E 17u
#define RSA_D 2753u

// Message to send (sender mode). Keep short to fit demo conversion.
#define RSA_MESSAGE "HELLO"

// Application-level tunables (move here so all values are centralized)
// Maximum plaintext message length (bytes)
#define MAX_MSG_LEN 64

// Maximum ciphertext words (each word is an encrypted byte or small chunk)
#define MAX_CIPHER_WORDS 128

// Interval between automatic sends in sender mode (milliseconds)
#define SEND_INTERVAL_MS 5000UL

// Serial text prefixes
#define CIPHER_PREFIX "CIPHER"
#define DECRYPTED_PREFIX "DECRYPTED: "

#endif // APP_CFG_H


