Marlin IPTV-ES Content Decryption Module
=============================

## Description
Marlin IPTV-ES Content Decryption Module provides a way to acquire right and decrypt for Marlin IPTV-ES protected content.

## Supported OS
 * LINUX OS

## Supported EME version
 * 0.1b

### Deliverable of Marlin IPTV-ES Content Decryption Module
#### Source code
 * "CDM/include/MarlinCdmInterface.h"
   This header file defines interface of Marlin IPTV-ES CDM.
 * "CDM/include/MarlinCommonTypes.h"
   This header file includes defined values of Marlin IPTV-ES CDM.
 * "CDM/include/MarlinCdmEngine.h"
   This header file is for the engine part of Marlin IPTV-ES CDM.
 * "CDM/include/CdmSessionManager.h"
   This header file is for the internal module that generates SessionID.
 * "CDM/include/MarlinError.h"
   This header file includes error-defined values of Marlin IPTV-ES CDM.
 * "CDM/include/MarlinLog.h"
   This header file defines output log macros of Marlin IPTV-ES CDM.
 * "CDM/include/CMutex.h"
   This header file defines output exclusive control macros of Marlin IPTV-ES CDM.
 * "CDM/src/MarlinCdmInterface.cpp"
   This is the source code that implements the interface of Marlin IPTV-ES CDM.
 * "CDM/src/MarlinCdmEngine.cpp"
   This is the source code for the engine part of Marlin IPTV-ES CDM.
 * "CDM/src/CdmSessionManager.cpp"
   This is the source code is for the internal module that generates SessionID.

### Environment
You should prepare these environment, which is required by the Marlin IPTV-ES CDM.
 * HTML5(EME ver.0.1b) supported Web browser client
 * Marlin DRM server  
  To work with Marlin IPTV-ES aware Application, you should prepare or choose the encounter server.
 * Build environment supported embedded device(like DTV/STB/BD player)
  The library needs to be run on LINUX OS.(Confirmed OS is Fedora release 8)


## Supported functionality
 * Get Permission Protocol of Marlin IPTV-ES
 * Get Trusted Time Protocol of Marlin IPTV-ES
 * Decrypt MPEG-2 Transport Stream(without ECM)
 * Decrypt MPEG-2 Transport Stream(with ECM)
 * Decrypt MP4 Common encryption media
To get detail information to enable listed functions, Refer to MarlinCdmInterface.h.
Rest of other functionality is not supported.


## Marlin IPTV-ES CDM Agent Handler
 * Marlin Agent Handler module
The Marlin Agent Handler module provides an interface for access to Marlin IPTV-ES core module.
It must be implemented in accordance with the applicable Marlin IPTV-ES core module.
Please refer to Marlin Agent Handler interface specifications.

Marlin IPTV-ES Agent Handler provides an interface only (empty implementation).

 * "AgentHandler/include/MarlinAgentHandler.h"
   This header file defines interface of Marlin IPTV-ES Agent Handler.
 * "AgentHandler/include/MarlinAgentHandlerType.h"
   This header file includes defined values of Marlin IPTV-ES Agent Handler.
 * "AgentHandler/src/MarlinAgentHandler.cpp"
   This is the source code that implements Marlin IPTV-ES Agent Handler.


## Marlin IPTV-ES CDM Interface
Please refer to the "CDM/include/MarlinCdmInterface.h".


## Marlin IPTV-ES Agent Handler Interface
Please refer to the "AgentHandler/include/MarlinAgentHandler.h".


## Notes
 * The Marlin IPTV-ES CDM does not correspond to multi-thread.
