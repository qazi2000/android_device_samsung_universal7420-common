/*
   Copyright (c) 2015, The Dokdo Project. All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.
   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   File Name : init_sec.c
   Create Date : 2015.11.03
   Author : Sunghun Ra
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>
#include <android-base/properties.h>

#include "property_service.h"
#include "vendor_init.h"

using android::base::GetProperty;
using android::base::ReadFileToString;
using android::base::Trim;

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void set_sim_info()
{
    const char *simslot_count_path = "/proc/simslot_count";
    std::string simslot_count;
    
    if (ReadFileToString(simslot_count_path, &simslot_count)) {
        simslot_count = Trim(simslot_count); // strip newline
        property_override("ro.multisim.simslotcount", simslot_count.c_str());
        if (simslot_count.compare("2") == 0) {
            property_override("rild.libpath2", "/system/vendor/lib64/libsec-ril-dsds.so");
            property_override("rild.libpath", "/system/vendor/lib64/libsec-ril.so");
            property_override("persist.radio.multisim.config", "dsds");
            property_override("persist.multisim.config", "dsds");
            property_override("ro.telephony.default_network", "9,9");
        }
    }
    else {
        LOG(ERROR) << "Could not open '" << simslot_count_path << "'\n";
    }
}

void vendor_load_properties()
{
    std::string bootloader = GetProperty("ro.bootloader", "");
    std::string device_orig = GetProperty("ro.product.device", "");
    std::string device;
    std::string model; 
    std::string product; 
        /*	
         * Note 5	
         */
        if (bootloader.find("N920") != std::string::npos) {

             product = "noblelte";
        
             if (bootloader.find("N920C") != std::string::npos) {	
                if (device_orig != "nobleltejv") {	
                    model = "SM-N920C";
                    device = "nobleltejv";
                }
            } else if (bootloader.find("N9208") != std::string::npos) {	
                model = "SM-N9208";
                device = "nobleltezt";
            } else if (bootloader.find("N920G") != std::string::npos) {	
                model = "SM-N920G";
                device = "nobleltedd";
            } else if (bootloader.find("N920I") != std::string::npos) {	
                model = "SM-N920I";
                device = "nobleltedv";
            } else if (bootloader.find("N920P") != std::string::npos) {	
                model = "SM-N920P";
                device = "nobleltespr";
            } else if (bootloader.find("N920S") != std::string::npos) {	
                model = "SM-N920S";
                device = "noblelteskt";
            } else if (bootloader.find("N920K") != std::string::npos) {	
                model = "SM-N920K";
                device = "nobleltektt";
            } else if (bootloader.find("N920L") != std::string::npos) {	
                model = "SM-N920L";
                device = "nobleltelgt";
            } else if (bootloader.find("N920T") != std::string::npos) {	
                model = "SM-N920T";
                device = "nobleltetmo";
            } else if (bootloader.find("N920W8") != std::string::npos) {	
                model = "SM-N920W8";
                device = "nobleltecan";
            }
        }
        
        /*
         * Edge Plus
         */
        else if (bootloader.find("G928") != std::string::npos) {
            
            product = "zenlte";
        
             if (bootloader.find("G928F") != std::string::npos) {
                if (device_orig != "zenltexx") {
                    model = "SM-G928F";
                    device = "zenltexx";
                }	
            } else if (bootloader.find("G928C") != std::string::npos) {	
                model = "SM-G928C";
                device = "zenltejv";	
            } else if (bootloader.find("G928I") != std::string::npos) {	
                model = "SM-G928I";	
                device = "zenltedv";	
            } else if (bootloader.find("G928G") != std::string::npos) {	
                model = "SM-G928G";
                device = "zenltedd";	
            } else if (bootloader.find("G928K") != std::string::npos) {	
                model = "SM-G928K";
                device = "zenltektt";
            } else if (bootloader.find("G928L") != std::string::npos) {	
                model = "SM-G928L";
                device = "zenltelgt";
            } else if (bootloader.find("G928S") != std::string::npos) {	
                model = "SM-G928S";
                device = "zenlteskt";
            } else if (bootloader.find("G928T") != std::string::npos) {	
                model = "SM-G928T";
                device = "zenltetmo";
            } else if (bootloader.find("G928W8") != std::string::npos) {	
                model = "SM-G928W8";
                device = "zenltecan";
            }
        }
        
        /*
         * Flat
         */
        else if (bootloader.find("G920") != std::string::npos) {
        
            product = "zeroflte";
        
            if (bootloader.find("G920F") != std::string::npos) {
                if (device_orig != "zeroflteduo") {
                    model = "SM-G920F";
                    device = "zerofltexx";
                }
            } else if (bootloader.find("G920I") != std::string::npos) {
                model = "SM-G920I";
                device = "zerofltexx";
            } else if (bootloader.find("G920K") != std::string::npos) {
                model = "SM-G920K";
                device = "zerofltektt";
            } else if (bootloader.find("G920L") != std::string::npos) {
                model = "SM-G920L";
                device = "zerofltelgt";
            } else if (bootloader.find("G920P") != std::string::npos) {
                model = "SM-G920P";
                device = "zerofltespr";
            } else if (bootloader.find("G920S") != std::string::npos) {
                model = "SM-G920S";
                device = "zeroflteskt";
            } else if (bootloader.find("G920T") != std::string::npos) {
                model = "SM-G920T";
                device = "zerofltetmo";
            } else if (bootloader.find("G920W8") != std::string::npos) {
                model = "SM-G920W8";
                device = "zerofltecan";
            }
        }
        
        /*
         * Edge
         */
        else if (bootloader.find("G925") != std::string::npos) {
        
             product = "zerolte";
        
             if (bootloader.find("G925F") != std::string::npos) {
                if (device_orig != "zeroflteduo") {
                    model = "SM-G925F";
                    device = "zeroltexx";
                }
            } else if (bootloader.find("G925I") != std::string::npos) {
                model = "SM-G925I";
                device = "zeroltexx";
            } else if (bootloader.find("G925K") != std::string::npos) {
                model = "SM-G925K";
                device = "zeroltektt";
            } else if (bootloader.find("G925L") != std::string::npos) {
                model = "SM-G925L";
                device = "zeroltelgt";
            } else if (bootloader.find("G925P") != std::string::npos) {
                model = "SM-G925P";
                device = "zeroltespr";
            } else if (bootloader.find("G925S") != std::string::npos) {
                model = "SM-G925S";
                device = "zerolteskt";
            } else if (bootloader.find("G925T") != std::string::npos) {
                model = "SM-G925T";
                device = "zeroltetmo";
            } else if (bootloader.find("G925W8") != std::string::npos) {
                model = "SM-G925W8";
                device = "zeroltecan";
            }
        }

    // load original properties
    std::string description_orig = GetProperty("ro.build.description", "");
    std::string fingerprint_orig = GetProperty("ro.build.fingerprint", "");

    // replace device-names with correct one
    if (device_orig != "") {
        if (description_orig != "")
            replace(description_orig, device_orig, device);

        if (fingerprint_orig != "")
            replace(fingerprint_orig, device_orig, device);
    }

    // update properties
    property_override("ro.product.model", model.c_str());
    property_override("ro.product.device", device.c_str());
    property_override("ro.build.product", product.c_str());
    property_override("ro.lineage.device", device.c_str());
    property_override("ro.vendor.product.device", device.c_str());
    property_override("ro.build.description", description_orig.c_str());
    property_override("ro.build.fingerprint", fingerprint_orig.c_str());
    property_override("ro.boot.warranty_bit", "0");
    property_override("ro.warranty_bit", "0");
    property_override("ro.boot.veritymode", "enforcing");
    property_override("ro.boot.verifiedbootstate", "green");
    property_override("ro.boot.flash.locked", "1");
    property_override("ro.boot.ddrinfo", "00000001");
    property_override("ro.build.selinux", "1");
    property_override("ro.fmp_config", "1");
    property_override("ro.boot.fmp_config", "1");
    property_override("sys.oem_unlock_allowed", "0");    

    // set dual-sim properties
    set_sim_info();

    device = GetProperty("ro.product.device", "");
    LOG(ERROR) << "Found bootloader id '" << bootloader.c_str() << "' setting build properties for '" << device.c_str() << "' device\n";
}