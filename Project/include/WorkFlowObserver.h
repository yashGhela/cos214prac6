#ifndef WORKFLOWOBSERVER_H
#define WORKFLOWOBSERVER_H

class WorkFlowObserver{
private:
    
public:
    virtual ~WorkFlowObserver() = 0;
	virtual void update() = 0;

};

#endif

