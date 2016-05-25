//
//
//    Copyright 2013 Vencislav Atanasov
//
//
//    This file is part of libavr.
//
//    libavr is free software: you can redistribute it and/or
//    modify it under the terms of the GNU General Public License as
//    published by the Free Software Foundation, either version 2 of the
//    License, or (at your option) any later version.
//
//    libavr is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//    See the GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with libavr. If not, see <http://www.gnu.org/licenses/>.
//
//

#ifndef ONEWIRE_CONF_H_
#define ONEWIRE_CONF_H_

/*******************************************/
/* Hardware connection                     */
/*******************************************/

/* Define OW_ONE_BUS if only one 1-Wire-Bus is used
 in the application -> shorter code.
 If not defined make sure to call ow_set_bus() before using
 a bus. Runtime bus-select increases code size by around 300
 bytes so use OW_ONE_BUS if possible */
#define OW_ONE_BUS

#ifdef OW_ONE_BUS

#define OW_PORT D
#define OW_PIN 6
#define OW_CONF_DELAYOFFSET 0

#else
#if ( F_CPU < 1843200 )
#warning | Experimental multi-bus-mode is not tested for
#warning | frequencies below 1,84MHz. Use OW_ONE_WIRE or
#warning | faster clock-source (i.e. internal 2MHz R/C-Osc.).
#endif
#define OW_CONF_CYCLESPERACCESS 13
#define OW_CONF_DELAYOFFSET ( (uint16_t)( ((OW_CONF_CYCLESPERACCESS) * 1000000L) / F_CPU ) )
#endif

#endif /* ONEWIRE_CONF_H_ */
