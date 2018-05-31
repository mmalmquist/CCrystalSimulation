#ifndef CRYSTAL_VIEW_H_
#define CRYSTAL_VIEW_H_

#include <gtk/gtk.h>
#include "CrystalModel.h"

#define WEAK_REF

typedef struct crystal_view_t CrystalView;

CrystalView *
CrystalView_create(WEAK_REF CrystalModel *weak_cm);
void
CrystalView_destroy(CrystalView *self);
void
CrystalView_repaint(CrystalView *self);
void
CrystalView_set_gtk_widget(CrystalView *self,
			   GtkWidget *widget);

#endif //CRYSTAL_VIEW_H_
