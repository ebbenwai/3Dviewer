#include "mainwindow.h"

#include "gif.h"
#include "openglviewer.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->VertexShapeComboBox->addItems({"Нет", "Круг", "Квадрат"});
  try {
    Sliders sliders = {ui->PosX, ui->PosY, ui->PosZ, ui->RotX,
                       ui->RotY, ui->RotZ, ui->Scale};
    ui->openGLWidget->set_sliders(sliders);
  } catch (const std::exception &e) {
    show_error("Error initializing sliders: " +
               QString::fromStdString(e.what()));
  }
  connect_signals_to_slots();
  load_settings();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::connect_signals_to_slots() {
  OpenGLViewer *viewer = findChild<OpenGLViewer *>("openGLWidget");
  if (!viewer) {
    show_error("Error: OpenGLViewer not found.");
    return;
  }

  connect(viewer, &OpenGLViewer::model_updated, this,
          &MainWindow::update_vertex_and_faces_info);
  connect(ui->GetScreenshot, &QPushButton::clicked, this,
          &MainWindow::on_GetScreenshot_clicked);
  connect(ui->GetGif, &QPushButton::clicked, this,
          &MainWindow::on_GetGif_clicked);
  connect(ui->PerspectiveView, &QPushButton::clicked, this,
          &MainWindow::on_PerspectiveView_clicked);
  connect(ui->OrthographicView, &QPushButton::clicked, this,
          &MainWindow::on_OrthographicView_clicked);
  connect(ui->EdgeColorButton, &QPushButton::clicked, this,
          &MainWindow::change_edge_color);
  connect(ui->VertexColorButton, &QPushButton::clicked, this,
          &MainWindow::change_vertex_color);
  connect(ui->EdgeThicknessSlider, &QSlider::valueChanged, this,
          &MainWindow::change_edge_thickness);
  connect(ui->EdgeDashedCheckbox, &QCheckBox::stateChanged, this,
          &MainWindow::change_edge_dashed);
  connect(ui->VertexSizeSlider, &QSlider::valueChanged, this,
          &MainWindow::change_vertex_size);
  connect(ui->VertexShapeComboBox,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::change_vertex_shape);
}

void MainWindow::on_GetGif_clicked() {
  QString file_name = QFileDialog::getSaveFileName(
      this, tr("Save GIF"), QCoreApplication::applicationDirPath() + "/images",
      tr("GIF (*.gif)"));

  if (file_name.isEmpty()) {
    show_error("Failed to save the GIF.");
    return;
  }

  create_gif(file_name);
}

void MainWindow::create_gif(const QString &fileName) {
  OpenGLViewer *viewer = findChild<OpenGLViewer *>("openGLWidget");
  if (!viewer) {
    show_error("Error: OpenGLViewer not found.");
    return;
  }

  QImage img(viewer->size(), QImage::Format_RGB32);
  QPainter painter(&img);
  QTime die_time;
  GifWriter gif;
  GifBegin(&gif, fileName.toLocal8Bit().data(), 640, 480, 10);

  for (int i = 0; i < 50; ++i) {
    ui->GetGif->setText(QString::number(i / 10.0, 'f', 1) + "s");
    viewer->render(&painter);
    QImage img640_480 = img.scaled(QSize(640, 480));
    GifWriteFrame(&gif, img640_480.bits(), 640, 480, 10);
    die_time = QTime::currentTime().addMSecs(100);
    while (QTime::currentTime() < die_time)
      QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
  }

  ui->GetGif->setText("GIF");
  GifEnd(&gif);
  show_info("GIF saved successfully.");
}

void MainWindow::on_GetScreenshot_clicked() {
  QPixmap screenshot = ui->openGLWidget->grab();
  QString file_path = QFileDialog::getSaveFileName(this, "Save Screenshot", "",
                                                   "Images (*.png *.jpg)");
  if (!file_path.isEmpty()) {
    screenshot.save(file_path);
  }
}

