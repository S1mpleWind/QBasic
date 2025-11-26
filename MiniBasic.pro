# Break the whole .pro into sub ones.

TEMPLATE = subdirs

SUBDIRS += core\
           runtime\
           ui\
           test


# Dependency
runtime.depends = core
ui.depends = core runtime
test.depends = core runtime
