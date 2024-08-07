#pragma once

#include <QColorDialog>
#include <QDebug>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>
#include <QSlider>
#include <QTime>
#include <QTimer>

struct Sliders {
  QSlider *pos_x;
  QSlider *pos_y;
  QSlider *pos_z;
  QSlider *rot_x;
  QSlider *rot_y;
  QSlider *rot_z;
  QSlider *scale;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() override;

  const char *get_file_path() const;
  void load_settings();
  void save_settings();

 private slots:
  // File operations
  void on_SelectFile_clicked();
  void on_GetScreenshot_clicked();
  void on_GetGif_clicked();

  // Transformations
  void on_PosX_valueChanged(int value);
  void on_PosY_valueChanged(int value);
  void on_PosZ_valueChanged(int value);
  void on_RotX_valueChanged(int value);
  void on_RotY_valueChanged(int value);
  void on_RotZ_valueChanged(int value);
  void on_Scale_valueChanged(int value);

  // View modes
  void on_PerspectiveView_clicked();
  void on_OrthographicView_clicked();

  // Appearance settings
  void on_BackgroundButton_clicked();
  void change_edge_color();
  void change_vertex_color();
  void change_edge_thickness(int thickness);
  void change_edge_dashed(int state);
  void change_vertex_size(int size);
  void change_vertex_shape(int index);

  // Utility functions
  void update_viewer_translation(float value, char axis);
  void update_viewer_rotation(float value, char axis);
  void update_viewer_projection(bool orthographic);
  void update_vertex_and_faces_info(int vertex_count, int face_count);
  void show_error(const QString &message);
  void show_info(const QString &message);

  // Settings
  void on_SettingsButton_clicked();
  void connect_signals_to_slots();

 private:
  Ui::MainWindow *ui;
  QString w_file_path;
  QTimer *recordingTimer;
  QVector<QImage> recordedFrames;

  void create_gif(const QString &fileName);
};
