#ifndef WINDOW_H
#define WINDOW_H
#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QMenu>
#include <QAction>
#include <QTextEdit>
#include <QSettings>
#include <QActionGroup>
#include <QPushButton>
#include <QLineEdit>
#include <QMenuBar>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QColorDialog>
#include <QMessageBox>
#include <QTimer>
#include <QScrollArea>
#include <QTextBlockFormat>
#include <QTextBlock>
#include <QClipboard>
#include <QImage>
#include <QMimeData>
#include <QFileDialog>
#include <QImageReader>
#include <QBuffer>
#include <QByteArray>
#include <QVariantMap>
#include <QTextObject>
#include <QTextImageFormat>
#include <QUndoStack>
#include <QTextTable>
#include <QFormLayout>
#include <QSpinBox>
#include <QMouseEvent>
#include <QPrintDialog>
#include <QPrinter>
#include <QScrollBar>
#include <QTextDocument>
#include <QTextDocumentWriter>
#include <QSizeGrip>
#include <QTableWidget>
#include <QPainter>
#include <QLabel>
#include <QComboBox>
#include <QBuffer>
#include <QByteArray>
#include <QTextDocumentFragment>
#include <QToolButton>
class Window : public QWidget {
    Q_OBJECT

public:
 explicit Window(QWidget *parent = nullptr);
    void saveState();
    void restoreState();
    void addNotebook();

protected:
    void saveContentForPage(QTreeWidgetItem* pageItem);
 void closeEvent(QCloseEvent *event) override;
// void resizeEvent(QResizeEvent *event) override;

private slots:
 void addTableRow();
 void addTableColumn();
 void handleCursorPositionChanged();
public slots:
    void showCustomMenu(const QPoint &pos);
signals:

private:
    void setNoteColor(const QString &colorName);
    void setAppColor(const QString &colorName);
    void setToolbarColor(const QString &colorName);
    void setAppFont(const QString &fontName);
    void dumpDataStructure();
    void setUnderlineColor(const QString &colorName);
    QColorDialog *color;
      QColor currentUnderlineColor;
    QColor currentbucketColor;
      QColor currentAppColor;
    QColor currentToolbarColor;
      QColor currentNoteColor;
     int currentfontSize = 12;
    void setUnderlineStyle(const QString &underlineType);
    // Add with your other member variables
    QTextCharFormat codeCharFormat;     // For the default code text color
    QTextBlockFormat codeBlockFormat;

    // NEW: Formats for specific code elements
    QTextCharFormat codeKeywordFormat;  // e.g., for 'int', 'class', 'void'
    QTextCharFormat codeStringFormat;   // e.g., for "hello world"
    QTextCharFormat codeCommentFormat;
    bool contentModified;
    enum UnderlineStyle {
        NoUnderline,
        SingleUnderline,
        DoubleUnderline,
        DottedUnderline,
        WaveUnderline,
        DashUnderline
    };

    enum BorderStyle {
        None,
        Solid,
        Dotted,
        Dashed,
        Double,
        DotDash,
        DotDotDash,
        Groove,
        Ridge,
        Inset,
        Outset
    };
    UnderlineStyle currentUnderlineStyle;
    BorderStyle currentBorderlineStyle;
    struct PageData {
        QString name;
        QString content;
    };
    struct NotebookData {
        QString name;
        QList<PageData> pages;
    };
    QString getPageContent(QTreeWidgetItem *notebook, QTreeWidgetItem *page);
    void setPageContent(QTreeWidgetItem *notebook, QTreeWidgetItem *page, const QString &content);
    void saveNotebookData();
    void restoreNotebookData();
    QMap<QString, QMap<QString, QString>> notebookData; // notebook->page->content
    QTreeWidgetItem* currentPage = nullptr;
    void setupNotebookConnections();
    void savePageContent();
    void loadPageContent(QTreeWidgetItem* page);

