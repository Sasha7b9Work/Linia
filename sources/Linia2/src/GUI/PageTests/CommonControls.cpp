// 2026/10/08 12:24:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "GUI/PageTests/CommonControls.h"
#include "GUI/PageTests/Entities/MeasurerSourcer.h"
#include "GUI/PageTests/PanelViewTest.h"


void CommonControls::Create(wxWindow *parent)
{
    commutator = new Commutator(parent, { 10, 40 }, 171);

    int width = MeasurerSourcer::WIDTH_CONTROL;

#define CREATE_BUTTONS_COMBO(name, parent, title, num, func, _x, _y)                                        \
    name = new ButtonsCombo(parent, title, width, titles, tooltips, num, #name, ButtonsCombo::Type::Text);  \
    name->Bind(wxEVT_COMBOBOX, &CommonControls::func, this);                                                 \
    name->SetPosition( {_x, _y} );

    {
        boxScan = new StaticBox(parent, L("Развёртка"), { width + 20, 100 });

        boxScan->SetPosition({ 200, 40 });

        if (!bcScanMode)
        {
            wxArrayString titles;
            titles.push_back("SIN+");
            titles.push_back("SIN-");
            titles.push_back("AC");
            titles.push_back("DC-");
            titles.push_back("DC+");
            titles.push_back("IMP+");
            titles.push_back("IMP-");
            titles.push_back("IMP_CVC");

            wxArrayString tooltips;
            tooltips.push_back("");

            CREATE_BUTTONS_COMBO(bcScanMode, boxScan, L("Тип"), 1, OnChangedScanMode, 10, SD::Y_SB(20));
        }

        if (!bcScanNumberPoints)
        {
            wxArrayString titles =
            {
                "20",
                "50",
                "100",
                "200"
            };

            wxArrayString tooltips =
            {
                ""
            };

            CREATE_BUTTONS_COMBO(bcScanNumberPoints, boxScan, L("Кол-во точек"), 1, OnChangedScanNumberPoints, 10, SD::Y_SB(20 + 30));
        }
    }

    {
        boxCover = new StaticBox(parent, L("Крышка"), { width + 20, 100 });

        boxCover->SetPosition({ 400, 40 });

        txtCover = new StaticText(boxCover, L(""), { boxCover->GetSize().x - 20, boxCover->GetSize().y - 30 }, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);

        txtCover->SetPosition({ 10, SD::Y_SB(20) });

        wxFont font = txtCover->GetFont();
        font.SetPointSize(23);
        txtCover->SetFont(font);

        OpenCover();
    }

    {
        CreateButton(&btnEditSave, parent, L("Сохранить"), { 600, 40 }, bcScanNumberPoints->GetSize(), [this](wxCommandEvent &)
            {
                if (InModeEdit())
                {
                    btnEditSave->Hide();
                    btnEditExit->Hide();
                }
                else
                {

                }
            });

        btnEditSave->Hide();

        CreateButton(&btnEditExit, parent, L("Выход"), { btnEditSave->GetPosition().x, 80 }, btnEditSave->GetSize(), [this](wxCommandEvent &)
            {
                if (InModeEdit())
                {
                    btnEditSave->Hide();
                    btnEditExit->Hide();
                }
                else
                {

                }
            });

        btnEditExit->Hide();
    }

#undef CREATE_BUTTONS_COMBO
}


void CommonControls::Refresh()
{
    commutator->Refresh();
}


void CommonControls::Show()
{
    commutator->Show();
    boxScan->Show();
    boxCover->Show();
}


void CommonControls::Hide()
{
    commutator->Hide();
    boxScan->Hide();
    boxCover->Hide();
}


void CommonControls::CreateButton(Button **btn, wxWindow *parent,
    const wxString &label, const wxPoint &pos,
    const wxSize &size, std::function<void(wxCommandEvent &)> onClick)
{
    *btn = new Button(parent, label, size);
    (*btn)->SetPosition(pos);
    (*btn)->Bind(wxEVT_BUTTON, [onClick](wxCommandEvent &event)
        {
            onClick(event);
        });
}


void CommonControls::OnChangedScanMode(wxCommandEvent &)
{

}


void CommonControls::OnChangedScanNumberPoints(wxCommandEvent &)
{

}


void CommonControls::OpenCover()
{
    cover_is_opened = true;

    txtCover->SetLabel(L("Открыта"));

    txtCover->SetBackgroundColour(*wxRED);

    txtCover->Refresh();
}


void CommonControls::CloseCover()
{
    cover_is_opened = false;

    txtCover->SetLabel(L("Закрыта"));

    txtCover->SetBackgroundColour(PanelViewTest::self->GetBackgroundColour());

    txtCover->Refresh();
}


bool CommonControls::InModeEdit() const
{
    return !btnEditSave->IsShown();
}
