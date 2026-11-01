/**
 * @file TestScenario.cpp
 * @brief Commented-out demo scenario (animation studio) kept for reference.
 */
// #include <iostream>
// #include "WorkFlow.h"
// #include "WorkItem.h"
// #include "TIVIDYFacade.h"
// #include "Team.h"
// #include "Member.h"
// using namespace std;

// int main()
// {

//     TIVIDYFacade *facade = new TIVIDYFacade();

//     WorkFlow *wf = new WorkFlow("KeyFraming");

//     WorkItem *a = new WorkItem("First 40 frames");

//     WorkItem *b = new WorkItem("Second 40 frames");

//     wf->addChild(a);
//     wf->addChild(b);

//     Team *team = new Team("Key Frame Team");

//     Member *c = new Member("Peter Parker");

//     Member *d = new Member("Bruce Wayne");

//     team->addMember(c);
//     team->addMember(d);

//     a->setParticipant(c);
//     b->setParticipant(d);

//     team->doWork();

//     wf->createMemento();
// }