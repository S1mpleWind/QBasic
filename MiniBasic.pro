TEMPLATE = subdirs

SUBDIRS += core \
           runtime \
           ui \
           interpreter\
           test

test.depends = core runtime
ui.depends = core runtime

# 如果编译失败，检查在子文件的pro文件里，使用的lib的路径
# 修改代码的时候要重新编译来更新lib
