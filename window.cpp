#include "window.h"
#include <QApplication>
#include <QTextDocument>

Window::Window(QWidget *parent):
    QWidget{parent}, currentfontSize(12), currentSize(12), currentFont("Arial"), currentColor("white")
{
  setWindowTitle("TripleNote");
    this -> setWindowIcon(QIcon(":/main"));
  //the layout of tools on very top
  toolLayout = new QVBoxLayout();
  //layout of the whole window
 layout = new QVBoxLayout();
  layout -> addLayout(toolLayout);
 toolLayout -> setAlignment(Qt::AlignTop);

    //the app setting and file management toolbar
  topToolBar = new QToolBar;
  toolLayout -> addWidget(topToolBar);
  file = new QMenu("File");
  file -> setStyleSheet("background-color: #202020; color:white ");
  topToolBar->addAction(file->menuAction());
  //actions to the file menu
  saveFile = new QAction("Save file");
  saveFile -> setShortcut(QKeySequence("Ctrl+S"));
  //savingFile
connect(saveFile, &QAction::triggered, this, &Window::savingFile);
  insertFile = new QAction("Insert file");
insertFile -> setShortcut(QKeySequence("Ctrl+O"));
   connect(insertFile, &QAction::triggered, this, &Window::insertingFile);

   print = new QAction("Print");
   connect(print, &QAction::triggered, this, &Window::printing);
   print -> setShortcut(QKeySequence("Ctrl+P"));
  file->addAction(saveFile);
   file->addAction(insertFile);
   file->addAction(print);

  //view menu (showing toolbars, custom etc.)
  view = new QMenu("View");
  view -> setStyleSheet("background-color: #202020; color:white ");
   topToolBar->addAction(view->menuAction());
  appCustom = new QAction("App custom");
   appCustom -> setShortcut(QKeySequence("Shift+C"));
   connect(appCustom, &QAction::triggered, this, &Window::appCustomTrigg);
   view -> addAction(appCustom);
  toggleSidebar = new QAction("Toggle bar");
  view -> addAction(toggleSidebar);
  toggleSidebar->setShortcut(QKeySequence("Ctrl+B"));
  toggleSidebar -> setCheckable(true);
  connect(toggleSidebar, &QAction::triggered, this, &Window::ToggleToolbar);
  toggleMenuBar = new QAction("Toggle menu bar");
  toggleMenuBar->setShortcut(QKeySequence("Ctrl+C"));
  toggleMenuBar -> setCheckable(true);
  view -> addAction(toggleMenuBar);
connect(toggleMenuBar, &QAction::triggered, this, &Window::ToggleMenubar);
  //tools menu - options and commands of the app
    tools = new QMenu("Tools");
    tools -> setStyleSheet("background-color: #202020; color:white ");
   topToolBar->addAction(tools->menuAction());
    options = new QAction("Options");
   tools -> addAction(options);
    commandPallete = new QAction("Command pallete");
   tools -> addAction(commandPallete);
      //the layout on the side for notebooks
   Notebooks = new QHBoxLayout(this);

//TREE OF NOTEBOOKS
  // Create the tree widget
  NoteTree = new QTreeWidget();
  NoteTree->setHeaderHidden(true);
  NoteTree->setMinimumSize(250,600);
  NoteTree->setMaximumSize(250,1100);

  // Add the tree widget to the layout
  Notebooks->addWidget(NoteTree);

  // Initialize notebook data structure
  notebookData = QMap<QString, QMap<QString, QString>>();

  // Create the "Add notebook" item
  QTreeWidgetItem *addNotebookItem = new QTreeWidgetItem(NoteTree);
  addNotebookItem->setText(0, "+ Add Notebook");
  addNotebookItem->setFlags(addNotebookItem->flags() | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

  // Connect the item click to your addButton function
  connect(NoteTree, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem *item) {
      if (item->text(0) == "+ Add Notebook") {
          addNotebook();
      }
  });

  NoteTree->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  Notebooks -> addLayout(layout);



//CREATING THE TOOLBAR
  //the toolbar that would keep the function and menus for the editor
    toolbar = new QToolBar("Main Toolbar", this);
    //layout -> addWidget(toolbar);
    toolbar -> setStyleSheet("background-color: #202020");
    this -> setStyleSheet("background-color: #404040; color:white;");
    toolbar -> setStyleSheet("color: white");
     layout->addWidget(toolbar);
    //INSERT SOMETHING INTO THE TEXT EDIT

    // Adding insert menu to the toolbar
    insert = new QMenu("Insert", this);
    toolbar->addAction(insert->menuAction());
    insert -> setIcon(QIcon(":/insert"));

    //an action to insert a line
    line = new QAction("Line", this);
    insert -> addAction(line);
    connect(line, &QAction::triggered, this, &Window::doLine);


    //HERE I WOULD ADD ACTIONS OF THE INSERT MENU
    table = new QAction("Table", this);
    insert -> addAction(table);
    connect(table, &QAction::triggered, this, &Window::createTable);

    date = new QAction("Date", this);
    insert -> addAction(date);
    connect(date, &QAction::triggered, this, &Window::insertDate);

    time = new QAction("Time", this);
    insert -> addAction(time);
    connect(time, &QAction::triggered, this, &Window::insertTime);

    quote = new QAction("Quote", this);
    insert -> addAction(quote);
   connect(quote, &QAction::triggered, this, &Window::insertQuote);

    codeBlock = new QAction("Code Block",this);
   insert -> addAction(codeBlock);
    connect(codeBlock, &QAction::triggered, this, &Window::CodeBlock);

    //ACTIONS OF UNDO AND REDO
    //adding the back action
    back = new QAction(QIcon(":/back"), "", this);
    toolbar->addAction(back);
    connect(back, &QAction::triggered, this, &Window::undo);

    // Create redo action (next)
    next = new QAction(QIcon(":/next"), "", this);
    toolbar->addAction(next);
   connect(next, &QAction::triggered, this, &Window::redo);



    //SETTING FONTS
    //adding the menu for changing font
    font= new QMenu(tr("Fonts"), this);

    fontGroup = new QActionGroup(this);
    QStringList fontTexts = {        "Arial",
                             "Courier New",
                             "Consolas",
                             "Comic Sans MS",
                             "Roboto Mono",
                             "Helvetica",
                             "Times New Roman",
                             "Georgia",
                             "Sans Serif",
                             "Calibri",
                             "Noto Serif",
                             "Lato"

                             };
    createActions(font, fontGroup, fontTexts, [this](const QString &text) { setFont(text); });


    // Add font menu to toolbar
    toolbar->addAction(font->menuAction());

    // Set font menu tooltip
    font->menuAction()->setToolTip(tr("Change text font family"));

    //adding the menu for changing size of the text
    sizeMenu = new QMenu(tr("Size"), this);
    toolbar->addAction(sizeMenu->menuAction());


    //group of actions for sizes of text
    sizesAction = new QActionGroup(this);
    QStringList sizeTexts = {"8",
                             "9",
                             "10",
                             "12",
                             "14",
                             "16",
                             "18",
                             "20",
                             "22",
                             "24",
                             "26",
                             "28",
                             "35",
                             "45",
                             "55",
                             "65",
                             "75"};
    createActions(sizeMenu, sizesAction, sizeTexts, [this](const QString &text) { setSize(text.toInt()); });


    //adding action for making the text bold
    specialText = new QMenu("", this);
    specialText -> setIcon(QIcon(":/special"));
    bold = new QAction(QIcon(":/bold"), "", this);
    specialText -> addAction(bold);
    connect(bold, &QAction::triggered, this, &Window::setBold);
    //adding action for making cursive
    cursive = new QAction(QIcon(":/Cursive"), "", this);
    specialText -> addAction(cursive);
 connect(cursive, &QAction::triggered, this, &Window::setCursive);
   //adding menu for underline
    underline = new QMenu("", this);
    underline -> setIcon(QIcon(":/underline"));
    toolbar ->addAction(underline->menuAction());
    toolbar -> addAction(specialText->menuAction());

    //underline styles. actiongroup
    QActionGroup* underlineGroup = new QActionGroup(this);
    QStringList underlineTexts = {
        "None",
        "Single",
        "Dash Dotted",
        "Dotted",
        "Wavy",
        "Dashed",
    };
    createActions(underline, underlineGroup, underlineTexts, [this](const QString &text) {
        setUnderlineStyle(text);
    });

    //styles of a whole border
    borderline = new QMenu("", this);
    borderline -> setIcon(QIcon(":/border"));
    toolbar->addAction(borderline->menuAction());
    QActionGroup* BorderType = new QActionGroup(this);
    QStringList  borderTexts = {
       "None",
        "Solid",
        "Dotted",
        "Dashed",
        "Double",
        "DotDash",
        "DotDotDash",
        "Groove",
        "Ridge",
        "Inset",
        "Outset"
    };
    createActions(borderline, BorderType,  borderTexts, [this](const QString &text) {
        setBorderlineStyle(text);

    });

    //adding menu for selecting the type of lists
    lists = new QMenu("", this);
    lists -> setIcon(QIcon(":/list"));
    toolbar -> addAction(lists->menuAction());


    listGroup = new QActionGroup(this);
    QStringList listTexts = {
                 "Decimal",
                 "Empty circle",
                 "Filled circle",
                 "Lower Alpha",
                 "Upper Alpha",
                 "Lower Roman",
                 "Upper Roman",
                "Filled square"


                             };
    createActions(lists, listGroup, listTexts, [this](const QString &text) { setList(text); });

    //adding menu for selecting color of the text
    colors = new QMenu("", this);
    colors -> setIcon(QIcon(":/color"));
    toolbar -> addAction(colors->menuAction());

    colorGroup = new QActionGroup(this);
    QStringList colorTexts = {
        "Black",
        "Red",
        "Green",
        "Blue",
        "Yellow",
        "Purple",
        "Orange",
        "Gray",
        "White",
        "Cyan",
        "Pink",
        "Custom..."
    };
    createActions(colors, colorGroup, colorTexts, [this](const QString &text) { setTextColor(text); });


    //adding underline option
    UnderlineColors = new QMenu("Underline Color", this);
    UnderlineColors->setIcon(QIcon(":/pen"));
    toolbar->addAction(UnderlineColors->menuAction());

    QActionGroup* underlineColorGroup = new QActionGroup(this);
    QStringList colorUnderline = {
        "Black",
        "White",
        "Red",
        "Green",
        "Blue",
        "Purple",
        "Pink",
        "Light Blue",
        "Light Green",
        "Gray",
        "Orange",
        "Custom..."
    };
    createActions(UnderlineColors, underlineColorGroup, colorUnderline, [this](const QString &text) {
        setUnderlineColor(text);
    });



    //adding action for the AlignLeft
    AlignLeft = new QAction(QIcon(":/AlignLeft"), "", this);
    toolbar -> addAction(AlignLeft);
     connect(AlignLeft, &QAction::triggered, this, &Window::setLeft);

    //adding action for the AlignRight
    AlignCenter = new QAction(QIcon(":/AlignCenter"), "", this);
    toolbar -> addAction(AlignCenter);
    connect(AlignCenter, &QAction::triggered, this, &Window::setCenter);

    //adding action for AlignCenter
    AlignRight = new QAction(QIcon(":/AlignRight"), "", this);
    toolbar -> addAction(AlignRight);
    connect(AlignRight, &QAction::triggered, this, &Window::setRight);

    //adding action for AlignRight
    indentie = new QAction(QIcon(":/AlignBlock"), "", this);
    toolbar -> addAction(indentie);
     connect(indentie, &QAction::triggered, this, &Window::setIndent);

    // In your Window constructor or setup function:
    QAction *UnindentAction = new QAction(QIcon(":/unindent"),"", this);
     toolbar -> addAction(UnindentAction);
    // TOTO POZDEJI DODELAT UnindentAction->setShortcut(QKeySequence("Tab"));
    connect(UnindentAction, &QAction::triggered, this, &Window::unindentText);



    //THERE WOULD BE COLORINg OF A WHOLE ROW
    bucket = new QMenu("", this);
    bucket -> setIcon(QIcon(":/bucket"));
    toolbar -> addAction(bucket->menuAction());

    bucketGroup = new QActionGroup(this);
    colorGroup = new QActionGroup(this);
    QStringList bucketColors = {
        "Black",
        "Red",
        "Green",
        "Blue",
        "Yellow",
        "Purple",
        "Orange",
        "Gray",
        "White",
        "Cyan",
        "Pink",
        "Default",
        "Custom..."
    };
 createActions(bucket, bucketGroup, bucketColors, [this](const QString &text) { setBucketColor(text); });
    //textEdit for the user input
 tableMenu = new QMenu();
 tableMenu -> setToolTip("Table edit");
 tableMenu -> setIcon(QIcon(":/table"));
 tableMenu-> setStyleSheet("background-color: #202020; color:white ");
 toolbar -> addAction(tableMenu->menuAction());
 tableAct = new QAction("add row");
 tableMenu -> addAction(tableAct);
 connect(tableAct, &QAction::triggered, this, &Window::addTableRow);
 tableAct2 = new QAction("Add column");
 tableMenu -> addAction(tableAct2);
 connect(tableAct2, &QAction::triggered, this, &Window::addTableColumn);
 QAction* deleteRowAct = new QAction("Delete row");
 tableMenu->addAction(deleteRowAct);
 connect(deleteRowAct, &QAction::triggered, this, &Window::deleteTableRow);

 QAction* deleteColAct = new QAction("Delete column");
 tableMenu->addAction(deleteColAct);
 connect(deleteColAct, &QAction::triggered, this, &Window::deleteTableColumn);
 //HERE
    edit = new QTextEdit(this);
    layout->addWidget(edit);
      edit -> setMinimumWidth(500);

//ADD TO MENU
    // In your Window constructor or where you set up actions:
      QAction *pasteAction = new QAction(tr("Paste"), this);
      pasteAction->setShortcut(QKeySequence::Paste);
      connect(pasteAction, &QAction::triggered, this, &Window::handleImagePaste);

      edit->addAction(pasteAction);
      edit->installEventFilter(this);
      edit -> setStyleSheet("background-color: #202020");

    QTimer* autoSaveTimer = new QTimer(this);
    connect(autoSaveTimer, &QTimer::timeout, this, [this]() {
        if (contentModified) {
           savePageContent();
            contentModified = false;
        }
    });
    autoSaveTimer->start(30000);
    if (NoteTree->topLevelItemCount() == 1) { // Only "+ Add Notebook" exists
        addNotebook();
        QTreeWidgetItem* firstNotebook = NoteTree->topLevelItem(0);
        if (firstNotebook && firstNotebook->childCount() > 0) {
            QTreeWidgetItem* firstPage = firstNotebook->child(0);
            NoteTree->setCurrentItem(firstPage);
            currentPage = firstPage;
            edit->clear();
        }
    }
    NoteTree -> setStyleSheet("background-color: #202020");
    //HERE WOULD BE KEY ACTION TO DELETE NOTEBOOKS AND NOTES
    NoteTree -> setContextMenuPolicy(Qt::CustomContextMenu);
connect(NoteTree, &QTreeView::customContextMenuRequested, this, &Window::showCustomMenu);
//connect(this, &Window::rightClicked, this, &Window::slotCustomMenuRequested);
qDebug() << "Signal-slot connection established.";

setupNotebookConnections();
    restoreState();
   setMouseTracking(true);
}

