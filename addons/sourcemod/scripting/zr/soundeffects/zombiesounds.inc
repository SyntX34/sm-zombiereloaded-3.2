/*
 * ============================================================================
 *
 *  Zombie:Reloaded
 *
 *  File:          zombiesounds.inc
 *  Type:          Core 
 *  Description:   Zombie sound effects.
 *
 *  Copyright (C) 2009-2013  Greyscale, Richard Helgeby
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 */

/**
 * Zombie sound types
 */
enum ZombieSounds
{
    Moan, /** Zombie's moan periodically */
    Groan, /** When zombie is hurt */
    Death, /** When a zombie is killed */
    Countdown, /** When motherzombies countdown */
}
 
/**
 * Array for storing zombie moaning timer handles per client.
 */
new Handle:tSEffectsMoan[MAXPLAYERS + 1];

/**
 * Number of sound commands executed by the player.
 */
new g_SEffectsCommandCount[MAXPLAYERS + 1];

/**
 * Timers for resetting sound command counters.
 */
new Handle:g_hSEffectsCommandTimer[MAXPLAYERS + 1];


/**
 * Initialize zombie sounds module.
 */
ZombieSoundsInit()
{
    // Hook cvars for changes
    ZombieSoundsHookCvars();
    
    // Precache sounds
    PrecacheSounds();
}

/**
 * Client is joining the server.
 * 
 * @param client    The client index.
 */
ZombieSoundsClientInit(client)
{
    // Reset timer handle.
    tSEffectsMoan[client] = INVALID_HANDLE;
    
    // Reset command counter and make sure there's no timer running.
    g_SEffectsCommandCount[client] = 0;
    ZREndTimer(g_hSEffectsCommandTimer[client]);
}

void PrecacheSounds()
{
    // Only precache if sounds are enabled
    if (GetConVarBool(g_hCvarsList[CVAR_SEFFECTS_MOAN_ENABLED]))
    {
        char sound[PLATFORM_MAX_PATH];
        char path[PLATFORM_MAX_PATH];
        char fullSoundPath[PLATFORM_MAX_PATH];
        
        // Get moan path from cvar
        GetConVarString(g_hCvarsList[CVAR_SEFFECTS_MOAN_PATH], path, sizeof(path));
        int min = GetConVarInt(g_hCvarsList[CVAR_SEFFECTS_MOAN_MIN]);
        int max = GetConVarInt(g_hCvarsList[CVAR_SEFFECTS_MOAN_MAX]);
        
        for (int i = min; i <= max; i++)
        {
            Format(sound, sizeof(sound), path, i);
            Format(fullSoundPath, sizeof(fullSoundPath), "sound/%s", sound);
            PrecacheSound(sound, true);
            AddFileToDownloadsTable(fullSoundPath);
        }
    }
    
    if (GetConVarBool(g_hCvarsList[CVAR_SEFFECTS_GROAN_ENABLED]))
    {
        char sound[PLATFORM_MAX_PATH];
        char path[PLATFORM_MAX_PATH];
        char fullSoundPath[PLATFORM_MAX_PATH];
        
        // Get groan path from cvar
        GetConVarString(g_hCvarsList[CVAR_SEFFECTS_GROAN_PATH], path, sizeof(path));
        int min = GetConVarInt(g_hCvarsList[CVAR_SEFFECTS_GROAN_MIN]);
        int max = GetConVarInt(g_hCvarsList[CVAR_SEFFECTS_GROAN_MAX]);
        
        for (int i = min; i <= max; i++)
        {
            Format(sound, sizeof(sound), path, i);
            Format(fullSoundPath, sizeof(fullSoundPath), "sound/%s", sound);
            PrecacheSound(sound, true);
            AddFileToDownloadsTable(fullSoundPath);
        }
    }
    
    if (GetConVarBool(g_hCvarsList[CVAR_SEFFECTS_DEATH_ENABLED]))
    {
        char sound[PLATFORM_MAX_PATH];
        char path[PLATFORM_MAX_PATH];
        char fullSoundPath[PLATFORM_MAX_PATH];
        
        // Get death path from cvar
        GetConVarString(g_hCvarsList[CVAR_SEFFECTS_DEATH_PATH], path, sizeof(path));
        int min = GetConVarInt(g_hCvarsList[CVAR_SEFFECTS_DEATH_MIN]);
        int max = GetConVarInt(g_hCvarsList[CVAR_SEFFECTS_DEATH_MAX]);
        
        for (int i = min; i <= max; i++)
        {
            Format(sound, sizeof(sound), path, i);
            Format(fullSoundPath, sizeof(fullSoundPath), "sound/%s", sound);
            PrecacheSound(sound, true);
            AddFileToDownloadsTable(fullSoundPath);
        }
    }
    
    if (GetConVarBool(g_hCvarsList[CVAR_SEFFECTS_COUNTDOWN_ENABLED]))
    {
        char sound[PLATFORM_MAX_PATH];
        char path[PLATFORM_MAX_PATH];
        char fullSoundPath[PLATFORM_MAX_PATH];
        
        // Get countdown path from cvar
        GetConVarString(g_hCvarsList[CVAR_SEFFECTS_COUNTDOWN_PATH], path, sizeof(path));
        int min = GetConVarInt(g_hCvarsList[CVAR_SEFFECTS_COUNTDOWN_MIN]);
        int max = GetConVarInt(g_hCvarsList[CVAR_SEFFECTS_COUNTDOWN_MAX]);
        
        for (int i = min; i <= max; i++)
        {
            Format(sound, sizeof(sound), path, i);
            Format(fullSoundPath, sizeof(fullSoundPath), "sound/%s", sound);
            PrecacheSound(sound, true);
            AddFileToDownloadsTable(fullSoundPath);
        }
    }
}

