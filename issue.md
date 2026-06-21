rik@laptop-erik-linux:~/source/sysperf$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Success
-- Found Threads: TRUE  
-- Performing Test HAVE_STDATOMIC
-- Performing Test HAVE_STDATOMIC - Success
-- Found WrapAtomic: TRUE  
-- Found OpenGL: /usr/lib/x86_64-linux-gnu/libOpenGL.so   
-- Found WrapOpenGL: TRUE  
-- Could NOT find XKB (missing: XKB_LIBRARY XKB_INCLUDE_DIR) (Required is at least version "0.5.0")
-- Found WrapVulkanHeaders: /usr/include  
-- Configuring done (0.5s)
-- Generating done (0.0s)
-- Build files have been written to: /home/erik/source/sysperf/build
erik@laptop-erik-linux:~/source/sysperf$ cmake --build build -j8
[ 12%] Building CXX object CMakeFiles/sysperf_core.dir/src/collector/meminfo.cpp.o
[ 12%] Building CXX object CMakeFiles/sysperf_core.dir/src/collector/proc_util.cpp.o
[ 12%] Building CXX object CMakeFiles/sysperf_core.dir/src/collector/cpu.cpp.o
[ 12%] Building CXX object CMakeFiles/sysperf_core.dir/src/collector/diskstats.cpp.o
[ 12%] Building CXX object CMakeFiles/sysperf_core.dir/src/collector/netdev.cpp.o
[ 12%] Building CXX object CMakeFiles/sysperf_core.dir/src/collector/gpu_fdinfo.cpp.o
[ 12%] Building CXX object CMakeFiles/sysperf_core.dir/src/collector/gpu_nvml.cpp.o
[ 14%] Building CXX object CMakeFiles/sysperf_core.dir/src/collector/self_metrics.cpp.o
[ 16%] Linking CXX static library libsysperf_core.a
[ 16%] Built target sysperf_core
[ 16%] Built target test_diskstats_autogen_timestamp_deps
[ 16%] Built target test_budget_smoke_autogen_timestamp_deps
[ 16%] Built target test_netdev_autogen_timestamp_deps
[ 16%] Built target test_cpu_autogen_timestamp_deps
[ 16%] Built target sysperf_autogen_timestamp_deps
[ 16%] Built target test_meminfo_autogen_timestamp_deps
[ 16%] Built target test_fdinfo_autogen_timestamp_deps
[ 16%] Built target test_self_autogen_timestamp_deps
[ 23%] Automatic MOC for target test_fdinfo
[ 23%] Automatic MOC for target test_diskstats
[ 23%] Automatic MOC for target test_budget_smoke
[ 23%] Automatic MOC for target test_netdev
[ 23%] Built target test_diskstats_autogen
[ 23%] Built target test_netdev_autogen
[ 23%] Built target test_budget_smoke_autogen
[ 23%] Built target test_fdinfo_autogen
[ 37%] Automatic MOC for target test_meminfo
[ 37%] Building CXX object tests/CMakeFiles/test_diskstats.dir/test_diskstats_autogen/mocs_compilation.cpp.o
[ 37%] Automatic MOC for target test_cpu
[ 37%] Automatic MOC for target test_self
[ 37%] Building CXX object tests/CMakeFiles/test_fdinfo.dir/test_fdinfo_autogen/mocs_compilation.cpp.o
[ 37%] Building CXX object tests/CMakeFiles/test_netdev.dir/test_netdev_autogen/mocs_compilation.cpp.o
[ 37%] Automatic MOC for target sysperf
[ 37%] Building CXX object tests/CMakeFiles/test_budget_smoke.dir/test_budget_smoke_autogen/mocs_compilation.cpp.o
[ 44%] Building CXX object tests/CMakeFiles/test_budget_smoke.dir/test_budget_smoke.cpp.o
[ 44%] Building CXX object tests/CMakeFiles/test_netdev.dir/test_netdev.cpp.o
[ 44%] Building CXX object tests/CMakeFiles/test_diskstats.dir/test_diskstats.cpp.o
[ 44%] Building CXX object tests/CMakeFiles/test_fdinfo.dir/test_fdinfo.cpp.o
[ 44%] Built target test_cpu_autogen
[ 44%] Built target test_self_autogen
[ 44%] Built target test_meminfo_autogen
[ 50%] Building CXX object tests/CMakeFiles/test_self.dir/test_self_autogen/mocs_compilation.cpp.o
[ 50%] Building CXX object tests/CMakeFiles/test_cpu.dir/test_cpu_autogen/mocs_compilation.cpp.o
[ 50%] Building CXX object tests/CMakeFiles/test_self.dir/test_self.cpp.o
[ 51%] Building CXX object tests/CMakeFiles/test_cpu.dir/test_cpu.cpp.o
[ 53%] Linking CXX executable test_self
[ 53%] Built target sysperf_autogen
[ 55%] Linking CXX executable test_diskstats
[ 57%] Building CXX object tests/CMakeFiles/test_meminfo.dir/test_meminfo_autogen/mocs_compilation.cpp.o
[ 58%] Building CXX object CMakeFiles/sysperf.dir/sysperf_autogen/mocs_compilation.cpp.o
[ 60%] Built target test_self
[ 62%] Building CXX object tests/CMakeFiles/test_meminfo.dir/test_meminfo.cpp.o
[ 64%] Building CXX object CMakeFiles/sysperf.dir/src/main.cpp.o
[ 66%] Linking CXX executable test_fdinfo
[ 67%] Linking CXX executable test_cpu
[ 69%] Built target test_diskstats
[ 71%] Building CXX object CMakeFiles/sysperf.dir/src/config.cpp.o
[ 73%] Built target test_fdinfo
[ 75%] Building CXX object CMakeFiles/sysperf.dir/src/collector/worker.cpp.o
[ 76%] Built target test_cpu
[ 78%] Building CXX object CMakeFiles/sysperf.dir/src/ui/mainwindow.cpp.o
[ 80%] Linking CXX executable test_netdev
[ 82%] Linking CXX executable test_budget_smoke
[ 83%] Built target test_netdev
[ 85%] Building CXX object CMakeFiles/sysperf.dir/src/ui/graphwidget.cpp.o
In file included from /usr/include/x86_64-linux-gnu/qt6/QtCore/qtypeinfo.h:6,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qglobal.h:1397,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qatomic.h:5,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qrefcount.h:7,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qbytearray.h:8,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/QByteArray:1,
                 from /home/erik/source/sysperf/src/config.cpp:3:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qcontainerfwd.h:28:7: error: conflicting declaration ‘using QStringList = class QList<QString>’
   28 | using QStringList = QList<QString>;
      |       ^~~~~~~~~~~
