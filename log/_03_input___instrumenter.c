unsigned int __cs_active_thread[5]={1};
unsigned int __cs_pc[5];
unsigned int __cs_pc_cs[5];
unsigned int __cs_last_thread;
unsigned int __cs_thread_lines[5] = {4, 5, 5, 7, 2};

void *__cs_safe_malloc(unsigned int __cs_size) {
	void *__cs_ptr = malloc(__cs_size);
	return __cs_ptr;
}

void __cs_init_scalar(void *__cs_var, unsigned int __cs_size) {
	if (__cs_size == sizeof(int))
		*(int *)__cs_var = __CSEQ_nondet_int();
	else {
		__cs_var = malloc(__cs_size);
	}
}

void __CSEQ_message(char *__cs_message) { ; }

typedef int __cs_t;

void *__cs_threadargs[5];
void *__cs_thread_joinargs[5];

int __cs_create(__cs_t *__cs_new_thread_id, void *__cs_attr, void *(*__cs_thread_function)(void*), void *__cs_arg, int __cs_threadID) {
	if (__cs_threadID > THREADS) return 0;
	*__cs_new_thread_id = __cs_threadID;
	__cs_active_thread[__cs_threadID] = 1;
	__cs_threadargs[__cs_threadID] = __cs_arg;
	__CSEQ_message("thread spawned");
	return 0;
}

int __cs_join(__cs_t __cs_id, void **__cs_value_ptr) {
	__CSEQ_assume(__cs_pc[__cs_id] == __cs_thread_lines[__cs_id]);
	*__cs_value_ptr = __cs_thread_joinargs[__cs_id];
	return 0;
}

int __cs_exit(void *__cs_value_ptr, unsigned int __cs_thread_index) {
	__cs_thread_joinargs[__cs_thread_index] = __cs_value_ptr;
	return 0;
}

int __cs_self(void) { return 1; }

typedef int __cs_mutex_t;

int __cs_mutex_init (__cs_mutex_t *__cs_m, int __cs_val) {
	*__cs_m = -1;
	return 0;
}

int __cs_mutex_destroy(__cs_mutex_t *__cs_mutex_to_destroy) {
	__CSEQ_assertext(*__cs_mutex_to_destroy!=0,"attempt to destroy an uninitialized mutex");
	__CSEQ_assertext(*__cs_mutex_to_destroy!=-2,"attempt to destroy a previously destroyed mutex");
	__CSEQ_assertext(*__cs_mutex_to_destroy==-1,"attempt to destroy a locked mutex");
	*__cs_mutex_to_destroy = -2;
	__CSEQ_message("lock destroyed");
	return 0;
}

int __cs_mutex_lock(__cs_mutex_t *__cs_mutex_to_lock, unsigned int __cs_thread_index) {
	__CSEQ_assertext(*__cs_mutex_to_lock!=0,"attempt to lock an uninitialized mutex");
	__CSEQ_assertext(*__cs_mutex_to_lock!=-2,"attempt to lock a destroyed mutex");
	__CSEQ_assume(*__cs_mutex_to_lock==-1);
	*__cs_mutex_to_lock = __cs_thread_index+1;
	__CSEQ_message("lock acquired");
	return 0;
}

int __cs_mutex_unlock(__cs_mutex_t *__cs_mutex_to_unlock, unsigned int __cs_thread_index) {
	__CSEQ_assertext(*__cs_mutex_to_unlock!=0,"attempt to unlock an uninitialized mutex");
	__CSEQ_assertext(*__cs_mutex_to_unlock!=-2,"attempt to unlock a destroyed mutex");
	__CSEQ_assume(*__cs_mutex_to_unlock==(__cs_thread_index+1));
	*__cs_mutex_to_unlock = -1;
	__CSEQ_message("lock released");
	return 0;
}

typedef int __cs_cond_t;

int __cs_cond_init(__cs_cond_t *__cs_cond_to_init, void *__cs_attr) {
	*__cs_cond_to_init = -1;
	return 0;
}

int __cs_cond_destroy(__cs_cond_t *__cs_cond_to_destroy) {
	*__cs_cond_to_destroy = -2;
	return 0;
}

int __cs_cond_wait_1(__cs_cond_t *__cs_cond_to_wait_for, __cs_mutex_t *__cs_m, unsigned int __cs_thread_index) {
	__CSEQ_assertext(*__cs_cond_to_wait_for!=0,"attempt to use an uninitialized conditional variable");
	__CSEQ_assertext(*__cs_cond_to_wait_for!=-2,"attempt to use a destroyed conditional variable");
	__cs_mutex_unlock(__cs_m, __cs_thread_index);
	return 0;
}

int __cs_cond_wait_2(__cs_cond_t *__cs_cond_to_wait_for, __cs_mutex_t *__cs_m, unsigned int __cs_thread_index) {
	__CSEQ_assume(*__cs_cond_to_wait_for == 1);
	__cs_mutex_lock(__cs_m, __cs_thread_index);
	return 0;
}

int __cs_cond_signal(__cs_cond_t *__cs_cond_to_signal) {
	*__cs_cond_to_signal = 1;
	__CSEQ_message("conditional variable signal");
	return 0;
}

int __cs_cond_broadcast(__cs_cond_t *__cs_cond_to_broadcast) {
	*__cs_cond_to_broadcast = 1;
	__CSEQ_message("conditional variable broadcast");
	return 0;
}

typedef struct __cs_barrier_t{
	unsigned int init;
	unsigned int current;
} __cs_barrier_t;

int __cs_barrier_init(__cs_barrier_t *__cs_barrier_to_init, void * __cs_attr, unsigned int count){
	__CSEQ_assertext(count > 0, "count must be greater than 0");
	__cs_barrier_to_init->current = count;
	__cs_barrier_to_init->init = count;
	return 0;
}

int __cs_barrier_destroy(__cs_barrier_t *__cs_barrier_to_destroy) {
	__cs_barrier_to_destroy->init = -1;
	__cs_barrier_to_destroy->current = -1;
	return 0;
}


int __cs_barrier_wait_1(__cs_barrier_t *__cs_barrier_to_wait){
	__CSEQ_assertext(__cs_barrier_to_wait->init > 0, "attempt to use an uninitialized barrier variable");
	__cs_barrier_to_wait->current--;
	return 0;
}

int __cs_barrier_wait_2(__cs_barrier_t *__cs_barrier_to_wait){
	__CSEQ_assertext(__cs_barrier_to_wait->init > 0, "attempt to use an uninitialized barrier variable");
	__CSEQ_assume(__cs_barrier_to_wait->current == 0);
	__cs_barrier_to_wait->current = __cs_barrier_to_wait->init;
	return 0;
}

