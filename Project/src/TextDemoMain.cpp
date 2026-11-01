#include "TIVIDYFacade.h"

#include "WorkFlow.h"
#include "WorkItem.h"
#include "Participant.h"
#include "Member.h"
#include "Team.h"
#include "Roles.h"
#include "RolesApprover.h"

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
void getWorkflows(TIVIDYFacade& facade){
    vector<WorkFlow*> workflows = facade.getWorkflows();
    if (workflows.empty()){
        cout << "No workflows \n";
        return;
    }
    int workflowCount = 0;
    for(auto flow:workflows){
        cout<<++workflowCount<<". "<<flow->getName();
    }
}

void displayWorkItems(TIVIDYFacade& facade){
    vector<WorkFlow*> workflows = facade.getWorkflows();
    if(workflows.empty()){
        cout<<"\033[91m"<<"No workflows are assigned\n\033[0m";
        return;
    }
    int flowCount = 0;
    cout<<"====  AVAILABLE WORK ITEMS  ====\n\n";
    for(auto workflow:workflows){
        int workitemCount = 0;
        cout<<"\033[96m"<<++flowCount<<". "<<workflow->getName()<<"\033[0m\n";
        vector<WorkFlowComponent*> workitems = workflow->getChildren();
        for(auto item:workitems){
            if(!item->isInProgress()){
                cout<<"\033[95m"<<++workitemCount<<". "<<item->getName()<<"\033[0m\n";
            }
        }
    }
}

// Populate the Animation and restaurant scenario choices: 
void setupAnimationScenario(TIVIDYFacade& facade)
{
    // Create the main workflow
    facade.CreateWorkFlow("Animation Production");

    WorkFlow* animation = dynamic_cast<WorkFlow*>(facade.GetCurrentWorkflow());

    if (animation == nullptr)
        return;


    WorkFlow* preProduction = new WorkFlow("Pre-Production");
    WorkFlow* assetProduction = new WorkFlow("Asset Production");
    WorkFlow* postProduction = new WorkFlow("Post-Production");

    WorkItem* ConceptD = new WorkItem("Concept Design");
    preProduction->addChild(ConceptD);

    WorkItem* Storyboarding = new WorkItem("Storyboarding");
    preProduction->addChild(Storyboarding);

    WorkItem* Modelling = new WorkItem("Modelling");
    assetProduction->addChild(Modelling);

    WorkItem* Texturing = new WorkItem("Texturing");
    assetProduction->addChild(Texturing);

    WorkItem* Rigging = new WorkItem("Rigging");
    assetProduction->addChild(Rigging);

    WorkItem* Animation = new WorkItem("Animation Rendering");
    postProduction->addChild(Animation);

    WorkItem* Quality = new WorkItem("Quality Check");
    postProduction->addChild(Quality);

    animation->addChild(preProduction);
    animation->addChild(assetProduction);
    animation->addChild(postProduction);

    // Create participants
    Participant* p1 = new Member("Dorris");
    Participant* p2 = new Member("DavidMan");

    // Create teams
    Team* productionTeam = new Team("Production Team");
    Team* assetTeam = new Team("Asset Production Team");
    Team* designTeam = new Team("Design Team");

    // Add participants to subteams
    designTeam->addMember(p1);
    assetTeam->addMember(p2);

    // Add subteams to the main team
    productionTeam->addMember(designTeam);
    productionTeam->addMember(assetTeam);

    // Register the main team
    facade.AddParticipant(productionTeam);

    // Assign work
    facade.AssignParticipant(p1, Modelling);
    facade.AssignParticipant(p2, Texturing);
}

