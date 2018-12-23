import sys

platform=sys.platform

b_tools=[]

if(platform.startswith('linux')):

	print 'platform: '+platform
	b_tools+=['g++','gnulink']

elif(platform.startswith('win32') or platform.startswith('msys')):

	print 'platform: '+platform
	b_tools+=['mingw']

elif(platform.startswith('darwin')):

	print 'platform: '+platform+' not yet implemented'
	Exit(1)

else:

	print 'platform: '+platform+' not supported'
	Exit(1)

import os;

env=Environment(tools=b_tools)

env.Append(ENV={'PATH':os.environ['PATH']})

h_paths=[]

if(platform.startswith('linux')):
  print ''
#	env.Append(LIBS=['nspr4'])
#	h_paths+=['/usr/include/nspr4/']

elif(platform.startswith('win32') or platform.startswith('msys')):

#	env.Append(LIBS=['nspr4'])
#	env.Append(CXXFLAGS=['-static-libgcc','-static-libstdc++'])
#	env.Append(LINKFLAGS=['-static-libgcc','-static-libstdc++'])
	env.Append(LINKFLAGS=['-flto-partition=none'])
#	h_paths+=['c:/msys32/mingw32/include/nspr/']

elif(platform.startswith('darwin')):

	Exit(1)

else:

	Exit(1)


h_paths+=['include/']

c_flags=[
#	'-S',
#	'-Ofast',
	'-flto',
	'-O2',
	'-std=c++17',
	'-pedantic',
	'-pedantic-errors',
	'-Wall',
	'-Wextra',
	'-Wshadow',
	'-Wfatal-errors'
]

env.Append(CPPPATH=h_paths)
env.Append(CXXFLAGS=c_flags)
env.Append(LINKFLAGS=c_flags)

sources=[
	Glob('src/*.cpp')
]

env.Program('run',sources)