In file included from /home/erik/source/sysperf/src/config.cpp:1:
/home/erik/source/sysperf/src/config.h:8:7: note: previous declaration as ‘class QStringList’
    8 | class QStringList;
      |       ^~~~~~~~~~~
[ 87%] Linking CXX executable test_meminfo
[ 89%] Built target test_budget_smoke
[ 91%] Building CXX object CMakeFiles/sysperf.dir/src/ui/corebars.cpp.o
[ 92%] Built target test_meminfo
[ 94%] Building CXX object CMakeFiles/sysperf.dir/src/ui/panel.cpp.o
In file included from /usr/include/x86_64-linux-gnu/qt6/QtCore/qlist.h:992,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qobject.h:13,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qeventloop.h:7,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qcoreapplication.h:11,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/QCoreApplication:1,
                 from /home/erik/source/sysperf/src/config.cpp:4:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘void QListSpecialMethods<QString>::sort(Qt::CaseSensitivity)’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:70:39: error: cannot convert ‘QListSpecialMethodsBase<QString>::Self*’ {aka ‘QList<QString>*’} to ‘QStringList*’
   70 |     { QtPrivate::QStringList_sort(self(), cs); }
      |                                   ~~~~^~
      |                                       |
      |                                       QListSpecialMethodsBase<QString>::Self* {aka QList<QString>*}
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:26:54: note:   initializing argument 1 of ‘void QtPrivate::QStringList_sort(QStringList*, Qt::CaseSensitivity)’
   26 |     void Q_CORE_EXPORT QStringList_sort(QStringList *that, Qt::CaseSensitivity cs);
      |                                         ~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘qsizetype QListSpecialMethods<QString>::removeDuplicates()’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:72:58: error: cannot convert ‘QListSpecialMethodsBase<QString>::Self*’ {aka ‘QList<QString>*’} to ‘QStringList*’
   72 |     { return QtPrivate::QStringList_removeDuplicates(self()); }
      |                                                      ~~~~^~
      |                                                          |
      |                                                          QListSpecialMethodsBase<QString>::Self* {aka QList<QString>*}
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:27:71: note:   initializing argument 1 of ‘qsizetype QtPrivate::QStringList_removeDuplicates(QStringList*)’
   27 |     qsizetype Q_CORE_EXPORT QStringList_removeDuplicates(QStringList *that);
      |                                                          ~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘QString QListSpecialMethods<QString>::join(QStringView) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:75:41: error: no matching function for call to ‘QStringList_join(const QListSpecialMethodsBase<QString>::Self*, QStringView&)’
   75 |     { return QtPrivate::QStringList_join(self(), sep); }
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:28:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList*, QStringView)’
   28 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, QStringView sep);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:28:63: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   28 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, QStringView sep);
      |                                            ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:29:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList*, const QChar*, qsizetype)’
   29 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, const QChar *sep, qsizetype seplen);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:29:27: note:   candidate expects 3 arguments, 2 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:30:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList&, QLatin1StringView)’
   30 |     Q_CORE_EXPORT QString QStringList_join(const QStringList &list, QLatin1StringView sep);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:30:63: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList&’
   30 |     Q_CORE_EXPORT QString QStringList_join(const QStringList &list, QLatin1StringView sep);
      |                                            ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘QString QListSpecialMethods<QString>::join(QLatin1StringView) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:77:41: error: no matching function for call to ‘QStringList_join(const QListSpecialMethodsBase<QString>::Self&, QLatin1StringView&)’
   77 |     { return QtPrivate::QStringList_join(*self(), sep); }
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:28:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList*, QStringView)’
   28 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, QStringView sep);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:28:63: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self’ {aka ‘const QList<QString>’} to ‘const QStringList*’
   28 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, QStringView sep);
      |                                            ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:29:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList*, const QChar*, qsizetype)’
   29 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, const QChar *sep, qsizetype seplen);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:29:27: note:   candidate expects 3 arguments, 2 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:30:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList&, QLatin1StringView)’
   30 |     Q_CORE_EXPORT QString QStringList_join(const QStringList &list, QLatin1StringView sep);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:30:63: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self’ {aka ‘const QList<QString>’} to ‘const QStringList&’
   30 |     Q_CORE_EXPORT QString QStringList_join(const QStringList &list, QLatin1StringView sep);
      |                                            ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘QString QListSpecialMethods<QString>::join(QChar) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:79:41: error: no matching function for call to ‘QStringList_join(const QListSpecialMethodsBase<QString>::Self*, QChar*, int)’
   79 |     { return QtPrivate::QStringList_join(self(), &sep, 1); }
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:28:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList*, QStringView)’
   28 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, QStringView sep);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:28:27: note:   candidate expects 2 arguments, 3 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:29:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList*, const QChar*, qsizetype)’
   29 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, const QChar *sep, qsizetype seplen);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:29:63: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   29 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, const QChar *sep, qsizetype seplen);
      |                                            ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:30:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList&, QLatin1StringView)’
   30 |     Q_CORE_EXPORT QString QStringList_join(const QStringList &list, QLatin1StringView sep);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:30:27: note:   candidate expects 2 arguments, 3 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: At global scope:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:82:5: error: return type ‘class QStringList’ is incomplete
   82 |     { return QtPrivate::QStringList_filter(self(), str, cs); }
      |     ^
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘void QListSpecialMethods<QString>::filter(QStringView, Qt::CaseSensitivity) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:82:43: error: no matching function for call to ‘QStringList_filter(const QListSpecialMethodsBase<QString>::Self*, QStringView&, Qt::CaseSensitivity&)’
   82 |     { return QtPrivate::QStringList_filter(self(), str, cs); }
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:31:31: note: candidate: ‘QStringList QtPrivate::QStringList_filter(const QStringList*, QStringView, Qt::CaseSensitivity)’
   31 |     QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, QStringView str,
      |                               ^~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:31:69: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   31 |     QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, QStringView str,
      |                                                  ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:40:31: note: candidate: ‘QStringList QtPrivate::QStringList_filter(const QStringList*, const QRegularExpression&)’
   40 |     QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, const QRegularExpression &re);
      |                               ^~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:40:31: note:   candidate expects 2 arguments, 3 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘QStringList& QListSpecialMethods<QString>::replaceInStrings(QStringView, QStringView, Qt::CaseSensitivity)’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:85:48: error: no matching function for call to ‘QStringList_replaceInStrings(QListSpecialMethodsBase<QString>::Self*, QStringView&, QStringView&, Qt::CaseSensitivity&)’
   85 |         QtPrivate::QStringList_replaceInStrings(self(), before, after, cs);
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:35:24: note: candidate: ‘void QtPrivate::QStringList_replaceInStrings(QStringList*, QStringView, QStringView, Qt::CaseSensitivity)’
   35 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, QStringView before, QStringView after,
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:35:66: note:   no known conversion for argument 1 from ‘QListSpecialMethodsBase<QString>::Self*’ {aka ‘QList<QString>*’} to ‘QStringList*’
   35 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, QStringView before, QStringView after,
      |                                                     ~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:39:24: note: candidate: ‘void QtPrivate::QStringList_replaceInStrings(QStringList*, const QRegularExpression&, const QString&)’
   39 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, const QRegularExpression &rx, const QString &after);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:39:24: note:   candidate expects 3 arguments, 4 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:86:16: error: invalid initialization of reference of type ‘QStringList&’ from expression of type ‘QListSpecialMethodsBase<QString>::Self’ {aka ‘QList<QString>’}
   86 |         return *self();
      |                ^~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘QString QListSpecialMethods<QString>::join(const QString&) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:90:41: error: no matching function for call to ‘QStringList_join(const QListSpecialMethodsBase<QString>::Self*, const QChar*, qsizetype)’
   90 |     { return QtPrivate::QStringList_join(self(), sep.constData(), sep.size()); }
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:28:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList*, QStringView)’
   28 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, QStringView sep);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:28:27: note:   candidate expects 2 arguments, 3 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:29:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList*, const QChar*, qsizetype)’
   29 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, const QChar *sep, qsizetype seplen);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:29:63: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   29 |     QString Q_CORE_EXPORT QStringList_join(const QStringList *that, const QChar *sep, qsizetype seplen);
      |                                            ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:30:27: note: candidate: ‘QString QtPrivate::QStringList_join(const QStringList&, QLatin1StringView)’
   30 |     Q_CORE_EXPORT QString QStringList_join(const QStringList &list, QLatin1StringView sep);
      |                           ^~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:30:27: note:   candidate expects 2 arguments, 3 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: At global scope:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:92:5: error: return type ‘class QStringList’ is incomplete
   92 |     { return QtPrivate::QStringList_filter(self(), str, cs); }
      |     ^
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘void QListSpecialMethods<QString>::filter(const QString&, Qt::CaseSensitivity) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:92:43: error: no matching function for call to ‘QStringList_filter(const QListSpecialMethodsBase<QString>::Self*, const QString&, Qt::CaseSensitivity&)’
   92 |     { return QtPrivate::QStringList_filter(self(), str, cs); }
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:31:31: note: candidate: ‘QStringList QtPrivate::QStringList_filter(const QStringList*, QStringView, Qt::CaseSensitivity)’
   31 |     QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, QStringView str,
      |                               ^~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:31:69: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   31 |     QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, QStringView str,
      |                                                  ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:40:31: note: candidate: ‘QStringList QtPrivate::QStringList_filter(const QStringList*, const QRegularExpression&)’
   40 |     QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, const QRegularExpression &re);
      |                               ^~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:40:31: note:   candidate expects 2 arguments, 3 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘QStringList& QListSpecialMethods<QString>::replaceInStrings(const QString&, const QString&, Qt::CaseSensitivity)’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:95:48: error: no matching function for call to ‘QStringList_replaceInStrings(QListSpecialMethodsBase<QString>::Self*, const QString&, const QString&, Qt::CaseSensitivity&)’
   95 |         QtPrivate::QStringList_replaceInStrings(self(), before, after, cs);
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:35:24: note: candidate: ‘void QtPrivate::QStringList_replaceInStrings(QStringList*, QStringView, QStringView, Qt::CaseSensitivity)’
   35 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, QStringView before, QStringView after,
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:35:66: note:   no known conversion for argument 1 from ‘QListSpecialMethodsBase<QString>::Self*’ {aka ‘QList<QString>*’} to ‘QStringList*’
   35 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, QStringView before, QStringView after,
      |                                                     ~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:39:24: note: candidate: ‘void QtPrivate::QStringList_replaceInStrings(QStringList*, const QRegularExpression&, const QString&)’
   39 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, const QRegularExpression &rx, const QString &after);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:39:24: note:   candidate expects 3 arguments, 4 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:96:16: error: invalid initialization of reference of type ‘QStringList&’ from expression of type ‘QListSpecialMethodsBase<QString>::Self’ {aka ‘QList<QString>’}
   96 |         return *self();
      |                ^~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘QStringList& QListSpecialMethods<QString>::replaceInStrings(const QString&, QStringView, Qt::CaseSensitivity)’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:100:48: error: no matching function for call to ‘QStringList_replaceInStrings(QListSpecialMethodsBase<QString>::Self*, const QString&, QStringView&, Qt::CaseSensitivity&)’
  100 |         QtPrivate::QStringList_replaceInStrings(self(), before, after, cs);
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:35:24: note: candidate: ‘void QtPrivate::QStringList_replaceInStrings(QStringList*, QStringView, QStringView, Qt::CaseSensitivity)’
   35 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, QStringView before, QStringView after,
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:35:66: note:   no known conversion for argument 1 from ‘QListSpecialMethodsBase<QString>::Self*’ {aka ‘QList<QString>*’} to ‘QStringList*’
   35 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, QStringView before, QStringView after,
      |                                                     ~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:39:24: note: candidate: ‘void QtPrivate::QStringList_replaceInStrings(QStringList*, const QRegularExpression&, const QString&)’
   39 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, const QRegularExpression &rx, const QString &after);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:39:24: note:   candidate expects 3 arguments, 4 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:101:16: error: invalid initialization of reference of type ‘QStringList&’ from expression of type ‘QListSpecialMethodsBase<QString>::Self’ {aka ‘QList<QString>’}
  101 |         return *self();
      |                ^~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘QStringList& QListSpecialMethods<QString>::replaceInStrings(QStringView, const QString&, Qt::CaseSensitivity)’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:105:48: error: no matching function for call to ‘QStringList_replaceInStrings(QListSpecialMethodsBase<QString>::Self*, QStringView&, const QString&, Qt::CaseSensitivity&)’
  105 |         QtPrivate::QStringList_replaceInStrings(self(), before, after, cs);
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:35:24: note: candidate: ‘void QtPrivate::QStringList_replaceInStrings(QStringList*, QStringView, QStringView, Qt::CaseSensitivity)’
   35 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, QStringView before, QStringView after,
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:35:66: note:   no known conversion for argument 1 from ‘QListSpecialMethodsBase<QString>::Self*’ {aka ‘QList<QString>*’} to ‘QStringList*’
   35 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, QStringView before, QStringView after,
      |                                                     ~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:39:24: note: candidate: ‘void QtPrivate::QStringList_replaceInStrings(QStringList*, const QRegularExpression&, const QString&)’
   39 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, const QRegularExpression &rx, const QString &after);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:39:24: note:   candidate expects 3 arguments, 4 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:106:16: error: invalid initialization of reference of type ‘QStringList&’ from expression of type ‘QListSpecialMethodsBase<QString>::Self’ {aka ‘QList<QString>’}
  106 |         return *self();
      |                ^~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘bool QListSpecialMethods<QString>::contains(QLatin1StringView, Qt::CaseSensitivity) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:113:45: error: no matching function for call to ‘QStringList_contains(const QListSpecialMethodsBase<QString>::Self*, QLatin1StringView&, Qt::CaseSensitivity&)’
  113 |     { return QtPrivate::QStringList_contains(self(), str, cs); }
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:33:24: note: candidate: ‘bool QtPrivate::QStringList_contains(const QStringList*, QStringView, Qt::CaseSensitivity)’
   33 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QStringView str, Qt::CaseSensitivity cs);
      |                        ^~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:33:64: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   33 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QStringView str, Qt::CaseSensitivity cs);
      |                                             ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:34:24: note: candidate: ‘bool QtPrivate::QStringList_contains(const QStringList*, QLatin1StringView, Qt::CaseSensitivity)’
   34 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QLatin1StringView str, Qt::CaseSensitivity cs);
      |                        ^~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:34:64: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   34 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QLatin1StringView str, Qt::CaseSensitivity cs);
      |                                             ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘bool QListSpecialMethods<QString>::contains(QStringView, Qt::CaseSensitivity) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:115:45: error: no matching function for call to ‘QStringList_contains(const QListSpecialMethodsBase<QString>::Self*, QStringView&, Qt::CaseSensitivity&)’
  115 |     { return QtPrivate::QStringList_contains(self(), str, cs); }
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:33:24: note: candidate: ‘bool QtPrivate::QStringList_contains(const QStringList*, QStringView, Qt::CaseSensitivity)’
   33 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QStringView str, Qt::CaseSensitivity cs);
      |                        ^~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:33:64: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   33 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QStringView str, Qt::CaseSensitivity cs);
      |                                             ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:34:24: note: candidate: ‘bool QtPrivate::QStringList_contains(const QStringList*, QLatin1StringView, Qt::CaseSensitivity)’
   34 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QLatin1StringView str, Qt::CaseSensitivity cs);
      |                        ^~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:34:64: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   34 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QLatin1StringView str, Qt::CaseSensitivity cs);
      |                                             ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘bool QListSpecialMethods<QString>::contains(const QString&, Qt::CaseSensitivity) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:118:45: error: no matching function for call to ‘QStringList_contains(const QListSpecialMethodsBase<QString>::Self*, const QString&, Qt::CaseSensitivity&)’
  118 |     { return QtPrivate::QStringList_contains(self(), str, cs); }
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:33:24: note: candidate: ‘bool QtPrivate::QStringList_contains(const QStringList*, QStringView, Qt::CaseSensitivity)’
   33 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QStringView str, Qt::CaseSensitivity cs);
      |                        ^~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:33:64: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   33 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QStringView str, Qt::CaseSensitivity cs);
      |                                             ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:34:24: note: candidate: ‘bool QtPrivate::QStringList_contains(const QStringList*, QLatin1StringView, Qt::CaseSensitivity)’
   34 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QLatin1StringView str, Qt::CaseSensitivity cs);
      |                        ^~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:34:64: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   34 |     bool Q_CORE_EXPORT QStringList_contains(const QStringList *that, QLatin1StringView str, Qt::CaseSensitivity cs);
      |                                             ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: At global scope:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:126:5: error: return type ‘class QStringList’ is incomplete
  126 |     { return QtPrivate::QStringList_filter(self(), re); }
      |     ^
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘void QListSpecialMethods<QString>::filter(const QRegularExpression&) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:126:43: error: no matching function for call to ‘QStringList_filter(const QListSpecialMethodsBase<QString>::Self*, const QRegularExpression&)’
  126 |     { return QtPrivate::QStringList_filter(self(), re); }
      |              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:31:31: note: candidate: ‘QStringList QtPrivate::QStringList_filter(const QStringList*, QStringView, Qt::CaseSensitivity)’
   31 |     QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, QStringView str,
      |                               ^~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:31:31: note:   candidate expects 3 arguments, 2 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:40:31: note: candidate: ‘QStringList QtPrivate::QStringList_filter(const QStringList*, const QRegularExpression&)’
   40 |     QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, const QRegularExpression &re);
      |                               ^~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:40:69: note:   no known conversion for argument 1 from ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
   40 |     QStringList Q_CORE_EXPORT QStringList_filter(const QStringList *that, const QRegularExpression &re);
      |                                                  ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘QStringList& QListSpecialMethods<QString>::replaceInStrings(const QRegularExpression&, const QString&)’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:129:48: error: no matching function for call to ‘QStringList_replaceInStrings(QListSpecialMethodsBase<QString>::Self*, const QRegularExpression&, const QString&)’
  129 |         QtPrivate::QStringList_replaceInStrings(self(), re, after);
      |         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:35:24: note: candidate: ‘void QtPrivate::QStringList_replaceInStrings(QStringList*, QStringView, QStringView, Qt::CaseSensitivity)’
   35 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, QStringView before, QStringView after,
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:35:24: note:   candidate expects 4 arguments, 3 provided
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:39:24: note: candidate: ‘void QtPrivate::QStringList_replaceInStrings(QStringList*, const QRegularExpression&, const QString&)’
   39 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, const QRegularExpression &rx, const QString &after);
      |                        ^~~~~~~~~~~~~~~~~~~~~~~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:39:66: note:   no known conversion for argument 1 from ‘QListSpecialMethodsBase<QString>::Self*’ {aka ‘QList<QString>*’} to ‘QStringList*’
   39 |     void Q_CORE_EXPORT QStringList_replaceInStrings(QStringList *that, const QRegularExpression &rx, const QString &after);
      |                                                     ~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:130:16: error: invalid initialization of reference of type ‘QStringList&’ from expression of type ‘QListSpecialMethodsBase<QString>::Self’ {aka ‘QList<QString>’}
  130 |         return *self();
      |                ^~~~~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘qsizetype QListSpecialMethods<QString>::indexOf(const QRegularExpression&, qsizetype) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:133:49: error: cannot convert ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
  133 |     { return QtPrivate::QStringList_indexOf(self(), re, from); }
      |                                             ~~~~^~
      |                                                 |
      |                                                 const QListSpecialMethodsBase<QString>::Self* {aka const QList<QString>*}
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:41:68: note:   initializing argument 1 of ‘qsizetype QtPrivate::QStringList_indexOf(const QStringList*, const QRegularExpression&, qsizetype)’
   41 |     qsizetype Q_CORE_EXPORT QStringList_indexOf(const QStringList *that, const QRegularExpression &re, qsizetype from);
      |                                                 ~~~~~~~~~~~~~~~~~~~^~~~
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h: In member function ‘qsizetype QListSpecialMethods<QString>::lastIndexOf(const QRegularExpression&, qsizetype) const’:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:135:53: error: cannot convert ‘const QListSpecialMethodsBase<QString>::Self*’ {aka ‘const QList<QString>*’} to ‘const QStringList*’
  135 |     { return QtPrivate::QStringList_lastIndexOf(self(), re, from); }
      |                                                 ~~~~^~
      |                                                     |
      |                                                     const QListSpecialMethodsBase<QString>::Self* {aka const QList<QString>*}
