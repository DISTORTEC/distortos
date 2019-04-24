#!/usr/bin/env python

#
# file: runTest.py
#
# author: Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import argparse
import itertools
import os
import re
import shutil
import subprocess
import sys
import textwrap

optimizations = (
		(('CMAKE_C_FLAGS_RELWITHDEBINFO', '-O1 -g -ggdb3'), ('CMAKE_CXX_FLAGS_RELWITHDEBINFO', '-O1 -g -ggdb3'),),
		(('CMAKE_C_FLAGS_RELWITHDEBINFO', '-O2 -g -ggdb3'), ('CMAKE_CXX_FLAGS_RELWITHDEBINFO', '-O2 -g -ggdb3'),),
		(('CMAKE_C_FLAGS_RELWITHDEBINFO', '-O3 -g -ggdb3'), ('CMAKE_CXX_FLAGS_RELWITHDEBINFO', '-O3 -g -ggdb3'),),
		(('CMAKE_C_FLAGS_RELWITHDEBINFO', '-Os -g -ggdb3'), ('CMAKE_CXX_FLAGS_RELWITHDEBINFO', '-Os -g -ggdb3'),),
		(('CMAKE_C_FLAGS_RELWITHDEBINFO', '-Og -g -ggdb3'), ('CMAKE_CXX_FLAGS_RELWITHDEBINFO', '-Og -g -ggdb3'),),
)

detachments = (
		(('distortos_Scheduler_03_Support_for_thread_detachment', 'OFF'),),
		(('distortos_Scheduler_03_Support_for_thread_detachment', 'ON'),),
)

associations = {
		'ST_32F429IDISCOVERY': (
				'configurations/ST_32F429IDISCOVERY/test/distortosConfiguration.cmake',
				['-f', 'board/stm32f4discovery.cfg']),
		'ST_32F746GDISCOVERY': (
				'configurations/ST_32F746GDISCOVERY/test/distortosConfiguration.cmake',
				['-f', 'board/stm32f7discovery.cfg']),
		'ST_NUCLEO-F091RC': (
				'configurations/ST_NUCLEO-F091RC/test/distortosConfiguration.cmake',
				['-f', 'board/st_nucleo_f0.cfg']),
		'ST_NUCLEO-F103RB': (
				'configurations/ST_NUCLEO-F103RB/test/distortosConfiguration.cmake',
				['-f', 'board/st_nucleo_f103rb.cfg']),
		'ST_NUCLEO-F401RE': (
				'configurations/ST_NUCLEO-F401RE/test/distortosConfiguration.cmake',
				['-f', 'board/st_nucleo_f4.cfg']),
		'ST_NUCLEO-F429ZI': (
				'configurations/ST_NUCLEO-F429ZI/test/distortosConfiguration.cmake',
				['-f', 'board/st_nucleo_f4.cfg']),
		'ST_NUCLEO-L073RZ': (
				'configurations/ST_NUCLEO-L073RZ/test/distortosConfiguration.cmake',
				['-f', 'board/st_nucleo_l073rz.cfg']),
		'ST_NUCLEO-L432KC': (
				'configurations/ST_NUCLEO-L432KC/test/distortosConfiguration.cmake',
				['-f', 'board/st_nucleo_l4.cfg']),
		'ST_NUCLEO-L476RG': (
				'configurations/ST_NUCLEO-L476RG/test/distortosConfiguration.cmake',
				['-f', 'board/st_nucleo_l4.cfg']),
		'ST_STM32F4DISCOVERY': (
				'configurations/ST_STM32F4DISCOVERY/test/distortosConfiguration.cmake',
				['-f', 'board/stm32f4discovery.cfg']),
}

class TestError(RuntimeError):
    """Test error exception."""

    pass

def error(arguments, input, returncode, stderr, stdout):
	"""Prints info about errored subprocess and raises `TestError` exception.

	* `arguments` is a list with program name and its arguments
	* `input` is the string passed to stdin of subprocess
	* `returncode` is the return code of the subprocess
	* `stderr` is the stderr of the subprocess
	* `stdout` is the stdout of the subprocess
	"""

	print('Subprocess \'{}\' failed, return code = {}'.format(' '.join(arguments), returncode))
	if input:
		print('  input:\n{}'.format(textwrap.indent(input.strip(), '    ')))
	if stderr:
		print('  stderr:\n{}'.format(textwrap.indent(stderr.strip(), '    ')))
	if stdout:
		print('  stdout:\n{}'.format(textwrap.indent(stdout.strip(), '    ')))
	raise TestError

def runner(arguments, environment = None, input = None, timeout = 5, workingDirectory = None):
	"""Starts subprocess and waits until it terminates.

	If subprocess terminates successfully, this function returns the struct returned by `subprocess.run()`. Otherwise
	`error()` is called.

	* `arguments` is a list with program name and its arguments
	* `environment` is the environment for subprocess, default - None
	* `input` is the string passed to stdin of subprocess, default - None
	* `timeout` is the timeout for subprocess, seconds, default - 5
	* `workingDirectory` is the working directory for subprocess, default - None
	"""

	ret = subprocess.run(arguments, cwd = workingDirectory, env = environment, input = input, stdout = subprocess.PIPE,
			stderr = subprocess.PIPE, timeout = timeout, universal_newlines = True)
	if ret.returncode != 0:
		error(arguments, input, ret.returncode, ret.stderr, ret.stdout)

	return ret