//CUSTOM MENU for NoteTree delete action for deleting tree items
void Window::showCustomMenu(const QPoint &pos) {
    //menu that would pop up when NoteTree is pressed
    QMenu *menu = new QMenu(this);
    //action that should delete a note that is clicked on by mouse right bttn
    deleteNote = new QAction("Delete Note");
    connect(deleteNote, &QAction::triggered, this, &Window::deleteNoteAction);
    menu -> addAction(deleteNote);
    //positioning that will find we are clicking on NoteTree
    QPoint globalPos = NoteTree->mapToGlobal(pos);
    //menu will pop up at certain postition
    menu->popup(globalPos);
}

//an action that deletes specific note from the TreeView
void Window::deleteNoteAction() {

    //current item is selected (current note)
        QTreeWidgetItem* selectedItem = NoteTree->currentItem();
        if (!selectedItem) {
            return;
        }
        //add page and add notebook should not be deletable, cause its needed for proper working

        if (selectedItem->text(0) == "+ Add Page" || selectedItem->text(0) == "+ Add Notebook") {
            return;
        }

        //parent item
        QTreeWidgetItem* parentItem = selectedItem->parent();
       //Would ask an user, if he wants to delete the note using QMessageBox
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Confirm Delete",
            QString("Are you sure you want to delete '%1'?")
                .arg(selectedItem->text(0)),
            QMessageBox::Yes | QMessageBox::No
            );

        //define what happens if user clicks yes
        if (reply == QMessageBox::Yes) {
            if (parentItem) {
                // It's a page item, delete it from its parent (notebook)
                QString notebookName = parentItem->text(0);
                QString pageName = selectedItem->text(0);
                //it should be unique note based on its string, with arguments of its notebook and page name
                QString uniqueKey = QString("%1_%2").arg(notebookName, pageName);

                // Remove from notebookData
                if (notebookData.contains(notebookName) && notebookData[notebookName].contains(pageName)) {
                    notebookData[notebookName].remove(pageName);
                }

                // Remove removed item from settings so it wouldnt be restored again if we turn off and on the app
                QSettings settings(getConfigPath(), QSettings::IniFormat);
                settings.beginGroup("PageContents");
                settings.remove(uniqueKey);
                settings.endGroup();
                settings.sync();

                // Remove the item from the tree
                parentItem->removeChild(selectedItem);
                delete selectedItem;
                selectedItem = nullptr;

                // If the deleted page was the current page, clear the editor
                if (currentPage == selectedItem) {
                    edit->clear();
                    currentPage = nullptr;
                }

                // After deleting a page, we might want to load another page in the same notebook
                if (parentItem->childCount() > 1) { // Check if there are other pages
                    QTreeWidgetItem* nextItem = parentItem->child(0);
                    if (nextItem && nextItem->text(0) != "+ Add Page") {
                        loadPageContent(nextItem);
                        NoteTree->setCurrentItem(nextItem);
                    } else if (parentItem->childCount() == 1 && parentItem->child(0)->text(0) == "+ Add Page") {
                        edit->clear();
                        currentPage = nullptr;
                    }
                } else if (parentItem->childCount() == 1 && parentItem->child(0)->text(0) == "+ Add Page") {
                    edit->clear();
                    currentPage = nullptr;
                }

            } else {
                // It's a top-level notebook item
                QString notebookName = selectedItem->text(0);

                // Remove all associated page content from settings
                QSettings settings(getConfigPath(), QSettings::IniFormat);
                settings.beginGroup("PageContents");
                QMapIterator<QString, QString> i(notebookData[notebookName]);
                while (i.hasNext()) {
                    i.next();
                    QString uniqueKey = QString("%1_%2").arg(notebookName, i.key());
                    settings.remove(uniqueKey);
                }
                settings.endGroup();
                settings.sync();

                // Remove the notebook data
                notebookData.remove(notebookName);

                // Remove the item from the tree
                delete NoteTree->takeTopLevelItem(NoteTree->indexOfTopLevelItem(selectedItem));
                selectedItem = nullptr; // Important to nullify the pointer

                // If the deleted notebook contained the current page, clear the editor
                if (currentPage && currentPage->parent() == selectedItem) {
                    edit->clear();
                    currentPage = nullptr;
                }
            }

            // Save the updated notebook structure (which now excludes the deleted item)
            saveNotebookStructure();
        }
    }