typedef int __cs_attr_t;
struct sched_param {
   int sched_priority;
};
int   __cs_attr_init(__cs_attr_t * t) { return 0;}
int   __cs_attr_destroy(__cs_attr_t * t) { return 0;}
int   __cs_attr_getdetachstate(const __cs_attr_t * t, int * s) { return 0;}
int   __cs_attr_getguardsize(const __cs_attr_t * t, unsigned int * s) { return 0;}
int   __cs_attr_getinheritsched(const __cs_attr_t * t, int * s) { return 0;}
int   __cs_attr_getschedparam(const __cs_attr_t * t, struct sched_param * s) { return 0;}
int   __cs_attr_getschedpolicy(const __cs_attr_t * t, int * s) { return 0;}
int   __cs_attr_getscope(const __cs_attr_t * t, int * s) { return 0;}
int   __cs_attr_getstackaddr(const __cs_attr_t * t, void ** s) { return 0;}
int   __cs_attr_getstacksize(const __cs_attr_t * t, unsigned int *s) { return 0;}
int   __cs_attr_setdetachstate(__cs_attr_t * t, int s) { return 0;}
int   __cs_attr_setguardsize(__cs_attr_t * t, unsigned int s) { return 0;}
int   __cs_attr_setinheritsched(__cs_attr_t * t, int s) { return 0;}
int   __cs_attr_setschedparam(__cs_attr_t * t, const struct sched_param * s) { return 0;}
int   __cs_attr_setschedpolicy(__cs_attr_t * t, int s) { return 0;}
int   __cs_attr_setscope(__cs_attr_t * t, int s) { return 0;}
int   __cs_attr_setstackaddr(__cs_attr_t * t, void * s) { return 0;}
int   __cs_attr_setstacksize(__cs_attr_t * t, unsigned int s) { return 0;}
typedef int _____STARTSTRIPPINGFROMHERE_____;
typedef int __cs_barrier_t;
typedef int __cs_barrierattr_t;
typedef int __cs_attr_t;
typedef int __cs_cond_t;
typedef int __cs_condattr_t;
typedef int __cs_key_t;
typedef int __cs_mutex_t;
typedef int __cs_mutexattr_t;
typedef int __cs_once_t;
typedef int __cs_rwlock_t;
typedef int __cs_rwlockattr_t;
typedef int __cs_t;
typedef int size_t;
typedef int __builtin_va_list;
typedef int __gnuc_va_list;
typedef int __int8_t;
typedef int __uint8_t;
typedef int __int16_t;
typedef int __uint16_t;
typedef int __int_least16_t;
typedef int __uint_least16_t;
typedef int __int32_t;
typedef int __uint32_t;
typedef int __int64_t;
typedef int __uint64_t;
typedef int __int_least32_t;
typedef int __uint_least32_t;
typedef int __s8;
typedef int __u8;
typedef int __s16;
typedef int __u16;
typedef int __s32;
typedef int __u32;
typedef int __s64;
typedef int __u64;
typedef int _LOCK_T;
typedef int _LOCK_RECURSIVE_T;
typedef int _off_t;
typedef int __dev_t;
typedef int __uid_t;
typedef int __gid_t;
typedef int _off64_t;
typedef int _fpos_t;
typedef int _ssize_t;
typedef int wint_t;
typedef int _mbstate_t;
typedef int _flock_t;
typedef int _iconv_t;
typedef int __ULong;
typedef int __FILE;
typedef int ptrdiff_t;
typedef int wchar_t;
typedef int __off_t;
typedef int __pid_t;
typedef int __loff_t;
typedef int u_char;
typedef int u_short;
typedef int u_int;
typedef int u_long;
typedef int ushort;
typedef int uint;
typedef int clock_t;
typedef int time_t;
typedef int daddr_t;
typedef int caddr_t;
typedef int ino_t;
typedef int off_t;
typedef int dev_t;
typedef int uid_t;
typedef int gid_t;
typedef int pid_t;
typedef int key_t;
typedef int ssize_t;
typedef int mode_t;
typedef int nlink_t;
typedef int fd_mask;
typedef int _types_fd_set;
typedef int fd_set;
typedef int clockid_t;
typedef int timer_t;
typedef int useconds_t;
typedef int suseconds_t;
typedef int FILE;
typedef int fpos_t;
typedef int cookie_read_function_t;
typedef int cookie_write_function_t;
typedef int cookie_seek_function_t;
typedef int cookie_close_function_t;
typedef int cookie_io_functions_t;
typedef int div_t;
typedef int ldiv_t;
typedef int lldiv_t;
typedef int sigset_t;
typedef int __sigset_t;
typedef int _sig_func_ptr;
typedef int sig_atomic_t;
typedef int __tzrule_type;
typedef int __tzinfo_type;
typedef int mbstate_t;
typedef int sem_t;
typedef int pthread_t;
typedef int pthread_attr_t;
typedef int pthread_mutex_t;
typedef int pthread_mutexattr_t;
typedef int pthread_cond_t;
typedef int pthread_condattr_t;
typedef int pthread_key_t;
typedef int pthread_once_t;
typedef int pthread_rwlock_t;
typedef int pthread_rwlockattr_t;
typedef int pthread_spinlock_t;
typedef int pthread_barrier_t;
typedef int pthread_barrierattr_t;
typedef int jmp_buf;
typedef int rlim_t;
typedef int sa_family_t;
typedef int sigjmp_buf;
typedef int stack_t;
typedef int siginfo_t;
typedef int z_stream;
typedef int int8_t;
typedef int uint8_t;
typedef int int16_t;
typedef int uint16_t;
typedef int int32_t;
typedef int uint32_t;
typedef int int64_t;
typedef int uint64_t;
typedef int int_least8_t;
typedef int uint_least8_t;
typedef int int_least16_t;
typedef int uint_least16_t;
typedef int int_least32_t;
typedef int uint_least32_t;
typedef int int_least64_t;
typedef int uint_least64_t;
typedef int int_fast8_t;
typedef int uint_fast8_t;
typedef int int_fast16_t;
typedef int uint_fast16_t;
typedef int int_fast32_t;
typedef int uint_fast32_t;
typedef int int_fast64_t;
typedef int uint_fast64_t;
typedef int intptr_t;
typedef int uintptr_t;
typedef int intmax_t;
typedef int uintmax_t;
typedef _Bool bool;
typedef void BZFILE;
typedef int va_list;
typedef int loff_t;
typedef int _____STOPSTRIPPINGFROMHERE_____;
int var0;
int z1;
int casvar;
int fib1;
int fib2;
static int __cs_pscopy_t2__x;
static int __cs_pscopy_t2__y;
static int __cs_pscopy_t2__s;
static int __cs_pscopy_t3__x;
static int __cs_pscopy_t3__y;
static int __cs_pscopy_t3__s;
static int __cs_pscopy_t0___lv;
static int __cs_pscopy_t0__n;
static int __cs_pscopy_t1___lv;
void *t0_0(void *__cs_param_t0___cs_unused)
{
static unsigned int __cs_local_t0__ps_non_det_int_var;
static _Bool __cs_local_t0__ps_non_det_bool_var;
static int __cs_local_t0___lv;
static int __cs_local_t0__n;
int __cs_local_t0__int_update_helper_var;
    {
static int __cs_local_t0___lv;
__CSEQ_rawline("IF(3,0,tt0_0_1)");
__cs_local_t0___lv = 0;
static int __cs_local_t0__n;
__cs_local_t0__n = __CSEQ_nondet_int();
if (nondet_bool())
        {
if (_lazyps_active[ps_thread_num] == 0)
            {
__CSEQ_rawline("tt0_0_1: IF(3,1,tt0_0_2)");
;
_lazyps_active[ps_thread_num] = 1;
            }
            __CSEQ_assume( __cs_pc_cs[3] >= 2 );;
_lazyps_retAddr[ps_thread_num] = 0;
save_state_ps(ps_thread_num);
;
__cs_pscopy_t0___lv = __cs_local_t0___lv;
__cs_pscopy_t0__n = __cs_local_t0__n;
t0_pscopy(ps_thread_num);
      __CPROVER_assume(_ps_flag[0] != ps_thread_num);
      if(_ps_flag[0] == 255){
          _ps_flag[0] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[1] != ps_thread_num);
      if(_ps_flag[1] == 255){
          _ps_flag[1] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[2] != ps_thread_num);
      if(_ps_flag[2] == 255){
          _ps_flag[2] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[3] != ps_thread_num);
      if(_ps_flag[3] == 255){
          _ps_flag[3] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[4] != ps_thread_num);
      if(_ps_flag[4] == 255){
          _ps_flag[4] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[5] != ps_thread_num);
      if(_ps_flag[5] == 255){
          _ps_flag[5] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[6] != ps_thread_num);
      if(_ps_flag[6] == 255){
          _ps_flag[6] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[7] != ps_thread_num);
      if(_ps_flag[7] == 255){
          _ps_flag[7] = ps_thread_num;
      }
  __CPROVER_assume(var0_liveChain[0] == 0);
  __CPROVER_assume(var0_liveChain[0] == 0);
if(_lazyps_usedReservation_var0[0] == 0 && var0_ps_reserve[0] == ps_thread_num){;
  var0_ps_reserve[0] = 0;
  _lazyps_upd_var0[0] = 1;
}
_lazyps_usedReservation_var0[0] = 0;
if(_lazyps_usedReservation_var0[1] == 0 && var0_ps_reserve[1] == ps_thread_num){;
  var0_ps_reserve[1] = 0;
  _lazyps_upd_var0[1] = 1;
}
_lazyps_usedReservation_var0[1] = 0;
if(_lazyps_usedReservation_var0[2] == 0 && var0_ps_reserve[2] == ps_thread_num){;
  var0_ps_reserve[2] = 0;
  _lazyps_upd_var0[2] = 1;
}
_lazyps_usedReservation_var0[2] = 0;
if(_lazyps_usedReservation_var0[3] == 0 && var0_ps_reserve[3] == ps_thread_num){;
  var0_ps_reserve[3] = 0;
  _lazyps_upd_var0[3] = 1;
}
_lazyps_usedReservation_var0[3] = 0;
if(_lazyps_usedReservation_var0[4] == 0 && var0_ps_reserve[4] == ps_thread_num){;
  var0_ps_reserve[4] = 0;
  _lazyps_upd_var0[4] = 1;
}
_lazyps_usedReservation_var0[4] = 0;
if(_lazyps_usedReservation_var0[5] == 0 && var0_ps_reserve[5] == ps_thread_num){;
  var0_ps_reserve[5] = 0;
  _lazyps_upd_var0[5] = 1;
}
_lazyps_usedReservation_var0[5] = 0;
if(_lazyps_usedReservation_var0[6] == 0 && var0_ps_reserve[6] == ps_thread_num){;
  var0_ps_reserve[6] = 0;
  _lazyps_upd_var0[6] = 1;
}
_lazyps_usedReservation_var0[6] = 0;
if(_lazyps_usedReservation_var0[7] == 0 && var0_ps_reserve[7] == ps_thread_num){;
  var0_ps_reserve[7] = 0;
  _lazyps_upd_var0[7] = 1;
}
_lazyps_usedReservation_var0[7] = 0;
if(_lazyps_usedReservation_var0[8] == 0 && var0_ps_reserve[8] == ps_thread_num){;
  var0_ps_reserve[8] = 0;
  _lazyps_upd_var0[8] = 1;
}
_lazyps_usedReservation_var0[8] = 0;
  __CPROVER_assume(z1_liveChain[0] == 0);
  __CPROVER_assume(z1_liveChain[0] == 0);
if(_lazyps_usedReservation_z1[0] == 0 && z1_ps_reserve[0] == ps_thread_num){;
  z1_ps_reserve[0] = 0;
  _lazyps_upd_z1[0] = 1;
}
_lazyps_usedReservation_z1[0] = 0;
if(_lazyps_usedReservation_z1[1] == 0 && z1_ps_reserve[1] == ps_thread_num){;
  z1_ps_reserve[1] = 0;
  _lazyps_upd_z1[1] = 1;
}
_lazyps_usedReservation_z1[1] = 0;
if(_lazyps_usedReservation_z1[2] == 0 && z1_ps_reserve[2] == ps_thread_num){;
  z1_ps_reserve[2] = 0;
  _lazyps_upd_z1[2] = 1;
}
_lazyps_usedReservation_z1[2] = 0;
if(_lazyps_usedReservation_z1[3] == 0 && z1_ps_reserve[3] == ps_thread_num){;
  z1_ps_reserve[3] = 0;
  _lazyps_upd_z1[3] = 1;
}
_lazyps_usedReservation_z1[3] = 0;
if(_lazyps_usedReservation_z1[4] == 0 && z1_ps_reserve[4] == ps_thread_num){;
  z1_ps_reserve[4] = 0;
  _lazyps_upd_z1[4] = 1;
}
_lazyps_usedReservation_z1[4] = 0;
if(_lazyps_usedReservation_z1[5] == 0 && z1_ps_reserve[5] == ps_thread_num){;
  z1_ps_reserve[5] = 0;
  _lazyps_upd_z1[5] = 1;
}
_lazyps_usedReservation_z1[5] = 0;
if(_lazyps_usedReservation_z1[6] == 0 && z1_ps_reserve[6] == ps_thread_num){;
  z1_ps_reserve[6] = 0;
  _lazyps_upd_z1[6] = 1;
}
_lazyps_usedReservation_z1[6] = 0;
if(_lazyps_usedReservation_z1[7] == 0 && z1_ps_reserve[7] == ps_thread_num){;
  z1_ps_reserve[7] = 0;
  _lazyps_upd_z1[7] = 1;
}
_lazyps_usedReservation_z1[7] = 0;
if(_lazyps_usedReservation_z1[8] == 0 && z1_ps_reserve[8] == ps_thread_num){;
  z1_ps_reserve[8] = 0;
  _lazyps_upd_z1[8] = 1;
}
_lazyps_usedReservation_z1[8] = 0;
  __CPROVER_assume(casvar_liveChain[0] == 0);
  __CPROVER_assume(casvar_liveChain[0] == 0);
if(_lazyps_usedReservation_casvar[0] == 0 && casvar_ps_reserve[0] == ps_thread_num){;
  casvar_ps_reserve[0] = 0;
  _lazyps_upd_casvar[0] = 1;
}
_lazyps_usedReservation_casvar[0] = 0;
if(_lazyps_usedReservation_casvar[1] == 0 && casvar_ps_reserve[1] == ps_thread_num){;
  casvar_ps_reserve[1] = 0;
  _lazyps_upd_casvar[1] = 1;
}
_lazyps_usedReservation_casvar[1] = 0;
if(_lazyps_usedReservation_casvar[2] == 0 && casvar_ps_reserve[2] == ps_thread_num){;
  casvar_ps_reserve[2] = 0;
  _lazyps_upd_casvar[2] = 1;
}
_lazyps_usedReservation_casvar[2] = 0;
if(_lazyps_usedReservation_casvar[3] == 0 && casvar_ps_reserve[3] == ps_thread_num){;
  casvar_ps_reserve[3] = 0;
  _lazyps_upd_casvar[3] = 1;
}
_lazyps_usedReservation_casvar[3] = 0;
if(_lazyps_usedReservation_casvar[4] == 0 && casvar_ps_reserve[4] == ps_thread_num){;
  casvar_ps_reserve[4] = 0;
  _lazyps_upd_casvar[4] = 1;
}
_lazyps_usedReservation_casvar[4] = 0;
if(_lazyps_usedReservation_casvar[5] == 0 && casvar_ps_reserve[5] == ps_thread_num){;
  casvar_ps_reserve[5] = 0;
  _lazyps_upd_casvar[5] = 1;
}
_lazyps_usedReservation_casvar[5] = 0;
if(_lazyps_usedReservation_casvar[6] == 0 && casvar_ps_reserve[6] == ps_thread_num){;
  casvar_ps_reserve[6] = 0;
  _lazyps_upd_casvar[6] = 1;
}
_lazyps_usedReservation_casvar[6] = 0;
if(_lazyps_usedReservation_casvar[7] == 0 && casvar_ps_reserve[7] == ps_thread_num){;
  casvar_ps_reserve[7] = 0;
  _lazyps_upd_casvar[7] = 1;
}
_lazyps_usedReservation_casvar[7] = 0;
if(_lazyps_usedReservation_casvar[8] == 0 && casvar_ps_reserve[8] == ps_thread_num){;
  casvar_ps_reserve[8] = 0;
  _lazyps_upd_casvar[8] = 1;
}
_lazyps_usedReservation_casvar[8] = 0;
  __CPROVER_assume(fib1_liveChain[0] == 0);
  __CPROVER_assume(fib1_liveChain[0] == 0);
if(_lazyps_usedReservation_fib1[0] == 0 && fib1_ps_reserve[0] == ps_thread_num){;
  fib1_ps_reserve[0] = 0;
  _lazyps_upd_fib1[0] = 1;
}
_lazyps_usedReservation_fib1[0] = 0;
if(_lazyps_usedReservation_fib1[1] == 0 && fib1_ps_reserve[1] == ps_thread_num){;
  fib1_ps_reserve[1] = 0;
  _lazyps_upd_fib1[1] = 1;
}
_lazyps_usedReservation_fib1[1] = 0;
if(_lazyps_usedReservation_fib1[2] == 0 && fib1_ps_reserve[2] == ps_thread_num){;
  fib1_ps_reserve[2] = 0;
  _lazyps_upd_fib1[2] = 1;
}
_lazyps_usedReservation_fib1[2] = 0;
if(_lazyps_usedReservation_fib1[3] == 0 && fib1_ps_reserve[3] == ps_thread_num){;
  fib1_ps_reserve[3] = 0;
  _lazyps_upd_fib1[3] = 1;
}
_lazyps_usedReservation_fib1[3] = 0;
if(_lazyps_usedReservation_fib1[4] == 0 && fib1_ps_reserve[4] == ps_thread_num){;
  fib1_ps_reserve[4] = 0;
  _lazyps_upd_fib1[4] = 1;
}
_lazyps_usedReservation_fib1[4] = 0;
if(_lazyps_usedReservation_fib1[5] == 0 && fib1_ps_reserve[5] == ps_thread_num){;
  fib1_ps_reserve[5] = 0;
  _lazyps_upd_fib1[5] = 1;
}
_lazyps_usedReservation_fib1[5] = 0;
if(_lazyps_usedReservation_fib1[6] == 0 && fib1_ps_reserve[6] == ps_thread_num){;
  fib1_ps_reserve[6] = 0;
  _lazyps_upd_fib1[6] = 1;
}
_lazyps_usedReservation_fib1[6] = 0;
if(_lazyps_usedReservation_fib1[7] == 0 && fib1_ps_reserve[7] == ps_thread_num){;
  fib1_ps_reserve[7] = 0;
  _lazyps_upd_fib1[7] = 1;
}
_lazyps_usedReservation_fib1[7] = 0;
if(_lazyps_usedReservation_fib1[8] == 0 && fib1_ps_reserve[8] == ps_thread_num){;
  fib1_ps_reserve[8] = 0;
  _lazyps_upd_fib1[8] = 1;
}
_lazyps_usedReservation_fib1[8] = 0;
  __CPROVER_assume(fib2_liveChain[0] == 0);
  __CPROVER_assume(fib2_liveChain[0] == 0);
if(_lazyps_usedReservation_fib2[0] == 0 && fib2_ps_reserve[0] == ps_thread_num){;
  fib2_ps_reserve[0] = 0;
  _lazyps_upd_fib2[0] = 1;
}
_lazyps_usedReservation_fib2[0] = 0;
if(_lazyps_usedReservation_fib2[1] == 0 && fib2_ps_reserve[1] == ps_thread_num){;
  fib2_ps_reserve[1] = 0;
  _lazyps_upd_fib2[1] = 1;
}
_lazyps_usedReservation_fib2[1] = 0;
if(_lazyps_usedReservation_fib2[2] == 0 && fib2_ps_reserve[2] == ps_thread_num){;
  fib2_ps_reserve[2] = 0;
  _lazyps_upd_fib2[2] = 1;
}
_lazyps_usedReservation_fib2[2] = 0;
if(_lazyps_usedReservation_fib2[3] == 0 && fib2_ps_reserve[3] == ps_thread_num){;
  fib2_ps_reserve[3] = 0;
  _lazyps_upd_fib2[3] = 1;
}
_lazyps_usedReservation_fib2[3] = 0;
if(_lazyps_usedReservation_fib2[4] == 0 && fib2_ps_reserve[4] == ps_thread_num){;
  fib2_ps_reserve[4] = 0;
  _lazyps_upd_fib2[4] = 1;
}
_lazyps_usedReservation_fib2[4] = 0;
if(_lazyps_usedReservation_fib2[5] == 0 && fib2_ps_reserve[5] == ps_thread_num){;
  fib2_ps_reserve[5] = 0;
  _lazyps_upd_fib2[5] = 1;
}
_lazyps_usedReservation_fib2[5] = 0;
if(_lazyps_usedReservation_fib2[6] == 0 && fib2_ps_reserve[6] == ps_thread_num){;
  fib2_ps_reserve[6] = 0;
  _lazyps_upd_fib2[6] = 1;
}
_lazyps_usedReservation_fib2[6] = 0;
if(_lazyps_usedReservation_fib2[7] == 0 && fib2_ps_reserve[7] == ps_thread_num){;
  fib2_ps_reserve[7] = 0;
  _lazyps_upd_fib2[7] = 1;
}
_lazyps_usedReservation_fib2[7] = 0;
if(_lazyps_usedReservation_fib2[8] == 0 && fib2_ps_reserve[8] == ps_thread_num){;
  fib2_ps_reserve[8] = 0;
  _lazyps_upd_fib2[8] = 1;
}
_lazyps_usedReservation_fib2[8] = 0;
;
load_state_ps(ps_thread_num);
;
_lazyps_active[ps_thread_num] = 0;
;
        }
        __CSEQ_assume( __cs_pc_cs[3] >= 2 );;
if (_lazyps_active[ps_thread_num] == 0)
        {
__CSEQ_rawline("tt0_0_2: IF(3,2,tt0_0_3)");
;
_lazyps_active[ps_thread_num] = 1;
        }
        __CSEQ_assume( __cs_pc_cs[3] >= 3 );;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t0__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t0__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t0__ps_non_det_int_var] == 2);
 __CPROVER_assume((z1_view_t[ps_thread_num] < z1_ps_time[__cs_local_t0__ps_non_det_int_var]) || ((z1_view_t[ps_thread_num] == z1_ps_time[__cs_local_t0__ps_non_det_int_var]) && z1_view_l[ps_thread_num]));
 z1_view_t[ps_thread_num] = z1_ps_time[__cs_local_t0__ps_non_det_int_var];
 z1_view_l[ps_thread_num] = 1;
 z1_view[ps_thread_num] = z1_values[__cs_local_t0__ps_non_det_int_var];
 z1_extView[0] = 1;
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(z1_view_v[ps_thread_num]);
}
__cs_local_t0___lv = z1_view[ps_thread_num];
;
if (__cs_local_t0___lv == 1)
        {
__CSEQ_assert(0);
        }
        else
        { 
if (nondet_bool())
            {
if (_lazyps_active[ps_thread_num] == 0)
                {
__CSEQ_rawline("tt0_0_3: IF(3,3,tt0_0_4)");
;
_lazyps_active[ps_thread_num] = 1;
                }
                __CSEQ_assume( __cs_pc_cs[3] >= 4 );;
_lazyps_retAddr[ps_thread_num] = 1;
save_state_ps(ps_thread_num);
;
__cs_pscopy_t0___lv = __cs_local_t0___lv;
__cs_pscopy_t0__n = __cs_local_t0__n;
t0_pscopy(ps_thread_num);
      __CPROVER_assume(_ps_flag[0] != ps_thread_num);
      if(_ps_flag[0] == 255){
          _ps_flag[0] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[1] != ps_thread_num);
      if(_ps_flag[1] == 255){
          _ps_flag[1] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[2] != ps_thread_num);
      if(_ps_flag[2] == 255){
          _ps_flag[2] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[3] != ps_thread_num);
      if(_ps_flag[3] == 255){
          _ps_flag[3] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[4] != ps_thread_num);
      if(_ps_flag[4] == 255){
          _ps_flag[4] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[5] != ps_thread_num);
      if(_ps_flag[5] == 255){
          _ps_flag[5] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[6] != ps_thread_num);
      if(_ps_flag[6] == 255){
          _ps_flag[6] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[7] != ps_thread_num);
      if(_ps_flag[7] == 255){
          _ps_flag[7] = ps_thread_num;
      }
  __CPROVER_assume(var0_liveChain[0] == 0);
  __CPROVER_assume(var0_liveChain[0] == 0);
if(_lazyps_usedReservation_var0[0] == 0 && var0_ps_reserve[0] == ps_thread_num){;
  var0_ps_reserve[0] = 0;
  _lazyps_upd_var0[0] = 1;
}
_lazyps_usedReservation_var0[0] = 0;
if(_lazyps_usedReservation_var0[1] == 0 && var0_ps_reserve[1] == ps_thread_num){;
  var0_ps_reserve[1] = 0;
  _lazyps_upd_var0[1] = 1;
}
_lazyps_usedReservation_var0[1] = 0;
if(_lazyps_usedReservation_var0[2] == 0 && var0_ps_reserve[2] == ps_thread_num){;
  var0_ps_reserve[2] = 0;
  _lazyps_upd_var0[2] = 1;
}
_lazyps_usedReservation_var0[2] = 0;
if(_lazyps_usedReservation_var0[3] == 0 && var0_ps_reserve[3] == ps_thread_num){;
  var0_ps_reserve[3] = 0;
  _lazyps_upd_var0[3] = 1;
}
_lazyps_usedReservation_var0[3] = 0;
if(_lazyps_usedReservation_var0[4] == 0 && var0_ps_reserve[4] == ps_thread_num){;
  var0_ps_reserve[4] = 0;
  _lazyps_upd_var0[4] = 1;
}
_lazyps_usedReservation_var0[4] = 0;
if(_lazyps_usedReservation_var0[5] == 0 && var0_ps_reserve[5] == ps_thread_num){;
  var0_ps_reserve[5] = 0;
  _lazyps_upd_var0[5] = 1;
}
_lazyps_usedReservation_var0[5] = 0;
if(_lazyps_usedReservation_var0[6] == 0 && var0_ps_reserve[6] == ps_thread_num){;
  var0_ps_reserve[6] = 0;
  _lazyps_upd_var0[6] = 1;
}
_lazyps_usedReservation_var0[6] = 0;
if(_lazyps_usedReservation_var0[7] == 0 && var0_ps_reserve[7] == ps_thread_num){;
  var0_ps_reserve[7] = 0;
  _lazyps_upd_var0[7] = 1;
}
_lazyps_usedReservation_var0[7] = 0;
if(_lazyps_usedReservation_var0[8] == 0 && var0_ps_reserve[8] == ps_thread_num){;
  var0_ps_reserve[8] = 0;
  _lazyps_upd_var0[8] = 1;
}
_lazyps_usedReservation_var0[8] = 0;
  __CPROVER_assume(z1_liveChain[0] == 0);
  __CPROVER_assume(z1_liveChain[0] == 0);
if(_lazyps_usedReservation_z1[0] == 0 && z1_ps_reserve[0] == ps_thread_num){;
  z1_ps_reserve[0] = 0;
  _lazyps_upd_z1[0] = 1;
}
_lazyps_usedReservation_z1[0] = 0;
if(_lazyps_usedReservation_z1[1] == 0 && z1_ps_reserve[1] == ps_thread_num){;
  z1_ps_reserve[1] = 0;
  _lazyps_upd_z1[1] = 1;
}
_lazyps_usedReservation_z1[1] = 0;
if(_lazyps_usedReservation_z1[2] == 0 && z1_ps_reserve[2] == ps_thread_num){;
  z1_ps_reserve[2] = 0;
  _lazyps_upd_z1[2] = 1;
}
_lazyps_usedReservation_z1[2] = 0;
if(_lazyps_usedReservation_z1[3] == 0 && z1_ps_reserve[3] == ps_thread_num){;
  z1_ps_reserve[3] = 0;
  _lazyps_upd_z1[3] = 1;
}
_lazyps_usedReservation_z1[3] = 0;
if(_lazyps_usedReservation_z1[4] == 0 && z1_ps_reserve[4] == ps_thread_num){;
  z1_ps_reserve[4] = 0;
  _lazyps_upd_z1[4] = 1;
}
_lazyps_usedReservation_z1[4] = 0;
if(_lazyps_usedReservation_z1[5] == 0 && z1_ps_reserve[5] == ps_thread_num){;
  z1_ps_reserve[5] = 0;
  _lazyps_upd_z1[5] = 1;
}
_lazyps_usedReservation_z1[5] = 0;
if(_lazyps_usedReservation_z1[6] == 0 && z1_ps_reserve[6] == ps_thread_num){;
  z1_ps_reserve[6] = 0;
  _lazyps_upd_z1[6] = 1;
}
_lazyps_usedReservation_z1[6] = 0;
if(_lazyps_usedReservation_z1[7] == 0 && z1_ps_reserve[7] == ps_thread_num){;
  z1_ps_reserve[7] = 0;
  _lazyps_upd_z1[7] = 1;
}
_lazyps_usedReservation_z1[7] = 0;
if(_lazyps_usedReservation_z1[8] == 0 && z1_ps_reserve[8] == ps_thread_num){;
  z1_ps_reserve[8] = 0;
  _lazyps_upd_z1[8] = 1;
}
_lazyps_usedReservation_z1[8] = 0;
  __CPROVER_assume(casvar_liveChain[0] == 0);
  __CPROVER_assume(casvar_liveChain[0] == 0);
if(_lazyps_usedReservation_casvar[0] == 0 && casvar_ps_reserve[0] == ps_thread_num){;
  casvar_ps_reserve[0] = 0;
  _lazyps_upd_casvar[0] = 1;
}
_lazyps_usedReservation_casvar[0] = 0;
if(_lazyps_usedReservation_casvar[1] == 0 && casvar_ps_reserve[1] == ps_thread_num){;
  casvar_ps_reserve[1] = 0;
  _lazyps_upd_casvar[1] = 1;
}
_lazyps_usedReservation_casvar[1] = 0;
if(_lazyps_usedReservation_casvar[2] == 0 && casvar_ps_reserve[2] == ps_thread_num){;
  casvar_ps_reserve[2] = 0;
  _lazyps_upd_casvar[2] = 1;
}
_lazyps_usedReservation_casvar[2] = 0;
if(_lazyps_usedReservation_casvar[3] == 0 && casvar_ps_reserve[3] == ps_thread_num){;
  casvar_ps_reserve[3] = 0;
  _lazyps_upd_casvar[3] = 1;
}
_lazyps_usedReservation_casvar[3] = 0;
if(_lazyps_usedReservation_casvar[4] == 0 && casvar_ps_reserve[4] == ps_thread_num){;
  casvar_ps_reserve[4] = 0;
  _lazyps_upd_casvar[4] = 1;
}
_lazyps_usedReservation_casvar[4] = 0;
if(_lazyps_usedReservation_casvar[5] == 0 && casvar_ps_reserve[5] == ps_thread_num){;
  casvar_ps_reserve[5] = 0;
  _lazyps_upd_casvar[5] = 1;
}
_lazyps_usedReservation_casvar[5] = 0;
if(_lazyps_usedReservation_casvar[6] == 0 && casvar_ps_reserve[6] == ps_thread_num){;
  casvar_ps_reserve[6] = 0;
  _lazyps_upd_casvar[6] = 1;
}
_lazyps_usedReservation_casvar[6] = 0;
if(_lazyps_usedReservation_casvar[7] == 0 && casvar_ps_reserve[7] == ps_thread_num){;
  casvar_ps_reserve[7] = 0;
  _lazyps_upd_casvar[7] = 1;
}
_lazyps_usedReservation_casvar[7] = 0;
if(_lazyps_usedReservation_casvar[8] == 0 && casvar_ps_reserve[8] == ps_thread_num){;
  casvar_ps_reserve[8] = 0;
  _lazyps_upd_casvar[8] = 1;
}
_lazyps_usedReservation_casvar[8] = 0;
  __CPROVER_assume(fib1_liveChain[0] == 0);
  __CPROVER_assume(fib1_liveChain[0] == 0);
if(_lazyps_usedReservation_fib1[0] == 0 && fib1_ps_reserve[0] == ps_thread_num){;
  fib1_ps_reserve[0] = 0;
  _lazyps_upd_fib1[0] = 1;
}
_lazyps_usedReservation_fib1[0] = 0;
if(_lazyps_usedReservation_fib1[1] == 0 && fib1_ps_reserve[1] == ps_thread_num){;
  fib1_ps_reserve[1] = 0;
  _lazyps_upd_fib1[1] = 1;
}
_lazyps_usedReservation_fib1[1] = 0;
if(_lazyps_usedReservation_fib1[2] == 0 && fib1_ps_reserve[2] == ps_thread_num){;
  fib1_ps_reserve[2] = 0;
  _lazyps_upd_fib1[2] = 1;
}
_lazyps_usedReservation_fib1[2] = 0;
if(_lazyps_usedReservation_fib1[3] == 0 && fib1_ps_reserve[3] == ps_thread_num){;
  fib1_ps_reserve[3] = 0;
  _lazyps_upd_fib1[3] = 1;
}
_lazyps_usedReservation_fib1[3] = 0;
if(_lazyps_usedReservation_fib1[4] == 0 && fib1_ps_reserve[4] == ps_thread_num){;
  fib1_ps_reserve[4] = 0;
  _lazyps_upd_fib1[4] = 1;
}
_lazyps_usedReservation_fib1[4] = 0;
if(_lazyps_usedReservation_fib1[5] == 0 && fib1_ps_reserve[5] == ps_thread_num){;
  fib1_ps_reserve[5] = 0;
  _lazyps_upd_fib1[5] = 1;
}
_lazyps_usedReservation_fib1[5] = 0;
if(_lazyps_usedReservation_fib1[6] == 0 && fib1_ps_reserve[6] == ps_thread_num){;
  fib1_ps_reserve[6] = 0;
  _lazyps_upd_fib1[6] = 1;
}
_lazyps_usedReservation_fib1[6] = 0;
if(_lazyps_usedReservation_fib1[7] == 0 && fib1_ps_reserve[7] == ps_thread_num){;
  fib1_ps_reserve[7] = 0;
  _lazyps_upd_fib1[7] = 1;
}
_lazyps_usedReservation_fib1[7] = 0;
if(_lazyps_usedReservation_fib1[8] == 0 && fib1_ps_reserve[8] == ps_thread_num){;
  fib1_ps_reserve[8] = 0;
  _lazyps_upd_fib1[8] = 1;
}
_lazyps_usedReservation_fib1[8] = 0;
  __CPROVER_assume(fib2_liveChain[0] == 0);
  __CPROVER_assume(fib2_liveChain[0] == 0);
if(_lazyps_usedReservation_fib2[0] == 0 && fib2_ps_reserve[0] == ps_thread_num){;
  fib2_ps_reserve[0] = 0;
  _lazyps_upd_fib2[0] = 1;
}
_lazyps_usedReservation_fib2[0] = 0;
if(_lazyps_usedReservation_fib2[1] == 0 && fib2_ps_reserve[1] == ps_thread_num){;
  fib2_ps_reserve[1] = 0;
  _lazyps_upd_fib2[1] = 1;
}
_lazyps_usedReservation_fib2[1] = 0;
if(_lazyps_usedReservation_fib2[2] == 0 && fib2_ps_reserve[2] == ps_thread_num){;
  fib2_ps_reserve[2] = 0;
  _lazyps_upd_fib2[2] = 1;
}
_lazyps_usedReservation_fib2[2] = 0;
if(_lazyps_usedReservation_fib2[3] == 0 && fib2_ps_reserve[3] == ps_thread_num){;
  fib2_ps_reserve[3] = 0;
  _lazyps_upd_fib2[3] = 1;
}
_lazyps_usedReservation_fib2[3] = 0;
if(_lazyps_usedReservation_fib2[4] == 0 && fib2_ps_reserve[4] == ps_thread_num){;
  fib2_ps_reserve[4] = 0;
  _lazyps_upd_fib2[4] = 1;
}
_lazyps_usedReservation_fib2[4] = 0;
if(_lazyps_usedReservation_fib2[5] == 0 && fib2_ps_reserve[5] == ps_thread_num){;
  fib2_ps_reserve[5] = 0;
  _lazyps_upd_fib2[5] = 1;
}
_lazyps_usedReservation_fib2[5] = 0;
if(_lazyps_usedReservation_fib2[6] == 0 && fib2_ps_reserve[6] == ps_thread_num){;
  fib2_ps_reserve[6] = 0;
  _lazyps_upd_fib2[6] = 1;
}
_lazyps_usedReservation_fib2[6] = 0;
if(_lazyps_usedReservation_fib2[7] == 0 && fib2_ps_reserve[7] == ps_thread_num){;
  fib2_ps_reserve[7] = 0;
  _lazyps_upd_fib2[7] = 1;
}
_lazyps_usedReservation_fib2[7] = 0;
if(_lazyps_usedReservation_fib2[8] == 0 && fib2_ps_reserve[8] == ps_thread_num){;
  fib2_ps_reserve[8] = 0;
  _lazyps_upd_fib2[8] = 1;
}
_lazyps_usedReservation_fib2[8] = 0;
;
load_state_ps(ps_thread_num);
;
_lazyps_active[ps_thread_num] = 0;
;
            }
            __CSEQ_assume( __cs_pc_cs[3] >= 4 );;
if (_lazyps_active[ps_thread_num] == 0)
            {
__CSEQ_rawline("tt0_0_4: IF(3,4,tt0_0_5)");
;
_lazyps_active[ps_thread_num] = 1;
            }
            __CSEQ_assume( __cs_pc_cs[3] >= 5 );;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t0__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t0__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t0__ps_non_det_int_var] == 4);
 __CPROVER_assume((fib1_view_t[ps_thread_num] < fib1_ps_time[__cs_local_t0__ps_non_det_int_var]) || ((fib1_view_t[ps_thread_num] == fib1_ps_time[__cs_local_t0__ps_non_det_int_var]) && fib1_view_l[ps_thread_num]));
 fib1_view_t[ps_thread_num] = fib1_ps_time[__cs_local_t0__ps_non_det_int_var];
 fib1_view_l[ps_thread_num] = 1;
 fib1_view[ps_thread_num] = fib1_values[__cs_local_t0__ps_non_det_int_var];
 fib1_extView[0] = 1;
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib1_view_v[ps_thread_num]);
}
__cs_local_t0__n = fib1_view[ps_thread_num];
;
if (nondet_bool())
            {
if (_lazyps_active[ps_thread_num] == 0)
                {
__CSEQ_rawline("tt0_0_5: IF(3,5,tt0_0_6)");
;
_lazyps_active[ps_thread_num] = 1;
                }
                __CSEQ_assume( __cs_pc_cs[3] >= 6 );;
_lazyps_retAddr[ps_thread_num] = 2;
save_state_ps(ps_thread_num);
;
__cs_pscopy_t0___lv = __cs_local_t0___lv;
__cs_pscopy_t0__n = __cs_local_t0__n;
t0_pscopy(ps_thread_num);
      __CPROVER_assume(_ps_flag[0] != ps_thread_num);
      if(_ps_flag[0] == 255){
          _ps_flag[0] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[1] != ps_thread_num);
      if(_ps_flag[1] == 255){
          _ps_flag[1] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[2] != ps_thread_num);
      if(_ps_flag[2] == 255){
          _ps_flag[2] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[3] != ps_thread_num);
      if(_ps_flag[3] == 255){
          _ps_flag[3] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[4] != ps_thread_num);
      if(_ps_flag[4] == 255){
          _ps_flag[4] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[5] != ps_thread_num);
      if(_ps_flag[5] == 255){
          _ps_flag[5] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[6] != ps_thread_num);
      if(_ps_flag[6] == 255){
          _ps_flag[6] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[7] != ps_thread_num);
      if(_ps_flag[7] == 255){
          _ps_flag[7] = ps_thread_num;
      }
  __CPROVER_assume(var0_liveChain[0] == 0);
  __CPROVER_assume(var0_liveChain[0] == 0);
if(_lazyps_usedReservation_var0[0] == 0 && var0_ps_reserve[0] == ps_thread_num){;
  var0_ps_reserve[0] = 0;
  _lazyps_upd_var0[0] = 1;
}
_lazyps_usedReservation_var0[0] = 0;
if(_lazyps_usedReservation_var0[1] == 0 && var0_ps_reserve[1] == ps_thread_num){;
  var0_ps_reserve[1] = 0;
  _lazyps_upd_var0[1] = 1;
}
_lazyps_usedReservation_var0[1] = 0;
if(_lazyps_usedReservation_var0[2] == 0 && var0_ps_reserve[2] == ps_thread_num){;
  var0_ps_reserve[2] = 0;
  _lazyps_upd_var0[2] = 1;
}
_lazyps_usedReservation_var0[2] = 0;
if(_lazyps_usedReservation_var0[3] == 0 && var0_ps_reserve[3] == ps_thread_num){;
  var0_ps_reserve[3] = 0;
  _lazyps_upd_var0[3] = 1;
}
_lazyps_usedReservation_var0[3] = 0;
if(_lazyps_usedReservation_var0[4] == 0 && var0_ps_reserve[4] == ps_thread_num){;
  var0_ps_reserve[4] = 0;
  _lazyps_upd_var0[4] = 1;
}
_lazyps_usedReservation_var0[4] = 0;
if(_lazyps_usedReservation_var0[5] == 0 && var0_ps_reserve[5] == ps_thread_num){;
  var0_ps_reserve[5] = 0;
  _lazyps_upd_var0[5] = 1;
}
_lazyps_usedReservation_var0[5] = 0;
if(_lazyps_usedReservation_var0[6] == 0 && var0_ps_reserve[6] == ps_thread_num){;
  var0_ps_reserve[6] = 0;
  _lazyps_upd_var0[6] = 1;
}
_lazyps_usedReservation_var0[6] = 0;
if(_lazyps_usedReservation_var0[7] == 0 && var0_ps_reserve[7] == ps_thread_num){;
  var0_ps_reserve[7] = 0;
  _lazyps_upd_var0[7] = 1;
}
_lazyps_usedReservation_var0[7] = 0;
if(_lazyps_usedReservation_var0[8] == 0 && var0_ps_reserve[8] == ps_thread_num){;
  var0_ps_reserve[8] = 0;
  _lazyps_upd_var0[8] = 1;
}
_lazyps_usedReservation_var0[8] = 0;
  __CPROVER_assume(z1_liveChain[0] == 0);
  __CPROVER_assume(z1_liveChain[0] == 0);
if(_lazyps_usedReservation_z1[0] == 0 && z1_ps_reserve[0] == ps_thread_num){;
  z1_ps_reserve[0] = 0;
  _lazyps_upd_z1[0] = 1;
}
_lazyps_usedReservation_z1[0] = 0;
if(_lazyps_usedReservation_z1[1] == 0 && z1_ps_reserve[1] == ps_thread_num){;
  z1_ps_reserve[1] = 0;
  _lazyps_upd_z1[1] = 1;
}
_lazyps_usedReservation_z1[1] = 0;
if(_lazyps_usedReservation_z1[2] == 0 && z1_ps_reserve[2] == ps_thread_num){;
  z1_ps_reserve[2] = 0;
  _lazyps_upd_z1[2] = 1;
}
_lazyps_usedReservation_z1[2] = 0;
if(_lazyps_usedReservation_z1[3] == 0 && z1_ps_reserve[3] == ps_thread_num){;
  z1_ps_reserve[3] = 0;
  _lazyps_upd_z1[3] = 1;
}
_lazyps_usedReservation_z1[3] = 0;
if(_lazyps_usedReservation_z1[4] == 0 && z1_ps_reserve[4] == ps_thread_num){;
  z1_ps_reserve[4] = 0;
  _lazyps_upd_z1[4] = 1;
}
_lazyps_usedReservation_z1[4] = 0;
if(_lazyps_usedReservation_z1[5] == 0 && z1_ps_reserve[5] == ps_thread_num){;
  z1_ps_reserve[5] = 0;
  _lazyps_upd_z1[5] = 1;
}
_lazyps_usedReservation_z1[5] = 0;
if(_lazyps_usedReservation_z1[6] == 0 && z1_ps_reserve[6] == ps_thread_num){;
  z1_ps_reserve[6] = 0;
  _lazyps_upd_z1[6] = 1;
}
_lazyps_usedReservation_z1[6] = 0;
if(_lazyps_usedReservation_z1[7] == 0 && z1_ps_reserve[7] == ps_thread_num){;
  z1_ps_reserve[7] = 0;
  _lazyps_upd_z1[7] = 1;
}
_lazyps_usedReservation_z1[7] = 0;
if(_lazyps_usedReservation_z1[8] == 0 && z1_ps_reserve[8] == ps_thread_num){;
  z1_ps_reserve[8] = 0;
  _lazyps_upd_z1[8] = 1;
}
_lazyps_usedReservation_z1[8] = 0;
  __CPROVER_assume(casvar_liveChain[0] == 0);
  __CPROVER_assume(casvar_liveChain[0] == 0);
if(_lazyps_usedReservation_casvar[0] == 0 && casvar_ps_reserve[0] == ps_thread_num){;
  casvar_ps_reserve[0] = 0;
  _lazyps_upd_casvar[0] = 1;
}
_lazyps_usedReservation_casvar[0] = 0;
if(_lazyps_usedReservation_casvar[1] == 0 && casvar_ps_reserve[1] == ps_thread_num){;
  casvar_ps_reserve[1] = 0;
  _lazyps_upd_casvar[1] = 1;
}
_lazyps_usedReservation_casvar[1] = 0;
if(_lazyps_usedReservation_casvar[2] == 0 && casvar_ps_reserve[2] == ps_thread_num){;
  casvar_ps_reserve[2] = 0;
  _lazyps_upd_casvar[2] = 1;
}
_lazyps_usedReservation_casvar[2] = 0;
if(_lazyps_usedReservation_casvar[3] == 0 && casvar_ps_reserve[3] == ps_thread_num){;
  casvar_ps_reserve[3] = 0;
  _lazyps_upd_casvar[3] = 1;
}
_lazyps_usedReservation_casvar[3] = 0;
if(_lazyps_usedReservation_casvar[4] == 0 && casvar_ps_reserve[4] == ps_thread_num){;
  casvar_ps_reserve[4] = 0;
  _lazyps_upd_casvar[4] = 1;
}
_lazyps_usedReservation_casvar[4] = 0;
if(_lazyps_usedReservation_casvar[5] == 0 && casvar_ps_reserve[5] == ps_thread_num){;
  casvar_ps_reserve[5] = 0;
  _lazyps_upd_casvar[5] = 1;
}
_lazyps_usedReservation_casvar[5] = 0;
if(_lazyps_usedReservation_casvar[6] == 0 && casvar_ps_reserve[6] == ps_thread_num){;
  casvar_ps_reserve[6] = 0;
  _lazyps_upd_casvar[6] = 1;
}
_lazyps_usedReservation_casvar[6] = 0;
if(_lazyps_usedReservation_casvar[7] == 0 && casvar_ps_reserve[7] == ps_thread_num){;
  casvar_ps_reserve[7] = 0;
  _lazyps_upd_casvar[7] = 1;
}
_lazyps_usedReservation_casvar[7] = 0;
if(_lazyps_usedReservation_casvar[8] == 0 && casvar_ps_reserve[8] == ps_thread_num){;
  casvar_ps_reserve[8] = 0;
  _lazyps_upd_casvar[8] = 1;
}
_lazyps_usedReservation_casvar[8] = 0;
  __CPROVER_assume(fib1_liveChain[0] == 0);
  __CPROVER_assume(fib1_liveChain[0] == 0);
if(_lazyps_usedReservation_fib1[0] == 0 && fib1_ps_reserve[0] == ps_thread_num){;
  fib1_ps_reserve[0] = 0;
  _lazyps_upd_fib1[0] = 1;
}
_lazyps_usedReservation_fib1[0] = 0;
if(_lazyps_usedReservation_fib1[1] == 0 && fib1_ps_reserve[1] == ps_thread_num){;
  fib1_ps_reserve[1] = 0;
  _lazyps_upd_fib1[1] = 1;
}
_lazyps_usedReservation_fib1[1] = 0;
if(_lazyps_usedReservation_fib1[2] == 0 && fib1_ps_reserve[2] == ps_thread_num){;
  fib1_ps_reserve[2] = 0;
  _lazyps_upd_fib1[2] = 1;
}
_lazyps_usedReservation_fib1[2] = 0;
if(_lazyps_usedReservation_fib1[3] == 0 && fib1_ps_reserve[3] == ps_thread_num){;
  fib1_ps_reserve[3] = 0;
  _lazyps_upd_fib1[3] = 1;
}
_lazyps_usedReservation_fib1[3] = 0;
if(_lazyps_usedReservation_fib1[4] == 0 && fib1_ps_reserve[4] == ps_thread_num){;
  fib1_ps_reserve[4] = 0;
  _lazyps_upd_fib1[4] = 1;
}
_lazyps_usedReservation_fib1[4] = 0;
if(_lazyps_usedReservation_fib1[5] == 0 && fib1_ps_reserve[5] == ps_thread_num){;
  fib1_ps_reserve[5] = 0;
  _lazyps_upd_fib1[5] = 1;
}
_lazyps_usedReservation_fib1[5] = 0;
if(_lazyps_usedReservation_fib1[6] == 0 && fib1_ps_reserve[6] == ps_thread_num){;
  fib1_ps_reserve[6] = 0;
  _lazyps_upd_fib1[6] = 1;
}
_lazyps_usedReservation_fib1[6] = 0;
if(_lazyps_usedReservation_fib1[7] == 0 && fib1_ps_reserve[7] == ps_thread_num){;
  fib1_ps_reserve[7] = 0;
  _lazyps_upd_fib1[7] = 1;
}
_lazyps_usedReservation_fib1[7] = 0;
if(_lazyps_usedReservation_fib1[8] == 0 && fib1_ps_reserve[8] == ps_thread_num){;
  fib1_ps_reserve[8] = 0;
  _lazyps_upd_fib1[8] = 1;
}
_lazyps_usedReservation_fib1[8] = 0;
  __CPROVER_assume(fib2_liveChain[0] == 0);
  __CPROVER_assume(fib2_liveChain[0] == 0);
if(_lazyps_usedReservation_fib2[0] == 0 && fib2_ps_reserve[0] == ps_thread_num){;
  fib2_ps_reserve[0] = 0;
  _lazyps_upd_fib2[0] = 1;
}
_lazyps_usedReservation_fib2[0] = 0;
if(_lazyps_usedReservation_fib2[1] == 0 && fib2_ps_reserve[1] == ps_thread_num){;
  fib2_ps_reserve[1] = 0;
  _lazyps_upd_fib2[1] = 1;
}
_lazyps_usedReservation_fib2[1] = 0;
if(_lazyps_usedReservation_fib2[2] == 0 && fib2_ps_reserve[2] == ps_thread_num){;
  fib2_ps_reserve[2] = 0;
  _lazyps_upd_fib2[2] = 1;
}
_lazyps_usedReservation_fib2[2] = 0;
if(_lazyps_usedReservation_fib2[3] == 0 && fib2_ps_reserve[3] == ps_thread_num){;
  fib2_ps_reserve[3] = 0;
  _lazyps_upd_fib2[3] = 1;
}
_lazyps_usedReservation_fib2[3] = 0;
if(_lazyps_usedReservation_fib2[4] == 0 && fib2_ps_reserve[4] == ps_thread_num){;
  fib2_ps_reserve[4] = 0;
  _lazyps_upd_fib2[4] = 1;
}
_lazyps_usedReservation_fib2[4] = 0;
if(_lazyps_usedReservation_fib2[5] == 0 && fib2_ps_reserve[5] == ps_thread_num){;
  fib2_ps_reserve[5] = 0;
  _lazyps_upd_fib2[5] = 1;
}
_lazyps_usedReservation_fib2[5] = 0;
if(_lazyps_usedReservation_fib2[6] == 0 && fib2_ps_reserve[6] == ps_thread_num){;
  fib2_ps_reserve[6] = 0;
  _lazyps_upd_fib2[6] = 1;
}
_lazyps_usedReservation_fib2[6] = 0;
if(_lazyps_usedReservation_fib2[7] == 0 && fib2_ps_reserve[7] == ps_thread_num){;
  fib2_ps_reserve[7] = 0;
  _lazyps_upd_fib2[7] = 1;
}
_lazyps_usedReservation_fib2[7] = 0;
if(_lazyps_usedReservation_fib2[8] == 0 && fib2_ps_reserve[8] == ps_thread_num){;
  fib2_ps_reserve[8] = 0;
  _lazyps_upd_fib2[8] = 1;
}
_lazyps_usedReservation_fib2[8] = 0;
;
load_state_ps(ps_thread_num);
;
_lazyps_active[ps_thread_num] = 0;
;
            }
            __CSEQ_assume( __cs_pc_cs[3] >= 6 );;
