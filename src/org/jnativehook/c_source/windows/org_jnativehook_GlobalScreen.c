/*
 *	JNativeHook - GrabKeyHook - 09/08/06
 *  Alexander Barker
 *
 *	JNI Interface for setting a Keyboard Hook and monitoring
 *	it with java.
 *
 *  TODO Add LGPL License
 */

/*
Compiling Options:
	gcc -m32 -march=i586 -shared -fPIC -lX11 -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux ./org_jnativehook_keyboard_GrabKeyHook.c -o libJNativeHook_Keyboard.so
	gcc -m64 -march=k8 -shared -fPIC -lX11 -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux ./org_jnativehook_keyboard_GrabKeyHook.c -o libJNativeHook_Keyboard.so
*/

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif


#ifdef DEBUG
#include <stdio.h>
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <windows.h>

#include <jni.h>

#include "include/org_jnativehook_GlobalScreen.h"
#include "include/JConvertToNative.h"
#include "WinKeyCodes.h"

//Instance Variables
bool bRunning = TRUE;
HHOOK handleKeyboardHook = NULL;
HHOOK handleMouseHook = NULL;


unsigned short int keysize = 0;
KeyCode * grabkeys = NULL;

unsigned short int buttonsize = 0;
KeyCode * grabbuttons = NULL;

bool rshift_mask = FALSE;
bool lshift_mask = FALSE;
bool rcontrol_mask = FALSE;
bool lcontrol_mask = FALSE;
bool ralt_mask = FALSE;
bool lalt_mask = FALSE;
bool rmeta_mask = FALSE;
bool lmeta_mask = FALSE;


JavaVM * jvm = NULL;
//pthread_t hookThreadId = 0;

void jniFatalError(char * message) {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	#ifdef DEBUG
	printf("Native: Fatal Error - %s\n", message);
	#endif

	(*env)->FatalError(env, message);
	exit(1);
}

void throwException(char * message) {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	(*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL);

	//Locate our exception class
	jclass objExceptionClass = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyException");

	if (objExceptionClass != NULL) {
		#ifdef DEBUG
		printf("Native: Exception - %s\n", message);
		#endif

		(*env)->ThrowNew(env, objExceptionClass, message);
	}
	else {
		//Unable to find exception class, Terminate with error.
		jniFatalError("Unable to locate NativeKeyException class.");
	}
}

