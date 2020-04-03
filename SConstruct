import enum;

@enum.unique
class target(enum.Enum):
  linux = 0
  win32 = 1
  win64 = 2

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
# '-fsanitize=address',
# '-fsanitize=leak',
  '-flto',
  '-O2',
  '-std=c++20' if use_clang else '-std=c++2a',
  '' if use_clang else '-fconcepts',
  '-fsized-deallocation',
  '-pedantic',
  '-pedantic-errors',
  '-Wall',
  '-Wextra',
  '-Wshadow',
  '-Wfatal-errors'
];

import os;

static_env = None;
shared_env = None;
test_env = None;

if(compilation_target == target.linux):
  if(use_clang):
    static_env = Environment(platform = 'posix', tools = ['default', 'clangxx']);
  else:
    static_env = Environment(platform = 'posix', tools = ['default', 'g++']);
elif(compilation_target == target.win32 or compilation_target == target.win64):
  if(use_clang):
    static_env = Environment(platform = 'win32', tools = ['mingw', 'clangxx']);
  else:
    static_env = Environment(platform = 'win32', tools = ['mingw', 'g++']);

static_env.Append(ENV = { 'PATH':os.environ['PATH'] });

if(use_clang):
  static_env.Append(LINKFLAGS = '-fuse-ld=lld');
  if(compilation_target == target.win64):
    clang_target_flag = '-target x86_64-pc-windows-gnu';
    static_env.Append(CXXFLAGS = clang_target_flag);
    static_env.Append(LINKFLAGS = clang_target_flag);
  elif(compilation_target == target.win32):
    clang_target_flag = '-target i686-pc-windows-gnu';
    static_env.Append(CXXFLAGS = clang_target_flag);
    static_env.Append(LINKFLAGS = clang_target_flag);


static_env.Append(CPPPATH = ['include/']);
static_env.Append(CXXFLAGS = flags);
static_env.Append(LINKFLAGS = flags);

if(compile_test):
  static_env.Append(CPPDEFINES = 'DANGO_TESTING_DANGO');

if(use_clang):
  static_env.Replace(AR = 'llvm-ar');
else:
  static_env.Replace(AR = 'gcc-ar');

static_env.Replace(ARFLAGS = 'rcs');
static_env.Replace(RANLIBCOM = '');

if(no_debug):
  static_env.Append(CPPDEFINES = 'DANGO_NO_DEBUG');
if(no_multicore):
  static_env.Append(CPPDEFINES = 'DANGO_NO_MULTICORE');
if(big_cache_lines):
  static_env.Append(CPPDEFINES = ('DANGO_CACHE_LINE_SIZE', '128'));
if(new_noexcept):
  static_env.Append(CPPDEFINES = 'DANGO_NEW_NOEXCEPT');

shared_env = static_env.Clone();

if(compile_test):
  test_env = static_env.Clone();

static_env.Append(CPPPATH = ['shared/include/']);
shared_env.Append(CPPPATH = ['shared/include/']);

static_env.Append(CPPDEFINES = ['DANGO_COMPILING_DANGO']);
shared_env.Append(CPPDEFINES = ['DANGO_COMPILING_DANGO', 'DANGO_COMPILING_DANGO_SHARED']);

static_flags = [];
shared_flags = [];

if(compilation_target == target.linux):
  shared_env.Append(LIBS = ['pthread']);
  static_flags += ['-fPIC'];
  shared_flags += ['-fvisibility=hidden']; # SCons automatically adds -fPIC when building shared library
elif(compilation_target == target.win32 or compilation_target == target.win64):
  shared_env.Append(LIBS = ['winmm']);
  shared_env.Append(CPPDEFINES = [('WINVER', '0x0601'), ('_WIN32_WINNT', '0x0601')]);

shared_env.Append(LIBS = ['dango']);
shared_env.Append(LIBPATH = ['./']);

static_env.Append(CXXFLAGS = static_flags);
static_env.Append(LINKFLAGS = static_flags);

shared_env.Append(CXXFLAGS = shared_flags);
shared_env.Append(LINKFLAGS = shared_flags);

static_lib = static_env.StaticLibrary('dango', [Glob('src/*.cpp')]);
shared_lib = shared_env.SharedLibrary('dangoshared', [Glob('shared/src/*.cpp')]);

if(compile_test):
  test_env.Append(CPPPATH = ['test/include/']);
  test_env.Append(LIBS = ['dangoshared', 'dango']);
  test_env.Append(LIBPATH = ['./']);
  test_env.Append(RPATH = ['./']);
  test_env.Program('exe_test', [Glob('test/src/*.cpp')]);

