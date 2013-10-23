#include "BonjourRegistration.h"

#include <stdio.h>

typedef union { unsigned char b[2]; unsigned short NotAnInteger; } Opaque16;

void BonjourRegistration::registerService(uint16_t port) {
    Opaque16 registerPort = { { port >> 8, port & 0xFF } };
    DNSServiceErrorType error = DNSServiceRegister(&client, 0, 0, BONJOUR_SERVICE_NAME, BONJOUR_SERVICE_TYPE, NULL, NULL, registerPort.NotAnInteger, 0, NULL, NULL, NULL);
    if (error != kDNSServiceErr_NoError) {
        throw BonjourRegistrationException();
    }
}

