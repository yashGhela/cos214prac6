#include "TIVIDYFacade.h"

#include "WorkFlow.h"
#include "WorkItem.h"

using namespace std;

#include <iostream>
#include <vector>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <iomanip> 

//helper functions 

void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
int readInt() {
    int value;
    if (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }return value;
}

int main(){
    TIVIDYFacade facade;

    int ScenarioChoice;
    do{
        clearConsole();
        cout << "\n\n \033[32m -------------------------------------------------------- \033[0m" << "\n";
        cout << "\033[32m ฅ(^•ﻌ•^ฅ) ++++++ TIVIDY WORKFLOW ENGINE +++++ ฅ (^•ﻌ•^ฅ) \033[0m" << "\n";
        cout << "\033[32m -------------------------------------------------------- \033[0m" << "\n\n";

        cout << "\033[1;36m\nSelect a Scenario:\n\033[0m";
        cout << " 1. Animation Studio\n";
        cout << " 2. Restaurant\n";
        cout << " 3. Create a scenario\n";
        cout << " 0. Exit\n\n";
        cout << "Choice: ";

        ScenarioChoice = readInt();

        switch(ScenarioChoice){
            //AnimationStudio
            case 1:{
                break;
            }
            //CRestaurant
            case 2:{
                break;
            }
            //Create a new Scenario 
            case 3:{
                break;
            }
            case 0:{
                cout << "\033[1;32m\nFinished Managing workflows. Yippeee! \033[0m\n\n";
                clearConsole();
                break;
            }
        }

    }while (ScenarioChoice != 0);


    return 0;
}