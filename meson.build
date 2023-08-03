# define the project 
# (note C++17)
project('vespula', 'cpp',
  version : '0.1',
  license : 'GPLv3',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

# add thread dependency
thread_dep = dependency('threads')
add_global_arguments('-std=c++17', language : 'cpp')

# make include/ available
communication_inc = include_directories('include')

# add communication as a library
communication_sources = ['src/topic.cpp', 'src/node.cpp']  # TODO: main.cpp shouldn't be here, the comm should be a standalone lib
communication_lib = static_library('communication',
                            communication_sources,
                            include_directories : communication_inc,
                            dependencies : thread_dep,
                            install : true)

communication_dep = declare_dependency(link_with : communication_lib, include_directories : communication_inc)

# create an executable called 'vespula' with entry point src/main.cpp
exe = executable('vespula', 'src/main.cpp',
  dependencies : communication_dep,
  install : true)

# handle building + running all tests
subdir('test')
