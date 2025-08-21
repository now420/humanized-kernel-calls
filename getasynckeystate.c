// windows 11 24h2 26100.4946

__int64 __fastcall NtUserGetAsyncKeyState(unsigned int virtualKey)
{
    bool shouldTrace = true;
    __int64 currentWin32Process;
    __int64 processObj;
    __int64 validatedWin32Process;
    int deviceResult = 0;
    __int64 (*queryDeviceFn)(void);
    int queryDeviceStatus;
    __int64 (*checkDeviceFn)(void);
    __int16 asyncKeyStateResult = 0;
    bool recorderInitFlag;
    __int64 userSessionState;
    int recorderFlag1;
    int recorderFlag2;
    __int64 sessionStateTemp;
    __int64 sessionInputState;
    int sessionInputFlags;
    __int64 sessionInputState2;
    bool recorderInitFlag2;
    __int64 userSessionState2;
    int recorderFlag3;
    int recorderFlag4;
    InputDelegation* inputDelegation;
    __int64 sessionContext;
    __int64 keyboardStateObj;
    InputDelegation* inputDelegation2;
    __int64 sessionContext2;

    __int64 etwTraceData = 0;
    int etwTraceFlags = 0;
    InputDelegation* inputDelegationCtx = nullptr;
    __int64 sessionSubState = 0;

    EnterLeaveCritShared::EnterLeaveCritShared(&inputDelegationCtx, 1LL);

    // check if input is bound to a process
    if ( *(_QWORD *)(((__int64 (*)(void))W32GetUserSessionState)() + 18896) )
    {
        currentWin32Process = PsGetCurrentProcessWin32Process();
        validatedWin32Process = currentWin32Process;
        if ( currentWin32Process )
        {
            processObj = -*(_QWORD *)currentWin32Process;
            validatedWin32Process = -(__int64)(*(_QWORD *)currentWin32Process != 0LL) & currentWin32Process;
        }
        if ( validatedWin32Process != *(_QWORD *)(*(_QWORD *)(W32GetUserSessionState(processObj) + 18896) + 464LL) )
            EtwTraceGetAsyncKeyState(inputDelegationCtx);
    }

    // get input device state
    queryDeviceFn = *(__int64 (**)(void))(*(_QWORD *)(W32GetWin32kBaseApiSetTable() + 48) + 688LL);
    if ( queryDeviceFn )
        queryDeviceStatus = queryDeviceFn();
    else
        queryDeviceStatus = -1073741637;

    if ( queryDeviceStatus >= 0 )
    {
        checkDeviceFn = *(__int64 (**)(void))(*(_QWORD *)(W32GetWin32kBaseApiSetTable() + 48) + 696LL);
        if ( checkDeviceFn )
            deviceResult = checkDeviceFn();
    }

    if ( deviceResult )
    {
        // input device not accessible
        if ( !(unsigned int)Feature_AsyncKeyStateRecentDown_Removal2__private_IsEnabledDeviceUsageNoInline() )
        {
FAIL_AND_SET_ERROR:
            UserSetLastError(5LL);
FAIL_RETURN_ZERO:
            asyncKeyStateResult = 0;
            goto EXIT_CRIT;
        }

        // tracing
        if ( WPP_GLOBAL_Control == (_UNKNOWN *)&WPP_GLOBAL_Control
          || (*((_DWORD *)WPP_GLOBAL_Control + 11) & 2) == 0
          || *((_BYTE *)WPP_GLOBAL_Control + 41) < 2u )
        {
            shouldTrace = false;
        }
        recorderInitFlag = WPP_RECORDER_INITIALIZED != (_UNKNOWN *)&WPP_RECORDER_INITIALIZED;
        if ( shouldTrace || recorderInitFlag )
        {
            userSessionState = W32GetUserSessionState(&WPP_RECORDER_INITIALIZED);
            LOBYTE(recorderFlag1) = recorderInitFlag;
            LOBYTE(recorderFlag2) = shouldTrace;
            WPP_RECORDER_AND_TRACE_SF_(
                *((_QWORD *)WPP_GLOBAL_Control + 3),
                recorderFlag2,
                recorderFlag1,
                *(_QWORD *)(userSessionState + 69128),
                2,
                2,
                100,
                (__int64)&WPP_7d857b1eaae03a1e9e7fd82e3d98c426_Traceguids);
        }
        asyncKeyStateResult = 0;
        goto FAIL_AND_SET_ERROR;
    }

    // foreground check
    if ( !(unsigned int)ApiSetEditionIsGpqForegroundAccessibleCurrent(1LL) )
    {
        sessionInputState = *(_QWORD *)(W32GetUserSessionState(sessionStateTemp) + 18880);
        sessionSubState = *(_QWORD *)(sessionInputState + 448);
        sessionInputFlags = *(_DWORD *)(sessionInputState + 456);

        sessionInputState2 = *(_QWORD *)(W32GetUserSessionState(sessionInputState) + 18880);
        etwTraceData = sessionSubState;
        etwTraceFlags = sessionInputFlags;
        EtwTraceUIPIInputError((_DWORD)inputDelegationCtx, 0, sessionInputState2, (unsigned int)&etwTraceData, 3);

        if ( !(unsigned int)Feature_AsyncKeyStateRecentDown_Removal2__private_IsEnabledDeviceUsageNoInline() )
            goto FAIL_AND_SET_ERROR;

        if ( WPP_GLOBAL_Control == (_UNKNOWN *)&WPP_GLOBAL_Control
          || (*((_DWORD *)WPP_GLOBAL_Control + 11) & 2) == 0
          || *((_BYTE *)WPP_GLOBAL_Control + 41) < 2u )
        {
            shouldTrace = false;
        }
        recorderInitFlag2 = WPP_RECORDER_INITIALIZED != (_UNKNOWN *)&WPP_RECORDER_INITIALIZED;
        if ( shouldTrace || recorderInitFlag2 )
        {
            userSessionState2 = W32GetUserSessionState(&WPP_RECORDER_INITIALIZED);
            LOBYTE(recorderFlag3) = recorderInitFlag2;
            LOBYTE(recorderFlag4) = shouldTrace;
            WPP_RECORDER_AND_TRACE_SF_(
                *((_QWORD *)WPP_GLOBAL_Control + 3),
                recorderFlag4,
                recorderFlag3,
                *(_QWORD *)(userSessionState2 + 69128),
                2,
                2,
                101,
                (__int64)&WPP_7d857b1eaae03a1e9e7fd82e3d98c426_Traceguids);
        }
        asyncKeyStateResult = 0;
        goto FAIL_AND_SET_ERROR;
    }

    // delegation check
    inputDelegation = inputDelegationCtx;
    if ( (unsigned __int8)IsKeyboardDelegationEnabledForThread(inputDelegationCtx) )
    {
        *(_DWORD *)(*((_QWORD *)inputDelegation + 65) + 124LL) = 0;
        *(_QWORD *)(*((_QWORD *)inputDelegation + 65) + 128LL) = 0LL;
        if ( !(unsigned int)Feature_AsyncKeyStateRecentDown_Removal2__private_IsEnabledDeviceUsageNoInline() )
            *(_QWORD *)(*((_QWORD *)inputDelegation + 65) + 136LL) = 0LL;
        goto FAIL_RETURN_ZERO;
    }

    // inlined getasynckeystate, this is a separate function in the actual decompilation
    if ( virtualKey < 0x100 )
    {
        __int16 recentDownFlag = 0;

        // check "recent key down" bitfield
        if ( !(unsigned int)Feature_AsyncKeyStateRecentDown_Removal2__private_IsEnabledDeviceUsageNoInline() )
        {
            __int64 session = W32GetUserSessionState(0);
            unsigned __int64 byteIndex = (unsigned __int64)(virtualKey >> 3);   // which byte
            unsigned int bitIndex = virtualKey & 7;                             // which bit in that byte

            if ( (*(_BYTE *)(session + 14456 + byteIndex) & (1 << bitIndex)) != 0 )
            {
                __int64 session2 = W32GetUserSessionState(0);
                recentDownFlag = 1;

                // clear the "recent down" bit
                unsigned __int8 val = *(unsigned __int8 *)(session2 + 14456 + byteIndex);
                val &= ~(1 << bitIndex);
                *(_BYTE *)(session2 + 14456 + byteIndex) = val;
            }
        }

        // check the "currently down" bitfield
        char isDown = *(_BYTE *)(W32GetUserSessionState(0) + 14328 + (virtualKey >> 2))
                      & (1 << (2 * (virtualKey & 3)));

        asyncKeyStateResult = recentDownFlag | 0x8000;
        if ( !isDown )
            asyncKeyStateResult = recentDownFlag;   // if not currently down, drop 0x8000
    }
    else
    {
        UserSetLastError(87LL);  // invalid VK code
        asyncKeyStateResult = 0;
    }

    // update delegation context with current session keyboard state
    keyboardStateObj = *(_QWORD *)(W32GetUserSessionState(sessionContext) + 19864);
    sessionSubState = keyboardStateObj;
    inputDelegation2 = inputDelegationCtx;
    *(_DWORD *)(*((_QWORD *)inputDelegationCtx + 65) + 124LL) = *(_DWORD *)(keyboardStateObj + 6988);
    *(_QWORD *)(*((_QWORD *)inputDelegation2 + 65) + 128LL) = *(_QWORD *)(W32GetUserSessionState(keyboardStateObj) + 14328);

    if ( !(unsigned int)Feature_AsyncKeyStateRecentDown_Removal2__private_IsEnabledDeviceUsageNoInline() )
        *(_QWORD *)(*((_QWORD *)inputDelegation2 + 65) + 136LL) = *(_QWORD *)(W32GetUserSessionState(sessionContext2) + 14456);

EXIT_CRIT:
    UserSessionSwitchLeaveCritWithNonPaged(0LL);
    return asyncKeyStateResult;
}
