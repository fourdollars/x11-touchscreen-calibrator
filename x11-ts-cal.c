/* -*- coding: utf-8; indent-tabs-mode: nil; tab-width: 4; c-basic-offset: 4; -*- */
/**
 * Copyright (C) 2013 Shih-Yuan Lee (FourDollars) <sylee@canonical.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/Xrandr.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int pw; /* preferred width */
static int ph; /* preferred height */
static int sw; /* screen width */
static int sh; /* screen height */
static int dw; /* display width */
static int dh; /* display height */
static int dx; /* display x */
static int dy; /* display y */

typedef unsigned short ScalingMode;
#define ScalingMode_None        0
#define ScalingMode_Full        1
#define ScalingMode_Center      2
#define ScalingMode_Full_aspect 3
static ScalingMode scaling_mode;

static Rotation rotation = RR_Rotate_0;
static char* preferred;
static char* touch_screen;

static const char* white_list[] = {
    "LVDS",
    "LVDS1",
    "eDP1"
};

void search_touchscreen_device(Display *display)
{
    XIDeviceInfo     *info  = NULL;
    XIDeviceInfo     *dev   = NULL;
    XITouchClassInfo *touch = NULL;

    int i, j, ndevices;

    info = XIQueryDevice(display, XIAllDevices, &ndevices);

    for (i = 0; i < ndevices; i++) {
        dev = info + i;
        for (j = 0; j < dev->num_classes; j++) {
            touch = (XITouchClassInfo*) dev->classes[j];
            if (touch->type == XITouchClass && touch->mode == XIDirectTouch) {
                if (touch_screen) free(touch_screen);
                touch_screen = strdup(dev->name);
            }
        }
    }

    XIFreeDeviceInfo(info);
}

void get_display_info(Display *display)
{
	int                 screen = DefaultScreen(display);
    Window	            root   = RootWindow(display, screen);
    XRRScreenResources *res    = XRRGetScreenResourcesCurrent(display, root);

    int i, j, k, l;
    int minWidth, minHeight, maxWidth, maxHeight;

    if (XRRGetScreenSizeRange(display, root, &minWidth, &minHeight, &maxWidth, &maxHeight) == True)
    {
        sw = DisplayWidth(display, screen);
        sh = DisplayHeight(display, screen);
    }

    for (i = 0; i < res->noutput; i++)
    {
        XRROutputInfo *output = XRRGetOutputInfo (display, res, res->outputs[i]);

        if (output->connection == RR_Connected)
        {
            int size = sizeof(white_list) / sizeof(char*);

            for (l = 0; l < size; l++)
            {
                if (strcmp(white_list[l], output->name) == 0)
                {
                    for (j = 0; j < output->nmode; j++)
                    {
                        if (j < output->npreferred)
                        {
                            RRMode *mode = output->modes + j;

                            for (k = 0; k < res->nmode; k++)
                            {
                                XRRModeInfo *mode_info = &res->modes[k];
                                if (mode_info->id == *mode) {
                                    int nprop = 0;
                                    if (preferred) free(preferred);
                                    preferred = strdup(output->name);
                                    pw = mode_info->width;
                                    ph = mode_info->height;
                                    Atom *props = XRRListOutputProperties (display, res->outputs[i], &nprop);
                                    for (l = 0; l < nprop; l++)
                                    {
                                        if (strcmp("scaling mode", XGetAtomName (display, props[l])) == 0)
                                        {
                                            unsigned char *prop;
                                            int actual_format;
                                            unsigned long nitems, bytes_after;
                                            Atom actual_type;
                                            XRRGetOutputProperty (display, res->outputs[i], props[l],
                                                    0, 100, False, False,
                                                    AnyPropertyType,
                                                    &actual_type, &actual_format,
                                                    &nitems, &bytes_after, &prop);
                                            if (actual_type == XA_ATOM &&
                                                actual_format == 32 &&
                                                nitems == 1 &&
                                                bytes_after == 0)
                                            {
                                                const char* name = XGetAtomName (display, ((Atom *)prop)[0]);
                                                if (strcmp("None", name) == 0) {
                                                    scaling_mode = ScalingMode_None;
                                                } else if (strcmp("Full", name) == 0) {
                                                    scaling_mode = ScalingMode_Full;
                                                } else if (strcmp("Center", name) == 0) {
                                                    scaling_mode = ScalingMode_Center;
                                                } else if (strcmp("Full aspect", name) == 0) {
                                                    scaling_mode = ScalingMode_Full_aspect;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < res->ncrtc; i++)
    {
        XRRCrtcInfo *crtc = XRRGetCrtcInfo (display, res, res->crtcs[i]);
        for (j = 0; j < res->nmode; j++)
        {
            XRRModeInfo mode = res->modes[j];
            if (mode.id == crtc->mode && crtc->noutput == 1) {
                XRROutputInfo *output = XRRGetOutputInfo (display, res, crtc->outputs[0]);
                if (preferred && strcmp(preferred, output->name) == 0) {
                    dw = crtc->width;
                    dh = crtc->height;
                    dx = crtc->x;
                    dy = crtc->y;
                    rotation = crtc->rotation;
                }
            }
        }
    }
}

int main(int argc, char *argv[])
{
    Display *display = XOpenDisplay(NULL);

    if (display == NULL) {
        fprintf(stderr, "Unable to connect to X server\n");
        return EXIT_FAILURE;
    }

    search_touchscreen_device(display);
    get_display_info(display);

    if (touch_screen) {
        printf("Touchscreen: '%s'\n", touch_screen);
    }

    printf("screen: %dx%d, display: %dx%d (%d,%d), preferred: %dx%d (%s)", sw, sh, dw, dh, dx, dy, pw, ph, preferred);
    printf(", scaling mode:");
    switch (scaling_mode) {
        case ScalingMode_None:
            printf(" 'None'");
            break;
        case ScalingMode_Full:
            printf(" 'Full'");
            break;
        case ScalingMode_Center:
            printf(" 'Center'");
            break;
        case ScalingMode_Full_aspect:
            printf(" 'Full aspect'");
            break;
    }
    if (rotation & RR_Rotate_0) printf(" RR_Rotate_0");
    if (rotation & RR_Rotate_90) printf(" RR_Rotate_90");
    if (rotation & RR_Rotate_180) printf(" RR_Rotate_180");
    if (rotation & RR_Rotate_270) printf(" RR_Rotate_270");
    if (rotation & RR_Reflect_X) printf(" RR_Reflect_X");
    if (rotation & RR_Reflect_Y) printf(" RR_Reflect_Y");
    printf("\n");

    XSync(display, False);
    XCloseDisplay(display);

    return EXIT_SUCCESS;
}

/* vim:set fileencodings=utf-8 tabstop=4 expandtab shiftwidth=4 softtabstop=4: */
