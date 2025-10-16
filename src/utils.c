#include "utils.h"
#include <psapi.h>

HWND get_game_window()
{
    return FindWindowA(NULL, "Growtopia");
}

HMODULE get_game_exe_module()
{
    return GetModuleHandleA(NULL);
}

void *patch_memory(void *dest, const void *src, size_t count)
{
    DWORD old = 0;

    if (!VirtualProtect(dest, count, PAGE_EXECUTE_READWRITE, &old))
    {
        return NULL;
    }
    void *out = memcpy(dest, src, count);

    if (!VirtualProtect(dest, count, old, &old))
    {
        /*
         * Treat failure to set old page protection as a failed operation altogether, as incorrect memory page
         * protection values could trip anti-tamper mechanics on certain clients
         */
        return NULL;
    }

    return out;
}

void *find_memory(const uint16_t *pattern, size_t count)
{
    if (pattern == NULL || count == 0)
    {
        return NULL;
    }

    MODULEINFO mi;
    if (!GetModuleInformation(GetCurrentProcess(), get_game_exe_module(), &mi, sizeof(mi)))
    {
        return NULL;
    }

    uint8_t *cur = mi.lpBaseOfDll;
    uint8_t *end = cur + mi.SizeOfImage - count;

    /* Locate the first non-wildcard (-1) byte in the pattern */
    size_t anchor = 0;
    while (anchor < count && pattern[anchor] == (uint16_t)-1)
    {
        ++anchor;
    }

    if (anchor == count)
    {
        /* If the entire pattern is just wildcards, anything matches */
        return (void *)cur;
    }

    while (cur <= end)
    {
        int ok = 1;
        for (size_t i = anchor; i < count; i++)
        {
            if (pattern[i] == (uint16_t)-1)
            {
                /* Skip wildcards */
                continue;
            }

            if (*(cur + i) != pattern[i])
            {
                ok = 0;
                break;
            }
        }

        if (ok)
        {
            return (void *)(cur);
        }
        cur++;
    }

    return NULL;
}
