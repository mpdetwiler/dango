
AddOption(
  '--target',
  dest = 'target_opt',
  type = 'string',
  nargs = 1,
  action = 'store',
  metavar = 'DIR',
  help = 'build target',
  default = 'linux'
);

AddOption(
  '--test',
  dest = 'test_opt',
  action = 'store_true',
  metavar = 'DIR',
  help = 'compile test program',
  default = False
);

compilation_target = GetOption('target_opt');
compile_test = GetOption('test_opt');

if(compilation_target == 'linux'):
  print('building for \"' + compilation_target + '\"');
elif(compilation_target == 'win32'):
  print('building for \"' + compilation_target + '\"');
elif(compilation_target == 'win64'):
  print('building for \"' + compilation_target + '\"');
else:
  print('invalid target \"' + compilation_target + '\"');
  Exit(1);

flags = [
#	'-S',
#	'-Ofast',
# '-v',
#  '-fsanitize=address',
	'-flto',
	'-O2',
	'-std=c++17',
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

if(compilation_target == 'linux'):
  static_env = DefaultEnvironment();
elif(compilation_target == 'win32' or compilation_target == 'win64'):
  static_env = DefaultEnvironment();

static_env.Append(ENV = {'PATH':os.environ['PATH']});
static_env.Append(CPPPATH = ['include/']);
static_env.Append(CXXFLAGS = flags);
static_env.Append(LINKFLAGS = flags);

if(compile_test):
  static_env.Append(CPPDEFINES = 'DANGO_TESTING_DANGO');

#static_env.Append(CPPDEFINES = ('DANGO_CACHE_LINE_SIZE', '64'));
#static_env.Append(CPPDEFINES = 'DANGO_NO_DEBUG');
#static_env.Append(CPPDEFINES = 'DANGO_NO_MULTICORE');
#static_env.Append(CPPDEFINES = 'DANGO_NEW_NOEXCEPT');

shared_env = static_env.Clone();

if(compile_test):
  test_env = static_env.Clone();

static_env.Append(CPPPATH = ['shared/include/']);
shared_env.Append(CPPPATH = ['shared/include/']);

static_env.Replace(AR = 'gcc-ar');
static_env.Replace(ARFLAGS = 'rcs');
static_env.Replace(RANLIBCOM = '');

static_env.Append(CPPDEFINES = 'DANGO_COMPILING_DANGO');
shared_env.Append(CPPDEFINES = 'DANGO_COMPILING_DANGO');

shared_flags = ['-fPIC', '-fvisibility=hidden'];

if(compilation_target == 'linux'):
  shared_env.Append(LIBS = ['pthread']);
elif(compilation_target == 'win32' or compilation_target == 'win64'):
  shared_env.Append(LIBS = ['winmm']);
  static_env.Append(CPPDEFINES = [('WINVER', '0x0601'), ('_WIN32_WINNT', '0x0601')]);
  shared_env.Append(CPPDEFINES = [('WINVER', '0x0601'), ('_WIN32_WINNT', '0x0601')]);

shared_env.Append(CXXFLAGS = shared_flags);
shared_env.Append(LINKFLAGS = shared_flags);

static_lib = static_env.StaticLibrary('dango', Glob('src/*.cpp'));
shared_lib = shared_env.SharedLibrary('dangoshared', [Glob('shared/src/*.cpp'), static_lib]);

if(compile_test):
  test_env.Append(CPPPATH = ['test/include/']);
  test_env.Append(RPATH = ['./']);
  test_env.Program('run', [Glob('test/src/*.cpp'), shared_lib, static_lib]);

