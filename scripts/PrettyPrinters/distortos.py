#
# file: distortos.py
#
# author: Copyright (C) 2017 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

import gdb
import gdb.printing
import PrettyPrinters.estd

########################################################################################################################
# InternalThreadList class
########################################################################################################################

class InternalThreadList(PrettyPrinters.estd.SortedIntrusiveList):
	'Print distortos::internal::ThreadList'

	def __init__(self, value):
		super().__init__(value, 'distortos::internal::ThreadList')

########################################################################################################################
# Semaphore class
########################################################################################################################

class Semaphore():
	'Helper class for distortos::Semaphore'

	def __init__(self, value):
		self.value = value

	def isValid(self):
		# current value of semaphore must be less than or equal to its max value
		if self.value['value_'] > self.value['maxValue_']:
			return False
		# list of blocked threads must be valid
		return PrettyPrinters.estd.SortedIntrusiveList(self.value['blockedList_']).isValid()

########################################################################################################################
# InternalFifoQueueBase class
########################################################################################################################

class InternalFifoQueueBase:
	'Print distortos::internal::FifoQueueBase'

	class Iterator:
		'Iterate over distortos::internal::FifoQueueBase'

		def __init__(self, begin, length, storageBegin, storageEnd, elementSize, storageToElement):
			uint8PointerType = gdb.lookup_type('uint8_t').pointer()
			self.length = length
			self.storageBegin = storageBegin.cast(uint8PointerType)
			self.storageEnd = storageEnd.cast(uint8PointerType)
			self.elementSize = elementSize
			self.storageToElement = storageToElement
			self.index = 0
			self.iterator = begin.cast(uint8PointerType)

		def __iter__(self):
			return self

		def __next__(self):
			if self.index == self.length:
				raise StopIteration
			index = self.index
			element = self.storageToElement(self.iterator)
			self.index += 1
			self.iterator += self.elementSize
			if self.iterator == self.storageEnd:
				self.iterator = self.storageBegin
			return ('[%d]' % index, element)

	def __init__(self, value, name = 'distortos::internal::FifoQueueBase'):
		self.value = value
		self.name = name

	def children(self):
		if self.isValid() == False:
			return iter([])
		return self.Iterator(self.value['readPosition_'], self.value['popSemaphore_']['value_'],
				self.value['storageUniquePointer_']['_M_t']['_M_head_impl'], self.value['storageEnd_'],
				self.value['elementSize_'], self.storageToElement)

	def display_hint(self):
		# https://bugs.eclipse.org/bugs/show_bug.cgi?id=512795
		return 'array'

	def isValid(self):
		storageBegin = self.value['storageUniquePointer_']['_M_t']['_M_head_impl']
		storageEnd = self.value['storageEnd_']
		# value of "storage begin" must be less than or equal to "storage end"
		if storageBegin > storageEnd:
			return False
		sizeType = gdb.lookup_type('size_t')
		rawLength = storageEnd.cast(sizeType) - storageBegin.cast(sizeType)
		elementSize = self.value['elementSize_']
		# raw difference between values of "storage begin" and "storage end" must be a multiple of element size
		if rawLength % elementSize != 0:
			return False
		readPosition = self.value['readPosition_']
		# raw difference between values of "storage begin" and "read position" must be a multiple of element size
		if (readPosition.cast(sizeType) - storageBegin.cast(sizeType)) % elementSize != 0:
			return False
		writePosition = self.value['writePosition_']
		# raw difference between values of "storage begin" and "write position" must be a multiple of element size
		if (writePosition.cast(sizeType) - storageBegin.cast(sizeType)) % elementSize != 0:
			return False
		# both semaphores must be valid
		return Semaphore(self.value['popSemaphore_']).isValid() and Semaphore(self.value['pushSemaphore_']).isValid()

	def storageToElement(self, storage):
		return storage.cast(gdb.lookup_type('void').pointer())

	def to_string(self):
		valid = self.isValid()
		popSemaphore = self.value['popSemaphore_']
		length = popSemaphore['value_'] if valid else 0
		capacity = popSemaphore['maxValue_'] if valid else 0
		return ('%s of length %d, capacity %d' % (self.name, length, capacity))

########################################################################################################################
# FifoQueue class
########################################################################################################################

class FifoQueue(InternalFifoQueueBase):
	'Print distortos::FifoQueue'

	def __init__(self, value, name = 'distortos::FifoQueue'):
		super().__init__(value['fifoQueueBase_'], name)
		self.type = value.type.template_argument(0).pointer()

	def storageToElement(self, storage):
		return storage.cast(self.type).dereference()

########################################################################################################################
# StaticFifoQueue class
########################################################################################################################

class StaticFifoQueue(FifoQueue):
	'Print distortos::StaticFifoQueue'

	def __init__(self, value, name = 'distortos::StaticFifoQueue'):
		super().__init__(value, name)

########################################################################################################################
# registerPrettyPrinters()
########################################################################################################################

def registerPrettyPrinters(obj):
	'Register pretty-printers for distortos'

	prettyPrinters = gdb.printing.RegexpCollectionPrettyPrinter('distortos::internal')
	prettyPrinters.add_printer('distortos::internal::FifoQueueBase', '^distortos::internal::FifoQueueBase$',
			InternalFifoQueueBase)
	prettyPrinters.add_printer('distortos::internal::ThreadList', '^distortos::internal::ThreadList$',
			InternalThreadList)
	gdb.printing.register_pretty_printer(obj, prettyPrinters)

	prettyPrinters = gdb.printing.RegexpCollectionPrettyPrinter('distortos')
	prettyPrinters.add_printer('distortos::FifoQueue', '^distortos::FifoQueue<.*>$', FifoQueue)
	prettyPrinters.add_printer('distortos::StaticFifoQueue', '^distortos::StaticFifoQueue<.*>$', StaticFifoQueue)
	gdb.printing.register_pretty_printer(obj, prettyPrinters)
