#pragma once

#include <dns_sd.h>
#include <iostream>

#define BONJOUR_SERVICE_NAME "Quantum TopTracking"
#define BONJOUR_SERVICE_TYPE "_quantum-toptracking._tcp"

class BonjourRegistrationException : public std::exception {
public:
    BonjourRegistrationException() {};
    virtual const char *what() const throw() {
        return "Bonjour registration failed.";
    }
};

class BonjourRegistration {
private:
    DNSServiceRef client;
public:
    BonjourRegistration()
        : client(NULL)
    {};
    void registerService(uint16_t port);
};