/usr/include/x86_64-linux-gnu/qt6/QtCore/qstringlist.h:42:72: note:   initializing argument 1 of ‘qsizetype QtPrivate::QStringList_lastIndexOf(const QStringList*, const QRegularExpression&, qsizetype)’
   42 |     qsizetype Q_CORE_EXPORT QStringList_lastIndexOf(const QStringList *that, const QRegularExpression &re, qsizetype from);
      |                                                     ~~~~~~~~~~~~~~~~~~~^~~~
In file included from /home/erik/source/sysperf/build/sysperf_autogen/R4O6FMLXZH/../../../src/collector/worker.h:8,
                 from /home/erik/source/sysperf/build/sysperf_autogen/R4O6FMLXZH/moc_worker.cpp:10,
                 from /home/erik/source/sysperf/build/sysperf_autogen/mocs_compilation.cpp:2:
/home/erik/source/sysperf/src/config.h:8:7: error: using typedef-name ‘using QStringList = class QList<QString>’ after ‘class’
    8 | class QStringList;
      |       ^~~~~~~~~~~
In file included from /usr/include/x86_64-linux-gnu/qt6/QtCore/qtypeinfo.h:6,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qglobal.h:1397,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qnamespace.h:8,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qobjectdefs.h:12,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qobject.h:10,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/QObject:1,
                 from /home/erik/source/sysperf/build/sysperf_autogen/R4O6FMLXZH/../../../src/collector/worker.h:6:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qcontainerfwd.h:28:7: note: ‘using QStringList = class QList<QString>’ has a previous declaration here
   28 | using QStringList = QList<QString>;
      |       ^~~~~~~~~~~
