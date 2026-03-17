// 2025/7/12 11:08:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Panels/PanelMenu/MenuDialog.h"


class DialogModels : public MenuDialog
{
public:
 
    DialogModels();

    virtual ~DialogModels() { }

    static DialogModels *self;

    virtual void Update() override;

private:

    static const pchar BTN_NEW;
    static const pchar BTN_OPEN;
    static const pchar BTN_CLOSE;
    static const pchar BTN_SAVE;
    static const pchar BTN_SAVE_AS;
    static const pchar BTN_DELETE;
};
