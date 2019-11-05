#include <am.h>
#include <amdev.h>
#include <nemu.h>

//for test
static uint32_t boot_time;

size_t __am_timer_read(uintptr_t reg, void *buf, size_t size) {//this is very basic func
  switch (reg) {
    case _DEVREG_TIMER_UPTIME: {
			uint32_t now_time = inl(RTC_ADDR);
			uint32_t passed_time = now_time - boot_time;
      _DEV_TIMER_UPTIME_t *uptime = (_DEV_TIMER_UPTIME_t *)buf;
      //read ms into buf(explained as uptime struct)
			
			uptime->hi = 0;//TODO
      uptime->lo = passed_time;//TODO
			//printf("hi=%d, lo=%d\n", uptime->hi, uptime->lo); 
			return sizeof(_DEV_TIMER_UPTIME_t);
			
															 }
    case _DEVREG_TIMER_DATE: {
      _DEV_TIMER_DATE_t *rtc = (_DEV_TIMER_DATE_t *)buf;
      rtc->second = 0;
      rtc->minute = 0;
      rtc->hour   = 0;
      rtc->day    = 0;
      rtc->month  = 0;
      rtc->year   = 2000;
      return sizeof(_DEV_TIMER_DATE_t);
    }
  }
  return 0;
}

void __am_timer_init() {
	boot_time = inl(RTC_ADDR);
}