void setupRestaurantScenario(TIVIDYFacade& facade){
    facade.CreateWorkFlow("Hungry Cat Restaurant");
    WorkFlow* food = dynamic_cast<WorkFlow*>(facade.GetCurrentWorkflow());
    if(food==nullptr){
        cout<<"Oops, something went wrong with setting up the scenario!"<<endl;
        return;
    }
    WorkFlow* serversFront = new WorkFlow("Front Orders");
    WorkItem* greetCustomer = new WorkItem("Greet Customers");
    WorkItem* giveDrinks = new WorkItem("Serving Drinks");
    WorkItem* smallOrder = new WorkItem("Small Chips");
    WorkItem* largeOrder = new WorkItem("Basted Duck");
    WorkItem* serveFood = new WorkItem("Serving Orders");
    serversFront->addChild(greetCustomer);
    serversFront->addChild(giveDrinks);
    serversFront->addChild(smallOrder);
    serversFront->addChild(largeOrder);
    serversFront->addChild(serveFood);
    food->addChild(serversFront);

    WorkFlow* cooksBack = new WorkFlow("Back Kitchen");
    WorkItem* washHands = new WorkItem("Washing Hands");
    WorkItem* cleanDishes = new WorkItem("Cleaning Dirty Dishes");
    WorkItem* prepFood = new WorkItem("Preparing Ingredients");
    WorkItem* getOrders = new WorkItem("Receiving Orders from Servers");
    WorkItem* cookOrders = new WorkItem("Cooking Pending Orders");
    cooksBack->addChild(washHands);
    cooksBack->addChild(cleanDishes);
    cooksBack->addChild(prepFood);
    cooksBack->addChild(getOrders);
    cooksBack->addChild(cookOrders);
    food->addChild(cooksBack);

    Participant* s1 = new Member("Mark");
    Participant* c1 = new Member("Chef Crazy Dave");
    Roles* a1 = new RolesApprover(c1);
    Participant* c2 = new Member("Rachel");

    Team* serverTeam = new Team("Server Team");
    Team* cooksTeam = new Team("Cook Team");
    Team* completeStaff = new Team("Restaurant Staff");

    serverTeam->addMember(s1);
    cooksTeam->addMember(c2);
    cooksTeam->addMember(a1);
    completeStaff->addMember(serverTeam);
    completeStaff->addMember(cooksTeam);

    facade.AddParticipant(completeStaff);
    facade.AssignParticipant(s1, greetCustomer);
    facade.AssignParticipant(s1, getOrders);
    facade.AssignParticipant(c2, cleanDishes);
    facade.AssignParticipant(c2, prepFood);
    facade.AssignParticipant(a1, cookOrders);
}


void MenuWorkflowMang(){
    cout << "\033[1;36m\n        --- MANAGE --       \n\n\033[0m";

    cout << "1. View teams      | 3. View available work  | 5. View progress \n";
    cout << "2. View workflows  | 4. View history         | 0. <-- back      \n\n";

    cout<< "choice - ";
}
void MenuHistoryWorkflow(TIVIDYFacade& facade){
    cout << "\033[1;36m\n ========== WORKFLOW HISTORY ========== \n\n\033[0m";

    cout << "Current version: " + facade.getVersion();
    cout<<"\n\n";
    cout<<"0. <-- Back              | 1. Save Current Version\n";
    cout<<"2. View Saved Versions   | 3. Restore Previous Version\n";
    /*cout << "\n\n1. Save current version\n";
    cout << "2. View saved versions\n";
    cout << "3. Restore a previous version\n";
    cout << "0. <-- Back\n\n";*/

    cout<< "choice - ";
}
//can maybe reuse below func for the team input stuff - remove if not want :3 - Rei
void TeamsViewMenu(int optionChosen, vector<Team*>& teams, TIVIDYFacade& facade){
    switch(optionChosen){
        //view Team members
        case 1:{
            cout << "Enter the number of the team you would like to view: ";
            int choice = readInt();

            if (choice >= 1 && choice <= static_cast<int>(teams.size())){
                Team* selectedTeam = teams[choice - 1];
                cout << "\n--- " << selectedTeam->getName() << " ---\n";
                vector<Participant*> members = facade.getTeamMembers(selectedTeam);

                if (members.empty()){
                    cout << "This team has "<<"\033[91m"<<"no members"<<"\033[0m"<<".\n";
                }else{
                    for (size_t i = 0; i < members.size(); i++){
                        cout << i + 1 << ". "<< members[i]->getName() << "\n";
                    }
                }
            }else{
                cout << "\033[91m"<<"Invalid choice"<<"\033[0m"<<".\n";
            }
            break;
        }
        //assign work
        case 2:{
            cout <<"Enter the Team you would like to"<<"\033[96m "<<" assign work"<<"\033[0m"<<" to";
            string question;
            getline(cin, question);
            break;
        }
    }
}

