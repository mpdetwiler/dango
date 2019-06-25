
AddOption(
  '--target',
  dest = 'target',
  type = 'string',
  nargs = 1,
  action = 'store',
  metavar = 'DIR',
  help = 'build target',
  default = 'linux'
);

target = GetOption('target');

if(target == 'linux'):
  print('building for \"' + target + '\"');
elif(target == 'win32'):
  print('building for \"' + target + '\"');
elif(target == 'win64'):
  print('building for \"' + target + '\"');
else:
  print('invalid target \"' + target + '\"');
  Exit(1);

import os;

env = Environment();

env.Append(ENV = {'PATH':os.environ['PATH']});

lib_name = 'libdango';
header_paths = ['include/'];
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

#env.Append(CPPDEFINES = ('DANGO_CACHE_LINE_SIZE', '64'));
#env.Append(CPPDEFINES = 'DANGO_NO_DEBUG');
#env.Append(CPPDEFINES = 'DANGO_NO_MULTICORE');
#env.Append(CPPDEFINES = 'DANGO_NEW_NOEXCEPT');

env.Replace(AR = 'gcc-ar');
env.Replace(ARFLAGS = 'rcs');
env.Replace(RANLIBCOM = '');

if(target == 'linux'):
  env.Append(LIBS = ['pthread']);
elif(target == 'win32' or target == 'win64'):
  env.Replace(CXX = 'g++');
  env.Append(LIBS = ['winmm']);
  flags += ['-static-libgcc', '-static-libstdc++'];
  env.Append(CPPDEFINES = [('WINVER', '0x0601'), ('_WIN32_WINNT', '0x0601')]);

env.Append(CPPPATH = header_paths);
env.Append(CXXFLAGS = flags);
env.Append(LINKFLAGS = flags);

sources = [Glob('src/*.cpp')];

env.StaticLibrary(lib_name, sources);

