
#ifndef __FHC_THREAD_H
#define __FHC_THREAD_H

int fhc_thread_pow_init(void);
int fhc_thread_init(void);

int fhc_fan_thread_init(void);
int fhc_hot_thread_init(void);

void soft_timeout1(void* parameter);

void pow_thread_entry(void *par);
void key_thread_entry(void *par);
void dp_thread_entry(void *par);
void temp_thread_entry(void *par);
void time_thread_entry(void *par);
void hot_thread_entry(void *par);
void fan_thread_entry(void *par);

void fhc_set_fan_speed(float vol);

extern float DP_PT100_temp;
//extern unsigned int start_key_count;


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
