TEMPLATE = subdirs

SUBDIRS = lang src tests
src.depends = lang

OTHER_FILES += CHANGELOG \
               LICENSE \
               README.md \
               .gitignore \
               .travis.yml
