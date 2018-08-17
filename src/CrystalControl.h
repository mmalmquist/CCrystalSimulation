#ifndef CRYSTALCONTROL_H_
#define CRYSTALCONTROL_H_

#include <gtk/gtk.h>
#include "CrystalModel.h"
#include "CrystalView.h"

typedef struct crystal_control_t CrystalControl;

extern CrystalControl *
CrystalControl_create(CrystalModel *cm,
		      CrystalView *cv);
extern void
CrystalControl_destroy(CrystalControl *self);
extern void
CrystalControl_init_ui(CrystalControl *self,
		       GtkBuilder *builder);

#endif //CRYSTALCONTROL_H_
