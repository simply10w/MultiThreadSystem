#include "Timer.h"
#include <dos.h>
#include "pcb.h"
#include "Thread.h"
#include "Schedule.h"
#include "KernSem.h"
#include <iostream.h>

volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp; 

extern volatile KernelSem* sys_sem = 0;
extern PCB* initial;
extern PCB* idlethread;
extern volatile int contextChFlag = 0;
volatile unsigned int counter = 20; //pocetna random vrednosst

volatile int asleepCnt = 0;
static volatile PCB* asleep = 0;

extern void tick();


void interrupt timer_routine() {
	if(!contextChFlag && (PCB::running->unlimited!=1)) counter--;
	if( ( (counter==0) && (PCB::running->unlimited!=1) && !lockFlag ) || contextChFlag==1) { //time_left==0 or dispatch()
			#ifndef BCC_BLOCK_IGNORE
				asm{
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}
			#endif
			PCB::running->sp=tsp;
			PCB::running->ss=tss;
			PCB::running->bp=tbp;
			if(PCB::running!= initial && PCB::running!=idlethread) Scheduler::put((PCB*)PCB::running);
			PCB::running = Scheduler::get();

			if(!PCB::running) {
				if(PCB::active_threads) PCB::running=idlethread;
				else PCB::running=initial;
			}
			counter = PCB::running->time_left;
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			#ifndef BCC_BLOCK_IGNORE
				asm{
					  mov ss,tss
					  mov sp,tsp
					  mov bp,tbp
				}
			#endif

	} 
	else if(contextChFlag==2) { //exitThread
			while(PCB::running->toSignal) {
					PCB* prev = (PCB*) PCB::running->toSignal;
					PCB* temp = (PCB*) PCB::running->toSignal->next;
					temp->next=0;
					if(prev!=initial && prev != idlethread)
						Scheduler::put(prev);
					PCB::running->toSignal = temp;
			}
			PCB::running = Scheduler::get();
			if(!PCB::running) {
							if(PCB::active_threads) PCB::running=idlethread;
							else PCB::running=initial;
						}
			counter = PCB::running->time_left;
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			#ifndef BCC_BLOCK_IGNORE
			asm{
					mov ss,tss
					mov sp,tsp
					mov bp,tbp
			}
			#endif
	} 
	else if(contextChFlag == 3) { //sleep
			if(asleepCnt == 0) { asleep = PCB::running;  }
 			else { PCB::running->next = asleep; asleep = PCB::running; }
			asleepCnt++;
			#ifndef BCC_BLOCK_IGNORE
			asm{
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			#endif
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;
			PCB::running = Scheduler::get();
			if(!PCB::running) {
							if(PCB::active_threads) PCB::running=idlethread;
							else PCB::running=initial;
						}
			counter = PCB::running->time_left;
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			#ifndef BCC_BLOCK_IGNORE
			asm{
				  mov ss,tss
				  mov sp,tsp
				  mov bp,tbp
			}
			#endif
	}
	else if(contextChFlag == 4) { //block
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			#endif
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;
			PCB::running->next = sys_sem->headBlocked;
			sys_sem->headBlocked = PCB::running;
			PCB::running = Scheduler::get();
			if(!PCB::running) {
							if(PCB::active_threads) PCB::running=idlethread;
							else PCB::running=initial;
						}
			counter = PCB::running->time_left;
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			#endif
	}
	else if(contextChFlag == 5) { //waitToComplete
			#ifndef BCC_BLOCK_IGNORE
				asm {
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}
			#endif
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			if(carrier->toSignal != 0 ) {
					PCB::running->next = carrier->toSignal;
			}
			carrier->toSignal = PCB::running;

			PCB::running = Scheduler::get();
			if(!PCB::running) {
							if(PCB::active_threads) PCB::running=idlethread;
							else PCB::running=initial;
						}
			counter = PCB::running->time_left;
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			#endif
	}
	else if(contextChFlag == 6) { //KernelEv wait()
			#ifndef BCC_BLOCK_IGNORE
				asm {
					mov tsp, sp
					mov tss, ss
					mov tbp, bp
				}
			#endif
			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;
			PCB::running = Scheduler::get();
			if(!PCB::running) {
							if(PCB::active_threads) PCB::running=idlethread;
							else PCB::running=initial;
						}
			counter = PCB::running->time_left;
			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;
			#ifndef BCC_BLOCK_IGNORE
			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			#endif
	}

	if(contextChFlag == 0) {
			if(asleep != 0) { //azuriranje vremena spavanja i budjenje niti
			  PCB* temp = (PCB*)asleep;
			  PCB* prev = 0;
			  while(temp) {
				  if((--temp->sleep_left) == 0) {
					  if(!prev) asleep=temp->next;
					  else prev->next = temp->next;
					  temp->next = 0;
					  if(temp!=initial && temp!=idlethread)
					  Scheduler::put((PCB*) temp);
					  asleepCnt--;
				  }
				  prev = temp;
				  temp = (PCB*)temp->next;
			  }
			}//if(asleep) end

			#ifndef BCC_BLOCK_IGNORE
			tick();
			asm int 60h
			#endif
		}
	contextChFlag = 0;
} //end timer

//setting and restoring timer routine in IVTable
unsigned oldTimerOFF, oldTimerSEG;

void timer_routine_override(){
	#ifndef BCC_BLOCK_IGNORE
		asm{
			cli
			push es
			push ax

			mov ax,0   //  ; inicijalizuje rutinu za tajmer
			mov es,ax

			mov ax, word ptr es:0022h //; pamti staru rutinu
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax

			mov word ptr es:0022h, seg timer_routine	 //postavlja
			mov word ptr es:0020h, offset timer_routine //novu rutinu

			mov ax, oldTimerSEG	 //	postavlja staru rutinu
			mov word ptr es:0182h, ax //; na int 60h
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax

			pop ax
			pop es
			sti
	}
	#endif
}

void timer_routine_old(){
	#ifndef BCC_BLOCK_IGNORE
		asm {
			cli
			push es
			push ax

			mov ax,0
			mov es,ax


			mov ax, word ptr oldTimerSEG
			mov word ptr es:0022h, ax
			mov ax, word ptr oldTimerOFF
			mov word ptr es:0020h, ax

			pop ax
			pop es
			sti
	}
	#endif
}

