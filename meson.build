project(
  'dima',
  'c',
  version: '1.0.1',
  license: 'Apache-2.0',
  default_options: [
    'c_std=c11',
    'warning_level=3',
    'werror=true'
  ])

cc_flags = [
  '-Wformat-security',
  '-Wold-style-declaration',
  '-Wold-style-definition',
  '-Wmissing-declarations',
  '-Wstrict-prototypes',
]
cc = meson.get_compiler('c')
add_global_arguments(cc.get_supported_arguments(cc_flags), language: 'c')

name = 'dima'
slot = meson.project_version().split('.')[0]
slotname = name + '-' + slot

pkg = import('pkgconfig')

attributes = ['cold', 'noreturn', 'unused']
foreach attribute : attributes
  if cc.has_function_attribute(attribute)
    value = '__attribute__((' + attribute + '))'
  else
    value = ''
  endif
  define = attribute.to_upper()
  add_global_arguments('-D' + define + '=' + value, language: 'c')
endforeach

prefix = '''
#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
'''
functions = ['reallocarray', 'strdup', 'strndup']
have_any_function = false
foreach function : functions
  if cc.has_function(function, prefix: prefix)
    have_any_function = true
    value = '1'
  else
    value = '0'
  endif
  arg = '-DHAVE_' + function.to_upper() + '=' + value
  add_global_arguments(arg, language: 'c')
endforeach
if have_any_function
  add_global_arguments('-D_GNU_SOURCE', language: 'c')
endif

install_headers(
  [
    'include/dima/derived.h',
    'include/dima/dima.h',
    'include/dima/eventually_failing.h',
    'include/dima/exiting_on_failure.h',
    'include/dima/failing.h',
    'include/dima/failure_hook.h',
    'include/dima/invocation.h',
    'include/dima/proxy.h',
    'include/dima/pthread_mutex_locked.h',
    'include/dima/randomly_failing.h',
    'include/dima/system.h',
  ],
  subdir: join_paths(slotname, name))

incdirs = include_directories('include')

threads = dependency('threads')
has_pthread_mutex = cc.has_type(
    'pthread_mutex_t',
    prefix: '#include <pthread.h>',
    dependencies: threads)

lib_src = [
  'src/derived.c',
  'src/dima.c',
  'src/eventually_failing.c',
  'src/exiting_on_failure.c',
  'src/failing.c',
  'src/failure_hook.c',
  'src/invocation.c',
  'src/proxy.c',
  'src/randomly_failing.c',
  'src/system.c',
]
if has_pthread_mutex
  lib_src += ['src/pthread_mutex_locked.c']
endif
lib = library(
  slotname,
  lib_src,
  include_directories: incdirs,
  install: true,
  dependencies: threads)

dima = declare_dependency(
  link_with: lib,
  include_directories: incdirs)

url = 'https://github.com/karllinden/dima'
pkg.generate(
  lib,
  name: slotname,
  url: url,
  subdirs: slotname,
  description: 'Dependency injectable memory allocator')

check = dependency(
  'check',
  required: get_option('test'),
  version: '<0.13.0')
if check.found()
  testlib = static_library(
    'testlib',
    [
      'tests/forwarding_tests.c',
      'tests/invocations.c',
      'tests/powz.c',
      'tests/standard_tests.c',
      'tests/test.c'
    ],
    dependencies: [check, dima])

  test_names = [
    'derived',
    'dima',
    'eventually_failing',
    'exiting_on_failure',
    'failure_hook',
    'powz',
    'proxy',
    'randomly_failing',
    'system',
  ]
  if has_pthread_mutex
    test_names += ['pthread_mutex_locked']
  endif
  foreach name : test_names
    test_source = join_paths('tests', name + '_test.c')
    exe = executable(
      name + '_test',
      test_source,
      dependencies: [check, dima],
      link_with: testlib)
    test(name, exe)
  endforeach
endif
