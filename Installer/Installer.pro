TEMPLATE = aux

CONFIG(release, debug|release) {
    INSTALLER_OFFLINE = $$OUT_PWD/../../SOSRequsetRelease/Installer/SOSRequestInstaller.exe
    DESTDIR_WIN = $$PWD/packages/net.sos-dn.sosrequest/data
    DESTDIR_WIN ~= s,/,\\,g

    PWD_WIN = $$OUT_PWD/../../SOSRequsetRelease
    PWD_WIN ~= s,/,\\,g

    copydata.commands = $(COPY_DIR) $$PWD_WIN $$DESTDIR_WIN
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata

    INPUT = $$PWD/config/config.xml $$PWD/packages
    offlineInstaller.depends = copydata
    offlineInstaller.input = INPUT
    offlineInstaller.output = $$INSTALLER_OFFLINE
    offlineInstaller.commands = $$(QTDIR)/../../Tools/QtInstallerFramework/3.2/bin/binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages ${QMAKE_FILE_OUT}
    offlineInstaller.CONFIG += target_predeps no_link combine
    QMAKE_EXTRA_COMPILERS += offlineInstaller

}

DISTFILES += \
    packages/net.sos-dn.sosrequest/meta/package.xml \
    packages/net.sos-dn.sosrequest/meta/license.txt \
    config/config.xml