In file included from /home/erik/source/sysperf/src/collector/worker.h:8,
                 from /home/erik/source/sysperf/src/collector/worker.cpp:1:
/home/erik/source/sysperf/src/config.h:8:7: error: using typedef-name ‘using QStringList = class QList<QString>’ after ‘class’
    8 | class QStringList;
      |       ^~~~~~~~~~~
In file included from /usr/include/x86_64-linux-gnu/qt6/QtCore/qtypeinfo.h:6,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qglobal.h:1397,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qnamespace.h:8,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qobjectdefs.h:12,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qobject.h:10,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/QObject:1,
                 from /home/erik/source/sysperf/src/collector/worker.h:6:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qcontainerfwd.h:28:7: note: ‘using QStringList = class QList<QString>’ has a previous declaration here
   28 | using QStringList = QList<QString>;
      |       ^~~~~~~~~~~
In file included from /home/erik/source/sysperf/src/collector/worker.h:8,
                 from /home/erik/source/sysperf/src/main.cpp:15:
/home/erik/source/sysperf/src/config.h:8:7: error: using typedef-name ‘using QStringList = class QList<QString>’ after ‘class’
    8 | class QStringList;
      |       ^~~~~~~~~~~
In file included from /usr/include/x86_64-linux-gnu/qt6/QtCore/qtypeinfo.h:6,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qglobal.h:1397,
                 from /usr/include/x86_64-linux-gnu/qt6/QtGui/qtguiglobal.h:7,
                 from /usr/include/x86_64-linux-gnu/qt6/QtWidgets/qtwidgetsglobal.h:7,
                 from /usr/include/x86_64-linux-gnu/qt6/QtWidgets/qapplication.h:7,
                 from /usr/include/x86_64-linux-gnu/qt6/QtWidgets/QApplication:1,
                 from /home/erik/source/sysperf/src/main.cpp:9:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qcontainerfwd.h:28:7: note: ‘using QStringList = class QList<QString>’ has a previous declaration here
   28 | using QStringList = QList<QString>;
      |       ^~~~~~~~~~~
