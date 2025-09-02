#include "gui/StyleManager.h"
#include <QtWidgets/QWidget>

// Professional color definitions
const QString StyleManager::BACKGROUND_COLOR = "#F8F8F8";
const QString StyleManager::HEADER_COLOR = "#E7E7E7";
const QString StyleManager::BORDER_COLOR = "#D0D0D0";
const QString StyleManager::ACCENT_COLOR = "#217346";
const QString StyleManager::HOVER_COLOR = "#F2F2F2";
const QString StyleManager::SELECTED_COLOR = "#CCE5F0";
const QString StyleManager::TEXT_COLOR = "#333333";
const QString StyleManager::BUTTON_COLOR = "#F5F5F5";
const QString StyleManager::GRID_COLOR = "#E1E1E1";

QString StyleManager::getProfessionalStylesheet()
{
    return QString(R"(
        /* Main Application Style */
        QMainWindow {
            background-color: %1;
            color: %2;
        }

        /* Menu Bar */
        QMenuBar {
            background-color: %1;
            border-bottom: 1px solid %3;
            padding: 2px;
            color: %7;
        }

        QMenuBar::item {
            background-color: transparent;
            padding: 4px 8px;
            margin: 1px;
            color: %7;
        }

        QMenuBar::item:selected {
            background-color: %5;
            border: 1px solid %3;
            border-radius: 3px;
            color: %7;
        }

        QMenu {
            background-color: %8;
            border: 1px solid %3;
            border-radius: 3px;
            padding: 2px;
            color: %7;
        }

        QMenu::item {
            padding: 6px 20px;
            margin: 1px;
            color: %7;
        }

        QMenu::item:selected {
            background-color: %6;
            border-radius: 2px;
            color: %7;
        }

        /* Status Bar */
        QStatusBar {
            background-color: %2;
            border-top: 1px solid %3;
            color: %7;
            padding: 2px;
        }

        /* Buttons */
        QPushButton {
            background-color: %8;
            border: 1px solid %3;
            border-radius: 3px;
            padding: 6px 16px;
            color: %7;
            font-weight: 500;
            min-width: 80px;
        }

        QPushButton:hover {
            background-color: %5;
            border-color: #B0B0B0;
        }

        QPushButton:pressed {
            background-color: %6;
            border-color: %4;
        }

        QPushButton:default {
            background-color: %4;
            color: white;
            border-color: %4;
        }

        QPushButton:default:hover {
            background-color: #1B5E3F;
        }

        /* Input Fields */
        QLineEdit, QTextEdit, QSpinBox, QDoubleSpinBox, QComboBox {
            background-color: %8;
            border: 1px solid %3;
            border-radius: 2px;
            padding: 4px 8px;
            color: %7;
            selection-background-color: %6;
            selection-color: %7;
        }

        QLineEdit:focus, QTextEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus, QComboBox:focus {
            border: 2px solid %4;
            border-radius: 2px;
            selection-background-color: %6;
            selection-color: %7;
        }

        QComboBox::drop-down {
            border: none;
            width: 20px;
        }

        QComboBox::down-arrow {
            image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTAiIGhlaWdodD0iNiIgdmlld0JveD0iMCAwIDEwIDYiIGZpbGw9Im5vbmUiIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyI+CjxwYXRoIGQ9Ik0xIDFMNSA1TDkgMSIgc3Ryb2tlPSIjNjY2NjY2IiBzdHJva2Utd2lkdGg9IjIiIHN0cm9rZS1saW5lY2FwPSJyb3VuZCIgc3Ryb2tlLWxpbmVqb2luPSJyb3VuZCIvPgo8L3N2Zz4K);
        }

        /* ComboBox dropdown list */
        QComboBox QAbstractItemView {
            background-color: %8;
            border: 1px solid %3;
            border-radius: 2px;
            color: %7;
            selection-background-color: %6;
            selection-color: %7;
            outline: none;
        }

        QComboBox QAbstractItemView::item {
            padding: 4px 8px;
            color: %7;
            background-color: %8;
        }

        QComboBox QAbstractItemView::item:selected {
            background-color: %6;
            color: %7;
        }

        QComboBox QAbstractItemView::item:hover {
            background-color: %5;
            color: %7;
        }

        /* Checkboxes */
        QCheckBox {
            color: %7;
            background-color: transparent;
            spacing: 8px;
            outline: none;
        }

        QCheckBox::indicator {
            width: 16px;
            height: 16px;
            border: 1px solid %3;
            border-radius: 2px;
            background-color: %8;
        }

        QCheckBox::indicator:hover {
            border-color: %4;
            background-color: %5;
        }

        QCheckBox::indicator:checked {
            background-color: %4;
            border-color: %4;
            width: 16px;
            height: 16px;
            image: url(data:image/svg+xml;base64,PHN2ZyB3aWR0aD0iMTIiIGhlaWdodD0iMTIiIHZpZXdCb3g9IjAgMCAxMiAxMiIgZmlsbD0ibm9uZSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIj4KPHBhdGggZD0iTTIgNkw0LjUgOC41TDEwIDMiIHN0cm9rZT0id2hpdGUiIHN0cm9rZS13aWR0aD0iMiIgc3Ryb2tlLWxpbmVjYXA9InJvdW5kIiBzdHJva2UtbGluZWpvaW49InJvdW5kIi8+Cjwvc3ZnPgo=);
        }

        QCheckBox::indicator:checked:hover {
            background-color: #1B5E3F;
        }

        QCheckBox::indicator:disabled {
            border-color: #CCCCCC;
            background-color: #F0F0F0;
        }

        QCheckBox:disabled {
            color: #999999;
        }

        /* Table Widget (Professional) */
        QTableWidget {
            background-color: %8;
            alternate-background-color: %1;
            gridline-color: %9;
            border: 1px solid %3;
            selection-background-color: %6;
            selection-color: %7;
        }

        /* Table viewport - the area where table content is displayed */
        QTableWidget::viewport {
            background-color: %8;
        }

        /* Ensure the empty area below rows has proper background */
        QTableWidget QAbstractScrollArea::corner {
            background-color: %2;
        }

        QTableWidget::item {
            border: none;
            padding: 4px 8px;
            color: %7;
        }

        QTableWidget::item:selected {
            background-color: %6 !important;
            color: %7 !important;
        }

        QTableWidget::item:selected:focus {
            background-color: %6 !important;
            color: %7 !important;
        }

        QTableWidget::item:hover {
            background-color: %5;
        }

        /* Table cell editor styling - makes edit box fill the entire cell */
        QTableWidget QLineEdit {
            border: none !important;
            border-radius: 0px !important;
            background-color: %8 !important;
            color: %7 !important;
            padding: 0px !important;
            margin: 0px !important;
            selection-background-color: %6 !important;
            selection-color: %7 !important;
            min-height: 100% !important;
            max-height: 100% !important;
        }

        QTableWidget QLineEdit:focus {
            border: none !important;
            border-radius: 0px !important;
            background-color: %8 !important;
            color: %7 !important;
            padding: 0px !important;
        }

        /* Alternative approach - target the item widget directly */
        QTableWidget QWidget {
            border: none !important;
            margin: 0px !important;
            padding: 0px !important;
        }

        /* Target any input widget inside table cells */
        QTableWidget * {
            border: none !important;
            margin: 0px !important;
        }

        QHeaderView::section {
            background-color: %2;
            border: 1px solid %3;
            border-left: none;
            border-top: none;
            padding: 6px 8px;
            color: %7;
            font-weight: 600;
        }

        QHeaderView::section:hover {
            background-color: #DDDDDD;
        }

        QHeaderView::section:pressed {
            background-color: #CCCCCC;
        }

        /* Specific styling for vertical header (row numbers) */
        QTableWidget QHeaderView::section:vertical {
            background-color: %2;
            border: 1px solid %3;
            border-top: none;
            border-left: none;
            color: %7;
            text-align: center;
            min-width: 40px;
        }

        /* Vertical header background - fixes dark area below row numbers */
        QTableWidget > QHeaderView {
            background-color: %2;
        }

        QTableWidget > QHeaderView::viewport {
            background-color: %2;
        }

        /* Corner button (top-left of table) */
        QTableWidget QTableCornerButton::section {
            background-color: %2;
            border: 1px solid %3;
            border-top: none;
            border-left: none;
        }

        /* Splitter */
        QSplitter::handle {
            background-color: %3;
            border: 1px solid %3;
        }

        QSplitter::handle:horizontal {
            width: 6px;
            border-left: 1px solid %3;
            border-right: 1px solid %3;
        }

        QSplitter::handle:vertical {
            height: 6px;
            border-top: 1px solid %3;
            border-bottom: 1px solid %3;
        }

        /* Group Box */
        QGroupBox {
            border: 1px solid %3;
            border-radius: 4px;
            margin-top: 10px;
            padding-top: 8px;
            color: %7;
            font-weight: 600;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 8px;
            padding: 0 8px 0 8px;
            background-color: %1;
        }

        /* Labels */
        QLabel {
            color: %7;
            background-color: transparent;
        }

        /* Progress Bar */
        QProgressBar {
            border: 1px solid %3;
            border-radius: 3px;
            background-color: %1;
            text-align: center;
            color: %7;
        }

        QProgressBar::chunk {
            background-color: %4;
            border-radius: 2px;
        }

        /* Scroll Bars */
        QScrollBar:vertical {
            background-color: %1;
            width: 12px;
            border: none;
        }

        QScrollBar::handle:vertical {
            background-color: %3;
            border-radius: 6px;
            min-height: 20px;
            margin: 2px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: #B0B0B0;
        }

        QScrollBar:horizontal {
            background-color: %1;
            height: 12px;
            border: none;
        }

        QScrollBar::handle:horizontal {
            background-color: %3;
            border-radius: 6px;
            min-width: 20px;
            margin: 2px;
        }

        QScrollBar::handle:horizontal:hover {
            background-color: #B0B0B0;
        }

        QScrollBar::add-line, QScrollBar::sub-line {
            border: none;
            background: none;
        }

        /* Dialog */
        QDialog {
            background-color: %1;
            color: %7;
        }

        /* Tool Tips */
        QToolTip {
            background-color: #FFFFDC;
            border: 1px solid %3;
            color: %7;
            padding: 4px;
            border-radius: 2px;
        }
    )")
    .arg(BACKGROUND_COLOR)     // %1
    .arg(HEADER_COLOR)         // %2
    .arg(BORDER_COLOR)         // %3
    .arg(ACCENT_COLOR)         // %4
    .arg(HOVER_COLOR)          // %5
    .arg(SELECTED_COLOR)       // %6
    .arg(TEXT_COLOR)           // %7
    .arg(BUTTON_COLOR)         // %8
    .arg(GRID_COLOR);          // %9
}

void StyleManager::applyProfessionalStyle(QWidget* widget)
{
    if (widget) {
        widget->setStyleSheet(getProfessionalStylesheet());
    }
}
