#ifndef CRYSTAL_VIEW_H_
#define CRYSTAL_VIEW_H_

#include <gtk/gtk.h>
#include "CrystalModel.h"

typedef struct crystal_view_t CrystalView;

extern CrystalView *
CrystalView_create(CrystalModel *cm);
extern void
CrystalView_destroy(CrystalView *self);
extern void
CrystalView_repaint(CrystalView *self);
extern void
CrystalView_set_gtk_widget(CrystalView *self,
			   GtkWidget *widget);

#endif //CRYSTAL_VIEW_H_