void Window::setUnderlineColor(const QString &colorName)
{
    QColor color;

    if (colorName == "Black") color = Qt::black;
    if (colorName == "White") color = Qt::white;
    else if (colorName == "Red") color = Qt::red;
    else if (colorName == "Green") color = Qt::green;
    else if (colorName == "Blue") color = Qt::blue;
    else if (colorName == "Purple") color = QColor(128, 0, 128);
    else if (colorName == "Pink") color = QColor(255,153,255);
    else if (colorName == "Light Blue") color = QColor(204,255,255);
    else if (colorName == "Light Green") color = QColor(204,255,204);
    else if (colorName == "Gray") color = QColor(129,129,129);
     else if (colorName == "Orange") color = QColor(255,178,102);
    else if (colorName == "Custom...") {
        color = QColorDialog::getColor(currentUnderlineColor, this, "Select Underline Color");
        if (!color.isValid()) return; // User canceled

        /*        "Light Blue",
        "Light Green",
        "Gray", */
    }

    currentUnderlineColor = color;

    QTextCursor cursor = edit->textCursor();
    QTextCharFormat format;
    format.setUnderlineColor(color);

    if (cursor.hasSelection()) {
        cursor.mergeCharFormat(format);
    } else {
        edit->mergeCurrentCharFormat(format);
    }
}


//setting the size of the selected text
void Window::setSize(int size)
{
    currentSize = size;


    QTextCursor cursor = edit->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat format;
        format.setFontPointSize(currentSize);
        cursor.mergeCharFormat(format);
    } else {
        QFont font = edit->font();
        font.setPointSize(currentSize);
     edit ->setFont(font);
    }
}

//function that creates actions for fonts, colors etc. to not having creating them manually
void Window::createActions(QMenu *menu, QActionGroup *group, const QStringList &texts, const std::function<void(const QString &)> &slot)
{
    for (const QString &text : texts) {
        QAction *action = new QAction(text, this);
        action->setCheckable(true);
        group->addAction(action);
        menu->addAction(action);
        connect(action, &QAction::triggered, [slot, text]() { slot(text); });
    }
    group->setExclusive(true);
}

void Window::setBold() {
    QTextCursor cursor = edit->textCursor();
    QTextCharFormat format = cursor.charFormat();
    QFont font = format.font();
    font.setBold(!font.bold()); // Toggle the bold state
    format.setFont(font);
    cursor.mergeCharFormat(format);
    edit->mergeCurrentCharFormat(format); // Apply to current and future input
}

void Window::setCursive(){
    //TextCursor that we would apply on a text edit
    QTextCursor cursor2 = edit->textCursor();
    //TextCharFormat set char format to cursor
    QTextCharFormat format2 = cursor2.charFormat();
    //Setting font to a format
    QFont font2 = format2.font();
    //setting font as italic
    font2.setItalic(!font2.italic());
    //format -> our font
    format2.setFont(font2);
    //connecting cursor to format
    cursor2.mergeCharFormat(format2);
    //giving edit the current format
    edit->mergeCurrentCharFormat(format2);
}

void Window::undo() {
   edit->undo();
}

void Window::redo() {
   edit->redo();
}

void Window::ToggleToolbar() {
    NoteTree -> setHidden(toggleSidebar->isChecked());
    NoteTree -> isHidden();
}
void Window::ToggleMenubar() {
    toolbar -> setHidden(toggleMenuBar->isChecked());
    toolbar -> isHidden();

}

//function of the window for customization of app
void Window::appCustomTrigg() {
    //setting the scrollarea of custom window
    scrollBar = new QScrollArea();
    scrollBar->setWindowTitle("Custom Settings");
    scrollBar->setWidgetResizable(true);
    scrollBar->resize(300, 400);

    //the custom widget
    custom = new QWidget(scrollBar);
    custom->setStyleSheet("background-color:#202020;");
    customLayout = new QVBoxLayout(custom);

    customLayout->setContentsMargins(5, 5, 5, 5);
    customLayout->setSpacing(5);

     //the label for font size
    QLabel *fontSizeLabel = new QLabel("Font Size");
    fontSizeLabel->setStyleSheet("color:white;");
    customLayout->addWidget(fontSizeLabel);

    //the ComboBox fort font sizes and its stylesheet
    QComboBox *fontSizeComboBox = new QComboBox(custom);
    fontSizeComboBox->setStyleSheet(
        "QComboBox { background-color: #303030; color: white; border: 1px solid #505050; padding: 5px; }"
        "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: center right; width: 20px; }"
        "QComboBox QAbstractItemView { background-color: #303030; color: white; selection-background-color: #505050; }"
        "QComboBox QAbstractItemView::item { padding: 5px; }"
        );

    //string where function picks which size to set based on its value
    QStringList fontsizeTexts = {
        "8", "9", "10", "12", "14", "16", "18", "20", "22", "24", "26", "28", "35"
    };
    //adding these values to combobox
    fontSizeComboBox->addItems(fontsizeTexts);

    //calling our functions where the sizes are defined by clicking on ComboBox item
    connect(fontSizeComboBox, QOverload<int>::of(&QComboBox::activated),
            this, [this, fontSizeComboBox](int index) {
                int selectedSize = fontSizeComboBox->currentText().toInt();
                setfontSize(selectedSize);
            });
    customLayout->addWidget(fontSizeComboBox);


  //the label of color picking
    QLabel *colorsLabel = new QLabel("Background color");
    colorsLabel->setStyleSheet("color:white;");
    customLayout->addWidget(colorsLabel);
//the combobox where user can pick colors and its stylesheet
    QComboBox *colorComboBox = new QComboBox(custom);
    colorComboBox->setStyleSheet(
        "QComboBox { background-color: #303030; color: white; border: 1px solid #505050; padding: 5px; }"
        "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: center right; width: 20px; }"
        "QComboBox QAbstractItemView { background-color: #303030; color: white; selection-background-color: #505050; }"
        "QComboBox QAbstractItemView::item { padding: 5px; }"
        );

    //the list of colors it can use
    QList<QColor> appColors = {
        Qt::black, Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::magenta, Qt::darkGray, Qt::white
    };
    QStringList appColorNames = {
        "Black", "Red", "Green", "Blue", "Yellow", "Purple", "Dark Gray", "White"
    };

    for (int i = 0; i < appColors.size(); ++i) {
        QPixmap pix(16, 16);
        pix.fill(appColors.at(i));
        colorComboBox->addItem(QIcon(pix), appColorNames.at(i));
    }
    colorComboBox->addItem("Custom...");

//handling the possibility of custom option - let user pick color from pallete
    connect(colorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this, colorComboBox](int index) {
                if (colorComboBox->itemText(index) == "Custom...") {
                    QColor newColor = QColorDialog::getColor(Qt::white, this, "Select Custom Color");
                    if (newColor.isValid()) {
                        setAppColor(newColor.name()); // <--- HERE'S THE ISSUE
                        // ...
                    }
                } else {
                    QString selectedColorName = colorComboBox->itemText(index);
                    setAppColor(selectedColorName);
                    // ...
                }

            });


    customLayout->addWidget(colorComboBox);


    QLabel *NoteColors = new QLabel("NoteTree text");
    NoteColors->setStyleSheet("color:white;");
    customLayout->addWidget(NoteColors);
    QComboBox *NoteComboBox = new QComboBox(custom);
   NoteComboBox->setStyleSheet(
        "QComboBox { background-color: #303030; color: white; border: 1px solid #505050; padding: 5px; }"
        "QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: center right; width: 20px; }"
        "QComboBox QAbstractItemView { background-color: #303030; color: white; selection-background-color: #505050; }"
        "QComboBox QAbstractItemView::item { padding: 5px; }"
        );

    //the list of colors it can use
    QList<QColor> noteColors = {
        Qt::black, Qt::red, Qt::green, Qt::blue, Qt::yellow, Qt::magenta, Qt::darkGray, Qt::white
    };
    QStringList noteColorNames = {
        "Black", "Red", "Green", "Blue", "Yellow", "Purple", "Dark Gray", "White"
    };

    for (int i = 0; i < noteColors.size(); ++i) {
        QPixmap pix(16, 16);
        pix.fill(noteColors.at(i));
        NoteComboBox->addItem(QIcon(pix), noteColorNames.at(i));
    }
    NoteComboBox->addItem("Custom...");

    //handling the possibility of custom option - let user pick color from pallete
    connect(NoteComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this, NoteComboBox](int index) {
                if (NoteComboBox->itemText(index) == "Custom...") {
                    QColor newColor = QColorDialog::getColor(Qt::white, this, "Select Custom Color");
                    if (newColor.isValid()) {
                        setNoteColor(newColor.name());

                    }
                } else {
                    QString selectedColorName = NoteComboBox->itemText(index);
                    setNoteColor(selectedColorName);
                }

            });

    customLayout -> addWidget(NoteComboBox);
      customLayout->addStretch(1);
    scrollBar->setWidget(custom);
    scrollBar->show();
}


