/**
 * @file AdapterObserver.h
 * @brief Placeholder observer intended to adapt external notifications to WorkFlowObserver.
 */
#ifndef ADAPTEROBSERVER_H
#define ADAPTEROBSERVER_H

#include <string>

#include "WorkFlowObserver.h"

/**
 * @brief Observer intended to adapt an outside notification source to the WorkFlowObserver interface.
 *
 * @note This class is currently a stub: it declares a destructor and does not yet implement update().
 */
class AdapterObserver: public WorkFlowObserver{
private:
public:
    /** @brief Virtual destructor. */
    virtual ~AdapterObserver();
};

#endif