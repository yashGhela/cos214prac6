#include "TIVIDYFacade.h"

#include "WorkFlow.h"
#include "WorkItem.h"
#include "Participant.h"
#include "Member.h"
#include "Team.h"
#include "Roles.h"
#include "RolesApprover.h"
#include "Event.h"

using namespace std;

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstdlib>
#include <limits>
#include <algorithm>
#include <iomanip>

// ============================================================================
//  Input / output helpers
// ============================================================================

void clearConsole() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Reads a whole line of input and trims it. Used for names, which can contain
// spaces ("Chef Crazy Dave"), so "cin >> name" cannot be used for them.
string readLine(const string& prompt) {
    cout << prompt;
    string line;
    getline(cin, line);

    size_t start = line.find_first_not_of(" \t\r");
    if (start == string::npos)
        return "";
    size_t end = line.find_last_not_of(" \t\r");
    return line.substr(start, end - start + 1);
}

// Reads a menu number. Returns -1 for anything that is not a number.
// If the input stream has closed (EOF) it returns 0, so every menu treats it as
// "back" / "exit" and the program unwinds instead of looping forever.
int readInt() {
    int value;
    if (!(cin >> value)) {
        if (cin.eof())
            return 0;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return -1;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

void printError(const string& message) {
    cout << "\033[91m" << message << "\033[0m\n";
}

struct ActionOption {
    const char* label;
    ActionRegistry action;
};

static const ActionOption actionOptions[] = {
    {"Begin work",       ActionRegistry::BeginWork},
    {"Send for review",  ActionRegistry::InReview},
    {"Approve",          ActionRegistry::Approved},
    {"Cancel",           ActionRegistry::Cancel},
    {"Finish",           ActionRegistry::Finish}
};
static const int actionOptionCount = 5;

string actionLabel(ActionRegistry a) {
    for (int i = 0; i < actionOptionCount; ++i)
        if (actionOptions[i].action == a)
            return actionOptions[i].label;
    return "unknown";
}

// Shows the actions and lets the user pick one. Returns false on an invalid choice.
bool chooseAction(ActionRegistry& out) {
    cout << "\nWhat should this event do?\n";
    for (int i = 0; i < actionOptionCount; ++i)
        cout << i + 1 << ". " << actionOptions[i].label << "\n";
    cout << "choice - ";
    int choice = readInt();
    if (choice < 1 || choice > actionOptionCount)
        return false;
    out = actionOptions[choice - 1].action;
    return true;
}

string statusOf(const WorkFlowComponent& c) {
    if (c.isCancelled())  return "cancelled";
    if (c.isFinished())   return "finished";
    if (c.isApproved())   return "approved";
    if (c.isInReview())   return "in review";
    if (c.isInProgress()) return "in progress";
    return "created";
}

// ============================================================================
//  Scenario: one facade per scenario
//
//  The facade holds a single workflow, and its participants point at that
//  workflow's work items. If all scenarios shared one facade, entering the
//  Restaurant would delete the Animation workflow while the Animation
//  participants still pointed at its work items. Each scenario therefore gets
//  its own facade, and the participants it creates are freed with it.
// ============================================================================

struct Scenario {
    TIVIDYFacade facade;
    vector<Participant*> owned; // participants created for this scenario
    bool ready;                 // has the scenario been set up yet?
    EventRegistry events;

    Scenario() : ready(false) {}
    ~Scenario() {
        for (size_t i = 0; i < owned.size(); ++i)
            delete owned[i];
    }

    // Remembers a participant so it is deleted with the scenario.
    template <typename T>
    T* own(T* participant) {
        owned.push_back(participant);
        return participant;
    }

private:
    Scenario(const Scenario&);
    Scenario& operator=(const Scenario&);
};

bool isRegistered(TIVIDYFacade& facade, Participant* p) {
    vector<Participant*> all = facade.getParticipants();
    return find(all.begin(), all.end(), p) != all.end();
}

// AssignParticipant() only accepts participants that were registered with
// AddParticipant(), and registering a team does not register its members.
void registerOnce(TIVIDYFacade& facade, Participant* p) {
    if (p != nullptr && !isRegistered(facade, p))
        facade.AddParticipant(p);
}

// ============================================================================
//  Display helpers
// ============================================================================

void displayParticipants(int index, const string& name, string assignedWorkItem, int width = 22, int indent = 6) {
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

void getParticipants(TIVIDYFacade& facade) {
    vector<Participant*> participants = facade.getParticipants();
    if (participants.empty()) {
        cout << "No participants found.\n";
        return;
    }

    for (size_t i = 0; i < participants.size(); i++) {
        Participant* participant = participants[i];
        const map<string, WorkItem*>& assignedWork = participant->getAssignedWork();

        if (assignedWork.empty()) {
            displayParticipants(i + 1, participant->getName(), "No assigned work");
        } else {
            for (map<string, WorkItem*>::const_iterator it = assignedWork.begin();
                 it != assignedWork.end(); ++it) {
                if (it->second != nullptr) {
                    displayParticipants(i + 1, participant->getName(), it->second->getName());
                }
            }
        }
        cout << "\n";
    }
}

// Menu option 2: every workflow (root and nested) with its status.
void getWorkflows(TIVIDYFacade& facade) {
    vector<WorkFlow*> workflows = facade.getWorkflows();
    if (workflows.empty()) {
        printError("No workflows");
        return;
    }
    cout << "====  WORKFLOWS  ====\n\n";
    int workflowCount = 0;
    for (size_t i = 0; i < workflows.size(); i++) {
        cout << ++workflowCount << ". " << workflows[i]->getName()
             << "  [" << statusOf(*workflows[i]) << ", "
             << workflows[i]->getChildren().size() << " children]\n";
    }
}

// Menu option 3: work items that have not been started yet, grouped by workflow.
void displayWorkItems(TIVIDYFacade& facade) {
    vector<WorkFlow*> workflows = facade.getWorkflows();
    if (workflows.empty()) {
        printError("No workflows are assigned");
        return;
    }
    int flowCount = 0;
    cout << "====  AVAILABLE WORK ITEMS  ====\n\n";
    for (size_t i = 0; i < workflows.size(); i++) {
        int workitemCount = 0;
        cout << "\033[96m" << ++flowCount << ". " << workflows[i]->getName() << "\033[0m\n";
        vector<WorkFlowComponent*> children = workflows[i]->getChildren();
        for (size_t j = 0; j < children.size(); j++) {
            // children also contains nested workflows; only list real work items
            WorkItem* item = dynamic_cast<WorkItem*>(children[j]);
            if (item != nullptr && item->isCreated()) {
                cout << "\033[95m" << ++workitemCount << ". " << item->getName() << "\033[0m\n";
            }
        }
    }
}

// ============================================================================
//  Scenario setup
// ============================================================================

void setupAnimationScenario(Scenario& sc) {
    TIVIDYFacade& facade = sc.facade;

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
    Participant* p1 = sc.own(new Member("Dorris"));
    Participant* p2 = sc.own(new Member("DavidMan"));

    // Create teams
    Team* productionTeam = sc.own(new Team("Production Team"));
    Team* assetTeam = sc.own(new Team("Asset Production Team"));
    Team* designTeam = sc.own(new Team("Design Team"));

    // Add participants to subteams
    designTeam->addMember(p1);
    assetTeam->addMember(p2);

    // Add subteams to the main team
    productionTeam->addMember(designTeam);
    productionTeam->addMember(assetTeam);

    // Register everyone: assigning work only works for registered participants
    facade.AddParticipant(productionTeam);
    facade.AddParticipant(designTeam);
    facade.AddParticipant(assetTeam);
    facade.AddParticipant(p1);
    facade.AddParticipant(p2);

    // Assign work
    facade.AssignParticipant(p1, Modelling);
    facade.AssignParticipant(p2, Texturing);
}

void setupRestaurantScenario(Scenario& sc) {
    TIVIDYFacade& facade = sc.facade;

    facade.CreateWorkFlow("Hungry Cat Restaurant");
    WorkFlow* food = dynamic_cast<WorkFlow*>(facade.GetCurrentWorkflow());
    if (food == nullptr) {
        cout << "Oops, something went wrong with setting up the scenario!" << endl;
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

    Participant* s1 = sc.own(new Member("Mark"));
    // c1 is owned by a1: RolesApprover deletes the participant it decorates,
    // so c1 must not also be added to the scenario's owned list.
    Participant* c1 = new Member("Chef Crazy Dave");
    Roles* a1 = sc.own(new RolesApprover(c1));
    Participant* c2 = sc.own(new Member("Rachel"));

    Team* serverTeam = sc.own(new Team("Server Team"));
    Team* cooksTeam = sc.own(new Team("Cook Team"));
    Team* completeStaff = sc.own(new Team("Restaurant Staff"));

    serverTeam->addMember(s1);
    cooksTeam->addMember(c2);
    cooksTeam->addMember(a1);
    completeStaff->addMember(serverTeam);
    completeStaff->addMember(cooksTeam);

    // Register everyone: assigning work only works for registered participants
    facade.AddParticipant(completeStaff);
    facade.AddParticipant(serverTeam);
    facade.AddParticipant(cooksTeam);
    facade.AddParticipant(s1);
    facade.AddParticipant(c2);
    facade.AddParticipant(a1);

    facade.AssignParticipant(s1, greetCustomer);
    facade.AssignParticipant(s1, getOrders);
    facade.AssignParticipant(c2, cleanDishes);
    facade.AssignParticipant(c2, prepFood);
    facade.AssignParticipant(a1, cookOrders);
}

// ============================================================================
//  Menus shared by every scenario
// ============================================================================

void MenuWorkflowMang(bool custom) {
    cout << "\033[1;36m\n        --- MANAGE --       \n\n\033[0m";

    cout << "1. View teams      | 3. View available work  | 5. View progress \n";
    cout << "2. View workflows  | 4. View history         | 6. Run current workflow \n";
    if (custom)
        cout << "7. Customise Workflow\n";
    cout << "0. <-- Back\n\n";

    cout << "choice - ";
}

void MenuHistoryWorkflow(TIVIDYFacade& facade) {
    cout << "\033[1;36m\n ========== WORKFLOW HISTORY ========== \n\n\033[0m";

    cout << "Current version: " << facade.getVersion();
    cout << "\n\n";
    cout << "0. <-- Back              | 1. Save Current Version\n";
    cout << "2. View Saved Versions   | 3. Restore Previous Version\n\n";

    cout << "choice - ";
}

// Assign a registered participant (member or team) to a work item, by name.
void assignWorkMenu(TIVIDYFacade& facade) {
    vector<Participant*> workers = facade.getParticipants();
    vector<WorkItem*> workitems = facade.getWorkItems();
    if (workers.empty() || workitems.empty()) {
        printError("There are no workers or work items assigned to the workflow.");
        return;
    }

    string workerName = readLine("Enter name of participant to be assigned EXACTLY: ");
    Participant* worker = nullptr;
    for (size_t i = 0; i < workers.size(); i++) {
        if (workers[i]->getName() == workerName) {
            worker = workers[i];
            break;
        }
    }
    if (worker == nullptr) {
        cout << "\033[93m" << workerName << "\033[0m " << "\033[91m" << "could not be found.\n\033[0m";
        return;
    }

    string workitemName = readLine("\nEnter name of work item to be assigned EXACTLY: ");
    WorkItem* item = nullptr;
    for (size_t i = 0; i < workitems.size(); i++) {
        if (workitems[i]->getName() == workitemName) {
            item = workitems[i];
            break;
        }
    }
    if (item == nullptr) {
        cout << "\033[93m" << workitemName << "\033[0m " << "\033[91m" << "could not be found.\n\033[0m";
        return;
    }

    cout << "\n";
    // the facade prints the confirmation (or the reason it refused)
    facade.AssignParticipant(worker, item);
}

// Menu option 1: list the teams, then view a team's members or assign work.
void teamsMenu(TIVIDYFacade& facade) {
    vector<Team*> teams = facade.getTeams();
    if (teams.empty()) {
        // Checked before clearing the screen so the message is not wiped.
        printError("No teams found. Create a team and finish customising first.");
        return;
    }
    clearConsole();
    cout << "\033[33m\nAvailable teams:\n\n\033[0m";
    for (size_t i = 0; i < teams.size(); i++) {
        cout << i + 1 << ". " << teams[i]->getName() << "\n";
    }
    cout << "\n";

    cout << "\033[34m| 1. View team members   | 2. Assign work  | 0. <-- Back | \033[0m\n\n";
    cout << "choice - ";
    int optionChosen = readInt();

    switch (optionChosen) {
        // view team members
        case 1: {
            cout << "Enter the number of the team you would like to view: ";
            int choice = readInt();

            if (choice >= 1 && choice <= static_cast<int>(teams.size())) {
                Team* selectedTeam = teams[choice - 1];
                cout << "\n--- " << selectedTeam->getName() << " ---\n";
                vector<Participant*> members = facade.getTeamMembers(selectedTeam);

                if (members.empty()) {
                    cout << "This team has " << "\033[91m" << "no members" << "\033[0m" << ".\n";
                } else {
                    for (size_t i = 0; i < members.size(); i++) {
                        cout << i + 1 << ". " << members[i]->getName() << "\n";
                    }
                }
            } else {
                printError("Invalid choice.");
            }
            break;
        }
        // assign work
        case 2: {
            assignWorkMenu(facade);
            break;
        }
        case 0:
            break;
        default:
            printError("Invalid choice!");
    }
}

// Menu option 4: save and restore versions. Stays open until the user goes back.
void historyMenu(TIVIDYFacade& facade) {
    int choice;
    do {
        MenuHistoryWorkflow(facade);
        choice = readInt();

        switch (choice) {
            case 0:
                break;
            // save current version of workflow
            case 1: {
                facade.saveVersion();
                break;
            }
            // display saved versions of workflow (numbered from 1)
            case 2: {
                int topVersions = facade.getVersion();
                if (topVersions == 0) {
                    cout << "There are currently no saved versions of the workflow\n";
                } else {
                    cout << "====  CURRENTLY SAVED VERSIONS  ====\n";
                    for (int i = 1; i <= topVersions; i++) {
                        cout << "V. " << i << "\n";
                    }
                }
                break;
            }
            // return to a previous version of workflow
            case 3: {
                int topVersions = facade.getVersion();
                if (topVersions == 0) {
                    cout << "There are no saved versions to restore.\n";
                    break;
                }
                cout << "Enter the version number to restore (1-" << topVersions << "): ";
                int version = readInt();
                facade.regressVersion(&version);
                break;
            }
            default: {
                printError("Invalid option provided.\nPlease ensure you choose 0-3");
            }
        }
    } while (choice != 0);
}

// ============================================================================
//  Custom scenario: building a workflow by hand
// ============================================================================

template <typename T>
T* findByName(const vector<T*>& items, const string& name) {
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i]->getName() == name)
            return items[i];
    }
    return nullptr;
}

Participant* findParticipant(const vector<Member*>& members, const vector<Team*>& teams, const string& name) {
    Participant* found = findByName(members, name);
    if (found == nullptr)
        found = findByName(teams, name);
    return found;
}

// True if `root` is `target`, or `target` is a team somewhere below `root`.
// Used to stop a team being added to itself or into one of its own sub-teams.
bool teamContains(Team* root, Participant* target) {
    if (root == target)
        return true;
    const map<string, Participant*>& members = root->getMembers();
    for (map<string, Participant*>::const_iterator it = members.begin(); it != members.end(); ++it) {
        Team* sub = dynamic_cast<Team*>(it->second);
        if (sub != nullptr && teamContains(sub, target))
            return true;
    }
    return false;
}

void customiseWorkflow(Scenario& sc) {
    TIVIDYFacade& facade = sc.facade;
    WorkFlow* root = dynamic_cast<WorkFlow*>(facade.GetCurrentWorkflow());
    if (root == nullptr) {
        printError("There is no workflow to customise.");
        return;
    }

    vector<WorkFlow*> currentflows;
    vector<Member*> currentParticipants;
    vector<Team*> currentTeams;
    vector<WorkItem*> currentWorkItems;
    set<WorkItem*> placedItems; // items already inside a sub-workflow

    int workflowCustomChoice;
    do {
        cout << "\n";
        cout << "1. Add Workflow                  | 2. Add Member/Team to Main Workflow     | 3. Add Work Item to Workflow\n";
        cout << "4. Assign Member/Team to Team    | 5. Assign Work Item to Member           | 6. Create Member\n";
        cout << "7. Create Team                   | 8. Create Work Item                     | 9. Create Event\n";
        cout << "10. Add Reactor to Event         | 0. Finish Customisation\n\n";
        cout << "choice - ";
        workflowCustomChoice = readInt();

        switch (workflowCustomChoice) {
            case 1: {
                string workflowName = readLine("Enter name for workflow: ");
                if (workflowName.empty()) {
                    printError("Invalid name entered!");
                    break;
                }
                if (findByName(currentflows, workflowName) != nullptr) {
                    printError("A workflow with that name already exists.");
                    break;
                }
                currentflows.push_back(new WorkFlow(workflowName));
                cout << "\033[94m" << workflowName << " has been created.\n\033[0m";
                break;
            }
            case 2: {
                if (currentParticipants.empty() && currentTeams.empty()) {
                    printError("There are no participants to assign.");
                    break;
                }
                string workerName = readLine("Enter name of member/team to be assigned EXACTLY: ");
                Participant* worker = findParticipant(currentParticipants, currentTeams, workerName);
                if (worker == nullptr) {
                    cout << workerName << " cannot be found.\n";
                    break;
                }
                registerOnce(facade, worker);
                break;
            }
            case 3: {
                if (currentflows.empty() || currentWorkItems.empty()) {
                    printError("There are either no work items or no sub-workflows to use.");
                    break;
                }
                string workitemName = readLine("Enter name of work item to be assigned EXACTLY: ");
                WorkItem* item = findByName(currentWorkItems, workitemName);
                if (item == nullptr) {
                    printError(workitemName + " cannot be found.");
                    break;
                }
                if (placedItems.count(item) > 0) {
                    printError(workitemName + " is already in a sub-workflow.");
                    break;
                }
                string workflowName = readLine("\nEnter name of sub-workflow to be assigned to EXACTLY: ");
                WorkFlow* flow = findByName(currentflows, workflowName);
                if (flow == nullptr) {
                    cout << workflowName << " cannot be found.\n";
                    break;
                }
                cout << "Adding " << workitemName << " to sub-workflow " << workflowName << ".\n";
                flow->addChild(item);
                placedItems.insert(item);
                break;
            }
            case 4: {
                if (currentTeams.empty() || (currentParticipants.empty() && currentTeams.size() < 2)) {
                    printError("There need to be a team, and a member or another team, to add.");
                    break;
                }
                string toBeAdd = readLine("Enter name of member/team to be assigned EXACTLY: ");
                Participant* toAdd = findParticipant(currentParticipants, currentTeams, toBeAdd);
                if (toAdd == nullptr) {
                    cout << toBeAdd << " could not be found.\n";
                    break;
                }
                string addTo = readLine("Enter name of team to be assigned to EXACTLY: ");
                Team* target = findByName(currentTeams, addTo);
                if (target == nullptr) {
                    cout << addTo << " cannot be found.\n";
                    break;
                }
                Team* addingTeam = dynamic_cast<Team*>(toAdd);
                if (toAdd == target || (addingTeam != nullptr && teamContains(addingTeam, target))) {
                    printError("Cannot add " + toBeAdd + " to " + addTo + ": a team cannot contain itself.");
                    break;
                }
                target->addMember(toAdd);
                cout << "\033[94m" << toBeAdd << " has been added to " << addTo << ".\n\033[0m";
                break;
            }
            case 5: {
                if (currentWorkItems.empty() || currentParticipants.empty()) {
                    cout << "There are either no members or work items to assign\n";
                    break;
                }
                string workitemName = readLine("Enter name of work item to be assinged EXACTLY: ");
                WorkItem* item = findByName(currentWorkItems, workitemName);
                if (item == nullptr) {
                    printError(workitemName + " cannot be found.");
                    break;
                }
                string memberName = readLine("Enter name of member to be assigned to EXACTLY: ");
                Member* member = findByName(currentParticipants, memberName);
                if (member == nullptr) {
                    cout << memberName << " cannot be found.\n";
                    break;
                }
                cout << "Adding " << item->getName() << " to member " << member->getName() << ".\n";
                member->assignWork(item);
                break;
            }
            case 6: {
                string memberName = readLine("Enter name of new member: ");
                if (memberName.empty()) {
                    printError("Invalid name entered!");
                    break;
                }
                if (findParticipant(currentParticipants, currentTeams, memberName) != nullptr) {
                    printError("A member or team with that name already exists.");
                    break;
                }
                Member* newMember = sc.own(new Member(memberName));
                currentParticipants.push_back(newMember);
                cout << "\033[94m" << memberName << " has been created.\n\033[0m";
                break;
            }
            case 7: {
                string teamName = readLine("Enter name of new team: ");
                if (teamName.empty()) {
                    printError("Invalid name entered!");
                    break;
                }
                if (findParticipant(currentParticipants, currentTeams, teamName) != nullptr) {
                    printError("A member or team with that name already exists.");
                    break;
                }
                Team* newTeam = sc.own(new Team(teamName));
                currentTeams.push_back(newTeam);
                cout << "\033[94m" << teamName << " has been created.\n\033[0m";
                break;
            }
            case 8: {
                string itemName = readLine("Enter name of new work item: ");
                if (itemName.empty()) {
                    printError("Invalid name entered!");
                    break;
                }
                if (findByName(currentWorkItems, itemName) != nullptr) {
                    printError("A work item with that name already exists.");
                    break;
                }
                WorkItem* newWorkitem = new WorkItem(itemName);
                currentWorkItems.push_back(newWorkitem);
                cout << "\033[94m" << itemName << " has been created.\n\033[0m";
                break;
            }
            case 9: {
            string eventName = readLine("Enter name of new event (e.g. Kitchen Closed): ");
            if (eventName.empty()) {
                printError("Invalid name entered!");
                break;
            }
            if (sc.events.exists(eventName)) {
                printError("An event with that name already exists.");
                break;
            }
            ActionRegistry action;
            if (!chooseAction(action)) {
                printError("Invalid action chosen.");
                break;
            }
            sc.events.define(eventName, action);
            cout << "\033[94m" << EventRegistry::normalise(eventName)
                << " has been created (" << actionLabel(action) << ").\n\033[0m";
            break;
            }
            case 10: {
                vector<string> names = sc.events.all();
                if (names.empty()) {
                    printError("There are no events yet. Create one first.");
                    break;
                }
                cout << "Events:\n";
                for (size_t i = 0; i < names.size(); i++)
                    cout << i + 1 << ". " << names[i] << "\n";

                string eventName = readLine("Enter name of event EXACTLY: ");
                Event* event = sc.events.get(eventName);
                if (event == nullptr) {
                    cout << eventName << " cannot be found.\n";
                    break;
                }

                string reactorName = readLine("Enter name of workflow or work item that should react EXACTLY: ");
                WorkFlowObserver* reactor = nullptr;
                if (reactorName == root->getName()) {
                    reactor = root;
                } else {
                    WorkFlow* flow = findByName(currentflows, reactorName);
                    if (flow != nullptr)
                        reactor = flow;
                    else
                        reactor = findByName(currentWorkItems, reactorName);
                }
                if (reactor == nullptr) {
                    cout << reactorName << " cannot be found.\n";
                    break;
                }

                // the main workflow already reaches everything inside it
                const vector<WorkFlowObserver*>& current = event->getReactors();
                WorkFlowObserver* rootObserver = static_cast<WorkFlowObserver*>(root);
                if (reactor != rootObserver &&
                    find(current.begin(), current.end(), rootObserver) != current.end()) {
                    printError("The main workflow already reacts to this event, and that covers everything inside it.");
                    break;
                }

                root->Attach(*event, reactor);
                cout << "\033[94m" << reactorName << " will now react to " << event->getName() << ".\n\033[0m";
                break;
            }
            case 0:
                break;
            default: {
                printError("Invalid choice!");
            }
        }
    } while (workflowCustomChoice != 0);

    // Finish: attach everything that was built to the main workflow.
    cout << "Finishing up customisation...\n";

    cout << "Adding Workflows...\n";
    for (size_t i = 0; i < currentflows.size(); i++) {
        root->addChild(currentflows[i]);
    }
    cout << "Adding Work Items...\n";
    for (size_t i = 0; i < currentWorkItems.size(); i++) {
        // items inside a sub-workflow already have a parent; giving them a second
        // one would make both workflows delete the same item
        if (placedItems.count(currentWorkItems[i]) == 0)
            facade.AddWorkItem(currentWorkItems[i]);
    }
    cout << "Adding Teams...\n";
    for (size_t i = 0; i < currentTeams.size(); i++) {
        registerOnce(facade, currentTeams[i]);
    }
    cout << "Adding Members...\n";
    for (size_t i = 0; i < currentParticipants.size(); i++) {
        registerOnce(facade, currentParticipants[i]);
    }
    cout << "Customisation complete! Returning to workflow menu.\n";
}

// ============================================================================
//  The manage menu, shared by all three scenarios
// ============================================================================

void triggerEventMenu(Scenario& sc) {
    WorkFlow* root = dynamic_cast<WorkFlow*>(sc.facade.GetCurrentWorkflow());
    if (root == nullptr) {
        printError("There is no workflow to run events on.");
        return;
    }

    vector<string> names = sc.events.all();
    if (names.empty()) {
        printError("No events have been created yet.");
        return;
    }

    cout << "====  EVENTS  ====\n\n";
    for (size_t i = 0; i < names.size(); i++) {
        Event* e = sc.events.get(names[i]);
        cout << i + 1 << ". " << names[i]
             << "  [" << actionLabel(e->getAction()) << ", "
             << e->getReactors().size() << " reactors]\n";
    }

    string eventName = readLine("\nEnter name of event to trigger EXACTLY: ");
    Event* event = sc.events.get(eventName);
    if (event == nullptr) {
        cout << eventName << " cannot be found.\n";
        return;
    }
    if (event->getReactors().empty()) {
        printError("Nobody reacts to this event yet. Add a reactor in the customise menu first.");
        return;
    }

    cout << "Triggering " << event->getName() << "...\n";
    root->notify(*event, root);
}

void manageLoop(Scenario& sc, bool custom) {
    TIVIDYFacade& facade = sc.facade;
    int manageChoice;
    do {
        MenuWorkflowMang(custom);
        manageChoice = readInt();

        switch (manageChoice) {
            // view teams
            case 1: {
                teamsMenu(facade);
                break;
            }
            // view workflows
            case 2: {
                getWorkflows(facade);
                break;
            }
            // view available work
            case 3: {
                displayWorkItems(facade);
                break;
            }
            // view history
            case 4: {
                historyMenu(facade);
                break;
            }
            // view progress
            case 5: {
                cout << "Progress: " << facade.getProgress() << "%\n";
                break;
            }
            // start workflow
            case 6: {
                facade.StartWorkFlow();
                break;
            }
            // customise (custom scenario only)
            case 7: {
                if (custom)
                    customiseWorkflow(sc);
                else
                    printError("Invalid choice!");
                break;
            }
            // trigger event (custom scenario only)
            case 8: {
                if (custom)
                    triggerEventMenu(sc);
                else
                    printError("Invalid choice!");
                break;
            }
            case 0:
                break;
            default: {
                printError("Invalid choice!");
            }
        }
    } while (manageChoice != 0);
}

// ============================================================================
//  main
// ============================================================================

int main() {
    // one facade (and one set of participants) per scenario
    Scenario animation;
    Scenario restaurant;
    Scenario custom;

    int ScenarioChoice;
    do {
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

        switch (ScenarioChoice) {
            // Animation Studio
            case 1: {
                clearConsole();

                cout << "\033[34m" << "-----------------------------------------------" << "\n";
                cout << "      ANIMATION STUDIO WORKFLOW MANAGER  ＼(^o^)／" << "\n";
                cout << "-----------------------------------------------" << "\033[0m" << "\n\n";

                if (!animation.ready) {
                    setupAnimationScenario(animation);
                    animation.ready = true;
                }
                manageLoop(animation, false);
                break;
            }
            // Restaurant
            case 2: {
                cout << "\033[96m-----------------------------------------------" << "\n";
                cout << "      RESTAURANT WORKFLOW MANAGER  ᕕ( ᐛ )ᕗ    " << "\n";
                cout << "-----------------------------------------------\033[0m" << "\n\n";

                if (!restaurant.ready) {
                    setupRestaurantScenario(restaurant);
                    restaurant.ready = true;
                }
                manageLoop(restaurant, false);
                break;
            }
            // Create a new scenario
            case 3: {
                cout << "\033[94m" << "-----------------------------------------------\n";
                cout << "         CUSTOM WORKFLOW  ⚞^. .^⚟\n";
                cout << "-----------------------------------------------\n\033[0m";

                if (!custom.ready) {
                    string customMainName;
                    do {
                        customMainName = readLine("Please enter a name for your main workflow, i.e. Constructor Bros.: ");
                    } while (customMainName.empty() && !cin.eof());

                    if (customMainName.empty())
                        break;

                    custom.facade.CreateWorkFlow(customMainName);
                    custom.ready = true;
                    cout << "\033[92m\n\n" << customMainName << " has been initialised!\n\n\033[0m";
                }
                manageLoop(custom, true);
                break;
            }
            case 0: {
                cout << "\033[1;31m" << "=====  QUITING TIVIDY  =====\n\033[0m";
                cout << "\033[1;32m\nFinished Managing workflows. Yippeee! ദ്ദി˙∇˙)ว \033[0m\n\n";
                break;
            }
            default: {
                printError("Invalid choice!");
                break;
            }
        }
        if (ScenarioChoice != 0) {
            cout << "\nPress ENTER to return to the main menu...";
            cin.get();
        }
    } while (ScenarioChoice != 0);

    return 0;
}
