#ifdef CHANGED
#include "fork.h"
#include "thread.h"
#include "addrspace.h"
#include "synch.h"
#include "system.h"
#include "console.h"

struct Serialisation{
	AddrSpace* space;
};

void StartProcess(int arg){
	Serialisation* restor = (Serialisation*) arg;
	currentThread->space = restor->space;
	currentThread->space->InitRegisters ();	// set the initial register values
    currentThread->space->RestoreState ();	// load page table register

    machine->Run ();		// jump to the user progam
}

int do_UserFork(char *s){

	OpenFile *executable = fileSystem->Open (s);
    AddrSpace *space = new AddrSpace(executable);

    Thread* newThread = new Thread("newProcess");

    Serialisation* save = new Serialisation;
    save->space = space;

    newThread->Fork(StartProcess,(int)save);
    machine->SetNbProcess(machine->GetNbProcess()+1);
    delete executable;
    currentThread->Yield();
	return 0;
}

#endif 