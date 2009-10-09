/* ASE - Allegro Sprite Editor
 * Copyright (C) 2001-2009  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

#include <allegro/unicode.h>

#include "jinete/jinete.h"

#include "commands/command.h"
#include "modules/gui.h"
#include "raster/image.h"
#include "raster/mask.h"
#include "raster/sprite.h"
#include "sprite_wrappers.h"
#include "undoable.h"
#include "widgets/colbar.h"

class CanvasSizeCommand : public Command
{
  int m_left, m_right, m_top, m_bottom;

public:
  CanvasSizeCommand();
  Command* clone() const { return new CanvasSizeCommand(*this); }

protected:
  bool enabled(Context* context);
  void execute(Context* context);
};

CanvasSizeCommand::CanvasSizeCommand()
  : Command("canvas_size",
	    "Canvas Size",
	    CmdRecordableFlag)
{
  m_left = m_right = m_top = m_bottom = 0;
}

bool CanvasSizeCommand::enabled(Context* context)
{
  const CurrentSpriteReader sprite(context);
  return sprite != NULL;
}

void CanvasSizeCommand::execute(Context* context)
{
  CurrentSpriteWriter sprite(context);

  if (context->is_ui_available()) {
    JWidget left, top, right, bottom, ok;

    // load the window widget
    JWidgetPtr window(load_widget("canvas.jid", "canvas_size"));
    get_widgets(window,
		"left", &left,
		"top", &top,
		"right", &right,
		"bottom", &bottom,
		"ok", &ok, NULL);

    jwindow_remap(window);
    jwindow_center(window);

    left->textf("%d", m_left);
    right->textf("%d", m_right);
    top->textf("%d", m_top);
    bottom->textf("%d", m_bottom);

    load_window_pos(window, "CanvasSize");
    jwidget_show(window);
    jwindow_open_fg(window);
    save_window_pos(window, "CanvasSize");

    if (jwindow_get_killer(window) != ok)
      return;

    m_left = left->text_int();
    m_right = right->text_int();
    m_top = top->text_int();
    m_bottom = bottom->text_int();
  }

  // resize canvas

  int x1 = -m_left;
  int y1 = -m_top;
  int x2 = sprite->w + m_right;
  int y2 = sprite->h + m_bottom;

  if (x2 <= x1) x2 = x1+1;
  if (y2 <= y1) y2 = y1+1;

  {
    Undoable undoable(sprite, "Canvas Size");
    int bgcolor = context->get_bg_color();
    undoable.crop_sprite(x1, y1, x2-x1, y2-y1, bgcolor);
    undoable.commit();
  }
  sprite_generate_mask_boundaries(sprite);
  update_screen_for_sprite(sprite);
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::create_canvas_size_command()
{
  return new CanvasSizeCommand;
}