#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <libk/io.h>
#include <stdio.h>
#include "cpu.h"
#include "../kernel/util.h"

char cpu_vendor[CPUID_VENDOR_LENGTH];
char cpu_family[CPUID_FAMILY_LENGTH];
char cpu_model[CPUID_MODEL_LENGTH];
char cpu_type[CPUID_TYPE_LENGTH];
uint8_t cpu_stepping_id = 0;

uint32_t cpu_cpuidcallsmax;

void cpu_getvendor() {
  char* value = get_cpu_vendor();  
  strcpy(cpu_vendor, value);
}

// https://en.wikipedia.org/wiki/CPUID
void cpu_getmodel() {
  uint32_t eax = get_cpu_model();  

  cpu_stepping_id = eax & 0b1111;
  uint8_t model_id = (eax >> 4) & 0b1111;
  uint8_t family_id = (eax >> 8) & 0b1111;
  uint8_t processor_type = (eax >> 12) & 0b11;
  uint8_t extended_model_id = (eax >> 16) & 0b1111;
  uint8_t extended_family_id = (eax >> 20) & 0b11111111;

  switch (processor_type) {
    case 0b00:
      strcpy(cpu_type, "Original equipment manufacturer (OEM) Processor");
      break;
    case 0b01:
      strcpy(cpu_type, "Intel Overdrive Processor");
      break;
    case 0b10:
      strcpy(cpu_type, "Dual processor (applicable to Instrcpy(tel P5 P,tium processors only)");
      break;
    case 0b11:
      strcpy(cpu_type, "Reserved");
      break;
    default:
      strcpy(cpu_type, "Error detecting cpu type");
      break;
  }

  uint8_t model = 0;
  if (family_id == 6 || family_id == 15) {
    model = (extended_model_id << 4) + model_id;
  } else {
    model = model_id;
  }

  uint8_t family = 0;
  if (family_id == 15) {
    family = extended_family_id + family_id;
  } else {
    family = family_id;
  }

  if (strcmp(cpu_vendor, CPUID_VENDOR_INTEL) == 0) {
    switch(family) {
      case 0x4:
        strcpy(cpu_family, "Intel 80486");
        break;
      case 0x5:
        strcpy(cpu_family, "Pentium, Pentium MMX or Quark X1000");
        break;
      case 0x6:
        strcpy(cpu_family, "Intel core, Pentium pro, II, III and M, Intel Atom, Xeon and Xeon Phi");
        break;
      case 0x7:
        strcpy(cpu_family, "Itanium");
        break;
      case 0xB:
        strcpy(cpu_family, "Xeon Phi");
        break;
      case 0xF:
        strcpy(cpu_family, "NetBurst");
        break;
      case 0x11:
        strcpy(cpu_family, "Itanium 2");
        break;
      case 0x12:
        strcpy(cpu_family, "Intel core");
        break;
      case 0x13:
        strcpy(cpu_family, "Xeon");
        break;
      default:
        cpu_stepping_id = family;
        strcpy(cpu_family, "Unknown Intel family");
        break;
    }

    switch (model) {
      // TODO: Add more cpu models, only if they have stopped gatekeeping all the model numbers
      case 58:
        strcpy(cpu_model, "i5-3570");
        break;
      // case:
      //   break;
      default:
        strcpy(cpu_model, "Unknown Intel model");
        break;
    }
  } else if (strcmp(cpu_vendor, CPUID_VENDOR_AMD) == 0 || strcmp(cpu_vendor, CPUID_VENDOR_AMD_OLD) == 0) { // TODO: check for all AMD cpu families
    strcpy(cpu_model, "Unknown AMD cpu");
  } else {
    strcpy(cpu_model, "Unknown cpu");
  }
}

bool cpu_supportcpuid() {
  return true; // TODO: make the check actually happen, either by running cpuid and checking for invalid opcode interrupt or running an eflags assembly procedure
}

void cpu_init() {
  if (!cpu_supportcpuid()) {
    printkf("Failed to identify cpu: cpuid instruction not supported\n");
    return;
  }

  cpu_cpuidcallsmax = get_cpu_support();

  cpu_getvendor();
  if (cpu_cpuidcallsmax >= 1) cpu_getmodel();
}

