 ###################################################################
#                                                                   #
#  Alexandra Video Library                                          #
#  Copyright (C) 2014-2015 Eugene Melnik <jeka7js@gmail.com>        #
#                                                                   #
 ###################################################################

TEMPLATE = aux

    # Generating Alexandra translations #

system( $$[QT_INSTALL_BINS]/lrelease alexandra-cs.ts -qm alexandra-cs.qm )
system( $$[QT_INSTALL_BINS]/lrelease alexandra-fr.ts -qm alexandra-fr.qm )
system( $$[QT_INSTALL_BINS]/lrelease alexandra-ru_RU.ts -qm alexandra-ru_RU.qm )
system( $$[QT_INSTALL_BINS]/lrelease alexandra-uk_UA.ts -qm alexandra-uk_UA.qm )

    # Qt translations #

system( cp $$[QT_INSTALL_TRANSLATIONS]/qtbase_cs.qm ../lang/qt-cs.qm )
system( cp $$[QT_INSTALL_TRANSLATIONS]/qt_fr.qm     ../lang/qt-fr.qm )
system( cp $$[QT_INSTALL_TRANSLATIONS]/qtbase_ru.qm ../lang/qt-ru_RU.qm )
system( cp $$[QT_INSTALL_TRANSLATIONS]/qtbase_uk.qm ../lang/qt-uk_UA.qm )

