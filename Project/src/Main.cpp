#include "TIVIDYFacade.h"

using namespace std;
#include <iostream>


void ChooseAScenario() {
    
    cout << "----------------------------------------------------------------------------";
    cout << "\n----------------------- SCENARIO MENU -----------------------------------\n";
    cout << "---------------------- Choose scenario [ 1 - 2 ] ------------------------\n\n";

    cout << "\033[35m |1.  Animation studio   | 2.   Restaurant   |\033[0m \n";
    cout << "----------------------------------------------------------------------------\n\n";
    cout << "Action number - ";
}

void showFacadeWorkflowMenu() {
    cout << "----------------------------------------------------------------------------";
    cout << "\n------------------------ WORKFLOW MENU ---------------------------------\n";
    cout << "----------------- Choose an action -- [ 0 - 10 ] ---------------\n\n";

    cout << "\033[35m 1.  Create Workflow      | 6.   View Version  \033[0m \n";
    cout << "\033[35m 2.  Start Workflow       | 7.   Cancel Workflow  \033[0m \n";
    cout << "\033[35m 3.  Review Workflow      | 8.   View Progress   \033[0m \n";
    cout << "\033[35m 4.  Reject Workflow      | 9.   Regress to Version   \033[0m \n";
    cout << "\033[35m 5.  Complete Workflow    | 10.  Save Version   \033[0m \n\n";

    cout << "\033[35m 0.  Return to Industry Menu \033[0m \n\n";

    cout << "----------------------------------------------------------------------------\n\n";

    cout << "Action number - ";

}

int main(){

    cout << "\n\n \033[32m -------------------------------------------------------- \033[0m" << "\n";
    cout << "\033[32m ฅ(^•ﻌ•^ฅ) ++++++ TIVIDY WORKFLOW ENGINE +++++ ฅ (^•ﻌ•^ฅ) \033[0m" << "\n";
    cout << "--------------------------------------------------------" << "\n\n";

    ChooseAScenario();
    int ScenarioChoice ;
    cin >> ScenarioChoice;

    switch (ScenarioChoice){
        case 1:{
            cout << "-----------------------------------------------" << "\n";
            cout << "      ANIMATION STUDIO WORKFLOW MANAGER  ＼(^o^)／" << "\n";
            cout << "-----------------------------------------------" << "\n\n";
            break;
        }
        case 2:{
            cout << "-----------------------------------------------" << "\n";
            cout << "RESTAURANT WORKFLOW MANAGER  ᕕ( ᐛ )ᕗ" << "\n";
            cout << "-----------------------------------------------" << "\n\n";
            break;
        }
        default:{
            
            break;
        }
    }

    showFacadeWorkflowMenu();
    int MenuChoice ;
    cin >> MenuChoice;

    TIVIDYFacade facade;

    facade.CreateWorkFlow("Animation");
    facade.StartWorkFlow();
    facade.ReviewWorkFlow();
    facade.CancelWorkFlow();
    facade.getProgress();
    facade.getVersion();
    facade.saveVersion();
            

    int version;
    cout << "Enter version number you would like to return to: ";
    cin >> version;

    facade.regressVersion(version);
    facade.CompleteWorkFlow();


    return 0;
}