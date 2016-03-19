 ###################################################################
#                                                                   #
#  Alexandra Video Library                                          #
#  Copyright (C) 2014-2016 Eugene Melnik <jeka7js@gmail.com>        #
#                                                                   #
 ###################################################################

TEMPLATE = aux

    # Generating Alexandra translations #

LRELEASE = $$[QT_INSTALL_BINS]/lrelease
win32:LRELEASE ~= s,/,\\,g

system( $$LRELEASE  alexandra-cs.ts     -qm  alexandra-cs.qm )
system( $$LRELEASE  alexandra-de.ts     -qm  alexandra-de.qm )
system( $$LRELEASE  alexandra-fr.ts     -qm  alexandra-fr.qm )
system( $$LRELEASE  alexandra-ru_RU.ts  -qm  alexandra-ru_RU.qm )
system( $$LRELEASE  alexandra-uk_UA.ts  -qm  alexandra-uk_UA.qm )

    # Qt translations #

COPY = cp
QT_CS_SRC = $$[QT_INSTALL_TRANSLATIONS]/qtbase_cs.qm
QT_CS_DST = $$PWD/qt-cs.qm
QT_DE_SRC = $$[QT_INSTALL_TRANSLATIONS]/qtbase_de.qm
QT_DE_DST = $$PWD/qt-de.qm
QT_FR_SRC = $$[QT_INSTALL_TRANSLATIONS]/qt_fr.qm
QT_FR_DST = $$PWD/qt-fr.qm
QT_RU_SRC = $$[QT_INSTALL_TRANSLATIONS]/qtbase_ru.qm
QT_RU_DST = $$PWD/qt-ru_RU.qm
QT_UK_SRC = $$[QT_INSTALL_TRANSLATIONS]/qtbase_uk.qm
QT_UK_DST = $$PWD/qt-uk_UA.qm

win32 {
    # Terrible code... #
    COPY = copy
    QT_CS_SRC ~= s,/,\\,g
    QT_CS_DST ~= s,/,\\,g
    QT_DE_SRC ~= s,/,\\,g
    QT_DE_DST ~= s,/,\\,g
    QT_FR_SRC ~= s,/,\\,g
    QT_FR_DST ~= s,/,\\,g
    QT_RU_SRC ~= s,/,\\,g
    QT_RU_DST ~= s,/,\\,g
    QT_UK_SRC ~= s,/,\\,g
    QT_UK_DST ~= s,/,\\,g
}

system( $$COPY $$QT_CS_SRC $$QT_CS_DST )
system( $$COPY $$QT_DE_SRC $$QT_DE_DST )
system( $$COPY $$QT_FR_SRC $$QT_FR_DST )
system( $$COPY $$QT_RU_SRC $$QT_RU_DST )
system( $$COPY $$QT_UK_SRC $$QT_UK_DST )

