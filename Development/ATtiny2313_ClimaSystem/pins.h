/*
 ============================================================================
 Name        : pins.h
 Author      : Stefan Petkov
 Version     :
 Copyright   :
 Description : pins definition
 ============================================================================
 */

#ifndef PINS_H_
#define PINS_H_

#define CONCATENATE(x, y) x ## y

#define PORT(x) CONCATENATE(PORT, x)
#define PIN(x) CONCATENATE(PIN, x)
#define DDR(x) CONCATENATE(DDR, x)

#define D_(x, y) CONCATENATE(D, x ## y)
#define D(x, y) D_(x, y)

#define P_(x, y) CONCATENATE(P, x ## y)
#define P(x, y) P_(x, y)

#endif /* PINS_H_ */