LRESULT CALLBACK LowLevelProc(int nCode, WPARAM wParam, LPARAM lParam) {
	//Attach to the currently running jvm
	JNIEnv * env = NULL;
	if ((*jvm)->AttachCurrentThread(jvm, (void **)(&env), NULL) >= 0) {

		//Class and Constructor for the NativeKeyEvent Object
		jclass clsKeyEvent = (*env)->FindClass(env, "org/jnativehook/keyboard/NativeKeyEvent");
		jmethodID KeyEvent_ID = (*env)->GetMethodID(env, clsKeyEvent, "<init>", "(IJIICI)V");

		//Class and Constructor for the NativeMouseEvent Object
		jclass clsButtonEvent = (*env)->FindClass(env, "org/jnativehook/mouse/NativeMouseEvent");
		jmethodID MouseEvent_ID = (*env)->GetMethodID(env, clsButtonEvent, "<init>", "(IJIIIIZI)V");

		//Class and getInstance method id for the GlobalScreen Object
		jclass clsGlobalScreen = (*env)->FindClass(env, "org/jnativehook/GlobalScreen");
		jmethodID getInstance_ID = (*env)->GetStaticMethodID(env, clsGlobalScreen, "getInstance", "()Lorg/jnativehook/GlobalScreen;");

		//A reference to the GlobalScreen Object
		jobject objGlobalScreen = (*env)->CallStaticObjectMethod(env, clsGlobalScreen, getInstance_ID);

		//ID's for the pressed, typed and released callbacks
		jmethodID fireKeyPressed_ID = (*env)->GetMethodID(env, clsGlobalScreen, "fireKeyPressed", "(Lorg/jnativehook/keyboard/NativeKeyEvent;)V");
		jmethodID fireKeyReleased_ID = (*env)->GetMethodID(env, clsGlobalScreen, "fireKeyReleased", "(Lorg/jnativehook/keyboard/NativeKeyEvent;)V");

		jmethodID fireMousePressed_ID = (*env)->GetMethodID(env, clsGlobalScreen, "fireMousePressed", "(Lorg/jnativehook/mouse/NativeMouseEvent;)V");
		jmethodID fireMouseReleased_ID = (*env)->GetMethodID(env, clsGlobalScreen, "fireMousePressed", "(Lorg/jnativehook/mouse/NativeMouseEvent;)V");

		switch (wParam) {
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				KBDLLHOOKSTRUCT * p = (KBDLLHOOKSTRUCT *)lParam;

				#ifdef DEBUG
				printf("Native: LowLevelProc - Key pressed (%i)\n", p.vkCode);
				#endif

				if (xev.xkey.keycode == VK_LSHIFT)		lshift_mask = TRUE;
				if (xev.xkey.keycode == VK_RSHIFT)		rshift_mask = TRUE;

				if (xev.xkey.keycode == VK_LCONTROL)	lcontrol_mask = TRUE;
				if (xev.xkey.keycode == VK_RCONTROL)	rcontrol_mask = TRUE;

				if (xev.xkey.keycode == VK_LMENU)		lalt_mask = TRUE;
				if (xev.xkey.keycode == VK_RMENU)		ralt_mask = TRUE;

				if (xev.xkey.keycode == VK_LWIN)		lmeta_mask = TRUE;
				if (xev.xkey.keycode == VK_RWIN)		rmeta_mask = TRUE;


				xev.xkey.keycode

				jkey = NativeToJKeycode(xev.xkey.keycode);
				modifiers = 0;
				if (xev.xkey.state & ShiftMask)			modifiers |= NativeToJModifier(ShiftMask);
				if (xev.xkey.state & ControlMask)		modifiers |= NativeToJModifier(ControlMask);
				if (xev.xkey.state & getMetaMask())		modifiers |= NativeToJModifier(getMetaMask());
				if (xev.xkey.state & getAltMask())		modifiers |= NativeToJModifier(getAltMask());

				objEvent = (*env)->NewObject(env, clsKeyEvent, KeyEvent_ID, JK_KEY_PRESSED, (jlong) xev.xkey.time, modifiers, jkey.keycode, (jchar) jkey.keycode, jkey.location);
				(*env)->CallVoidMethod(env, objGlobalScreen, fireKeyPressed_ID, objEvent);
				objEvent = NULL;
			break;

			case WM_KEYUP:
			case WM_SYSKEYUP:
				KBDLLHOOKSTRUCT * p = (KBDLLHOOKSTRUCT *)lParam;

				#ifdef DEBUG
				printf("Native: LowLevelProc - Key released(%i)\n", p.vkCode);
				#endif

				if (xev.xkey.keycode == VK_LSHIFT)		lshift_mask = FALSE;
				if (xev.xkey.keycode == VK_RSHIFT)		rshift_mask = FALSE;

				if (xev.xkey.keycode == VK_LCONTROL)	lcontrol_mask = FALSE;
				if (xev.xkey.keycode == VK_RCONTROL)	rcontrol_mask = FALSE;

				if (xev.xkey.keycode == VK_LMENU)		lalt_mask = FALSE;
				if (xev.xkey.keycode == VK_RMENU)		ralt_mask = FALSE;

				if (xev.xkey.keycode == VK_LWIN)		lmeta_mask = FALSE;
				if (xev.xkey.keycode == VK_RWIN)		rmeta_mask = FALSE;


				jkey = NativeToJKeycode(xev.xkey.keycode);
				modifiers = 0;
				if (xev.xkey.state & ShiftMask)			modifiers |= NativeToJModifier(ShiftMask);
				if (xev.xkey.state & ControlMask)		modifiers |= NativeToJModifier(ControlMask);
				if (xev.xkey.state & getMetaMask())		modifiers |= NativeToJModifier(getMetaMask());
				if (xev.xkey.state & getAltMask())		modifiers |= NativeToJModifier(getAltMask());

				objEvent = (*env)->NewObject(env, clsKeyEvent, KeyEvent_ID, JK_KEY_RELEASED, (jlong) xev.xkey.time, modifiers, jkey.keycode, (jchar) jkey.keycode, jkey.location);
				(*env)->CallVoidMethod(env, objGlobalScreen, fireKeyReleased_ID, objEvent);
				objEvent = NULL;
			break;

			case WM_XBUTTONDOWN:
			case WM_NCXBUTTONDOWN:
				fwButton = GET_XBUTTON_WPARAM(wParam);

				#ifdef DEBUG
				printf("Native: LowLevelProc - Button pressed (%i)\n", sizeof );
				#endif

				jkey = NativeToJKeycode(xev.xkey.keycode);
				modifiers = 0;
				if (xev.xkey.state & ShiftMask)			modifiers |= NativeToJModifier(ShiftMask);
				if (xev.xkey.state & ControlMask)		modifiers |= NativeToJModifier(ControlMask);
				if (xev.xkey.state & getMetaMask())		modifiers |= NativeToJModifier(getMetaMask());
				if (xev.xkey.state & getAltMask())		modifiers |= NativeToJModifier(getAltMask());

				objEvent = (*env)->NewObject(env, clsKeyEvent, KeyEvent_ID, JK_KEY_PRESSED, (jlong) xev.xkey.time, modifiers, jkey.keycode, (jchar) jkey.keycode, jkey.location);
				(*env)->CallVoidMethod(env, objGlobalScreen, fireKeyPressed_ID, objEvent);
				objEvent = NULL;
			break;

			case WM_XBUTTONUP:
			case WM_NCXBUTTONUP:
				fwButton = GET_XBUTTON_WPARAM(wParam);

				#ifdef DEBUG
				printf("Native: LowLevelProc - Button released(%i)\n", xev.xkey.keycode);
				#endif

				jkey = NativeToJKeycode(xev.xkey.keycode);
				modifiers = 0;
				if (xev.xkey.state & ShiftMask)			modifiers |= NativeToJModifier(ShiftMask);
				if (xev.xkey.state & ControlMask)		modifiers |= NativeToJModifier(ControlMask);
				if (xev.xkey.state & getMetaMask())		modifiers |= NativeToJModifier(getMetaMask());
				if (xev.xkey.state & getAltMask())		modifiers |= NativeToJModifier(getAltMask());

				objEvent = (*env)->NewObject(env, clsKeyEvent, KeyEvent_ID, JK_KEY_RELEASED, (jlong) xev.xkey.time, modifiers, jkey.keycode, (jchar) jkey.keycode, jkey.location);
				(*env)->CallVoidMethod(env, objGlobalScreen, fireKeyReleased_ID, objEvent);
				objEvent = NULL;
			break;

			WM_MOUSEWHEEL:
			default:
			break;
		}
	}
	else {
		#ifdef DEBUG
		printf("Native: LowLevelProc - Error on the attach current thread.\n");
		#endif
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void MsgLoop() {
	MSG message;

	while (bRunning && GetMessage(&message, NULL, 0, 0)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	#ifdef DEBUG
	printf("Native: MsgLoop() stop successful.\n");
	#endif
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_grabKey(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jmodifiers, jint jkeycode, jint jkeylocation) {
	JKeyCode jkey;
	jkey.keycode = jkeycode;
	jkey.location = jkeylocation;

	#ifdef DEBUG
	printf("Native: grabKey - KeyCode(%i) Modifier(%X)\n", (unsigned int) jkeycode, jmodifiers);
	#endif

	KeyCode newkey;
	newkey.keycode = JKeycodeToNative(jkey);
	newkey.shift_mask = jmodifiers & JK_SHIFT_MASK;
	newkey.control_mask = jmodifiers & JK_CTRL_MASK;
	newkey.alt_mask = jmodifiers & JK_ALT_MASK;
	newkey.meta_mask = jmodifiers & JK_META_MASK;

	if (keysize == USHRT_MAX) {
		//TODO Throw exception
		return;
	}

	KeyCode * tmp = malloc( (keysize + 1) * sizeof(KeyCode) );

	int i = 0;
	for (; i < keysize; i++) {
		if (grabkey[i].keycode		== newkey.keycode &&
			grabkey[i].shift_mask	== newkey.shift_mask &&
			grabkey[i].control_mask	== newkey.control_mask &&
			grabkey[i].alt_mask		== newkey.alt_mask &&
			grabkey[i].meta_mask	== newkey.meta_mask
		) {
			//We have a duplicate.
			free(tmp);
			return;
		}

		tmp[i] = grabkey[i];
	}

	free(grabkey);
	grabkey = tmp;
	grabkey[keysize] = newkey;
	keysize++;
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_ungrabKey(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jmodifiers, jint jkeycode, jint jkeylocation) {
	JKeyCode jkey;
	jkey.keycode = jkeycode;
	jkey.location = jkeylocation;

	#ifdef DEBUG
	printf("Native: ungrabKey - KeyCode(%i) Modifier(%X)\n", (unsigned int) jkeycode, jmodifiers);
	#endif

	KeyCode newkey;
	newkey.keycode = JKeycodeToNative(jkey);
	newkey.shift_mask = jmodifiers & JK_SHIFT_MASK;
	newkey.control_mask = jmodifiers & JK_CTRL_MASK;
	newkey.alt_mask = jmodifiers & JK_ALT_MASK;
	newkey.meta_mask = jmodifiers & JK_META_MASK;

	KeyCode * tmp = malloc( (keysize - 1) * sizeof(KeyCode) );

	int i = 0, j = 0;
	for (; i < keysize; i++) {
		if (grabkey[i].keycode		!= newkey.keycode ||
			grabkey[i].shift_mask	!= newkey.shift_mask ||
			grabkey[i].control_mask	!= newkey.control_mask ||
			grabkey[i].alt_mask		!= newkey.alt_mask ||
			grabkey[i].meta_mask	!= newkey.meta_mask
		) {
			tmp[j++] = grabkey[i];
		}
	}

	free(grabkey);
	keysize--;
	grabkey = tmp;
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_grabButton(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jbutton) {
	#ifdef DEBUG
	printf("Native: grabButton - Button(%i)\n", (unsigned int) button);
	#endif

	ButtonCode newbutton;
	newbutton.keycode = JButtonToNative(jbutton);
	/*
	newbutton.shift_mask = jmodifiers & JK_SHIFT_MASK;
	newbutton.control_mask = jmodifiers & JK_CTRL_MASK;
	newbutton.alt_mask = jmodifiers & JK_ALT_MASK;
	newbutton.meta_mask = jmodifiers & JK_META_MASK;
	*/

	if (buttonsize == USHRT_MAX) {
		//TODO Throw exception
		return;
	}

	ButtonCode * tmp = malloc( (buttonsize + 1) * sizeof(ButtonCode) );

	int i = 0;
	for (; i < buttonsize; i++) {
		if (grabbutton[i].keycode			== newkey.keycode &&
				grabbutton[i].shift_mask	== newkey.shift_mask &&
				grabbutton[i].control_mask	== newkey.control_mask &&
				grabbutton[i].alt_mask		== newkey.alt_mask &&
				grabbutton[i].meta_mask		== newkey.meta_mask
		) {
			//We have a duplicate.
			free(tmp);
			return;
		}

		tmp[i] = grabbutton[i];
	}

	free(grabbutton);
	grabbutton = tmp;
	grabbutton[buttonsize] = newbutton;
	buttonsize++;
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_ungrabButton(JNIEnv * UNUSED(env), jobject UNUSED(obj), jint jbutton) {
	#ifdef DEBUG
	printf("Native: ungrabButton - Button(%i)\n", (unsigned int) button);
	#endif

	ButtonCode newbutton;
	newbutton.keycode = JButtonToNative(jbutton);
	/*
	newbutton.shift_mask = jmodifiers & JK_SHIFT_MASK;
	newbutton.control_mask = jmodifiers & JK_CTRL_MASK;
	newbutton.alt_mask = jmodifiers & JK_ALT_MASK;
	newbutton.meta_mask = jmodifiers & JK_META_MASK;
	*/

	ButtonCode * tmp = malloc( (buttonsize - 1) * sizeof(ButtonCode) );

	int i = 0, j = 0;
	for (; i < buttonsize; i++) {
		if (grabkey[i].keycode		!= newkey.keycode ||
			grabkey[i].shift_mask	!= newkey.shift_mask ||
			grabkey[i].control_mask	!= newkey.control_mask ||
			grabkey[i].alt_mask		!= newkey.alt_mask ||
			grabkey[i].meta_mask	!= newkey.meta_mask
		) {
			tmp[j++] = grabkey[i];
		}
	}

	free(grabbutton);
	grabbutton = tmp;
	buttonsize--;
}


//This is where java attaches to the native machine.  Its kind of like the java + native constructor.
JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_initialize(JNIEnv * env, jobject UNUSED(obj)) {
	//Grab the currently running virtual machine so we can attach to it in
	//functions that are not called from java. ( I.E. MsgLoop )
	(*env)->GetJavaVM(env, &jvm);

	//Setup the native hooks and their callbacks.
	handleKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0);

	if (handleKeyboardHook != NULL) {
		#ifdef DEBUG
		printf("Native: SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0) successful\n");
		#endif
	}
	else {
		#ifdef DEBUG
		printf("Native: SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelProc, hInst, 0) failed\n");
		#endif

		throwException(env, "Failed to hook keyboard using SetWindowsHookEx");

		//Naturaly exit so jni exception is thrown.
		return;
	}


	handleMouseHook = SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, hInst, 0);
	if (handleMouseHook != NULL) {
		#ifdef DEBUG
		printf("Native: SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, hInst, 0) successful\n");
		#endif
	}
	else {
		#ifdef DEBUG
		printf("Native: SetWindowsHookEx(WH_MOUSE_LL, LowLevelProc, hInst, 0) failed\n");
		#endif

		throwException(env, "Failed to hook mouse using SetWindowsHookEx");

		//Naturaly exit so jni exception is thrown.
		return;
	}

	bRunning = True;

	/*
	if( pthread_create( &hookThreadId, NULL, (void *) &MsgLoop, NULL) ) {
		#ifdef DEBUG
		printf("Native: MsgLoop() start failure.\n");
		#endif
		//TODO Throw an exception
	}
	else {
		#ifdef DEBUG
		printf("Native: MsgLoop() start successful.\n");
		#endif
	}
	*/
}

JNIEXPORT void JNICALL Java_org_jnativehook_GlobalScreen_deinitialize(JNIEnv * UNUSED(env), jobject UNUSED(obj)) {
	if (handleKeyboardHook != NULL) {
		UnhookWindowsHookEx(hookHandle);
		handleKeyboardHook = NULL;
	}

	//Try to exit the thread naturally.
	bRunning = False;
	interruptMsgLoop();
	pthread_join(hookThreadId, NULL);

	#ifdef DEBUG
	printf("Native: Thread terminated successful.\n");
	#endif
}

BOOL APIENTRY DllMain(HINSTANCE _hInst, DWORD reason, LPVOID reserved) {
	switch (reason) {
		case DLL_PROCESS_ATTACH:
			#ifdef DEBUG
			printf("Native: DllMain - DLL Process attach.\n");
			#endif

			hInst = _hInst;
		break;

		case DLL_PROCESS_DETACH:
			#ifdef DEBUG
			printf("Native: DllMain - DLL Process Detach.\n");
			#endif
		break;

		default:
		break;
	}

	return TRUE;
}