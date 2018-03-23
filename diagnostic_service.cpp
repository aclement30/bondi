#include "diagnostic_service.h"

bool DiagnosticService::isCompleted() {
    return completed;
}

bool DiagnosticService::isCancelled() {
    return cancelled;
}