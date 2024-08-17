/* This is the MSP430 implementation.
 * This should work on any MSP430 with
 * at least 128 bytes of RAM.
 * As for ROM, 8-16 KiB should suffice.
 */

/* Not sure why the header doesn't
 * already disable these. */
#pragma diag_push
#pragma CHECK_MISRA("-2.2")
#pragma CHECK_MISRA("-8.7")
#pragma CHECK_MISRA("-16.3")
#pragma CHECK_MISRA("-16.4")
#pragma CHECK_MISRA("-19.4")
#pragma CHECK_MISRA("-19.6")
#pragma CHECK_MISRA("-19.10")
#include <msp430.h>
#pragma diag_pop

#include <string.h>
#include "platform.h"

int
main(void);

static volatile u8
_rbuf[16], _wbuf[16],
_rfrnt, _rback, _rqty, _rrsvd,
_wfrnt, _wback, _wqty, _wrsvd;

static u8
_cget(void);

static void
_cput(u8 src);

__interrupt void
_rxdo(void),
_txdo(void);

i16
input(u8 dst[], i16 qty) {
	register u16 ii = 0;

	P1OUT ^= BIT0;
	while (qty) {
		*dst = _cget();
		dst = &dst[1];
		++ii;
		--qty;
	}
	P1OUT ^= BIT0;
	return (i16)ii;
}
i16
output(cu8 src[], i16 qty) {
	register u16 oi = 0;

	while (qty) {
		_cput(*src);
		src = &src[1];
		++oi;
		--qty;
	}
	return (i16)oi;
}
void
flush(void) {
	/*fflush(stdout);*/
}
void
warn(cchr *msg) {
	/*fputs(msg, stderr);*/
}
u32
curtime(void) {
	/* return (u32)clock(); */
	return 0u;
}
void
scopy(char dst[64], cchr *src) {
	/* size_t a = strlen(src);
	size_t b = 64u < a? 64u: a;
	memcpy(dst, src, b); */
	strncpy(dst, src, 64u);
}
i8
strim(
	char dst[], cchr buf[64], i8 max
) {
	register i8 i = 64;

	while (i) {
		--i;
		if (' ' != buf[i]) {
			break;
		}
	}
	++i;
	if (i > max) {
		i = -1;
	}
	else {
		memcpy(
			dst, buf,
			(size_t)i
		);
		dst[i] = '\0';
	}
	return i;
}
i16
slen(register cchr src[64]) {
	register u8 i = 0u;

	while (
		('\0' != src[i])
		&& (64u != i)
	) {
		++i;
	}
	return (i16)i;
}
void
mcopy(void *dst, cv *src, i16 qty) {
	memcpy(dst, src, (size_t)qty);
}
void
tmp0(void) {
	_EINT();
}
int
main(void) {
	int ac = 1;
	char *av = 0;

	_rfrnt = 0u;
	_rback = 0u;
	_rqty = 0u;
	_wfrnt = 0u;
	_wback = 0u;
	_wqty = 0u;

	/* Stop watchdog. */
#ifdef __MSP430_HAS_WDT__
	WDTCTL = WDTPW | WDTHOLD;
#endif

	/* Set I/O ports. */
#ifdef __MSP430_HAS_PORT1_R__
	P1REN = 0xF8u;
	P1OUT = 0x05u;
	P1DIR = 0x05u;
#endif

#ifdef __MSP430_HAS_PORT2_R__
	P2REN = 0xFFu;
	P2OUT = 0x00u;
#endif

#ifdef __MSP430_HAS_PORT3_R__
	P3REN = 0xFFu;
	P3OUT = 0x00u;
#endif

	/* Set clocks. */
#ifdef __MSP430_HAS_BC2__
	/* Set DCO to 8 MHZ */
	DCOCTL = 0u;
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;
#endif

	/* Set serial port. */
#if defined __MSP430_HAS_P1SEL__ \
&& defined __MSP430_HAS_P1SEL2__
	P1SEL2 = 0x06u;
	P1SEL = 0x06u;
#endif

#ifdef __MSP430_HAS_USCI__
	/* Assuming BRCLK is 8 MHz. */
	UCA0CTL0 = 0u;
	UCA0BR0 = 69u;
	UCA0BR1 = 0u;
	UCA0MCTL = UCBRF_0 | UCBRS_4;
	UCA0CTL1 = UCSSEL_2 | UCRXEIE | UCSWRST;
	UCA0CTL1 &= ~UCSWRST;
	UC0IE = UCA0RXIE;
#endif
	_EINT();
	/*_cput(0x55u);
	_cput(0xAAu);*/
	ac = start(ac, &av);
	return ac;
}
static u8
_cget(void) {
	register u8 __cg;

	/* Wait if empty. */
	while (!_rqty) { LPM0; }
	__cg = _rbuf[_rfrnt];
	++_rfrnt;
	_rfrnt &= (sizeof _rbuf - 1u);
	_DINT();
	--_rqty;
	_EINT();
	return __cg;
}
static void
_cput(u8 src) {
	/* Wait if full. */
	while (sizeof _wbuf == _wqty) {
		LPM0;
	}
	_wbuf[_wback] = src;
	++_wback;
	_wback &= sizeof _wbuf - 1u;
	_DINT();
	++_wqty;
	_EINT();
	UC0IE |= UCA0TXIE;
}
#pragma vector=USCIAB0RX_VECTOR
__interrupt void
_rxdo(void) {
	/* TODO: handle overflow. */
	_rbuf[_rback] = UCA0RXBUF;
	++_rback;
	_rback &= sizeof _rbuf - 1u;
	++_rqty;
	LPM0_EXIT;
}
#pragma vector=USCIAB0TX_VECTOR
__interrupt void
_txdo(void) {
	UCA0TXBUF = _wbuf[_wfrnt];
	++_wfrnt;
	_wfrnt &= sizeof _rbuf - 1u;
	--_wqty;
	if (!_wqty) {
		UC0IE &= ~UCA0TXIE;
	}
	LPM0_EXIT;
}