void MainWindow::on_SelectFile_clicked() {
  QString file_name = QFileDialog::getOpenFileName(this, tr("Select OBJ File"),
                                                   QDir::currentPath(),
                                                   tr("OBJ Files (*.obj)"));

  if (file_name.isEmpty()) {
    show_error("No file selected.");
    return;
  }

  QFileInfo file_info(file_name);
  if (!file_info.exists() || !file_info.isFile()) {
    show_error("Error: Selected file does not exist or is not a valid file.");
    return;
  }

  w_file_path = file_name;
  QString base_name = file_info.fileName();
  ui->FileName->setText(base_name);

  OpenGLViewer *viewer = findChild<OpenGLViewer *>("openGLWidget");
  if (!viewer) {
    show_error("Error: OpenGLViewer not found.");
    return;
  }

  try {
    viewer->set_file_path(file_name);
    viewer->update();
    show_info("Model loaded successfully: " + base_name);
    update_vertex_and_faces_info(viewer->get_vertex_count(),
                                 viewer->get_face_count());
  } catch (const std::exception &e) {
    show_error("Error setting file path: " + QString::fromStdString(e.what()));
  }
}

void MainWindow::on_PosX_valueChanged(int value) {
  update_viewer_translation(static_cast<float>(value), 'x');
}

void MainWindow::on_PosY_valueChanged(int value) {
  update_viewer_translation(static_cast<float>(value), 'y');
}

void MainWindow::on_PosZ_valueChanged(int value) {
  update_viewer_translation(static_cast<float>(value), 'z');
}

void MainWindow::update_viewer_translation(float value, char axis) {
  OpenGLViewer *viewer = findChild<OpenGLViewer *>("openGLWidget");
  if (viewer) {
    viewer->set_translation(value, axis);
  } else {
    show_error("Error: OpenGLViewer not found.");
  }
}

void MainWindow::show_error(const QString &message) {
  QMessageBox::critical(this, "Error", message);
}

void MainWindow::show_info(const QString &message) {
  QMessageBox::information(this, "Info", message);
}

void MainWindow::update_vertex_and_faces_info(int vertex_count,
                                              int face_count) {
  QString info =
      QString("Vertices: %1\nEdges: %2").arg(vertex_count).arg(face_count);
  ui->VertexAndFacesInfo->setPlainText(info);
}

void MainWindow::on_RotX_valueChanged(int value) {
  update_viewer_rotation(static_cast<float>(value), 'x');
}

void MainWindow::on_RotY_valueChanged(int value) {
  update_viewer_rotation(static_cast<float>(value), 'y');
}

void MainWindow::on_RotZ_valueChanged(int value) {
  update_viewer_rotation(static_cast<float>(value), 'z');
}

void MainWindow::update_viewer_rotation(float value, char axis) {
  OpenGLViewer *viewer = findChild<OpenGLViewer *>("openGLWidget");
  if (viewer) {
    viewer->set_rotation(value, axis);
  } else {
    show_error("Error: OpenGLViewer not found.");
  }
}

void MainWindow::on_Scale_valueChanged(int value) {
  OpenGLViewer *viewer = findChild<OpenGLViewer *>("openGLWidget");
  if (viewer) {
    viewer->set_scaling(value);
  } else {
    show_error("Error: OpenGLViewer not found.");
  }
}

void MainWindow::on_PerspectiveView_clicked() {
  update_viewer_projection(true);
}

void MainWindow::on_OrthographicView_clicked() {
  update_viewer_projection(false);
}

void MainWindow::update_viewer_projection(bool orthographic) {
  OpenGLViewer *viewer = findChild<OpenGLViewer *>("openGLWidget");
  if (viewer) {
    viewer->set_projection(orthographic);
  } else {
    qDebug() << "Error: OpenGLViewer not found.";
  }
}