gmake[2]: *** [CMakeFiles/sysperf.dir/build.make:124: CMakeFiles/sysperf.dir/src/collector/worker.cpp.o] Error 1
gmake[2]: *** Waiting for unfinished jobs....
In file included from /home/erik/source/sysperf/src/ui/mainwindow.h:10,
                 from /home/erik/source/sysperf/src/ui/mainwindow.cpp:1:
/home/erik/source/sysperf/src/config.h:8:7: error: using typedef-name ‘using QStringList = class QList<QString>’ after ‘class’
    8 | class QStringList;
      |       ^~~~~~~~~~~
In file included from /usr/include/x86_64-linux-gnu/qt6/QtCore/qtypeinfo.h:6,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qglobal.h:1397,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qcontainertools_impl.h:14,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/qhash.h:8,
                 from /usr/include/x86_64-linux-gnu/qt6/QtCore/QHash:1,
                 from /home/erik/source/sysperf/src/ui/mainwindow.h:6:
/usr/include/x86_64-linux-gnu/qt6/QtCore/qcontainerfwd.h:28:7: note: ‘using QStringList = class QList<QString>’ has a previous declaration here
   28 | using QStringList = QList<QString>;
      |       ^~~~~~~~~~~
/home/erik/source/sysperf/src/config.cpp: In function ‘QString sysperf::{anonymous}::resolveConfigPath(const QStringList&)’:
/home/erik/source/sysperf/src/config.cpp:26:25: error: invalid use of incomplete type ‘const class QStringList’
   26 |     for (int i = 0; i < args.size() - 1; ++i)
      |                         ^~~~
