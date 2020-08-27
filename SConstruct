import enum;

@enum.unique
class target(enum.Enum):
  linux = 0
  win32 = 1
  win64 = 2
  darwin = 3

AddOption(
  '--target',
  dest = 'target-opt',
  type = 'string',
  nargs = 1,
  action = 'store',
  metavar = 'STRING',
  help = 'build target',
  default = target.linux.name
);

AddOption(
  '--test',
  dest = 'test-opt',
  action = 'store_true',
  help = 'compile test program',
  default = False
);

AddOption(
  '--use-clang',
  dest = 'use-clang-opt',
  action = 'store_true',
  help = 'use clang++ instead of g++',
  default = False
);

AddOption(
  '--no-debug',
  dest = 'no-debug-opt',
  action = 'store_true',
  help = 'define DANGO_NO_DEBUG',
  default = False
);

AddOption(
  '--no-multicore',
  dest = 'no-multicore-opt',
  action = 'store_true',
  help = 'define DANGO_NO_MULTICORE',
  default = False
);

AddOption(
  '--big-cache-lines',
  dest = 'big-cache-lines-opt',
  action = 'store_true',
  help = 'define DANGO_CACHE_LINE_SIZE as 128 instead of 64',
  default = False
);

AddOption(
  '--new-noexcept',
  dest = 'new-noexcept-opt',
  action = 'store_true',
  help = 'define DANGO_NEW_NOEXCEPT',
  default = False
);

compilation_target = None;

try:
  compilation_target = target[GetOption('target-opt')];
except KeyError as err:
  print('invalid compilation target \"' + err.args[0] + '\"');
  Exit(1);

compile_test = GetOption('test-opt');
use_clang = GetOption('use-clang-opt');
no_debug = GetOption('no-debug-opt');
no_multicore = GetOption('no-multicore-opt');
big_cache_lines = GetOption('big-cache-lines-opt');
new_noexcept = GetOption('new-noexcept-opt');

print('building for target \"' + compilation_target.name + '\"');

flags = [
# '-S',
# '-Ofast',
# '-v',
# '-fsanitize=thread',
# '-fsanitize=address',
# '-fsanitize=leak',
  '-flto',
  '-O2',
  '-std=c++20',
  '-fsized-deallocation',
  '-pedantic',
  '-pedantic-errors',
  '-Wall',
  '-Wextra',
  '-Wshadow',
  '-Wfatal-errors',
  '-Wno-gnu-zero-variadic-macro-arguments' if use_clang else '',
  ''
];

import os;

lib_env = None;
test_env = None;

if(compilation_target == target.linux):
  if(use_clang):
    lib_env = Environment(platform = 'posix', tools = ['default', 'clangxx']);
  else:
    lib_env = Environment(platform = 'posix', tools = ['default', 'g++']);
elif(compilation_target == target.win32 or compilation_target == target.win64):
  if(use_clang):
    lib_env = Environment(platform = 'win32', tools = ['mingw', 'clangxx']);
  else:
    lib_env = Environment(platform = 'win32', tools = ['mingw', 'g++']);
elif(compilation_target == target.darwin):
  if(use_clang):
    lib_env = Environment(platform = 'darwin', tools = ['default', 'clangxx']);
  else:
    lib_env = Environment(platform = 'darwin', tools = ['default', 'g++']);

lib_env.Append(ENV = { 'PATH':os.environ['PATH'] });

clang_target_flag = None;

if(use_clang):
  lib_env.Append(LINKFLAGS = '-fuse-ld=lld');
  if(compilation_target == target.win64):
    clang_target_flag = '-target x86_64-pc-windows-gnu';
  elif(compilation_target == target.win32):
    clang_target_flag = '-target i686-pc-windows-gnu';

if(not clang_target_flag is None):
  lib_env.Append(CXXFLAGS = clang_target_flag);
  lib_env.Append(LINKFLAGS = clang_target_flag);

lib_env.Append(CPPPATH = ['include/']);
lib_env.Append(LIBPATH = ['./']);
lib_env.Append(RPATH = ['./']);
lib_env.Append(CXXFLAGS = flags);
lib_env.Append(LINKFLAGS = flags);

if(no_debug):
  lib_env.Append(CPPDEFINES = 'DANGO_NO_DEBUG');
if(no_multicore):
  lib_env.Append(CPPDEFINES = 'DANGO_NO_MULTICORE');
if(big_cache_lines):
  lib_env.Append(CPPDEFINES = ('DANGO_CACHE_LINE_SIZE', '128'));
if(new_noexcept):
  lib_env.Append(CPPDEFINES = 'DANGO_NEW_NOEXCEPT');

if(compile_test):
  test_env = lib_env.Clone();
  test_env.Append(CPPDEFINES = 'DANGO_ENABLE_UNIT_TESTS');

lib_env.Append(CPPDEFINES = ['DANGO_BUILDING_DANGO']);
lib_env.Append(CPPPATH = ['src/private/']);

lib_flags = [];

if(compilation_target == target.linux or compilation_target == target.darwin):
  lib_env.Append(LIBS = ['pthread']);
  lib_flags += ['-fvisibility=hidden'];
elif(compilation_target == target.win32 or compilation_target == target.win64):
  lib_env.Append(LIBS = ['winmm']);
  lib_env.Append(CPPDEFINES = [('WINVER', '0x0601'), ('_WIN32_WINNT', '0x0601')]);

lib_env.Append(CXXFLAGS = lib_flags);
lib_env.Append(LINKFLAGS = lib_flags);

lib_env.SharedLibrary('dango', [Glob('src/*.cpp')]);

if(compile_test):
  test_env.Append(CPPPATH = ['test/include/']);
  test_env.Append(LIBS = ['dango']);
  test_env.Program('exe_test', [Glob('test/src/*.cpp')]);