void Window::setAppColor(const QString &colorName) {
    QColor color;
     currentAppColor = color;
    if (colorName == "Black") {
        color = Qt::black;
    } else if (colorName == "Red") {
        color = Qt::red;
    } else if (colorName == "Green") {
        color = Qt::green;
    } else if (colorName == "Blue") {
        color = Qt::blue;
    } else if (colorName == "Yellow") {
        color = Qt::yellow;
    } else if (colorName == "Purple") {
        color = QColor(128, 0, 128);
    } else if (colorName == "Orange") {
        color = QColor(255, 165, 0);
    } else if (colorName == "Gray") {
        color = Qt::gray;
    } else if (colorName == "White") {
        color = Qt::white;
    } else if (colorName == "Cyan") {
        color = Qt::cyan;
    } else if (colorName == "Pink") {
        color = QColor(255, 192, 203);
    } else if (colorName == "Custom...") {
        QColorDialog::getColor(QApplication::palette().color(QPalette::WindowText), this, "Choose Custom Color");
        if (!color.isValid()) {
            return;
        }
        QPalette palette = this->palette();
        palette.setColor(QPalette::WindowText, color);
        this->setPalette(palette);
        return;
    } else {

        color = QColor(colorName);
        if (!color.isValid()) {
            color = Qt::black;
        }
    }

     this -> setPalette(color);
    currentAppColor = color.name();
}
void Window::setNoteColor(const QString &colorName) {
    QColor color;
    if (colorName == "Black") {
        color = Qt::black;
    } else if (colorName == "Red") {
        color = Qt::red;
    } else if (colorName == "Green") {
        color = Qt::green;
    } else if (colorName == "Blue") {
        color = Qt::blue;
    } else if (colorName == "Yellow") {
        color = Qt::yellow;
    } else if (colorName == "Purple") {
        color = QColor(128, 0, 128); // Matches your defined purple
    } else if (colorName == "Orange") {
        color = QColor(255, 165, 0);
    } else if (colorName == "Gray") {
        color = Qt::gray;
    } else if (colorName == "White") {
        color = Qt::white;
    } else if (colorName == "Cyan") {
        color = Qt::cyan;
    } else if (colorName == "Pink") {
        color = QColor(255, 192, 203);
    } else if (colorName == "Dark Gray") {
        color = Qt::darkGray;
    }
    else {
        color = QColor(colorName);
        if (!color.isValid()) {

            color = Qt::black;
        }
    }

    QPalette palette = NoteTree->palette();
    palette.setColor(QPalette::WindowText, color); // Sets the color for the text items in QTreeWidget
    palette.setColor(QPalette::Text, color); // Also apply to QPalette::Text for robustness
    NoteTree->setPalette(palette);
    NoteTree->update(); // Request a repaint
    currentNoteColor = color.name();
}

//action that increases the size of NoteTree and Menu Items in toolbar
void Window::setfontSize(int size)
{
     QApplication::font().pointSize();

    currentfontSize = size;

    QFont appFont = QApplication::font();
    appFont.setPointSize(currentfontSize);
    QApplication::setFont(appFont);

    if (toolbar) {
        QFont toolbarFont = toolbar->font();
        toolbarFont.setPointSize(currentfontSize);
        toolbar->setFont(toolbarFont);
        toolbar->update();
    }

    if (topToolBar) {
        QFont topToolbarFont = topToolBar->font();
        topToolbarFont.setPointSize(currentfontSize);
        topToolBar->setFont(topToolbarFont);
         topToolBar->update();
    }

    if (NoteTree) {
        QFont noteTreeFont = NoteTree->font();
        noteTreeFont.setPointSize(currentfontSize);
        NoteTree->setFont(noteTreeFont);
        NoteTree->update();
    }

        }


/*function for creating table, unfortunately i didnt manage to
 make the table resizable by dragging. :(
 */

void Window::createTable() {
    //dialog that will pull out if user clicks to insert table action
    QDialog dialog(this);
    dialog.setWindowTitle("Insert Table");
    //form dialog inside of the QDialog
    QFormLayout form(&dialog);

    //widget, where user can pick the rows and columns of his app
    QSpinBox* rowsSpin = new QSpinBox(&dialog);
    //setting the range of possible cells and columns, default in dialog is 3
    rowsSpin->setRange(1, 100);
    rowsSpin->setValue(3);
    QSpinBox* colsSpin = new QSpinBox(&dialog);
    colsSpin->setRange(1, 100);
    colsSpin->setValue(3);

    //adding the labels of Rows and Columns and relate it to the vars
    form.addRow("Rows:", rowsSpin);
    form.addRow("Columns:", colsSpin);

    //The dialog button that would decide whats gonna happen by users choice
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    // connecting action with the vote of user, what happens if acc or rejct
    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    //if user votes accepted, the rows and cols by choice are added
    if (dialog.exec() == QDialog::Accepted) {
        int rows = rowsSpin->value();
        int cols = colsSpin->value();

        QTextCursor cursor = edit->textCursor();
        cursor.insertBlock();

       //the design of table
        QTextTableFormat format;
        format.setBorder(2.0);
        format.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
        format.setBorderBrush(QBrush(Qt::black));
        format.setCellPadding(6);
        format.setCellSpacing(0);
        format.setAlignment(Qt::AlignLeft);

        //creating table of the rows, columns and format, applying it all
        QTextTable* table = cursor.insertTable(rows, cols, format);

        //managing the function of cells and their design
        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                QTextTableCell cell = table->cellAt(row, col);
                QTextCursor cellCursor = cell.firstCursorPosition();

                QTextTableCellFormat cellFormat;
                cellFormat.setBorder(1.0);
                cellFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
                cellFormat.setBorderBrush(QBrush(Qt::black));
                if (row % 2 == 1) {
                    cellFormat.setBackground(QBrush(QColor(220,220,220)));
                } else {
                    cellFormat.setBackground(QBrush(Qt::gray));
                }
                cellCursor.setBlockCharFormat(cellFormat);
                //Setting the text to many spaces, becuase idk how to do the cells larger without a text :DD
                cellCursor.insertText(QString("                     ").arg(row + 1).arg(col + 1));

            }
        }
    }

}

void Window::addTableRow() {
    QTextCursor cursor = edit->textCursor();
    QTextTable *table = cursor.currentTable();

    if (table) {
        QTextTableCell currentCell = table->cellAt(cursor);
        if (!currentCell.isValid()) {
            QMessageBox::warning(this, "Error", "Cursor not in a valid table cell.");
            return;
        }
        int currentRow = currentCell.row();
        table->insertRows(currentRow + 1, 1);
        int newRow = currentRow + 1;

        for (int col = 0; col < table->columns(); ++col) {
            QTextTableCellFormat formatToCopy;
            if (currentRow >= 0) {
                // Copy format from cell above (same column)
                formatToCopy = table->cellAt(currentRow, col).format().toTableCellFormat();
            }
            QTextCursor newCellCursor = table->cellAt(newRow, col).firstCursorPosition();
            newCellCursor.setBlockCharFormat(formatToCopy);
            newCellCursor.insertText("                     ");
        }
        QTextCursor newCursor = table->cellAt(newRow, 0).firstCursorPosition();
        edit->setTextCursor(newCursor);
    } else {
        QMessageBox::information(this, "No Table Selected", "Please place the cursor inside a table to add a row.");
    }
}
// Your existing addTableColumn function (no changes needed)
void Window::addTableColumn() {
    QTextCursor cursor = edit->textCursor();
    QTextTable *table = cursor.currentTable();

    if (table) {
        QTextTableCell currentCell = table->cellAt(cursor);
        if (!currentCell.isValid()) {
            QMessageBox::warning(this, "Error", "Cursor not in a valid table cell.");
            return;
        }
        int currentCol = currentCell.column();
        table->insertColumns(currentCol + 1, 1);
        int newCol = currentCol + 1;

        for (int row = 0; row < table->rows(); ++row) {
            QTextTableCellFormat formatToCopy;
            if (currentCol >= 0) {
                // Copy format from cell to the left (same row)
                formatToCopy = table->cellAt(row, currentCol).format().toTableCellFormat();
            }
            QTextCursor newCellCursor = table->cellAt(row, newCol).firstCursorPosition();
            newCellCursor.setBlockCharFormat(formatToCopy);
            newCellCursor.insertText("                     ");
        }
        QTextCursor newCursor = table->cellAt(0, newCol).firstCursorPosition();
        edit->setTextCursor(newCursor);
    } else {
        QMessageBox::information(this, "No Table Selected", "Please place the cursor inside a table to add a column.");
    }
}
void Window::deleteTableRow() {
    QTextCursor cursor = edit->textCursor();
    QTextTable *table = cursor.currentTable();

    if (table) {
        QTextTableCell currentCell = table->cellAt(cursor);
        if (!currentCell.isValid()) {
            QMessageBox::warning(this, "Error", "Cursor not in a valid table cell.");
            return;
        }
        int currentRow = currentCell.row();
        // Ensure there's more than one row before deleting
        if (table->rows() > 1) {
            table->removeRows(currentRow, 1);
        } else {

            QMessageBox::information(this, "Cannot Delete Row", "Cannot delete the last row of the table. To remove the table, select and delete it manually.");
        }
    } else {
        QMessageBox::information(this, "No Table Selected", "Please place the cursor inside a table to delete a row.");
    }
}
void Window::deleteTableColumn() {
    QTextCursor cursor = edit->textCursor();
    QTextTable *table = cursor.currentTable();

    if (table) {
        QTextTableCell currentCell = table->cellAt(cursor);
        if (!currentCell.isValid()) {
            QMessageBox::warning(this, "Error", "Cursor not in a valid table cell.");
            return;
        }
        int currentCol = currentCell.column();

        if (table->columns() > 1) {
            table->removeColumns(currentCol, 1);
        } else {
            QMessageBox::information(this, "Cannot Delete Column", "Cannot delete the last column of the table. To remove the table, select and delete it manually.");
        }
    } else {
        QMessageBox::information(this, "No Table Selected", "Please place the cursor inside a table to delete a column.");
    }
}
void Window::insertDate() {

   QDateTime dateTime = dateTime.currentDateTime();
    QString currentDateTime = dateTime.toString("yyyy-MM-dd");
    edit -> insertPlainText(currentDateTime);
}

