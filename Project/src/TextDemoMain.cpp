#include "TIVIDYFacade.h"

#include "WorkFlow.h"
#include "WorkItem.h"
#include "Participant.h"
#include "Member.h"

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
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

void menuWorkFlowManager(){

}
void displayParticipants(int index, const string& name,string assignedWorkItem,int width = 22, int indent = 6){
    string border(width, '-');
    string indentation(indent, ' ');

    cout << indentation << "+" << border << "+\n";

    string numberedName = to_string(index) + ". " + name;
    int paddingName = (width - numberedName.length()) / 2;
    cout << indentation << "|" 
         << string(paddingName, ' ') 
         << "\033[95m" << numberedName << "\033[0m"
         << string(width - numberedName.length() - paddingName, ' ') 
         << "|\n";

    int paddingPrice = (width - assignedWorkItem.length()) / 2;
    cout << indentation << "|" 
         << string(paddingPrice, ' ') << assignedWorkItem
         << string(width - assignedWorkItem.length() - paddingPrice, ' ') 
         << "|\n";

    cout << indentation << "+" << border << "+\n";
}
void getParticipants(TIVIDYFacade& facade)
{
    vector<Participant*> participants = facade.getParticipants();
    if (participants.empty()){
        cout << "No participants found.\n";
        return;
    }

    for (size_t i = 0; i < participants.size(); i++){
        Participant* participant = participants[i];
        const map<string, WorkItem*>& assignedWork = participant->getAssignedWork();

        if (assignedWork.empty()){
            displayParticipants(i + 1,participant->getName(),"No assigned work");
        }else{
            for (map<string, WorkItem*>::const_iterator it = assignedWork.begin();
                 it != assignedWork.end(); ++it){
                if (it->second != nullptr){
                    displayParticipants(
                        i + 1,
                        participant->getName(),
                        it->second->getName()
                    );
                }
            }
        }
        cout << "\n";
    }
}

// Populate the Animation and restaurant scenario choices: 
void setupAnimationScenario(TIVIDYFacade& facade)
{
    // Create the main workflow
    facade.CreateWorkFlow("Animation Production");

    WorkFlow* animation =
        dynamic_cast<WorkFlow*>(facade.GetCurrentWorkflow());

    if (animation == nullptr)
        return;

    // Create sub-workflows
    WorkFlow* preProduction = new WorkFlow("Pre-Production");
    WorkFlow* assetProduction = new WorkFlow("Asset Production");
    WorkFlow* postProduction = new WorkFlow("Post-Production");

    // Pre-Production tasks
    preProduction->addChild(new WorkItem("Concept Design"));
    preProduction->addChild(new WorkItem("Storyboarding"));

    // Asset Production tasks
    assetProduction->addChild(new WorkItem("Modelling"));
    assetProduction->addChild(new WorkItem("Texturing"));
    assetProduction->addChild(new WorkItem("Rigging"));

    // Post-Production tasks
    postProduction->addChild(new WorkItem("Animation Rendering"));
    postProduction->addChild(new WorkItem("Quality Check"));

    // Add sub-workflows to main workflow
    animation->addChild(preProduction);
    animation->addChild(assetProduction);
    animation->addChild(postProduction);

    Participant* p1 = new Member("Dorris");

    AddParticipant(Participant* participant);
    std::vector<Participant*> getParticipants();
    bool AssignParticipant(Participant* participant, WorkItem* item);
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
        bool animationSetup = false;

        switch(ScenarioChoice){
            //AnimationStudio
           
            case 1:{
                clearConsole();

                cout << "-----------------------------------------------" << "\n";
                cout << "      ANIMATION STUDIO WORKFLOW MANAGER  ＼(^o^)／" << "\n";
                cout << "-----------------------------------------------" << "\n\n";




                if (!animationSetup){
                    setupAnimationScenario(facade);
                    animationSetup = true;
                }

                
                getParticipants(facade);

                break;
            }
            //CRestaurant
            case 2:{
            cout << "-----------------------------------------------" << "\n";
            cout << "RESTAURANT WORKFLOW MANAGER  ᕕ( ᐛ )ᕗ" << "\n";
            cout << "-----------------------------------------------" << "\n\n";
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
            default: {
                cout << "Invalid choice!\n";
                break;
            }
        }
        if (ScenarioChoice != 0) {
            cout << "\nPress ENTER to return to the main menu...";
            cin.get();
        }
    }while (ScenarioChoice != 0);


    return 0;
}