void MainWindow::on_BackgroundButton_clicked() {
  OpenGLViewer *viewer = findChild<OpenGLViewer *>("openGLWidget");
  if (viewer) {
    QColor color =
        QColorDialog::getColor(Qt::white, this, "Choose background color");
    if (color.isValid()) {
      viewer->change_background_color(color);
    }
  } else {
    qDebug() << "Error: OpenGLViewer not found.";
  }
}

void MainWindow::change_edge_color() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Choose Edge Color");
  if (color.isValid()) {
    ui->openGLWidget->set_edge_color(color);
  }
}

void MainWindow::change_vertex_color() {
  QColor color = QColorDialog::getColor(Qt::white, this, "Choose Vertex Color");
  if (color.isValid()) {
    ui->openGLWidget->set_vertex_color(color);
  }
}

void MainWindow::change_edge_thickness(int thickness) {
  ui->openGLWidget->set_edge_thickness(static_cast<GLfloat>(thickness));
}

void MainWindow::change_edge_dashed(int state) {
  ui->openGLWidget->set_edge_dashed(state == Qt::Checked);
}

void MainWindow::change_vertex_size(int size) {
  ui->openGLWidget->set_vertex_size(static_cast<GLfloat>(size));
}

void MainWindow::change_vertex_shape(int index) {
  QString shape = ui->VertexShapeComboBox->itemText(index);
  ui->openGLWidget->set_vertex_shape(shape);
}

void MainWindow::on_SettingsButton_clicked() {
  save_settings();
  show_info("Settings saved.");
}

void MainWindow::save_settings() {
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, "3D Viewer",
                     "Settings");
  OpenGLViewer *viewer = findChild<OpenGLViewer *>("openGLWidget");
  if (!viewer) {
    qDebug() << "Error: OpenGLViewer not found.";
    return;
  }

  settings.setValue("EdgeColor", viewer->get_edge_color().name());
  settings.setValue("VertexColor", viewer->get_vertex_color().name());
  settings.setValue("EdgeThickness", ui->EdgeThicknessSlider->value());
  settings.setValue("EdgeDashed", ui->EdgeDashedCheckbox->isChecked());
  settings.setValue("VertexSize", ui->VertexSizeSlider->value());
  settings.setValue("VertexShape", ui->VertexShapeComboBox->currentIndex());
  settings.setValue("BackgroundColor", viewer->get_background_color().name());
  settings.setValue("OrthographicView", viewer->get_projection());
}

void MainWindow::load_settings() {
  QSettings settings(QSettings::IniFormat, QSettings::UserScope, "3D Viewer",
                     "Settings");
  OpenGLViewer *viewer = findChild<OpenGLViewer *>("openGLWidget");
  if (!viewer) {
    qDebug() << "Error: OpenGLViewer not found.";
    return;
  }

  QColor edge_color(settings.value("EdgeColor", "#00FF00").toString());
  QColor vertex_color(settings.value("VertexColor", "#00FF00").toString());
  QColor background_color(
      settings.value("BackgroundColor", "#000000").toString());

  viewer->set_edge_color(edge_color);
  viewer->set_vertex_color(vertex_color);
  viewer->change_background_color(background_color);

  ui->EdgeThicknessSlider->setValue(settings.value("EdgeThickness", 1).toInt());
  ui->EdgeDashedCheckbox->setChecked(
      settings.value("EdgeDashed", false).toBool());
  ui->VertexSizeSlider->setValue(settings.value("VertexSize", 0).toInt());
  ui->VertexShapeComboBox->setCurrentIndex(
      settings.value("VertexShape", 0).toInt());

  viewer->set_edge_thickness(ui->EdgeThicknessSlider->value());
  viewer->set_edge_dashed(ui->EdgeDashedCheckbox->isChecked());
  viewer->set_vertex_size(ui->VertexSizeSlider->value());
  viewer->set_vertex_shape(ui->VertexShapeComboBox->currentText());

  bool orthographic_view = settings.value("PerspectiveView", true).toBool();
  viewer->set_projection(orthographic_view);

  viewer->update();
}