int main(){
    TIVIDYFacade facade;
    bool animationSetup = false;
    bool restSetup = false;
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
                clearConsole();

                cout<< "\033[34m"<< "-----------------------------------------------" << "\n";
                cout << "      ANIMATION STUDIO WORKFLOW MANAGER  ＼(^o^)／" << "\n";
                cout << "-----------------------------------------------"<<"\033[0m" << "\n\n";

                if (!animationSetup){
                    setupAnimationScenario(facade);
                    animationSetup = true;
                }

                MenuWorkflowMang();

                int managaeChoice;
                managaeChoice =readInt();

                switch(managaeChoice){ 
                    //view Teams
                    case 1:{
                        
                        clearConsole();

                        vector<Team*> teams = facade.getTeams();
                        if (teams.empty()){
                            cout << "No teams found.\n";
                            break;
                        }
                        cout << "\033[33m|\nAvailable teams:\n \n \033[0m";
                        for (size_t i = 0; i < teams.size(); i++){
                            cout << i + 1 << ". " << teams[i]->getName() << "\n";
                        }
                        cout<<"\n";

                        cout << "\033[34m| 1. View team members   | 2. assign work  | 5. | 0. <-- back | \033[0m\n\n";
                        cout<< "choice - ";
                        int optionChosen;
                        optionChosen = readInt();

                        switch(optionChosen){
                            //view Team members
                            case 1:{
                                cout << "Enter the number of the team you would like to view: ";
                                int choice = readInt();

                                if (choice >= 1 && choice <= static_cast<int>(teams.size())){
                                    Team* selectedTeam = teams[choice - 1];
                                    cout << "\n--- " << selectedTeam->getName() << " ---\n";
                                    vector<Participant*> members =
                                        facade.getTeamMembers(selectedTeam);

                                    if (members.empty()){
                                        cout << "This team has no members.\n";
                                    }
                                    else{
                                        for (size_t i = 0; i < members.size(); i++){
                                            cout << i + 1 << ". "<< members[i]->getName() << "\n";
                                        }
                                    }
                                }
                                else{
                                    cout << "Invalid choice.\n";
                                }
                                break;
                            }

                            //assign work
                            case 2:{
                                cout <<"Enter the Team you would like to assign work to";
                                string question;
                                getline(cin, question);

                                break;
                            }
                        }
                        break;
                    }
                    //view Workflows
                    case 2:{
                        getWorkflows(facade);
                        break;
                    }
                    //view available work
                    case 3:{
                    
                       /*for (workflows that are not assigned){
                            getWorkflows(facade);
                        }
                        */
                        displayWorkItems(facade);
                        break;
                    }
                    //view history
                    case 4:{
                        MenuHistoryWorkflow(facade);
                        int optionChosen;
                        optionChosen = readInt();
                        switch(optionChosen){
                            do{//loop for workflow management screen
                                case 0:{
                                    cout<<"Return to regular workflow screen\n";
                                    break;
                                }
                                case 1:{
                                    facade.saveVersion();
                                    break;
                                }
                                case 2:{
                                    int topVersions = facade.getVersion();
                                    if(topVersions==0){
                                        cout<<"There are no currently saved versions of workflow\n";
                                    }else{
                                        cout<<"====  CURRENTLY SAVED VERSIONS  ====\n";
                                        for(int i=0; i<=topVersions; i++){
                                            cout<<"V. "<<i<<"\n";
                                        }
                                    }
                                    break;
                                }
                                case 3:{
                                    int* prevVersion = new int(facade.getVersion()-1);
                                    facade.regressVersion(prevVersion);
                                    break;
                                }
                                default:{
                                    cout<<"\033[91m"<<"Invalid option provided.\n";
                                    cout<<"Please ensure you choose 0-3\n\033[0m";
                                }
                            }while(optionChosen!=0);
                        }
                        break;
                    }
                    //view progrss
                    case 5:{
                        cout << facade.getProgress();
                        break;
                    }
                    case 0:{
                        
                        cin.get();
                        break;
                    }
                }
                break;
            }
            //CRestaurant
            case 2:{
                cout << "\033[96m-----------------------------------------------" << "\n";
                cout << "      RESTAURANT WORKFLOW MANAGER  ᕕ( ᐛ )ᕗ    " << "\n";
                cout << "-----------------------------------------------\033[0m" << "\n\n";
                if(!restSetup){
                    setupRestaurantScenario(facade);
                    restSetup = true;
                }
                MenuWorkflowMang();

                int managaeChoice;
                managaeChoice =readInt();

                switch(managaeChoice){
                    case 1:{
                        clearConsole();
                        vector<Team*> teams = facade.getTeams();
                        if(teams.empty()){
                            cout<<"No teams found.\n";
                            break;
                        }
                        vector<Team*> teams = facade.getTeams();
                        if (teams.empty()){
                            cout << "No teams found.\n";
                            break;
                        }
                        cout << "\033[33m|\nAvailable teams:\n \n \033[0m";
                        for (size_t i = 0; i < teams.size(); i++){
                            cout << i + 1 << ". " << teams[i]->getName() << "\n";
                        }
                        cout<<"\n";

                        cout << "\033[34m| 1. View team members   | 2. assign work  | 5. | 0. <-- back | \033[0m\n\n";
                        cout<< "choice - ";
                        int optionChosen;
                        optionChosen = readInt();
                    //idk if this works properly - rei
                        //TeamsViewMenu(optionChosen, teams&, facade&);
                        break;
                    }
                    case 2:{
                        getWorkflows(facade);
                        break;
                    }
                    //view available work
                    case 3:{
                    
                       /*for (workflows that are not assigned){
                            getWorkflows(facade);
                        }
                        */ 
                        displayWorkItems(facade);
                        break;
                    }
                    //view history
                    case 4:{
                        MenuHistoryWorkflow(facade);
                        int optionChosen;
                        optionChosen = readInt();
                        switch(optionChosen){
                            do{//loop for workflow management screen
                                case 0:{
                                    cout<<"Return to regular workflow screen\n";
                                    break;
                                }
                                case 1:{
                                    facade.saveVersion();
                                    break;
                                }
                                case 2:{
                                    int topVersions = facade.getVersion();
                                    if(topVersions==0){
                                        cout<<"There are no currently saved versions of workflow\n";
                                    }else{
                                        cout<<"====  CURRENTLY SAVED VERSIONS  ====\n";
                                        for(int i=0; i<=topVersions; i++){
                                            cout<<"V. "<<i<<"\n";
                                        }
                                    }
                                    break;
                                }
                                case 3:{
                                    int* prevVersion = new int(facade.getVersion()-1);
                                    facade.regressVersion(prevVersion);
                                    break;
                                }
                                default:{
                                    cout<<"\033[91m"<<"Invalid option provided.\n";
                                    cout<<"Please ensure you choose 0-3\n\033[0m";
                                }
                            }while(optionChosen!=0);
                        }
                        break;
                    }
                    //view progrss
                    case 5:{
                        cout << facade.getProgress();
                        break;
                    }
                    case 0:{
                        
                        cin.get();
                        break;
                    }
                }

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