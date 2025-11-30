# RSA Arduino Demo (Uno / ESP32)

This repository contains a small educational RSA demo for Arduino-compatible boards (Arduino Uno, ESP32). It demonstrates a trivial RSA encrypt/decrypt using small example keys. This is for learning only — keys and approach are insecure for real use.

Files of interest
- `RSA_Arduino.ino` — main sketch (calls `app_setup()` and `app_loop()`).
- `src/APP_Cfg.h` — central configuration. All tunable values are defined here (mode, baud, RSA keys, buffer sizes, prefixes, send interval).
- `src/hal/RSA/RSA.h`, `src/hal/RSA/RSA.cpp` — small RSA helper (modular exponentiation, byte-level encrypt/decrypt).
- `src/app/app.h`, `src/app/app.cpp` — application logic for SENDER / RECEIVER modes.

How it works
- Set `MODE_SENDER` in `src/APP_Cfg.h` to `1` for sender mode or `0` for receiver mode.
- Sender encrypts each plaintext byte independently and prints a line like:
  - `CIPHER 2790 2187 123 ...`
- Receiver reads a cipher line, decrypts each word and prints `DECRYPTED: <plaintext>`.

Important configuration (top of `src/APP_Cfg.h`)
- `MODE_SENDER` — 1 = sender, 0 = receiver
- `SERIAL_BAUD` — baud rate for `Serial`
- `RSA_N`, `RSA_E`, `RSA_D` — demo RSA parameters (n, e, d)
- `RSA_MESSAGE` — message the sender sends periodically
- `MAX_MSG_LEN` — buffer length for plaintext
- `MAX_CIPHER_WORDS` — buffer length for ciphertext words
- `SEND_INTERVAL_MS` — interval between sends when in sender mode
- `CIPHER_PREFIX`, `DECRYPTED_PREFIX` — serial text prefixes
 - `ENABLE_CRC` — when set to `1` the sender appends a CRC-8 byte to the plaintext before encryption; receiver verifies and prints CRC status. One plaintext byte is reserved for CRC when enabled.

Build & upload

Using the Arduino IDE
1. Open the `RSA_Arduino` folder in Arduino IDE (open the `RSA_Arduino.ino`).
2. Edit `src/APP_Cfg.h` to set `MODE_SENDER` for the device you are programming.
3. Select board (`Arduino Uno` or your `ESP32` board) and correct serial port.
4. Click `Upload`.

Using `arduino-cli`
1. Install `arduino-cli` (https://arduino.github.io/arduino-cli/latest/installation/).
2. Install board core if needed. Example for AVR Uno:
```bash
arduino-cli core update-index
arduino-cli core install arduino:avr
```
For ESP32 boards use the esp32 core from Espressif (follow esp32 core instructions).

3. Compile & upload (example for Uno):
```bash
cd /path/to/RSA_Arduino
arduino-cli compile --fqbn arduino:avr:uno .
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno .
```
Replace `/dev/ttyACM0` and the `--fqbn` value for your environment/board.

Testing
- Two-device test (recommended):
  - Program one board with `MODE_SENDER 1` and the other with `MODE_SENDER 0`.
  - Wire `TX` of sender -> `RX` of receiver and `TX` of receiver -> `RX` of sender. Connect grounds.
  - Open serial monitors at `SERIAL_BAUD` on both devices. Sender prints cipher lines; receiver prints `DECRYPTED:` lines.

- Single-board manual test:
  - Upload sender or receiver firmware to one board.
  - If you have sender firmware, copy a `CIPHER ...` line from monitor, paste into the monitor's input, and press Enter while the board runs receiver firmware.

CRC / Integrity check
- This demo can optionally append a CRC-8 (polynomial 0x07) byte to the plaintext before encryption. Enable by setting `ENABLE_CRC` to `1` in `src/APP_Cfg.h`.
- On send: the CRC byte is appended to the plaintext, then the whole buffer (plaintext+CRC) is encrypted per-byte and emitted as `CIPHER ...` words.
- On receive: after decryption the last byte is treated as the CRC; the receiver computes CRC over the payload and prints the decrypted payload followed by either `[CRC OK]` or `[CRC FAIL]`.
- Example receiver output with CRC enabled:
  - `DECRYPTED: HELLO [CRC OK]`

Buffer and behavior notes
- When `ENABLE_CRC` is `1`, one byte of `MAX_MSG_LEN` is reserved for the CRC. If you need to send messages of the previous full `MAX_MSG_LEN` length, increase `MAX_MSG_LEN` by 1 or disable `ENABLE_CRC`.
- The CRC is an educational integrity check and is not a substitute for authenticated encryption.

Notes & caveats
- This demo uses very small RSA values (n=3233). It's intentionally simple for pedagogy and cannot be used for secure communication.
- Each byte is encrypted independently; no padding or authenticated encryption.
- On Arduino Uno, `Serial` is the USB serial — using it for two-way device-to-device comms requires wiring or an extra USB-serial adapter.
