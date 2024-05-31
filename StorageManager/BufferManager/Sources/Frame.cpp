#include "Frame.h"

Frame::Frame() : pageID(-1), isPinned(false), dirtyFlag(false), pinCount(0), lastUsed(0) {
    // constructor que inicializa un frame con valores por defecto
}

void Frame::pin() {
    // marca el frame como "pin", indicando que está en uso
    isPinned = true;
    pinCount++;  // incrementa el contador de pin para este frame
}

void Frame::unpin() {
    if (pinCount > 0) {
        pinCount--;  // decrementar el contador de pines
        if (pinCount == 0) {
            isPinned = false;  // si no hay más pines, marcar como no "pinned"
        }
    }
}

void Frame::setDirty(bool flag) {
    // establece la bandera de suciedad para el frame
    dirtyFlag = flag;  // establece la bandera a true o false según el flag proporcionado
}

void Frame::reset() {
    pageID = -1; // manejar si esta o no en frame
    isPinned = false;
    dirtyFlag = false;
    pinCount = 0;
    lastUsed = 0;  // restablece el tiempo de último uso
}