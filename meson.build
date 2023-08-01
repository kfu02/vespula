# define the project 
# (note C++17)
project('vespula', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

# add thread dependency
thread_dep = dependency('threads')
add_global_arguments('-std=c++17', language : 'cpp')

# create an executable called 'vespula'
exe = executable('vespula', 'vespula.cpp',
  dependencies : thread_dep,
  install : true)

# TODO: add real tests
test('basic', exe)
