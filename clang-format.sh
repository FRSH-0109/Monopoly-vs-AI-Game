find Monopoly/ -maxdepth 2 -regex '.*\.\(cc\|cpp\|hpp\|h\)' -exec clang-format -style=file:./config_files/.clang-format -i {} \;