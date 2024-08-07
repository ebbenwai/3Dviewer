#pragma once

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QSlider>

#include "mainwindow.h"

extern "C" {
#include "../../modules/move/move.h"
#include "../../modules/parser/parser.h"
#include "../../modules/rotate/rotate.h"
#include "../../modules/scale/scale.h"
}

struct Sliders;

class OpenGLViewer : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT

 private:
  GLuint model{0};
  GLfloat x_rotate{0};
  GLfloat y_rotate{0};
  QPoint position;
  Vertex* vertices{nullptr};
  Face* faces{nullptr};
  GLint vertex_count{0};
  GLint face_count{0};
  QString w_file_path;
  GLfloat tx{0}, ty{0}, tz{0};
  QSlider *x_slider{nullptr}, *y_slider{nullptr}, *z_slider{nullptr};
  Sliders sliders;
  GLfloat camera_x{0.0f}, camera_y{0.0f}, camera_z{-10.0f};
  GLfloat current_tx{0.0f}, current_ty{0.0f}, current_tz{0.0f};
  GLfloat total_angle_x{0.0f}, total_angle_y{0.0f}, total_angle_z{0.0f};
  GLfloat current_scale{1.0f};
  GLfloat ortho_scale{10.0f};
  bool perspective_view{true};
  QColor background_color{Qt::black};
  QColor edge_color{Qt::green};
  QColor vertex_color{Qt::green};
  GLfloat edge_thickness{1.0f};
  bool edge_dashed{false};
  GLfloat vertex_size{0.0f};
  QString vertex_shape{"None"};

  void reset_sliders();
  void update_background_color();
  void draw_circle(GLfloat size);
  void draw_square(GLfloat size);
  void setup_projection();
  void setup_view();

 protected:
  virtual void initializeGL() override;
  virtual void resizeGL(int width, int height) override;
  virtual void paintGL() override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;
  virtual void keyPressEvent(QKeyEvent* event) override;
  virtual void focusInEvent(QFocusEvent* event) override;

 public:
  OpenGLViewer(QWidget* parent = nullptr);
  ~OpenGLViewer();

  void create_default_cube();
  void reload_model();
  void create_model();
  void clear();
  void draw_grid();
  int get_vertex_count() const;
  int get_face_count() const;
  void reset_rotation();
  void find_model_center(Vertex* vertices, GLint vertex_count, GLfloat& cx,
                         GLfloat& cy, GLfloat& cz);

  void set_file_path(const QString& file_path);
  void set_translation(GLfloat value, GLchar axis);
  void set_rotation(GLfloat angle, GLchar axis);
  void set_sliders(const Sliders& sliders);
  void set_scaling(GLint slider_value);
  void set_projection(bool perspective);
  void set_edge_color(const QColor& color);
  void set_vertex_color(const QColor& color);
  void set_edge_thickness(GLfloat thickness);
  void set_edge_dashed(bool dashed);
  void set_vertex_size(GLfloat size);
  void set_vertex_shape(const QString& shape);

  QColor get_edge_color() const;
  QColor get_vertex_color() const;
  QColor get_background_color() const;
  bool get_projection() const;

 signals:
  void model_updated(int vertex_count, int face_count);

 public slots:
  void change_background_color(const QColor& color);
};