if (_lazyps_active[ps_thread_num] == 0)
            {
__CSEQ_rawline("tt0_0_6: IF(3,6,tt0_0_7)");
;
_lazyps_active[ps_thread_num] = 1;
            }
            __CSEQ_assume( __cs_pc_cs[3] >= 7 );;
var0_view[ps_thread_num] = __cs_local_t0__n;
var0_view_v[ps_thread_num] = 1;
if(nondet_int()){
  var0_view_l[ps_thread_num] = 0;
}
else if(nondet_bool()){
    var0_view_l[ps_thread_num] = 1;
 __cs_local_t0__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t0__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t0__ps_non_det_int_var > var0_view_t[ps_thread_num]);
    var0_view_t[ps_thread_num] = __cs_local_t0__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_var0[__cs_local_t0__ps_non_det_int_var]);
      _lazyps_unavail_var0[__cs_local_t0__ps_non_det_int_var] = 1;
      _lazyps_upd_var0[__cs_local_t0__ps_non_det_int_var] = 1;
    if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
 _ps_flag[__cs_local_t0__ps_non_det_int_var] = 0;
         var0_ps_time[_lazyps_index_used] = var0_view_t[ps_thread_num];
         var0_ps_timeL[_lazyps_index_used] = var0_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 1;
         var0_values[_lazyps_index_used] = var0_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
    }
}
else{
 var0_view_l[ps_thread_num] = 1;
 __cs_local_t0__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t0__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t0__ps_non_det_int_var] == 1);
 __CPROVER_assume(var0_view_t[ps_thread_num] < var0_ps_time[__cs_local_t0__ps_non_det_int_var]);
 __CPROVER_assume(_ps_flag[__cs_local_t0__ps_non_det_int_var] == ps_thread_num);
 __CPROVER_assume(var0_values[__cs_local_t0__ps_non_det_int_var] == var0_view[ps_thread_num]);
 _ps_flag[__cs_local_t0__ps_non_det_int_var] = 0;
