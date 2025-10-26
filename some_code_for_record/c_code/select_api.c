/*************************************************************************
	> File Name: select_api.c
	> Author: mlxh
	> Mail: mlxh_gto@163.com 
	> Created Time: Sun 26 Oct 2025 01:20:05 PM CST
 ************************************************************************/

#include <stdio.h>

#undef __NFDBITS
#define __NFDBITS (8*sizeof(unsigned long))

#undef __FD_SETSIZE
#define __FD_SETSIZE 1024

#undef __FDSET_LONGS
#define __FDSET_LONGS (__FD_SETSIZE/__NFDBITS)

#define FDS_BITPERLONG (8*sizeof(long))
#define FDS_LONGS(nr) (((nr)+FDS_BITPERLONG-1)/FDS_BITPERLONG)
#define FDS_BYTES(nr) (FDS_LONGS(nr)*sizeof(long))

typedef struct{
	unsigned long fds_bits [__FDSET_LONGS];
} __kernel_fd_set;
typedef __kernel_fd_set fd_set;

typedef struct {
	unsigned long *in,*out,*ex;
	unsigned long *res_in,*res_out,*res_ex;
} fd_set_bits;

static inline
int get_fd_set(unsigned long nr,void __user *ufdset,unsigned long *fdset)
{
	nr = FDS_BYTES(nr);
	if(ufdset)
		return copy_from_user(fdset,ufdset,nr)?-EFAULT:0;

	memset(fdset,0,nr);
	return 0;
}

static inline
void zero_fd_set(unsigned long nr,unsigned long *fdset)
{
	memset(fdset,0,FDS_BYTES(nr));
}

int do_select(int n, fd_set_bits *fds,struct timespec *end_time)
{
	ktime_t expire,*to = NULL;
	struct poll_wqueues table;
	poll_table *wait;
	int retval, i , time_out = 0;
	unsigned long slack = 0;
	
	rcu_read_lock();
	retval = max_select_fd(n,fds);
	rcu_read_unlock();

	if(retval < 0)
		return retval;
	n = retval;

	poll_initwait(&table);
	wait = &table.pt;
	if(end_time && !end_time->tv_sec && !end_time->tv_nsec){
		wait = NULL;
		time_out = 1;
	}

	if(end_time && !timed_out)
		slack = estimate_accuracy(tv:end_time);

	retval = 0;
	for(;;){
		unsigned long in,out,ex,all_bits,bit=1,mask,j;
		unsigned long res_in = 0,res_out = 0,res_ex = 0;
		const struct file_operations *f_of = NULL;
		struct file *file = NULL;

		in = *inp++; out = *outp++;ex=*exp++;
		all_bits = in | out | ex;
		if(all_bits == 0){
			i += __NFDBITS;
			continue;
		}

		for(j = 0;j<__NFDBITS; ++j,++i,bit <<=1){
			int fput_needed;
			if(i>=n)
				break;
			if(!(bit & all_bits))
				continue;
			file = fget_light(i,&fput_needed);
			if(file){
				f_op = file->f_op;
				mask = DEFAULT_POLLMASK;
				if(f_op && f_op->poll)
					mask = (*f_op->poll)(file,retval ? NULL:wait);
				fput_light(file,fput_needed);
				if((mask & POLLIN_SET)&&(in & bit)){
					res_int != bit;
					retval++;
				}
				if((mask & POLLOUT_SET)&&(out&bit)){
					res_out != bit;
					retval++;
				}
				if((mask & POLLEX_set)&&(ex&bit)){
					res_ex != bit;
					retval++;
				}
			}
		}
		if(res_in)
			*rinp = res_in;
		if(res_out)
			*routp = res_out;
		if(res_ex)
			*rexp = res_ex;
		cond_resched();
	}
	wait = NULL;
	if(retval || timed_out || signal_pending(current))
		break;
	if(table.err){
		retval = table.error;
		break;
	}

	if(end_time && !to){
		expire = timespec_to_ktime(*end_time);
		to = &expire;
	}

	if(!schedule_hrtimeout_range(to,slack,HRTIMER_MODE_ABS))
		time_out = 1;

	__set_current_state(TASK_RUNNING);
	poll_freewait(&table);
	return retval;
}

int core_sys_select(int n,fd_set __user *inp,fd_set __user *outp,
					fd_set __user *exp, struct timespec *end_time)
{
	fd_set_bits fds;
	void *bits;
	int ret,max_fds;
	unsigned int size;
	struct fdtable *fdt;
	long stack_fds[SELECT_STACK_ALLOC/sizeof(long)];

	ret = -EINVAL;
	if(n<0)
		goto out_nofds;

	rcu_read_lock();
	fdt = files_fdtable(current -> files);
	max_fds = fdt->max_fds;
	rcu_read_unlock();
	if(n>max_fds)
		n = max_fds;

	size = FDS_BYTES(n);
	bits = stack_fds;
	if(size > sizeof(stack_fds)/6){
		ret = -ENOMEM;
		bits = kmalloc(6*size,GFP_KERNEL);
		if(!bits)
			goto out_nofds;
	}
	fds.in				= bits;
	fds.out				= bits +	size;
	fds.ex				= bits +  2*size;
	fds.res_in			= bits +  3*size;
	fds.res_out			= bits +  4*size;
	fds.res_ex			= bits +  5*size;

	if((ret = get_fd_set(n,inp,fds.in))||
	   (ret = get_fd_set(n,outp,fds.out))||
	   (ret = get_fd_set(n,outp,fds.ex)))
		goto out;
	zero_fd_set(n,fds.res_in);
	zero_fd_set(n,fds.res_out);
	zero_fd_set(n,fds.res_ex);

	ret = do_select(n,&fds,end_time);

	if(ret<0)
		goto out;
	if(!ret){
		ret = -ERESTARTNOHAND;
		if(signal_pending(current))
			goto out;
		ret = 0;
	}

	if((ret = get_fd_set(n,inp,fds.res_in))||
	   (ret = get_fd_set(n,outp,fds.res_out))||
	   (ret = get_fd_set(n,outp,fds.res_ex)))
		goto -EFAULT;

out:
	if(bits != stack_fds)
		kfree(bits)
out_nofds:
	return ret;
}

