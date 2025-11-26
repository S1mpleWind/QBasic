TEMPLATE = subdirs

SUBDIRS += core \
           runtime \
           ui \
           test

test.depends = core runtime
ui.depends = core runtime