var0_view_t[ps_thread_num] = var0_ps_time[__cs_local_t0__ps_non_det_int_var];
}
 var0_extView[0] = 1;
;
        }
        __CSEQ_assume( __cs_pc_cs[3] >= 7 );;
if (_lazyps_active[ps_thread_num] == 1)
        {
      __CPROVER_assume(_ps_flag[0] != ps_thread_num);
      if(_ps_flag[0] == 255){
          _ps_flag[0] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[1] != ps_thread_num);
      if(_ps_flag[1] == 255){
          _ps_flag[1] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[2] != ps_thread_num);
      if(_ps_flag[2] == 255){
          _ps_flag[2] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[3] != ps_thread_num);
      if(_ps_flag[3] == 255){
          _ps_flag[3] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[4] != ps_thread_num);
      if(_ps_flag[4] == 255){
          _ps_flag[4] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[5] != ps_thread_num);
      if(_ps_flag[5] == 255){
          _ps_flag[5] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[6] != ps_thread_num);
      if(_ps_flag[6] == 255){
          _ps_flag[6] = ps_thread_num;
      }
      __CPROVER_assume(_ps_flag[7] != ps_thread_num);
      if(_ps_flag[7] == 255){
          _ps_flag[7] = ps_thread_num;
      }
  __CPROVER_assume(var0_liveChain[0] == 0);
  __CPROVER_assume(var0_liveChain[0] == 0);
if(_lazyps_usedReservation_var0[0] == 0 && var0_ps_reserve[0] == ps_thread_num){;
  var0_ps_reserve[0] = 0;
  _lazyps_upd_var0[0] = 1;
}
_lazyps_usedReservation_var0[0] = 0;
if(_lazyps_usedReservation_var0[1] == 0 && var0_ps_reserve[1] == ps_thread_num){;
  var0_ps_reserve[1] = 0;
  _lazyps_upd_var0[1] = 1;
}
_lazyps_usedReservation_var0[1] = 0;
if(_lazyps_usedReservation_var0[2] == 0 && var0_ps_reserve[2] == ps_thread_num){;
  var0_ps_reserve[2] = 0;
  _lazyps_upd_var0[2] = 1;
}
_lazyps_usedReservation_var0[2] = 0;
if(_lazyps_usedReservation_var0[3] == 0 && var0_ps_reserve[3] == ps_thread_num){;
  var0_ps_reserve[3] = 0;
  _lazyps_upd_var0[3] = 1;
}
_lazyps_usedReservation_var0[3] = 0;
if(_lazyps_usedReservation_var0[4] == 0 && var0_ps_reserve[4] == ps_thread_num){;
  var0_ps_reserve[4] = 0;
  _lazyps_upd_var0[4] = 1;
}
_lazyps_usedReservation_var0[4] = 0;
if(_lazyps_usedReservation_var0[5] == 0 && var0_ps_reserve[5] == ps_thread_num){;
  var0_ps_reserve[5] = 0;
  _lazyps_upd_var0[5] = 1;
}
_lazyps_usedReservation_var0[5] = 0;
if(_lazyps_usedReservation_var0[6] == 0 && var0_ps_reserve[6] == ps_thread_num){;
  var0_ps_reserve[6] = 0;
  _lazyps_upd_var0[6] = 1;
}
_lazyps_usedReservation_var0[6] = 0;
if(_lazyps_usedReservation_var0[7] == 0 && var0_ps_reserve[7] == ps_thread_num){;
  var0_ps_reserve[7] = 0;
  _lazyps_upd_var0[7] = 1;
}
_lazyps_usedReservation_var0[7] = 0;
if(_lazyps_usedReservation_var0[8] == 0 && var0_ps_reserve[8] == ps_thread_num){;
  var0_ps_reserve[8] = 0;
  _lazyps_upd_var0[8] = 1;
}
_lazyps_usedReservation_var0[8] = 0;
  __CPROVER_assume(z1_liveChain[0] == 0);
  __CPROVER_assume(z1_liveChain[0] == 0);
if(_lazyps_usedReservation_z1[0] == 0 && z1_ps_reserve[0] == ps_thread_num){;
  z1_ps_reserve[0] = 0;
  _lazyps_upd_z1[0] = 1;
}
_lazyps_usedReservation_z1[0] = 0;
if(_lazyps_usedReservation_z1[1] == 0 && z1_ps_reserve[1] == ps_thread_num){;
  z1_ps_reserve[1] = 0;
  _lazyps_upd_z1[1] = 1;
}
_lazyps_usedReservation_z1[1] = 0;
if(_lazyps_usedReservation_z1[2] == 0 && z1_ps_reserve[2] == ps_thread_num){;
  z1_ps_reserve[2] = 0;
  _lazyps_upd_z1[2] = 1;
}
_lazyps_usedReservation_z1[2] = 0;
if(_lazyps_usedReservation_z1[3] == 0 && z1_ps_reserve[3] == ps_thread_num){;
  z1_ps_reserve[3] = 0;
  _lazyps_upd_z1[3] = 1;
}
_lazyps_usedReservation_z1[3] = 0;
if(_lazyps_usedReservation_z1[4] == 0 && z1_ps_reserve[4] == ps_thread_num){;
  z1_ps_reserve[4] = 0;
  _lazyps_upd_z1[4] = 1;
}
_lazyps_usedReservation_z1[4] = 0;
if(_lazyps_usedReservation_z1[5] == 0 && z1_ps_reserve[5] == ps_thread_num){;
  z1_ps_reserve[5] = 0;
  _lazyps_upd_z1[5] = 1;
}
_lazyps_usedReservation_z1[5] = 0;
if(_lazyps_usedReservation_z1[6] == 0 && z1_ps_reserve[6] == ps_thread_num){;
  z1_ps_reserve[6] = 0;
  _lazyps_upd_z1[6] = 1;
}
_lazyps_usedReservation_z1[6] = 0;
if(_lazyps_usedReservation_z1[7] == 0 && z1_ps_reserve[7] == ps_thread_num){;
  z1_ps_reserve[7] = 0;
  _lazyps_upd_z1[7] = 1;
}
_lazyps_usedReservation_z1[7] = 0;
if(_lazyps_usedReservation_z1[8] == 0 && z1_ps_reserve[8] == ps_thread_num){;
  z1_ps_reserve[8] = 0;
  _lazyps_upd_z1[8] = 1;
}
_lazyps_usedReservation_z1[8] = 0;
  __CPROVER_assume(casvar_liveChain[0] == 0);
  __CPROVER_assume(casvar_liveChain[0] == 0);
if(_lazyps_usedReservation_casvar[0] == 0 && casvar_ps_reserve[0] == ps_thread_num){;
  casvar_ps_reserve[0] = 0;
  _lazyps_upd_casvar[0] = 1;
}
_lazyps_usedReservation_casvar[0] = 0;
if(_lazyps_usedReservation_casvar[1] == 0 && casvar_ps_reserve[1] == ps_thread_num){;
  casvar_ps_reserve[1] = 0;
  _lazyps_upd_casvar[1] = 1;
}
_lazyps_usedReservation_casvar[1] = 0;
if(_lazyps_usedReservation_casvar[2] == 0 && casvar_ps_reserve[2] == ps_thread_num){;
  casvar_ps_reserve[2] = 0;
  _lazyps_upd_casvar[2] = 1;
}
_lazyps_usedReservation_casvar[2] = 0;
if(_lazyps_usedReservation_casvar[3] == 0 && casvar_ps_reserve[3] == ps_thread_num){;
  casvar_ps_reserve[3] = 0;
  _lazyps_upd_casvar[3] = 1;
}
_lazyps_usedReservation_casvar[3] = 0;
if(_lazyps_usedReservation_casvar[4] == 0 && casvar_ps_reserve[4] == ps_thread_num){;
  casvar_ps_reserve[4] = 0;
  _lazyps_upd_casvar[4] = 1;
}
_lazyps_usedReservation_casvar[4] = 0;
if(_lazyps_usedReservation_casvar[5] == 0 && casvar_ps_reserve[5] == ps_thread_num){;
  casvar_ps_reserve[5] = 0;
  _lazyps_upd_casvar[5] = 1;
}
_lazyps_usedReservation_casvar[5] = 0;
if(_lazyps_usedReservation_casvar[6] == 0 && casvar_ps_reserve[6] == ps_thread_num){;
  casvar_ps_reserve[6] = 0;
  _lazyps_upd_casvar[6] = 1;
}
_lazyps_usedReservation_casvar[6] = 0;
if(_lazyps_usedReservation_casvar[7] == 0 && casvar_ps_reserve[7] == ps_thread_num){;
  casvar_ps_reserve[7] = 0;
  _lazyps_upd_casvar[7] = 1;
}
_lazyps_usedReservation_casvar[7] = 0;
if(_lazyps_usedReservation_casvar[8] == 0 && casvar_ps_reserve[8] == ps_thread_num){;
  casvar_ps_reserve[8] = 0;
  _lazyps_upd_casvar[8] = 1;
}
_lazyps_usedReservation_casvar[8] = 0;
  __CPROVER_assume(fib1_liveChain[0] == 0);
  __CPROVER_assume(fib1_liveChain[0] == 0);
if(_lazyps_usedReservation_fib1[0] == 0 && fib1_ps_reserve[0] == ps_thread_num){;
  fib1_ps_reserve[0] = 0;
  _lazyps_upd_fib1[0] = 1;
}
_lazyps_usedReservation_fib1[0] = 0;
if(_lazyps_usedReservation_fib1[1] == 0 && fib1_ps_reserve[1] == ps_thread_num){;
  fib1_ps_reserve[1] = 0;
  _lazyps_upd_fib1[1] = 1;
}
_lazyps_usedReservation_fib1[1] = 0;
if(_lazyps_usedReservation_fib1[2] == 0 && fib1_ps_reserve[2] == ps_thread_num){;
  fib1_ps_reserve[2] = 0;
  _lazyps_upd_fib1[2] = 1;
}
_lazyps_usedReservation_fib1[2] = 0;
if(_lazyps_usedReservation_fib1[3] == 0 && fib1_ps_reserve[3] == ps_thread_num){;
  fib1_ps_reserve[3] = 0;
  _lazyps_upd_fib1[3] = 1;
}
_lazyps_usedReservation_fib1[3] = 0;
if(_lazyps_usedReservation_fib1[4] == 0 && fib1_ps_reserve[4] == ps_thread_num){;
  fib1_ps_reserve[4] = 0;
  _lazyps_upd_fib1[4] = 1;
}
_lazyps_usedReservation_fib1[4] = 0;
if(_lazyps_usedReservation_fib1[5] == 0 && fib1_ps_reserve[5] == ps_thread_num){;
  fib1_ps_reserve[5] = 0;
  _lazyps_upd_fib1[5] = 1;
}
_lazyps_usedReservation_fib1[5] = 0;
if(_lazyps_usedReservation_fib1[6] == 0 && fib1_ps_reserve[6] == ps_thread_num){;
  fib1_ps_reserve[6] = 0;
  _lazyps_upd_fib1[6] = 1;
}
_lazyps_usedReservation_fib1[6] = 0;
if(_lazyps_usedReservation_fib1[7] == 0 && fib1_ps_reserve[7] == ps_thread_num){;
  fib1_ps_reserve[7] = 0;
  _lazyps_upd_fib1[7] = 1;
}
_lazyps_usedReservation_fib1[7] = 0;
if(_lazyps_usedReservation_fib1[8] == 0 && fib1_ps_reserve[8] == ps_thread_num){;
  fib1_ps_reserve[8] = 0;
  _lazyps_upd_fib1[8] = 1;
}
_lazyps_usedReservation_fib1[8] = 0;
  __CPROVER_assume(fib2_liveChain[0] == 0);
  __CPROVER_assume(fib2_liveChain[0] == 0);
if(_lazyps_usedReservation_fib2[0] == 0 && fib2_ps_reserve[0] == ps_thread_num){;
  fib2_ps_reserve[0] = 0;
  _lazyps_upd_fib2[0] = 1;
}
_lazyps_usedReservation_fib2[0] = 0;
if(_lazyps_usedReservation_fib2[1] == 0 && fib2_ps_reserve[1] == ps_thread_num){;
  fib2_ps_reserve[1] = 0;
  _lazyps_upd_fib2[1] = 1;
}
_lazyps_usedReservation_fib2[1] = 0;
if(_lazyps_usedReservation_fib2[2] == 0 && fib2_ps_reserve[2] == ps_thread_num){;
  fib2_ps_reserve[2] = 0;
  _lazyps_upd_fib2[2] = 1;
}
_lazyps_usedReservation_fib2[2] = 0;
if(_lazyps_usedReservation_fib2[3] == 0 && fib2_ps_reserve[3] == ps_thread_num){;
  fib2_ps_reserve[3] = 0;
  _lazyps_upd_fib2[3] = 1;
}
_lazyps_usedReservation_fib2[3] = 0;
if(_lazyps_usedReservation_fib2[4] == 0 && fib2_ps_reserve[4] == ps_thread_num){;
  fib2_ps_reserve[4] = 0;
  _lazyps_upd_fib2[4] = 1;
}
_lazyps_usedReservation_fib2[4] = 0;
if(_lazyps_usedReservation_fib2[5] == 0 && fib2_ps_reserve[5] == ps_thread_num){;
  fib2_ps_reserve[5] = 0;
  _lazyps_upd_fib2[5] = 1;
}
_lazyps_usedReservation_fib2[5] = 0;
if(_lazyps_usedReservation_fib2[6] == 0 && fib2_ps_reserve[6] == ps_thread_num){;
  fib2_ps_reserve[6] = 0;
  _lazyps_upd_fib2[6] = 1;
}
_lazyps_usedReservation_fib2[6] = 0;
if(_lazyps_usedReservation_fib2[7] == 0 && fib2_ps_reserve[7] == ps_thread_num){;
  fib2_ps_reserve[7] = 0;
  _lazyps_upd_fib2[7] = 1;
}
_lazyps_usedReservation_fib2[7] = 0;
if(_lazyps_usedReservation_fib2[8] == 0 && fib2_ps_reserve[8] == ps_thread_num){;
  fib2_ps_reserve[8] = 0;
  _lazyps_upd_fib2[8] = 1;
}
_lazyps_usedReservation_fib2[8] = 0;
;
_lazyps_active[ps_thread_num] = 0;
;
        }
        ;
        __exit_t0: __CSEQ_assume( __cs_pc_cs[3] >= 7 );
