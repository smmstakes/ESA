#include "tpl_os.h"
#include "Arduino.h"
#include "board.h"

TaskStateType taskState;

void setup() {
    Serial.begin(115200);
}

TASK(task1) {
    static unsigned int qtdTask1 = 0;
    qtdTask1++;
    Serial.print("Task1 foi ativada: ");
    Serial.println(qtdTask1);

    TerminateTask();
}

TASK(task2) {
    static unsigned int qtdTask2 = 0;
    qtdTask2++;
    Serial.print("Task2 foi ativada: ");
    Serial.println(qtdTask2);

    TerminateTask();
}

TASK(task3) {
    static unsigned int qtdTask3 = 0;
    qtdTask3++;
    Serial.print("Task3 foi ativada: ");
    Serial.println(qtdTask3);

    TerminateTask();
}

TASK(task4) {
    TaskType task;
    short r = rand() % 3;
    switch (r) {
        case 0:
            task = task1;
            break;
        case 1:
            task = task2;
            break;
        case 2:
            task = task3;
            break;
        default:
            task = task1;
            break;
    }

    StatusType status = GetTaskState(task, &taskState);

    Serial.print("Task escolhida: ");
    Serial.print(task+1);
    Serial.print(", Status: ");
    Serial.println(taskState);

    TerminateTask();
}
