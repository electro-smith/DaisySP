#pragma once
#ifndef __DAISY_PC_H__
#define __DAISY_PC_H__

#if defined(_WIN32)

#include <cstdint>
#include <cstdio>
#include <cfloat>
#include <windows.h>
#include "daisysp.h"
#include "hid/logger.h"

/**   @brief Quick and dirty PC simulation for running module unit tests
 *    @author Alexander Petrov-Savchenko (axp@soft-amp.com)
 *    @date December 2020
 */


/* fix discrepnacy in msvc/gcc functions */
constexpr auto isnanf = _isnanf;

/* no need for specific allocation */
#define DSY_SDRAM_BSS /* emtpy */

namespace daisy
{
/* Stub for blocking interrupts, ignore on Windows*/
class ScopedIrqBlocker
{
  public:
    ScopedIrqBlocker() { (void)0; }
    ~ScopedIrqBlocker() { (void)0; }
};


/** Simulation of Daisy System class 
* Only a few timestamping services are implemented
*/
class SystemPC
{
public:
    /** \return a uint32_t of ticks at (PCLk1 * 2)Hz
     ** */
    static uint32_t GetTick()
    {
        LARGE_INTEGER q;
        QueryPerformanceCounter(&q);
        return q.LowPart;
    }
    static uint32_t GetPClk1Freq()
    {
        LARGE_INTEGER q;
        QueryPerformanceFrequency(&q);
        return (uint32_t)(q.QuadPart / 2u);
    }
};

/** Simple simulation of Daisy platform on windows
  * TODO: consider moving to libDaisy
  * Add services as needed
  */
class DaisyPC
{
  public:
    DaisyPC() {}
    ~DaisyPC() {}
    /* redirect the log to stdout */
    using Log = daisy::Logger<daisy::LOGGER_INTERNAL>;

    /* Import logging functions */
    static constexpr auto Print     = Log::Print;
    static constexpr auto PrintLine = Log::PrintLine;

    /* Nothing to do for console output */
    static void StartLog(bool wait_for_pc = false) {}

    SystemPC system;
};






/** Stubs for UsbHandle class 
* Consider moving to a .cpp file if used in multiple places
*/

void UsbHandle::Init(UsbPeriph dev)
{
}

UsbHandle::Result UsbHandle::TransmitInternal(uint8_t* buff, size_t size)
{
    _write(STDOUT_FILENO, "[INT] ", 6);
    if (size == _write(STDOUT_FILENO, buff, size))
    {
        return UsbHandle::Result::OK;
    }
    else
    {
        return UsbHandle::Result::ERR;
    }
}
UsbHandle::Result UsbHandle::TransmitExternal(uint8_t* buff, size_t size)
{
    _write(STDERR_FILENO, "[EXT] ", 6);
    if (size == _write(STDERR_FILENO, buff, size))
    {
        return UsbHandle::Result::OK;
    }
    else
    {
        return UsbHandle::Result::ERR;
    }
}

void UsbHandle::SetReceiveCallback(ReceiveCallback cb, UsbPeriph dev)
{
}


} // namespace daisy

#endif // _WIN32

#endif //__DAISY_PC_H__