;;
__CSEQ_rawline("tt0_0_7: ");
__cs_exit(0, 3);
    }
;
}
void t0_pscopy(int __cs_param_t0_pscopy__ps_t1)
{
static unsigned int __cs_local_t0_pscopy__ps_non_det_int_var;
static _Bool __cs_local_t0_pscopy__ps_non_det_bool_var;
static int __cs_local_t0_pscopy___lv;
__cs_local_t0_pscopy___lv = __cs_pscopy_t0___lv;
static int __cs_local_t0_pscopy__n;
__cs_local_t0_pscopy__n = __cs_pscopy_t0__n;
int __cs_local_t0_pscopy__int_update_helper_var;
if (_lazyps_retAddr[__cs_param_t0_pscopy__ps_t1] < 0)
    {
    }
    else
if (_lazyps_retAddr[__cs_param_t0_pscopy__ps_t1] == 0)
    { 
goto t0_pscopy_pslabel_0;;
    }
    else
if (_lazyps_retAddr[__cs_param_t0_pscopy__ps_t1] == 1)
    { 
goto t0_pscopy_pslabel_1;;
    }
    else
if (_lazyps_retAddr[__cs_param_t0_pscopy__ps_t1] == 2)
    { 
goto t0_pscopy_pslabel_2;;
    }
    ;
    {
static int __cs_local_t0_pscopy___lv;
__cs_local_t0_pscopy___lv = 0;
static int __cs_local_t0_pscopy__n;
__cs_local_t0_pscopy__n = __CSEQ_nondet_int();
        t0_pscopy_pslabel_0: ;;
if (nondet_bool())
        {
goto __exit_t0;;
        }
        ;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(z1_liveChain[0] == 0);
__cs_local_t0_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t0_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t0_pscopy__ps_non_det_int_var] == 2);
 __CPROVER_assume((z1_view_t[__cs_param_t0_pscopy__ps_t1] < z1_ps_time[__cs_local_t0_pscopy__ps_non_det_int_var]) || ((z1_view_t[__cs_param_t0_pscopy__ps_t1] == z1_ps_time[__cs_local_t0_pscopy__ps_non_det_int_var]) && z1_view_l[__cs_param_t0_pscopy__ps_t1]));
 z1_view_t[__cs_param_t0_pscopy__ps_t1] = z1_ps_time[__cs_local_t0_pscopy__ps_non_det_int_var];
 z1_view_l[__cs_param_t0_pscopy__ps_t1] = 1;
 z1_view[__cs_param_t0_pscopy__ps_t1] = z1_values[__cs_local_t0_pscopy__ps_non_det_int_var];
 z1_extView[0] = 1;
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(z1_view_v[__cs_param_t0_pscopy__ps_t1]);
}
__cs_local_t0_pscopy___lv = z1_view[__cs_param_t0_pscopy__ps_t1];
;
if (__cs_local_t0_pscopy___lv == 1)
        {
;
        }
        else
        { 
            t0_pscopy_pslabel_1: ;;
if (nondet_bool())
            {
goto __exit_t0;;
            }
            ;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(fib1_liveChain[0] == 0);
__cs_local_t0_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t0_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t0_pscopy__ps_non_det_int_var] == 4);
 __CPROVER_assume((fib1_view_t[__cs_param_t0_pscopy__ps_t1] < fib1_ps_time[__cs_local_t0_pscopy__ps_non_det_int_var]) || ((fib1_view_t[__cs_param_t0_pscopy__ps_t1] == fib1_ps_time[__cs_local_t0_pscopy__ps_non_det_int_var]) && fib1_view_l[__cs_param_t0_pscopy__ps_t1]));
 fib1_view_t[__cs_param_t0_pscopy__ps_t1] = fib1_ps_time[__cs_local_t0_pscopy__ps_non_det_int_var];
 fib1_view_l[__cs_param_t0_pscopy__ps_t1] = 1;
 fib1_view[__cs_param_t0_pscopy__ps_t1] = fib1_values[__cs_local_t0_pscopy__ps_non_det_int_var];
 fib1_extView[0] = 1;
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib1_view_v[__cs_param_t0_pscopy__ps_t1]);
}
__cs_local_t0_pscopy__n = fib1_view[__cs_param_t0_pscopy__ps_t1];
;
            t0_pscopy_pslabel_2: ;;
if (nondet_bool())
            {
goto __exit_t0;;
            }
            ;
