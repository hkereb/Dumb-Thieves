//
// Created by hkere on 26/04/2025.
//

#ifndef LOGIC_H
#define LOGIC_H

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "queue.h"
#include "process.h"

void run_logic(int num_houses, int num_fences);
void leave_critical_sections(Process* process);

#endif //LOGIC_H