def cmake(configurationFilename, pairs, environment):
	"""Configures project with CMake.

	* `configurationFilename` is the name of configuration file
	* `pairs` is a tuple of pairs with CMake configuration options
	* `environment` is the environment for CMake
	"""

	os.mkdir('output/')
	arguments = ['cmake', '-C', '../' + configurationFilename]
	for key, value in pairs:
		arguments.append('-D' + key + '=' + value)
	arguments += ['..', '-GNinja']
	runner(arguments, environment = environment, workingDirectory = 'output/')

def distclean():
	"""Removes build output."""

	shutil.rmtree('output/')

def gdb():
	"""Starts debugging session with GDB.

	If GDB session terminates successfully, returns tuple with result of test, number of system ticks used by the test
	and number of context switches executed during the test. Otherwise GDB's stdout is printed and `TestError` exception
	is raised.
	"""

	input = 'target remote localhost:3333\n' \
			'monitor reset halt\n' \
			'load\n' \
			'monitor reset halt\n' \
			'hbreak main.cpp:54\n' \
			'continue\n' \
			'print result\n' \
			'print distortos::internal::schedulerInstance.tickCount_\n' \
			'print distortos::internal::schedulerInstance.contextSwitchCount_\n'
	ret = runner(['arm-none-eabi-gdb', 'output/test/distortosTest.elf'], input = input, timeout = 30)
	match = re.search(r"\(gdb\) \$1 = (\w+)\s\(gdb\) \$2 = (\d+)\s\(gdb\) \$3 = (\d+)", ret.stdout)
	try:
		result = match.group(1)
		tickCount = match.group(2)
		contextSwitchCount = match.group(3)
	except AttributeError:
		print('Parsing GDB output failed\n  stdout:\n{}'.format(textwrap.indent(ret.stdout.strip(), '    ')))
		raise TestError

	return result, tickCount, contextSwitchCount

def ninja():
	"""Builds `distortosTest` target with `ninja`.

	If build finishes successfully, returns tuple with sizes of test, data and bss sections. Otherwise ninja's stdout is
	printed and `TestError` exception is raised.
	"""

	ret = runner(['ninja', 'distortosTest'], timeout = 20, workingDirectory = 'output/')
	match = re.search(r"(\d+)\s+(\d+)\s+(\d+)\s+\d+\s+[\da-f]+\s+.+output/test/distortosTest\.elf", ret.stdout)
	try:
		text = match.group(1)
		data = match.group(2)
		bss = match.group(3)
	except AttributeError:
		print('Parsing make output failed\n  stdout:\n{}'.format(textwrap.indent(ret.stdout.strip(), '    ')))
		raise TestError

	return text, data, bss

########################################################################################################################
# main
########################################################################################################################

if __name__ == '__main__':
	parser = argparse.ArgumentParser()
	parser.add_argument('board', help = 'board name')
	parser.add_argument('--paths', nargs = '+', help = 'paths prepended to PATH environment variable')
	arguments = parser.parse_args()

	parameters = associations[arguments.board]
	configurationFilename = parameters[0]
	openocdArguments = parameters[1]

	openocd = subprocess.Popen(['openocd'] + openocdArguments, stdout = subprocess.PIPE, stderr = subprocess.PIPE,
			universal_newlines = True)

	try:

		try:
			returncode = openocd.wait(5)
			if returncode != None:
				error(openocd.args, None, returncode, openocd.stderr.read(), openocd.stdout.read())
		except subprocess.TimeoutExpired:
			pass

		totalTests = 0
		completedTests = 0
		successfulTests = 0

		for path in arguments.paths or ['']:
			environment = os.environ.copy()
			if path:
				environment['PATH'] = path + os.pathsep + environment['PATH']
			for tuples in itertools.product(optimizations, detachments):
				totalTests += 1
				print('-' * 80)
				print('PATH = \'{}\''.format(environment['PATH']))

				pairs = tuple(itertools.chain.from_iterable(tuples))
				for key, value in pairs:
					print('{} = {}'.format(key, value))

				try:
					print('Cleaning...')
					distclean()
					print('Configuring...')
					cmake(configurationFilename, pairs, environment)
					print('Building...')
					text, data, bss = ninja()
					print('text = {}, data = {}, bss = {}'.format(text, data, bss))
					print('Running...')
					result, tickCount, contextSwitchCount = gdb()
					print('result = {}, tick count = {}, context switch count = {}'.format(result, tickCount,
							contextSwitchCount))
					completedTests += 1
					successfulTests += 1 if result else 0
				except TestError:
					print('Failed')
				except subprocess.TimeoutExpired:
					print('Timed-out')

	finally:
		openocd.kill()

	print('=' * 80)
	print('tests = {}, completed = {}, successful = {}'.format(totalTests, completedTests, successfulTests))