var0_view[__cs_param_t0_pscopy__ps_t1] = __cs_local_t0_pscopy__n;
var0_view_v[__cs_param_t0_pscopy__ps_t1] = 1;
if(nondet_int()){
  var0_view_l[__cs_param_t0_pscopy__ps_t1] = 0;
 if(var0_liveChain[0] == 0){
     __cs_local_t0_pscopy__ps_non_det_int_var = nondet_uint();
     __CPROVER_assume(__cs_local_t0_pscopy__ps_non_det_int_var <= _lazyps_rounds);
     __CPROVER_assume(__cs_local_t0_pscopy__ps_non_det_int_var >= var0_view_t[__cs_param_t0_pscopy__ps_t1]);
     var0_view_t[__cs_param_t0_pscopy__ps_t1] = __cs_local_t0_pscopy__ps_non_det_int_var;
     if(nondet_bool()){
         var0_liveChain[0] = 1;
     }
     else{
         if(var0_ps_reserve[__cs_local_t0_pscopy__ps_non_det_int_var] != __cs_param_t0_pscopy__ps_t1){
             __CPROVER_assume(_lazyps_upd_var0[__cs_local_t0_pscopy__ps_non_det_int_var]);
             __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
             _lazyps_numEE = _lazyps_numEE + 1;
             var0_ps_reserve[__cs_local_t0_pscopy__ps_non_det_int_var] = __cs_param_t0_pscopy__ps_t1;
             _lazyps_upd_var0[__cs_local_t0_pscopy__ps_non_det_int_var] = 0;
         }
         _lazyps_usedReservation_var0[__cs_local_t0_pscopy__ps_non_det_int_var] = 1;
     }
 }
}
else if(nondet_bool()){
    var0_view_l[__cs_param_t0_pscopy__ps_t1] = 1;
 if(var0_liveChain[0] == 1){
     __cs_local_t0_pscopy__ps_non_det_int_var = var0_view_t[__cs_param_t0_pscopy__ps_t1] + 1;
 }
 else{
     __cs_local_t0_pscopy__ps_non_det_int_var = nondet_uint();
 }
    __CPROVER_assume(__cs_local_t0_pscopy__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t0_pscopy__ps_non_det_int_var > var0_view_t[__cs_param_t0_pscopy__ps_t1]);
    var0_view_t[__cs_param_t0_pscopy__ps_t1] = __cs_local_t0_pscopy__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_var0[__cs_local_t0_pscopy__ps_non_det_int_var]);
      _lazyps_unavail_var0[__cs_local_t0_pscopy__ps_non_det_int_var] = 1;
      _lazyps_upd_var0[__cs_local_t0_pscopy__ps_non_det_int_var] = 1;
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
 __CPROVER_assume(!var0_blockPromise[0]);
 var0_liveChain[0] = 0;
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 _lazyps_numEE = _lazyps_numEE + 1;
 _ps_flag[__cs_local_t0_pscopy__ps_non_det_int_var] = 255;
         var0_ps_time[_lazyps_index_used] = var0_view_t[__cs_param_t0_pscopy__ps_t1];
         var0_ps_timeL[_lazyps_index_used] = var0_view_l[__cs_param_t0_pscopy__ps_t1];
         _ps_var[_lazyps_index_used] = 1;
         var0_values[_lazyps_index_used] = var0_view[__cs_param_t0_pscopy__ps_t1];
         _lazyps_index_used = _lazyps_index_used + 1;
}
else{
 __CPROVER_assume(!var0_blockPromise[0]);
 var0_view_l[__cs_param_t0_pscopy__ps_t1] = 1;
 __cs_local_t0_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t0_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t0_pscopy__ps_non_det_int_var] == 1);
 __CPROVER_assume(var0_view_t[__cs_param_t0_pscopy__ps_t1] < var0_ps_time[__cs_local_t0_pscopy__ps_non_det_int_var]);
 __CPROVER_assume(_ps_flag[__cs_local_t0_pscopy__ps_non_det_int_var] == __cs_param_t0_pscopy__ps_t1);
 __CPROVER_assume(var0_values[__cs_local_t0_pscopy__ps_non_det_int_var] == var0_view[__cs_param_t0_pscopy__ps_t1]);
 _ps_flag[__cs_local_t0_pscopy__ps_non_det_int_var] = 255;
 __CPROVER_assume((var0_ps_time[__cs_local_t0_pscopy__ps_non_det_int_var] == var0_view_t[__cs_param_t0_pscopy__ps_t1] + 1) || !var0_liveChain[0]);
 var0_liveChain[0] = 0;
var0_view_t[__cs_param_t0_pscopy__ps_t1] = var0_ps_time[__cs_local_t0_pscopy__ps_non_det_int_var];
}
 var0_extView[0] = 0;
;
        }
        ;
        __exit_t0: ;;
return;;
;;
    }
;
}
void *t1_0(void *__cs_param_t1___cs_unused)
{
static unsigned int __cs_local_t1__ps_non_det_int_var;
static _Bool __cs_local_t1__ps_non_det_bool_var;
int __cs_local_t1__int_update_helper_var;
__CSEQ_rawline("tt1_0_0: IF(4,0,tt1_0_1)");
;
    {
static int __cs_local_t1___lv;
__cs_local_t1___lv = __CSEQ_nondet_int();
;
__CSEQ_rawline("tt1_0_1: IF(4,1,tt1_0_2)");
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t1__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t1__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t1__ps_non_det_int_var] == 1);
 __CPROVER_assume((var0_view_t[ps_thread_num] < var0_ps_time[__cs_local_t1__ps_non_det_int_var]) || ((var0_view_t[ps_thread_num] == var0_ps_time[__cs_local_t1__ps_non_det_int_var]) && var0_view_l[ps_thread_num]));
 var0_view_t[ps_thread_num] = var0_ps_time[__cs_local_t1__ps_non_det_int_var];
 var0_view_l[ps_thread_num] = 1;
 var0_view[ps_thread_num] = var0_values[__cs_local_t1__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(var0_view_v[ps_thread_num]);
}
__cs_local_t1___lv = var0_view[ps_thread_num];
;
__CSEQ_assume(__cs_local_t1___lv == 8);
z1_view[ps_thread_num] = 1;
z1_view_v[ps_thread_num] = 1;
if(nondet_bool()){
    z1_view_l[ps_thread_num] = 1;
    __cs_local_t1__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t1__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t1__ps_non_det_int_var > z1_view_t[ps_thread_num]);
    z1_view_t[ps_thread_num] = __cs_local_t1__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_z1[__cs_local_t1__ps_non_det_int_var]);
      _lazyps_unavail_z1[__cs_local_t1__ps_non_det_int_var] = 1;
      _lazyps_upd_z1[__cs_local_t1__ps_non_det_int_var] = 1;
     if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
         z1_ps_time[_lazyps_index_used] = z1_view_t[ps_thread_num];
         z1_ps_timeL[_lazyps_index_used] = z1_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 2;
         z1_values[_lazyps_index_used] = z1_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
     }
}
else{
  z1_view_l[ps_thread_num] = 0;
}
;
;
        __exit_t1: __CSEQ_assume( __cs_pc_cs[4] >= 2 );
;;
__CSEQ_rawline("tt1_0_2: ");
__cs_exit(0, 4);
    }
;
}
void t1_pscopy(int __cs_param_t1_pscopy__ps_t1)
{
static unsigned int __cs_local_t1_pscopy__ps_non_det_int_var;
static _Bool __cs_local_t1_pscopy__ps_non_det_bool_var;
static int __cs_local_t1_pscopy___lv;
__cs_local_t1_pscopy___lv = __cs_pscopy_t1___lv;
int __cs_local_t1_pscopy__int_update_helper_var;
    {
static int __cs_local_t1_pscopy___lv;
__cs_local_t1_pscopy___lv = __CSEQ_nondet_int();
;
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(var0_liveChain[0] == 0);
__cs_local_t1_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t1_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t1_pscopy__ps_non_det_int_var] == 1);
 __CPROVER_assume((var0_view_t[__cs_param_t1_pscopy__ps_t1] < var0_ps_time[__cs_local_t1_pscopy__ps_non_det_int_var]) || ((var0_view_t[__cs_param_t1_pscopy__ps_t1] == var0_ps_time[__cs_local_t1_pscopy__ps_non_det_int_var]) && var0_view_l[__cs_param_t1_pscopy__ps_t1]));
 var0_view_t[__cs_param_t1_pscopy__ps_t1] = var0_ps_time[__cs_local_t1_pscopy__ps_non_det_int_var];
 var0_view_l[__cs_param_t1_pscopy__ps_t1] = 1;
 var0_view[__cs_param_t1_pscopy__ps_t1] = var0_values[__cs_local_t1_pscopy__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(var0_view_v[__cs_param_t1_pscopy__ps_t1]);
}
__cs_local_t1_pscopy___lv = var0_view[__cs_param_t1_pscopy__ps_t1];
;
__CSEQ_assume(__cs_local_t1_pscopy___lv == 8);
z1_view[ps_thread_num] = 1;
z1_view_v[ps_thread_num] = 1;
if(nondet_bool()){
    z1_view_l[ps_thread_num] = 1;
    __cs_local_t1_pscopy__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t1_pscopy__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t1_pscopy__ps_non_det_int_var > z1_view_t[ps_thread_num]);
    z1_view_t[ps_thread_num] = __cs_local_t1_pscopy__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_z1[__cs_local_t1_pscopy__ps_non_det_int_var]);
      _lazyps_unavail_z1[__cs_local_t1_pscopy__ps_non_det_int_var] = 1;
      _lazyps_upd_z1[__cs_local_t1_pscopy__ps_non_det_int_var] = 1;
     if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
         z1_ps_time[_lazyps_index_used] = z1_view_t[ps_thread_num];
         z1_ps_timeL[_lazyps_index_used] = z1_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 2;
         z1_values[_lazyps_index_used] = z1_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
     }
}
else{
  z1_view_l[ps_thread_num] = 0;
}
;
;
        __exit_t1: ;;
return;;
;;
    }
;
}
void *t2_0(void *__cs_param_t2___cs_unused)
{
static unsigned int __cs_local_t2__ps_non_det_int_var;
static _Bool __cs_local_t2__ps_non_det_bool_var;
int __cs_local_t2__int_update_helper_var;
__CSEQ_rawline("tt2_0_0: IF(1,0,tt2_0_1)");
;
    {
static int __cs_local_t2__x;
__cs_local_t2__x = __CSEQ_nondet_int();
static int __cs_local_t2__y;
__cs_local_t2__y = __CSEQ_nondet_int();
static int __cs_local_t2__s;
;
__CSEQ_rawline("tt2_0_1: IF(1,1,tt2_0_2)");
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t2__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t2__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t2__ps_non_det_int_var] == 4);
 __CPROVER_assume((fib1_view_t[ps_thread_num] < fib1_ps_time[__cs_local_t2__ps_non_det_int_var]) || ((fib1_view_t[ps_thread_num] == fib1_ps_time[__cs_local_t2__ps_non_det_int_var]) && fib1_view_l[ps_thread_num]));
 fib1_view_t[ps_thread_num] = fib1_ps_time[__cs_local_t2__ps_non_det_int_var];
 fib1_view_l[ps_thread_num] = 1;
 fib1_view[ps_thread_num] = fib1_values[__cs_local_t2__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib1_view_v[ps_thread_num]);
}
__cs_local_t2__x = fib1_view[ps_thread_num];
;
;
__CSEQ_rawline("tt2_0_2: IF(1,2,tt2_0_3)");
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t2__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t2__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t2__ps_non_det_int_var] == 5);
 __CPROVER_assume((fib2_view_t[ps_thread_num] < fib2_ps_time[__cs_local_t2__ps_non_det_int_var]) || ((fib2_view_t[ps_thread_num] == fib2_ps_time[__cs_local_t2__ps_non_det_int_var]) && fib2_view_l[ps_thread_num]));
 fib2_view_t[ps_thread_num] = fib2_ps_time[__cs_local_t2__ps_non_det_int_var];
 fib2_view_l[ps_thread_num] = 1;
 fib2_view[ps_thread_num] = fib2_values[__cs_local_t2__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib2_view_v[ps_thread_num]);
}
__cs_local_t2__y = fib2_view[ps_thread_num];
;
__cs_local_t2__s = __cs_local_t2__x + __cs_local_t2__y;
fib1_view[ps_thread_num] = __cs_local_t2__s;
fib1_view_v[ps_thread_num] = 1;
if(nondet_bool()){
    fib1_view_l[ps_thread_num] = 1;
    __cs_local_t2__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t2__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t2__ps_non_det_int_var > fib1_view_t[ps_thread_num]);
    fib1_view_t[ps_thread_num] = __cs_local_t2__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_fib1[__cs_local_t2__ps_non_det_int_var]);
      _lazyps_unavail_fib1[__cs_local_t2__ps_non_det_int_var] = 1;
      _lazyps_upd_fib1[__cs_local_t2__ps_non_det_int_var] = 1;
     if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
         fib1_ps_time[_lazyps_index_used] = fib1_view_t[ps_thread_num];
         fib1_ps_timeL[_lazyps_index_used] = fib1_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 4;
         fib1_values[_lazyps_index_used] = fib1_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
     }
}
else{
  fib1_view_l[ps_thread_num] = 0;
}
;
;
__CSEQ_rawline("tt2_0_3: IF(1,3,tt2_0_4)");
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t2__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t2__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t2__ps_non_det_int_var] == 4);
 __CPROVER_assume((fib1_view_t[ps_thread_num] < fib1_ps_time[__cs_local_t2__ps_non_det_int_var]) || ((fib1_view_t[ps_thread_num] == fib1_ps_time[__cs_local_t2__ps_non_det_int_var]) && fib1_view_l[ps_thread_num]));
 fib1_view_t[ps_thread_num] = fib1_ps_time[__cs_local_t2__ps_non_det_int_var];
 fib1_view_l[ps_thread_num] = 1;
 fib1_view[ps_thread_num] = fib1_values[__cs_local_t2__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib1_view_v[ps_thread_num]);
}
__cs_local_t2__x = fib1_view[ps_thread_num];
;
;
__CSEQ_rawline("tt2_0_4: IF(1,4,tt2_0_5)");
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t2__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t2__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t2__ps_non_det_int_var] == 5);
 __CPROVER_assume((fib2_view_t[ps_thread_num] < fib2_ps_time[__cs_local_t2__ps_non_det_int_var]) || ((fib2_view_t[ps_thread_num] == fib2_ps_time[__cs_local_t2__ps_non_det_int_var]) && fib2_view_l[ps_thread_num]));
 fib2_view_t[ps_thread_num] = fib2_ps_time[__cs_local_t2__ps_non_det_int_var];
 fib2_view_l[ps_thread_num] = 1;
 fib2_view[ps_thread_num] = fib2_values[__cs_local_t2__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib2_view_v[ps_thread_num]);
}
__cs_local_t2__y = fib2_view[ps_thread_num];
;
__cs_local_t2__s = __cs_local_t2__x + __cs_local_t2__y;
fib1_view[ps_thread_num] = __cs_local_t2__s;
fib1_view_v[ps_thread_num] = 1;
if(nondet_bool()){
    fib1_view_l[ps_thread_num] = 1;
    __cs_local_t2__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t2__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t2__ps_non_det_int_var > fib1_view_t[ps_thread_num]);
    fib1_view_t[ps_thread_num] = __cs_local_t2__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_fib1[__cs_local_t2__ps_non_det_int_var]);
      _lazyps_unavail_fib1[__cs_local_t2__ps_non_det_int_var] = 1;
      _lazyps_upd_fib1[__cs_local_t2__ps_non_det_int_var] = 1;
     if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
         fib1_ps_time[_lazyps_index_used] = fib1_view_t[ps_thread_num];
         fib1_ps_timeL[_lazyps_index_used] = fib1_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 4;
         fib1_values[_lazyps_index_used] = fib1_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
     }
}
else{
  fib1_view_l[ps_thread_num] = 0;
}
;
;
        __exit_t2: __CSEQ_assume( __cs_pc_cs[1] >= 5 );
;;
__CSEQ_rawline("tt2_0_5: ");
__cs_exit(0, 1);
    }
