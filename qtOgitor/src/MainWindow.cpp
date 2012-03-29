MainWindow(int argc, char *argv[])

QWidget *renderWindowWidget = new QWidget(mEditorTab);
QToolBar *renderWindowToolBar = new QToolBar(renderWindowWidget);

mCameraViewModeBox = new QComboBox();
mSnapMultiplierBox = new QComboBox();

QLabel *snapLabel = new QLabel(QString(" ") + tr("Snap :"));
QLabel *viewModeLabel = new QLabel(QString("  ") + tr("View :"));

QSignalMapper *cameraMapper = new QSignalMapper(this);

menuCameraPositionMain = new QMain(this);
mCameraSpeedSlider = new QSlider(Qt::Horizontal);

QWidgetAction * sliderActionWidget = new QWidgetAction( this );

mOgreWidget = new OgreWidget(renderWindowWidget, mHasFileArgs);

QVBoxLayout *renderWindowLayout = new QVBoxLayout();