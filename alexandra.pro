 ###################################################################
#                                                                   #
#  Alexandra Video Library                                          #
#  Copyright (C) 2014-2017 Eugene Melnik <eugene@melnik.solutions>  #
#                                                                   #
 ###################################################################

TEMPLATE = subdirs

SUBDIRS = lang src
src.depends = lang

OTHER_FILES += CHANGELOG \
               LICENSE \
               README.md