;
}
void t2_pscopy(int __cs_param_t2_pscopy__ps_t1)
{
static unsigned int __cs_local_t2_pscopy__ps_non_det_int_var;
static _Bool __cs_local_t2_pscopy__ps_non_det_bool_var;
static int __cs_local_t2_pscopy__x;
__cs_local_t2_pscopy__x = __cs_pscopy_t2__x;
static int __cs_local_t2_pscopy__y;
__cs_local_t2_pscopy__y = __cs_pscopy_t2__y;
static int __cs_local_t2_pscopy__s;
__cs_local_t2_pscopy__s = __cs_pscopy_t2__s;
int __cs_local_t2_pscopy__int_update_helper_var;
    {
static int __cs_local_t2_pscopy__x;
__cs_local_t2_pscopy__x = __CSEQ_nondet_int();
static int __cs_local_t2_pscopy__y;
__cs_local_t2_pscopy__y = __CSEQ_nondet_int();
static int __cs_local_t2_pscopy__s;
;
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(fib1_liveChain[0] == 0);
__cs_local_t2_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t2_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t2_pscopy__ps_non_det_int_var] == 4);
 __CPROVER_assume((fib1_view_t[__cs_param_t2_pscopy__ps_t1] < fib1_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var]) || ((fib1_view_t[__cs_param_t2_pscopy__ps_t1] == fib1_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var]) && fib1_view_l[__cs_param_t2_pscopy__ps_t1]));
 fib1_view_t[__cs_param_t2_pscopy__ps_t1] = fib1_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var];
 fib1_view_l[__cs_param_t2_pscopy__ps_t1] = 1;
 fib1_view[__cs_param_t2_pscopy__ps_t1] = fib1_values[__cs_local_t2_pscopy__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib1_view_v[__cs_param_t2_pscopy__ps_t1]);
}
__cs_local_t2_pscopy__x = fib1_view[__cs_param_t2_pscopy__ps_t1];
;
;
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(fib2_liveChain[0] == 0);
__cs_local_t2_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t2_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t2_pscopy__ps_non_det_int_var] == 5);
 __CPROVER_assume((fib2_view_t[__cs_param_t2_pscopy__ps_t1] < fib2_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var]) || ((fib2_view_t[__cs_param_t2_pscopy__ps_t1] == fib2_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var]) && fib2_view_l[__cs_param_t2_pscopy__ps_t1]));
 fib2_view_t[__cs_param_t2_pscopy__ps_t1] = fib2_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var];
 fib2_view_l[__cs_param_t2_pscopy__ps_t1] = 1;
 fib2_view[__cs_param_t2_pscopy__ps_t1] = fib2_values[__cs_local_t2_pscopy__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib2_view_v[__cs_param_t2_pscopy__ps_t1]);
}
__cs_local_t2_pscopy__y = fib2_view[__cs_param_t2_pscopy__ps_t1];
;
__cs_local_t2_pscopy__s = __cs_local_t2_pscopy__x + __cs_local_t2_pscopy__y;
fib1_view[ps_thread_num] = __cs_local_t2_pscopy__s;
fib1_view_v[ps_thread_num] = 1;
if(nondet_bool()){
    fib1_view_l[ps_thread_num] = 1;
    __cs_local_t2_pscopy__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t2_pscopy__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t2_pscopy__ps_non_det_int_var > fib1_view_t[ps_thread_num]);
    fib1_view_t[ps_thread_num] = __cs_local_t2_pscopy__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_fib1[__cs_local_t2_pscopy__ps_non_det_int_var]);
      _lazyps_unavail_fib1[__cs_local_t2_pscopy__ps_non_det_int_var] = 1;
      _lazyps_upd_fib1[__cs_local_t2_pscopy__ps_non_det_int_var] = 1;
     if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
         fib1_ps_time[_lazyps_index_used] = fib1_view_t[ps_thread_num];
         fib1_ps_timeL[_lazyps_index_used] = fib1_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 4;
         fib1_values[_lazyps_index_used] = fib1_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
     }
}
else{
  fib1_view_l[ps_thread_num] = 0;
}
;
;
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(fib1_liveChain[0] == 0);
__cs_local_t2_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t2_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t2_pscopy__ps_non_det_int_var] == 4);
 __CPROVER_assume((fib1_view_t[__cs_param_t2_pscopy__ps_t1] < fib1_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var]) || ((fib1_view_t[__cs_param_t2_pscopy__ps_t1] == fib1_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var]) && fib1_view_l[__cs_param_t2_pscopy__ps_t1]));
 fib1_view_t[__cs_param_t2_pscopy__ps_t1] = fib1_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var];
 fib1_view_l[__cs_param_t2_pscopy__ps_t1] = 1;
 fib1_view[__cs_param_t2_pscopy__ps_t1] = fib1_values[__cs_local_t2_pscopy__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib1_view_v[__cs_param_t2_pscopy__ps_t1]);
}
__cs_local_t2_pscopy__x = fib1_view[__cs_param_t2_pscopy__ps_t1];
;
;
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(fib2_liveChain[0] == 0);
__cs_local_t2_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t2_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t2_pscopy__ps_non_det_int_var] == 5);
 __CPROVER_assume((fib2_view_t[__cs_param_t2_pscopy__ps_t1] < fib2_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var]) || ((fib2_view_t[__cs_param_t2_pscopy__ps_t1] == fib2_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var]) && fib2_view_l[__cs_param_t2_pscopy__ps_t1]));
 fib2_view_t[__cs_param_t2_pscopy__ps_t1] = fib2_ps_time[__cs_local_t2_pscopy__ps_non_det_int_var];
 fib2_view_l[__cs_param_t2_pscopy__ps_t1] = 1;
 fib2_view[__cs_param_t2_pscopy__ps_t1] = fib2_values[__cs_local_t2_pscopy__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib2_view_v[__cs_param_t2_pscopy__ps_t1]);
}
__cs_local_t2_pscopy__y = fib2_view[__cs_param_t2_pscopy__ps_t1];
;
__cs_local_t2_pscopy__s = __cs_local_t2_pscopy__x + __cs_local_t2_pscopy__y;
fib1_view[ps_thread_num] = __cs_local_t2_pscopy__s;
fib1_view_v[ps_thread_num] = 1;
if(nondet_bool()){
    fib1_view_l[ps_thread_num] = 1;
    __cs_local_t2_pscopy__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t2_pscopy__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t2_pscopy__ps_non_det_int_var > fib1_view_t[ps_thread_num]);
    fib1_view_t[ps_thread_num] = __cs_local_t2_pscopy__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_fib1[__cs_local_t2_pscopy__ps_non_det_int_var]);
      _lazyps_unavail_fib1[__cs_local_t2_pscopy__ps_non_det_int_var] = 1;
      _lazyps_upd_fib1[__cs_local_t2_pscopy__ps_non_det_int_var] = 1;
     if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
         fib1_ps_time[_lazyps_index_used] = fib1_view_t[ps_thread_num];
         fib1_ps_timeL[_lazyps_index_used] = fib1_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 4;
         fib1_values[_lazyps_index_used] = fib1_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
     }
}
else{
  fib1_view_l[ps_thread_num] = 0;
}
;
;
        __exit_t2: ;;
return;;
;;
    }
;
}
void *t3_0(void *__cs_param_t3___cs_unused)
{
static unsigned int __cs_local_t3__ps_non_det_int_var;
static _Bool __cs_local_t3__ps_non_det_bool_var;
int __cs_local_t3__int_update_helper_var;
__CSEQ_rawline("tt3_0_0: IF(2,0,tt3_0_1)");
;
    {
static int __cs_local_t3__x;
__cs_local_t3__x = __CSEQ_nondet_int();
static int __cs_local_t3__y;
__cs_local_t3__y = __CSEQ_nondet_int();
static int __cs_local_t3__s;
;
__CSEQ_rawline("tt3_0_1: IF(2,1,tt3_0_2)");
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t3__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t3__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t3__ps_non_det_int_var] == 4);
 __CPROVER_assume((fib1_view_t[ps_thread_num] < fib1_ps_time[__cs_local_t3__ps_non_det_int_var]) || ((fib1_view_t[ps_thread_num] == fib1_ps_time[__cs_local_t3__ps_non_det_int_var]) && fib1_view_l[ps_thread_num]));
 fib1_view_t[ps_thread_num] = fib1_ps_time[__cs_local_t3__ps_non_det_int_var];
 fib1_view_l[ps_thread_num] = 1;
 fib1_view[ps_thread_num] = fib1_values[__cs_local_t3__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib1_view_v[ps_thread_num]);
}
__cs_local_t3__x = fib1_view[ps_thread_num];
;
;
__CSEQ_rawline("tt3_0_2: IF(2,2,tt3_0_3)");
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t3__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t3__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t3__ps_non_det_int_var] == 5);
 __CPROVER_assume((fib2_view_t[ps_thread_num] < fib2_ps_time[__cs_local_t3__ps_non_det_int_var]) || ((fib2_view_t[ps_thread_num] == fib2_ps_time[__cs_local_t3__ps_non_det_int_var]) && fib2_view_l[ps_thread_num]));
 fib2_view_t[ps_thread_num] = fib2_ps_time[__cs_local_t3__ps_non_det_int_var];
 fib2_view_l[ps_thread_num] = 1;
 fib2_view[ps_thread_num] = fib2_values[__cs_local_t3__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib2_view_v[ps_thread_num]);
}
__cs_local_t3__y = fib2_view[ps_thread_num];
;
__cs_local_t3__s = __cs_local_t3__x + __cs_local_t3__y;
fib2_view[ps_thread_num] = __cs_local_t3__s;
fib2_view_v[ps_thread_num] = 1;
if(nondet_bool()){
    fib2_view_l[ps_thread_num] = 1;
    __cs_local_t3__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t3__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t3__ps_non_det_int_var > fib2_view_t[ps_thread_num]);
    fib2_view_t[ps_thread_num] = __cs_local_t3__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_fib2[__cs_local_t3__ps_non_det_int_var]);
      _lazyps_unavail_fib2[__cs_local_t3__ps_non_det_int_var] = 1;
      _lazyps_upd_fib2[__cs_local_t3__ps_non_det_int_var] = 1;
     if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
         fib2_ps_time[_lazyps_index_used] = fib2_view_t[ps_thread_num];
         fib2_ps_timeL[_lazyps_index_used] = fib2_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 5;
         fib2_values[_lazyps_index_used] = fib2_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
     }
}
else{
  fib2_view_l[ps_thread_num] = 0;
}
;
;
__CSEQ_rawline("tt3_0_3: IF(2,3,tt3_0_4)");
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t3__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t3__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t3__ps_non_det_int_var] == 4);
 __CPROVER_assume((fib1_view_t[ps_thread_num] < fib1_ps_time[__cs_local_t3__ps_non_det_int_var]) || ((fib1_view_t[ps_thread_num] == fib1_ps_time[__cs_local_t3__ps_non_det_int_var]) && fib1_view_l[ps_thread_num]));
 fib1_view_t[ps_thread_num] = fib1_ps_time[__cs_local_t3__ps_non_det_int_var];
 fib1_view_l[ps_thread_num] = 1;
 fib1_view[ps_thread_num] = fib1_values[__cs_local_t3__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib1_view_v[ps_thread_num]);
}
__cs_local_t3__x = fib1_view[ps_thread_num];
;
;
__CSEQ_rawline("tt3_0_4: IF(2,4,tt3_0_5)");
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
__cs_local_t3__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t3__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t3__ps_non_det_int_var] == 5);
 __CPROVER_assume((fib2_view_t[ps_thread_num] < fib2_ps_time[__cs_local_t3__ps_non_det_int_var]) || ((fib2_view_t[ps_thread_num] == fib2_ps_time[__cs_local_t3__ps_non_det_int_var]) && fib2_view_l[ps_thread_num]));
 fib2_view_t[ps_thread_num] = fib2_ps_time[__cs_local_t3__ps_non_det_int_var];
 fib2_view_l[ps_thread_num] = 1;
 fib2_view[ps_thread_num] = fib2_values[__cs_local_t3__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib2_view_v[ps_thread_num]);
}
__cs_local_t3__y = fib2_view[ps_thread_num];
;
__cs_local_t3__s = __cs_local_t3__x + __cs_local_t3__y;
fib2_view[ps_thread_num] = __cs_local_t3__s;
fib2_view_v[ps_thread_num] = 1;
if(nondet_bool()){
    fib2_view_l[ps_thread_num] = 1;
    __cs_local_t3__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t3__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t3__ps_non_det_int_var > fib2_view_t[ps_thread_num]);
    fib2_view_t[ps_thread_num] = __cs_local_t3__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_fib2[__cs_local_t3__ps_non_det_int_var]);
      _lazyps_unavail_fib2[__cs_local_t3__ps_non_det_int_var] = 1;
      _lazyps_upd_fib2[__cs_local_t3__ps_non_det_int_var] = 1;
     if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
         fib2_ps_time[_lazyps_index_used] = fib2_view_t[ps_thread_num];
         fib2_ps_timeL[_lazyps_index_used] = fib2_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 5;
         fib2_values[_lazyps_index_used] = fib2_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
     }
}
else{
  fib2_view_l[ps_thread_num] = 0;
}
;
;
        __exit_t3: __CSEQ_assume( __cs_pc_cs[2] >= 5 );
;;
__CSEQ_rawline("tt3_0_5: ");
__cs_exit(0, 2);
    }
;
}
void t3_pscopy(int __cs_param_t3_pscopy__ps_t1)
{
static unsigned int __cs_local_t3_pscopy__ps_non_det_int_var;
static _Bool __cs_local_t3_pscopy__ps_non_det_bool_var;
static int __cs_local_t3_pscopy__x;
__cs_local_t3_pscopy__x = __cs_pscopy_t3__x;
static int __cs_local_t3_pscopy__y;
__cs_local_t3_pscopy__y = __cs_pscopy_t3__y;
static int __cs_local_t3_pscopy__s;
__cs_local_t3_pscopy__s = __cs_pscopy_t3__s;
int __cs_local_t3_pscopy__int_update_helper_var;
    {
static int __cs_local_t3_pscopy__x;
__cs_local_t3_pscopy__x = __CSEQ_nondet_int();
static int __cs_local_t3_pscopy__y;
__cs_local_t3_pscopy__y = __CSEQ_nondet_int();
static int __cs_local_t3_pscopy__s;
;
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(fib1_liveChain[0] == 0);
__cs_local_t3_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t3_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t3_pscopy__ps_non_det_int_var] == 4);
 __CPROVER_assume((fib1_view_t[__cs_param_t3_pscopy__ps_t1] < fib1_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var]) || ((fib1_view_t[__cs_param_t3_pscopy__ps_t1] == fib1_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var]) && fib1_view_l[__cs_param_t3_pscopy__ps_t1]));
 fib1_view_t[__cs_param_t3_pscopy__ps_t1] = fib1_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var];
 fib1_view_l[__cs_param_t3_pscopy__ps_t1] = 1;
 fib1_view[__cs_param_t3_pscopy__ps_t1] = fib1_values[__cs_local_t3_pscopy__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib1_view_v[__cs_param_t3_pscopy__ps_t1]);
}
__cs_local_t3_pscopy__x = fib1_view[__cs_param_t3_pscopy__ps_t1];
;
;
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(fib2_liveChain[0] == 0);
__cs_local_t3_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t3_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t3_pscopy__ps_non_det_int_var] == 5);
 __CPROVER_assume((fib2_view_t[__cs_param_t3_pscopy__ps_t1] < fib2_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var]) || ((fib2_view_t[__cs_param_t3_pscopy__ps_t1] == fib2_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var]) && fib2_view_l[__cs_param_t3_pscopy__ps_t1]));
 fib2_view_t[__cs_param_t3_pscopy__ps_t1] = fib2_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var];
 fib2_view_l[__cs_param_t3_pscopy__ps_t1] = 1;
 fib2_view[__cs_param_t3_pscopy__ps_t1] = fib2_values[__cs_local_t3_pscopy__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib2_view_v[__cs_param_t3_pscopy__ps_t1]);
}
__cs_local_t3_pscopy__y = fib2_view[__cs_param_t3_pscopy__ps_t1];
;
__cs_local_t3_pscopy__s = __cs_local_t3_pscopy__x + __cs_local_t3_pscopy__y;
fib2_view[ps_thread_num] = __cs_local_t3_pscopy__s;
fib2_view_v[ps_thread_num] = 1;
if(nondet_bool()){
    fib2_view_l[ps_thread_num] = 1;
    __cs_local_t3_pscopy__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t3_pscopy__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t3_pscopy__ps_non_det_int_var > fib2_view_t[ps_thread_num]);
    fib2_view_t[ps_thread_num] = __cs_local_t3_pscopy__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_fib2[__cs_local_t3_pscopy__ps_non_det_int_var]);
      _lazyps_unavail_fib2[__cs_local_t3_pscopy__ps_non_det_int_var] = 1;
      _lazyps_upd_fib2[__cs_local_t3_pscopy__ps_non_det_int_var] = 1;
     if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
         fib2_ps_time[_lazyps_index_used] = fib2_view_t[ps_thread_num];
         fib2_ps_timeL[_lazyps_index_used] = fib2_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 5;
         fib2_values[_lazyps_index_used] = fib2_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
     }
}
else{
  fib2_view_l[ps_thread_num] = 0;
}
;
;
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(fib1_liveChain[0] == 0);
__cs_local_t3_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t3_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t3_pscopy__ps_non_det_int_var] == 4);
 __CPROVER_assume((fib1_view_t[__cs_param_t3_pscopy__ps_t1] < fib1_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var]) || ((fib1_view_t[__cs_param_t3_pscopy__ps_t1] == fib1_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var]) && fib1_view_l[__cs_param_t3_pscopy__ps_t1]));
 fib1_view_t[__cs_param_t3_pscopy__ps_t1] = fib1_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var];
 fib1_view_l[__cs_param_t3_pscopy__ps_t1] = 1;
 fib1_view[__cs_param_t3_pscopy__ps_t1] = fib1_values[__cs_local_t3_pscopy__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib1_view_v[__cs_param_t3_pscopy__ps_t1]);
}
__cs_local_t3_pscopy__x = fib1_view[__cs_param_t3_pscopy__ps_t1];
;
;
;
if(nondet_bool()){
  __CPROVER_assume(_lazyps_numEE < _lazyps_rounds);
 __CPROVER_assume(fib2_liveChain[0] == 0);
__cs_local_t3_pscopy__ps_non_det_int_var = nondet_int();
 __CPROVER_assume(__cs_local_t3_pscopy__ps_non_det_int_var < _lazyps_index_used);
 __CPROVER_assume(_ps_var[__cs_local_t3_pscopy__ps_non_det_int_var] == 5);
 __CPROVER_assume((fib2_view_t[__cs_param_t3_pscopy__ps_t1] < fib2_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var]) || ((fib2_view_t[__cs_param_t3_pscopy__ps_t1] == fib2_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var]) && fib2_view_l[__cs_param_t3_pscopy__ps_t1]));
 fib2_view_t[__cs_param_t3_pscopy__ps_t1] = fib2_ps_time[__cs_local_t3_pscopy__ps_non_det_int_var];
 fib2_view_l[__cs_param_t3_pscopy__ps_t1] = 1;
 fib2_view[__cs_param_t3_pscopy__ps_t1] = fib2_values[__cs_local_t3_pscopy__ps_non_det_int_var];
