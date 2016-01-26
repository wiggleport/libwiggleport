// Copyright (c) 2016 Micah Elizabeth Scott
// See attached LICENSE file for the MIT-style terms.

#ifndef _WIGGLEPORT_H_
#define _WIGGLEPORT_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque types
typedef struct wigl_context wigl_context;
typedef struct wigl_enumerator wigl_enumerator;
typedef struct wigl_model wigl_model;
typedef struct wigl_action wigl_action;
typedef struct wigl_stream wigl_stream;

// Callbacks
typedef void (wigl_enumerator_cb) (wigl_enumerator* en, void* cb_data);

// Context -- your connection to libwiggleport itself

wigl_context* wigl_context_new(const char* options, char** err);
void wigl_context_unref(wigl_context* ctx);

// Package management

bool wigl_package_load(wigl_context* ctx, const char* content, const char* files, char** error);
bool wigl_package_load_file(wigl_context* ctx, const char* path, char** error);
const char* wigl_package_inventory(wigl_context* ctx);
const char* wigl_package_content(wigl_context* ctx, const char* name);

// Enumerator -- find hardware models, and get hotplug notifications

wigl_enumerator* wigl_enumerator_new(wigl_context* ctx, const char* options, char** error);
void wigl_enumerator_delete(wigl_enumerator* en);

bool wigl_enumerator_dequeue(wigl_enumerator* en, wigl_model** add, wigl_model** remove);
void wigl_enumerator_notify(wigl_enumerator* en, wigl_enumerator_cb* cb, void* cb_data);

// Model -- a data tree for interacting with hardware

void wigl_model_unref(wigl_model* mod);
const char* wigl_model_content(wigl_model* mod);

void wigl_model_notify(wigl_model* mod, wigl_model_cb* cb, void* cb_data);
// xxx: lifetime of this is bunk, probe object could be better?

int64_t wigl_model_get_int(wigl_model* mod, const char* key, char** error);
double wigl_model_get_number(wigl_model* mod, const char* key, char** error);
const char* wigl_model_get_string(wigl_model* mod, const char* key, char** error);
// xxx: obj lifetime of return is error prone; do we need strings? use cases?

// Action -- a change we want to make to the model

wigl_action* wigl_action_new(wigl_model* mod, const char* dict, char** error);
void wigl_action_unref(wigl_action* ac);

bool wigl_action_set_int(wigl_action* ac, const char* key, int64_t value, char** error);
bool wigl_action_set_number(wigl_action* ac, const char* key, int64_t value, char** error);
bool wigl_action_set_string(wigl_action* ac, const char* key, const char* value, char** error);

bool wigl_action_schedule(wigl_action* ac, const char* options, char** error);
bool wigl_action_complet
const char* wigl_action_detail()

// Stream -- real-time I/O interface

wigl_stream* wigl_stream_open(wigl_model* mod, const char* key, const char* mode, char** error);
void wigl_stream_close(wigl_stream* st);

bool wigl_stream_has_fixed_rate(wigl_stream* st);
double wigl_stream_get_rate_hz(wigl_stream* st);
uint64_t wigl_stream_get_clock(wigl_stream* st);



#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _WIGGLEPORT_H_


// Guidelines:
//    - make it trivial to wrap in any language
//    - JSON strings, minimal exposed memory management

// handles? how?

// What happens to superceded devices?
//     - resources stay around, threads stop
//     - can client choose to transparently port to new version?

// (counted root reference, tree path)
//   ^- client rep? hash? small int? OS handle?

// Package management / Init

//  -- Start up with/without standard packages
//  -- Query installed packages
//  -- Load/upgrade package

// Class enumeration

//  -- enumerate objects matching 'class'
//     -- how is an object represented?
//     -- what can I do to a class once it's found?

/*

per-user root service
	"open class" broadcasts to all roots



class client-side ID: invalidated on -reload-

- open class
   notify on exist, supercede (by new version or null)
   JSON description of static content (exist, reload)
   read-only SHM for dynamic content

   - open stream
      writeable SHM
	  notify on read and/or write

   - open expression
	  notify on value, constraint error

* event queues
    - matching class
    - expression
  common insert/remove/monitor API?



*/