void Window::insertTime(){
    QDateTime dateTime = dateTime.currentDateTime();
    QString currentDateTime = dateTime.toString("HH:mm");
    edit -> insertPlainText(currentDateTime);
}

void Window::insertQuote() {
    QTextCursor cursor = edit->textCursor();
    QString selectedText = cursor.selectedText();
   QTextCharFormat charFormat = cursor.charFormat();
    QTextBlockFormat blockFormat = cursor.blockFormat();
    QString formattedText("|");
   blockFormat.setIndent(blockFormat.indent() + 1);
  charFormat.setFontPointSize(edit->font().pointSize() + 15);
   cursor.setBlockFormat(blockFormat);
    cursor.setCharFormat(charFormat);
   cursor.insertText(formattedText);

}
void Window::handleCursorPositionChanged() {
    QTextCursor cursor = edit->textCursor();
    QTextTable *table = cursor.currentTable();

    if (table) {
        addRowButton->setEnabled(true);
        addColumnButton->setEnabled(true);
    } else {
        addRowButton->setEnabled(false);
        addColumnButton->setEnabled(false);
    }
}
void Window::doLine() {
    QTextCursor cursor = edit->textCursor();
    cursor.insertHtml("<hr>");
}

void Window::CodeBlock() {
 QTextCharFormat codeKeywordFormat;
    QTextCharFormat codeCharFormat;
    QTextBlockFormat codeBlockFormat;
 QTextCharFormat codeVariableFormat;
    codeCharFormat.setFontFamily("Inconsolata");
    if (!QFontDatabase::families().contains("Inconsolata")) {
        codeCharFormat.setFontFamily("monospace");
    }
    codeCharFormat.setFontFixedPitch(true);
    codeCharFormat.setFontPointSize(10); // Don't forget pointSize here too!
    codeCharFormat.setForeground(QColor("#DCDCDC"));

    // Code block container format (background, margins, no wrap)
    codeBlockFormat.setBackground(QColor("#2B2B2B"));
    codeBlockFormat.setTextIndent(0);
    codeBlockFormat.setLeftMargin(20);
    codeBlockFormat.setRightMargin(20);
    codeBlockFormat.setTopMargin(5);
    codeBlockFormat.setBottomMargin(5);
    codeBlockFormat.setIndent(0);
    codeBlockFormat.setNonBreakableLines(true);

    // Keyword format (e.g., 'if', 'for', 'class')
    codeKeywordFormat.setForeground(QColor("#569CD6")); // Blue
    codeKeywordFormat.setFontWeight(QFont::Bold);
    codeKeywordFormat.setFontFamily(codeCharFormat.fontFamily());
    codeKeywordFormat.setFontFixedPitch(true);
    codeKeywordFormat.setFontPointSize(10); // Keep consistent point size

    // String literal format (e.g., "hello")
    codeStringFormat.setForeground(QColor("#D69D85")); // Orange-ish
    codeStringFormat.setFontFamily(codeCharFormat.fontFamily());
    codeStringFormat.setFontFixedPitch(true);
    codeStringFormat.setFontPointSize(10);

    // Comment format (e.g., // a comment)
    codeCommentFormat.setForeground(QColor("#6A9955")); // Green
    codeCommentFormat.setFontFamily(codeCharFormat.fontFamily());
    codeCommentFormat.setFontFixedPitch(true);
    codeCommentFormat.setFontPointSize(10);
    codeVariableFormat.setForeground(QColor("#00ff7f")); // A light blue/cyan
    codeVariableFormat.setFontFamily(codeCharFormat.fontFamily());
    codeVariableFormat.setFontFixedPitch(true);
    codeVariableFormat.setFontPointSize(10);


    QTextCursor cursor = edit->textCursor();
    QTextBlock currentBlock = cursor.block();
    QTextCharFormat currentCharFormat = cursor.charFormat();

    // The comparison should now use the *member* codeBlockFormat and codeCharFormat
    bool isCodeBlock = (currentBlock.blockFormat().background() == codeBlockFormat.background() &&
                        currentBlock.blockFormat().leftMargin() == codeBlockFormat.leftMargin() &&
                        currentCharFormat.fontFamily() == codeCharFormat.fontFamily() &&
                        currentCharFormat.fontFixedPitch() == codeCharFormat.fontFixedPitch());
                        //currentCharFormat.pointSize() == codeCharFormat.pointSize()); // Added pointSize check back

    cursor.beginEditBlock();

    if (isCodeBlock) {
        // Revert to default formatting
        QTextBlockFormat defaultBlockFormat;
        QTextCharFormat defaultCharFormat;

        cursor.setBlockFormat(defaultBlockFormat);
        cursor.mergeCharFormat(defaultCharFormat);

        QTextBlockFormat newBlockFormat = cursor.blockFormat();
        newBlockFormat.setNonBreakableLines(false);
        cursor.setBlockFormat(newBlockFormat);

    } else {
        // Apply main code block formatting
        cursor.setBlockFormat(codeBlockFormat);
        cursor.mergeCharFormat(codeCharFormat); // Applies monospace font and default code color

        QString selectedText = cursor.selectedText();
        // If nothing is selected, apply to the entire current block's text
        if (selectedText.isEmpty()) {
            selectedText = currentBlock.text();
            // IMPORTANT: To format the whole block correctly when nothing is selected,
            // you need to move the cursor to the start of the block and select it.
            cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
            cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        } else {
            // If text is selected, store its original start position to correctly apply formatting
            cursor.setPosition(cursor.selectionStart(), QTextCursor::MoveAnchor);
        }

        int currentBlockStartPos = cursor.position(); // Store the absolute start position of the selected/block text

        // --- Apply specific colors using the member variables ---

        QStringList keywords = {"void", "int", "class", "return", "if", "else", "for", "while", "array","print","string","echo","cout","cin","printf","bool"};
        for (const QString& keyword : qAsConst(keywords)) {
            int index = 0;
            // Iterate through the selectedText (or current block text)
            while ((index = selectedText.indexOf(keyword, index, Qt::CaseSensitive)) != -1) {
                QTextCursor keywordCursor = edit->textCursor(); // Use editor's cursor to create a new one
                keywordCursor.setPosition(currentBlockStartPos + index, QTextCursor::MoveAnchor);
                keywordCursor.setPosition(currentBlockStartPos + index + keyword.length(), QTextCursor::KeepAnchor);
                keywordCursor.mergeCharFormat(codeKeywordFormat);
                index += keyword.length();
            }
        }

        // Apply string color
        QRegularExpression stringRx("\"([^\"]*)\"");
        QRegularExpressionMatchIterator i = stringRx.globalMatch(selectedText);
        while (i.hasNext()) {
            QRegularExpressionMatch match = i.next();
            QTextCursor stringCursor = edit->textCursor(); // Use editor's cursor to create a new one
            stringCursor.setPosition(currentBlockStartPos + match.capturedStart(), QTextCursor::MoveAnchor);
            stringCursor.setPosition(currentBlockStartPos + match.capturedEnd(), QTextCursor::KeepAnchor);
            stringCursor.mergeCharFormat(codeStringFormat);
        }

        // Apply comment color (basic: "//" to end of line)
        int commentIndex = selectedText.indexOf("//");
        if (commentIndex != -1) {
            QTextCursor commentCursor = edit->textCursor(); // Use editor's cursor to create a new one
            commentCursor.setPosition(currentBlockStartPos + commentIndex, QTextCursor::MoveAnchor);
            commentCursor.setPosition(currentBlockStartPos + selectedText.length(), QTextCursor::KeepAnchor);
            commentCursor.mergeCharFormat(codeCommentFormat);
        }
    }
     QString selectedText = cursor.selectedText();
    int currentBlockStartPos;
    QTextCursor entireBlockCursor = edit->textCursor();

    entireBlockCursor.setPosition(currentBlockStartPos, QTextCursor::MoveAnchor);
    entireBlockCursor.setPosition(currentBlockStartPos + selectedText.length(), QTextCursor::KeepAnchor);

    QRegularExpression wordRx("\\b[A-Za-z_][A-Za-z0-9_]*\\b"); // Matches C-style identifiers
    QRegularExpressionMatchIterator wordIt = wordRx.globalMatch(selectedText);
    while (wordIt.hasNext()) {
        QRegularExpressionMatch wordMatch = wordIt.next();
        QTextCursor wordCursor = edit->textCursor();
        wordCursor.setPosition(currentBlockStartPos + wordMatch.capturedStart(), QTextCursor::MoveAnchor);
        wordCursor.setPosition(currentBlockStartPos + wordMatch.capturedEnd(), QTextCursor::KeepAnchor);

        // ONLY apply variable format if its current foreground color is the default code text color
        // This prevents overwriting keywords, strings, comments that were already colored.
        if (wordCursor.charFormat().foreground() == codeCharFormat.foreground()) {
            wordCursor.mergeCharFormat(codeVariableFormat);
        }
         cursor.endEditBlock();
    }
}