_lazyps_numEE = _lazyps_numEE + 1;
}
else{
  __CPROVER_assume(fib2_view_v[__cs_param_t3_pscopy__ps_t1]);
}
__cs_local_t3_pscopy__y = fib2_view[__cs_param_t3_pscopy__ps_t1];
;
__cs_local_t3_pscopy__s = __cs_local_t3_pscopy__x + __cs_local_t3_pscopy__y;
fib2_view[ps_thread_num] = __cs_local_t3_pscopy__s;
fib2_view_v[ps_thread_num] = 1;
if(nondet_bool()){
    fib2_view_l[ps_thread_num] = 1;
    __cs_local_t3_pscopy__ps_non_det_int_var = nondet_uint();
    __CPROVER_assume(__cs_local_t3_pscopy__ps_non_det_int_var <= _lazyps_rounds);
    __CPROVER_assume(__cs_local_t3_pscopy__ps_non_det_int_var > fib2_view_t[ps_thread_num]);
    fib2_view_t[ps_thread_num] = __cs_local_t3_pscopy__ps_non_det_int_var;
    __CPROVER_assume(!_lazyps_unavail_fib2[__cs_local_t3_pscopy__ps_non_det_int_var]);
      _lazyps_unavail_fib2[__cs_local_t3_pscopy__ps_non_det_int_var] = 1;
      _lazyps_upd_fib2[__cs_local_t3_pscopy__ps_non_det_int_var] = 1;
     if(nondet_bool()){
         __CPROVER_assume(_lazyps_index_used < _lazyps_rounds);
         fib2_ps_time[_lazyps_index_used] = fib2_view_t[ps_thread_num];
         fib2_ps_timeL[_lazyps_index_used] = fib2_view_l[ps_thread_num];
         _ps_var[_lazyps_index_used] = 5;
         fib2_values[_lazyps_index_used] = fib2_view[ps_thread_num];
         _lazyps_index_used = _lazyps_index_used + 1;
     }
}
else{
  fib2_view_l[ps_thread_num] = 0;
}
;
;
        __exit_t3: ;;
return;;
;;
    }
;
}
int main_thread(void)
{
__CSEQ_rawline("IF(0,0,tmain_1)");
unsigned int __cs_local_main_helper_temporary = 1;
_lazyps_unavail_var0[0] = 1;
_lazyps_upd_var0[0] = 1;
_lazyps_unavail_z1[0] = 1;
_lazyps_upd_z1[0] = 1;
_lazyps_unavail_casvar[0] = 1;
_lazyps_upd_casvar[0] = 1;
_lazyps_unavail_fib1[0] = 1;
_lazyps_upd_fib1[0] = 1;
_lazyps_unavail_fib2[0] = 1;
_lazyps_upd_fib2[0] = 1;
var0_view_l[0] = 1;
var0_view_u[0] = 1;
var0_liveChain[0] = 0;
var0_extView[0] = 1;
var0_blockPromise[0] = 0;
var0_view_v[0] = 1;
z1_view_l[0] = 1;
z1_view_u[0] = 1;
z1_liveChain[0] = 0;
z1_extView[0] = 1;
z1_blockPromise[0] = 0;
z1_view_v[0] = 1;
casvar_view_l[0] = 1;
casvar_view_u[0] = 1;
casvar_liveChain[0] = 0;
casvar_extView[0] = 1;
casvar_blockPromise[0] = 0;
casvar_view_v[0] = 1;
fib1_view_l[0] = 1;
fib1_view_u[0] = 1;
fib1_liveChain[0] = 0;
fib1_extView[0] = 1;
fib1_blockPromise[0] = 0;
fib1_view_v[0] = 1;
fib2_view_l[0] = 1;
fib2_view_u[0] = 1;
fib2_liveChain[0] = 0;
fib2_extView[0] = 1;
fib2_blockPromise[0] = 0;
fib2_view_v[0] = 1;
static unsigned int __cs_local_main__ps_non_det_int_var;
static _Bool __cs_local_main__ps_non_det_bool_var;
int __cs_local_main__int_update_helper_var;
    {
static __cs_t __cs_local_main_thr0;
;;
static __cs_t __cs_local_main_thr1;
;;
static __cs_t __cs_local_main_thr2;
;;
static __cs_t __cs_local_main_thr3;
;;
var0_view[ps_thread_num] = 0;
z1_view[ps_thread_num] = 0;
fib1_view[ps_thread_num] = 1;
fib2_view[ps_thread_num] = 1;
merge_views_ps(0, 1);
__cs_create(&__cs_local_main_thr2, 0, t2_0, 0, 1);
__CSEQ_rawline("tmain_1: IF(0,1,tmain_2)");
merge_views_ps(0, 2);
__cs_create(&__cs_local_main_thr3, 0, t3_0, 0, 2);
__CSEQ_rawline("tmain_2: IF(0,2,tmain_3)");
merge_views_ps(0, 3);
__cs_create(&__cs_local_main_thr0, 0, t0_0, 0, 3);
__CSEQ_rawline("tmain_3: IF(0,3,tmain_4)");
merge_views_ps(0, 4);
__cs_create(&__cs_local_main_thr1, 0, t1_0, 0, 4);
goto __exit_main;;
        __exit_main: __CSEQ_assume( __cs_pc_cs[0] >= 4 );
;;
__CSEQ_rawline("tmain_4: ");
__cs_exit(0, 0);
    }
;
}
int main(void) {
__CSEQ_rawline("/* round  0 */");
__CSEQ_rawline("    /* main */");
          unsigned int __cs_tmp_t0_r0 ;
          __cs_pc_cs[0] = __cs_tmp_t0_r0;
          __CSEQ_assume(__cs_pc_cs[0] > 0);
          __CSEQ_assume(__cs_pc_cs[0] <= 4);
          main_thread();
          __cs_pc[0] = __cs_pc_cs[0];

__CSEQ_rawline("    /* t2_0 */");
         unsigned int __cs_tmp_t1_r0 ;
         if (__cs_active_thread[1]) {
             __cs_pc_cs[1] = __cs_tmp_t1_r0;
             __CSEQ_assume(__cs_pc_cs[1] <= 5);
             t2_0(__cs_threadargs[1]);
             __cs_pc[1] = __cs_pc_cs[1];
         }

__CSEQ_rawline("    /* t3_0 */");
         unsigned int __cs_tmp_t2_r0 ;
         if (__cs_active_thread[2]) {
             __cs_pc_cs[2] = __cs_tmp_t2_r0;
             __CSEQ_assume(__cs_pc_cs[2] <= 5);
             t3_0(__cs_threadargs[2]);
             __cs_pc[2] = __cs_pc_cs[2];
         }

__CSEQ_rawline("    /* t0_0 */");
         unsigned int __cs_tmp_t3_r0 ;
         if (__cs_active_thread[3]) {
             __cs_pc_cs[3] = __cs_tmp_t3_r0;
             __CSEQ_assume(__cs_pc_cs[3] <= 7);
             t0_0(__cs_threadargs[3]);
             __cs_pc[3] = __cs_pc_cs[3];
         }

__CSEQ_rawline("    /* t1_0 */");
         unsigned int __cs_tmp_t4_r0 ;
         if (__cs_active_thread[4]) {
             __cs_pc_cs[4] = __cs_tmp_t4_r0;
             __CSEQ_assume(__cs_pc_cs[4] <= 2);
             t1_0(__cs_threadargs[4]);
             __cs_pc[4] = __cs_pc_cs[4];
         }

__CSEQ_rawline("/* round  1 */");
__CSEQ_rawline("    /* main */");
          unsigned int __cs_tmp_t0_r1 ;
          if (__cs_active_thread[0]) {
             __cs_pc_cs[0] = __cs_pc[0] + __cs_tmp_t0_r1;
             __CSEQ_assume(__cs_pc_cs[0] >= __cs_pc[0]);
             __CSEQ_assume(__cs_pc_cs[0] <= 4);
             main_thread();
             __cs_pc[0] = __cs_pc_cs[0];
          }


__CSEQ_rawline("    /* t2_0 */");
         unsigned int __cs_tmp_t1_r1 ;
         if (__cs_active_thread[1]) {
             __cs_pc_cs[1] = __cs_pc[1] + __cs_tmp_t1_r1;
             __CSEQ_assume(__cs_pc_cs[1] >= __cs_pc[1]);
             __CSEQ_assume(__cs_pc_cs[1] <= 5);
             t2_0(__cs_threadargs[1]);
             __cs_pc[1] = __cs_pc_cs[1];
         }

__CSEQ_rawline("    /* t3_0 */");
         unsigned int __cs_tmp_t2_r1 ;
         if (__cs_active_thread[2]) {
             __cs_pc_cs[2] = __cs_pc[2] + __cs_tmp_t2_r1;
             __CSEQ_assume(__cs_pc_cs[2] >= __cs_pc[2]);
             __CSEQ_assume(__cs_pc_cs[2] <= 5);
             t3_0(__cs_threadargs[2]);
             __cs_pc[2] = __cs_pc_cs[2];
         }

__CSEQ_rawline("    /* t0_0 */");
         unsigned int __cs_tmp_t3_r1 ;
         if (__cs_active_thread[3]) {
             __cs_pc_cs[3] = __cs_pc[3] + __cs_tmp_t3_r1;
             __CSEQ_assume(__cs_pc_cs[3] >= __cs_pc[3]);
             __CSEQ_assume(__cs_pc_cs[3] <= 7);
             t0_0(__cs_threadargs[3]);
             __cs_pc[3] = __cs_pc_cs[3];
         }

__CSEQ_rawline("    /* t1_0 */");
         unsigned int __cs_tmp_t4_r1 ;
         if (__cs_active_thread[4]) {
             __cs_pc_cs[4] = __cs_pc[4] + __cs_tmp_t4_r1;
             __CSEQ_assume(__cs_pc_cs[4] >= __cs_pc[4]);
             __CSEQ_assume(__cs_pc_cs[4] <= 2);
             t1_0(__cs_threadargs[4]);
             __cs_pc[4] = __cs_pc_cs[4];
         }

__CSEQ_rawline("/* round  2 */");
__CSEQ_rawline("    /* main */");
          unsigned int __cs_tmp_t0_r2 ;
          if (__cs_active_thread[0]) {
             __cs_pc_cs[0] = __cs_pc[0] + __cs_tmp_t0_r2;
             __CSEQ_assume(__cs_pc_cs[0] >= __cs_pc[0]);
             __CSEQ_assume(__cs_pc_cs[0] <= 4);
             main_thread();
             __cs_pc[0] = __cs_pc_cs[0];
          }


__CSEQ_rawline("    /* t2_0 */");
         unsigned int __cs_tmp_t1_r2 ;
         if (__cs_active_thread[1]) {
             __cs_pc_cs[1] = __cs_pc[1] + __cs_tmp_t1_r2;
             __CSEQ_assume(__cs_pc_cs[1] >= __cs_pc[1]);
             __CSEQ_assume(__cs_pc_cs[1] <= 5);
             t2_0(__cs_threadargs[1]);
             __cs_pc[1] = __cs_pc_cs[1];
         }

__CSEQ_rawline("    /* t3_0 */");
         unsigned int __cs_tmp_t2_r2 ;
         if (__cs_active_thread[2]) {
             __cs_pc_cs[2] = __cs_pc[2] + __cs_tmp_t2_r2;
             __CSEQ_assume(__cs_pc_cs[2] >= __cs_pc[2]);
             __CSEQ_assume(__cs_pc_cs[2] <= 5);
             t3_0(__cs_threadargs[2]);
             __cs_pc[2] = __cs_pc_cs[2];
         }

__CSEQ_rawline("    /* t0_0 */");
         unsigned int __cs_tmp_t3_r2 ;
         if (__cs_active_thread[3]) {
             __cs_pc_cs[3] = __cs_pc[3] + __cs_tmp_t3_r2;
             __CSEQ_assume(__cs_pc_cs[3] >= __cs_pc[3]);
             __CSEQ_assume(__cs_pc_cs[3] <= 7);
             t0_0(__cs_threadargs[3]);
             __cs_pc[3] = __cs_pc_cs[3];
         }

__CSEQ_rawline("    /* t1_0 */");
         unsigned int __cs_tmp_t4_r2 ;
         if (__cs_active_thread[4]) {
             __cs_pc_cs[4] = __cs_pc[4] + __cs_tmp_t4_r2;
             __CSEQ_assume(__cs_pc_cs[4] >= __cs_pc[4]);
             __CSEQ_assume(__cs_pc_cs[4] <= 2);
             t1_0(__cs_threadargs[4]);
             __cs_pc[4] = __cs_pc_cs[4];
         }

__CSEQ_rawline("/* round  3 */");
__CSEQ_rawline("    /* main */");
          unsigned int __cs_tmp_t0_r3 ;
          if (__cs_active_thread[0]) {
             __cs_pc_cs[0] = __cs_pc[0] + __cs_tmp_t0_r3;
             __CSEQ_assume(__cs_pc_cs[0] >= __cs_pc[0]);
             __CSEQ_assume(__cs_pc_cs[0] <= 4);
             main_thread();
             __cs_pc[0] = __cs_pc_cs[0];
          }


__CSEQ_rawline("    /* t2_0 */");
         unsigned int __cs_tmp_t1_r3 ;
         if (__cs_active_thread[1]) {
             __cs_pc_cs[1] = __cs_pc[1] + __cs_tmp_t1_r3;
             __CSEQ_assume(__cs_pc_cs[1] >= __cs_pc[1]);
             __CSEQ_assume(__cs_pc_cs[1] <= 5);
             t2_0(__cs_threadargs[1]);
             __cs_pc[1] = __cs_pc_cs[1];
         }

__CSEQ_rawline("    /* t3_0 */");
         unsigned int __cs_tmp_t2_r3 ;
         if (__cs_active_thread[2]) {
             __cs_pc_cs[2] = __cs_pc[2] + __cs_tmp_t2_r3;
             __CSEQ_assume(__cs_pc_cs[2] >= __cs_pc[2]);
             __CSEQ_assume(__cs_pc_cs[2] <= 5);
             t3_0(__cs_threadargs[2]);
             __cs_pc[2] = __cs_pc_cs[2];
         }

__CSEQ_rawline("    /* t0_0 */");
         unsigned int __cs_tmp_t3_r3 ;
         if (__cs_active_thread[3]) {
             __cs_pc_cs[3] = __cs_pc[3] + __cs_tmp_t3_r3;
             __CSEQ_assume(__cs_pc_cs[3] >= __cs_pc[3]);
             __CSEQ_assume(__cs_pc_cs[3] <= 7);
             t0_0(__cs_threadargs[3]);
             __cs_pc[3] = __cs_pc_cs[3];
         }

__CSEQ_rawline("    /* t1_0 */");
         unsigned int __cs_tmp_t4_r3 ;
         if (__cs_active_thread[4]) {
             __cs_pc_cs[4] = __cs_pc[4] + __cs_tmp_t4_r3;
             __CSEQ_assume(__cs_pc_cs[4] >= __cs_pc[4]);
             __CSEQ_assume(__cs_pc_cs[4] <= 2);
             t1_0(__cs_threadargs[4]);
             __cs_pc[4] = __cs_pc_cs[4];
         }

          unsigned int __cs_tmp_t0_r4 ;
           if (__cs_active_thread[0] == 1) {
             __cs_pc_cs[0] = __cs_pc[0] + __cs_tmp_t0_r4;
             __CSEQ_assume(__cs_pc_cs[0] >= __cs_pc[0]);
             __CSEQ_assume(__cs_pc_cs[0] <= 4);
             main_thread();
           }
    return 0;
}

