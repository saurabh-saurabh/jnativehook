/* JNativeHook: Global keyboard and mouse hooking for Java.
 * Copyright (C) 2006-2013 Alexander Barker.  All Rights Received.
 * http://code.google.com/p/jnativehook/
 *
 * JNativeHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * JNativeHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdbool.h>

#include "nativehook.h"
#include "hook_thread.h"

static void (*current_dispatch_proc)(VirtualEvent * const) = null;

void set_dispatch_proc(void (*dispatch_proc)(VirtualEvent * const)) {
	current_dispatch_proc = dispatch_proc;
}

int dispatch_event(VirtualEvent * const event) {
	int status = NATIVEHOOK_FAILURE;

	if (current_dispatch_proc != null) {
		status = current_dispatch_proc(event);
	}

	return status;
}

int enable_native_hook() {
	return StartNativeThread();
}

int disable_native_hook() {
	return StopNativeThread();
}

bool is_native_hook_enabled() {
	return IsNativeThreadRunning();
}