void Window::handleImagePaste() {
    qDebug() << "handleImagePaste() called!";
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    qDebug() << "Available MIME types: " << mimeData->formats();

    if (mimeData->hasImage()) {
        QImage image = qvariant_cast<QImage>(mimeData->imageData());
        qDebug() << "Pasting as Image - isNull: " << image.isNull() << ", size: " << image.size();

        if (!image.isNull()) {
            const int MAX_WIDTH = 600;
            const int MAX_HEIGHT = 400;
            if (image.width() > MAX_WIDTH || image.height() > MAX_HEIGHT) {
                image = image.scaled(MAX_WIDTH, MAX_HEIGHT, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                qDebug() << "Image scaled down to: " << image.size();
            }

            QByteArray imageData;
            QBuffer buffer(&imageData);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");
            QString base64Image = QString::fromLatin1(imageData.toBase64().data());

            QString html = QString("<img src=\"data:image/png;base64,%1\" width=\"%2\" height=\"%3\" />")
                               .arg(base64Image)
                               .arg(image.width())
                               .arg(image.height());

            QTextCursor cursor = edit->textCursor();
            cursor.insertHtml(html);

            qDebug() << "Image inserted as Base64 HTML.";


        } else {
            qDebug() << "Error: Could not retrieve image data from clipboard.";
        }
    }
    else if (mimeData->hasHtml()) {
        qDebug() << "Pasting as HTML - content (first 500 chars): " << mimeData->html().left(500);
        QTextCursor cursor = edit->textCursor();
        cursor.insertHtml(mimeData->html());
    }
    else if (mimeData->hasText()) {
        qDebug() << "Pasting as Text - content (first 200 chars): " << mimeData->text().left(200);
        QTextCursor cursor = edit->textCursor();
        cursor.insertText(mimeData->text());
    }
    else {
        qDebug() << "Clipboard content not handled (no image, html, or text). Formats:" << mimeData->formats();
    }
}
bool Window::eventFilter(QObject *watched, QEvent *event) {
    if (watched == edit && event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_V && (keyEvent->modifiers() & Qt::ControlModifier)) {
            qDebug() << "Ctrl+V pressed on QTextEdit - event filter triggered!";
            handleImagePaste();
            return true; // Consume the event to prevent default paste
        }
    }
    return QWidget::eventFilter(watched, event);
}

//setting aligment of selected text
void Window::setLeft()
{
    edit->setAlignment(Qt::AlignLeft);
}
void Window::setRight()
{
      edit->setAlignment(Qt::AlignRight);
}
void Window::setCenter()
{
     edit->setAlignment(Qt::AlignCenter);
}
void Window::setIndent()
{
    QTextCursor cursor = edit->textCursor();

    if (cursor.hasSelection()) {
        // Indent all selected blocks (paragraphs)
        QTextBlockFormat blockFormat = cursor.blockFormat();
        blockFormat.setIndent(blockFormat.indent() + 1); // Increase the indent level
        cursor.mergeBlockFormat(blockFormat);
    } else {
        // Insert a tab at the current cursor position
        cursor.insertText("\t");
}
}

//A function that would set a list type
void Window::setList(const QString &listGroup) {
    QTextListFormat form;
    QTextCursor cursor = edit->textCursor();

    form.setStart(1);
     if (listGroup == "Decimal")
    form.setStyle(QTextListFormat::ListDecimal);
     else if (listGroup == "Empty circle")
             form.setStyle(QTextListFormat::ListCircle);
     else if (listGroup == "Filled circle")
         form.setStyle(QTextListFormat::ListDisc);
     else if (listGroup == "Filled square")
         form.setStyle(QTextListFormat::ListSquare);
     else if (listGroup == "Lower Alpha")
         form.setStyle(QTextListFormat::ListLowerAlpha);
     else if (listGroup == "Upper Alpha")
         form.setStyle(QTextListFormat::ListUpperAlpha);
     else if (listGroup == "Lower Roman")
         form.setStyle(QTextListFormat::ListLowerRoman);
     else if (listGroup == "Upper Roman")
         form.setStyle(QTextListFormat::ListUpperRoman);
     else
         form.setStyle(QTextListFormat::ListStyleUndefined);

    cursor.createList(form);
}

void Window::unindentText() {
    QTextCursor cursor = edit->textCursor();

    QTextBlockFormat blockFormat = cursor.blockFormat();
    if (blockFormat.indent() > 0) {
        blockFormat.setIndent(blockFormat.indent() - 1);
        cursor.mergeBlockFormat(blockFormat);
    } else if (!cursor.hasSelection() && cursor.positionInBlock() == 0) {
        // If at the start of a line and no indent, maybe remove a leading tab
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, 1);
        if (cursor.selectedText() == "\t") {
            cursor.removeSelectedText();
        } else {
            cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1); // Deselect
        }
        // Connect this to another action or shortcut (e.g., Shift+Tab)
        QAction *unindentAction = new QAction("Unindent", this);
        unindentAction->setShortcut(QKeySequence("Shift+Tab"));
        connect(unindentAction, &QAction::triggered, this, &Window::unindentText);
    }
}

void Window::setFont(const QString &fontName)
{

    currentFont = fontName;
    QTextCursor cursor = edit->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat format;
        format.setFontFamily(currentFont);
        cursor.mergeCharFormat(format);
    } else {
        QFont font = edit->font();
        font.setFamily(currentFont);
         edit->setFont(font);
    }
}
void Window::setTextColor(const QString &colorName)
{
    currentColor = QColor(colorName);
    QTextCursor cursor = edit->textCursor();
    if (cursor.hasSelection()) {
        QTextCharFormat format;
        format.setForeground(currentColor);
        cursor.mergeCharFormat(format);
    } else {
        QPalette palette = edit->palette();
        palette.setColor(QPalette::Text, currentColor);
        edit->setPalette(palette);
    }
    if (colorName == "Custom...") {
        QColor color;
         color = QColorDialog::getColor(currentColor, this, "Select Text Color");
        if (!color.isValid()) return;
        QTextCursor cursor = edit->textCursor();
        QTextCharFormat format;
        format.setForeground(color);
        if (cursor.hasSelection()) {
            cursor.mergeCharFormat(format);
        } else {
            edit->mergeCurrentCharFormat(format);
        }

    }
}
//SET BUCKET COLOR
void Window::setBucketColor(const QString &colorName)
{
    QColor selectedColor = QColor(colorName);
    currentbucketColor = QColor(colorName);
    QTextCursor cursor = edit->textCursor();

    if (cursor.hasSelection()) {
        QTextCharFormat charFormat;
        if (colorName == "Default") selectedColor = QColor(32,32,32);
        charFormat.setBackground(selectedColor);

        cursor.mergeCharFormat(charFormat);
    }


if (colorName == "Custom...") {
    QColor color;
    color = QColorDialog::getColor(currentbucketColor, this, "Select Text Color");
    if (!color.isValid()) return;
    QTextCursor cursor = edit->textCursor();
    QTextCharFormat format;
    format.setBackground(color);
    if (cursor.hasSelection()) {
        cursor.mergeCharFormat(format);
    } else {
        edit->mergeCurrentCharFormat(format);
    }


}
}
//function for setting underline
void Window::setUnderlineStyle(const QString &underlineType)
{
    QTextCharFormat format;


    if (underlineType == "Single") {
        format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
        currentUnderlineStyle = SingleUnderline;
    }
    else if (underlineType == "Dash Dotted") {
        format.setUnderlineStyle(QTextCharFormat::DashDotLine);
        currentUnderlineStyle = DoubleUnderline; //HERE REPAIR
    }
    else if (underlineType == "Dotted") {
        format.setUnderlineStyle(QTextCharFormat::DotLine);
        currentUnderlineStyle = DottedUnderline;
    }
    else if (underlineType == "Wavy") {
        format.setUnderlineStyle(QTextCharFormat::WaveUnderline);
        currentUnderlineStyle = WaveUnderline;
    }
    else if (underlineType == "Dashed") {
        format.setUnderlineStyle(QTextCharFormat::DashUnderline);
        currentUnderlineStyle = DashUnderline;
    }

    else {
        format.setUnderlineStyle(QTextCharFormat::NoUnderline);
        currentUnderlineStyle = NoUnderline;
    }

    QTextCursor cursor = edit->textCursor();
    if (cursor.hasSelection()) {
        cursor.mergeCharFormat(format);
    } else {
        // Apply to new text
        edit->mergeCurrentCharFormat(format);
    }
}

