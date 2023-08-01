# define the project 
# (note C++17)
project('vespula', 'cpp',
  version : '0.1',
  license : 'GPLv3',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

# add thread dependency
thread_dep = dependency('threads')
add_global_arguments('-std=c++17', language : 'cpp')

# add concurrent queue
concurrent_queue_dep = dependency('concurrentqueue')

# create an executable called 'vespula'
exe = executable('vespula', 'vespula.cpp',
  dependencies : [thread_dep, concurrent_queue_dep],
  install : true)

# TODO: add real tests
test('basic', exe)
