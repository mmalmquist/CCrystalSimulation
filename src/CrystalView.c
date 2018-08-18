#include "CrystalView.h"

#include <math.h>
#include <stdlib.h>

#include "Point.h"

struct crystal_view_t
{
  CrystalModel *_cm;
  cairo_surface_t *_surface;
  GtkWidget *_canvas;

  cairo_t *_cr;
  unsigned _width;
  unsigned _height;

  int _scale_required;
  double _x_scale;
  double _y_scale;
};

static void
draw_background(CrystalView *self);
static void
draw_edge(CrystalView *self);
static void
draw_crystal(CrystalView *self);
static gboolean
draw_cb(GtkWidget *widget,
	cairo_t *cr,
	gpointer data);
static gboolean
configure_event_cb(GtkWidget *widget,
		   GdkEventConfigure *event,
		   gpointer data);

CrystalView *
CrystalView_create(CrystalModel *cm)
{
  CrystalView *self = (CrystalView *)calloc(1, sizeof(CrystalView));
  self->_cm = cm;
  
  self->_width = CrystalModel_get_bath_width(self->_cm);
  self->_height = CrystalModel_get_bath_width(self->_cm);
  
  return self;
}

void
CrystalView_destroy(CrystalView *self)
{
  if (!self) { return; }

  cairo_destroy(self->_cr);
  cairo_surface_destroy(self->_surface);
  free(self);
}

void
CrystalView_repaint(CrystalView *self)
{
  gtk_widget_queue_draw(self->_canvas);
}

void
CrystalView_set_gtk_widget(CrystalView *self, GtkWidget *widget)
{
  self->_canvas = widget;
  
  gtk_widget_set_size_request(self->_canvas,
			      CrystalModel_get_bath_width(self->_cm),
			      CrystalModel_get_bath_width(self->_cm));
  g_signal_connect(self->_canvas, "draw", G_CALLBACK(draw_cb), self);
  g_signal_connect(self->_canvas, "configure-event", G_CALLBACK(configure_event_cb), self);
}

static void
draw_background(CrystalView *self)
{
  cairo_set_source_rgb(self->_cr, 0, 0, 0);
  cairo_paint(self->_cr);
}

static void
draw_edge(CrystalView *self)
{
  Point p = { CrystalModel_x_bath_to_model_rep(self->_cm, 0),
	      CrystalModel_y_bath_to_model_rep(self->_cm, 0) };
  cairo_set_line_width(self->_cr, 1);
  
  cairo_arc(self->_cr, p.x, p.y, CrystalModel_get_radius(self->_cm), 0, 2 * M_PI);
  cairo_set_source_rgb(self->_cr, 0, 0, 1);
  cairo_stroke(self->_cr);
  
  cairo_arc(self->_cr, p.x, p.y, CrystalModel_get_r_bounds(self->_cm), 0, 2 * M_PI);
  cairo_set_source_rgb(self->_cr, 1, 1, 0);
  cairo_stroke(self->_cr);
}

static void
draw_crystal(CrystalView *self)
{
  int i, j;
  int const x_l = CrystalModel_get_x(self->_cm), y_l = CrystalModel_get_y(self->_cm);
  long const r = CrystalModel_get_radius(self->_cm);
  
  cairo_set_line_width(self->_cr, 0);
  
  for(i = -r; i <= r; i++) {
    for(j = -r; j <= r; j++) {
      if (CrystalModel_get_model_value(self->_cm, i, j)) {
	cairo_rectangle(self->_cr,
			CrystalModel_x_bath_to_model_rep(self->_cm, i),
			CrystalModel_y_bath_to_model_rep(self->_cm, j),
			1, 1);
      }
    }
  }
  cairo_set_source_rgb(self->_cr, 1, 0, 0);
  cairo_fill(self->_cr);
        
  cairo_rectangle(self->_cr,
		  CrystalModel_x_bath_to_model_rep(self->_cm, x_l),
		  CrystalModel_y_bath_to_model_rep(self->_cm, y_l),
		  1, 1);
  cairo_set_source_rgb(self->_cr, 0, 1, 0);
  cairo_fill(self->_cr);
}

static gboolean
draw_cb(GtkWidget *widget,
	cairo_t *cr,
	gpointer data)
{
  CrystalView *self;
  (void)widget;
  
  if (!data) {
    return FALSE;
  }
  self = (CrystalView *)data;
  draw_background(self);
  draw_edge(self);
  draw_crystal(self);
  
  if (self->_scale_required) {
    cairo_scale(cr, self->_x_scale, self->_y_scale);
  }
  cairo_set_source_surface(cr, self->_surface, 0, 0);
  cairo_paint(cr);
  
  return FALSE;
}

static gboolean
configure_event_cb(GtkWidget *widget,
		   GdkEventConfigure *event,
		   gpointer data)
{
  CrystalView *self;
  unsigned width, height;
  (void)event;
  
  if (!data) {
    return FALSE;
  }
  self = (CrystalView *)data;
  
  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);
  self->_scale_required = (width != self->_width || height != self->_height);
  self->_x_scale = (double) width / (double) self->_width;
  self->_y_scale = (double) height / (double) self->_height;

  if (self->_surface) {
    return TRUE;
  }
  
  self->_surface = gdk_window_create_similar_surface(gtk_widget_get_window(self->_canvas),
						      CAIRO_CONTENT_COLOR,
						      self->_width,
						      self->_height);
  self->_cr = cairo_create(self->_surface);
  return TRUE;
}