/**
 * Client is spawning into the game.
 * 
 * @param client    The client index.
 */
ZombieSoundsOnClientSpawn(client)
{
    // If timer is running, kill it.
    if (tSEffectsMoan[client] != INVALID_HANDLE)
    {
        KillTimer(tSEffectsMoan[client]);
    }
    
    // Reset timer handle.
    tSEffectsMoan[client] = INVALID_HANDLE;
    
    // Reset command counter and kill timer.
    g_SEffectsCommandCount[client] = 0;
    ZREndTimer(g_hSEffectsCommandTimer[client]);
}

/**
 * Client has been killed.
 * 
 * @param client    The client index.
 */
ZombieSoundsOnClientDeath(client)
{
    // If timer is running, kill it.
    if (tSEffectsMoan[client] != INVALID_HANDLE)
    {
        KillTimer(tSEffectsMoan[client]);
    }
    
    // Reset timer handle.
    tSEffectsMoan[client] = INVALID_HANDLE;
    
    // Reset command counter and kill timer.
    g_SEffectsCommandCount[client] = 0;
    ZREndTimer(g_hSEffectsCommandTimer[client]);
    
    // If player isn't a zombie, then stop.
    if (!InfectIsClientInfected(client))
    {
        return;
    }
    
    // Check if death sounds are enabled via cvar
    if (!GetConVarBool(g_hCvarsList[CVAR_SEFFECTS_DEATH_ENABLED]))
    {
        return;
    }
    
    // If death sound cvar is disabled, then stop.
    new bool:death = GetConVarBool(g_hCvarsList[CVAR_SEFFECTS_DEATH]);
    if (!death)
    {
        return;
    }
    
    // Get random death sound.
    decl String:sound[SOUND_MAX_PATH];
    ZombieSoundsGetRandomSound(sound, Death);
    
    SEffectsEmitSoundFromClient(client, sound);
}

/**
 * Client has been hurt.
 * 
 * @param client    The client index.
 */
ZombieSoundsOnClientHurt(client)
{
    // If player isn't a zombie, then stop.
    if (!InfectIsClientInfected(client))
    {
        return;
    }
    
    // Check if groan sounds are enabled via cvar
    if (!GetConVarBool(g_hCvarsList[CVAR_SEFFECTS_GROAN_ENABLED]))
    {
        return;
    }
    
    // Get groan factor, if 0, then stop.
    new groan = GetConVarInt(g_hCvarsList[CVAR_SEFFECTS_GROAN]);
    if (!groan)
    {
        return;
    }
    
    // 1 in 'groan' chance of groaning.
    if (GetRandomInt(1, groan) == 1)
    {
        // Get random groan sound.
        decl String:sound[SOUND_MAX_PATH];
        ZombieSoundsGetRandomSound(sound, Groan);
        
        SEffectsEmitSoundFromClient(client, sound);
    }
}

