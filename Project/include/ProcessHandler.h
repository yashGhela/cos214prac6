#pragma once

class ProcessHandler{
    public:
    virtual ~ProcessHandler() = default;
    virtual void process(WorkItem& item);
};
