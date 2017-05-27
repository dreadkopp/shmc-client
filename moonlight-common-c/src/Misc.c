#include "Limelight-internal.h"

#define ENET_SERVICE_RETRIES 10

// This function wraps enet_host_service() and hides the fact that it must be called
// multiple times for retransmissions to work correctly. It is meant to be a drop-in
// replacement for enet_host_service().
int serviceEnetHost(ENetHost* client, ENetEvent* event, enet_uint32 timeoutMs) {
    int i;
    int ret = -1;
    
    // We need to call enet_host_service() multiple times to make sure retransmissions happen
    for (i = 0; i < ENET_SERVICE_RETRIES; i++) {
        ret = enet_host_service(client, event, timeoutMs / ENET_SERVICE_RETRIES);
        if (ret != 0 || timeoutMs == 0) {
            break;
        }
    }
    
    return ret;
}

int isBeforeSignedInt(int numA, int numB, int ambiguousCase) {
    // This should be the common case for most callers
    if (numA == numB) {
        return 0;
    }

    // If numA and numB have the same signs,
    // we can just do a regular comparison.
    if ((numA < 0 && numB < 0) || (numA >= 0 && numB >= 0)) {
        return numA < numB;
    }
    else {
        // The sign switch is ambiguous
        return ambiguousCase;
    }
}

int extractVersionQuadFromString(const char* string, int* quad) {
    char versionString[128];
    char* nextDot;
    char* nextNumber;
    int i;
    
    strcpy(versionString, string);
    nextNumber = versionString;
    
    for (i = 0; i < 4; i++) {
        if (i == 3) {
            nextDot = strchr(nextNumber, '\0');
        }
        else {
            nextDot = strchr(nextNumber, '.');
        }
        if (nextDot == NULL) {
            return -1;
        }
        
        // Cut the string off at the next dot
        *nextDot = '\0';
        
        quad[i] = atoi(nextNumber);
        
        // Move on to the next segment
        nextNumber = nextDot + 1;
    }
    
    return 0;
}

void LiInitializeStreamConfiguration(PSTREAM_CONFIGURATION streamConfig) {
    memset(streamConfig, 0, sizeof(*streamConfig));
}

void LiInitializeVideoCallbacks(PDECODER_RENDERER_CALLBACKS drCallbacks) {
    memset(drCallbacks, 0, sizeof(*drCallbacks));
}

void LiInitializeAudioCallbacks(PAUDIO_RENDERER_CALLBACKS arCallbacks) {
    memset(arCallbacks, 0, sizeof(*arCallbacks));
}

void LiInitializeConnectionCallbacks(PCONNECTION_LISTENER_CALLBACKS clCallbacks) {
    memset(clCallbacks, 0, sizeof(*clCallbacks));
}

void LiInitializeServerInformation(PSERVER_INFORMATION serverInfo) {
    memset(serverInfo, 0, sizeof(*serverInfo));
}