    // ... existing members ...
    //void addButton();
    void createactions();
   // void setFont(const QString &fontName);
    void setSize(int size);
   // void setColor(const QString &color);
    void createActions(QMenu *menu, QActionGroup *group, const QStringList &texts, const std::function<void(const QString &)> &slot);
    void setBold();
    void setCursive();
    void setRight();
    void setLeft();
    void setCenter();
    void setIndent();
    void unindentText();
    void setFont(const QString &fontName);
    void setTextColor(const QString &colorName);
    void saveTextFormat(QSettings &settings, const QTextCharFormat &format);
    QTextCharFormat loadTextFormat(QSettings &settings);
    void saveCurrentFormat(QSettings &settings);
    void restoreCurrentFormat(QSettings &settings);
    void addPage(QTreeWidgetItem *notebook);
    void saveNotebookStructure();
    void restoreNotebookStructure();
    void handleImagePaste();
    void setInsert();
    bool eventFilter(QObject *watched, QEvent *event);
    void setBucketColor(const QString &colorName);
    void undo();
    void redo();
    void createTable();
    void insertTableAction();
    void setBorderlineStyle(const QString &BorderType);
    void showContextMenu(const QPoint &pos);
    void deleteNoteAction();
    void doLine();
     void setList(const QString &listGroup);
    void printing();
     void insertingFile();
    void savingFile();
     void ToggleToolbar();
    void ToggleMenubar();
     void insertDate();
    void insertTime();
     void insertQuote();
    void appCustomTrigg();
     void setfontSize(int size);
    void CodeBlock();
     void deleteTableRow();
    void deleteTableColumn();
     //toolbar for file management and settings etc.
     QVBoxLayout *toolLayout;
     QToolBar *topToolBar;
     QMenu *file;
     QAction *saveFile;
     QAction *insertFile;
     QFileDialog *filedialog;
     QAction *print;
     QPrintDialog *printDialog;
     QPrinter printer;

     QMenu *view;
     QAction *appCustom;
     QWidget *custom;
     QAction *toggleSidebar;
     QAction *toggleMenuBar;

     QMenu *tools;
     QAction *options;
     QAction *commandPallete;

    QTreeWidgetItem* pageItem;
    QString getConfigPath();
    //widgets
    //main layout
    QVBoxLayout *layout;
    //toolbar where would be the functions of the editor
    QToolBar *toolbar;
    //Menu for insert something in the file
    QMenu *insert;
    //action to take back a change
    QAction *table;
    QAction *line;

    QTreeWidgetItem* firstNotebook;
    QTreeWidgetItem* firstPage;
    QMenu *deleteMenu;
    QAction *deleteNote;

    QAction *back;
    //action to get back what you deleted
    QAction *next;
    //menu for changing font
    QMenu *font;
    QActionGroup *fontGroup;
    //menu for changing the size of font
    //action pro QMenu font
     int currentSize;
    QString currentFont;


    QMenu *sizeMenu;
    QActionGroup *sizesAction;
    QMenu *specialText;
    QAction *bold;
    //action for making cursive
    QAction *cursive;
    //menu for underline
    QMenu *underline;

    QMenu *borderline;
    //menu for selecting lists
    QAction *wholeBorder;
    QMenu *lists;

    //menu for colors of font
    QMenu *colors;
    //the action group of colors (strings of it)
    QActionGroup *colorGroup;
    //string for setting current color
    QColor currentColor;

    //menu for color of underline
    QMenu *UnderlineColors;
    //action for align left
    QAction *AlignLeft;
    //action for align center
    QAction *AlignCenter;
    //action for align right
    QAction *AlignRight;
    //action for align to block
    QAction *indentie;
    //menu for coloring the whole line or page

    QTextBlockFormat *indentation;
    QMenu *bucket;
    QActionGroup *bucketGroup;
    //TextEdit where the user can write the text
    QTextEdit *edit;

    QActionGroup *listGroup;
    //here I would declare the side panel of app (notebooks and pages)

    //layout where the notebooks would be
    QHBoxLayout *Notebooks;
    QTreeWidget *NoteTree;
    //the button of the notebook
    QTreeWidgetItem *Notebook;
    //line edit to name the notebook
    QLineEdit *Noteedit;
    //button to add new notebook
    QTreeWidgetItem *newNotebook;
    QVBoxLayout *Note;
    QMessageBox *warning;

    //adding scrollbars
    QScrollArea *scrollArea;
 QTimer *reformatTimer;
    QPushButton *addRowButton;
    QPushButton *addColumnButton;

    QMenu *tableMenu;
    QAction *tableAct;
    QAction *tableAct2;

    QAction *date;
    QDateTime dateTime;
    QAction *time;
    QAction *quote;

    QAction *codeBlock;

    //Inside the window of app custom

    //font
    QVBoxLayout *customLayout;
    QLabel *fonts;
    QLabel *fontSize;
    QLabel *fontColor;
    QToolBar *sizesToolbar;
    QMenu *fontSizesMenu;
    QMenu *fontColorsMenu;
    QActionGroup *fontSizes;
    QActionGroup *fontColors;
    QVBoxLayout *colorsLayout;
    QScrollArea *scrollBar;


};
#endif // WINDOW_H
