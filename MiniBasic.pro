TEMPLATE = subdirs

SUBDIRS += core \
           runtime \
           ui \
           interpreter\
           test

test.depends = core runtime
ui.depends = core runtime
