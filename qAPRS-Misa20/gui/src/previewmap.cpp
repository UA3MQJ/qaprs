#include "include/previewmap.h"

PreviewMapDialog::PreviewMapDialog()
{

    setupUi(this);

    show();

    connect( loadButton, SIGNAL( clicked(  ) ),
                                      this, SLOT( loadClick( ) ) );

};


PreviewMapDialog::~PreviewMapDialog()
{

}

void PreviewMapDialog::loadClick( ) {

    close();

}



