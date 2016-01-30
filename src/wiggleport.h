/*
 * Copyright (c) 2016 Micah Elizabeth Scott
 *
 * Wiggleport is free software; you can redistribute it and/or modify
 * it under the terms of the MIT license. See LICENSE for details.
 */

#ifndef _WIGGLEPORT_H_
#define _WIGGLEPORT_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque types
typedef struct wigl_context wigl_context;
typedef struct wigl_enumerator wigl_enumerator;
typedef struct wigl_model wigl_model;
typedef struct wigl_tuple wigl_tuple;
typedef struct wigl_action wigl_action;
typedef struct wigl_schedule wigl_schedule;
typedef struct wigl_stream wigl_stream;

// Callbacks
typedef void (wigl_enumerator_cb) (wigl_enumerator* en, void* cb_data);
typedef void (wigl_tuple_cb) (wigl_tuple* tuple, void* cb_data);
typedef void (wigl_schedule_cb) (wigl_schedule* sched, void* cb_data);
typedef void (wigl_stream_cb) (wigl_stream* st, void* cb_data);

// Context -- your connection to libwiggleport itself

wigl_context* wigl_context_new(const char* json_options, char** error);
void wigl_context_unref(wigl_context* ctx);

// Package management -- packages generate models based on the attached hardware
//   (By default, libwiggleport loads a standard set of packages. Most programs
//    won't need the package management functions.)

bool wigl_package_load(wigl_context* ctx, const char* json_content, const char* json_files, char** error);
bool wigl_package_load_file(wigl_context* ctx, const char* path, char** error);
const char* wigl_package_name(const wigl_context* ctx, int index);
const char* wigl_package_json(const wigl_context* ctx, const char* name);

// Enumerator -- find hardware models, and get hotplug notifications

wigl_enumerator* wigl_enumerator_new(wigl_context* ctx, const char* json_options, char** error);
void wigl_enumerator_delete(wigl_enumerator* en);

bool wigl_enumerator_next(wigl_enumerator* en, wigl_model** added, wigl_model** removed);
void wigl_enumerator_notify(wigl_enumerator* en, wigl_enumerator_cb* cb, void* cb_data);

// Model -- a data tree for interacting with hardware

void wigl_model_unref(wigl_model* model);
const char* wigl_model_json(const wigl_model* model);

// Tuple -- an observed set of values from the model

wigl_tuple* wigl_tuple_new(const wigl_model* model, const char* json_refs, char** error);
void wigl_tuple_delete(wigl_tuple* tuple);

bool wigl_tuple_next(wigl_tuple* tuple, char** json_change_detail);
void wigl_tuple_notify(wigl_tuple* tuple, wigl_tuple_cb* cb, void* cb_data);

const char* wigl_tuple_json(const wigl_tuple* tuple);
int64_t wigl_tuple_int(const wigl_tuple* tuple, int index, char** error);
double wigl_tuple_number(const wigl_tuple* tuple, int index, char** error);
const char* wigl_tuple_string(const wigl_tuple* tuple, int index, char** error);

// Action -- a change that can be applied to the model

wigl_action* wigl_action_new(const wigl_model* model, const char* json_dict, char** error);
void wigl_action_delete(wigl_action* ac);

bool wigl_action_set_int(wigl_action* ac, const char* name, int64_t value, char** error);
bool wigl_action_set_number(wigl_action* ac, const char* name, double value, char** error);
bool wigl_action_set_string(wigl_action* ac, const char* name, const char* value, char** error);

wigl_schedule* wigl_action_schedule(wigl_action* ac, const char* json_options, char** error);

// Schedule -- things happening at a particular time across one or more streams

void wigl_schedule_delete(wigl_schedule* sched);
const char* wigl_schedule_json(const wigl_schedule* sched);

bool wigl_schedule_has_finished(const wigl_schedule* sched);
void wigl_schedule_wait(const wigl_schedule* sched);
void wigl_schedule_notify(wigl_schedule* sched, wigl_schedule_cb* cb, void* cb_data);

// Stream -- real-time I/O interface

wigl_stream* wigl_stream_new(wigl_model* model, const char* json_options, char** error);
void wigl_stream_delete(wigl_stream* st);
const char* wigl_stream_json(const wigl_stream* st);

uint64_t wigl_stream_clock(const wigl_stream* st);
wigl_schedule* wigl_stream_read(wigl_stream* st, uint8_t *buffer, size_t byte_count, uint64_t time_ref);
wigl_schedule* wigl_stream_write(wigl_stream* st, const uint8_t *buffer, size_t byte_count, uint64_t time_ref);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif // _WIGGLEPORT_H_
