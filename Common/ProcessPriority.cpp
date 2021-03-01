void SetProcessPriority()
{
// Suppresses Mac OS X Warning since logChannel isn't used.

#if defined(_WIN32) || defined(__linux__)

    ///- Handle affinity for multiple processors and process priority
    bool highPriority = true;

#ifdef _WIN32 // Windows

    HANDLE hProcess = GetCurrentProcess();

    if (highPriority)
    {
        if (SetPriorityClass(hProcess, HIGH_PRIORITY_CLASS))
            sLog->outInfo(LOG_DEFAULT, "Process priority class set to HIGH");
        else
            sLog->outError(LOG_DEFAULT, "Can't set process priority class.");
    }

#else // Linux

    if (highPriority)
    {
        if (setpriority(PRIO_PROCESS, 0, PROCESS_HIGH_PRIORITY))
            TC_LOG_ERROR(logChannel, "Can't set process priority class, error: %s", strerror(errno));
        else
            TC_LOG_INFO(logChannel, "Process priority class set to %i", getpriority(PRIO_PROCESS, 0));
    }

#endif
#endif
}