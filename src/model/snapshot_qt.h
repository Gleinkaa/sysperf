// snapshot_qt.h — Qt-only adornment for Snapshot. Included by every Qt translation
// unit that uses the queued signal. Keeps QMetaType out of the Qt-free core.
//
// The queued cross-thread connection silently drops the custom struct unless the
// metatype is both declared (here) and registered at runtime (qRegisterMetaType in
// main()). See docs/ARCHITECTURE.md.
#pragma once

#include <QMetaType>
#include "model/snapshot.h"

Q_DECLARE_METATYPE(sysperf::Snapshot)