void Window::setBorderlineStyle(const QString &BorderType) {
    QTextCursor cursor1 = edit->textCursor();

    QTextFrameFormat frameFormat;

    // Create the border brush (white color in this case)
    QBrush borderBrush(Qt::white);


    if (BorderType == "Solid") {
       frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Solid);
             // currentBorderlineStyle = Solid;
    }
    else if (BorderType == "Dotted") {
         frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Dotted);
           // currentBorderlineStyle = Dotted;
    }
    else if (BorderType == "Dashed") {
     frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Dashed);
            //currentBorderlineStyle= Dashed;
    }
    else if (BorderType == "Double") {
       frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Double);
           //currentBorderlineStyle = Double;
    }
    else if (BorderType== "DotDash") {
         frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_DotDash);
            //currentBorderlineStyle= DotDash;
    }
    else if (BorderType== "DotDotDash") {
       frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_DotDotDash);
        //currentBorderlineStyle= DotDotDash;
    }
    else if (BorderType== "Groove") {
        frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Groove);
        //currentBorderlineStyle= Groove;
    }

    else if (BorderType== "Ridge") {
        frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Ridge);
        //currentBorderlineStyle= Ridge;
    }

    else if (BorderType== "Inset") {
        frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Inset);
        //currentBorderlineStyle=Inset;
    }
    else if (BorderType== "Outset") {
        frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_Outset);
       // currentBorderlineStyle=Outset;
    }
    else {
        frameFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
            //currentBorderlineStyle = None;
        return;
    }


    if (cursor1.hasSelection()) {
       frameFormat.setBorderBrush(borderBrush);

       frameFormat.setBorder(1);
        cursor1.insertFrame(frameFormat);
    }
}

// Save the application state
void Window::saveState() {
    // Save current page content first
    if (currentPage && currentPage->parent()) {
        savePageContent();
    }

    QString configPath = getConfigPath();
    QSettings settings(configPath, QSettings::IniFormat);

    // Save editor preferences
    settings.setValue("Editor/Font", currentFont);
    settings.setValue("Editor/Size", currentSize);
    settings.setValue("Editor/Color", currentColor.name());
    settings.setValue("Editor/FontSize", currentfontSize);
    settings.setValue("Editor/AppColor", currentAppColor);
    settings.setValue("Editor/NoteColor", currentNoteColor);

    // Save the complete notebook structure
    saveNotebookStructure();

    settings.sync();
}

void Window::restoreState() {
    QString configPath = getConfigPath();
    QSettings settings(configPath, QSettings::IniFormat);

    // Restore the notebook structure with all content
    restoreNotebookStructure();

    // Restore editor formatting preferences
    if (settings.contains("Editor/Font")) {
        setFont(settings.value("Editor/Font").toString());
    }
    if (settings.contains("Editor/Size")) {
        setSize(settings.value("Editor/Size").toInt());
    }
    if (settings.contains("Editor/Color")) {
        setTextColor(settings.value("Editor/Color").toString());
    }
    if (settings.contains("Editor/FontSize")) {
        int restoredSize = settings.value("Editor/FontSize").toInt();
        setfontSize(restoredSize);
    }
    if (settings.contains("Editor/AppColor")) {
        setAppColor(settings.value("Editor/AppColor").toString());
    }
    if (settings.contains("Editor/NoteColor")) {
        setNoteColor(settings.value("Editor/NoteColor").toString());
    }
    // Automatically select and load the first page
    if (NoteTree->topLevelItemCount() > 1) { // Exclude "+ Add Notebook"
        QTreeWidgetItem* firstNotebook = NoteTree->topLevelItem(0);
        if (firstNotebook && firstNotebook->childCount() > 1) { // Exclude "+ Add Page"
            QTreeWidgetItem* firstPage = firstNotebook->child(0);
            NoteTree->setCurrentItem(firstPage);
            loadPageContent(firstPage);
        }
    }
}

QString Window::getConfigPath() {
    return QCoreApplication::applicationDirPath() + "/notebook_config.ini";
}

//HERE
void Window::addNotebook() {
    QTreeWidgetItem* notebookItem = new QTreeWidgetItem(NoteTree);
    notebookItem->setIcon(0, QIcon(":/notebook"));
    notebookItem->setFlags(notebookItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable);

    // Add "+ Add Page" item
    QTreeWidgetItem* addPageItem = new QTreeWidgetItem(notebookItem);
    addPageItem->setText(0, "+ Add Page");
    addPageItem->setFlags(addPageItem->flags() | Qt::ItemIsSelectable);

    // Initialize notebook data
    QString notebookName = notebookItem->text(0);
    notebookData[notebookName] = QMap<QString, QString>();

    // Add the first page automatically
    addPage(notebookItem);

    notebookItem->setExpanded(true);

}

void Window::addPage(QTreeWidgetItem* notebookItem) {
    if (!notebookItem) return;

    // Save current content before adding new page
    if (currentPage && currentPage->parent()) {
        savePageContent();
    }

    // Create new page item
    QTreeWidgetItem* pageItem = new QTreeWidgetItem();
    pageItem->setText(0, QString("New Page %1").arg(notebookItem->childCount())); // Add unique number to page name
    pageItem->setFlags(pageItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable);

    // Insert before "+ Add Page" item
    int insertPos = notebookItem->childCount() - 1;
    notebookItem->insertChild(insertPos, pageItem);

    // Initialize notebook data if needed
    QString notebookName = notebookItem->text(0);
    if (!notebookData.contains(notebookName)) {
        notebookData[notebookName] = QMap<QString, QString>();
    }

    // Initialize with empty content in memory
    notebookData[notebookName][pageItem->text(0)] = "";

    // Clear the text edit to show an empty page
    edit->clear();

    // Focus the new page
    NoteTree->setCurrentItem(pageItem);
    loadPageContent(pageItem); // Load the (empty) content for the new page
    NoteTree->editItem(pageItem, 0);
}

//function that provides printing dialog
void Window::printing() {
    QPrinter printer;
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        edit->print(&printer);
    }
}

//function to insert a file to our textEdit
void Window::insertingFile() {

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Rich Text Files (*.rtf);;Images (*.png *.xpm *.jpg);;Text files (*.txt);;All files (*.*)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
        if (!fileNames.isEmpty()) {
            QString filePath = fileNames.first();
            QFileInfo fileInfo(filePath);
            QString fileExtension = fileInfo.suffix().toLower();
            if (fileExtension == "txt") {
                QFile file(filePath);
                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&file);
                    QString fileContent = in.readAll();
                    file.close();
                    edit->insertPlainText(fileContent);
                } else {
                    QMessageBox::critical(this, tr("Error"), tr("Could not open text file."));

                }

            } else if (fileExtension == "rtf") {
                QFile file(filePath);
                if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                    QTextStream in(&file);
                    QString rtfContent = in.readAll();
                    file.close();
                    QTextDocument doc;
                    doc.setHtml(rtfContent);
                    QByteArray rtfData = rtfContent.toUtf8();
                    doc.setPlainText(QString::fromUtf8(rtfData));
                    doc.setHtml(QString(rtfData));
                    edit->setAcceptRichText(true);
                    edit->setText(rtfContent);

                } else {

                    QMessageBox::critical(this, tr("Error"), tr("Could not open RTF file."));
                }
            }
            else if (fileExtension == "png" || fileExtension == "xpm" || fileExtension == "jpg") {
                QImageReader reader(filePath);
                if (reader.canRead()) {
                    QUrl imageUrl = QUrl::fromLocalFile(filePath);
                    edit->insertHtml(QString("<img src=\"%1\">").arg(imageUrl.toString()));
                } else {
                    QMessageBox::critical(this, tr("Error"), tr("Could not read image file or unsupported format."));
                }
            } else {
                QMessageBox::warning(this, tr("Warning"), tr("Unsupported file format."));

            }

        }

    }

}
//function from saving text to file from text edit
void Window::savingFile() {
     QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save As"), "",
                                                    tr("Rich Text Files (*.rtf);;Text files (*.txt);;All files (*)"), &selectedFilter);

    if (fileName.isEmpty())
        return; // User cancelled the dialog

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file for writing."));
        return;
    }
    QTextStream out(&file);

    if (selectedFilter == "Rich Text Files (*.rtf)") {
        edit -> setAcceptRichText(true);
         out << edit->toHtml();
    }

    file.close();
}
/*void Window::resizeEvent(QResizeEvent* event) {


}*/
void Window::savePageContent() {
    qDebug() << "--- savePageContent() called ---";
    if (!currentPage || !currentPage->parent() || currentPage->text(0) == "+ Add Page") {
        qDebug() << "  currentPage is invalid, returning.";
        return;
    }

    QString notebookName = currentPage->parent()->text(0);
    QString pageName = currentPage->text(0);
    QString uniqueKey = QString("%1_%2").arg(notebookName, pageName);

    qDebug() << "  Saving content for notebook:" << notebookName << ", page:" << pageName;
    qDebug() << "  Unique Key being used to save:" << uniqueKey;
    qDebug() << "  Content (first 50 chars):" << edit->document()->toHtml().left(50); // Changed to toHtml()
    qDebug() << "  Current currentPage text:" << currentPage->text(0);
    qDebug() << "  Current currentPage parent text:" << currentPage->parent()->text(0);

    // Save the content as HTML to preserve images
    notebookData[notebookName][pageName] = edit->document()->toHtml();

    // Save to settings with unique key
    QSettings settings(getConfigPath(), QSettings::IniFormat);
    settings.beginGroup("PageContents");
    settings.setValue(uniqueKey, edit->document()->toHtml()); // Changed to toHtml()
    settings.endGroup();
    settings.sync();

    contentModified = false;
    qDebug() << "--- savePageContent() finished ---";
}

