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

#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/Xrandr.h>

#include <stdio.h>
#include <stdlib.h>

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
                printf("%s\n", dev->name);
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

    int i, j, k;
    int minWidth, minHeight, maxWidth, maxHeight;

    printf("Total: Crtc %d, Mode %d, Output %d\n", res->ncrtc, res->nmode, res->noutput);

    if (XRRGetScreenSizeRange(display, root, &minWidth, &minHeight, &maxWidth, &maxHeight) == True)
    {
        printf("Screen %d: minimum %d x %d, current %d x %d, maximum %d x %d\n",
                screen,
                minWidth, minHeight,
                DisplayWidth(display, screen),
                DisplayHeight(display, screen),
                maxWidth, maxHeight);
    }

    for (i = 0; i < res->ncrtc; i++)
    {
        XRRCrtcInfo *crtc = XRRGetCrtcInfo (display, res, res->crtcs[i]);
        for (j = 0; j < res->nmode; j++)
        {
            XRRModeInfo mode = res->modes[j];
            if (mode.id == crtc->mode && crtc->noutput == 1) {
                XRROutputInfo *output = XRRGetOutputInfo (display, res, crtc->outputs[0]);
                printf("Current %s %d x %d (%d, %d)\n", output->name, crtc->width, crtc->height, crtc->x, crtc->y);
            }
        }
    }
    for (i = 0; i < res->noutput; i++)
    {
        XRROutputInfo *output = XRRGetOutputInfo (display, res, res->outputs[i]);

        if (output->connection == RR_Connected)
        {
            if (strcmp("LVDS", output->name) == 0)
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
                                printf("Preferred %s %d x %d %s\n", output->name, mode_info->width, mode_info->height, mode_info->name);
                            }
                        }
                    }
                }
            } else if (strcmp("eDP1", output->name) == 0) {
                for (j = 0; j < output->nmode; j++)
                {
                    if (j < output->npreferred)
                    {
                        RRMode *mode = output->modes + j;

                        for (k = 0; k < res->nmode; k++)
                        {
                            XRRModeInfo *mode_info = &res->modes[k];
                            if (mode_info->id == *mode) {
                                printf("Preferred %s %d x %d %s\n", output->name, mode_info->width, mode_info->height, mode_info->name);
                            }
                        }
                    }
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
    XSync(display, False);
    get_display_info(display);
    XSync(display, False);
    XCloseDisplay(display);

    return EXIT_SUCCESS;
}

/* vim:set fileencodings=utf-8 tabstop=4 expandtab shiftwidth=4 softtabstop=4: */
