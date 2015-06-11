/*                                              -*- mode:C++ -*-
  AssetManager.h SDL_Surface management system
  Copyright (C) 2014 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
  USA
*/

/**
  \file AssetManager.h SDL_Surface management system
  \author Trent R. Small.
  \author David H. Ackley.
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "SDL_image.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "Logger.h"
#include "Utils.h"
#include "Point.h"

namespace MFM
{
  enum ImageAsset
  {
    IMAGE_ASSET_SELECTOR_ICON = 0,
    IMAGE_ASSET_ATOM_SELECTOR_ICON,
    IMAGE_ASSET_PENCIL_ICON,
    IMAGE_ASSET_ERASER_ICON,
    IMAGE_ASSET_BRUSH_ICON,
    IMAGE_ASSET_BUCKET_ICON,
    IMAGE_ASSET_XRAY_ICON,
    IMAGE_ASSET_CLONE_ICON,
    IMAGE_ASSET_AIRBRUSH_ICON,
    IMAGE_ASSET_CHECKBOX_ICON_ON,
    IMAGE_ASSET_CHECKBOX_ICON_OFF,
    IMAGE_ASSET_SLIDER_HANDLE,
    IMAGE_ASSET_SELECTOR_ICON_BIG,
    IMAGE_ASSET_ATOM_SELECTOR_ICON_BIG,
    IMAGE_ASSET_PENCIL_ICON_BIG,
    IMAGE_ASSET_ERASER_ICON_BIG,
    IMAGE_ASSET_BRUSH_ICON_BIG,
    IMAGE_ASSET_BUCKET_ICON_BIG,
    IMAGE_ASSET_XRAY_ICON_BIG,
    IMAGE_ASSET_CLONE_ICON_BIG,
    IMAGE_ASSET_AIRBRUSH_ICON_BIG,
    IMAGE_ASSET_CLOSE_WINDOW_ICON,

    // MUST REMAIN LAST
    IMAGE_ASSET_NONE,
    IMAGE_ASSET_COUNT = IMAGE_ASSET_NONE
  };

  enum FontAsset
  {
    FONT_ASSET_ELEMENT = 0,
    FONT_ASSET_ELEMENT_BIG,
    FONT_ASSET_HELPPANEL_BIG,
    FONT_ASSET_HELPPANEL_SMALL,
    FONT_ASSET_LOGGER,
    FONT_ASSET_LABEL,
    FONT_ASSET_BUTTON_SMALL,
    FONT_ASSET_BUTTON_MEDIUM,
    FONT_ASSET_BUTTON = FONT_ASSET_BUTTON_MEDIUM,
    FONT_ASSET_BUTTON_BIG,

    // MUST REMAIN LAST
    FONT_ASSET_NONE,
    FONT_ASSET_COUNT=FONT_ASSET_NONE
  };

  class AssetManager
  {
  private:

    static SDL_Surface* surfaces[IMAGE_ASSET_COUNT];

    static TTF_Font* fonts[FONT_ASSET_COUNT];

    static bool initialized;

    static SDL_Surface* LoadImage(const char* relativeFilename)
    {
      const u32 BUFFER_SIZE = 1024;
      char filename[BUFFER_SIZE];
      SDL_Surface* loaded = NULL;
      SDL_Surface* opped = NULL;

      if(Utils::GetReadableResourceFile(relativeFilename, filename, BUFFER_SIZE))
      {
        loaded = IMG_Load(filename);

        if(loaded)
        {
          opped = SDL_DisplayFormatAlpha(loaded);

          SDL_FreeSurface(loaded);

          LOG.Debug("Surface %s loaded: %p, opped: %p", filename, loaded, opped);
        }
        else
        {
          LOG.Error("Image %s not loaded : %s",
                    filename,
                    IMG_GetError());
        }
      }
      else
      {
        LOG.Error("Cannot compute relative path to: %s", relativeFilename);
      }

      return opped;
    }

    static TTF_Font* LoadFont(const char* relativePath, u32 size)
    {
      const u32 BUFFER_SIZE = 1024;
      TTF_Font* font = NULL;
      char path[BUFFER_SIZE];

      if(Utils::GetReadableResourceFile(relativePath, path, BUFFER_SIZE))
      {
        font = TTF_OpenFont(path, size);

        if(!font)
        {
          LOG.Error("Font %s not loaded: %s",
                    path,
                    TTF_GetError());
        }

        LOG.Debug("Font %s loaded: %p", relativePath, font);
      }
      else
      {
        LOG.Error("Cannot compute relative path to: %s", relativePath);
      }

      return font;
    }

  public:

    static bool IsInitialized()
    {
      return initialized;
    }

    /**
     * Initializes all held Assets. This should only be called once a
     * screen has been created by SDL_SetVideoMode .
     */
    static void Initialize()
    {
      if(!initialized)
      {
        surfaces[IMAGE_ASSET_SELECTOR_ICON] = LoadImage("images/selector_icon.png");
        surfaces[IMAGE_ASSET_ATOM_SELECTOR_ICON] = LoadImage("images/atom_selector_icon.png");
        surfaces[IMAGE_ASSET_PENCIL_ICON] = LoadImage("images/pencil_icon.png");
        surfaces[IMAGE_ASSET_ERASER_ICON] = LoadImage("images/eraser_icon.png");
        surfaces[IMAGE_ASSET_BRUSH_ICON] = LoadImage("images/brush_icon.png");
        surfaces[IMAGE_ASSET_BUCKET_ICON] = LoadImage("images/bucket_icon.png");
        surfaces[IMAGE_ASSET_XRAY_ICON] = LoadImage("images/xray_icon.png");
        surfaces[IMAGE_ASSET_CLONE_ICON] = LoadImage("images/clone_icon.png");
        surfaces[IMAGE_ASSET_AIRBRUSH_ICON] = LoadImage("images/airbrush_icon.png");
        surfaces[IMAGE_ASSET_CHECKBOX_ICON_ON] = LoadImage("images/checkbox_on.png");
        surfaces[IMAGE_ASSET_CHECKBOX_ICON_OFF] = LoadImage("images/checkbox_off.png");
        surfaces[IMAGE_ASSET_SLIDER_HANDLE] = LoadImage("images/slider_handle.png");
        surfaces[IMAGE_ASSET_SELECTOR_ICON_BIG] = LoadImage("images/selector_icon_big.png");
        surfaces[IMAGE_ASSET_ATOM_SELECTOR_ICON_BIG] = LoadImage("images/atom_selector_icon_big.png");
        surfaces[IMAGE_ASSET_PENCIL_ICON_BIG] = LoadImage("images/pencil_icon_big.png");
        surfaces[IMAGE_ASSET_ERASER_ICON_BIG] = LoadImage("images/eraser_icon_big.png");
        surfaces[IMAGE_ASSET_BRUSH_ICON_BIG] = LoadImage("images/brush_icon_big.png");
        surfaces[IMAGE_ASSET_BUCKET_ICON_BIG] = LoadImage("images/bucket_icon_big.png");
        surfaces[IMAGE_ASSET_XRAY_ICON_BIG] = LoadImage("images/xray_icon_big.png");
        surfaces[IMAGE_ASSET_CLONE_ICON_BIG] = LoadImage("images/clone_icon_big.png");
        surfaces[IMAGE_ASSET_AIRBRUSH_ICON_BIG] = LoadImage("images/airbrush_icon_big.png");
        surfaces[IMAGE_ASSET_CLOSE_WINDOW_ICON] = LoadImage("images/close_window_icon.png");

        const char * FIX_FONT = "fonts/Inconsolata.ttf";
        const char * PRO_FONT = "fonts/Mate-Regular.ttf";
        const char * PSC_FONT = "fonts/MateSC-Regular.ttf";

        fonts[FONT_ASSET_ELEMENT] = LoadFont(FIX_FONT, 26);
        fonts[FONT_ASSET_ELEMENT_BIG] = LoadFont(FIX_FONT, 40);
        fonts[FONT_ASSET_HELPPANEL_BIG] = LoadFont(PSC_FONT, 26);
        fonts[FONT_ASSET_HELPPANEL_SMALL] = LoadFont(PRO_FONT, 22);
        fonts[FONT_ASSET_LOGGER] = LoadFont(FIX_FONT, 14);
        fonts[FONT_ASSET_LABEL] = LoadFont(PSC_FONT, 20);
        fonts[FONT_ASSET_BUTTON_SMALL] = LoadFont(PSC_FONT, 16);
        fonts[FONT_ASSET_BUTTON_MEDIUM] = LoadFont(PSC_FONT, 20);
        fonts[FONT_ASSET_BUTTON_BIG] = LoadFont(PSC_FONT, 24);

        initialized = true;
      }
    }

    /**
     * Frees all held assets, which are subsequently set to NULL.
     */
    static void Destroy()
    {
      if(initialized)
      {
        for(ImageAsset i = (ImageAsset) 0; i < IMAGE_ASSET_COUNT; i = (ImageAsset) (i + 1))
        {
          SDL_FreeSurface(surfaces[i]);
          surfaces[i] = NULL;
        }

        for(FontAsset i = (FontAsset) 0; i < FONT_ASSET_COUNT; i = (FontAsset) (i + 1))
        {
          TTF_CloseFont(fonts[i]);
          fonts[i] = NULL;
        }

        initialized = false;
      }
    }

    static UPoint GetSize(ImageAsset a)
    {
      SDL_Surface * s = GetReal(a);
      return UPoint(s->w, s->h);
    }

    static SDL_Surface* Get(ImageAsset a)
    {
      if (!initialized) FAIL(ILLEGAL_STATE);
      if (a >= IMAGE_ASSET_NONE) return 0;
      return surfaces[a];
    }

    static SDL_Surface* GetReal(ImageAsset a)
    {
      SDL_Surface* s = Get(a);
      if (!s) FAIL(ILLEGAL_ARGUMENT);
      return s;
    }

    static TTF_Font* Get(FontAsset a)
    {
      if (!initialized) FAIL(ILLEGAL_STATE);
      if (a >= FONT_ASSET_NONE) return 0;
      return fonts[a];
    }

    static TTF_Font* GetReal(FontAsset a)
    {
      TTF_Font * f = Get(a);
      if (!f) FAIL(ILLEGAL_ARGUMENT);
      return f;
    }
  };
}

#endif /* ASSETMANAGER_H */