void Window::loadPageContent(QTreeWidgetItem* page) {
    qDebug() << "--- loadPageContent() called ---";
    if (!page || !page->parent() || page->text(0) == "+ Add Page") {
        qDebug() << "  Loading invalid page, clearing edit and returning.";
        edit->clear();
        currentPage = nullptr;
        return;
    }

    QString notebookName = page->parent()->text(0);
    QString pageName = page->text(0);
    qDebug() << "  Loading content for notebook:" << notebookName << ", page:" << pageName;
    QString uniqueKey = QString("%1_%2").arg(notebookName, pageName);
    qDebug() << "  Unique Key to load:" << uniqueKey;

    QSettings settings(getConfigPath(), QSettings::IniFormat);
    settings.beginGroup("PageContents");
    QString content = settings.value(uniqueKey).toString();
    settings.endGroup();

    // Debugging: Show a snippet of the loaded content to verify it contains image data
    qDebug() << "  Loaded content (first 1000 chars):" << content.left(1000);

    if (!content.isEmpty()) {
        edit->document()->setHtml(content); // This is the crucial part that will now load Base64 images
        if (edit->document()->isEmpty()) {
            qDebug() << "Error: QTextDocument is empty after setting HTML.";
        } else if (edit->document()->rootFrame()->childFrames().isEmpty() && content.contains("<img")) {
            qDebug() << "Warning: No image frames found after loading HTML with <img> tag.";
        }
        notebookData[notebookName][pageName] = content;
        qDebug() << "  Content loaded from settings.";
    } else {
        edit->clear(); // Clear if no content found
        qDebug() << "  No content found for this page, clearing editor.";
    }

    currentPage = page;
    contentModified = false;
    qDebug() << "  currentPage is now:" << (currentPage ? currentPage->text(0) : "nullptr");
    qDebug() << "--- loadPageContent() finished ---";
}


void Window::saveNotebookStructure() {
    QSettings settings(getConfigPath(), QSettings::IniFormat);

    // Don't clear all settings, just the notebook structure
    settings.remove("Notebooks");

    // Save current page before saving structure
    if (currentPage && currentPage->parent()) {
        savePageContent();
    }

    settings.beginWriteArray("Notebooks");
    int notebookIndex = 0;

    for (int i = 0; i < NoteTree->topLevelItemCount(); i++) {
        QTreeWidgetItem* notebookItem = NoteTree->topLevelItem(i);
        if (!notebookItem || notebookItem->text(0) == "+ Add Notebook") continue;

        QString notebookName = notebookItem->text(0);
        settings.setArrayIndex(notebookIndex++);
        settings.setValue("Name", notebookName);

        // Save the icon path or identifier
        settings.setValue("Icon", notebookItem->icon(0).name()); // Save the path/name of the icon

        settings.beginWriteArray("Pages");
        int pageIndex = 0;

        for (int j = 0; j < notebookItem->childCount(); j++) {
            QTreeWidgetItem* pageItem = notebookItem->child(j);
            if (!pageItem || pageItem->text(0) == "+ Add Page") continue;

            QString pageName = pageItem->text(0);
            settings.setArrayIndex(pageIndex++);
            settings.setValue("Name", pageName);
        }
        settings.endArray();
    }
    settings.endArray();
    settings.sync();
}
void Window::restoreNotebookStructure() {
    QSettings settings(getConfigPath(), QSettings::IniFormat);

    notebookData.clear();
    NoteTree->clear();

    // First restore the structure
    int notebookCount = settings.beginReadArray("Notebooks");
    for (int i = 0; i < notebookCount; i++) {
        settings.setArrayIndex(i);
        QString notebookName = settings.value("Name").toString();
        QString iconName = settings.value("Icon").toString(); // Load the icon name

        QTreeWidgetItem* notebookItem = new QTreeWidgetItem(NoteTree);
        notebookItem->setText(0, notebookName);
        if (!iconName.isEmpty()) {
            notebookItem->setIcon(0, QIcon(iconName)); // Set the icon when restoring
        } else {
            notebookItem->setIcon(0, QIcon(":/notebook")); // Set a default icon if none was saved
        }
        notebookItem->setFlags(notebookItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable);

        notebookData[notebookName] = QMap<QString, QString>();

        int pageCount = settings.beginReadArray("Pages");
        for (int j = 0; j < pageCount; j++) {
            settings.setArrayIndex(j);
            QString pageName = settings.value("Name").toString();

            QTreeWidgetItem* pageItem = new QTreeWidgetItem(notebookItem);
            pageItem->setText(0, pageName);
            pageItem->setFlags(pageItem->flags() | Qt::ItemIsEditable | Qt::ItemIsSelectable);

            // Load the page content immediately after creating the page item
            settings.beginGroup("PageContents");
            QString uniqueKey = QString("%1_%2").arg(notebookName, pageName);
            QString content = settings.value(uniqueKey).toString();
            settings.endGroup();

            notebookData[notebookName][pageName] = content;
        }
        settings.endArray();

        // Add "+ Add Page" item
        QTreeWidgetItem* addPageItem = new QTreeWidgetItem(notebookItem);
        addPageItem->setText(0, "+ Add Page");
        addPageItem->setFlags(addPageItem->flags() | Qt::ItemIsSelectable);
    }
    settings.endArray();

    // Add "+ Add Notebook" item
    QTreeWidgetItem* addNotebookItem = new QTreeWidgetItem(NoteTree);
    addNotebookItem->setText(0, "+ Add Notebook");
    addNotebookItem->setFlags(addNotebookItem->flags() | Qt::ItemIsSelectable);
}
// Setup notebook tree connections
void Window::saveContentForPage(QTreeWidgetItem* pageItem) {
    qDebug() << "--- saveContentForPage() called ---";
    if (!pageItem || !pageItem->parent() || pageItem->text(0) == "+ Add Page") {
        qDebug() << "  Invalid page item provided for saving.";
        return;
    }

    QString notebookName = pageItem->parent()->text(0);
    QString pageName = pageItem->text(0);
    QString uniqueKey = QString("%1_%2").arg(notebookName, pageName);

    qDebug() << "  Saving content for notebook:" << notebookName << ", page:" << pageName;
    qDebug() << "  Unique Key:" << uniqueKey;
    qDebug() << "  Content (first 50 chars):" << edit->toPlainText().left(50);

    notebookData[notebookName][pageName] = edit->toHtml();
    QSettings settings(getConfigPath(), QSettings::IniFormat);
    settings.beginGroup("PageContents");
    settings.setValue(uniqueKey, edit->toHtml());
    settings.endGroup();
    settings.sync();

    qDebug() << "--- saveContentForPage() finished ---";
}

void Window::setupNotebookConnections() {
    disconnect(NoteTree, &QTreeWidget::itemClicked, 0, 0);

    connect(NoteTree, &QTreeWidget::itemClicked, this, [this](QTreeWidgetItem* clickedItem) {
        if (!clickedItem) return;

        if (clickedItem->text(0) == "+ Add Notebook" && !clickedItem->parent()) {
            addNotebook();
        } else if (clickedItem->text(0) == "+ Add Page" && clickedItem->parent()) {
            addPage(clickedItem->parent());
        } else if (clickedItem->parent() && clickedItem != currentPage) {
            qDebug() << "--- Page switch detected ---";
            qDebug() << "  Current page before switch:" << (currentPage ? currentPage->text(0) : "nullptr");
            qDebug() << "  Clicked item:" << clickedItem->text(0);

            QTreeWidgetItem* previousPage = currentPage;

            // 1. Save the content of the *previous* currentPage
            if (previousPage && previousPage->parent() && previousPage->text(0) != "+ Add Page") {
                qDebug() << "  Saving content of previous page.";
                savePageContent(); // This function uses the current currentPage at the time of call
            } else {
                qDebug() << "  No valid current page to save.";
            }

            // 2. Load the content of the *new* clickedItem
            qDebug() << "  Loading content for clicked item.";
            loadPageContent(clickedItem);
            qDebug() << "--- Page switch handling finished ---";
        }
        // If clicking the same page, do nothing
    });

    // Handle renaming
    connect(NoteTree, &QTreeWidget::itemChanged, this, [this](QTreeWidgetItem* item, int column) {
        // ... renaming logic ...
    });
}
// Handle window close
void Window::closeEvent(QCloseEvent* event) {
    saveState();

    QWidget::closeEvent(event);
}
