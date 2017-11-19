TEMPLATE = subdirs

SUBDIRS = lang src
src.depends = lang

OTHER_FILES += CHANGELOG \
               LICENSE \
               README.md