/home/erik/source/sysperf/src/config.h:8:7: note: forward declaration of ‘class QStringList’
    8 | class QStringList;
      |       ^~~~~~~~~~~
/home/erik/source/sysperf/src/config.cpp:27:17: error: no match for ‘operator[]’ (operand types are ‘const QStringList’ and ‘int’)
   27 |         if (args[i] == "--config") return args[i + 1];
      |                 ^
/home/erik/source/sysperf/src/config.cpp:27:47: error: no match for ‘operator[]’ (operand types are ‘const QStringList’ and ‘int’)
   27 |         if (args[i] == "--config") return args[i + 1];
      |                                               ^
/home/erik/source/sysperf/src/config.cpp:29:17: error: aggregate ‘QStringList candidates’ has incomplete type and cannot be defined
   29 |     QStringList candidates;
      |                 ^~~~~~~~~~
/home/erik/source/sysperf/src/config.cpp: In function ‘void sysperf::{anonymous}::applyCli(sysperf::AppConfig&, const QStringList&)’:
/home/erik/source/sysperf/src/config.cpp:80:25: error: invalid use of incomplete type ‘const class QStringList’
   80 |     for (int i = 0; i < args.size(); ++i) {
      |                         ^~~~
/home/erik/source/sysperf/src/config.h:8:7: note: forward declaration of ‘class QStringList’
    8 | class QStringList;
      |       ^~~~~~~~~~~
/home/erik/source/sysperf/src/config.cpp:81:32: error: no match for ‘operator[]’ (operand types are ‘const QStringList’ and ‘int’)
   81 |         const QString& a = args[i];
      |                                ^
/home/erik/source/sysperf/src/config.cpp: In lambda function:
/home/erik/source/sysperf/src/config.cpp:83:25: error: invalid use of incomplete type ‘const class QStringList’
   83 |             if (i + 1 < args.size()) dst = toStd(args[++i]);
      |                         ^~~~
/home/erik/source/sysperf/src/config.h:8:7: note: forward declaration of ‘class QStringList’
    8 | class QStringList;
      |       ^~~~~~~~~~~
/home/erik/source/sysperf/src/config.cpp:83:54: error: no match for ‘operator[]’ (operand types are ‘const QStringList’ and ‘int’)
   83 |             if (i + 1 < args.size()) dst = toStd(args[++i]);
      |                                                      ^
/home/erik/source/sysperf/src/config.cpp: In function ‘void sysperf::{anonymous}::applyCli(sysperf::AppConfig&, const QStringList&)’:
/home/erik/source/sysperf/src/config.cpp:87:50: error: invalid use of incomplete type ‘const class QStringList’
   87 |         else if (a == "--interval-ms" && i + 1 < args.size()) {
      |                                                  ^~~~
/home/erik/source/sysperf/src/config.h:8:7: note: forward declaration of ‘class QStringList’
    8 | class QStringList;
      |       ^~~~~~~~~~~
/home/erik/source/sysperf/src/config.cpp:89:25: error: no match for ‘operator[]’ (operand types are ‘const QStringList’ and ‘int’)
   89 |             int n = args[++i].toInt(&ok);
      |                         ^
gmake[2]: *** [CMakeFiles/sysperf.dir/build.make:82: CMakeFiles/sysperf.dir/sysperf_autogen/mocs_compilation.cpp.o] Error 1
gmake[2]: *** [CMakeFiles/sysperf.dir/build.make:96: CMakeFiles/sysperf.dir/src/main.cpp.o] Error 1
/home/erik/source/sysperf/src/ui/mainwindow.cpp: In constructor ‘sysperf::MainWindow::MainWindow(sysperf::AppConfig, QWidget*)’:
/home/erik/source/sysperf/src/ui/mainwindow.cpp:55:60: error: invalid use of incomplete type ‘class sysperf::QLabel’
   55 |     selfLabel_ = new QLabel(QStringLiteral("self: —"), this);
      |                                                            ^
In file included from /home/erik/source/sysperf/src/ui/mainwindow.cpp:19:
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
gmake[2]: *** [CMakeFiles/sysperf.dir/build.make:110: CMakeFiles/sysperf.dir/src/config.cpp.o] Error 1
/home/erik/source/sysperf/src/ui/panel.cpp: In constructor ‘sysperf::Panel::Panel(const QString&, QWidget*)’:
/home/erik/source/sysperf/src/ui/panel.cpp:18:39: error: invalid use of incomplete type ‘class sysperf::QLabel’
   18 |     titleLbl_ = new QLabel(title, this);
      |                                       ^
In file included from /home/erik/source/sysperf/src/ui/panel.cpp:1:
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:20:28: error: invalid use of incomplete type ‘class sysperf::QLabel’
   20 |         QFont f = titleLbl_->font();
      |                            ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:23:18: error: invalid use of incomplete type ‘class sysperf::QLabel’
   23 |         titleLbl_->setFont(f);
      |                  ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:25:21: error: cannot convert ‘sysperf::QLabel*’ to ‘QWidget*’
   25 |     root->addWidget(titleLbl_);
      |                     ^~~~~~~~~
      |                     |
      |                     sysperf::QLabel*
In file included from /usr/include/x86_64-linux-gnu/qt6/QtWidgets/qlayout.h:132,
                 from /usr/include/x86_64-linux-gnu/qt6/QtWidgets/qgridlayout.h:8,
                 from /usr/include/x86_64-linux-gnu/qt6/QtWidgets/QGridLayout:1,
                 from /home/erik/source/sysperf/src/ui/panel.cpp:4:
/usr/include/x86_64-linux-gnu/qt6/QtWidgets/qboxlayout.h:38:20: note:   initializing argument 1 of ‘void QBoxLayout::addWidget(QWidget*, int, Qt::Alignment)’
   38 |     void addWidget(QWidget *, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
      |                    ^~~~~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:27:46: error: invalid use of incomplete type ‘class sysperf::QLabel’
   27 |     headlineLbl_ = new QLabel(QString(), this);
      |                                              ^
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:29:31: error: invalid use of incomplete type ‘class sysperf::QLabel’
   29 |         QFont f = headlineLbl_->font();
      |                               ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:31:21: error: invalid use of incomplete type ‘class sysperf::QLabel’
   31 |         headlineLbl_->setFont(f);
      |                     ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:33:17: error: invalid use of incomplete type ‘class sysperf::QLabel’
   33 |     headlineLbl_->setVisible(false);
      |                 ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:34:21: error: cannot convert ‘sysperf::QLabel*’ to ‘QWidget*’
   34 |     root->addWidget(headlineLbl_);
      |                     ^~~~~~~~~~~~
      |                     |
      |                     sysperf::QLabel*
/usr/include/x86_64-linux-gnu/qt6/QtWidgets/qboxlayout.h:38:20: note:   initializing argument 1 of ‘void QBoxLayout::addWidget(QWidget*, int, Qt::Alignment)’
   38 |     void addWidget(QWidget *, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
      |                    ^~~~~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:36:46: error: invalid use of incomplete type ‘class sysperf::QLabel’
   36 |     subtitleLbl_ = new QLabel(QString(), this);
      |                                              ^
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:37:17: error: invalid use of incomplete type ‘class sysperf::QLabel’
   37 |     subtitleLbl_->setEnabled(false);  // grey it out
      |                 ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:38:17: error: invalid use of incomplete type ‘class sysperf::QLabel’
   38 |     subtitleLbl_->setVisible(false);
      |                 ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:39:21: error: cannot convert ‘sysperf::QLabel*’ to ‘QWidget*’
   39 |     root->addWidget(subtitleLbl_);
      |                     ^~~~~~~~~~~~
      |                     |
      |                     sysperf::QLabel*
/usr/include/x86_64-linux-gnu/qt6/QtWidgets/qboxlayout.h:38:20: note:   initializing argument 1 of ‘void QBoxLayout::addWidget(QWidget*, int, Qt::Alignment)’
   38 |     void addWidget(QWidget *, int stretch = 0, Qt::Alignment alignment = Qt::Alignment());
      |                    ^~~~~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:44:34: error: invalid use of incomplete type ‘class sysperf::QGridLayout’
   44 |     statsGrid_ = new QGridLayout();
      |                                  ^
/home/erik/source/sysperf/src/ui/panel.h:14:7: note: forward declaration of ‘class sysperf::QGridLayout’
   14 | class QGridLayout;
      |       ^~~~~~~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:45:15: error: invalid use of incomplete type ‘class sysperf::QGridLayout’
   45 |     statsGrid_->setHorizontalSpacing(24);
      |               ^~
/home/erik/source/sysperf/src/ui/panel.h:14:7: note: forward declaration of ‘class sysperf::QGridLayout’
   14 | class QGridLayout;
      |       ^~~~~~~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:46:15: error: invalid use of incomplete type ‘class sysperf::QGridLayout’
   46 |     statsGrid_->setVerticalSpacing(6);
      |               ^~
/home/erik/source/sysperf/src/ui/panel.h:14:7: note: forward declaration of ‘class sysperf::QGridLayout’
   14 | class QGridLayout;
      |       ^~~~~~~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:47:21: error: cannot convert ‘sysperf::QGridLayout*’ to ‘QLayout*’
   47 |     root->addLayout(statsGrid_);
      |                     ^~~~~~~~~~
      |                     |
      |                     sysperf::QGridLayout*
/usr/include/x86_64-linux-gnu/qt6/QtWidgets/qboxlayout.h:39:29: note:   initializing argument 1 of ‘void QBoxLayout::addLayout(QLayout*, int)’
   39 |     void addLayout(QLayout *layout, int stretch = 0);
      |                    ~~~~~~~~~^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp: In member function ‘void sysperf::Panel::setHeadline(const QString&)’:
/home/erik/source/sysperf/src/ui/panel.cpp:64:17: error: invalid use of incomplete type ‘class sysperf::QLabel’
   64 |     headlineLbl_->setText(text);
      |                 ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:65:17: error: invalid use of incomplete type ‘class sysperf::QLabel’
   65 |     headlineLbl_->setVisible(!text.isEmpty());
      |                 ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp: In member function ‘void sysperf::Panel::setSubtitle(const QString&)’:
/home/erik/source/sysperf/src/ui/panel.cpp:69:17: error: invalid use of incomplete type ‘class sysperf::QLabel’
   69 |     subtitleLbl_->setText(text);
      |                 ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:70:17: error: invalid use of incomplete type ‘class sysperf::QLabel’
   70 |     subtitleLbl_->setVisible(!text.isEmpty());
      |                 ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp: In member function ‘void sysperf::Panel::addStat(const QString&, const QString&)’:
/home/erik/source/sysperf/src/ui/panel.cpp:79:49: error: invalid use of incomplete type ‘class sysperf::QLabel’
   79 |     auto* nameLbl = new QLabel(label + ":", this);
      |                                                 ^
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:81:56: error: invalid use of incomplete type ‘class sysperf::QLabel’
   81 |     auto* valLbl = new QLabel(QStringLiteral("—"), this);
      |                                                        ^
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:86:15: error: invalid use of incomplete type ‘class sysperf::QGridLayout’
   86 |     statsGrid_->addWidget(nameLbl, row, col);
      |               ^~
/home/erik/source/sysperf/src/ui/panel.h:14:7: note: forward declaration of ‘class sysperf::QGridLayout’
   14 | class QGridLayout;
      |       ^~~~~~~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp:87:15: error: invalid use of incomplete type ‘class sysperf::QGridLayout’
   87 |     statsGrid_->addWidget(valLbl, row, col + 1);
      |               ^~
/home/erik/source/sysperf/src/ui/panel.h:14:7: note: forward declaration of ‘class sysperf::QGridLayout’
   14 | class QGridLayout;
      |       ^~~~~~~~~~~
/home/erik/source/sysperf/src/ui/panel.cpp: In member function ‘void sysperf::Panel::setStat(const QString&, const QString&)’:
/home/erik/source/sysperf/src/ui/panel.cpp:94:44: error: invalid use of incomplete type ‘class sysperf::QLabel’
   94 |     if (it != statValues_.end()) it.value()->setText(value);
      |                                            ^~
/home/erik/source/sysperf/src/ui/panel.h:15:7: note: forward declaration of ‘class sysperf::QLabel’
   15 | class QLabel;
      |       ^~~~~~
gmake[2]: *** [CMakeFiles/sysperf.dir/build.make:180: CMakeFiles/sysperf.dir/src/ui/panel.cpp.o] Error 1
gmake[2]: *** [CMakeFiles/sysperf.dir/build.make:138: CMakeFiles/sysperf.dir/src/ui/mainwindow.cpp.o] Error 1
gmake[1]: *** [CMakeFiles/Makefile2:158: CMakeFiles/sysperf.dir/all] Error 2
gmake: *** [Makefile:146: all] Error 2
erik@laptop-erik-linux:~/source/sysperf$ 


