#include "app.h"
#include "../APP_Cfg.h"
#include "../hal/RSA/RSA.h"
#include <Arduino.h>

static unsigned long lastSend = 0;

void printCipherWords(const uint16_t* words, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        Serial.print(words[i]);
        if (i + 1 < len) Serial.print(' ');
    }
    Serial.println();
}

void sendMessage(const char* msg) {
    uint8_t in[MAX_MSG_LEN];
    uint16_t out[MAX_CIPHER_WORDS];
    size_t len = strlen(msg);
    if (len > MAX_MSG_LEN) len = MAX_MSG_LEN;
    memcpy(in, msg, len);
    size_t out_len = rsa_encrypt_bytes(in, len, out, RSA_E, RSA_N);
    Serial.print(CIPHER_PREFIX);
    Serial.print(' ');
    printCipherWords(out, out_len);
}

void handleReceivedLine(String line) {
    line.trim();
    if (line.length() == 0) return;
    if (line.startsWith(CIPHER_PREFIX)) {
        int spaceIndex = line.indexOf(' ');
        if (spaceIndex >= 0) line = line.substring(spaceIndex + 1);
    }
    uint16_t ciphers[MAX_CIPHER_WORDS];
    size_t count = 0;
    char buf[line.length() + 1];
    line.toCharArray(buf, sizeof(buf));
    char* tok = strtok(buf, " \t,;");
    while (tok && count < MAX_CIPHER_WORDS) {
        long v = atol(tok);
        if (v >= 0) ciphers[count++] = (uint16_t)v;
        tok = strtok(NULL, " \t,;");
    }
    if (count == 0) return;
    uint8_t out[MAX_MSG_LEN];
    size_t got = rsa_decrypt_bytes(ciphers, count, out, RSA_D, RSA_N);
    String outStr = "";
    for (size_t i = 0; i < got; ++i) {
        outStr += (char)out[i];
    }
    Serial.print(DECRYPTED_PREFIX);
    Serial.println(outStr);
}

void app_setup() {
    Serial.begin(SERIAL_BAUD);
    delay(100);
#if MODE_SENDER
    Serial.println("Mode: SENDER");
    Serial.print("Using RSA n="); Serial.print(RSA_N);
    Serial.print(" e="); Serial.println(RSA_E);
#else
    Serial.println("Mode: RECEIVER");
    Serial.print("Using RSA n="); Serial.print(RSA_N);
    Serial.print(" d="); Serial.println(RSA_D);
#endif
}

void app_loop() {
#if MODE_SENDER
    if (millis() - lastSend >= SEND_INTERVAL_MS) {
        lastSend = millis();
        sendMessage(RSA_MESSAGE);
    }
#else
    if (Serial.available()) {
        String line = Serial.readStringUntil('\n');
        handleReceivedLine(line);
    }
#endif
}
