/* Copyright (c) 2009-2012, Avian Contributors

   Permission to use, copy, modify, and/or distribute this software
   for any purpose with or without fee is hereby granted, provided
   that the above copyright notice and this permission notice appear
   in all copies.

   There is NO WARRANTY for this software.  See license.txt for
   details. */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"

namespace avian {

namespace tools {

String::String(const char* text):
  text(text),
  length(strlen(text)) {}

Buffer::Buffer():
  capacity(100),
  length(0),
  data((uint8_t*)malloc(capacity)) {}

Buffer::~Buffer() {
  free(data);
}

void Buffer::ensure(size_t more) {
  if(length + more > capacity) {
    capacity = capacity * 2 + more;
    data = (uint8_t*)realloc(data, capacity);
  }
}

void Buffer::write(const void* d, size_t size) {
  ensure(size);
  memcpy(data + length, d, size);
  length += size;
}

unsigned StringTable::add(String str) {
  unsigned offset = Buffer::length;
  Buffer::write(str.text, str.length + 1);
  return offset;
}

void OutputStream::write(uint8_t byte) {
  writeChunk(&byte, 1);
}

void OutputStream::writeRepeat(uint8_t byte, size_t size) {
  for(size_t i = 0; i < size; i++) {
    write(byte);
  }
}

FileOutputStream::FileOutputStream(const char* name):
  file(fopen(name, "wb")) {}

FileOutputStream::~FileOutputStream() {
  if(file) {
    fclose(file);
  }
}

bool FileOutputStream::isValid() {
  return file;
}

void FileOutputStream::writeChunk(const void* data, size_t size) {
  fwrite(data, size, 1, file);
}

void FileOutputStream::write(uint8_t byte) {
  fputc(byte, file);
}


Platform* Platform::first = 0;

PlatformInfo::OperatingSystem PlatformInfo::osFromString(const char* os) {
  if(strcmp(os, "linux") == 0) {
    return Linux;
  } else if(strcmp(os, "windows") == 0) {
    return Windows;
  } else if(strcmp(os, "darwin") == 0) {
    return Darwin;
  } else {
    return UnknownOS;
  }
}

PlatformInfo::Architecture PlatformInfo::archFromString(const char* arch) {
  if(strcmp(arch, "i386") == 0) {
    return x86;
  } else if(strcmp(arch, "x86_64") == 0) {
    return x86_64;
  } else if(strcmp(arch, "powerpc") == 0) {
    return PowerPC;
  } else if(strcmp(arch, "arm") == 0) {
    return Arm;
  } else {
    return UnknownArch;
  }
}

Platform* Platform::getPlatform(PlatformInfo info) {
  for(Platform* p = first; p; p = p->next) {
    if(p->info == info) {
      return p;
    }
  }
  return 0;
}

} // namespace tools

} // namespace avian