/**
 * Client has been infected.
 * 
 * @param client    The client index.
 */
ZombieSoundsOnClientInfected(client)
{
    // Check if moan sounds are enabled via cvar
    if (!GetConVarBool(g_hCvarsList[CVAR_SEFFECTS_MOAN_ENABLED]))
    {
        return;
    }
    
    // If interval is set to 0, then stop.
    new Float:interval = GetConVarFloat(g_hCvarsList[CVAR_SEFFECTS_MOAN]);
    if (!interval)
    {
        return;
    }
    
    // If timer is currently running, kill it.
    if (tSEffectsMoan[client] != INVALID_HANDLE)
    {
        KillTimer(tSEffectsMoan[client]);
    }
    
    // Start repeating timer.
    tSEffectsMoan[client] = CreateTimer(interval, ZombieSoundsMoanTimer, client, TIMER_FLAG_NO_MAPCHANGE|TIMER_REPEAT);
}

/**
 * Round ended.
 */
ZombieSoundsOnRoundEnd()
{
    ZombieSoundsResetCmdCounters();
    ZombieSoundsResetCmdTimers();
}

ZombieSoundsOnMapEnd()
{
    ZombieSoundsOnRoundEnd();
}

ZombieSoundsOnCommandsCreate()
{
    RegConsoleCmd("scream", ZombieSoundsScreamCommand, "Emits a scream sound, if the player is a zombie.");
    RegConsoleCmd("moan", ZombieSoundsMoanCommand, "Emits a moan sound, if the player is a zombie.");
}

/**
 * Gets a random zombie sound from configured path.
 *
 * @param sound     The randomly picked sound.
 * @param soundtype The type of sound to get. (See enum ZombieSounds)
 * @return          True if sound was successfully picked, false otherwise. 
 */    
bool:ZombieSoundsGetRandomSound(String:sound[], ZombieSounds:soundtype)
{
    // Declare cvar indexes
    new CvarsList:enabledCvar = CvarsList:-1;
    new CvarsList:pathCvar = CvarsList:-1;
    new CvarsList:minCvar = CvarsList:-1;
    new CvarsList:maxCvar = CvarsList:-1;
    
    switch(soundtype)
    {
        // Find moan sound.
        case Moan:
        {
            enabledCvar = CVAR_SEFFECTS_MOAN_ENABLED;
            pathCvar = CVAR_SEFFECTS_MOAN_PATH;
            minCvar = CVAR_SEFFECTS_MOAN_MIN;
            maxCvar = CVAR_SEFFECTS_MOAN_MAX;
        }
        // Find groan sound. (zombie shot)
        case Groan:
        {
            enabledCvar = CVAR_SEFFECTS_GROAN_ENABLED;
            pathCvar = CVAR_SEFFECTS_GROAN_PATH;
            minCvar = CVAR_SEFFECTS_GROAN_MIN;
            maxCvar = CVAR_SEFFECTS_GROAN_MAX;
        }
        // Find death sound.
        case Death:
        {
            enabledCvar = CVAR_SEFFECTS_DEATH_ENABLED;
            pathCvar = CVAR_SEFFECTS_DEATH_PATH;
            minCvar = CVAR_SEFFECTS_DEATH_MIN;
            maxCvar = CVAR_SEFFECTS_DEATH_MAX;
        }
        // Invalid case given.
        default:
        {
            // No handled case was given.
            return false;
        }
    }
    
    // Check if sound type is enabled
    if (!GetConVarBool(g_hCvarsList[enabledCvar]))
    {
        return false;
    }
    
    // Get path and range from cvars
    decl String:path[PLATFORM_MAX_PATH];
    GetConVarString(g_hCvarsList[pathCvar], path, sizeof(path));
    
    new min = GetConVarInt(g_hCvarsList[minCvar]);
    new max = GetConVarInt(g_hCvarsList[maxCvar]);
    
    // Validate range
    if (min <= 0 || max <= 0 || min > max)
    {
        return false;
    }
    
    // Pick a random integer between min and max sound file index.
    new randsound = GetRandomInt(min, max);
    
    // Format random index into sound path.
    Format(sound, SOUND_MAX_PATH, path, randsound);
    
    // Found sound.
    return true;
}

