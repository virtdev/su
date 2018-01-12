#ifndef _NOTIFIER_H
#define _NOTIFIER_H

#define enableNotifier() do { \
    if (_notifier) \
        _notifier->open(); \
} while (0)

#define disableNotifier() do { \
    if (_notifier) \
        _notifier->close(); \
} while (0)

#define setNotifier(notifier) do {_notifier = notifier;} while (0)

class Driver;
extern Driver *_notifier;

#endif
