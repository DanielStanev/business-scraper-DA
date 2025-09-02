#ifndef STYLE_MANAGER_H
#define STYLE_MANAGER_H

#include <QString>

class StyleManager
{
public:
    static QString getProfessionalStylesheet();
    static void applyProfessionalStyle(class QWidget* widget);

private:
    // Professional color palette
    static const QString BACKGROUND_COLOR;      // #FAFAFA (very light gray)
    static const QString HEADER_COLOR;          // #E7E7E7 (light gray)
    static const QString BORDER_COLOR;          // #D0D0D0 (medium gray)
    static const QString ACCENT_COLOR;          // #217346 (professional green)
    static const QString HOVER_COLOR;           // #F5F5F5 (hover gray)
    static const QString SELECTED_COLOR;        // #CCE5F0 (light blue)
    static const QString TEXT_COLOR;            // #333333 (dark gray)
    static const QString BUTTON_COLOR;          // #FFFFFF (white)
    static const QString GRID_COLOR;            // #E1E1E1 (grid gray)
};

#endif // STYLE_MANAGER_H