/**
 * Timer callback, repeats a moaning sound on zombies.
 * 
 * @param timer     The timer handle.
 * @param client    The client index.
 */
public Action:ZombieSoundsMoanTimer(Handle:timer, any:client)
{
    // If client isn't in-game or client is no longer a zombie, then stop.
    if (!IsClientInGame(client) || !InfectIsClientInfected(client))
    {
        // Reset timer handle.
        tSEffectsMoan[client] = INVALID_HANDLE;
        
        // Stop timer.
        return Plugin_Stop;
    }
    
    // Check if moan sounds are enabled via cvar
    if (!GetConVarBool(g_hCvarsList[CVAR_SEFFECTS_MOAN_ENABLED]))
    {
        // Reset timer handle.
        tSEffectsMoan[client] = INVALID_HANDLE;
        
        // Stop timer.
        return Plugin_Stop;
    }
    
    // Emit moan sound.
    ZombieSoundsMoan(client);
    
    // Allow timer to continue.
    return Plugin_Continue;
}

/**
 * Emits a moan sound from the specified client.
 *
 * @param client    Client index.
 */
ZombieSoundsMoan(client)
{
    // Get random moan sound.
    decl String:sound[SOUND_MAX_PATH];
    ZombieSoundsGetRandomSound(sound, Moan);
    
    // Emit sound from client.
    SEffectsEmitSoundFromClient(client, sound, SNDLEVEL_SCREAMING);
}

/**
 * Emits the scream sound (on infection) from the specified client.
 *
 * @param client    Client index.
 */
ZombieSoundsScream(client)
{
    decl String:sound[PLATFORM_MAX_PATH];
    GetConVarString(g_hCvarsList[CVAR_INFECT_SOUND], sound, sizeof(sound));
    
    // If cvar contains path, then continue.
    if (sound[0])
    {
        // Emit infect sound from infected client.
        SEffectsEmitSoundFromClient(client, sound, SNDLEVEL_SCREAMING);
    }
}

/**
 * Starts a reset timer for the client's command counter if not already started.
 *
 * @param client    Client index.
 */
ZombieSoundsCmdTimerCheck(client)
{
    // Only create timer if it doesn't exist.
    if (g_hSEffectsCommandTimer[client] == INVALID_HANDLE)
    {
        new Float:timespan = GetConVarFloat(g_hCvarsList[CVAR_SEFFECTS_COMMAND_TIMESPAN]);
        
        // Only create timer if time span is enabled.
        if (timespan > 0.0)
        {
            g_hSEffectsCommandTimer[client] = CreateTimer(timespan, ZombieSoundsCmdTimer, client, TIMER_FLAG_NO_MAPCHANGE | TIMER_REPEAT);
        }
    }
}

/**
 * Resets all command counters.
 */
ZombieSoundsResetCmdCounters()
{
    for (new client = 0; client <= MAXPLAYERS; client++)
    {
        g_SEffectsCommandCount[client] = 0;
    }
}

/**
 * Stops all command counter timers.
 */
ZombieSoundsResetCmdTimers()
{
    for (new client = 0; client <= MAXPLAYERS; client++)
    {
        ZREndTimer(g_hSEffectsCommandTimer[client]);
    }
}

/**
 * Returns whether a player is allowed to play a zombie sound or not.
 *
 * @param client    Client index.
 * @return          True if allowed, false otherwise.
 */
bool:ZombieSoundsCommandAllowed(client)
{
    new limit = GetConVarInt(g_hCvarsList[CVAR_SEFFECTS_COMMAND_LIMIT]);
    
    if (limit <= 0 ||
        g_SEffectsCommandCount[client] < limit)
    {
        return true;
    }
    
    return false;
}

/**
 * Scream command handler.
 */
public Action:ZombieSoundsScreamCommand(client, argc)
{
    if (IsClientInGame(client) &&
        IsPlayerAlive(client) &&
        InfectIsClientInfected(client) &&
        ZombieSoundsCommandAllowed(client))
    {
        ZombieSoundsScream(client);
        g_SEffectsCommandCount[client]++;
        ZombieSoundsCmdTimerCheck(client);
    }
    
    return Plugin_Handled;
}

