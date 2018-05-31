#include "CrystalView.h"

#include <math.h>
#include <stdlib.h>

#include "Point.h"
#include "Color.h"

struct crystal_view_t
{
  WEAK_REF CrystalModel *m_cm;
  cairo_surface_t *m_surface;
  GtkWidget *m_canvas;

  cairo_t *m_cr;
  int m_width;
  int m_height;

  bool m_scale_required;
  double m_x_scale;
  double m_y_scale;
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
CrystalView_create(WEAK_REF CrystalModel *weak_cm)
{
  CrystalView *self = (CrystalView *)calloc(1, sizeof(CrystalView));
  self->m_cm = weak_cm;
  
  self->m_width = CrystalModel_get_bath_width(self->m_cm);
  self->m_height = CrystalModel_get_bath_width(self->m_cm);
  
  return self;
}

void
CrystalView_destroy(CrystalView *self)
{
  if (!self) { return; }

  cairo_destroy(self->m_cr);
  cairo_surface_destroy(self->m_surface);
  free(self);
}

void
CrystalView_repaint(CrystalView *self)
{
  gtk_widget_queue_draw(self->m_canvas);
}

void
CrystalView_set_gtk_widget(CrystalView *self, GtkWidget *widget)
{
  self->m_canvas = widget;
  
  gtk_widget_set_size_request(self->m_canvas,
			      CrystalModel_get_bath_width(self->m_cm),
			      CrystalModel_get_bath_width(self->m_cm));
  g_signal_connect(self->m_canvas, "draw", G_CALLBACK(draw_cb), self);
  g_signal_connect(self->m_canvas, "configure-event", G_CALLBACK(configure_event_cb), self);
}

static void
draw_background(CrystalView *self)
{
  cairo_set_source_rgb(self->m_cr, 0, 0, 0);
  cairo_paint(self->m_cr);
}

static void
draw_edge(CrystalView *self)
{
  Point p = {CrystalModel_x_bath_to_model_rep(self->m_cm, 0),
	     CrystalModel_y_bath_to_model_rep(self->m_cm, 0)};
  cairo_set_line_width(self->m_cr, 1);
  
  cairo_arc(self->m_cr, p.x, p.y, CrystalModel_get_radius(self->m_cm), 0, 2 * M_PI);
  cairo_set_source_rgb(self->m_cr, 0, 0, 1);
  cairo_stroke(self->m_cr);
  
  cairo_arc(self->m_cr, p.x, p.y, CrystalModel_get_r_bounds(self->m_cm), 0, 2 * M_PI);
  cairo_set_source_rgb(self->m_cr, 1, 1, 0);
  cairo_stroke(self->m_cr);
}

static void
draw_crystal(CrystalView *self)
{
  int x_l = CrystalModel_get_x(self->m_cm);
  int y_l = CrystalModel_get_y(self->m_cm);
  int r = CrystalModel_get_radius(self->m_cm);
  
  cairo_set_line_width(self->m_cr, 0);
  
  for(int i = -r; i <= r; i++) {
    for(int j = -r; j <= r; j++) {
      if (CrystalModel_get_model_value(self->m_cm, i, j)) {
	cairo_rectangle(self->m_cr,
			CrystalModel_x_bath_to_model_rep(self->m_cm, i),
			CrystalModel_y_bath_to_model_rep(self->m_cm, j),
			1, 1);
      }
    }
  }
  cairo_set_source_rgb(self->m_cr, 1, 0, 0);
  cairo_fill(self->m_cr);
        
  cairo_rectangle(self->m_cr,
		  CrystalModel_x_bath_to_model_rep(self->m_cm, x_l),
		  CrystalModel_y_bath_to_model_rep(self->m_cm, y_l),
		  1, 1);
  cairo_set_source_rgb(self->m_cr, 0, 1, 0);
  cairo_fill(self->m_cr);
}

static gboolean
draw_cb(GtkWidget *widget,
	cairo_t *cr,
	gpointer data)
{
  (void)widget;
  if (!data) {
    return FALSE;
  }
  CrystalView *self = (CrystalView *)data;
  draw_background(self);
  draw_edge(self);
  draw_crystal(self);

  
  if (self->m_scale_required) {
    cairo_scale(cr, self->m_x_scale, self->m_y_scale);
  }
  cairo_set_source_surface(cr, self->m_surface, 0, 0);
  cairo_paint(cr);
  
  return FALSE;
}

static gboolean
configure_event_cb(GtkWidget *widget,
		   GdkEventConfigure *event,
		   gpointer data)
{
  (void)event;
  if (!data) {
    return FALSE;
  }
  CrystalView *self = (CrystalView *)data;
  
  int width = gtk_widget_get_allocated_width(widget);
  int height = gtk_widget_get_allocated_height(widget);
  self->m_scale_required = (width != self->m_width || height != self->m_height);
  self->m_x_scale = (double) width / (double) self->m_width;
  self->m_y_scale = (double) height / (double) self->m_height;

  if (self->m_surface) {
    return TRUE;
  }
  
  self->m_surface = gdk_window_create_similar_surface(gtk_widget_get_window(self->m_canvas),
						      CAIRO_CONTENT_COLOR,
						      self->m_width,
						      self->m_height);
  self->m_cr = cairo_create(self->m_surface);
  return TRUE;
}