/**
 * Moan command handler.
 */
public Action:ZombieSoundsMoanCommand(client, argc)
{
    // Check if moan sounds are enabled via cvar
    if (!GetConVarBool(g_hCvarsList[CVAR_SEFFECTS_MOAN_ENABLED]))
    {
        return Plugin_Handled;
    }
    
    if (IsClientInGame(client) &&
        IsPlayerAlive(client) &&
        InfectIsClientInfected(client) &&
        ZombieSoundsCommandAllowed(client))
    {
        ZombieSoundsMoan(client);
        g_SEffectsCommandCount[client]++;
        ZombieSoundsCmdTimerCheck(client);
    }
    
    return Plugin_Handled;
}

/**
 * Command counter reset timer.
 */
public Action:ZombieSoundsCmdTimer(Handle:timer, any:client)
{
    g_SEffectsCommandCount[client] = 0;
}

/**
 * Zombie sounds cvar change handler.
 */
public void ZombieSoundsCvarChanged(Handle convar, const char[] oldValue, const char[] newValue)
{
    // Get the cvar name to identify which one changed
    char cvarName[64];
    GetConVarName(convar, cvarName, sizeof(cvarName));
    
    // Check which cvar changed and take appropriate action
    if (StrEqual(cvarName, "zr_seffects_moan_enabled") ||
        StrEqual(cvarName, "zr_seffects_groan_enabled") ||
        StrEqual(cvarName, "zr_seffects_death_enabled") ||
        StrEqual(cvarName, "zr_seffects_countdown_enabled"))
    {
        // Sound enabled/disabled changed - stop or start timers immediately
        bool newEnabled = StringToInt(newValue) != 0;
        bool oldEnabled = StringToInt(oldValue) != 0;
        
        if (newEnabled != oldEnabled)
        {
            if (StrEqual(cvarName, "zr_seffects_moan_enabled"))
            {
                if (!newEnabled)
                {
                    // Disable all moan timers
                    for (int client = 1; client <= MaxClients; client++)
                    {
                        if (IsClientInGame(client) && InfectIsClientInfected(client))
                        {
                            if (tSEffectsMoan[client] != INVALID_HANDLE)
                            {
                                KillTimer(tSEffectsMoan[client]);
                                tSEffectsMoan[client] = INVALID_HANDLE;
                            }
                        }
                    }
                }
                else
                {
                    // Enable moan timers for all zombies
                    for (int client = 1; client <= MaxClients; client++)
                    {
                        if (IsClientInGame(client) && IsPlayerAlive(client) && InfectIsClientInfected(client))
                        {
                            ZombieSoundsOnClientInfected(client);
                        }
                    }
                }
            }
            
            LogEvent(false, LogType_Normal, LOG_CORE_EVENTS, LogModule_SEffects, "Cvar Changed", "Sound effect cvar \"%s\" changed from %s to %s", cvarName, oldValue, newValue);
        }
    }
    else if (StrContains(cvarName, "_path") != -1 || 
             StrContains(cvarName, "_min") != -1 || 
             StrContains(cvarName, "_max") != -1)
    {
        LogEvent(false, LogType_Normal, LOG_CORE_EVENTS, LogModule_SEffects, "Cvar Changed", "Sound path/range cvar \"%s\" changed from %s to %s. Map restart needed for full effect.", cvarName, oldValue, newValue);
    }
}

/**
 * Hook zombie sounds cvars for changes.
 */
void ZombieSoundsHookCvars()
{
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_MOAN_ENABLED], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_GROAN_ENABLED], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_DEATH_ENABLED], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_COUNTDOWN_ENABLED], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_MOAN_PATH], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_GROAN_PATH], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_DEATH_PATH], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_COUNTDOWN_PATH], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_MOAN_MIN], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_MOAN_MAX], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_GROAN_MIN], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_GROAN_MAX], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_DEATH_MIN], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_DEATH_MAX], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_COUNTDOWN_MIN], ZombieSoundsCvarChanged);
    HookConVarChange(g_hCvarsList[CVAR_SEFFECTS_COUNTDOWN_MAX], ZombieSoundsCvarChanged);
}